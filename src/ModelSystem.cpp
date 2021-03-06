#include <overkill/ModelSystem.hpp>


namespace overkill
{

std::vector<Model> ModelSystem::m_models;
std::unordered_map<C::Tag, C::ID> ModelSystem::m_mapModelID;



void ModelSystem::toggleShader(const C::Tag& modelTag, 
                               const C::Tag& meshTag,
                               const C::Tag& shader,
                               const C::Tag& otherShader) 
{
 
    auto& terrainModelMeshes = ModelSystem::getByTag(modelTag).m_meshes;

    for (auto& mesh: terrainModelMeshes) 
    {
        if (mesh.m_tag == meshTag) 
        {
            if (mesh.m_shaderProgram.m_tag == shader) {
                mesh.m_shaderProgram = ShaderSystem::copyByTag(otherShader);
                
                mesh.m_shaderProgram.setMaterial(
                    MaterialSystem::getById(mesh.m_materialID));

                LOG_DEBUG("swapped shader %s -> %s", shader.data(), otherShader.data());
            } else {
                
                mesh.m_shaderProgram = ShaderSystem::copyByTag(shader);
                mesh.m_shaderProgram.setMaterial(
                    MaterialSystem::getById(mesh.m_materialID));

                LOG_DEBUG("swapped shader %s -> %s", otherShader.data(), shader.data());
            }
            break;
        }
    }


}

auto ModelSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapModelID[tag];
}

auto ModelSystem::getByTag(const C::Tag& tag) -> Model&
{
    return ModelSystem::m_models[m_mapModelID[tag]];
}

auto ModelSystem::getById(C::ID modelID) -> Model&
{
    return ModelSystem::m_models[modelID];
}

void ModelSystem::clean() 
{
    // Clean out all buffers from GPU
    for (auto model : ModelSystem::m_models)
    {
        for (auto mesh : model.m_meshes)
        {
            mesh.m_ebo.clean();
        }
        model.m_vbo.clean();
        model.m_vao.clean();
    }

    // Clear the models on CPU
    ModelSystem::m_models.clear();
}

void ModelSystem::reload() 
{   
    ModelSystem::clean();

    // Unbind callbacks
    MaterialSystem::unbindAll();
    ShaderSystem::unbindAll();

    // Finally load new data
    Watcher::pollEvents();
    ModelSystem::load();
}


auto ModelSystem::makeModel(const C::Tag& tag, const std::string& modelString, Model* outModel) -> C::Err
{
    auto p = Parser(modelString);
    //
    // VERTICES
    //
    std::vector<Vertex> vertices;

    // Vertex count
    if (auto[key, vertexCount, err] = p.keyInteger("vertices"); err) {
        return 1;
    } else {
        vertices.resize(vertexCount);
        LOG_DEBUG("vertexcount: %d", vertexCount);

        for(int i = 0; i < vertexCount; ++i) 
        {
            vertices[i] = p.onlyVertex();
        }        
    }



    auto vbufLayout = VertexBufferAttribLayout();

    vbufLayout.push<GL_FLOAT>(3);                       //position -> 0  -> 12 -> 12
    vbufLayout.pushPacked<GL_INT_2_10_10_10_REV>(4);    //normal   -> 12 -> 16 -> 16
    vbufLayout.push<GL_UNSIGNED_SHORT>(2, GL_TRUE);                         //uv       -> 16 -> 20 -> 24 ?
    vbufLayout.push<GL_UNSIGNED_BYTE>(4, GL_TRUE);      //color    -> 18 -> 22 -> 20


    // Buffer vertex data to GPU VAO and VBO
    Model newModel;
    newModel.m_tag = tag;
    newModel.m_vbo = VertexBuffer(vertices.data(), vertices.size() * vbufLayout.getStride()); //@NOTE: if there are exploding mesh issues this probably has to do with alignment issues, your GPU preffers data packed 4 by 4 bytes
    newModel.m_vao.addBuffer(newModel.m_vbo, vbufLayout);
    //
    // MESHES
    //
    // Mesh count
    
    int meshCount;
    if (auto[key, meshCount_, err] = p.keyInteger("meshes"); err) {
        return 1;
    } else {
        meshCount = meshCount_;
    }

    // Iterate meshes
    for(int i = 0; i < meshCount; ++i) 
    {
        std::string meshTag,
                    materialTag,
                    shaderTag;

        // mesh tag
        if (auto[key, meshTag_, err] = p.keyString("mesh"); err) {
            return 1;
        } else {
            meshTag = std::string(meshTag_);
        }

        // material tag
        if (auto[key, materialTag_, err] = p.keyString("material"); err) {
            return 1;
        } else {
            materialTag = std::string(materialTag_);
        }

        // shader tag
        if (auto[key, shaderTag_, err] = p.keyString("shader"); err) {
            return 1;
        } else {
            shaderTag = std::string(shaderTag_);            
        }
        

        //
        // TRIANGLES 
        //
        std::vector<Triangle> triangles;

        // Triangle count
        if (auto[key, triCount, err] = p.keyInteger("triangles"); err){
            return err;
        } else {

            triangles.resize(triCount);
            LOG_DEBUG("tricount: %d", triCount);
            for(int j = 0; j < triCount; ++j) 
            {
                triangles[j] = p.onlyTriangle();
            } 
        }

        // Construct mesh, buffer ElementBuffer data to GPU
        auto newMesh = newModel.m_meshes.emplace_back(
            Mesh{
                meshTag,
                ElementBuffer((unsigned int*)triangles.data(), triangles.size() * 3),
                MaterialSystem::getIdByTag(materialTag),
                ShaderSystem::copyByTag(shaderTag)
            }
        );
        newMesh.m_shaderProgram.setMaterial(MaterialSystem::getByTag(materialTag));

    }


    *outModel = newModel;
    return 0;
}


void ModelSystem::push(const C::Tag& tag, const std::string& filepath)  
{
    C::Err err;

    std::string modelString;
    err = Util::fileToString(filepath, &modelString);
    if (err) {
        LOG_WARN("Util::fileToString: Failed to load model file %s", filepath.data());
        ModelSystem::m_mapModelID[tag] = 0;
        return;
    }

    Model model;
    err = ModelSystem::makeModel(tag, modelString, &model);
    if(err) {
        LOG_WARN("ModelSystem::makeModel: Failed to parse model string");
        ModelSystem::m_mapModelID[tag] = 0;
        return;
    }

    ModelSystem::m_mapModelID[tag] = ModelSystem::m_models.size();
    ModelSystem::m_models.push_back(model);

    for (std::size_t i = 0; i < model.m_meshes.size(); i++)
    {
        // Bind to MaterialSystem update event
        MaterialSystem::bindOnUpdate(
            MaterialSystem::getById(model.m_meshes[i].m_materialID).m_tag,
            m_mapModelID[model.m_tag],
            i,
            [](C::ID materialID, C::ID modelID, C::ID meshID) {
                auto& model = ModelSystem::getById(modelID);
                auto& mesh = model.m_meshes[meshID];
                mesh.m_materialID = materialID;
                mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(materialID));
            }
        );

        // Bind to ShaderSystem update event
        ShaderSystem::bindOnUpdate(
            model.m_meshes[i].m_shaderProgram.m_tag,
            m_mapModelID[model.m_tag],
            i,
            [](C::ID shaderID, C::ID modelID, C::ID meshID) {
                auto& model = ModelSystem::getById(modelID);
                auto& mesh = model.m_meshes[meshID];
                mesh.m_shaderProgram = ShaderSystem::copyById(shaderID);
                mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(mesh.m_materialID));
            }
        );
    }
}




void ModelSystem::load()
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "models");

    ModelSystem::push("_default", "assets/models/_default.yml");
    LOG_DEBUG("Model from file: assets/models/_default.yml");

    for (auto e: fevents)
    {   
        auto filepath = C::ModelsFolder + ("/" + e.tag) + "." + e.extension;
        LOG_DEBUG("Model from file: %s", filepath.data());

        ModelSystem::push(e.tag, filepath);
    }

}

} // ::overkill

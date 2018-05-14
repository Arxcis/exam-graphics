#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <overkill/Config.hpp>
#include <overkill/Init.hpp>
#include <overkill/Input.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/Renderer.hpp>
#include <overkill/Util.hpp>

#include <overkill/GUI.hpp>

#include <overkill/TextureSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/Scene.hpp>
#include <overkill/ModelSystem.hpp>
#include <overkill/EntityModel.hpp>
#include <overkill/EntityCamera.hpp>
#include <overkill/Watcher.hpp>


#define PI 3.141592


using namespace overkill;


void updateGlider(float dt);


int main(int argc, char** args)
{
    if (argc > 2)
    {
        LOG_ERROR("No more than 0 or 1 parameters. Parameter is path to scene file to load. Example: ./cube assets/scenes/_default.yml");
    }

    Init::Config();

    // Init libraries + Watcher callbacks
	C::window = Init::GLFW(
        C::GLVersionMajor, 
        C::GLVersionMinor, 
        C::WinWidth, 
        C::WinHeight,
        C::WinName);

    Init::GLEW();
    Init::OpenGL(C::ClearColor);
    Init::ImGUI(C::window);

    Watcher::pollEvents();
    Input::Init();

    // Load asset subsystems
    TextureSystem::load();
    ShaderSystem::createUniformBuffers();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();
    

    
    LOG_DEBUG("Argc %d, args[1] %s", argc, args[1]);

    if (argc == 1)
    {
        Scene::load("assets/scenes/base.yml");
    }
    else if (argc == 2)
    {
        Scene::load(std::string(args[1]));
    }


    float oldT = 0, t = 0, dt = 0;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = true;
    bool show_another_window = false;

    for(;;)
    {

        //
        // RENDER GUI
        //
        GUI::begin();



        t = (float)glfwGetTime();        

        dt = t - oldT;


        if ((glfwGetKey(C::window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(C::window) != 0))
            break;

        Renderer::clear();
        Scene::update(dt);

        // Draws all the models in the scene.
        Scene::draw(t);     


        //
        // CONTROL TIME
        //

        // 1. Elapsed time
        Scene::times.elapsed_time = t;


        // 2. Day time in seconds
        if (C::DayRunning) 
        {
            const auto TimeMultiplier = 5000; // 10k seconds for every second
            Scene::times.timeofday_seconds += dt * TimeMultiplier;
        }

        // 3. Year time in days
        if (C::YearRunning) 
        {
            const auto TimeMultiplier = 10; // 30 days for very second
            Scene::times.timeofyear_days += dt *  TimeMultiplier;

        }


        //This can be done to debug positions
        //glm::mat4 camPosDebugM2W = glm::translate(glm::mat4(1), glm::vec3(cameraTransform.position));
        //Renderer::draw(ModelSystem::getById(0), camPosDebugM2W, t);

 
        GUI::updateTimeOfDay(Scene::times.timeofday_seconds);
        GUI::updateTimeOfYear(Scene::times.timeofyear_days);
        GUI::updateTimeOfTide(Scene::times.timeoftide_percent);
        
        updateGlider(dt);

        GUI::end();

        //
        // RENDER OpenGL
        //
        glfwSwapBuffers(C::window);
        glfwPollEvents();
        
        oldT = t;

        // break; // For testing load performance
    }

    Scene::clean();
    ModelSystem::clean();
    ShaderSystem::clean();
    TextureSystem::clean();


    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(C::window);
    glfwTerminate();
	return 0;
}




void updateGlider(float dt) 
{

    auto glider = Scene::getEntityByTag("glider-rig");
    if (!glider)
        return;



    if (Input::m_keysPressed[Key::W]) {
       glider->setRotation(glider->getRotation() + glm::vec3(1,0,0));
    }

    if (Input::m_keysPressed[Key::S]) {
        glider->setRotation(glider->getRotation() - glm::vec3(1,0,0));
    }


    if (Input::m_keysPressed[Key::A]) {
        glider->setRotation(glider->getRotation() + glm::vec3(0,1,0));
    }

    if (Input::m_keysPressed[Key::D]) {
        glider->setRotation(glider->getRotation() - glm::vec3(0,1,0));
    }

    //
    // Update current speed
    //
    static float currentSpeed = 1.0;

    if (Input::m_keysPressed[Key::Comma]) {
        currentSpeed -= 1.0 * dt;
        if (currentSpeed < 0)
            currentSpeed = 0;
    }
    if (Input::m_keysPressed[Key::Period]) {
        currentSpeed += 1.0 * dt;
        if (currentSpeed < 0)
            currentSpeed = 0;
    }

    static bool F_keyIsDown = false;
    static bool R_keyIsDown = false;
    static int currentStartPoisitionIndex = 0;

    const std::vector<std::string> possibleStartPositionNames = 
    {
        "Lillehammer",
        "Gjøvik",
        "Hamar",
    };
    const std::vector<glm::vec3> possibleStartPositions = 
    {
        glm::vec3(18, 3, 46),
        glm::vec3(65, 3, 263),
        glm::vec3(173, 3, 255)
    };

    //
    // Cycle between glider start positions
    //
    if (Input::m_keysPressed[Key::F] && !F_keyIsDown) {
        F_keyIsDown = true;

        currentStartPoisitionIndex = ++currentStartPoisitionIndex % possibleStartPositions.size();
        LOG_DEBUG("current start position index: %d", currentStartPoisitionIndex);

        glider->setPosition( possibleStartPositions[currentStartPoisitionIndex] );
    }
    else if (!Input::m_keysPressed[Key::F] && F_keyIsDown) {
        F_keyIsDown = false;
    }


    //
    // Reset to current glider start position
    //
    if (Input::m_keysPressed[Key::R] && !R_keyIsDown) {
        R_keyIsDown = true;

        LOG_DEBUG("current start position index: %d", currentStartPoisitionIndex);

        glider->setPosition( possibleStartPositions[currentStartPoisitionIndex] );
    }
    else if (!Input::m_keysPressed[Key::R] && R_keyIsDown) {
        R_keyIsDown = false;
    }

    


    auto rotation = glm::radians(glider-> getRotation());

    auto gliderpropeller = Scene::getEntityByTag("glider-propeller-rig");
    if (!gliderpropeller) {
        LOG_DEBUG("DID NOT FIND gliderpropeller");
        return;
    }


    auto propellerSineMixin = sin(rotation.x - (25/180*PI));
    gliderpropeller->setAngularVelocity(glm::vec3(-90 + (propellerSineMixin * 4000),  0, 0));





    //
    // @begin hack
    //
    // Compute velocity
    // In coorporation with Jone Martin Skaara
    //
    // Why is this a hack you may ask?
    // Because actually we just want the glider to go in the direction it is facing.
    // It should do this by default if the scene graph is set up properly.
    // We should not have to explicitly set the velocity.
    auto velocity = glm::vec3( glm::sin(rotation.y * glm::cos(rotation.x)),  // forstått
                               glm::sin(-rotation.x),                       // forstått
                               glm::cos(rotation.y) * glm::cos(rotation.x)); // forstår

    velocity *= glm::vec3(currentSpeed);

    glider->setVelocity(velocity);
    //
    // @end hack
    //

    auto mainCamera = Scene::getActiveCamera();
    if (!mainCamera) {
        LOG_DEBUG("main-camera not found in scene");
        return;
    }

    ImGui::Text("\nActive camera:");

    GUI::writeVector("rotation", mainCamera->getRotation());
    GUI::writeVector("position", mainCamera->getPosition());




    ImGui::Text("\nGlider:");
    GUI::writeSpeed(currentSpeed);
    GUI::writeVector("velocity", velocity);
    GUI::writeVector("position", glider-> getPosition());
    ImGui::Text("Started from %s!", possibleStartPositionNames[currentStartPoisitionIndex].data());
}
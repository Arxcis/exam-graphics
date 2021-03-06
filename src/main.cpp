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

        Scene::times.timeofday_seconds = GUI::sliderDaytime("Hours of day", 0, 24, Scene::times.timeofday_seconds);
        Scene::times.timeofyear_days   = GUI::sliderYeartime("Days of year", 1, 365, Scene::times.timeofyear_days);

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
    auto gliderVistaCam = Scene::getEntityByTag("cam-glider-vista");
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
    static float currentSpeed = 0.0;

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
    static int currentStartLocationIndex = 2;


    struct PossibleStartLocation 
    {
        std::string name;
        glm::vec3   gliderPosition;
        glm::vec3   cameraVistaPosition;
        glm::vec3   cameraVistaRotation;
        glm::vec3   gliderRotation = glm::vec3(0);
    };

    const std::vector<PossibleStartLocation> possibleStarts = 
    {
        PossibleStartLocation{"Lillehammer", glm::vec3(19.86, 2.91, 53), glm::vec3( -9.2, 6.2, 12.4), glm::vec3(0.76, 190, 0)},
        PossibleStartLocation{"Helgøya",     glm::vec3(153.18, 2.26, 299.0), glm::vec3(-11, 11.95, 24), glm::vec3(5.76, 231.32, 0)},
        PossibleStartLocation{"1500 century Gjøvik", glm::vec3(81.9, 1.49, 272.6), glm::vec3(-15.0, 14.73, 23.0), glm::vec3(2.77, 242.61, 0), glm::vec3(2.0, 155, 0) },
    };



    //
    // Cycle between glider start positions
    //
    if (Input::m_keysPressed[Key::F] && !F_keyIsDown) {
        F_keyIsDown = true;

        currentStartLocationIndex = ++currentStartLocationIndex % possibleStarts.size();
        LOG_DEBUG("current start position index: %d", currentStartLocationIndex);

        glider->setPosition( possibleStarts[currentStartLocationIndex].gliderPosition );
        glider->setRotation( possibleStarts[currentStartLocationIndex].gliderRotation );
        gliderVistaCam->setPosition( possibleStarts[currentStartLocationIndex].cameraVistaPosition );
        gliderVistaCam->setRotation( possibleStarts[currentStartLocationIndex].cameraVistaRotation );

        Scene::m_activeCamera = (EntityCamera*)gliderVistaCam;

        currentSpeed = 0.0;
    }
    else if (!Input::m_keysPressed[Key::F] && F_keyIsDown) {
        F_keyIsDown = false;
    }


    //
    // Reset to current glider start position
    //
    if (Input::m_keysPressed[Key::R] && !R_keyIsDown) {
        R_keyIsDown = true;

        LOG_DEBUG("current start position index: %d", currentStartLocationIndex);

        glider->setPosition( possibleStarts[currentStartLocationIndex].gliderPosition );
        currentSpeed = 0.0;
    }
    else if (!Input::m_keysPressed[Key::R] && R_keyIsDown) {
        R_keyIsDown = false;
    }

    


    auto rotation = glm::radians(glider-> getRotation());


    // 
    // Rotate the glider propeller
    //
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

    auto activeCamera = Scene::getActiveCamera();
    if (!activeCamera) {
        LOG_WARN("Scene::getActiveCamera() not found in scene");
        return;
    }

    ImGui::Text("\nActive camera: %s", activeCamera->getTag().data());

    GUI::writeVector("rotation", activeCamera->getRotation());
    GUI::writeVector("position", activeCamera->getPosition());




    ImGui::Text("\nGlider:");
    GUI::writeSpeed(currentSpeed);
    GUI::writeVector("velocity", velocity);
    GUI::writeVector("position", glider-> getPosition());
    GUI::writeVector("rotation", glider-> getRotation());
    ImGui::Text("Teleported to:\n%s", possibleStarts[currentStartLocationIndex].name.data());
}
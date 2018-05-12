#include <overkill/Input.hpp>


namespace overkill
{
    bool Input::m_leftButtonDown = false;
    bool Input::m_rightButtonDown = false;
    
    float Input::m_fovy = C::FOV;
    float Input::m_cursorX = 0;
    float Input::m_cursorY = 0;
    float Input::m_camRotX = 0;
    float Input::m_camRotY = 0;
    float Input::m_camPanX = 0;
    float Input::m_camPanY = 0;
    bool Input::m_navKeyPressed[] = {false, false, false, false, false, false}; // Which navigation keys are pressed. WASD-QE keys.

    void Input::OnInputKeyPress(GLFWwindow* window, int keyCode, int /*scanCode*/, int mods)
    {
        const int ShiftMod = 1;

        // LOG_DEBUG("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        if (keyCode == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, 1);
        }

        // PRESS 1 TO RELOAD ShaderSystem
        else if(keyCode == GLFW_KEY_1 && mods == ShiftMod) {
            ShaderSystem::reload();
        }
        // PRESS 2 TO RELOAD MaterialSystem
        else if(keyCode == GLFW_KEY_2 && mods == ShiftMod) {
            MaterialSystem::reload();
        }
        // PRESS 3 TO RELOAD ModelSystem
        else if(keyCode == GLFW_KEY_3 && mods == ShiftMod) {
            ModelSystem::reload();
        }
        // PRESS 4 TO RELOAD Scene
        else if(keyCode == GLFW_KEY_4 && mods == ShiftMod) {
            Scene::reload();
        }
        else if (keyCode == GLFW_KEY_C) {

            LOG_INFO("TOGGLING");

            ModelSystem::toggleShader("terrain",
                                      "terrain",
                                      "terrain",
                                      "contour");

        }


        //
        // @note
        // Here comes all the key combinations which are required to
        // pass the exam.
        // -----------------------
        //  key 1: Pick sprint
        //  key 2: Pick Summer
        //  key 3: Pick autumn
        //  Key 4: Pick Winter
        //  Key 5: Pause season change
        //
        //  Key 6: Pick morning
        //  Key 7: Pick noon
        //  Key 8: Pick afternoon
        //  Key 9: Pick night
        //  Key 0: Pause daycycle
        //

        // Key ,: up tide   percent
        // Key .: down tide percent 


        else if(keyCode == GLFW_KEY_1) {

        }
        else if(keyCode == GLFW_KEY_2) {

        }
        else if(keyCode == GLFW_KEY_3) {

        }
        else if(keyCode == GLFW_KEY_4) {

        }
        else if (keyCode == GLFW_KEY_5) {

        }
        else if (keyCode == GLFW_KEY_6) {
            const float MorningInSeconds = 6*60*60;
            Scene::times.timeofday_seconds = MorningInSeconds;
        }   
        else if (keyCode == GLFW_KEY_7) {
            const float NoonInSeconds = 12*60*60;
            Scene::times.timeofday_seconds = NoonInSeconds;

        }
        else if (keyCode == GLFW_KEY_8) {
            const float AfternoonInSeconds = 18*60*60;
            Scene::times.timeofday_seconds = AfternoonInSeconds;
        }
        else if (keyCode == GLFW_KEY_9) {
            const float MidnightInSeconds = 0;
            Scene::times.timeofday_seconds = MidnightInSeconds;
        }
        else if (keyCode == GLFW_KEY_0) {
            C::DayRunning = (C::DayRunning) ? false : true;
        }

        else if (keyCode == GLFW_KEY_PERIOD) {

            Scene::times.timeoftide_percent += 3;
        }

        else if (keyCode == GLFW_KEY_COMMA) {
            Scene::times.timeoftide_percent -= 3;
        }


        // PRESS SPACE to toggle camera mode
        else if(keyCode == GLFW_KEY_SPACE) {
            EntityCamera* cam = (EntityCamera*) Scene::getActiveCamera();
            if (cam != nullptr)
            {
                cam-> cycleMode();
                LOG_INFO("Cycled camera mode.");
            }
            else 
            {
                LOG_WARN("Main camera not set. The scene most likely loaded incorrectly.")
            }

        }
        // PRESS TAB to cycle through camera
        else if(keyCode == GLFW_KEY_TAB) {
            Scene::cycleCameras();
            LOG_INFO("Cycled cameras.");
        }


        switch (keyCode)
        {
            case GLFW_KEY_W:        
                m_navKeyPressed[W] = true;
                break;
            case GLFW_KEY_S:        
                m_navKeyPressed[S] = true;
                break;
            case GLFW_KEY_D:        
                m_navKeyPressed[D] = true;    
                break;
            case GLFW_KEY_A:        
                m_navKeyPressed[A] = true;
                break;
            case GLFW_KEY_Q:        
                m_navKeyPressed[Q] = true;
                break;
            case GLFW_KEY_E:        
                m_navKeyPressed[E] = true;
                break;      
        }
    }

    void Input::OnInputKeyHold(GLFWwindow* /*window*/, int /*keyCode*/, int /*scanCode*/, int /*mods*/)
    {
        // LOG_DEBUG("Holding %i, as char: %c\n", keyCode, char(keyCode));
    }

    void Input::OnInputKeyUnpress(GLFWwindow* /*window*/, int keyCode, int /*scanCode*/, int /*mods*/)
    {
        // LOG_DEBUG("Unpressed %i, as char: %c\n", keyCode, char(keyCode));

        switch (keyCode)
        {
            case GLFW_KEY_W:
                m_navKeyPressed[W] = false;
                break;
            case GLFW_KEY_S:
                m_navKeyPressed[S] = false;
                break;
            case GLFW_KEY_D:
                m_navKeyPressed[D] = false;    
                break;
            case GLFW_KEY_A:
                m_navKeyPressed[A] = false;
                break;
            case GLFW_KEY_Q:
                m_navKeyPressed[Q] = false;
                break;
            case GLFW_KEY_E:
                m_navKeyPressed[E] = false;
                break;
        }
    }

    void Input::OnInputKey(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
    {
        switch (action)
        {
        case GLFW_PRESS:
            OnInputKeyPress(window, keyCode, scanCode, mods);
            break;
        case GLFW_REPEAT:
            OnInputKeyHold(window, keyCode, scanCode, mods);
            break;
        case GLFW_RELEASE:
            OnInputKeyUnpress(window, keyCode, scanCode, mods);
            break;
        }
    }

    void Input::OnCursorHover(GLFWwindow* /*window*/, double x, double y)
    {
        float deltaX = x - m_cursorX;
        float deltaY= y - m_cursorY;

        ////Camera rotation:
        
#ifdef __APPLE__
        if (m_leftButtonDown) //Click and drag to rotate.
        {
#endif
            EntityCamera* camera = (EntityCamera*) Scene::getActiveCamera();
            if (camera != nullptr)
            {
                glm::vec3 rot = camera-> getRotation();
                camera-> setRotation(rot + glm::vec3((-deltaY), -deltaX, 0) * C::LookSensitivity);
            }
            else 
            {
                LOG_WARN("Main camera not set. The scene most likely loaded incorrectly.")
            }
#ifdef __APPLE__
        }
#endif


        m_cursorX = x;  // Save current cursor pos.
        m_cursorY = y;  // important to do this last since the logic to rotate and pan*/
    }                   // uses the difference between now sand last mouse pos.

    void Input::OnScrollChange(GLFWwindow* /*window*/, double /*x*/, double /*y*/)
    {
    //    printf("Scroll: x: %f,\ty:%f\t\tfovy:%f\n", x, y, m_fovy);
    }

    void Input::OnMouseClick(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (action == GLFW_PRESS)
            {
                // printf("Left button pressed.\n");
                m_leftButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                // printf("Left button released.\n");       
                m_leftButtonDown = false;
            }
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                // printf("Right button pressed.\n");
                m_rightButtonDown = true;
            }
            else if (action == GLFW_RELEASE)
            {
                // printf("Right button released.\n");       
                m_rightButtonDown = false;
            }
        }
    }
}
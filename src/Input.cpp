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
    bool  Input::m_navKeyPressed[14] = {
         false, 
         false, 
         false, 
         false, 
         false, 
         false,
         false, 
         false, 
         false, 
         false, 
         false, 
         false,
         false,
         false,
     };



    //
    // @note
    // Here comes all the key combinations which are required to
    // pass the exam + some more
    // -----------------------
    //  key ESC: quit app
    //
    // Time and wheater:
    //  1: Pick spring
    //  2: Pick Summer
    //  3: Pick autumn
    //  4: Pick Winter
    //  5: Pause season change
    //
    //  6: Pick morning
    //  7: Pick noon
    //  8: Pick afternoon
    //  9: Pick night
    //  0: Pause daycycle
    //
    //  UP:   up tide   percent
    //  DOWN: down tide percent
    //
    // Camera:
    //   i: + z position 
    //   k: - z position
    //   l: + x position
    //   j: - x position
    //   y: + y position
    //   h: - y positon
    //  space: toggl camera mode [FREELOOK | ORBITAL]
    //
    // Visualizations:
    //  c: countour lines
    //
    // Systems:
    //  shift + 1 TO RELOAD ShaderSystem
    //  shift + 2 TO RELOAD MaterialSystem
    //  shift + 3 TO RELOAD ModelSystem
    //  shift + 4 TO RELOAD Scene + Config file

    void Input::OnInputKeyPress(GLFWwindow* window, int keyCode, int /*scanCode*/, int mods)
    {
        const int ShiftMod = 1;
        const float DaysInAMonth = 4.33*7;

        // LOG_DEBUG("Pressing %i, as char: %c\n", keyCode, char(keyCode));
        if (keyCode == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, 1);
        }



        else if(keyCode == GLFW_KEY_1 && mods == ShiftMod) {
            ShaderSystem::reload();
        }
        else if(keyCode == GLFW_KEY_2 && mods == ShiftMod) {
            MaterialSystem::reload();
        }
        else if(keyCode == GLFW_KEY_3 && mods == ShiftMod) {
            ModelSystem::reload();
        }
        else if(keyCode == GLFW_KEY_4 && mods == ShiftMod) {
            Scene::reload();
        }

        else if(keyCode == GLFW_KEY_1) {
            const float MidSpringInDays = DaysInAMonth*4;
            Scene::times.timeofyear_days = MidSpringInDays;
        }
        else if(keyCode == GLFW_KEY_2) {
            const float MidSummerInDays = DaysInAMonth*7;
            Scene::times.timeofyear_days = MidSummerInDays;

        }
        else if(keyCode == GLFW_KEY_3) {
            const float MidAutumnInDays = DaysInAMonth*10;
            Scene::times.timeofyear_days = MidAutumnInDays;

        }
        else if(keyCode == GLFW_KEY_4) {
            const float MidWinterInDays = DaysInAMonth*1;
            Scene::times.timeofyear_days = MidWinterInDays;
        }
        else if (keyCode == GLFW_KEY_5) {
            C::YearRunning = (C::YearRunning)? false : true;
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

        else if (keyCode == GLFW_KEY_UP) {

            Scene::times.timeoftide_percent += 3;
        }

        else if (keyCode == GLFW_KEY_DOWN) {
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


        else if (keyCode == GLFW_KEY_C) {

            LOG_INFO("TOGGLING");

            ModelSystem::toggleShader("terrain",
                                      "terrain",
                                      "terrain",
                                      "contour");

        }


        switch (keyCode)
        {
            case GLFW_KEY_I:
                m_navKeyPressed[I] = true;
                break;
            case GLFW_KEY_K:        
                m_navKeyPressed[K] = true;
                break;
            case GLFW_KEY_L:    
                m_navKeyPressed[L] = true;    
                break;
            case GLFW_KEY_J:        
                m_navKeyPressed[J] = true;
                break;
            case GLFW_KEY_Y:        
                m_navKeyPressed[Y] = true;
                break;
            case GLFW_KEY_H:        
                m_navKeyPressed[H] = true;
                break;      
     
            case GLFW_KEY_N:
                m_navKeyPressed[N] = true;
                break;
            case GLFW_KEY_M:        
                m_navKeyPressed[M] = true;
                break;      
     

            case GLFW_KEY_W:
                m_navKeyPressed[W] = true;
                break;
            case GLFW_KEY_A:        
                m_navKeyPressed[A] = true;
                break;      
            case GLFW_KEY_S:
                m_navKeyPressed[S] = true;
                break;
            case GLFW_KEY_D:        
                m_navKeyPressed[D] = true;
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
            case GLFW_KEY_I:
                m_navKeyPressed[I] = false;
                break;
            case GLFW_KEY_K:        
                m_navKeyPressed[K] = false;
                break;
            case GLFW_KEY_L:    
                m_navKeyPressed[L] = false;    
                break;
            case GLFW_KEY_J:        
                m_navKeyPressed[J] = false;
                break;
            case GLFW_KEY_Y:        
                m_navKeyPressed[Y] = false;
                break;
            case GLFW_KEY_H:        
                m_navKeyPressed[H] = false;
                break;      
     
            case GLFW_KEY_N:
                m_navKeyPressed[N] = false;
                break;
            case GLFW_KEY_M:        
                m_navKeyPressed[M] = false;
                break;      
     

            case GLFW_KEY_W:
                m_navKeyPressed[W] = false;
                break;
            case GLFW_KEY_A:        
                m_navKeyPressed[A] = false;
                break;      
            case GLFW_KEY_S:
                m_navKeyPressed[S] = false;
                break;
            case GLFW_KEY_D:        
                m_navKeyPressed[D] = false;
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
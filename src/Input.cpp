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

std::vector<int>  Input::m_activeKeys;
std::vector<bool> Input::m_keysPressed;


 void Input::Init() 
 {

    Input::m_activeKeys.resize(18);
    Input::m_activeKeys[Key::I] = GLFW_KEY_I;
    Input::m_activeKeys[Key::J] = GLFW_KEY_J;
    Input::m_activeKeys[Key::K] = GLFW_KEY_K;
    Input::m_activeKeys[Key::L] = GLFW_KEY_L;

    Input::m_activeKeys[Key::Y] = GLFW_KEY_Y;
    Input::m_activeKeys[Key::H] = GLFW_KEY_H;

    Input::m_activeKeys[Key::W] = GLFW_KEY_W;
    Input::m_activeKeys[Key::S] = GLFW_KEY_S;
    Input::m_activeKeys[Key::A] = GLFW_KEY_A;
    Input::m_activeKeys[Key::D] = GLFW_KEY_D;

    Input::m_activeKeys[Key::Q] = GLFW_KEY_Q;
    Input::m_activeKeys[Key::E] = GLFW_KEY_E;

    Input::m_activeKeys[Key::N] = GLFW_KEY_N;
    Input::m_activeKeys[Key::M] = GLFW_KEY_M;
    Input::m_activeKeys[Key::Comma] = GLFW_KEY_COMMA;
    Input::m_activeKeys[Key::Period] = GLFW_KEY_PERIOD;
    Input::m_activeKeys[Key::F] = GLFW_KEY_F;
    Input::m_activeKeys[Key::R] = GLFW_KEY_R;



    // Init all keys to false (unpressed)
    Input::m_keysPressed.resize(Input::m_activeKeys.size(), false);
 }




//
//////////////////////////////////// CONTROLS ///////////////////////////////
//
// 
// --------------------
//    General:
// --------------------
//  ESC: quit app
//
//
// -------------------
//   Time and weather:
//  -------------------
//   1: Pick spring
//   2: Pick Summer
//   3: Pick autumn
//   4: Pick Winter
//   5: Pause season change
//
//   6: Pick morning
//   7: Pick noon
//   8: Pick afternoon
//   9: Pick night
//   0: Pause daycycle
//
//   UP:   up tide   percent
//   DOWN: down tide percent
//
//
// ------------------------
//   Camera:
// ------------------------
//   i: forward
//   k: backward
//   l: right
//   j: left
//   y: up
//   h: down
//   space: toggl camera mode [FREELOOK | ORBITAL]
//   tab: cycle through cameras
//
// 
// -----------------------
//    Glider:
// -----------------------  
//    W: pitch down
//    S: pitch up
//    A: yaw left
//    D: yaw right 
//   
//    comma:  speed donw
//    period: speed up
//     - : lock camera behind glider
//
//
// -----------------------
//    Visualizations:
// -----------------------
//   g: toggle grey scale
//   c: toggle countour lines
//
// ---------------------------
//    Reload of systems:
// ----------------------------
//   shift + 1 TO RELOAD ShaderSystem
//   shift + 2 TO RELOAD MaterialSystem
//   shift + 3 TO RELOAD ModelSystem
//   shift + 4 TO RELOAD Scene + Config file
//
//
//////////////////////////////////////////////////////////////////////////////////////



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

            LOG_INFO("TOGGLING CONTOUR LINES");

            ModelSystem::toggleShader("terrain",
                                      "terrain",
                                      "terrain",
                                      "contour");

        }

        else if (keyCode == GLFW_KEY_G) {

            LOG_INFO("TOGGLING GREYSCALE MAP");

            ModelSystem::toggleShader("terrain",
                                      "terrain",
                                      "terrain",
                                      "heightmap");

        }
        //
        // Press down active keys
        //
        int count = 0;
        for (const auto activeKey : m_activeKeys) {

            if (keyCode == activeKey) 
            {
                m_keysPressed[count] = true;
            }
            count++;
        }
    }

    void Input::OnInputKeyHold(GLFWwindow* /*window*/, int /*keyCode*/, int /*scanCode*/, int /*mods*/)
    {
        // LOG_DEBUG("Holding %i, as char: %c\n", keyCode, char(keyCode));
    }

    void Input::OnInputKeyUnpress(GLFWwindow* /*window*/, int keyCode, int /*scanCode*/, int /*mods*/)
    {
        // LOG_DEBUG("Unpressed %i, as char: %c\n", keyCode, char(keyCode));


        //
        // Unpress active keys
        //
        int count = 0;
        for (const auto activeKey: m_activeKeys) {

            if (keyCode == activeKey) 
            {
                m_keysPressed[count] = false;
            }
            count++;
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
        
        if (m_leftButtonDown) //Click and drag to rotate.
        {
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
        }


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
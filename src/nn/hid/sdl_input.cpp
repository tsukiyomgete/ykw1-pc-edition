#include "nn/hid/sdl_input.h"
#include <SDL.h>
#include <mutex>

namespace nn::hid::detail {

static std::mutex g_Lock;
static bool g_Initialized = false;
static SDL_GameController* g_Pad = nullptr;
static int64_t g_SamplingNumber = 0;

void InitializeSdl()
{
    std::lock_guard<std::mutex> guard(g_Lock);
    if (g_Initialized)
        return;
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");   
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    g_Initialized = true;
}


static void OpenPadIfNeeded()
{
    if (g_Pad != nullptr && SDL_GameControllerGetAttached(g_Pad))
        return;
    if (g_Pad != nullptr) {
        SDL_GameControllerClose(g_Pad);
        g_Pad = nullptr;
    }
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            g_Pad = SDL_GameControllerOpen(i);
            if (g_Pad != nullptr)
                return;
        }
    }
}

void ReadInput(NpadCommonState* outState)
{
    std::lock_guard<std::mutex> guard(g_Lock);
    *outState = NpadCommonState{};
    outState->samplingNumber = ++g_SamplingNumber;
    outState->attributes.Set(NpadAttribute::IsConnected);
    if (!g_Initialized)
    {
        return;
    }
    SDL_PumpEvents();
    SDL_GameControllerUpdate();
    OpenPadIfNeeded();

    if (g_Pad != nullptr)
    {
        //Les boutons
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_A))
        {
            outState->buttons.Set(NpadButton::A); //A
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_B))
        {
            outState->buttons.Set(NpadButton::B); //B
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_X))
        {
            outState->buttons.Set(NpadButton::X); //X
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_Y))
        {
            outState->buttons.Set(NpadButton::Y); //Y
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
        {
            outState->buttons.Set(NpadButton::L); //L
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
        {
            outState->buttons.Set(NpadButton::R); //R
        }
        if (SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 16000)
        {
            outState->buttons.Set(NpadButton::ZL); //ZL
        }
        if (SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 16000)
        {
            outState->buttons.Set(NpadButton::ZR); //ZR
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_START))
        {
            outState->buttons.Set(NpadButton::Plus); //+
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_BACK))
        {
            outState->buttons.Set(NpadButton::Minus); //-
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_LEFTSTICK))
        {
            outState->buttons.Set(NpadButton::StickL); //clic stick gauche
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
        {
            outState->buttons.Set(NpadButton::StickR); //clic stick droit
        }

        //La croix
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
        {
            outState->buttons.Set(NpadButton::Left);
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_DPAD_UP))
        {
            outState->buttons.Set(NpadButton::Up);
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
        {
            outState->buttons.Set(NpadButton::Right);
        }
        if (SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
        {
            outState->buttons.Set(NpadButton::Down);
        }

        //Les sticks
        //SDL donne de -32768 a 32767 avec le bas en positif
        //La Switch veut de -32767 a 32767 avec le haut en positif
        int deadZone = 4000;

        //- Stick gauche, horizontal
        int leftX = SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_LEFTX);
        if (leftX < -AnalogStickMax)
        {
            leftX = -AnalogStickMax;
        }
        if (leftX > -deadZone && leftX < deadZone)
        {
            leftX = 0;
        }
        outState->analogStickL.x = leftX;

        //- Stick gauche, vertical (on inverse)
        int leftY = SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_LEFTY);
        if (leftY < -AnalogStickMax)
        {
            leftY = -AnalogStickMax;
        }
        if (leftY > -deadZone && leftY < deadZone)
        {
            leftY = 0;
        }
        outState->analogStickL.y = -leftY;

        //- Stick droit, horizontal
        int rightX = SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_RIGHTX);
        if (rightX < -AnalogStickMax)
        {
            rightX = -AnalogStickMax;
        }
        if (rightX > -deadZone && rightX < deadZone)
        {
            rightX = 0;
        }
        outState->analogStickR.x = rightX;

        //- Stick droit, vertical (on inverse)
        int rightY = SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_RIGHTY);
        if (rightY < -AnalogStickMax)
        {
            rightY = -AnalogStickMax;
        }
        if (rightY > -deadZone && rightY < deadZone)
        {
            rightY = 0;
        }
        outState->analogStickR.y = -rightY;
    }

    //Le clavier (marche seulement quand la fenetre du jeu existera)
    //Les scancodes sont des positions de touches : W A S D = Z Q S D sur un clavier AZERTY
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_RETURN])
    {
        outState->buttons.Set(NpadButton::A); //Entree
    }
    if (keys[SDL_SCANCODE_BACKSPACE])
    {
        outState->buttons.Set(NpadButton::B); //Retour arriere
    }
    if (keys[SDL_SCANCODE_F])
    {
        outState->buttons.Set(NpadButton::X);
    }
    if (keys[SDL_SCANCODE_R])
    {
        outState->buttons.Set(NpadButton::Y);
    }
    if (keys[SDL_SCANCODE_Q])
    {
        outState->buttons.Set(NpadButton::L); //A en AZERTY
    }
    if (keys[SDL_SCANCODE_E])
    {
        outState->buttons.Set(NpadButton::R);
    }
    if (keys[SDL_SCANCODE_1])
    {
        outState->buttons.Set(NpadButton::ZL);
    }
    if (keys[SDL_SCANCODE_3])
    {
        outState->buttons.Set(NpadButton::ZR);
    }
    if (keys[SDL_SCANCODE_ESCAPE])
    {
        outState->buttons.Set(NpadButton::Plus);
    }
    if (keys[SDL_SCANCODE_TAB])
    {
        outState->buttons.Set(NpadButton::Minus);
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        outState->buttons.Set(NpadButton::Left);
    }
    if (keys[SDL_SCANCODE_UP])
    {
        outState->buttons.Set(NpadButton::Up);
    }
    if (keys[SDL_SCANCODE_RIGHT])
    {
        outState->buttons.Set(NpadButton::Right);
    }
    if (keys[SDL_SCANCODE_DOWN])
    {
        outState->buttons.Set(NpadButton::Down);
    }
    //- Stick gauche au clavier : Z Q S D
    if (keys[SDL_SCANCODE_A])
    {
        outState->analogStickL.x = -AnalogStickMax;
    }
    if (keys[SDL_SCANCODE_D])
    {
        outState->analogStickL.x = AnalogStickMax;
    }
    if (keys[SDL_SCANCODE_W])
    {
        outState->analogStickL.y = AnalogStickMax;
    }
    if (keys[SDL_SCANCODE_S])
    {
        outState->analogStickL.y = -AnalogStickMax;
    }
    //- Stick droit au clavier : I J K L
    if (keys[SDL_SCANCODE_J])
    {
        outState->analogStickR.x = -AnalogStickMax;
    }
    if (keys[SDL_SCANCODE_L])
    {
        outState->analogStickR.x = AnalogStickMax;
    }
    if (keys[SDL_SCANCODE_I])
    {
        outState->analogStickR.y = AnalogStickMax;
    }
    if (keys[SDL_SCANCODE_K])
    {
        outState->analogStickR.y = -AnalogStickMax;
    }

    //Les directions des sticks comme des boutons (les menus s'en servent)
    //On compte une direction quand le stick depasse la moitie
    int half = AnalogStickMax / 2;
    if (outState->analogStickL.x < -half)
    {
        outState->buttons.Set(NpadButton::StickLLeft);
    }
    if (outState->analogStickL.x > half)
    {
        outState->buttons.Set(NpadButton::StickLRight);
    }
    if (outState->analogStickL.y > half)
    {
        outState->buttons.Set(NpadButton::StickLUp);
    }
    if (outState->analogStickL.y < -half)
    {
        outState->buttons.Set(NpadButton::StickLDown);
    }
    if (outState->analogStickR.x < -half)
    {
        outState->buttons.Set(NpadButton::StickRLeft);
    }
    if (outState->analogStickR.x > half)
    {
        outState->buttons.Set(NpadButton::StickRRight);
    }
    if (outState->analogStickR.y > half)
    {
        outState->buttons.Set(NpadButton::StickRUp);
    }
    if (outState->analogStickR.y < -half)
    {
        outState->buttons.Set(NpadButton::StickRDown);
    }
}

void Rumble(float low, float high)
{
    std::lock_guard<std::mutex> guard(g_Lock);
    if (g_Pad == nullptr)
    {
        return;
    }

    //Le jeu donne une force entre 0.0 et 1.0, SDL veut un nombre entre 0 et 65535
    if (low < 0.0f)
    {
        low = 0.0f;
    }
    if (low > 1.0f)
    {
        low = 1.0f;
    }
    if (high < 0.0f)
    {
        high = 0.0f;
    }
    if (high > 1.0f)
    {
        high = 1.0f;
    }
    Uint16 lowMotor = (Uint16)(low * 65535);
    Uint16 highMotor = (Uint16)(high * 65535);

    //Le jeu renvoie une valeur a chaque image, donc 100 ms suffisent
    SDL_GameControllerRumble(g_Pad, lowMotor, highMotor, 100);
}

}

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
        if(SDL_GameControllerGetButton(g_Pad,SDL_CONTROLLER_BUTTON_START))
        {
            outState->buttons.Set(NpadButton::Plus); //+
        }
        if(SDL_GameControllerGetButton(g_Pad,SDL_CONTROLLER_BUTTON_BACK))
        {
            outState->buttons.Set(NpadButton::Minus); //-
        }
        //Les sticks
        //- Les stick Gauches
        outState->analogStickL.x = SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_LEFTX);
    }

    // DONE 1 - buttons, BY NAME (the button marked A is the Switch A)
    //   if (g_Pad != nullptr): for each button, SDL_GameControllerGetButton(g_Pad, SDL_CONTROLLER_BUTTON_A)
    //   returns 1 when pressed -> outState->buttons.Set(NpadButton::A)
    //   SDL names: A B X Y LEFTSTICK RIGHTSTICK LEFTSHOULDER (= L) RIGHTSHOULDER (= R)
    //              START (= Plus) BACK (= Minus) DPAD_LEFT DPAD_UP DPAD_RIGHT DPAD_DOWN
    //   ZL / ZR are analog triggers on PC: SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT)
    //   goes from 0 to 32767 -> pressed above half.

    // TODO 2 - sticks
    //   SDL_GameControllerGetAxis(g_Pad, SDL_CONTROLLER_AXIS_LEFTX / LEFTY / RIGHTX / RIGHTY)
    //   SDL gives -32768..32767 with DOWN = positive.
    //   The Switch wants -32767..32767 (AnalogStickMax) with UP = positive:
    //   clamp -32768 to -32767, and invert y. Add a small dead zone (about 4000) so a resting stick reads 0.
    //   -> outState->analogStickL.x / .y and outState->analogStickR.x / .y

    // TODO 3 - stick directions as buttons (menus use them)
    //   NpadButton::StickLLeft / StickLUp / StickLRight / StickLDown (same for StickR):
    //   set when the stick is past half (AnalogStickMax / 2) in that direction.

    // TODO 4 (later, needs the window from nn::vi) - keyboard
    //   const Uint8* keys = SDL_GetKeyboardState(nullptr); keys[SDL_SCANCODE_RETURN] is 1 when pressed.
    //   Scancodes are key POSITIONS: SDL_SCANCODE_W is the Z key on an AZERTY keyboard.
}

void Rumble(float low, float high)
{
    // TODO 5 (optional) - vibration
    //   low and high go from 0.0 to 1.0; SDL wants 0..0xFFFF for each motor:
    //   SDL_GameControllerRumble(g_Pad, lowMotor, highMotor, durationMs)
    //   The game sends a value every frame, so a duration of about 100 ms is enough.
    (void)low; (void)high;
}

}

#include "nn/vi.h"
#include "nn/os/thread.h"
#include <SDL.h>
#include <atomic>
#include <iostream>

namespace nn::vi {

    nn::os::SystemEventType* vsyncEvent = nullptr; 
    nn::os::ThreadType vsyncThread;
    std::atomic<bool> isRunning = false; 

    
    void VsyncLoop(void* argument)
    {
        (void)argument;
        while (isRunning)
        {
            nn::os::SleepThread(nn::TimeSpan::FromMilliSeconds(16));
            nn::os::SignalSystemEvent(vsyncEvent);
        }
    }

    void Initialize()
    {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
    }
    void Finalize()
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    Result OpenDefaultDisplay(Display** outDisplay)
    {
        Display* newDisplay = new Display();
        *outDisplay = newDisplay;
        return Result{};
    }
    void CloseDisplay(Display* display)
    {
        if (isRunning)
        {
            isRunning = false;
            nn::os::WaitThread(&vsyncThread);
            nn::os::DestroyThread(&vsyncThread);
            nn::os::DestroySystemEvent(vsyncEvent);
            vsyncEvent = nullptr;
        }
        delete display;
    }

    Result CreateLayer(Layer** outLayer, Display* display, int width, int height)
    {
        (void)display;
        SDL_Window* window = SDL_CreateWindow("YKW1-PC TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL);

        if (window == nullptr)
        {
            std::cout << SDL_GetError() << std::endl;
            return Result{1};
        }
        Layer* layer = new Layer();
        layer->window = window;
        *outLayer = layer;
        return Result{};
    }

    void DestroyLayer(Layer* layer)
    {
        SDL_DestroyWindow(layer->window);
        delete layer;
    }
    Result SetLayerScalingMode(Layer* layer, ScalingMode mode)
    {
        
        (void)layer;
        (void)mode;
        return Result{};
    }
    Result GetNativeWindow(void** outWindow, Layer* layer)
    {
        //Le jeu donnera ce pointeur a EGL pour savoir ou dessiner
        *outWindow = layer->window;
        return Result{};
    }

    Result GetDisplayVsyncEvent(nn::os::SystemEventType* outEvent, Display* display)
    {
        (void)display;
        if (!isRunning)
        {
            //On prepare l'event du jeu, puis on lance le thread qui le signale
            nn::os::CreateSystemEvent(outEvent, nn::os::EventClearMode_AutoClear, false);
            vsyncEvent = outEvent;
            isRunning = true;
            nn::os::CreateThread(&vsyncThread, VsyncLoop, nullptr, nullptr, 0, 0);
            nn::os::SetThreadName(&vsyncThread, "VsyncThread");
            nn::os::StartThread(&vsyncThread);
        }
        return Result{};
    }
}

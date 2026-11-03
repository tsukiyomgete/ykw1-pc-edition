#pragma once
#include "nn/types.h"
#include "nn/os/event.h"

struct SDL_Window; //on annonce juste que le type existe (SDL.h est inclus dans le .cpp)

namespace nn::vi
{
    //L'ecran
    struct Display {
        int number = 0;
    };

    //La surface de dessin : sur PC, une fenetre SDL
    struct Layer {
        SDL_Window* window = nullptr;
    };

    enum ScalingMode {
        ScalingMode_None,
        ScalingMode_Exact,
        ScalingMode_FitToLayer,
        ScalingMode_ScaleAndCrop,
        ScalingMode_PreserveAspectRatio,
    };

    void   Initialize();
    void   Finalize();

    Result OpenDefaultDisplay(Display** outDisplay);
    void   CloseDisplay(Display* display);

    Result CreateLayer(Layer** outLayer, Display* display, int width, int height);
    void   DestroyLayer(Layer* layer);
    Result SetLayerScalingMode(Layer* layer, ScalingMode mode);
    Result GetNativeWindow(void** outWindow, Layer* layer);

    Result GetDisplayVsyncEvent(nn::os::SystemEventType* outEvent, Display* display);
}

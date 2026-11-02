#include "nn/hid/npad.h"
#include "nn/hid/sdl_input.h"

namespace nn::hid {

void InitializeNpad()
{
    detail::InitializeSdl();
}

void SetSupportedNpadStyleSet(NpadStyleSet style)
{
    (void)style;   // we accept every style
}

void SetSupportedNpadIdType(const NpadIdType* ids, size_t count)
{
    (void)ids; (void)count;
}

NpadStyleSet GetNpadStyleSet(const NpadIdType& id)
{
    // One player on PC: player 1 is a "Pro Controller", the handheld slot is the console itself
    NpadStyleSet style;
    if (id == NpadId::No1)
        style.Set(NpadStyle::FullKey);
    else if (id == NpadId::Handheld)
        style.Set(NpadStyle::Handheld);
    return style;
}

static void Read(NpadCommonState* outState, const NpadIdType& id)
{
    if (id == NpadId::No1 || id == NpadId::Handheld)
        detail::ReadInput(outState);
    else
        *outState = NpadCommonState{};   // other players: not connected
}

void GetNpadState(NpadFullKeyState* outState, const NpadIdType& id)  { Read(outState, id); }
void GetNpadState(NpadHandheldState* outState, const NpadIdType& id) { Read(outState, id); }
void GetNpadState(NpadJoyDualState* outState, const NpadIdType& id)  { Read(outState, id); }

}

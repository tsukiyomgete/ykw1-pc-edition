#include "nn/hid/npad.h"
#include "nn/hid/sdl_input.h"

namespace nn::hid {

void InitializeNpad()
{
    detail::InitializeSdl();
}

void SetSupportedNpadStyleSet(NpadStyleSet style)
{
    (void)style; //on accepte tous les styles
}

void SetSupportedNpadIdType(const NpadIdType* ids, size_t count)
{
    (void)ids;
    (void)count;
}

NpadStyleSet GetNpadStyleSet(const NpadIdType& id)
{
    //Un seul joueur sur PC : le joueur 1 est une Pro Controller, Handheld est la console
    NpadStyleSet style;
    if (id == NpadId::No1)
    {
        style.Set(NpadStyle::FullKey);
    }
    if (id == NpadId::Handheld)
    {
        style.Set(NpadStyle::Handheld);
    }
    return style;
}

//Les 3 versions de GetNpadState font la meme chose
void GetNpadState(NpadFullKeyState* outState, const NpadIdType& id)
{
    if (id == NpadId::No1 || id == NpadId::Handheld)
    {
        detail::ReadInput(outState);
    }
    else
    {
        *outState = NpadFullKeyState{}; //les autres joueurs ne sont pas branches
    }
}

void GetNpadState(NpadHandheldState* outState, const NpadIdType& id)
{
    if (id == NpadId::No1 || id == NpadId::Handheld)
    {
        detail::ReadInput(outState);
    }
    else
    {
        *outState = NpadHandheldState{};
    }
}

void GetNpadState(NpadJoyDualState* outState, const NpadIdType& id)
{
    if (id == NpadId::No1 || id == NpadId::Handheld)
    {
        detail::ReadInput(outState);
    }
    else
    {
        *outState = NpadJoyDualState{};
    }
}

}

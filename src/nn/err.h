#pragma once
#include <cstdint>
#include "nn/types.h"

namespace nn::err
{
    //Code d'erreur affichable, par exemple 2002-0001
    struct ErrorCode {
        uint32_t category = 0;
        uint32_t number = 0;
    };

    enum ErrorResultVariantState {
        ErrorResultVariantState_HasNone = 0,
        ErrorResultVariantState_HasResult,
        ErrorResultVariantState_HasErrorCode,
    };

    //Une petite boite qui contient soit un Result, soit un ErrorCode (le jeu s'en sert partout)
    class ErrorResultVariant {
    public:
        ErrorResultVariant();
        ErrorResultVariant(const Result& result);
        ErrorResultVariant(const ErrorCode& code);

        ErrorResultVariantState GetState() const;
        operator Result() const;
        operator ErrorCode() const;
        ErrorResultVariant& operator=(const Result& result);
        ErrorResultVariant& operator=(const ErrorCode& code);

    private:
        ErrorResultVariantState state;
        Result result;
        ErrorCode code;
    };

    //Sur Switch : fenetre d'erreur. Sur PC : un message dans la console.
    void ShowError(Result result);
}

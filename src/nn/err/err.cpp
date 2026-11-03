#include "nn/err.h"
#include <cstdio>

namespace nn::err {

ErrorResultVariant::ErrorResultVariant()
{
    state = ErrorResultVariantState_HasNone;
}

ErrorResultVariant::ErrorResultVariant(const Result& newResult)
{
    state = ErrorResultVariantState_HasResult;
    result = newResult;
}

ErrorResultVariant::ErrorResultVariant(const ErrorCode& newCode)
{
    state = ErrorResultVariantState_HasErrorCode;
    code = newCode;
}

ErrorResultVariantState ErrorResultVariant::GetState() const
{
    return state;
}

ErrorResultVariant::operator Result() const
{
    return result;
}

ErrorResultVariant::operator ErrorCode() const
{
    if (state == ErrorResultVariantState_HasResult)
    {
        //Un Result devient un code "2xxx-yyyy" : module + 2000, puis la description
        ErrorCode converted;
        converted.category = 2000 + (result.value & 0x1FF);
        converted.number = (result.value >> 9) & 0x1FFF;
        return converted;
    }
    return code;
}

ErrorResultVariant& ErrorResultVariant::operator=(const Result& newResult)
{
    state = ErrorResultVariantState_HasResult;
    result = newResult;
    return *this;
}

ErrorResultVariant& ErrorResultVariant::operator=(const ErrorCode& newCode)
{
    state = ErrorResultVariantState_HasErrorCode;
    code = newCode;
    return *this;
}

void ShowError(Result result)
{
    ErrorCode converted = ErrorResultVariant(result);
    std::printf("[nn::err] Erreur %04u-%04u (Result 0x%x)\n", converted.category, converted.number, result.value);
}

}

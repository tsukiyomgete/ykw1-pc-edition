#pragma once
#include "nn/types.h"

namespace nn::fs
{
    // Nintendo codes: module 2 (fs) | (description << 9)
    inline constexpr Result ResultPathNotFound      = { 0x202 };
    inline constexpr Result ResultPathAlreadyExists = { 0x402 };
}

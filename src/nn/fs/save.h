#pragma once
#include "nn/types.h"
#include "nn/account/uid.h"

namespace nn::fs
{
    Result EnsureSaveData(const nn::account::Uid& user);
    Result CommitSaveData(const char* name);
}

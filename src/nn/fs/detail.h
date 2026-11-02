#pragma once
#include <string>

namespace nn::fs::detail
{
    // "rom:/data/x.bin" -> real path on the PC (uses the mount table)
    std::string ResolvePath(const char* path);
}

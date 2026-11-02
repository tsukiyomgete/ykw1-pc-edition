#pragma once
#include <cstdio>
#include <mutex>
#include <string>

namespace nn::fs::detail
{
    // "rom:/data/x.bin" -> real path on the PC (uses the mount table)
    std::string ResolvePath(const char* path);

    // Folder used for the save data (exists even before MountSaveData)
    const std::string& SaveRoot();

    // What a FileHandle points to
    struct FileObject {
        FILE* file = nullptr;
        std::string path;
        std::mutex lock;   
    };

    // Flushes every file still open (used by CommitSaveData)
    void FlushAllFiles();
}

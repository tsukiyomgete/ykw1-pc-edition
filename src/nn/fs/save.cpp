#include "nn/fs/save.h"
#include "nn/fs/detail.h"
#include <filesystem>

namespace nn::fs {

Result EnsureSaveData(const nn::account::Uid& user)
{
    // One player on PC: just make sure the save folder exists
    (void)user;
    std::error_code error;
    std::filesystem::create_directories(detail::SaveRoot(), error);
    return Result{};
}

Result CommitSaveData(const char* name)
{
    // On Switch, written files only become permanent after a commit.
    // On PC they are already on disk once flushed.
    (void)name;
    detail::FlushAllFiles();
    return Result{};
}

}

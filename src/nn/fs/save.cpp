#include "nn/fs/save.h"
#include "nn/fs/detail.h"
#include <filesystem>

namespace nn::fs {

Result EnsureSaveData(const nn::account::Uid& user)
{
    //Un seul joueur sur PC : on cree juste le dossier de sauvegarde s'il n'existe pas
    (void)user;
    std::error_code error;
    std::filesystem::create_directories(detail::GetSaveRoot(), error);
    return Result{};
}

Result CommitSaveData(const char* name)
{
    //Sur Switch, il faut valider pour que la sauvegarde soit gardee.
    //Sur PC, les fichiers sont deja sur le disque, on vide juste les tampons.
    (void)name;
    detail::FlushAllFiles();
    return Result{};
}

}

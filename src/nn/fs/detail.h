#pragma once
#include <cstdio>
#include <mutex>
#include <string>

//Outils internes a nn::fs (le jeu ne s'en sert pas)
namespace nn::fs::detail
{
    //"rom:/data/x.bin" -> vrai chemin sur le PC
    std::string ResolvePath(const char* path);

    //Dossier des sauvegardes
    std::string GetSaveRoot();

    //Ce que contient un FileHandle
    struct FileObject {
        FILE* file = nullptr;
        std::string path;
        std::mutex lock;
    };

    //Vide le tampon de tous les fichiers ouverts (pour CommitSaveData)
    void FlushAllFiles();
}

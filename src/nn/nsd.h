#pragma once
#include "nn/types.h"

//Adresses des serveurs de Nintendo : pas utilisees sur PC
namespace nn::nsd
{
    struct Fqdn {
        char value[256] = {};
    };

    Result ResolveEx(Fqdn* outFqdn, const Fqdn& fqdn);
}

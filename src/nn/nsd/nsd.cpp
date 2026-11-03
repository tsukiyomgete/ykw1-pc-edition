#include "nn/nsd.h"
#include "nn/offline.h"

namespace nn::nsd {

Result ResolveEx(Fqdn* outFqdn, const Fqdn& fqdn)
{
    *outFqdn = fqdn; //on renvoie l'adresse telle quelle
    return ResultOffline;
}

}

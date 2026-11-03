#include "nn/web.h"
#include "nn/offline.h"
#include <cstdio>

namespace nn::web {

ShowOfflineHtmlPageArg::ShowOfflineHtmlPageArg(const char* path)
{
    pagePath = path;
}

void ShowOfflineHtmlPageArg::SetBootAsMediaPlayer(bool enabled)
{
    (void)enabled;
}

void ShowOfflineHtmlPageArg::SetMediaPlayerAutoCloseEnabled(bool enabled)
{
    (void)enabled;
}

OfflineHtmlPageReturnValue::OfflineHtmlPageReturnValue()
{
    exitReason = 0;
}

Result ShowOfflineHtmlPage(OfflineHtmlPageReturnValue* outValue, const ShowOfflineHtmlPageArg& arg)
{
    (void)outValue;
    (void)arg;
    std::printf("[nn::web] Page web demandee, pas disponible sur PC\n");
    return ResultOffline;
}

}

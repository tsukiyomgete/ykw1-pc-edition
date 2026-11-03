#pragma once
#include <cstdint>
#include "nn/types.h"

//Navigateur web integre de la Switch (pages hors ligne, videos) : pas sur PC
namespace nn::web
{
    class ShowOfflineHtmlPageArg {
    public:
        ShowOfflineHtmlPageArg(const char* path);
        void SetBootAsMediaPlayer(bool enabled);
        void SetMediaPlayerAutoCloseEnabled(bool enabled);

    private:
        const char* pagePath;
    };

    class OfflineHtmlPageReturnValue {
    public:
        OfflineHtmlPageReturnValue();

    private:
        uint64_t exitReason;
    };

    Result ShowOfflineHtmlPage(OfflineHtmlPageReturnValue* outValue, const ShowOfflineHtmlPageArg& arg);
}

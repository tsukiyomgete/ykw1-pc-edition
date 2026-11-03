#include "nn/settings.h"
#include <cstring>

namespace nn::settings {

//Les codes dans l'ordre de l'enum Language
static const char* LANGUAGE_CODES[] = {
    "ja", "en-US", "fr", "de", "it", "es", "zh-CN", "ko", "nl", "pt", "ru",
    "zh-TW", "en-GB", "fr-CA", "es-419", "zh-Hans", "zh-Hant",
};

LanguageCode LanguageCode::Make(Language language)
{
    LanguageCode code;
    if (language >= 0 && language <= Language_TraditionalChinese)
    {
        std::strcpy(code.string, LANGUAGE_CODES[language]);
    }
    return code;
}

bool operator==(const Language& language, const LanguageCode& code)
{
    LanguageCode other = LanguageCode::Make(language);
    return std::strcmp(other.string, code.string) == 0;
}

bool operator==(const LanguageCode& code, const Language& language)
{
    return language == code;
}

}

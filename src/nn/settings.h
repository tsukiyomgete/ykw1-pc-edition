#pragma once

namespace nn::settings
{
    enum Language {
        Language_Japanese = 0,
        Language_AmericanEnglish,
        Language_French,
        Language_German,
        Language_Italian,
        Language_Spanish,
        Language_Chinese,
        Language_Korean,
        Language_Dutch,
        Language_Portuguese,
        Language_Russian,
        Language_Taiwanese,
        Language_BritishEnglish,
        Language_CanadianFrench,
        Language_LatinAmericanSpanish,
        Language_SimplifiedChinese,
        Language_TraditionalChinese,
    };

    //Code de langue, par exemple "ja" ou "fr"
    struct LanguageCode {
        char string[8] = {};

        static LanguageCode Make(Language language);
    };

    bool operator==(const Language& language, const LanguageCode& code);
    bool operator==(const LanguageCode& code, const Language& language);
}

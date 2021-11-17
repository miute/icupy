#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/unistr.h>
#include <unicode/uscript.h>

void init_uscript(py::module &m) {
  py::enum_<UScriptCode>(
      m, "UScriptCode", py::arithmetic(),
      "Constants for ISO 15924 script codes.\n\n"
      "The current set of script code constants supports at least all scripts that are encoded in the version of "
      "Unicode which ICU currently supports. The names of the constants are usually derived from the Unicode script "
      "property value aliases. See UAX #24 Unicode Script Property (http://www.unicode.org/reports/tr24/) and "
      "http://www.unicode.org/Public/UCD/latest/ucd/PropertyValueAliases.txt .\n\n"
      "In addition, constants for many ISO 15924 script codes are included, for use with language tags, CLDR data, and "
      "similar. Some of those codes are not used in the Unicode Character Database (UCD). For example, there are no "
      "characters that have a UCD script property value of Hans or Hant. All Han ideographs have the Hani script "
      "property value in Unicode.\n\n"
      "Private-use codes Qaaa..Qabx are not included, except as used in the UCD or in CLDR.\n\n"
      "Starting with ICU 55, script codes are only added when their scripts have been or will certainly be encoded in "
      "Unicode, and have been assigned Unicode script property value aliases, to ensure that their script names are "
      "stable and match the names of the constants. Script codes like Latf and Aran that are not subject to separate "
      "encoding may be added at any time.")
      .value("USCRIPT_INVALID_CODE", USCRIPT_INVALID_CODE)
      .value("USCRIPT_COMMON", USCRIPT_COMMON)
      .value("USCRIPT_INHERITED", USCRIPT_INHERITED)
      .value("USCRIPT_ARABIC", USCRIPT_ARABIC)
      .value("USCRIPT_ARMENIAN", USCRIPT_ARMENIAN)
      .value("USCRIPT_BENGALI", USCRIPT_BENGALI)
      .value("USCRIPT_BOPOMOFO", USCRIPT_BOPOMOFO)
      .value("USCRIPT_CHEROKEE", USCRIPT_CHEROKEE)
      .value("USCRIPT_COPTIC", USCRIPT_COPTIC)
      .value("USCRIPT_CYRILLIC", USCRIPT_CYRILLIC)
      .value("USCRIPT_DESERET", USCRIPT_DESERET)
      .value("USCRIPT_DEVANAGARI", USCRIPT_DEVANAGARI)
      .value("USCRIPT_ETHIOPIC", USCRIPT_ETHIOPIC)
      .value("USCRIPT_GEORGIAN", USCRIPT_GEORGIAN)
      .value("USCRIPT_GOTHIC", USCRIPT_GOTHIC)
      .value("USCRIPT_GREEK", USCRIPT_GREEK)
      .value("USCRIPT_GUJARATI", USCRIPT_GUJARATI)
      .value("USCRIPT_GURMUKHI", USCRIPT_GURMUKHI)
      .value("USCRIPT_HAN", USCRIPT_HAN)
      .value("USCRIPT_HANGUL", USCRIPT_HANGUL)
      .value("USCRIPT_HEBREW", USCRIPT_HEBREW)
      .value("USCRIPT_HIRAGANA", USCRIPT_HIRAGANA)
      .value("USCRIPT_KANNADA", USCRIPT_KANNADA)
      .value("USCRIPT_KATAKANA", USCRIPT_KATAKANA)
      .value("USCRIPT_KHMER", USCRIPT_KHMER)
      .value("USCRIPT_LAO", USCRIPT_LAO)
      .value("USCRIPT_LATIN", USCRIPT_LATIN)
      .value("USCRIPT_MALAYALAM", USCRIPT_MALAYALAM)
      .value("USCRIPT_MONGOLIAN", USCRIPT_MONGOLIAN)
      .value("USCRIPT_MYANMAR", USCRIPT_MYANMAR)
      .value("USCRIPT_OGHAM", USCRIPT_OGHAM)
      .value("USCRIPT_OLD_ITALIC", USCRIPT_OLD_ITALIC)
      .value("USCRIPT_ORIYA", USCRIPT_ORIYA)
      .value("USCRIPT_RUNIC", USCRIPT_RUNIC)
      .value("USCRIPT_SINHALA", USCRIPT_SINHALA)
      .value("USCRIPT_SYRIAC", USCRIPT_SYRIAC)
      .value("USCRIPT_TAMIL", USCRIPT_TAMIL)
      .value("USCRIPT_TELUGU", USCRIPT_TELUGU)
      .value("USCRIPT_THAANA", USCRIPT_THAANA)
      .value("USCRIPT_THAI", USCRIPT_THAI)
      .value("USCRIPT_TIBETAN", USCRIPT_TIBETAN)
      .value("USCRIPT_CANADIAN_ABORIGINAL", USCRIPT_CANADIAN_ABORIGINAL, "Canadian_Aboriginal script.")
      .value("USCRIPT_UCAS", USCRIPT_UCAS, "Canadian_Aboriginal script (alias).")
      .value("USCRIPT_YI", USCRIPT_YI)
      .value("USCRIPT_TAGALOG", USCRIPT_TAGALOG)
      .value("USCRIPT_HANUNOO", USCRIPT_HANUNOO)
      .value("USCRIPT_BUHID", USCRIPT_BUHID)
      .value("USCRIPT_TAGBANWA", USCRIPT_TAGBANWA)
      .value("USCRIPT_BRAILLE", USCRIPT_BRAILLE)
      .value("USCRIPT_CYPRIOT", USCRIPT_CYPRIOT)
      .value("USCRIPT_LIMBU", USCRIPT_LIMBU)
      .value("USCRIPT_LINEAR_B", USCRIPT_LINEAR_B)
      .value("USCRIPT_OSMANYA", USCRIPT_OSMANYA)
      .value("USCRIPT_SHAVIAN", USCRIPT_SHAVIAN)
      .value("USCRIPT_TAI_LE", USCRIPT_TAI_LE)
      .value("USCRIPT_UGARITIC", USCRIPT_UGARITIC)
      .value("USCRIPT_KATAKANA_OR_HIRAGANA", USCRIPT_KATAKANA_OR_HIRAGANA, "New script code in Unicode 4.0.1.")
      .value("USCRIPT_BUGINESE", USCRIPT_BUGINESE)
      .value("USCRIPT_GLAGOLITIC", USCRIPT_GLAGOLITIC)
      .value("USCRIPT_KHAROSHTHI", USCRIPT_KHAROSHTHI)
      .value("USCRIPT_SYLOTI_NAGRI", USCRIPT_SYLOTI_NAGRI)
      .value("USCRIPT_NEW_TAI_LUE", USCRIPT_NEW_TAI_LUE)
      .value("USCRIPT_TIFINAGH", USCRIPT_TIFINAGH)
      .value("USCRIPT_OLD_PERSIAN", USCRIPT_OLD_PERSIAN)
      .value("USCRIPT_BALINESE", USCRIPT_BALINESE)
      .value("USCRIPT_BATAK", USCRIPT_BATAK)
      .value("USCRIPT_BLISSYMBOLS", USCRIPT_BLISSYMBOLS)
      .value("USCRIPT_BRAHMI", USCRIPT_BRAHMI)
      .value("USCRIPT_CHAM", USCRIPT_CHAM)
      .value("USCRIPT_CIRTH", USCRIPT_CIRTH)
      .value("USCRIPT_OLD_CHURCH_SLAVONIC_CYRILLIC", USCRIPT_OLD_CHURCH_SLAVONIC_CYRILLIC)
      .value("USCRIPT_DEMOTIC_EGYPTIAN", USCRIPT_DEMOTIC_EGYPTIAN)
      .value("USCRIPT_HIERATIC_EGYPTIAN", USCRIPT_HIERATIC_EGYPTIAN)
      .value("USCRIPT_EGYPTIAN_HIEROGLYPHS", USCRIPT_EGYPTIAN_HIEROGLYPHS)
      .value("USCRIPT_KHUTSURI", USCRIPT_KHUTSURI)
      .value("USCRIPT_SIMPLIFIED_HAN", USCRIPT_SIMPLIFIED_HAN)
      .value("USCRIPT_TRADITIONAL_HAN", USCRIPT_TRADITIONAL_HAN)
      .value("USCRIPT_PAHAWH_HMONG", USCRIPT_PAHAWH_HMONG)
      .value("USCRIPT_OLD_HUNGARIAN", USCRIPT_OLD_HUNGARIAN)
      .value("USCRIPT_HARAPPAN_INDUS", USCRIPT_HARAPPAN_INDUS)
      .value("USCRIPT_JAVANESE", USCRIPT_JAVANESE)
      .value("USCRIPT_KAYAH_LI", USCRIPT_KAYAH_LI)
      .value("USCRIPT_LATIN_FRAKTUR", USCRIPT_LATIN_FRAKTUR)
      .value("USCRIPT_LATIN_GAELIC", USCRIPT_LATIN_GAELIC)
      .value("USCRIPT_LEPCHA", USCRIPT_LEPCHA)
      .value("USCRIPT_LINEAR_A", USCRIPT_LINEAR_A)
      .value("USCRIPT_MANDAIC", USCRIPT_MANDAIC)
      .value("USCRIPT_MANDAEAN", USCRIPT_MANDAEAN)
      .value("USCRIPT_MAYAN_HIEROGLYPHS", USCRIPT_MAYAN_HIEROGLYPHS)
      .value("USCRIPT_MEROITIC_HIEROGLYPHS", USCRIPT_MEROITIC_HIEROGLYPHS)
      .value("USCRIPT_MEROITIC", USCRIPT_MEROITIC)
      .value("USCRIPT_NKO", USCRIPT_NKO)
      .value("USCRIPT_ORKHON", USCRIPT_ORKHON)
      .value("USCRIPT_OLD_PERMIC", USCRIPT_OLD_PERMIC)
      .value("USCRIPT_PHAGS_PA", USCRIPT_PHAGS_PA)
      .value("USCRIPT_PHOENICIAN", USCRIPT_PHOENICIAN)
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
      .value("USCRIPT_MIAO", USCRIPT_MIAO)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
      .value("USCRIPT_PHONETIC_POLLARD", USCRIPT_PHONETIC_POLLARD)
      .value("USCRIPT_RONGORONGO", USCRIPT_RONGORONGO)
      .value("USCRIPT_SARATI", USCRIPT_SARATI)
      .value("USCRIPT_ESTRANGELO_SYRIAC", USCRIPT_ESTRANGELO_SYRIAC)
      .value("USCRIPT_WESTERN_SYRIAC", USCRIPT_WESTERN_SYRIAC)
      .value("USCRIPT_EASTERN_SYRIAC", USCRIPT_EASTERN_SYRIAC)
      .value("USCRIPT_TENGWAR", USCRIPT_TENGWAR)
      .value("USCRIPT_VAI", USCRIPT_VAI)
      .value("USCRIPT_VISIBLE_SPEECH", USCRIPT_VISIBLE_SPEECH)
      .value("USCRIPT_CUNEIFORM", USCRIPT_CUNEIFORM)
      .value("USCRIPT_UNWRITTEN_LANGUAGES", USCRIPT_UNWRITTEN_LANGUAGES)
      .value("USCRIPT_UNKNOWN", USCRIPT_UNKNOWN)
      .value("USCRIPT_CARIAN", USCRIPT_CARIAN)
      .value("USCRIPT_JAPANESE", USCRIPT_JAPANESE)
      .value("USCRIPT_LANNA", USCRIPT_LANNA)
      .value("USCRIPT_LYCIAN", USCRIPT_LYCIAN)
      .value("USCRIPT_LYDIAN", USCRIPT_LYDIAN)
      .value("USCRIPT_OL_CHIKI", USCRIPT_OL_CHIKI)
      .value("USCRIPT_REJANG", USCRIPT_REJANG)
      .value("USCRIPT_SAURASHTRA", USCRIPT_SAURASHTRA)
      .value("USCRIPT_SIGN_WRITING", USCRIPT_SIGN_WRITING, "Sutton SignWriting.")
      .value("USCRIPT_SUNDANESE", USCRIPT_SUNDANESE)
      .value("USCRIPT_MOON", USCRIPT_MOON)
      .value("USCRIPT_MEITEI_MAYEK", USCRIPT_MEITEI_MAYEK)
      .value("USCRIPT_IMPERIAL_ARAMAIC", USCRIPT_IMPERIAL_ARAMAIC)
      .value("USCRIPT_AVESTAN", USCRIPT_AVESTAN)
      .value("USCRIPT_CHAKMA", USCRIPT_CHAKMA)
      .value("USCRIPT_KOREAN", USCRIPT_KOREAN)
      .value("USCRIPT_KAITHI", USCRIPT_KAITHI)
      .value("USCRIPT_MANICHAEAN", USCRIPT_MANICHAEAN)
      .value("USCRIPT_INSCRIPTIONAL_PAHLAVI", USCRIPT_INSCRIPTIONAL_PAHLAVI)
      .value("USCRIPT_PSALTER_PAHLAVI", USCRIPT_PSALTER_PAHLAVI)
      .value("USCRIPT_BOOK_PAHLAVI", USCRIPT_BOOK_PAHLAVI)
      .value("USCRIPT_INSCRIPTIONAL_PARTHIAN", USCRIPT_INSCRIPTIONAL_PARTHIAN)
      .value("USCRIPT_SAMARITAN", USCRIPT_SAMARITAN)
      .value("USCRIPT_TAI_VIET", USCRIPT_TAI_VIET)
      .value("USCRIPT_MATHEMATICAL_NOTATION", USCRIPT_MATHEMATICAL_NOTATION)
      .value("USCRIPT_SYMBOLS", USCRIPT_SYMBOLS)
      .value("USCRIPT_BAMUM", USCRIPT_BAMUM)
      .value("USCRIPT_LISU", USCRIPT_LISU)
      .value("USCRIPT_NAKHI_GEBA", USCRIPT_NAKHI_GEBA)
      .value("USCRIPT_OLD_SOUTH_ARABIAN", USCRIPT_OLD_SOUTH_ARABIAN)
      .value("USCRIPT_BASSA_VAH", USCRIPT_BASSA_VAH)
      .value("USCRIPT_DUPLOYAN", USCRIPT_DUPLOYAN)
#ifndef U_HIDE_DEPRECATED_API
      .value("USCRIPT_DUPLOYAN_SHORTAND", USCRIPT_DUPLOYAN_SHORTAND,
             "**Deprecated:** ICU 54 Typo, use *USCRIPT_DUPLOYAN*.")
#endif // U_HIDE_DEPRECATED_API
      .value("USCRIPT_ELBASAN", USCRIPT_ELBASAN)
      .value("USCRIPT_GRANTHA", USCRIPT_GRANTHA)
      .value("USCRIPT_KPELLE", USCRIPT_KPELLE)
      .value("USCRIPT_LOMA", USCRIPT_LOMA)
      .value("USCRIPT_MENDE", USCRIPT_MENDE, "Mende Kikakui.")
      .value("USCRIPT_MEROITIC_CURSIVE", USCRIPT_MEROITIC_CURSIVE)
      .value("USCRIPT_OLD_NORTH_ARABIAN", USCRIPT_OLD_NORTH_ARABIAN)
      .value("USCRIPT_NABATAEAN", USCRIPT_NABATAEAN)
      .value("USCRIPT_PALMYRENE", USCRIPT_PALMYRENE)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("USCRIPT_KHUDAWADI", USCRIPT_KHUDAWADI)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("USCRIPT_SINDHI", USCRIPT_SINDHI)
      .value("USCRIPT_WARANG_CITI", USCRIPT_WARANG_CITI)
      .value("USCRIPT_AFAKA", USCRIPT_AFAKA)
      .value("USCRIPT_JURCHEN", USCRIPT_JURCHEN)
      .value("USCRIPT_MRO", USCRIPT_MRO)
      .value("USCRIPT_NUSHU", USCRIPT_NUSHU)
      .value("USCRIPT_SHARADA", USCRIPT_SHARADA)
      .value("USCRIPT_SORA_SOMPENG", USCRIPT_SORA_SOMPENG)
      .value("USCRIPT_TAKRI", USCRIPT_TAKRI)
      .value("USCRIPT_TANGUT", USCRIPT_TANGUT)
      .value("USCRIPT_WOLEAI", USCRIPT_WOLEAI)
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("USCRIPT_ANATOLIAN_HIEROGLYPHS", USCRIPT_ANATOLIAN_HIEROGLYPHS)
      .value("USCRIPT_KHOJKI", USCRIPT_KHOJKI)
      .value("USCRIPT_TIRHUTA", USCRIPT_TIRHUTA)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
      .value("USCRIPT_CAUCASIAN_ALBANIAN", USCRIPT_CAUCASIAN_ALBANIAN)
      .value("USCRIPT_MAHAJANI", USCRIPT_MAHAJANI)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("USCRIPT_AHOM", USCRIPT_AHOM)
      .value("USCRIPT_HATRAN", USCRIPT_HATRAN)
      .value("USCRIPT_MODI", USCRIPT_MODI)
      .value("USCRIPT_MULTANI", USCRIPT_MULTANI)
      .value("USCRIPT_PAU_CIN_HAU", USCRIPT_PAU_CIN_HAU)
      .value("USCRIPT_SIDDHAM", USCRIPT_SIDDHAM)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("USCRIPT_ADLAM", USCRIPT_ADLAM)
      .value("USCRIPT_BHAIKSUKI", USCRIPT_BHAIKSUKI)
      .value("USCRIPT_MARCHEN", USCRIPT_MARCHEN)
      .value("USCRIPT_NEWA", USCRIPT_NEWA)
      .value("USCRIPT_OSAGE", USCRIPT_OSAGE)
      .value("USCRIPT_HAN_WITH_BOPOMOFO", USCRIPT_HAN_WITH_BOPOMOFO)
      .value("USCRIPT_JAMO", USCRIPT_JAMO)
      .value("USCRIPT_SYMBOLS_EMOJI", USCRIPT_SYMBOLS_EMOJI)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
      .value("USCRIPT_MASARAM_GONDI", USCRIPT_MASARAM_GONDI)
      .value("USCRIPT_SOYOMBO", USCRIPT_SOYOMBO)
      .value("USCRIPT_ZANABAZAR_SQUARE", USCRIPT_ZANABAZAR_SQUARE)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("USCRIPT_DOGRA", USCRIPT_DOGRA)
      .value("USCRIPT_GUNJALA_GONDI", USCRIPT_GUNJALA_GONDI)
      .value("USCRIPT_MAKASAR", USCRIPT_MAKASAR)
      .value("USCRIPT_MEDEFAIDRIN", USCRIPT_MEDEFAIDRIN)
      .value("USCRIPT_HANIFI_ROHINGYA", USCRIPT_HANIFI_ROHINGYA)
      .value("USCRIPT_SOGDIAN", USCRIPT_SOGDIAN)
      .value("USCRIPT_OLD_SOGDIAN", USCRIPT_OLD_SOGDIAN)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("USCRIPT_ELYMAIC", USCRIPT_ELYMAIC)
      .value("USCRIPT_NYIAKENG_PUACHUE_HMONG", USCRIPT_NYIAKENG_PUACHUE_HMONG)
      .value("USCRIPT_NANDINAGARI", USCRIPT_NANDINAGARI)
      .value("USCRIPT_WANCHO", USCRIPT_WANCHO)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
#if (U_ICU_VERSION_MAJOR_NUM >= 66)
      .value("USCRIPT_CHORASMIAN", USCRIPT_CHORASMIAN)
      .value("USCRIPT_DIVES_AKURU", USCRIPT_DIVES_AKURU)
      .value("USCRIPT_KHITAN_SMALL_SCRIPT", USCRIPT_KHITAN_SMALL_SCRIPT)
      .value("USCRIPT_YEZIDI", USCRIPT_YEZIDI)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 66)
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
      .value("USCRIPT_CYPRO_MINOAN", USCRIPT_CYPRO_MINOAN)
      .value("USCRIPT_OLD_UYGHUR", USCRIPT_OLD_UYGHUR)
      .value("USCRIPT_TANGSA", USCRIPT_TANGSA)
      .value("USCRIPT_TOTO", USCRIPT_TOTO)
      .value("USCRIPT_VITHKUQI", USCRIPT_VITHKUQI)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
#ifndef U_HIDE_DEPRECATED_API
      .value("USCRIPT_CODE_LIMIT", USCRIPT_CODE_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  py::enum_<UScriptUsage>(
      m, "UScriptUsage", py::arithmetic(),
      "Script usage constants.\n\n"
      "See UAX #31 Unicode Identifier and Pattern Syntax. "
      "http://www.unicode.org/reports/tr31/#Table_Candidate_Characters_for_Exclusion_from_Identifiers")
      .value("USCRIPT_USAGE_NOT_ENCODED", USCRIPT_USAGE_NOT_ENCODED, "Not encoded in Unicode.")
      .value("USCRIPT_USAGE_UNKNOWN", USCRIPT_USAGE_UNKNOWN, "Unknown script usage.")
      .value("USCRIPT_USAGE_EXCLUDED", USCRIPT_USAGE_EXCLUDED, "Candidate for Exclusion from Identifiers.")
      .value("USCRIPT_USAGE_LIMITED_USE", USCRIPT_USAGE_LIMITED_USE, "Limited Use script.")
      .value("USCRIPT_USAGE_ASPIRATIONAL", USCRIPT_USAGE_ASPIRATIONAL, "Aspirational Use script.")
      .value("USCRIPT_USAGE_RECOMMENDED", USCRIPT_USAGE_RECOMMENDED, "Recommended script.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def("uscript_breaks_between_letters", &uscript_breaksBetweenLetters, py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uscript_get_code",
      [](const char *name_or_abbr_or_locale) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto length = uscript_getCode(name_or_abbr_or_locale, nullptr, 0, &error_code);
        std::vector<UScriptCode> result(length);
        error_code = U_ZERO_ERROR;
        uscript_getCode(name_or_abbr_or_locale, result.data(), static_cast<int32_t>(result.size()), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("name_or_abbr_or_locale"));
  m.def("uscript_get_name", &uscript_getName, py::arg("script_code"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uscript_get_sample_string",
      [](UScriptCode script) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto length = uscript_getSampleString(script, nullptr, 0, &error_code);
        std::u16string result(length, u'\0');
        error_code = U_ZERO_ERROR;
        uscript_getSampleString(script, result.data(), static_cast<int32_t>(result.size()), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("script"));
  m.def("uscript_get_sample_unicode_string", &uscript_getSampleUnicodeString, py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uscript_get_script",
      [](UChar32 codepoint) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uscript_getScript(codepoint, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("codepoint"));
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def(
      "uscript_get_script_extensions",
      [](UChar32 c) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto length = uscript_getScriptExtensions(c, nullptr, 0, &error_code);
        std::vector<UScriptCode> result(length);
        error_code = U_ZERO_ERROR;
        uscript_getScriptExtensions(c, result.data(), static_cast<int32_t>(result.size()), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("c"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def("uscript_get_short_name", &uscript_getShortName, py::arg("script_code"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def("uscript_get_usage", &uscript_getUsage, py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def("uscript_has_script", &uscript_hasScript, py::arg("c"), py::arg("sc"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def("uscript_is_cased", &uscript_isCased, py::arg("script"));
  m.def("uscript_is_right_to_left", &uscript_isRightToLeft, py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

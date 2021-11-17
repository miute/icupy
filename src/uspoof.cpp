#include "main.hpp"
#include "usetptr.hpp"
#include "uspoofptr.hpp"
#include <pybind11/stl.h>

using namespace icu;

_USpoofCheckerPtr::_USpoofCheckerPtr(USpoofChecker *p) : p_(p) {}
_USpoofCheckerPtr::~_USpoofCheckerPtr() {}
USpoofChecker *_USpoofCheckerPtr::get() const { return p_; }

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
_USpoofCheckResultPtr::_USpoofCheckResultPtr(USpoofCheckResult *p) : p_(p) {}
_USpoofCheckResultPtr::~_USpoofCheckResultPtr() {}
USpoofCheckResult *_USpoofCheckResultPtr::get() const { return p_; }
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

void init_uspoof(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  py::enum_<URestrictionLevel>(m, "URestrictionLevel", py::arithmetic(),
                               "Constants from UAX #39 for use in *uspoof_set_restriction_level*, and for returned "
                               "identifier restriction levels in check results.")
      .value("USPOOF_ASCII", USPOOF_ASCII,
             "All characters in the string are in the identifier profile and all characters in the string are in the "
             "ASCII range.")
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
      .value("USPOOF_SINGLE_SCRIPT_RESTRICTIVE", USPOOF_SINGLE_SCRIPT_RESTRICTIVE,
             "The string classifies as ASCII-Only, or all characters in the string are in the identifier profile and "
             "the string is single-script, according to the definition in UTS 39 section 5.1.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
      .value("USPOOF_HIGHLY_RESTRICTIVE", USPOOF_HIGHLY_RESTRICTIVE,
             "The string classifies as Single Script, or all characters in the string are in the identifier profile "
             "and the string is covered by any of the following sets of scripts, according to the definition in UTS 39 "
             "section 5.1:\n\n  "
             "* Latin + Han + Bopomofo (or equivalently: Latn + Hanb)\n  "
             "* Latin + Han + Hiragana + Katakana (or equivalently: Latn + Jpan)\n  "
             "* Latin + Han + Hangul (or equivalently: Latn +Kore)\n\n  "
             "This is the default restriction in ICU.")
      .value("USPOOF_MODERATELY_RESTRICTIVE", USPOOF_MODERATELY_RESTRICTIVE,
             "The string classifies as Highly Restrictive, or all characters in the string are in the identifier "
             "profile and the string is covered by Latin and any one other Recommended or Aspirational script, except "
             "Cyrillic, Greek, and Cherokee.")
      .value("USPOOF_MINIMALLY_RESTRICTIVE", USPOOF_MINIMALLY_RESTRICTIVE,
             "All characters in the string are in the identifier profile. Allow arbitrary mixtures of scripts.")
      .value("USPOOF_UNRESTRICTIVE", USPOOF_UNRESTRICTIVE,
             "Any valid identifiers, including characters outside of the Identifier Profile.")
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
      .value("USPOOF_RESTRICTION_LEVEL_MASK", USPOOF_RESTRICTION_LEVEL_MASK,
             "Mask for selecting the Restriction Level bits from the return value of *uspoof_check*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  py::enum_<USpoofChecks>(m, "USpoofChecks", py::arithmetic(),
                          "Enum for the kinds of checks that *USpoofChecker* can perform.\n\n"
                          "These enum values are used both to select the set of checks that will be performed, and to "
                          "report results from the check function.")
      .value("USPOOF_SINGLE_SCRIPT_CONFUSABLE", USPOOF_SINGLE_SCRIPT_CONFUSABLE,
             "When performing the two-string *uspoof_are_confusable* test, this flag in the return value indicates "
             "that the two strings are visually confusable and that they are from the same script, according to UTS 39 "
             "section 4.")
      .value("USPOOF_MIXED_SCRIPT_CONFUSABLE", USPOOF_MIXED_SCRIPT_CONFUSABLE,
             "When performing the two-string *uspoof_are_confusable* test, this flag in the return value indicates "
             "that the two strings are visually confusable and that they are not from the same script, according to "
             "UTS 39 section 4.")
      .value("USPOOF_WHOLE_SCRIPT_CONFUSABLE", USPOOF_WHOLE_SCRIPT_CONFUSABLE,
             "When performing the two-string *uspoof_are_confusable* test, this flag in the return value indicates "
             "that the two strings are visually confusable and that they are not from the same script but both of them "
             "are single-script strings, according to UTS 39 section 4.")
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("USPOOF_CONFUSABLE", USPOOF_CONFUSABLE,
             "Enable this flag in *uspoof_set_checks* to turn on all types of confusables.\n\n  "
             "You may set the checks to some subset of SINGLE_SCRIPT_CONFUSABLE, MIXED_SCRIPT_CONFUSABLE, or "
             "WHOLE_SCRIPT_CONFUSABLE to make *uspoof_are_confusable* return only those types of confusables.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

#ifndef U_HIDE_DEPRECATED_API
      .value("USPOOF_ANY_CASE", USPOOF_ANY_CASE,
             "**Deprecated:** ICU 58 Any case confusable mappings were removed from UTS 39; the corresponding ICU API "
             "was deprecated.")
#endif // U_HIDE_DEPRECATED_API
      .value("USPOOF_RESTRICTION_LEVEL", USPOOF_RESTRICTION_LEVEL,
             "Check that an identifier is no looser than the specified RestrictionLevel.\n\n  "
             "The default if *uspoof_set_restriction_level* is not called is HIGHLY_RESTRICTIVE.\n\n  "
             "If *USPOOF_AUX_INFO* is enabled the actual restriction level of the identifier being tested will also be "
             "returned by *uspoof_check()*.")
#ifndef U_HIDE_DEPRECATED_API
      .value("USPOOF_SINGLE_SCRIPT", USPOOF_SINGLE_SCRIPT, "**Deprecated:** ICU 51 Use RESTRICTION_LEVEL instead.")
#endif // U_HIDE_DEPRECATED_API
      .value("USPOOF_INVISIBLE", USPOOF_INVISIBLE,
             "Check an identifier for the presence of invisible characters, such as zero-width spaces, or character "
             "sequences that are likely not to display, such as multiple occurrences of the same non-spacing mark. "
             "This check does not test the input string as a whole for conformance to any particular syntax for "
             "identifiers.")
      .value("USPOOF_CHAR_LIMIT", USPOOF_CHAR_LIMIT,
             "Check that an identifier contains only characters from a specified set of acceptable characters.\n\n  "
             "See *uspoof_set_allowed_chars* and *uspoof_set_allowed_locales*. Note that a string that fails this "
             "check will also fail the *USPOOF_RESTRICTION_LEVEL* check.")
      .value("USPOOF_MIXED_NUMBERS", USPOOF_MIXED_NUMBERS,
             "Check that an identifier does not mix numbers from different numbering systems.\n\n  "
             "For more information, see UTS 39 section 5.3.")
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("USPOOF_HIDDEN_OVERLAY", USPOOF_HIDDEN_OVERLAY,
             "Check that an identifier does not have a combining character following a character in which that "
             "combining character would be hidden; for example 'i' followed by a U+0307 combining dot.\n\n  "
             "More specifically, the following characters are forbidden from preceding a U+0307:\n\n  "
             "* Those with the Soft_Dotted Unicode property (which includes 'i' and 'j')\n  "
             "* Latin lowercase letter 'l'\n  "
             "* Dotless 'i' and 'j' ('ı' and 'ȷ', U+0131 and U+0237)\n  "
             "* Any character whose confusable prototype ends with such a character (Soft_Dotted, 'l', 'ı', or "
             "'ȷ')\n\n  "
             "In addition, combining characters are allowed between the above characters and U+0307 except those "
             "with combining class 0 or combining class \"Above\" (230, same class as U+0307).\n\n  "
             "This list and the number of combing characters considered by this check may grow over time.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("USPOOF_ALL_CHECKS", USPOOF_ALL_CHECKS, "Enable all spoof checks.")
      .value("USPOOF_AUX_INFO", USPOOF_AUX_INFO,
             "Enable the return of auxiliary (non-error) information in the upper bits of the check results "
             "value.\n\n  "
             "If this \"check\" is not enabled, the results of *uspoof_check* will be zero when an identifier passes "
             "all of the enabled checks.\n\n  "
             "If this \"check\" is enabled, (*uspoof_check()* & *USPOOF_ALL_CHECKS*) will be zero when an identifier "
             "passes all checks.")
      .export_values();

  // USpoofChecker
  py::class_<_USpoofCheckerPtr>(m, "_USpoofCheckerPtr");

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  // USpoofCheckResult
  py::class_<_USpoofCheckResultPtr>(m, "_USpoofCheckResultPtr");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

  m.def(
      "uspoof_are_confusable",
      [](const _USpoofCheckerPtr &sc, const UChar *id1, int32_t length1, const UChar *id2, int32_t length2) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_areConfusable(sc, id1, length1, id2, length2, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id1"), py::arg("length1"), py::arg("id2"), py::arg("length2"));
  m.def(
      "uspoof_are_confusable_unicode_string",
      [](const _USpoofCheckerPtr &sc, const UnicodeString &s1, const UnicodeString &s2) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_areConfusableUnicodeString(sc, s1, s2, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("s1"), py::arg("s2"));
  m.def(
      "uspoof_are_confusable_utf8",
      [](const _USpoofCheckerPtr &sc, const char *id1, int32_t length1, const char *id2, int32_t length2) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_areConfusableUTF8(sc, id1, length1, id2, length2, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id1"), py::arg("length1"), py::arg("id2"), py::arg("length2"));
  m.def(
      "uspoof_check",
      [](const _USpoofCheckerPtr &sc, const UChar *id, int32_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_check(sc, id, length, nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id_"), py::arg("length") = -1);

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  m.def(
      "uspoof_check2",
      [](const _USpoofCheckerPtr &sc, const UChar *id, int32_t length,
         std::optional<_USpoofCheckResultPtr> &check_result) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_check2(sc, id, length, check_result.value_or(nullptr), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id_"), py::arg("length") = -1, py::arg("check_result") = std::nullopt);
  m.def(
      "uspoof_check2_unicode_string",
      [](const _USpoofCheckerPtr &sc, const UnicodeString &id, std::optional<_USpoofCheckResultPtr> &check_result) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_check2UnicodeString(sc, id, check_result.value_or(nullptr), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id_"), py::arg("check_result") = std::nullopt);
  m.def(
      "uspoof_check2_utf8",
      [](const _USpoofCheckerPtr &sc, const char *id, int32_t length,
         std::optional<_USpoofCheckResultPtr> &check_result) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_check2UTF8(sc, id, length, check_result.value_or(nullptr), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id_"), py::arg("length") = -1, py::arg("check_result") = std::nullopt);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

  m.def(
      "uspoof_check_unicode_string",
      [](const _USpoofCheckerPtr &sc, const UnicodeString &id) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_checkUnicodeString(sc, id, nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id_"));
  m.def(
      "uspoof_check_utf8",
      [](const _USpoofCheckerPtr &sc, const char *id, int32_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_checkUTF8(sc, id, length, nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("id_"), py::arg("length") = -1);
  m.def(
      "uspoof_clone",
      [](const _USpoofCheckerPtr &sc) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = uspoof_clone(sc, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_USpoofCheckerPtr>(p);
      },
      py::arg("sc"));
  m.def(
      "uspoof_close", [](_USpoofCheckerPtr &sc) { uspoof_close(sc); }, py::arg("sc"));

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  m.def(
      "uspoof_close_check_result", [](_USpoofCheckResultPtr &check_result) { uspoof_closeCheckResult(check_result); },
      py::arg("check_result"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

  m.def(
      "uspoof_get_allowed_chars",
      [](const _USpoofCheckerPtr &sc) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = uspoof_getAllowedChars(sc, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_ConstUSetPtr>(p);
      },
      py::arg("sc"));
  m.def(
      "uspoof_get_allowed_locales",
      [](const _USpoofCheckerPtr &sc) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getAllowedLocales(sc, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"));
  m.def(
      "uspoof_get_allowed_unicode_set",
      [](const _USpoofCheckerPtr &sc) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getAllowedUnicodeSet(sc, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("sc"));

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  m.def(
      "uspoof_get_check_result_checks",
      [](const _USpoofCheckResultPtr &check_result) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getCheckResultChecks(check_result, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("check_result"));
  m.def(
      "uspoof_get_check_result_numerics",
      [](const _USpoofCheckResultPtr &check_result) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = uspoof_getCheckResultNumerics(check_result, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_ConstUSetPtr>(p);
      },
      py::arg("check_result"));
  m.def(
      "uspoof_get_check_result_restriction_level",
      [](const _USpoofCheckResultPtr &check_result) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getCheckResultRestrictionLevel(check_result, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("check_result"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

  m.def(
      "uspoof_get_checks",
      [](const _USpoofCheckerPtr &sc) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getChecks(sc, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def("uspoof_get_inclusion_set", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto p = uspoof_getInclusionSet(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_ConstUSetPtr>(p);
  });
  m.def(
      "uspoof_get_inclusion_unicode_set",
      []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getInclusionUnicodeSet(&error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  m.def("uspoof_get_recommended_set", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto p = uspoof_getRecommendedSet(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_ConstUSetPtr>(p);
  });
  m.def(
      "uspoof_get_recommended_unicode_set",
      []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = uspoof_getRecommendedUnicodeSet(&error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  m.def(
      "uspoof_get_restriction_level", [](const _USpoofCheckerPtr &sc) { return uspoof_getRestrictionLevel(sc); },
      py::arg("sc"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  m.def(
      "uspoof_get_skeleton",
      [](const _USpoofCheckerPtr &sc, uint32_t type, const UChar *id, int32_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto dest_size = uspoof_getSkeleton(sc, type, id, length, nullptr, 0, &error_code);
        std::u16string result(dest_size, u'\0');
        error_code = U_ZERO_ERROR;
        uspoof_getSkeleton(sc, type, id, length, result.data(), dest_size, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("type_"), py::arg("id_"), py::arg("length") = -1);
  m.def(
      "uspoof_get_skeleton_unicode_string",
      [](const _USpoofCheckerPtr &sc, uint32_t type, const UnicodeString &id, UnicodeString &dest) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = uspoof_getSkeletonUnicodeString(sc, type, id, dest, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("type_"), py::arg("id_"), py::arg("dest"));
  m.def(
      "uspoof_get_skeleton_utf8",
      [](const _USpoofCheckerPtr &sc, uint32_t type, const char *id, int32_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto dest_size = uspoof_getSkeletonUTF8(sc, type, id, length, nullptr, 0, &error_code);
        std::string result(dest_size, '\0');
        error_code = U_ZERO_ERROR;
        uspoof_getSkeletonUTF8(sc, type, id, length, result.data(), dest_size, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("sc"), py::arg("type_"), py::arg("id_"), py::arg("length") = -1);
  m.def("uspoof_open", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto sc = uspoof_open(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_USpoofCheckerPtr>(sc);
  });
  m.def("uspoof_open_check_result", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto p = uspoof_openCheckResult(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_USpoofCheckResultPtr>(p);
  });
  m.def(
      "uspoof_open_from_serialized",
      [](const py::buffer &data, int32_t length) {
        auto info = data.request();
        if (length == -1) {
          length = static_cast<int32_t>(info.size);
        }
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = uspoof_openFromSerialized(info.ptr, length, nullptr, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_USpoofCheckerPtr>(p);
      },
      py::arg("data"), py::arg("length") = -1);
  m.def(
      "uspoof_open_from_source",
      [](const char *confusables, int32_t confusables_len, const char *confusables_whole_script,
         int32_t confusables_whole_script_len, UParseError *pe) {
        int32_t err_type = 0;
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = uspoof_openFromSource(confusables, confusables_len, confusables_whole_script,
                                       confusables_whole_script_len, &err_type, pe, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        // return py::make_tuple(std::make_unique<_USpoofCheckerPtr>(p), err_type);
        return std::make_unique<_USpoofCheckerPtr>(p);
      },
      py::arg("confusables"), py::arg("confusables_len"), py::arg("confusables_whole_script"),
      py::arg("confusables_whole_script_len"), py::arg("pe"));
  m.def(
      "uspoof_serialize",
      [](_USpoofCheckerPtr &sc) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto dest_size = uspoof_serialize(sc, nullptr, 0, &error_code);
        std::string data(dest_size, '\0');
        error_code = U_ZERO_ERROR;
        uspoof_serialize(sc, data.data(), dest_size, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::bytes(data);
      },
      py::arg("sc"));
  m.def(
       "uspoof_set_allowed_chars",
       [](_USpoofCheckerPtr &sc, _ConstUSetPtr &chars) {
         UErrorCode error_code = U_ZERO_ERROR;
         uspoof_setAllowedChars(sc, chars, &error_code);
         if (U_FAILURE(error_code)) {
           throw ICUError(error_code);
         }
       },
       py::arg("sc"), py::arg("chars"))
      .def(
          "uspoof_set_allowed_chars",
          [](_USpoofCheckerPtr &sc, const _USetPtr &chars) {
            UErrorCode error_code = U_ZERO_ERROR;
            uspoof_setAllowedChars(sc, chars, &error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("sc"), py::arg("chars"));
  m.def(
      "uspoof_set_allowed_locales",
      [](_USpoofCheckerPtr &sc, const char *locales_list) {
        UErrorCode error_code = U_ZERO_ERROR;
        uspoof_setAllowedLocales(sc, locales_list, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("sc"), py::arg("locales_list"));
  m.def(
      "uspoof_set_allowed_unicode_set",
      [](_USpoofCheckerPtr &sc, const UnicodeSet *chars) {
        UErrorCode error_code = U_ZERO_ERROR;
        uspoof_setAllowedUnicodeSet(sc, chars, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("sc"), py::arg("chars"));
  m.def(
      "uspoof_set_checks",
      [](_USpoofCheckerPtr &sc, int32_t checks) {
        UErrorCode error_code = U_ZERO_ERROR;
        uspoof_setChecks(sc, checks, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("sc"), py::arg("checks"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uspoof_set_restriction_level",
      [](_USpoofCheckerPtr &sc, URestrictionLevel restriction_level) {
        uspoof_setRestrictionLevel(sc, restriction_level);
      },
      py::arg("sc"), py::arg("restriction_level"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

#include "main.hpp"
#include <unicode/uloc.h>

void init_uloc(py::module &m) {
  //
  // enum UAcceptResult
  //
  py::enum_<UAcceptResult>(m, "UAcceptResult", py::arithmetic(), R"doc(
Output values which uloc_acceptLanguage()
writes to the `outResult` parameter.
      )doc")
      .value("ULOC_ACCEPT_FAILED", ULOC_ACCEPT_FAILED, R"doc(
             No exact match was found.
             )doc")
      .value("ULOC_ACCEPT_VALID", ULOC_ACCEPT_VALID, R"doc(
             An exact match was found.
             )doc")
      .value("ULOC_ACCEPT_FALLBACK", ULOC_ACCEPT_FALLBACK, R"doc(
             A fallback was found.

             For example, the Accept-Language list includes 'ja_JP' and is
             matched with available locale 'ja'.
             )doc")
      .export_values();

  //
  // enum ULayoutType
  //
  py::enum_<ULayoutType>(m, "ULayoutType", py::arithmetic(), R"doc(
Enums for the return value for the character and line orientation functions.
      )doc")
      .value("ULOC_LAYOUT_LTR", ULOC_LAYOUT_LTR)
      .value("ULOC_LAYOUT_RTL", ULOC_LAYOUT_RTL)
      .value("ULOC_LAYOUT_TTB", ULOC_LAYOUT_TTB)
      .value("ULOC_LAYOUT_BTT", ULOC_LAYOUT_BTT)
      .value("ULOC_LAYOUT_UNKNOWN", ULOC_LAYOUT_UNKNOWN)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  //
  // enum ULocAvailableType
  //
  py::enum_<ULocAvailableType>(m, "ULocAvailableType", py::arithmetic(), R"doc(
Types for ``uloc_getAvailableByType`` and ``uloc_countAvailableByType``.
      )doc")
      .value("ULOC_AVAILABLE_DEFAULT", ULOC_AVAILABLE_DEFAULT, R"doc(
             Locales that return data when passed to ICU APIs, but not
             including legacy or alias locales.
             )doc")
      .value("ULOC_AVAILABLE_ONLY_LEGACY_ALIASES",
             ULOC_AVAILABLE_ONLY_LEGACY_ALIASES, R"doc(
             Legacy or alias locales that return data when passed to ICU APIs.

             Examples of supported legacy or alias locales:

             - iw (alias to he)
             - mo (alias to ro)
             - zh_CN (alias to zh_Hans_CN)
             - sr_BA (alias to sr_Cyrl_BA)
             - ars (alias to ar_SA)

             The locales in this set are disjoint from the ones in
             ULOC_AVAILABLE_DEFAULT. To get both sets at the same time, use
             ULOC_AVAILABLE_WITH_LEGACY_ALIASES.
             )doc")
      .value("ULOC_AVAILABLE_WITH_LEGACY_ALIASES",
             ULOC_AVAILABLE_WITH_LEGACY_ALIASES, R"doc(
             The union of the locales in ULOC_AVAILABLE_DEFAULT and
             ULOC_AVAILABLE_ONLY_LEGACY_ALIASES.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

  //
  // enum ULocDataLocaleType
  //
  py::enum_<ULocDataLocaleType>(m, "ULocDataLocaleType", py::arithmetic(),
                                R"doc(
Constants for ``.get_locale()``.

Allow user to select whether she wants information on requested, valid or
actual locale. For example, a collator for "en_US_CALIFORNIA" was requested.
In the current state of ICU (2.0), the requested locale is "en_US_CALIFORNIA",
the valid locale is "en_US" (most specific locale supported by ICU) and the
actual locale is "root" (the collation data comes unmodified from the UCA).
The locale is considered supported by ICU if there is a core ICU bundle for
that locale (although it may be empty).
      )doc")
      .value("ULOC_ACTUAL_LOCALE", ULOC_ACTUAL_LOCALE, R"doc(
             This is locale the data actually comes from.
             )doc")
      .value("ULOC_VALID_LOCALE", ULOC_VALID_LOCALE, R"doc(
             This is the most specific locale supported by ICU.
             )doc")
      .value("ULOC_REQUESTED_LOCALE", ULOC_REQUESTED_LOCALE, R"doc(
             Deprecated: ICU 2.8
             )doc")
      .value("ULOC_DATA_LOCALE_TYPE_LIMIT", ULOC_DATA_LOCALE_TYPE_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  m.attr("ULOC_CANADA") = ULOC_CANADA;
  m.attr("ULOC_CANADA_FRENCH") = ULOC_CANADA_FRENCH;
  m.attr("ULOC_CHINA") = ULOC_CHINA;
  m.attr("ULOC_CHINESE") = ULOC_CHINESE;
  m.attr("ULOC_ENGLISH") = ULOC_ENGLISH;
  m.attr("ULOC_FRANCE") = ULOC_FRANCE;
  m.attr("ULOC_FRENCH") = ULOC_FRENCH;
  m.attr("ULOC_GERMAN") = ULOC_GERMAN;
  m.attr("ULOC_GERMANY") = ULOC_GERMANY;
  m.attr("ULOC_ITALIAN") = ULOC_ITALIAN;
  m.attr("ULOC_ITALY") = ULOC_ITALY;
  m.attr("ULOC_JAPAN") = ULOC_JAPAN;
  m.attr("ULOC_JAPANESE") = ULOC_JAPANESE;
  m.attr("ULOC_KOREA") = ULOC_KOREA;
  m.attr("ULOC_KOREAN") = ULOC_KOREAN;
  m.attr("ULOC_PRC") = ULOC_PRC;
  m.attr("ULOC_SIMPLIFIED_CHINESE") = ULOC_SIMPLIFIED_CHINESE;
  m.attr("ULOC_TAIWAN") = ULOC_TAIWAN;
  m.attr("ULOC_TRADITIONAL_CHINESE") = ULOC_TRADITIONAL_CHINESE;
  m.attr("ULOC_UK") = ULOC_UK;
  m.attr("ULOC_US") = ULOC_US;
}

#include "main.hpp"
#include <unicode/uloc.h>

void init_uloc(py::module &m) {
  py::enum_<UAcceptResult>(m, "UAcceptResult", py::arithmetic(),
                           "Output values which *uloc_accept_language()* writes to the 'out_result' parameter.")
      .value("ULOC_ACCEPT_FAILED", ULOC_ACCEPT_FAILED, "No exact match was found.")
      .value("ULOC_ACCEPT_VALID", ULOC_ACCEPT_VALID, "An exact match was found.")
      .value("ULOC_ACCEPT_FALLBACK", ULOC_ACCEPT_FALLBACK,
             "A fallback was found.\n\n  "
             "For example, the Accept-Language list includes 'ja_JP' and is matched with available locale 'ja'.")
      .export_values();

  py::enum_<ULayoutType>(m, "ULayoutType", py::arithmetic(),
                         "enums for the return value for the character and line orientation functions.")
      .value("ULOC_LAYOUT_LTR", ULOC_LAYOUT_LTR)
      .value("ULOC_LAYOUT_RTL", ULOC_LAYOUT_RTL)
      .value("ULOC_LAYOUT_TTB", ULOC_LAYOUT_TTB)
      .value("ULOC_LAYOUT_BTT", ULOC_LAYOUT_BTT)
      .value("ULOC_LAYOUT_UNKNOWN", ULOC_LAYOUT_UNKNOWN)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  py::enum_<ULocAvailableType>(m, "ULocAvailableType", py::arithmetic(),
                               "Types for *uloc_get_available_by_type* and *uloc_count_available_by_type*.")
      .value("ULOC_AVAILABLE_DEFAULT", ULOC_AVAILABLE_DEFAULT,
             "Locales that return data when passed to ICU APIs, but not including legacy or alias locales.")
      .value("ULOC_AVAILABLE_ONLY_LEGACY_ALIASES", ULOC_AVAILABLE_ONLY_LEGACY_ALIASES,
             "Legacy or alias locales that return data when passed to ICU APIs.\n\n  "
             "Examples of supported legacy or alias locales:\n\n  "
             "* iw (alias to he)\n  "
             "* mo (alias to ro)\n  "
             "* zh_CN (alias to zh_Hans_CN)\n  "
             "* sr_BA (alias to sr_Cyrl_BA)\n  "
             "* ars (alias to ar_SA)\n\n  "
             "The locales in this set are disjoint from the ones in *ULOC_AVAILABLE_DEFAULT*. To get both sets at the "
             "same time, use *ULOC_AVAILABLE_WITH_LEGACY_ALIASES*.")
      .value("ULOC_AVAILABLE_WITH_LEGACY_ALIASES", ULOC_AVAILABLE_WITH_LEGACY_ALIASES,
             "The union of the locales in *ULOC_AVAILABLE_DEFAULT* and *ULOC_AVAILABLE_ONLY_LEGACY_ALIAS*.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

  py::enum_<ULocDataLocaleType>(
      m, "ULocDataLocaleType", py::arithmetic(),
      "Constants for *get_locale()*.\n\n"
      "Allow user to select whether she wants information on requested, valid or actual locale. "
      "For example, a collator for \"en_US_CALIFORNIA\" was requested. In the current state of ICU (2.0), the "
      "requested locale is \"en_US_CALIFORNIA\", the valid locale is \"en_US\" (most specific locale supported by ICU) "
      "and the actual locale is \"root\" (the collation data comes unmodified from the UCA) The locale is considered "
      "supported by ICU if there is a core ICU bundle for that locale (although it may be empty).")
      .value("ULOC_ACTUAL_LOCALE", ULOC_ACTUAL_LOCALE, "This is locale the data actually comes from.")
      .value("ULOC_VALID_LOCALE", ULOC_VALID_LOCALE, "This is the most specific locale supported by ICU.")
#ifndef U_HIDE_DEPRECATED_API
      .value("ULOC_REQUESTED_LOCALE", ULOC_REQUESTED_LOCALE, "**Deprecated:** ICU 2.8")
      .value("ULOC_DATA_LOCALE_TYPE_LIMIT", ULOC_DATA_LOCALE_TYPE_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
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

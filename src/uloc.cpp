#include "main.hpp"
#include <unicode/uloc.h>

void init_uloc(py::module &m) {
  py::enum_<UAcceptResult>(m, "UAcceptResult", py::arithmetic())
      .value("ULOC_ACCEPT_FAILED", ULOC_ACCEPT_FAILED)
      .value("ULOC_ACCEPT_VALID", ULOC_ACCEPT_VALID)
      .value("ULOC_ACCEPT_FALLBACK", ULOC_ACCEPT_FALLBACK)
      .export_values();

  py::enum_<ULayoutType>(m, "ULayoutType", py::arithmetic())
      .value("ULOC_LAYOUT_LTR", ULOC_LAYOUT_LTR)
      .value("ULOC_LAYOUT_RTL", ULOC_LAYOUT_RTL)
      .value("ULOC_LAYOUT_TTB", ULOC_LAYOUT_TTB)
      .value("ULOC_LAYOUT_BTT", ULOC_LAYOUT_BTT)
      .value("ULOC_LAYOUT_UNKNOWN", ULOC_LAYOUT_UNKNOWN)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  py::enum_<ULocAvailableType>(m, "ULocAvailableType", py::arithmetic())
      .value("ULOC_AVAILABLE_DEFAULT", ULOC_AVAILABLE_DEFAULT)
      .value("ULOC_AVAILABLE_ONLY_LEGACY_ALIASES", ULOC_AVAILABLE_ONLY_LEGACY_ALIASES)
      .value("ULOC_AVAILABLE_WITH_LEGACY_ALIASES", ULOC_AVAILABLE_WITH_LEGACY_ALIASES)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

  py::enum_<ULocDataLocaleType>(m, "ULocDataLocaleType", py::arithmetic())
      .value("ULOC_ACTUAL_LOCALE", ULOC_ACTUAL_LOCALE)
      .value("ULOC_VALID_LOCALE", ULOC_VALID_LOCALE)
#ifndef U_HIDE_DEPRECATED_API
      .value("ULOC_REQUESTED_LOCALE", ULOC_REQUESTED_LOCALE)
      .value("ULOC_DATA_LOCALE_TYPE_LIMIT", ULOC_DATA_LOCALE_TYPE_LIMIT)
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

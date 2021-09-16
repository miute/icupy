#include "main.hpp"
#include <unicode/ucurr.h>

void init_ucurr(py::module &m) {
  py::enum_<UCurrCurrencyType>(m, "UCurrCurrencyType", py::arithmetic())
      .value("UCURR_ALL", UCURR_ALL)
      .value("UCURR_COMMON", UCURR_COMMON)
      .value("UCURR_UNCOMMON", UCURR_UNCOMMON)
      .value("UCURR_DEPRECATED", UCURR_DEPRECATED)
      .value("UCURR_NON_DEPRECATED", UCURR_NON_DEPRECATED)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  py::enum_<UCurrencyUsage>(m, "UCurrencyUsage", py::arithmetic())
      .value("UCURR_USAGE_STANDARD", UCURR_USAGE_STANDARD)
      .value("UCURR_USAGE_CASH", UCURR_USAGE_CASH)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  py::enum_<UCurrNameStyle>(m, "UCurrNameStyle", py::arithmetic())
      .value("UCURR_SYMBOL_NAME", UCURR_SYMBOL_NAME)
      .value("UCURR_LONG_NAME", UCURR_LONG_NAME)
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("UCURR_NARROW_SYMBOL_NAME", UCURR_NARROW_SYMBOL_NAME)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UCURR_FORMAL_SYMBOL_NAME", UCURR_FORMAL_SYMBOL_NAME)
      .value("UCURR_VARIANT_SYMBOL_NAME", UCURR_VARIANT_SYMBOL_NAME)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .export_values();
}

#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
#include <unicode/unumberrangeformatter.h>
#else
#include <unicode/numberrangeformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_unumberrangeformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UNumberRangeCollapse>(m, "UNumberRangeCollapse", py::arithmetic())
      .value("UNUM_RANGE_COLLAPSE_AUTO", UNUM_RANGE_COLLAPSE_AUTO)
      .value("UNUM_RANGE_COLLAPSE_NONE", UNUM_RANGE_COLLAPSE_NONE)
      .value("UNUM_RANGE_COLLAPSE_UNIT", UNUM_RANGE_COLLAPSE_UNIT)
      .value("UNUM_RANGE_COLLAPSE_ALL", UNUM_RANGE_COLLAPSE_ALL)
      .export_values();

  py::enum_<UNumberRangeIdentityFallback>(m, "UNumberRangeIdentityFallback", py::arithmetic())
      .value("UNUM_IDENTITY_FALLBACK_SINGLE_VALUE", UNUM_IDENTITY_FALLBACK_SINGLE_VALUE)
      .value("UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE",
             UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE)
      .value("UNUM_IDENTITY_FALLBACK_APPROXIMATELY", UNUM_IDENTITY_FALLBACK_APPROXIMATELY)
      .value("UNUM_IDENTITY_FALLBACK_RANGE", UNUM_IDENTITY_FALLBACK_RANGE)
      .export_values();

  py::enum_<UNumberRangeIdentityResult>(m, "UNumberRangeIdentityResult", py::arithmetic())
      .value("UNUM_IDENTITY_RESULT_EQUAL_BEFORE_ROUNDING", UNUM_IDENTITY_RESULT_EQUAL_BEFORE_ROUNDING)
      .value("UNUM_IDENTITY_RESULT_EQUAL_AFTER_ROUNDING", UNUM_IDENTITY_RESULT_EQUAL_AFTER_ROUNDING)
      .value("UNUM_IDENTITY_RESULT_NOT_EQUAL", UNUM_IDENTITY_RESULT_NOT_EQUAL)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

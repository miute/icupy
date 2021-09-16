#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
#include <unicode/unumberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

void init_unumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  py::enum_<UNumberDecimalSeparatorDisplay>(m, "UNumberDecimalSeparatorDisplay", py::arithmetic())
      .value("UNUM_DECIMAL_SEPARATOR_AUTO", UNUM_DECIMAL_SEPARATOR_AUTO)
      .value("UNUM_DECIMAL_SEPARATOR_ALWAYS", UNUM_DECIMAL_SEPARATOR_ALWAYS)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UNumberGroupingStrategy>(m, "UNumberGroupingStrategy", py::arithmetic())
      .value("UNUM_GROUPING_OFF", UNUM_GROUPING_OFF)
      .value("UNUM_GROUPING_MIN2", UNUM_GROUPING_MIN2)
      .value("UNUM_GROUPING_AUTO", UNUM_GROUPING_AUTO)
      .value("UNUM_GROUPING_ON_ALIGNED", UNUM_GROUPING_ON_ALIGNED)
      .value("UNUM_GROUPING_THOUSANDS", UNUM_GROUPING_THOUSANDS)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  py::enum_<UNumberRoundingPriority>(m, "UNumberRoundingPriority", py::arithmetic())
      .value("UNUM_ROUNDING_PRIORITY_RELAXED", UNUM_ROUNDING_PRIORITY_RELAXED)
      .value("UNUM_ROUNDING_PRIORITY_STRICT", UNUM_ROUNDING_PRIORITY_STRICT)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  py::enum_<UNumberSignDisplay>(m, "UNumberSignDisplay", py::arithmetic())
      .value("UNUM_SIGN_AUTO", UNUM_SIGN_AUTO)
      .value("UNUM_SIGN_ALWAYS", UNUM_SIGN_ALWAYS)
      .value("UNUM_SIGN_NEVER", UNUM_SIGN_NEVER)
      .value("UNUM_SIGN_ACCOUNTING", UNUM_SIGN_ACCOUNTING)
      .value("UNUM_SIGN_ACCOUNTING_ALWAYS", UNUM_SIGN_ACCOUNTING_ALWAYS)
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("UNUM_SIGN_EXCEPT_ZERO", UNUM_SIGN_EXCEPT_ZERO)
      .value("UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO", UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UNUM_SIGN_NEGATIVE", UNUM_SIGN_NEGATIVE)
      .value("UNUM_SIGN_ACCOUNTING_NEGATIVE", UNUM_SIGN_ACCOUNTING_NEGATIVE)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  py::enum_<UNumberTrailingZeroDisplay>(m, "UNumberTrailingZeroDisplay", py::arithmetic())
      .value("UNUM_TRAILING_ZERO_AUTO", UNUM_TRAILING_ZERO_AUTO)
      .value("UNUM_TRAILING_ZERO_HIDE_IF_WHOLE", UNUM_TRAILING_ZERO_HIDE_IF_WHOLE)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  py::enum_<UNumberUnitWidth>(m, "UNumberUnitWidth", py::arithmetic())
      .value("UNUM_UNIT_WIDTH_NARROW", UNUM_UNIT_WIDTH_NARROW)
      .value("UNUM_UNIT_WIDTH_SHORT", UNUM_UNIT_WIDTH_SHORT)
      .value("UNUM_UNIT_WIDTH_FULL_NAME", UNUM_UNIT_WIDTH_FULL_NAME)
      .value("UNUM_UNIT_WIDTH_ISO_CODE", UNUM_UNIT_WIDTH_ISO_CODE)
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UNUM_UNIT_WIDTH_FORMAL", UNUM_UNIT_WIDTH_FORMAL)
      .value("UNUM_UNIT_WIDTH_VARIANT", UNUM_UNIT_WIDTH_VARIANT)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UNUM_UNIT_WIDTH_HIDDEN", UNUM_UNIT_WIDTH_HIDDEN)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
}

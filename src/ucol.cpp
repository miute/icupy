#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/ucol.h>

void init_ucol(py::module &m) {
  // UCollationStrength

  py::enum_<UColAttribute>(m, "UColAttribute", py::arithmetic())
      .value("UCOL_FRENCH_COLLATION", UCOL_FRENCH_COLLATION)
      .value("UCOL_ALTERNATE_HANDLING", UCOL_ALTERNATE_HANDLING)
      .value("UCOL_CASE_FIRST", UCOL_CASE_FIRST)
      .value("UCOL_CASE_LEVEL", UCOL_CASE_LEVEL)
      .value("UCOL_NORMALIZATION_MODE", UCOL_NORMALIZATION_MODE)
      .value("UCOL_DECOMPOSITION_MODE", UCOL_DECOMPOSITION_MODE)
      .value("UCOL_STRENGTH", UCOL_STRENGTH)
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_HIRAGANA_QUATERNARY_MODE", UCOL_HIRAGANA_QUATERNARY_MODE)
#endif // U_HIDE_DEPRECATED_API
      .value("UCOL_NUMERIC_COLLATION", UCOL_NUMERIC_COLLATION)
#ifndef U_FORCE_HIDE_DEPRECATED_API
      .value("UCOL_ATTRIBUTE_COUNT", UCOL_ATTRIBUTE_COUNT)
#endif // U_FORCE_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UColAttributeValue>(m, "UColAttributeValue", py::arithmetic())
      .value("UCOL_DEFAULT", UCOL_DEFAULT)
      .value("UCOL_PRIMARY", UCOL_PRIMARY)
      .value("UCOL_SECONDARY", UCOL_SECONDARY)
      .value("UCOL_TERTIARY", UCOL_TERTIARY)
      .value("UCOL_DEFAULT_STRENGTH", UCOL_DEFAULT_STRENGTH)
      .value("UCOL_CE_STRENGTH_LIMIT", UCOL_CE_STRENGTH_LIMIT)
      .value("UCOL_QUATERNARY", UCOL_QUATERNARY)
      .value("UCOL_IDENTICAL", UCOL_IDENTICAL)
      .value("UCOL_STRENGTH_LIMIT", UCOL_STRENGTH_LIMIT)
      .value("UCOL_OFF", UCOL_OFF)
      .value("UCOL_ON", UCOL_ON)
      .value("UCOL_SHIFTED", UCOL_SHIFTED)
      .value("UCOL_NON_IGNORABLE", UCOL_NON_IGNORABLE)
      .value("UCOL_LOWER_FIRST", UCOL_LOWER_FIRST)
      .value("UCOL_UPPER_FIRST", UCOL_UPPER_FIRST)
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_ATTRIBUTE_VALUE_COUNT", UCOL_ATTRIBUTE_VALUE_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UColBoundMode>(m, "UColBoundMode", py::arithmetic())
      .value("UCOL_BOUND_LOWER", UCOL_BOUND_LOWER)
      .value("UCOL_BOUND_UPPER", UCOL_BOUND_UPPER)
      .value("UCOL_BOUND_UPPER_LONG", UCOL_BOUND_UPPER_LONG)
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_BOUND_VALUE_COUNT", UCOL_BOUND_VALUE_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UCollationResult>(m, "UCollationResult", py::arithmetic())
      .value("UCOL_EQUAL", UCOL_EQUAL)
      .value("UCOL_GREATER", UCOL_GREATER)
      .value("UCOL_LESS", UCOL_LESS)
      .export_values();

  py::enum_<UColReorderCode>(m, "UColReorderCode", py::arithmetic())
      .value("UCOL_REORDER_CODE_DEFAULT", UCOL_REORDER_CODE_DEFAULT)
      .value("UCOL_REORDER_CODE_NONE", UCOL_REORDER_CODE_NONE)
      .value("UCOL_REORDER_CODE_OTHERS", UCOL_REORDER_CODE_OTHERS)
      .value("UCOL_REORDER_CODE_SPACE", UCOL_REORDER_CODE_SPACE)
      .value("UCOL_REORDER_CODE_FIRST", UCOL_REORDER_CODE_FIRST)
      .value("UCOL_REORDER_CODE_PUNCTUATION", UCOL_REORDER_CODE_PUNCTUATION)
      .value("UCOL_REORDER_CODE_SYMBOL", UCOL_REORDER_CODE_SYMBOL)
      .value("UCOL_REORDER_CODE_CURRENCY", UCOL_REORDER_CODE_CURRENCY)
      .value("UCOL_REORDER_CODE_DIGIT", UCOL_REORDER_CODE_DIGIT)
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_REORDER_CODE_LIMIT", UCOL_REORDER_CODE_LIMIT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UColRuleOption>(m, "UColRuleOption", py::arithmetic())
      .value("UCOL_TAILORING_ONLY", UCOL_TAILORING_ONLY)
      .value("UCOL_FULL_RULES", UCOL_FULL_RULES)
      .export_values();
}

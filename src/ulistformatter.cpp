#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include <unicode/ulistformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_ulistformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UListFormatterField>(
      m, "UListFormatterField", py::arithmetic(),
      "*FieldPosition* and *UFieldPosition* selectors for format fields defined by *ListFormatter*.")
      .value("ULISTFMT_LITERAL_FIELD", ULISTFMT_LITERAL_FIELD,
             "The literal text in the result which came from the resources.")
      .value("ULISTFMT_ELEMENT_FIELD", ULISTFMT_ELEMENT_FIELD,
             "The element text in the result which came from the input strings.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  py::enum_<UListFormatterType>(m, "UListFormatterType", py::arithmetic(), "Type of meaning expressed by the list.")
      .value("ULISTFMT_TYPE_AND", ULISTFMT_TYPE_AND,
             "Conjunction formatting, e.g. "
             "\"Alice, Bob, Charlie, and Delta\".")
      .value("ULISTFMT_TYPE_OR", ULISTFMT_TYPE_OR,
             "Disjunction (or alternative, or simply one of) formatting, e.g. "
             "\"Alice, Bob, Charlie, or Delta\".")
      .value("ULISTFMT_TYPE_UNITS", ULISTFMT_TYPE_UNITS,
             "Formatting of a list of values with units, e.g. "
             "\"5 pounds, 12 ounces\".")
      .export_values();

  py::enum_<UListFormatterWidth>(m, "UListFormatterWidth", py::arithmetic(), "Verbosity level of the list patterns.")
      .value("ULISTFMT_WIDTH_WIDE", ULISTFMT_WIDTH_WIDE,
             "Use list formatting with full words (no abbreviations) when possible.")
      .value("ULISTFMT_WIDTH_SHORT", ULISTFMT_WIDTH_SHORT, "Use list formatting of typical length.")
      .value("ULISTFMT_WIDTH_NARROW", ULISTFMT_WIDTH_NARROW, "Use list formatting of the shortest possible length.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
}

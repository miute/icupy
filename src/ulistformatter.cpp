#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include <unicode/ulistformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_ulistformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  //
  // enum UListFormatterField
  //
  py::enum_<UListFormatterField>(m, "UListFormatterField", py::arithmetic(),
                                 R"doc(
:class:`FieldPosition` selector for format fields defined by
:class:`ListFormatter`.
      )doc")
      .value("ULISTFMT_LITERAL_FIELD", ULISTFMT_LITERAL_FIELD, R"doc(
             The literal text in the result which came from the resources.
             )doc")
      .value("ULISTFMT_ELEMENT_FIELD", ULISTFMT_ELEMENT_FIELD, R"doc(
             The element text in the result which came from the input strings.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  //
  // enum UListFormatterType
  //
  py::enum_<UListFormatterType>(m, "UListFormatterType", py::arithmetic(),
                                "Type of meaning expressed by the list.")
      .value("ULISTFMT_TYPE_AND", ULISTFMT_TYPE_AND, R"doc(
             Conjunction formatting, e.g. "Alice, Bob, Charlie, and Delta".
             )doc")
      .value("ULISTFMT_TYPE_OR", ULISTFMT_TYPE_OR, R"doc(
             Disjunction (or alternative, or simply one of) formatting, e.g.
             "Alice, Bob, Charlie, or Delta".
             )doc")
      .value("ULISTFMT_TYPE_UNITS", ULISTFMT_TYPE_UNITS, R"doc(
             Formatting of a list of values with units, e.g.
             "5 pounds, 12 ounces".
             )doc")
      .export_values();

  //
  // enum UListFormatterWidth
  //
  py::enum_<UListFormatterWidth>(m, "UListFormatterWidth", py::arithmetic(),
                                 "Verbosity level of the list patterns.")
      .value("ULISTFMT_WIDTH_WIDE", ULISTFMT_WIDTH_WIDE, R"doc(
             Use list formatting with full words (no abbreviations) when
             possible.
             )doc")
      .value("ULISTFMT_WIDTH_SHORT", ULISTFMT_WIDTH_SHORT, R"doc(
             Use list formatting of typical length.
             )doc")
      .value("ULISTFMT_WIDTH_NARROW", ULISTFMT_WIDTH_NARROW, R"doc(
             Use list formatting of the shortest possible length.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include <pybind11/native_enum.h>
#include <unicode/ulistformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_ulistformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  //
  // enum UListFormatterField
  //
  py::native_enum<UListFormatterField>(m, "UListFormatterField", "enum.IntEnum",
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
      .export_values()
      .finalize();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  //
  // enum UListFormatterType
  //
  py::native_enum<UListFormatterType>(m, "UListFormatterType", "enum.IntEnum",
                                      R"doc(
Type of meaning expressed by the list.
      )doc")
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
      .export_values()
      .finalize();

  //
  // enum UListFormatterWidth
  //
  py::native_enum<UListFormatterWidth>(m, "UListFormatterWidth", "enum.IntEnum",
                                       R"doc(
Verbosity level of the list patterns.
      )doc")
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
      .export_values()
      .finalize();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include <unicode/ulistformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_ulistformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UListFormatterField>(m, "UListFormatterField", py::arithmetic())
      .value("ULISTFMT_LITERAL_FIELD", ULISTFMT_LITERAL_FIELD)
      .value("ULISTFMT_ELEMENT_FIELD", ULISTFMT_ELEMENT_FIELD)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  py::enum_<UListFormatterType>(m, "UListFormatterType", py::arithmetic())
      .value("ULISTFMT_TYPE_AND", ULISTFMT_TYPE_AND)
      .value("ULISTFMT_TYPE_OR", ULISTFMT_TYPE_OR)
      .value("ULISTFMT_TYPE_UNITS", ULISTFMT_TYPE_UNITS)
      .export_values();

  py::enum_<UListFormatterWidth>(m, "UListFormatterWidth", py::arithmetic())
      .value("ULISTFMT_WIDTH_WIDE", ULISTFMT_WIDTH_WIDE)
      .value("ULISTFMT_WIDTH_SHORT", ULISTFMT_WIDTH_SHORT)
      .value("ULISTFMT_WIDTH_NARROW", ULISTFMT_WIDTH_NARROW)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
}

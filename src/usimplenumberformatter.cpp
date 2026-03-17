#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
#include <unicode/usimplenumberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

void init_usimplenumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  //
  // enum USimpleNumberSign
  //
  py::enum_<USimpleNumberSign>(m, "USimpleNumberSign", py::arithmetic(), R"doc(
Explicit sign option for a :class:`~icupy.icu.number.SimpleNumber`.
      )doc")
      .value("UNUM_SIMPLE_NUMBER_PLUS_SIGN", UNUM_SIMPLE_NUMBER_PLUS_SIGN,
             R"doc(
             Render a plus sign.
             )doc")
      .value("UNUM_SIMPLE_NUMBER_NO_SIGN", UNUM_SIMPLE_NUMBER_NO_SIGN, R"doc(
             Render no sign.
             )doc")
      .value("UNUM_SIMPLE_NUMBER_MINUS_SIGN", UNUM_SIMPLE_NUMBER_MINUS_SIGN,
             R"doc(
             Render a minus sign.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)
}

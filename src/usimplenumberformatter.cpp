#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
#include <unicode/usimplenumberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

void init_usimplenumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  //
  // USimpleNumberSign
  //
  py::enum_<USimpleNumberSign>(m, "USimpleNumberSign", py::arithmetic(), "An explicit sign option for a SimpleNumber.")
      .value("UNUM_SIMPLE_NUMBER_PLUS_SIGN", UNUM_SIMPLE_NUMBER_PLUS_SIGN, "Render a plus sign.")
      .value("UNUM_SIMPLE_NUMBER_NO_SIGN", UNUM_SIMPLE_NUMBER_NO_SIGN, "Render no sign.")
      .value("UNUM_SIMPLE_NUMBER_MINUS_SIGN", UNUM_SIMPLE_NUMBER_MINUS_SIGN, "Render a minus sign.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)
}

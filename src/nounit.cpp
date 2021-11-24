#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
#include <unicode/nounit.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

using namespace icu;

void init_nounit(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  // icu::NoUnit
  auto m2 = m.def_submodule("NoUnit");

  m2.def("base", &NoUnit::base);
  m2.def("percent", &NoUnit::percent);
  m2.def("permille", &NoUnit::permille);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
}

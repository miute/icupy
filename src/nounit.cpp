#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
#include <unicode/nounit.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

using namespace icu;

void init_nounit(py::module & /* m */, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  //
  // namespace icu::NoUnit
  //
  m2.def("base", &NoUnit::base, R"doc(
      Return the base unit (dimensionless and no scaling).
      )doc");

  m2.def("percent", &NoUnit::percent, R"doc(
      Return the unit of concentration: percent.
      )doc");

  m2.def("permille", &NoUnit::permille, R"doc(
      Return the unit of concentration: permille or 1/1000 of a base unit.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
}

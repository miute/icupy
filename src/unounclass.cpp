#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM == 71)
#include <unicode/unounclass.h>
#endif // (U_ICU_VERSION_MAJOR_NUM == 71)

using namespace icu;

void init_unounclass(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM == 71)
  //
  // icu::NounClass
  //
  py::enum_<NounClass>(m, "NounClass", py::arithmetic(),
                       "Represents all the grammatical noun classes that are supported by CLDR.")
      .value("OTHER", OTHER)
      .value("NEUTER", NEUTER)
      .value("FEMININE", FEMININE)
      .value("MASCULINE", MASCULINE)
      .value("ANIMATE", ANIMATE)
      .value("INANIMATE", INANIMATE)
      .value("PERSONAL", PERSONAL)
      .value("COMMON", COMMON)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM == 71)
}

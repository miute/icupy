#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM == 71)
#include <pybind11/native_enum.h>
#include <unicode/unounclass.h>
#endif // (U_ICU_VERSION_MAJOR_NUM == 71)

using namespace icu;

void init_unounclass(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM == 71)
  //
  // enum icu::NounClass
  //
  py::native_enum<NounClass>(m, "NounClass", "enum.IntEnum", R"doc(
Represent all the grammatical noun classes that are supported by CLDR.
      )doc")
      .value("OTHER", OTHER, "")
      .value("NEUTER", NEUTER, "")
      .value("FEMININE", FEMININE, "")
      .value("MASCULINE", MASCULINE, "")
      .value("ANIMATE", ANIMATE, "")
      .value("INANIMATE", INANIMATE, "")
      .value("PERSONAL", PERSONAL, "")
      .value("COMMON", COMMON, "")
      .export_values()
      .finalize();
#endif // (U_ICU_VERSION_MAJOR_NUM == 71)
}

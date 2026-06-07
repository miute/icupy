#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/native_enum.h>
#include <unicode/ugender.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

void init_ugender(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // enum UGender
  //
  py::native_enum<UGender>(m, "UGender", "enum.IntEnum", R"doc(
Genders.
      )doc")
      .value("UGENDER_MALE", UGENDER_MALE, R"doc(
             Male gender.
             )doc")
      .value("UGENDER_FEMALE", UGENDER_FEMALE, R"doc(
             Female gender.
             )doc")
      .value("UGENDER_OTHER", UGENDER_OTHER, R"doc(
             Neutral gender.
             )doc")
      .export_values()
      .finalize();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

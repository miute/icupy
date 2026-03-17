#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <unicode/ugender.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

void init_ugender(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // enum UGender
  //
  py::enum_<UGender>(m, "UGender", "Genders.")
      .value("UGENDER_MALE", UGENDER_MALE, R"doc(
             Male gender.
             )doc")
      .value("UGENDER_FEMALE", UGENDER_FEMALE, R"doc(
             Female gender.
             )doc")
      .value("UGENDER_OTHER", UGENDER_OTHER, R"doc(
             Neutral gender.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

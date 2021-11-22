#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <unicode/ugender.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

void init_ugender(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  py::enum_<UGender>(m, "UGender", "Genders.")
      .value("UGENDER_MALE", UGENDER_MALE, "Male gender.")
      .value("UGENDER_FEMALE", UGENDER_FEMALE, "Female gender.")
      .value("UGENDER_OTHER", UGENDER_OTHER, "Neutral gender.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

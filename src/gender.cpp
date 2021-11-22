#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/stl.h>
#include <unicode/errorcode.h>
#include <unicode/gender.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_gender(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  // icu::GenderInfo
  py::class_<GenderInfo, UObject> gi(m, "GenderInfo");
  gi.def_static(
        "get_instance",
        [](const Locale &locale) {
          ErrorCode error_code;
          auto result = GenderInfo::getInstance(locale, error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::return_value_policy::reference, py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "get_instance",
          [](const char *locale) {
            ErrorCode error_code;
            auto result = GenderInfo::getInstance(locale, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("locale"));
  gi.def(
      "get_list_gender",
      [](const GenderInfo &self, const std::vector<UGender> &genders, int32_t length) {
        if (length == -1) {
          length = static_cast<int32_t>(genders.size());
        }
        ErrorCode error_code;
        auto result = self.getListGender(genders.data(), length, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("genders"), py::arg("length") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

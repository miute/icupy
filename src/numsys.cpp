#include "main.hpp"
#include <unicode/numsys.h>
#include <unicode/strenum.h>

using namespace icu;

void init_numsys(py::module &m) {
  // icu::NumberingSystem
  py::class_<NumberingSystem, UObject> ns(m, "NumberingSystem");
  ns.def(py::init<const NumberingSystem &>(), py::arg("other"));
  ns.def_static(
        // [1] NumberingSystem::createInstance
        "create_instance",
        [](const Locale &locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberingSystem::createInstance(locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberingSystem::createInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def_static(
          // [2] NumberingSystem::createInstance
          "create_instance",
          [](int32_t radix, UBool is_algorithmic, const UnicodeString &description) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberingSystem::createInstance(radix, is_algorithmic, description, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("radix"), py::arg("is_algorithmic"), py::arg("description"))
      .def_static(
          // const char16_t *description -> const UnicodeString &description
          "create_instance",
          [](int32_t radix, UBool is_algorithmic, const char16_t *description) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberingSystem::createInstance(radix, is_algorithmic, description, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("radix"), py::arg("is_algorithmic"), py::arg("description"))
      .def_static(
          // [3] NumberingSystem::createInstance
          "create_instance", []() {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberingSystem::createInstance(error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          });
  ns.def_static(
      "create_instance_by_name",
      [](const char *name) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = NumberingSystem::createInstanceByName(name, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("name"));
  ns.def_static("get_available_names", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = NumberingSystem::getAvailableNames(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ns.def("get_description", &NumberingSystem::getDescription);
  ns.def("get_name", &NumberingSystem::getName);
  ns.def("get_radix", &NumberingSystem::getRadix);
  ns.def("is_algorithmic", &NumberingSystem::isAlgorithmic);
}

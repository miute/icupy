#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/numsys.h>
#include <unicode/strenum.h>

using namespace icu;

void init_numsys(py::module &m) {
  //
  // icu::NumberingSystem
  //
  py::class_<NumberingSystem, UObject> ns(m, "NumberingSystem");

  ns.def(py::init<const NumberingSystem &>(), py::arg("other"));

  ns.def_static(
        // [1] NumberingSystem::createInstance
        "create_instance",
        [](const _LocaleVariant &locale) {
          ErrorCode error_code;
          auto result = NumberingSystem::createInstance(VARIANT_TO_LOCALE(locale), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
      .def_static(
          // [2] NumberingSystem::createInstance
          "create_instance",
          [](int32_t radix, UBool is_algorithmic, const _UnicodeStringVariant &description) {
            ErrorCode error_code;
            auto result =
                NumberingSystem::createInstance(radix, is_algorithmic, VARIANT_TO_UNISTR(description), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("radix"), py::arg("is_algorithmic"), py::arg("description"))
      .def_static(
          // [3] NumberingSystem::createInstance
          "create_instance", []() {
            ErrorCode error_code;
            auto result = NumberingSystem::createInstance(error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          });

  ns.def_static(
      "create_instance_by_name",
      [](const char *name) {
        ErrorCode error_code;
        auto result = NumberingSystem::createInstanceByName(name, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("name"));

  ns.def_static("get_available_names", []() {
    ErrorCode error_code;
    auto result = NumberingSystem::getAvailableNames(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });

  ns.def("get_description", &NumberingSystem::getDescription);

  ns.def("get_name", &NumberingSystem::getName);

  ns.def("get_radix", &NumberingSystem::getRadix);

  ns.def("is_algorithmic", &NumberingSystem::isAlgorithmic);
}

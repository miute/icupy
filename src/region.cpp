#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
#include <pybind11/operators.h>
#include <unicode/region.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

using namespace icu;

void init_region(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  //
  // icu::Region
  //
  py::class_<Region, UObject> reg(m, "Region");

  reg.def(
      "__contains__", [](const Region &self, const Region &item) -> py::bool_ { return self.contains(item); },
      py::arg("item"));

  reg.def(
      "__eq__", [](const Region &self, const Region &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  reg.def(
      "__ne__", [](const Region &self, const Region &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  reg.def(
      "contains", [](const Region &self, const Region &other) -> py::bool_ { return self.contains(other); },
      py::arg("other"));

#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  reg.def_static(
      "get_available",
      [](URegionType type) {
        ErrorCode error_code;
        auto result = Region::getAvailable(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));

  reg.def("get_contained_regions",
          [](const Region &self) {
            ErrorCode error_code;
            auto result = self.getContainedRegions(error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          })
      .def(
          "get_contained_regions",
          [](const Region &self, URegionType type) {
            ErrorCode error_code;
            auto result = self.getContainedRegions(type, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("type_"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)

  reg.def("get_containing_region", py::overload_cast<>(&Region::getContainingRegion, py::const_),
          py::return_value_policy::reference)
      .def("get_containing_region", py::overload_cast<URegionType>(&Region::getContainingRegion, py::const_),
           py::return_value_policy::reference, py::arg("type_"));

  reg.def_static(
         "get_instance",
         [](const char *region_code) {
           ErrorCode error_code;
           auto result = Region::getInstance(region_code, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::return_value_policy::reference, py::arg("region_code"))
      .def_static(
          "get_instance",
          [](int32_t code) {
            ErrorCode error_code;
            auto result = Region::getInstance(code, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("code"));

  reg.def("get_numeric_code", &Region::getNumericCode);

#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  reg.def("get_preferred_values", [](const Region &self) {
    ErrorCode error_code;
    auto result = self.getPreferredValues(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)

  reg.def("get_region_code", &Region::getRegionCode);

  reg.def("get_type", &Region::getType);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/strenum.h>

using namespace icu;

class PyStringEnumeration : StringEnumeration {
public:
  int32_t count(UErrorCode &status) const override {
    PYBIND11_OVERRIDE_PURE(int32_t, StringEnumeration, count, status);
  }

  void reset(UErrorCode &status) override { PYBIND11_OVERRIDE_PURE(void, StringEnumeration, reset, status); }

protected:
  using StringEnumeration::StringEnumeration;
};

void init_strenum(py::module &m) {
  py::class_<StringEnumeration, UObject, PyStringEnumeration> se(m, "StringEnumeration");
  se.def("__copy__", &StringEnumeration::clone)
      .def(
          "__deepcopy__", [](const StringEnumeration &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const StringEnumeration &self, const StringEnumeration &other) { return self == other; },
          py::is_operator(), py::arg("other"))
      .def("__iter__",
           [](StringEnumeration &self) -> StringEnumeration & {
             ErrorCode error_code;
             self.reset(error_code);
             if (error_code.isFailure()) {
               throw ICUError(error_code);
             }
             return self;
           })
      .def("__len__",
           [](const StringEnumeration &self) {
             ErrorCode error_code;
             auto result = self.count(error_code);
             if (error_code.isFailure()) {
               throw ICUError(error_code);
             }
             return result;
           })
      .def(
          "__ne__", [](const StringEnumeration &self, const StringEnumeration &other) { return self != other; },
          py::is_operator(), py::arg("other"))
      .def("__next__", [](StringEnumeration &self) {
        ErrorCode error_code;
        auto result = self.next(nullptr, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        } else if (result == nullptr) {
          throw py::stop_iteration();
        }
        return result;
      });
  se.def("clone", &StringEnumeration::clone);
  se.def("count", [](const StringEnumeration &self) {
    ErrorCode error_code;
    auto result = self.count(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  se.def("next", [](StringEnumeration &self) {
    ErrorCode error_code;
    auto result = self.next(nullptr, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  se.def("reset", [](StringEnumeration &self) {
    ErrorCode error_code;
    self.reset(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
  });
  se.def(
      "snext",
      [](StringEnumeration &self) {
        ErrorCode error_code;
        auto result = self.snext(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  se.def("unext", [](StringEnumeration &self) {
    ErrorCode error_code;
    auto result = self.unext(nullptr, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
}

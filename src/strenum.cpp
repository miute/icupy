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
  py::class_<StringEnumeration, PyStringEnumeration> se(m, "StringEnumeration");
  // se.def(py::self != py::self).def(py::self == py::self);
  se.def("__copy__", &StringEnumeration::clone)
      .def("__deepcopy__", [](const StringEnumeration &self, py::dict) { return self.clone(); })
      .def(
          "__eq__", [](const StringEnumeration &self, const StringEnumeration &item) { return self == item; },
          py::is_operator(), py::arg("item"))
      .def("__iter__",
           [](StringEnumeration &self) -> StringEnumeration & {
             UErrorCode error_code = U_ZERO_ERROR;
             self.reset(error_code);
             if (U_FAILURE(error_code)) {
               throw ICUException(error_code);
             }
             return self;
           })
      .def("__len__",
           [](const StringEnumeration &self) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = self.count(error_code);
             if (U_FAILURE(error_code)) {
               throw ICUException(error_code);
             }
             return result;
           })
      .def(
          "__ne__", [](const StringEnumeration &self, const StringEnumeration &item) { return self != item; },
          py::is_operator(), py::arg("item"))
      .def("__next__", [](StringEnumeration &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.next(NULL, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        } else if (result == NULL) {
          throw py::stop_iteration();
        }
        return result;
      });
  se.def("clone", &StringEnumeration::clone);
  se.def("count", [](const StringEnumeration &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.count(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
    return result;
  });
  se.def("next", [](StringEnumeration &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.next(NULL, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
    return result;
  });
  se.def("reset", [](StringEnumeration &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    self.reset(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
  });
  se.def(
      "snext",
      [](StringEnumeration &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.snext(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  se.def("unext", [](StringEnumeration &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.unext(NULL, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
    return result;
  });
}

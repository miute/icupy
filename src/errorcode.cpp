#include "main.hpp"
#include <unicode/errorcode.h>

using namespace icu;

void init_errorcode(py::module &m) {
  // icu::ErrorCode
  py::class_<ErrorCode, UMemory> ec(m, "ErrorCode");
  ec.def(py::init<>());
  ec.def("__eq__", [](const ErrorCode &self, UErrorCode error_code) { return self.get() == error_code; });
  ec.def_property_readonly("error_name", &ErrorCode::errorName);
  ec.def("get", &ErrorCode::get);
  ec.def("is_failure", &ErrorCode::isFailure);
  ec.def("is_success", &ErrorCode::isSuccess);
  ec.def("reset", &ErrorCode::reset);
  ec.def("set", &ErrorCode::set, py::arg("value"));
}

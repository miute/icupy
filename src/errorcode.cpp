#include "main.hpp"
#include <sstream>

using namespace icu;

void init_errorcode(py::module &m) {
  //
  // icu::ErrorCode
  //
  py::class_<ErrorCode, UMemory> ec(m, "ErrorCode");

  ec.def(py::init<>());

  ec.def(
      "__eq__", [](const ErrorCode &self, UErrorCode error_code) { return self.get() == error_code; },
      py::is_operator(), py::arg("other"));

  ec.def("__repr__", [](const ErrorCode &self) {
    std::stringstream ss;
    ss << "<ErrorCode(<";
    ss << self.errorName();
    ss << ": " << self.get();
    ss << ">)>";
    return ss.str();
  });

  ec.def("__str__", &ErrorCode::errorName);

  ec.def_property_readonly("error_name", &ErrorCode::errorName);

  ec.def("get", &ErrorCode::get);

  ec.def("is_failure", [](const ErrorCode &self) -> py::bool_ { return self.isFailure(); });

  ec.def("is_success", [](const ErrorCode &self) -> py::bool_ { return self.isSuccess(); });

  ec.def("reset", &ErrorCode::reset);

  ec.def("set", &ErrorCode::set, py::arg("value"));
}

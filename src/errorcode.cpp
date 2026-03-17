#include "main.hpp"
#include <sstream>

using namespace icu;

void init_errorcode(py::module &m) {
  //
  // class icu::ErrorCode
  //
  py::class_<ErrorCode, UMemory> ec(m, "ErrorCode", R"doc(
      Wrapper class for :class:`UErrorCode`.
      )doc");

  ec.def(py::init<>(), R"doc(
      Initialize the ``ErrorCode`` instance to
      :attr:`~UErrorCode.U_ZERO_ERROR`.
      )doc");

  ec.def(
      "__eq__",
      [](const ErrorCode &self, UErrorCode error_code) {
        return self.get() == error_code;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  ec.def("__repr__", [](const ErrorCode &self) {
    std::stringstream ss;
    ss << "<ErrorCode(<";
    ss << self.errorName();
    ss << ": " << self.get();
    ss << ">)>";
    return ss.str();
  });

  ec.def("__str__", &ErrorCode::errorName, R"doc(
      Return a string representation of the ``UErrorCode``.
      )doc");

  ec.def_property_readonly("error_name", &ErrorCode::errorName, R"doc(
      str: The name of the ``UErrorCode``, e.g. "U_ZERO_ERROR".
      )doc");

  ec.def("get", &ErrorCode::get, R"doc(
      Return the ``UErrorCode``.
      )doc");

  ec.def(
      "is_failure",
      [](const ErrorCode &self) -> py::bool_ { return self.isFailure(); },
      R"doc(
      Return ``True`` if the ``UErrorCode`` indicates a failure.
      )doc");

  ec.def(
      "is_success",
      [](const ErrorCode &self) -> py::bool_ { return self.isSuccess(); },
      R"doc(
      Return ``True`` if the ``UErrorCode`` indicates success.
      )doc");

  ec.def("reset", &ErrorCode::reset, R"doc(
      Return the ``UErrorCode`` value and reset it to
      :attr:`~UErrorCode.U_ZERO_ERROR`.
      )doc");

  ec.def("set", &ErrorCode::set, py::arg("value"), R"doc(
      Set the ``UErrorCode`` value.
      )doc");
}

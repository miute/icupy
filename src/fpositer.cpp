#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/fpositer.h>

using namespace icu;

void init_fpositer(py::module &m) {
  //
  // icu::FieldPositionIterator
  //
  py::class_<FieldPositionIterator, UObject> fpi(m, "FieldPositionIterator");

  fpi.def(py::init<>()).def(py::init<const FieldPositionIterator &>(), py::arg("other"));

  fpi.def(
      "__eq__", [](const FieldPositionIterator &self, const FieldPositionIterator &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  fpi.def(
      "__ne__", [](const FieldPositionIterator &self, const FieldPositionIterator &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  fpi.def(
      "next", [](FieldPositionIterator &self, FieldPosition &fp) -> py::bool_ { return self.next(fp); }, py::arg("fp"));
}

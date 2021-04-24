#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/fpositer.h>

using namespace icu;

void init_fpositer(py::module &m) {
  // icu::FieldPositionIterator
  py::class_<FieldPositionIterator, UObject> fpi(m, "FieldPositionIterator");
  fpi.def(py::init<>())
      .def(py::init<const FieldPositionIterator &>(), py::arg("rhs"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  fpi.def("next", &FieldPositionIterator::next, py::arg("fp"));
}

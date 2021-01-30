#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/parsepos.h>

using namespace icu;

void init_parsepos(py::module &m) {
  py::class_<ParsePosition> pp(m, "ParsePosition");
  pp.def(py::init<>())
      .def(py::init<int32_t>(), py::arg("new_index"))
      .def(py::init<const ParsePosition &>(), py::arg("copy"))
      .def(py::self != py::self)
      .def(py::self == py::self);
  pp.def("__copy__", &ParsePosition::clone).def("__deepcopy__", [](const ParsePosition &self, py::dict) {
    return self.clone();
  });
  pp.def("clone", &ParsePosition::clone);
  pp.def("get_error_index", &ParsePosition::getErrorIndex);
  pp.def("get_index", &ParsePosition::getIndex);
  pp.def("set_error_index", &ParsePosition::setErrorIndex, py::arg("ei"));
  pp.def("set_index", &ParsePosition::setIndex, py::arg("index"));
}

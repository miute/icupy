#include "main.hpp"
#include <pybind11/operators.h>
#include <sstream>
#include <unicode/parsepos.h>

using namespace icu;

void init_parsepos(py::module &m) {
  //
  // icu::ParsePosition
  //
  py::class_<ParsePosition, UObject> pp(m, "ParsePosition");

  pp.def(py::init<>())
      .def(py::init<int32_t>(), py::arg("new_index"))
      .def(py::init<const ParsePosition &>(), py::arg("other"));

  pp.def("__copy__", &ParsePosition::clone);

  pp.def(
      "__deepcopy__", [](const ParsePosition &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  pp.def(
      "__eq__", [](const ParsePosition &self, const ParsePosition &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  pp.def(
      "__ne__", [](const ParsePosition &self, const ParsePosition &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  pp.def("__repr__", [](const ParsePosition &self) {
    std::stringstream ss;
    ss << "<ParsePosition(";
    ss << "index=" << self.getIndex();
    ss << ", error_index=" << self.getErrorIndex();
    ss << ")>";
    return ss.str();
  });

  pp.def("clone", &ParsePosition::clone);

  pp.def("get_error_index", &ParsePosition::getErrorIndex);

  pp.def("get_index", &ParsePosition::getIndex);

  pp.def("set_error_index", &ParsePosition::setErrorIndex, py::arg("ei"));

  pp.def("set_index", &ParsePosition::setIndex, py::arg("index"));
}

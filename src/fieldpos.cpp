#include "main.hpp"
#include <pybind11/operators.h>
#include <sstream>
#include <unicode/fieldpos.h>

using namespace icu;

void init_fieldpos(py::module &m) {
  // icu::FieldPosition
  py::class_<FieldPosition, UObject> fp(m, "FieldPosition");

  py::enum_<decltype(FieldPosition::DONT_CARE)>(fp, "FieldPosition", py::arithmetic())
      .value("DONT_CARE", FieldPosition::DONT_CARE,
             "*DONT_CARE* may be specified as the field to indicate that the caller doesn't need to specify a field.")
      .export_values();

  fp.def(py::init<>())
      .def(py::init<int32_t>(), py::arg("field"))
      .def(py::init<const FieldPosition &>(), py::arg("copy"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  fp.def("__copy__", &FieldPosition::clone)
      .def(
          "__deepcopy__", [](const FieldPosition &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def("__repr__", [](const FieldPosition &self) {
        std::stringstream ss;
        ss << "FieldPosition(";
        ss << "field=" << self.getField();
        ss << ", begin_index=" << self.getBeginIndex();
        ss << ", end_index=" << self.getEndIndex();
        ss << ")";
        return ss.str();
      });
  fp.def("clone", &FieldPosition::clone);
  fp.def("get_begin_index", &FieldPosition::getBeginIndex);
  fp.def("get_end_index", &FieldPosition::getEndIndex);
  fp.def("get_field", &FieldPosition::getField);
  fp.def("set_begin_index", &FieldPosition::setBeginIndex, py::arg("bi"));
  fp.def("set_end_index", &FieldPosition::setEndIndex, py::arg("ei"));
  fp.def("set_field", &FieldPosition::setField, py::arg("f"));
}

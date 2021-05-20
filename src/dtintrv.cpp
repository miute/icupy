#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/dtintrv.h>

using namespace icu;

void init_dtintrv(py::module &m) {
  // icu::DateInterval
  py::class_<DateInterval, UObject> di(m, "DateInterval");
  di.def(py::init<UDate, UDate>(), py::arg("from_date"), py::arg("to_date"))
      .def(py::init<const DateInterval &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  di.def("__copy__", &DateInterval::clone)
      .def(
          "__deepcopy__", [](const DateInterval &self, py::dict) { return self.clone(); }, py::arg("memo"));
  di.def("clone", &DateInterval::clone);
  di.def("get_from_date", &DateInterval::getFromDate);
  di.def("get_to_date", &DateInterval::getToDate);
}

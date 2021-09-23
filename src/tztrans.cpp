#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/tzrule.h>
#include <unicode/tztrans.h>

using namespace icu;

void init_tztrans(py::module &m) {
  // icu::TimeZoneTransition
  py::class_<TimeZoneTransition, UObject> tzt(m, "TimeZoneTransition");
  tzt.def(py::init<UDate, const TimeZoneRule &, const TimeZoneRule &>(), py::arg("time"), py::arg("from"),
          py::arg("to"))
      .def(py::init<>())
      .def(py::init<const TimeZoneTransition &>(), py::arg("source"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  tzt.def("__copy__", &TimeZoneTransition::clone)
      .def(
          "__deepcopy__", [](const TimeZoneTransition &self, py::dict) { return self.clone(); }, py::arg("memo"));
  // FIXME: Implement "void icu::TimeZoneTransition::adoptFrom(TimeZoneRule *from)".
  // FIXME: Implement "void icu::TimeZoneTransition::adoptTo(TimeZoneRule *to)".
  /*
  tzt.def(
      "adopt_from",
      [](TimeZoneTransition &self, TimeZoneRule *from) { self.adoptFrom(from ? from->clone() : nullptr); },
      py::arg("from"));
  tzt.def(
      "adopt_to", [](TimeZoneTransition &self, TimeZoneRule *to) { self.adoptTo(to ? to->clone() : nullptr); },
      py::arg("to"));
  */
  tzt.def("clone", &TimeZoneTransition::clone);
  tzt.def("get_from", &TimeZoneTransition::getFrom, py::return_value_policy::reference);
  tzt.def("get_time", &TimeZoneTransition::getTime);
  tzt.def("get_to", &TimeZoneTransition::getTo, py::return_value_policy::reference);
  tzt.def("set_from", &TimeZoneTransition::setFrom, py::arg("from"));
  tzt.def("set_time", &TimeZoneTransition::setTime, py::arg("time"));
  tzt.def("set_to", &TimeZoneTransition::setTo, py::arg("to"));
}

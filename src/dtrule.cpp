#include "main.hpp"
#include <pybind11/operators.h>
#include <sstream>
#include <unicode/dtrule.h>

using namespace icu;

void init_dtrule(py::module &m) {
  // icu::DateTimeRule
  py::class_<DateTimeRule, UObject> dtr(m, "DateTimeRule");

  py::enum_<DateTimeRule::DateRuleType>(dtr, "DateRuleType", py::arithmetic())
      .value("DOM", DateTimeRule::DateRuleType::DOM)
      .value("DOW", DateTimeRule::DateRuleType::DOW)
      .value("DOW_GEQ_DOM", DateTimeRule::DateRuleType::DOW_GEQ_DOM)
      .value("DOW_LEQ_DOM", DateTimeRule::DateRuleType::DOW_LEQ_DOM)
      .export_values();

  py::enum_<DateTimeRule::TimeRuleType>(dtr, "TimeRuleType", py::arithmetic())
      .value("WALL_TIME", DateTimeRule::TimeRuleType::WALL_TIME)
      .value("STANDARD_TIME", DateTimeRule::TimeRuleType::STANDARD_TIME)
      .value("UTC_TIME", DateTimeRule::TimeRuleType::UTC_TIME)
      .export_values();

  dtr.def(py::init<int32_t, int32_t, int32_t, DateTimeRule::TimeRuleType>(), py::arg("month"), py::arg("day_of_month"),
          py::arg("millis_in_day"), py::arg("time_type"))
      .def(py::init<int32_t, int32_t, int32_t, int32_t, DateTimeRule::TimeRuleType>(), py::arg("month"),
           py::arg("week_in_month"), py::arg("day_of_week"), py::arg("millis_in_day"), py::arg("time_type"))
      .def(py::init<int32_t, int32_t, int32_t, UBool, int32_t, DateTimeRule::TimeRuleType>(), py::arg("month"),
           py::arg("day_of_month"), py::arg("day_of_week"), py::arg("after"), py::arg("millis_in_day"),
           py::arg("time_type"))
      .def(py::init<const DateTimeRule &>(), py::arg("source"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  dtr.def("__copy__", &DateTimeRule::clone)
      .def(
          "__deepcopy__", [](const DateTimeRule &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def("__repr__", [](const DateTimeRule &self) {
        std::stringstream ss;
        ss << "DateTimeRule(";
        ss << "date_rule_type=" << self.getDateRuleType();
        ss << ", rule_month=" << self.getRuleMonth();
        ss << ", rule_day_of_month=" << self.getRuleDayOfMonth();
        ss << ", rule_week_in_month=" << self.getRuleWeekInMonth();
        ss << ", rule_day_of_week=" << self.getRuleDayOfWeek();
        ss << ", rule_millis_in_day=" << self.getRuleMillisInDay();
        ss << ", time_rule_type=" << self.getTimeRuleType();
        ss << ")";
        return ss.str();
      });
  dtr.def("clone", &DateTimeRule::clone);
  dtr.def("get_date_rule_type", &DateTimeRule::getDateRuleType);
  dtr.def("get_rule_day_of_month", &DateTimeRule::getRuleDayOfMonth);
  dtr.def("get_rule_day_of_week", &DateTimeRule::getRuleDayOfWeek);
  dtr.def("get_rule_millis_in_day", &DateTimeRule::getRuleMillisInDay);
  dtr.def("get_rule_month", &DateTimeRule::getRuleMonth);
  dtr.def("get_rule_week_in_month", &DateTimeRule::getRuleWeekInMonth);
  dtr.def("get_time_rule_type", &DateTimeRule::getTimeRuleType);
}

#include "main.hpp"
#include <iomanip>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/tzrule.h>

using namespace icu;

void init_tzrule(py::module &m) {
  //
  // icu::TimeZoneRule
  //
  py::class_<TimeZoneRule, UObject> tzr(m, "TimeZoneRule");

  tzr.def("__copy__", &TimeZoneRule::clone);

  tzr.def(
      "__deepcopy__", [](const TimeZoneRule &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  tzr.def(
      "__eq__", [](const TimeZoneRule &self, const TimeZoneRule &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  tzr.def(
      "__ne__", [](const TimeZoneRule &self, const TimeZoneRule &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  tzr.def("clone", &TimeZoneRule::clone);

  tzr.def("get_dst_savings", &TimeZoneRule::getDSTSavings);

  tzr.def(
      "get_final_start",
      [](const TimeZoneRule &self, int32_t prev_raw_offset, int32_t prev_dst_savings) {
        UDate time = 0;
        auto result = self.getFinalStart(prev_raw_offset, prev_dst_savings, time);
        return py::make_tuple(py::bool_(result), time);
      },
      py::arg("prev_raw_offset"), py::arg("prev_dst_savings"));

  tzr.def(
      "get_first_start",
      [](const TimeZoneRule &self, int32_t prev_raw_offset, int32_t prev_dst_savings) {
        UDate time = 0;
        auto result = self.getFirstStart(prev_raw_offset, prev_dst_savings, time);
        return py::make_tuple(py::bool_(result), time);
      },
      py::arg("prev_raw_offset"), py::arg("prev_dst_savings"));

  tzr.def("get_name", &TimeZoneRule::getName, py::arg("name"));

  tzr.def(
      "get_next_start",
      [](const TimeZoneRule &self, UDate base, int32_t prev_raw_offset, int32_t prev_dst_savings, py::bool_ inclusive) {
        UDate time = 0;
        auto result = self.getNextStart(base, prev_raw_offset, prev_dst_savings, inclusive, time);
        return py::make_tuple(py::bool_(result), time);
      },
      py::arg("base"), py::arg("prev_raw_offset"), py::arg("prev_dst_savings"), py::arg("inclusive"));

  tzr.def(
      "get_previous_start",
      [](const TimeZoneRule &self, UDate base, int32_t prev_raw_offset, int32_t prev_dst_savings, py::bool_ inclusive) {
        UDate time = 0;
        auto result = self.getPreviousStart(base, prev_raw_offset, prev_dst_savings, inclusive, time);
        return py::make_tuple(py::bool_(result), time);
      },
      py::arg("base"), py::arg("prev_raw_offset"), py::arg("prev_dst_savings"), py::arg("inclusive"));

  tzr.def("get_raw_offset", &TimeZoneRule::getRawOffset);

  tzr.def(
      "is_equivalent_to",
      [](const TimeZoneRule &self, const TimeZoneRule &that) -> py::bool_ { return self.isEquivalentTo(that); },
      py::arg("that"));

  //
  // icu::AnnualTimeZoneRule
  //
  py::class_<AnnualTimeZoneRule, TimeZoneRule> atzr(m, "AnnualTimeZoneRule");

  atzr.def_property_readonly_static("MAX_YEAR", [](const py::object &) { return AnnualTimeZoneRule::MAX_YEAR; });

  atzr.def(py::init([](const icupy::UnicodeStringVariant &name, int32_t raw_offset, int32_t dst_savings,
                       const DateTimeRule &date_time_rule, int32_t start_year, int32_t end_year) {
             return std::make_unique<AnnualTimeZoneRule>(icupy::to_unistr(name), raw_offset, dst_savings,
                                                         date_time_rule, start_year, end_year);
           }),
           py::arg("name"), py::arg("raw_offset"), py::arg("dst_savings"), py::arg("date_time_rule"),
           py::arg("start_year"), py::arg("end_year"))
      /*
      .def(py::init<const UnicodeString &, int32_t, int32_t, DateTimeRule *, int32_t, int32_t>(), py::arg("name"),
           py::arg("raw_offset"), py::arg("dst_savings"), py::arg("date_time_rule"), py::arg("start_year"),
           py::arg("end_year"))
      */
      .def(py::init<const AnnualTimeZoneRule &>(), py::arg("other"));

  atzr.def("__copy__", &AnnualTimeZoneRule::clone);

  atzr.def(
      "__deepcopy__", [](const AnnualTimeZoneRule &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  atzr.def("__repr__", [](const AnnualTimeZoneRule &self) {
    std::stringstream ss;
    ss << "<AnnualTimeZoneRule(";
    UnicodeString dest;
    self.getName(dest);
    std::string name;
    dest.toUTF8String(name);
    ss << "name=" << std::quoted(name, '\'');
    ss << ", raw_offset=" << self.getRawOffset();
    ss << ", dst_savings=" << self.getDSTSavings();
    ss << ", start_year=" << self.getStartYear();
    ss << ", end_year=" << self.getEndYear();
    ss << ")>";
    return ss.str();
  });

  atzr.def("clone", &AnnualTimeZoneRule::clone);

  atzr.def("get_end_year", &AnnualTimeZoneRule::getEndYear);

  atzr.def("get_rule", &AnnualTimeZoneRule::getRule, py::return_value_policy::reference);

  atzr.def(
      "get_start_in_year",
      [](const AnnualTimeZoneRule &self, int32_t year, int32_t prev_raw_offset, int32_t prev_dst_savings) {
        UDate time = 0;
        auto result = self.getStartInYear(year, prev_raw_offset, prev_dst_savings, time);
        return py::make_tuple(py::bool_(result), time);
      },
      py::arg("year"), py::arg("prev_raw_offset"), py::arg("prev_dst_savings"));

  atzr.def("get_start_year", &AnnualTimeZoneRule::getStartYear);

  //
  // icu::InitialTimeZoneRule
  //
  py::class_<InitialTimeZoneRule, TimeZoneRule> itzr(m, "InitialTimeZoneRule");

  itzr.def(py::init([](const icupy::UnicodeStringVariant &name, int32_t raw_offset, int32_t dst_savings) {
             return std::make_unique<InitialTimeZoneRule>(icupy::to_unistr(name), raw_offset, dst_savings);
           }),
           py::arg("name"), py::arg("raw_offset"), py::arg("dst_savings"))
      .def(py::init<const InitialTimeZoneRule &>(), py::arg("other"));

  itzr.def("__copy__", &InitialTimeZoneRule::clone);

  itzr.def(
      "__deepcopy__", [](const InitialTimeZoneRule &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  itzr.def("__repr__", [](const InitialTimeZoneRule &self) {
    std::stringstream ss;
    ss << "<InitialTimeZoneRule(";
    UnicodeString dest;
    self.getName(dest);
    std::string name;
    dest.toUTF8String(name);
    ss << "name=" << std::quoted(name, '\'');
    ss << ", raw_offset=" << self.getRawOffset();
    ss << ", dst_savings=" << self.getDSTSavings();
    ss << ")>";
    return ss.str();
  });

  itzr.def("clone", &InitialTimeZoneRule::clone);

  //
  // icu::TimeArrayTimeZoneRule
  //
  py::class_<TimeArrayTimeZoneRule, TimeZoneRule> tatzr(m, "TimeArrayTimeZoneRule");

  tatzr
      .def(py::init([](const icupy::UnicodeStringVariant &name, int32_t raw_offset, int32_t dst_savings,
                       const std::vector<UDate> &start_times, int32_t num_start_times,
                       DateTimeRule::TimeRuleType time_rule_type) {
             return std::make_unique<TimeArrayTimeZoneRule>(icupy::to_unistr(name), raw_offset, dst_savings,
                                                            start_times.data(), num_start_times, time_rule_type);
           }),
           py::arg("name"), py::arg("raw_offset"), py::arg("dst_savings"), py::arg("start_times"),
           py::arg("num_start_times"), py::arg("time_rule_type"))
      .def(py::init<const TimeArrayTimeZoneRule &>(), py::arg("other"));

  tatzr.def("__copy__", &TimeArrayTimeZoneRule::clone);

  tatzr.def(
      "__deepcopy__", [](const TimeArrayTimeZoneRule &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  tatzr.def("__repr__", [](const TimeArrayTimeZoneRule &self) {
    std::stringstream ss;
    ss << "<TimeArrayTimeZoneRule(";
    UnicodeString dest;
    self.getName(dest);
    std::string name;
    dest.toUTF8String(name);
    ss << "name=" << std::quoted(name, '\'');
    ss << ", raw_offset=" << self.getRawOffset();
    ss << ", dst_savings=" << self.getDSTSavings();
    ss << ", start_times=[";
    const auto num_start_times = self.countStartTimes();
    UDate date;
    for (int32_t n = 0; n < num_start_times; ++n) {
      self.getStartTimeAt(n, date);
      ss << date;
      if (n + 1 < num_start_times) {
        ss << ", ";
      }
    }
    ss << "])>";
    return ss.str();
  });

  tatzr.def("clone", &TimeArrayTimeZoneRule::clone);

  tatzr.def("count_start_times", &TimeArrayTimeZoneRule::countStartTimes);

  tatzr.def(
      "get_start_time_at",
      [](const TimeArrayTimeZoneRule &self, int32_t index) {
        UDate time = 0;
        auto result = self.getStartTimeAt(index, time);
        return py::make_tuple(py::bool_(result), time);
      },
      py::arg("index"));

  tatzr.def("get_time_type", &TimeArrayTimeZoneRule::getTimeType);
}

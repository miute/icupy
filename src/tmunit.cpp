#include "main.hpp"
#include <unicode/tmunit.h>

using namespace icu;

void init_tmunit(py::module &m) {
  // icu::TimeUnit
  py::class_<TimeUnit, MeasureUnit> tu(m, "TimeUnit");

  py::enum_<TimeUnit::UTimeUnitFields>(tu, "UTimeUnitFields", py::arithmetic(),
                                       "Constants for all the time units we supported.")
      .value("UTIMEUNIT_YEAR", TimeUnit::UTIMEUNIT_YEAR)
      .value("UTIMEUNIT_MONTH", TimeUnit::UTIMEUNIT_MONTH)
      .value("UTIMEUNIT_DAY", TimeUnit::UTIMEUNIT_DAY)
      .value("UTIMEUNIT_WEEK", TimeUnit::UTIMEUNIT_WEEK)
      .value("UTIMEUNIT_HOUR", TimeUnit::UTIMEUNIT_HOUR)
      .value("UTIMEUNIT_MINUTE", TimeUnit::UTIMEUNIT_MINUTE)
      .value("UTIMEUNIT_SECOND", TimeUnit::UTIMEUNIT_SECOND)
      .export_values();

  tu.def(py::init<const TimeUnit &>(), py::arg("other"));
  tu.def("__copy__", &TimeUnit::clone)
      .def(
          "__deepcopy__", [](const TimeUnit &self, py::dict) { return self.clone(); }, py::arg("memo"));
  tu.def("clone", &TimeUnit::clone);
  tu.def_static(
      "create_instance",
      [](TimeUnit::UTimeUnitFields time_unit_field) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = TimeUnit::createInstance(time_unit_field, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("time_unit_field"));
  tu.def("get_time_unit_field", &TimeUnit::getTimeUnitField);
}

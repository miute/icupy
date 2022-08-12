#include "main.hpp"
#include <unicode/tmunit.h>

using namespace icu;

void init_tmunit(py::module &m) {
  //
  // icu::TimeUnit
  //
  py::class_<TimeUnit, MeasureUnit> tu(m, "TimeUnit");

  //
  // icu::TimeUnit::UTimeUnitFields
  //
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

  //
  // icu::TimeUnit
  //
  tu.def(py::init<const TimeUnit &>(), py::arg("other"));

  tu.def("__copy__", &TimeUnit::clone);

  tu.def(
      "__deepcopy__", [](const TimeUnit &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  tu.def("clone", &TimeUnit::clone);

  tu.def_static(
      "create_instance",
      [](TimeUnit::UTimeUnitFields time_unit_field) {
        ErrorCode error_code;
        auto result = TimeUnit::createInstance(time_unit_field, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("time_unit_field"));

  tu.def("get_time_unit_field", &TimeUnit::getTimeUnitField);
}

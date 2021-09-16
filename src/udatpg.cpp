#include "main.hpp"
#include <unicode/udatpg.h>

void init_udatpg(py::module &m) {
  py::enum_<UDateTimePatternConflict>(m, "UDateTimePatternConflict", py::arithmetic())
      .value("UDATPG_NO_CONFLICT", UDATPG_NO_CONFLICT)
      .value("UDATPG_BASE_CONFLICT", UDATPG_BASE_CONFLICT)
      .value("UDATPG_CONFLICT", UDATPG_CONFLICT)
#ifndef U_HIDE_DEPRECATED_API
      .value("UDATPG_CONFLICT_COUNT", UDATPG_CONFLICT_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UDateTimePatternField>(m, "UDateTimePatternField", py::arithmetic())
      .value("UDATPG_ERA_FIELD", UDATPG_ERA_FIELD)
      .value("UDATPG_YEAR_FIELD", UDATPG_YEAR_FIELD)
      .value("UDATPG_QUARTER_FIELD", UDATPG_QUARTER_FIELD)
      .value("UDATPG_MONTH_FIELD", UDATPG_MONTH_FIELD)
      .value("UDATPG_WEEK_OF_YEAR_FIELD", UDATPG_WEEK_OF_YEAR_FIELD)
      .value("UDATPG_WEEK_OF_MONTH_FIELD", UDATPG_WEEK_OF_MONTH_FIELD)
      .value("UDATPG_WEEKDAY_FIELD", UDATPG_WEEKDAY_FIELD)
      .value("UDATPG_DAY_OF_YEAR_FIELD", UDATPG_DAY_OF_YEAR_FIELD)
      .value("UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD", UDATPG_DAY_OF_WEEK_IN_MONTH_FIELD)
      .value("UDATPG_DAY_FIELD", UDATPG_DAY_FIELD)
      .value("UDATPG_DAYPERIOD_FIELD", UDATPG_DAYPERIOD_FIELD)
      .value("UDATPG_HOUR_FIELD", UDATPG_HOUR_FIELD)
      .value("UDATPG_MINUTE_FIELD", UDATPG_MINUTE_FIELD)
      .value("UDATPG_SECOND_FIELD", UDATPG_SECOND_FIELD)
      .value("UDATPG_FRACTIONAL_SECOND_FIELD", UDATPG_FRACTIONAL_SECOND_FIELD)
      .value("UDATPG_ZONE_FIELD", UDATPG_ZONE_FIELD)
#ifndef U_FORCE_HIDE_DEPRECATED_API
      .value("UDATPG_FIELD_COUNT", UDATPG_FIELD_COUNT)
#endif // U_FORCE_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UDateTimePatternMatchOptions>(m, "UDateTimePatternMatchOptions", py::arithmetic())
      .value("UDATPG_MATCH_NO_OPTIONS", UDATPG_MATCH_NO_OPTIONS)
      .value("UDATPG_MATCH_HOUR_FIELD_LENGTH", UDATPG_MATCH_HOUR_FIELD_LENGTH)
      .value("UDATPG_MATCH_ALL_FIELDS_LENGTH", UDATPG_MATCH_ALL_FIELDS_LENGTH)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  py::enum_<UDateTimePGDisplayWidth>(m, "UDateTimePGDisplayWidth", py::arithmetic())
      .value("UDATPG_WIDE", UDATPG_WIDE)
      .value("UDATPG_ABBREVIATED", UDATPG_ABBREVIATED)
      .value("UDATPG_NARROW", UDATPG_NARROW)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
}

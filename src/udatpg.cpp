#include "main.hpp"
#include <unicode/udatpg.h>

void init_udatpg(py::module &m) {
  py::enum_<UDateTimePatternConflict>(m, "UDateTimePatternConflict", py::arithmetic(),
                                      "Status return values from *udatpg_add_pattern()*.")
      .value("UDATPG_NO_CONFLICT", UDATPG_NO_CONFLICT)
      .value("UDATPG_BASE_CONFLICT", UDATPG_BASE_CONFLICT)
      .value("UDATPG_CONFLICT", UDATPG_CONFLICT)
#ifndef U_HIDE_DEPRECATED_API
      .value("UDATPG_CONFLICT_COUNT", UDATPG_CONFLICT_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UDateTimePatternField>(
      m, "UDateTimePatternField", py::arithmetic(),
      "Field number constants for *udatpg_get_append_item_formats()* and similar functions.\n\n"
      "These constants are separate from *UDateFormatField* despite semantic overlap because some fields are merged "
      "for the date/time pattern generator.")
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
      .value("UDATPG_FIELD_COUNT", UDATPG_FIELD_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_FORCE_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UDateTimePatternMatchOptions>(
      m, "UDateTimePatternMatchOptions", py::arithmetic(),
      "Masks to control forcing the length of specified fields in the returned pattern to match those in the skeleton "
      "(when this would not happen otherwise).\n\n"
      "These may be combined to force the length of multiple fields. Used with *udatpg_get_best_pattern_with_options*, "
      "*udatpg_replace_field_types_with_options*.")
      .value("UDATPG_MATCH_NO_OPTIONS", UDATPG_MATCH_NO_OPTIONS)
      .value("UDATPG_MATCH_HOUR_FIELD_LENGTH", UDATPG_MATCH_HOUR_FIELD_LENGTH)
      .value("UDATPG_MATCH_ALL_FIELDS_LENGTH", UDATPG_MATCH_ALL_FIELDS_LENGTH)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  py::enum_<UDateTimePGDisplayWidth>(m, "UDateTimePGDisplayWidth", py::arithmetic(),
                                     "Field display name width constants for *udatpg_get_field_display_name()*.")
      .value("UDATPG_WIDE", UDATPG_WIDE)
      .value("UDATPG_ABBREVIATED", UDATPG_ABBREVIATED)
      .value("UDATPG_NARROW", UDATPG_NARROW)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
}

#include "main.hpp"
#include <unicode/ucal.h>

void init_ucal(py::module &m) {
  py::enum_<UCalendarAMPMs>(m, "UCalendarAMPMs", py::arithmetic())
      .value("UCAL_AM", UCAL_AM)
      .value("UCAL_PM", UCAL_PM)
      .export_values();

  py::enum_<UCalendarAttribute>(m, "UCalendarAttribute", py::arithmetic())
      .value("UCAL_LENIENT", UCAL_LENIENT)
      .value("UCAL_FIRST_DAY_OF_WEEK", UCAL_FIRST_DAY_OF_WEEK)
      .value("UCAL_MINIMAL_DAYS_IN_FIRST_WEEK", UCAL_MINIMAL_DAYS_IN_FIRST_WEEK)
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UCAL_REPEATED_WALL_TIME", UCAL_REPEATED_WALL_TIME)
      .value("UCAL_SKIPPED_WALL_TIME", UCAL_SKIPPED_WALL_TIME)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();

  py::enum_<UCalendarDateFields>(m, "UCalendarDateFields", py::arithmetic())
      .value("UCAL_ERA", UCAL_ERA)
      .value("UCAL_YEAR", UCAL_YEAR)
      .value("UCAL_MONTH", UCAL_MONTH)
      .value("UCAL_WEEK_OF_YEAR", UCAL_WEEK_OF_YEAR)
      .value("UCAL_WEEK_OF_MONTH", UCAL_WEEK_OF_MONTH)
      .value("UCAL_DATE", UCAL_DATE)
      .value("UCAL_DAY_OF_YEAR", UCAL_DAY_OF_YEAR)
      .value("UCAL_DAY_OF_WEEK", UCAL_DAY_OF_WEEK)
      .value("UCAL_DAY_OF_WEEK_IN_MONTH", UCAL_DAY_OF_WEEK_IN_MONTH)
      .value("UCAL_AM_PM", UCAL_AM_PM)
      .value("UCAL_HOUR", UCAL_HOUR)
      .value("UCAL_HOUR_OF_DAY", UCAL_HOUR_OF_DAY)
      .value("UCAL_MINUTE", UCAL_MINUTE)
      .value("UCAL_SECOND", UCAL_SECOND)
      .value("UCAL_MILLISECOND", UCAL_MILLISECOND)
      .value("UCAL_ZONE_OFFSET", UCAL_ZONE_OFFSET)
      .value("UCAL_DST_OFFSET", UCAL_DST_OFFSET)
      .value("UCAL_YEAR_WOY", UCAL_YEAR_WOY)
      .value("UCAL_DOW_LOCAL", UCAL_DOW_LOCAL)
      .value("UCAL_EXTENDED_YEAR", UCAL_EXTENDED_YEAR)
      .value("UCAL_JULIAN_DAY", UCAL_JULIAN_DAY)
      .value("UCAL_MILLISECONDS_IN_DAY", UCAL_MILLISECONDS_IN_DAY)
      .value("UCAL_IS_LEAP_MONTH", UCAL_IS_LEAP_MONTH)
#ifndef U_FORCE_HIDE_DEPRECATED_API
      .value("UCAL_FIELD_COUNT", UCAL_FIELD_COUNT)
#endif // U_FORCE_HIDE_DEPRECATED_API
      .value("UCAL_DAY_OF_MONTH", UCAL_DAY_OF_MONTH)
      .export_values();

  py::enum_<UCalendarDaysOfWeek>(m, "UCalendarDaysOfWeek", py::arithmetic())
      .value("UCAL_SUNDAY", UCAL_SUNDAY)
      .value("UCAL_MONDAY", UCAL_MONDAY)
      .value("UCAL_TUESDAY", UCAL_TUESDAY)
      .value("UCAL_WEDNESDAY", UCAL_WEDNESDAY)
      .value("UCAL_THURSDAY", UCAL_THURSDAY)
      .value("UCAL_FRIDAY", UCAL_FRIDAY)
      .value("UCAL_SATURDAY", UCAL_SATURDAY)
      .export_values();

  py::enum_<UCalendarDisplayNameType>(m, "UCalendarDisplayNameType", py::arithmetic())
      .value("UCAL_STANDARD", UCAL_STANDARD)
      .value("UCAL_SHORT_STANDARD", UCAL_SHORT_STANDARD)
      .value("UCAL_DST", UCAL_DST)
      .value("UCAL_SHORT_DST", UCAL_SHORT_DST)
      .export_values();

  py::enum_<UCalendarLimitType>(m, "UCalendarLimitType", py::arithmetic())
      .value("UCAL_MINIMUM", UCAL_MINIMUM)
      .value("UCAL_MAXIMUM", UCAL_MAXIMUM)
      .value("UCAL_GREATEST_MINIMUM", UCAL_GREATEST_MINIMUM)
      .value("UCAL_LEAST_MAXIMUM", UCAL_LEAST_MAXIMUM)
      .value("UCAL_ACTUAL_MINIMUM", UCAL_ACTUAL_MINIMUM)
      .value("UCAL_ACTUAL_MAXIMUM", UCAL_ACTUAL_MAXIMUM)
      .export_values();

  py::enum_<UCalendarMonths>(m, "UCalendarMonths", py::arithmetic())
      .value("UCAL_JANUARY", UCAL_JANUARY)
      .value("UCAL_FEBRUARY", UCAL_FEBRUARY)
      .value("UCAL_MARCH", UCAL_MARCH)
      .value("UCAL_APRIL", UCAL_APRIL)
      .value("UCAL_MAY", UCAL_MAY)
      .value("UCAL_JUNE", UCAL_JUNE)
      .value("UCAL_JULY", UCAL_JULY)
      .value("UCAL_AUGUST", UCAL_AUGUST)
      .value("UCAL_SEPTEMBER", UCAL_SEPTEMBER)
      .value("UCAL_OCTOBER", UCAL_OCTOBER)
      .value("UCAL_NOVEMBER", UCAL_NOVEMBER)
      .value("UCAL_DECEMBER", UCAL_DECEMBER)
      .value("UCAL_UNDECIMBER", UCAL_UNDECIMBER)
      .export_values();

  py::enum_<UCalendarType>(m, "UCalendarType", py::arithmetic())
      .value("UCAL_TRADITIONAL", UCAL_TRADITIONAL)
      .value("UCAL_DEFAULT", UCAL_DEFAULT)
      .value("UCAL_GREGORIAN", UCAL_GREGORIAN)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  py::enum_<UCalendarWallTimeOption>(m, "UCalendarWallTimeOption", py::arithmetic())
      .value("UCAL_WALLTIME_LAST", UCAL_WALLTIME_LAST)
      .value("UCAL_WALLTIME_FIRST", UCAL_WALLTIME_FIRST)
      .value("UCAL_WALLTIME_NEXT_VALID", UCAL_WALLTIME_NEXT_VALID)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  py::enum_<UCalendarWeekdayType>(m, "UCalendarWeekdayType", py::arithmetic())
      .value("UCAL_WEEKDAY", UCAL_WEEKDAY)
      .value("UCAL_WEEKEND", UCAL_WEEKEND)
      .value("UCAL_WEEKEND_ONSET", UCAL_WEEKEND_ONSET)
      .value("UCAL_WEEKEND_CEASE", UCAL_WEEKEND_CEASE)
      .export_values();

  py::enum_<USystemTimeZoneType>(m, "USystemTimeZoneType", py::arithmetic())
      .value("UCAL_ZONE_TYPE_ANY", UCAL_ZONE_TYPE_ANY)
      .value("UCAL_ZONE_TYPE_CANONICAL", UCAL_ZONE_TYPE_CANONICAL)
      .value("UCAL_ZONE_TYPE_CANONICAL_LOCATION", UCAL_ZONE_TYPE_CANONICAL_LOCATION)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  py::enum_<UTimeZoneLocalOption>(m, "UTimeZoneLocalOption", py::arithmetic())
      .value("UCAL_TZ_LOCAL_FORMER", UCAL_TZ_LOCAL_FORMER)
      .value("UCAL_TZ_LOCAL_LATTER", UCAL_TZ_LOCAL_LATTER)
      .value("UCAL_TZ_LOCAL_STANDARD_FORMER", UCAL_TZ_LOCAL_STANDARD_FORMER)
      .value("UCAL_TZ_LOCAL_STANDARD_LATTER", UCAL_TZ_LOCAL_STANDARD_LATTER)
      .value("UCAL_TZ_LOCAL_DAYLIGHT_FORMER", UCAL_TZ_LOCAL_DAYLIGHT_FORMER)
      .value("UCAL_TZ_LOCAL_DAYLIGHT_LATTER", UCAL_TZ_LOCAL_DAYLIGHT_LATTER)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  py::enum_<UTimeZoneTransitionType>(m, "UTimeZoneTransitionType", py::arithmetic())
      .value("UCAL_TZ_TRANSITION_NEXT", UCAL_TZ_TRANSITION_NEXT)
      .value("UCAL_TZ_TRANSITION_NEXT_INCLUSIVE", UCAL_TZ_TRANSITION_NEXT_INCLUSIVE)
      .value("UCAL_TZ_TRANSITION_PREVIOUS", UCAL_TZ_TRANSITION_PREVIOUS)
      .value("UCAL_TZ_TRANSITION_PREVIOUS_INCLUSIVE", UCAL_TZ_TRANSITION_PREVIOUS_INCLUSIVE)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  m.attr("UCAL_UNKNOWN_ZONE_ID") = UCAL_UNKNOWN_ZONE_ID;
}

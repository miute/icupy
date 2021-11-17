#include "main.hpp"
#include <unicode/ucal.h>

void init_ucal(py::module &m) {
  py::enum_<UCalendarAMPMs>(m, "UCalendarAMPMs", py::arithmetic(), "Possible AM/PM values in a *UCalendar*.")
      .value("UCAL_AM", UCAL_AM, "AM.")
      .value("UCAL_PM", UCAL_PM, "PM.")
      .export_values();

  py::enum_<UCalendarAttribute>(m, "UCalendarAttribute", py::arithmetic(), "Types of *UCalendar* attributes.")
      .value("UCAL_LENIENT", UCAL_LENIENT, "Lenient parsing.")
      .value("UCAL_FIRST_DAY_OF_WEEK", UCAL_FIRST_DAY_OF_WEEK, "First day of week.")
      .value("UCAL_MINIMAL_DAYS_IN_FIRST_WEEK", UCAL_MINIMAL_DAYS_IN_FIRST_WEEK,
             "Minimum number of days in first week.")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UCAL_REPEATED_WALL_TIME", UCAL_REPEATED_WALL_TIME,
             "The behavior for handling wall time repeating multiple times at negative time zone offset transitions.")
      .value("UCAL_SKIPPED_WALL_TIME", UCAL_SKIPPED_WALL_TIME,
             "The behavior for handling skipped wall time at positive time zone offset transitions.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();

  py::enum_<UCalendarDateFields>(m, "UCalendarDateFields", py::arithmetic(), "Possible fields in a *UCalendar*.")
      .value("UCAL_ERA", UCAL_ERA,
             "Field number indicating the era, e.g., AD or BC in the Gregorian (Julian) calendar.\n\n  "
             "This is a calendar-specific value.")
      .value("UCAL_YEAR", UCAL_YEAR,
             "Field number indicating the year.\n\n  "
             "This is a calendar-specific value.")
      .value("UCAL_MONTH", UCAL_MONTH,
             "Field number indicating the month.\n\n  "
             "This is a calendar-specific value. The first month of the year is JANUARY; the last depends on the "
             "number of months in a year.")
      .value("UCAL_WEEK_OF_YEAR", UCAL_WEEK_OF_YEAR,
             "Field number indicating the week number within the current year.\n\n  "
             "The first week of the year, as defined by *UCAL_FIRST_DAY_OF_WEEK* and *UCAL_MINIMAL_DAYS_IN_FIRST_WEEK* "
             "attributes, has value 1. Subclasses define the value of *UCAL_WEEK_OF_YEAR* for days before the first "
             "week of the year.")
      .value("UCAL_WEEK_OF_MONTH", UCAL_WEEK_OF_MONTH,
             "Field number indicating the week number within the current month.\n\n  "
             "The first week of the month, as defined by *UCAL_FIRST_DAY_OF_WEEK* and "
             "*UCAL_MINIMAL_DAYS_IN_FIRST_WEEK* attributes, has value 1. Subclasses define the value of "
             "WEEK_OF_MONTH for days before the first week of the month.")
      .value("UCAL_DATE", UCAL_DATE,
             "Field number indicating the day of the month.\n\n  "
             "This is a synonym for DAY_OF_MONTH. The first day of the month has value 1.")
      .value("UCAL_DAY_OF_YEAR", UCAL_DAY_OF_YEAR,
             "Field number indicating the day number within the current year.\n\n  "
             "The first day of the year has value 1.")
      .value("UCAL_DAY_OF_WEEK", UCAL_DAY_OF_WEEK,
             "Field number indicating the day of the week.\n\n  "
             "This field takes values SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, and SATURDAY.")
      .value("UCAL_DAY_OF_WEEK_IN_MONTH", UCAL_DAY_OF_WEEK_IN_MONTH,
             "Field number indicating the ordinal number of the day of the week within the current month.\n\n  "
             "Together with the DAY_OF_WEEK field, this uniquely specifies a day within a month. Unlike "
             "WEEK_OF_MONTH and WEEK_OF_YEAR, this field's value does not depend on *get_first_day_of_week()* or "
             "*get_minimal_days_in_first_week()*. DAY_OF_MONTH 1 through 7 always correspond to "
             "DAY_OF_WEEK_IN_MONTH 1; 8 through 15 correspond to DAY_OF_WEEK_IN_MONTH 2, and so on. "
             "DAY_OF_WEEK_IN_MONTH 0 indicates the week before DAY_OF_WEEK_IN_MONTH 1. Negative values count back "
             "from the end of the month, so the last Sunday of a month is specified as DAY_OF_WEEK = SUNDAY, "
             "DAY_OF_WEEK_IN_MONTH = -1. Because negative values count backward they will usually be aligned "
             "differently within the month than positive values. For example, if a month has 31 days, "
             "DAY_OF_WEEK_IN_MONTH -1 will overlap DAY_OF_WEEK_IN_MONTH 5 and the end of 4.")
      .value("UCAL_AM_PM", UCAL_AM_PM,
             "Field number indicating whether the HOUR is before or after noon.\n\n  "
             "E.g., at 10:04:15.250 PM the AM_PM is PM.")
      .value("UCAL_HOUR", UCAL_HOUR,
             "Field number indicating the hour of the morning or afternoon.\n\n  "
             "HOUR is used for the 12-hour clock. E.g., at 10:04:15.250 PM the HOUR is 10.")
      .value("UCAL_HOUR_OF_DAY", UCAL_HOUR_OF_DAY,
             "Field number indicating the hour of the day.\n\n  "
             "HOUR_OF_DAY is used for the 24-hour clock. E.g., at 10:04:15.250 PM the HOUR_OF_DAY is 22.")
      .value("UCAL_MINUTE", UCAL_MINUTE,
             "Field number indicating the minute within the hour.\n\n  "
             "E.g., at 10:04:15.250 PM the *UCAL_MINUTE* is 4.")
      .value("UCAL_SECOND", UCAL_SECOND,
             "Field number indicating the second within the minute.\n\n  "
             "E.g., at 10:04:15.250 PM the *UCAL_SECOND* is 15.")
      .value("UCAL_MILLISECOND", UCAL_MILLISECOND,
             "Field number indicating the millisecond within the second.\n\n  "
             "E.g., at 10:04:15.250 PM the *UCAL_MILLISECOND* is 250.")
      .value("UCAL_ZONE_OFFSET", UCAL_ZONE_OFFSET, "Field number indicating the raw offset from GMT in milliseconds.")
      .value("UCAL_DST_OFFSET", UCAL_DST_OFFSET, "Field number indicating the daylight savings offset in milliseconds.")
      .value("UCAL_YEAR_WOY", UCAL_YEAR_WOY,
             "Field number indicating the extended year corresponding to the *UCAL_WEEK_OF_YEAR* field.\n\n  "
             "This may be one greater or less than the value of *UCAL_EXTENDED_YEAR*.")
      .value("UCAL_DOW_LOCAL", UCAL_DOW_LOCAL,
             "Field number indicating the localized day of week.\n\n  "
             "This will be a value from 1 to 7 inclusive, with 1 being the localized first day of the week.")
      .value("UCAL_EXTENDED_YEAR", UCAL_EXTENDED_YEAR,
             "Year of this calendar system, encompassing all supra-year fields.\n\n  "
             "For example, in Gregorian/Julian calendars, positive Extended Year values indicate years AD, 1 BC = 0 "
             "extended, 2 BC = -1 extended, and so on.")
      .value("UCAL_JULIAN_DAY", UCAL_JULIAN_DAY,
             "Field number indicating the modified Julian day number.\n\n  "
             "This is different from the conventional Julian day number in two regards. First, it demarcates days at "
             "local zone midnight, rather than noon GMT. Second, it is a local number; that is, it depends on the "
             "local time zone. It can be thought of as a single number that encompasses all the date-related fields.")
      .value("UCAL_MILLISECONDS_IN_DAY", UCAL_MILLISECONDS_IN_DAY,
             "Ranges from 0 to 23:59:59.999 (regardless of DST).\n\n  "
             "This field behaves exactly like a composite of all time-related fields, not including the zone fields. "
             "As such, it also reflects discontinuities of those fields on DST transition days. On a day of DST onset, "
             "it will jump forward. On a day of DST cessation, it will jump backward. This reflects the fact that it "
             "must be combined with the DST_OFFSET field to obtain a unique local time value.")
      .value("UCAL_IS_LEAP_MONTH", UCAL_IS_LEAP_MONTH,
             "Whether or not the current month is a leap month (0 or 1).\n\n  "
             "See the Chinese calendar for an example of this.")
#ifndef U_FORCE_HIDE_DEPRECATED_API
      .value("UCAL_FIELD_COUNT", UCAL_FIELD_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_FORCE_HIDE_DEPRECATED_API
      .value("UCAL_DAY_OF_MONTH", UCAL_DAY_OF_MONTH,
             "Field number indicating the day of the month.\n\n  "
             "This is a synonym for *UCAL_DATE*. The first day of the month has value 1.")
      .export_values();

  py::enum_<UCalendarDaysOfWeek>(
      m, "UCalendarDaysOfWeek", py::arithmetic(),
      "Useful constant for days of week.\n\n"
      "Note: Calendar day-of-week is 1-based. Clients who create locale resources for the field of first-day-of-week "
      "should be aware of this. For instance, in US locale, first-day-of-week is set to 1, i.e., *UCAL_SUNDAY*. "
      "Possible days of the week in a *UCalendar*.")
      .value("UCAL_SUNDAY", UCAL_SUNDAY, "Sunday.")
      .value("UCAL_MONDAY", UCAL_MONDAY, "Monday.")
      .value("UCAL_TUESDAY", UCAL_TUESDAY, "Tuesday.")
      .value("UCAL_WEDNESDAY", UCAL_WEDNESDAY, "Wednesday.")
      .value("UCAL_THURSDAY", UCAL_THURSDAY, "Thursday.")
      .value("UCAL_FRIDAY", UCAL_FRIDAY, "Friday.")
      .value("UCAL_SATURDAY", UCAL_SATURDAY, "Saturday.")
      .export_values();

  py::enum_<UCalendarDisplayNameType>(m, "UCalendarDisplayNameType", py::arithmetic(),
                                      "Possible formats for a *UCalendar's* display name.")
      .value("UCAL_STANDARD", UCAL_STANDARD, "Standard display name.")
      .value("UCAL_SHORT_STANDARD", UCAL_SHORT_STANDARD, "Short standard display name.")
      .value("UCAL_DST", UCAL_DST, "Daylight savings display name.")
      .value("UCAL_SHORT_DST", UCAL_SHORT_DST, "Short daylight savings display name.")
      .export_values();

  py::enum_<UCalendarLimitType>(m, "UCalendarLimitType", py::arithmetic(), "Possible limit values for a *UCalendar*.")
      .value("UCAL_MINIMUM", UCAL_MINIMUM, "Minimum value.")
      .value("UCAL_MAXIMUM", UCAL_MAXIMUM, "Maximum value.")
      .value("UCAL_GREATEST_MINIMUM", UCAL_GREATEST_MINIMUM, "Greatest minimum value.")
      .value("UCAL_LEAST_MAXIMUM", UCAL_LEAST_MAXIMUM, "Least maximum value.")
      .value("UCAL_ACTUAL_MINIMUM", UCAL_ACTUAL_MINIMUM, "Actual minimum value.")
      .value("UCAL_ACTUAL_MAXIMUM", UCAL_ACTUAL_MAXIMUM, "Actual maximum value.")
      .export_values();

  py::enum_<UCalendarMonths>(m, "UCalendarMonths", py::arithmetic(),
                             "Possible months in a *UCalendar*.\n\n"
                             "Note: Calendar month is 0-based.")
      .value("UCAL_JANUARY", UCAL_JANUARY, "January.")
      .value("UCAL_FEBRUARY", UCAL_FEBRUARY, "February.")
      .value("UCAL_MARCH", UCAL_MARCH, "March.")
      .value("UCAL_APRIL", UCAL_APRIL, "April.")
      .value("UCAL_MAY", UCAL_MAY, "May.")
      .value("UCAL_JUNE", UCAL_JUNE, "June.")
      .value("UCAL_JULY", UCAL_JULY, "July.")
      .value("UCAL_AUGUST", UCAL_AUGUST, "August.")
      .value("UCAL_SEPTEMBER", UCAL_SEPTEMBER, "September.")
      .value("UCAL_OCTOBER", UCAL_OCTOBER, "October.")
      .value("UCAL_NOVEMBER", UCAL_NOVEMBER, "November.")
      .value("UCAL_DECEMBER", UCAL_DECEMBER, "December.")
      .value("UCAL_UNDECIMBER", UCAL_UNDECIMBER,
             "Value of the *UCAL_MONTH* field indicating the thirteenth month of the year.\n\n  "
             "Although the Gregorian calendar does not use this value, lunar calendars do.")
      .export_values();

  py::enum_<UCalendarType>(m, "UCalendarType", py::arithmetic(), "Possible types of *UCalendar*.")
      .value("UCAL_TRADITIONAL", UCAL_TRADITIONAL,
             "Despite the name, *UCAL_TRADITIONAL* designates the locale's default calendar, which may be the "
             "Gregorian calendar or some other calendar.")
      .value("UCAL_DEFAULT", UCAL_DEFAULT, "A better name for *UCAL_TRADITIONAL*.")
      .value("UCAL_GREGORIAN", UCAL_GREGORIAN, "Unambiguously designates the Gregorian calendar for the locale.")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  py::enum_<UCalendarWallTimeOption>(m, "UCalendarWallTimeOption", py::arithmetic(),
                                     "Options for handling ambiguous wall time at time zone offset transitions.")
      .value("UCAL_WALLTIME_LAST", UCAL_WALLTIME_LAST,
             "An ambiguous wall time to be interpreted as the latest.\n\n  "
             "This option is valid for *UCAL_REPEATED_WALL_TIME* and *UCAL_SKIPPED_WALL_TIME*.")
      .value("UCAL_WALLTIME_FIRST", UCAL_WALLTIME_FIRST,
             "An ambiguous wall time to be interpreted as the earliest.\n\n  "
             "This option is valid for *UCAL_REPEATED_WALL_TIME* and *UCAL_SKIPPED_WALL_TIME*.")
      .value("UCAL_WALLTIME_NEXT_VALID", UCAL_WALLTIME_NEXT_VALID,
             "An ambiguous wall time to be interpreted as the next valid wall time.\n\n  "
             "This option is valid for *UCAL_SKIPPED_WALL_TIME*.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  py::enum_<UCalendarWeekdayType>(m, "UCalendarWeekdayType", py::arithmetic(),
                                  "Weekday types, as returned by *ucal_get_day_of_week_type()*.")
      .value("UCAL_WEEKDAY", UCAL_WEEKDAY, "Designates a full weekday (no part of the day is included in the weekend).")
      .value("UCAL_WEEKEND", UCAL_WEEKEND, "Designates a full weekend day (the entire day is included in the weekend).")
      .value("UCAL_WEEKEND_ONSET", UCAL_WEEKEND_ONSET,
             "Designates a day that starts as a weekday and transitions to the weekend.\n\n  "
             "Call *ucal_get_weekend_transition()* to get the time of transition.")
      .value("UCAL_WEEKEND_CEASE", UCAL_WEEKEND_CEASE,
             "Designates a day that starts as the weekend and transitions to a weekday.\n\n  "
             "Call *ucal_get_weekend_transition()* to get the time of transition.")
      .export_values();

  py::enum_<USystemTimeZoneType>(
      m, "USystemTimeZoneType", py::arithmetic(),
      "System time zone type constants used by filtering zones in *ucal_open_time_zone_id_enumeration*.")
      .value("UCAL_ZONE_TYPE_ANY", UCAL_ZONE_TYPE_ANY, "Any system zones.")
      .value("UCAL_ZONE_TYPE_CANONICAL", UCAL_ZONE_TYPE_CANONICAL, "Canonical system zones.")
      .value("UCAL_ZONE_TYPE_CANONICAL_LOCATION", UCAL_ZONE_TYPE_CANONICAL_LOCATION,
             "Canonical system zones associated with actual locations.")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  py::enum_<UTimeZoneLocalOption>(
      m, "UTimeZoneLocalOption", py::arithmetic(),
      "Options used by *ucal_get_time_zone_offset_from_local* and *BasicTimeZone::get_offset_from_local()* to specify "
      "how to interpret an input time when it does not exist, or when it is ambiguous, around a time zone transition.")
      .value("UCAL_TZ_LOCAL_FORMER", UCAL_TZ_LOCAL_FORMER,
             "An input time is always interpreted as local time before a time zone transition.")
      .value("UCAL_TZ_LOCAL_LATTER", UCAL_TZ_LOCAL_LATTER,
             "An input time is always interpreted as local time after a time zone transition.")
      .value("UCAL_TZ_LOCAL_STANDARD_FORMER", UCAL_TZ_LOCAL_STANDARD_FORMER,
             "An input time is interpreted as standard time when local time is switched to/from daylight saving "
             "time.\n\n  "
             "When both sides of a time zone transition are standard time, or daylight saving time, the local time "
             "before the transition is used.")
      .value("UCAL_TZ_LOCAL_STANDARD_LATTER", UCAL_TZ_LOCAL_STANDARD_LATTER,
             "An input time is interpreted as standard time when local time is switched to/from daylight saving "
             "time.\n\n  "
             "When both sides of a time zone transition are standard time, or daylight saving time, the local time "
             "after the transition is used.")
      .value("UCAL_TZ_LOCAL_DAYLIGHT_FORMER", UCAL_TZ_LOCAL_DAYLIGHT_FORMER,
             "An input time is interpreted as daylight saving time when local time is switched to/from standard "
             "time.\n\n  "
             "When both sides of a time zone transition are standard time, or daylight saving time, the local time "
             "before the transition is used.")
      .value("UCAL_TZ_LOCAL_DAYLIGHT_LATTER", UCAL_TZ_LOCAL_DAYLIGHT_LATTER,
             "An input time is interpreted as daylight saving time when local time is switched to/from standard "
             "time.\n\n  "
             "When both sides of a time zone transition are standard time, or daylight saving time, the local time "
             "after the transition is used.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  py::enum_<UTimeZoneTransitionType>(m, "UTimeZoneTransitionType", py::arithmetic(),
                                     "Time zone transition types for *ucal_get_time_zone_transition_date*.")
      .value("UCAL_TZ_TRANSITION_NEXT", UCAL_TZ_TRANSITION_NEXT,
             "Get the next transition after the current date, i.e. excludes the current date.")
      .value("UCAL_TZ_TRANSITION_NEXT_INCLUSIVE", UCAL_TZ_TRANSITION_NEXT_INCLUSIVE,
             "Get the next transition on or after the current date, i.e. may include the current date.")
      .value("UCAL_TZ_TRANSITION_PREVIOUS", UCAL_TZ_TRANSITION_PREVIOUS,
             "Get the previous transition before the current date, i.e. excludes the current date.")
      .value("UCAL_TZ_TRANSITION_PREVIOUS_INCLUSIVE", UCAL_TZ_TRANSITION_PREVIOUS_INCLUSIVE,
             "Get the previous transition on or before the current date, i.e. may include the current date.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  m.attr("UCAL_UNKNOWN_ZONE_ID") = UCAL_UNKNOWN_ZONE_ID;
}

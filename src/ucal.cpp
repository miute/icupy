#include "main.hpp"
#include <unicode/ucal.h>

void init_ucal(py::module &m) {
  //
  // enum UCalendarAMPMs
  //
  py::enum_<UCalendarAMPMs>(m, "UCalendarAMPMs", py::arithmetic(),
                            "Possible AM/PM values in a UCalendar.")
      .value("UCAL_AM", UCAL_AM, R"doc(
             AM.
             )doc")
      .value("UCAL_PM", UCAL_PM, R"doc(
             PM.
             )doc")
      .export_values();

  //
  // enum UCalendarAttribute
  //
  py::enum_<UCalendarAttribute>(m, "UCalendarAttribute", py::arithmetic(),
                                "Types of UCalendar attributes.")
      .value("UCAL_LENIENT", UCAL_LENIENT, R"doc(
             Lenient parsing.
             )doc")
      .value("UCAL_FIRST_DAY_OF_WEEK", UCAL_FIRST_DAY_OF_WEEK, R"doc(
             First day of week.
             )doc")
      .value("UCAL_MINIMAL_DAYS_IN_FIRST_WEEK", UCAL_MINIMAL_DAYS_IN_FIRST_WEEK,
             R"doc(
             Minimum number of days in first week.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UCAL_REPEATED_WALL_TIME", UCAL_REPEATED_WALL_TIME,
             R"doc(
             The behavior for handling wall time repeating multiple times at
             negative time zone offset transitions.
             )doc")
      .value("UCAL_SKIPPED_WALL_TIME", UCAL_SKIPPED_WALL_TIME,
             R"doc(
             The behavior for handling skipped wall time at positive time zone
             offset transitions.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();

  //
  // enum UCalendarDateFields
  //
  py::enum_<UCalendarDateFields>(m, "UCalendarDateFields", py::arithmetic(),
                                 "Possible fields in a UCalendar.")
      .value("UCAL_ERA", UCAL_ERA,
             R"doc(
             Field number indicating the era, e.g., AD or BC in the Gregorian
             (Julian) calendar.

             This is a calendar-specific value.
             )doc")
      .value("UCAL_YEAR", UCAL_YEAR, R"doc(
             Field number indicating the year.

             This is a calendar-specific value.
             )doc")
      .value("UCAL_MONTH", UCAL_MONTH, R"doc(
             Field number indicating the month.

             This is a calendar-specific value. The first month of the year is
             :attr:`~UCalendarMonths.UCAL_JANUARY`; the last depends on the
             number of months in a year.
             )doc")
      .value("UCAL_WEEK_OF_YEAR", UCAL_WEEK_OF_YEAR, R"doc(
             Field number indicating the week number within the current year.

             The first week of the year, as defined by
             :attr:`~UCalendarAttribute.UCAL_FIRST_DAY_OF_WEEK` and
             :attr:`~UCalendarAttribute.UCAL_MINIMAL_DAYS_IN_FIRST_WEEK`
             attributes, has value 1.
             Subclasses define the value of :attr:`UCAL_WEEK_OF_YEAR` for days
             before the first week of the year.
             )doc")
      .value("UCAL_WEEK_OF_MONTH", UCAL_WEEK_OF_MONTH, R"doc(
             Field number indicating the week number within the current month.

             The first week of the month, as defined by
             :attr:`~UCalendarAttribute.UCAL_FIRST_DAY_OF_WEEK` and
             :attr:`~UCalendarAttribute.UCAL_MINIMAL_DAYS_IN_FIRST_WEEK`
             attributes, has value 1.
             Subclasses define the value of :attr:`UCAL_WEEK_OF_MONTH` for days
             before the first week of the month.
             )doc")
      .value("UCAL_DATE", UCAL_DATE, R"doc(
             Field number indicating the day of the month.

             This is a synonym for :attr:`UCAL_DAY_OF_MONTH`. The first day of the
             month has value 1.
             )doc")
      .value("UCAL_DAY_OF_YEAR", UCAL_DAY_OF_YEAR, R"doc(
             Field number indicating the day number within the current year.

             The first day of the year has value 1.
             )doc")
      .value("UCAL_DAY_OF_WEEK", UCAL_DAY_OF_WEEK, R"doc(
             Field number indicating the day of the week.

             This field takes values
             :attr:`~UCalendarDaysOfWeek.UCAL_SUNDAY`,
             :attr:`~UCalendarDaysOfWeek.UCAL_MONDAY`,
             :attr:`~UCalendarDaysOfWeek.UCAL_TUESDAY`,
             :attr:`~UCalendarDaysOfWeek.UCAL_WEDNESDAY`,
             :attr:`~UCalendarDaysOfWeek.UCAL_THURSDAY`,
             :attr:`~UCalendarDaysOfWeek.UCAL_FRIDAY`, and
             :attr:`~UCalendarDaysOfWeek.UCAL_SATURDAY`.
             )doc")
      .value("UCAL_DAY_OF_WEEK_IN_MONTH", UCAL_DAY_OF_WEEK_IN_MONTH, R"doc(
             Field number indicating the ordinal number of the day of the week
             within the current month.

             Together with the :attr:`UCAL_DAY_OF_WEEK` field, this uniquely
             specifies a day within a month. Unlike :attr:`UCAL_WEEK_OF_MONTH`
             and :attr:`UCAL_WEEK_OF_YEAR`, this field's value does not depend
             on :meth:`Calendar.get_first_day_of_week` or
             :meth:`Calendar.get_minimal_days_in_first_week`.
             :attr:`UCAL_DAY_OF_MONTH` 1 through 7 always correspond to
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` 1; 8 through 15 correspond to
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` 2, and so on.
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` 0 indicates the week before
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` 1. Negative values count back
             from the end of the month, so the last Sunday of a month is
             specified as :attr:`UCAL_DAY_OF_WEEK` =
             :attr:`~UCalendarDaysOfWeek.UCAL_SUNDAY`,
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` = -1. Because negative values
             count backward they will usually be aligned differently within the
             month than positive values. For example, if a month has 31 days,
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` -1 will overlap
             :attr:`UCAL_DAY_OF_WEEK_IN_MONTH` 5 and the end of 4.
             )doc")
      .value("UCAL_AM_PM", UCAL_AM_PM, R"doc(
             Field number indicating whether the :attr:`UCAL_HOUR` is before or
             after noon.

             E.g., at 10:04:15.250 PM the AM_PM is PM.
             )doc")
      .value("UCAL_HOUR", UCAL_HOUR, R"doc(
             Field number indicating the hour of the morning or afternoon.

             :attr:`UCAL_HOUR` is used for the 12-hour clock. E.g., at
             10:04:15.250 PM the :attr:`UCAL_HOUR` is 10.
             )doc")
      .value("UCAL_HOUR_OF_DAY", UCAL_HOUR_OF_DAY, R"doc(
             Field number indicating the hour of the day.

             :attr:`UCAL_HOUR_OF_DAY` is used for the 24-hour clock. E.g., at
             10:04:15.250 PM the :attr:`UCAL_HOUR_OF_DAY` is 22.
             )doc")
      .value("UCAL_MINUTE", UCAL_MINUTE, R"doc(
             Field number indicating the minute within the hour.

             E.g., at 10:04:15.250 PM the :attr:`UCAL_MINUTE` is 4.
             )doc")
      .value("UCAL_SECOND", UCAL_SECOND, R"doc(
             Field number indicating the second within the minute.

             E.g., at 10:04:15.250 PM the :attr:`UCAL_SECOND` is 15.
             )doc")
      .value("UCAL_MILLISECOND", UCAL_MILLISECOND, R"doc(
             Field number indicating the millisecond within the second.

             E.g., at 10:04:15.250 PM the :attr:`UCAL_MILLISECOND` is 250.
             )doc")
      .value("UCAL_ZONE_OFFSET", UCAL_ZONE_OFFSET, R"doc(
             Field number indicating the raw offset from GMT in milliseconds.
             )doc")
      .value("UCAL_DST_OFFSET", UCAL_DST_OFFSET, R"doc(
             Field number indicating the daylight savings offset in
             milliseconds.
             )doc")
      .value("UCAL_YEAR_WOY", UCAL_YEAR_WOY, R"doc(
             Field number indicating the extended year corresponding to the
             :attr:`UCAL_WEEK_OF_YEAR` field.

             This may be one greater or less than the value of
             :attr:`UCAL_EXTENDED_YEAR`.
             )doc")
      .value("UCAL_DOW_LOCAL", UCAL_DOW_LOCAL, R"doc(
             Field number indicating the localized day of week.

             This will be a value from 1 to 7 inclusive, with 1 being the
             localized first day of the week.
             )doc")
      .value("UCAL_EXTENDED_YEAR", UCAL_EXTENDED_YEAR, R"doc(
             Year of this calendar system, encompassing all supra-year fields.

             For example, in Gregorian/Julian calendars, positive Extended
             Year values indicate years AD, 1 BC = 0 extended, 2 BC = -1
             extended, and so on.
             )doc")
      .value("UCAL_JULIAN_DAY", UCAL_JULIAN_DAY, R"doc(
             Field number indicating the modified Julian day number.

             This is different from the conventional Julian day number in two
             regards. First, it demarcates days at local zone midnight, rather
             than noon GMT. Second, it is a local number; that is, it depends
             on the local time zone. It can be thought of as a single number
             that encompasses all the date-related fields.
             )doc")
      .value("UCAL_MILLISECONDS_IN_DAY", UCAL_MILLISECONDS_IN_DAY, R"doc(
             Ranges from 0 to 23:59:59.999 (regardless of DST).

             This field behaves exactly like a composite of all time-related
             fields, not including the zone fields. As such, it also reflects
             discontinuities of those fields on DST transition days. On a day
             of DST onset, it will jump forward. On a day of DST cessation, it
             will jump backward. This reflects the fact that it backward. This
             reflects the fact that it must be combined with the
             :attr:`UCAL_DST_OFFSET` field to obtain a unique local time value.
             )doc")
      .value("UCAL_IS_LEAP_MONTH", UCAL_IS_LEAP_MONTH, R"doc(
             Whether or not the current month is a leap month (0 or 1).

             See the Chinese calendar for an example of this.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 73)
      .value("UCAL_ORDINAL_MONTH", UCAL_ORDINAL_MONTH, R"doc(
             Field number indicating the month.

             This is a calendar-specific value. Differ from
             :attr:`UCAL_MONTH`, this value is continuous and unique within a
             year and range from 0 to 11 or 0 to 12 depending on how many
             months in a year, the calendar system has leap month or not, and
             in leap year or not. It is the ordinal position of that month in
             the corresponding year of the calendar.
             For Chinese, Dangi, and Hebrew calendar, the range is 0 to 11 in
             non-leap years and 0 to 12 in leap years.
             For Coptic and Ethiopian calendar, the range is always 0 to 12.
             For other calendars supported by ICU now, the range is 0 to 11.
             When the number of months in a year of the identified calendar is
             variable, a different :attr:`UCAL_ORDINAL_MONTH` value can be used
             for dates that are part of the same named month in different years.
             For example, in the Hebrew calendar, "1 Nisan 5781" is associated
             with :attr:`UCAL_ORDINAL_MONTH` value 6 while
             "1 Nisan 5782" is associated with :attr:`UCAL_ORDINAL_MONTH` value
             7 because 5782 is a leap year and Nisan follows the insertion of
             Adar I.
             In Chinese calendar, "Year 4664 Month 6 Day 2" is associated with
             :attr:`UCAL_ORDINAL_MONTH` value 5 while "Year 4665 Month 6 Day 2"
             is associated with :attr:`UCAL_ORDINAL_MONTH` value 6 because 4665
             is a leap year and there is an extra "Leap Month 5" which
             associated with :attr:`UCAL_ORDINAL_MONTH` value 5 before
             "Month 6" of year 4664.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)
      .value("UCAL_FIELD_COUNT", UCAL_FIELD_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("UCAL_DAY_OF_MONTH", UCAL_DAY_OF_MONTH, R"doc(
             Field number indicating the day of the month.

             This is a synonym for :attr:`UCAL_DATE`. The first day of the
             month has value 1.
             )doc")
      .export_values();

  //
  // enum UCalendarDaysOfWeek
  //
  py::enum_<UCalendarDaysOfWeek>(m, "UCalendarDaysOfWeek", py::arithmetic(),
                                 R"doc(
Useful constant for days of week.

.. note::

    Calendar day-of-week is 1-based. Clients who create locale resources for
    the field of first-day-of-week should be aware of this. For instance, in US
    locale, first-day-of-week is set to 1, i.e., UCAL_SUNDAY.
    Possible days of the week in a UCalendar.
      )doc")
      .value("UCAL_SUNDAY", UCAL_SUNDAY, R"doc(
             Sunday.
             )doc")
      .value("UCAL_MONDAY", UCAL_MONDAY, R"doc(
             Monday.
             )doc")
      .value("UCAL_TUESDAY", UCAL_TUESDAY, R"doc(
             Tuesday.
             )doc")
      .value("UCAL_WEDNESDAY", UCAL_WEDNESDAY, R"doc(
             Wednesday.
             )doc")
      .value("UCAL_THURSDAY", UCAL_THURSDAY, R"doc(
             Thursday.
             )doc")
      .value("UCAL_FRIDAY", UCAL_FRIDAY, R"doc(
             Friday.
             )doc")
      .value("UCAL_SATURDAY", UCAL_SATURDAY, R"doc(
             Saturday.
             )doc")
      .export_values();

  //
  // enum UCalendarDisplayNameType
  //
  py::enum_<UCalendarDisplayNameType>(
      m, "UCalendarDisplayNameType", py::arithmetic(),
      "Possible formats for a UCalendar's display name.")
      .value("UCAL_STANDARD", UCAL_STANDARD, R"doc(
             Standard display name.
             )doc")
      .value("UCAL_SHORT_STANDARD", UCAL_SHORT_STANDARD, R"doc(
             Short standard display name.
             )doc")
      .value("UCAL_DST", UCAL_DST, R"doc(
             Daylight savings display name.
             )doc")
      .value("UCAL_SHORT_DST", UCAL_SHORT_DST,
             R"doc(
             Short daylight savings display name.
             )doc")
      .export_values();

  //
  // enum UCalendarLimitType
  //
  py::enum_<UCalendarLimitType>(m, "UCalendarLimitType", py::arithmetic(),
                                "Possible limit values for a UCalendar.")
      .value("UCAL_MINIMUM", UCAL_MINIMUM, R"doc(
             Minimum value.
             )doc")
      .value("UCAL_MAXIMUM", UCAL_MAXIMUM, R"doc(
             Maximum value.
             )doc")
      .value("UCAL_GREATEST_MINIMUM", UCAL_GREATEST_MINIMUM, R"doc(
             Greatest minimum value.
             )doc")
      .value("UCAL_LEAST_MAXIMUM", UCAL_LEAST_MAXIMUM, R"doc(
             Least maximum value.
             )doc")
      .value("UCAL_ACTUAL_MINIMUM", UCAL_ACTUAL_MINIMUM, R"doc(
             Actual minimum value.
             )doc")
      .value("UCAL_ACTUAL_MAXIMUM", UCAL_ACTUAL_MAXIMUM, R"doc(
             Actual maximum value.
             )doc")
      .export_values();

  //
  // enum UCalendarMonths
  //
  py::enum_<UCalendarMonths>(m, "UCalendarMonths", py::arithmetic(), R"doc(
Possible months in a UCalendar.

.. note::

    Calendar month is 0-based.
      )doc")
      .value("UCAL_JANUARY", UCAL_JANUARY, R"doc(
             January.
             )doc")
      .value("UCAL_FEBRUARY", UCAL_FEBRUARY, R"doc(
             February.
             )doc")
      .value("UCAL_MARCH", UCAL_MARCH, R"doc(
             March.
             )doc")
      .value("UCAL_APRIL", UCAL_APRIL, R"doc(
             April.
             )doc")
      .value("UCAL_MAY", UCAL_MAY, R"doc(
             May.
             )doc")
      .value("UCAL_JUNE", UCAL_JUNE, R"doc(
             June.
             )doc")
      .value("UCAL_JULY", UCAL_JULY, R"doc(
             July.
             )doc")
      .value("UCAL_AUGUST", UCAL_AUGUST, R"doc(
             August.
             )doc")
      .value("UCAL_SEPTEMBER", UCAL_SEPTEMBER, R"doc(
             September.
             )doc")
      .value("UCAL_OCTOBER", UCAL_OCTOBER, R"doc(
             October.
             )doc")
      .value("UCAL_NOVEMBER", UCAL_NOVEMBER, R"doc(
             November.
             )doc")
      .value("UCAL_DECEMBER", UCAL_DECEMBER, R"doc(
             December.
             )doc")
      .value("UCAL_UNDECIMBER", UCAL_UNDECIMBER, R"doc(
             Value of the :attr:`~UCalendarDateFields.UCAL_MONTH` field
             indicating the thirteenth month of the year.

             Although the Gregorian calendar does not use this value, lunar
             calendars do.
             )doc")
      .export_values();

  //
  // enum UCalendarType
  //
  py::enum_<UCalendarType>(m, "UCalendarType", py::arithmetic(),
                           "Possible types of UCalendar.")
      .value("UCAL_TRADITIONAL", UCAL_TRADITIONAL, R"doc(
             Despite the name, :attr:`UCAL_TRADITIONAL` designates the locale's
             default calendar, which may be the Gregorian calendar or some
             other calendar.
             )doc")
      .value("UCAL_DEFAULT", UCAL_DEFAULT, R"doc(
             A better name for :attr:`UCAL_TRADITIONAL`.
             )doc")
      .value("UCAL_GREGORIAN", UCAL_GREGORIAN, R"doc(
             Unambiguously designates the Gregorian calendar for the locale.
             )doc")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  //
  // enum UCalendarWallTimeOption
  //
  py::enum_<UCalendarWallTimeOption>(m, "UCalendarWallTimeOption",
                                     py::arithmetic(), R"doc(
Options for handling ambiguous wall time at time zone offset transitions.
      )doc")
      .value("UCAL_WALLTIME_LAST", UCAL_WALLTIME_LAST, R"doc(
             An ambiguous wall time to be interpreted as the latest.

             This option is valid for
             :attr:`~UCalendarAttribute.UCAL_REPEATED_WALL_TIME` and
             :attr:`~UCalendarAttribute.UCAL_SKIPPED_WALL_TIME`.
             )doc")
      .value("UCAL_WALLTIME_FIRST", UCAL_WALLTIME_FIRST, R"doc(
             An ambiguous wall time to be interpreted as the earliest.

             This option is valid for
             :attr:`~UCalendarAttribute.UCAL_REPEATED_WALL_TIME` and
             :attr:`~UCalendarAttribute.UCAL_SKIPPED_WALL_TIME`.
             )doc")
      .value("UCAL_WALLTIME_NEXT_VALID", UCAL_WALLTIME_NEXT_VALID, R"doc(
             An ambiguous wall time to be interpreted as the next valid wall
             time.

             This option is valid for
             :attr:`~UCalendarAttribute.UCAL_SKIPPED_WALL_TIME`.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  //
  // enum UCalendarWeekdayType
  //
  py::enum_<UCalendarWeekdayType>(
      m, "UCalendarWeekdayType", py::arithmetic(),
      "Weekday types, as returned by :meth:`Calendar.get_day_of_week_type`.")
      .value("UCAL_WEEKDAY", UCAL_WEEKDAY, R"doc(
             Designates a full weekday (no part of the day is included in the
             weekend).
             )doc")
      .value("UCAL_WEEKEND", UCAL_WEEKEND, R"doc(
             Designates a full weekend day (the entire day is included in the
             weekend).
             )doc")
      .value("UCAL_WEEKEND_ONSET", UCAL_WEEKEND_ONSET, R"doc(
             Designates a day that starts as a weekday and transitions to the
             weekend.

             Call :meth:`Calendar.get_weekend_transition` to get the time of
             transition.
             )doc")
      .value("UCAL_WEEKEND_CEASE", UCAL_WEEKEND_CEASE, R"doc(
             Designates a day that starts as the weekend and transitions to a
             weekday.

             Call :meth:`Calendar.get_weekend_transition` to get the time of
             transition.
             )doc")
      .export_values();

  //
  // enum USystemTimeZoneType
  //
  py::enum_<USystemTimeZoneType>(m, "USystemTimeZoneType", py::arithmetic(),
                                 R"doc(
System time zone type constants used by filtering zones in
:meth:`TimeZone.create_time_zone_id_enumeration`.
      )doc")
      .value("UCAL_ZONE_TYPE_ANY", UCAL_ZONE_TYPE_ANY, R"doc(
             Any system zones.
             )doc")
      .value("UCAL_ZONE_TYPE_CANONICAL", UCAL_ZONE_TYPE_CANONICAL, R"doc(
             Canonical system zones.
             )doc")
      .value("UCAL_ZONE_TYPE_CANONICAL_LOCATION",
             UCAL_ZONE_TYPE_CANONICAL_LOCATION, R"doc(
             Canonical system zones associated with actual locations.
             )doc")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  //
  // enum UTimeZoneLocalOption
  //
  py::enum_<UTimeZoneLocalOption>(m, "UTimeZoneLocalOption", py::arithmetic(),
                                  R"doc(
Options used by :meth:`BasicTimeZone.get_offset_from_local` to specify how to
interpret an input time when it does not exist, or when it is ambiguous, around
a time zone transition.
      )doc")
      .value("UCAL_TZ_LOCAL_FORMER", UCAL_TZ_LOCAL_FORMER, R"doc(
             An input time is always interpreted as local time before a time
             zone transition.
             )doc")
      .value("UCAL_TZ_LOCAL_LATTER", UCAL_TZ_LOCAL_LATTER, R"doc(
             An input time is always interpreted as local time after a time
             zone transition.
             )doc")
      .value("UCAL_TZ_LOCAL_STANDARD_FORMER", UCAL_TZ_LOCAL_STANDARD_FORMER,
             R"doc(
             An input time is interpreted as standard time when local time is
             switched to/from daylight saving time.

             When both sides of a time zone transition are standard time, or
             daylight saving time, the local time before the transition is
             used.
             )doc")
      .value("UCAL_TZ_LOCAL_STANDARD_LATTER", UCAL_TZ_LOCAL_STANDARD_LATTER,
             R"doc(
             An input time is interpreted as standard time when local time is
             switched to/from daylight saving time.

             When both sides of a time zone transition are standard time, or
             daylight saving time, the local time after the transition is used.
             )doc")
      .value("UCAL_TZ_LOCAL_DAYLIGHT_FORMER", UCAL_TZ_LOCAL_DAYLIGHT_FORMER,
             R"doc(
             An input time is interpreted as daylight saving time when local
             time is switched to/from standard time.

             When both sides of a time zone transition are standard time, or
             daylight saving time, the local time before the transition is
             used.
             )doc")
      .value("UCAL_TZ_LOCAL_DAYLIGHT_LATTER", UCAL_TZ_LOCAL_DAYLIGHT_LATTER,
             R"doc(
             An input time is interpreted as daylight saving time when local
             time is switched to/from standard time.

             When both sides of a time zone transition are standard time, or
             daylight saving time, the local time after the transition is used.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // enum UTimeZoneTransitionType
  //
  py::enum_<UTimeZoneTransitionType>(
      m, "UTimeZoneTransitionType", py::arithmetic(),
      "Time zone transition types for ``ucal_getTimeZoneTransitionDate()``.")
      .value("UCAL_TZ_TRANSITION_NEXT", UCAL_TZ_TRANSITION_NEXT, R"doc(
             Get the next transition after the current date, i.e. excludes
             the current date.
             )doc")
      .value("UCAL_TZ_TRANSITION_NEXT_INCLUSIVE",
             UCAL_TZ_TRANSITION_NEXT_INCLUSIVE, R"doc(
             Get the next transition on or after the current date, i.e. may
             include the current date.
             )doc")
      .value("UCAL_TZ_TRANSITION_PREVIOUS", UCAL_TZ_TRANSITION_PREVIOUS, R"doc(
             Get the previous transition before the current date, i.e.
             excludes the current date.
             )doc")
      .value("UCAL_TZ_TRANSITION_PREVIOUS_INCLUSIVE",
             UCAL_TZ_TRANSITION_PREVIOUS_INCLUSIVE, R"doc(
             Get the previous transition on or before the current date, i.e.
             may include the current date.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

  m.attr("UCAL_UNKNOWN_ZONE_ID") = UCAL_UNKNOWN_ZONE_ID;
}

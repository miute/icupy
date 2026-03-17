#include "main.hpp"
#include <unicode/udat.h>

void init_udat(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  //
  // enum UDateFormatBooleanAttribute
  //
  py::enum_<UDateFormatBooleanAttribute>(m, "UDateFormatBooleanAttribute",
                                         py::arithmetic(), R"doc(
:class:`DateFormat` boolean attributes.
      )doc")
      .value("UDAT_PARSE_ALLOW_WHITESPACE", UDAT_PARSE_ALLOW_WHITESPACE, R"doc(
             Indicates whether whitespace is allowed.

             Includes trailing dot tolerance.
             )doc")
      .value("UDAT_PARSE_ALLOW_NUMERIC", UDAT_PARSE_ALLOW_NUMERIC, R"doc(
             Indicates tolerance of numeric data when String data may be
             assumed.

             e.g.: :attr:`~UDateFormatField.UDAT_YEAR_NAME_FIELD`,
             :attr:`~UDateFormatField.UDAT_STANDALONE_MONTH_FIELD`,
             :attr:`~UDateFormatField.UDAT_DAY_OF_WEEK_FIELD`
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UDAT_PARSE_PARTIAL_LITERAL_MATCH",
             UDAT_PARSE_PARTIAL_LITERAL_MATCH, R"doc(
             Indicates tolerance of a partial literal match e.g.
             Accepting "--mon-02-march-2011" for a pattern of
             "'--: 'EEE-WW-MMMM-yyyy"
             )doc")
      .value("UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH",
             UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH, R"doc(
             Indicates tolerance of pattern mismatch between input data and
             specified format pattern.

             e.g. Accepting "September" for a month pattern of MMM ("Sep")
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  //
  // enum UDateFormatField
  //
  py::enum_<UDateFormatField>(m, "UDateFormatField", py::arithmetic(), R"doc(
:class:`FieldPosition` selector for format fields defined by
:class:`DateFormat`.
      )doc")
      .value("UDAT_ERA_FIELD", UDAT_ERA_FIELD, R"doc(
             :class:`FieldPosition` selector for 'G' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ERA` field.
             )doc")
      .value("UDAT_YEAR_FIELD", UDAT_YEAR_FIELD, R"doc(
             :class:`FieldPosition` selector for 'y' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_YEAR` field.
             )doc")
      .value("UDAT_MONTH_FIELD", UDAT_MONTH_FIELD, R"doc(
             :class:`FieldPosition` selector for 'M' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_MONTH`
             field.
             )doc")
      .value("UDAT_DATE_FIELD", UDAT_DATE_FIELD, R"doc(
             :class:`FieldPosition` selector for 'd' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_DATE` field.
             )doc")
      .value("UDAT_HOUR_OF_DAY1_FIELD", UDAT_HOUR_OF_DAY1_FIELD, R"doc(
             :class:`FieldPosition` selector for 'k' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_HOUR_OF_DAY`
             field.

             UDAT_HOUR_OF_DAY1_FIELD is used for the one-based 24-hour clock.
             For example, 23:59 + 01:00 results in 24:59.
             )doc")
      .value("UDAT_HOUR_OF_DAY0_FIELD", UDAT_HOUR_OF_DAY0_FIELD, R"doc(
             :class:`FieldPosition` selector for 'H' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_HOUR_OF_DAY`
             field.

             UDAT_HOUR_OF_DAY0_FIELD is used for the zero-based 24-hour clock.
             For example, 23:59 + 01:00 results in 00:59.
             )doc")
      .value("UDAT_MINUTE_FIELD", UDAT_MINUTE_FIELD, R"doc(
             :class:`FieldPosition` selector for 'm' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_MINUTE`
             field.
             )doc")
      .value("UDAT_SECOND_FIELD", UDAT_SECOND_FIELD, R"doc(
             :class:`FieldPosition` selector for 's' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_SECOND`
             field.
             )doc")
      .value("UDAT_FRACTIONAL_SECOND_FIELD", UDAT_FRACTIONAL_SECOND_FIELD,
             R"doc(
             :class:`FieldPosition` selector for 'S' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_MILLISECOND`
             field.

             .. note::

                 Time formats that use 'S' can display a maximum of three
                 significant digits for fractional seconds, corresponding to
                 millisecond resolution and a fractional seconds sub-pattern of
                 SSS. If the sub-pattern is S or SS, the fractional seconds
                 value will be truncated (not rounded) to the number of display
                 places specified. If the fractional seconds sub-pattern is
                 longer than SSS, the additional display places will be filled
                 with zeros.
             )doc")
      .value("UDAT_DAY_OF_WEEK_FIELD", UDAT_DAY_OF_WEEK_FIELD, R"doc(
             :class:`FieldPosition` selector for 'E' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_DAY_OF_WEEK`
             field.
             )doc")
      .value("UDAT_DAY_OF_YEAR_FIELD", UDAT_DAY_OF_YEAR_FIELD, R"doc(
             :class:`FieldPosition` selector for 'D' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_DAY_OF_YEAR`
             field.
             )doc")
      .value("UDAT_DAY_OF_WEEK_IN_MONTH_FIELD", UDAT_DAY_OF_WEEK_IN_MONTH_FIELD,
             R"doc(
             :class:`FieldPosition` selector for 'F' field alignment,
             corresponding to the
             :attr:`~UCalendarDateFields.UCAL_DAY_OF_WEEK_IN_MONTH` field.
             )doc")
      .value("UDAT_WEEK_OF_YEAR_FIELD", UDAT_WEEK_OF_YEAR_FIELD, R"doc(
             :class:`FieldPosition` selector for 'w' field alignment,
             corresponding to the
             :attr:`~UCalendarDateFields.UCAL_WEEK_OF_YEAR` field.
             )doc")
      .value("UDAT_WEEK_OF_MONTH_FIELD", UDAT_WEEK_OF_MONTH_FIELD, R"doc(
             :class:`FieldPosition` selector for 'W' field alignment,
             corresponding to the
             :attr:`~UCalendarDateFields.UCAL_WEEK_OF_MONTH` field.
             )doc")
      .value("UDAT_AM_PM_FIELD", UDAT_AM_PM_FIELD, R"doc(
             :class:`FieldPosition` selector for 'a' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_AM_PM`
             field.
             )doc")
      .value("UDAT_HOUR1_FIELD", UDAT_HOUR1_FIELD, R"doc(
             :class:`FieldPosition` selector for 'h' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_HOUR` field.

             UDAT_HOUR1_FIELD is used for the one-based 12-hour clock. For
             example, 11:30 PM + 1 hour results in 12:30 AM.
             )doc")
      .value("UDAT_HOUR0_FIELD", UDAT_HOUR0_FIELD, R"doc(
             :class:`FieldPosition` selector for 'K' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_HOUR` field.

             UDAT_HOUR0_FIELD is used for the zero-based 12-hour clock. For
             example, 11:30 PM + 1 hour results in 00:30 AM.
             )doc")
      .value("UDAT_TIMEZONE_FIELD", UDAT_TIMEZONE_FIELD, R"doc(
             :class:`FieldPosition` selector for 'z' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             and :attr:`~UCalendarDateFields.UCAL_DST_OFFSET` fields.
             )doc")
      .value("UDAT_YEAR_WOY_FIELD", UDAT_YEAR_WOY_FIELD, R"doc(
             :class:`FieldPosition` selector for 'Y' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_YEAR_WOY`
             field.
             )doc")
      .value("UDAT_DOW_LOCAL_FIELD", UDAT_DOW_LOCAL_FIELD, R"doc(
             :class:`FieldPosition` selector for 'e' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_DOW_LOCAL`
             field.
             )doc")
      .value("UDAT_EXTENDED_YEAR_FIELD", UDAT_EXTENDED_YEAR_FIELD, R"doc(
             :class:`FieldPosition` selector for 'u' field alignment,
             corresponding to the
             :attr:`~UCalendarDateFields.UCAL_EXTENDED_YEAR` field.
             )doc")
      .value("UDAT_JULIAN_DAY_FIELD", UDAT_JULIAN_DAY_FIELD, R"doc(
             :class:`FieldPosition` selector for 'g' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_JULIAN_DAY`
             field.
             )doc")
      .value("UDAT_MILLISECONDS_IN_DAY_FIELD", UDAT_MILLISECONDS_IN_DAY_FIELD,
             R"doc(
             :class:`FieldPosition` selector for 'A' field alignment,
             corresponding to the
             :attr:`~UCalendarDateFields.UCAL_MILLISECONDS_IN_DAY` field.
             )doc")
      .value("UDAT_TIMEZONE_RFC_FIELD", UDAT_TIMEZONE_RFC_FIELD, R"doc(
             :class:`FieldPosition` selector for 'Z' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             and :attr:`~UCalendarDateFields.UCAL_DST_OFFSET`
             fields.
             )doc")
      .value("UDAT_TIMEZONE_GENERIC_FIELD", UDAT_TIMEZONE_GENERIC_FIELD, R"doc(
             :class:`FieldPosition` selector for 'v' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             field.
             )doc")
      .value("UDAT_STANDALONE_DAY_FIELD", UDAT_STANDALONE_DAY_FIELD, R"doc(
             :class:`FieldPosition` selector for 'c' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_DOW_LOCAL`
             field.

             This displays the stand alone day name, if available.
             )doc")
      .value("UDAT_STANDALONE_MONTH_FIELD", UDAT_STANDALONE_MONTH_FIELD, R"doc(
             :class:`FieldPosition` selector for 'L' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_MONTH`
             field.

             This displays the stand alone month name, if available.
             )doc")
      .value("UDAT_QUARTER_FIELD", UDAT_QUARTER_FIELD, R"doc(
             :class:`FieldPosition` selector for 'Q' field alignment,
             corresponding to quarters.

             This is implemented using the
             :attr:`~UCalendarDateFields.UCAL_MONTH` field. This displays the
             quarter.
             )doc")
      .value("UDAT_STANDALONE_QUARTER_FIELD", UDAT_STANDALONE_QUARTER_FIELD,
             R"doc(
             :class:`FieldPosition` selector for the 'q' field alignment,
             corresponding to stand-alone quarters.

             This is implemented using the
             :attr:`~UCalendarDateFields.UCAL_MONTH` field. This displays the
             stand-alone quarter.
             )doc")
      .value("UDAT_TIMEZONE_SPECIAL_FIELD", UDAT_TIMEZONE_SPECIAL_FIELD, R"doc(
             :class:`FieldPosition` selector for 'V' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             field.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UDAT_YEAR_NAME_FIELD", UDAT_YEAR_NAME_FIELD, R"doc(
             :class:`FieldPosition` selector for 'U' field alignment,
             corresponding to cyclic year names.

             This is implemented using the
             :attr:`~UCalendarDateFields.UCAL_YEAR` field. This displays the
             cyclic year name, if available.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UDAT_TIMEZONE_LOCALIZED_GMT_OFFSET_FIELD",
             UDAT_TIMEZONE_LOCALIZED_GMT_OFFSET_FIELD, R"doc(
             :class:`FieldPosition` selector for 'O' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             and :attr:`~UCalendarDateFields.UCAL_DST_OFFSET` fields.

             This displays the localized GMT format.
             )doc")
      .value("UDAT_TIMEZONE_ISO_FIELD", UDAT_TIMEZONE_ISO_FIELD, R"doc(
             :class:`FieldPosition` selector for 'X' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             and :attr:`~UCalendarDateFields.UCAL_DST_OFFSET` fields.

             This displays the ISO 8601 local time offset format or UTC
             indicator ("Z").
             )doc")
      .value("UDAT_TIMEZONE_ISO_LOCAL_FIELD", UDAT_TIMEZONE_ISO_LOCAL_FIELD,
             R"doc(
             :class:`FieldPosition` selector for 'x' field alignment,
             corresponding to the :attr:`~UCalendarDateFields.UCAL_ZONE_OFFSET`
             and :attr:`~UCalendarDateFields.UCAL_DST_OFFSET` fields.

             This displays the ISO 8601 local time offset format.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
#if (U_ICU_VERSION_MAJOR_NUM >= 57)
      .value("UDAT_AM_PM_MIDNIGHT_NOON_FIELD", UDAT_AM_PM_MIDNIGHT_NOON_FIELD,
             R"doc(
             :class:`FieldPosition` selector for 'b' field alignment.

             Displays midnight and noon for 12am and 12pm, respectively, if
             available; otherwise fall back to AM / PM.
             )doc")
      .value("UDAT_FLEXIBLE_DAY_PERIOD_FIELD", UDAT_FLEXIBLE_DAY_PERIOD_FIELD,
             R"doc(
             :class:`FieldPosition` selector for 'B' field alignment.

             Displays flexible day periods, such as "in the morning", if
             available.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  //
  // enum UDateFormatHourCycle
  //
  py::enum_<UDateFormatHourCycle>(m, "UDateFormatHourCycle", py::arithmetic(),
                                  "Hour Cycle.")
      .value("UDAT_HOUR_CYCLE_11", UDAT_HOUR_CYCLE_11, R"doc(
             Hour in am/pm (0~11)
             )doc")
      .value("UDAT_HOUR_CYCLE_12", UDAT_HOUR_CYCLE_12, R"doc(
             Hour in am/pm (1~12)
             )doc")
      .value("UDAT_HOUR_CYCLE_23", UDAT_HOUR_CYCLE_23, R"doc(
             Hour in day (0~23)
             )doc")
      .value("UDAT_HOUR_CYCLE_24", UDAT_HOUR_CYCLE_24, R"doc(
             Hour in day (1~24)
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  //
  // enum UDateFormatStyle
  //
  py::enum_<UDateFormatStyle>(m, "UDateFormatStyle", py::arithmetic(),
                              "Possible date/time format styles.")
      .value("UDAT_FULL", UDAT_FULL, R"doc(
             Full style.
             )doc")
      .value("UDAT_LONG", UDAT_LONG, R"doc(
             Long style.
             )doc")
      .value("UDAT_MEDIUM", UDAT_MEDIUM, R"doc(
             Medium style.
             )doc")
      .value("UDAT_SHORT", UDAT_SHORT, R"doc(
             Short style.
             )doc")
      .value("UDAT_DEFAULT", UDAT_DEFAULT, R"doc(
             Default style.
             )doc")
      .value("UDAT_RELATIVE", UDAT_RELATIVE, R"doc(
             Bitfield for relative date.
             )doc")
      .value("UDAT_FULL_RELATIVE", UDAT_FULL_RELATIVE)
      .value("UDAT_LONG_RELATIVE", UDAT_LONG_RELATIVE)
      .value("UDAT_MEDIUM_RELATIVE", UDAT_MEDIUM_RELATIVE)
      .value("UDAT_SHORT_RELATIVE", UDAT_SHORT_RELATIVE)
      .value("UDAT_NONE", UDAT_NONE, R"doc(
             No style.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UDAT_PATTERN", UDAT_PATTERN,
             R"doc(
             Use the pattern given in the parameter to ``udat_open()``.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      .export_values();

  //
  // enum UDateFormatSymbolType
  //
  py::enum_<UDateFormatSymbolType>(m, "UDateFormatSymbolType", py::arithmetic(),
                                   "Possible types of date format symbols.")
      .value("UDAT_ERAS", UDAT_ERAS, R"doc(
             The era names, for example AD.
             )doc")
      .value("UDAT_MONTHS", UDAT_MONTHS, R"doc(
             The month names, for example February.
             )doc")
      .value("UDAT_SHORT_MONTHS", UDAT_SHORT_MONTHS, R"doc(
             The short month names, for example Feb.
             )doc")
      .value("UDAT_WEEKDAYS", UDAT_WEEKDAYS, R"doc(
             The CLDR-style format "wide" weekday names, for example Monday.
             )doc")
      .value("UDAT_SHORT_WEEKDAYS", UDAT_SHORT_WEEKDAYS, R"doc(
             The CLDR-style format "abbreviated" (not "short") weekday names,
             for example "Mon." For the CLDR-style format "short" weekday
             names, use UDAT_SHORTER_WEEKDAYS.
             )doc")
      .value("UDAT_AM_PMS", UDAT_AM_PMS, R"doc(
             The AM/PM names, for example AM.
             )doc")
      .value("UDAT_LOCALIZED_CHARS", UDAT_LOCALIZED_CHARS, R"doc(
             The localized characters.
             )doc")
      .value("UDAT_ERA_NAMES", UDAT_ERA_NAMES, R"doc(
             The long era names, for example Anno Domini.
             )doc")
      .value("UDAT_NARROW_MONTHS", UDAT_NARROW_MONTHS, R"doc(
             The narrow month names, for example F.
             )doc")
      .value("UDAT_NARROW_WEEKDAYS", UDAT_NARROW_WEEKDAYS, R"doc(
             The CLDR-style format "narrow" weekday names, for example "M".
             )doc")
      .value("UDAT_STANDALONE_MONTHS", UDAT_STANDALONE_MONTHS, R"doc(
             Standalone context versions of months.
             )doc")
      .value("UDAT_STANDALONE_SHORT_MONTHS", UDAT_STANDALONE_SHORT_MONTHS)
      .value("UDAT_STANDALONE_NARROW_MONTHS", UDAT_STANDALONE_NARROW_MONTHS)
      .value("UDAT_STANDALONE_WEEKDAYS", UDAT_STANDALONE_WEEKDAYS, R"doc(
             The CLDR-style stand-alone "wide" weekday names.
             )doc")
      .value("UDAT_STANDALONE_SHORT_WEEKDAYS", UDAT_STANDALONE_SHORT_WEEKDAYS,
             R"doc(
             The CLDR-style stand-alone "abbreviated" (not "short") weekday
             names.

             For the CLDR-style stand-alone "short" weekday names, use
             UDAT_STANDALONE_SHORTER_WEEKDAYS.
             )doc")
      .value("UDAT_STANDALONE_NARROW_WEEKDAYS", UDAT_STANDALONE_NARROW_WEEKDAYS,
             R"doc(
             The CLDR-style stand-alone "narrow" weekday names.
             )doc")
      .value("UDAT_QUARTERS", UDAT_QUARTERS, R"doc(
             The quarters, for example 1st Quarter.
             )doc")
      .value("UDAT_SHORT_QUARTERS", UDAT_SHORT_QUARTERS, R"doc(
             The short quarter names, for example Q1.
             )doc")
      .value("UDAT_STANDALONE_QUARTERS", UDAT_STANDALONE_QUARTERS, R"doc(
             Standalone context versions of quarters.
             )doc")
      .value("UDAT_STANDALONE_SHORT_QUARTERS", UDAT_STANDALONE_SHORT_QUARTERS)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UDAT_SHORTER_WEEKDAYS", UDAT_SHORTER_WEEKDAYS, R"doc(
             The CLDR-style short weekday names, e.g. "Su", "Mo", etc.

             These are named "SHORTER" to contrast with the constants using
             SHORT above, which actually get the CLDR-style abbreviated
             versions of the corresponding names.
             )doc")
      .value("UDAT_STANDALONE_SHORTER_WEEKDAYS",
             UDAT_STANDALONE_SHORTER_WEEKDAYS, R"doc(
             Standalone version of UDAT_SHORTER_WEEKDAYS.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UDAT_CYCLIC_YEARS_WIDE", UDAT_CYCLIC_YEARS_WIDE, R"doc(
             Cyclic year names (only supported for some calendars, and only
             for FORMAT usage; ``udat_setSymbols()`` not supported for
             UDAT_CYCLIC_YEARS_WIDE)
             )doc")
      .value("UDAT_CYCLIC_YEARS_ABBREVIATED", UDAT_CYCLIC_YEARS_ABBREVIATED,
             R"doc(
             Cyclic year names (only supported for some calendars, and only
             for FORMAT usage)
             )doc")
      .value("UDAT_CYCLIC_YEARS_NARROW", UDAT_CYCLIC_YEARS_NARROW, R"doc(
             Cyclic year names (only supported for some calendars, and only
             for FORMAT usage; ``udat_setSymbols()`` not supported for
             UDAT_CYCLIC_YEARS_NARROW)
             )doc")
      .value("UDAT_ZODIAC_NAMES_WIDE", UDAT_ZODIAC_NAMES_WIDE, R"doc(
             Calendar zodiac names (only supported for some calendars, and
             only for FORMAT usage; ``udat_setSymbols()`` not supported for
             UDAT_ZODIAC_NAMES_WIDE)
             )doc")
      .value("UDAT_ZODIAC_NAMES_ABBREVIATED", UDAT_ZODIAC_NAMES_ABBREVIATED,
             R"doc(
             Calendar zodiac names (only supported for some calendars, and
             only for FORMAT usage)
             )doc")
      .value("UDAT_ZODIAC_NAMES_NARROW", UDAT_ZODIAC_NAMES_NARROW, R"doc(
             Calendar zodiac names (only supported for some calendars, and only
             for FORMAT usage; ``udat_setSymbols()`` not supported for
             UDAT_ZODIAC_NAMES_NARROW)
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
      .value("UDAT_NARROW_QUARTERS", UDAT_NARROW_QUARTERS, R"doc(
             The narrow quarter names, for example 1.
             )doc")
      .value("UDAT_STANDALONE_NARROW_QUARTERS", UDAT_STANDALONE_NARROW_QUARTERS,
             R"doc(
             The narrow standalone quarter names, for example 1.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
#if (U_ICU_VERSION_MAJOR_NUM >= 78)
      .value("UDAT_AM_PMS_NARROW", UDAT_AM_PMS_NARROW, R"doc(
             Narrow AM/PM names. For example: "A" and "P".
             )doc")
      .value("UDAT_AM_PMS_WIDE", UDAT_AM_PMS_WIDE, R"doc(
             Wide AM/PM names. For example: "Ante Meridiem" and
             "Post Meridiem".
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)
      .export_values();

  // Skeletons for dates
  m.attr("UDAT_ABBR_MONTH_DAY") = UDAT_ABBR_MONTH_DAY;
  m.attr("UDAT_ABBR_MONTH_WEEKDAY_DAY") = UDAT_ABBR_MONTH_WEEKDAY_DAY;
  m.attr("UDAT_ABBR_MONTH") = UDAT_ABBR_MONTH;
  m.attr("UDAT_DAY") = UDAT_DAY;
  m.attr("UDAT_MONTH_DAY") = UDAT_MONTH_DAY;
  m.attr("UDAT_MONTH_WEEKDAY_DAY") = UDAT_MONTH_WEEKDAY_DAY;
  m.attr("UDAT_MONTH") = UDAT_MONTH;
  m.attr("UDAT_NUM_MONTH_DAY") = UDAT_NUM_MONTH_DAY;
  m.attr("UDAT_NUM_MONTH_WEEKDAY_DAY") = UDAT_NUM_MONTH_WEEKDAY_DAY;
  m.attr("UDAT_NUM_MONTH") = UDAT_NUM_MONTH;
  m.attr("UDAT_YEAR_ABBR_MONTH_DAY") = UDAT_YEAR_ABBR_MONTH_DAY;
  m.attr("UDAT_YEAR_ABBR_MONTH_WEEKDAY_DAY") = UDAT_YEAR_ABBR_MONTH_WEEKDAY_DAY;
  m.attr("UDAT_YEAR_ABBR_MONTH") = UDAT_YEAR_ABBR_MONTH;
  m.attr("UDAT_YEAR_ABBR_QUARTER") = UDAT_YEAR_ABBR_QUARTER;
  m.attr("UDAT_YEAR_MONTH_DAY") = UDAT_YEAR_MONTH_DAY;
  m.attr("UDAT_YEAR_MONTH_WEEKDAY_DAY") = UDAT_YEAR_MONTH_WEEKDAY_DAY;
  m.attr("UDAT_YEAR_MONTH") = UDAT_YEAR_MONTH;
  m.attr("UDAT_YEAR_NUM_MONTH_DAY") = UDAT_YEAR_NUM_MONTH_DAY;
  m.attr("UDAT_YEAR_NUM_MONTH_WEEKDAY_DAY") = UDAT_YEAR_NUM_MONTH_WEEKDAY_DAY;
  m.attr("UDAT_YEAR_NUM_MONTH") = UDAT_YEAR_NUM_MONTH;
  m.attr("UDAT_YEAR_QUARTER") = UDAT_YEAR_QUARTER;
  m.attr("UDAT_YEAR") = UDAT_YEAR;

  // Skeletons for times
  m.attr("UDAT_HOUR_MINUTE_SECOND") = UDAT_HOUR_MINUTE_SECOND;
  m.attr("UDAT_HOUR_MINUTE") = UDAT_HOUR_MINUTE;
  m.attr("UDAT_HOUR") = UDAT_HOUR;
  m.attr("UDAT_HOUR24_MINUTE_SECOND") = UDAT_HOUR24_MINUTE_SECOND;
  m.attr("UDAT_HOUR24_MINUTE") = UDAT_HOUR24_MINUTE;
  m.attr("UDAT_MINUTE_SECOND") = UDAT_MINUTE_SECOND;

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  // Skeletons for dates
  m.attr("UDAT_ABBR_QUARTER") = UDAT_ABBR_QUARTER;
  m.attr("UDAT_ABBR_WEEKDAY") = UDAT_ABBR_WEEKDAY;
  m.attr("UDAT_QUARTER") = UDAT_QUARTER;
  m.attr("UDAT_WEEKDAY") = UDAT_WEEKDAY;

  // Skeletons for times
  m.attr("UDAT_HOUR24") = UDAT_HOUR24;
  m.attr("UDAT_MINUTE") = UDAT_MINUTE;
  m.attr("UDAT_SECOND") = UDAT_SECOND;

  // Skeletons for time zones
  m.attr("UDAT_ABBR_GENERIC_TZ") = UDAT_ABBR_GENERIC_TZ;
  m.attr("UDAT_ABBR_SPECIFIC_TZ") = UDAT_ABBR_SPECIFIC_TZ;
  m.attr("UDAT_ABBR_UTC_TZ") = UDAT_ABBR_UTC_TZ;
  m.attr("UDAT_GENERIC_TZ") = UDAT_GENERIC_TZ;
  m.attr("UDAT_LOCATION_TZ") = UDAT_LOCATION_TZ;
  m.attr("UDAT_SPECIFIC_TZ") = UDAT_SPECIFIC_TZ;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

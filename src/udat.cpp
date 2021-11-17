#include "main.hpp"
#include <unicode/udat.h>

void init_udat(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  py::enum_<UDateFormatBooleanAttribute>(m, "UDateFormatBooleanAttribute", py::arithmetic(),
                                         "*DateFormat* boolean attributes.")
      .value("UDAT_PARSE_ALLOW_WHITESPACE", UDAT_PARSE_ALLOW_WHITESPACE,
             "Indicates whether whitespace is allowed.\n\n  "
             "Includes trailing dot tolerance.")
      .value("UDAT_PARSE_ALLOW_NUMERIC", UDAT_PARSE_ALLOW_NUMERIC,
             "Indicates tolerance of numeric data when String data may be assumed.\n\n  "
             "e.g.: *UDAT_YEAR_NAME_FIELD*, *UDAT_STANDALONE_MONTH_FIELD*, *UDAT_DAY_OF_WEEK_FIELD*")
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UDAT_PARSE_PARTIAL_LITERAL_MATCH", UDAT_PARSE_PARTIAL_LITERAL_MATCH,
             "Indicates tolerance of a partial literal match e.g.\n\n  "
             "Accepting \"--mon-02-march-2011\" for a pattern of \"'--: 'EEE-WW-MMMM-yyyy\"")
      .value("UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH", UDAT_PARSE_MULTIPLE_PATTERNS_FOR_MATCH,
             "Indicates tolerance of pattern mismatch between input data and specified format pattern.\n\n  "
             "e.g. Accepting \"September\" for a month pattern of MMM (\"Sep\")")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  py::enum_<UDateFormatField>(
      m, "UDateFormatField", py::arithmetic(),
      "*FieldPosition* and *UFieldPosition* selectors for format fields defined by *DateFormat* and *UDateFormat*.")
      .value("UDAT_ERA_FIELD", UDAT_ERA_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'G' field alignment, corresponding to the *UCAL_ERA* "
             "field.")
      .value("UDAT_YEAR_FIELD", UDAT_YEAR_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'y' field alignment, corresponding to the *UCAL_YEAR* "
             "field.")
      .value("UDAT_MONTH_FIELD", UDAT_MONTH_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'M' field alignment, corresponding to the *UCAL_MONTH* "
             "field.")
      .value("UDAT_DATE_FIELD", UDAT_DATE_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'd' field alignment, corresponding to the *UCAL_DATE* "
             "field.")
      .value("UDAT_HOUR_OF_DAY1_FIELD", UDAT_HOUR_OF_DAY1_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'k' field alignment, corresponding to the "
             "*UCAL_HOUR_OF_DAY* field.\n\n  "
             "*UDAT_HOUR_OF_DAY1_FIELD* is used for the one-based 24-hour clock. For example, 23:59 + 01:00 results in "
             "24:59.")
      .value("UDAT_HOUR_OF_DAY0_FIELD", UDAT_HOUR_OF_DAY0_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'H' field alignment, corresponding to the "
             "*UCAL_HOUR_OF_DAY* field.\n\n  "
             "*UDAT_HOUR_OF_DAY0_FIELD* is used for the zero-based 24-hour clock. For example, 23:59 + 01:00 results "
             "in 00:59.")
      .value("UDAT_MINUTE_FIELD", UDAT_MINUTE_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'm' field alignment, corresponding to the "
             "*UCAL_MINUTE* field.")
      .value("UDAT_SECOND_FIELD", UDAT_SECOND_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 's' field alignment, corresponding to the "
             "*UCAL_SECOND* field.")
      .value("UDAT_FRACTIONAL_SECOND_FIELD", UDAT_FRACTIONAL_SECOND_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'S' field alignment, corresponding to the "
             "*UCAL_MILLISECOND* field.\n\n  "
             "Note: Time formats that use 'S' can display a maximum of three significant digits for fractional "
             "seconds, corresponding to millisecond resolution and a fractional seconds sub-pattern of SSS. If the "
             "sub-pattern is S or SS, the fractional seconds value will be truncated (not rounded) to the number of "
             "display places specified. If the fractional seconds sub-pattern is longer than SSS, the additional "
             "display places will be filled with zeros.")
      .value("UDAT_DAY_OF_WEEK_FIELD", UDAT_DAY_OF_WEEK_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'E' field alignment, corresponding to the "
             "*UCAL_DAY_OF_WEEK* field.")
      .value("UDAT_DAY_OF_YEAR_FIELD", UDAT_DAY_OF_YEAR_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'D' field alignment, corresponding to the "
             "*UCAL_DAY_OF_YEAR* field.")
      .value("UDAT_DAY_OF_WEEK_IN_MONTH_FIELD", UDAT_DAY_OF_WEEK_IN_MONTH_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'F' field alignment, corresponding to the "
             "*UCAL_DAY_OF_WEEK_IN_MONTH* field.")
      .value("UDAT_WEEK_OF_YEAR_FIELD", UDAT_WEEK_OF_YEAR_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'w' field alignment, corresponding to the "
             "*UCAL_WEEK_OF_YEAR* field.")
      .value("UDAT_WEEK_OF_MONTH_FIELD", UDAT_WEEK_OF_MONTH_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'W' field alignment, corresponding to the "
             "*UCAL_WEEK_OF_MONTH* field.")
      .value("UDAT_AM_PM_FIELD", UDAT_AM_PM_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'a' field alignment, corresponding to the *UCAL_AM_PM* "
             "field.")
      .value("UDAT_HOUR1_FIELD", UDAT_HOUR1_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'h' field alignment, corresponding to the *UCAL_HOUR* "
             "field.\n\n  "
             "*UDAT_HOUR1_FIELD* is used for the one-based 12-hour clock. For example, 11:30 PM + 1 hour results in "
             "12:30 AM.")
      .value("UDAT_HOUR0_FIELD", UDAT_HOUR0_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'K' field alignment, corresponding to the *UCAL_HOUR* "
             "field.\n\n  "
             "*UDAT_HOUR0_FIELD* is used for the zero-based 12-hour clock. For example, 11:30 PM + 1 hour results in "
             "00:30 AM.")
      .value("UDAT_TIMEZONE_FIELD", UDAT_TIMEZONE_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'z' field alignment, corresponding to the "
             "*UCAL_ZONE_OFFSET* and *UCAL_DST_OFFSET* fields.")
      .value("UDAT_YEAR_WOY_FIELD", UDAT_YEAR_WOY_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'Y' field alignment, corresponding to the "
             "*UCAL_YEAR_WOY* field.")
      .value("UDAT_DOW_LOCAL_FIELD", UDAT_DOW_LOCAL_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'e' field alignment, corresponding to the "
             "*UCAL_DOW_LOCAL* field.")
      .value("UDAT_EXTENDED_YEAR_FIELD", UDAT_EXTENDED_YEAR_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'u' field alignment, corresponding to the "
             "*UCAL_EXTENDED_YEAR* field.")
      .value("UDAT_JULIAN_DAY_FIELD", UDAT_JULIAN_DAY_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'g' field alignment, corresponding to the "
             "*UCAL_JULIAN_DAY* field.")
      .value("UDAT_MILLISECONDS_IN_DAY_FIELD", UDAT_MILLISECONDS_IN_DAY_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'A' field alignment, corresponding to the "
             "*UCAL_MILLISECONDS_IN_DAY* field.")
      .value("UDAT_TIMEZONE_RFC_FIELD", UDAT_TIMEZONE_RFC_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'Z' field alignment, corresponding to the "
             "*UCAL_ZONE_OFFSET* and *UCAL_DST_OFFSET* fields.")
      .value("UDAT_TIMEZONE_GENERIC_FIELD", UDAT_TIMEZONE_GENERIC_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'v' field alignment, corresponding to the "
             "*UCAL_ZONE_OFFSET* field.")
      .value("UDAT_STANDALONE_DAY_FIELD", UDAT_STANDALONE_DAY_FIELD,
             "*FieldPosition* selector for 'c' field alignment, corresponding to the *UCAL_DOW_LOCAL* field.\n\n  "
             "This displays the stand alone day name, if available.")
      .value("UDAT_STANDALONE_MONTH_FIELD", UDAT_STANDALONE_MONTH_FIELD,
             "*FieldPosition* selector for 'L' field alignment, corresponding to the *UCAL_MONTH* field.\n\n  "
             "This displays the stand alone month name, if available.")
      .value("UDAT_QUARTER_FIELD", UDAT_QUARTER_FIELD,
             "*FieldPosition* selector for \"Q\" field alignment, corresponding to quarters.\n\n  "
             "This is implemented using the *UCAL_MONTH* field. This displays the quarter.")
      .value("UDAT_STANDALONE_QUARTER_FIELD", UDAT_STANDALONE_QUARTER_FIELD,
             "*FieldPosition* selector for the \"q\" field alignment, corresponding to stand-alone quarters.\n\n  "
             "This is implemented using the *UCAL_MONTH* field. This displays the stand-alone quarter.")
      .value("UDAT_TIMEZONE_SPECIAL_FIELD", UDAT_TIMEZONE_SPECIAL_FIELD,
             "*FieldPosition* and *UFieldPosition* selector for 'V' field alignment, corresponding to the "
             "*UCAL_ZONE_OFFSET* field.")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UDAT_YEAR_NAME_FIELD", UDAT_YEAR_NAME_FIELD,
             "*FieldPosition* selector for \"U\" field alignment, corresponding to cyclic year names.\n\n  "
             "This is implemented using the *UCAL_YEAR* field. This displays the cyclic year name, if available.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UDAT_TIMEZONE_LOCALIZED_GMT_OFFSET_FIELD", UDAT_TIMEZONE_LOCALIZED_GMT_OFFSET_FIELD,
             "*FieldPosition* selector for 'O' field alignment, corresponding to the *UCAL_ZONE_OFFSET* and "
             "*UCAL_DST_OFFSET* fields.\n\n  "
             "This displays the localized GMT format.")
      .value("UDAT_TIMEZONE_ISO_FIELD", UDAT_TIMEZONE_ISO_FIELD,
             "*FieldPosition* selector for 'X' field alignment, corresponding to the *UCAL_ZONE_OFFSET* and "
             "*UCAL_DST_OFFSET* fields.\n\n  "
             "This displays the ISO 8601 local time offset format or UTC indicator (\"Z\").")
      .value("UDAT_TIMEZONE_ISO_LOCAL_FIELD", UDAT_TIMEZONE_ISO_LOCAL_FIELD,
             "*FieldPosition* selector for 'x' field alignment, corresponding to the *UCAL_ZONE_OFFSET* and "
             "*UCAL_DST_OFFSET* fields.\n\n  "
             "This displays the ISO 8601 local time offset format.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
      .value("UDAT_AM_PM_MIDNIGHT_NOON_FIELD", UDAT_AM_PM_MIDNIGHT_NOON_FIELD,
             "*FieldPosition* selector for 'b' field alignment.\n\n  "
             "Displays midnight and noon for 12am and 12pm, respectively, if available; otherwise fall back to AM / "
             "PM.")
      .value("UDAT_FLEXIBLE_DAY_PERIOD_FIELD", UDAT_FLEXIBLE_DAY_PERIOD_FIELD,
             "*FieldPosition* selector for 'B' field alignment.\n\n  "
             "Displays flexible day periods, such as \"in the morning\", if available.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  py::enum_<UDateFormatHourCycle>(m, "UDateFormatHourCycle", py::arithmetic(), "Hour Cycle.")
      .value("UDAT_HOUR_CYCLE_11", UDAT_HOUR_CYCLE_11, "Hour in am/pm (0~11)")
      .value("UDAT_HOUR_CYCLE_12", UDAT_HOUR_CYCLE_12, "Hour in am/pm (1~12)")
      .value("UDAT_HOUR_CYCLE_23", UDAT_HOUR_CYCLE_23, "Hour in day (0~23)")
      .value("UDAT_HOUR_CYCLE_24", UDAT_HOUR_CYCLE_24, "Hour in day (1~24)")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  py::enum_<UDateFormatStyle>(m, "UDateFormatStyle", py::arithmetic(), "The possible date/time format styles.")
      .value("UDAT_FULL", UDAT_FULL, "Full style.")
      .value("UDAT_LONG", UDAT_LONG, "Long style.")
      .value("UDAT_MEDIUM", UDAT_MEDIUM, "Medium style.")
      .value("UDAT_SHORT", UDAT_SHORT, "Short style.")
      .value("UDAT_DEFAULT", UDAT_DEFAULT, "Default style.")
      .value("UDAT_RELATIVE", UDAT_RELATIVE, "Bitfield for relative date.")
      .value("UDAT_FULL_RELATIVE", UDAT_FULL_RELATIVE)
      .value("UDAT_LONG_RELATIVE", UDAT_LONG_RELATIVE)
      .value("UDAT_MEDIUM_RELATIVE", UDAT_MEDIUM_RELATIVE)
      .value("UDAT_SHORT_RELATIVE", UDAT_SHORT_RELATIVE)
      .value("UDAT_NONE", UDAT_NONE, "No style.")
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UDAT_PATTERN", UDAT_PATTERN, "Use the pattern given in the parameter to *udat_open*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      .export_values();

  py::enum_<UDateFormatSymbolType>(m, "UDateFormatSymbolType", py::arithmetic(),
                                   "The possible types of date format symbols.")
      .value("UDAT_ERAS", UDAT_ERAS, "The era names, for example AD.")
      .value("UDAT_MONTHS", UDAT_MONTHS, "The month names, for example February.")
      .value("UDAT_SHORT_MONTHS", UDAT_SHORT_MONTHS, "The short month names, for example Feb.")
      .value("UDAT_WEEKDAYS", UDAT_WEEKDAYS, "The CLDR-style format \"wide\" weekday names, for example Monday.")
      .value("UDAT_SHORT_WEEKDAYS", UDAT_SHORT_WEEKDAYS,
             "The CLDR-style format \"abbreviated\" (not \"short\") weekday names, for example \"Mon.\" For the "
             "CLDR-style format \"short\" weekday names, use *UDAT_SHORTER_WEEKDAYS*.")
      .value("UDAT_AM_PMS", UDAT_AM_PMS, "The AM/PM names, for example AM.")
      .value("UDAT_LOCALIZED_CHARS", UDAT_LOCALIZED_CHARS, "The localized characters.")
      .value("UDAT_ERA_NAMES", UDAT_ERA_NAMES, "The long era names, for example Anno Domini.")
      .value("UDAT_NARROW_MONTHS", UDAT_NARROW_MONTHS, "The narrow month names, for example F.")
      .value("UDAT_NARROW_WEEKDAYS", UDAT_NARROW_WEEKDAYS,
             "The CLDR-style format \"narrow\" weekday names, for example \"M\".")
      .value("UDAT_STANDALONE_MONTHS", UDAT_STANDALONE_MONTHS, "Standalone context versions of months.")
      .value("UDAT_STANDALONE_SHORT_MONTHS", UDAT_STANDALONE_SHORT_MONTHS)
      .value("UDAT_STANDALONE_NARROW_MONTHS", UDAT_STANDALONE_NARROW_MONTHS)
      .value("UDAT_STANDALONE_WEEKDAYS", UDAT_STANDALONE_WEEKDAYS, "The CLDR-style stand-alone \"wide\" weekday names.")
      .value("UDAT_STANDALONE_SHORT_WEEKDAYS", UDAT_STANDALONE_SHORT_WEEKDAYS,
             "The CLDR-style stand-alone \"abbreviated\" (not \"short\") weekday names.\n\n  "
             "For the CLDR-style stand-alone \"short\" weekday names, use *UDAT_STANDALONE_SHORTER_WEEKDAYS*.")
      .value("UDAT_STANDALONE_NARROW_WEEKDAYS", UDAT_STANDALONE_NARROW_WEEKDAYS,
             "The CLDR-style stand-alone \"narrow\" weekday names.")
      .value("UDAT_QUARTERS", UDAT_QUARTERS, "The quarters, for example 1st Quarter.")
      .value("UDAT_SHORT_QUARTERS", UDAT_SHORT_QUARTERS, "The short quarter names, for example Q1.")
      .value("UDAT_STANDALONE_QUARTERS", UDAT_STANDALONE_QUARTERS, "Standalone context versions of quarters.")
      .value("UDAT_STANDALONE_SHORT_QUARTERS", UDAT_STANDALONE_SHORT_QUARTERS)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UDAT_SHORTER_WEEKDAYS", UDAT_SHORTER_WEEKDAYS,
             "The CLDR-style short weekday names, e.g. \"Su\", \"Mo\", etc.\n\n  "
             "These are named \"SHORTER\" to contrast with the constants using SHORT above, which actually get the "
             "CLDR-style abbreviated versions of the corresponding names.")
      .value("UDAT_STANDALONE_SHORTER_WEEKDAYS", UDAT_STANDALONE_SHORTER_WEEKDAYS,
             "Standalone version of *UDAT_SHORTER_WEEKDAYS*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UDAT_CYCLIC_YEARS_WIDE", UDAT_CYCLIC_YEARS_WIDE,
             "Cyclic year names (only supported for some calendars, and only for FORMAT usage; *udat_set_symbols* "
             "not supported for *UDAT_CYCLIC_YEARS_WIDE*)")
      .value("UDAT_CYCLIC_YEARS_ABBREVIATED", UDAT_CYCLIC_YEARS_ABBREVIATED,
             "Cyclic year names (only supported for some calendars, and only for FORMAT usage)")
      .value("UDAT_CYCLIC_YEARS_NARROW", UDAT_CYCLIC_YEARS_NARROW,
             "Cyclic year names (only supported for some calendars, and only for FORMAT usage; *udat_set_symbols* "
             "not supported for *UDAT_CYCLIC_YEARS_NARROW*)")
      .value("UDAT_ZODIAC_NAMES_WIDE", UDAT_ZODIAC_NAMES_WIDE,
             "Calendar zodiac names (only supported for some calendars, and only for FORMAT usage; "
             "*udat_set_symbols* not supported for *UDAT_ZODIAC_NAMES_WIDE*)")
      .value("UDAT_ZODIAC_NAMES_ABBREVIATED", UDAT_ZODIAC_NAMES_ABBREVIATED,
             "Calendar zodiac names (only supported for some calendars, and only for FORMAT usage)")
      .value("UDAT_ZODIAC_NAMES_NARROW", UDAT_ZODIAC_NAMES_NARROW,
             "Calendar zodiac names (only supported for some calendars, and only for FORMAT usage; "
             "*udat_set_symbols* not supported for *UDAT_ZODIAC_NAMES_NARROW*)")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
      .value("UDAT_NARROW_QUARTERS", UDAT_NARROW_QUARTERS, "The narrow quarter names, for example 1.")
      .value("UDAT_STANDALONE_NARROW_QUARTERS", UDAT_STANDALONE_NARROW_QUARTERS,
             "The narrow standalone quarter names, for example 1.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
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

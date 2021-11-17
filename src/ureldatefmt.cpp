#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
#include <unicode/ureldatefmt.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

void init_ureldatefmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  py::enum_<UDateRelativeDateTimeFormatterStyle>(m, "UDateRelativeDateTimeFormatterStyle", py::arithmetic(),
                                                 "The formatting style.")
      .value("UDAT_STYLE_LONG", UDAT_STYLE_LONG, "Everything spelled out.")
      .value("UDAT_STYLE_SHORT", UDAT_STYLE_SHORT, "Abbreviations used when possible.")
      .value("UDAT_STYLE_NARROW", UDAT_STYLE_NARROW, "Use the shortest possible form.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UDAT_STYLE_COUNT", UDAT_STYLE_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  py::enum_<URelativeDateTimeFormatterField>(
      m, "URelativeDateTimeFormatterField", py::arithmetic(),
      "*FieldPosition* and *UFieldPosition* selectors for format fields defined by *RelativeDateTimeFormatter*.")
      .value("UDAT_REL_LITERAL_FIELD", UDAT_REL_LITERAL_FIELD,
             "Represents a literal text string, like \"tomorrow\" or \"days ago\".")
      .value("UDAT_REL_NUMERIC_FIELD", UDAT_REL_NUMERIC_FIELD,
             "Represents a number quantity, like \"3\" in \"3 days ago\".")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  py::enum_<URelativeDateTimeUnit>(m, "URelativeDateTimeUnit", py::arithmetic(),
                                   "Represents the unit for formatting a relative date.\n\n"
                                   "e.g. \"in 5 days\" or \"next year\"")
      .value("UDAT_REL_UNIT_YEAR", UDAT_REL_UNIT_YEAR,
             "Specifies that relative unit is year, e.g. \"last year\", \"in 5 years\".")
      .value("UDAT_REL_UNIT_QUARTER", UDAT_REL_UNIT_QUARTER,
             "Specifies that relative unit is quarter, e.g. \"last quarter\", \"in 5 quarters\".")
      .value("UDAT_REL_UNIT_MONTH", UDAT_REL_UNIT_MONTH,
             "Specifies that relative unit is month, e.g. \"last month\", \"in 5 months\".")
      .value("UDAT_REL_UNIT_WEEK", UDAT_REL_UNIT_WEEK,
             "Specifies that relative unit is week, e.g. \"last week\", \"in 5 weeks\".")
      .value("UDAT_REL_UNIT_DAY", UDAT_REL_UNIT_DAY,
             "Specifies that relative unit is day, e.g. \"yesterday\", \"in 5 days\".")
      .value("UDAT_REL_UNIT_HOUR", UDAT_REL_UNIT_HOUR,
             "Specifies that relative unit is hour, e.g. \"1 hour ago\", \"in 5 hours\".")
      .value("UDAT_REL_UNIT_MINUTE", UDAT_REL_UNIT_MINUTE,
             "Specifies that relative unit is minute, e.g. \"1 minute ago\", \"in 5 minutes\".")
      .value("UDAT_REL_UNIT_SECOND", UDAT_REL_UNIT_SECOND,
             "Specifies that relative unit is second, e.g. \"1 second ago\", \"in 5 seconds\".")
      .value("UDAT_REL_UNIT_SUNDAY", UDAT_REL_UNIT_SUNDAY,
             "Specifies that relative unit is Sunday, e.g. \"last Sunday\", \"this Sunday\", \"next Sunday\", \"in 5 "
             "Sundays\".")
      .value("UDAT_REL_UNIT_MONDAY", UDAT_REL_UNIT_MONDAY,
             "Specifies that relative unit is Monday, e.g. \"last Monday\", \"this Monday\", \"next Monday\", \"in 5 "
             "Mondays\".")
      .value("UDAT_REL_UNIT_TUESDAY", UDAT_REL_UNIT_TUESDAY,
             "Specifies that relative unit is Tuesday, e.g. \"last Tuesday\", \"this Tuesday\", \"next Tuesday\", \"in "
             "5 Tuesdays\".")
      .value("UDAT_REL_UNIT_WEDNESDAY", UDAT_REL_UNIT_WEDNESDAY,
             "Specifies that relative unit is Wednesday, e.g. \"last Wednesday\", \"this Wednesday\", \"next "
             "Wednesday\", \"in 5 Wednesdays\".")
      .value("UDAT_REL_UNIT_THURSDAY", UDAT_REL_UNIT_THURSDAY,
             "Specifies that relative unit is Thursday, e.g. \"last Thursday\", \"this Thursday\", \"next Thursday\", "
             "\"in 5 Thursdays\".")
      .value("UDAT_REL_UNIT_FRIDAY", UDAT_REL_UNIT_FRIDAY,
             "Specifies that relative unit is Friday, e.g. \"last Friday\", \"this Friday\", \"next Friday\", \"in 5 "
             "Fridays\".")
      .value("UDAT_REL_UNIT_SATURDAY", UDAT_REL_UNIT_SATURDAY,
             "Specifies that relative unit is Saturday, e.g. \"last Saturday\", \"this Saturday\", \"next Saturday\", "
             "\"in 5 Saturdays\".")
#ifndef U_HIDE_DEPRECATED_API
      .value("UDAT_REL_UNIT_COUNT", UDAT_REL_UNIT_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)
}

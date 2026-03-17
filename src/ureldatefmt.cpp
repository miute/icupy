#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
#include <unicode/ureldatefmt.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

void init_ureldatefmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  //
  // UDateRelativeDateTimeFormatterStyle
  //
  py::enum_<UDateRelativeDateTimeFormatterStyle>(
      m, "UDateRelativeDateTimeFormatterStyle", py::arithmetic(), R"doc(
Formatting style for :class:`RelativeDateTimeFormatter`.
      )doc")
      .value("UDAT_STYLE_LONG", UDAT_STYLE_LONG, R"doc(
             Everything spelled out.
             )doc")
      .value("UDAT_STYLE_SHORT", UDAT_STYLE_SHORT, R"doc(
             Abbreviations used when possible.
             )doc")
      .value("UDAT_STYLE_NARROW", UDAT_STYLE_NARROW, R"doc(
             Use the shortest possible form.
             )doc")
      .value("UDAT_STYLE_COUNT", UDAT_STYLE_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // enum URelativeDateTimeFormatterField
  //
  py::enum_<URelativeDateTimeFormatterField>(
      m, "URelativeDateTimeFormatterField", py::arithmetic(), R"doc(
:class:`FieldPosition` selector for format fields defined by
:class:`RelativeDateTimeFormatter`.
      )doc")
      .value("UDAT_REL_LITERAL_FIELD", UDAT_REL_LITERAL_FIELD, R"doc(
             Represent a literal text string, like "tomorrow" or "days ago".
             )doc")
      .value("UDAT_REL_NUMERIC_FIELD", UDAT_REL_NUMERIC_FIELD, R"doc(
             Represent a number quantity, like "3" in "3 days ago".
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  //
  // enum URelativeDateTimeUnit
  //
  py::enum_<URelativeDateTimeUnit>(m, "URelativeDateTimeUnit", py::arithmetic(),
                                   R"doc(
Represent the unit for formatting a relative date.

e.g. "in 5 days" or "next year"
      )doc")
      .value("UDAT_REL_UNIT_YEAR", UDAT_REL_UNIT_YEAR, R"doc(
             Specifies that relative unit is year, e.g. "last year",
             "in 5 years".
             )doc")
      .value("UDAT_REL_UNIT_QUARTER", UDAT_REL_UNIT_QUARTER, R"doc(
             Specifies that relative unit is quarter, e.g. "last quarter",
             "in 5 quarters".
             )doc")
      .value("UDAT_REL_UNIT_MONTH", UDAT_REL_UNIT_MONTH, R"doc(
             Specifies that relative unit is month, e.g. "last month",
             "in 5 months".
             )doc")
      .value("UDAT_REL_UNIT_WEEK", UDAT_REL_UNIT_WEEK, R"doc(
             Specifies that relative unit is week, e.g. "last week",
             "in 5 weeks".
             )doc")
      .value("UDAT_REL_UNIT_DAY", UDAT_REL_UNIT_DAY, R"doc(
             Specifies that relative unit is day, e.g. "yesterday",
             "in 5 days".
             )doc")
      .value("UDAT_REL_UNIT_HOUR", UDAT_REL_UNIT_HOUR, R"doc(
             Specifies that relative unit is hour, e.g. "1 hour ago",
             "in 5 hours".
             )doc")
      .value("UDAT_REL_UNIT_MINUTE", UDAT_REL_UNIT_MINUTE, R"doc(
             Specifies that relative unit is minute, e.g. "1 minute ago",
             "in 5 minutes".
             )doc")
      .value("UDAT_REL_UNIT_SECOND", UDAT_REL_UNIT_SECOND, R"doc(
             Specifies that relative unit is second, e.g. "1 second ago",
             "in 5 seconds".
             )doc")
      .value("UDAT_REL_UNIT_SUNDAY", UDAT_REL_UNIT_SUNDAY, R"doc(
             Specifies that relative unit is Sunday, e.g. "last Sunday",
             "this Sunday", "next Sunday", "in 5 Sundays".
             )doc")
      .value("UDAT_REL_UNIT_MONDAY", UDAT_REL_UNIT_MONDAY, R"doc(
             Specifies that relative unit is Monday, e.g. "last Monday",
             "this Monday", "next Monday", "in 5 Mondays".
             )doc")
      .value("UDAT_REL_UNIT_TUESDAY", UDAT_REL_UNIT_TUESDAY, R"doc(
             Specifies that relative unit is Tuesday, e.g. "last Tuesday",
             "this Tuesday", "next Tuesday", "in 5 Tuesdays".
             )doc")
      .value("UDAT_REL_UNIT_WEDNESDAY", UDAT_REL_UNIT_WEDNESDAY, R"doc(
             Specifies that relative unit is Wednesday, e.g. "last Wednesday",
             "this Wednesday", "next Wednesday", "in 5 Wednesdays".
             )doc")
      .value("UDAT_REL_UNIT_THURSDAY", UDAT_REL_UNIT_THURSDAY, R"doc(
             Specifies that relative unit is Thursday, e.g. "last Thursday",
             "this Thursday", "next Thursday", "in 5 Thursdays".
             )doc")
      .value("UDAT_REL_UNIT_FRIDAY", UDAT_REL_UNIT_FRIDAY, R"doc(
             Specifies that relative unit is Friday, e.g. "last Friday",
             "this Friday", "next Friday", "in 5 Fridays".
             )doc")
      .value("UDAT_REL_UNIT_SATURDAY", UDAT_REL_UNIT_SATURDAY, R"doc(
             Specifies that relative unit is Saturday, e.g. "last Saturday",
             "this Saturday", "next Saturday", "in 5 Saturdays".
             )doc")
      .value("UDAT_REL_UNIT_COUNT", UDAT_REL_UNIT_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
#include <optional>
#include <pybind11/stl.h>
#include <unicode/decimfmt.h>
#include <unicode/reldatefmt.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

using namespace icu;

void init_reldatefmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  //
  // enum UDateAbsoluteUnit
  //
  py::enum_<UDateAbsoluteUnit>(m, "UDateAbsoluteUnit", py::arithmetic(),
                               "Represent an absolute unit.")
      .value("UDAT_ABSOLUTE_SUNDAY", UDAT_ABSOLUTE_SUNDAY, R"doc(
             Sunday.
             )doc")
      .value("UDAT_ABSOLUTE_MONDAY", UDAT_ABSOLUTE_MONDAY, R"doc(
             Monday.
             )doc")
      .value("UDAT_ABSOLUTE_TUESDAY", UDAT_ABSOLUTE_TUESDAY, R"doc(
             Tuesday.
             )doc")
      .value("UDAT_ABSOLUTE_WEDNESDAY", UDAT_ABSOLUTE_WEDNESDAY, R"doc(
             Wednesday.
             )doc")
      .value("UDAT_ABSOLUTE_THURSDAY", UDAT_ABSOLUTE_THURSDAY, R"doc(
             Thursday.
             )doc")
      .value("UDAT_ABSOLUTE_FRIDAY", UDAT_ABSOLUTE_FRIDAY, R"doc(
             Friday.
             )doc")
      .value("UDAT_ABSOLUTE_SATURDAY", UDAT_ABSOLUTE_SATURDAY, R"doc(
             Saturday.
             )doc")
      .value("UDAT_ABSOLUTE_DAY", UDAT_ABSOLUTE_DAY, R"doc(
             Day.
             )doc")
      .value("UDAT_ABSOLUTE_WEEK", UDAT_ABSOLUTE_WEEK, R"doc(
             Week.
             )doc")
      .value("UDAT_ABSOLUTE_MONTH", UDAT_ABSOLUTE_MONTH, R"doc(
             Month.
             )doc")
      .value("UDAT_ABSOLUTE_YEAR", UDAT_ABSOLUTE_YEAR, R"doc(
             Year.
             )doc")
      .value("UDAT_ABSOLUTE_NOW", UDAT_ABSOLUTE_NOW, R"doc(
             Now.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
      .value("UDAT_ABSOLUTE_QUARTER", UDAT_ABSOLUTE_QUARTER, R"doc(
             Quarter.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
      .value("UDAT_ABSOLUTE_HOUR", UDAT_ABSOLUTE_HOUR, R"doc(
             Hour.
             )doc")
      .value("UDAT_ABSOLUTE_MINUTE", UDAT_ABSOLUTE_MINUTE, R"doc(
             Minute.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)
      .value("UDAT_ABSOLUTE_UNIT_COUNT", UDAT_ABSOLUTE_UNIT_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UDateDirection
  //
  py::enum_<UDateDirection>(m, "UDateDirection", py::arithmetic(), R"doc(
Represent a direction for an absolute unit e.g., "Next Tuesday" or
"Last Tuesday".
      )doc")
      .value("UDAT_DIRECTION_LAST_2", UDAT_DIRECTION_LAST_2, R"doc(
             Two before.

             Not fully supported in every locale.
             )doc")
      .value("UDAT_DIRECTION_LAST", UDAT_DIRECTION_LAST, R"doc(
             Last.
             )doc")
      .value("UDAT_DIRECTION_THIS", UDAT_DIRECTION_THIS, R"doc(
             This.
             )doc")
      .value("UDAT_DIRECTION_NEXT", UDAT_DIRECTION_NEXT, R"doc(
             Next.
             )doc")
      .value("UDAT_DIRECTION_NEXT_2", UDAT_DIRECTION_NEXT_2, R"doc(
             Two after.

             Not fully supported in every locale.
             )doc")
      .value("UDAT_DIRECTION_PLAIN", UDAT_DIRECTION_PLAIN, R"doc(
             Plain, which means the absence of a qualifier.
             )doc")
      .value("UDAT_DIRECTION_COUNT", UDAT_DIRECTION_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UDateRelativeUnit
  //
  py::enum_<UDateRelativeUnit>(m, "UDateRelativeUnit", py::arithmetic(), R"doc(
Represent the unit for formatting a relative date.

e.g., "in 5 days" or "in 3 months"
      )doc")
      .value("UDAT_RELATIVE_SECONDS", UDAT_RELATIVE_SECONDS, R"doc(
             Seconds.
             )doc")
      .value("UDAT_RELATIVE_MINUTES", UDAT_RELATIVE_MINUTES, R"doc(
             Minutes.
             )doc")
      .value("UDAT_RELATIVE_HOURS", UDAT_RELATIVE_HOURS, R"doc(
             Hours.
             )doc")
      .value("UDAT_RELATIVE_DAYS", UDAT_RELATIVE_DAYS, R"doc(
             Days.
             )doc")
      .value("UDAT_RELATIVE_WEEKS", UDAT_RELATIVE_WEEKS, R"doc(
             Weeks.
             )doc")
      .value("UDAT_RELATIVE_MONTHS", UDAT_RELATIVE_MONTHS, R"doc(
             Months.
             )doc")
      .value("UDAT_RELATIVE_YEARS", UDAT_RELATIVE_YEARS, R"doc(
             Years.
             )doc")
      .value("UDAT_RELATIVE_UNIT_COUNT", UDAT_RELATIVE_UNIT_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // class icu::FormattedRelativeDateTime
  //
  py::class_<FormattedRelativeDateTime, UMemory, FormattedValue> frdt(
      m, "FormattedRelativeDateTime", R"doc(
      Immutable class containing the result of a relative datetime formatting
      operation.

      See Also:
          :class:`RelativeDateTimeFormatter`
      )doc");

  frdt.def(py::init<>());
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  //
  // class icu::RelativeDateTimeFormatter
  //
  py::class_<RelativeDateTimeFormatter, UObject> rdtf(
      m, "RelativeDateTimeFormatter", R"doc(
      Format relative dates and times.

      Examples:
          Appends "in 1 day":

          .. code-block:: python

              from icupy import icu
              fmt = icu.RelativeDateTimeFormatter(icu.ULOC_US)
              dest = icu.UnicodeString()
              fmt.format(1, icu.UDAT_DIRECTION_NEXT, icu.UDAT_RELATIVE_DAYS, dest)

          Appends "in 3 days":

          .. code-block:: python

              fmt.format(3, icu.UDAT_DIRECTION_NEXT, icu.UDAT_RELATIVE_DAYS, dest)

          Appends "last Sunday":

          .. code-block:: python

              fmt.format(icu.UDAT_DIRECTION_LAST, icu.UDAT_ABSOLUTE_SUNDAY, dest)

          Appends "Sunday":

          .. code-block:: python

              fmt.format(icu.UDAT_DIRECTION_PLAIN, icu.UDAT_ABSOLUTE_SUNDAY, dest)

          Appends "yesterday":

          .. code-block:: python

              fmt.format(icu.UDAT_DIRECTION_LAST, icu.UDAT_ABSOLUTE_DAY, dest)
      )doc");

  rdtf.def(
          // [1] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([]() {
            ErrorCode error_code;
            auto result =
                std::make_unique<RelativeDateTimeFormatter>(error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }))
      .def(
          // [2] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([](const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                icupy::to_locale(locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(
          // [3] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([](const icupy::LocaleVariant &locale,
                      std::optional<NumberFormat *> &nf_to_adopt) {
            ErrorCode error_code;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                icupy::to_locale(locale),
                nf_to_adopt ? (*nf_to_adopt)->clone() : nullptr, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("nf_to_adopt"))
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .def(
          // [4] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([](const icupy::LocaleVariant &locale,
                      std::optional<NumberFormat *> &nf_to_adopt,
                      UDateRelativeDateTimeFormatterStyle style,
                      UDisplayContext capitalization_context) {
            ErrorCode error_code;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                icupy::to_locale(locale),
                nf_to_adopt ? (*nf_to_adopt)->clone() : nullptr, style,
                capitalization_context, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("nf_to_adopt"), py::arg("style"),
          py::arg("capitalization_context"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .def(
          // [5] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init<const RelativeDateTimeFormatter &>(), py::arg("other"));

  rdtf.def(
      "combine_date_and_time",
      [](const RelativeDateTimeFormatter &self,
         const icupy::UnicodeStringVariant &relative_date_string,
         const icupy::UnicodeStringVariant &time_string,
         UnicodeString &append_to) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.combineDateAndTime(
            icupy::to_unistr(relative_date_string),
            icupy::to_unistr(time_string), append_to, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("relative_date_string"), py::arg("time_string"),
      py::arg("append_to"));

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  rdtf.def(
      // [1] RelativeDateTimeFormatter::format
      "format",
      [](const RelativeDateTimeFormatter &self, double offset,
         URelativeDateTimeUnit unit,
         UnicodeString &append_to) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.format(offset, unit, append_to, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("offset"), py::arg("unit"), py::arg("append_to"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

  rdtf.def(
          // [2] RelativeDateTimeFormatter::format
          "format",
          [](const RelativeDateTimeFormatter &self, double quantity,
             UDateDirection direction, UDateRelativeUnit unit,
             UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result =
                self.format(quantity, direction, unit, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("quantity"), py::arg("direction"), py::arg("unit"),
          py::arg("append_to"))
      .def(
          // [3] RelativeDateTimeFormatter::format
          "format",
          [](const RelativeDateTimeFormatter &self, UDateDirection direction,
             UDateAbsoluteUnit unit,
             UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(direction, unit, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("direction"), py::arg("unit"), py::arg("append_to"));

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  rdtf.def(
      "format_numeric",
      [](const RelativeDateTimeFormatter &self, double offset,
         URelativeDateTimeUnit unit,
         UnicodeString &append_to) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.formatNumeric(offset, unit, append_to, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("offset"), py::arg("unit"), py::arg("append_to"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  rdtf.def(
      "format_numeric_to_value",
      [](const RelativeDateTimeFormatter &self, double offset,
         URelativeDateTimeUnit unit) {
        ErrorCode error_code;
        auto result = self.formatNumericToValue(offset, unit, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("offset"), py::arg("unit"));

  rdtf.def(
          // [1] RelativeDateTimeFormatter::formatToValue
          "format_to_value",
          [](const RelativeDateTimeFormatter &self, double offset,
             URelativeDateTimeUnit unit) {
            ErrorCode error_code;
            auto result = self.formatToValue(offset, unit, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("offset"), py::arg("unit"))
      .def(
          // [2] RelativeDateTimeFormatter::formatToValue
          "format_to_value",
          [](const RelativeDateTimeFormatter &self, double quantity,
             UDateDirection direction, UDateRelativeUnit unit) {
            ErrorCode error_code;
            auto result =
                self.formatToValue(quantity, direction, unit, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("quantity"), py::arg("direction"), py::arg("unit"))
      .def(
          // [3] RelativeDateTimeFormatter::formatToValue
          "format_to_value",
          [](const RelativeDateTimeFormatter &self, UDateDirection direction,
             UDateAbsoluteUnit unit) {
            ErrorCode error_code;
            auto result = self.formatToValue(direction, unit, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("direction"), py::arg("unit"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  rdtf.def("get_capitalization_context",
           &RelativeDateTimeFormatter::getCapitalizationContext);

  rdtf.def("get_format_style", &RelativeDateTimeFormatter::getFormatStyle);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  rdtf.def("get_number_format", &RelativeDateTimeFormatter::getNumberFormat,
           py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
}

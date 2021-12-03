#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
#include <unicode/decimfmt.h>
#include <unicode/reldatefmt.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

using namespace icu;

void init_reldatefmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  py::enum_<UDateAbsoluteUnit>(m, "UDateAbsoluteUnit", py::arithmetic(), "Represents an absolute unit.")
      .value("UDAT_ABSOLUTE_SUNDAY", UDAT_ABSOLUTE_SUNDAY, "Sunday.")
      .value("UDAT_ABSOLUTE_MONDAY", UDAT_ABSOLUTE_MONDAY, "Monday.")
      .value("UDAT_ABSOLUTE_TUESDAY", UDAT_ABSOLUTE_TUESDAY, "Tuesday.")
      .value("UDAT_ABSOLUTE_WEDNESDAY", UDAT_ABSOLUTE_WEDNESDAY, "Wednesday.")
      .value("UDAT_ABSOLUTE_THURSDAY", UDAT_ABSOLUTE_THURSDAY, "Thursday.")
      .value("UDAT_ABSOLUTE_FRIDAY", UDAT_ABSOLUTE_FRIDAY, "Friday.")
      .value("UDAT_ABSOLUTE_SATURDAY", UDAT_ABSOLUTE_SATURDAY, "Saturday.")
      .value("UDAT_ABSOLUTE_DAY", UDAT_ABSOLUTE_DAY, "Day.")
      .value("UDAT_ABSOLUTE_WEEK", UDAT_ABSOLUTE_WEEK, "Week.")
      .value("UDAT_ABSOLUTE_MONTH", UDAT_ABSOLUTE_MONTH, "Month.")
      .value("UDAT_ABSOLUTE_YEAR", UDAT_ABSOLUTE_YEAR, "Year.")
      .value("UDAT_ABSOLUTE_NOW", UDAT_ABSOLUTE_NOW, "Now.")
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
      .value("UDAT_ABSOLUTE_QUARTER", UDAT_ABSOLUTE_QUARTER, "Quarter.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
      .value("UDAT_ABSOLUTE_HOUR", UDAT_ABSOLUTE_HOUR, "Hour.")
      .value("UDAT_ABSOLUTE_MINUTE", UDAT_ABSOLUTE_MINUTE, "Minute.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#ifndef U_HIDE_DEPRECATED_API
      .value("UDAT_ABSOLUTE_UNIT_COUNT", UDAT_ABSOLUTE_UNIT_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UDateDirection>(m, "UDateDirection", py::arithmetic(),
                            "Represents a direction for an absolute unit e.g., \"Next Tuesday\" or \"Last Tuesday\".")
      .value("UDAT_DIRECTION_LAST_2", UDAT_DIRECTION_LAST_2,
             "Two before.\n\n  "
             "Not fully supported in every locale.")
      .value("UDAT_DIRECTION_LAST", UDAT_DIRECTION_LAST, "Last.")
      .value("UDAT_DIRECTION_THIS", UDAT_DIRECTION_THIS, "This.")
      .value("UDAT_DIRECTION_NEXT", UDAT_DIRECTION_NEXT, "Next.")
      .value("UDAT_DIRECTION_NEXT_2", UDAT_DIRECTION_NEXT_2,
             "Two after.\n\n  "
             "Not fully supported in every locale.")
      .value("UDAT_DIRECTION_PLAIN", UDAT_DIRECTION_PLAIN, "Plain, which means the absence of a qualifier.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UDAT_DIRECTION_COUNT", UDAT_DIRECTION_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UDateRelativeUnit>(m, "UDateRelativeUnit", py::arithmetic(),
                               "Represents the unit for formatting a relative date.\n\n"
                               "e.g., \"in 5 days\" or \"in 3 months\"")
      .value("UDAT_RELATIVE_SECONDS", UDAT_RELATIVE_SECONDS, "Seconds.")
      .value("UDAT_RELATIVE_MINUTES", UDAT_RELATIVE_MINUTES, "Minutes.")
      .value("UDAT_RELATIVE_HOURS", UDAT_RELATIVE_HOURS, "Hours.")
      .value("UDAT_RELATIVE_DAYS", UDAT_RELATIVE_DAYS, "Days.")
      .value("UDAT_RELATIVE_WEEKS", UDAT_RELATIVE_WEEKS, "Weeks.")
      .value("UDAT_RELATIVE_MONTHS", UDAT_RELATIVE_MONTHS, "Months.")
      .value("UDAT_RELATIVE_YEARS", UDAT_RELATIVE_YEARS, "Years.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UDAT_RELATIVE_UNIT_COUNT", UDAT_RELATIVE_UNIT_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  // icu::FormattedRelativeDateTime
  py::class_<FormattedRelativeDateTime, UMemory, FormattedValue> frdt(m, "FormattedRelativeDateTime");
  frdt.def(
      "append_to",
      [](const FormattedRelativeDateTime &self, Appendable &appendable) -> Appendable & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.appendTo(appendable, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"));
  frdt.def(
      "next_position",
      [](const FormattedRelativeDateTime &self, ConstrainedFieldPosition &cfpos) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.nextPosition(cfpos, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("cfpos"));
  frdt.def("to_string", [](const FormattedRelativeDateTime &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  frdt.def("to_temp_string", [](const FormattedRelativeDateTime &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toTempString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  // icu::RelativeDateTimeFormatter
  py::class_<RelativeDateTimeFormatter, UObject> rdtf(m, "RelativeDateTimeFormatter");
  rdtf.def(
          // [1] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([]() {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }))
      .def(
          // [2] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([](const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(
          // [3] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([](const Locale &locale, NumberFormat *nf_to_adopt) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                locale, reinterpret_cast<NumberFormat *>(nf_to_adopt ? nf_to_adopt->clone() : nullptr), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("nf_to_adopt"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, NumberFormat *nf_to_adopt) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                locale, reinterpret_cast<NumberFormat *>(nf_to_adopt ? nf_to_adopt->clone() : nullptr), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("nf_to_adopt"))
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .def(
          // [4] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init([](const Locale &locale, NumberFormat *nf_to_adopt, UDateRelativeDateTimeFormatterStyle style,
                      UDisplayContext capitalization_context) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                locale, reinterpret_cast<NumberFormat *>(nf_to_adopt ? nf_to_adopt->clone() : nullptr), style,
                capitalization_context, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("nf_to_adopt"), py::arg("style"), py::arg("capitalization_context"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, NumberFormat *nf_to_adopt, UDateRelativeDateTimeFormatterStyle style,
                      UDisplayContext capitalization_context) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RelativeDateTimeFormatter>(
                locale, reinterpret_cast<NumberFormat *>(nf_to_adopt ? nf_to_adopt->clone() : nullptr), style,
                capitalization_context, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("nf_to_adopt"), py::arg("style"), py::arg("capitalization_context"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .def(
          // [5] RelativeDateTimeFormatter::RelativeDateTimeFormatter
          py::init<const RelativeDateTimeFormatter &>(), py::arg("other"));
  rdtf.def(
          "combine_date_and_time",
          [](const RelativeDateTimeFormatter &self, const UnicodeString &relative_date_string,
             const UnicodeString &time_string, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.combineDateAndTime(relative_date_string, time_string, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("relative_date_string"), py::arg("time_string"), py::arg("append_to"))
      .def(
          // const char16_t *relative_date_string -> const UnicodeString &relative_date_string
          "combine_date_and_time",
          [](const RelativeDateTimeFormatter &self, const char16_t *relative_date_string,
             const UnicodeString &time_string, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.combineDateAndTime(relative_date_string, time_string, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("relative_date_string"), py::arg("time_string"), py::arg("append_to"))
      .def(
          // const char16_t *time_string -> const UnicodeString &time_string
          "combine_date_and_time",
          [](const RelativeDateTimeFormatter &self, const UnicodeString &relative_date_string,
             const char16_t *time_string, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.combineDateAndTime(relative_date_string, time_string, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("relative_date_string"), py::arg("time_string"), py::arg("append_to"))
      .def(
          // const char16_t *relative_date_string -> const UnicodeString &relative_date_string
          // const char16_t *time_string -> const UnicodeString &time_string
          "combine_date_and_time",
          [](const RelativeDateTimeFormatter &self, const char16_t *relative_date_string, const char16_t *time_string,
             UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.combineDateAndTime(relative_date_string, time_string, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("relative_date_string"), py::arg("time_string"), py::arg("append_to"));
#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  rdtf.def(
      // [1] RelativeDateTimeFormatter::format
      "format",
      [](const RelativeDateTimeFormatter &self, double offset, URelativeDateTimeUnit unit,
         UnicodeString &append_to) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.format(offset, unit, append_to, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("offset"), py::arg("unit"), py::arg("append_to"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)
  rdtf.def(
          // [2] RelativeDateTimeFormatter::format
          "format",
          [](const RelativeDateTimeFormatter &self, double quantity, UDateDirection direction, UDateRelativeUnit unit,
             UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(quantity, direction, unit, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("quantity"), py::arg("direction"), py::arg("unit"), py::arg("append_to"))
      .def(
          // [3] RelativeDateTimeFormatter::format
          "format",
          [](const RelativeDateTimeFormatter &self, UDateDirection direction, UDateAbsoluteUnit unit,
             UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(direction, unit, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("direction"), py::arg("unit"), py::arg("append_to"));
#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  rdtf.def(
      "format_numeric",
      [](const RelativeDateTimeFormatter &self, double offset, URelativeDateTimeUnit unit,
         UnicodeString &append_to) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.formatNumeric(offset, unit, append_to, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("offset"), py::arg("unit"), py::arg("append_to"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  rdtf.def(
      "format_numeric_to_value",
      [](const RelativeDateTimeFormatter &self, double offset, URelativeDateTimeUnit unit) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.formatNumericToValue(offset, unit, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("offset"), py::arg("unit"));
  rdtf.def(
          // [1] RelativeDateTimeFormatter::formatToValue
          "format_to_value",
          [](const RelativeDateTimeFormatter &self, double offset, URelativeDateTimeUnit unit) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.formatToValue(offset, unit, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("offset"), py::arg("unit"))
      .def(
          // [2] RelativeDateTimeFormatter::formatToValue
          "format_to_value",
          [](const RelativeDateTimeFormatter &self, double quantity, UDateDirection direction, UDateRelativeUnit unit) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.formatToValue(quantity, direction, unit, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("quantity"), py::arg("direction"), py::arg("unit"))
      .def(
          // [3] RelativeDateTimeFormatter::formatToValue
          "format_to_value",
          [](const RelativeDateTimeFormatter &self, UDateDirection direction, UDateAbsoluteUnit unit) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.formatToValue(direction, unit, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("direction"), py::arg("unit"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  rdtf.def("get_capitalization_context", &RelativeDateTimeFormatter::getCapitalizationContext);
  rdtf.def("get_format_style", &RelativeDateTimeFormatter::getFormatStyle);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  rdtf.def("get_number_format", &RelativeDateTimeFormatter::getNumberFormat, py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
}

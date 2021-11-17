#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/basictz.h>
#include <unicode/tzfmt.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_tzfmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  py::enum_<UTimeZoneFormatGMTOffsetPatternType>(m, "UTimeZoneFormatGMTOffsetPatternType", py::arithmetic(),
                                                 "Constants for GMT offset pattern types.")
      .value("UTZFMT_PAT_POSITIVE_HM", UTZFMT_PAT_POSITIVE_HM, "Positive offset with hours and minutes fields.")
      .value("UTZFMT_PAT_POSITIVE_HMS", UTZFMT_PAT_POSITIVE_HMS,
             "Positive offset with hours, minutes and seconds fields.")
      .value("UTZFMT_PAT_NEGATIVE_HM", UTZFMT_PAT_NEGATIVE_HM, "Negative offset with hours and minutes fields.")
      .value("UTZFMT_PAT_NEGATIVE_HMS", UTZFMT_PAT_NEGATIVE_HMS,
             "Negative offset with hours, minutes and seconds fields.")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UTZFMT_PAT_POSITIVE_H", UTZFMT_PAT_POSITIVE_H, "Positive offset with hours field.")
      .value("UTZFMT_PAT_NEGATIVE_H", UTZFMT_PAT_NEGATIVE_H, "Negative offset with hours field.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
      .export_values();

  py::enum_<UTimeZoneFormatParseOption>(
      m, "UTimeZoneFormatParseOption", py::arithmetic(),
      "Constants for parse option flags, used for specifying optional parse behavior.")
      .value("UTZFMT_PARSE_OPTION_NONE", UTZFMT_PARSE_OPTION_NONE, "No option.")
      .value("UTZFMT_PARSE_OPTION_ALL_STYLES", UTZFMT_PARSE_OPTION_ALL_STYLES,
             "When a time zone display name is not found within a set of display names used for the specified style, "
             "look for the name from display names used by other styles.")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UTZFMT_PARSE_OPTION_TZ_DATABASE_ABBREVIATIONS", UTZFMT_PARSE_OPTION_TZ_DATABASE_ABBREVIATIONS,
             "When parsing a time zone display name in *UTZFMT_STYLE_SPECIFIC_SHORT*, look for the IANA tz database "
             "compatible zone abbreviations in addition to the localized names coming from the *icu::TimeZoneNames* "
             "currently used by the *icu::TimeZoneFormat*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .export_values();

  py::enum_<UTimeZoneFormatStyle>(
      m, "UTimeZoneFormatStyle", py::arithmetic(),
      "Constants for time zone display format style used by format/parse APIs in *TimeZoneFormat*.")
      .value("UTZFMT_STYLE_GENERIC_LOCATION", UTZFMT_STYLE_GENERIC_LOCATION,
             "Generic location format, such as \"United States Time(New York)\", \"Italy Time\".")
      .value("UTZFMT_STYLE_GENERIC_LONG", UTZFMT_STYLE_GENERIC_LONG,
             "Generic long non-location format, such as \"Eastern Time\".")
      .value("UTZFMT_STYLE_GENERIC_SHORT", UTZFMT_STYLE_GENERIC_SHORT,
             "Generic short non-location format, such as \"ET\".")
      .value("UTZFMT_STYLE_SPECIFIC_LONG", UTZFMT_STYLE_SPECIFIC_LONG,
             "Specific long format, such as \"Eastern Standard Time\".")
      .value("UTZFMT_STYLE_SPECIFIC_SHORT", UTZFMT_STYLE_SPECIFIC_SHORT,
             "Specific short format, such as \"EST\", \"PDT\".")
      .value("UTZFMT_STYLE_LOCALIZED_GMT", UTZFMT_STYLE_LOCALIZED_GMT,
             "Localized GMT offset format, such as \"GMT-05:00\", \"UTC+0100\".")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UTZFMT_STYLE_LOCALIZED_GMT_SHORT", UTZFMT_STYLE_LOCALIZED_GMT_SHORT,
             "Short localized GMT offset format, such as \"GMT-5\", \"UTC+1:30\" This style is equivalent to the LDML "
             "date format pattern \"O\".")
      .value("UTZFMT_STYLE_ISO_BASIC_SHORT", UTZFMT_STYLE_ISO_BASIC_SHORT,
             "Short ISO 8601 local time difference (basic format) or the UTC indicator.\n\n  "
             "For example, \"-05\", \"+0530\", and \"Z\"(UTC). This style is equivalent to the LDML date format "
             "pattern \"X\".")
      .value("UTZFMT_STYLE_ISO_BASIC_LOCAL_SHORT", UTZFMT_STYLE_ISO_BASIC_LOCAL_SHORT,
             "Short ISO 8601 locale time difference (basic format).\n\n  "
             "For example, \"-05\" and \"+0530\". This style is equivalent to the LDML date format pattern \"x\".")
      .value("UTZFMT_STYLE_ISO_BASIC_FIXED", UTZFMT_STYLE_ISO_BASIC_FIXED,
             "Fixed width ISO 8601 local time difference (basic format) or the UTC indicator.\n\n  "
             "For example, \"-0500\", \"+0530\", and \"Z\"(UTC). This style is equivalent to the LDML date format "
             "pattern \"XX\".")
      .value("UTZFMT_STYLE_ISO_BASIC_LOCAL_FIXED", UTZFMT_STYLE_ISO_BASIC_LOCAL_FIXED,
             "Fixed width ISO 8601 local time difference (basic format).\n\n  "
             "For example, \"-0500\" and \"+0530\". This style is equivalent to the LDML date format pattern \"xx\".")
      .value("UTZFMT_STYLE_ISO_BASIC_FULL", UTZFMT_STYLE_ISO_BASIC_FULL,
             "ISO 8601 local time difference (basic format) with optional seconds field, or the UTC indicator.\n\n  "
             "For example, \"-0500\", \"+052538\", and \"Z\"(UTC). This style is equivalent to the LDML date format "
             "pattern \"XXXX\".")
      .value("UTZFMT_STYLE_ISO_BASIC_LOCAL_FULL", UTZFMT_STYLE_ISO_BASIC_LOCAL_FULL,
             "ISO 8601 local time difference (basic format) with optional seconds field.\n\n  "
             "For example, \"-0500\" and \"+052538\". This style is equivalent to the LDML date format pattern "
             "\"xxxx\".")
      .value("UTZFMT_STYLE_ISO_EXTENDED_FIXED", UTZFMT_STYLE_ISO_EXTENDED_FIXED,
             "Fixed width ISO 8601 local time difference (extended format) or the UTC indicator.\n\n  "
             "For example, \"-05:00\", \"+05:30\", and \"Z\"(UTC). This style is equivalent to the LDML date format "
             "pattern \"XXX\".")
      .value("UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FIXED", UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FIXED,
             "Fixed width ISO 8601 local time difference (extended format).\n\n  "
             "For example, \"-05:00\" and \"+05:30\". This style is equivalent to the LDML date format pattern \"xxx\" "
             "and \"ZZZZZ\".")
      .value("UTZFMT_STYLE_ISO_EXTENDED_FULL", UTZFMT_STYLE_ISO_EXTENDED_FULL,
             "ISO 8601 local time difference (extended format) with optional seconds field, or the UTC indicator.\n\n  "
             "For example, \"-05:00\", \"+05:25:38\", and \"Z\"(UTC). This style is equivalent to the LDML date format "
             "pattern \"XXXXX\".")
      .value("UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FULL", UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FULL,
             "ISO 8601 local time difference (extended format) with optional seconds field.\n\n  "
             "For example, \"-05:00\" and \"+05:25:38\". This style is equivalent to the LDML date format pattern "
             "\"xxxxx\".")
      .value("UTZFMT_STYLE_ZONE_ID", UTZFMT_STYLE_ZONE_ID, "Time Zone ID, such as \"America/Los_Angeles\".")
      .value("UTZFMT_STYLE_ZONE_ID_SHORT", UTZFMT_STYLE_ZONE_ID_SHORT,
             "Short Time Zone ID (BCP 47 Unicode location extension, time zone type value), such as \"uslax\".")
      .value("UTZFMT_STYLE_EXEMPLAR_LOCATION", UTZFMT_STYLE_EXEMPLAR_LOCATION,
             "Exemplar location, such as \"Los Angeles\" and \"Paris\".")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
      .export_values();

  py::enum_<UTimeZoneFormatTimeType>(m, "UTimeZoneFormatTimeType", py::arithmetic(),
                                     "Constants for time types used by *TimeZoneFormat* APIs for receiving time type "
                                     "(standard time, daylight time or unknown).")
      .value("UTZFMT_TIME_TYPE_UNKNOWN", UTZFMT_TIME_TYPE_UNKNOWN, "Unknown.")
      .value("UTZFMT_TIME_TYPE_STANDARD", UTZFMT_TIME_TYPE_STANDARD, "Standard time.")
      .value("UTZFMT_TIME_TYPE_DAYLIGHT", UTZFMT_TIME_TYPE_DAYLIGHT, "Daylight saving time.")
      .export_values();

  // icu::TimeZoneFormat
  py::class_<TimeZoneFormat, Format> tzf(m, "TimeZoneFormat");
  tzf.def(py::init<const TimeZoneFormat &>(), py::arg("other")).def(py::self == py::self, py::arg("other"));
  tzf.def("__copy__", &TimeZoneFormat::clone)
      .def(
          "__deepcopy__", [](const TimeZoneFormat &self, py::dict) { return self.clone(); }, py::arg("memo"));
  // FIXME: Implement "void icu::TimeZoneFormat::adoptTimeZoneNames(TimeZoneNames *tznames)".
  tzf.def("clone", &TimeZoneFormat::clone);
  tzf.def_static(
         "create_instance",
         [](const Locale &locale) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = TimeZoneFormat::createInstance(locale, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = TimeZoneFormat::createInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"));
  tzf.def(
         // [1] TimeZoneFormat::format
         "format",
         [](const TimeZoneFormat &self, const Formattable &obj, UnicodeString &append_to,
            FieldPosition &pos) -> UnicodeString & {
           UErrorCode error_code = U_ZERO_ERROR;
           auto &result = self.format(obj, append_to, pos, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [3] TimeZoneFormat::format
          "format",
          [](const TimeZoneFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [4] TimeZoneFormat::format
          "format",
          [](const TimeZoneFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] TimeZoneFormat::format
          "format",
          py::overload_cast<UTimeZoneFormatStyle, const TimeZone &, UDate, UnicodeString &, UTimeZoneFormatTimeType *>(
              &TimeZoneFormat::format, py::const_),
          py::arg("style"), py::arg("tz"), py::arg("date"), py::arg("name"), py::arg("time_type") = nullptr);
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def(
      "format_offset_iso8601_basic",
      [](const TimeZoneFormat &self, int32_t offset, UBool use_utc_indicator, UBool is_short, UBool ignore_seconds,
         UnicodeString &result) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &string =
            self.formatOffsetISO8601Basic(offset, use_utc_indicator, is_short, ignore_seconds, result, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return string;
      },
      py::arg("offset"), py::arg("use_utc_indicator"), py::arg("is_short"), py::arg("ignore_seconds"),
      py::arg("result"));
  tzf.def(
      "format_offset_iso8601_extended",
      [](const TimeZoneFormat &self, int32_t offset, UBool use_utc_indicator, UBool is_short, UBool ignore_seconds,
         UnicodeString &result) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &string =
            self.formatOffsetISO8601Extended(offset, use_utc_indicator, is_short, ignore_seconds, result, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return string;
      },
      py::arg("offset"), py::arg("use_utc_indicator"), py::arg("is_short"), py::arg("ignore_seconds"),
      py::arg("result"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def(
      "format_offset_localized_gmt",
      [](const TimeZoneFormat &self, int32_t offset, UnicodeString &result) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &string = self.formatOffsetLocalizedGMT(offset, result, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return string;
      },
      py::arg("offset"), py::arg("result"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def(
      "format_offset_short_localized_gmt",
      [](const TimeZoneFormat &self, int32_t offset, UnicodeString &result) -> UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &string = self.formatOffsetShortLocalizedGMT(offset, result, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return string;
      },
      py::arg("offset"), py::arg("result"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def("get_default_parse_options", &TimeZoneFormat::getDefaultParseOptions);
  tzf.def("get_gmt_offset_digits", &TimeZoneFormat::getGMTOffsetDigits, py::arg("digits"));
  tzf.def("get_gmt_offset_pattern", &TimeZoneFormat::getGMTOffsetPattern, py::arg("type_"), py::arg("pattern"));
  tzf.def("get_gmt_pattern", &TimeZoneFormat::getGMTPattern, py::arg("pattern"));
  tzf.def("get_gmt_zero_format", &TimeZoneFormat::getGMTZeroFormat, py::arg("gmt_zero_format"));
  tzf.def("get_time_zone_names", &TimeZoneFormat::getTimeZoneNames, py::return_value_policy::reference);
  tzf.def(
         "parse",
         [](const TimeZoneFormat &self, UTimeZoneFormatStyle style, const UnicodeString &text, ParsePosition &pos,
            int32_t parse_options, UTimeZoneFormatTimeType *time_type) -> std::variant<BasicTimeZone *, TimeZone *> {
           auto tz = self.parse(style, text, pos, parse_options, time_type);
           auto btz = dynamic_cast<BasicTimeZone *>(tz);
           if (btz) {
             return btz;
           }
           return tz;
         },
         py::arg("style"), py::arg("text"), py::arg("pos"), py::arg("parse_options"), py::arg("time_type") = nullptr)
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const TimeZoneFormat &self, UTimeZoneFormatStyle style, const char16_t *text, ParsePosition &pos,
             int32_t parse_options, UTimeZoneFormatTimeType *time_type) -> std::variant<BasicTimeZone *, TimeZone *> {
            auto tz = self.parse(style, text, pos, parse_options, time_type);
            auto btz = dynamic_cast<BasicTimeZone *>(tz);
            if (btz) {
              return btz;
            }
            return tz;
          },
          py::arg("style"), py::arg("text"), py::arg("pos"), py::arg("parse_options"), py::arg("time_type") = nullptr)
      .def(
          "parse",
          [](const TimeZoneFormat &self, UTimeZoneFormatStyle style, const UnicodeString &text, ParsePosition &pos,
             UTimeZoneFormatTimeType *time_type) -> std::variant<BasicTimeZone *, TimeZone *> {
            auto tz = self.parse(style, text, pos, time_type);
            auto btz = dynamic_cast<BasicTimeZone *>(tz);
            if (btz) {
              return btz;
            }
            return tz;
          },
          py::arg("style"), py::arg("text"), py::arg("pos"), py::arg("time_type") = nullptr)
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const TimeZoneFormat &self, UTimeZoneFormatStyle style, const char16_t *text, ParsePosition &pos,
             UTimeZoneFormatTimeType *time_type) -> std::variant<BasicTimeZone *, TimeZone *> {
            auto tz = self.parse(style, text, pos, time_type);
            auto btz = dynamic_cast<BasicTimeZone *>(tz);
            if (btz) {
              return btz;
            }
            return tz;
          },
          py::arg("style"), py::arg("text"), py::arg("pos"), py::arg("time_type") = nullptr);
  tzf.def("parse_object",
          py::overload_cast<const UnicodeString &, Formattable &, ParsePosition &>(&TimeZoneFormat::parseObject,
                                                                                   py::const_),
          py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const TimeZoneFormat &self, const char16_t *source, Formattable &result, ParsePosition &parse_pos) {
            self.parseObject(source, result, parse_pos);
          },
          py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // [2] Format::parseObject
          "parse_object",
          [](const Format &self, const UnicodeString &source, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parseObject(source, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const Format &self, const char16_t *source, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parseObject(source, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
  tzf.def("parse_offset_iso8601",
          py::overload_cast<const UnicodeString &, ParsePosition &>(&TimeZoneFormat::parseOffsetISO8601, py::const_),
          py::arg("text"), py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse_offset_iso8601",
          [](const TimeZoneFormat &self, const char16_t *text, ParsePosition &pos) {
            return self.parseOffsetISO8601(text, pos);
          },
          py::arg("text"), py::arg("pos"));
  tzf.def("parse_offset_localized_gmt",
          py::overload_cast<const UnicodeString &, ParsePosition &>(&TimeZoneFormat::parseOffsetLocalizedGMT,
                                                                    py::const_),
          py::arg("text"), py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse_offset_localized_gmt",
          [](const TimeZoneFormat &self, const char16_t *text, ParsePosition &pos) {
            return self.parseOffsetLocalizedGMT(text, pos);
          },
          py::arg("text"), py::arg("pos"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def("parse_offset_short_localized_gmt", &TimeZoneFormat::parseOffsetShortLocalizedGMT, py::arg("text"),
          py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse_offset_short_localized_gmt",
          [](const TimeZoneFormat &self, const char16_t *text, ParsePosition &pos) {
            return self.parseOffsetShortLocalizedGMT(text, pos);
          },
          py::arg("text"), py::arg("pos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def("set_default_parse_options", &TimeZoneFormat::setDefaultParseOptions, py::arg("flags"));
  tzf.def(
         "set_gmt_offset_digits",
         [](TimeZoneFormat &self, const UnicodeString &digits) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setGMTOffsetDigits(digits, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("digits"))
      .def(
          // const char16_t *digits -> const UnicodeString &digits
          "set_gmt_offset_digits",
          [](TimeZoneFormat &self, const char16_t *digits) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setGMTOffsetDigits(digits, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("digits"));
  tzf.def(
         "set_gmt_offset_pattern",
         [](TimeZoneFormat &self, UTimeZoneFormatGMTOffsetPatternType type, const UnicodeString &pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setGMTOffsetPattern(type, pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("type_"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "set_gmt_offset_pattern",
          [](TimeZoneFormat &self, UTimeZoneFormatGMTOffsetPatternType type, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setGMTOffsetPattern(type, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("type_"), py::arg("pattern"));
  tzf.def(
         "set_gmt_pattern",
         [](TimeZoneFormat &self, const UnicodeString &pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setGMTPattern(pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "set_gmt_pattern",
          [](TimeZoneFormat &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setGMTPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"));
  tzf.def(
         "set_gmt_zero_format",
         [](TimeZoneFormat &self, const UnicodeString &gmt_zero_format) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setGMTZeroFormat(gmt_zero_format, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("gmt_zero_format"))
      .def(
          // const char16_t *gmt_zero_format -> const UnicodeString &gmt_zero_format
          "set_gmt_zero_format",
          [](TimeZoneFormat &self, const char16_t *gmt_zero_format) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setGMTZeroFormat(gmt_zero_format, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("gmt_zero_format"));
  tzf.def("set_time_zone_names", &TimeZoneFormat::setTimeZoneNames, py::arg("tznames"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

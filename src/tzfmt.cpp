#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/basictz.h>
#include <unicode/tzfmt.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_tzfmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // enum UTimeZoneFormatGMTOffsetPatternType
  //
  py::enum_<UTimeZoneFormatGMTOffsetPatternType>(
      m, "UTimeZoneFormatGMTOffsetPatternType", py::arithmetic(),
      "Constants for GMT offset pattern types.")
      .value("UTZFMT_PAT_POSITIVE_HM", UTZFMT_PAT_POSITIVE_HM, R"doc(
             Positive offset with hours and minutes fields.
             )doc")
      .value("UTZFMT_PAT_POSITIVE_HMS", UTZFMT_PAT_POSITIVE_HMS, R"doc(
             Positive offset with hours, minutes and seconds fields.
             )doc")
      .value("UTZFMT_PAT_NEGATIVE_HM", UTZFMT_PAT_NEGATIVE_HM, R"doc(
             Negative offset with hours and minutes fields.
             )doc")
      .value("UTZFMT_PAT_NEGATIVE_HMS", UTZFMT_PAT_NEGATIVE_HMS, R"doc(
             Negative offset with hours, minutes and seconds fields.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UTZFMT_PAT_POSITIVE_H", UTZFMT_PAT_POSITIVE_H, R"doc(
             Positive offset with hours field.
             )doc")
      .value("UTZFMT_PAT_NEGATIVE_H", UTZFMT_PAT_NEGATIVE_H, R"doc(
             Negative offset with hours field.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51
      .export_values();

  //
  // enum UTimeZoneFormatParseOption
  //
  py::enum_<UTimeZoneFormatParseOption>(m, "UTimeZoneFormatParseOption",
                                        py::arithmetic(), R"doc(
Constants for parse option flags, used for specifying optional parse behavior.
      )doc")
      .value("UTZFMT_PARSE_OPTION_NONE", UTZFMT_PARSE_OPTION_NONE, R"doc(
             No option.
             )doc")
      .value("UTZFMT_PARSE_OPTION_ALL_STYLES", UTZFMT_PARSE_OPTION_ALL_STYLES,
             R"doc(
             When a time zone display name is not found within a set of
             display names used for the specified style, look for the name from
             display names used by other styles.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UTZFMT_PARSE_OPTION_TZ_DATABASE_ABBREVIATIONS",
             UTZFMT_PARSE_OPTION_TZ_DATABASE_ABBREVIATIONS, R"doc(
             When parsing a time zone display name in
             :attr:`~UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT`, look
             for the IANA tz database compatible zone abbreviations in addition
             to the localized names coming from the :class:`TimeZoneNames`
             currently used by the :class:`TimeZoneFormat`.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .export_values();

  //
  // enum UTimeZoneFormatStyle
  //
  py::enum_<UTimeZoneFormatStyle>(m, "UTimeZoneFormatStyle", py::arithmetic(),
                                  R"doc(
Constants for time zone display format style used by format/parse APIs in
:class:`TimeZoneFormat`.
      )doc")
      .value("UTZFMT_STYLE_GENERIC_LOCATION", UTZFMT_STYLE_GENERIC_LOCATION,
             R"doc(
             Generic location format, such as "United States Time(New York)",
             "Italy Time".
             )doc")
      .value("UTZFMT_STYLE_GENERIC_LONG", UTZFMT_STYLE_GENERIC_LONG, R"doc(
             Generic long non-location format, such as "Eastern Time".
             )doc")
      .value("UTZFMT_STYLE_GENERIC_SHORT", UTZFMT_STYLE_GENERIC_SHORT, R"doc(
             Generic short non-location format, such as "ET".
             )doc")
      .value("UTZFMT_STYLE_SPECIFIC_LONG", UTZFMT_STYLE_SPECIFIC_LONG, R"doc(
             Specific long format, such as "Eastern Standard Time".
             )doc")
      .value("UTZFMT_STYLE_SPECIFIC_SHORT", UTZFMT_STYLE_SPECIFIC_SHORT, R"doc(
             Specific short format, such as "EST", "PDT".
             )doc")
      .value("UTZFMT_STYLE_LOCALIZED_GMT", UTZFMT_STYLE_LOCALIZED_GMT, R"doc(
             Localized GMT offset format, such as "GMT-05:00", "UTC+0100".
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UTZFMT_STYLE_LOCALIZED_GMT_SHORT",
             UTZFMT_STYLE_LOCALIZED_GMT_SHORT, R"doc(
             Short localized GMT offset format, such as "GMT-5", "UTC+1:30"
             This style is equivalent to the LDML date format pattern "O".
             )doc")
      .value("UTZFMT_STYLE_ISO_BASIC_SHORT", UTZFMT_STYLE_ISO_BASIC_SHORT,
             R"doc(
             Short ISO 8601 local time difference (basic format) or the UTC
             indicator.

             For example, "-05", "+0530", and "Z"(UTC). This style is
             equivalent to the LDML date format pattern "X".
             )doc")
      .value("UTZFMT_STYLE_ISO_BASIC_LOCAL_SHORT",
             UTZFMT_STYLE_ISO_BASIC_LOCAL_SHORT, R"doc(
             Short ISO 8601 local time difference (basic format).

             For example, "-05" and "+0530". This style is equivalent to the
             LDML date format pattern "x".
             )doc")
      .value("UTZFMT_STYLE_ISO_BASIC_FIXED", UTZFMT_STYLE_ISO_BASIC_FIXED,
             R"doc(
             Fixed width ISO 8601 local time difference (basic format) or the
             UTC indicator.

             For example, "-0500", "+0530", and "Z"(UTC). This style is
             equivalent to the LDML date format pattern "XX".
             )doc")
      .value("UTZFMT_STYLE_ISO_BASIC_LOCAL_FIXED",
             UTZFMT_STYLE_ISO_BASIC_LOCAL_FIXED, R"doc(
             Fixed width ISO 8601 local time difference (basic format).

             For example, "-0500" and "+0530". This style is equivalent to the
             LDML date format pattern "xx".
             )doc")
      .value("UTZFMT_STYLE_ISO_BASIC_FULL", UTZFMT_STYLE_ISO_BASIC_FULL, R"doc(
             ISO 8601 local time difference (basic format) with optional
             seconds field, or the UTC indicator.

             For example, "-0500", "+052538", and "Z"(UTC). This style is
             equivalent to the LDML date format pattern "XXXX".
             )doc")
      .value("UTZFMT_STYLE_ISO_BASIC_LOCAL_FULL",
             UTZFMT_STYLE_ISO_BASIC_LOCAL_FULL, R"doc(
             ISO 8601 local time difference (basic format) with optional
             seconds field.

             For example, "-0500" and "+052538". This style is equivalent to
             the LDML date format pattern "xxxx".
             )doc")
      .value("UTZFMT_STYLE_ISO_EXTENDED_FIXED", UTZFMT_STYLE_ISO_EXTENDED_FIXED,
             R"doc(
             Fixed width ISO 8601 local time difference (extended format) or
             the UTC indicator.

             For example, "-05:00", "+05:30", and "Z"(UTC). This style is
             equivalent to the LDML date format pattern "XXX".
             )doc")
      .value("UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FIXED",
             UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FIXED, R"doc(
             Fixed width ISO 8601 local time difference (extended format).

             For example, "-05:00" and "+05:30". This style is equivalent to
             the LDML date format pattern "xxx" and "ZZZZZ".
             )doc")
      .value("UTZFMT_STYLE_ISO_EXTENDED_FULL", UTZFMT_STYLE_ISO_EXTENDED_FULL,
             R"doc(
             ISO 8601 local time difference (extended format) with optional
             seconds field, or the UTC indicator.

             For example, "-05:00", "+05:25:38", and "Z"(UTC). This style is
             equivalent to the LDML date format pattern "XXXXX".
             )doc")
      .value("UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FULL",
             UTZFMT_STYLE_ISO_EXTENDED_LOCAL_FULL, R"doc(
             ISO 8601 local time difference (extended format) with optional
             seconds field.

             For example, "-05:00" and "+05:25:38". This style is equivalent to
             the LDML date format pattern "xxxxx".
             )doc")
      .value("UTZFMT_STYLE_ZONE_ID", UTZFMT_STYLE_ZONE_ID, R"doc(
             Time Zone ID, such as "America/Los_Angeles".
             )doc")
      .value("UTZFMT_STYLE_ZONE_ID_SHORT", UTZFMT_STYLE_ZONE_ID_SHORT, R"doc(
             Short Time Zone ID (BCP 47 Unicode location extension, time zone
             type value), such as "uslax".
             )doc")
      .value("UTZFMT_STYLE_EXEMPLAR_LOCATION", UTZFMT_STYLE_EXEMPLAR_LOCATION,
             R"doc(
             Exemplar location, such as "Los Angeles" and "Paris".
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
      .export_values();

  //
  // enum UTimeZoneFormatTimeType
  //
  py::enum_<UTimeZoneFormatTimeType>(m, "UTimeZoneFormatTimeType",
                                     py::arithmetic(), R"doc(
Constants for time types used by :class:`TimeZoneFormat` APIs for receiving
time type (standard time, daylight time or unknown).
      )doc")
      .value("UTZFMT_TIME_TYPE_UNKNOWN", UTZFMT_TIME_TYPE_UNKNOWN, R"doc(
             Unknown.
             )doc")
      .value("UTZFMT_TIME_TYPE_STANDARD", UTZFMT_TIME_TYPE_STANDARD, R"doc(
             Standard time.
             )doc")
      .value("UTZFMT_TIME_TYPE_DAYLIGHT", UTZFMT_TIME_TYPE_DAYLIGHT, R"doc(
             Daylight saving time.
             )doc")
      .export_values();

  //
  // class icu::TimeZoneFormat
  //
  py::class_<TimeZoneFormat, Format> tzf(m, "TimeZoneFormat", R"doc(
      Perform formatting and parsing of time zone display names.
      )doc");

  tzf.def(py::init<const TimeZoneFormat &>(), py::arg("other"));

  tzf.def("__copy__", &TimeZoneFormat::clone);

  tzf.def(
      "__deepcopy__",
      [](const TimeZoneFormat &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"));

  // FIXME: Implement "void icu::TimeZoneFormat::adoptTimeZoneNames(
  //  TimeZoneNames *tznames)".

  tzf.def("clone", &TimeZoneFormat::clone);

  tzf.def_static(
      "create_instance",
      [](const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        auto result = TimeZoneFormat::createInstance(icupy::to_locale(locale),
                                                     error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"));

  tzf.def(
         // [1] TimeZoneFormat::format
         "format",
         [](const TimeZoneFormat &self, const Formattable &obj,
            UnicodeString &append_to, FieldPosition &pos) -> UnicodeString & {
           ErrorCode error_code;
           auto &result = self.format(obj, append_to, pos, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [3] TimeZoneFormat::format
          "format",
          [](const TimeZoneFormat &self, const Formattable &obj,
             UnicodeString &append_to,
             std::optional<FieldPositionIterator *> &pos_iter)
              -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to,
                                       pos_iter.value_or(nullptr), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [4] TimeZoneFormat::format
          "format",
          [](const TimeZoneFormat &self, const Formattable &obj,
             UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] TimeZoneFormat::format
          "format",
          [](const TimeZoneFormat &self, UTimeZoneFormatStyle style,
             const TimeZone &tz, UDate date, UnicodeString &name)
              -> std::tuple<UnicodeString &, UTimeZoneFormatTimeType> {
            auto time_type = UTZFMT_TIME_TYPE_UNKNOWN;
            auto &result = self.format(style, tz, date, name, &time_type);
            return std::make_tuple(std::ref(result), time_type);
          },
          py::arg("style"), py::arg("tz"), py::arg("date"), py::arg("name"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def(
      "format_offset_iso8601_basic",
      [](const TimeZoneFormat &self, int32_t offset,
         py::bool_ use_utc_indicator, py::bool_ is_short,
         py::bool_ ignore_seconds, UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &value =
            self.formatOffsetISO8601Basic(offset, use_utc_indicator, is_short,
                                          ignore_seconds, result, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return value;
      },
      py::arg("offset"), py::arg("use_utc_indicator"), py::arg("is_short"),
      py::arg("ignore_seconds"), py::arg("result"));

  tzf.def(
      "format_offset_iso8601_extended",
      [](const TimeZoneFormat &self, int32_t offset,
         py::bool_ use_utc_indicator, py::bool_ is_short,
         py::bool_ ignore_seconds, UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &value = self.formatOffsetISO8601Extended(
            offset, use_utc_indicator, is_short, ignore_seconds, result,
            error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return value;
      },
      py::arg("offset"), py::arg("use_utc_indicator"), py::arg("is_short"),
      py::arg("ignore_seconds"), py::arg("result"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  tzf.def(
      "format_offset_localized_gmt",
      [](const TimeZoneFormat &self, int32_t offset,
         UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &value = self.formatOffsetLocalizedGMT(offset, result, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return value;
      },
      py::arg("offset"), py::arg("result"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def(
      "format_offset_short_localized_gmt",
      [](const TimeZoneFormat &self, int32_t offset,
         UnicodeString &result) -> UnicodeString & {
        ErrorCode error_code;
        auto &value =
            self.formatOffsetShortLocalizedGMT(offset, result, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return value;
      },
      py::arg("offset"), py::arg("result"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  tzf.def("get_default_parse_options", &TimeZoneFormat::getDefaultParseOptions);

  tzf.def("get_gmt_offset_digits", &TimeZoneFormat::getGMTOffsetDigits,
          py::arg("digits"));

  tzf.def("get_gmt_offset_pattern", &TimeZoneFormat::getGMTOffsetPattern,
          py::arg("type"), py::arg("pattern"));

  tzf.def("get_gmt_pattern", &TimeZoneFormat::getGMTPattern,
          py::arg("pattern"));

  tzf.def("get_gmt_zero_format", &TimeZoneFormat::getGMTZeroFormat,
          py::arg("gmt_zero_format"));

  tzf.def("get_time_zone_names", &TimeZoneFormat::getTimeZoneNames,
          py::return_value_policy::reference);

  tzf.def(
         "parse",
         [](const TimeZoneFormat &self, UTimeZoneFormatStyle style,
            const icupy::UnicodeStringVariant &text, ParsePosition &pos)
             -> std::tuple<
                 std::variant<BasicTimeZone *, TimeZone *, std::nullptr_t>,
                 UTimeZoneFormatTimeType> {
           auto time_type = UTZFMT_TIME_TYPE_UNKNOWN;
           auto tz = self.parse(style, icupy::to_unistr(text), pos, &time_type);
           if (tz) {
             auto btz = dynamic_cast<BasicTimeZone *>(tz);
             if (btz) {
               return std::make_tuple(btz, time_type);
             }
           }
           return std::make_tuple(tz, time_type);
         },
         py::arg("style"), py::arg("text"), py::arg("pos"))
      .def(
          "parse",
          [](const TimeZoneFormat &self, UTimeZoneFormatStyle style,
             const icupy::UnicodeStringVariant &text, ParsePosition &pos,
             int32_t parse_options)
              -> std::tuple<
                  std::variant<BasicTimeZone *, TimeZone *, std::nullptr_t>,
                  UTimeZoneFormatTimeType> {
            auto time_type = UTZFMT_TIME_TYPE_UNKNOWN;
            auto tz = self.parse(style, icupy::to_unistr(text), pos,
                                 parse_options, &time_type);
            if (tz) {
              auto btz = dynamic_cast<BasicTimeZone *>(tz);
              if (btz) {
                return std::make_tuple(btz, time_type);
              }
            }
            return std::make_tuple(tz, time_type);
          },
          py::arg("style"), py::arg("text"), py::arg("pos"),
          py::arg("parse_options"));

  tzf.def(
      "parse_offset_iso8601",
      [](const TimeZoneFormat &self, const icupy::UnicodeStringVariant &text,
         ParsePosition &pos) {
        return self.parseOffsetISO8601(icupy::to_unistr(text), pos);
      },
      py::arg("text"), py::arg("pos"));

  tzf.def(
      "parse_offset_localized_gmt",
      [](const TimeZoneFormat &self, const icupy::UnicodeStringVariant &text,
         ParsePosition &pos) {
        return self.parseOffsetLocalizedGMT(icupy::to_unistr(text), pos);
      },
      py::arg("text"), py::arg("pos"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  tzf.def(
      "parse_offset_short_localized_gmt",
      [](const TimeZoneFormat &self, const icupy::UnicodeStringVariant &text,
         ParsePosition &pos) {
        return self.parseOffsetShortLocalizedGMT(icupy::to_unistr(text), pos);
      },
      py::arg("text"), py::arg("pos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  tzf.def("set_default_parse_options", &TimeZoneFormat::setDefaultParseOptions,
          py::arg("flags"));

  tzf.def(
      "set_gmt_offset_digits",
      [](TimeZoneFormat &self, const icupy::UnicodeStringVariant &digits) {
        ErrorCode error_code;
        self.setGMTOffsetDigits(icupy::to_unistr(digits), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("digits"));

  tzf.def(
      "set_gmt_offset_pattern",
      [](TimeZoneFormat &self, UTimeZoneFormatGMTOffsetPatternType type,
         const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.setGMTOffsetPattern(type, icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("type"), py::arg("pattern"));

  tzf.def(
      "set_gmt_pattern",
      [](TimeZoneFormat &self, const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.setGMTPattern(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("pattern"));

  tzf.def(
      "set_gmt_zero_format",
      [](TimeZoneFormat &self,
         const icupy::UnicodeStringVariant &gmt_zero_format) {
        ErrorCode error_code;
        self.setGMTZeroFormat(icupy::to_unistr(gmt_zero_format), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("gmt_zero_format"));

  tzf.def("set_time_zone_names", &TimeZoneFormat::setTimeZoneNames,
          py::arg("tznames"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

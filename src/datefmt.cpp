#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/basictz.h>
#include <unicode/datefmt.h>

using namespace icu;

void init_datefmt(py::module &m) {
  // icu::DateFormat
  py::class_<DateFormat, Format> df(m, "DateFormat");

  py::enum_<DateFormat::EStyle>(df, "EStyle", py::arithmetic(),
                                "Constants for various style patterns.\n\n"
                                "These reflect the order of items in the DateTimePatterns resource. There are 4 time "
                                "patterns, 4 date patterns, the default date-time pattern, and 4 date-time patterns. "
                                "Each block of 4 values in the resource occurs in the order full, long, medium, short.")
      .value("NONE", DateFormat::EStyle::kNone)
      .value("FULL", DateFormat::EStyle::kFull)
      .value("LONG", DateFormat::EStyle::kLong)
      .value("MEDIUM", DateFormat::EStyle::kMedium)
      .value("SHORT", DateFormat::EStyle::kShort)
      .value("DATE_OFFSET", DateFormat::EStyle::kDateOffset)
      .value("DATE_TIME", DateFormat::EStyle::kDateTime)
      .value("DATE_TIME_OFFSET", DateFormat::EStyle::kDateTimeOffset)
      .value("RELATIVE", DateFormat::EStyle::kRelative)
      .value("FULL_RELATIVE", DateFormat::EStyle::kFullRelative)
      .value("LONG_RELATIVE", DateFormat::EStyle::kLongRelative)
      .value("MEDIUM_RELATIVE", DateFormat::EStyle::kMediumRelative)
      .value("SHORT_RELATIVE", DateFormat::EStyle::kShortRelative)
      .value("DEFAULT", DateFormat::EStyle::kDefault)
      .export_values();

  df.def("__copy__", &DateFormat::clone)
      .def(
          "__deepcopy__", [](const DateFormat &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const DateFormat &self, const DateFormat &other) { return self == other; }, py::is_operator(),
          py::arg("other"));
  // FIXME: Implement "void icu::DateFormat::adoptCalendar(Calendar *calendarToAdopt)".
  // FIXME: Implement "void icu::DateFormat::adoptNumberFormat(NumberFormat *formatToAdopt)".
  // FIXME: Implement "void icu::DateFormat::adoptTimeZone(TimeZone *zoneToAdopt)".
  df.def("clone", &DateFormat::clone);
  df.def_static("create_date_instance", &DateFormat::createDateInstance, py::arg("style") = DateFormat::kDefault,
                py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"));
  df.def_static("create_date_time_instance", &DateFormat::createDateTimeInstance,
                py::arg("date_style") = DateFormat::kDefault, py::arg("time_style") = DateFormat::kDefault,
                py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"));
  df.def_static("create_instance", &DateFormat::createInstance);
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  // FIXME: Implement "static DateFormat *icu::DateFormat::createInstanceForSkeleton(Calendar *calendarToAdopt,
  // const UnicodeString &skeleton, const Locale &locale, UErrorCode &status)".
  df.def_static(
        "create_instance_for_skeleton",
        [](const _UnicodeStringVariant &skeleton, const _LocaleVariant &locale) {
          ErrorCode error_code;
          auto result =
              DateFormat::createInstanceForSkeleton(VARIANT_TO_UNISTR(skeleton), VARIANT_TO_LOCALE(locale), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("skeleton"), py::arg("locale"))
      .def_static(
          "create_instance_for_skeleton",
          [](const _UnicodeStringVariant &skeleton) {
            ErrorCode error_code;
            auto result = DateFormat::createInstanceForSkeleton(VARIANT_TO_UNISTR(skeleton), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
  df.def_static("create_time_instance", &DateFormat::createTimeInstance, py::arg("style") = DateFormat::kDefault,
                py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"));
  df.def(
        // [1] DateFormat::format
        "format", py::overload_cast<Calendar &, UnicodeString &, FieldPosition &>(&DateFormat::format, py::const_),
        py::arg("cal"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [2] DateFormat::format
          "format",
          [](const DateFormat &self, Calendar &cal, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(cal, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("cal"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [3] DateFormat::format
          "format",
          [](const DateFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [6] DateFormat::format
          "format",
          [](const DateFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [8] DateFormat::format
          "format", py::overload_cast<UDate, UnicodeString &>(&DateFormat::format, py::const_), py::arg("date"),
          py::arg("append_to"))
      .def(
          // [9] DateFormat::format
          "format", py::overload_cast<UDate, UnicodeString &, FieldPosition &>(&DateFormat::format, py::const_),
          py::arg("date"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [10] DateFormat::format
          "format",
          [](const DateFormat &self, UDate date, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(date, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("date"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [3] Format::format
          "format",
          [](const DateFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));
  df.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = DateFormat::getAvailableLocales(count);
        std::vector<const Locale *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def(
      "get_boolean_attribute",
      [](const DateFormat &self, UDateFormatBooleanAttribute attr) {
        ErrorCode error_code;
        auto result = self.getBooleanAttribute(attr, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("get_calendar", &DateFormat::getCalendar, py::return_value_policy::reference);
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def(
      "get_context",
      [](const DateFormat &self, UDisplayContextType type) {
        ErrorCode error_code;
        auto result = self.getContext(type, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("get_number_format", &DateFormat::getNumberFormat, py::return_value_policy::reference);
  df.def(
      "get_time_zone",
      [](const DateFormat &self) -> std::variant<const BasicTimeZone *, const TimeZone *> {
        auto tz = &self.getTimeZone();
        auto btz = dynamic_cast<const BasicTimeZone *>(tz);
        if (btz) {
          return btz;
        }
        return tz;
      },
      py::return_value_policy::reference);
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("is_calendar_lenient", &DateFormat::isCalendarLenient);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("is_lenient", &DateFormat::isLenient);
  df.def(
        "parse",
        [](const DateFormat &self, const _UnicodeStringVariant &text, Calendar &cal, ParsePosition &pos) {
          return self.parse(VARIANT_TO_UNISTR(text), cal, pos);
        },
        py::arg("text"), py::arg("cal"), py::arg("pos"))
      .def(
          "parse",
          [](const DateFormat &self, const _UnicodeStringVariant &text, ParsePosition &pos) {
            return self.parse(VARIANT_TO_UNISTR(text), pos);
          },
          py::arg("text"), py::arg("pos"))
      .def(
          "parse",
          [](const DateFormat &self, const _UnicodeStringVariant &text) {
            ErrorCode error_code;
            auto result = self.parse(VARIANT_TO_UNISTR(text), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("text"));
  df.def(
        "parse_object",
        [](const DateFormat &self, const _UnicodeStringVariant &source, Formattable &result, ParsePosition &parse_pos) {
          self.parseObject(VARIANT_TO_UNISTR(source), result, parse_pos);
        },
        py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // [2] Format::parseObject
          "parse_object",
          [](const Format &self, const _UnicodeStringVariant &source, Formattable &result) {
            ErrorCode error_code;
            self.parseObject(VARIANT_TO_UNISTR(source), result, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def(
      "set_boolean_attribute",
      [](DateFormat &self, UDateFormatBooleanAttribute attr, UBool new_value) -> DateFormat & {
        ErrorCode error_code;
        auto &result = self.setBooleanAttribute(attr, new_value, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"), py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("set_calendar", &DateFormat::setCalendar, py::arg("new_calendar"));
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("set_calendar_lenient", &DateFormat::setCalendarLenient, py::arg("lenient"));
  df.def(
      "set_context",
      [](DateFormat &self, UDisplayContext value) {
        ErrorCode error_code;
        self.setContext(value, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("set_lenient", &DateFormat::setLenient, py::arg("lenient"));
  df.def("set_number_format", &DateFormat::setNumberFormat, py::arg("new_number_format"));
  df.def("set_time_zone", &DateFormat::setTimeZone, py::arg("zone"));
}

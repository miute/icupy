#include "main.hpp"
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/basictz.h>
#include <unicode/dtitvfmt.h>

using namespace icu;

void init_dtitvfmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // class icu::FormattedDateInterval
  //
  py::class_<FormattedDateInterval, UMemory, FormattedValue> fdi(
      m, "FormattedDateInterval", R"doc(
      Immutable class containing the result of a date interval formatting
      operation.

      See Also:
          :class:`DateIntervalFormat`
      )doc");

  fdi.def(py::init<>());
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  //
  // class icu::DateIntervalFormat
  //
  py::class_<DateIntervalFormat, Format> fmt(m, "DateIntervalFormat", R"doc(
      Perform date interval formatting and parsing in a language-independent
      manner.

      Example:
          >>> from icupy import icu
          >>> fmt = icu.DateIntervalFormat.create_instance(icu.UDAT_YEAR_MONTH_DAY, icu.Locale("en", "GB", ""))
          >>> interval = icu.DateInterval(1000 * 3600 * 24, 1000 * 3600 * 24 * 2)
          >>> dest = icu.UnicodeString()
          >>> pos = icu.FieldPosition()
          >>> fmt.format(interval, dest, pos)
          UnicodeString('2\u2009\u2013\u20093 January 1970', text_length=18)
          >>> print(dest)
          2 – 3 January 1970
      )doc");

  fmt.def("__copy__", &DateIntervalFormat::clone);

  fmt.def(
      "__deepcopy__",
      [](const DateIntervalFormat &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"));

  // FIXME: Implement "void icu::DateIntervalFormat::adoptTimeZone(
  //  TimeZone *zoneToAdopt)".

  fmt.def("clone", &DateIntervalFormat::clone);

  fmt.def_static(
         // [1] DateIntervalFormat::createInstance
         "create_instance",
         [](const icupy::UnicodeStringVariant &skeleton,
            const DateIntervalInfo &dtitvinf) {
           ErrorCode error_code;
           auto result = DateIntervalFormat::createInstance(
               icupy::to_unistr(skeleton), dtitvinf, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("skeleton"), py::arg("dtitvinf"))
      .def_static(
          // [2] DateIntervalFormat::createInstance
          "create_instance",
          [](const icupy::UnicodeStringVariant &skeleton,
             const icupy::LocaleVariant &locale,
             const DateIntervalInfo &dtitvinf) {
            ErrorCode error_code;
            auto result = DateIntervalFormat::createInstance(
                icupy::to_unistr(skeleton), icupy::to_locale(locale), dtitvinf,
                error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"), py::arg("dtitvinf"))
      .def_static(
          // [3] DateIntervalFormat::createInstance
          "create_instance",
          [](const icupy::UnicodeStringVariant &skeleton,
             const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = DateIntervalFormat::createInstance(
                icupy::to_unistr(skeleton), icupy::to_locale(locale),
                error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"))
      .def_static(
          // [4] DateIntervalFormat::createInstance
          "create_instance",
          [](const icupy::UnicodeStringVariant &skelton) {
            ErrorCode error_code;
            auto result = DateIntervalFormat::createInstance(
                icupy::to_unistr(skelton), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"));

  fmt.def(
         // [1] DateIntervalFormat::format
         "format",
         [](const DateIntervalFormat &self, Calendar &from_calendar,
            Calendar &to_calendar, UnicodeString &append_to,
            FieldPosition &field_position) -> UnicodeString & {
           ErrorCode error_code;
           auto &result = self.format(from_calendar, to_calendar, append_to,
                                      field_position, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("from_calendar"), py::arg("to_calendar"), py::arg("append_to"),
         py::arg("field_position"))
      .def(
          // [2] DateIntervalFormat::format
          "format",
          [](const DateIntervalFormat &self, const DateInterval *dt_interval,
             UnicodeString &append_to,
             FieldPosition &field_position) -> UnicodeString & {
            ErrorCode error_code;
            auto &result =
                self.format(dt_interval, append_to, field_position, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("dt_interval").none(false), py::arg("append_to"),
          py::arg("field_position"))
      .def(
          // [3] DateIntervalFormat::format
          // [4] Format::format
          "format",
          [](const DateIntervalFormat &self, const Formattable &obj,
             UnicodeString &append_to,
             FieldPosition &field_position) -> UnicodeString & {
            ErrorCode error_code;
            auto &result =
                self.format(obj, append_to, field_position, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [5] Format::format
          "format",
          [](const Format &self, const Formattable &obj,
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
          // [6] Format::format
          "format",
          [](const Format &self, const Formattable &obj,
             UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  fmt.def(
         "format_to_value",
         [](const DateIntervalFormat &self, Calendar &from_calendar,
            Calendar &to_calendar) {
           ErrorCode error_code;
           auto result =
               self.formatToValue(from_calendar, to_calendar, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("from_calendar"), py::arg("to_calendar"))
      .def(
          "format_to_value",
          [](const DateIntervalFormat &self, const DateInterval &dt_interval) {
            ErrorCode error_code;
            auto result = self.formatToValue(dt_interval, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("dt_interval"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fmt.def(
      "get_context",
      [](const DateIntervalFormat &self, UDisplayContextType type) {
        ErrorCode error_code;
        auto result = self.getContext(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  fmt.def("get_date_format", &DateIntervalFormat::getDateFormat,
          py::return_value_policy::reference);

  fmt.def("get_date_interval_info", &DateIntervalFormat::getDateIntervalInfo,
          py::return_value_policy::reference);

  fmt.def(
      "get_time_zone",
      [](const DateIntervalFormat &self)
          -> std::variant<const BasicTimeZone *, const TimeZone *> {
        auto tz = &self.getTimeZone();
        auto btz = dynamic_cast<const BasicTimeZone *>(tz);
        if (btz) {
          return btz;
        }
        return tz;
      },
      py::return_value_policy::reference);

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fmt.def(
      "set_context",
      [](DateIntervalFormat &self, UDisplayContext value) {
        ErrorCode error_code;
        self.setContext(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  fmt.def(
      "set_date_interval_info",
      [](DateIntervalFormat &self,
         const DateIntervalInfo &new_interval_patterns) {
        ErrorCode error_code;
        self.setDateIntervalInfo(new_interval_patterns, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("new_interval_patterns"));

  fmt.def("set_time_zone", &DateIntervalFormat::setTimeZone, py::arg("zone"));
}

#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/basictz.h>
#include <unicode/dtitvfmt.h>

using namespace icu;

void init_dtitvfmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  // icu::FormattedDateInterval
  py::class_<FormattedDateInterval, UMemory, FormattedValue> fdi(m, "FormattedDateInterval");
  // FIXME: Implement "icu::FormattedDateInterval::FormattedDateInterval(FormattedDateInterval &&src)".
  fdi.def(py::init<>());
  fdi.def(
      "append_to",
      [](FormattedDateInterval &self, Appendable &appendable) -> Appendable & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.appendTo(appendable, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"));
  fdi.def(
      "next_position",
      [](const FormattedDateInterval &self, ConstrainedFieldPosition &cfpos) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.nextPosition(cfpos, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("cfpos"));
  fdi.def("to_string", [](const FormattedDateInterval &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  fdi.def("to_temp_string", [](const FormattedDateInterval &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toTempString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  // icu::DateIntervalFormat
  py::class_<DateIntervalFormat, Format> fmt(m, "DateIntervalFormat");
  fmt.def(py::self != py::self, py::arg("other")).def(py::self == py::self, py::arg("other"));
  fmt.def("__copy__", &DateIntervalFormat::clone)
      .def(
          "__deepcopy__", [](const DateIntervalFormat &self, py::dict) { return self.clone(); }, py::arg("memo"));
  // FIXME: Implement "void icu::DateIntervalFormat::adoptTimeZone(TimeZone *zoneToAdopt)".
  fmt.def("clone", &DateIntervalFormat::clone);
  fmt.def_static(
         // [1] DateIntervalFormat::createInstance
         "create_instance",
         [](const UnicodeString &skeleton, const DateIntervalInfo &dtitvinf) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = DateIntervalFormat::createInstance(skeleton, dtitvinf, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("skeleton"), py::arg("dtitvinf"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "create_instance",
          [](const char16_t *skeleton, const DateIntervalInfo &dtitvinf) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, dtitvinf, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("dtitvinf"))
      .def_static(
          // [2] DateIntervalFormat::createInstance
          "create_instance",
          [](const UnicodeString &skeleton, const Locale &locale, const DateIntervalInfo &dtitvinf) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, dtitvinf, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"), py::arg("dtitvinf"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "create_instance",
          [](const char16_t *skeleton, const Locale &locale, const DateIntervalInfo &dtitvinf) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, dtitvinf, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"), py::arg("dtitvinf"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const UnicodeString &skeleton, const char *locale, const DateIntervalInfo &dtitvinf) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, dtitvinf, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"), py::arg("dtitvinf"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char16_t *skeleton, const char *locale, const DateIntervalInfo &dtitvinf) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, dtitvinf, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"), py::arg("dtitvinf"))
      .def_static(
          // [3] DateIntervalFormat::createInstance
          "create_instance",
          [](const UnicodeString &skeleton, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "create_instance",
          [](const char16_t *skeleton, const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const UnicodeString &skeleton, const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char16_t *skeleton, const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skeleton, locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("locale"))
      .def_static(
          // [4] DateIntervalFormat::createInstance
          "create_instance",
          [](const UnicodeString &skelton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skelton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "create_instance",
          [](const char16_t *skelton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = DateIntervalFormat::createInstance(skelton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"));
  fmt.def(
         // [1] DateIntervalFormat::format
         "format",
         [](const DateIntervalFormat &self, Calendar &from_calendar, Calendar &to_calendar, UnicodeString &append_to,
            FieldPosition &field_position) -> UnicodeString & {
           UErrorCode error_code = U_ZERO_ERROR;
           auto &result = self.format(from_calendar, to_calendar, append_to, field_position, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("from_calendar"), py::arg("to_calendar"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [2] DateIntervalFormat::format
          "format",
          [](const DateIntervalFormat &self, const DateInterval *dt_interval, UnicodeString &append_to,
             FieldPosition &field_position) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(dt_interval, append_to, field_position, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("dt_interval"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [3] DateIntervalFormat::format
          // [4] Format::format
          "format",
          [](const DateIntervalFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPosition &field_position) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, field_position, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("field_position"))
      .def(
          // [5] Format::format
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to,
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
          // [6] Format::format
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  fmt.def(
         "format_to_value",
         [](const DateIntervalFormat &self, Calendar &from_calendar, Calendar &to_calendar) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = self.formatToValue(from_calendar, to_calendar, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("from_calendar"), py::arg("to_calendar"))
      .def(
          "format_to_value",
          [](const DateIntervalFormat &self, const DateInterval &dt_interval) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.formatToValue(dt_interval, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("dt_interval"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fmt.def(
      "get_context",
      [](const DateIntervalFormat &self, UDisplayContextType type) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getContext(type, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
  fmt.def("get_date_format", &DateIntervalFormat::getDateFormat, py::return_value_policy::reference);
  fmt.def("get_date_interval_info", &DateIntervalFormat::getDateIntervalInfo, py::return_value_policy::reference);
  fmt.def(
      "get_time_zone",
      [](const DateIntervalFormat &self) -> std::variant<const BasicTimeZone *, const TimeZone *> {
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
        UErrorCode error_code = U_ZERO_ERROR;
        self.setContext(value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
  fmt.def(
      "set_date_interval_info",
      [](DateIntervalFormat &self, const DateIntervalInfo &new_interval_patterns) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setDateIntervalInfo(new_interval_patterns, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("new_interval_patterns"));
  fmt.def("set_time_zone", &DateIntervalFormat::setTimeZone, py::arg("zone"));
}

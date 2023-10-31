#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/basictz.h>
#include <unicode/gregocal.h>
#include <unicode/locid.h>
#include <unicode/smpdtfmt.h>

using namespace icu;

void init_gregocal(py::module &m) {
  //
  // icu::Calendar
  //
  py::class_<Calendar, UObject> cal(m, "Calendar");

  cal.def(
      "__eq__", [](const Calendar &self, const Calendar &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  cal.def(
      "__ne__", [](const Calendar &self, const Calendar &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  cal.def(
      "add",
      [](Calendar &self, UCalendarDateFields field, int32_t amount) {
        ErrorCode error_code;
        self.add(field, amount, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("field"), py::arg("amount"));

  // FIXME: Implement "void icu::Calendar::adoptTimeZone(TimeZone *value)".
  /*
  cal.def(
      "adopt_time_zone", [](Calendar &self, TimeZone *value) { self.adoptTimeZone(value ? value->clone() : nullptr); },
      py::arg("value"));
  */

  cal.def(
      "after",
      [](const Calendar &self, const Calendar &when) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.after(when, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("when"));

  cal.def(
      "before",
      [](const Calendar &self, const Calendar &when) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.before(when, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("when"));

  cal.def("clear", py::overload_cast<UCalendarDateFields>(&Calendar::clear), py::arg("field"))
      .def("clear", py::overload_cast<>(&Calendar::clear));

  cal.def("clone", &Calendar::clone);

  cal.def_static(
         "create_instance",
         [](const icupy::LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = Calendar::createInstance(icupy::to_locale(locale), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"))
      .def_static(
          "create_instance",
          [](const TimeZone &zone, const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = Calendar::createInstance(zone, icupy::to_locale(locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("zone"), py::arg("locale"))
      .def_static(
          "create_instance",
          [](const TimeZone &zone) {
            ErrorCode error_code;
            auto result = Calendar::createInstance(zone, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("zone"))
      .def_static("create_instance", []() {
        ErrorCode error_code;
        auto result = Calendar::createInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  cal.def(
      "equals",
      [](const Calendar &self, const Calendar &when) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.equals(when, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("when"));

  cal.def(
      "field_difference",
      [](Calendar &self, UDate when, UCalendarDateFields field) {
        ErrorCode error_code;
        auto result = self.fieldDifference(when, field, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("when"), py::arg("field"));

  cal.def(
      "get",
      [](const Calendar &self, UCalendarDateFields field) {
        ErrorCode error_code;
        auto result = self.get(field, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("field"));

  cal.def(
      "get_actual_maximum",
      [](const Calendar &self, UCalendarDateFields field) {
        ErrorCode error_code;
        auto result = self.getActualMaximum(field, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("field"));

  cal.def(
      "get_actual_minimum",
      [](const Calendar &self, UCalendarDateFields field) {
        ErrorCode error_code;
        auto result = self.getActualMinimum(field, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("field"));

  cal.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = Calendar::getAvailableLocales(count);
        std::vector<const Locale *> result(count, nullptr);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);

  cal.def(
      "get_day_of_week_type",
      [](const Calendar &self, UCalendarDaysOfWeek day_of_week) {
        ErrorCode error_code;
        auto result = self.getDayOfWeekType(day_of_week, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("day_of_week"));

  cal.def("get_first_day_of_week", [](const Calendar &self) {
    ErrorCode error_code;
    auto result = self.getFirstDayOfWeek(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  cal.def("get_greatest_minimum", py::overload_cast<UCalendarDateFields>(&Calendar::getGreatestMinimum, py::const_),
          py::arg("field"));

  cal.def_static(
      "get_keyword_values_for_locale",
      [](const char *key, const icupy::LocaleVariant &locale, py::bool_ commonly_used) {
        ErrorCode error_code;
        auto result = Calendar::getKeywordValuesForLocale(key, icupy::to_locale(locale), commonly_used, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("key"), py::arg("locale"), py::arg("commonly_used"));

  cal.def("get_least_maximum", py::overload_cast<UCalendarDateFields>(&Calendar::getLeastMaximum, py::const_),
          py::arg("field"));

  cal.def(
      "get_locale",
      [](const Calendar &self, ULocDataLocaleType type) {
        ErrorCode error_code;
        auto result = self.getLocale(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));

  cal.def("get_maximum", py::overload_cast<UCalendarDateFields>(&Calendar::getMaximum, py::const_), py::arg("field"));

  cal.def("get_minimal_days_in_first_week", &Calendar::getMinimalDaysInFirstWeek);

  cal.def("get_minimum", py::overload_cast<UCalendarDateFields>(&Calendar::getMinimum, py::const_), py::arg("field"));

  cal.def_static("get_now", &Calendar::getNow);

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  cal.def("get_repeated_wall_time_option", &Calendar::getRepeatedWallTimeOption);

  cal.def("get_skipped_wall_time_option", &Calendar::getSkippedWallTimeOption);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  cal.def("get_temporal_month_code", [](const Calendar &self) {
    ErrorCode error_code;
    auto result = self.getTemporalMonthCode(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

  cal.def("get_time", [](const Calendar &self) {
    ErrorCode error_code;
    auto result = self.getTime(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  cal.def(
      "get_time_zone",
      [](const Calendar &self) -> std::variant<const BasicTimeZone *, const TimeZone *> {
        auto tz = &self.getTimeZone();
        auto btz = dynamic_cast<const BasicTimeZone *>(tz);
        if (btz) {
          return btz;
        }
        return tz;
      },
      py::return_value_policy::reference);

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  cal.def("get_type", &Calendar::getType);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  cal.def(
      "get_weekend_transition",
      [](const Calendar &self, UCalendarDaysOfWeek day_of_week) {
        ErrorCode error_code;
        auto result = self.getWeekendTransition(day_of_week, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("day_of_week"));

  cal.def("in_daylight_time", [](const Calendar &self) -> py::bool_ {
    ErrorCode error_code;
    auto result = self.inDaylightTime(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  cal.def("in_temporal_leap_year", [](const Calendar &self) {
    ErrorCode error_code;
    auto result = self.inTemporalLeapYear(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

  cal.def(
      "is_equivalent_to",
      [](const Calendar &self, const Calendar &other) -> py::bool_ { return self.isEquivalentTo(other); },
      py::arg("other"));

  cal.def("is_lenient", [](const Calendar &self) -> py::bool_ { return self.isLenient(); });

  cal.def(
      "is_set", [](const Calendar &self, UCalendarDateFields field) -> py::bool_ { return self.isSet(field); },
      py::arg("field"));

  cal.def(
         "is_weekend",
         [](const Calendar &self, UDate date) -> py::bool_ {
           ErrorCode error_code;
           auto result = self.isWeekend(date, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("date"))
      .def("is_weekend", [](const Calendar &self) -> py::bool_ { return self.isWeekend(); });

  cal.def("orphan_time_zone", [](Calendar &self) -> std::variant<BasicTimeZone *, TimeZone *> {
    auto tz = self.orphanTimeZone();
    auto btz = dynamic_cast<BasicTimeZone *>(tz);
    if (btz) {
      return btz;
    }
    return tz;
  });

  cal.def(
      "roll",
      [](Calendar &self, UCalendarDateFields field, int32_t amount) {
        ErrorCode error_code;
        self.roll(field, amount, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("field"), py::arg("amount"));

  cal.def("set", py::overload_cast<int32_t, int32_t, int32_t>(&Calendar::set), py::arg("year"), py::arg("month"),
          py::arg("date"))
      .def("set", py::overload_cast<int32_t, int32_t, int32_t, int32_t, int32_t>(&Calendar::set), py::arg("year"),
           py::arg("month"), py::arg("date"), py::arg("hour"), py::arg("minute"))
      .def("set", py::overload_cast<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t>(&Calendar::set),
           py::arg("year"), py::arg("month"), py::arg("date"), py::arg("hour"), py::arg("minute"), py::arg("second"))
      .def("set", py::overload_cast<UCalendarDateFields, int32_t>(&Calendar::set), py::arg("field"), py::arg("value"));

  cal.def("set_first_day_of_week", py::overload_cast<UCalendarDaysOfWeek>(&Calendar::setFirstDayOfWeek),
          py::arg("value"));

  cal.def(
      "set_lenient", [](Calendar &self, py::bool_ lenient) { self.setLenient(lenient); }, py::arg("lenient"));

  cal.def("set_minimal_days_in_first_week", &Calendar::setMinimalDaysInFirstWeek, py::arg("value"));

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  cal.def("set_repeated_wall_time_option", &Calendar::setRepeatedWallTimeOption, py::arg("option"));

  cal.def("set_skipped_wall_time_option", &Calendar::setSkippedWallTimeOption, py::arg("option"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  cal.def(
      "set_temporal_month_code",
      [](Calendar &self, const char *temporal_month) {
        ErrorCode error_code;
        self.setTemporalMonthCode(temporal_month, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("temporal_month").none(false));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

  cal.def(
      "set_time",
      [](Calendar &self, UDate date) {
        ErrorCode error_code;
        self.setTime(date, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("date"));

  cal.def("set_time_zone", &Calendar::setTimeZone, py::arg("zone"));

  //
  // icu::GregorianCalendar
  //
  py::class_<GregorianCalendar, Calendar> gc(m, "GregorianCalendar");

  //
  // icu::GregorianCalendar::EEras
  //
  py::enum_<GregorianCalendar::EEras>(gc, "EEras", py::arithmetic(),
                                      "Useful constants for *GregorianCalendar* and *TimeZone*.")
      .value("BC", GregorianCalendar::EEras::BC)
      .value("AD", GregorianCalendar::EEras::AD)
      .export_values();

  //
  // icu::GregorianCalendar
  //
  gc.def(py::init([]() {
      ErrorCode error_code;
      auto result = std::make_unique<GregorianCalendar>(error_code);
      if (error_code.isFailure()) {
        throw icupy::ICUError(error_code);
      }
      return result;
    }))
      .def(py::init([](const TimeZone &zone) {
             ErrorCode error_code;
             auto result = std::make_unique<GregorianCalendar>(zone, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("zone"))
      .def(py::init([](const icupy::LocaleVariant &locale) {
             ErrorCode error_code;
             auto result = std::make_unique<GregorianCalendar>(icupy::to_locale(locale), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("locale"))
      .def(py::init([](const TimeZone &zone, const icupy::LocaleVariant &locale) {
             ErrorCode error_code;
             auto result = std::make_unique<GregorianCalendar>(zone, icupy::to_locale(locale), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("zone"), py::arg("locale"))
      .def(py::init([](int32_t year, int32_t month, int32_t date) {
             ErrorCode error_code;
             auto result = std::make_unique<GregorianCalendar>(year, month, date, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("year"), py::arg("month"), py::arg("date"))
      .def(py::init([](int32_t year, int32_t month, int32_t date, int32_t hour, int32_t minute) {
             ErrorCode error_code;
             auto result = std::make_unique<GregorianCalendar>(year, month, date, hour, minute, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("year"), py::arg("month"), py::arg("date"), py::arg("hour"), py::arg("minute"))
      .def(py::init([](int32_t year, int32_t month, int32_t date, int32_t hour, int32_t minute, int32_t second) {
             ErrorCode error_code;
             auto result = std::make_unique<GregorianCalendar>(year, month, date, hour, minute, second, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("year"), py::arg("month"), py::arg("date"), py::arg("hour"), py::arg("minute"), py::arg("second"))
      .def(py::init<const GregorianCalendar &>(), py::arg("other"));

  gc.def("__copy__", &GregorianCalendar::clone);

  gc.def(
      "__deepcopy__", [](const GregorianCalendar &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  gc.def("__repr__", [](const Calendar &self) {
    std::stringstream ss;
    ErrorCode error_code;
    auto fmt = SimpleDateFormat(u"yyyy-MM-dd'T'HH:mm:ss.SSSXXX", error_code);
    fmt.setCalendar(self);
    UnicodeString dest;
    fmt.format(self.getTime(error_code), dest);
    std::string s;
    dest.toUTF8String(s);
    ss << "<GregorianCalendar('" << s << "')>";
    return ss.str();
  });

  gc.def("clone", &GregorianCalendar::clone);

  gc.def("get_gregorian_change", &GregorianCalendar::getGregorianChange);

  gc.def(
      "is_leap_year", [](const GregorianCalendar &self, int32_t year) -> py::bool_ { return self.isLeapYear(year); },
      py::arg("year"));

  gc.def(
      "set_gregorian_change",
      [](GregorianCalendar &self, UDate date) {
        ErrorCode error_code;
        self.setGregorianChange(date, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("date"));
}

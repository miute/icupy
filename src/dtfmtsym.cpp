#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dtfmtsym.h>

using namespace icu;

void init_dtfmtsym(py::module &m) {
  // icu::DateFormatSymbols
  py::class_<DateFormatSymbols, UObject> dfs(m, "DateFormatSymbols");

  py::enum_<DateFormatSymbols::DtContextType>(dfs, "DtContextType", py::arithmetic(),
                                              "Selector for date formatting context.")
      .value("FORMAT", DateFormatSymbols::DtContextType::FORMAT)
      .value("STANDALONE", DateFormatSymbols::DtContextType::STANDALONE)
#ifndef U_HIDE_DEPRECATED_API
      .value("DT_CONTEXT_COUNT", DateFormatSymbols::DtContextType::DT_CONTEXT_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<DateFormatSymbols::DtWidthType>(dfs, "DtWidthType", py::arithmetic(), "Selector for date formatting width.")
      .value("ABBREVIATED", DateFormatSymbols::DtWidthType::ABBREVIATED)
      .value("WIDE", DateFormatSymbols::DtWidthType::WIDE)
      .value("NARROW", DateFormatSymbols::DtWidthType::NARROW)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("SHORT", DateFormatSymbols::DtWidthType::SHORT,
             "Short width is currently only supported for weekday names.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#ifndef U_HIDE_DEPRECATED_API
      .value("DT_WIDTH_COUNT", DateFormatSymbols::DtWidthType::DT_WIDTH_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  dfs.def(py::init([]() {
       UErrorCode error_code = U_ZERO_ERROR;
       auto result = std::make_unique<DateFormatSymbols>(error_code);
       if (U_FAILURE(error_code)) {
         throw ICUError(error_code);
       }
       return result;
     }))
      .def(py::init([](const Locale &locale) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<DateFormatSymbols>(locale, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("locale"))
      .def(py::init<const DateFormatSymbols &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  dfs.def(
      "get_am_pm_strings",
      [](const DateFormatSymbols &self) {
        int32_t count;
        auto p = self.getAmPmStrings(count);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  dfs.def(
      "get_era_names",
      [](const DateFormatSymbols &self) {
        int32_t count;
        auto p = self.getEraNames(count);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  dfs.def(
      "get_eras",
      [](const DateFormatSymbols &self) {
        int32_t count;
        auto p = self.getEras(count);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  dfs.def(
      "get_locale",
      [](const DateFormatSymbols &self, ULocDataLocaleType type) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getLocale(type, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
  dfs.def("get_local_pattern_chars", &DateFormatSymbols::getLocalPatternChars, py::arg("result"));
  dfs.def(
         "get_months",
         [](const DateFormatSymbols &self) {
           int32_t count;
           auto p = self.getMonths(count);
           std::vector<const UnicodeString *> result(count);
           for (int32_t i = 0; i < count; ++i) {
             result[i] = p + i;
           }
           return result;
         },
         py::return_value_policy::reference)
      .def(
          "get_months",
          [](const DateFormatSymbols &self, DateFormatSymbols::DtContextType context,
             DateFormatSymbols::DtWidthType width) {
            int32_t count;
            auto p = self.getMonths(count, context, width);
            std::vector<const UnicodeString *> result(count);
            for (int32_t i = 0; i < count; ++i) {
              result[i] = p + i;
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("context"), py::arg("width"));
  dfs.def(
      "get_narrow_eras",
      [](const DateFormatSymbols &self) {
        int32_t count;
        auto p = self.getNarrowEras(count);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  dfs.def_static("get_pattern_uchars", &DateFormatSymbols::getPatternUChars);
  dfs.def(
      "get_quarters",
      [](const DateFormatSymbols &self, DateFormatSymbols::DtContextType context,
         DateFormatSymbols::DtWidthType width) {
        int32_t count;
        auto p = self.getQuarters(count, context, width);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("context"), py::arg("width"));
  dfs.def(
      "get_short_months",
      [](const DateFormatSymbols &self) {
        int32_t count;
        auto p = self.getShortMonths(count);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  dfs.def(
      "get_short_weekdays",
      [](const DateFormatSymbols &self) {
        int32_t count;
        auto p = self.getShortWeekdays(count);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  dfs.def(
         "get_weekdays",
         [](const DateFormatSymbols &self) {
           int32_t count;
           auto p = self.getWeekdays(count);
           std::vector<const UnicodeString *> result(count);
           for (int32_t i = 0; i < count; ++i) {
             result[i] = p + i;
           }
           return result;
         },
         py::return_value_policy::reference)
      .def(
          "get_weekdays",
          [](const DateFormatSymbols &self, DateFormatSymbols::DtContextType context,
             DateFormatSymbols::DtWidthType width) {
            int32_t count;
            auto p = self.getWeekdays(count, context, width);
            std::vector<const UnicodeString *> result(count);
            for (int32_t i = 0; i < count; ++i) {
              result[i] = p + i;
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("context"), py::arg("width"));
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  dfs.def(
      "get_year_names",
      [](const DateFormatSymbols &self, DateFormatSymbols::DtContextType context,
         DateFormatSymbols::DtWidthType width) {
        int32_t count;
        auto p = self.getYearNames(count, context, width);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("context"), py::arg("width"));
  dfs.def(
      "get_zodiac_names",
      [](const DateFormatSymbols &self, DateFormatSymbols::DtContextType context,
         DateFormatSymbols::DtWidthType width) {
        int32_t count;
        auto p = self.getZodiacNames(count, context, width);
        std::vector<const UnicodeString *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("context"), py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  dfs.def(
      "set_am_pm_strings",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &ampms, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(ampms.size());
        }
        std::vector<UnicodeString> _ampms(std::begin(ampms), std::end(ampms));
        self.setAmPmStrings(_ampms.data(), count);
      },
      py::arg("ampms"), py::arg("count") = -1);
  dfs.def(
      "set_era_names",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &era_names, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(era_names.size());
        }
        std::vector<UnicodeString> _era_names(std::begin(era_names), std::end(era_names));
        self.setEraNames(_era_names.data(), count);
      },
      py::arg("era_names"), py::arg("count") = -1);
  dfs.def(
      "set_eras",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &eras, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(eras.size());
        }
        std::vector<UnicodeString> _eras(std::begin(eras), std::end(eras));
        self.setEras(_eras.data(), count);
      },
      py::arg("eras"), py::arg("count") = -1);
  dfs.def("set_local_pattern_chars", &DateFormatSymbols::setLocalPatternChars, py::arg("new_local_pattern_chars"))
      .def(
          // const char16_t *new_local_pattern_chars -> const UnicodeString &new_local_pattern_chars
          "set_local_pattern_chars",
          [](DateFormatSymbols &self, const char16_t *new_local_pattern_chars) {
            self.setLocalPatternChars(new_local_pattern_chars);
          },
          py::arg("new_local_pattern_chars"));
  dfs.def(
         "set_months",
         [](DateFormatSymbols &self, const std::list<UnicodeString> &months, int32_t count) {
           if (count == -1) {
             count = static_cast<int32_t>(months.size());
           }
           std::vector<UnicodeString> _months(std::begin(months), std::end(months));
           self.setMonths(_months.data(), count);
         },
         py::arg("months"), py::arg("count") = -1)
      .def(
          "set_months",
          [](DateFormatSymbols &self, const std::list<UnicodeString> &months, int32_t count,
             DateFormatSymbols::DtContextType context, DateFormatSymbols::DtWidthType width) {
            std::vector<UnicodeString> _months(std::begin(months), std::end(months));
            if (count == -1) {
              count = static_cast<int32_t>(months.size());
            }
            self.setMonths(_months.data(), count, context, width);
          },
          py::arg("months"), py::arg("count"), py::arg("context"), py::arg("width"));
  dfs.def(
      "set_narrow_eras",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &narrow_eras, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(narrow_eras.size());
        }
        std::vector<UnicodeString> _narrow_eras(std::begin(narrow_eras), std::end(narrow_eras));
        self.setNarrowEras(_narrow_eras.data(), count);
      },
      py::arg("narrow_eras"), py::arg("count") = -1);
  dfs.def(
      "set_quarters",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &quarters, int32_t count,
         DateFormatSymbols::DtContextType context, DateFormatSymbols::DtWidthType width) {
        std::vector<UnicodeString> _quarters(std::begin(quarters), std::end(quarters));
        if (count == -1) {
          count = static_cast<int32_t>(quarters.size());
        }
        self.setQuarters(_quarters.data(), count, context, width);
      },
      py::arg("quarters"), py::arg("count"), py::arg("context"), py::arg("width"));
  dfs.def(
      "set_short_months",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &short_months, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(short_months.size());
        }
        std::vector<UnicodeString> _short_months(std::begin(short_months), std::end(short_months));
        self.setShortMonths(_short_months.data(), count);
      },
      py::arg("short_months"), py::arg("count") = -1);
  dfs.def(
      "set_short_weekdays",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &abbrev_weekdays, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(abbrev_weekdays.size());
        }
        std::vector<UnicodeString> _abbrev_weekdays(std::begin(abbrev_weekdays), std::end(abbrev_weekdays));
        self.setShortWeekdays(_abbrev_weekdays.data(), count);
      },
      py::arg("abbrev_weekdays"), py::arg("count") = -1);
  dfs.def(
         "set_weekdays",
         [](DateFormatSymbols &self, const std::list<UnicodeString> &weekdays, int32_t count) {
           if (count == -1) {
             count = static_cast<int32_t>(weekdays.size());
           }
           std::vector<UnicodeString> _weekdays(std::begin(weekdays), std::end(weekdays));
           self.setWeekdays(_weekdays.data(), count);
         },
         py::arg("weekdays"), py::arg("count") = -1)
      .def(
          "set_weekdays",
          [](DateFormatSymbols &self, const std::list<UnicodeString> &weekdays, int32_t count,
             DateFormatSymbols::DtContextType context, DateFormatSymbols::DtWidthType width) {
            std::vector<UnicodeString> _weekdays(std::begin(weekdays), std::end(weekdays));
            if (count == -1) {
              count = static_cast<int32_t>(weekdays.size());
            }
            self.setWeekdays(_weekdays.data(), count, context, width);
          },
          py::arg("weekdays"), py::arg("count"), py::arg("context"), py::arg("width"));
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  dfs.def(
      "set_year_names",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &year_names, int32_t count,
         DateFormatSymbols::DtContextType context, DateFormatSymbols::DtWidthType width) {
        std::vector<UnicodeString> _year_names(std::begin(year_names), std::end(year_names));
        if (count == -1) {
          count = static_cast<int32_t>(year_names.size());
        }
        self.setYearNames(_year_names.data(), count, context, width);
      },
      py::arg("year_names"), py::arg("count"), py::arg("context"), py::arg("width"));
  dfs.def(
      "set_zodiac_names",
      [](DateFormatSymbols &self, const std::list<UnicodeString> &zodiac_names, int32_t count,
         DateFormatSymbols::DtContextType context, DateFormatSymbols::DtWidthType width) {
        std::vector<UnicodeString> _zodiac_names(std::begin(zodiac_names), std::end(zodiac_names));
        if (count == -1) {
          count = static_cast<int32_t>(zodiac_names.size());
        }
        self.setZodiacNames(_zodiac_names.data(), count, context, width);
      },
      py::arg("zodiac_names"), py::arg("count"), py::arg("context"), py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  // FIXME: Implement "void DateFormatSymbols::setZoneStrings(const UnicodeString *const *strings, int32_t rowCount,
  // int32_t columnCount)".
}

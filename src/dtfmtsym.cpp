#include "main.hpp"
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dtfmtsym.h>

using namespace icu;

void init_dtfmtsym(py::module &m) {
  // icu::DateFormatSymbols
  py::class_<DateFormatSymbols, UObject> dfs(m, "DateFormatSymbols");

  py::enum_<DateFormatSymbols::DtContextType>(dfs, "DtContextType", py::arithmetic())
      .value("FORMAT", DateFormatSymbols::DtContextType::FORMAT)
      .value("STANDALONE", DateFormatSymbols::DtContextType::STANDALONE)
#ifndef U_HIDE_DEPRECATED_API
      .value("DT_CONTEXT_COUNT", DateFormatSymbols::DtContextType::DT_CONTEXT_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<DateFormatSymbols::DtWidthType>(dfs, "DtWidthType", py::arithmetic())
      .value("ABBREVIATED", DateFormatSymbols::DtWidthType::ABBREVIATED)
      .value("WIDE", DateFormatSymbols::DtWidthType::WIDE)
      .value("NARROW", DateFormatSymbols::DtWidthType::NARROW)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("SHORT", DateFormatSymbols::DtWidthType::SHORT)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#ifndef U_HIDE_DEPRECATED_API
      .value("DT_WIDTH_COUNT", DateFormatSymbols::DtWidthType::DT_WIDTH_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  dfs.def(py::init([]() {
       UErrorCode error_code = U_ZERO_ERROR;
       auto result = std::make_unique<DateFormatSymbols>(error_code);
       if (U_FAILURE(error_code)) {
         throw ICUException(error_code);
       }
       return result;
     }))
      .def(py::init([](const Locale &locale) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<DateFormatSymbols>(locale, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUException(error_code);
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
          throw ICUException(error_code);
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
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &ampms, std::optional<int32_t> count) {
        self.setAmPmStrings(ampms.data(), count.value_or((int32_t)ampms.size()));
      },
      py::arg("ampms"), py::arg("count") = std::nullopt);
  dfs.def(
      "set_era_names",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &era_names, std::optional<int32_t> count) {
        self.setEraNames(era_names.data(), count.value_or((int32_t)era_names.size()));
      },
      py::arg("era_names"), py::arg("count") = std::nullopt);
  dfs.def(
      "set_eras",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &eras, std::optional<int32_t> count) {
        self.setEras(eras.data(), count.value_or((int32_t)eras.size()));
      },
      py::arg("eras"), py::arg("count") = std::nullopt);
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
         [](DateFormatSymbols &self, const std::vector<UnicodeString> &months, std::optional<int32_t> count) {
           self.setMonths(months.data(), count.value_or((int32_t)months.size()));
         },
         py::arg("months"), py::arg("count") = std::nullopt)
      .def(
          "set_months",
          [](DateFormatSymbols &self, const std::vector<UnicodeString> &months, int32_t count,
             DateFormatSymbols::DtContextType context,
             DateFormatSymbols::DtWidthType width) { self.setMonths(months.data(), count, context, width); },
          py::arg("months"), py::arg("count"), py::arg("context"), py::arg("width"));
  dfs.def(
      "set_narrow_eras",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &narrow_eras, std::optional<int32_t> count) {
        self.setNarrowEras(narrow_eras.data(), count.value_or((int32_t)narrow_eras.size()));
      },
      py::arg("narrow_eras"), py::arg("count") = std::nullopt);
  dfs.def(
      "set_quarters",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &quarters, int32_t count,
         DateFormatSymbols::DtContextType context,
         DateFormatSymbols::DtWidthType width) { self.setQuarters(quarters.data(), count, context, width); },
      py::arg("quarters"), py::arg("count"), py::arg("context"), py::arg("width"));
  dfs.def(
      "set_short_months",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &short_months, std::optional<int32_t> count) {
        self.setShortMonths(short_months.data(), count.value_or((int32_t)short_months.size()));
      },
      py::arg("short_months"), py::arg("count") = std::nullopt);
  dfs.def(
      "set_short_weekdays",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &abbrev_weekdays, std::optional<int32_t> count) {
        self.setShortWeekdays(abbrev_weekdays.data(), count.value_or((int32_t)abbrev_weekdays.size()));
      },
      py::arg("abbrev_weekdays"), py::arg("count") = std::nullopt);
  dfs.def(
         "set_weekdays",
         [](DateFormatSymbols &self, const std::vector<UnicodeString> &weekdays, std::optional<int32_t> count) {
           self.setWeekdays(weekdays.data(), count.value_or((int32_t)weekdays.size()));
         },
         py::arg("weekdays"), py::arg("count") = std::nullopt)
      .def(
          "set_weekdays",
          [](DateFormatSymbols &self, const std::vector<UnicodeString> &weekdays, int32_t count,
             DateFormatSymbols::DtContextType context,
             DateFormatSymbols::DtWidthType width) { self.setWeekdays(weekdays.data(), count, context, width); },
          py::arg("weekdays"), py::arg("count"), py::arg("context"), py::arg("width"));
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  dfs.def(
      "set_year_names",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &year_names, int32_t count,
         DateFormatSymbols::DtContextType context,
         DateFormatSymbols::DtWidthType width) { self.setYearNames(year_names.data(), count, context, width); },
      py::arg("year_names"), py::arg("count"), py::arg("context"), py::arg("width"));
  dfs.def(
      "set_zodiac_names",
      [](DateFormatSymbols &self, const std::vector<UnicodeString> &zodiac_names, int32_t count,
         DateFormatSymbols::DtContextType context,
         DateFormatSymbols::DtWidthType width) { self.setZodiacNames(zodiac_names.data(), count, context, width); },
      py::arg("zodiac_names"), py::arg("count"), py::arg("context"), py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  // FIXME: Implement "void DateFormatSymbols::setZoneStrings(const UnicodeString *const *strings, int32_t rowCount,
  // int32_t columnCount)".
}

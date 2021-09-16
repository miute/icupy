#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
#include <pybind11/stl.h>
#include <unicode/errorcode.h>
#include <unicode/localematcher.h>

using namespace icu;
using Builder = LocaleMatcher::Builder;
using Result = LocaleMatcher::Result;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

void init_localematcher(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  py::enum_<ULocMatchDemotion>(m, "ULocMatchDemotion", py::arithmetic())
      .value("ULOCMATCH_DEMOTION_NONE", ULOCMATCH_DEMOTION_NONE)
      .value("ULOCMATCH_DEMOTION_REGION", ULOCMATCH_DEMOTION_REGION)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  py::enum_<ULocMatchDirection>(m, "ULocMatchDirection", py::arithmetic())
      .value("ULOCMATCH_DIRECTION_WITH_ONE_WAY", ULOCMATCH_DIRECTION_WITH_ONE_WAY)
      .value("ULOCMATCH_DIRECTION_ONLY_TWO_WAY", ULOCMATCH_DIRECTION_ONLY_TWO_WAY)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  py::enum_<ULocMatchFavorSubtag>(m, "ULocMatchFavorSubtag", py::arithmetic())
      .value("ULOCMATCH_FAVOR_LANGUAGE", ULOCMATCH_FAVOR_LANGUAGE)
      .value("ULOCMATCH_FAVOR_SCRIPT", ULOCMATCH_FAVOR_SCRIPT)
      .export_values();

  // icu::LocaleMatcher
  py::class_<LocaleMatcher, UMemory> lm(m, "LocaleMatcher");

  // icu::LocaleMatcher::Builder
  py::class_<Builder, UMemory> lmb(lm, "Builder");

  // icu::LocaleMatcher::Result
  py::class_<Result, UMemory> lmr(lm, "Result");

  // icu::LocaleMatcher
  lm.def(
        "get_best_match",
        [](const LocaleMatcher &self, const Locale &desired_locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.getBestMatch(desired_locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::return_value_policy::reference, py::arg("desired_locale"))
      .def(
          // const char *desired_locale -> const Locale &desired_locale
          "get_best_match",
          [](const LocaleMatcher &self, const char *desired_locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBestMatch(desired_locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("desired_locale"))
      .def(
          "get_best_match",
          [](const LocaleMatcher &self, const std::vector<Locale> &desired_locales) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto first = desired_locales.data();
            Locale::RangeIterator<const Locale *> iter(first, first + desired_locales.size());
            auto result = self.getBestMatch(iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("desired_locales"));
  lm.def(
      "get_best_match_for_list_string",
      [](const LocaleMatcher &self, const char *desired_locale_list) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getBestMatchForListString(desired_locale_list, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("desired_locale_list"));
  lm.def(
        "get_best_match_result",
        [](const LocaleMatcher &self, const Locale &desired_locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.getBestMatchResult(desired_locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("desired_locale"))
      .def(
          "get_best_match_result",
          [](const LocaleMatcher &self, const std::vector<Locale> &desired_locales) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto first = desired_locales.data();
            Locale::RangeIterator<const Locale *> iter(first, first + desired_locales.size());
            auto result = self.getBestMatchResult(iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("desired_locales"));
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  lm.def(
        "is_match",
        [](const LocaleMatcher &self, const Locale &desired, const Locale &supported) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.isMatch(desired, supported, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("desired"), py::arg("supported"))
      .def(
          // const char *desired -> const Locale &desired
          "is_match",
          [](const LocaleMatcher &self, const char *desired, const Locale &supported) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.isMatch(desired, supported, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("desired"), py::arg("supported"))
      .def(
          // const char *supported -> const Locale &supported
          "is_match",
          [](const LocaleMatcher &self, const Locale &desired, const char *supported) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.isMatch(desired, supported, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("desired"), py::arg("supported"))
      .def(
          // const char *desired -> const Locale &desired
          // const char *supported -> const Locale &supported
          "is_match",
          [](const LocaleMatcher &self, const char *desired, const char *supported) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.isMatch(desired, supported, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("desired"), py::arg("supported"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  // icu::LocaleMatcher::Builder
  lmb.def(py::init<>());
  lmb.def("add_supported_locale", &Builder::addSupportedLocale, py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "add_supported_locale",
          [](Builder &self, const char *locale) -> Builder & { return self.addSupportedLocale(locale); },
          py::arg("locale"));
  lmb.def("build", [](const Builder &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.build(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  lmb.def(
      "copy_error_to", [](const Builder &self, ErrorCode &out_error_code) { return self.copyErrorTo(out_error_code); },
      py::arg("out_error_code"));
  lmb.def("set_default_locale", &Builder::setDefaultLocale, py::arg("default_locale"));
  lmb.def("set_demotion_per_desired_locale", &Builder::setDemotionPerDesiredLocale, py::arg("demotion"));
#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  lmb.def("set_direction", &Builder::setDirection, py::arg("direction"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
  lmb.def("set_favor_subtag", &Builder::setFavorSubtag, py::arg("subtag"));
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  lmb.def("set_max_distance", &Builder::setMaxDistance, py::arg("desired"), py::arg("supported"))
      .def(
          // const char *desired -> const Locale &desired
          "set_max_distance",
          [](Builder &self, const char *desired, const Locale &supported) -> Builder & {
            return self.setMaxDistance(desired, supported);
          },
          py::arg("desired"), py::arg("supported"))
      .def(
          // const char *supported -> const Locale &supported
          "set_max_distance",
          [](Builder &self, const Locale &desired, const char *supported) -> Builder & {
            return self.setMaxDistance(desired, supported);
          },
          py::arg("desired"), py::arg("supported"))
      .def(
          // const char *desired -> const Locale &desired
          // const char *supported -> const Locale &supported
          "set_max_distance",
          [](Builder &self, const char *desired, const char *supported) -> Builder & {
            return self.setMaxDistance(desired, supported);
          },
          py::arg("desired"), py::arg("supported"));
  lmb.def("set_no_default_locale", &Builder::setNoDefaultLocale);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
  lmb.def(
      "set_supported_locales",
      [](Builder &self, const std::vector<Locale> &locales) -> Builder & {
        auto first = locales.data();
        Locale::RangeIterator<const Locale *> iter(first, first + locales.size());
        return self.setSupportedLocales(iter);
      },
      py::arg("locales"));
  lmb.def(
      "set_supported_locales_from_list_string",
      [](Builder &self, const char *locales) -> Builder & { return self.setSupportedLocalesFromListString(locales); },
      py::arg("locales"));

  // icu::LocaleMatcher::Result
  lmr.def("get_desired_index", &Result::getDesiredIndex);
  lmr.def("get_desired_locale", &Result::getDesiredLocale, py::return_value_policy::reference);
  lmr.def("get_supported_index", &Result::getSupportedIndex);
  lmr.def("get_supported_locale", &Result::getSupportedLocale, py::return_value_policy::reference);
  lmr.def("make_resolved_locale", [](const Result &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.makeResolvedLocale(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)
}

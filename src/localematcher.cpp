#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
#include <pybind11/stl.h>
#include <unicode/localematcher.h>

using namespace icu;
using Builder = LocaleMatcher::Builder;
using Result = LocaleMatcher::Result;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

void init_localematcher(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  //
  // ULocMatchDemotion
  //
  py::enum_<ULocMatchDemotion>(
      m, "ULocMatchDemotion", py::arithmetic(),
      "*Builder* option for whether all desired locales are treated equally or earlier ones are preferred.")
      .value("ULOCMATCH_DEMOTION_NONE", ULOCMATCH_DEMOTION_NONE, "All desired locales are treated equally.")
      .value("ULOCMATCH_DEMOTION_REGION", ULOCMATCH_DEMOTION_REGION,
             "Earlier desired locales are preferred. From each desired locale to the next, the distance to any "
             "supported locale is increased by an additional amount which is at least as large as most region "
             "mismatches. A later desired locale has to have a better match with some supported locale due to more "
             "than merely having the same region subtag.\n\n  "
             "For example: *Supported={en, sv} desired=[en-GB, sv]* yields *Result(en-GB, en)* because with the "
             "demotion of sv its perfect match is no better than the region distance between the earlier desired "
             "locale en-GB and en=en-US.\n\n  "
             "Notes:\n\n  "
             "* In some cases, language and/or script differences can be as small as the typical region difference. "
             "(Example: sr-Latn vs. sr-Cyrl)\n  "
             "* It is possible for certain region differences to be larger than usual, and larger than the demotion. "
             "(As of CLDR 35 there is no such case, but this is possible in future versions of the data.)")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  //
  // ULocMatchDirection
  //
  py::enum_<ULocMatchDirection>(
      m, "ULocMatchDirection", py::arithmetic(),
      "*Builder* option for whether to include or ignore one-way (fallback) match data.\n\n"
      "The *LocaleMatcher* uses CLDR languageMatch data which includes fallback (oneway=true) entries. Sometimes it is "
      "desirable to ignore those.\n\n"
      "For example, consider a web application with the UI in a given language, with a link to another, related web "
      "app. The link should include the UI language, and the target server may also use the client’s Accept-Language "
      "header data. The target server has its own list of supported languages. One may want to favor UI language "
      "consistency, that is, if there is a decent match for the original UI language, we want to use it, but not if it "
      "is merely a fallback.")
      .value("ULOCMATCH_DIRECTION_WITH_ONE_WAY", ULOCMATCH_DIRECTION_WITH_ONE_WAY,
             "Locale matching includes one-way matches such as Breton→French. (default)")
      .value("ULOCMATCH_DIRECTION_ONLY_TWO_WAY", ULOCMATCH_DIRECTION_ONLY_TWO_WAY,
             "Locale matching limited to two-way matches including e.g. Danish↔Norwegian but ignoring one-way matches.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  //
  // ULocMatchFavorSubtag
  //
  py::enum_<ULocMatchFavorSubtag>(
      m, "ULocMatchFavorSubtag", py::arithmetic(),
      "*Builder* option for whether the language subtag or the script subtag is most important.")
      .value("ULOCMATCH_FAVOR_LANGUAGE", ULOCMATCH_FAVOR_LANGUAGE,
             "Language differences are most important, then script differences, then region differences. (This is the "
             "default behavior.)")
      .value("ULOCMATCH_FAVOR_SCRIPT", ULOCMATCH_FAVOR_SCRIPT,
             "Makes script differences matter relatively more than language differences.")
      .export_values();

  //
  // icu::LocaleMatcher
  //
  py::class_<LocaleMatcher, UMemory> lm(m, "LocaleMatcher");

  //
  // icu::LocaleMatcher::Builder
  //
  py::class_<Builder, UMemory> lmb(lm, "Builder");

  //
  // icu::LocaleMatcher::Result
  //
  py::class_<Result, UMemory> lmr(lm, "Result");

  //
  // icu::LocaleMatcher
  //
  lm.def(
        "get_best_match",
        [](const LocaleMatcher &self, const icupy::LocaleVariant &desired_locale) {
          ErrorCode error_code;
          auto result = self.getBestMatch(icupy::to_locale(desired_locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::return_value_policy::reference, py::arg("desired_locale"))
      .def(
          "get_best_match",
          [](const LocaleMatcher &self, const std::vector<Locale> &desired_locales) {
            ErrorCode error_code;
            auto first = desired_locales.data();
            Locale::RangeIterator<const Locale *> iter(first, first + desired_locales.size());
            auto result = self.getBestMatch(iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("desired_locales"));

  lm.def(
      "get_best_match_for_list_string",
      [](const LocaleMatcher &self, const char *desired_locale_list) {
        ErrorCode error_code;
        auto result = self.getBestMatchForListString(desired_locale_list, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("desired_locale_list"));

  lm.def(
        "get_best_match_result",
        [](const LocaleMatcher &self, const Locale &desired_locale) {
          ErrorCode error_code;
          auto result = self.getBestMatchResult(desired_locale, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("desired_locale"))
      .def(
          "get_best_match_result",
          [](const LocaleMatcher &self, const std::vector<Locale> &desired_locales) {
            ErrorCode error_code;
            auto first = desired_locales.data();
            Locale::RangeIterator<const Locale *> iter(first, first + desired_locales.size());
            auto result = self.getBestMatchResult(iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("desired_locales"));

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  lm.def(
      "is_match",
      [](const LocaleMatcher &self, const icupy::LocaleVariant &desired,
         const icupy::LocaleVariant &supported) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.isMatch(icupy::to_locale(desired), icupy::to_locale(supported), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("desired"), py::arg("supported"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  //
  // icu::LocaleMatcher::Builder
  //
  lmb.def(py::init<>());

  lmb.def(
      "add_supported_locale",
      [](Builder &self, const icupy::LocaleVariant &locale) -> Builder & {
        return self.addSupportedLocale(icupy::to_locale(locale));
      },
      py::arg("locale"));

  lmb.def("build", [](const Builder &self) {
    ErrorCode error_code;
    auto result = self.build(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  lmb.def(
      "copy_error_to",
      [](const Builder &self, ErrorCode &out_error_code) -> py::bool_ { return self.copyErrorTo(out_error_code); },
      py::arg("out_error_code"));

  lmb.def("set_default_locale", &Builder::setDefaultLocale, py::arg("default_locale"));

  lmb.def("set_demotion_per_desired_locale", &Builder::setDemotionPerDesiredLocale, py::arg("demotion"));

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  lmb.def("set_direction", &Builder::setDirection, py::arg("direction"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  lmb.def("set_favor_subtag", &Builder::setFavorSubtag, py::arg("subtag"));

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  lmb.def(
      "set_max_distance",
      [](Builder &self, const icupy::LocaleVariant &desired, const icupy::LocaleVariant &supported) -> Builder & {
        return self.setMaxDistance(icupy::to_locale(desired), icupy::to_locale(supported));
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

  //
  // icu::LocaleMatcher::Result
  //
  lmr.def("get_desired_index", &Result::getDesiredIndex);

  lmr.def("get_desired_locale", &Result::getDesiredLocale, py::return_value_policy::reference);

  lmr.def("get_supported_index", &Result::getSupportedIndex);

  lmr.def("get_supported_locale", &Result::getSupportedLocale, py::return_value_policy::reference);

  lmr.def("make_resolved_locale", [](const Result &self) {
    ErrorCode error_code;
    auto result = self.makeResolvedLocale(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
#include <pybind11/native_enum.h>
#include <pybind11/stl.h>
#include <unicode/localematcher.h>

using namespace icu;
using Builder = LocaleMatcher::Builder;
using Result = LocaleMatcher::Result;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

void init_localematcher(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  //
  // enum ULocMatchDemotion
  //
  py::native_enum<ULocMatchDemotion>(m, "ULocMatchDemotion", "enum.IntEnum",
                                     R"doc(
:class:`~LocaleMatcher.Builder` option for whether all desired locales are
treated equally or earlier ones are preferred.
      )doc")
      .value("ULOCMATCH_DEMOTION_NONE", ULOCMATCH_DEMOTION_NONE, R"doc(
             All desired locales are treated equally.
             )doc")
      .value("ULOCMATCH_DEMOTION_REGION", ULOCMATCH_DEMOTION_REGION, R"doc(
             Earlier desired locales are preferred.

             From each desired locale to the next, the distance to any
             supported locale is increased by an additional amount which is
             at least as large as most region mismatches. A later desired
             locale has to have a better match with some supported locale due
             to more than merely having the same region subtag.

             For example: ``Supported={en, sv} desired=[en-GB, sv]`` yields
             ``Result(en-GB, en)`` because with the demotion of sv its perfect
             match is no better than the region distance between the earlier
             desired locale en-GB and en=en-US.

             .. note::

                - In some cases, language and/or script differences can be as
                  small as the typical region difference.
                  (Example: sr-Latn vs. sr-Cyrl)
                - It is possible for certain region differences to be larger
                  than usual, and larger than the demotion.
                  (As of CLDR 35 there is no such case, but this is possible in
                  future versions of the data.)
             )doc")
      .export_values()
      .finalize();

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  //
  // enum ULocMatchDirection
  //
  py::native_enum<ULocMatchDirection>(m, "ULocMatchDirection", "enum.IntEnum",
                                      R"doc(
:class:`~LocaleMatcher.Builder` option for whether to include or ignore
one-way (fallback) match data.

The :class:`LocaleMatcher` uses CLDR languageMatch data which includes
fallback (oneway=true) entries. Sometimes it is desirable to ignore those.

For example, consider a web application with the UI in a given language, with
a link to another, related web app. The link should include the UI language,
and the target server may also use the client’s Accept-Language header data.
The target server has its own list of supported languages. One may want to
favor UI language consistency, that is, if there is a decent match for the
original UI language, we want to use it, but not if it is merely a fallback.
      )doc")
      .value("ULOCMATCH_DIRECTION_WITH_ONE_WAY",
             ULOCMATCH_DIRECTION_WITH_ONE_WAY, R"doc(
             Locale matching includes one-way matches such as Breton→French.
             (default)
             )doc")
      .value("ULOCMATCH_DIRECTION_ONLY_TWO_WAY",
             ULOCMATCH_DIRECTION_ONLY_TWO_WAY, R"doc(
             Locale matching limited to two-way matches including e.g.
             Danish↔Norwegian but ignoring one-way matches.
             )doc")
      .export_values()
      .finalize();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  //
  // enum ULocMatchFavorSubtag
  //
  py::native_enum<ULocMatchFavorSubtag>(m, "ULocMatchFavorSubtag",
                                        "enum.IntEnum", R"doc(
:class:`~LocaleMatcher.Builder` option for whether the language subtag or the
script subtag is most important.
      )doc")
      .value("ULOCMATCH_FAVOR_LANGUAGE", ULOCMATCH_FAVOR_LANGUAGE, R"doc(
             Language differences are most important, then script differences,
             then region differences. (This is the default behavior.)
             )doc")
      .value("ULOCMATCH_FAVOR_SCRIPT", ULOCMATCH_FAVOR_SCRIPT, R"doc(
             Makes script differences matter relatively more than language
             differences.
             )doc")
      .export_values()
      .finalize();

  //
  // class icu::LocaleMatcher
  //
  py::class_<LocaleMatcher, UMemory> lm(m, "LocaleMatcher", R"doc(
      Immutable class that picks the best match between a user's desired
      locales and an application's supported locales.

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         matcher = (
             icu.LocaleMatcher.Builder()
             .set_supported_locales_from_list_string("ja, en-US, fr-FR, de-DE")
             .set_default_locale(icu.Locale.get_english())
             .build()
         )
         desired_locales = [icu.Locale("es_AR"), icu.Locale("fr_CA"), icu.Locale("de_CH")]
         matcher.get_best_match(desired_locales)  # Locale('fr_FR')
         result = matcher.get_best_match_result(desired_locales)
         result.get_desired_index(), result.get_desired_locale()  # (1, Locale('fr_CA'))
         result.get_supported_index(), result.get_supported_locale()  # (2, Locale('fr_FR'))
         result.make_resolved_locale()  # Locale('fr_CA')
      )doc");

  //
  // class icu::LocaleMatcher::Builder
  //
  py::class_<Builder, UMemory> lmb(lm, "Builder", R"doc(
      :class:`LocaleMatcher` builder.

      .. seealso::

         :class:`LocaleMatcher`
      )doc");

  //
  // class icu::LocaleMatcher::Result
  //
  py::class_<Result, UMemory> lmr(lm, "Result", R"doc(
      Data for the best-matching pair of a desired and a supported locale.

      .. seealso::

         :meth:`LocaleMatcher.get_best_match_result`
      )doc");

  //
  // class icu::LocaleMatcher
  //
  lm.def(
        "get_best_match",
        [](const LocaleMatcher &self,
           const icupy::LocaleVariant &desired_locale) {
          ErrorCode error_code;
          auto result =
              self.getBestMatch(icupy::to_locale(desired_locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::return_value_policy::reference, py::arg("desired_locale"), R"doc(
      Return the best matching locale from this matcher's supported locales for
      the specified desired locale.

      *desired_locale* is the desired locale, typically the user's language.
      )doc")
      .def(
          "get_best_match",
          [](const LocaleMatcher &self,
             const std::vector<Locale> &desired_locales) {
            auto first = desired_locales.data();
            Locale::RangeIterator<const Locale *> iter(
                first, first + desired_locales.size());
            ErrorCode error_code;
            auto result = self.getBestMatch(iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("desired_locales"), R"doc(
      Return the best matching locale from this matcher's supported locales for
      the specified list of desired locales.

      *desired_locales* is a list of desired locales, typically in descending
      order of the user's language preferences.
      )doc");

  lm.def(
      "get_best_match_for_list_string",
      [](const LocaleMatcher &self, const std::string &desired_locale_list) {
        ErrorCode error_code;
        auto result =
            self.getBestMatchForListString(desired_locale_list, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("desired_locale_list"), R"doc(
      Parse the Accept-Language string as desired locales and return the best
      matching locale from these locales and this matcher's supported locales.

      *desired_locale_list* is an Accept-Language string, such as
      "af, en, fr;q=0.9". For more information, see
      `RFC 2616 Section 14.4 <https://tools.ietf.org/html/rfc2616#section-14.4>`__.

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         matcher = (
             icu.LocaleMatcher.Builder()
             .set_supported_locales_from_list_string("ja, en-US, fr-FR, de-DE")
             .set_default_locale(icu.Locale.get_english())
             .build()
         )
         matcher.get_best_match_for_list_string("es-AR, fr-CA, de-CH")  # Locale('fr_FR')
      )doc");

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
        py::arg("desired_locale"), R"doc(
      Return the best matching pair from the desired locale and this matcher's
      supported locales.

      *desired_locale* is the desired locale, typically the user's language.

      .. important::

         *desired_locale* must outlive the returned
         :class:`LocaleMatcher.Result` object.
      )doc")
      .def(
          "get_best_match_result",
          [](const LocaleMatcher &self,
             const std::vector<Locale> &desired_locales) {
            auto first = desired_locales.data();
            Locale::RangeIterator<const Locale *> iter(
                first, first + desired_locales.size());
            ErrorCode error_code;
            auto result = self.getBestMatchResult(iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("desired_locales"), R"doc(
      Return the best matching pair from the desired locales and this matcher's
      supported locales.

      *desired_locales* is a list of desired locales, typically in descending
      order of the user's language preferences.

      .. important::

         Each of the desired locales in *desired_locales* must outlive the
         returned :class:`LocaleMatcher.Result` object.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  lm.def(
      "is_match",
      [](const LocaleMatcher &self, const icupy::LocaleVariant &desired,
         const icupy::LocaleVariant &supported) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.isMatch(icupy::to_locale(desired),
                                   icupy::to_locale(supported), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("desired"), py::arg("supported"), R"doc(
      Return ``True`` if the desired locale matches the supported locale,
      ``False`` otherwise.

      *desired* is the desired locale.

      *supported* is the supported locale.

      This method is influenced by :class:`Builder` options such as
      :meth:`.Builder.set_direction`, :meth:`.Builder.set_favor_subtag`, and
      :meth:`.Builder.set_max_distance`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  //
  // class icu::LocaleMatcher::Builder
  //
  lmb.def(py::init<>(), R"doc(
      Initialize a ``LocaleMatcher.Builder`` instance.
      )doc");

  lmb.def(
      "add_supported_locale",
      [](Builder &self, const icupy::LocaleVariant &locale) -> Builder & {
        return self.addSupportedLocale(icupy::to_locale(locale));
      },
      py::arg("locale"), R"doc(
      Add the supported locale to this builder and return the builder itself.

      *locale* is the supported locale.
      )doc");

  lmb.def(
      "build",
      [](const Builder &self) {
        ErrorCode error_code;
        auto result = self.build(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Create a new :class:`LocaleMatcher` instance with the current builder
      settings.
      )doc");

  lmb.def(
      "copy_error_to",
      [](const Builder &self, ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Copy the internal :class:`UErrorCode` to *out_error_code*; return
      ``True`` if :class:`UErrorCode` indicates a failure, ``False``
      otherwise.

      If *out_error_code* already contains an error, its value will not be
      changed.
      )doc");

  lmb.def(
      "set_default_locale",
      [](Builder &self,
         std::optional<const icupy::LocaleVariant> &default_locale)
          -> Builder & {
        Locale locale;
        if (default_locale.has_value()) {
          locale = icupy::to_locale(default_locale.value());
        }
        return self.setDefaultLocale(default_locale ? &locale : nullptr);
      },
      py::arg("default_locale"), R"doc(
      Set the default locale for this builder and return the builder itself.

      If *default_locale* is ``None``, the first supported locale is used as
      the default locale.
      )doc");

  lmb.def("set_demotion_per_desired_locale",
          &Builder::setDemotionPerDesiredLocale, py::arg("demotion"), R"doc(
      Set the demotion for each desired locale for this builder and return the
      builder itself.

      The default is REGION.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  lmb.def("set_direction", &Builder::setDirection, py::arg("direction"), R"doc(
      Set the matching direction for this builder and return the builder
      itself.

      The default is WITH_ONE_WAY.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  lmb.def("set_favor_subtag", &Builder::setFavorSubtag, py::arg("subtag"),
          R"doc(
      Set the favor subtag for this builder and return the builder itself.

      The default is LANGUAGE.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  lmb.def(
      "set_max_distance",
      [](Builder &self, const icupy::LocaleVariant &desired,
         const icupy::LocaleVariant &supported) -> Builder & {
        return self.setMaxDistance(icupy::to_locale(desired),
                                   icupy::to_locale(supported));
      },
      py::arg("desired"), py::arg("supported"), R"doc(
      Set the maximum acceptable matching distance for this builder and return
      the builder itself.

      *desired* is the desired locale for which to be matched.

      *supported* is the supported locale for which to be matched.

      The matcher will return a match for a pair of locales only if they match
      at least as well as the pair given here.
      For example, ``set_max_distance(en-US, en-GB)`` limits matches to ones
      where the (desired, support) locales have a distance no greater than a
      region subtag difference. This is much stricter than the CLDR default.

      The details of locale matching are subject to changes in CLDR data and in
      the algorithm. Specifying a maximum distance in relative terms via a
      sample pair of locales insulates from changes that affect all distance
      metrics similarly, but some changes will necessarily affect relative
      distances between different pairs of locales.
      )doc");

  lmb.def("set_no_default_locale", &Builder::setNoDefaultLocale, R"doc(
      Set the no default locale flag for this builder and return the builder
      itself.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  lmb.def(
      "set_supported_locales",
      [](Builder &self, const std::vector<Locale> &locales) -> Builder & {
        auto first = locales.data();
        Locale::RangeIterator<const Locale *> iter(first,
                                                   first + locales.size());
        return self.setSupportedLocales(iter);
      },
      py::arg("locales"), R"doc(
      Set the supported locales for this builder and return the builder itself.

      All previously set or added supported locales will be removed.
      )doc");

  lmb.def(
      "set_supported_locales_from_list_string",
      [](Builder &self, const std::string &locales) -> Builder & {
        return self.setSupportedLocalesFromListString(locales);
      },
      py::arg("locales"), R"doc(
      Parse the Accept-Language string, set the result as the supported locales
      for this builder, and return the builder itself.

      *locales* is an Accept-Language string such as "af, en, fr;q=0.9",
      but does not allow "*". For more information, see
      `RFC 2616 Section 14.4 <https://tools.ietf.org/html/rfc2616#section-14.4>`__.

      All previously set or added supported locales will be removed.

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         matcher = (
             icu.LocaleMatcher.Builder()
             .set_supported_locales_from_list_string("ja, en-US, fr-FR, de-DE")
             .set_default_locale(icu.Locale.get_english())
             .build()
         )
         matcher.get_best_match("es_AR")  # Locale('en')
         matcher.get_best_match("fr_CA")  # Locale('fr_FR')
         matcher.get_best_match("de_CH")  # Locale('de_DE')
      )doc");

  //
  // class icu::LocaleMatcher::Result
  //
  lmr.def("get_desired_index", &Result::getDesiredIndex, R"doc(
      Return the index of the best-matching desired locale, or -1 if the list
      of desired locales is empty or if no matches were found.

      .. seealso::

         :meth:`LocaleMatcher.get_best_match_result`
      )doc");

  lmr.def(
      "get_desired_locale",
      [](const Result &self) -> std::optional<const Locale *> {
        return self.getDesiredLocale();
      },
      py::return_value_policy::reference, R"doc(
      Return the best-matching desired locale, or ``None`` if the list of
      desired locales is empty or if no matches were found.
      )doc");

  lmr.def("get_supported_index", &Result::getSupportedIndex, R"doc(
      Return the index of the best-matching supported locale, or -1 if the list
      of supported locales is empty or if no matches were found.

      .. seealso::

         :meth:`LocaleMatcher.get_best_match_result`
      )doc");

  lmr.def(
      "get_supported_locale",
      [](const Result &self) -> std::optional<const Locale *> {
        return self.getSupportedLocale();
      },
      py::return_value_policy::reference, R"doc(
      Return the best-matching supported locale, or ``None`` if the list of
      supported locales is empty or if no matches were found.
      )doc");

  lmr.def(
      "make_resolved_locale",
      [](const Result &self) {
        ErrorCode error_code;
        auto result = self.makeResolvedLocale(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the best-matching supported locale with the best-matching desired
      locale's relevant fields appended, such as extensions like -t- or -u-.

      For example, if the desired locale is "ar-SA-u-nu-latn" and the supported
      locale is "ar-EG", then the resolved locale is "ar-SA-u-nu-latn".

      If the supported locale does not exist, the root locale will be used
      instead.

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         matcher = (
             icu.LocaleMatcher.Builder()
             .set_supported_locales_from_list_string("ja, en-US, fr-FR, de-DE")
             .set_default_locale(icu.Locale.get_english())
             .build()
         )
         desired_locale = icu.Locale.for_language_tag("ja-JP-u-ca-japanese-nu-hanidec")
         matcher.get_best_match(desired_locale)  # Locale('ja')
         result = matcher.get_best_match_result(desired_locale)
         result.get_desired_locale()  # Locale('ja_JP@calendar=japanese;numbers=hanidec')
         result.get_supported_locale()  # Locale('ja')
         result.make_resolved_locale()  # Locale('ja_JP@calendar=japanese;numbers=hanidec')
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)
}

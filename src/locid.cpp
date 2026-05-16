#include "main.hpp"
#include <iomanip>
#include <iterator>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/locid.h>
#include <unicode/strenum.h>

using namespace icu;

void init_locid(py::module &m, py::class_<Locale, UObject> &loc) {
  //
  // class icu::Locale
  //
  loc.def(py::init<>(), R"doc(
      Initialize a ``Locale`` instance with the default locale.

      .. seealso::

         :meth:`.get_default`
      )doc")
      .def(py::init([](std::optional<const std::string> &language,
                       std::optional<const std::string> &country,
                       std::optional<const std::string> &variant,
                       std::optional<const std::string> &keywords_and_values) {
             return std::make_unique<Locale>(
                 language ? language->data() : nullptr,
                 country ? country->data() : nullptr,
                 variant ? variant->data() : nullptr,
                 keywords_and_values ? keywords_and_values->data() : nullptr);
           }),
           py::arg("language"), py::arg("country") = std::nullopt,
           py::arg("variant") = std::nullopt,
           py::arg("keywords_and_values") = std::nullopt, R"doc(
      Initialize a ``Locale`` instance with the specified language, country,
      variant, and keyword-value pairs.

      *language* is a lowercase two- or three-letter ISO-639 code, the
      ICU-style locale (e.g., "en_US"), or ``None``. If the ICU-style locale or
      ``None`` is specified, other parameters must not be specified.
      If ``None`` is specified, the locale is initialized as the default
      locale.

      *country* is optional and is an uppercase two-letter ISO-3166 code.

      *variant* is optional and is an uppercase vendor and browser specific
      code.

      *keywords_and_values* is optional and is a string consisting of
      keyword-value pairs, such as "collation=phonebook;currency=euro".

      .. seealso::

         :class:`LocaleBuilder`
         :meth:`.get_default`
      )doc")
      .def(py::init<const Locale &>(), py::arg("other"), R"doc(
      Initialize a ``Locale`` instance from another ``Locale``.
      )doc");

  loc.def("__copy__", &Locale::clone, R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  loc.def(
      "__deepcopy__",
      [](const Locale &self, py::dict & /* memo */) { return self.clone(); },
      py::arg("memo"), R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  loc.def(
      "__eq__",
      [](const Locale &self, const Locale &other) { return self == other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  loc.def("__hash__", &Locale::hashCode, R"doc(
      Return a hash value of this instance.

      This is equivalent to calling :meth:`.hash_code`.
      )doc");

  loc.def(
      "__ne__",
      [](const Locale &self, const Locale &other) { return self != other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  loc.def("__repr__", [](const Locale &self) {
    std::stringstream ss;
    ss << "Locale(" << std::quoted(self.getName(), '\'') << ")";
    return ss.str();
  });

  loc.def("__str__", &Locale::getName, R"doc(
      Return a string representation of this instance.

      This is equivalent to calling :meth:`.get_name`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "add_likely_subtags",
      [](Locale &self) {
        ErrorCode error_code;
        self.addLikelySubtags(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      R"doc(
      Add likely subtags to this instance.

      If this ``Locale`` is already in the maximum form, is invalid, or
      there is no data available to maximize it, the ``Locale`` will not be
      changed.

      For details, see
      https://www.unicode.org/reports/tr35/#likely-subtags.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  loc.def(
      "canonicalize",
      [](Locale &self) {
        ErrorCode error_code;
        self.canonicalize(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      R"doc(
      Canonicalize this instance according to CLDR.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  loc.def("clone", &Locale::clone, R"doc(
      Return a copy of this instance.

      .. seealso::

         :meth:`.__copy__`
         :meth:`.__deepcopy__`
      )doc");

  loc.def_static(
      "create_canonical",
      [](const std::string &name) {
        return Locale::createCanonical(name.data());
      },
      py::arg("name"), R"doc(
      Create a new ``Locale`` instance using a string canonicalized in
      accordance with CLDR.
      )doc");

  loc.def_static(
      "create_from_name",
      [](std::optional<const std::string> &name) {
        return Locale::createFromName(name ? name->data() : nullptr);
      },
      py::arg("name"), R"doc(
      Create a new ``Locale`` instance using a minimally canonicalized string.

      If *name* is ``None``, the default locale will be used.
      )doc");

  loc.def(
      "create_keywords",
      [](const Locale &self) -> std::optional<StringEnumeration *> {
        ErrorCode error_code;
        auto result = self.createKeywords(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return an enumeration of keywords in this instance, or ``None`` if there
      are no keywords.

      .. seealso::

         :meth:`.get_keywords`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "create_unicode_keywords",
      [](const Locale &self) -> std::optional<StringEnumeration *> {
        ErrorCode error_code;
        auto result = self.createUnicodeKeywords(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return an enumeration of Unicode keywords in this instance, or ``None``
      if there are no keywords.

      .. seealso::

         :meth:`.get_unicode_keywords`
      )doc");

  loc.def_static(
      "for_language_tag",
      [](const std::string &tag) {
        ErrorCode error_code;
        auto result = Locale::forLanguageTag(tag, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("tag"), R"doc(
      Create a new ``Locale`` instance from the BCP 47 language tag.

      .. seealso::

         :meth:`.to_language_tag`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

  loc.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = Locale::getAvailableLocales(count);
        std::vector<const Locale *> result(count, nullptr);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a list of available locales.
      )doc");

  loc.def("get_base_name", &Locale::getBaseName, R"doc(
      Return the programmatic base name of this instance.

      The base name is the result of :meth:`.get_name` without the keywords.

      .. seealso::

         :meth:`.get_name`
      )doc");

  loc.def_static("get_canada", &Locale::getCanada, R"doc(
      Return a ``Locale`` instance for English (Canada).
      )doc");

  loc.def_static("get_canada_french", &Locale::getCanadaFrench, R"doc(
      Return a ``Locale`` instance for French (Canada).
      )doc");

  loc.def_static("get_china", &Locale::getChina, R"doc(
      Return a ``Locale`` instance for Chinese (China).
      )doc");

  loc.def_static("get_chinese", &Locale::getChinese, R"doc(
      Return a ``Locale`` instance for Chinese.
      )doc");

  loc.def("get_country", &Locale::getCountry, R"doc(
      Return the ISO-3166 country code of this instance.
      )doc");

  loc.def_static("get_default", &Locale::getDefault, R"doc(
      Return the current default ``Locale`` instance.

      .. seealso::

         :meth:`.set_default`
      )doc");

  loc.def(
         "get_display_country",
         [](const Locale &self, const icupy::LocaleVariant &display_locale,
            UnicodeString &disp_country) -> UnicodeString & {
           return self.getDisplayCountry(icupy::to_locale(display_locale),
                                         disp_country);
         },
         py::arg("display_locale"), py::arg("disp_country"), R"doc(
      Copy the country name for this locale to *disp_country* in a format
      suitable for display in the locale specified by *display_locale*, and
      return *disp_country* itself.
      )doc")
      .def("get_display_country",
           py::overload_cast<UnicodeString &>(&Locale::getDisplayCountry,
                                              py::const_),
           py::arg("disp_country"), R"doc(
      Copy the country name for this locale to *disp_country* in a format
      suitable for display in the default locale, and return *disp_country*
      itself.
      )doc");

  loc.def(
         "get_display_language",
         [](const Locale &self, const icupy::LocaleVariant &display_locale,
            UnicodeString &disp_lang) -> UnicodeString & {
           return self.getDisplayLanguage(icupy::to_locale(display_locale),
                                          disp_lang);
         },
         py::arg("display_locale"), py::arg("disp_lang"), R"doc(
      Copy the language name for this locale to *disp_lang* in a format
      suitable for display in the locale specified by *display_locale*, and
      return *disp_lang* itself.
      )doc")
      .def("get_display_language",
           py::overload_cast<UnicodeString &>(&Locale::getDisplayLanguage,
                                              py::const_),
           py::arg("disp_lang"), R"doc(
      Copy the language name for this locale to *disp_lang* in a format
      suitable for display in the default locale, and return *disp_lang*
      itself.
      )doc");

  loc.def(
         "get_display_name",
         [](const Locale &self, const icupy::LocaleVariant &display_locale,
            UnicodeString &name) -> UnicodeString & {
           return self.getDisplayName(icupy::to_locale(display_locale), name);
         },
         py::arg("display_locale"), py::arg("name"), R"doc(
      Copy the name for this locale to *name* in a format suitable for display
      in the locale specified by *display_locale*, and return *name* itself.
      )doc")
      .def("get_display_name",
           py::overload_cast<UnicodeString &>(&Locale::getDisplayName,
                                              py::const_),
           py::arg("name"), R"doc(
      Copy the name for this locale to *name* in a format suitable for display
      in the default locale, and return *name* itself.
      )doc");

  loc.def(
         "get_display_script",
         [](const Locale &self, const icupy::LocaleVariant &display_locale,
            UnicodeString &disp_script) -> UnicodeString & {
           return self.getDisplayScript(icupy::to_locale(display_locale),
                                        disp_script);
         },
         py::arg("display_locale"), py::arg("disp_script"), R"doc(
      Copy the script name for this locale to *disp_script* in a format
      suitable for display in the locale specified by *display_locale*, and
      return *disp_script* itself.
      )doc")
      .def("get_display_script",
           py::overload_cast<UnicodeString &>(&Locale::getDisplayScript,
                                              py::const_),
           py::arg("disp_script"), R"doc(
      Copy the script name for this locale to *disp_script* in a format
      suitable for display in the default locale, and return *disp_script*
      itself.
      )doc");

  loc.def(
         "get_display_variant",
         [](const Locale &self, const icupy::LocaleVariant &display_locale,
            UnicodeString &disp_var) -> UnicodeString & {
           return self.getDisplayVariant(icupy::to_locale(display_locale),
                                         disp_var);
         },
         py::arg("display_locale"), py::arg("disp_var"), R"doc(
      Copy the variant name for this locale to *disp_var* in a format
      suitable for display in the locale specified by *display_locale*, and
      return *disp_var* itself.
      )doc")
      .def("get_display_variant",
           py::overload_cast<UnicodeString &>(&Locale::getDisplayVariant,
                                              py::const_),
           py::arg("disp_var"), R"doc(
      Copy the variant name for this locale to *disp_var* in a format
      suitable for display in the default locale, and return *disp_var* itself.
      )doc");

  loc.def_static("get_english", &Locale::getEnglish, R"doc(
      Return a ``Locale`` instance for English.
      )doc");

  loc.def_static("get_france", &Locale::getFrance, R"doc(
      Return a ``Locale`` instance for French (France).
      )doc");

  loc.def_static("get_french", &Locale::getFrench, R"doc(
      Return a ``Locale`` instance for French.
      )doc");

  loc.def_static("get_german", &Locale::getGerman, R"doc(
      Return a ``Locale`` instance for German.
      )doc");

  loc.def_static("get_germany", &Locale::getGermany, R"doc(
      Return a ``Locale`` instance for German (Germany).
      )doc");

  loc.def("get_iso3_country", &Locale::getISO3Country, R"doc(
      Return the three-letter ISO-3166 country code of this instance.
      )doc");

  loc.def("get_iso3_language", &Locale::getISO3Language, R"doc(
      Return the three-letter ISO-639-2 language code of this instance.
      )doc");

  loc.def_static(
      "get_iso_countries",
      []() {
        auto p = Locale::getISOCountries();
        std::vector<const char *> result;
        while (*p) {
          result.push_back(*p++);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a list of two-letter ISO-3166 country codes.
      )doc");

  loc.def_static(
      "get_iso_languages",
      []() {
        auto p = Locale::getISOLanguages();
        std::vector<const char *> result;
        while (*p) {
          result.push_back(*p++);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a list of ISO-639 language codes.
      )doc");

  loc.def_static("get_italian", &Locale::getItalian, R"doc(
      Return a ``Locale`` instance for Italian.
      )doc");

  loc.def_static("get_italy", &Locale::getItaly, R"doc(
      Return a ``Locale`` instance for Italian (Italy).
      )doc");

  loc.def_static("get_japan", &Locale::getJapan, R"doc(
      Return a ``Locale`` instance for Japanese (Japan).
      )doc");

  loc.def_static("get_japanese", &Locale::getJapanese, R"doc(
      Return a ``Locale`` instance for Japanese.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "get_keywords",
      [](const Locale &self) {
        // https://unicode-org.atlassian.net/browse/ICU-20573
        // Locale::getKeywords() and Locale::getUnicodeKeywords() segfault on
        // empty
        std::set<std::string> result;
        ErrorCode error_code;
        self.getKeywords<std::string>(
            std::insert_iterator<decltype(result)>(result, result.begin()),
            error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return a set of keywords in this instance.

      .. seealso::

         :meth:`.create_keywords`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM < 63)
  loc.def(
      "get_keyword_value",
      [](const Locale &self, const char *keyword_name) {
        if (self.isBogus()) {
          throw icupy::ICUError(U_ILLEGAL_ARGUMENT_ERROR);
        }
        ErrorCode error_code;
        const auto length =
            self.getKeywordValue(keyword_name, nullptr, 0, error_code);
        std::string result(length, '\0');
        error_code.reset();
        self.getKeywordValue(keyword_name, result.data(),
                             static_cast<int32_t>(result.size()), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword_name"));
#else
  loc.def(
      "get_keyword_value",
      [](const Locale &self, const std::string &keyword_name) {
        ErrorCode error_code;
        auto result =
            self.getKeywordValue<std::string>(keyword_name, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword_name"), R"doc(
      Return the value of the specified keyword in this instance.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM < 63)

  loc.def_static("get_korea", &Locale::getKorea, R"doc(
      Return a ``Locale`` instance for Korean (South Korea).
      )doc");

  loc.def_static("get_korean", &Locale::getKorean, R"doc(
      Return a ``Locale`` instance for Korean.
      )doc");

  loc.def("get_language", &Locale::getLanguage, R"doc(
      Return the ISO-639 language code of this instance.
      )doc");

  loc.def("get_lcid", &Locale::getLCID, R"doc(
      Return the Windows LCID value of this instance, or 0 if this instance
      does not have a Windows LCID.
      )doc");

  loc.def("get_name", &Locale::getName, R"doc(
      Return the programmatic name of the entire locale of this instance.

      .. seealso::

         :meth:`.__str__`
      )doc");

  loc.def_static("get_prc", &Locale::getPRC, R"doc(
      Return a ``Locale`` instance for Chinese (China).
      )doc");

  loc.def_static("get_root", &Locale::getRoot, R"doc(
      Return the root locale instance.
      )doc");

  loc.def("get_script", &Locale::getScript, R"doc(
      Return the ISO-15924 abbreviation script code of this instance.
      )doc");

  loc.def_static("get_simplified_chinese", &Locale::getSimplifiedChinese, R"doc(
      Return a ``Locale`` instance for Chinese (China).
      )doc");

  loc.def_static("get_taiwan", &Locale::getTaiwan, R"doc(
      Return a ``Locale`` instance for Chinese (Taiwan).
      )doc");

  loc.def_static("get_traditional_chinese", &Locale::getTraditionalChinese,
                 R"doc(
      Return a ``Locale`` instance for Chinese (Taiwan).
      )doc");

  loc.def_static("get_uk", &Locale::getUK, R"doc(
      Return a ``Locale`` instance for English (United Kingdom).
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "get_unicode_keywords",
      [](const Locale &self) {
        // https://unicode-org.atlassian.net/browse/ICU-20573
        // Locale::getKeywords() and Locale::getUnicodeKeywords() segfault on
        // empty
        std::set<std::string> result;
        ErrorCode error_code;
        self.getUnicodeKeywords<std::string>(
            std::insert_iterator<decltype(result)>(result, result.begin()),
            error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return a set of Unicode keywords in this instance.

      .. seealso::

         :meth:`.create_unicode_keywords`
      )doc");

  loc.def(
      "get_unicode_keyword_value",
      [](const Locale &self, const std::string &keyword_name) {
        ErrorCode error_code;
        auto result =
            self.getUnicodeKeywordValue<std::string>(keyword_name, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword_name"), R"doc(
      Return the Unicode value of the specified Unicode keyword in this
      instance.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

  loc.def_static("get_us", &Locale::getUS, R"doc(
      Return a ``Locale`` instance for English (United States).
      )doc");

  loc.def("get_variant", &Locale::getVariant, R"doc(
      Return the variant code of this instance.
      )doc");

  loc.def("hash_code", &Locale::hashCode, R"doc(
      Return a hash code for this instance.

      .. seealso::

         :meth:`.__hash__`.
      )doc");

  loc.def(
      "is_bogus",
      [](const Locale &self) -> py::bool_ { return self.isBogus(); }, R"doc(
      Return ``True`` if this instance is invalid; ``False`` otherwise.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  loc.def(
      "is_right_to_left",
      [](const Locale &self) -> py::bool_ { return self.isRightToLeft(); },
      R"doc(
      Return ``True`` if this instance's script is right-to-left.

      If the script is not specified in this instance, the likely script for
      this instance is used. If the likely script cannot be determined,
      ``False`` is returned.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "minimize_subtags",
      [](Locale &self) {
        ErrorCode error_code;
        self.minimizeSubtags(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      R"doc(
      Minimize the subtags of this instance using the algorithm described in
      the following CLDR technical report:
      https://www.unicode.org/reports/tr35/#likely-subtags.

      If this ``Locale`` is already in the minimal form, is invalid, or there
      is no data available to minimize it, the ``Locale`` will not be changed.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

  loc.def_static(
      "set_default",
      [](std::optional<const Locale> new_locale) {
        ErrorCode error_code;
        Locale::setDefault(new_locale.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("new_locale"), R"doc(
      Set the default locale to *new_locale*.

      If *new_locale* is ``None``, set the default locale using the value
      obtained from the execution environment.

      .. note::

         Normally, this is set once at the start of the process and is not
         reset thereafter. :meth:`.set_default` changes only the ICU default
         locale ID; it does not change the execution environment's default
         locale ID.

      .. seealso::

         :meth:`.get_default`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  loc.def(
      "set_keyword_value",
      [](Locale &self, const std::string &keyword_name,
         std::optional<const std::string> &keyword_value) {
        ErrorCode error_code;
        self.setKeywordValue(keyword_name, keyword_value.value_or(""),
                             error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("keyword_name"), py::arg("keyword_value"), R"doc(
      Set or remove the value of the specified keyword in this instance.

      If *keyword_value* is empty or ``None``, the specified keyword is removed
      from this instance.

      .. note::

         To remove all keywords, create a new ``Locale`` using the result of
         :meth:`.get_base_name`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  loc.def("set_to_bogus", &Locale::setToBogus, R"doc(
      Set this instance to be invalid.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "set_unicode_keyword_value",
      [](Locale &self, const std::string &keyword_name,
         std::optional<const std::string> &keyword_value) {
        ErrorCode error_code;
        self.setUnicodeKeywordValue(keyword_name, keyword_value.value_or(""),
                                    error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("keyword_name"), py::arg("keyword_value"), R"doc(
      Set or remove the Unicode value of the specified Unicode keyword in this
      instance.

      If *keyword_value* is empty or ``None``, the specified Unicode keyword is
      removed from this instance.

      .. note::

         To remove all Unicode keywords, create a new ``Locale`` using the
         result of :meth:`.get_base_name`.
      )doc");

  loc.def(
      "to_language_tag",
      [](const Locale &self) {
        ErrorCode error_code;
        auto result = self.toLanguageTag<std::string>(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return a BCP 47 language tag for this instance.

      .. seealso::

         :meth:`.for_language_tag`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

  loc.def_property_readonly_static(
      "CANADA", [](const py::object &) { return ULOC_CANADA; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "CANADA_FRENCH", [](const py::object &) { return ULOC_CANADA_FRENCH; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "CHINA", [](const py::object &) { return ULOC_CHINA; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "CHINESE", [](const py::object &) { return ULOC_CHINESE; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "ENGLISH", [](const py::object &) { return ULOC_ENGLISH; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "FRANCE", [](const py::object &) { return ULOC_FRANCE; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "FRENCH", [](const py::object &) { return ULOC_FRENCH; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "GERMAN", [](const py::object &) { return ULOC_GERMAN; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "GERMANY", [](const py::object &) { return ULOC_GERMANY; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "ITALIAN", [](const py::object &) { return ULOC_ITALIAN; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "ITALY", [](const py::object &) { return ULOC_ITALY; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "JAPAN", [](const py::object &) { return ULOC_JAPAN; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "JAPANESE", [](const py::object &) { return ULOC_JAPANESE; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "KOREA", [](const py::object &) { return ULOC_KOREA; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "KOREAN", [](const py::object &) { return ULOC_KOREAN; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "PRC", [](const py::object &) { return ULOC_PRC; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "SIMPLIFIED_CHINESE",
      [](const py::object &) { return ULOC_SIMPLIFIED_CHINESE; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "TAIWAN", [](const py::object &) { return ULOC_TAIWAN; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "TRADITIONAL_CHINESE",
      [](const py::object &) { return ULOC_TRADITIONAL_CHINESE; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "UK", [](const py::object &) { return ULOC_UK; },
      py::return_value_policy::reference);

  loc.def_property_readonly_static(
      "US", [](const py::object &) { return ULOC_US; },
      py::return_value_policy::reference);
}

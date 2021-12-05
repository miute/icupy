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
  // icu::Locale
  loc.def(py::init<>())
      .def(py::init<const char *, const char *, const char *, const char *>(), py::arg("language"),
           py::arg("country") = nullptr, py::arg("variant") = nullptr, py::arg("keywords_and_values") = nullptr)
      .def(py::init<const Locale &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  loc.def("__copy__", [](const Locale &self) { return self.clone(); })
      .def(
          "__deepcopy__", [](const Locale &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def("__repr__",
           [](const Locale &self) {
             std::stringstream ss;
             ss << "Locale(" << std::quoted(self.getName(), '\'') << ")";
             return ss.str();
           })
      .def("__str__", &Locale::getName);
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def("add_likely_subtags", [](Locale &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    self.addLikelySubtags(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  loc.def("canonicalize", [](Locale &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    self.canonicalize(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
  loc.def("clone", &Locale::clone);
  loc.def_static("create_canonical", &Locale::createCanonical, py::arg("name"));
  loc.def_static("create_from_name", &Locale::createFromName, py::arg("name"));
  loc.def("create_keywords", [](const Locale &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.createKeywords(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def("create_unicode_keywords", [](const Locale &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.createUnicodeKeywords(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  loc.def_static(
      "for_language_tag",
      [](const char *tag) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = Locale::forLanguageTag(tag, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code, tag);
        }
        return result;
      },
      py::arg("tag"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def_static(
      "get_available_locales",
      []() {
        int32_t count = 0;
        auto p = Locale::getAvailableLocales(count);
        std::vector<const Locale *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  loc.def("get_base_name", &Locale::getBaseName);
  loc.def_static("get_canada", &Locale::getCanada);
  loc.def_static("get_canada_french", &Locale::getCanadaFrench);
  loc.def_static("get_china", &Locale::getChina);
  loc.def_static("get_chinese", &Locale::getChinese);
  loc.def("get_country", &Locale::getCountry);
  loc.def_static("get_default", &Locale::getDefault);
  loc.def("get_display_country",
          py::overload_cast<const Locale &, UnicodeString &>(&Locale::getDisplayCountry, py::const_),
          py::arg("display_locale"), py::arg("disp_country"))
      .def(
          // const char *display_locale -> const Locale &display_locale
          "get_display_country",
          [](const Locale &self, const char *display_locale, UnicodeString &disp_country) -> UnicodeString & {
            return self.getDisplayCountry(display_locale, disp_country);
          },
          py::arg("display_locale"), py::arg("disp_country"))
      .def("get_display_country", py::overload_cast<UnicodeString &>(&Locale::getDisplayCountry, py::const_),
           py::arg("disp_country"));
  loc.def("get_display_language",
          py::overload_cast<const Locale &, UnicodeString &>(&Locale::getDisplayLanguage, py::const_),
          py::arg("display_locale"), py::arg("disp_lang"))
      .def(
          // const char *display_locale -> const Locale &display_locale
          "get_display_language",
          [](const Locale &self, const char *display_locale, UnicodeString &disp_lang) -> UnicodeString & {
            return self.getDisplayLanguage(display_locale, disp_lang);
          },
          py::arg("display_locale"), py::arg("disp_lang"))
      .def("get_display_language", py::overload_cast<UnicodeString &>(&Locale::getDisplayLanguage, py::const_),
           py::arg("disp_lang"));
  loc.def("get_display_name", py::overload_cast<const Locale &, UnicodeString &>(&Locale::getDisplayName, py::const_),
          py::arg("display_locale"), py::arg("name"))
      .def(
          // const char *display_locale -> const Locale &display_locale
          "get_display_name",
          [](const Locale &self, const char *display_locale, UnicodeString &name) -> UnicodeString & {
            return self.getDisplayName(display_locale, name);
          },
          py::arg("display_locale"), py::arg("name"))
      .def("get_display_name", py::overload_cast<UnicodeString &>(&Locale::getDisplayName, py::const_),
           py::arg("name"));
  loc.def("get_display_script",
          py::overload_cast<const Locale &, UnicodeString &>(&Locale::getDisplayScript, py::const_),
          py::arg("display_locale"), py::arg("disp_script"))
      .def(
          // const char *display_locale -> const Locale &display_locale
          "get_display_script",
          [](const Locale &self, const char *display_locale, UnicodeString &disp_script) -> UnicodeString & {
            return self.getDisplayScript(display_locale, disp_script);
          },
          py::arg("display_locale"), py::arg("disp_script"))
      .def("get_display_script", py::overload_cast<UnicodeString &>(&Locale::getDisplayScript, py::const_),
           py::arg("disp_script"));
  loc.def("get_display_variant",
          py::overload_cast<const Locale &, UnicodeString &>(&Locale::getDisplayVariant, py::const_),
          py::arg("display_locale"), py::arg("disp_var"))
      .def(
          // const char *display_locale -> const Locale &display_locale
          "get_display_variant",
          [](const Locale &self, const char *display_locale, UnicodeString &disp_var) -> UnicodeString & {
            return self.getDisplayVariant(display_locale, disp_var);
          },
          py::arg("display_locale"), py::arg("disp_var"))
      .def("get_display_variant", py::overload_cast<UnicodeString &>(&Locale::getDisplayVariant, py::const_),
           py::arg("disp_var"));
  loc.def_static("get_english", &Locale::getEnglish);
  loc.def_static("get_france", &Locale::getFrance);
  loc.def_static("get_french", &Locale::getFrench);
  loc.def_static("get_german", &Locale::getGerman);
  loc.def_static("get_germany", &Locale::getGermany);
  loc.def("get_iso3_country", &Locale::getISO3Country);
  loc.def("get_iso3_language", &Locale::getISO3Language);
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
      py::return_value_policy::reference);
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
      py::return_value_policy::reference);
  loc.def_static("get_italian", &Locale::getItalian);
  loc.def_static("get_italy", &Locale::getItaly);
  loc.def_static("get_japan", &Locale::getJapan);
  loc.def_static("get_japanese", &Locale::getJapanese);
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def("get_keywords", [](const Locale &self) {
    // https://unicode-org.atlassian.net/browse/ICU-20573
    // Locale::getKeywords() and Locale::getUnicodeKeywords() segfault on empty
    UErrorCode error_code = U_ZERO_ERROR;
    std::set<std::string> result;
    self.getKeywords<std::string>(std::insert_iterator<decltype(result)>(result, result.begin()), error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
#if (U_ICU_VERSION_MAJOR_NUM < 63)
  loc.def(
      "get_keyword_value",
      [](const Locale &self, const char *keyword_name) {
        if (self.isBogus()) {
          throw ICUError(U_ILLEGAL_ARGUMENT_ERROR);
        }
        UErrorCode error_code = U_ZERO_ERROR;
        const auto length = self.getKeywordValue(keyword_name, nullptr, 0, error_code);
        std::string result(length, '\0');
        error_code = U_ZERO_ERROR;
        self.getKeywordValue(keyword_name, result.data(), static_cast<int32_t>(result.size()), error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword_name"));
#else
  loc.def(
      "get_keyword_value",
      [](const Locale &self, const char *keyword_name) {
        UErrorCode error_code = U_ZERO_ERROR;
        std::string result = self.getKeywordValue<std::string>(keyword_name, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword_name"));
#endif // (U_ICU_VERSION_MAJOR_NUM < 63)
  loc.def_static("get_korea", &Locale::getKorea);
  loc.def_static("get_korean", &Locale::getKorean);
  loc.def("get_language", &Locale::getLanguage);
  loc.def("get_lcid", &Locale::getLCID);
  loc.def("get_name", &Locale::getName);
  loc.def_static("get_prc", &Locale::getPRC);
  loc.def_static("get_root", &Locale::getRoot);
  loc.def("get_script", &Locale::getScript);
  loc.def_static("get_simplified_chinese", &Locale::getSimplifiedChinese);
  loc.def_static("get_taiwan", &Locale::getTaiwan);
  loc.def_static("get_traditional_chinese", &Locale::getTraditionalChinese);
  loc.def_static("get_uk", &Locale::getUK);
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def("get_unicode_keywords", [](const Locale &self) {
    // https://unicode-org.atlassian.net/browse/ICU-20573
    // Locale::getKeywords() and Locale::getUnicodeKeywords() segfault on empty
    UErrorCode error_code = U_ZERO_ERROR;
    std::set<std::string> result;
    self.getUnicodeKeywords<std::string>(std::insert_iterator<decltype(result)>(result, result.begin()), error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  loc.def(
      "get_unicode_keyword_value",
      [](const Locale &self, const char *keyword_name) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getUnicodeKeywordValue<std::string>(keyword_name, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code, keyword_name);
        }
        return result;
      },
      py::arg("keyword_name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def_static("get_us", &Locale::getUS);
  loc.def("get_variant", &Locale::getVariant);
  loc.def("hash_code", &Locale::hashCode);
  loc.def("is_bogus", &Locale::isBogus);
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  loc.def("is_right_to_left", &Locale::isRightToLeft);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def("minimize_subtags", [](Locale &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    self.minimizeSubtags(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def_static(
      "set_default",
      [](std::optional<const Locale> new_locale) {
        UErrorCode error_code = U_ZERO_ERROR;
        Locale::setDefault(new_locale.value_or(nullptr), error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("new_locale"));
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  loc.def(
      "set_keyword_value",
      [](Locale &self, const char *keyword_name, const char *keyword_value) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setKeywordValue(keyword_name, keyword_value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("keyword_name"), py::arg("keyword_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
  loc.def("set_to_bogus", &Locale::setToBogus);
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  loc.def(
      "set_unicode_keyword_value",
      [](Locale &self, const char *keyword_name, const char *keyword_value) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setUnicodeKeywordValue(keyword_name, keyword_value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("keyword_name"), py::arg("keyword_value"));
  loc.def("to_language_tag", [](const Locale &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toLanguageTag<std::string>(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

  loc.def_property_readonly_static(
      "CANADA", [](py::object) { return ULOC_CANADA; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "CANADA_FRENCH", [](py::object) { return ULOC_CANADA_FRENCH; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "CHINA", [](py::object) { return ULOC_CHINA; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "CHINESE", [](py::object) { return ULOC_CHINESE; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "ENGLISH", [](py::object) { return ULOC_ENGLISH; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "FRANCE", [](py::object) { return ULOC_FRANCE; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "FRENCH", [](py::object) { return ULOC_FRENCH; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "GERMAN", [](py::object) { return ULOC_GERMAN; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "GERMANY", [](py::object) { return ULOC_GERMANY; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "ITALIAN", [](py::object) { return ULOC_ITALIAN; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "ITALY", [](py::object) { return ULOC_ITALY; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "JAPAN", [](py::object) { return ULOC_JAPAN; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "JAPANESE", [](py::object) { return ULOC_JAPANESE; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "KOREA", [](py::object) { return ULOC_KOREA; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "KOREAN", [](py::object) { return ULOC_KOREAN; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "PRC", [](py::object) { return ULOC_PRC; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "SIMPLIFIED_CHINESE", [](py::object) { return ULOC_SIMPLIFIED_CHINESE; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "TAIWAN", [](py::object) { return ULOC_TAIWAN; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "TRADITIONAL_CHINESE", [](py::object) { return ULOC_TRADITIONAL_CHINESE; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "UK", [](py::object) { return ULOC_UK; }, py::return_value_policy::reference);
  loc.def_property_readonly_static(
      "US", [](py::object) { return ULOC_US; }, py::return_value_policy::reference);
}

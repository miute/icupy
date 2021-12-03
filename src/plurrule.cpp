#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <unicode/numberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
#include <unicode/numberrangeformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#include <unicode/plurrule.h>
#include <unicode/strenum.h>

using namespace icu;

void init_plurrule(py::module &, py::class_<PluralRules, UObject> &pr) {
  // icu::PluralRules
  /*
  pr.def(py::init([]() {
      UErrorCode error_code = U_ZERO_ERROR;
      auto result = std::make_unique<PluralRules>(error_code);
      if (U_FAILURE(error_code)) {
        throw ICUError(error_code);
      }
      return result;
    }));
  */
  pr.def(py::init<const PluralRules &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  pr.def("__copy__", py::overload_cast<>(&PluralRules::clone, py::const_))
      .def(
          "__deepcopy__", [](const PluralRules &self, py::dict) { return self.clone(); }, py::arg("memo"));
  pr.def("clone", py::overload_cast<>(&PluralRules::clone, py::const_));
  pr.def_static("create_default_rules", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = PluralRules::createDefaultRules(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  pr.def_static(
        "create_rules",
        [](const UnicodeString &description) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = PluralRules::createRules(description, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("description"))
      .def_static(
          // const char16_t *description -> const UnicodeString &description
          "create_rules",
          [](const char16_t *description) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = PluralRules::createRules(description, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("description"));
  pr.def_static(
        "for_locale",
        [](const Locale &locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = PluralRules::forLocale(locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "for_locale",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = PluralRules::forLocale(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .def_static(
          "for_locale",
          [](const Locale &locale, UPluralType type) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = PluralRules::forLocale(locale, type, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("type_"))
      .def_static(
          // const char *locale -> const Locale &locale
          "for_locale",
          [](const char *locale, UPluralType type) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = PluralRules::forLocale(locale, type, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("type_"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      ;
  pr.def("get_keyword_other", &PluralRules::getKeywordOther);
  pr.def("get_keywords", [](const PluralRules &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getKeywords(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  pr.def(
        "get_samples",
        [](PluralRules &self, const UnicodeString &keyword) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto dest_capacity = self.getSamples(keyword, (double *)nullptr, 0, error_code);
          std::vector<double> result(dest_capacity);
          error_code = U_ZERO_ERROR;
          self.getSamples(keyword, result.data(), dest_capacity, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("keyword"))
      .def(
          // const char16_t *keyword -> const UnicodeString &keyword
          "get_samples",
          [](PluralRules &self, const char16_t *keyword) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto dest_capacity = self.getSamples(keyword, (double *)nullptr, 0, error_code);
            std::vector<double> result(dest_capacity);
            error_code = U_ZERO_ERROR;
            self.getSamples(keyword, result.data(), dest_capacity, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("keyword"));
  pr.def("is_keyword", &PluralRules::isKeyword, py::arg("keyword"))
      .def(
          // const char16_t *keyword -> const UnicodeString &keyword
          "is_keyword", [](const PluralRules &self, const char16_t *keyword) { return self.isKeyword(keyword); },
          py::arg("keyword"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  pr.def(
      "select",
      [](const PluralRules &self, const number::FormattedNumber &number) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.select(number, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("number"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  pr.def(
      "select",
      [](const PluralRules &self, const number::FormattedNumberRange &number) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.select(number, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("number"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
  pr.def("select", py::overload_cast<double>(&PluralRules::select, py::const_), py::arg("number").noconvert())
      .def("select", py::overload_cast<int32_t>(&PluralRules::select, py::const_), py::arg("number"));
}

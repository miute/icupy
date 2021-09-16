#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/currpinf.h>
#include <unicode/plurrule.h>

using namespace icu;

void init_currpinf(py::module &m) {
  // icu::CurrencyPluralInfo
  py::class_<CurrencyPluralInfo, UObject> cpi(m, "CurrencyPluralInfo");
  cpi.def(py::init([]() {
       UErrorCode error_code = U_ZERO_ERROR;
       auto result = std::make_unique<CurrencyPluralInfo>(error_code);
       if (U_FAILURE(error_code)) {
         throw ICUError(error_code);
       }
       return result;
     }))
      .def(py::init([](const Locale &locale) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<CurrencyPluralInfo>(locale, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<CurrencyPluralInfo>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(py::init<const CurrencyPluralInfo &>(), py::arg("info"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  cpi.def("__copy__", &CurrencyPluralInfo::clone)
      .def(
          "__deepcopy__", [](const CurrencyPluralInfo &self, py::dict) { return self.clone(); }, py::arg("memo"));
  cpi.def("clone", &CurrencyPluralInfo::clone);
  cpi.def("get_currency_plural_pattern", &CurrencyPluralInfo::getCurrencyPluralPattern, py::arg("plural_count"),
          py::arg("result"))
      .def(
          // const char16_t *plural_count -> const UnicodeString &plural_count
          "get_currency_plural_pattern",
          [](const CurrencyPluralInfo &self, const char16_t *plural_count, UnicodeString &result) -> UnicodeString & {
            return self.getCurrencyPluralPattern(plural_count, result);
          },
          py::arg("plural_count"), py::arg("result"));
  cpi.def("get_locale", &CurrencyPluralInfo::getLocale);
  cpi.def("get_plural_rules", &CurrencyPluralInfo::getPluralRules, py::return_value_policy::reference);
  cpi.def(
         "set_currency_plural_pattern",
         [](CurrencyPluralInfo &self, const UnicodeString &plural_count, const UnicodeString &pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setCurrencyPluralPattern(plural_count, pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("plural_count"), py::arg("pattern"))
      .def(
          // const char16_t *plural_count -> const UnicodeString &plural_count
          "set_currency_plural_pattern",
          [](CurrencyPluralInfo &self, const char16_t *plural_count, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setCurrencyPluralPattern(plural_count, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("plural_count"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "set_currency_plural_pattern",
          [](CurrencyPluralInfo &self, const UnicodeString &plural_count, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setCurrencyPluralPattern(plural_count, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("plural_count"), py::arg("pattern"))
      .def(
          // const char16_t *plural_count -> const UnicodeString &plural_count
          // const char16_t *pattern -> const UnicodeString &pattern
          "set_currency_plural_pattern",
          [](CurrencyPluralInfo &self, const char16_t *plural_count, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setCurrencyPluralPattern(plural_count, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("plural_count"), py::arg("pattern"));
  cpi.def(
         "set_locale",
         [](CurrencyPluralInfo &self, const Locale &loc) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setLocale(loc, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("loc"))
      .def(
          // const char *loc -> const Locale &loc
          "set_locale",
          [](CurrencyPluralInfo &self, const char *loc) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setLocale(loc, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("loc"));
  cpi.def(
         "set_plural_rules",
         [](CurrencyPluralInfo &self, const UnicodeString &rule_description) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setPluralRules(rule_description, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("rule_description"))
      .def(
          // const char16_t *rule_description -> const UnicodeString &rule_description
          "set_plural_rules",
          [](CurrencyPluralInfo &self, const char16_t *rule_description) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setPluralRules(rule_description, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("rule_description"));
}

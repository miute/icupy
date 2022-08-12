#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/currpinf.h>
#include <unicode/plurrule.h>

using namespace icu;

void init_currpinf(py::module &m) {
  //
  // icu::CurrencyPluralInfo
  //
  py::class_<CurrencyPluralInfo, UObject> cpi(m, "CurrencyPluralInfo");

  cpi.def(py::init([]() {
       ErrorCode error_code;
       auto result = std::make_unique<CurrencyPluralInfo>(error_code);
       if (error_code.isFailure()) {
         throw icupy::ICUError(error_code);
       }
       return result;
     }))
      .def(py::init([](const icupy::LocaleVariant &locale) {
             ErrorCode error_code;
             auto result = std::make_unique<CurrencyPluralInfo>(icupy::to_locale(locale), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("locale"))
      .def(py::init<const CurrencyPluralInfo &>(), py::arg("other"));

  cpi.def("__copy__", &CurrencyPluralInfo::clone);

  cpi.def(
      "__deepcopy__", [](const CurrencyPluralInfo &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  cpi.def(
      "__eq__", [](const CurrencyPluralInfo &self, const CurrencyPluralInfo &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  cpi.def(
      "__ne__", [](const CurrencyPluralInfo &self, const CurrencyPluralInfo &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  cpi.def("clone", &CurrencyPluralInfo::clone);

  cpi.def(
      "get_currency_plural_pattern",
      [](const CurrencyPluralInfo &self, const icupy::UnicodeStringVariant &plural_count, UnicodeString &result)
          -> UnicodeString & { return self.getCurrencyPluralPattern(icupy::to_unistr(plural_count), result); },
      py::arg("plural_count"), py::arg("result"));

  cpi.def("get_locale", &CurrencyPluralInfo::getLocale);

  cpi.def("get_plural_rules", &CurrencyPluralInfo::getPluralRules, py::return_value_policy::reference);

  cpi.def(
      "set_currency_plural_pattern",
      [](CurrencyPluralInfo &self, const icupy::UnicodeStringVariant &plural_count,
         const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.setCurrencyPluralPattern(icupy::to_unistr(plural_count), icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("plural_count"), py::arg("pattern"));

  cpi.def(
      "set_locale",
      [](CurrencyPluralInfo &self, const icupy::LocaleVariant &loc) {
        ErrorCode error_code;
        self.setLocale(icupy::to_locale(loc), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("loc"));

  cpi.def(
      "set_plural_rules",
      [](CurrencyPluralInfo &self, const icupy::UnicodeStringVariant &rule_description) {
        ErrorCode error_code;
        self.setPluralRules(icupy::to_unistr(rule_description), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("rule_description"));
}

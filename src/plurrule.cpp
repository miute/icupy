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
  //
  // icu::PluralRules
  //
  /*
  pr.def(py::init([]() {
      ErrorCode error_code;
      auto result = std::make_unique<PluralRules>(error_code);
      if (error_code.isFailure()) {
        throw icupy::ICUError(error_code);
      }
      return result;
    }));
  */
  pr.def(py::init<const PluralRules &>(), py::arg("other"));

  pr.def("__copy__", py::overload_cast<>(&PluralRules::clone, py::const_));

  pr.def(
      "__deepcopy__", [](const PluralRules &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  pr.def(
      "__eq__", [](const PluralRules &self, const PluralRules &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  pr.def(
      "__ne__", [](const PluralRules &self, const PluralRules &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  pr.def("clone", py::overload_cast<>(&PluralRules::clone, py::const_));

  pr.def_static("create_default_rules", []() {
    ErrorCode error_code;
    auto result = PluralRules::createDefaultRules(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  pr.def_static(
      "create_rules",
      [](const icupy::UnicodeStringVariant &description) {
        ErrorCode error_code;
        auto result = PluralRules::createRules(icupy::to_unistr(description), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("description"));

  pr.def_static(
        "for_locale",
        [](const icupy::LocaleVariant &locale) {
          ErrorCode error_code;
          auto result = PluralRules::forLocale(icupy::to_locale(locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .def_static(
          "for_locale",
          [](const icupy::LocaleVariant &locale, UPluralType type) {
            ErrorCode error_code;
            auto result = PluralRules::forLocale(icupy::to_locale(locale), type, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("type_"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      ;

  pr.def("get_keyword_other", &PluralRules::getKeywordOther);

  pr.def("get_keywords", [](const PluralRules &self) {
    ErrorCode error_code;
    auto result = self.getKeywords(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  pr.def(
      "get_samples",
      [](PluralRules &self, const icupy::UnicodeStringVariant &keyword, int32_t dest_capacity) {
        ErrorCode error_code;
        std::vector<double> result(dest_capacity);
        auto count = self.getSamples(icupy::to_unistr(keyword), result.data(), dest_capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        result.resize(count);
        return result;
      },
      py::arg("keyword"), py::arg("dest_capacity"));

  pr.def(
      "is_keyword",
      [](const PluralRules &self, const icupy::UnicodeStringVariant &keyword) -> py::bool_ {
        return self.isKeyword(icupy::to_unistr(keyword));
      },
      py::arg("keyword"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  pr.def(
      "select",
      [](const PluralRules &self, const number::FormattedNumber &number) {
        ErrorCode error_code;
        auto result = self.select(number, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("number"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  pr.def(
      "select",
      [](const PluralRules &self, const number::FormattedNumberRange &number) {
        ErrorCode error_code;
        auto result = self.select(number, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("number"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  pr.def("select", py::overload_cast<double>(&PluralRules::select, py::const_), py::arg("number").noconvert())
      .def("select", py::overload_cast<int32_t>(&PluralRules::select, py::const_), py::arg("number"));
}

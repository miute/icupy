#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dcfmtsym.h>

using namespace icu;

void init_dcfmtsym(py::module &m) {
  //
  // class icu::DecimalFormatSymbols
  //
  py::class_<DecimalFormatSymbols, UObject> dfs(m, "DecimalFormatSymbols",
                                                R"doc(
      Representation of the set of symbols required by :class:`DecimalFormat`
      for number formatting.
      )doc");

  //
  // enum icu::DecimalFormatSymbols::ENumberFormatSymbol
  //
  py::enum_<DecimalFormatSymbols::ENumberFormatSymbol>(
      dfs, "ENumberFormatSymbol", py::arithmetic(),
      "Constants for specifying a number format symbol.")
      .value("DECIMAL_SEPARATOR_SYMBOL",
             DecimalFormatSymbols::kDecimalSeparatorSymbol, R"doc(
             The decimal separator.
             )doc")
      .value("GROUPING_SEPARATOR_SYMBOL",
             DecimalFormatSymbols::kGroupingSeparatorSymbol, R"doc(
             The grouping separator.
             )doc")
      .value("PATTERN_SEPARATOR_SYMBOL",
             DecimalFormatSymbols::kPatternSeparatorSymbol, R"doc(
             The pattern separator.
             )doc")
      .value("PERCENT_SYMBOL", DecimalFormatSymbols::kPercentSymbol, R"doc(
             The percent sign.
             )doc")
      .value("ZERO_DIGIT_SYMBOL", DecimalFormatSymbols::kZeroDigitSymbol, R"doc(
             Zero.
             )doc")
      .value("DIGIT_SYMBOL", DecimalFormatSymbols::kDigitSymbol, R"doc(
             Character representing a digit in the pattern.
             )doc")
      .value("MINUS_SIGN_SYMBOL", DecimalFormatSymbols::kMinusSignSymbol, R"doc(
             The minus sign.
             )doc")
      .value("PLUS_SIGN_SYMBOL", DecimalFormatSymbols::kPlusSignSymbol, R"doc(
             The plus sign.
             )doc")
      .value("CURRENCY_SYMBOL", DecimalFormatSymbols::kCurrencySymbol, R"doc(
             The currency symbol.
             )doc")
      .value("INTL_CURRENCY_SYMBOL", DecimalFormatSymbols::kIntlCurrencySymbol,
             R"doc(
             The international currency symbol.
             )doc")
      .value("MONETARY_SEPARATOR_SYMBOL",
             DecimalFormatSymbols::kMonetarySeparatorSymbol, R"doc(
             The monetary separator.
             )doc")
      .value("EXPONENTIAL_SYMBOL", DecimalFormatSymbols::kExponentialSymbol,
             R"doc(
             The exponential symbol.
             )doc")
      .value("PER_MILL_SYMBOL", DecimalFormatSymbols::kPerMillSymbol, R"doc(
             Per mill symbol.
             )doc")
      .value("PAD_ESCAPE_SYMBOL", DecimalFormatSymbols::kPadEscapeSymbol, R"doc(
             Escape padding character.
             )doc")
      .value("INFINITY_SYMBOL", DecimalFormatSymbols::kInfinitySymbol, R"doc(
             Infinity symbol.
             )doc")
      .value("NAN_SYMBOL", DecimalFormatSymbols::kNaNSymbol, R"doc(
             Nan symbol.
             )doc")
      .value("SIGNIFICANT_DIGIT_SYMBOL",
             DecimalFormatSymbols::kSignificantDigitSymbol, R"doc(
             Significant digit symbol.
             )doc")
      .value("MONETARY_GROUPING_SEPARATOR_SYMBOL",
             DecimalFormatSymbols::kMonetaryGroupingSeparatorSymbol, R"doc(
             The monetary grouping separator.
             )doc")
      .value("ONE_DIGIT_SYMBOL", DecimalFormatSymbols::kOneDigitSymbol, R"doc(
             One.
             )doc")
      .value("TWO_DIGIT_SYMBOL", DecimalFormatSymbols::kTwoDigitSymbol, R"doc(
             Two.
             )doc")
      .value("THREE_DIGIT_SYMBOL", DecimalFormatSymbols::kThreeDigitSymbol,
             R"doc(
             Three.
             )doc")
      .value("FOUR_DIGIT_SYMBOL", DecimalFormatSymbols::kFourDigitSymbol, R"doc(
             Four.
             )doc")
      .value("FIVE_DIGIT_SYMBOL", DecimalFormatSymbols::kFiveDigitSymbol, R"doc(
             Five.
             )doc")
      .value("SIX_DIGIT_SYMBOL", DecimalFormatSymbols::kSixDigitSymbol, R"doc(
             Six.
             )doc")
      .value("SEVEN_DIGIT_SYMBOL", DecimalFormatSymbols::kSevenDigitSymbol,
             R"doc(
             Seven.
             )doc")
      .value("EIGHT_DIGIT_SYMBOL", DecimalFormatSymbols::kEightDigitSymbol,
             R"doc(
             Eight.
             )doc")
      .value("NINE_DIGIT_SYMBOL", DecimalFormatSymbols::kNineDigitSymbol, R"doc(
             Nine.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("EXPONENT_MULTIPLICATION_SYMBOL",
             DecimalFormatSymbols::kExponentMultiplicationSymbol, R"doc(
             Multiplication sign.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("FORMAT_SYMBOL_COUNT", DecimalFormatSymbols::kFormatSymbolCount,
             R"doc(
             Count symbol constants.
             )doc")
      .export_values();

  //
  // class icu::DecimalFormatSymbols
  //
  dfs.def(
         // [1] DecimalFormatSymbols::DecimalFormatSymbols
         py::init([](const icupy::LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = std::make_unique<DecimalFormatSymbols>(
               icupy::to_locale(locale), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }),
         py::arg("locale"))
      .def(
          // [2] DecimalFormatSymbols::DecimalFormatSymbols
          py::init([](const icupy::LocaleVariant &locale,
                      const NumberingSystem &ns) {
            ErrorCode error_code;
            auto result = std::make_unique<DecimalFormatSymbols>(
                icupy::to_locale(locale), ns, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("ns"))
      .def(
          // [3] DecimalFormatSymbols::DecimalFormatSymbols
          py::init([]() {
            ErrorCode error_code;
            auto result = std::make_unique<DecimalFormatSymbols>(error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }))
      .def(
          // [4] DecimalFormatSymbols::DecimalFormatSymbols
          py::init<const DecimalFormatSymbols &>(), py::arg("other"));

  dfs.def(
      "__eq__",
      [](const DecimalFormatSymbols &self, const DecimalFormatSymbols &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"));

  dfs.def(
      "__ne__",
      [](const DecimalFormatSymbols &self, const DecimalFormatSymbols &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"));

#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  dfs.def_static("create_with_last_resort_data", []() {
    ErrorCode error_code;
    auto result = DecimalFormatSymbols::createWithLastResortData(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)

  dfs.def("get_locale",
          py::overload_cast<>(&DecimalFormatSymbols::getLocale, py::const_))
      .def(
          "get_locale",
          [](const DecimalFormatSymbols &self, ULocDataLocaleType type) {
            ErrorCode error_code;
            auto result = self.getLocale(type, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("type"));

  dfs.def(
      "get_pattern_for_currency_spacing",
      [](const DecimalFormatSymbols &self, UCurrencySpacing type,
         py::bool_ before_currency) -> const UnicodeString & {
        ErrorCode error_code;
        auto &result = self.getPatternForCurrencySpacing(type, before_currency,
                                                         error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type"), py::arg("before_currency"));

  dfs.def("get_symbol", &DecimalFormatSymbols::getSymbol, py::arg("symbol"));

  dfs.def(
      "set_pattern_for_currency_spacing",
      [](DecimalFormatSymbols &self, UCurrencySpacing type,
         py::bool_ before_currency,
         const icupy::UnicodeStringVariant &pattern) {
        self.setPatternForCurrencySpacing(type, before_currency,
                                          icupy::to_unistr(pattern));
      },
      py::arg("type"), py::arg("before_currency"), py::arg("pattern"));

  dfs.def(
      "set_symbol",
      [](DecimalFormatSymbols &self,
         DecimalFormatSymbols::ENumberFormatSymbol symbol,
         const icupy::UnicodeStringVariant &value,
         const py::bool_ propagate_digits) {
        self.setSymbol(symbol, icupy::to_unistr(value), propagate_digits);
      },
      py::arg("symbol"), py::arg("value"), py::arg("propagate_digits") = true);
}

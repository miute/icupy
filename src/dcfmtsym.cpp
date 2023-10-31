#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/dcfmtsym.h>

using namespace icu;

void init_dcfmtsym(py::module &m) {
  //
  // icu::DecimalFormatSymbols
  //
  py::class_<DecimalFormatSymbols, UObject> dfs(m, "DecimalFormatSymbols");

  //
  // icu::DecimalFormatSymbols::ENumberFormatSymbol
  //
  py::enum_<DecimalFormatSymbols::ENumberFormatSymbol>(dfs, "ENumberFormatSymbol", py::arithmetic(),
                                                       "Constants for specifying a number format symbol.")
      .value("DECIMAL_SEPARATOR_SYMBOL", DecimalFormatSymbols::kDecimalSeparatorSymbol, "The decimal separator.")
      .value("GROUPING_SEPARATOR_SYMBOL", DecimalFormatSymbols::kGroupingSeparatorSymbol, "The grouping separator.")
      .value("PATTERN_SEPARATOR_SYMBOL", DecimalFormatSymbols::kPatternSeparatorSymbol, "The pattern separator.")
      .value("PERCENT_SYMBOL", DecimalFormatSymbols::kPercentSymbol, "The percent sign.")
      .value("ZERO_DIGIT_SYMBOL", DecimalFormatSymbols::kZeroDigitSymbol, "Zero.")
      .value("DIGIT_SYMBOL", DecimalFormatSymbols::kDigitSymbol, "Character representing a digit in the pattern.")
      .value("MINUS_SIGN_SYMBOL", DecimalFormatSymbols::kMinusSignSymbol, "The minus sign.")
      .value("PLUS_SIGN_SYMBOL", DecimalFormatSymbols::kPlusSignSymbol, "The plus sign.")
      .value("CURRENCY_SYMBOL", DecimalFormatSymbols::kCurrencySymbol, "The currency symbol.")
      .value("INTL_CURRENCY_SYMBOL", DecimalFormatSymbols::kIntlCurrencySymbol, "The international currency symbol.")
      .value("MONETARY_SEPARATOR_SYMBOL", DecimalFormatSymbols::kMonetarySeparatorSymbol, "The monetary separator.")
      .value("EXPONENTIAL_SYMBOL", DecimalFormatSymbols::kExponentialSymbol, "The exponential symbol.")
      .value("PER_MILL_SYMBOL", DecimalFormatSymbols::kPerMillSymbol, "Per mill symbol - replaces kPermillSymbol.")
      .value("PAD_ESCAPE_SYMBOL", DecimalFormatSymbols::kPadEscapeSymbol, "Escape padding character.")
      .value("INFINITY_SYMBOL", DecimalFormatSymbols::kInfinitySymbol, "Infinity symbol.")
      .value("NAN_SYMBOL", DecimalFormatSymbols::kNaNSymbol, "Nan symbol.")
      .value("SIGNIFICANT_DIGIT_SYMBOL", DecimalFormatSymbols::kSignificantDigitSymbol, "Significant digit symbol.")
      .value("MONETARY_GROUPING_SEPARATOR_SYMBOL", DecimalFormatSymbols::kMonetaryGroupingSeparatorSymbol,
             "The monetary grouping separator.")
      .value("ONE_DIGIT_SYMBOL", DecimalFormatSymbols::kOneDigitSymbol, "One.")
      .value("TWO_DIGIT_SYMBOL", DecimalFormatSymbols::kTwoDigitSymbol, "Two.")
      .value("THREE_DIGIT_SYMBOL", DecimalFormatSymbols::kThreeDigitSymbol, "Three.")
      .value("FOUR_DIGIT_SYMBOL", DecimalFormatSymbols::kFourDigitSymbol, "Four.")
      .value("FIVE_DIGIT_SYMBOL", DecimalFormatSymbols::kFiveDigitSymbol, "Five.")
      .value("SIX_DIGIT_SYMBOL", DecimalFormatSymbols::kSixDigitSymbol, "Six.")
      .value("SEVEN_DIGIT_SYMBOL", DecimalFormatSymbols::kSevenDigitSymbol, "Seven.")
      .value("EIGHT_DIGIT_SYMBOL", DecimalFormatSymbols::kEightDigitSymbol, "Eight.")
      .value("NINE_DIGIT_SYMBOL", DecimalFormatSymbols::kNineDigitSymbol, "Nine.")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("EXPONENT_MULTIPLICATION_SYMBOL", DecimalFormatSymbols::kExponentMultiplicationSymbol,
             "Multiplication sign.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("FORMAT_SYMBOL_COUNT", DecimalFormatSymbols::kFormatSymbolCount, "Count symbol constants.")
      .export_values();

  //
  // icu::DecimalFormatSymbols
  //
  dfs.def(
         // [1] DecimalFormatSymbols::DecimalFormatSymbols
         py::init([](const icupy::LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = std::make_unique<DecimalFormatSymbols>(icupy::to_locale(locale), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }),
         py::arg("locale"))
      .def(
          // [2] DecimalFormatSymbols::DecimalFormatSymbols
          py::init([](const icupy::LocaleVariant &locale, const NumberingSystem &ns) {
            ErrorCode error_code;
            auto result = std::make_unique<DecimalFormatSymbols>(icupy::to_locale(locale), ns, error_code);
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
      "__eq__", [](const DecimalFormatSymbols &self, const DecimalFormatSymbols &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  dfs.def(
      "__ne__", [](const DecimalFormatSymbols &self, const DecimalFormatSymbols &other) { return self != other; },
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

  dfs.def("get_locale", py::overload_cast<>(&DecimalFormatSymbols::getLocale, py::const_))
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
          py::arg("type_"));

  dfs.def(
      "get_pattern_for_currency_spacing",
      [](const DecimalFormatSymbols &self, UCurrencySpacing type, py::bool_ before_currency) -> const UnicodeString & {
        ErrorCode error_code;
        auto &result = self.getPatternForCurrencySpacing(type, before_currency, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"), py::arg("before_currency"));

  dfs.def("get_symbol", &DecimalFormatSymbols::getSymbol, py::arg("symbol"));

  dfs.def(
      "set_pattern_for_currency_spacing",
      [](DecimalFormatSymbols &self, UCurrencySpacing type, py::bool_ before_currency,
         const icupy::UnicodeStringVariant &pattern) {
        self.setPatternForCurrencySpacing(type, before_currency, icupy::to_unistr(pattern));
      },
      py::arg("type_"), py::arg("before_currency"), py::arg("pattern"));

  dfs.def(
      "set_symbol",
      [](DecimalFormatSymbols &self, DecimalFormatSymbols::ENumberFormatSymbol symbol,
         const icupy::UnicodeStringVariant &value,
         const py::bool_ propagate_digits) { self.setSymbol(symbol, icupy::to_unistr(value), propagate_digits); },
      py::arg("symbol"), py::arg("value"), py::arg("propagate_digits") = true);
}

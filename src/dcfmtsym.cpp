#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/dcfmtsym.h>

using namespace icu;

void init_dcfmtsym(py::module &m) {
  // icu::DecimalFormatSymbols
  py::class_<DecimalFormatSymbols, UObject> dfs(m, "DecimalFormatSymbols");

  py::enum_<DecimalFormatSymbols::ENumberFormatSymbol>(dfs, "ENumberFormatSymbol", py::arithmetic())
      .value("DECIMAL_SEPARATOR_SYMBOL", DecimalFormatSymbols::kDecimalSeparatorSymbol)
      .value("GROUPING_SEPARATOR_SYMBOL", DecimalFormatSymbols::kGroupingSeparatorSymbol)
      .value("PATTERN_SEPARATOR_SYMBOL", DecimalFormatSymbols::kPatternSeparatorSymbol)
      .value("PERCENT_SYMBOL", DecimalFormatSymbols::kPercentSymbol)
      .value("ZERO_DIGIT_SYMBOL", DecimalFormatSymbols::kZeroDigitSymbol)
      .value("DIGIT_SYMBOL", DecimalFormatSymbols::kDigitSymbol)
      .value("MINUS_SIGN_SYMBOL", DecimalFormatSymbols::kMinusSignSymbol)
      .value("PLUS_SIGN_SYMBOL", DecimalFormatSymbols::kPlusSignSymbol)
      .value("CURRENCY_SYMBOL", DecimalFormatSymbols::kCurrencySymbol)
      .value("INTL_CURRENCY_SYMBOL", DecimalFormatSymbols::kIntlCurrencySymbol)
      .value("MONETARY_SEPARATOR_SYMBOL", DecimalFormatSymbols::kMonetarySeparatorSymbol)
      .value("EXPONENTIAL_SYMBOL", DecimalFormatSymbols::kExponentialSymbol)
      .value("PER_MILL_SYMBOL", DecimalFormatSymbols::kPerMillSymbol)
      .value("PAD_ESCAPE_SYMBOL", DecimalFormatSymbols::kPadEscapeSymbol)
      .value("INFINITY_SYMBOL", DecimalFormatSymbols::kInfinitySymbol)
      .value("NAN_SYMBOL", DecimalFormatSymbols::kNaNSymbol)
      .value("SIGNIFICANT_DIGIT_SYMBOL", DecimalFormatSymbols::kSignificantDigitSymbol)
      .value("MONETARY_GROUPING_SEPARATOR_SYMBOL", DecimalFormatSymbols::kMonetaryGroupingSeparatorSymbol)
      .value("ONE_DIGIT_SYMBOL", DecimalFormatSymbols::kOneDigitSymbol)
      .value("TWO_DIGIT_SYMBOL", DecimalFormatSymbols::kTwoDigitSymbol)
      .value("THREE_DIGIT_SYMBOL", DecimalFormatSymbols::kThreeDigitSymbol)
      .value("FOUR_DIGIT_SYMBOL", DecimalFormatSymbols::kFourDigitSymbol)
      .value("FIVE_DIGIT_SYMBOL", DecimalFormatSymbols::kFiveDigitSymbol)
      .value("SIX_DIGIT_SYMBOL", DecimalFormatSymbols::kSixDigitSymbol)
      .value("SEVEN_DIGIT_SYMBOL", DecimalFormatSymbols::kSevenDigitSymbol)
      .value("EIGHT_DIGIT_SYMBOL", DecimalFormatSymbols::kEightDigitSymbol)
      .value("NINE_DIGIT_SYMBOL", DecimalFormatSymbols::kNineDigitSymbol)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("EXPONENT_MULTIPLICATION_SYMBOL", DecimalFormatSymbols::kExponentMultiplicationSymbol)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("FORMAT_SYMBOL_COUNT", DecimalFormatSymbols::kFormatSymbolCount)
      .export_values();

  dfs.def(
         // [1] DecimalFormatSymbols::DecimalFormatSymbols
         py::init([](const Locale &locale) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = std::make_unique<DecimalFormatSymbols>(locale, error_code);
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
            auto result = std::make_unique<DecimalFormatSymbols>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(
          // [2] DecimalFormatSymbols::DecimalFormatSymbols
          py::init([](const Locale &locale, const NumberingSystem &ns) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormatSymbols>(locale, ns, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("ns"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, const NumberingSystem &ns) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormatSymbols>(locale, ns, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("ns"))
      .def(
          // [3] DecimalFormatSymbols::DecimalFormatSymbols
          py::init([]() {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormatSymbols>(error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }))
      .def(
          // [4] DecimalFormatSymbols::DecimalFormatSymbols
          py::init<const DecimalFormatSymbols &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  dfs.def_static("create_with_last_resort_data", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = DecimalFormatSymbols::createWithLastResortData(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
  dfs.def("get_locale", py::overload_cast<>(&DecimalFormatSymbols::getLocale, py::const_))
      .def(
          "get_locale",
          [](const DecimalFormatSymbols &self, ULocDataLocaleType type) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getLocale(type, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("type_"));
  dfs.def(
      "get_pattern_for_currency_spacing",
      [](const DecimalFormatSymbols &self, UCurrencySpacing type, UBool before_currency) -> const UnicodeString & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.getPatternForCurrencySpacing(type, before_currency, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"), py::arg("before_currency"));
  dfs.def("get_symbol", &DecimalFormatSymbols::getSymbol, py::arg("symbol"));
  dfs.def("set_pattern_for_currency_spacing", &DecimalFormatSymbols::setPatternForCurrencySpacing, py::arg("type_"),
          py::arg("before_currency"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "set_pattern_for_currency_spacing",
          [](DecimalFormatSymbols &self, UCurrencySpacing type, UBool before_currency, const char16_t *pattern) {
            self.setPatternForCurrencySpacing(type, before_currency, pattern);
          },
          py::arg("type_"), py::arg("before_currency"), py::arg("pattern"));
  dfs.def("set_symbol", &DecimalFormatSymbols::setSymbol, py::arg("symbol"), py::arg("value"),
          py::arg("propogate_digits") = true)
      .def(
          // const char16_t *value -> const UnicodeString &value
          "set_symbol",
          [](DecimalFormatSymbols &self, DecimalFormatSymbols::ENumberFormatSymbol symbol, const char16_t *value,
             const UBool propogate_digits) { self.setSymbol(symbol, value, propogate_digits); },
          py::arg("symbol"), py::arg("value"), py::arg("propogate_digits") = true);
}

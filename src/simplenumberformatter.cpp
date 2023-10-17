#include "main.hpp"
#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
#include <unicode/simplenumberformatter.h>
#include <unicode/usimplenumberformatter.h>

using namespace icu;
using namespace icu::number;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

void init_simplenumberformatter(py::module & /*m*/, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  //
  // icu::number::SimpleNumber
  //
  py::class_<SimpleNumber, UMemory> sn(m2, "SimpleNumber");

  //
  // icu::number::SimpleNumberFormatter
  //
  py::class_<SimpleNumberFormatter, UMemory> fmt(m2, "SimpleNumberFormatter");

  //
  // icu::number::SimpleNumber
  //
  sn.def(py::init<>());

  sn.def_static(
      "for_int64",
      [](int64_t value) {
        ErrorCode error_code;
        auto result = SimpleNumber::forInt64(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("value"));

  sn.def(
      "multiply_by_power_of_ten",
      [](SimpleNumber &self, int32_t power) {
        ErrorCode error_code;
        self.multiplyByPowerOfTen(power, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("power"));

  sn.def(
      "round_to",
      [](SimpleNumber &self, int32_t power, UNumberFormatRoundingMode rounding_mode) {
        ErrorCode error_code;
        self.roundTo(power, rounding_mode, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("power"), py::arg("rounding_mode"));

  sn.def(
      "set_minimum_fraction_digits",
      [](SimpleNumber &self, uint32_t minimum_fraction_digits) {
        ErrorCode error_code;
        self.setMinimumFractionDigits(minimum_fraction_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("minimum_fraction_digits"));

  sn.def(
      "set_minimum_integer_digits",
      [](SimpleNumber &self, uint32_t minimum_integer_digits) {
        ErrorCode error_code;
        self.setMinimumIntegerDigits(minimum_integer_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("minimum_integer_digits"));

  sn.def(
      "set_sign",
      [](SimpleNumber &self, USimpleNumberSign sign) {
        ErrorCode error_code;
        self.setSign(sign, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("sign"));

  sn.def(
      "truncate_start",
      [](SimpleNumber &self, uint32_t maximum_integer_digits) {
        ErrorCode error_code;
        self.truncateStart(maximum_integer_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("maximum_integer_digits"));

  //
  // icu::number::SimpleNumberFormatter
  //
  fmt.def(py::init<>());

  fmt.def_static(
      "for_locale",
      [](const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        auto result = SimpleNumberFormatter::forLocale(icupy::to_locale(locale), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"));

  fmt.def_static(
      "for_locale_and_grouping_strategy",
      [](const icupy::LocaleVariant &locale, UNumberGroupingStrategy grouping_strategy) {
        ErrorCode error_code;
        auto result = SimpleNumberFormatter::forLocaleAndGroupingStrategy(icupy::to_locale(locale), grouping_strategy,
                                                                          error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("grouping_strategy"));

  fmt.def_static(
      "for_locale_and_symbols_and_grouping_strategy",
      [](const icupy::LocaleVariant &locale, const DecimalFormatSymbols &symbols,
         UNumberGroupingStrategy grouping_strategy) {
        ErrorCode error_code;
        auto result = SimpleNumberFormatter::forLocaleAndSymbolsAndGroupingStrategy(icupy::to_locale(locale), symbols,
                                                                                    grouping_strategy, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("symbols"), py::arg("grouping_strategy"));

  fmt.def(
      "format",
      [](const SimpleNumberFormatter &self, SimpleNumber &value) {
        ErrorCode error_code;
        auto result = self.format(std::move(value), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("value"));

  fmt.def(
      "format_int64",
      [](const SimpleNumberFormatter &self, int64_t value) {
        ErrorCode error_code;
        auto result = self.formatInt64(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)
}

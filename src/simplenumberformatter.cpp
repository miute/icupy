#include "main.hpp"
#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
#include <unicode/simplenumberformatter.h>
#include <unicode/usimplenumberformatter.h>

using namespace icu;
using namespace icu::number;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

void init_simplenumberformatter(py::module & /* m */, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  //
  // class icu::number::SimpleNumber
  //
  py::class_<SimpleNumber, UMemory> sn(m2, "SimpleNumber", R"doc(
      Input type for :class:`SimpleNumberFormatter`.
      )doc");

  //
  // class icu::number::SimpleNumberFormatter
  //
  py::class_<SimpleNumberFormatter, UMemory> fmt(m2, "SimpleNumberFormatter",
                                                 R"doc(
      Special :class:`NumberFormatter` focused on smaller binary size and
      memory use.

      SimpleNumberFormatter is capable of basic number formatting, including
      grouping separators, sign display, and rounding. It is not capable of
      currencies, compact notation, or units.

      Example:

      .. code-block:: python

          from icupy import icu
          from icupy.icu import number
          fmt = number.SimpleNumberFormatter.for_locale_and_grouping_strategy("de-CH", icu.UNUM_GROUPING_ON_ALIGNED)
          fmtval = fmt.format_int64(1234567)
          fmtval.to_string()  # "1'234'567"
      )doc");

  //
  // class icu::number::SimpleNumber
  //
  sn.def(py::init<>(), R"doc(
      Initialize a ``SimpleNumber`` instance as an empty SimpleNumber.
      )doc");

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
      py::arg("value"), R"doc(
      Create a ``SimpleNumber`` for an integer.
      )doc");

  sn.def(
      "multiply_by_power_of_ten",
      [](SimpleNumber &self, int32_t power) {
        ErrorCode error_code;
        self.multiplyByPowerOfTen(power, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("power"), R"doc(
      Change the value of the ``SimpleNumber`` by a power of 10.
      )doc");

  sn.def(
      "round_to",
      [](SimpleNumber &self, int32_t power,
         UNumberFormatRoundingMode rounding_mode) {
        ErrorCode error_code;
        self.roundTo(power, rounding_mode, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("power"), py::arg("rounding_mode"), R"doc(
      Round the value currently stored in the ``SimpleNumber`` to the given
      power of 10, which can be before or after the decimal separator.

      This function does not change minimum integer digits.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 75)
  sn.def(
      "set_maximum_integer_digits",
      [](SimpleNumber &self, uint32_t maximum_integer_digits) {
        ErrorCode error_code;
        self.setMaximumIntegerDigits(maximum_integer_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("maximum_integer_digits"), R"doc(
      Set the number of integer digits to the given amount, truncating if
      necessary.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 75)

  sn.def(
      "set_minimum_fraction_digits",
      [](SimpleNumber &self, uint32_t minimum_fraction_digits) {
        ErrorCode error_code;
        self.setMinimumFractionDigits(minimum_fraction_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("minimum_fraction_digits"), R"doc(
      Pad the end of the number with zeros up to the given minimum number of
      fraction digits.
      )doc");

  sn.def(
      "set_minimum_integer_digits",
      [](SimpleNumber &self, uint32_t minimum_integer_digits) {
        ErrorCode error_code;
        self.setMinimumIntegerDigits(minimum_integer_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("minimum_integer_digits"), R"doc(
      Pad the beginning of the number with zeros up to the given minimum number
      of integer digits.
      )doc");

  sn.def(
      "set_sign",
      [](SimpleNumber &self, USimpleNumberSign sign) {
        ErrorCode error_code;
        self.setSign(sign, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("sign"), R"doc(
      Set the sign of the number.

      This setting is applied upon formatting the number.

      .. note::

          This does not support accounting sign notation.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM < 76)
  sn.def(
      "truncate_start",
      [](SimpleNumber &self, uint32_t maximum_integer_digits) {
        ErrorCode error_code;
        self.truncateStart(maximum_integer_digits, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("maximum_integer_digits"), R"doc(
      Deprecated: ICU 75. Use :meth:`set_maximum_integer_digits` instead.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM < 76)

  //
  // class icu::number::SimpleNumberFormatter
  //
  fmt.def(py::init<>(), R"doc(
      Initialize a ``SimpleNumberFormatter`` as a non-functional
      SimpleNumberFormatter.
      )doc");

  fmt.def_static(
      "for_locale",
      [](const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        auto result = SimpleNumberFormatter::forLocale(icupy::to_locale(locale),
                                                       error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), R"doc(
      Create a new ``SimpleNumberFormatter`` instance with all locale
      defaults.
      )doc");

  fmt.def_static(
      "for_locale_and_grouping_strategy",
      [](const icupy::LocaleVariant &locale,
         UNumberGroupingStrategy grouping_strategy) {
        ErrorCode error_code;
        auto result = SimpleNumberFormatter::forLocaleAndGroupingStrategy(
            icupy::to_locale(locale), grouping_strategy, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("grouping_strategy"), R"doc(
      Override the grouping strategy to create a new ``SimpleNumberFormatter``
      instance with all locale defaults.
      )doc");

  fmt.def_static(
      "for_locale_and_symbols_and_grouping_strategy",
      [](const icupy::LocaleVariant &locale,
         const DecimalFormatSymbols &symbols,
         UNumberGroupingStrategy grouping_strategy) {
        ErrorCode error_code;
        auto result =
            SimpleNumberFormatter::forLocaleAndSymbolsAndGroupingStrategy(
                icupy::to_locale(locale), symbols, grouping_strategy,
                error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("symbols"), py::arg("grouping_strategy"),
      R"doc(
      Override the grouping strategy and symbols to create a new
      ``SimpleNumberFormatter`` instance with all locale defaults.

      .. important::

          *symbols* must outlive the ``SimpleNumberFormatter`` object.
      )doc");

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
      py::arg("value"), R"doc(
      Format a value using this ``SimpleNumberFormatter``.

      .. important::

          The :class:`SimpleNumber` argument is consumed. A new
          :class:`SimpleNumber` object should be created for every formatting
          operation.
      )doc");

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
      py::arg("value"), R"doc(
      Format an integer value using this ``SimpleNumberFormatter``.

      For more control over the formatting, use :class:`SimpleNumber`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)
}

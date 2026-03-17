#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
#include <pybind11/stl.h>
#include <unicode/numberformatter.h>

#if (U_ICU_VERSION_MAJOR_NUM == 71)
#include <unicode/unounclass.h>
#elif (U_ICU_VERSION_MAJOR_NUM >= 72)
#include <unicode/displayoptions.h>
#endif // (U_ICU_VERSION_MAJOR_NUM == 71)

using namespace icu;
using namespace icu::number;

using _LocalizedNumberFormatterSettings =
    NumberFormatterSettings<LocalizedNumberFormatter>;
using _UnlocalizedNumberFormatterSettings =
    NumberFormatterSettings<UnlocalizedNumberFormatter>;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

void init_numberformatter(py::module &, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  //
  // class icu::number::NumberFormatter
  //
  py::class_<NumberFormatter> nf(m2, "NumberFormatter", R"doc(
      All-in-one formatter for localized numbers, currencies, and units.

      Examples:
          Basic usage:

          .. code-block:: python

              from icupy import icu
              from icupy.icu import number
              fmt = number.NumberFormatter.with_locale(icu.ULOC_US)
              fmt.format_double(1.234).to_string()  # "1.234" in en-US

          Custom notation, unit, and rounding precision:

          .. code-block:: python

              template = (
                  number.NumberFormatter.with_()
                  .notation(number.Notation.compact_short())
                  .unit(icu.CurrencyUnit("EUR"))
                  .precision(number.Precision.max_significant_digits(2))
              )
              template.locale(icu.ULOC_US).format_int(1234).to_string()  # "€1.2K" in en-US
      )doc");

  //
  // class icu::number::FormattedNumber
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  py::class_<FormattedNumber, UMemory, FormattedValue> fn(m2, "FormattedNumber",
                                                          R"doc(
      Result of a number formatting operation.
      )doc");
#else
  py::class_<FormattedNumber, UMemory> fn(m2, "FormattedNumber"
                                              R"doc(
      Result of a number formatting operation.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  //
  // class icu::number::IntegerWidth
  //
  py::class_<IntegerWidth, UMemory> iw(m2, "IntegerWidth", R"doc(
      Define a strategy for padding and truncating the integer preceding the
      decimal separator.
      )doc");

  //
  // class icu::number::Notation
  //
  py::class_<Notation, UMemory> no(m2, "Notation", R"doc(
      Define the notation style used when formatting numbers with
      :class:`NumberFormatter`.

      See Also:
          :class:`ScientificNotation`
      )doc");

  //
  // class icu::number::ScientificNotation
  //
  py::class_<ScientificNotation, Notation> sn(m2, "ScientificNotation", R"doc(
      Define the scientific notation style used when formatting numbers
      with :class:`NumberFormatter`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::Precision
  //
  py::class_<Precision, UMemory> pre(m2, "Precision", R"doc(
      Define the rounding precision used when formatting numbers with
      :class:`NumberFormatter`.

      See Also:
          :class:`CurrencyPrecision`
          :class:`FractionPrecision`
          :class:`IncrementPrecision`
      )doc");

  //
  // class icu::number::CurrencyPrecision
  //
  py::class_<CurrencyPrecision, Precision> cp(m2, "CurrencyPrecision", R"doc(
      Define a rounding precision parameterized by a currency to be used when
      formatting numbers in :class:`NumberFormatter`.
      )doc");

  //
  // class icu::number::FractionPrecision
  //
  py::class_<FractionPrecision, Precision> fp(m2, "FractionPrecision", R"doc(
      Define a rounding precision based on a number of fraction places and
      optionally significant digits to be used when formatting numbers in
      :class:`NumberFormatter`.
      )doc");

  //
  // class icu::number::IncrementPrecision
  //
  py::class_<IncrementPrecision, Precision> ip(m2, "IncrementPrecision", R"doc(
      Define a rounding precision parameterized by a rounding increment to be
      used when formatting numbers in :class:`NumberFormatter`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::Scale
  //
  py::class_<Scale, UMemory> sc(m2, "Scale", R"doc(
      Define a quantity by which a number should be multiplied when formatting.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  //
  // class icu::number::NumberFormatterSettings<LocalizedNumberFormatter>
  //
  py::class_<_LocalizedNumberFormatterSettings> nfs_lnf(
      m2, "_LocalizedNumberFormatterSettings", R"doc(
      Abstract base class for specifying settings related to number formatting.

      See Also:
          :class:`LocalizedNumberFormatter`
      )doc");

  //
  // class icu::number::NumberFormatterSettings<UnlocalizedNumberFormatter>
  //
  py::class_<_UnlocalizedNumberFormatterSettings> nfs_unf(
      m2, "_UnlocalizedNumberFormatterSettings", R"doc(
      Abstract base class for specifying settings related to number formatting.

      See Also:
          :class:`UnlocalizedNumberFormatter`
      )doc");

  //
  // class icu::number::LocalizedNumberFormatter
  //
  py::class_<LocalizedNumberFormatter, _LocalizedNumberFormatterSettings,
             UMemory>
      lnf(m2, "LocalizedNumberFormatter", R"doc(
      :class:`NumberFormatter` that has a locale associated with it.

      See Also:
          :class:`NumberFormatter`
          :class:`UnlocalizedNumberFormatter`
      )doc");

  //
  // class icu::number::UnlocalizedNumberFormatter
  //
  py::class_<UnlocalizedNumberFormatter, _UnlocalizedNumberFormatterSettings,
             UMemory>
      unf(m2, "UnlocalizedNumberFormatter", R"doc(
      :class:`NumberFormatter` that does not yet have a locale.

      In order to format numbers, a locale must be specified.

      See Also:
          :class:`LocalizedNumberFormatter`
          :class:`NumberFormatter`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::CurrencyPrecision
  //
  cp.def("with_currency", &CurrencyPrecision::withCurrency, py::arg("currency"),
         R"doc(
      Associate a currency with this rounding precision.

      .. attention::
         Calling this method is not required, because the currency specified
         ``unit()`` is automatically applied to currency rounding precisions.
         However, this method enables you to override that automatic
         association.

         This method also enables numbers to be formatted using currency
         rounding rules without explicitly using a currency format.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  //
  // class icu::number::FormattedNumber
  //
  fn.def(py::init<>(), R"doc(
      Initialize a ``FormattedNumber`` instance as an empty FormattedNumber.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62 && U_ICU_VERSION_MAJOR_NUM < 64)
  fn.def(
      "append_to",
      [](const FormattedNumber &self, Appendable &appendable) -> Appendable & {
        ErrorCode error_code;
        auto &result = self.appendTo(appendable, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"), R"doc(
      Append the formatted string to *appendable*.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62 && U_ICU_VERSION_MAJOR_NUM < 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 71)
  fn.def(
      "get_noun_class",
      [](const FormattedNumber &self) {
        ErrorCode error_code;
        auto result = self.getNounClass(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the noun class of the formatted output.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fn.def(
      "get_output_unit",
      [](const FormattedNumber &self) {
        ErrorCode error_code;
        auto result = self.getOutputUnit(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the resolved output unit.

      The output unit is dependent upon the localized preferences for the usage
      specified via :meth:`_LocalizedNumberFormatterSettings.usage`, and may be
      a unit with :attr:`~icupy.icu.UMeasureUnitComplexity.UMEASURE_UNIT_MIXED`
      unit complexity (:meth:`icupy.icu.MeasureUnit.get_complexity`), such as
      "foot-and-inch" or "hour-and-minute-and-second".
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  fn.def(
      "to_decimal_number",
      [](const FormattedNumber &self) {
        ErrorCode error_code;
        auto result = self.toDecimalNumber<std::string>(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Export the formatted number as a "numeric string" conforming to the
      syntax defined in the
      `Decimal Arithmetic Specification <http://speleotrove.com/decimal>`__.

      This endpoint is useful for obtaining the exact number being printed
      after scaling and rounding have been applied by the number formatter.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 62 && U_ICU_VERSION_MAJOR_NUM < 64)
  fn.def(
      "to_string",
      [](const FormattedNumber &self) {
        ErrorCode error_code;
        auto result = self.toString(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the formatted string.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62 && U_ICU_VERSION_MAJOR_NUM < 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::FractionPrecision
  //
  fp.def("with_max_digits", &FractionPrecision::withMaxDigits,
         py::arg("max_significant_digits"), R"doc(
      Ensure that no more than this number of significant digits are retained
      when rounding according to fraction rules.

      For example, with integer rounding, the number 123.4 becomes "123".
      However, with maximum figures set to 2, 123.4 becomes "120" instead.

      This setting does not affect the number of trailing zeros. For example,
      with fixed fraction of 2, 123.4 would become "120.00".

      This is equivalent to:

      .. code-block:: python

          with_significant_digits(1, max_significant_digits, UNUM_ROUNDING_PRIORITY_STRICT)
      )doc");

  fp.def("with_min_digits", &FractionPrecision::withMinDigits,
         py::arg("min_significant_digits"), R"doc(
      Ensure that no less than this number of significant digits are retained
      when rounding according to fraction rules.

      For example, with integer rounding, the number 3.141 becomes "3".
      However, with minimum figures set to 2, 3.141 becomes "3.1" instead.

      This setting does not affect the number of trailing zeros. For example,
      3.01 would print as "3", not "3.0".

      This is equivalent to:

      .. code-block:: python

          with_significant_digits(1, min_significant_digits, UNUM_ROUNDING_PRIORITY_RELAXED)
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  fp.def("with_significant_digits", &FractionPrecision::withSignificantDigits,
         py::arg("min_significant_digits"), py::arg("max_significant_digits"),
         py::arg("priority"), R"doc(
      Override maximum fraction digits with maximum significant digits
      depending on the magnitude of the number.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::IncrementPrecision
  //
  ip.def("with_min_fraction", &IncrementPrecision::withMinFraction,
         py::arg("min_frac"), R"doc(
      Specifies the minimum number of fraction digits to render after the
      decimal separator, padding with zeros if necessary.

      By default, no trailing zeros are added.

      For example, if the rounding increment is 0.5 and *min_frac* is 2, then
      the resulting strings include "0.00", "0.50", "1.00", and "1.50".
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  //
  // class icu::number::IntegerWidth
  //
  iw.def("truncate_at", &IntegerWidth::truncateAt, py::arg("max_int"), R"doc(
      Truncate numbers exceeding a certain number of numerals before the
      decimal separator.

      For example, with *max_int=3*, the number 1234 will get printed as "234".

      *max_int=-1* means no truncation.
      )doc");

  iw.def_static("zero_fill_to", &IntegerWidth::zeroFillTo, py::arg("min_int"),
                R"doc(
      Pad numbers at the beginning with zeros to guarantee a certain number of
      numerals before the decimal separator.

      For example, with *min_int=3*, the number 55 will get printed as "055".
      )doc");

  //
  // class icu::number::LocalizedNumberFormatter
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  lnf.def(py::init<>(), R"doc(
      Initialize a ``LocalizedNumberFormatter`` instance in a valid but
      undefined state.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  lnf.def(py::init<const LocalizedNumberFormatter &>(), py::arg("other"), R"doc(
      Initialize a ``LocalizedNumberFormatter`` instance from another
      ``LocalizedNumberFormatter``.
      )doc");

  lnf.def(
      "format_decimal",
      [](const LocalizedNumberFormatter &self, const std::string &value) {
        ErrorCode error_code;
        auto result = self.formatDecimal(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("value"), R"doc(
      Format the given decimal number to a string using the settings specified
      in the :class:`NumberFormatter` fluent setting chain.

      The syntax of *value* is a "numeric string" as defined in the
      `Decimal Arithmetic Specification <http://speleotrove.com/decimal>`__.
      )doc");

  lnf.def(
      "format_double",
      [](const LocalizedNumberFormatter &self, double value) {
        ErrorCode error_code;
        auto result = self.formatDouble(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("value"), R"doc(
      Format the given float to a string using the settings specified in
      the :class:`NumberFormatter` fluent setting chain.
      )doc");

  lnf.def(
      "format_int",
      [](const LocalizedNumberFormatter &self, int64_t value) {
        ErrorCode error_code;
        auto result = self.formatInt(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("value"), R"doc(
      Format the given integer number to a string using the settings specified
      in the :class:`NumberFormatter` fluent setting chain.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  lnf.def(
      "to_format",
      [](const LocalizedNumberFormatter &self) {
        ErrorCode error_code;
        auto result = self.toFormat(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Create a representation of this ``LocalizedNumberFormat`` as
      :class:`~icupy.icu.Format`.

      .. important::
         This API is not intended to be used other than for enabling API
         compatibility. For number formatting, use :meth:`.format_decimal`,
         :meth:`.format_double`, and :meth:`.format_int` instead.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 75)
  lnf.def(
      "without_locale",
      [](const LocalizedNumberFormatter &self) { return self.withoutLocale(); },
      R"doc(
      Disassociate the locale from this formatter.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 75)

  //
  // class icu::number::NumberFormatter
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nf.def_static(
      "for_skeleton",
      [](const icupy::UnicodeStringVariant &skeleton) {
        ErrorCode error_code;
        auto result = NumberFormatter::forSkeleton(icupy::to_unistr(skeleton),
                                                   error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("skeleton"), R"doc(
      Create a new :class:`UnlocalizedNumberFormatter` object with the
      specified skeleton string.

      For more information, see the ICU User Guide:
      `Number Skeletons <https://unicode-org.github.io/icu/userguide/format_parse/numbers/skeletons.html>`__.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nf.def_static(
      "for_skeleton",
      [](const icupy::UnicodeStringVariant &skeleton, UParseError &perror) {
        ErrorCode error_code;
        auto result = NumberFormatter::forSkeleton(icupy::to_unistr(skeleton),
                                                   perror, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("skeleton"), py::arg("perror"), R"doc(
      Create a new :class:`UnlocalizedNumberFormatter` object with the
      specified skeleton string.

      If an error occurs while parsing the skeleton string, the offset into the
      skeleton string at which the error occurred will be saved into the
      :class:`~icupy.icu.UParseError`.

      For more information, see the ICU User Guide:
      `Number Skeletons <https://unicode-org.github.io/icu/userguide/format_parse/numbers/skeletons.html>`__.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  nf.def_static("with_", &NumberFormatter::with, R"doc(
      Create a new :class:`UnlocalizedNumberFormatter` object without locale.
      )doc");

  nf.def_static(
      "with_locale",
      [](const icupy::LocaleVariant &locale) {
        return NumberFormatter::withLocale(icupy::to_locale(locale));
      },
      py::arg("locale"), R"doc(
      Create a new :class:`LocalizedNumberFormatter` object associated with
      the specified *locale*.
      )doc");

  //
  // class icu::number::NumberFormatterSettings<LocalizedNumberFormatter>
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nfs_lnf.def(
      "__copy__",
      [](const _LocalizedNumberFormatterSettings &self) {
        return std::unique_ptr<LocalizedNumberFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nfs_lnf.def(
      "__deepcopy__",
      [](const _LocalizedNumberFormatterSettings &self, py::dict & /* memo */) {
        return std::unique_ptr<LocalizedNumberFormatter>(self.clone());
      },
      py::arg("memo"),
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nfs_lnf.def(
      "clone",
      [](const _LocalizedNumberFormatterSettings &self) {
        return std::unique_ptr<LocalizedNumberFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  nfs_lnf.def(
      "copy_error_to",
      [](const _LocalizedNumberFormatterSettings &self,
         ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Set the :class:`~icupy.icu.UErrorCode` if an error occurred in the
      fluent chain.

      Preserves older error codes in the *out_error_code*.
      )doc");

  nfs_lnf.def(
      "decimal",
      [](const _LocalizedNumberFormatterSettings &self,
         UNumberDecimalSeparatorDisplay style) { return self.decimal(style); },
      py::arg("style"), R"doc(
      Set the decimal separator display strategy.

      This affects integer numbers with no fraction part. Most common values:

      - AUTO: "1"
      - ALWAYS: "1."

      The default is AUTO decimal separator display.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  nfs_lnf.def(
      "display_options",
      [](const _LocalizedNumberFormatterSettings &self,
         const DisplayOptions &display_options) {
        return self.displayOptions(display_options);
      },
      py::arg("display_options"), R"doc(
      Specify the :class:`~icupy.icu.DisplayOptions`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_lnf.def(
      "grouping",
      [](const _LocalizedNumberFormatterSettings &self,
         UNumberGroupingStrategy strategy) { return self.grouping(strategy); },
      py::arg("strategy"), R"doc(
      Specify the grouping strategy used when formatting numbers.

      - Default grouping: "12,300" and "1,230"
      - Grouping with at least 2 digits: "12,300" and "1230"
      - No grouping: "12300" and "1230"

      The exact grouping width is selected based on the locale.

      The default is to perform grouping according to locale data; most
      locales, but not all locales, enable it by default.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

  nfs_lnf.def(
      "integer_width",
      [](const _LocalizedNumberFormatterSettings &self,
         const IntegerWidth &style) { return self.integerWidth(style); },
      py::arg("style"), R"doc(
      Specify the minimum and maximum number of digits to render before the
      decimal mark.

      - Zero minimum integer digits: ".08"
      - One minimum integer digit: "0.08"
      - Two minimum integer digits: "00.08"

      The default is to have one minimum integer digit.
      )doc");

  nfs_lnf.def(
      "notation",
      [](const _LocalizedNumberFormatterSettings &self,
         const Notation &notation) { return self.notation(notation); },
      py::arg("notation"), R"doc(
      Specify the notation style (simple, scientific, or compact) for
      rendering numbers.

      - Simple notation: "12,300"
      - Scientific notation: "1.23E4"
      - Compact notation: "12K"

      The default is to use simple notation.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_lnf.def(
      "per_unit",
      [](const _LocalizedNumberFormatterSettings &self,
         const MeasureUnit &per_unit) { return self.perUnit(per_unit); },
      py::arg("per_unit"), R"doc(
      Set a unit to be used in the denominator.

      The default is not to display any unit in the denominator.

      If a per-unit is specified without a primary unit via unit, the behavior
      is undefined.

      Example:
          To format "3 m/s", pass METER as the unit and SECOND as the unit per:

          .. code-block:: python

              NumberFormatter.with_().unit(MeasureUnit.get_meter()).per_unit(MeasureUnit.get_second())
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_lnf.def(
      "precision",
      [](const _LocalizedNumberFormatterSettings &self,
         const Precision &precision) { return self.precision(precision); },
      py::arg("precision"), R"doc(
      Specify the rounding precision to use when formatting numbers.

      - Round to 3 decimal places: "3.142"
      - Round to 3 significant figures: "3.14"
      - Round to the closest nickel: "3.15"
      - Do not perform rounding: "3.1415926..."

      In most cases, the default rounding strategy is to round to 6 fraction
      places; i.e., Precision.max_fraction(6). The exceptions are if compact
      notation is being used, then the compact notation rounding strategy is
      used (see :meth:`Notation.compact_short` for details), or if the unit is
      a currency, then standard currency rounding is used, which varies from
      currency to currency (see :meth:`Precision.currency` for details).
      )doc");

  nfs_lnf.def(
      "rounding_mode",
      [](const _LocalizedNumberFormatterSettings &self,
         UNumberFormatRoundingMode rounding_mode) {
        return self.roundingMode(rounding_mode);
      },
      py::arg("rounding_mode"), R"doc(
      Specify how to determine the direction to round a number when it has more
      digits than fit in the desired precision.

      When formatting 1.235:

      - Ceiling rounding mode with integer precision: "2"
      - Half-down rounding mode with 2 fixed fraction digits: "1.23"
      - Half-up rounding mode with 2 fixed fraction digits: "1.24"

      The default is HALF_EVEN. For more information, see the ICU User Guide:
      `Rounding Modes <https://unicode-org.github.io/icu/userguide/format_parse/numbers/rounding-modes>`__.
      )doc");

  nfs_lnf.def(
      "scale",
      [](const _LocalizedNumberFormatterSettings &self, const Scale &scale) {
        return self.scale(scale);
      },
      py::arg("scale"), R"doc(
      Set a scale (multiplier) to be used to scale the number by an arbitrary
      amount before formatting.

      Most common values:

      - Multiply by 100: useful for percentages.
      - Multiply by an arbitrary value: useful for unit conversions.

      The default is to not apply any multiplier.
      )doc");

  nfs_lnf.def(
      "sign",
      [](const _LocalizedNumberFormatterSettings &self,
         UNumberSignDisplay style) { return self.sign(style); },
      py::arg("style"), R"doc(
      Set the plus/minus sign display strategy.

      Most common values:

      - AUTO: "123", "-123"
      - ALWAYS: "+123", "-123"
      - ACCOUNTING: "$123", "($123)"

      The default is AUTO sign display.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  nfs_lnf.def(
      "symbols",
      [](const _LocalizedNumberFormatterSettings &self,
         const DecimalFormatSymbols &symbols) { return self.symbols(symbols); },
      py::arg("symbols"), R"doc(
      Specify the symbols (decimal separator, grouping separator, percent sign,
      numerals, etc.) to use when rendering numbers.

      - en_US symbols: "12,345.67"
      - fr_FR symbols: "12 345,67"
      - de_CH symbols: "12’345.67"
      - my_MY symbols: "၁၂,၃၄၅.၆၇"

      The default is to choose the symbols based on the locale specified in the
      fluent chain.

      .. note::

          :class:`~icupy.icu.DecimalFormatSymbols` automatically chooses the
          best numbering system based on the locale. In the examples above, the
          first three are using the Latin numbering system, and the fourth is
          using the Myanmar numbering system.

      .. note::

          An instance of :class:`~icupy.icu.DecimalFormatSymbols` will be
          copied: changes made to the symbols object after passing it into the
          fluent chain will not be seen.

      .. note::

          Calling this method will override any previously specified
          :class:`~icupy.icu.DecimalFormatSymbols` or
          :class:`~icupy.icu.NumberingSystem`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_lnf.def(
      "to_skeleton",
      [](const _LocalizedNumberFormatterSettings &self) {
        ErrorCode error_code;
        auto result = self.toSkeleton(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Create a skeleton string representation of this number formatter.

      A skeleton string is a locale-agnostic serialized form of a number
      formatter.

      For more information, see the ICU User Guide:
      `Number Skeletons <https://unicode-org.github.io/icu/userguide/format_parse/numbers/skeletons.html>`__.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  nfs_lnf.def(
      "unit",
      [](const _LocalizedNumberFormatterSettings &self,
         const MeasureUnit &unit) { return self.unit(unit); },
      py::arg("unit"), R"doc(
      Specify the unit (unit of measure, currency, or percent) to associate
      with rendered numbers.

      - Unit of measure: "12.3 meters"
      - Currency: "$12.30"
      - Percent: "12.3%"

      The default is to render without units.

      .. note::

          If the :meth:`.usage` is set, the output unit will be changed to
          produce localised units, according to usage, locale and unit. See
          :meth:`FormattedNumber.get_output_unit`.
      )doc");

  nfs_lnf.def(
      "unit_width",
      [](const _LocalizedNumberFormatterSettings &self,
         UNumberUnitWidth width) { return self.unitWidth(width); },
      py::arg("width"), R"doc(
      Set the width of the unit (measure unit or currency).

      Most common values:

      - SHORT: "$12.00", "12 m"
      - ISO_CODE: "USD 12.00"
      - FULL_NAME: "12.00 US dollars", "12 meters"

      The default is the SHORT width.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  nfs_lnf.def(
      "usage",
      [](const _LocalizedNumberFormatterSettings &self,
         const std::string &usage) { return self.usage(usage); },
      py::arg("usage"), R"doc(
      Specify the usage for which numbers will be formatted ("person-height",
      "road", "rainfall", etc.).

      When a usage is specified, the output unit will change depending on the
      :class:`~icupy.icu.Locale` and the unit quantity. For example, formatting
      length measurements specified in meters:

      .. code-block:: python

          NumberFormatter.with_().usage("person").unit(MeasureUnit.get_meter()).locale("en-US")

          # When formatting 0.25, the output will be "10 inches".
          # When formatting 1.50, the output will be "4 feet and 11 inches".

      The input unit specified via :meth:`.unit` determines the type of
      measurement being formatted (e.g. "length" when the unit is "foot"). The
      usage requested will be looked for only within this category of
      measurement units.

      The output unit can be found via :meth:`FormattedNumber.get_output_unit`.

      If the usage has multiple parts (e.g. "land-agriculture-grain") and does
      not match a known usage preference, the last part will be dropped
      repeatedly until a match is found (e.g. trying "land-agriculture",
      then "land"). If a match is still not found, usage will fall back to
      "default".

      Setting usage to an empty string clears the usage (disables usage-based
      localized formatting).

      Setting a usage string but not a correct input unit will result in an
      :attr:`~icupy.icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR`.

      When using usage, specifying rounding or precision is unnecessary.
      Specifying a precision in some manner will override the default
      formatting.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  //
  // class icu::number::NumberFormatterSettings<UnlocalizedNumberFormatter>
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nfs_unf.def(
      "__copy__",
      [](const _UnlocalizedNumberFormatterSettings &self) {
        return std::unique_ptr<UnlocalizedNumberFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nfs_unf.def(
      "__deepcopy__",
      [](const _UnlocalizedNumberFormatterSettings &self,
         py::dict & /* memo */) {
        return std::unique_ptr<UnlocalizedNumberFormatter>(self.clone());
      },
      py::arg("memo"),
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nfs_unf.def(
      "clone",
      [](const _UnlocalizedNumberFormatterSettings &self) {
        return std::unique_ptr<UnlocalizedNumberFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  nfs_unf.def(
      "copy_error_to",
      [](const _UnlocalizedNumberFormatterSettings &self,
         ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Set the :class:`~icupy.icu.UErrorCode` if an error occurred in the
      fluent chain.

      Preserves older error codes in the *out_error_code*.
      )doc");

  nfs_unf.def(
      "decimal",
      [](const _UnlocalizedNumberFormatterSettings &self,
         UNumberDecimalSeparatorDisplay style) { return self.decimal(style); },
      py::arg("style"), R"doc(
      Set the decimal separator display strategy.

      This affects integer numbers with no fraction part. Most common values:

      - AUTO: "1"
      - ALWAYS: "1."

      The default is AUTO decimal separator display.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  nfs_unf.def(
      "display_options",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const DisplayOptions &display_options) {
        return self.displayOptions(display_options);
      },
      py::arg("display_options"), R"doc(
      Specify the :class:`~icupy.icu.DisplayOptions`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_unf.def(
      "grouping",
      [](const _UnlocalizedNumberFormatterSettings &self,
         UNumberGroupingStrategy strategy) { return self.grouping(strategy); },
      py::arg("strategy"), R"doc(
      Specify the grouping strategy used when formatting numbers.

      - Default grouping: "12,300" and "1,230"
      - Grouping with at least 2 digits: "12,300" and "1230"
      - No grouping: "12300" and "1230"

      The exact grouping width is selected based on the locale.

      The default is to perform grouping according to locale data; most
      locales, but not all locales, enable it by default.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

  nfs_unf.def(
      "integer_width",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const IntegerWidth &style) { return self.integerWidth(style); },
      py::arg("style"), R"doc(
      Specify the minimum and maximum number of digits to render before the
      decimal mark.

      - Zero minimum integer digits: ".08"
      - One minimum integer digit: "0.08"
      - Two minimum integer digits: "00.08"

      The default is to have one minimum integer digit.
      )doc");

  nfs_unf.def(
      "notation",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const Notation &notation) { return self.notation(notation); },
      py::arg("notation"), R"doc(
      Specify the notation style (simple, scientific, or compact) for
      rendering numbers.

      - Simple notation: "12,300"
      - Scientific notation: "1.23E4"
      - Compact notation: "12K"

      The default is to use simple notation.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_unf.def(
      "per_unit",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const MeasureUnit &per_unit) { return self.perUnit(per_unit); },
      py::arg("per_unit"), R"doc(
      Set a unit to be used in the denominator.

      The default is not to display any unit in the denominator.

      If a per-unit is specified without a primary unit via unit, the behavior
      is undefined.

      Example:
          To format "3 m/s", pass METER as the unit and SECOND as the unit per:

          .. code-block:: python

              NumberFormatter.with_().unit(MeasureUnit.get_meter()).per_unit(MeasureUnit.get_second())
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_unf.def(
      "precision",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const Precision &precision) { return self.precision(precision); },
      py::arg("precision"), R"doc(
      Specify the rounding precision to use when formatting numbers.

      - Round to 3 decimal places: "3.142"
      - Round to 3 significant figures: "3.14"
      - Round to the closest nickel: "3.15"
      - Do not perform rounding: "3.1415926..."

      In most cases, the default rounding strategy is to round to 6 fraction
      places; i.e., Precision.max_fraction(6). The exceptions are if compact
      notation is being used, then the compact notation rounding strategy is
      used (see :meth:`Notation.compact_short` for details), or if the unit is
      a currency, then standard currency rounding is used, which varies from
      currency to currency (see :meth:`Precision.currency` for details).
      )doc");

  nfs_unf.def(
      "rounding_mode",
      [](const _UnlocalizedNumberFormatterSettings &self,
         UNumberFormatRoundingMode rounding_mode) {
        return self.roundingMode(rounding_mode);
      },
      py::arg("rounding_mode"), R"doc(
      Specify how to determine the direction to round a number when it has more
      digits than fit in the desired precision.

      When formatting 1.235:

      - Ceiling rounding mode with integer precision: "2"
      - Half-down rounding mode with 2 fixed fraction digits: "1.23"
      - Half-up rounding mode with 2 fixed fraction digits: "1.24"

      The default is HALF_EVEN. For more information, see the ICU User Guide:
      `Rounding Modes <https://unicode-org.github.io/icu/userguide/format_parse/numbers/rounding-modes>`__.
      )doc");

  nfs_unf.def(
      "scale",
      [](const _UnlocalizedNumberFormatterSettings &self, const Scale &scale) {
        return self.scale(scale);
      },
      py::arg("scale"), R"doc(
      Set a scale (multiplier) to be used to scale the number by an arbitrary
      amount before formatting.

      Most common values:

      - Multiply by 100: useful for percentages.
      - Multiply by an arbitrary value: useful for unit conversions.

      The default is to not apply any multiplier.
      )doc");

  nfs_unf.def(
      "sign",
      [](const _UnlocalizedNumberFormatterSettings &self,
         UNumberSignDisplay style) { return self.sign(style); },
      py::arg("style"), R"doc(
      Set the plus/minus sign display strategy.

      Most common values:

      - AUTO: "123", "-123"
      - ALWAYS: "+123", "-123"
      - ACCOUNTING: "$123", "($123)"

      The default is AUTO sign display.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  nfs_unf.def(
      "symbols",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const DecimalFormatSymbols &symbols) { return self.symbols(symbols); },
      py::arg("symbols"), R"doc(
      Specify the symbols (decimal separator, grouping separator, percent sign,
      numerals, etc.) to use when rendering numbers.

      - en_US symbols: "12,345.67"
      - fr_FR symbols: "12 345,67"
      - de_CH symbols: "12’345.67"
      - my_MY symbols: "၁၂,၃၄၅.၆၇"

      The default is to choose the symbols based on the locale specified in the
      fluent chain.

      .. note::

          :class:`~icupy.icu.DecimalFormatSymbols` automatically chooses the
          best numbering system based on the locale. In the examples above, the
          first three are using the Latin numbering system, and the fourth is
          using the Myanmar numbering system.

      .. note::

          An instance of :class:`~icupy.icu.DecimalFormatSymbols` will be
          copied: changes made to the symbols object after passing it into the
          fluent chain will not be seen.

      .. note::

          Calling this method will override any previously specified
          :class:`~icupy.icu.DecimalFormatSymbols` or
          :class:`~icupy.icu.NumberingSystem`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_unf.def(
      "to_skeleton",
      [](const _UnlocalizedNumberFormatterSettings &self) {
        ErrorCode error_code;
        auto result = self.toSkeleton(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Create a skeleton string representation of this number formatter.

      A skeleton string is a locale-agnostic serialized form of a number
      formatter.

      For more information, see the ICU User Guide:
      `Number Skeletons <https://unicode-org.github.io/icu/userguide/format_parse/numbers/skeletons.html>`__.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  nfs_unf.def(
      "unit",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const MeasureUnit &unit) { return self.unit(unit); },
      py::arg("unit"), R"doc(
      Specify the unit (unit of measure, currency, or percent) to associate
      with rendered numbers.

      - Unit of measure: "12.3 meters"
      - Currency: "$12.30"
      - Percent: "12.3%"

      The default is to render without units.

      .. note::

          If the :meth:`.usage` is set, the output unit will be changed to
          produce localised units, according to usage, locale and unit. See
          :meth:`FormattedNumber.get_output_unit`.
      )doc");

  nfs_unf.def(
      "unit_width",
      [](const _UnlocalizedNumberFormatterSettings &self,
         UNumberUnitWidth width) { return self.unitWidth(width); },
      py::arg("width"), R"doc(
      Set the width of the unit (measure unit or currency).

      Most common values:

      - SHORT: "$12.00", "12 m"
      - ISO_CODE: "USD 12.00"
      - FULL_NAME: "12.00 US dollars", "12 meters"

      The default is the SHORT width.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  nfs_unf.def(
      "usage",
      [](const _UnlocalizedNumberFormatterSettings &self,
         const std::string &usage) { return self.usage(usage); },
      py::arg("usage"), R"doc(
      Specify the usage for which numbers will be formatted ("person-height",
      "road", "rainfall", etc.).

      When a usage is specified, the output unit will change depending on the
      :class:`~icupy.icu.Locale` and the unit quantity. For example, formatting
      length measurements specified in meters:

      .. code-block:: python

          NumberFormatter.with_().usage("person").unit(MeasureUnit.get_meter()).locale("en-US")

          # When formatting 0.25, the output will be "10 inches".
          # When formatting 1.50, the output will be "4 feet and 11 inches".

      The input unit specified via :meth:`.unit` determines the type of
      measurement being formatted (e.g. "length" when the unit is "foot"). The
      usage requested will be looked for only within this category of
      measurement units.

      The output unit can be found via :meth:`FormattedNumber.get_output_unit`.

      If the usage has multiple parts (e.g. "land-agriculture-grain") and does
      not match a known usage preference, the last part will be dropped
      repeatedly until a match is found (e.g. trying "land-agriculture",
      then "land"). If a match is still not found, usage will fall back to
      "default".

      Setting usage to an empty string clears the usage (disables usage-based
      localized formatting).

      Setting a usage string but not a correct input unit will result in an
      :attr:`~icupy.icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR`.

      When using usage, specifying rounding or precision is unnecessary.
      Specifying a precision in some manner will override the default
      formatting.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  //
  // class icu::number::Notation
  //
  no.def_static("compact_long", &Notation::compactLong, R"doc(
      Print the number using long-form compact notation.

      In long form, the powers of ten are spelled out fully. Example outputs in
      en-US when printing 8.765E7 through 8.765E0:

      - 88 million
      - 8.8 million
      - 876 thousand
      - 88 thousand
      - 8.8 thousand
      - 876
      - 88
      - 8.8
      )doc");

  no.def_static("compact_short", &Notation::compactShort, R"doc(
      Print the number using short-form compact notation.

      Compact notation, defined in Unicode Technical Standard #35 Part 3
      Section 2.4.1, prints numbers with localized prefixes or suffixes
      corresponding to different powers of ten. Compact notation is similar to
      engineering notation in how it scales numbers.

      Compact notation is ideal for displaying large numbers (over ~1000) to
      humans while at the same time minimizing screen real estate.

      In short form, the powers of ten are abbreviated. In en-US, the
      abbreviations are "K" for thousands, "M" for millions, "B" for billions,
      and "T" for trillions. Example outputs in en-US when printing 8.765E7
      through 8.765E0:

      - 88M
      - 8.8M
      - 876K
      - 88K
      - 8.8K
      - 876
      - 88
      - 8.8

      .. note::

          When compact notation is specified without an explicit rounding
          precision, numbers are rounded off to the closest integer after
          scaling the number by the corresponding power of 10, but with a digit
          shown after the decimal separator if there is only one digit before
          the decimal separator. The default compact notation rounding precision
          is equivalent to:

          .. code-block:: python

              Precision.integer().with_min_digits(2)
      )doc");

  no.def_static("engineering", &Notation::engineering, R"doc(
      Print the number using engineering notation, a variant of scientific
      notation in which the exponent must be divisible by 3.

      Example outputs in en-US when printing 8.765E4 through 8.765E-3:

      - 87.65E3
      - 8.765E3
      - 876.5E0
      - 87.65E0
      - 8.765E0
      - 876.5E-3
      - 87.65E-3
      - 8.765E-3
      - 0E0
      )doc");

  no.def_static("scientific", &Notation::scientific, R"doc(
      Print the number using scientific notation.

      Example outputs in en-US when printing 8.765E4 through 8.765E-3:

      - 8.765E4
      - 8.765E3
      - 8.765E2
      - 8.765E1
      - 8.765E0
      - 8.765E-1
      - 8.765E-2
      - 8.765E-3
      - 0E0
      )doc");

  no.def_static("simple", &Notation::simple, R"doc(
      Print the number using simple notation without any scaling by powers of
      ten.

      This is the default behavior.

      Since this is the default behavior, this method needs to be called only
      when it is necessary to override a previous setting.

      Example outputs in en-US when printing 8.765E7 through 8.765E0:

      - 87,650,000
      - 8,765,000
      - 876,500
      - 87,650
      - 8,765
      - 876.5
      - 87.65
      - 8.765
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::Precision
  //
  pre.def_static("currency", &Precision::currency, py::arg("currency_usage"),
                 R"doc(
      Show numbers rounded and padded according to the rules for the currency
      unit.

      The most common rounding precision settings for currencies include
      ``Precision.fixed_fraction(2)``, ``Precision.integer()``, and
      ``Precision.increment(0.05)`` for cash transactions ("nickel rounding").
      )doc");

  pre.def_static("fixed_fraction", &Precision::fixedFraction,
                 py::arg("min_max_fraction_places"), R"doc(
      Show numbers rounded if necessary to a certain number of fraction places
      (numerals after the decimal separator).

      Additionally, pad with zeros to ensure that this number of places are
      always shown.

      Example output with *min_max_fraction_places=3*:

      - 87,650.000
      - 8,765.000
      - 876.500
      - 87.650
      - 8.765
      - 0.876
      - 0.088
      - 0.009
      - 0.000 (zero)

      This method is equivalent to :meth:`.min_max_fraction` with both
      arguments equal.
      )doc");

  pre.def_static("fixed_significant_digits", &Precision::fixedSignificantDigits,
                 py::arg("min_max_significant_digits"), R"doc(
      Show numbers rounded if necessary to a certain number of significant
      digits or significant figures.

      Additionally, pad with zeros to ensure that this number of significant
      digits/figures are always shown.

      This method is equivalent to :meth:`.min_max_significant_digits` with
      both arguments equal.
      )doc");

  pre.def_static("increment", &Precision::increment,
                 py::arg("rounding_increment"), R"doc(
      Show numbers rounded if necessary to the closest multiple of a certain
      rounding increment.

      For example, if the rounding increment is 0.5, then round 1.2 to 1 and
      round 1.3 to 1.5.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 71)
  pre.def_static("increment_exact", &Precision::incrementExact,
                 py::arg("mantissa"), py::arg("magnitude"), R"doc(
      Show numbers rounded if necessary to the closest multiple of a certain
      rounding increment specified as an integer at a particular power of 10.

      For example, to round to the nearest 0.5 and display 2 fraction digits,
      with this function, you should write one of the following:

      - ``Precision.increment_exact(5, -1).with_min_fraction(2)``
      - ``Precision.increment_exact(50, -2).with_min_fraction(2)``
      - ``Precision.increment_exact(50, -2)``

      This is analagous to ICU4J:

      .. code-block::

          Precision.increment(new BigDecimal("0.50"))
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)

  pre.def_static("integer", &Precision::integer, R"doc(
      Show numbers rounded if necessary to the nearest integer.
      )doc");

  pre.def_static("max_fraction", &Precision::maxFraction,
                 py::arg("max_fraction_places"), R"doc(
      Show numbers rounded if necessary to a certain number of fraction places
      (numerals after the decimal separator).

      Unlike the other fraction rounding strategies, this strategy does not pad
      zeros to the end of the number.
      )doc");

  pre.def_static("max_significant_digits", &Precision::maxSignificantDigits,
                 py::arg("max_significant_digits"), R"doc(
      Show numbers rounded if necessary to a certain number of significant
      digits/figures.
      )doc");

  pre.def_static("min_fraction", &Precision::minFraction,
                 py::arg("min_fraction_places"), R"doc(
      Always show at least a certain number of fraction places after the
      decimal separator, padding with zeros if necessary.

      Do not perform rounding (display numbers to their full precision).

      .. note::
         If you are formatting doubles, see the performance note in
         :meth:`.unlimited`.
      )doc");

  pre.def_static("min_max_fraction", &Precision::minMaxFraction,
                 py::arg("min_fraction_places"), py::arg("max_fraction_places"),
                 R"doc(
      Show numbers rounded if necessary to a certain number of fraction places
      (numerals after the decimal separator); in addition, always show at least
      a certain number of places after the decimal separator, padding with
      zeros if necessary.
      )doc");

  pre.def_static("min_max_significant_digits",
                 &Precision::minMaxSignificantDigits,
                 py::arg("min_significant_digits"),
                 py::arg("max_significant_digits"), R"doc(
        Show numbers rounded if necessary to a certain number of significant
        digits/figures; in addition, always show at least a certain number of
        significant digits, padding with zeros if necessary.
        )doc");

  pre.def_static("min_significant_digits", &Precision::minSignificantDigits,
                 py::arg("min_significant_digits"), R"doc(
      Always show at least a certain number of significant digits/figures,
      padding with zeros if necessary.

      Do not perform rounding (display numbers to their full precision).

      .. note::
         If you are formatting doubles, see the performance note in
         :meth:`.unlimited`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  pre.def("trailing_zero_display", &Precision::trailingZeroDisplay,
          py::arg("trailing_zero_display"), R"doc(
      Configure how trailing zeros are displayed on numbers.

      For example, to hide trailing zeros when the number is an integer, use
      :attr:`~icupy.icu.UNumberTrailingZeroDisplay.UNUM_TRAILING_ZERO_HIDE_IF_WHOLE`.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  pre.def_static("unlimited", &Precision::unlimited, R"doc(
      Show all available digits to full precision.

      .. note::
         When formatting a double, this method, along with
         :meth:`.min_fraction` and :meth:`.min_significant_digits`, will
         trigger complex algorithm similar to Dragon4 to determine the
         low-order digits and the number of digits to display based on the
         value of the double. If the number of fraction places or significant
         digits can be bounded, consider using :meth:`max_fraction` or
         :meth:`max_significant_digits` instead to maximize performance.
         For more information, read the following blog post:
         http://www.serpentine.com/blog/2011/06/29/here-be-dragons-advances-in-problems-you-didnt-even-know-you-had/
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  //
  // class icu::number::Scale
  //
  sc.def(py::init<const Scale &>(), py::arg("other"), R"doc(
      Initialize a ``Scale`` instance from another ``Scale``.
      )doc");

  sc.def_static(
      "by_decimal",
      [](const std::string &multiplicand) {
        return Scale::byDecimal(multiplicand);
      },
      py::arg("multiplicand"), R"doc(
      Multiply numbers by an arbitrary value before formatting.

      Useful for unit conversions.

      The syntax of *multiplicand* is a "numeric string" as defined in the
      `Decimal Arithmetic Specification <http://speleotrove.com/decimal>`__.
      )doc");

  sc.def_static("by_double", &Scale::byDouble, py::arg("multiplicand"), R"doc(
      Multiply numbers by an arbitrary value before formatting.

      Useful for unit conversions.
      )doc");

  sc.def_static("by_double_and_power_of_ten", &Scale::byDoubleAndPowerOfTen,
                py::arg("multiplicand"), py::arg("power"), R"doc(
      Multiply a number by both a power of ten and by an arbitrary double
      value.
      )doc");

  sc.def_static("none", &Scale::none, R"doc(
      Do not change the value of numbers when formatting or parsing.
      )doc");

  sc.def_static("power_of_ten", &Scale::powerOfTen, py::arg("power"), R"doc(
      Multiply numbers by a power of ten before formatting.

      Useful for combining with a percent unit.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  //
  // class icu::number::ScientificNotation
  //
  sn.def("with_exponent_sign_display",
         &ScientificNotation::withExponentSignDisplay,
         py::arg("exponent_sign_display"), R"doc(
         Sets whether to show the sign on positive and negative exponents in
         scientific notation.

         The default is AUTO, showing the minus sign but not the plus sign.

         For example, with *exponent_sign_display=ALWAYS*, the number 123 will
         be printed as "1.23E+2" in en-US instead of the default "1.23E2".
         )doc");

  sn.def("with_min_exponent_digits", &ScientificNotation::withMinExponentDigits,
         py::arg("min_exponent_digits"), R"doc(
         Sets the minimum number of digits to show in the exponent of
         scientific notation, padding with zeros if necessary.

         Useful for fixed-width display.

         For example, with *min_exponent_digits=2*, the number 123 will be
         printed as "1.23E02" in en-US instead of the default "1.23E2".
         )doc");

  //
  // class icu::number::UnlocalizedNumberFormatter
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  unf.def(py::init<>(), R"doc(
      Initialize a ``UnlocalizedNumberFormatter`` instance in a valid but
      undefined state.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  unf.def(py::init<const UnlocalizedNumberFormatter &>(), py::arg("other"),
          R"doc(
      Initialize a ``UnlocalizedNumberFormatter`` instance from another
      ``UnlocalizedNumberFormatter``.
      )doc");

  unf.def(
      "locale",
      [](const UnlocalizedNumberFormatter &self,
         const icupy::LocaleVariant &locale) {
        return self.locale(icupy::to_locale(locale));
      },
      py::arg("locale"), R"doc(
      Associate the given locale with the number formatter.

      The locale is used for picking the appropriate symbols, formats, and
      other data for number display.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
}

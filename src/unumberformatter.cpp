#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
#include <unicode/unumberformatter.h>
#else
#include <unicode/numberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

void init_unumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  //
  // enum UNumberDecimalSeparatorDisplay
  //
  py::enum_<UNumberDecimalSeparatorDisplay>(m, "UNumberDecimalSeparatorDisplay",
                                            py::arithmetic(), R"doc(
Enum declaring how to render the decimal separator.

- AUTO: "1", "1.1"
- ALWAYS: "1.", "1.1"
      )doc")
      .value("UNUM_DECIMAL_SEPARATOR_AUTO", UNUM_DECIMAL_SEPARATOR_AUTO, R"doc(
             Show the decimal separator when there are one or more digits to
             display after the separator, and do not show it otherwise.

             This is the default behavior.
             )doc")
      .value("UNUM_DECIMAL_SEPARATOR_ALWAYS", UNUM_DECIMAL_SEPARATOR_ALWAYS,
             R"doc(
             Always show the decimal separator, even if there are no digits
             to display after the separator.
             )doc")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  //
  // enum UNumberGroupingStrategy
  //
  py::enum_<UNumberGroupingStrategy>(m, "UNumberGroupingStrategy",
                                     py::arithmetic(), R"doc(
Enum declaring the strategy for when and how to display grouping separators
(i.e., the separator, often a comma or period, after every 2-3 powers of ten).

The choices are several pre-built strategies for different use cases that
employ locale data whenever possible. Example outputs for 1234 and 1234567 in
*en-IN*:

- OFF: 1234 and 12345
- MIN2: 1234 and 12,34,567
- AUTO: 1,234 and 12,34,567
- ON_ALIGNED: 1,234 and 12,34,567
- THOUSANDS: 1,234 and 1,234,567

The default is UNUM_GROUPING_AUTO, which displays grouping separators unless
the locale data says that grouping is not customary. To force grouping for all
numbers greater than 1000 consistently across locales, use
UNUM_GROUPING_ON_ALIGNED. On the other hand, to display grouping less
frequently than the default, use UNUM_GROUPING_MIN2 or UNUM_GROUPING_OFF. See
the docs of each option for details.

.. note::

    This enum specifies the strategy for grouping sizes. To set which character
    to use as the grouping separator, use the "symbols" setter.
      )doc")
      .value("UNUM_GROUPING_OFF", UNUM_GROUPING_OFF, R"doc(
             Do not display grouping separators in any locale.
             )doc")
      .value("UNUM_GROUPING_MIN2", UNUM_GROUPING_MIN2, R"doc(
             Display grouping using locale defaults, except do not show
             grouping on values smaller than 10000 (such that there is a
             *minimum of two digits* before the first separator).

             Note that locales may restrict grouping separators to be displayed
             only on 1 million or greater (for example, ee and hu) or disable
             grouping altogether (for example, bg currency).

             Locale data is used to determine whether to separate larger
             numbers into groups of 2 (customary in South Asia) or groups of 3
             (customary in Europe and the Americas).
             )doc")
      .value("UNUM_GROUPING_AUTO", UNUM_GROUPING_AUTO, R"doc(
             Display grouping using the default strategy for all locales.

             This is the default behavior.

             Note that locales may restrict grouping separators to be displayed
             only on 1 million or greater (for example, ee and hu) or disable
             grouping altogether (for example, bg currency).

             Locale data is used to determine whether to separate larger
             numbers into groups of 2 (customary in South Asia) or groups of 3
             (customary in Europe and the Americas).
             )doc")
      .value("UNUM_GROUPING_ON_ALIGNED", UNUM_GROUPING_ON_ALIGNED, R"doc(
             Always display the grouping separator on values of at least 1000.

             This option ignores the locale data that restricts or disables
             grouping, described in :attr:`UNUM_GROUPING_MIN2` and
             :attr:`UNUM_GROUPING_AUTO`.
             This option may be useful to normalize the alignment of numbers,
             such as in a spreadsheet.

             Locale data is used to determine whether to separate larger
             numbers into groups of 2 (customary in South Asia) or groups of 3
             (customary in Europe and the Americas).
             )doc")
      .value("UNUM_GROUPING_THOUSANDS", UNUM_GROUPING_THOUSANDS, R"doc(
             Use the Western defaults: groups of 3 and enabled for all numbers
             1000 or greater.

             Do not use locale data for determining the grouping strategy.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  //
  // enum UNumberRoundingPriority
  //
  py::enum_<UNumberRoundingPriority>(m, "UNumberRoundingPriority",
                                     py::arithmetic(), R"doc(
Enum declaring how to resolve conflicts between maximum fraction digits and
maximum significant digits.

There are two modes, RELAXED and STRICT:

- RELAXED: Relax one of the two constraints (fraction digits or significant
  digits) in order to round the number to a higher level of precision.
- STRICT: Enforce both constraints, resulting in the number being rounded to a
  lower level of precision.

The default settings for compact notation rounding are Max-Fraction = 0
(round to the nearest integer), Max-Significant = 2 (round to 2 significant
digits), and priority RELAXED (choose the constraint that results in more
digits being displayed).

Conflicting minimum fraction and significant digits are always resolved in the
direction that results in more trailing zeros.

Example 1: Consider the number 3.141, with various different settings:

- Max-Fraction = 1: "3.1"
- Max-Significant = 3: "3.14"

The rounding priority determines how to resolve the conflict when both
Max-Fraction and Max-Significant are set. With RELAXED, the less-strict setting
(the one that causes more digits to be displayed) will be used;
Max-Significant wins. With STRICT, the more-strict setting (the one that causes
fewer digits to be displayed) will be used; Max-Fraction wins.

Example 2: Consider the number 8317, with various different settings:

- Max-Fraction = 1: "8317"
- Max-Significant = 3: "8320"

Here, RELAXED favors Max-Fraction and STRICT favors Max-Significant.
Note that this larger number caused the two modes to favor the opposite result.
      )doc")
      .value("UNUM_ROUNDING_PRIORITY_RELAXED", UNUM_ROUNDING_PRIORITY_RELAXED,
             R"doc(
             Favor greater precision by relaxing one of the rounding
             constraints.
             )doc")
      .value("UNUM_ROUNDING_PRIORITY_STRICT", UNUM_ROUNDING_PRIORITY_STRICT,
             R"doc(
             Favor adherence to all rounding constraints by producing lower
             precision.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  //
  // enum UNumberSignDisplay
  //
  py::enum_<UNumberSignDisplay>(m, "UNumberSignDisplay", py::arithmetic(),
                                R"doc(
Enum declaring how to denote positive and negative numbers.

Example outputs when formatting 123, 0, and -123 in *en-US*:

- AUTO: "123", "0", and "-123"
- ALWAYS: "+123", "+0", and "-123"
- NEVER: "123", "0", and "123"
- ACCOUNTING: "$123", "$0", and "($123)"
- ACCOUNTING_ALWAYS: "+$123", "+$0", and "($123)"
- EXCEPT_ZERO: "+123", "0", and "-123"
- ACCOUNTING_EXCEPT_ZERO: "+$123", "$0", and "($123)"

The exact format, including the position and the code point of the sign, differ
by locale.
      )doc")
      .value("UNUM_SIGN_AUTO", UNUM_SIGN_AUTO, R"doc(
             Show the minus sign on negative numbers, and do not show the sign
             on positive numbers.

             This is the default behavior.

             If using this option, a sign will be displayed on negative zero,
             including negative numbers that round to zero. To hide the sign
             on negative zero, use the :attr:`UNUM_SIGN_NEGATIVE` option.
             )doc")
      .value("UNUM_SIGN_ALWAYS", UNUM_SIGN_ALWAYS, R"doc(
             Show the minus sign on negative numbers and the plus sign on
             positive numbers, including zero.

             To hide the sign on zero, see :attr:`UNUM_SIGN_EXCEPT_ZERO`.
             )doc")
      .value("UNUM_SIGN_NEVER", UNUM_SIGN_NEVER, R"doc(
             Do not show the sign on positive or negative numbers.
             )doc")
      .value("UNUM_SIGN_ACCOUNTING", UNUM_SIGN_ACCOUNTING, R"doc(
             Use the locale-dependent accounting format on negative numbers,
             and do not show the sign on positive numbers.

             The accounting format is defined in CLDR and varies by locale;
             in many Western locales, the format is a pair of parentheses
             around the number.

             .. note::

                 Since CLDR defines the accounting format in the monetary
                 context only, this option falls back to the
                 :attr:`UNUM_SIGN_AUTO` sign display strategy when formatting
                 without a currency unit. This limitation may be lifted in the
                 future.
             )doc")
      .value("UNUM_SIGN_ACCOUNTING_ALWAYS", UNUM_SIGN_ACCOUNTING_ALWAYS, R"doc(
             Use the locale-dependent accounting format on negative numbers,
             and show the plus sign on positive numbers, including zero.

             For more information on the accounting format, see the
             :attr:`UNUM_SIGN_ACCOUNTING` sign display strategy. To hide the
             sign on zero, see :attr:`UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO`.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("UNUM_SIGN_EXCEPT_ZERO", UNUM_SIGN_EXCEPT_ZERO, R"doc(
             Show the minus sign on negative numbers and the plus sign on
             positive numbers.

             Do not show a sign on zero, numbers that round to zero, or NaN.
             )doc")
      .value("UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO",
             UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO, R"doc(
             Use the locale-dependent accounting format on negative numbers,
             and show the plus sign on positive numbers.

             Do not show a sign on zero, numbers that round to zero, or NaN.
             For more information on the accounting format, see the
             :attr:`UNUM_SIGN_ACCOUNTING` sign display strategy.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UNUM_SIGN_NEGATIVE", UNUM_SIGN_NEGATIVE, R"doc(
             Same as :attr:`UNUM_SIGN_AUTO`, but do not show the sign on
             negative zero.
             )doc")
      .value("UNUM_SIGN_ACCOUNTING_NEGATIVE", UNUM_SIGN_ACCOUNTING_NEGATIVE,
             R"doc(
             Same as :attr:`UNUM_SIGN_ACCOUNTING`, but do not show the sign on
             negative zero.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  //
  // enum UNumberTrailingZeroDisplay
  //
  py::enum_<UNumberTrailingZeroDisplay>(m, "UNumberTrailingZeroDisplay",
                                        py::arithmetic(), R"doc(
Enum declaring how to render trailing zeros.

- AUTO: 0.90, 1.00, 1.10
- HIDE_IF_WHOLE: 0.90, 1, 1.10
      )doc")
      .value("UNUM_TRAILING_ZERO_AUTO", UNUM_TRAILING_ZERO_AUTO, R"doc(
             Display trailing zeros according to the settings for minimum
             fraction and significant digits.
             )doc")
      .value("UNUM_TRAILING_ZERO_HIDE_IF_WHOLE",
             UNUM_TRAILING_ZERO_HIDE_IF_WHOLE, R"doc(
             Same as :attr:`UNUM_TRAILING_ZERO_AUTO`, but hide trailing zeros
             after the decimal separator if they are all zero.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  //
  // enum UNumberUnitWidth
  //
  py::enum_<UNumberUnitWidth>(m, "UNumberUnitWidth", py::arithmetic(), R"doc(
Enum declaring how to render units, including currencies.

Example outputs when formatting 123 USD and 123 meters in *en-CA*:

- NARROW: "$123.00" and "123 m"
- SHORT: "US$ 123.00" and "123 m"
- FULL_NAME: "123.00 US dollars" and "123 meters"
- ISO_CODE: "USD 123.00" and undefined behavior
- HIDDEN: "123.00" and "123"

This enum is similar to :class:`UMeasureFormatWidth`.
      )doc")
      .value("UNUM_UNIT_WIDTH_NARROW", UNUM_UNIT_WIDTH_NARROW, R"doc(
             Print an abbreviated version of the unit name.

             Similar to :attr:`UNUM_UNIT_WIDTH_SHORT`, but always use the
             shortest available abbreviation or symbol. This option can be
             used when the context hints at the identity of the unit. For more
             information on the difference between NARROW and SHORT, see
             :attr:`UNUM_UNIT_WIDTH_SHORT`.

             In CLDR, this option corresponds to the "Narrow" format for
             measure units and the "¤¤¤¤¤" placeholder for currencies.
             )doc")
      .value("UNUM_UNIT_WIDTH_SHORT", UNUM_UNIT_WIDTH_SHORT, R"doc(
             Print an abbreviated version of the unit name.

             Similar to :attr:`UNUM_UNIT_WIDTH_NARROW`, but use a slightly
             wider abbreviation or symbol when there may be ambiguity. This is
             the default behavior.

             For example, in *es-US*, the :attr:`UNUM_UNIT_WIDTH_SHORT` form
             for Fahrenheit is "{0} °F\", but the
             :attr:`UNUM_UNIT_WIDTH_NARROW` form is "{0}°", since Fahrenheit is
             the customary unit for temperature in that locale.

             In CLDR, this option corresponds to the "Short" format for measure
             units and the "¤" placeholder for currencies.
             )doc")
      .value("UNUM_UNIT_WIDTH_FULL_NAME", UNUM_UNIT_WIDTH_FULL_NAME, R"doc(
             Print the full name of the unit, without any abbreviations.

             In CLDR, this option corresponds to the default format for measure
             units and the "¤¤¤" placeholder for currencies.
             )doc")
      .value("UNUM_UNIT_WIDTH_ISO_CODE", UNUM_UNIT_WIDTH_ISO_CODE, R"doc(
             Use the three-digit ISO XXX code in place of the symbol for
             displaying currencies.

             The behavior of this option is currently undefined for use with
             measure units.

             In CLDR, this option corresponds to the "¤¤" placeholder for
             currencies.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UNUM_UNIT_WIDTH_FORMAL", UNUM_UNIT_WIDTH_FORMAL, R"doc(
             Use the formal variant of the currency symbol; for example,
             "NT$" for the New Taiwan dollar in zh-TW.

             Behavior of this option with non-currency units is not defined at
             this time.
             )doc")
      .value("UNUM_UNIT_WIDTH_VARIANT", UNUM_UNIT_WIDTH_VARIANT, R"doc(
             Use the alternate variant of the currency symbol; for example,
             "TL" for the Turkish lira (TRY).

             Behavior of this option with non-currency units is not defined
             at this time.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UNUM_UNIT_WIDTH_HIDDEN", UNUM_UNIT_WIDTH_HIDDEN, R"doc(
             Format the number according to the specified unit, but do not
             display the unit.

             For currencies, apply monetary symbols and formats as with
             :attr:`UNUM_UNIT_WIDTH_SHORT`, but omit the currency symbol. For
             measure units, the behavior is equivalent to not specifying the
             unit at all.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
}

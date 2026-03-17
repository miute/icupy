#include "main.hpp"
#include <unicode/unum.h>

void init_unum(py::module &m) {
  //
  // enum UCurrencySpacing
  //
  py::enum_<UCurrencySpacing>(m, "UCurrencySpacing", py::arithmetic(),
                              "Constants for specifying currency spacing.")
      .value("UNUM_CURRENCY_MATCH", UNUM_CURRENCY_MATCH)
      .value("UNUM_CURRENCY_SURROUNDING_MATCH", UNUM_CURRENCY_SURROUNDING_MATCH)
      .value("UNUM_CURRENCY_INSERT", UNUM_CURRENCY_INSERT)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  //
  // enum UNumberCompactStyle
  //
  py::enum_<UNumberCompactStyle>(
      m, "UNumberCompactStyle", py::arithmetic(),
      "Constants for specifying short or long format.")
      .value("UNUM_SHORT", UNUM_SHORT)
      .value("UNUM_LONG", UNUM_LONG)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  //
  // enum UNumberFormatAttribute
  //
  py::enum_<UNumberFormatAttribute>(
      m, "UNumberFormatAttribute", py::arithmetic(),
      "Possible :class:`NumberFormat` numeric attributes.")
      .value("UNUM_PARSE_INT_ONLY", UNUM_PARSE_INT_ONLY, R"doc(
             Parse integers only.
             )doc")
      .value("UNUM_GROUPING_USED", UNUM_GROUPING_USED, R"doc(
             Use grouping separator.
             )doc")
      .value("UNUM_DECIMAL_ALWAYS_SHOWN", UNUM_DECIMAL_ALWAYS_SHOWN, R"doc(
             Always show decimal point.
             )doc")
      .value("UNUM_MAX_INTEGER_DIGITS", UNUM_MAX_INTEGER_DIGITS, R"doc(
             Maximum integer digits.
             )doc")
      .value("UNUM_MIN_INTEGER_DIGITS", UNUM_MIN_INTEGER_DIGITS, R"doc(
             Minimum integer digits.
             )doc")
      .value("UNUM_INTEGER_DIGITS", UNUM_INTEGER_DIGITS, R"doc(
             Integer digits.
             )doc")
      .value("UNUM_MAX_FRACTION_DIGITS", UNUM_MAX_FRACTION_DIGITS, R"doc(
             Maximum fraction digits.
             )doc")
      .value("UNUM_MIN_FRACTION_DIGITS", UNUM_MIN_FRACTION_DIGITS, R"doc(
             Minimum fraction digits.
             )doc")
      .value("UNUM_FRACTION_DIGITS", UNUM_FRACTION_DIGITS, R"doc(
             Fraction digits.
             )doc")
      .value("UNUM_MULTIPLIER", UNUM_MULTIPLIER, R"doc(
             Multiplier.
             )doc")
      .value("UNUM_GROUPING_SIZE", UNUM_GROUPING_SIZE, R"doc(
             Grouping size.
             )doc")
      .value("UNUM_ROUNDING_MODE", UNUM_ROUNDING_MODE, R"doc(
             Rounding Mode.
             )doc")
      .value("UNUM_ROUNDING_INCREMENT", UNUM_ROUNDING_INCREMENT, R"doc(
             Rounding increment.
             )doc")
      .value("UNUM_FORMAT_WIDTH", UNUM_FORMAT_WIDTH, R"doc(
             The width to which the output of ``format()`` is padded.
             )doc")
      .value("UNUM_PADDING_POSITION", UNUM_PADDING_POSITION, R"doc(
             The position at which padding will take place.
             )doc")
      .value("UNUM_SECONDARY_GROUPING_SIZE", UNUM_SECONDARY_GROUPING_SIZE,
             R"doc(
             Secondary grouping size.
             )doc")
      .value("UNUM_SIGNIFICANT_DIGITS_USED", UNUM_SIGNIFICANT_DIGITS_USED,
             R"doc(
             Use significant digits.
             )doc")
      .value("UNUM_MIN_SIGNIFICANT_DIGITS", UNUM_MIN_SIGNIFICANT_DIGITS, R"doc(
             Minimum significant digits.
             )doc")
      .value("UNUM_MAX_SIGNIFICANT_DIGITS", UNUM_MAX_SIGNIFICANT_DIGITS, R"doc(
             Maximum significant digits.
             )doc")
      .value("UNUM_LENIENT_PARSE", UNUM_LENIENT_PARSE, R"doc(
             Lenient parse mode used by rule-based formats.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UNUM_SCALE", UNUM_SCALE, R"doc(
             Scale, which adjusts the position of the decimal point when
             formatting.

             Amounts will be multiplied by 10 ^ (scale) before they are
             formatted. The default value for the scale is 0 (no adjustment).

             Example: setting the scale to 3, 123 formats as "123,000"

             Example: setting the scale to -4, 123 formats as "0.0123"

             This setting is analogous to
             :meth:`DecimalFormat.get_multiplier_scale` and
             :meth:`DecimalFormat.set_multiplier_scale` in decimfmt.h.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_MINIMUM_GROUPING_DIGITS", UNUM_MINIMUM_GROUPING_DIGITS,
             R"doc(
             Minimum grouping digits; most commonly set to 2 to print "1000"
             instead of "1,000".

             See :meth:`DecimalFormat.get_minimum_grouping_digits`.

             For better control over grouping strategies, use
             :class:`number.NumberFormatter`.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_CURRENCY_USAGE", UNUM_CURRENCY_USAGE, R"doc(
             if this attribute is set to 0, it is set to
             UNUM_CURRENCY_STANDARD purpose, otherwise it is
             UNUM_CASH_CURRENCY purpose Default: 0
             (UNUM_CURRENCY_STANDARD purpose)
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS",
             UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS, R"doc(
             If 1, specifies that if setting the "max integer digits"
             attribute would truncate a value, set an error status rather than
             silently truncating.

             For example, formatting the value 1234 with 4 max int digits
             would succeed, but formatting 12345 would fail. There is no effect
             on parsing. Default: 0 (not set)
             )doc")
      .value("UNUM_PARSE_NO_EXPONENT", UNUM_PARSE_NO_EXPONENT, R"doc(
             if this attribute is set to 1, specifies that, if the pattern
             doesn't contain an exponent, the exponent will not be parsed.

             If the pattern does contain an exponent, this attribute has no
             effect. Has no effect on formatting. Default: 0 (unset)
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_PARSE_DECIMAL_MARK_REQUIRED",
             UNUM_PARSE_DECIMAL_MARK_REQUIRED, R"doc(
             if this attribute is set to 1, specifies that, if the pattern
             contains a decimal mark the input is required to have one.

             If this attribute is set to 0, specifies that input does not
             have to contain a decimal mark. Has no effect on formatting.
             Default: 0 (unset)
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_PARSE_CASE_SENSITIVE", UNUM_PARSE_CASE_SENSITIVE, R"doc(
             Parsing: if set to 1, parsing is sensitive to case
             (lowercase/uppercase).
             )doc")
      .value("UNUM_SIGN_ALWAYS_SHOWN", UNUM_SIGN_ALWAYS_SHOWN, R"doc(
             Formatting: if set to 1, whether to show the plus sign on
             non-negative numbers.

             For better control over sign display, use
             :class:`number.NumberFormatter`.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  //
  // enum UNumberFormatFields
  //
  py::enum_<UNumberFormatFields>(m, "UNumberFormatFields", py::arithmetic(),
                                 R"doc(
:class:`FieldPosition` selector for format fields defined by
:class:`NumberFormat`.
      )doc")
      .value("UNUM_INTEGER_FIELD", UNUM_INTEGER_FIELD)
      .value("UNUM_FRACTION_FIELD", UNUM_FRACTION_FIELD)
      .value("UNUM_DECIMAL_SEPARATOR_FIELD", UNUM_DECIMAL_SEPARATOR_FIELD)
      .value("UNUM_EXPONENT_SYMBOL_FIELD", UNUM_EXPONENT_SYMBOL_FIELD)
      .value("UNUM_EXPONENT_SIGN_FIELD", UNUM_EXPONENT_SIGN_FIELD)
      .value("UNUM_EXPONENT_FIELD", UNUM_EXPONENT_FIELD)
      .value("UNUM_GROUPING_SEPARATOR_FIELD", UNUM_GROUPING_SEPARATOR_FIELD)
      .value("UNUM_CURRENCY_FIELD", UNUM_CURRENCY_FIELD)
      .value("UNUM_PERCENT_FIELD", UNUM_PERCENT_FIELD)
      .value("UNUM_PERMILL_FIELD", UNUM_PERMILL_FIELD)
      .value("UNUM_SIGN_FIELD", UNUM_SIGN_FIELD)
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_MEASURE_UNIT_FIELD", UNUM_MEASURE_UNIT_FIELD)
      .value("UNUM_COMPACT_FIELD", UNUM_COMPACT_FIELD)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
#if (U_ICU_VERSION_MAJOR_NUM >= 71)
      .value("UNUM_APPROXIMATELY_SIGN_FIELD", UNUM_APPROXIMATELY_SIGN_FIELD,
             R"doc(
             Approximately sign. In ICU 70, this was categorized under the
             generic SIGN field.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  //
  // enum UNumberFormatMinimumGroupingDigits
  //
  py::enum_<UNumberFormatMinimumGroupingDigits>(
      m, "UNumberFormatMinimumGroupingDigits", py::arithmetic(), R"doc(
Selectors with special numeric values to use locale default minimum grouping
digits for the :class:`DecimalFormat.set_minimum_grouping_digits` method.

Do not use these constants with the :class:`number.NumberFormatter` API.
      )doc")
      .value("UNUM_MINIMUM_GROUPING_DIGITS_AUTO",
             UNUM_MINIMUM_GROUPING_DIGITS_AUTO, R"doc(
             Display grouping using the default strategy for all locales.
             )doc")
      .value("UNUM_MINIMUM_GROUPING_DIGITS_MIN2",
             UNUM_MINIMUM_GROUPING_DIGITS_MIN2, R"doc(
             Display grouping using locale defaults, except do not show grouping
             on values smaller than 10000 (such that there is a minimum of two
             digits before the first separator).
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  //
  // enum UNumberFormatPadPosition
  //
  py::enum_<UNumberFormatPadPosition>(m, "UNumberFormatPadPosition",
                                      py::arithmetic(),
                                      "Possible number format pad positions.")
      .value("UNUM_PAD_BEFORE_PREFIX", UNUM_PAD_BEFORE_PREFIX)
      .value("UNUM_PAD_AFTER_PREFIX", UNUM_PAD_AFTER_PREFIX)
      .value("UNUM_PAD_BEFORE_SUFFIX", UNUM_PAD_BEFORE_SUFFIX)
      .value("UNUM_PAD_AFTER_SUFFIX", UNUM_PAD_AFTER_SUFFIX)
      .export_values();

  //
  // enum UNumberFormatRoundingMode
  //
  py::enum_<UNumberFormatRoundingMode>(m, "UNumberFormatRoundingMode",
                                       py::arithmetic(), R"doc(
Possible number format rounding modes.

For more detail on rounding modes, see:
https://unicode-org.github.io/icu/userguide/format_parse/numbers/rounding-modes
      )doc")
      .value("UNUM_ROUND_CEILING", UNUM_ROUND_CEILING)
      .value("UNUM_ROUND_FLOOR", UNUM_ROUND_FLOOR)
      .value("UNUM_ROUND_DOWN", UNUM_ROUND_DOWN)
      .value("UNUM_ROUND_UP", UNUM_ROUND_UP)
      .value("UNUM_ROUND_HALFEVEN", UNUM_ROUND_HALFEVEN, R"doc(
             Half-even rounding.
             )doc")
      .value("UNUM_ROUND_HALFDOWN", UNUM_ROUND_HALFDOWN)
      .value("UNUM_ROUND_HALFUP", UNUM_ROUND_HALFUP)
      .value("UNUM_ROUND_UNNECESSARY", UNUM_ROUND_UNNECESSARY, R"doc(
             UNUM_ROUND_UNNECESSARY reports an error if formatted result is
             not exact.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UNUM_ROUND_HALF_ODD", UNUM_ROUND_HALF_ODD, R"doc(
             Rounds ties toward the odd number.
             )doc")
      .value("UNUM_ROUND_HALF_CEILING", UNUM_ROUND_HALF_CEILING, R"doc(
             Rounds ties toward +∞.
             )doc")
      .value("UNUM_ROUND_HALF_FLOOR", UNUM_ROUND_HALF_FLOOR, R"doc(
             Rounds ties toward -∞.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();

  //
  // enum UNumberFormatStyle
  //
  py::enum_<UNumberFormatStyle>(m, "UNumberFormatStyle", py::arithmetic(),
                                "Possible number format styles.")
      .value("UNUM_PATTERN_DECIMAL", UNUM_PATTERN_DECIMAL, R"doc(
             Decimal format defined by a pattern string.
             )doc")
      .value("UNUM_DECIMAL", UNUM_DECIMAL, R"doc(
             Decimal format ("normal" style).
             )doc")
      .value("UNUM_CURRENCY", UNUM_CURRENCY, R"doc(
             Currency format (generic).

             Defaults to :attr:`UNUM_CURRENCY_STANDARD` style (using currency
             symbol,
             e.g., "$1.00", with non-accounting style for negative values
             e.g. using minus sign). The specific style may be specified using
             the -cf- locale key.
             )doc")
      .value("UNUM_PERCENT", UNUM_PERCENT, R"doc(
             Percent format.
             )doc")
      .value("UNUM_SCIENTIFIC", UNUM_SCIENTIFIC, R"doc(
             Scientific format.
             )doc")
      .value("UNUM_SPELLOUT", UNUM_SPELLOUT, R"doc(
             Spellout rule-based format.

             The default ruleset can be specified/changed using
             ``unum_setTextAttribute`` with
             :attr:`~UNumberFormatTextAttribute.UNUM_DEFAULT_RULESET`; the
             available public rulesets can be listed using
             ``unum_getTextAttribute`` with
             :attr:`~UNumberFormatTextAttribute.UNUM_PUBLIC_RULESETS`.
             )doc")
      .value("UNUM_ORDINAL", UNUM_ORDINAL, R"doc(
             Ordinal rule-based format.

             The default ruleset can be specified/changed using
             ``unum_setTextAttribute`` with
             :attr:`~UNumberFormatTextAttribute.UNUM_DEFAULT_RULESET`; the
             available public rulesets can be listed using
             ``unum_getTextAttribute`` with
             :attr:`~UNumberFormatTextAttribute.UNUM_PUBLIC_RULESETS`.
             )doc")
      .value("UNUM_DURATION", UNUM_DURATION, R"doc(
             Duration rule-based format.
             )doc")
      .value("UNUM_NUMBERING_SYSTEM", UNUM_NUMBERING_SYSTEM, R"doc(
             Numbering system rule-based format.
             )doc")
      .value("UNUM_PATTERN_RULEBASED", UNUM_PATTERN_RULEBASED, R"doc(
             Rule-based format defined by a pattern string.
             )doc")
      .value("UNUM_CURRENCY_ISO", UNUM_CURRENCY_ISO, R"doc(
             Currency format with an ISO currency code, e.g., "USD1.00".
             )doc")
      .value("UNUM_CURRENCY_PLURAL", UNUM_CURRENCY_PLURAL, R"doc(
             Currency format with a pluralized currency name, e.g.,
             "1.00 US dollar" and "3.00 US dollars".
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
      .value("UNUM_CURRENCY_ACCOUNTING", UNUM_CURRENCY_ACCOUNTING, R"doc(
             Currency format for accounting, e.g., "($3.00)" for negative
             currency amount instead of "-$3.00" (:attr:`UNUM_CURRENCY`).

             Overrides any style specified using -cf- key in locale.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_CASH_CURRENCY", UNUM_CASH_CURRENCY, R"doc(
             Currency format with a currency symbol given CASH usage, e.g.,
             "NT$3" instead of "NT$3.23".
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UNUM_DECIMAL_COMPACT_SHORT", UNUM_DECIMAL_COMPACT_SHORT, R"doc(
             Decimal format expressed using compact notation (short form,
             corresponds to UNumberCompactStyle=UNUM_SHORT) e.g.,
             "23K", "45B"
             )doc")
      .value("UNUM_DECIMAL_COMPACT_LONG", UNUM_DECIMAL_COMPACT_LONG, R"doc(
             Decimal format expressed using compact notation (long form,
             corresponds to UNumberCompactStyle=UNUM_LONG) e.g.,
             "23 thousand", "45 billion"
             )doc")
      .value("UNUM_CURRENCY_STANDARD", UNUM_CURRENCY_STANDARD, R"doc(
             Currency format with a currency symbol, e.g., "$1.00", using
             non-accounting style for negative values (e.g., minus sign).
             Overrides any style specified using -cf- key in locale.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UNUM_DEFAULT", UNUM_DEFAULT, R"doc(
             Default format.
             )doc")
      .value("UNUM_IGNORE", UNUM_IGNORE, R"doc(
             Alias for :attr:`UNUM_PATTERN_DECIMAL`.
             )doc")
      .export_values();

  //
  // enum UNumberFormatSymbol
  //
  py::enum_<UNumberFormatSymbol>(
      m, "UNumberFormatSymbol", py::arithmetic(),
      "Constants for specifying a number format symbol.")
      .value("UNUM_DECIMAL_SEPARATOR_SYMBOL", UNUM_DECIMAL_SEPARATOR_SYMBOL,
             R"doc(
             The decimal separator.
             )doc")
      .value("UNUM_GROUPING_SEPARATOR_SYMBOL", UNUM_GROUPING_SEPARATOR_SYMBOL,
             R"doc(
             The grouping separator.
             )doc")
      .value("UNUM_PATTERN_SEPARATOR_SYMBOL", UNUM_PATTERN_SEPARATOR_SYMBOL,
             R"doc(
             The pattern separator.
             )doc")
      .value("UNUM_PERCENT_SYMBOL", UNUM_PERCENT_SYMBOL, R"doc(
             The percent sign.
             )doc")
      .value("UNUM_ZERO_DIGIT_SYMBOL", UNUM_ZERO_DIGIT_SYMBOL, R"doc(
             Zero.
             )doc")
      .value("UNUM_DIGIT_SYMBOL", UNUM_DIGIT_SYMBOL, R"doc(
             Character representing a digit in the pattern.
             )doc")
      .value("UNUM_MINUS_SIGN_SYMBOL", UNUM_MINUS_SIGN_SYMBOL, R"doc(
             The minus sign.
             )doc")
      .value("UNUM_PLUS_SIGN_SYMBOL", UNUM_PLUS_SIGN_SYMBOL, R"doc(
             The plus sign.
             )doc")
      .value("UNUM_CURRENCY_SYMBOL", UNUM_CURRENCY_SYMBOL, R"doc(
             The currency symbol.
             )doc")
      .value("UNUM_INTL_CURRENCY_SYMBOL", UNUM_INTL_CURRENCY_SYMBOL, R"doc(
             The international currency symbol.
             )doc")
      .value("UNUM_MONETARY_SEPARATOR_SYMBOL", UNUM_MONETARY_SEPARATOR_SYMBOL,
             R"doc(
             The monetary separator.
             )doc")
      .value("UNUM_EXPONENTIAL_SYMBOL", UNUM_EXPONENTIAL_SYMBOL, R"doc(
             The exponential symbol.
             )doc")
      .value("UNUM_PERMILL_SYMBOL", UNUM_PERMILL_SYMBOL, R"doc(
             Per mill symbol.
             )doc")
      .value("UNUM_PAD_ESCAPE_SYMBOL", UNUM_PAD_ESCAPE_SYMBOL, R"doc(
             Escape padding character.
             )doc")
      .value("UNUM_INFINITY_SYMBOL", UNUM_INFINITY_SYMBOL, R"doc(
             Infinity symbol.
             )doc")
      .value("UNUM_NAN_SYMBOL", UNUM_NAN_SYMBOL, R"doc(
             Nan symbol.
             )doc")
      .value("UNUM_SIGNIFICANT_DIGIT_SYMBOL", UNUM_SIGNIFICANT_DIGIT_SYMBOL,
             R"doc(
             Significant digit symbol.
             )doc")
      .value("UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL",
             UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL, R"doc(
             The monetary grouping separator.
             )doc")
      .value("UNUM_ONE_DIGIT_SYMBOL", UNUM_ONE_DIGIT_SYMBOL, R"doc(
             One.
             )doc")
      .value("UNUM_TWO_DIGIT_SYMBOL", UNUM_TWO_DIGIT_SYMBOL, R"doc(
             Two.
             )doc")
      .value("UNUM_THREE_DIGIT_SYMBOL", UNUM_THREE_DIGIT_SYMBOL, R"doc(
             Three.
             )doc")
      .value("UNUM_FOUR_DIGIT_SYMBOL", UNUM_FOUR_DIGIT_SYMBOL, R"doc(
             Four.
             )doc")
      .value("UNUM_FIVE_DIGIT_SYMBOL", UNUM_FIVE_DIGIT_SYMBOL, R"doc(
             Five.
             )doc")
      .value("UNUM_SIX_DIGIT_SYMBOL", UNUM_SIX_DIGIT_SYMBOL, R"doc(
             Six.
             )doc")
      .value("UNUM_SEVEN_DIGIT_SYMBOL", UNUM_SEVEN_DIGIT_SYMBOL, R"doc(
             Seven.
             )doc")
      .value("UNUM_EIGHT_DIGIT_SYMBOL", UNUM_EIGHT_DIGIT_SYMBOL, R"doc(
             Eight.
             )doc")
      .value("UNUM_NINE_DIGIT_SYMBOL", UNUM_NINE_DIGIT_SYMBOL, R"doc(
             Nine.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_EXPONENT_MULTIPLICATION_SYMBOL",
             UNUM_EXPONENT_MULTIPLICATION_SYMBOL, R"doc(
             Multiplication sign.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .export_values();

  //
  // enum UNumberFormatTextAttribute
  //
  py::enum_<UNumberFormatTextAttribute>(
      m, "UNumberFormatTextAttribute", py::arithmetic(),
      "Possible :class:`NumberFormat` text attributes.")
      .value("UNUM_POSITIVE_PREFIX", UNUM_POSITIVE_PREFIX, R"doc(
             Positive prefix.
             )doc")
      .value("UNUM_POSITIVE_SUFFIX", UNUM_POSITIVE_SUFFIX, R"doc(
             Positive suffix.
             )doc")
      .value("UNUM_NEGATIVE_PREFIX", UNUM_NEGATIVE_PREFIX, R"doc(
             Negative prefix.
             )doc")
      .value("UNUM_NEGATIVE_SUFFIX", UNUM_NEGATIVE_SUFFIX, R"doc(
             Negative suffix.
             )doc")
      .value("UNUM_PADDING_CHARACTER", UNUM_PADDING_CHARACTER, R"doc(
             The character used to pad to the format width.
             )doc")
      .value("UNUM_CURRENCY_CODE", UNUM_CURRENCY_CODE, R"doc(
             The ISO currency code.
             )doc")
      .value("UNUM_DEFAULT_RULESET", UNUM_DEFAULT_RULESET, R"doc(
             The default rule set, such as "%spellout-numbering-year:",
             "%spellout-cardinal:",
             "%spellout-ordinal-masculine-plural:",
             "%spellout-ordinal-feminine:", or
             "%spellout-ordinal-neuter:".

             The available public rulesets can be listed using
             unum_getTextAttribute with UNUM_PUBLIC_RULESETS.
             This is only available with rule-based formatters.
             )doc")
      .value("UNUM_PUBLIC_RULESETS", UNUM_PUBLIC_RULESETS, R"doc(
             The public rule sets.

             This is only available with rule-based formatters. This is a
             read-only attribute. The public rulesets are returned as a single
             string, with each ruleset name delimited by ';' (semicolon). See
             the CLDR LDML spec for more information about RBNF rulesets:
             http://www.unicode.org/reports/tr35/tr35-numbers.html#Rule-Based_Number_Formatting
             )doc")
      .export_values();
}

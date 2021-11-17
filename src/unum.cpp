#include "main.hpp"
#include <unicode/unum.h>

void init_unum(py::module &m) {
  py::enum_<UCurrencySpacing>(m, "UCurrencySpacing", py::arithmetic(), "Constants for specifying currency spacing.")
      .value("UNUM_CURRENCY_MATCH", UNUM_CURRENCY_MATCH)
      .value("UNUM_CURRENCY_SURROUNDING_MATCH", UNUM_CURRENCY_SURROUNDING_MATCH)
      .value("UNUM_CURRENCY_INSERT", UNUM_CURRENCY_INSERT)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  py::enum_<UNumberCompactStyle>(m, "UNumberCompactStyle", py::arithmetic(),
                                 "Constants for specifying short or long format.")
      .value("UNUM_SHORT", UNUM_SHORT)
      .value("UNUM_LONG", UNUM_LONG)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  py::enum_<UNumberFormatAttribute>(m, "UNumberFormatAttribute", py::arithmetic(),
                                    "The possible *UNumberFormat* numeric attributes.")
      .value("UNUM_PARSE_INT_ONLY", UNUM_PARSE_INT_ONLY, "Parse integers only.")
      .value("UNUM_GROUPING_USED", UNUM_GROUPING_USED, "Use grouping separator.")
      .value("UNUM_DECIMAL_ALWAYS_SHOWN", UNUM_DECIMAL_ALWAYS_SHOWN, "Always show decimal point.")
      .value("UNUM_MAX_INTEGER_DIGITS", UNUM_MAX_INTEGER_DIGITS, "Maximum integer digits.")
      .value("UNUM_MIN_INTEGER_DIGITS", UNUM_MIN_INTEGER_DIGITS, "Minimum integer digits.")
      .value("UNUM_INTEGER_DIGITS", UNUM_INTEGER_DIGITS, "Integer digits.")
      .value("UNUM_MAX_FRACTION_DIGITS", UNUM_MAX_FRACTION_DIGITS, "Maximum fraction digits.")
      .value("UNUM_MIN_FRACTION_DIGITS", UNUM_MIN_FRACTION_DIGITS, "Minimum fraction digits.")
      .value("UNUM_FRACTION_DIGITS", UNUM_FRACTION_DIGITS, "Fraction digits.")
      .value("UNUM_MULTIPLIER", UNUM_MULTIPLIER, "Multiplier.")
      .value("UNUM_GROUPING_SIZE", UNUM_GROUPING_SIZE, "Grouping size.")
      .value("UNUM_ROUNDING_MODE", UNUM_ROUNDING_MODE, "Rounding Mode.")
      .value("UNUM_ROUNDING_INCREMENT", UNUM_ROUNDING_INCREMENT, "Rounding increment.")
      .value("UNUM_FORMAT_WIDTH", UNUM_FORMAT_WIDTH, "The width to which the output of *format()* is padded.")
      .value("UNUM_PADDING_POSITION", UNUM_PADDING_POSITION, "The position at which padding will take place.")
      .value("UNUM_SECONDARY_GROUPING_SIZE", UNUM_SECONDARY_GROUPING_SIZE, "Secondary grouping size.")
      .value("UNUM_SIGNIFICANT_DIGITS_USED", UNUM_SIGNIFICANT_DIGITS_USED, "Use significant digits.")
      .value("UNUM_MIN_SIGNIFICANT_DIGITS", UNUM_MIN_SIGNIFICANT_DIGITS, "Minimum significant digits.")
      .value("UNUM_MAX_SIGNIFICANT_DIGITS", UNUM_MAX_SIGNIFICANT_DIGITS, "Maximum significant digits.")
      .value("UNUM_LENIENT_PARSE", UNUM_LENIENT_PARSE, "Lenient parse mode used by rule-based formats.")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UNUM_SCALE", UNUM_SCALE,
             "Scale, which adjusts the position of the decimal point when formatting.\n\n  "
             "Amounts will be multiplied by 10 ^ (scale) before they are formatted. The default value for the scale is "
             "0 (no adjustment).\n\n  "
             "Example: setting the scale to 3, 123 formats as \"123,000\"\n\n  "
             "Example: setting the scale to -4, 123 formats as \"0.0123\"\n\n  "
             "This setting is analogous to *get_multiplier_scale()* and *set_multiplier_scale()* in decimfmt.h.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_MINIMUM_GROUPING_DIGITS", UNUM_MINIMUM_GROUPING_DIGITS,
             "Minimum grouping digits; most commonly set to 2 to print \"1000\" instead of \"1,000\".\n\n  "
             "See *DecimalFormat::get_minimum_grouping_digits()*.\n\n  "
             "For better control over grouping strategies, use UNumberFormatter.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_CURRENCY_USAGE", UNUM_CURRENCY_USAGE,
             "if this attribute is set to 0, it is set to *UNUM_CURRENCY_STANDARD* purpose, otherwise it is "
             "*UNUM_CASH_CURRENCY* purpose Default: 0 (*UNUM_CURRENCY_STANDARD* purpose)")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS", UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS,
             "If 1, specifies that if setting the \"max integer digits\" attribute would truncate a value, set an "
             "error status rather than silently truncating.\n\n  "
             "For example, formatting the value 1234 with 4 max int digits would succeed, but formatting 12345 would "
             "fail. There is no effect on parsing. Default: 0 (not set)")
      .value("UNUM_PARSE_NO_EXPONENT", UNUM_PARSE_NO_EXPONENT,
             "if this attribute is set to 1, specifies that, if the pattern doesn't contain an exponent, the exponent "
             "will not be parsed.\n\n  "
             "If the pattern does contain an exponent, this attribute has no effect. Has no effect on formatting. "
             "Default: 0 (unset)")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_PARSE_DECIMAL_MARK_REQUIRED", UNUM_PARSE_DECIMAL_MARK_REQUIRED,
             "if this attribute is set to 1, specifies that, if the pattern contains a decimal mark the input is "
             "required to have one.\n\n  "
             "If this attribute is set to 0, specifies that input does not have to contain a decimal mark. Has no "
             "effect on formatting. Default: 0 (unset)")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_PARSE_CASE_SENSITIVE", UNUM_PARSE_CASE_SENSITIVE,
             "Parsing: if set to 1, parsing is sensitive to case (lowercase/uppercase).")
      .value("UNUM_SIGN_ALWAYS_SHOWN", UNUM_SIGN_ALWAYS_SHOWN,
             "Formatting: if set to 1, whether to show the plus sign on non-negative numbers.\n\n  "
             "For better control over sign display, use *UNumberFormatter*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  py::enum_<UNumberFormatFields>(
      m, "UNumberFormatFields", py::arithmetic(),
      "*FieldPosition* and *UFieldPosition* selectors for format fields defined by *NumberFormat* and *UNumberFormat*.")
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
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  py::enum_<UNumberFormatMinimumGroupingDigits>(
      m, "UNumberFormatMinimumGroupingDigits", py::arithmetic(),
      "Selectors with special numeric values to use locale default minimum grouping digits for the "
      "*DecimalFormat*/*UNumberFormat* *set_minimum_grouping_digits* method.\n\n"
      "Do not use these constants with the *[U]NumberFormatter* API.")
      .value("UNUM_MINIMUM_GROUPING_DIGITS_AUTO", UNUM_MINIMUM_GROUPING_DIGITS_AUTO,
             "Display grouping using the default strategy for all locales.")
      .value("UNUM_MINIMUM_GROUPING_DIGITS_MIN2", UNUM_MINIMUM_GROUPING_DIGITS_MIN2,
             "Display grouping using locale defaults, except do not show grouping on values smaller than 10000 (such "
             "that there is a minimum of two digits before the first separator).")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  py::enum_<UNumberFormatPadPosition>(m, "UNumberFormatPadPosition", py::arithmetic(),
                                      "The possible number format pad positions.")
      .value("UNUM_PAD_BEFORE_PREFIX", UNUM_PAD_BEFORE_PREFIX)
      .value("UNUM_PAD_AFTER_PREFIX", UNUM_PAD_AFTER_PREFIX)
      .value("UNUM_PAD_BEFORE_SUFFIX", UNUM_PAD_BEFORE_SUFFIX)
      .value("UNUM_PAD_AFTER_SUFFIX", UNUM_PAD_AFTER_SUFFIX)
      .export_values();

  py::enum_<UNumberFormatRoundingMode>(
      m, "UNumberFormatRoundingMode", py::arithmetic(),
      "The possible number format rounding modes.\n\n"
      "For more detail on rounding modes, see: "
      "https://unicode-org.github.io/icu/userguide/format_parse/numbers/rounding-modes")
      .value("UNUM_ROUND_CEILING", UNUM_ROUND_CEILING)
      .value("UNUM_ROUND_FLOOR", UNUM_ROUND_FLOOR)
      .value("UNUM_ROUND_DOWN", UNUM_ROUND_DOWN)
      .value("UNUM_ROUND_UP", UNUM_ROUND_UP)
      .value("UNUM_ROUND_HALFEVEN", UNUM_ROUND_HALFEVEN, "Half-even rounding.")
      .value("UNUM_ROUND_HALFDOWN", UNUM_ROUND_HALFDOWN)
      .value("UNUM_ROUND_HALFUP", UNUM_ROUND_HALFUP)
      .value("UNUM_ROUND_UNNECESSARY", UNUM_ROUND_UNNECESSARY,
             "ROUND_UNNECESSARY reports an error if formatted result is not exact.")
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UNUM_ROUND_HALF_ODD", UNUM_ROUND_HALF_ODD, "Rounds ties toward the odd number.")
      .value("UNUM_ROUND_HALF_CEILING", UNUM_ROUND_HALF_CEILING, "Rounds ties toward +\xe2\x88\x9e.")
      .value("UNUM_ROUND_HALF_FLOOR", UNUM_ROUND_HALF_FLOOR, "Rounds ties toward -\xe2\x88\x9e.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();

  py::enum_<UNumberFormatStyle>(m, "UNumberFormatStyle", py::arithmetic(), "The possible number format styles.")
      .value("UNUM_PATTERN_DECIMAL", UNUM_PATTERN_DECIMAL, "Decimal format defined by a pattern string.")
      .value("UNUM_DECIMAL", UNUM_DECIMAL, "Decimal format (\"normal\" style).")
      .value("UNUM_CURRENCY", UNUM_CURRENCY,
             "Currency format (generic).\n\n  "
             "Defaults to *UNUM_CURRENCY_STANDARD* style (using currency symbol, e.g., \"$1.00\", with non-accounting "
             "style for negative values e.g. using minus sign). The specific style may be specified using the -cf- "
             "locale key.")
      .value("UNUM_PERCENT", UNUM_PERCENT, "Percent format.")
      .value("UNUM_SCIENTIFIC", UNUM_SCIENTIFIC, "Scientific format.")
      .value("UNUM_SPELLOUT", UNUM_SPELLOUT,
             "Spellout rule-based format.\n\n  "
             "The default ruleset can be specified/changed using *unum_set_text_attribute* with "
             "*UNUM_DEFAULT_RULESET*; the available public rulesets can be listed using *unum_get_text_attribute* "
             "with *UNUM_PUBLIC_RULESETS*.")
      .value("UNUM_ORDINAL", UNUM_ORDINAL,
             "Ordinal rule-based format.\n\n  "
             "The default ruleset can be specified/changed using *unum_set_text_attribute* with "
             "*UNUM_DEFAULT_RULESET*; the available public rulesets can be listed using *unum_get_text_attribute* "
             "with "
             "*UNUM_PUBLIC_RULESETS*.")
      .value("UNUM_DURATION", UNUM_DURATION, "Duration rule-based format.")
      .value("UNUM_NUMBERING_SYSTEM", UNUM_NUMBERING_SYSTEM, "Numbering system rule-based format.")
      .value("UNUM_PATTERN_RULEBASED", UNUM_PATTERN_RULEBASED, "Rule-based format defined by a pattern string.")
      .value("UNUM_CURRENCY_ISO", UNUM_CURRENCY_ISO, "Currency format with an ISO currency code, e.g., \"USD1.00\".")
      .value("UNUM_CURRENCY_PLURAL", UNUM_CURRENCY_PLURAL,
             "Currency format with a pluralized currency name, e.g., \"1.00 US dollar\" and \"3.00 US dollars\".")
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
      .value("UNUM_CURRENCY_ACCOUNTING", UNUM_CURRENCY_ACCOUNTING,
             "Currency format for accounting, e.g., \"($3.00)\" for negative currency amount instead of \"-$3.00\" "
             "(*UNUM_CURRENCY*).\n\n  "
             "Overrides any style specified using -cf- key in locale.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_CASH_CURRENCY", UNUM_CASH_CURRENCY,
             "Currency format with a currency symbol given CASH usage, e.g., \"NT$3\" instead of \"NT$3.23\".")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UNUM_DECIMAL_COMPACT_SHORT", UNUM_DECIMAL_COMPACT_SHORT,
             "Decimal format expressed using compact notation (short form, corresponds to "
             "*UNumberCompactStyle=UNUM_SHORT*) e.g., "
             "\"23K\", \"45B\"")
      .value("UNUM_DECIMAL_COMPACT_LONG", UNUM_DECIMAL_COMPACT_LONG,
             "Decimal format expressed using compact notation (long form, corresponds to "
             "*UNumberCompactStyle=UNUM_LONG*) e.g., "
             "\"23 thousand\", \"45 billion\"")
      .value("UNUM_CURRENCY_STANDARD", UNUM_CURRENCY_STANDARD,
             "Currency format with a currency symbol, e.g., \"$1.00\", using non-accounting style for negative values "
             "(e.g., "
             "minus sign). Overrides any style specified using -cf- key in locale.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UNUM_DEFAULT", UNUM_DEFAULT, "Default format.")
      .value("UNUM_IGNORE", UNUM_IGNORE, "Alias for *UNUM_PATTERN_DECIMAL*.")
      .export_values();

  py::enum_<UNumberFormatSymbol>(m, "UNumberFormatSymbol", py::arithmetic(),
                                 "Constants for specifying a number format symbol.")
      .value("UNUM_DECIMAL_SEPARATOR_SYMBOL", UNUM_DECIMAL_SEPARATOR_SYMBOL, "The decimal separator.")
      .value("UNUM_GROUPING_SEPARATOR_SYMBOL", UNUM_GROUPING_SEPARATOR_SYMBOL, "The grouping separator.")
      .value("UNUM_PATTERN_SEPARATOR_SYMBOL", UNUM_PATTERN_SEPARATOR_SYMBOL, "The pattern separator.")
      .value("UNUM_PERCENT_SYMBOL", UNUM_PERCENT_SYMBOL, "The percent sign.")
      .value("UNUM_ZERO_DIGIT_SYMBOL", UNUM_ZERO_DIGIT_SYMBOL, "Zero.")
      .value("UNUM_DIGIT_SYMBOL", UNUM_DIGIT_SYMBOL, "Character representing a digit in the pattern.")
      .value("UNUM_MINUS_SIGN_SYMBOL", UNUM_MINUS_SIGN_SYMBOL, "The minus sign.")
      .value("UNUM_PLUS_SIGN_SYMBOL", UNUM_PLUS_SIGN_SYMBOL, "The plus sign.")
      .value("UNUM_CURRENCY_SYMBOL", UNUM_CURRENCY_SYMBOL, "The currency symbol.")
      .value("UNUM_INTL_CURRENCY_SYMBOL", UNUM_INTL_CURRENCY_SYMBOL, "The international currency symbol.")
      .value("UNUM_MONETARY_SEPARATOR_SYMBOL", UNUM_MONETARY_SEPARATOR_SYMBOL, "The monetary separator.")
      .value("UNUM_EXPONENTIAL_SYMBOL", UNUM_EXPONENTIAL_SYMBOL, "The exponential symbol.")
      .value("UNUM_PERMILL_SYMBOL", UNUM_PERMILL_SYMBOL, "Per mill symbol.")
      .value("UNUM_PAD_ESCAPE_SYMBOL", UNUM_PAD_ESCAPE_SYMBOL, "Escape padding character.")
      .value("UNUM_INFINITY_SYMBOL", UNUM_INFINITY_SYMBOL, "Infinity symbol.")
      .value("UNUM_NAN_SYMBOL", UNUM_NAN_SYMBOL, "Nan symbol.")
      .value("UNUM_SIGNIFICANT_DIGIT_SYMBOL", UNUM_SIGNIFICANT_DIGIT_SYMBOL, "Significant digit symbol.")
      .value("UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL", UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL,
             "The monetary grouping separator.")
      .value("UNUM_ONE_DIGIT_SYMBOL", UNUM_ONE_DIGIT_SYMBOL, "One.")
      .value("UNUM_TWO_DIGIT_SYMBOL", UNUM_TWO_DIGIT_SYMBOL, "Two.")
      .value("UNUM_THREE_DIGIT_SYMBOL", UNUM_THREE_DIGIT_SYMBOL, "Three.")
      .value("UNUM_FOUR_DIGIT_SYMBOL", UNUM_FOUR_DIGIT_SYMBOL, "Four.")
      .value("UNUM_FIVE_DIGIT_SYMBOL", UNUM_FIVE_DIGIT_SYMBOL, "Five.")
      .value("UNUM_SIX_DIGIT_SYMBOL", UNUM_SIX_DIGIT_SYMBOL, "Six.")
      .value("UNUM_SEVEN_DIGIT_SYMBOL", UNUM_SEVEN_DIGIT_SYMBOL, "Seven.")
      .value("UNUM_EIGHT_DIGIT_SYMBOL", UNUM_EIGHT_DIGIT_SYMBOL, "Eight.")
      .value("UNUM_NINE_DIGIT_SYMBOL", UNUM_NINE_DIGIT_SYMBOL, "Nine.")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_EXPONENT_MULTIPLICATION_SYMBOL", UNUM_EXPONENT_MULTIPLICATION_SYMBOL, "Multiplication sign.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .export_values();

  py::enum_<UNumberFormatTextAttribute>(m, "UNumberFormatTextAttribute", py::arithmetic(),
                                        "The possible *UNumberFormat* text attributes.")
      .value("UNUM_POSITIVE_PREFIX", UNUM_POSITIVE_PREFIX, "Positive prefix.")
      .value("UNUM_POSITIVE_SUFFIX", UNUM_POSITIVE_SUFFIX, "Positive suffix.")
      .value("UNUM_NEGATIVE_PREFIX", UNUM_NEGATIVE_PREFIX, "Negative prefix.")
      .value("UNUM_NEGATIVE_SUFFIX", UNUM_NEGATIVE_SUFFIX, "Negative suffix.")
      .value("UNUM_PADDING_CHARACTER", UNUM_PADDING_CHARACTER, "The character used to pad to the format width.")
      .value("UNUM_CURRENCY_CODE", UNUM_CURRENCY_CODE, "The ISO currency code.")
      .value("UNUM_DEFAULT_RULESET", UNUM_DEFAULT_RULESET,
             "The default rule set, such as \"%spellout-numbering-year:\", \"%spellout-cardinal:\", "
             "\"%spellout-ordinal-masculine-plural:\", \"%spellout-ordinal-feminine:\", or "
             "\"%spellout-ordinal-neuter:\".\n\n  "
             "The available public rulesets can be listed using *unum_get_text_attribute* with *UNUM_PUBLIC_RULESETS*. "
             "This is only available with rule-based formatters.")
      .value("UNUM_PUBLIC_RULESETS", UNUM_PUBLIC_RULESETS)
      .export_values();
}

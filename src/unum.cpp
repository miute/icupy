#include "main.hpp"
#include <unicode/unum.h>

void init_unum(py::module &m) {
  py::enum_<UCurrencySpacing>(m, "UCurrencySpacing", py::arithmetic())
      .value("UNUM_CURRENCY_MATCH", UNUM_CURRENCY_MATCH)
      .value("UNUM_CURRENCY_SURROUNDING_MATCH", UNUM_CURRENCY_SURROUNDING_MATCH)
      .value("UNUM_CURRENCY_INSERT", UNUM_CURRENCY_INSERT);

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  py::enum_<UNumberCompactStyle>(m, "UNumberCompactStyle", py::arithmetic())
      .value("UNUM_SHORT", UNUM_SHORT)
      .value("UNUM_LONG", UNUM_LONG);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  py::enum_<UNumberFormatAttribute>(m, "UNumberFormatAttribute", py::arithmetic())
      .value("UNUM_PARSE_INT_ONLY", UNUM_PARSE_INT_ONLY)
      .value("UNUM_GROUPING_USED", UNUM_GROUPING_USED)
      .value("UNUM_DECIMAL_ALWAYS_SHOWN", UNUM_DECIMAL_ALWAYS_SHOWN)
      .value("UNUM_MAX_INTEGER_DIGITS", UNUM_MAX_INTEGER_DIGITS)
      .value("UNUM_MIN_INTEGER_DIGITS", UNUM_MIN_INTEGER_DIGITS)
      .value("UNUM_INTEGER_DIGITS", UNUM_INTEGER_DIGITS)
      .value("UNUM_MAX_FRACTION_DIGITS", UNUM_MAX_FRACTION_DIGITS)
      .value("UNUM_MIN_FRACTION_DIGITS", UNUM_MIN_FRACTION_DIGITS)
      .value("UNUM_FRACTION_DIGITS", UNUM_FRACTION_DIGITS)
      .value("UNUM_MULTIPLIER", UNUM_MULTIPLIER)
      .value("UNUM_GROUPING_SIZE", UNUM_GROUPING_SIZE)
      .value("UNUM_ROUNDING_MODE", UNUM_ROUNDING_MODE)
      .value("UNUM_ROUNDING_INCREMENT", UNUM_ROUNDING_INCREMENT)
      .value("UNUM_FORMAT_WIDTH", UNUM_FORMAT_WIDTH)
      .value("UNUM_PADDING_POSITION", UNUM_PADDING_POSITION)
      .value("UNUM_SECONDARY_GROUPING_SIZE", UNUM_SECONDARY_GROUPING_SIZE)
      .value("UNUM_SIGNIFICANT_DIGITS_USED", UNUM_SIGNIFICANT_DIGITS_USED)
      .value("UNUM_MIN_SIGNIFICANT_DIGITS", UNUM_MIN_SIGNIFICANT_DIGITS)
      .value("UNUM_MAX_SIGNIFICANT_DIGITS", UNUM_MAX_SIGNIFICANT_DIGITS)
      .value("UNUM_LENIENT_PARSE", UNUM_LENIENT_PARSE)
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UNUM_SCALE", UNUM_SCALE)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_MINIMUM_GROUPING_DIGITS", UNUM_MINIMUM_GROUPING_DIGITS)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_CURRENCY_USAGE", UNUM_CURRENCY_USAGE)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS", UNUM_FORMAT_FAIL_IF_MORE_THAN_MAX_DIGITS)
      .value("UNUM_PARSE_NO_EXPONENT", UNUM_PARSE_NO_EXPONENT)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_PARSE_DECIMAL_MARK_REQUIRED", UNUM_PARSE_DECIMAL_MARK_REQUIRED)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("UNUM_PARSE_CASE_SENSITIVE", UNUM_PARSE_CASE_SENSITIVE)
      .value("UNUM_SIGN_ALWAYS_SHOWN", UNUM_SIGN_ALWAYS_SHOWN)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
      ;

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  py::enum_<UNumberFormatFields>(m, "UNumberFormatFields", py::arithmetic())
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
      ;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  py::enum_<UNumberFormatMinimumGroupingDigits>(m, "UNumberFormatMinimumGroupingDigits", py::arithmetic())
      .value("UNUM_MINIMUM_GROUPING_DIGITS_AUTO", UNUM_MINIMUM_GROUPING_DIGITS_AUTO)
      .value("UNUM_MINIMUM_GROUPING_DIGITS_MIN2", UNUM_MINIMUM_GROUPING_DIGITS_MIN2);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  py::enum_<UNumberFormatPadPosition>(m, "UNumberFormatPadPosition", py::arithmetic())
      .value("UNUM_PAD_BEFORE_PREFIX", UNUM_PAD_BEFORE_PREFIX)
      .value("UNUM_PAD_AFTER_PREFIX", UNUM_PAD_AFTER_PREFIX)
      .value("UNUM_PAD_BEFORE_SUFFIX", UNUM_PAD_BEFORE_SUFFIX)
      .value("UNUM_PAD_AFTER_SUFFIX", UNUM_PAD_AFTER_SUFFIX);

  py::enum_<UNumberFormatRoundingMode>(m, "UNumberFormatRoundingMode", py::arithmetic())
      .value("UNUM_ROUND_CEILING", UNUM_ROUND_CEILING)
      .value("UNUM_ROUND_FLOOR", UNUM_ROUND_FLOOR)
      .value("UNUM_ROUND_DOWN", UNUM_ROUND_DOWN)
      .value("UNUM_ROUND_UP", UNUM_ROUND_UP)
      .value("UNUM_ROUND_HALFEVEN", UNUM_ROUND_HALFEVEN)
      .value("UNUM_ROUND_HALFDOWN", UNUM_ROUND_HALFDOWN)
      .value("UNUM_ROUND_HALFUP", UNUM_ROUND_HALFUP)
      .value("UNUM_ROUND_UNNECESSARY", UNUM_ROUND_UNNECESSARY)
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UNUM_ROUND_HALF_ODD", UNUM_ROUND_HALF_ODD)
      .value("UNUM_ROUND_HALF_CEILING", UNUM_ROUND_HALF_CEILING)
      .value("UNUM_ROUND_HALF_FLOOR", UNUM_ROUND_HALF_FLOOR)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      ;

  py::enum_<UNumberFormatStyle>(m, "UNumberFormatStyle", py::arithmetic())
      .value("UNUM_PATTERN_DECIMAL", UNUM_PATTERN_DECIMAL)
      .value("UNUM_DECIMAL", UNUM_DECIMAL)
      .value("UNUM_CURRENCY", UNUM_CURRENCY)
      .value("UNUM_PERCENT", UNUM_PERCENT)
      .value("UNUM_SCIENTIFIC", UNUM_SCIENTIFIC)
      .value("UNUM_SPELLOUT", UNUM_SPELLOUT)
      .value("UNUM_ORDINAL", UNUM_ORDINAL)
      .value("UNUM_DURATION", UNUM_DURATION)
      .value("UNUM_NUMBERING_SYSTEM", UNUM_NUMBERING_SYSTEM)
      .value("UNUM_PATTERN_RULEBASED", UNUM_PATTERN_RULEBASED)
      .value("UNUM_CURRENCY_ISO", UNUM_CURRENCY_ISO)
      .value("UNUM_CURRENCY_PLURAL", UNUM_CURRENCY_PLURAL)
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
      .value("UNUM_CURRENCY_ACCOUNTING", UNUM_CURRENCY_ACCOUNTING)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_CASH_CURRENCY", UNUM_CASH_CURRENCY)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UNUM_DECIMAL_COMPACT_SHORT", UNUM_DECIMAL_COMPACT_SHORT)
      .value("UNUM_DECIMAL_COMPACT_LONG", UNUM_DECIMAL_COMPACT_LONG)
      .value("UNUM_CURRENCY_STANDARD", UNUM_CURRENCY_STANDARD)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
      .value("UNUM_DEFAULT", UNUM_DEFAULT)
      .value("UNUM_IGNORE", UNUM_IGNORE);

  py::enum_<UNumberFormatSymbol>(m, "UNumberFormatSymbol", py::arithmetic())
      .value("UNUM_DECIMAL_SEPARATOR_SYMBOL", UNUM_DECIMAL_SEPARATOR_SYMBOL)
      .value("UNUM_GROUPING_SEPARATOR_SYMBOL", UNUM_GROUPING_SEPARATOR_SYMBOL)
      .value("UNUM_PATTERN_SEPARATOR_SYMBOL", UNUM_PATTERN_SEPARATOR_SYMBOL)
      .value("UNUM_PERCENT_SYMBOL", UNUM_PERCENT_SYMBOL)
      .value("UNUM_ZERO_DIGIT_SYMBOL", UNUM_ZERO_DIGIT_SYMBOL)
      .value("UNUM_DIGIT_SYMBOL", UNUM_DIGIT_SYMBOL)
      .value("UNUM_MINUS_SIGN_SYMBOL", UNUM_MINUS_SIGN_SYMBOL)
      .value("UNUM_PLUS_SIGN_SYMBOL", UNUM_PLUS_SIGN_SYMBOL)
      .value("UNUM_CURRENCY_SYMBOL", UNUM_CURRENCY_SYMBOL)
      .value("UNUM_INTL_CURRENCY_SYMBOL", UNUM_INTL_CURRENCY_SYMBOL)
      .value("UNUM_MONETARY_SEPARATOR_SYMBOL", UNUM_MONETARY_SEPARATOR_SYMBOL)
      .value("UNUM_EXPONENTIAL_SYMBOL", UNUM_EXPONENTIAL_SYMBOL)
      .value("UNUM_PERMILL_SYMBOL", UNUM_PERMILL_SYMBOL)
      .value("UNUM_PAD_ESCAPE_SYMBOL", UNUM_PAD_ESCAPE_SYMBOL)
      .value("UNUM_INFINITY_SYMBOL", UNUM_INFINITY_SYMBOL)
      .value("UNUM_NAN_SYMBOL", UNUM_NAN_SYMBOL)
      .value("UNUM_SIGNIFICANT_DIGIT_SYMBOL", UNUM_SIGNIFICANT_DIGIT_SYMBOL)
      .value("UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL", UNUM_MONETARY_GROUPING_SEPARATOR_SYMBOL)
      .value("UNUM_ONE_DIGIT_SYMBOL", UNUM_ONE_DIGIT_SYMBOL)
      .value("UNUM_TWO_DIGIT_SYMBOL", UNUM_TWO_DIGIT_SYMBOL)
      .value("UNUM_THREE_DIGIT_SYMBOL", UNUM_THREE_DIGIT_SYMBOL)
      .value("UNUM_FOUR_DIGIT_SYMBOL", UNUM_FOUR_DIGIT_SYMBOL)
      .value("UNUM_FIVE_DIGIT_SYMBOL", UNUM_FIVE_DIGIT_SYMBOL)
      .value("UNUM_SIX_DIGIT_SYMBOL", UNUM_SIX_DIGIT_SYMBOL)
      .value("UNUM_SEVEN_DIGIT_SYMBOL", UNUM_SEVEN_DIGIT_SYMBOL)
      .value("UNUM_EIGHT_DIGIT_SYMBOL", UNUM_EIGHT_DIGIT_SYMBOL)
      .value("UNUM_NINE_DIGIT_SYMBOL", UNUM_NINE_DIGIT_SYMBOL)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UNUM_EXPONENT_MULTIPLICATION_SYMBOL", UNUM_EXPONENT_MULTIPLICATION_SYMBOL)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      ;

  py::enum_<UNumberFormatTextAttribute>(m, "UNumberFormatTextAttribute", py::arithmetic())
      .value("UNUM_POSITIVE_PREFIX", UNUM_POSITIVE_PREFIX)
      .value("UNUM_POSITIVE_SUFFIX", UNUM_POSITIVE_SUFFIX)
      .value("UNUM_NEGATIVE_PREFIX", UNUM_NEGATIVE_PREFIX)
      .value("UNUM_NEGATIVE_SUFFIX", UNUM_NEGATIVE_SUFFIX)
      .value("UNUM_PADDING_CHARACTER", UNUM_PADDING_CHARACTER)
      .value("UNUM_CURRENCY_CODE", UNUM_CURRENCY_CODE)
      .value("UNUM_DEFAULT_RULESET", UNUM_DEFAULT_RULESET)
      .value("UNUM_PUBLIC_RULESETS", UNUM_PUBLIC_RULESETS);
}

import copy

import pytest
from icupy.icu import (
    CurrencyPluralInfo, DecimalFormat, DecimalFormatSymbols, FieldPosition,
    FieldPositionIterator, Formattable, INT32_MAX, Locale, NumberFormat,
    ParsePosition, UParseError, U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # UBool icu::DecimalFormat::areSignificantDigitsUsed()
    assert not fmt.are_significant_digits_used()

    # void icu::DecimalFormat::setSignificantDigitsUsed(
    #       UBool useSignificantDigits
    # )
    fmt.set_significant_digits_used(True)
    assert fmt.are_significant_digits_used()

    # const CurrencyPluralInfo *icu::DecimalFormat::getCurrencyPluralInfo(void)
    result = fmt.get_currency_plural_info()
    assert result is None

    # void icu::DecimalFormat::setCurrencyPluralInfo(
    #       const CurrencyPluralInfo &info
    # )
    info = CurrencyPluralInfo(Locale.get_france())
    fmt.set_currency_plural_info(info)
    result = fmt.get_currency_plural_info()
    assert isinstance(result, CurrencyPluralInfo)
    assert result == info

    # const DecimalFormatSymbols *icu::DecimalFormat::getDecimalFormatSymbols()
    result = fmt.get_decimal_format_symbols()
    assert isinstance(result, DecimalFormatSymbols)
    assert result == symbols

    # void icu::DecimalFormat::setDecimalFormatSymbols(
    #       const DecimalFormatSymbols &symbols
    # )
    symbols2 = DecimalFormatSymbols(Locale.get_france())
    assert symbols != symbols2
    fmt.set_decimal_format_symbols(symbols2)
    assert fmt.get_decimal_format_symbols() == symbols2

    # int32_t icu::DecimalFormat::getFormatWidth(void)
    assert fmt.get_format_width() == -1

    # void icu::DecimalFormat::setFormatWidth(int32_t width)
    fmt.set_format_width(0)
    assert fmt.get_format_width() == 0

    # int32_t icu::DecimalFormat::getGroupingSize(void)
    assert fmt.get_grouping_size() == 0

    # void icu::DecimalFormat::setGroupingSize(int32_t newValue)
    fmt.set_grouping_size(3)
    assert fmt.get_grouping_size() == 3

    # int32_t icu::DecimalFormat::getMaximumSignificantDigits()
    assert fmt.get_maximum_significant_digits() == 6

    # void icu::DecimalFormat::setMaximumSignificantDigits(int32_t max)
    fmt.set_maximum_significant_digits(5)
    assert fmt.get_maximum_significant_digits() == 5

    # int8_t icu::DecimalFormat::getMinimumExponentDigits(void)
    assert fmt.get_minimum_exponent_digits() == -1

    # void icu::DecimalFormat::setMinimumExponentDigits(int8_t minExpDig)
    fmt.set_minimum_exponent_digits(1)
    assert fmt.get_minimum_exponent_digits() == 1

    # int32_t icu::DecimalFormat::getMinimumSignificantDigits()
    assert fmt.get_minimum_significant_digits() == 1

    # void icu::DecimalFormat::setMinimumSignificantDigits(int32_t min)
    fmt.set_minimum_significant_digits(2)
    assert fmt.get_minimum_significant_digits() == 2

    # int32_t icu::DecimalFormat::getMultiplier(void)
    assert fmt.get_multiplier() == 1

    # void icu::DecimalFormat::setMultiplier(int32_t newValue)
    fmt.set_multiplier(10)
    assert fmt.get_multiplier() == 10

    # UnicodeString &icu::DecimalFormat::getNegativePrefix(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = fmt.get_negative_prefix(result)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == "-"

    # void icu::DecimalFormat::setNegativePrefix(const UnicodeString &newValue)
    fmt.set_negative_prefix(UnicodeString("\u2212"))
    assert fmt.get_negative_prefix(result) == "\u2212"  # "−"
    fmt.set_negative_prefix("\u2212")
    assert fmt.get_negative_prefix(result) == "\u2212"  # "−"

    # UnicodeString &icu::DecimalFormat::getNegativeSuffix(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = fmt.get_negative_suffix(result)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == " euros"

    # void icu::DecimalFormat::setNegativeSuffix(const UnicodeString &newValue)
    fmt.set_negative_suffix(UnicodeString("%"))
    assert fmt.get_negative_suffix(result) == "%"
    fmt.set_negative_suffix("%")
    assert fmt.get_negative_suffix(result) == "%"

    # UnicodeString icu::DecimalFormat::getPadCharacterString()
    result = fmt.get_pad_character_string()
    assert isinstance(result, UnicodeString)
    assert result == "\x20"

    # void icu::DecimalFormat::setPadCharacter(const UnicodeString &padChar)
    fmt.set_pad_character(UnicodeString("_"))
    assert fmt.get_pad_character_string() == "_"
    fmt.set_pad_character("_")
    assert fmt.get_pad_character_string() == "_"

    # EPadPosition icu::DecimalFormat::getPadPosition(void)
    assert fmt.get_pad_position() == DecimalFormat.PAD_BEFORE_PREFIX

    # void icu::DecimalFormat::setPadPosition(EPadPosition padPos)
    fmt.set_pad_position(DecimalFormat.PAD_BEFORE_SUFFIX)
    assert fmt.get_pad_position() == DecimalFormat.PAD_BEFORE_SUFFIX

    # UnicodeString &icu::DecimalFormat::getPositivePrefix(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = fmt.get_positive_prefix(result)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(output)
    assert len(result) == 0

    # void icu::DecimalFormat::setPositivePrefix(const UnicodeString &newValue)
    fmt.set_positive_prefix(UnicodeString("+"))
    assert fmt.get_positive_prefix(result) == "+"
    fmt.set_positive_prefix("+")
    assert fmt.get_positive_prefix(result) == "+"

    # UnicodeString &icu::DecimalFormat::getPositiveSuffix(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = fmt.get_positive_suffix(result)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(output)
    assert result == " euros"

    # void icu::DecimalFormat::setPositiveSuffix(const UnicodeString &newValue)
    fmt.set_positive_suffix(UnicodeString("_"))
    assert fmt.get_positive_suffix(result) == "_"
    fmt.set_positive_suffix("_")
    assert fmt.get_positive_suffix(result) == "_"

    # double icu::DecimalFormat::getRoundingIncrement(void)
    assert fmt.get_rounding_increment() == 0.0

    # void icu::DecimalFormat::setRoundingIncrement(double newValue)
    fmt.set_rounding_increment(1.2)
    assert fmt.get_rounding_increment() == 1.2

    # ERoundingMode icu::DecimalFormat::getRoundingMode(void)
    assert fmt.get_rounding_mode() == DecimalFormat.ROUND_HALF_EVEN

    # void icu::DecimalFormat::setRoundingMode(ERoundingMode roundingMode)
    fmt.set_rounding_mode(DecimalFormat.ROUND_CEILING)
    assert fmt.get_rounding_mode() == DecimalFormat.ROUND_CEILING

    # int32_t icu::DecimalFormat::getSecondaryGroupingSize(void)
    assert fmt.get_secondary_grouping_size() == 0

    # void icu::DecimalFormat::setSecondaryGroupingSize(int32_t newValue)
    fmt.set_secondary_grouping_size(3)
    assert fmt.get_secondary_grouping_size() == 3

    # UBool icu::DecimalFormat::isDecimalSeparatorAlwaysShown(void)
    assert not fmt.is_decimal_separator_always_shown()

    # void icu::DecimalFormat::setDecimalSeparatorAlwaysShown(UBool newValue)
    fmt.set_decimal_separator_always_shown(True)
    assert fmt.is_decimal_separator_always_shown()

    # UBool icu::DecimalFormat::isExponentSignAlwaysShown(void)
    assert not fmt.is_exponent_sign_always_shown()

    # void icu::DecimalFormat::setExponentSignAlwaysShown(UBool expSignAlways)
    fmt.set_exponent_sign_always_shown(True)
    assert fmt.is_exponent_sign_always_shown()

    # UBool icu::DecimalFormat::isScientificNotation(void)
    assert fmt.is_scientific_notation()

    # void icu::DecimalFormat::setScientificNotation(UBool useScientific)
    fmt.set_scientific_notation(False)
    assert not fmt.is_scientific_notation()

    # UBool icu::DecimalFormat::operator==(const Format &other)
    fmt2 = DecimalFormat("0", DecimalFormatSymbols(Locale.get_us()))
    fmt3 = fmt2.clone()
    assert not (fmt == fmt2)
    assert not (fmt == fmt3)
    assert fmt2 == fmt3

    # void icu::DecimalFormat::setCurrency(
    #       const char16_t *theCurrency,
    #       UErrorCode &ec
    # )
    fmt.set_currency("EUR")

    # void icu::DecimalFormat::setLenient(UBool enable)
    fmt.set_lenient(True)

    # void icu::DecimalFormat::setMaximumFractionDigits(int32_t newValue)
    fmt.set_maximum_fraction_digits(4)

    # void icu::DecimalFormat::setMaximumIntegerDigits(int32_t newValue)
    fmt.set_maximum_integer_digits(4)

    # void icu::DecimalFormat::setMinimumFractionDigits(int32_t newValue)
    fmt.set_minimum_fraction_digits(0)

    # void icu::DecimalFormat::setMinimumIntegerDigits(int32_t newValue)
    fmt.set_minimum_integer_digits(0)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_api_49():
    from icupy.icu import CurrencyAmount

    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # CurrencyAmount *icu::DecimalFormat::parseCurrency(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = ParsePosition()
    camt = fmt.parse_currency(UnicodeString("USD-1,234.00"), pos)
    assert pos.get_error_index() == -1
    assert isinstance(camt, CurrencyAmount)

    pos = ParsePosition()
    camt = fmt.parse_currency(UnicodeString("-1,234.00"), pos)
    assert pos.get_error_index() != -1
    assert camt is None

    pos = ParsePosition()
    camt = fmt.parse_currency("USD-1,234.00", pos)
    assert pos.get_error_index() == -1
    assert isinstance(camt, CurrencyAmount)

    pos = ParsePosition()
    camt = fmt.parse_currency("-1,234.00", pos)
    assert pos.get_error_index() != -1
    assert camt is None


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51():
    from icupy.icu import UNumberFormatAttribute

    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # int32_t icu::DecimalFormat::getAttribute(
    #       UNumberFormatAttribute attr,
    #       UErrorCode &status
    # )
    assert fmt.get_attribute(UNumberFormatAttribute.UNUM_SCALE) == 0

    # DecimalFormat &icu::DecimalFormat::setAttribute(
    #       UNumberFormatAttribute attr,
    #       int32_t newValue,
    #       UErrorCode &status
    # )
    result = fmt.set_attribute(
        UNumberFormatAttribute.UNUM_SCALE,
        -2)
    assert isinstance(result, DecimalFormat)
    assert id(result) == id(fmt)
    assert fmt.get_attribute(UNumberFormatAttribute.UNUM_SCALE) == -2


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_api_53():
    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # void icu::DecimalFormat::setGroupingUsed(UBool newValue)
    fmt.set_grouping_used(True)

    # void icu::DecimalFormat::setParseIntegerOnly(UBool value)
    fmt.set_parse_integer_only(True)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_api_54():
    from icupy.icu import UCurrencyUsage

    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # UCurrencyUsage icu::DecimalFormat::getCurrencyUsage()
    assert fmt.get_currency_usage() == UCurrencyUsage.UCURR_USAGE_STANDARD

    # void icu::DecimalFormat::setCurrencyUsage(
    #       UCurrencyUsage newUsage,
    #       UErrorCode *ec
    # )
    fmt.set_currency_usage(UCurrencyUsage.UCURR_USAGE_CASH)
    assert fmt.get_currency_usage() == UCurrencyUsage.UCURR_USAGE_CASH

    # UBool icu::DecimalFormat::isDecimalPatternMatchRequired(void)
    assert not fmt.is_decimal_pattern_match_required()

    # void icu::DecimalFormat::setDecimalPatternMatchRequired(UBool newValue)
    fmt.set_decimal_pattern_match_required(True)
    assert fmt.is_decimal_pattern_match_required()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_api_62():
    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # int32_t icu::DecimalFormat::getMultiplierScale(void)
    assert fmt.get_multiplier_scale() == 0

    # void icu::DecimalFormat::setMultiplierScale(int32_t newValue)
    fmt.set_multiplier_scale(2)
    assert fmt.get_multiplier_scale() == 2


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_api_64():
    from icupy.icu.number import LocalizedNumberFormatter

    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("0", symbols)

    # int32_t icu::DecimalFormat::getMinimumGroupingDigits()
    assert fmt.get_minimum_grouping_digits() == -1

    # void icu::DecimalFormat::setMinimumGroupingDigits(int32_t newValue)
    fmt.set_minimum_grouping_digits(1)
    assert fmt.get_minimum_grouping_digits() == 1

    # UBool icu::DecimalFormat::isFormatFailIfMoreThanMaxDigits()
    assert not fmt.is_format_fail_if_more_than_max_digits()

    # void icu::DecimalFormat::setFormatFailIfMoreThanMaxDigits(UBool value)
    fmt.set_format_fail_if_more_than_max_digits(True)
    assert fmt.is_format_fail_if_more_than_max_digits()

    # UBool icu::DecimalFormat::isParseCaseSensitive()
    assert not fmt.is_parse_case_sensitive()

    # void icu::DecimalFormat::setParseCaseSensitive(UBool value)
    fmt.set_parse_case_sensitive(True)
    assert fmt.is_parse_case_sensitive()

    # UBool icu::DecimalFormat::isParseNoExponent()
    assert not fmt.is_parse_no_exponent()

    # void icu::DecimalFormat::setParseNoExponent(UBool value)
    fmt.set_parse_no_exponent(True)
    assert fmt.is_parse_no_exponent()

    # UBool icu::DecimalFormat::isSignAlwaysShown()
    assert not fmt.is_sign_always_shown()

    # void icu::DecimalFormat::setSignAlwaysShown(UBool value)
    fmt.set_sign_always_shown(True)
    assert fmt.is_sign_always_shown()

    # const number::LocalizedNumberFormatter *
    # icu::DecimalFormat::toNumberFormatter(UErrorCode &status)
    result = fmt.to_number_formatter()
    assert isinstance(result, LocalizedNumberFormatter)


def test_apply_localized_pattern():
    fmt = DecimalFormat()

    # [1]
    # void icu::DecimalFormat::applyLocalizedPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt.apply_localized_pattern(UnicodeString("0"))

    # UnicodeString &icu::DecimalFormat::toLocalizedPattern(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = fmt.to_localized_pattern(result)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(output)
    assert result == "0"

    fmt.apply_localized_pattern("#,##0.#")
    output = fmt.to_localized_pattern(result)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(output)
    assert result == "#,##0.#"

    # [2]
    # void icu::DecimalFormat::applyLocalizedPattern(
    #       const UnicodeString &pattern,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = UParseError()
    fmt.apply_localized_pattern(UnicodeString("0"), parse_error)
    assert parse_error.line <= 0
    assert parse_error.offset == 0

    parse_error = UParseError()
    fmt.apply_localized_pattern("0", parse_error)
    assert parse_error.line <= 0
    assert parse_error.offset == 0


def test_apply_pattern():
    fmt = DecimalFormat()

    # [1]
    # void icu::DecimalFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt.apply_pattern(UnicodeString("0"))

    # UnicodeString &icu::DecimalFormat::toPattern(UnicodeString &result)
    result = UnicodeString()
    output = fmt.to_pattern(result)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(output)
    assert result == "0"

    fmt.apply_pattern("#,##0.#")
    output = fmt.to_pattern(result)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(output)
    assert result == "#,##0.#"

    # [2]
    # void icu::DecimalFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = UParseError()
    fmt.apply_pattern(UnicodeString("0"), parse_error)
    assert parse_error.line <= 0
    assert parse_error.offset == 0

    parse_error = UParseError()
    fmt.apply_pattern("0", parse_error)
    assert parse_error.line <= 0
    assert parse_error.offset == 0


def test_clone():
    fmt1 = DecimalFormat()

    # DecimalFormat *icu::DecimalFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, DecimalFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_decimal_format():
    assert issubclass(DecimalFormat, NumberFormat)

    # [1]
    # icu::DecimalFormat::DecimalFormat(UErrorCode &status)
    fmt1 = DecimalFormat()

    # [2]
    # icu::DecimalFormat::DecimalFormat(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2 = DecimalFormat(UnicodeString("0"))
    assert fmt1 != fmt2

    fmt2a = DecimalFormat("0")
    assert fmt2 == fmt2a

    # [6]
    # icu::DecimalFormat::DecimalFormat(
    #       const UnicodeString &pattern,
    #       const DecimalFormatSymbols &symbols,
    #       UErrorCode &status
    # )
    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt6 = DecimalFormat(UnicodeString("0"), symbols)

    fmt6a = DecimalFormat("0", symbols)
    assert fmt6 == fmt6a

    # [7]
    # icu::DecimalFormat::DecimalFormat(const DecimalFormat &source)
    fmt7 = DecimalFormat(fmt1)
    assert fmt1 == fmt7


def test_format():
    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("#,##0.#", symbols)
    d = -10456.0037
    s = "-1.045600e+04"
    n32 = INT32_MAX
    n64 = INT32_MAX + 1
    append_to = UnicodeString()

    # [1]
    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(Formattable(d), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    # [2]
    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(Formattable(d), append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    append_to.remove()
    result = fmt.format(Formattable(d), append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    # [7]
    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(d, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    # [8], [9]
    # UnicodeString &icu::DecimalFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(d, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    # [12], [13]
    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(d, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    append_to.remove()
    result = fmt.format(d, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    # [14]
    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n32, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,647"

    # [15], [16]
    # UnicodeString &icu::DecimalFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n32, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,647"

    # [19], [20]
    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(n32, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,647"

    append_to.remove()
    result = fmt.format(n32, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,647"

    # [21]
    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n64, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,648"

    # [22], [23]
    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n64, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,648"

    # [26], [27]
    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(n64, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,648"

    append_to.remove()
    result = fmt.format(n64, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2,147,483,648"

    # [28], [29]
    # UnicodeString &icu::NumberFormat::format(
    #       StringPiece number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(s, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    append_to.remove()
    result = fmt.format(s, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"

    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(Formattable(d), append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10,456"


def test_parse():
    symbols = DecimalFormatSymbols(Locale.get_us())
    fmt = DecimalFormat("#,##0.#", symbols)
    d = -10456.0037
    s = "-10,456.0037"

    # [1], [2]
    # void icu::DecimalFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       ParsePosition &parsePosition
    # )
    result = Formattable()
    parse_position = ParsePosition()
    fmt.parse(UnicodeString(s), result, parse_position)
    assert parse_position.get_error_index() == -1
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

    result = Formattable()
    parse_position = ParsePosition()
    fmt.parse(s, result, parse_position)
    assert parse_position.get_error_index() == -1
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

    # [3]
    # void icu::DecimalFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    fmt.parse(UnicodeString(s), result)
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

    result = Formattable()
    fmt.parse(s, result)
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

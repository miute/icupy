import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 73:
    pytest.skip("ICU4C<73", allow_module_level=True)


def test_api():
    # icu::number::SimpleNumberFormatter::SimpleNumberFormatter()
    fmt1 = icu.number.SimpleNumberFormatter()

    # FormattedNumber icu::number::SimpleNumberFormatter::formatInt64(
    #       int64_t value,
    #       UErrorCode &status
    # ) const
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt1.format_int64(1)  # Default constructed formatter
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_STATE_ERROR


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM > 74, reason="ICU4C>74")
def test_deprecated_75():
    """Deprecated or Obsoleted in ICU 75."""
    snf = icu.number.SimpleNumberFormatter.for_locale_and_grouping_strategy(
        "en-US", icu.UNumberGroupingStrategy.UNUM_GROUPING_ON_ALIGNED
    )

    # void icu::number::SimpleNumber::truncateStart(
    #       uint32_t maximumIntegerDigits,
    #       UErrorCode &status
    # )
    num = icu.number.SimpleNumber.for_int64(918273645)
    num.multiply_by_power_of_ten(-4)
    num.truncate_start(4)

    result = snf.format(num)
    assert result.to_temp_string() == "1,827.3645"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = snf.format(num)  # Use of moved number
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_for_locale():
    # static SimpleNumberFormatter
    # icu::number::SimpleNumberFormatter::forLocale(
    #       const icu::Locale &locale,
    #       UErrorCode &status
    # )
    fmt1 = icu.number.SimpleNumberFormatter.for_locale(icu.Locale("de-CH"))
    assert isinstance(fmt1, icu.number.SimpleNumberFormatter)
    fv = fmt1.format_int64(-1000007)
    assert isinstance(fv, icu.number.FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"

    fmt2 = icu.number.SimpleNumberFormatter.for_locale("de-CH")
    assert isinstance(fmt2, icu.number.SimpleNumberFormatter)
    fv = fmt2.format_int64(-1000007)
    assert isinstance(fv, icu.number.FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"


def test_for_locale_and_grouping_strategy():
    # static SimpleNumberFormatter
    # icu::number::SimpleNumberFormatter::forLocaleAndGroupingStrategy(
    #       const icu::Locale &locale,
    #       UNumberGroupingStrategy groupingStrategy,
    #       UErrorCode &status
    # )
    fmt1 = icu.number.SimpleNumberFormatter.for_locale_and_grouping_strategy(
        icu.Locale("de-CH"), icu.UNumberGroupingStrategy.UNUM_GROUPING_AUTO
    )
    assert isinstance(fmt1, icu.number.SimpleNumberFormatter)
    fv = fmt1.format_int64(-1000007)
    assert isinstance(fv, icu.number.FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"

    fmt2 = icu.number.SimpleNumberFormatter.for_locale_and_grouping_strategy(
        "de-CH", icu.UNumberGroupingStrategy.UNUM_GROUPING_AUTO
    )
    assert isinstance(fmt2, icu.number.SimpleNumberFormatter)
    fv = fmt2.format_int64(-1000007)
    assert isinstance(fv, icu.number.FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"


def test_for_locale_and_symbols_and_grouping_strategy():
    # static SimpleNumberFormatter
    # icu::number::SimpleNumberFormatter::forLocaleAndSymbolsAndGroupingStrategy(
    #   	const icu::Locale &locale,
    #       const DecimalFormatSymbols &symbols,
    #       UNumberGroupingStrategy groupingStrategy,
    #       UErrorCode &status
    # )
    symbols = icu.DecimalFormatSymbols("bn")
    fmt1 = icu.number.SimpleNumberFormatter.for_locale_and_symbols_and_grouping_strategy(
        icu.Locale("en-US"),
        symbols,
        icu.UNumberGroupingStrategy.UNUM_GROUPING_ON_ALIGNED,
    )
    assert isinstance(fmt1, icu.number.SimpleNumberFormatter)
    fv = fmt1.format_int64(987654321)
    assert isinstance(fv, icu.number.FormattedNumber)
    assert fv.to_temp_string() == "৯৮৭,৬৫৪,৩২১"

    fmt2 = icu.number.SimpleNumberFormatter.for_locale_and_symbols_and_grouping_strategy(
        "en-US",
        symbols,
        icu.UNumberGroupingStrategy.UNUM_GROUPING_ON_ALIGNED,
    )
    assert isinstance(fmt2, icu.number.SimpleNumberFormatter)
    fv = fmt2.format_int64(987654321)
    assert isinstance(fv, icu.number.FormattedNumber)
    assert fv.to_temp_string() == "৯৮৭,৬৫৪,৩২১"


def test_format_73():
    fmt = icu.number.SimpleNumberFormatter.for_locale("bn")

    # icu::number::SimpleNumber::SimpleNumber()
    value = icu.number.SimpleNumber()

    # FormattedNumber icu::number::SimpleNumberFormatter::format(
    #       SimpleNumber value,
    #       UErrorCode &status
    # ) const
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(value)  # Default constructed number
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # static SimpleNumber icu::number::SimpleNumber::forInt64(
    #       int64_t value,
    #       UErrorCode &status
    # )
    value = icu.number.SimpleNumber.for_int64(1250000)
    assert isinstance(value, icu.number.SimpleNumber)

    # void icu::number::SimpleNumber::multiplyByPowerOfTen(
    #       int32_t power,
    #       UErrorCode &status
    # )
    value.multiply_by_power_of_ten(-2)

    # void icu::number::SimpleNumber::roundTo(
    #       int32_t power,
    #       UNumberFormatRoundingMode roundingMode,
    #       UErrorCode &status
    # )
    value.round_to(3, icu.UNumberFormatRoundingMode.UNUM_ROUND_HALFUP)

    # void icu::number::SimpleNumber::setMinimumFractionDigits(
    #       uint32_t minimumFractionDigits,
    #       UErrorCode &status
    # )
    value.set_minimum_fraction_digits(2)

    # void icu::number::SimpleNumber::setMinimumIntegerDigits(
    #       uint32_t minimumIntegerDigits,
    #       UErrorCode &status
    # )
    value.set_minimum_integer_digits(6)

    # void icu::number::SimpleNumber::setSign(
    #       USimpleNumberSign sign,
    #       UErrorCode &status
    # )
    value.set_sign(icu.USimpleNumberSign.UNUM_SIMPLE_NUMBER_NO_SIGN)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 75, reason="ICU4C<75")
def test_format_75():
    snf = icu.number.SimpleNumberFormatter.for_locale_and_grouping_strategy(
        "en-US", icu.UNumberGroupingStrategy.UNUM_GROUPING_ON_ALIGNED
    )

    # void icu::number::SimpleNumber::setMaximumIntegerDigits(
    #       uint32_t maximumIntegerDigits,
    #       UErrorCode &status
    # )
    num = icu.number.SimpleNumber.for_int64(918273645)
    num.multiply_by_power_of_ten(-4)
    num.set_maximum_integer_digits(4)
    result = snf.format(num)
    assert result.to_temp_string() == "1,827.3645"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = snf.format(num)  # Use of moved number
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

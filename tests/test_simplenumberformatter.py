import pytest

from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 73:
    pytest.skip("ICU4C<73", allow_module_level=True)

# fmt: off
from icupy.icu import (
    DecimalFormatSymbols, ICUError, Locale, UErrorCode,
    UNumberFormatRoundingMode, UNumberGroupingStrategy, USimpleNumberSign,
)
from icupy.icu.number import (
    FormattedNumber, SimpleNumber, SimpleNumberFormatter,
)

# fmt: on


def test_api():
    # icu::number::SimpleNumberFormatter::SimpleNumberFormatter()
    fmt1 = SimpleNumberFormatter()

    # FormattedNumber icu::number::SimpleNumberFormatter::formatInt64(
    #       int64_t value,
    #       UErrorCode &status
    # ) const
    with pytest.raises(ICUError) as exc_info:
        _ = fmt1.format_int64(1)  # Default constructed formatter
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_STATE_ERROR


def test_for_locale():
    # static SimpleNumberFormatter
    # icu::number::SimpleNumberFormatter::forLocale(
    #       const icu::Locale &locale,
    #       UErrorCode &status
    # )
    fmt1 = SimpleNumberFormatter.for_locale(Locale("de-CH"))
    assert isinstance(fmt1, SimpleNumberFormatter)
    fv = fmt1.format_int64(-1000007)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"

    fmt2 = SimpleNumberFormatter.for_locale("de-CH")
    assert isinstance(fmt2, SimpleNumberFormatter)
    fv = fmt2.format_int64(-1000007)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"


def test_for_locale_and_grouping_strategy():
    # static SimpleNumberFormatter
    # icu::number::SimpleNumberFormatter::forLocaleAndGroupingStrategy(
    #       const icu::Locale &locale,
    #       UNumberGroupingStrategy groupingStrategy,
    #       UErrorCode &status
    # )
    fmt1 = SimpleNumberFormatter.for_locale_and_grouping_strategy(
        Locale("de-CH"), UNumberGroupingStrategy.UNUM_GROUPING_AUTO
    )
    assert isinstance(fmt1, SimpleNumberFormatter)
    fv = fmt1.format_int64(-1000007)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"

    fmt2 = SimpleNumberFormatter.for_locale_and_grouping_strategy(
        "de-CH", UNumberGroupingStrategy.UNUM_GROUPING_AUTO
    )
    assert isinstance(fmt2, SimpleNumberFormatter)
    fv = fmt2.format_int64(-1000007)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "-1’000’007"


def test_for_locale_and_symbols_and_grouping_strategy():
    # static SimpleNumberFormatter
    # icu::number::SimpleNumberFormatter::forLocaleAndSymbolsAndGroupingStrategy(
    #   	const icu::Locale &locale,
    #       const DecimalFormatSymbols &symbols,
    #       UNumberGroupingStrategy groupingStrategy,
    #       UErrorCode &status
    # )
    symbols = DecimalFormatSymbols("bn")
    fmt1 = SimpleNumberFormatter.for_locale_and_symbols_and_grouping_strategy(
        Locale("en-US"),
        symbols,
        UNumberGroupingStrategy.UNUM_GROUPING_ON_ALIGNED,
    )
    assert isinstance(fmt1, SimpleNumberFormatter)
    fv = fmt1.format_int64(987654321)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "৯৮৭,৬৫৪,৩২১"

    fmt2 = SimpleNumberFormatter.for_locale_and_symbols_and_grouping_strategy(
        "en-US",
        symbols,
        UNumberGroupingStrategy.UNUM_GROUPING_ON_ALIGNED,
    )
    assert isinstance(fmt2, SimpleNumberFormatter)
    fv = fmt2.format_int64(987654321)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "৯৮৭,৬৫৪,৩২১"


def test_format():
    fmt = SimpleNumberFormatter.for_locale("bn")

    # icu::number::SimpleNumber::SimpleNumber()
    value = SimpleNumber()

    # FormattedNumber icu::number::SimpleNumberFormatter::format(
    #       SimpleNumber value,
    #       UErrorCode &status
    # ) const
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(value)  # Default constructed number
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # static SimpleNumber icu::number::SimpleNumber::forInt64(
    #       int64_t value,
    #       UErrorCode &status
    # )
    value = SimpleNumber.for_int64(1250000)
    assert isinstance(value, SimpleNumber)

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
    value.round_to(3, UNumberFormatRoundingMode.UNUM_ROUND_HALFUP)

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
    value.set_sign(USimpleNumberSign.UNUM_SIMPLE_NUMBER_NO_SIGN)

    # void icu::number::SimpleNumber::truncateStart(
    #       uint32_t maximumIntegerDigits,
    #       UErrorCode &status
    # )
    value.truncate_start(4)

    fv = fmt.format(value)
    assert isinstance(fv, FormattedNumber)
    assert fv.to_temp_string() == "০,০৩,০০০.০০"

    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(value)  # Use of moved number
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

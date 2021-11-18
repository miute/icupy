import pytest
from icupy.icu import (
    DecimalFormat, Format, Locale, NumberFormat, UNumberFormatStyle,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    assert issubclass(NumberFormat, Format)
    assert issubclass(DecimalFormat, NumberFormat)

    # [1]
    # static const Locale *icu::NumberFormat::getAvailableLocales(
    #       int32_t &count
    # )
    it = NumberFormat.get_available_locales()
    assert isinstance(it, list)
    assert len(it) > 0
    assert all(isinstance(x, Locale) for x in it)

    fmt = NumberFormat.create_currency_instance(Locale.get_us())

    # const char16_t *icu::NumberFormat::getCurrency()
    result = fmt.get_currency()
    assert isinstance(result, str)
    assert result == "USD"

    # int32_t icu::NumberFormat::getMaximumFractionDigits(void)
    assert fmt.get_maximum_fraction_digits() == 2

    # int32_t icu::NumberFormat::getMaximumIntegerDigits(void)
    assert fmt.get_maximum_integer_digits() == 2e+9

    # int32_t icu::NumberFormat::getMinimumFractionDigits(void)
    assert fmt.get_minimum_fraction_digits() == 2

    # int32_t icu::NumberFormat::getMinimumIntegerDigits(void)
    assert fmt.get_minimum_integer_digits() == 1

    # UBool icu::NumberFormat::isGroupingUsed(void)
    assert fmt.is_grouping_used()

    # UBool icu::NumberFormat::isParseIntegerOnly(void)
    assert not fmt.is_parse_integer_only()


def test_create_currency_instance():
    default_locale = Locale.get_default()

    try:
        in_locale = Locale.get_us()
        Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createCurrencyInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt1 = NumberFormat.create_currency_instance(in_locale)
        assert isinstance(fmt1, DecimalFormat)

        fmt1a = NumberFormat.create_currency_instance("en-US")
        assert isinstance(fmt1a, DecimalFormat)
        assert fmt1 == fmt1a

        append_to = UnicodeString()
        assert fmt1.format(1000, append_to) == "$1,000.00"

        # [2]
        # static NumberFormat *icu::NumberFormat::createCurrencyInstance(
        #       UErrorCode &
        # )
        fmt2 = NumberFormat.create_currency_instance()
        assert isinstance(fmt2, DecimalFormat)
        assert fmt1 == fmt2

        append_to.remove()
        assert fmt2.format(1000, append_to) == "$1,000.00"
    finally:
        Locale.set_default(default_locale)


def test_create_instance():
    default_locale = Locale.get_default()

    try:
        in_locale = Locale.get_us()
        Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createInstance(
        #       const Locale &desiredLocale,
        #       UNumberFormatStyle style,
        #       UErrorCode &errorCode
        # )
        fmt1 = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_DEFAULT)
        assert isinstance(fmt1, DecimalFormat)

        fmt1a = NumberFormat.create_instance(
            "en-US",
            UNumberFormatStyle.UNUM_DEFAULT)
        assert isinstance(fmt1a, DecimalFormat)
        assert fmt1 == fmt1a

        append_to = UnicodeString()
        assert fmt1.format(1000, append_to) == "1,000"

        # UNUM_PATTERN_DECIMAL -> U_UNSUPPORTED_ERROR
        # test = NumberFormat.create_instance(
        #     in_locale,
        #     UNumberFormatStyle.UNUM_PATTERN_DECIMAL)

        # UNUM_DECIMAL
        test = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_DECIMAL)
        assert isinstance(test, DecimalFormat)

        # UNUM_CURRENCY
        test = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_CURRENCY)
        assert isinstance(test, DecimalFormat)

        # UNUM_PERCENT
        test = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_PERCENT)
        assert isinstance(test, DecimalFormat)

        # UNUM_SCIENTIFIC
        test = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_SCIENTIFIC)
        assert isinstance(test, DecimalFormat)

        # UNUM_SPELLOUT -> U_UNSUPPORTED_ERROR
        # test = NumberFormat.create_instance(
        #     in_locale,
        #     UNumberFormatStyle.UNUM_SPELLOUT)

        # UNUM_ORDINAL -> U_UNSUPPORTED_ERROR
        # test = NumberFormat.create_instance(
        #     in_locale,
        #     UNumberFormatStyle.UNUM_ORDINAL)

        # UNUM_DURATION -> U_UNSUPPORTED_ERROR
        # test = NumberFormat.create_instance(
        #     in_locale,
        #     UNumberFormatStyle.UNUM_DURATION)

        # UNUM_NUMBERING_SYSTEM -> U_UNSUPPORTED_ERROR
        # test = NumberFormat.create_instance(
        #     in_locale,
        #     UNumberFormatStyle.UNUM_NUMBERING_SYSTEM)

        # UNUM_PATTERN_RULEBASED -> U_UNSUPPORTED_ERROR
        # test = NumberFormat.create_instance(
        #     in_locale,
        #     UNumberFormatStyle.UNUM_PATTERN_RULEBASED)

        # UNUM_CURRENCY_ISO
        test = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_CURRENCY_ISO)
        assert isinstance(test, DecimalFormat)

        # UNUM_CURRENCY_PLURAL
        test = NumberFormat.create_instance(
            in_locale,
            UNumberFormatStyle.UNUM_CURRENCY_PLURAL)
        assert isinstance(test, DecimalFormat)

        # [2]
        # static NumberFormat *icu::NumberFormat::createInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt2 = NumberFormat.create_instance(in_locale)
        assert isinstance(fmt2, DecimalFormat)

        fmt2a = NumberFormat.create_instance("en-US")
        assert isinstance(fmt2a, DecimalFormat)
        assert fmt2 == fmt2a

        append_to.remove()
        assert fmt2.format(1000, append_to) == "1,000"

        # [3]
        # static NumberFormat *icu::NumberFormat::createInstance(UErrorCode &)
        fmt3 = NumberFormat.create_instance()
        assert isinstance(fmt3, DecimalFormat)
        assert fmt1 == fmt2 == fmt3

        append_to.remove()
        assert fmt3.format(1000, append_to) == "1,000"
    finally:
        Locale.set_default(default_locale)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_create_instance_53():
    # UNUM_CURRENCY_ACCOUNTING
    test = NumberFormat.create_instance(
        Locale.get_us(),
        UNumberFormatStyle.UNUM_CURRENCY_ACCOUNTING)
    assert isinstance(test, DecimalFormat)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_create_instance_54():
    # UNUM_CASH_CURRENCY
    test = NumberFormat.create_instance(
        Locale.get_us(),
        UNumberFormatStyle.UNUM_CASH_CURRENCY)
    assert isinstance(test, DecimalFormat)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 56, reason="ICU4C<56")
def test_create_instance_56():
    # UNUM_DECIMAL_COMPACT_SHORT -> U_UNSUPPORTED_ERROR
    # test = NumberFormat.create_instance(
    #     Locale.get_us(),
    #     UNumberFormatStyle.UNUM_DECIMAL_COMPACT_SHORT)

    # UNUM_DECIMAL_COMPACT_LONG -> U_UNSUPPORTED_ERROR
    # test = NumberFormat.create_instance(
    #     Locale.get_us(),
    #     UNumberFormatStyle.UNUM_DECIMAL_COMPACT_LONG)

    # UNUM_CURRENCY_STANDARD
    test = NumberFormat.create_instance(
        Locale.get_us(),
        UNumberFormatStyle.UNUM_CURRENCY_STANDARD)
    assert isinstance(test, DecimalFormat)


def test_create_percent_instance():
    default_locale = Locale.get_default()

    try:
        in_locale = Locale.get_us()
        Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createPercentInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt1 = NumberFormat.create_percent_instance(in_locale)
        assert isinstance(fmt1, DecimalFormat)

        fmt1a = NumberFormat.create_percent_instance("en-US")
        assert isinstance(fmt1a, DecimalFormat)
        assert fmt1 == fmt1a

        append_to = UnicodeString()
        assert fmt1.format(0.8, append_to) == "80%"

        # [2]
        # static NumberFormat *icu::NumberFormat::createPercentInstance(
        #       UErrorCode &
        # )
        fmt2 = NumberFormat.create_percent_instance()
        assert isinstance(fmt2, DecimalFormat)
        assert fmt1 == fmt2

        append_to.remove()
        assert fmt2.format(0.8, append_to) == "80%"
    finally:
        Locale.set_default(default_locale)


def test_create_scientific_instance():
    default_locale = Locale.get_default()

    try:
        in_locale = Locale.get_us()
        Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createScientificInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt1 = NumberFormat.create_scientific_instance(in_locale)
        assert isinstance(fmt1, DecimalFormat)

        fmt1a = NumberFormat.create_scientific_instance("en-US")
        assert isinstance(fmt1a, DecimalFormat)
        assert fmt1 == fmt1a

        append_to = UnicodeString()
        assert fmt1.format(1000, append_to) == "1E3"

        # [2]
        # static NumberFormat *icu::NumberFormat::createScientificInstance(
        #       UErrorCode &
        # )
        fmt2 = NumberFormat.create_scientific_instance()
        assert isinstance(fmt2, DecimalFormat)
        assert fmt1 == fmt2

        append_to.remove()
        assert fmt2.format(1000, append_to) == "1E3"
    finally:
        Locale.set_default(default_locale)

import pytest

from icupy import icu


def test_api():
    assert issubclass(icu.NumberFormat, icu.Format)
    assert issubclass(icu.DecimalFormat, icu.NumberFormat)

    # [1]
    # static const Locale *icu::NumberFormat::getAvailableLocales(
    #       int32_t &count
    # )
    it = icu.NumberFormat.get_available_locales()
    assert isinstance(it, list)
    assert len(it) > 0
    assert all(isinstance(x, icu.Locale) for x in it)

    fmt = icu.NumberFormat.create_currency_instance(icu.Locale.get_us())

    # const char16_t *icu::NumberFormat::getCurrency()
    result = fmt.get_currency()
    assert isinstance(result, str)
    assert result == "USD"

    # int32_t icu::NumberFormat::getMaximumFractionDigits(void)
    assert fmt.get_maximum_fraction_digits() == 2

    # int32_t icu::NumberFormat::getMaximumIntegerDigits(void)
    assert fmt.get_maximum_integer_digits() == 2e9

    # int32_t icu::NumberFormat::getMinimumFractionDigits(void)
    assert fmt.get_minimum_fraction_digits() == 2

    # int32_t icu::NumberFormat::getMinimumIntegerDigits(void)
    assert fmt.get_minimum_integer_digits() == 1

    # UBool icu::NumberFormat::isGroupingUsed(void)
    assert fmt.is_grouping_used() is True

    # UBool icu::NumberFormat::isLenient(void) const
    assert fmt.is_lenient() is False

    # UBool icu::NumberFormat::isParseIntegerOnly(void)
    assert fmt.is_parse_integer_only() is False


def test_create_currency_instance():
    default_locale = icu.Locale.get_default()

    try:
        in_locale = icu.Locale.get_us()
        icu.Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createCurrencyInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt1 = icu.NumberFormat.create_currency_instance(in_locale)
        assert isinstance(fmt1, icu.DecimalFormat)

        fmt1a = icu.NumberFormat.create_currency_instance("en-US")
        assert isinstance(fmt1a, icu.DecimalFormat)
        assert fmt1 == fmt1a

        append_to = icu.UnicodeString()
        assert fmt1.format(1000, append_to) == "$1,000.00"

        # [2]
        # static NumberFormat *icu::NumberFormat::createCurrencyInstance(
        #       UErrorCode &
        # )
        fmt2 = icu.NumberFormat.create_currency_instance()
        assert isinstance(fmt2, icu.DecimalFormat)
        assert fmt1 == fmt2

        append_to.remove()
        assert fmt2.format(1000, append_to) == "$1,000.00"
    finally:
        icu.Locale.set_default(default_locale)


def test_create_instance():
    default_locale = icu.Locale.get_default()

    try:
        in_locale = icu.Locale.get_us()
        icu.Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createInstance(
        #       const Locale &desiredLocale,
        #       UNumberFormatStyle style,
        #       UErrorCode &errorCode
        # )
        fmt1 = icu.NumberFormat.create_instance(in_locale, icu.UNumberFormatStyle.UNUM_DEFAULT)
        assert isinstance(fmt1, icu.DecimalFormat)

        fmt1a = icu.NumberFormat.create_instance("en-US", icu.UNumberFormatStyle.UNUM_DEFAULT)
        assert isinstance(fmt1a, icu.DecimalFormat)
        assert fmt1 == fmt1a

        append_to = icu.UnicodeString()
        assert fmt1.format(1000, append_to) == "1,000"

        # UNUM_PATTERN_DECIMAL -> U_UNSUPPORTED_ERROR (ICU 69+)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.NumberFormat.create_instance(
                in_locale, icu.UNumberFormatStyle.UNUM_PATTERN_DECIMAL
            )
        assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

        # UNUM_DECIMAL
        test = icu.NumberFormat.create_instance(in_locale, icu.UNumberFormatStyle.UNUM_DECIMAL)
        assert isinstance(test, icu.DecimalFormat)

        # UNUM_CURRENCY
        test = icu.NumberFormat.create_instance(in_locale, icu.UNumberFormatStyle.UNUM_CURRENCY)
        assert isinstance(test, icu.DecimalFormat)

        # UNUM_PERCENT
        test = icu.NumberFormat.create_instance(in_locale, icu.UNumberFormatStyle.UNUM_PERCENT)
        assert isinstance(test, icu.DecimalFormat)

        # UNUM_SCIENTIFIC
        test = icu.NumberFormat.create_instance(
            in_locale, icu.UNumberFormatStyle.UNUM_SCIENTIFIC
        )
        assert isinstance(test, icu.DecimalFormat)

        # UNUM_SPELLOUT -> U_UNSUPPORTED_ERROR (ICU 69+)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.NumberFormat.create_instance(
                in_locale, icu.UNumberFormatStyle.UNUM_SPELLOUT
            )
        assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

        # UNUM_ORDINAL -> U_UNSUPPORTED_ERROR (ICU 69+)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.NumberFormat.create_instance(in_locale, icu.UNumberFormatStyle.UNUM_ORDINAL)
        assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

        # UNUM_DURATION -> U_UNSUPPORTED_ERROR (ICU 69+)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.NumberFormat.create_instance(
                in_locale, icu.UNumberFormatStyle.UNUM_DURATION
            )
        assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

        # UNUM_NUMBERING_SYSTEM -> U_UNSUPPORTED_ERROR (69<=ICU<72)
        if icu.U_ICU_VERSION_MAJOR_NUM >= 72:
            test = icu.NumberFormat.create_instance(
                in_locale, icu.UNumberFormatStyle.UNUM_NUMBERING_SYSTEM
            )
            assert isinstance(test, icu.DecimalFormat)
        else:  # U_ICU_VERSION_MAJOR_NUM < 72
            with pytest.raises(icu.ICUError) as exc_info:
                _ = icu.NumberFormat.create_instance(
                    in_locale, icu.UNumberFormatStyle.UNUM_NUMBERING_SYSTEM
                )
            assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

        # UNUM_PATTERN_RULEBASED -> U_UNSUPPORTED_ERROR (ICU 69+)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.NumberFormat.create_instance(
                in_locale, icu.UNumberFormatStyle.UNUM_PATTERN_RULEBASED
            )
        assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

        # UNUM_CURRENCY_ISO
        test = icu.NumberFormat.create_instance(
            in_locale, icu.UNumberFormatStyle.UNUM_CURRENCY_ISO
        )
        assert isinstance(test, icu.DecimalFormat)

        # UNUM_CURRENCY_PLURAL
        test = icu.NumberFormat.create_instance(
            in_locale, icu.UNumberFormatStyle.UNUM_CURRENCY_PLURAL
        )
        assert isinstance(test, icu.DecimalFormat)

        # [2]
        # static NumberFormat *icu::NumberFormat::createInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt2 = icu.NumberFormat.create_instance(in_locale)
        assert isinstance(fmt2, icu.DecimalFormat)

        fmt2a = icu.NumberFormat.create_instance("en-US")
        assert isinstance(fmt2a, icu.DecimalFormat)
        assert fmt2 == fmt2a

        append_to.remove()
        assert fmt2.format(1000, append_to) == "1,000"

        # [3]
        # static NumberFormat *icu::NumberFormat::createInstance(UErrorCode &)
        fmt3 = icu.NumberFormat.create_instance()
        assert isinstance(fmt3, icu.DecimalFormat)
        assert fmt1 == fmt2 == fmt3

        append_to.remove()
        assert fmt3.format(1000, append_to) == "1,000"
    finally:
        icu.Locale.set_default(default_locale)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_create_instance_53():
    # UNUM_CURRENCY_ACCOUNTING
    test = icu.NumberFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberFormatStyle.UNUM_CURRENCY_ACCOUNTING
    )
    assert isinstance(test, icu.DecimalFormat)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_create_instance_54():
    # UNUM_CASH_CURRENCY
    test = icu.NumberFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberFormatStyle.UNUM_CASH_CURRENCY
    )
    assert isinstance(test, icu.DecimalFormat)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 56, reason="ICU4C<56")
def test_create_instance_56():
    # UNUM_DECIMAL_COMPACT_SHORT -> U_UNSUPPORTED_ERROR (ICU 69+)
    with pytest.raises(icu.ICUError) as exc_info:
        _ = icu.NumberFormat.create_instance(
            icu.Locale.get_us(),
            icu.UNumberFormatStyle.UNUM_DECIMAL_COMPACT_SHORT,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # UNUM_DECIMAL_COMPACT_LONG -> U_UNSUPPORTED_ERROR (ICU 69+)
    with pytest.raises(icu.ICUError) as exc_info:
        _ = icu.NumberFormat.create_instance(
            icu.Locale.get_us(),
            icu.UNumberFormatStyle.UNUM_DECIMAL_COMPACT_LONG,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # UNUM_CURRENCY_STANDARD
    test = icu.NumberFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberFormatStyle.UNUM_CURRENCY_STANDARD
    )
    assert isinstance(test, icu.DecimalFormat)


def test_create_percent_instance():
    default_locale = icu.Locale.get_default()

    try:
        in_locale = icu.Locale.get_us()
        icu.Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createPercentInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt1 = icu.NumberFormat.create_percent_instance(in_locale)
        assert isinstance(fmt1, icu.DecimalFormat)

        fmt1a = icu.NumberFormat.create_percent_instance("en-US")
        assert isinstance(fmt1a, icu.DecimalFormat)
        assert fmt1 == fmt1a

        append_to = icu.UnicodeString()
        assert fmt1.format(0.8, append_to) == "80%"

        # [2]
        # static NumberFormat *icu::NumberFormat::createPercentInstance(
        #       UErrorCode &
        # )
        fmt2 = icu.NumberFormat.create_percent_instance()
        assert isinstance(fmt2, icu.DecimalFormat)
        assert fmt1 == fmt2

        append_to.remove()
        assert fmt2.format(0.8, append_to) == "80%"
    finally:
        icu.Locale.set_default(default_locale)


def test_create_scientific_instance():
    default_locale = icu.Locale.get_default()

    try:
        in_locale = icu.Locale.get_us()
        icu.Locale.set_default(in_locale)

        # [1]
        # static NumberFormat *icu::NumberFormat::createScientificInstance(
        #       const Locale &inLocale,
        #       UErrorCode &
        # )
        fmt1 = icu.NumberFormat.create_scientific_instance(in_locale)
        assert isinstance(fmt1, icu.DecimalFormat)

        fmt1a = icu.NumberFormat.create_scientific_instance("en-US")
        assert isinstance(fmt1a, icu.DecimalFormat)
        assert fmt1 == fmt1a

        append_to = icu.UnicodeString()
        assert fmt1.format(1000, append_to) == "1E3"

        # [2]
        # static NumberFormat *icu::NumberFormat::createScientificInstance(
        #       UErrorCode &
        # )
        fmt2 = icu.NumberFormat.create_scientific_instance()
        assert isinstance(fmt2, icu.DecimalFormat)
        assert fmt1 == fmt2

        append_to.remove()
        assert fmt2.format(1000, append_to) == "1E3"
    finally:
        icu.Locale.set_default(default_locale)

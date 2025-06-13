import pytest

from icupy import icu


def test_api():
    sym1 = icu.DecimalFormatSymbols(icu.Locale.get_us())
    sym2 = icu.DecimalFormatSymbols(icu.Locale.get_france())
    sym3 = icu.DecimalFormatSymbols(sym1)

    # UBool icu::DecimalFormatSymbols::operator!=(
    #       const DecimalFormatSymbols &other
    # )
    assert sym1 != sym2
    assert not (sym1 != sym3)
    assert sym2 != sym3

    # UBool icu::DecimalFormatSymbols::operator==(
    #       const DecimalFormatSymbols &other
    # )
    assert not (sym1 == sym2)
    assert sym1 == sym3
    assert not (sym2 == sym3)

    # [1]
    # Locale icu::DecimalFormatSymbols::getLocale(void)
    result = sym1.get_locale()
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("en_US")

    # [2]
    # Locale icu::DecimalFormatSymbols::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    result = sym1.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("en_US")

    # const UnicodeString &
    # icu::DecimalFormatSymbols::getPatternForCurrencySpacing(
    #       UCurrencySpacing type,
    #       UBool beforeCurrency,
    #       UErrorCode &status
    # )
    result = sym1.get_pattern_for_currency_spacing(
        icu.UCurrencySpacing.UNUM_CURRENCY_INSERT, True
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "\xa0"

    # UnicodeString icu::DecimalFormatSymbols::getSymbol(
    #       ENumberFormatSymbol symbol
    # )
    result = sym1.get_symbol(icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL)
    assert isinstance(result, icu.UnicodeString)
    assert result == "0"

    # void icu::DecimalFormatSymbols::setPatternForCurrencySpacing(
    #       UCurrencySpacing type,
    #       UBool beforeCurrency,
    #       const UnicodeString &pattern
    # )
    sym1.set_pattern_for_currency_spacing(
        icu.UCurrencySpacing.UNUM_CURRENCY_INSERT, True, icu.UnicodeString()
    )
    result = sym1.get_pattern_for_currency_spacing(
        icu.UCurrencySpacing.UNUM_CURRENCY_INSERT, True
    )
    assert isinstance(result, icu.UnicodeString)
    assert len(result) == 0

    sym1.set_pattern_for_currency_spacing(
        icu.UCurrencySpacing.UNUM_CURRENCY_INSERT, True, "\xa0"
    )
    result = sym1.get_pattern_for_currency_spacing(
        icu.UCurrencySpacing.UNUM_CURRENCY_INSERT, True
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "\xa0"

    # void icu::DecimalFormatSymbols::setSymbol(
    #       ENumberFormatSymbol symbol,
    #       const UnicodeString &value,
    #       const UBool propogateDigits = true
    # )
    sym1.set_symbol(
        icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL,
        icu.UnicodeString("\uff10"),
        False,
    )
    assert sym1.get_symbol(icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL) == "\uff10"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.ONE_DIGIT_SYMBOL) == "1"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.TWO_DIGIT_SYMBOL) == "2"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.THREE_DIGIT_SYMBOL) == "3"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.FOUR_DIGIT_SYMBOL) == "4"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.FIVE_DIGIT_SYMBOL) == "5"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.SIX_DIGIT_SYMBOL) == "6"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.SEVEN_DIGIT_SYMBOL) == "7"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.EIGHT_DIGIT_SYMBOL) == "8"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.NINE_DIGIT_SYMBOL) == "9"

    sym1.set_symbol(
        icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL,
        icu.UnicodeString("\uff10"),
        True,
    )
    assert sym1.get_symbol(icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL) == "\uff10"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.ONE_DIGIT_SYMBOL) == "\uff11"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.TWO_DIGIT_SYMBOL) == "\uff12"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.THREE_DIGIT_SYMBOL) == "\uff13"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.FOUR_DIGIT_SYMBOL) == "\uff14"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.FIVE_DIGIT_SYMBOL) == "\uff15"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.SIX_DIGIT_SYMBOL) == "\uff16"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.SEVEN_DIGIT_SYMBOL) == "\uff17"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.EIGHT_DIGIT_SYMBOL) == "\uff18"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.NINE_DIGIT_SYMBOL) == "\uff19"

    sym1.set_symbol(icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL, "0")
    assert sym1.get_symbol(icu.DecimalFormatSymbols.ZERO_DIGIT_SYMBOL) == "0"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.ONE_DIGIT_SYMBOL) == "1"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.TWO_DIGIT_SYMBOL) == "2"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.THREE_DIGIT_SYMBOL) == "3"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.FOUR_DIGIT_SYMBOL) == "4"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.FIVE_DIGIT_SYMBOL) == "5"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.SIX_DIGIT_SYMBOL) == "6"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.SEVEN_DIGIT_SYMBOL) == "7"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.EIGHT_DIGIT_SYMBOL) == "8"
    assert sym1.get_symbol(icu.DecimalFormatSymbols.NINE_DIGIT_SYMBOL) == "9"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_create_with_last_resort_data():
    # static DecimalFormatSymbols *
    # icu::DecimalFormatSymbols::createWithLastResortData(
    #       UErrorCode &status
    # )
    sym = icu.DecimalFormatSymbols.create_with_last_resort_data()
    assert isinstance(sym, icu.DecimalFormatSymbols)


def test_decimal_format_symbols():
    # [1]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    sym1 = icu.DecimalFormatSymbols(icu.Locale.get_us())
    sym1a = icu.DecimalFormatSymbols("en_US")
    assert sym1 == sym1a

    # [2]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(
    #       const Locale &locale,
    #       const NumberingSystem &ns,
    #       UErrorCode &status
    # )
    sym2 = icu.DecimalFormatSymbols(icu.Locale.get_us(), icu.NumberingSystem.create_instance())
    sym2a = icu.DecimalFormatSymbols("en_US", icu.NumberingSystem.create_instance())
    assert sym2 == sym2a

    # [3]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(UErrorCode &status)
    sym3 = icu.DecimalFormatSymbols()
    sym1b = icu.DecimalFormatSymbols(icu.Locale.get_default())
    assert sym3 == sym1b

    # [4]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(
    #       const DecimalFormatSymbols &
    # )
    sym4 = icu.DecimalFormatSymbols(sym1)
    assert sym4 == sym1


def test_numbering_system():
    # [1]
    # static NumberingSystem *icu::NumberingSystem::createInstance(
    #       const Locale &inLocale,
    #       UErrorCode &status
    # )
    ns1 = icu.NumberingSystem.create_instance(icu.Locale.get_us())
    assert isinstance(ns1, icu.NumberingSystem)

    ns2 = icu.NumberingSystem.create_instance("en_US")
    assert isinstance(ns2, icu.NumberingSystem)

    # [2]
    # static NumberingSystem *icu::NumberingSystem::createInstance(
    #       int32_t radix,
    #       UBool isAlgorithmic,
    #       const UnicodeString &description,
    #       UErrorCode &status
    # )
    ns3 = icu.NumberingSystem.create_instance(10, False, icu.UnicodeString("0123456789"))
    assert isinstance(ns3, icu.NumberingSystem)

    ns4 = icu.NumberingSystem.create_instance(10, False, "0123456789")
    assert isinstance(ns4, icu.NumberingSystem)

    # [3]
    # static NumberingSystem *icu::NumberingSystem::createInstance(
    #       UErrorCode &status
    # )
    ns5 = icu.NumberingSystem.create_instance()
    assert isinstance(ns5, icu.NumberingSystem)

    # static NumberingSystem *icu::NumberingSystem::createInstanceByName(
    #       const char *name,
    #       UErrorCode &status
    # )
    ns6 = icu.NumberingSystem.create_instance_by_name("latn")
    assert isinstance(ns6, icu.NumberingSystem)

    # [2]
    # icu::NumberingSystem::NumberingSystem(const NumberingSystem &other)
    ns7 = icu.NumberingSystem(ns1)

    # static StringEnumeration *icu::NumberingSystem::getAvailableNames(
    #       UErrorCode &status
    # )
    it = icu.NumberingSystem.get_available_names()
    assert isinstance(it, icu.StringEnumeration)
    assert len(it) > 0
    assert "latn" in it

    # UnicodeString icu::NumberingSystem::getDescription()
    result = ns7.get_description()
    assert isinstance(result, icu.UnicodeString)
    assert result == "0123456789"

    # const char *icu::NumberingSystem::getName()
    result = ns7.get_name()
    assert isinstance(result, str)
    assert result == "latn"

    # int32_t icu::NumberingSystem::getRadix()
    assert ns7.get_radix() == 10

    # UBool icu::NumberingSystem::isAlgorithmic()
    assert ns7.is_algorithmic() is False

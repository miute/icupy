import pytest
from icupy.icu import (
    DecimalFormatSymbols, Locale, NumberingSystem, StringEnumeration,
    UCurrencySpacing, ULocDataLocaleType, U_ICU_VERSION_MAJOR_NUM,
    UnicodeString,
)


def test_api():
    sym1 = DecimalFormatSymbols(Locale.get_us())
    sym2 = DecimalFormatSymbols(Locale.get_france())
    sym3 = DecimalFormatSymbols(sym1)

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
    assert isinstance(result, Locale)
    assert result == Locale("en_US")

    # [2]
    # Locale icu::DecimalFormatSymbols::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    result = sym1.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(result, Locale)
    assert result == Locale("en_US")

    # const UnicodeString &
    # icu::DecimalFormatSymbols::getPatternForCurrencySpacing(
    #       UCurrencySpacing type,
    #       UBool beforeCurrency,
    #       UErrorCode &status
    # )
    result = sym1.get_pattern_for_currency_spacing(
        UCurrencySpacing.UNUM_CURRENCY_INSERT,
        True)
    assert isinstance(result, UnicodeString)
    assert result == "\xa0"

    # UnicodeString icu::DecimalFormatSymbols::getSymbol(
    #       ENumberFormatSymbol symbol
    # )
    result = sym1.get_symbol(DecimalFormatSymbols.ZERO_DIGIT_SYMBOL)
    assert isinstance(result, UnicodeString)
    assert result == "0"

    # void icu::DecimalFormatSymbols::setPatternForCurrencySpacing(
    #       UCurrencySpacing type,
    #       UBool beforeCurrency,
    #       const UnicodeString &pattern
    # )
    sym1.set_pattern_for_currency_spacing(
        UCurrencySpacing.UNUM_CURRENCY_INSERT,
        True,
        UnicodeString())
    result = sym1.get_pattern_for_currency_spacing(
        UCurrencySpacing.UNUM_CURRENCY_INSERT,
        True)
    assert isinstance(result, UnicodeString)
    assert len(result) == 0

    sym1.set_pattern_for_currency_spacing(
        UCurrencySpacing.UNUM_CURRENCY_INSERT,
        True,
        "\xa0")
    result = sym1.get_pattern_for_currency_spacing(
        UCurrencySpacing.UNUM_CURRENCY_INSERT,
        True)
    assert isinstance(result, UnicodeString)
    assert result == "\xa0"

    # void icu::DecimalFormatSymbols::setSymbol(
    #       ENumberFormatSymbol symbol,
    #       const UnicodeString &value,
    #       const UBool propogateDigits = true
    # )
    sym1.set_symbol(
        DecimalFormatSymbols.ZERO_DIGIT_SYMBOL,
        UnicodeString("\uff10"),
        False)
    assert sym1.get_symbol(DecimalFormatSymbols.ZERO_DIGIT_SYMBOL) == "\uff10"
    assert sym1.get_symbol(DecimalFormatSymbols.ONE_DIGIT_SYMBOL) == "1"
    assert sym1.get_symbol(DecimalFormatSymbols.TWO_DIGIT_SYMBOL) == "2"
    assert sym1.get_symbol(DecimalFormatSymbols.THREE_DIGIT_SYMBOL) == "3"
    assert sym1.get_symbol(DecimalFormatSymbols.FOUR_DIGIT_SYMBOL) == "4"
    assert sym1.get_symbol(DecimalFormatSymbols.FIVE_DIGIT_SYMBOL) == "5"
    assert sym1.get_symbol(DecimalFormatSymbols.SIX_DIGIT_SYMBOL) == "6"
    assert sym1.get_symbol(DecimalFormatSymbols.SEVEN_DIGIT_SYMBOL) == "7"
    assert sym1.get_symbol(DecimalFormatSymbols.EIGHT_DIGIT_SYMBOL) == "8"
    assert sym1.get_symbol(DecimalFormatSymbols.NINE_DIGIT_SYMBOL) == "9"

    sym1.set_symbol(
        DecimalFormatSymbols.ZERO_DIGIT_SYMBOL,
        UnicodeString("\uff10"),
        True)
    assert sym1.get_symbol(DecimalFormatSymbols.ZERO_DIGIT_SYMBOL) == "\uff10"
    assert sym1.get_symbol(DecimalFormatSymbols.ONE_DIGIT_SYMBOL) == "\uff11"
    assert sym1.get_symbol(DecimalFormatSymbols.TWO_DIGIT_SYMBOL) == "\uff12"
    assert sym1.get_symbol(DecimalFormatSymbols.THREE_DIGIT_SYMBOL) == "\uff13"
    assert sym1.get_symbol(DecimalFormatSymbols.FOUR_DIGIT_SYMBOL) == "\uff14"
    assert sym1.get_symbol(DecimalFormatSymbols.FIVE_DIGIT_SYMBOL) == "\uff15"
    assert sym1.get_symbol(DecimalFormatSymbols.SIX_DIGIT_SYMBOL) == "\uff16"
    assert sym1.get_symbol(DecimalFormatSymbols.SEVEN_DIGIT_SYMBOL) == "\uff17"
    assert sym1.get_symbol(DecimalFormatSymbols.EIGHT_DIGIT_SYMBOL) == "\uff18"
    assert sym1.get_symbol(DecimalFormatSymbols.NINE_DIGIT_SYMBOL) == "\uff19"

    sym1.set_symbol(
        DecimalFormatSymbols.ZERO_DIGIT_SYMBOL,
        "0")
    assert sym1.get_symbol(DecimalFormatSymbols.ZERO_DIGIT_SYMBOL) == "0"
    assert sym1.get_symbol(DecimalFormatSymbols.ONE_DIGIT_SYMBOL) == "1"
    assert sym1.get_symbol(DecimalFormatSymbols.TWO_DIGIT_SYMBOL) == "2"
    assert sym1.get_symbol(DecimalFormatSymbols.THREE_DIGIT_SYMBOL) == "3"
    assert sym1.get_symbol(DecimalFormatSymbols.FOUR_DIGIT_SYMBOL) == "4"
    assert sym1.get_symbol(DecimalFormatSymbols.FIVE_DIGIT_SYMBOL) == "5"
    assert sym1.get_symbol(DecimalFormatSymbols.SIX_DIGIT_SYMBOL) == "6"
    assert sym1.get_symbol(DecimalFormatSymbols.SEVEN_DIGIT_SYMBOL) == "7"
    assert sym1.get_symbol(DecimalFormatSymbols.EIGHT_DIGIT_SYMBOL) == "8"
    assert sym1.get_symbol(DecimalFormatSymbols.NINE_DIGIT_SYMBOL) == "9"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_create_with_last_resort_data():
    # static DecimalFormatSymbols *
    # icu::DecimalFormatSymbols::createWithLastResortData(
    #       UErrorCode &status
    # )
    sym = DecimalFormatSymbols.create_with_last_resort_data()
    assert isinstance(sym, DecimalFormatSymbols)


def test_decimal_format_symbols():
    # [1]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    sym1 = DecimalFormatSymbols(Locale.get_us())
    sym1a = DecimalFormatSymbols("en_US")
    assert sym1 == sym1a

    # [2]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(
    #       const Locale &locale,
    #       const NumberingSystem &ns,
    #       UErrorCode &status
    # )
    sym2 = DecimalFormatSymbols(
        Locale.get_us(),
        NumberingSystem.create_instance())
    sym2a = DecimalFormatSymbols(
        "en_US",
        NumberingSystem.create_instance())
    assert sym2 == sym2a

    # [3]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(UErrorCode &status)
    sym3 = DecimalFormatSymbols()
    sym1b = DecimalFormatSymbols(Locale.get_default())
    assert sym3 == sym1b

    # [4]
    # icu::DecimalFormatSymbols::DecimalFormatSymbols(
    #       const DecimalFormatSymbols &
    # )
    sym4 = DecimalFormatSymbols(sym1)
    assert sym4 == sym1


def test_numbering_system():
    # [1]
    # static NumberingSystem *icu::NumberingSystem::createInstance(
    #       const Locale &inLocale,
    #       UErrorCode &status
    # )
    ns1 = NumberingSystem.create_instance(Locale.get_us())
    assert isinstance(ns1, NumberingSystem)

    ns2 = NumberingSystem.create_instance("en_US")
    assert isinstance(ns2, NumberingSystem)

    # [2]
    # static NumberingSystem *icu::NumberingSystem::createInstance(
    #       int32_t radix,
    #       UBool isAlgorithmic,
    #       const UnicodeString &description,
    #       UErrorCode &status
    # )
    ns3 = NumberingSystem.create_instance(
        10,
        False,
        UnicodeString("0123456789"))
    assert isinstance(ns3, NumberingSystem)

    ns4 = NumberingSystem.create_instance(
        10,
        False,
        "0123456789")
    assert isinstance(ns4, NumberingSystem)

    # [3]
    # static NumberingSystem *icu::NumberingSystem::createInstance(
    #       UErrorCode &status
    # )
    ns5 = NumberingSystem.create_instance()
    assert isinstance(ns5, NumberingSystem)

    # static NumberingSystem *icu::NumberingSystem::createInstanceByName(
    #       const char *name,
    #       UErrorCode &status
    # )
    ns6 = NumberingSystem.create_instance_by_name("latn")
    assert isinstance(ns6, NumberingSystem)

    # [2]
    # icu::NumberingSystem::NumberingSystem(const NumberingSystem &other)
    ns7 = NumberingSystem(ns1)

    # static StringEnumeration *icu::NumberingSystem::getAvailableNames(
    #       UErrorCode &status
    # )
    it = NumberingSystem.get_available_names()
    assert isinstance(it, StringEnumeration)
    assert len(it) > 0
    assert "latn" in it

    # UnicodeString icu::NumberingSystem::getDescription()
    result = ns7.get_description()
    assert isinstance(result, UnicodeString)
    assert result == "0123456789"

    # const char *icu::NumberingSystem::getName()
    result = ns7.get_name()
    assert isinstance(result, str)
    assert result == "latn"

    # int32_t icu::NumberingSystem::getRadix()
    assert ns7.get_radix() == 10

    # UBool icu::NumberingSystem::isAlgorithmic()
    assert not ns7.is_algorithmic()

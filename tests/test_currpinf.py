import copy

from icupy.icu import (
    CurrencyPluralInfo, Locale, PluralRules, UnicodeString,
)


def test_api():
    cpi1 = CurrencyPluralInfo(Locale.get_us())
    cpi2 = cpi1.clone()
    cpi3 = CurrencyPluralInfo(Locale.get_uk())

    # UBool icu::CurrencyPluralInfo::operator!=(const CurrencyPluralInfo &info)
    assert not (cpi1 != cpi2)
    assert cpi1 != cpi3
    assert cpi2 != cpi3

    # UBool icu::CurrencyPluralInfo::operator==(const CurrencyPluralInfo &info)
    assert cpi1 == cpi2
    assert not (cpi1 == cpi3)
    assert not (cpi2 == cpi3)

    # UnicodeString &icu::CurrencyPluralInfo::getCurrencyPluralPattern(
    #       const UnicodeString &pluralCount,
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = cpi1.get_currency_plural_pattern(UnicodeString("one"), result)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == "#,##0.### \xa4\xa4\xa4"

    output = cpi1.get_currency_plural_pattern("one", result)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == "#,##0.### \xa4\xa4\xa4"

    # const Locale &icu::CurrencyPluralInfo::getLocale()
    result = cpi1.get_locale()
    assert isinstance(result, Locale)
    assert result == Locale("en_US")

    # const PluralRules *icu::CurrencyPluralInfo::getPluralRules()
    result = cpi1.get_plural_rules()
    assert isinstance(result, PluralRules)

    # void icu::CurrencyPluralInfo::setCurrencyPluralPattern(
    #       const UnicodeString &pluralCount,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    cpi2.set_currency_plural_pattern(
        UnicodeString("one"),
        UnicodeString("0 qwerty"))
    cpi2.set_currency_plural_pattern("one", UnicodeString("0 qwerty"))
    cpi2.set_currency_plural_pattern(UnicodeString("one"), "0 qwerty")
    cpi2.set_currency_plural_pattern("one", "0 qwerty")

    # void icu::CurrencyPluralInfo::setLocale(
    #       const Locale &loc,
    #       UErrorCode &status
    # )
    cpi2.set_locale(Locale("fr"))
    assert cpi2.get_locale() == Locale("fr")

    cpi2.set_locale("de")
    assert cpi2.get_locale() == Locale("de")

    # void icu::CurrencyPluralInfo::setPluralRules(
    #       const UnicodeString &ruleDescription,
    #       UErrorCode &status
    # )
    cpi2.set_plural_rules(UnicodeString("a: n mod 10 is 2"))
    cpi2.set_plural_rules("a: n mod 10 is 2")


def test_clone():
    cpi1 = CurrencyPluralInfo()

    # CurrencyPluralInfo *icu::CurrencyPluralInfo::clone()
    cpi2 = cpi1.clone()
    assert isinstance(cpi2, CurrencyPluralInfo)
    assert cpi1 == cpi2

    cpi3 = copy.copy(cpi1)
    assert cpi1 == cpi3

    cpi4 = copy.deepcopy(cpi1)
    assert cpi1 == cpi4


def test_currency_plural_info():
    default_locale = Locale.get_default()

    try:
        locale = Locale.get_us()
        Locale.set_default(locale)

        # [1]
        # icu::CurrencyPluralInfo::CurrencyPluralInfo(UErrorCode &status)
        cpi1 = CurrencyPluralInfo()

        # [2]
        # icu::CurrencyPluralInfo::CurrencyPluralInfo(
        #       const Locale &locale,
        #       UErrorCode &status
        # )
        cpi2 = CurrencyPluralInfo(locale)
        assert cpi1 == cpi2

        cpi2a = CurrencyPluralInfo("en_US")
        assert cpi2 == cpi2a

        # [3]
        # icu::CurrencyPluralInfo::CurrencyPluralInfo(
        #       const CurrencyPluralInfo &info
        # )
        cpi3 = CurrencyPluralInfo(cpi1)
        assert cpi1 == cpi3
    finally:
        Locale.set_default(default_locale)

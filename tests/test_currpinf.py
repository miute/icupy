import copy

from icupy import icu


def test_api():
    cpi1 = icu.CurrencyPluralInfo(icu.Locale.get_us())
    cpi2 = cpi1.clone()
    cpi3 = icu.CurrencyPluralInfo(icu.Locale.get_uk())

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
    result = icu.UnicodeString()
    output = cpi1.get_currency_plural_pattern(icu.UnicodeString("one"), result)
    assert isinstance(output, icu.UnicodeString)
    assert id(result) == id(output)
    assert result == "#,##0.### \xa4\xa4\xa4"

    output = cpi1.get_currency_plural_pattern("one", result)
    assert isinstance(output, icu.UnicodeString)
    assert id(result) == id(output)
    assert result == "#,##0.### \xa4\xa4\xa4"

    # const Locale &icu::CurrencyPluralInfo::getLocale()
    result = cpi1.get_locale()
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("en_US")

    # const PluralRules *icu::CurrencyPluralInfo::getPluralRules()
    result = cpi1.get_plural_rules()
    assert isinstance(result, icu.PluralRules)

    # void icu::CurrencyPluralInfo::setCurrencyPluralPattern(
    #       const UnicodeString &pluralCount,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    cpi2.set_currency_plural_pattern(icu.UnicodeString("one"), icu.UnicodeString("0 qwerty"))
    cpi2.set_currency_plural_pattern("one", icu.UnicodeString("0 qwerty"))
    cpi2.set_currency_plural_pattern(icu.UnicodeString("one"), "0 qwerty")
    cpi2.set_currency_plural_pattern("one", "0 qwerty")

    # void icu::CurrencyPluralInfo::setLocale(
    #       const Locale &loc,
    #       UErrorCode &status
    # )
    cpi2.set_locale(icu.Locale("fr"))
    assert cpi2.get_locale() == icu.Locale("fr")

    cpi2.set_locale("de")
    assert cpi2.get_locale() == icu.Locale("de")

    # void icu::CurrencyPluralInfo::setPluralRules(
    #       const UnicodeString &ruleDescription,
    #       UErrorCode &status
    # )
    cpi2.set_plural_rules(icu.UnicodeString("a: n mod 10 is 2"))
    cpi2.set_plural_rules("a: n mod 10 is 2")


def test_clone():
    cpi1 = icu.CurrencyPluralInfo()

    # CurrencyPluralInfo *icu::CurrencyPluralInfo::clone()
    cpi2 = cpi1.clone()
    assert isinstance(cpi2, icu.CurrencyPluralInfo)
    assert cpi1 == cpi2

    cpi3 = copy.copy(cpi1)
    assert cpi1 == cpi3

    cpi4 = copy.deepcopy(cpi1)
    assert cpi1 == cpi4


def test_currency_plural_info():
    default_locale = icu.Locale.get_default()

    try:
        locale = icu.Locale.get_us()
        icu.Locale.set_default(locale)

        # [1]
        # icu::CurrencyPluralInfo::CurrencyPluralInfo(UErrorCode &status)
        cpi1 = icu.CurrencyPluralInfo()

        # [2]
        # icu::CurrencyPluralInfo::CurrencyPluralInfo(
        #       const Locale &locale,
        #       UErrorCode &status
        # )
        cpi2 = icu.CurrencyPluralInfo(locale)
        assert cpi1 == cpi2

        cpi2a = icu.CurrencyPluralInfo("en_US")
        assert cpi2 == cpi2a

        # [3]
        # icu::CurrencyPluralInfo::CurrencyPluralInfo(
        #       const CurrencyPluralInfo &info
        # )
        cpi3 = icu.CurrencyPluralInfo(cpi1)
        assert cpi1 == cpi3
    finally:
        icu.Locale.set_default(default_locale)

import copy

import pytest
from icupy.icu import (
    Locale, PluralRules, StringEnumeration, U_ICU_VERSION_MAJOR_NUM,
    UnicodeString,
)


def test_api():
    s1 = "a: n mod 10 is 2"
    s2 = "b: n mod 10 is 2"

    # static PluralRules *icu::PluralRules::createRules(
    #       const UnicodeString &description,
    #       UErrorCode &status
    # )
    rules1 = PluralRules.create_rules(UnicodeString(s1))
    assert isinstance(rules1, PluralRules)

    rules1a = PluralRules.create_rules(s1)
    assert isinstance(rules1a, PluralRules)

    rules1b = PluralRules.create_rules(s2)
    assert isinstance(rules1b, PluralRules)

    # [2]
    # icu::PluralRules::PluralRules(const PluralRules &other)
    rules2 = PluralRules(rules1)

    # UBool icu::PluralRules::operator!=(const PluralRules &other)
    assert not (rules1 != rules1a)
    assert rules1 != rules1b
    assert not (rules1 != rules2)

    # UBool icu::PluralRules::operator==(const PluralRules &other)
    assert rules1 == rules1a
    assert not (rules1 == rules1b)
    assert rules1 == rules2

    # UnicodeString icu::PluralRules::getKeywordOther()
    result = rules1.get_keyword_other()
    assert isinstance(result, UnicodeString)
    assert result == "other"

    # StringEnumeration *icu::PluralRules::getKeywords(UErrorCode &status)
    it = rules1.get_keywords()
    assert isinstance(it, StringEnumeration)
    assert list(it) == ["a", "other"]

    # UBool icu::PluralRules::isKeyword(const UnicodeString &keyword)
    assert rules1.is_keyword(UnicodeString("a"))
    assert rules1.is_keyword("a")
    assert not rules1.is_keyword(UnicodeString("b"))
    assert not rules1.is_keyword("b")


def test_clone():
    rules1 = PluralRules.create_default_rules()

    # PluralRules *icu::PluralRules::clone()
    rules2 = rules1.clone()
    assert isinstance(rules2, PluralRules)
    assert rules1 == rules2

    rules3 = copy.copy(rules1)
    assert rules1 == rules3

    rules4 = copy.deepcopy(rules1)
    assert rules1 == rules4


def test_create_default_rules():
    # static PluralRules *icu::PluralRules::createDefaultRules(
    #       UErrorCode &status
    # )
    rules = PluralRules.create_default_rules()
    assert isinstance(rules, PluralRules)


def test_for_locale():
    # [1]
    # static PluralRules *icu::PluralRules::forLocale(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    rules1 = PluralRules.for_locale(Locale("en"))
    assert isinstance(rules1, PluralRules)

    rules1a = PluralRules.for_locale("en")
    assert isinstance(rules1a, PluralRules)
    assert rules1 == rules1a


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_for_locale_50():
    from icupy.icu import UPluralType

    # [2]
    # static PluralRules *icu::PluralRules::forLocale(
    #       const Locale &locale,
    #       UPluralType type,
    #       UErrorCode &status
    # )
    rules2 = PluralRules.for_locale(
        Locale("en"),
        UPluralType.UPLURAL_TYPE_CARDINAL)
    assert isinstance(rules2, PluralRules)

    rules2a = PluralRules.for_locale(
        "en",
        UPluralType.UPLURAL_TYPE_CARDINAL)
    assert isinstance(rules2a, PluralRules)
    assert rules2 == rules2a


@pytest.mark.skip("FIXME: Enable test_get_samples().")
def test_get_samples():
    # FIXME: Enable test_get_samples().
    rules = PluralRules.for_locale(Locale("fr"))

    # [1]
    # int32_t icu::PluralRules::getSamples(
    #       const UnicodeString &keyword,
    #       double *dest,
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest = rules.get_samples(UnicodeString("other"))
    assert isinstance(dest, list)
    assert len(dest) > 0
    assert all(isinstance(x, float) for x in dest)

    dest = rules.get_samples("other")
    assert isinstance(dest, list)
    assert len(dest) > 0
    assert all(isinstance(x, float) for x in dest)


def test_select():
    rules = PluralRules.create_rules("a: n not in 0..100;")

    # [5]
    # UnicodeString icu::PluralRules::select(double number)
    result = rules.select(-100.0)
    assert isinstance(result, UnicodeString)
    assert result == "other"

    assert rules.select(-100.1) == "a"

    # [6]
    # UnicodeString icu::PluralRules::select(int32_t number)
    result = rules.select(-100)
    assert isinstance(result, UnicodeString)
    assert result == "other"

    assert rules.select(-101) == "a"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_select_64():
    from icupy.icu.number import NumberFormatter

    rules = PluralRules.create_rules("a: n not in 0..100;")
    fmt = NumberFormatter.with_locale("en")

    # [2]
    # UnicodeString icu::PluralRules::select(
    #       const number::FormattedNumber &number,
    #       UErrorCode &status
    # )
    result = rules.select(fmt.format_double(-100.0))
    assert isinstance(result, UnicodeString)
    assert result == "other"

    assert rules.select(fmt.format_double(-100.1)) == "a"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_select_68():
    from icupy.icu import Formattable
    from icupy.icu.number import NumberFormatter, NumberRangeFormatter

    rules = PluralRules.for_locale("fr")
    fmt = (NumberRangeFormatter.with_locale("fr")
           .number_formatter_first(NumberFormatter.with_())
           .number_formatter_second(NumberFormatter.with_()))

    # [3]
    # UnicodeString icu::PluralRules::select(
    #       const number::FormattedNumberRange &range,
    #       UErrorCode &status
    # )
    result = rules.select(
        fmt.format_formattable_range(Formattable(0), Formattable(2)))
    assert isinstance(result, UnicodeString)
    assert result == "other"

    assert rules.select(
        fmt.format_formattable_range(Formattable(0), Formattable(1))
    ) == "one"

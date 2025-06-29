import copy

import pytest

from icupy import icu


def test_api() -> None:
    s1 = "a: n mod 10 is 2"
    s2 = "b: n mod 10 is 2"

    # static PluralRules *icu::PluralRules::createRules(
    #       const UnicodeString &description,
    #       UErrorCode &status
    # )
    rules1 = icu.PluralRules.create_rules(icu.UnicodeString(s1))
    assert isinstance(rules1, icu.PluralRules)

    rules1a = icu.PluralRules.create_rules(s1)
    assert isinstance(rules1a, icu.PluralRules)

    rules1b = icu.PluralRules.create_rules(s2)
    assert isinstance(rules1b, icu.PluralRules)

    # [2]
    # icu::PluralRules::PluralRules(const PluralRules &other)
    rules2 = icu.PluralRules(rules1)

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
    assert isinstance(result, icu.UnicodeString)
    assert result == "other"

    # StringEnumeration *icu::PluralRules::getKeywords(UErrorCode &status)
    it = rules1.get_keywords()
    assert isinstance(it, icu.StringEnumeration)
    assert list(it) == ["a", "other"]

    # UBool icu::PluralRules::isKeyword(const UnicodeString &keyword)
    assert rules1.is_keyword(icu.UnicodeString("a")) is True
    assert rules1.is_keyword("a") is True
    assert rules1.is_keyword(icu.UnicodeString("b")) is False
    assert rules1.is_keyword("b") is False


def test_clone() -> None:
    rules1 = icu.PluralRules.create_default_rules()

    # PluralRules *icu::PluralRules::clone()
    rules2 = rules1.clone()
    assert isinstance(rules2, icu.PluralRules)
    assert rules1 == rules2

    rules3 = copy.copy(rules1)
    assert rules1 == rules3

    rules4 = copy.deepcopy(rules1)
    assert rules1 == rules4


def test_create_default_rules() -> None:
    # static PluralRules *icu::PluralRules::createDefaultRules(
    #       UErrorCode &status
    # )
    rules = icu.PluralRules.create_default_rules()
    assert isinstance(rules, icu.PluralRules)


def test_for_locale() -> None:
    # [1]
    # static PluralRules *icu::PluralRules::forLocale(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    rules1 = icu.PluralRules.for_locale(icu.Locale("en"))
    assert isinstance(rules1, icu.PluralRules)

    rules1a = icu.PluralRules.for_locale("en")
    assert isinstance(rules1a, icu.PluralRules)
    assert rules1 == rules1a


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_for_locale_50() -> None:
    # [2]
    # static PluralRules *icu::PluralRules::forLocale(
    #       const Locale &locale,
    #       UPluralType type,
    #       UErrorCode &status
    # )
    rules2 = icu.PluralRules.for_locale(icu.Locale("en"), icu.UPluralType.UPLURAL_TYPE_CARDINAL)
    assert isinstance(rules2, icu.PluralRules)

    rules2a = icu.PluralRules.for_locale("en", icu.UPluralType.UPLURAL_TYPE_CARDINAL)
    assert isinstance(rules2a, icu.PluralRules)
    assert rules2 == rules2a


def test_get_samples() -> None:
    rules = icu.PluralRules.for_locale(icu.Locale("fr"))

    # [1]
    # int32_t icu::PluralRules::getSamples(
    #       const UnicodeString &keyword,
    #       double *dest,
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest = rules.get_samples(icu.UnicodeString("other"), 100)
    assert isinstance(dest, list)
    assert len(dest) > 0
    assert all(isinstance(x, float) for x in dest)

    dest = rules.get_samples("other", 100)
    assert isinstance(dest, list)
    assert len(dest) > 0
    assert all(isinstance(x, float) for x in dest)

    with pytest.raises(icu.ICUError) as exc_info:
        _ = rules.get_samples(icu.UnicodeString("other"), 0)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INTERNAL_PROGRAM_ERROR

    with pytest.raises(ValueError):
        _ = rules.get_samples(icu.UnicodeString("other"), -1)


def test_select() -> None:
    rules = icu.PluralRules.create_rules("a: n not in 0..100;")

    # [5]
    # UnicodeString icu::PluralRules::select(double number)
    result = rules.select(-100.0)
    assert isinstance(result, icu.UnicodeString)
    assert result == "other"

    assert rules.select(-100.1) == "a"

    # [6]
    # UnicodeString icu::PluralRules::select(int32_t number)
    result = rules.select(-100)
    assert isinstance(result, icu.UnicodeString)
    assert result == "other"

    assert rules.select(-101) == "a"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_select_64() -> None:
    rules = icu.PluralRules.create_rules("a: n not in 0..100;")
    fmt = icu.number.NumberFormatter.with_locale("en")

    # [2]
    # UnicodeString icu::PluralRules::select(
    #       const number::FormattedNumber &number,
    #       UErrorCode &status
    # )
    result = rules.select(fmt.format_double(-100.0))
    assert isinstance(result, icu.UnicodeString)
    assert result == "other"

    assert rules.select(fmt.format_double(-100.1)) == "a"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_select_68() -> None:
    rules = icu.PluralRules.for_locale("fr")
    fmt = (
        icu.number.NumberRangeFormatter.with_locale("fr")
        .number_formatter_first(icu.number.NumberFormatter.with_())
        .number_formatter_second(icu.number.NumberFormatter.with_())
    )

    # [3]
    # UnicodeString icu::PluralRules::select(
    #       const number::FormattedNumberRange &range,
    #       UErrorCode &status
    # )
    result = rules.select(fmt.format_formattable_range(icu.Formattable(0), icu.Formattable(2)))
    assert isinstance(result, icu.UnicodeString)
    assert result == "other"

    assert (
        rules.select(fmt.format_formattable_range(icu.Formattable(0), icu.Formattable(1)))
        == "one"
    )

import copy
from functools import cmp_to_key

import pytest

from icupy import (
    CollationElementIterator, CollationKey, Collator, Locale,
    RuleBasedCollator, StringCharacterIterator, StringEnumeration,
    UColAttribute, UColAttributeValue, UColBoundMode, UColReorderCode,
    UColRuleOption, UCollationResult, UScriptCode,
    U_ICU_VERSION_MAJOR_NUM,
    UnicodeSet, UnicodeString,
)


def test_clone():
    test1 = Collator.create_instance(Locale.get_japanese())
    rules1 = test1.get_rules()

    test2 = test1.clone()
    rules2 = test2.get_rules()
    assert rules2 == rules1

    test3 = copy.copy(test1)
    rules3 = test3.get_rules()
    assert rules3 == rules1

    test4 = copy.deepcopy(test1)
    rules4 = test4.get_rules()
    assert rules4 == rules1


def test_compare():
    coll = Collator.create_instance(Locale.get_english())
    source = UnicodeString("ABC")
    target = UnicodeString("abc")

    # [2]
    # UCollationResult compare(const char16_t *source,
    #                          int32_t sourceLength,
    #                          const char16_t *target,
    #                          int32_t targetLength,
    #                          UErrorCode &status
    # )
    assert coll.compare("ABC", -1, "abc", -1) == UCollationResult.UCOL_GREATER

    # [5]
    # UCollationResult compare(const UnicodeString &source,
    #                          const UnicodeString &target,
    #                          int32_t length,
    #                          UErrorCode &status
    # )
    assert coll.compare(source, target, 3) == UCollationResult.UCOL_GREATER

    # [6]
    # UCollationResult compare(const UnicodeString &source,
    #                          const UnicodeString &target,
    #                          UErrorCode &status
    # )
    assert coll.compare(source, target) == UCollationResult.UCOL_GREATER

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_PRIMARY)
    assert coll.compare("ABC", -1, "abc", -1) == UCollationResult.UCOL_EQUAL
    assert coll.compare(source, target, 3) == UCollationResult.UCOL_EQUAL
    assert coll.compare(source, target) == UCollationResult.UCOL_EQUAL


def test_create_collation_element_iterator():
    coll = Collator.create_instance(Locale("es"))

    # [1]
    # CollationElementIterator* createCollationElementIterator(
    #       const CharacterIterator &source
    # )
    source = StringCharacterIterator(UnicodeString("cha"))
    it1 = coll.create_collation_element_iterator(source)
    assert isinstance(it1, CollationElementIterator)

    order1 = it1.next()
    assert order1 != CollationElementIterator.NULLORDER
    order2 = it1.next()
    assert order2 != CollationElementIterator.NULLORDER
    order3 = it1.next()
    assert order3 != CollationElementIterator.NULLORDER
    assert it1.next() == CollationElementIterator.NULLORDER

    it1.set_offset(3)
    assert it1.get_offset() == 3
    assert it1.previous() == order3
    assert it1.previous() == order2
    assert it1.previous() == order1
    assert it1.previous() == CollationElementIterator.NULLORDER

    assert list(it1) == [order1, order2, order3]

    # [2]
    # CollationElementIterator* createCollationElementIterator(
    #       const UnicodeString &source
    # )
    source = UnicodeString("cha")
    it2 = coll.create_collation_element_iterator(source)
    assert isinstance(it2, CollationElementIterator)

    order1 = it2.next()
    assert order1 != CollationElementIterator.NULLORDER
    order2 = it2.next()
    assert order2 != CollationElementIterator.NULLORDER
    order3 = it2.next()
    assert order3 != CollationElementIterator.NULLORDER
    assert it2.next() == CollationElementIterator.NULLORDER

    it2.set_offset(3)
    assert it2.get_offset() == 3
    assert it2.previous() == order3
    assert it2.previous() == order2
    assert it2.previous() == order1
    assert it2.previous() == CollationElementIterator.NULLORDER

    assert list(it2) == [order1, order2, order3]


def test_create_instance():
    # [1]
    # static Collator* Collator::createInstance(const Locale &loc,
    #                                           UErrorCode &err
    # )
    test1 = Collator.create_instance(Locale())
    rules1 = test1.get_rules()

    # [2]
    # static Collator* Collator::createInstance(UErrorCode &err)
    test2 = Collator.create_instance()
    rules2 = test2.get_rules()

    assert rules1 == rules2


def test_equals():
    coll = Collator.create_instance(Locale.get_english())
    source = UnicodeString("ABC")
    target = UnicodeString("abc")

    assert not coll.equals(source, target)

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_PRIMARY)
    assert coll.equals(source, target)


def test_get_attribute():
    coll = Collator.create_instance(Locale.get_canada_french())
    assert (coll.get_attribute(UColAttribute.UCOL_FRENCH_COLLATION)
            == UColAttributeValue.UCOL_ON)
    assert (coll.get_attribute(UColAttribute.UCOL_ALTERNATE_HANDLING)
            == UColAttributeValue.UCOL_NON_IGNORABLE)
    assert (coll.get_attribute(UColAttribute.UCOL_CASE_FIRST)
            == UColAttributeValue.UCOL_STRENGTH_LIMIT)
    assert (coll.get_attribute(UColAttribute.UCOL_CASE_LEVEL)
            == UColAttributeValue.UCOL_STRENGTH_LIMIT)
    assert (coll.get_attribute(UColAttribute.UCOL_NORMALIZATION_MODE)
            == UColAttributeValue.UCOL_STRENGTH_LIMIT)
    assert (coll.get_attribute(UColAttribute.UCOL_DECOMPOSITION_MODE)
            == UColAttributeValue.UCOL_STRENGTH_LIMIT)
    assert (coll.get_attribute(UColAttribute.UCOL_STRENGTH)
            == UColAttributeValue.UCOL_TERTIARY)
    assert (coll.get_attribute(UColAttribute.UCOL_NUMERIC_COLLATION)
            == UColAttributeValue.UCOL_STRENGTH_LIMIT)

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_PRIMARY)
    assert (coll.get_attribute(UColAttribute.UCOL_STRENGTH)
            == UColAttributeValue.UCOL_PRIMARY)


def test_get_available_locales():
    # [2]
    # static StringEnumeration* Collator::getAvailableLocales(void)
    it = Collator.get_available_locales()
    assert isinstance(it, StringEnumeration)
    assert len(it) > 0
    assert all(isinstance(x, str) for x in it)
    assert "ja" in it
    assert "und" not in it


def test_get_bound():
    coll = Collator.create_instance(Locale("sh"))
    source = coll.get_sort_key("Smith", -1)

    # static int32_t Collator::getBound(const uint8_t *source,
    #                                   int32_t sourceLength,
    #                                   UColBoundMode boundType,
    #                                   uint32_t noOfLevels,
    #                                   uint8_t *result,
    #                                   int32_t resultLength,
    #                                   UErrorCode &status
    # )
    result1 = Collator.get_bound(source,
                                 len(source),
                                 UColBoundMode.UCOL_BOUND_LOWER,
                                 1)
    result2 = Collator.get_bound(source,
                                 len(source),
                                 UColBoundMode.UCOL_BOUND_UPPER,
                                 1)
    assert isinstance(result1, list)
    assert len(result1) > 0
    assert all(isinstance(x, int) for x in result1)
    assert result1 < source
    assert result2 > source


def test_get_collation_key():
    coll = Collator.create_instance(Locale.get_english())

    # [1]
    # CollationKey& getCollationKey(const char16_t *source,
    #                               int32_t sourceLength,
    #                               CollationKey &key,
    #                               UErrorCode &status
    # )
    key1 = CollationKey()
    result1 = coll.get_collation_key("ABC", -1, key1)
    assert isinstance(result1, CollationKey)
    assert result1 == key1

    # [2]
    # CollationKey& getCollationKey(const UnicodeString &source,
    #                               CollationKey &key,
    #                               UErrorCode &status
    # )
    key2 = CollationKey()
    result2 = coll.get_collation_key(UnicodeString("abc"), key2)
    assert isinstance(result2, CollationKey)
    assert result2 == key2

    assert key1.compare_to(key2) == UCollationResult.UCOL_GREATER
    assert key2.compare_to(key1) == UCollationResult.UCOL_LESS


def test_get_display_name():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        display_locale = Locale.get_us()
        Locale.set_default(display_locale)
        object_locale = Locale("ja")

        # [1]
        # static UnicodeString& Collator::getDisplayName(
        #       const Locale &objectLocale,
        #       const Locale &displayLocale,
        #       UnicodeString &name
        # )
        name1 = UnicodeString()
        result1 = Collator.get_display_name(object_locale,
                                            display_locale,
                                            name1)
        assert isinstance(result1, UnicodeString)
        assert result1 == name1
        assert name1 == "Japanese"

        # [2]
        # static UnicodeString& Collator::getDisplayName(
        #       const Locale &objectLocale,
        #       UnicodeString &name
        # )
        name2 = UnicodeString()
        result2 = Collator.get_display_name(object_locale, name2)
        assert isinstance(result2, UnicodeString)
        assert result2 == name2
        assert name2 == "Japanese"
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_equivalent_reorder_codes():
    # static int32_t Collator::getEquivalentReorderCodes(
    #       int32_t reorderCode,
    #       int32_t *dest,
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest = Collator.get_equivalent_reorder_codes(UScriptCode.USCRIPT_KATAKANA)
    assert isinstance(dest, list)
    assert dest == [
        UScriptCode.USCRIPT_HIRAGANA,
        UScriptCode.USCRIPT_KATAKANA,
        UScriptCode.USCRIPT_KATAKANA_OR_HIRAGANA,
    ]


def test_get_functional_equivalent():
    # static Locale Collator::getFunctionalEquivalent(
    #       const char *keyword,
    #       const Locale &locale,
    #       UBool &isAvailable,
    #       UErrorCode &status
    # )
    result, is_available = Collator.get_functional_equivalent(
        "collation",
        Locale.get_japanese()
    )
    assert isinstance(result, Locale)
    assert result == Locale.get_japanese()
    assert is_available


def test_get_keywords():
    keywords = Collator.get_keywords()
    assert isinstance(keywords, StringEnumeration)
    assert len(keywords) > 0
    assert "collation" in keywords

    values = Collator.get_keyword_values("collation")
    assert isinstance(keywords, StringEnumeration)
    assert len(keywords) > 0
    assert "standard" in values

    it = Collator.get_keyword_values_for_locale(
        "collation",
        Locale.get_japanese(),
        True)
    assert isinstance(it, StringEnumeration)
    assert list(it) == ['standard', 'unihan', 'emoji', 'eor', 'search']


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_max_variable():
    coll = Collator.create_instance(Locale.get_japanese())
    assert (coll.get_max_variable()
            == UColReorderCode.UCOL_REORDER_CODE_PUNCTUATION)
    weight1 = coll.get_variable_top()

    coll.set_max_variable(UColReorderCode.UCOL_REORDER_CODE_CURRENCY)
    assert (coll.get_max_variable()
            == UColReorderCode.UCOL_REORDER_CODE_CURRENCY)
    weight2 = coll.get_variable_top()

    assert weight1 != weight2


def test_get_reorder_codes():
    coll = Collator.create_instance(Locale.get_japanese())
    dest = coll.get_reorder_codes()
    assert isinstance(dest, list)
    assert dest == [
        UScriptCode.USCRIPT_LATIN,
        UScriptCode.USCRIPT_KATAKANA,
        UScriptCode.USCRIPT_HAN,
    ]

    reorder_codes = [
        UScriptCode.USCRIPT_HAN,
        UScriptCode.USCRIPT_KATAKANA,
        UScriptCode.USCRIPT_LATIN,
    ]
    coll.set_reorder_codes(reorder_codes, len(reorder_codes))
    dest = coll.get_reorder_codes()
    assert dest == reorder_codes


def test_get_rules():
    coll = Collator.create_instance(Locale.get_japanese())

    # [1]
    # UnicodeString& getRules()
    rules1 = coll.get_rules()
    assert isinstance(rules1, UnicodeString)

    # [2]
    # void getRules(UColRuleOption delta,
    #               UnicodeString &buffer
    # )
    rules2 = UnicodeString()
    coll.get_rules(UColRuleOption.UCOL_TAILORING_ONLY, rules2)
    assert rules2 == rules1

    rules3 = UnicodeString()
    coll.get_rules(UColRuleOption.UCOL_FULL_RULES, rules3)
    assert rules3 != rules1


def test_get_sort_key():
    coll = Collator.create_instance(Locale.get_japanese())

    # [1]
    # int32_t getSortKey(const char16_t *source,
    #                    int32_t sourceLength,
    #                    uint8_t *result,
    #                    int32_t resultLength
    # )
    result1 = coll.get_sort_key("ABC", -1)
    assert isinstance(result1, list)
    assert len(result1) > 0
    assert all(isinstance(x, int) for x in result1)

    # [2]
    # int32_t getSortKey(const UnicodeString &source,
    #                    uint8_t *result,
    #                    int32_t resultLength
    # )
    result2 = coll.get_sort_key(UnicodeString("abc"))
    assert isinstance(result2, list)
    assert len(result2) > 0
    assert all(isinstance(x, int) for x in result2)

    assert result1 > result2


def test_get_tailored_set():
    coll1 = Collator.create_instance(Locale.get_english())
    uset1 = coll1.get_tailored_set()
    assert isinstance(uset1, UnicodeSet)
    assert len(uset1) == 0

    coll2 = Collator.create_instance(Locale.get_japanese())
    uset2 = coll2.get_tailored_set()
    assert len(uset2) > 0


def test_get_version():
    coll = Collator.create_instance(Locale.get_japanese())
    info = coll.get_version()
    assert isinstance(info, list)
    assert len(info) == 4
    assert all(isinstance(x, int) for x in info)
    assert info[0] > 0


def test_greater():
    coll = Collator.create_instance(Locale.get_english())
    source = UnicodeString("ABC")
    target = UnicodeString("abc")

    assert coll.greater(source, target)

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_PRIMARY)
    assert not coll.greater(source, target)


def test_greater_or_equal():
    coll = Collator.create_instance(Locale.get_english())
    source = UnicodeString("ABC")
    target = UnicodeString("abc")

    assert coll.greater_or_equal(source, target)

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_PRIMARY)
    assert coll.greater_or_equal(source, target)


def test_hash_code():
    coll1 = Collator.create_instance(Locale.get_english())
    coll2 = Collator.create_instance(Locale("da", "DK", ""))
    coll3 = Collator.create_instance(Locale.get_english())
    assert coll1.hash_code() != coll2.hash_code()
    assert coll1.hash_code() == coll3.hash_code()
    assert coll2.hash_code() != coll3.hash_code()


def test_operator():
    coll1 = Collator.create_instance(Locale.get_english())
    coll2 = Collator.create_instance(Locale("da", "DK", ""))
    coll3 = Collator.create_instance(Locale.get_english())
    assert coll1 != coll2
    assert coll1 == coll3
    assert coll2 != coll3


def test_rule_based_collator():
    base = Collator.create_instance(Locale.get_japanese())
    rules = base.get_rules()
    assert len(rules) > 0

    # [1]
    # RuleBasedCollator(const UnicodeString &rules,
    #                   UErrorCode &status
    # )
    test1 = RuleBasedCollator(rules)
    rules1 = test1.get_rules()
    assert rules1 == rules

    # [2]
    # RuleBasedCollator(const UnicodeString &rules,
    #                   ECollationStrength collationStrength,
    #                   UErrorCode &status
    # )
    test2 = RuleBasedCollator(rules,
                              Collator.ECollationStrength.TERTIARY)
    rules2 = test2.get_rules()
    assert rules2 == rules

    # [3]
    # RuleBasedCollator(const UnicodeString &rules,
    #                   UColAttributeValue decompositionMode,
    #                   UErrorCode &status
    # )
    test3 = RuleBasedCollator(rules,
                              UColAttributeValue.UCOL_ON)
    rules3 = test3.get_rules()
    assert rules3 == rules

    # [4]
    # RuleBasedCollator(const UnicodeString &rules,
    #                   ECollationStrength collationStrength,
    #                   UColAttributeValue decompositionMode,
    #                   UErrorCode &status
    # )
    test4 = RuleBasedCollator(rules,
                              Collator.ECollationStrength.TERTIARY,
                              UColAttributeValue.UCOL_ON)
    rules4 = test4.get_rules()
    assert rules4 == rules

    # [6]
    # RuleBasedCollator(const RuleBasedCollator &other)
    test6 = RuleBasedCollator(test1)
    rules6 = test6.get_rules()
    assert rules6 == rules

    # [7]
    # RuleBasedCollator(const uint8_t *bin,
    #                   int32_t length,
    #                   const RuleBasedCollator *base,
    #                   UErrorCode &status
    # )
    # buffer = test1.clone_binary()
    # assert isinstance(buffer, list)
    # assert len(buffer) > 0
    # root = Collator.create_instance(Locale.get_root())
    # test7 = RuleBasedCollator(buffer, len(buffer), root)
    # rules7 = test7.get_rules()
    # assert rules7 == rules
    pass  # NotImplemented


def test_sort():
    # From https://unicode-org.github.io/icu/userguide/collation/customization/#simple-tailoring-examples
    src = [
        "CUKIĆ RADOJICA", "ČUKIĆ SLOBODAN", "CUKIĆ SVETOZAR", "ČUKIĆ ZORAN",
        "CURIĆ MILOŠ", "ĆURIĆ MILOŠ", "CVRKALJ ÐURO",
    ]
    base = Collator.create_instance(Locale("sr_Latn"))
    rules = base.get_rules() + UnicodeString("& C < č <<< Č < ć <<< Ć")
    coll = RuleBasedCollator(rules)

    def _cmp(_a, _b):
        nonlocal coll
        _result = coll.compare(_a, -1, _b, -1)
        return (
            -1 if _result == UCollationResult.UCOL_LESS
            else 0 if _result == UCollationResult.UCOL_EQUAL
            else 1
        )

    result1 = sorted(src)
    result2 = sorted(src, key=cmp_to_key(_cmp))
    result3 = sorted(src, key=lambda s: coll.get_sort_key(s, -1))
    assert result1 != result2
    assert result1 != result3
    assert result2 == result3
    assert result3 == [
        "CUKIĆ RADOJICA", "CUKIĆ SVETOZAR", "CURIĆ MILOŠ", "CVRKALJ ÐURO",
        "ČUKIĆ SLOBODAN", "ČUKIĆ ZORAN", "ĆURIĆ MILOŠ",
    ]

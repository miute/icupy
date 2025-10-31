import copy
from functools import cmp_to_key

import pytest

from icupy import icu


def test_clone() -> None:
    test1 = icu.Collator.create_instance(icu.Locale.get_japanese())
    rules1 = test1.get_rules()

    # RuleBasedCollator *icu::RuleBasedCollator::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.RuleBasedCollator)
    rules2 = test2.get_rules()
    assert rules2 == rules1

    test3 = copy.copy(test1)
    rules3 = test3.get_rules()
    assert rules3 == rules1

    test4 = copy.deepcopy(test1)
    rules4 = test4.get_rules()
    assert rules4 == rules1


def test_compare() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_english())
    source = icu.UnicodeString("ABC")
    target = icu.UnicodeString("abc")

    # [2]
    # UCollationResult icu::Collator::compare(
    #       const char16_t *source,
    #       int32_t sourceLength,
    #       const char16_t *target,
    #       int32_t targetLength,
    #       UErrorCode &status
    # )
    assert coll.compare("ABC", -1, "abc", -1) == icu.UCollationResult.UCOL_GREATER

    # [5]
    # UCollationResult icu::Collator::compare(
    #       const UnicodeString &source,
    #       const UnicodeString &target,
    #       int32_t length,
    #       UErrorCode &status
    # )
    assert coll.compare(source, target, 3) == icu.UCollationResult.UCOL_GREATER
    assert coll.compare("ABC", target, 3) == icu.UCollationResult.UCOL_GREATER
    assert coll.compare(source, "abc", 3) == icu.UCollationResult.UCOL_GREATER
    assert coll.compare("ABC", "abc", 3) == icu.UCollationResult.UCOL_GREATER

    # [6]
    # UCollationResult icu::Collator::compare(
    #       const UnicodeString &source,
    #       const UnicodeString &target,
    #       UErrorCode &status
    # )
    assert coll.compare(source, target) == icu.UCollationResult.UCOL_GREATER
    assert coll.compare("ABC", target) == icu.UCollationResult.UCOL_GREATER
    assert coll.compare(source, "abc") == icu.UCollationResult.UCOL_GREATER
    assert coll.compare("ABC", "abc") == icu.UCollationResult.UCOL_GREATER

    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    assert coll.compare("ABC", -1, "abc", -1) == icu.UCollationResult.UCOL_EQUAL
    assert coll.compare(source, target, 3) == icu.UCollationResult.UCOL_EQUAL
    assert coll.compare(source, target) == icu.UCollationResult.UCOL_EQUAL

    # UCollationResult icu::Collator::compareUTF8(
    #       const StringPiece &source,
    #       const StringPiece &target,
    #       UErrorCode &status
    # ) const
    source = "a\ufffdz".encode()
    target = b"a\x80z"
    assert coll.compare_utf8(source, target) == icu.UCollationResult.UCOL_EQUAL


def test_create_collation_element_iterator() -> None:
    coll = icu.Collator.create_instance(icu.Locale("es"))

    # [1]
    # CollationElementIterator *
    # icu::RuleBasedCollator::createCollationElementIterator(
    #       const CharacterIterator &source
    # )
    source = icu.StringCharacterIterator(icu.UnicodeString("cha"))
    it1 = coll.create_collation_element_iterator(source)
    assert isinstance(it1, icu.CollationElementIterator)

    order1 = it1.next()
    assert order1 != icu.CollationElementIterator.NULLORDER
    order2 = it1.next()
    assert order2 != icu.CollationElementIterator.NULLORDER
    order3 = it1.next()
    assert order3 != icu.CollationElementIterator.NULLORDER
    assert it1.next() == icu.CollationElementIterator.NULLORDER

    it1.set_offset(3)
    assert it1.get_offset() == 3
    assert it1.previous() == order3
    assert it1.previous() == order2
    assert it1.previous() == order1
    assert it1.previous() == icu.CollationElementIterator.NULLORDER

    assert list(it1) == [order1, order2, order3]

    # [2]
    # CollationElementIterator *
    # icu::RuleBasedCollator::createCollationElementIterator(
    #       const UnicodeString &source
    # )
    source = icu.UnicodeString("cha")
    it2 = coll.create_collation_element_iterator(source)
    assert isinstance(it2, icu.CollationElementIterator)

    order1 = it2.next()
    assert order1 != icu.CollationElementIterator.NULLORDER
    order2 = it2.next()
    assert order2 != icu.CollationElementIterator.NULLORDER
    order3 = it2.next()
    assert order3 != icu.CollationElementIterator.NULLORDER
    assert it2.next() == icu.CollationElementIterator.NULLORDER

    it2.set_offset(3)
    assert it2.get_offset() == 3
    assert it2.previous() == order3
    assert it2.previous() == order2
    assert it2.previous() == order1
    assert it2.previous() == icu.CollationElementIterator.NULLORDER

    assert list(it2) == [order1, order2, order3]

    it2a = coll.create_collation_element_iterator("cha")
    assert isinstance(it2a, icu.CollationElementIterator)

    assert it2a.next() == order1
    assert it2a.next() == order2
    assert it2a.next() == order3
    assert it2a.next() == icu.CollationElementIterator.NULLORDER


def test_create_instance() -> None:
    # [1]
    # static Collator *icu::Collator::createInstance(
    #       const Locale &loc,
    #       UErrorCode &err
    # )
    test1 = icu.Collator.create_instance(icu.Locale())
    assert isinstance(test1, icu.RuleBasedCollator)
    rules1 = test1.get_rules()

    test1a = icu.Collator.create_instance(str(icu.Locale.get_default()))
    assert isinstance(test1a, icu.RuleBasedCollator)
    rules1a = test1a.get_rules()
    assert rules1 == rules1a

    # [2]
    # static Collator *icu::Collator::createInstance(UErrorCode &err)
    test2 = icu.Collator.create_instance()
    assert isinstance(test2, icu.RuleBasedCollator)
    rules2 = test2.get_rules()
    assert rules1 == rules2


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_equal_to() -> None:
    # from icu/source/test/intltest/collationtest.cpp
    #  CollationTest::TestCollatorPredicates()
    coll = icu.Collator.create_instance(icu.Locale.get_root())

    # auto icu::Collator::equal_to() const
    assert coll.equal_to("aaa", "aaa") is True
    assert coll.equal_to(icu.UnicodeString("aaa"), "aaa") is True
    assert coll.equal_to("aaa", icu.UnicodeString("aaa")) is True
    assert coll.equal_to(icu.UnicodeString("aaa"), icu.UnicodeString("aaa")) is True

    assert coll.equal_to("aaa", "bbb") is False


def test_equals() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_english())
    source = icu.UnicodeString("ABC")
    target = icu.UnicodeString("abc")

    # UBool icu::Collator::equals(
    #       const UnicodeString &source,
    #       const UnicodeString &target
    # )
    assert coll.equals(source, target) is False
    assert coll.equals("ABC", target) is False
    assert coll.equals(source, "abc") is False
    assert coll.equals("ABC", "abc") is False

    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    assert coll.equals(source, target) is True
    assert coll.equals("ABC", target) is True
    assert coll.equals(source, "abc") is True
    assert coll.equals("ABC", "abc") is True


def test_get_attribute() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_canada_french())

    # UColAttributeValue icu::RuleBasedCollator::getAttribute(
    #       UColAttribute attr,
    #       UErrorCode &status
    # )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_FRENCH_COLLATION)
        == icu.UColAttributeValue.UCOL_ON
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_ALTERNATE_HANDLING)
        == icu.UColAttributeValue.UCOL_NON_IGNORABLE
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_CASE_FIRST)
        == icu.UColAttributeValue.UCOL_STRENGTH_LIMIT
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_CASE_LEVEL)
        == icu.UColAttributeValue.UCOL_STRENGTH_LIMIT
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_NORMALIZATION_MODE)
        == icu.UColAttributeValue.UCOL_STRENGTH_LIMIT
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_DECOMPOSITION_MODE)
        == icu.UColAttributeValue.UCOL_STRENGTH_LIMIT
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_STRENGTH)
        == icu.UColAttributeValue.UCOL_TERTIARY
    )
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_NUMERIC_COLLATION)
        == icu.UColAttributeValue.UCOL_STRENGTH_LIMIT
    )

    # void icu::RuleBasedCollator::setAttribute(
    #       UColAttribute attr,
    #       UColAttributeValue value,
    #       UErrorCode &status
    # )
    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    assert (
        coll.get_attribute(icu.UColAttribute.UCOL_STRENGTH)
        == icu.UColAttributeValue.UCOL_PRIMARY
    )


def test_get_available_locales() -> None:
    # [1]
    # static const Locale *icu::Collator::getAvailableLocales(
    #       int32_t &count
    # )
    it = icu.Collator.get_available_locales()
    assert isinstance(it, list)
    assert len(it) > 0
    assert all(isinstance(x, icu.Locale) for x in it)
    assert icu.Locale("ja") in it


def test_get_bound() -> None:
    coll = icu.Collator.create_instance(icu.Locale("sh"))
    source = coll.get_sort_key("Smith", -1)

    # static int32_t icu::Collator::getBound(
    #       const uint8_t *source,
    #       int32_t sourceLength,
    #       UColBoundMode boundType,
    #       uint32_t noOfLevels,
    #       uint8_t *result,
    #       int32_t resultLength,
    #       UErrorCode &status
    # )
    result1 = icu.Collator.get_bound(source, len(source), icu.UColBoundMode.UCOL_BOUND_LOWER, 1)
    result2 = icu.Collator.get_bound(source, len(source), icu.UColBoundMode.UCOL_BOUND_UPPER, 1)
    assert isinstance(result1, bytes)
    assert isinstance(result2, bytes)
    assert len(result1) > 0
    assert len(result2) > 0
    assert result1 < source
    assert result2 > source

    result1a = icu.Collator.get_bound(source, -1, icu.UColBoundMode.UCOL_BOUND_LOWER, 1)
    result2a = icu.Collator.get_bound(source, -1, icu.UColBoundMode.UCOL_BOUND_UPPER, 1)
    assert result1 == result1a
    assert result2 == result2a


def test_get_collation_key() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_english())

    # [1]
    # CollationKey &icu::Collator::getCollationKey(
    #       const char16_t *source,
    #       int32_t sourceLength,
    #       CollationKey &key,
    #       UErrorCode &status
    # )
    key1 = icu.CollationKey()
    result = coll.get_collation_key("ABC", -1, key1)
    assert isinstance(result, icu.CollationKey)
    assert id(result) == id(key1)

    # [2]
    # CollationKey &icu::Collator::getCollationKey(
    #       const UnicodeString &source,
    #       CollationKey &key,
    #       UErrorCode &status
    # )
    key2 = icu.CollationKey()
    result = coll.get_collation_key(icu.UnicodeString("abc"), key2)
    assert isinstance(result, icu.CollationKey)
    assert id(result) == id(key2)

    result = coll.get_collation_key("abc", key2)
    assert isinstance(result, icu.CollationKey)
    assert id(result) == id(key2)

    assert key1.compare_to(key2) == icu.UCollationResult.UCOL_GREATER
    assert key2.compare_to(key1) == icu.UCollationResult.UCOL_LESS


def test_get_display_name() -> None:
    default_locale = None
    try:
        default_locale = icu.Locale.get_default()
        display_locale = icu.Locale.get_us()
        icu.Locale.set_default(display_locale)
        object_locale = icu.Locale("ja")

        # [1]
        # static UnicodeString &icu::Collator::getDisplayName(
        #       const Locale &objectLocale,
        #       const Locale &displayLocale,
        #       UnicodeString &name
        # )
        name1 = icu.UnicodeString()
        result = icu.Collator.get_display_name(object_locale, display_locale, name1)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name1)
        assert name1 == "Japanese"

        name1.remove()
        result = icu.Collator.get_display_name("ja", display_locale, name1)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name1)
        assert name1 == "Japanese"

        name1.remove()
        result = icu.Collator.get_display_name(object_locale, "en_US", name1)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name1)
        assert name1 == "Japanese"

        name1.remove()
        result = icu.Collator.get_display_name("ja", "en_US", name1)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name1)
        assert name1 == "Japanese"

        # [2]
        # static UnicodeString &icu::Collator::getDisplayName(
        #       const Locale &objectLocale,
        #       UnicodeString &name
        # )
        name2 = icu.UnicodeString()
        result = icu.Collator.get_display_name(object_locale, name2)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name2)
        assert name2 == "Japanese"

        name2.remove()
        result = icu.Collator.get_display_name("ja", name2)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name2)
        assert name2 == "Japanese"
    finally:
        if default_locale:
            icu.Locale.set_default(default_locale)


def test_get_equivalent_reorder_codes() -> None:
    # static int32_t icu::Collator::getEquivalentReorderCodes(
    #       int32_t reorderCode,
    #       int32_t *dest,
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest = icu.Collator.get_equivalent_reorder_codes(icu.UScriptCode.USCRIPT_KATAKANA)
    assert isinstance(dest, list)
    assert dest == [
        icu.UScriptCode.USCRIPT_HIRAGANA,
        icu.UScriptCode.USCRIPT_KATAKANA,
        icu.UScriptCode.USCRIPT_KATAKANA_OR_HIRAGANA,
    ]


def test_get_functional_equivalent() -> None:
    # static Locale icu::Collator::getFunctionalEquivalent(
    #       const char *keyword,
    #       const Locale &locale,
    #       UBool &isAvailable,
    #       UErrorCode &status
    # )
    result, is_available = icu.Collator.get_functional_equivalent(
        "collation", icu.Locale.get_japanese()
    )
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale.get_japanese()
    assert is_available is True

    result, is_available = icu.Collator.get_functional_equivalent("collation", "ja")
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale.get_japanese()
    assert is_available is True


def test_get_keywords() -> None:
    # static StringEnumeration *icu::Collator::getKeywords(UErrorCode &status)
    keywords = icu.Collator.get_keywords()
    assert isinstance(keywords, icu.StringEnumeration)
    assert len(keywords) > 0
    assert "collation" in keywords

    # static StringEnumeration *icu::Collator::getKeywordValues(
    #       const char *keyword,
    #       UErrorCode &status
    # )
    values = icu.Collator.get_keyword_values("collation")
    assert isinstance(values, icu.StringEnumeration)
    assert len(values) > 0
    assert "standard" in values

    # static StringEnumeration *icu::Collator::getKeywordValuesForLocale(
    #       const char *keyword,
    #       const Locale &locale,
    #       UBool commonlyUsed,
    #       UErrorCode &status
    # )
    it = icu.Collator.get_keyword_values_for_locale(
        "collation", icu.Locale.get_japanese(), True
    )
    assert isinstance(it, icu.StringEnumeration)
    assert list(it) == ["standard", "unihan", "emoji", "eor", "search"]

    it = icu.Collator.get_keyword_values_for_locale("collation", "ja-JP", True)
    assert isinstance(it, icu.StringEnumeration)
    assert list(it) == ["standard", "unihan", "emoji", "eor", "search"]


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_max_variable() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_japanese())

    # UColReorderCode icu::Collator::getMaxVariable()
    assert coll.get_max_variable() == icu.UColReorderCode.UCOL_REORDER_CODE_PUNCTUATION

    # uint32_t icu::Collator::getVariableTop(UErrorCode &status)
    weight1 = coll.get_variable_top()
    assert isinstance(weight1, int)

    # Collator& icu::Collator::setMaxVariable(
    #       UColReorderCode group,
    #       UErrorCode &errorCode
    # )
    coll.set_max_variable(icu.UColReorderCode.UCOL_REORDER_CODE_CURRENCY)
    assert coll.get_max_variable() == icu.UColReorderCode.UCOL_REORDER_CODE_CURRENCY
    weight2 = coll.get_variable_top()
    assert isinstance(weight2, int)

    assert weight1 != weight2


def test_get_reorder_codes() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_japanese())

    # int32_t icu::Collator::getReorderCodes(
    #       int32_t *dest,
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest = coll.get_reorder_codes()
    assert isinstance(dest, list)
    assert dest == [
        icu.UScriptCode.USCRIPT_LATIN,
        icu.UScriptCode.USCRIPT_KATAKANA,
        icu.UScriptCode.USCRIPT_HAN,
    ]

    # void icu::Collator::setReorderCodes(
    #       const int32_t *reorderCodes,
    #       int32_t reorderCodesLength,
    #       UErrorCode &status
    # )
    reorder_codes = [
        icu.UScriptCode.USCRIPT_HAN,
        icu.UScriptCode.USCRIPT_KATAKANA,
        icu.UScriptCode.USCRIPT_LATIN,
    ]
    coll.set_reorder_codes(reorder_codes, len(reorder_codes))
    dest = coll.get_reorder_codes()
    assert dest == reorder_codes


def test_get_rules() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_japanese())

    # [1]
    # UnicodeString &icu::RuleBasedCollator::getRules()
    rules1 = coll.get_rules()
    assert isinstance(rules1, icu.UnicodeString)

    # [2]
    # void icu::RuleBasedCollator::getRules(
    #       UColRuleOption delta,
    #       UnicodeString &buffer
    # )
    rules2 = icu.UnicodeString()
    coll.get_rules(icu.UColRuleOption.UCOL_TAILORING_ONLY, rules2)
    assert rules2 == rules1

    rules3 = icu.UnicodeString()
    coll.get_rules(icu.UColRuleOption.UCOL_FULL_RULES, rules3)
    assert rules3 != rules1


def test_get_sort_key() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_japanese())

    # [1]
    # int32_t icu::Collator::getSortKey(
    #       const char16_t *source,
    #       int32_t sourceLength,
    #       uint8_t *result,
    #       int32_t resultLength
    # )
    result1 = coll.get_sort_key("ABC", -1)
    assert isinstance(result1, bytes)
    assert len(result1) > 0

    # [2]
    # int32_t icu::Collator::getSortKey(
    #       const UnicodeString &source,
    #       uint8_t *result,
    #       int32_t resultLength
    # )
    result2 = coll.get_sort_key(icu.UnicodeString("abc"))
    assert isinstance(result2, bytes)
    assert len(result2) > 0

    result2a = coll.get_sort_key("abc")
    assert result2a == result2

    assert result1 > result2


def test_get_tailored_set() -> None:
    coll1 = icu.Collator.create_instance(icu.Locale.get_english())

    # UnicodeSet *icu::Collator::getTailoredSet(UErrorCode &status)
    uniset1 = coll1.get_tailored_set()
    assert isinstance(uniset1, icu.UnicodeSet)
    assert len(uniset1) == 0

    coll2 = icu.Collator.create_instance(icu.Locale.get_japanese())
    uniset2 = coll2.get_tailored_set()
    assert isinstance(uniset2, icu.UnicodeSet)
    assert len(uniset2) > 0


def test_get_version() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_japanese())

    # void icu::Collator::getVersion(UVersionInfo info)
    info = coll.get_version()
    assert isinstance(info, tuple)
    assert len(info) == 4
    assert all(isinstance(x, int) for x in info)
    assert info[0] > 0


def test_greater() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_english())
    source = icu.UnicodeString("ABC")
    target = icu.UnicodeString("abc")

    # UBool icu::Collator::greater(
    #       const UnicodeString &source,
    #       const UnicodeString &target
    # )
    assert coll.greater(source, target)
    assert coll.greater("ABC", target)
    assert coll.greater(source, "abc")
    assert coll.greater("ABC", "abc")

    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    assert not coll.greater(source, target)
    assert not coll.greater("ABC", target)
    assert not coll.greater(source, "abc")
    assert not coll.greater("ABC", "abc")


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_greater_equal() -> None:
    # from icu/source/test/intltest/collationtest.cpp
    #  CollationTest::TestCollatorPredicates()
    coll = icu.Collator.create_instance(icu.Locale.get_root())

    # auto icu::Collator::greater_equal() const
    assert coll.greater_equal("aaa", "aaa") is True
    assert coll.greater_equal(icu.UnicodeString("aaa"), "aaa") is True
    assert coll.greater_equal("bbb", icu.UnicodeString("aaa")) is True
    assert coll.greater_equal(icu.UnicodeString("bbb"), icu.UnicodeString("aaa")) is True

    assert coll.greater_equal("aaa", "bbb") is False


def test_greater_or_equal() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_english())
    source = icu.UnicodeString("ABC")
    target = icu.UnicodeString("abc")

    # UBool icu::Collator::greaterOrEqual(
    #       const UnicodeString &source,
    #       const UnicodeString &target
    # )
    assert coll.greater_or_equal(source, target)
    assert coll.greater_or_equal("ABC", target)
    assert coll.greater_or_equal(source, "abc")
    assert coll.greater_or_equal("ABC", "abc")

    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    assert coll.greater_or_equal(source, target)
    assert coll.greater_or_equal("ABC", target)
    assert coll.greater_or_equal(source, "abc")
    assert coll.greater_or_equal("ABC", "abc")


def test_hash_code() -> None:
    coll1 = icu.Collator.create_instance(icu.Locale.get_english())
    coll2 = icu.Collator.create_instance(icu.Locale("da", "DK", ""))
    coll3 = icu.Collator.create_instance(icu.Locale.get_english())

    # int32_t icu::Collator::hashCode(void)
    assert coll1.hash_code() != coll2.hash_code()
    assert coll1.hash_code() == coll3.hash_code()
    assert coll2.hash_code() != coll3.hash_code()

    assert hash(coll1) == coll1.hash_code()
    assert hash(coll2) == coll2.hash_code()
    assert hash(coll3) == coll3.hash_code()


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_less() -> None:
    # from icu/source/test/intltest/collationtest.cpp
    #  CollationTest::TestCollatorPredicates()
    coll = icu.Collator.create_instance(icu.Locale.get_root())

    # auto icu::Collator::less() const
    assert coll.less("aaa", "bbb") is True
    assert coll.less(icu.UnicodeString("aaa"), "bbb") is True
    assert coll.less("aaa", icu.UnicodeString("bbb")) is True
    assert coll.less(icu.UnicodeString("aaa"), icu.UnicodeString("bbb")) is True

    assert coll.less("aaa", "aaa") is False
    assert coll.less("bbb", "aaa") is False


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_less_equal() -> None:
    # from icu/source/test/intltest/collationtest.cpp
    #  CollationTest::TestCollatorPredicates()
    coll = icu.Collator.create_instance(icu.Locale.get_root())

    # auto icu::Collator::less_equal() const
    assert coll.less_equal("aaa", "aaa") is True
    assert coll.less_equal(icu.UnicodeString("aaa"), "aaa") is True
    assert coll.less_equal("aaa", icu.UnicodeString("bbb")) is True
    assert coll.less_equal(icu.UnicodeString("aaa"), icu.UnicodeString("bbb")) is True

    assert coll.less_equal("bbb", "aaa") is False


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_not_equal_to() -> None:
    # from icu/source/test/intltest/collationtest.cpp
    #  CollationTest::TestCollatorPredicates()
    coll = icu.Collator.create_instance(icu.Locale.get_root())

    # auto icu::Collator::not_equal_to() const
    assert coll.not_equal_to("aaa", "bbb") is True
    assert coll.not_equal_to(icu.UnicodeString("aaa"), "bbb") is True
    assert coll.not_equal_to("aaa", icu.UnicodeString("bbb")) is True
    assert coll.not_equal_to(icu.UnicodeString("aaa"), icu.UnicodeString("bbb")) is True

    assert coll.not_equal_to("aaa", "aaa") is False


def test_operator() -> None:
    coll1 = icu.Collator.create_instance(icu.Locale.get_english())
    coll2 = icu.Collator.create_instance(icu.Locale("da", "DK", ""))
    coll3 = icu.Collator.create_instance(icu.Locale.get_english())

    # UBool icu::Collator::operator!=(const Collator &other)
    assert coll1 != coll2

    # UBool icu::Collator::operator==(const Collator &other)
    assert coll1 == coll3
    assert coll2 != coll3


def test_rule_based_collator() -> None:
    assert issubclass(icu.RuleBasedCollator, icu.Collator)

    base = icu.Collator.create_instance(icu.Locale.get_japanese())
    rules = base.get_rules()
    assert len(rules) > 0

    # [1]
    # icu::RuleBasedCollator::RuleBasedCollator(
    #       const UnicodeString &rules,
    #       UErrorCode &status
    # )
    test1 = icu.RuleBasedCollator(rules)
    assert test1.get_rules() == rules

    test1a = icu.RuleBasedCollator(str(rules))
    assert test1a.get_rules() == rules

    # [2]
    # icu::RuleBasedCollator::RuleBasedCollator(
    #       const UnicodeString &rules,
    #       ECollationStrength collationStrength,
    #       UErrorCode &status
    # )
    test2 = icu.RuleBasedCollator(rules, icu.Collator.ECollationStrength.TERTIARY)
    assert test2.get_rules() == rules

    test2a = icu.RuleBasedCollator(str(rules), icu.Collator.TERTIARY)
    assert test2a.get_rules() == rules

    # [3]
    # icu::RuleBasedCollator::RuleBasedCollator(
    #       const UnicodeString &rules,
    #       UColAttributeValue decompositionMode,
    #       UErrorCode &status
    # )
    test3 = icu.RuleBasedCollator(rules, icu.UColAttributeValue.UCOL_ON)
    assert test3.get_rules() == rules

    test3a = icu.RuleBasedCollator(str(rules), icu.UColAttributeValue.UCOL_ON)
    assert test3a.get_rules() == rules

    # [4]
    # icu::RuleBasedCollator::RuleBasedCollator(
    #       const UnicodeString &rules,
    #       ECollationStrength collationStrength,
    #       UColAttributeValue decompositionMode,
    #       UErrorCode &status
    # )
    test4 = icu.RuleBasedCollator(
        rules,
        icu.Collator.ECollationStrength.TERTIARY,
        icu.UColAttributeValue.UCOL_ON,
    )
    assert test4.get_rules() == rules

    test4a = icu.RuleBasedCollator(
        str(rules),
        icu.Collator.ECollationStrength.TERTIARY,
        icu.UColAttributeValue.UCOL_ON,
    )
    assert test4a.get_rules() == rules

    # [6]
    # icu::RuleBasedCollator::RuleBasedCollator(const RuleBasedCollator &other)
    test6 = icu.RuleBasedCollator(test1)
    assert test6.get_rules() == rules

    # int32_t icu::RuleBasedCollator::cloneBinary(
    #       uint8_t *buffer,
    #       int32_t capacity,
    #       UErrorCode &status
    # )
    test1.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    buffer = test1.clone_binary()
    assert isinstance(buffer, bytes)
    assert len(buffer) > 0

    # [7]
    # icu::RuleBasedCollator::RuleBasedCollator(
    #       const uint8_t *bin,
    #       int32_t length,
    #       const RuleBasedCollator *base,
    #       UErrorCode &status
    # )
    root = icu.Collator.create_instance(icu.Locale.get_root())
    test7 = icu.RuleBasedCollator(buffer, len(buffer), root)
    assert test1 == test7
    assert (
        test7.get_attribute(icu.UColAttribute.UCOL_STRENGTH)
        == icu.UColAttributeValue.UCOL_PRIMARY
    )

    buffer2 = test7.clone_binary()
    assert len(buffer) == len(buffer2)
    assert buffer == buffer2

    test7a = icu.RuleBasedCollator(buffer2, -1, root)
    assert test1 == test7 == test7a
    assert (
        test7a.get_attribute(icu.UColAttribute.UCOL_STRENGTH)
        == icu.UColAttributeValue.UCOL_PRIMARY
    )

    buffer3 = test7a.clone_binary()
    assert len(buffer) == len(buffer3)
    assert buffer == buffer3


def test_sort() -> None:
    # From
    #  https://unicode-org.github.io/icu/userguide/collation/customization/#simple-tailoring-examples
    src = [
        "CUKIĆ RADOJICA",
        "ČUKIĆ SLOBODAN",
        "CUKIĆ SVETOZAR",
        "ČUKIĆ ZORAN",
        "CURIĆ MILOŠ",
        "ĆURIĆ MILOŠ",
        "CVRKALJ ÐURO",
    ]
    base = icu.Collator.create_instance(icu.Locale("sr_Latn"))
    rules = base.get_rules() + icu.UnicodeString("& C < č <<< Č < ć <<< Ć")
    coll = icu.RuleBasedCollator(rules)

    def _cmp(_a: str, _b: str) -> int:
        nonlocal coll
        _result = coll.compare(_a, -1, _b, -1)
        return (
            -1
            if _result == icu.UCollationResult.UCOL_LESS
            else 0
            if _result == icu.UCollationResult.UCOL_EQUAL
            else 1
        )

    result1 = sorted(src)
    result2 = sorted(src, key=cmp_to_key(_cmp))
    result3 = sorted(src, key=lambda s: coll.get_sort_key(s, -1))  # type: ignore
    assert result1 != result2
    assert result1 != result3
    assert result2 == result3
    assert result3 == [
        "CUKIĆ RADOJICA",
        "CUKIĆ SVETOZAR",
        "CURIĆ MILOŠ",
        "CVRKALJ ÐURO",
        "ČUKIĆ SLOBODAN",
        "ČUKIĆ ZORAN",
        "ĆURIĆ MILOŠ",
    ]

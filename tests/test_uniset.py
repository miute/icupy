import copy

import pytest
from icupy.icu import (
    ICUError, INT32_MAX, ParsePosition, UErrorCode, UMatchDegree,
    UProperty, USET_IGNORE_SPACE, USetSpanCondition, U_ICU_VERSION_MAJOR_NUM,
    UnicodeSet, UnicodeString,
)


def test_add():
    test1 = UnicodeSet()
    assert test1.size() == 0

    # [1]
    # UnicodeSet &icu::UnicodeSet::add(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::add(UChar32 c)
    #
    # [3]
    # UnicodeSet &icu::UnicodeSet::add(UChar32 start, UChar32 end)
    result = test1.add(UnicodeString("ab")).add(0xdf).add(0x30, 0x39)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 12  # [0-9\u00DF{ab}]

    result = test1.add("cd").add("ef")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 14  # [0-9ab\u00DF{ab}{cd}{ef}]


def test_add_all():
    test1 = UnicodeSet()
    assert test1.size() == 0
    test2 = UnicodeSet(UnicodeString("[\u00DF{ab}]"))
    assert test2.size() == 2

    # [1]
    # UnicodeSet &icu::UnicodeSet::addAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::addAll(const UnicodeString &s)
    result = test1.add_all(test2).add_all(UnicodeString("0123456789"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2 + 10  # [0-9\u00DF{ab}]

    result = test1.add_all("cd").add_all("ef")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2 + 10 + 4  # [0-9c-f\u00DF{ab}]


def test_add_match_set_to():
    test1 = UnicodeSet(UnicodeString("[0-9]"))
    test2 = UnicodeSet(UnicodeString("[a-z]"))
    assert test1.size() == 10
    assert test2.size() == 26

    # void icu::UnicodeSet::addMatchSetTo(UnicodeSet &toUnionTo)
    test1.add_match_set_to(test2)
    assert test1.size() == 10  # [0-9]
    assert test1.contains(0x30, 0x39)
    assert test2.size() == 10 + 26  # [0-9a-z]
    assert test2.contains(0x30, 0x39)
    assert test2.contains(0x61, 0x7a)


def test_api():
    test1 = UnicodeSet(0x30, 0x39)

    # int32_t icu::UnicodeSet::getRangeCount(void)
    assert test1.get_range_count() == 1

    # UChar32 icu::UnicodeSet::getRangeEnd(int32_t index)
    assert test1.get_range_end(0) == 0x39

    # UChar32 icu::UnicodeSet::getRangeStart(int32_t index)
    assert test1.get_range_start(0) == 0x30

    # int32_t icu::UnicodeSet::indexOf(UChar32 c)
    assert test1.index_of(0x39) == 9
    assert test1.index_of(0x61) == -1

    # UBool icu::UnicodeSet::isBogus(void)
    assert not test1.is_bogus()

    # UBool icu::UnicodeSet::isEmpty(void)
    assert not test1.is_empty()

    # UBool icu::UnicodeSet::isFrozen()
    assert not test1.is_frozen()

    # int32_t icu::UnicodeSet::size(void)
    assert test1.size() == 10
    assert len(test1) == 10

    # UnicodeSet &icu::UnicodeSet::compact()
    result = test1.compact()
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)

    # UnicodeSet &icu::UnicodeSet::clear(void)
    result = test1.clear()
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)

    assert not test1.is_bogus()
    assert test1.is_empty()
    assert not test1.is_frozen()
    assert test1.size() == 0
    assert len(test1) == 0

    # UnicodeSet *icu::UnicodeSet::freeze()
    test1.add(0x30, 0x39)
    result = test1.freeze()
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert not test1.is_bogus()
    assert not test1.is_empty()
    assert test1.is_frozen()
    assert test1.size() == 10
    assert len(test1) == 10

    # static UBool icu::UnicodeSet::resemblesPattern(
    #       const UnicodeString &pattern,
    #       int32_t pos
    # )
    pattern = UnicodeString("[0-9\u00DF{ab}]")
    assert UnicodeSet.resembles_pattern(pattern, 0)
    assert not UnicodeSet.resembles_pattern(pattern, 1)

    assert UnicodeSet.resembles_pattern("[0-9\u00DF{ab}]", 0)
    assert not UnicodeSet.resembles_pattern("[0-9\u00DF{ab}]", 1)

    test2 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert not test2.is_bogus()
    assert test2.size() == 12

    # UnicodeString &icu::UnicodeSet::toPattern(
    #       UnicodeString &result,
    #       UBool escapeUnprintable = false
    # )
    result = UnicodeString()
    output = test2.to_pattern(result)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == "[0-9\xDF{ab}]"

    output = test2.to_pattern(result, False)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == "[0-9\xDF{ab}]"

    output = test2.to_pattern(result, True)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert result == "[0-9\\u00DF{ab}]"

    # UnicodeSet &icu::UnicodeSet::set(
    #       UChar32 start,
    #       UChar32 end
    # )
    result = test2.set(0x30, 0x39)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 10

    # void icu::UnicodeSet::setToBogus()
    test2.set_to_bogus()
    assert test2.is_bogus()


def test_apply_int_property_value():
    test1 = UnicodeSet()
    assert test1.size() == 0

    # UnicodeSet &icu::UnicodeSet::applyIntPropertyValue(
    #       UProperty prop,
    #       int32_t value,
    #       UErrorCode &ec
    # )
    result = test1.apply_int_property_value(
        UProperty.UCHAR_CANONICAL_COMBINING_CLASS,
        8)  # 8 = Kana Voicing
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2  # [\u3099\u309A]
    assert test1.contains(0x3099)
    assert test1.contains(0x309a)

    with pytest.raises(ICUError) as exc_info:
        test1.apply_int_property_value(UProperty.UCHAR_INT_LIMIT, 0)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_apply_pattern():
    # [1]
    # UnicodeSet &icu::UnicodeSet::applyPattern(
    #       const UnicodeString &pattern,
    #       ParsePosition &pos,
    #       uint32_t options,
    #       const SymbolTable *symbols,
    #       UErrorCode &status
    # )
    test1 = UnicodeSet()
    pattern = UnicodeString("[a-z{ab}]")
    pos = ParsePosition()
    options = UnicodeSet.IGNORE_SPACE | UnicodeSet.CASE_INSENSITIVE
    result = test1.apply_pattern(pattern, pos, options, None)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert pos.get_index() == pattern.length()
    assert test1.size() == 26 * 2 + 3  # [A-Za-z\u017F\u212A{ab}]
    assert test1.contains(0x41, 0x5a)
    assert test1.contains(0x61, 0x7a)

    pos = ParsePosition()
    result = test1.apply_pattern("[0-9]", pos, options, None)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert pos.get_index() == 5
    assert test1.size() == 10  # [0-9]
    assert test1.contains(0x30, 0x39)

    with pytest.raises(ICUError) as exc_info:
        test1.apply_pattern(UnicodeString(), pos, options, None)
    assert exc_info.value.args[0] == UErrorCode.U_MALFORMED_SET

    # [2]
    # UnicodeSet &icu::UnicodeSet::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    test2 = UnicodeSet()
    result = test2.apply_pattern(pattern)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 26 + 1  # [a-z{ab}]
    assert test2.contains(0x61, 0x7a)

    result = test2.apply_pattern("[0-9]")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 10  # [0-9]
    assert test2.contains(0x30, 0x39)

    with pytest.raises(ICUError) as exc_info:
        test2.apply_pattern(UnicodeString())
    assert exc_info.value.args[0] == UErrorCode.U_MALFORMED_SET


def test_apply_property_alias():
    test1 = UnicodeSet()

    # UnicodeSet &icu::UnicodeSet::applyPropertyAlias(
    #       const UnicodeString &prop,
    #       const UnicodeString &value,
    #       UErrorCode &ec
    # )
    result = test1.apply_property_alias(
        UnicodeString("Canonical_Combining_Class"),
        UnicodeString("Kana_Voicing"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2  # [\u3099\u309A]
    assert test1.contains(0x3099)
    assert test1.contains(0x309a)

    with pytest.raises(ICUError) as exc_info:
        test1.apply_property_alias(UnicodeString("ccc"),
                                   UnicodeString("-"))
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    result = test1.apply_property_alias("ccc", UnicodeString("10"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 1
    assert test1.contains(0x05B0)

    result = test1.apply_property_alias(UnicodeString("ccc"), "11")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 1
    assert test1.contains(0x05B1)

    result = test1.apply_property_alias("ccc", "12")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 1
    assert test1.contains(0x05B2)


def test_char_at():
    test1 = UnicodeSet(0x30, 0x39)
    assert test1.size() == 10

    # UChar32 icu::UnicodeSet::charAt(int32_t index)
    assert test1.char_at(9) == 0x39
    assert test1.char_at(10) == -1
    assert test1.char_at(-1) == -1

    # __getitem__(self, index: int) -> int
    assert test1[9] == 0x39
    with pytest.raises(IndexError):  # IndexError: list index out of range
        _ = test1[10]
    assert test1[-1] == 0x39

    # __getitem__(self, slice: slice) -> List[int]
    assert test1[1:4] == [0x31, 0x32, 0x33]
    assert test1[1:6:2] == [0x31, 0x33, 0x35]
    with pytest.raises(ValueError):  # ValueError: slice step cannot be zero
        _ = test1[::0]

    test1.add(UnicodeString("ab"))
    t = [c for c in test1]
    assert t == [
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, -1,
    ]


def test_clone():
    test1 = UnicodeSet(0x30, 0x39)
    assert not test1.is_frozen()

    # UnicodeSet *icu::UnicodeSet::clone()
    test2 = test1.clone()
    assert isinstance(test2, UnicodeSet)
    assert test2 == test1
    assert not test2.is_frozen()

    test1.freeze()
    test3 = test1.clone()
    assert test3 == test1
    assert test1.is_frozen()
    assert test3.is_frozen()

    # UnicodeSet *icu::UnicodeSet::cloneAsThawed()
    test4 = test1.clone_as_thawed()
    assert isinstance(test4, UnicodeSet)
    assert test4 == test1
    assert test1.is_frozen()
    assert not test4.is_frozen()

    test5 = copy.copy(test1)
    assert test5 == test1
    assert test5.is_frozen()

    test6 = copy.deepcopy(test1)
    assert test6 == test1
    assert test6.is_frozen()


def test_close_over():
    pattern = UnicodeString("[aq\u00DF{Bc}{bC}{Fi}]")
    test1 = UnicodeSet(pattern)
    test2 = test1.clone()

    # UnicodeSet &icu::UnicodeSet::closeOver(int32_t attribute)
    result = test2.close_over(UnicodeSet.CASE_INSENSITIVE |
                              UnicodeSet.ADD_CASE_MAPPINGS)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)


def test_complement():
    test1 = UnicodeSet(UnicodeSet.MIN_VALUE, UnicodeSet.MAX_VALUE)

    # [1]
    # UnicodeSet &icu::UnicodeSet::complement(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::complement(UChar32 c)
    #
    # [3]
    # UnicodeSet &icu::UnicodeSet::complement(UChar32 start, UChar32 end)
    #
    # [4]
    # UnicodeSet &icu::UnicodeSet::complement()
    result = (test1.complement(UnicodeString("ab"))
              .complement("cd")
              .complement(0xdf)
              .complement(0x30, 0x39)
              .complement())
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 13  # [0-9\u00DF{ab}{cd}]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(0xdf)
    assert test1.contains("ab")
    assert test1.contains("cd")


def test_complement_all():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12
    test2 = UnicodeSet(0x30, 0x39)
    assert test2.size() == 10

    # [1]
    # UnicodeSet &icu::UnicodeSet::complementAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::complementAll(const UnicodeString &s)
    result = (test1.complement_all(test2)
              .complement_all(UnicodeString("ab"))
              .complement_all("cd"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 12 - 10 + 2 + 2  # [a-d\u00DF{ab}]
    assert test1.contains(0x61, 0x64)
    assert test1.contains(0xdf)
    assert test1.contains("ab")


def test_contains():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::contains(const UnicodeString &s)
    assert test1.contains(UnicodeString("0"))
    assert not test1.contains(UnicodeString("01"))

    assert test1.contains("0")
    assert not test1.contains("01")

    assert UnicodeString("0") in test1
    assert UnicodeString("01") not in test1

    assert "0" in test1
    assert "01" not in test1

    # [2]
    # UBool icu::UnicodeSet::contains(UChar32 c)
    assert test1.contains(0x30)
    assert not test1.contains(0x61)

    assert 0x30 in test1
    assert 0x61 not in test1

    # [3]
    # UBool icu::UnicodeSet::contains(UChar32 start, UChar32 end)
    assert test1.contains(0x30, 0x39)
    assert not test1.contains(0x61, 0x61)


def test_contains_all():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::containsAll(const UnicodeSet &c)
    assert test1.contains_all(UnicodeSet(0x30, 0x39))
    assert not test1.contains_all(UnicodeSet(0x2f, 0x3a))

    # [2]
    # UBool icu::UnicodeSet::containsAll(const UnicodeString &s)
    assert test1.contains_all(UnicodeString("0123456789"))
    assert not test1.contains_all(UnicodeString("/0123456789:"))

    assert test1.contains_all("0123456789")
    assert not test1.contains_all("/0123456789:")


def test_contains_none():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::containsNone(const UnicodeSet &c)
    assert not test1.contains_none(UnicodeSet(0x2f, 0x3a))
    assert test1.contains_none(UnicodeSet(0x2f, 0x2f))

    # [2]
    # UBool icu::UnicodeSet::containsNone(const UnicodeString &s)
    assert not test1.contains_none(UnicodeString("/0123456789:"))
    assert test1.contains_none(UnicodeString("/"))

    assert not test1.contains_none("/0123456789:")
    assert test1.contains_none("/")

    # [3]
    # UBool icu::UnicodeSet::containsNone(UChar32 start, UChar32 end)
    assert not test1.contains_none(0x2f, 0x3a)
    assert test1.contains_none(0x2f, 0x2f)


def test_contains_some():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::containsSome(const UnicodeSet &s)
    assert test1.contains_some(UnicodeSet(UnicodeString("[0-9{ab}]")))
    assert not test1.contains_some(UnicodeSet(UnicodeString("[a-z{ab}]")))

    # [2]
    # UBool icu::UnicodeSet::containsSome(const UnicodeString &s)
    assert test1.contains_some(UnicodeString("ab0123456789"))
    assert not test1.contains_some(UnicodeString("ab"))

    assert test1.contains_some("ab0123456789")
    assert not test1.contains_some("ab")

    # [3]
    # UBool icu::UnicodeSet::containsSome(UChar32 start, UChar32 end)
    assert test1.contains_some(0x2f, 0x3a)
    assert not test1.contains_some(0x2f, 0x2f)


def test_create_from():
    # static UnicodeSet *icu::UnicodeSet::createFrom(const UnicodeString &s)
    test1 = UnicodeSet.create_from(UnicodeString("ab"))
    assert isinstance(test1, UnicodeSet)
    assert test1.size() == 1  # [{ab}]
    assert test1.contains(UnicodeString("ab"))

    test1a = UnicodeSet.create_from("ab")
    assert isinstance(test1a, UnicodeSet)
    assert test1a.size() == 1  # [{ab}]
    assert test1a.contains(UnicodeString("ab"))


def test_create_from_all():
    # static UnicodeSet *icu::UnicodeSet::createFromAll(const UnicodeString &s)
    test1 = UnicodeSet.create_from_all(UnicodeString("ab"))
    assert isinstance(test1, UnicodeSet)
    assert test1.size() == 2  # [ab]
    assert test1.contains(0x61, 0x62)

    test1 = UnicodeSet.create_from_all("ab")
    assert isinstance(test1, UnicodeSet)
    assert test1.size() == 2  # [ab]
    assert test1.contains(0x61, 0x62)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_from_uset():
    from icupy.icu import u_get_binary_property_set

    # [1]
    # const UnicodeSet *icu::UnicodeSet::fromUSet(const USet *uset)
    uset1 = u_get_binary_property_set(UProperty.UCHAR_POSIX_BLANK)
    test1 = UnicodeSet.from_uset(uset1)
    assert isinstance(test1, UnicodeSet)
    assert test1.size() > 0
    assert test1.contains(0x20)  # U+0020: Space (SP)

    # USet *icu::UnicodeSet::toUSet()
    uset2 = test1.to_uset()

    # [2]
    # UnicodeSet *icu::UnicodeSet::fromUSet(USet *uset)
    test2 = UnicodeSet.from_uset(uset2)
    assert isinstance(test2, UnicodeSet)
    assert test2 == test1


def test_hash_code():
    test1 = UnicodeSet(0x30, 0x39)
    test2 = UnicodeSet()

    # int32_t icu::UnicodeSet::hashCode(void)
    assert test1.hash_code() != test2.hash_code()

    test2.add(0x30, 0x39)
    assert test1.hash_code() == test2.hash_code()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_has_strings():
    test1 = UnicodeSet("[a-z]")
    test2 = UnicodeSet("[a-z{ab}]")

    # UBool icu::UnicodeSet::hasStrings()
    assert not test1.has_strings()
    assert test2.has_strings()


def test_matches():
    test1 = UnicodeSet(UnicodeString("[0-9{abc}]"))
    text = UnicodeString("abcd 789")

    # UMatchDegree icu::UnicodeSet::matches(
    #       const Replaceable &text,
    #       int32_t &offset,
    #       int32_t limit,
    #       UBool incremental
    # )
    offset = 0
    result, offset = test1.matches(text, offset, text.length(), True)
    assert result == UMatchDegree.U_MATCH
    assert offset == 3

    result, offset = test1.matches(text, offset, text.length(), True)
    assert result == UMatchDegree.U_MISMATCH
    assert offset == 3

    offset = 5
    result, offset = test1.matches(text, offset, text.length(), True)
    assert result == UMatchDegree.U_MATCH
    assert offset == 6


def test_operator():
    test1 = UnicodeSet(0x30, 0x39)
    test2 = UnicodeSet(UnicodeString("[0-9]"))
    test3 = UnicodeSet(UnicodeString("[0-9{ab}]"))

    # UBool icu::UnicodeSet::operator!=(const UnicodeSet &o)
    assert not (test1 != test2)
    assert test1 != test3
    assert test2 != test3

    # UBool icu::UnicodeSet::operator==(const UnicodeSet &o)
    assert test1 == test2
    assert not (test1 == test3)
    assert not (test2 == test3)


def test_remove():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::remove(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::remove(UChar32 c)
    #
    # [3]
    # UnicodeSet &icu::UnicodeSet::remove(UChar32 start, UChar32 end)
    result = test1.remove(UnicodeString("ab")).remove(0xdf).remove(0x30, 0x39)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 0

    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    result = test1.remove("ab").remove(0xdf).remove(0x30, 0x39)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 0


def test_remove_all():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    test2 = test1.clone()
    assert test2.size() == 12
    test3 = UnicodeSet(0x30, 0x39)

    # [1]
    # UnicodeSet &icu::UnicodeSet::removeAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::removeAll(const UnicodeString &s)
    result = test2.remove_all(test3).remove_all(UnicodeString("ab"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 12 - 10  # [\u00DF{ab}]
    assert test2.contains(0xdf)
    assert test2.contains(UnicodeString("ab"))

    test2 = test1.clone()
    result = test2.remove_all(test3).remove_all("ab")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 12 - 10  # [\u00DF{ab}]
    assert test2.contains(0xdf)
    assert test2.contains(UnicodeString("ab"))


def test_remove_all_strings():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12

    # UnicodeSet &icu::UnicodeSet::removeAllStrings()
    result = test1.remove_all_strings()
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 11  # [0-9\u00DF]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(0xdf)
    assert not test1.contains("ab")


def test_retain():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::retain(UChar32 c)
    result = test2.retain(0xdf)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 1 + 1  # [\u00DF{ab}]
    assert test1.contains(0xdf)

    # [2]
    # UnicodeSet &icu::UnicodeSet::retain(UChar32 start, UChar32 end)
    result = test3.retain(0x30, 0x39)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 10 + 1  # [0-9{ab}]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(UnicodeString("ab"))


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_retain_69():
    test1 = UnicodeSet("[0-9\u00DF{ab}]")
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::retain(const UnicodeString &s)
    result = test2.retain(UnicodeString("\xdf"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 1 + 1  # [\u00DF{ab}]
    assert test2.contains(0xdf)
    assert test2.contains("ab")

    result = test3.retain("ab")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 1  # [{ab}]
    assert test3.contains("ab")


def test_retain_all():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::retainAll(const UnicodeSet &c)
    result = test2.retain_all(UnicodeSet(0xdf, 0xdf))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 1  # [\u00DF]
    assert test2.contains(0xdf)

    # [2]
    # UnicodeSet &icu::UnicodeSet::retainAll(const UnicodeString &s)
    result = test3.retain_all(UnicodeString("ab"))
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 0

    test3 = test1.clone()
    result = test3.retain_all("ab")
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 0


def test_serialize():
    # int32_t icu::UnicodeSet::serialize(
    #       uint16_t *dest,
    #       int32_t destCapacity,
    #       UErrorCode &ec
    # )
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    dest = test1.serialize()
    assert isinstance(dest, list)
    assert dest == [
        4,  # length=((n+2*m)|(m!=0?0x8000:0)) (n=4, m=0)
        ord("0"), ord("9") + 1,  # bmp code points
        0xdf, 0xdf + 1,
    ]

    test2 = UnicodeSet(UnicodeString("[\U0001F000-\U0001F003]"))
    dest = test2.serialize()
    assert isinstance(dest, list)
    assert dest == [
        (0 + 2 * 2) | 0x8000,  # length=((n+2*m)|(m!=0?0x8000:0)) (n=0, m=2)
        0,  # bmpLength
        1, 0xf000,  # supplementary code points
        (0x1f003 + 1) >> 16, (0x1f003 + 1) & 0xffff,
    ]


def test_span():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))

    # [1]
    # int32_t icu::UnicodeSet::span(
    #       const char16_t *s,
    #       int32_t length,
    #       USetSpanCondition spanCondition
    # )
    assert test1.span(
        "abcde",
        -1,
        USetSpanCondition.USET_SPAN_CONTAINED) == 2

    # [2]
    # int32_t icu::UnicodeSet::span(
    #       const UnicodeString &s,
    #       int32_t start,
    #       USetSpanCondition spanCondition
    # )
    assert test1.span(
        UnicodeString("abcde"),
        0,
        USetSpanCondition.USET_SPAN_CONTAINED) == 2


def test_span_back():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))

    # [1]
    # int32_t icu::UnicodeSet::spanBack(
    #       const char16_t *s,
    #       int32_t length,
    #       USetSpanCondition spanCondition
    # )
    assert test1.span_back(
        "aaaab",
        -1,
        USetSpanCondition.USET_SPAN_CONTAINED) == 3

    # [2]
    # int32_t icu::UnicodeSet::spanBack(
    #       const UnicodeString &s,
    #       int32_t limit,
    #       USetSpanCondition spanCondition
    # )
    assert test1.span_back(
        UnicodeString("aaaab"),
        INT32_MAX,
        USetSpanCondition.USET_SPAN_CONTAINED) == 3


def test_unicode_set():
    # [1]
    # icu::UnicodeSet::UnicodeSet()
    test1 = UnicodeSet()
    assert test1.size() == 0

    # [2]
    # icu::UnicodeSet::UnicodeSet(UChar32 start, UChar32 end)
    test2 = UnicodeSet(0x30, 0x39)
    assert test2.size() == 10
    assert test2.contains(0x30, 0x39)

    # [4]
    # icu::UnicodeSet::UnicodeSet(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    test4 = UnicodeSet(UnicodeString("[0-9]"))
    assert test4.size() == 10
    assert test4.contains(0x30, 0x39)

    test4a = UnicodeSet("[0-9]")
    assert test4a.size() == 10
    assert test4a.contains(0x30, 0x39)

    # [6]
    # icu::UnicodeSet::UnicodeSet(
    #       const UnicodeString &pattern,
    #       ParsePosition &pos,
    #       uint32_t options,
    #       const SymbolTable *symbols,
    #       UErrorCode &status
    # )
    pattern = UnicodeString("foo [ 0-9 ]")
    pos = ParsePosition()
    pos.set_index(4)
    options = USET_IGNORE_SPACE
    test6 = UnicodeSet(pattern, pos, options, None)
    assert pos.get_index() == pattern.length()
    assert test6.size() == 10
    assert test6.contains(0x30, 0x39)

    pos = ParsePosition()
    pos.set_index(4)
    test6a = UnicodeSet("foo [ 0-9 ]", pos, options, None)
    assert pos.get_index() == 11
    assert test6a.size() == 10
    assert test6a.contains(0x30, 0x39)

    # [7]
    # icu::UnicodeSet::UnicodeSet(const UnicodeSet &o)
    test7 = UnicodeSet(test2)
    assert test7.size() == 10
    assert test7.contains(0x30, 0x39)

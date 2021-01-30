import copy

import pytest

from icupy import (
    ICUException, INT32_MAX, ParsePosition, UErrorCode, UMatchDegree,
    UnicodeSet, UnicodeString, UProperty, USET_IGNORE_SPACE,
    USetSpanCondition, u_get_binary_property_set,
)


def test_add():
    test1 = UnicodeSet()
    assert test1.size() == 0

    # [1]
    # UnicodeSet& add(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet& add(UChar32 c)
    #
    # [3]
    # UnicodeSet& add(UChar32 start, UChar32 end)
    test1.add(UnicodeString("ab")).add(0xdf).add(0x30, 0x39)
    assert test1.size() == 12  # [0-9\u00DF{ab}]


def test_add_all():
    test1 = UnicodeSet()
    assert test1.size() == 0
    test2 = UnicodeSet(UnicodeString("[\u00DF{ab}]"))
    assert test2.size() == 2

    # [1]
    # UnicodeSet& addAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet& addAll(const UnicodeString &s)
    test1.add_all(test2).add_all(UnicodeString("0123456789"))
    assert test1.size() == 2 + 10  # [0-9\u00DF{ab}]


def test_add_match_set_to():
    test1 = UnicodeSet(UnicodeString("[0-9]"))
    test2 = UnicodeSet(UnicodeString("[a-z]"))
    assert test1.size() == 10
    assert test2.size() == 26

    test1.add_match_set_to(test2)
    assert test1.size() == 10  # [0-9]
    assert test1.contains(0x30, 0x39)
    assert test2.size() == 10 + 26  # [0-9a-z]
    assert test2.contains(0x30, 0x39)
    assert test2.contains(0x61, 0x7a)


def test_api():
    test1 = UnicodeSet(0x30, 0x39)
    assert test1.get_range_count() == 1
    assert test1.get_range_end(0) == 0x39
    assert test1.get_range_start(0) == 0x30

    assert test1.index_of(0x39) == 9
    assert test1.index_of(0x61) == -1

    assert not test1.is_bogus()
    assert not test1.is_empty()
    assert not test1.is_frozen()
    assert test1.size() == 10
    assert len(test1) == 10

    test1.compact()

    test1.clear()
    assert not test1.is_bogus()
    assert test1.is_empty()
    assert not test1.is_frozen()
    assert test1.size() == 0
    assert len(test1) == 0

    test1.add(0x30, 0x39)
    test1.freeze()
    assert not test1.is_bogus()
    assert not test1.is_empty()
    assert test1.is_frozen()
    assert test1.size() == 10
    assert len(test1) == 10

    pattern = UnicodeString("[0-9\u00DF{ab}]")
    assert UnicodeSet.resembles_pattern(pattern, 0)
    assert not UnicodeSet.resembles_pattern(pattern, 1)

    test2 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert not test2.is_bogus()
    assert test2.size() == 12

    dest = UnicodeString()
    test2.to_pattern(dest)
    assert dest == "[0-9\xDF{ab}]"

    test2.to_pattern(dest, False)
    assert dest == "[0-9\xDF{ab}]"

    test2.to_pattern(dest, True)
    assert dest == "[0-9\\u00DF{ab}]"

    test2.set(0x30, 0x39)
    assert test2.size() == 10

    test2.set_to_bogus()
    assert test2.is_bogus()


def test_apply_int_property_value():
    test1 = UnicodeSet()
    assert test1.size() == 0

    # UnicodeSet& applyIntPropertyValue(UProperty prop,
    #                                   int32_t value,
    #                                   UErrorCode &ec
    # )
    test1.apply_int_property_value(
        UProperty.UCHAR_CANONICAL_COMBINING_CLASS,
        8)  # 8 = Kana Voicing
    assert test1.size() == 2  # [\u3099\u309A]
    assert test1.contains(0x3099)
    assert test1.contains(0x309a)

    with pytest.raises(ICUException) as exc_info:
        test1.apply_int_property_value(UProperty.UCHAR_INT_LIMIT, 0)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_apply_pattern():
    # [1]
    # UnicodeSet& applyPattern(const UnicodeString &pattern,
    #                          ParsePosition &pos,
    #                          uint32_t options,
    #                          const SymbolTable *symbols,
    #                          UErrorCode &status
    # )
    test1 = UnicodeSet()
    pattern = UnicodeString("[a-z{ab}]")
    pos = ParsePosition()
    options = UnicodeSet.IGNORE_SPACE | UnicodeSet.CASE_INSENSITIVE
    test1.apply_pattern(pattern, pos, options, None)
    assert pos.get_index() == pattern.length()
    assert test1.size() == 26 * 2 + 3  # [A-Za-z\u017F\u212A{ab}]
    assert test1.contains(0x41, 0x5a)
    assert test1.contains(0x61, 0x7a)

    with pytest.raises(ICUException) as exc_info:
        test1.apply_pattern(UnicodeString(), pos, options, None)
    assert exc_info.value.args[0] == UErrorCode.U_MALFORMED_SET

    # [2]
    # UnicodeSet& applyPattern(const UnicodeString &pattern,
    #                          UErrorCode &status
    # )
    test2 = UnicodeSet()
    test2.apply_pattern(pattern)
    assert test2.size() == 26 + 1  # [a-z{ab}]
    assert test2.contains(0x61, 0x7a)

    with pytest.raises(ICUException) as exc_info:
        test2.apply_pattern(UnicodeString())
    assert exc_info.value.args[0] == UErrorCode.U_MALFORMED_SET


def test_apply_property_alias():
    test1 = UnicodeSet()

    # UnicodeSet& applyPropertyAlias(const UnicodeString &prop,
    #                                const UnicodeString &value,
    #                                UErrorCode &ec
    # )
    test1.apply_property_alias(UnicodeString("Canonical_Combining_Class"),
                               UnicodeString("Kana_Voicing"))
    assert test1.size() == 2  # [\u3099\u309A]
    assert test1.contains(0x3099)
    assert test1.contains(0x309a)

    with pytest.raises(ICUException) as exc_info:
        test1.apply_property_alias(UnicodeString("ccc"),
                                   UnicodeString("-"))
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_char_at():
    test1 = UnicodeSet(0x30, 0x39)
    assert test1.size() == 10

    assert test1.char_at(9) == 0x39
    assert test1.char_at(10) == -1
    assert test1.char_at(-1) == -1

    assert test1[9] == 0x39
    with pytest.raises(IndexError):  # IndexError: list index out of range
        _ = test1[10]
    assert test1[-1] == 0x39

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

    test2 = test1.clone()
    assert test2 == test1
    assert not test2.is_frozen()

    test1.freeze()
    test3 = test1.clone()
    assert test3 == test1
    assert test1.is_frozen()
    assert test3.is_frozen()

    test4 = test1.clone_as_thawed()
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
    assert test2 == test1
    test2.close_over(UnicodeSet.CASE_INSENSITIVE |
                     UnicodeSet.ADD_CASE_MAPPINGS)
    assert test2 != test1


def test_complement():
    test1 = UnicodeSet(UnicodeSet.MIN_VALUE, UnicodeSet.MAX_VALUE)

    # [1]
    # UnicodeSet& complement(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet& complement(UChar32 c)
    #
    # [3]
    # UnicodeSet& complement(UChar32 start, UChar32 end)
    #
    # [4]
    # UnicodeSet& complement()
    test1.complement(
        UnicodeString("ab")
    ).complement(
        0xdf
    ).complement(
        0x30, 0x39
    ).complement()
    assert test1.size() == 12  # [0-10\u00DF{ab}]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(0xdf)
    assert test1.contains(UnicodeString("ab"))


def test_complement_all():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12
    test2 = UnicodeSet(0x30, 0x39)
    assert test2.size() == 10

    # [1]
    # UnicodeSet& complementAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet& complementAll(const UnicodeString &s)
    test1.complement_all(test2).complement_all(UnicodeString("ab"))
    assert test1.size() == 12 - 10 + 2  # [ab]
    assert test1.contains(0x61, 0x62)


def test_contains():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool contains(const UnicodeString &s)
    assert test1.contains(UnicodeString("0"))
    assert not test1.contains(UnicodeString("01"))

    assert UnicodeString("0") in test1
    assert UnicodeString("01") not in test1

    # [2]
    # UBool contains(UChar32 c)
    assert test1.contains(0x30)
    assert not test1.contains(0x61)

    assert 0x30 in test1
    assert 0x61 not in test1

    # [3]
    # UBool contains(UChar32 start, UChar32 end)
    assert test1.contains(0x30, 0x39)
    assert not test1.contains(0x61, 0x61)


def test_contains_all():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool containsAll(const UnicodeSet &c)
    assert test1.contains_all(UnicodeSet(0x30, 0x39))
    assert not test1.contains_all(UnicodeSet(0x2f, 0x3a))

    # [2]
    # UBool containsAll(const UnicodeString &s)
    assert test1.contains_all(UnicodeString("0123456789"))
    assert not test1.contains_all(UnicodeString("/0123456789:"))


def test_contains_none():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool containsNone(const UnicodeSet &c)
    assert not test1.contains_none(UnicodeSet(0x2f, 0x3a))
    assert test1.contains_none(UnicodeSet(0x2f, 0x2f))

    # [2]
    # UBool containsNone(const UnicodeString &s)
    assert not test1.contains_none(UnicodeString("/0123456789:"))
    assert test1.contains_none(UnicodeString("/"))

    # [3]
    # UBool containsNone(UChar32 start, UChar32 end)
    assert not test1.contains_none(0x2f, 0x3a)
    assert test1.contains_none(0x2f, 0x2f)


def test_contains_some():
    test1 = UnicodeSet(0x30, 0x39)

    # [1]
    # UBool containsSome(const UnicodeSet &s)
    assert test1.contains_some(UnicodeSet(UnicodeString("[0-9{ab}]")))
    assert not test1.contains_some(UnicodeSet(UnicodeString("[a-z{ab}]")))

    # [2]
    # UBool containsSome(const UnicodeString &s)
    assert test1.contains_some(UnicodeString("ab0123456789"))
    assert not test1.contains_some(UnicodeString("ab"))

    # [3]
    # UBool containsSome(UChar32 start, UChar32 end)
    assert test1.contains_some(0x2f, 0x3a)
    assert not test1.contains_some(0x2f, 0x2f)


def test_create_from():
    test1 = UnicodeSet.create_from(UnicodeString("ab"))
    assert isinstance(test1, UnicodeSet)
    assert test1.size() == 1  # [{ab}]
    assert test1.contains(UnicodeString("ab"))


def test_create_from_all():
    test1 = UnicodeSet.create_from_all(UnicodeString("ab"))
    assert isinstance(test1, UnicodeSet)
    assert test1.size() == 2  # [ab]
    assert test1.contains(0x61, 0x62)


def test_from_uset():
    # [1]
    # const UnicodeSet * fromUSet(const USet *uset)
    uset1 = u_get_binary_property_set(UProperty.UCHAR_POSIX_BLANK)
    test1 = UnicodeSet.from_uset(uset1)
    assert isinstance(test1, UnicodeSet)
    assert test1.size() > 0
    assert test1.contains(0x20)  # U+0020: Space (SP)

    # [2]
    # UnicodeSet * fromUSet(USet *uset)
    uset2 = test1.to_uset()
    test2 = UnicodeSet.from_uset(uset2)
    assert isinstance(test2, UnicodeSet)
    assert test2 == test1


def test_hash_code():
    test1 = UnicodeSet(0x30, 0x39)
    test2 = UnicodeSet()

    assert test1.hash_code() != test2.hash_code()

    test2.add(0x30, 0x39)
    assert test1.hash_code() == test2.hash_code()


def test_matches():
    test1 = UnicodeSet(UnicodeString("[0-9{abc}]"))
    text = UnicodeString("abcd 789")

    # UMatchDegree matches(const Replaceable &text,
    #                      int32_t &offset,
    #                      int32_t limit,
    #                      UBool incremental
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

    # operator!=()
    assert not (test1 != test2)
    assert test1 != test3
    assert test2 != test3

    # operator==()
    assert test1 == test2
    assert not (test1 == test3)
    assert not (test2 == test3)


def test_remove():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12

    # [1]
    # UnicodeSet& remove(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet& remove(UChar32 c)
    #
    # [3]
    # UnicodeSet& remove(UChar32 start, UChar32 end)
    test1.remove(UnicodeString("ab")).remove(0xdf).remove(0x30, 0x39)
    assert test1.size() == 0


def test_remove_all():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12
    test2 = UnicodeSet(0x30, 0x39)

    # [1]
    # UnicodeSet& removeAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet& removeAll(const UnicodeString &s)
    test1.remove_all(test2).remove_all(UnicodeString("ab"))
    assert test1.size() == 12 - 10  # [\u00DF{ab}]
    assert test1.contains(0xdf)
    assert test1.contains(UnicodeString("ab"))


def test_remove_all_strings():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    assert test1.size() == 12

    assert test1.remove_all_strings()
    assert test1.size() == 11  # [0-9\u00DF]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(0xdf)


def test_retain():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet& retain(UChar32 c)
    test2.retain(0xdf)
    assert test2.size() == 1 + 1  # [\u00DF{ab}]
    assert test1.contains(0xdf)

    # [2]
    # UnicodeSet& retain(UChar32 start, UChar32 end)
    test3.retain(0x30, 0x39)
    assert test3.size() == 10 + 1  # [0-9{ab}]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(UnicodeString("ab"))


def test_retain_all():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet& retainAll(const UnicodeSet &c)
    test2.retain_all(UnicodeSet(0xdf, 0xdf))
    assert test2.size() == 1  # [\u00DF]
    assert test2.contains(0xdf)

    # [2]
    # UnicodeSet& retainAll(const UnicodeString &s)
    test3.retain_all(UnicodeString("ab"))
    assert test3.size() == 0


def test_serialize():
    # int32_t serialize(uint16_t *dest,
    #                   int32_t destCapacity,
    #                   UErrorCode &ec
    # )
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))
    result = test1.serialize()
    assert isinstance(result, list)
    assert result == [
        4,  # length=((n+2*m)|(m!=0?0x8000:0)) (n=4, m=0)
        ord("0"), ord("9") + 1,  # bmp code points
        0xdf, 0xdf + 1,
    ]

    test2 = UnicodeSet(UnicodeString("[\U0001F000-\U0001F003]"))
    result = test2.serialize()
    assert isinstance(result, list)
    assert result == [
        (0 + 2 * 2) | 0x8000,  # length=((n+2*m)|(m!=0?0x8000:0)) (n=0, m=2)
        0,  # bmpLength
        1, 0xf000,  # supplementary code points
        (0x1f003 + 1) >> 16, (0x1f003 + 1) & 0xffff,
    ]


def test_span():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))

    # [1]
    # int32_t span(const char16_t *s,
    #              int32_t length,
    #              USetSpanCondition spanCondition
    # )
    assert test1.span("abcde",
                      -1,
                      USetSpanCondition.USET_SPAN_CONTAINED) == 2

    # [2]
    # int32_t span(const UnicodeString &s,
    #              int32_t start,
    #              USetSpanCondition spanCondition
    # )
    assert test1.span(UnicodeString("abcde"),
                      0,
                      USetSpanCondition.USET_SPAN_CONTAINED) == 2


def test_span_back():
    test1 = UnicodeSet(UnicodeString("[0-9\u00DF{ab}]"))

    # [1]
    # int32_t spanBack(const char16_t *s,
    #                  int32_t length,
    #                  USetSpanCondition spanCondition
    # )
    assert test1.span_back("aaaab",
                           -1,
                           USetSpanCondition.USET_SPAN_CONTAINED) == 3

    # [2]
    # int32_t spanBack(const UnicodeString &s,
    #                  int32_t limit,
    #                  USetSpanCondition spanCondition
    # )
    assert test1.span_back(UnicodeString("aaaab"),
                           INT32_MAX,
                           USetSpanCondition.USET_SPAN_CONTAINED) == 3


def test_to_uset():
    test1 = UnicodeSet(0x30, 0x39)

    uset1 = test1.to_uset()
    assert not isinstance(uset1, UnicodeSet)
    assert uset1 == test1


def test_unicode_set():
    # [1]
    # UnicodeSet()
    test1 = UnicodeSet()
    assert test1.size() == 0

    # [2]
    # UnicodeSet(UChar32 start, UChar32 end)
    test2 = UnicodeSet(0x30, 0x39)
    assert test2.size() == 10

    # [4]
    # UnicodeSet(const UnicodeString &pattern,
    #            UErrorCode &status
    # )
    test4 = UnicodeSet(UnicodeString("[0-9]"))
    assert test4.size() == 10

    # [6]
    # UnicodeSet(const UnicodeString &pattern,
    #            ParsePosition &pos,
    #            uint32_t options,
    #            const SymbolTable *symbols,
    #            UErrorCode &status
    # )
    pattern = UnicodeString("foo [ 0-9 ]")
    pos = ParsePosition()
    pos.set_index(4)
    options = USET_IGNORE_SPACE
    test6 = UnicodeSet(pattern, pos, options, None)
    assert pos.get_index() == pattern.length()
    assert test6.size() == 10

    # [7]
    # UnicodeSet(const UnicodeSet &o)
    test7 = UnicodeSet(test2)
    assert test7.size() == 10

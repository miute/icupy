import copy

import pytest

from icupy import icu


def test_add() -> None:
    test1 = icu.UnicodeSet()
    assert test1.size() == 0

    # [1]
    # UnicodeSet &icu::UnicodeSet::add(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::add(UChar32 c)
    #
    # [3]
    # UnicodeSet &icu::UnicodeSet::add(UChar32 start, UChar32 end)
    result = test1.add(icu.UnicodeString("ab")).add(0xDF).add(0x30, 0x39)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 12  # [0-9\u00DF{ab}]

    result = test1.add("cd").add("ef")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 14  # [0-9ab\u00DF{ab}{cd}{ef}]


def test_add_all() -> None:
    test1 = icu.UnicodeSet()
    assert test1.size() == 0
    test2 = icu.UnicodeSet(icu.UnicodeString("[\u00df{ab}]"))
    assert test2.size() == 2

    # [1]
    # UnicodeSet &icu::UnicodeSet::addAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::addAll(const UnicodeString &s)
    result = test1.add_all(test2).add_all(icu.UnicodeString("0123456789"))
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2 + 10  # [0-9\u00DF{ab}]

    result = test1.add_all("cd").add_all("ef")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2 + 10 + 4  # [0-9c-f\u00DF{ab}]


def test_add_match_set_to() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9]"))
    test2 = icu.UnicodeSet(icu.UnicodeString("[a-z]"))
    assert test1.size() == 10
    assert test2.size() == 26

    # void icu::UnicodeSet::addMatchSetTo(UnicodeSet &toUnionTo)
    test1.add_match_set_to(test2)
    assert test1.size() == 10  # [0-9]
    assert test1.contains(0x30, 0x39)
    assert test2.size() == 10 + 26  # [0-9a-z]
    assert test2.contains(0x30, 0x39)
    assert test2.contains(0x61, 0x7A)


def test_api() -> None:
    assert icu.UnicodeSet.IGNORE_SPACE == icu.USET_IGNORE_SPACE
    assert icu.UnicodeSet.CASE_INSENSITIVE == icu.USET_CASE_INSENSITIVE
    assert icu.UnicodeSet.ADD_CASE_MAPPINGS == icu.USET_ADD_CASE_MAPPINGS
    assert (
        icu.UnicodeSet.SERIALIZED_STATIC_ARRAY_CAPACITY
        == icu.USET_SERIALIZED_STATIC_ARRAY_CAPACITY
    )

    test1 = icu.UnicodeSet(0x30, 0x39)

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
    assert test1.is_bogus() is False

    # UBool icu::UnicodeSet::isEmpty(void)
    assert test1.is_empty() is False

    # UBool icu::UnicodeSet::isFrozen()
    assert test1.is_frozen() is False

    # int32_t icu::UnicodeSet::size(void)
    assert test1.size() == 10
    assert len(test1) == 10

    # UnicodeSet &icu::UnicodeSet::compact()
    result = test1.compact()
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)

    # UnicodeSet &icu::UnicodeSet::clear(void)
    result = test1.clear()
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)

    assert test1.is_bogus() is False
    assert test1.is_empty() is True
    assert test1.is_frozen() is False
    assert test1.size() == 0
    assert len(test1) == 0

    # UnicodeSet *icu::UnicodeSet::freeze()
    test1.add(0x30, 0x39)
    result = test1.freeze()
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.is_bogus() is False
    assert test1.is_empty() is False
    assert test1.is_frozen() is True
    assert test1.size() == 10
    assert len(test1) == 10

    # static UBool icu::UnicodeSet::resemblesPattern(
    #       const UnicodeString &pattern,
    #       int32_t pos
    # )
    pattern = icu.UnicodeString("[0-9\u00df{ab}]")
    assert icu.UnicodeSet.resembles_pattern(pattern, 0) is True
    assert icu.UnicodeSet.resembles_pattern(pattern, 1) is False

    assert icu.UnicodeSet.resembles_pattern("[0-9\u00df{ab}]", 0) is True
    assert icu.UnicodeSet.resembles_pattern("[0-9\u00df{ab}]", 1) is False

    test2 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    assert test2.is_bogus() is False
    assert test2.size() == 12

    # UnicodeString &icu::UnicodeSet::toPattern(
    #       UnicodeString &result,
    #       UBool escapeUnprintable = false
    # )
    result = icu.UnicodeString()
    output = test2.to_pattern(result)
    assert isinstance(output, icu.UnicodeString)
    assert id(result) == id(output)
    assert result == "[0-9\xdf{ab}]"

    output = test2.to_pattern(result, False)
    assert isinstance(output, icu.UnicodeString)
    assert id(result) == id(output)
    assert result == "[0-9\xdf{ab}]"

    output = test2.to_pattern(result, True)
    assert isinstance(output, icu.UnicodeString)
    assert id(result) == id(output)
    assert result == "[0-9\\u00DF{ab}]"

    # UnicodeSet &icu::UnicodeSet::set(
    #       UChar32 start,
    #       UChar32 end
    # )
    result = test2.set(0x30, 0x39)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 10

    # void icu::UnicodeSet::setToBogus()
    test2.set_to_bogus()
    assert test2.is_bogus() is True

    # UnicodeSet.__repr__() -> str
    assert repr(test1) == "<UnicodeSet('[0-9]')>"

    test3 = icu.UnicodeSet(0, 16).add_all("'\"\\")
    assert repr(test3) == "<UnicodeSet('[\\u0000-\\u0010\"\\'\\\\]')>"


def test_apply_int_property_value() -> None:
    test1 = icu.UnicodeSet()
    assert test1.size() == 0

    # UnicodeSet &icu::UnicodeSet::applyIntPropertyValue(
    #       UProperty prop,
    #       int32_t value,
    #       UErrorCode &ec
    # )
    result = test1.apply_int_property_value(
        icu.UProperty.UCHAR_CANONICAL_COMBINING_CLASS, 8
    )  # 8 = Kana Voicing
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2  # [\u3099\u309A]
    assert test1.contains(0x3099)
    assert test1.contains(0x309A)

    with pytest.raises(icu.ICUError) as exc_info:
        test1.apply_int_property_value(icu.UProperty.UCHAR_INT_LIMIT, 0)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_apply_pattern() -> None:
    # [1]
    # UnicodeSet &icu::UnicodeSet::applyPattern(
    #       const UnicodeString &pattern,
    #       ParsePosition &pos,
    #       uint32_t options,
    #       const SymbolTable *symbols,
    #       UErrorCode &status
    # )
    test1 = icu.UnicodeSet()
    pattern = icu.UnicodeString("[a-z{ab}]")
    pos = icu.ParsePosition()
    options = icu.UnicodeSet.IGNORE_SPACE | icu.UnicodeSet.CASE_INSENSITIVE
    result = test1.apply_pattern(pattern, pos, options, None)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert pos.get_index() == pattern.length()
    assert test1.size() == 26 * 2 + 3  # [A-Za-z\u017F\u212A{ab}]
    assert test1.contains(0x41, 0x5A)
    assert test1.contains(0x61, 0x7A)

    pos = icu.ParsePosition()
    result = test1.apply_pattern("[0-9]", pos, options, None)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert pos.get_index() == 5
    assert test1.size() == 10  # [0-9]
    assert test1.contains(0x30, 0x39)

    with pytest.raises(icu.ICUError) as exc_info:
        test1.apply_pattern(icu.UnicodeString(), pos, options, None)
    assert exc_info.value.args[0] == icu.UErrorCode.U_MALFORMED_SET

    # [2]
    # UnicodeSet &icu::UnicodeSet::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    test2 = icu.UnicodeSet()
    result = test2.apply_pattern(pattern)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 26 + 1  # [a-z{ab}]
    assert test2.contains(0x61, 0x7A)

    result = test2.apply_pattern("[0-9]")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 10  # [0-9]
    assert test2.contains(0x30, 0x39)

    with pytest.raises(icu.ICUError) as exc_info:
        test2.apply_pattern(icu.UnicodeString())
    assert exc_info.value.args[0] == icu.UErrorCode.U_MALFORMED_SET


def test_apply_property_alias() -> None:
    test1 = icu.UnicodeSet()

    # UnicodeSet &icu::UnicodeSet::applyPropertyAlias(
    #       const UnicodeString &prop,
    #       const UnicodeString &value,
    #       UErrorCode &ec
    # )
    result = test1.apply_property_alias(
        icu.UnicodeString("Canonical_Combining_Class"),
        icu.UnicodeString("Kana_Voicing"),
    )
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 2  # [\u3099\u309A]
    assert test1.contains(0x3099)
    assert test1.contains(0x309A)

    with pytest.raises(icu.ICUError) as exc_info:
        test1.apply_property_alias(icu.UnicodeString("ccc"), icu.UnicodeString("-"))
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    result = test1.apply_property_alias("ccc", icu.UnicodeString("10"))
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 1
    assert test1.contains(0x05B0)

    result = test1.apply_property_alias(icu.UnicodeString("ccc"), "11")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 1
    assert test1.contains(0x05B1)

    result = test1.apply_property_alias("ccc", "12")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 1
    assert test1.contains(0x05B2)


def test_char_at() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)
    assert test1.size() == 10

    # UChar32 icu::UnicodeSet::charAt(int32_t index)
    assert test1.char_at(9) == 0x39
    assert test1.char_at(10) == -1
    assert test1.char_at(-1) == -1

    # # __getitem__(self, index: int) -> int
    # assert test1[9] == 0x39
    # with pytest.raises(IndexError):  # IndexError: list index out of range
    #     _ = test1[10]
    # assert test1[-1] == 0x39

    # # __getitem__(self, slice: slice) -> List[int]
    # assert isinstance(test1[:], UnicodeSet)
    # assert test1[1:4] == UnicodeSet(0x31, 0x33)
    # assert test1[1:6:2] == UnicodeSet("[135]")
    # with pytest.raises(ValueError):  # ValueError: slice step cannot be zero
    #     _ = test1[::0]


def test_clone() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)
    assert not test1.is_frozen()

    # UnicodeSet *icu::UnicodeSet::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.UnicodeSet)
    assert test2 == test1
    assert not test2.is_frozen()

    test1.freeze()
    test3 = test1.clone()
    assert test3 == test1
    assert test1.is_frozen()
    assert test3.is_frozen()

    # UnicodeSet *icu::UnicodeSet::cloneAsThawed()
    test4 = test1.clone_as_thawed()
    assert isinstance(test4, icu.UnicodeSet)
    assert test4 == test1
    assert test1.is_frozen()
    assert not test4.is_frozen()

    test5 = copy.copy(test1)
    assert test5 == test1
    assert test5.is_frozen()

    test6 = copy.deepcopy(test1)
    assert test6 == test1
    assert test6.is_frozen()


def test_close_over() -> None:
    pattern = icu.UnicodeString("[aq\u00df{Bc}{bC}{Fi}]")
    test1 = icu.UnicodeSet(pattern)
    test2 = test1.clone()

    # UnicodeSet &icu::UnicodeSet::closeOver(int32_t attribute)
    result = test2.close_over(
        icu.UnicodeSet.CASE_INSENSITIVE | icu.UnicodeSet.ADD_CASE_MAPPINGS
    )
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_code_points() -> None:
    # from icu/source/test/intltest/usettest.cpp
    #  UnicodeSetTest::TestCodePointIterator()
    us = icu.UnicodeSet("[abcçカ🚴]")

    # U_HEADER_NESTED_NAMESPACE::USetCodePoints
    # icu::UnicodeSet::codePoints() const
    it = us.code_points()
    assert [icu.UnicodeString(x) for x in it] == [
        "a",
        "b",
        "c",
        "ç",
        "カ",
        "🚴",
    ]


def test_complement() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeSet.MIN_VALUE, icu.UnicodeSet.MAX_VALUE)

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
    result = (
        test1.complement(icu.UnicodeString("ab"))
        .complement("cd")
        .complement(0xDF)
        .complement(0x30, 0x39)
        .complement()
    )
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 13  # [0-9\u00DF{ab}{cd}]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(0xDF)
    assert test1.contains("ab")
    assert test1.contains("cd")


def test_complement_all() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    assert test1.size() == 12
    test2 = icu.UnicodeSet(0x30, 0x39)
    assert test2.size() == 10

    # [1]
    # UnicodeSet &icu::UnicodeSet::complementAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::complementAll(const UnicodeString &s)
    result = (
        test1.complement_all(test2).complement_all(icu.UnicodeString("ab")).complement_all("cd")
    )
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 12 - 10 + 2 + 2  # [a-d\u00DF{ab}]
    assert test1.contains(0x61, 0x64)
    assert test1.contains(0xDF)
    assert test1.contains("ab")


def test_contains() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::contains(const UnicodeString &s)
    assert test1.contains(icu.UnicodeString("0")) is True
    assert test1.contains(icu.UnicodeString("01")) is False

    assert test1.contains("0") is True
    assert test1.contains("01") is False

    assert icu.UnicodeString("0") in test1
    assert icu.UnicodeString("01") not in test1

    assert "0" in test1
    assert "01" not in test1

    # [2]
    # UBool icu::UnicodeSet::contains(UChar32 c)
    assert test1.contains(0x30) is True
    assert test1.contains(0x61) is False

    assert 0x30 in test1
    assert 0x61 not in test1

    # [3]
    # UBool icu::UnicodeSet::contains(UChar32 start, UChar32 end)
    assert test1.contains(0x30, 0x39) is True
    assert test1.contains(0x61, 0x61) is False


def test_contains_all() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::containsAll(const UnicodeSet &c)
    assert test1.contains_all(icu.UnicodeSet(0x30, 0x39)) is True
    assert test1.contains_all(icu.UnicodeSet(0x2F, 0x3A)) is False

    # [2]
    # UBool icu::UnicodeSet::containsAll(const UnicodeString &s)
    assert test1.contains_all(icu.UnicodeString("0123456789")) is True
    assert test1.contains_all(icu.UnicodeString("/0123456789:")) is False

    assert test1.contains_all("0123456789") is True
    assert test1.contains_all("/0123456789:") is False


def test_contains_none() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::containsNone(const UnicodeSet &c)
    assert test1.contains_none(icu.UnicodeSet(0x2F, 0x3A)) is False
    assert test1.contains_none(icu.UnicodeSet(0x2F, 0x2F)) is True

    # [2]
    # UBool icu::UnicodeSet::containsNone(const UnicodeString &s)
    assert test1.contains_none(icu.UnicodeString("/0123456789:")) is False
    assert test1.contains_none(icu.UnicodeString("/")) is True

    assert test1.contains_none("/0123456789:") is False
    assert test1.contains_none("/") is True

    # [3]
    # UBool icu::UnicodeSet::containsNone(UChar32 start, UChar32 end)
    assert test1.contains_none(0x2F, 0x3A) is False
    assert test1.contains_none(0x2F, 0x2F) is True


def test_contains_some() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)

    # [1]
    # UBool icu::UnicodeSet::containsSome(const UnicodeSet &s)
    assert test1.contains_some(icu.UnicodeSet(icu.UnicodeString("[0-9{ab}]"))) is True
    assert test1.contains_some(icu.UnicodeSet(icu.UnicodeString("[a-z{ab}]"))) is False

    # [2]
    # UBool icu::UnicodeSet::containsSome(const UnicodeString &s)
    assert test1.contains_some(icu.UnicodeString("ab0123456789")) is True
    assert test1.contains_some(icu.UnicodeString("ab")) is False

    assert test1.contains_some("ab0123456789") is True
    assert test1.contains_some("ab") is False

    # [3]
    # UBool icu::UnicodeSet::containsSome(UChar32 start, UChar32 end)
    assert test1.contains_some(0x2F, 0x3A) is True
    assert test1.contains_some(0x2F, 0x2F) is False


def test_create_from() -> None:
    # static UnicodeSet *icu::UnicodeSet::createFrom(const UnicodeString &s)
    test1 = icu.UnicodeSet.create_from(icu.UnicodeString("ab"))
    assert isinstance(test1, icu.UnicodeSet)
    assert test1.size() == 1  # [{ab}]
    assert test1.contains(icu.UnicodeString("ab"))

    test1a = icu.UnicodeSet.create_from("ab")
    assert isinstance(test1a, icu.UnicodeSet)
    assert test1a.size() == 1  # [{ab}]
    assert test1a.contains(icu.UnicodeString("ab"))


def test_create_from_all() -> None:
    # static UnicodeSet *icu::UnicodeSet::createFromAll(const UnicodeString &s)
    test1 = icu.UnicodeSet.create_from_all(icu.UnicodeString("ab"))
    assert isinstance(test1, icu.UnicodeSet)
    assert test1.size() == 2  # [ab]
    assert test1.contains(0x61, 0x62)

    test1 = icu.UnicodeSet.create_from_all("ab")
    assert isinstance(test1, icu.UnicodeSet)
    assert test1.size() == 2  # [ab]
    assert test1.contains(0x61, 0x62)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_from_uset() -> None:
    # [1]
    # const UnicodeSet *icu::UnicodeSet::fromUSet(const USet *uset)
    uset1 = icu.u_get_binary_property_set(icu.UProperty.UCHAR_POSIX_BLANK)
    test1 = icu.UnicodeSet.from_uset(uset1)
    assert isinstance(test1, icu.UnicodeSet)
    assert test1.size() > 0
    assert test1.contains(0x20)  # U+0020: Space (SP)

    # USet *icu::UnicodeSet::toUSet()
    uset2 = test1.to_uset()

    # [2]
    # UnicodeSet *icu::UnicodeSet::fromUSet(USet *uset)
    test2 = icu.UnicodeSet.from_uset(uset2)
    assert isinstance(test2, icu.UnicodeSet)
    assert test2 == test1


def test_hash_code() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)
    test2 = icu.UnicodeSet()

    # int32_t icu::UnicodeSet::hashCode(void)
    assert test1.hash_code() != test2.hash_code()

    assert hash(test1) == test1.hash_code()
    assert hash(test2) == test2.hash_code()

    test2.add(0x30, 0x39)
    assert test1.hash_code() == test2.hash_code()

    assert hash(test1) == test1.hash_code()
    assert hash(test2) == test2.hash_code()


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_has_strings() -> None:
    test1 = icu.UnicodeSet("[a-z]")
    test2 = icu.UnicodeSet("[a-z{ab}]")

    # UBool icu::UnicodeSet::hasStrings()
    assert test1.has_strings() is False
    assert test2.has_strings() is True


def test_iter() -> None:
    # from icu/source/test/intltest/usettest.cpp
    #  UnicodeSetTest::TestElementIterator()
    us = icu.UnicodeSet("[abcçカ🚴{}{abc}{de}]")

    result = icu.UnicodeString()
    [result.append(' "').append(el).append('"') for el in us]
    if icu.U_ICU_VERSION_MAJOR_NUM < 76:
        assert result == ' "a" "b" "c" "ç" "カ" "🚴" "" "" ""'
    else:
        assert result == ' "a" "b" "c" "ç" "カ" "🚴" "" "abc" "de"'

    us = icu.UnicodeSet("[a-cA-D{}{abc}{de}]")
    assert all(isinstance(x, icu.UnicodeString) for x in us)
    assert us[0] == "A"
    assert us[1] == "B"
    assert us[2] == "C"
    assert us[3] == "D"
    assert us[4] == "a"
    assert us[5] == "b"
    assert us[6] == "c"
    if icu.U_ICU_VERSION_MAJOR_NUM < 76:
        assert len(us[7]) == 0
        assert len(us[8]) == 0
        assert len(us[9]) == 0
    else:
        assert len(us[7]) == 0
        assert us[8] == "abc"
        assert us[9] == "de"

    with pytest.raises(IndexError):
        _ = us[10]  # IndexError: elements index out of range

    if icu.U_ICU_VERSION_MAJOR_NUM < 76:
        assert len(us[-1]) == 0
    else:
        assert us[-1] == "de"


def test_matches() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9{abc}]"))
    text = icu.UnicodeString("abcd 789")

    # UMatchDegree icu::UnicodeSet::matches(
    #       const Replaceable &text,
    #       int32_t &offset,
    #       int32_t limit,
    #       UBool incremental
    # )
    offset = 0
    result, offset = test1.matches(text, offset, text.length(), True)
    assert result == icu.UMatchDegree.U_MATCH
    assert offset == 3

    result, offset = test1.matches(text, offset, text.length(), True)
    assert result == icu.UMatchDegree.U_MISMATCH
    assert offset == 3

    offset = 5
    result, offset = test1.matches(text, offset, text.length(), True)
    assert result == icu.UMatchDegree.U_MATCH
    assert offset == 6


def test_operator() -> None:
    test1 = icu.UnicodeSet(0x30, 0x39)
    test2 = icu.UnicodeSet(icu.UnicodeString("[0-9]"))
    test3 = icu.UnicodeSet(icu.UnicodeString("[0-9{ab}]"))

    # UBool icu::UnicodeSet::operator!=(const UnicodeSet &o)
    assert not (test1 != test2)
    assert test1 != test3
    assert test2 != test3

    # UBool icu::UnicodeSet::operator==(const UnicodeSet &o)
    assert test1 == test2
    assert not (test1 == test3)
    assert not (test2 == test3)

    # UnicodeSet.__eq__(_USetPtr) -> bool
    assert test1 == test2.to_uset()
    assert not (test1 == test3.to_uset())
    assert not (test2 == test3.to_uset())

    assert not (test1 != test2.to_uset())
    assert test1 != test3.to_uset()
    assert test2 != test3.to_uset()


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_operator_63() -> None:
    # UnicodeSet.__eq__(_ConstUSetPtr) -> bool
    uset = icu.u_get_binary_property_set(icu.UProperty.UCHAR_POSIX_BLANK)
    test1 = icu.UnicodeSet.from_uset(uset)
    assert test1 == uset
    assert not (test1 != uset)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_ranges() -> None:
    # from icu/source/test/intltest/usettest.cpp
    #  UnicodeSetTest::TestRangeIterator()
    us = icu.UnicodeSet("[abcçカ🚴]")

    # U_HEADER_NESTED_NAMESPACE::USetRanges icu::UnicodeSet::ranges() const
    ranges = [(it.range_start, it.range_end) for it in us.ranges()]
    result = icu.UnicodeString()
    [result.append(" ").append(start).append("-").append(end) for (start, end) in ranges]
    assert result == " a-c ç-ç カ-カ 🚴-🚴"


def test_remove() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::remove(const UnicodeString &s)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::remove(UChar32 c)
    #
    # [3]
    # UnicodeSet &icu::UnicodeSet::remove(UChar32 start, UChar32 end)
    result = test1.remove(icu.UnicodeString("ab")).remove(0xDF).remove(0x30, 0x39)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 0

    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    result = test1.remove("ab").remove(0xDF).remove(0x30, 0x39)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 0


def test_remove_all() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    test2 = test1.clone()
    assert test2.size() == 12
    test3 = icu.UnicodeSet(0x30, 0x39)

    # [1]
    # UnicodeSet &icu::UnicodeSet::removeAll(const UnicodeSet &c)
    #
    # [2]
    # UnicodeSet &icu::UnicodeSet::removeAll(const UnicodeString &s)
    result = test2.remove_all(test3).remove_all(icu.UnicodeString("ab"))
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 12 - 10  # [\u00DF{ab}]
    assert test2.contains(0xDF)
    assert test2.contains(icu.UnicodeString("ab"))

    test2 = test1.clone()
    result = test2.remove_all(test3).remove_all("ab")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 12 - 10  # [\u00DF{ab}]
    assert test2.contains(0xDF)
    assert test2.contains(icu.UnicodeString("ab"))


def test_remove_all_strings() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    assert test1.size() == 12

    # UnicodeSet &icu::UnicodeSet::removeAllStrings()
    result = test1.remove_all_strings()
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test1)
    assert test1.size() == 11  # [0-9\u00DF]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(0xDF)
    assert not test1.contains("ab")


def test_retain() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::retain(UChar32 c)
    result = test2.retain(0xDF)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 1 + 1  # [\u00DF{ab}]
    assert test1.contains(0xDF)

    # [2]
    # UnicodeSet &icu::UnicodeSet::retain(UChar32 start, UChar32 end)
    result = test3.retain(0x30, 0x39)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 10 + 1  # [0-9{ab}]
    assert test1.contains(0x30, 0x39)
    assert test1.contains(icu.UnicodeString("ab"))


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_retain_69() -> None:
    test1 = icu.UnicodeSet("[0-9\u00df{ab}]")
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::retain(const UnicodeString &s)
    result = test2.retain(icu.UnicodeString("\xdf"))
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 1 + 1  # [\u00DF{ab}]
    assert test2.contains(0xDF)
    assert test2.contains("ab")

    result = test3.retain("ab")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 1  # [{ab}]
    assert test3.contains("ab")


def test_retain_all() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    test2 = test1.clone()
    test3 = test1.clone()
    assert test1.size() == 12

    # [1]
    # UnicodeSet &icu::UnicodeSet::retainAll(const UnicodeSet &c)
    result = test2.retain_all(icu.UnicodeSet(0xDF, 0xDF))
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test2)
    assert test2.size() == 1  # [\u00DF]
    assert test2.contains(0xDF)

    # [2]
    # UnicodeSet &icu::UnicodeSet::retainAll(const UnicodeString &s)
    result = test3.retain_all(icu.UnicodeString("ab"))
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 0

    test3 = test1.clone()
    result = test3.retain_all("ab")
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(test3)
    assert test3.size() == 0


def test_serialize() -> None:
    # int32_t icu::UnicodeSet::serialize(
    #       uint16_t *dest,
    #       int32_t destCapacity,
    #       UErrorCode &ec
    # )
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))
    dest = test1.serialize()
    assert isinstance(dest, list)
    assert dest == [
        4,  # length=((n+2*m)|(m!=0?0x8000:0)) (n=4, m=0)
        ord("0"),
        ord("9") + 1,  # bmp code points
        0xDF,
        0xDF + 1,
    ]

    test2 = icu.UnicodeSet(icu.UnicodeString("[\U0001f000-\U0001f003]"))
    dest = test2.serialize()
    assert isinstance(dest, list)
    assert dest == [
        (0 + 2 * 2) | 0x8000,  # length=((n+2*m)|(m!=0?0x8000:0)) (n=0, m=2)
        0,  # bmpLength
        1,
        0xF000,  # supplementary code points
        (0x1F003 + 1) >> 16,
        (0x1F003 + 1) & 0xFFFF,
    ]


def test_span() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))

    # [1]
    # int32_t icu::UnicodeSet::span(
    #       const char16_t *s,
    #       int32_t length,
    #       USetSpanCondition spanCondition
    # )
    assert test1.span("abcde", -1, icu.USetSpanCondition.USET_SPAN_CONTAINED) == 2

    # [2]
    # int32_t icu::UnicodeSet::span(
    #       const UnicodeString &s,
    #       int32_t start,
    #       USetSpanCondition spanCondition
    # )
    assert (
        test1.span(
            icu.UnicodeString("abcde"),
            0,
            icu.USetSpanCondition.USET_SPAN_CONTAINED,
        )
        == 2
    )

    # int32_t icu::UnicodeSet::spanUTF8(
    #       const char *s,
    #       int32_t length,
    #       USetSpanCondition spanCondition
    # ) const
    b = b"abcde"
    assert test1.span_utf8(b, -1, icu.USetSpanCondition.USET_SPAN_CONTAINED) == 2
    assert test1.span_utf8(b, len(b), icu.USetSpanCondition.USET_SPAN_CONTAINED) == 2
    assert test1.span_utf8(b, 2, icu.USetSpanCondition.USET_SPAN_CONTAINED) == 2
    assert test1.span_utf8(b, 1, icu.USetSpanCondition.USET_SPAN_CONTAINED) == 0


def test_span_back() -> None:
    test1 = icu.UnicodeSet(icu.UnicodeString("[0-9\u00df{ab}]"))

    # [1]
    # int32_t icu::UnicodeSet::spanBack(
    #       const char16_t *s,
    #       int32_t length,
    #       USetSpanCondition spanCondition
    # )
    assert test1.span_back("aaaab", -1, icu.USetSpanCondition.USET_SPAN_CONTAINED) == 3

    # [2]
    # int32_t icu::UnicodeSet::spanBack(
    #       const UnicodeString &s,
    #       int32_t limit,
    #       USetSpanCondition spanCondition
    # )
    assert (
        test1.span_back(
            icu.UnicodeString("aaaab"),
            icu.INT32_MAX,
            icu.USetSpanCondition.USET_SPAN_CONTAINED,
        )
        == 3
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_strings() -> None:
    # from icu/source/test/intltest/usettest.cpp
    #  UnicodeSetTest::TestStringIterator()
    us = icu.UnicodeSet("[abcçカ🚴{}{abc}{de}]")

    # U_HEADER_NESTED_NAMESPACE::USetStrings icu::UnicodeSet::strings() const
    it = us.strings()
    result = icu.UnicodeString()
    [result.append(' "').append(s).append('"') for s in it]
    assert result == ' "" "abc" "de"'


def test_unicode_set() -> None:
    # [1]
    # icu::UnicodeSet::UnicodeSet()
    test1 = icu.UnicodeSet()
    assert test1.size() == 0

    # [2]
    # icu::UnicodeSet::UnicodeSet(UChar32 start, UChar32 end)
    test2 = icu.UnicodeSet(0x30, 0x39)
    assert test2.size() == 10
    assert test2.contains(0x30, 0x39)

    # [4]
    # icu::UnicodeSet::UnicodeSet(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    test4 = icu.UnicodeSet(icu.UnicodeString("[0-9]"))
    assert test4.size() == 10
    assert test4.contains(0x30, 0x39)

    test4a = icu.UnicodeSet("[0-9]")
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
    pattern = icu.UnicodeString("foo [ 0-9 ]")
    pos = icu.ParsePosition()
    pos.set_index(4)
    options = icu.USET_IGNORE_SPACE
    test6 = icu.UnicodeSet(pattern, pos, options, None)
    assert pos.get_index() == pattern.length()
    assert test6.size() == 10
    assert test6.contains(0x30, 0x39)

    pos = icu.ParsePosition()
    pos.set_index(4)
    test6a = icu.UnicodeSet("foo [ 0-9 ]", pos, options, None)
    assert pos.get_index() == 11
    assert test6a.size() == 10
    assert test6a.contains(0x30, 0x39)

    # [7]
    # icu::UnicodeSet::UnicodeSet(const UnicodeSet &o)
    test7 = icu.UnicodeSet(test2)
    assert test7.size() == 10
    assert test7.contains(0x30, 0x39)

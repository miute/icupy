import copy

import pytest

from icupy import icu
from icupy.utils import gc


def test_append() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::append(char16_t srcChar)
    test1 = icu.UnicodeString()
    result = test1.append(0x61).append(0x62).append(0x63)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "abc"

    # [2]
    # UnicodeString &icu::UnicodeString::append(
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString()
    result = test1.append("abc", 0, 1).append("abc", 1, 1).append("abc", 2, 1)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "abc"

    # [3]
    # UnicodeString &icu::UnicodeString::append(const UnicodeString &srcText)
    test1 = icu.UnicodeString("foo ", -1, icu.US_INV)
    result = test1.append(test1).append(test1).append(test1)
    # "foo " + "foo "
    # "foo foo " + "foo foo "
    # "foo foo foo foo " + "foo foo foo foo "
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo foo foo foo foo foo foo foo "

    test1 = icu.UnicodeString()
    result = test1.append("a").append("\U0001f338").append("a\U0001f338b")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "a\U0001f338a\U0001f338b"

    # [4]
    # UnicodeString &icu::UnicodeString::append(
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString()
    test2 = icu.UnicodeString("foo bar baz", -1, icu.US_INV)
    result = test1.append(test2, 0, 4).append(test2, 4, 4).append(test2, 8, 3)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"

    # [6]
    # UnicodeString &icu::UnicodeString::append(UChar32 srcChar)
    test1 = icu.UnicodeString()
    result = test1.append(0x1F338)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    expected = icu.UnicodeString("\\uD83C\\uDF38", -1, icu.US_INV).unescape()
    assert test1 == expected
    assert len(test1) == 2

    result = test1.append(0x0074)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    expected = icu.UnicodeString("\\uD83C\\uDF38t", -1, icu.US_INV).unescape()
    assert test1 == expected
    assert len(test1) == 3

    # [5]
    # UnicodeString &icu::UnicodeString::append(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString()
    result = test1.append("foo bar", 4).append("bar baz", 4).append("baz qux", 3)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"


def test_case_compare() -> None:
    s = "\x61\u0308\x62"
    test1 = icu.UnicodeString("a\\u0308b").unescape()
    test2 = icu.UnicodeString(s)

    # [1]
    # int8_t icu::UnicodeString::caseCompare(
    #       const UnicodeString &text,
    #       uint32_t options
    # )
    assert test1.case_compare(test2, icu.UnicodeString.FOLD_CASE_DEFAULT) == 0
    assert test1.case_compare(s, icu.UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [2]
    # int8_t icu::UnicodeString::caseCompare(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength,
    #       uint32_t options
    # )
    assert test1.case_compare(s, -1, icu.UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [3]
    # int8_t icu::UnicodeString::caseCompare(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength,
    #       uint32_t options
    # )
    assert test1.case_compare(0, len(s), s, 0, -1, icu.UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [4]
    # int8_t icu::UnicodeString::caseCompare(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars,
    #       uint32_t options
    # )
    assert test1.case_compare(0, len(s), s, icu.UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [5]
    # int8_t UnicodeString::caseCompare(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength,
    #       uint32_t options
    # )
    assert (
        test1.case_compare(
            0,
            len(test1),
            test2,
            0,
            len(test2),
            icu.UnicodeString.FOLD_CASE_DEFAULT,
        )
        == 0
    )

    # [6]
    # int8_t icu::UnicodeString::caseCompare(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText,
    #       uint32_t options
    # )
    assert test1.case_compare(0, len(test2), test2, icu.UnicodeString.FOLD_CASE_DEFAULT) == 0

    # int8_t icu::UnicodeString::caseCompareBetween(
    #       int32_t start,
    #       int32_t limit,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLimit,
    #       uint32_t options
    # )
    assert (
        test1.case_compare_between(
            0,
            len(test2),
            test2,
            0,
            len(test1),
            icu.UnicodeString.FOLD_CASE_DEFAULT,
        )
        == 0
    )
    assert (
        test1.case_compare_between(
            0,
            len(test2),
            str(test2),
            0,
            len(test1),
            icu.UnicodeString.FOLD_CASE_DEFAULT,
        )
        == 0
    )


def test_char_at() -> None:
    test1 = icu.UnicodeString("a\\uD83C\\uDF38b").unescape()
    assert test1.length() == 4
    assert len(test1) == 4

    # int32_t icu::UnicodeString::countChar32(
    #       int32_t start = 0,
    #       int32_t length = INT32_MAX
    # )
    assert test1.count_char32() == 3
    assert test1.count_char32(2) == 2
    assert test1.count_char32(2, 1) == 1

    # char16_t icu::UnicodeString::charAt(int32_t offset)
    assert test1.char_at(0) == 0x61
    assert test1.char_at(1) == 0xD83C
    assert test1.char_at(2) == 0xDF38
    assert test1.char_at(3) == 0x62

    # UChar32 icu::UnicodeString::char32At(int32_t offset)
    assert test1.char32_at(0) == 0x61
    assert test1.char32_at(1) == 0x1F338
    assert test1.char32_at(2) == 0x1F338
    assert test1.char32_at(3) == 0x62

    # int32_t icu::UnicodeString::getChar32Start(int32_t offset)
    assert test1.get_char32_start(0) == 0
    assert test1.get_char32_start(1) == 1
    assert test1.get_char32_start(2) == 1
    assert test1.get_char32_start(3) == 3

    # int32_t icu::UnicodeString::getChar32Limit(int32_t offset)
    assert test1.get_char32_limit(0) == 0
    assert test1.get_char32_limit(1) == 1
    assert test1.get_char32_limit(2) == 3
    assert test1.get_char32_limit(3) == 3

    # UBool icu::UnicodeString::hasMoreChar32Than(
    #       int32_t start,
    #       int32_t length,
    #       int32_t number
    # )
    assert test1.has_more_char32_than(0, icu.INT32_MAX, -1) is True
    assert test1.has_more_char32_than(0, icu.INT32_MAX, 0) is True
    assert test1.has_more_char32_than(0, icu.INT32_MAX, 1) is True
    assert test1.has_more_char32_than(0, icu.INT32_MAX, 2) is True
    assert test1.has_more_char32_than(0, icu.INT32_MAX, 3) is False

    # UnicodeString &icu::UnicodeString::setCharAt(
    #       int32_t offset,
    #       char16_t ch
    # )
    result = (
        test1.set_char_at(0, ord("A"))
        .set_char_at(3, ord("B"))
        .set_char_at(1, 0xD83C)
        .set_char_at(2, 0xDF63)
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "A\U0001f363B"


def test_clone() -> None:
    test1 = icu.UnicodeString(0x1F338)

    # UnicodeString *icu::UnicodeString::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.UnicodeString)
    assert id(test1) != id(test2)
    assert test2 == test1

    test3 = copy.copy(test1)
    assert test3 == test1

    test4 = copy.deepcopy(test1)
    assert test4 == test1


def test_compare() -> None:
    test1 = icu.UnicodeString("foo bar")
    test2 = icu.UnicodeString("foo bar")
    test3 = icu.UnicodeString("foo bar baz")
    s2 = "foo bar"
    s3 = "foo bar baz"

    # [1]
    # int8_t icu::UnicodeString::compare(const UnicodeString &text)
    assert test1.compare(test2) == 0
    assert test1.compare(test3) < 0
    assert test1.compare("foo bar") == 0
    assert test1.compare("foo bar baz") < 0

    # [2]
    # int8_t icu::UnicodeString::compare(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    assert test1.compare(s2, len(s2)) == 0
    assert test1.compare(s3, len(s3)) < 0

    # [3]
    # int8_t icu::UnicodeString::compare(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars
    # )
    assert test1.compare(0, len(test1), s2) == 0
    assert test1.compare(0, len(test1), s3) == 0

    # [4]
    # int8_t icu::UnicodeString::compare(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.compare(0, len(test1), s2, 0, -1) == 0
    assert test1.compare(0, len(test1), s3, 0, -1) < 0

    # [5]
    # int8_t icu::UnicodeString::compare(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.compare(0, len(test1), test2, 0, len(test2)) == 0
    assert test1.compare(0, len(test1), test3, 0, len(test3)) < 0

    # [6]
    # int8_t icu::UnicodeString::compare(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &text
    # )
    assert test1.compare(0, len(test1), test2) == 0
    assert test1.compare(0, len(test1), test3) < 0

    # int8_t icu::UnicodeString::compareBetween(
    #       int32_t start,
    #       int32_t limit,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLimit
    # )
    assert test1.compare_between(0, len(test1), test2, 0, len(test2)) == 0
    assert test1.compare_between(0, len(test1), test3, 0, len(test3)) < 0
    assert test1.compare_between(0, len(test1), str(test2), 0, len(test2)) == 0
    assert test1.compare_between(0, len(test1), str(test3), 0, len(test3)) < 0


def test_compare_code_point_order() -> None:
    s = "\x61\u0308\x62"
    test1 = icu.UnicodeString("a\\u0308b").unescape()
    test2 = icu.UnicodeString(s)

    # [1]
    # int8_t icu::UnicodeString::compareCodePointOrder(
    #       const UnicodeString &text
    # )
    assert test1.compare_code_point_order(test2) == 0
    assert test1.compare_code_point_order(s) == 0

    # [2]
    # int8_t icu::UnicodeString::compareCodePointOrder(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    assert test1.compare_code_point_order(s, -1) == 0

    # [3]
    # int8_t icu::UnicodeString::compareCodePointOrder(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars
    # )
    assert test1.compare_code_point_order(0, len(test1), s) == 0

    # [4]
    # int8_t icu::UnicodeString::compareCodePointOrder(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.compare_code_point_order(0, len(test1), s, 0, -1) == 0

    # [5]
    # int8_t icu::UnicodeString::compareCodePointOrder(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText
    # )
    assert test1.compare_code_point_order(0, len(test1), test2) == 0

    # [6]
    # int8_t icu::UnicodeString::compareCodePointOrder(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.compare_code_point_order(0, len(test2), test2, 0, len(test1)) == 0

    # int8_t icu::UnicodeString::compareCodePointOrderBetween(
    #       int32_t start,
    #       int32_t limit,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLimit
    # )
    assert test1.compare_code_point_order_between(0, len(test2), test2, 0, len(test1)) == 0
    assert test1.compare_code_point_order_between(0, len(test2), str(test2), 0, len(test1)) == 0


def test_constants() -> None:
    assert icu.UnicodeString.SENTINEL == icu.U_SENTINEL
    assert icu.UnicodeString.COMPARE_CODE_POINT_ORDER == icu.U_COMPARE_CODE_POINT_ORDER
    assert icu.UnicodeString.FOLD_CASE_DEFAULT == icu.U_FOLD_CASE_DEFAULT
    assert icu.UnicodeString.FOLD_CASE_EXCLUDE_SPECIAL_I == icu.U_FOLD_CASE_EXCLUDE_SPECIAL_I
    assert (
        icu.UnicodeString.TITLECASE_NO_BREAK_ADJUSTMENT == icu.U_TITLECASE_NO_BREAK_ADJUSTMENT
    )
    assert icu.UnicodeString.TITLECASE_NO_LOWERCASE == icu.U_TITLECASE_NO_LOWERCASE


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_constants_60() -> None:
    assert icu.UnicodeString.TITLECASE_ADJUST_TO_CASED == icu.U_TITLECASE_ADJUST_TO_CASED
    assert icu.UnicodeString.TITLECASE_SENTENCES == icu.U_TITLECASE_SENTENCES
    assert icu.UnicodeString.TITLECASE_WHOLE_STRING == icu.U_TITLECASE_WHOLE_STRING


def test_copy() -> None:
    # void icu::UnicodeString::copy(
    #       int32_t start,
    #       int32_t limit,
    #       int32_t dest
    # )
    test1 = icu.UnicodeString("abc")
    test1.copy(1, 2, 3)
    assert test1 == "abcb"


def test_ends_with() -> None:
    test1 = icu.UnicodeString("foo bar")
    test2 = icu.UnicodeString("foo")
    test3 = icu.UnicodeString("bar")
    s2 = "foo"
    s3 = "bar"

    # [1]
    # UBool icu::UnicodeString::endsWith(
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.ends_with(s2, 0, -1) is False
    assert test1.ends_with(s3, 0, -1) is True

    # [2]
    # UBool icu::UnicodeString::endsWith(
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.ends_with(test2, 0, len(test2)) is False
    assert test1.ends_with(test3, 0, len(test3)) is True

    # [3]
    # UBool icu::UnicodeString::endsWith(const UnicodeString &text)
    assert test1.ends_with(test2) is False
    assert test1.ends_with(test3) is True
    assert test1.ends_with("foo") is False
    assert test1.ends_with("bar") is True

    # [4]
    # UBool icu::UnicodeString::endsWith(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    assert test1.ends_with(s2, -1) is False
    assert test1.ends_with(s3, -1) is True


def test_extract() -> None:
    # [1]
    # int32_t icu::UnicodeString::extract(
    #       char *dest,
    #       int32_t destCapacity,
    #       UConverter *cnv,
    #       UErrorCode &errorCode
    # )
    with gc(icu.ucnv_open("ibm-943c"), icu.ucnv_close) as cnv:
        utf8 = "a[\x5c\uff71\u2160"
        sjis = b"\x61\x5b\x5c\xb1\x87\x54"  # utf8.encode("cp932")
        test1 = icu.UnicodeString(utf8, "utf-8")
        dest = test1.extract(cnv)
        assert isinstance(dest, bytes)
        assert dest == sjis

    src = "a\\uD83C\\uDF38b\\x00c"
    expected = "a\U0001f338b\x00c"
    test2 = icu.UnicodeString(src, -1, icu.US_INV).unescape()
    test3 = icu.UnicodeString()
    test3.set_to_bogus()

    # [2]
    # int32_t icu::UnicodeString::extract(
    #       Char16Ptr dest,
    #       int32_t destCapacity,
    #       UErrorCode &errorCode
    # )
    dest = test2.extract()
    assert isinstance(dest, str)
    assert dest == expected

    with pytest.raises(icu.ICUError) as exc_info:
        _ = test3.extract()
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # [3]
    # void icu::UnicodeString::extract(
    #       int32_t start,
    #       int32_t length,
    #       Char16Ptr dst,
    #       int32_t dstStart = 0
    # )
    # Same as [7]

    # [4]
    # void icu::UnicodeString::extract(
    #       int32_t start,
    #       int32_t length,
    #       UnicodeString &target
    # )
    target = icu.UnicodeString("foo")
    test2.extract(3, 3, target)
    assert len(target) == 3
    assert target == icu.UnicodeString("b\x00c", 3)

    test2.extract(3, 2, target)
    assert len(target) == 2
    assert target == icu.UnicodeString("b\x00", 2)

    test3.extract(3, 3, target)
    assert len(target) == 0

    # [5]
    # int32_t icu::UnicodeString::extract(
    #       int32_t start,
    #       int32_t startLength,
    #       char *target,
    #       const char *codepage = 0
    # )
    #
    # [8]
    # int32_t icu::UnicodeString::extract(
    #       int32_t start,
    #       int32_t startLength,
    #       char *target,
    #       uint32_t targetLength,
    #       const char *codepage
    # )
    target = test2.extract(3, 3, "utf-32-be")
    assert isinstance(target, bytes)
    assert target == b"\x00\x00\x00\x62\x00\x00\x00\x00\x00\x00\x00\x63"
    assert target.decode("utf-32-be") == "b\x00c"

    target = test2.extract(3, 2, "utf-32-be")
    assert isinstance(target, bytes)
    assert target == b"\x00\x00\x00\x62\x00\x00\x00\x00"
    assert target.decode("utf-32-be") == "b\x00"

    target = test3.extract(3, 3, "utf-32-be")
    assert isinstance(target, bytes)
    assert len(target) == 0

    # [6]
    # int32_t icu::UnicodeString::extract(
    #       int32_t start,
    #       int32_t startLength,
    #       char *target,
    #       int32_t targetCapacity,
    #       enum EInvariant inv
    # )
    target = test2.extract(3, 3, icu.US_INV)
    assert isinstance(target, bytes)
    assert target == b"b\x00c"

    target = test2.extract(3, 2, icu.US_INV)
    assert isinstance(target, bytes)
    assert target == b"b\x00"

    target = test3.extract(3, 3, icu.US_INV)
    assert isinstance(target, bytes)
    assert len(target) == 0

    # [7]
    # int32_t icu::UnicodeString::extract(
    #       int32_t start,
    #       int32_t startLength,
    #       char *target,
    #       uint32_t targetLength
    # )
    target = test2.extract(3, 3)
    assert isinstance(target, bytes)
    assert target == b"b\x00c"

    target = test2.extract(3, 2)
    assert isinstance(target, bytes)
    assert target == b"b\x00"

    target = test3.extract(3, 3)
    assert len(target) == 0


def test_extract_between() -> None:
    src = "a\\uD83C\\uDF38b\\x00c"
    test1 = icu.UnicodeString(src, -1, icu.US_INV).unescape()
    test2 = test1.clone()
    test2.set_to_bogus()

    # [1]
    # void icu::UnicodeString::extractBetween(
    #       int32_t start,
    #       int32_t limit,
    #       char16_t *dst,
    #       int32_t dstStart = 0
    # )
    dest = test1.extract_between(0, 1)
    assert isinstance(dest, str)
    assert dest == "a"

    dest = test1.extract_between(0, 3)
    assert isinstance(dest, str)
    assert dest == "a\U0001f338"  # "a\uD83C\uDF38" → "a\U0001F338"

    dest = test1.extract_between(0, 4)
    assert isinstance(dest, str)
    assert dest == "a\U0001f338b"  # "a\uD83C\uDF38b" → "a\U0001F338b"

    dest = test1.extract_between(0, 5)
    assert isinstance(dest, str)
    assert dest == "a\U0001f338b\x00"  # "a\uD83C\uDF38b\x00" → "a\U0001F338b\x00"

    dest = test1.extract_between(0, 6)
    assert isinstance(dest, str)
    assert dest == "a\U0001f338b\x00c"  # "a\uD83C\uDF38b\x00c" → "a\U0001F338b\x00c"

    dest = test1.extract_between(0, 1000)
    assert isinstance(dest, str)
    assert dest == "a\U0001f338b\x00c"  # "a\uD83C\uDF38b\x00c" → "a\U0001F338b\x00c"

    dest = test1.extract_between(3, 3)
    assert isinstance(dest, str)
    assert len(dest) == 0

    dest = test1.extract_between(3, 4)
    assert isinstance(dest, str)
    assert dest == "b"

    dest = test1.extract_between(3, 5)
    assert isinstance(dest, str)
    assert dest == "b\x00"

    dest = test1.extract_between(3, 6)
    assert isinstance(dest, str)
    assert dest == "b\x00c"

    dest = test1.extract_between(4, 6)
    assert isinstance(dest, str)
    assert dest == "\x00c"

    dest = test1.extract_between(5, 6)
    assert isinstance(dest, str)
    assert dest == "c"

    dest = test1.extract_between(6, 7)
    assert isinstance(dest, str)
    assert len(dest) == 0

    dest = test2.extract_between(3, 6)
    assert isinstance(dest, str)
    assert len(dest) == 0  # test2 is not valid

    dest = test1.extract_between(1, 0)
    assert isinstance(dest, str)
    assert len(dest) == 0

    dest = test1.extract_between(-2, -1)
    assert isinstance(dest, str)
    assert len(dest) == 0

    dest = test1.extract_between(-2, 1)
    assert isinstance(dest, str)
    assert dest == "a"

    # [2]
    # void icu::UnicodeString::extractBetween(
    #       int32_t start,
    #       int32_t limit,
    #       UnicodeString &target
    # )
    target = icu.UnicodeString("foo")
    test1.extract_between(3, 6, target)
    assert len(target) == 3
    assert target == icu.UnicodeString("b\x00c", 3)

    test1.extract_between(3, 5, target)
    assert len(target) == 2
    assert target == icu.UnicodeString("b\x00", 2)

    test2.extract_between(3, 6, target)
    assert len(target) == 0


def test_fast_copy_from() -> None:
    test1 = icu.UnicodeString(0x1F338)
    test2 = icu.UnicodeString("foo bar baz")

    # UnicodeString &icu::UnicodeString::fastCopyFrom(const UnicodeString &src)
    test3 = test1.fast_copy_from(test2)
    assert test1 == test2 == test3
    assert id(test1) == id(test3)


def test_find_and_replace() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::findAndReplace(
    #       const UnicodeString &oldText,
    #       const UnicodeString &newText
    # )
    test1 = icu.UnicodeString("abc")
    result = (
        test1.find_and_replace(icu.UnicodeString("b"), icu.UnicodeString("B"))
        .find_and_replace(icu.UnicodeString("c"), icu.UnicodeString("C"))
        .find_and_replace(icu.UnicodeString("a"), icu.UnicodeString("A"))
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "ABC"

    test1 = icu.UnicodeString("abc")
    result = (
        test1.find_and_replace("b", icu.UnicodeString("B"))
        .find_and_replace(icu.UnicodeString("c"), "C")
        .find_and_replace("a", "A")
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "ABC"

    # [2]
    # UnicodeString &icu::UnicodeString::findAndReplace(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &oldText,
    #       const UnicodeString &newText
    # )
    test2 = icu.UnicodeString("foo bar baz")
    result = (
        test2.find_and_replace(0, 3, icu.UnicodeString("bar"), icu.UnicodeString("BAR"))
        .find_and_replace(8, 11, icu.UnicodeString("baz"), icu.UnicodeString("BAZ"))
        .find_and_replace(0, 3, icu.UnicodeString("foo"), icu.UnicodeString("FOO"))
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test2)
    assert test2 == "FOO bar BAZ"

    test2 = icu.UnicodeString("foo bar baz")
    result = (
        test2.find_and_replace(0, 3, "bar", icu.UnicodeString("BAR"))
        .find_and_replace(8, 11, icu.UnicodeString("baz"), "BAZ")
        .find_and_replace(0, 3, "foo", "FOO")
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test2)
    assert test2 == "FOO bar BAZ"

    # [3]
    # UnicodeString &icu::UnicodeString::findAndReplace(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &oldText,
    #       int32_t oldStart,
    #       int32_t oldLength,
    #       const UnicodeString &newText,
    #       int32_t newStart,
    #       int32_t newLength
    # )
    test3 = icu.UnicodeString("foo bar baz")
    result = (
        test3.find_and_replace(
            0,
            3,
            icu.UnicodeString("bar"),
            0,
            1,
            icu.UnicodeString("BAR"),
            0,
            1,
        )
        .find_and_replace(
            8,
            11,
            icu.UnicodeString("baz"),
            0,
            1,
            icu.UnicodeString("BAZ"),
            0,
            1,
        )
        .find_and_replace(
            0,
            3,
            icu.UnicodeString("foo"),
            0,
            1,
            icu.UnicodeString("FOO"),
            0,
            1,
        )
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test3)
    assert test3 == "Foo bar Baz"

    test3 = icu.UnicodeString("foo bar baz")
    result = (
        test3.find_and_replace(0, 3, "bar", 0, 1, icu.UnicodeString("BAR"), 0, 1)
        .find_and_replace(8, 11, icu.UnicodeString("baz"), 0, 1, "BAZ", 0, 1)
        .find_and_replace(0, 3, "foo", 0, 1, "FOO", 0, 1)
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test3)
    assert test3 == "Foo bar Baz"


def test_fold_case() -> None:
    # U+0041: Latin Capital Letter A
    # U+0398: Greek Capital Letter Theta
    # U+0414: Cyrillic Capital Letter De
    # U+00DF: Latin Small Letter Sharp S
    # U+1F9B: Greek Capital Letter Eta with Dasia and Varia and Prosgegrammeni
    test1 = icu.UnicodeString("\x41\u0398\u0414\xdf\u1f9b")

    # UnicodeString &icu::UnicodeString::foldCase(uint32_t options = 0)
    result = test1.fold_case()
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    # U+0061: Latin Small Letter A
    # U+03B8: Greek Small Letter Theta
    # U+0434: Cyrillic Small Letter De
    # U+1F23: Greek Small Letter Eta with Dasia and Varia
    # U+03B9: Greek Small Letter Iota
    assert test1 == "a\u03b8\u0434ss\u1f23\u03b9"

    # U+0049: Latin Capital Letter I
    test2 = icu.UnicodeString("\x49")
    result = test2.fold_case(icu.UnicodeString.FOLD_CASE_DEFAULT)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test2)
    # U+0069: Latin Small Letter I
    assert test2 == "\x69"

    # U+0049: Latin Capital Letter I
    test3 = icu.UnicodeString("\x49")
    result = test3.fold_case(icu.UnicodeString.FOLD_CASE_EXCLUDE_SPECIAL_I)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test3)
    # U+0131: Latin Small Letter Dotless I
    assert test3 == "\u0131"


def test_from_utf32() -> None:
    s1 = "a\U0001f338b"

    # static UnicodeString icu::UnicodeString::fromUTF32(
    #       const UChar32 *utf32,
    #       int32_t length
    # )
    test1 = icu.UnicodeString.from_utf32(s1, len(s1))
    assert isinstance(test1, icu.UnicodeString)
    assert test1 == s1

    # int32_t icu::UnicodeString::toUTF32(
    #       UChar32 *utf32,
    #       int32_t capacity,
    #       UErrorCode &errorCode
    # )
    s2 = test1.to_utf32()
    assert isinstance(s2, str)
    assert s2 == s1


def test_from_utf8() -> None:
    b1 = b"a\xf0\x9f\x8c\xb8b"
    expected = "a\U0001f338b"

    # static UnicodeString icu::UnicodeString::fromUTF8(StringPiece utf8)
    test1 = icu.UnicodeString.from_utf8(b1)
    assert isinstance(test1, icu.UnicodeString)
    assert test1 == expected

    # void icu::UnicodeString::toUTF8(ByteSink &sink)
    b2 = test1.to_utf8()
    assert isinstance(b2, bytes)
    assert b2 == b1

    # template<typename StringClass>
    # StringClass &icu::UnicodeString::toUTF8String(StringClass &result)
    s3 = test1.to_utf8_string()
    assert isinstance(s3, str)
    assert s3 == expected
    assert str(test1) == s3

    # UnicodeString.__repr__() -> str
    assert repr(test1) == "UnicodeString('a\\U0001f338b', text_length=4)"

    test2 = icu.UnicodeString.from_utf8(b"'Julius\tC\xd3\x95sar'")
    assert repr(test2) == "UnicodeString('\\'Julius\\x09C\\u04d5sar\\'', text_length=14)"


def test_get_buffer() -> None:
    test1 = icu.UnicodeString("abc", -1, icu.US_INV)
    test1.append(icu.UnicodeString("\\uD83C \\uDF38").unescape())

    # [1]
    # const char16_t *icu::UnicodeString::getBuffer()
    p = test1.get_buffer()

    # _ConstChar16Ptr.__getitem__(index: int) -> int
    assert p[0] == 0x61
    assert p[1] == 0x62
    assert p[2] == 0x63
    assert p[3] == 0xD83C
    assert p[4] == 0x20
    assert p[5] == 0xDF38

    _ = p[test1.get_capacity() - 1]
    with pytest.raises(IndexError):
        _ = p[test1.get_capacity()]

    assert p[-1] == p[test1.get_capacity() - 1]
    assert p[-test1.get_capacity()] == p[0]
    with pytest.raises(IndexError):
        _ = p[-test1.get_capacity() - 1]

    assert list(p)[: test1.length()] == [
        0x61,
        0x62,
        0x63,
        0xD83C,
        0x20,
        0xDF38,
    ]

    # _ConstChar16Ptr.__len__()
    assert len(p) == test1.get_capacity()

    # const char16_t *icu::UnicodeString::getTerminatedBuffer()
    p = test1.get_terminated_buffer()

    # _ConstChar16Ptr.__getitem__(index: int) -> int
    assert p[0] == 0x61
    assert p[1] == 0x62
    assert p[2] == 0x63
    assert p[3] == 0xD83C
    assert p[4] == 0x20
    assert p[5] == 0xDF38
    assert p[6] == 0

    _ = p[test1.get_capacity() - 1]
    with pytest.raises(IndexError):
        _ = p[test1.get_capacity()]

    assert p[-1] == p[test1.get_capacity() - 1]
    assert p[-test1.get_capacity()] == p[0]
    with pytest.raises(IndexError):
        _ = p[-test1.get_capacity() - 1]

    assert list(p)[: test1.length() + 1] == [
        0x61,
        0x62,
        0x63,
        0xD83C,
        0x20,
        0xDF38,
        0,
    ]

    # _ConstChar16Ptr.__len__()
    assert len(p) == test1.get_capacity()

    test1.set_to_bogus()
    assert test1.get_buffer() is None
    assert test1.get_terminated_buffer() is None


def test_handle_replace_between() -> None:
    # virtual void icu::UnicodeString::handleReplaceBetween(
    #       int32_t start,
    #       int32_t limit,
    #       const UnicodeString &text
    # )
    test1 = icu.UnicodeString("foo bar baz")
    test1.handle_replace_between(4, 7, icu.UnicodeString("BAR"))
    test1.handle_replace_between(8, 11, icu.UnicodeString("BAZ"))
    test1.handle_replace_between(0, 3, icu.UnicodeString("FOO"))
    assert test1 == "FOO BAR BAZ"

    test1 = icu.UnicodeString("foo bar baz")
    test1.handle_replace_between(4, 7, "BAR")
    test1.handle_replace_between(8, 11, "BAZ")
    test1.handle_replace_between(0, 3, "FOO")
    assert test1 == "FOO BAR BAZ"


def test_hash_code() -> None:
    test1 = icu.UnicodeString("abc")
    test2 = icu.UnicodeString("abc")

    # int32_t icu::UnicodeString::hashCode(void)
    assert test1.hash_code() == test2.hash_code()

    assert hash(test1) == test1.hash_code()
    assert hash(test2) == test2.hash_code()

    test1.append("d")
    assert test1.hash_code() != test2.hash_code()

    assert hash(test1) == test1.hash_code()
    assert hash(test2) == test2.hash_code()

    test2.append("d")
    assert test1.hash_code() == test2.hash_code()

    assert hash(test1) == test1.hash_code()
    assert hash(test2) == test2.hash_code()


def test_index_of() -> None:
    raw_text = (
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna "
        "aliqua."
    )
    test1 = icu.UnicodeString(raw_text, -1, icu.US_INV)
    test2 = icu.UnicodeString("ipsum", -1, icu.US_INV)
    test3 = icu.UnicodeString(
        "\\ud841\\udc02\\u0071\\udc02\\ud841\\u0071\\ud841\\udc02"
        "\\u0071\\u0072\\ud841\\udc02\\u0071\\ud841\\udc02\\u0071"
        "\\udc02\\ud841\\u0073\\u0000"
    ).unescape()

    # [1]
    # int32_t icu::UnicodeString::indexOf(char16_t c)
    assert test1.index_of(ord("o")) == 1

    # [2]
    # int32_t icu::UnicodeString::indexOf(
    #       char16_t c,
    #       int32_t start
    # )
    assert test1.index_of(ord("o"), 10) == 13

    # [3]
    # int32_t icu::UnicodeString::indexOf(
    #       char16_t c,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.index_of(ord("o"), 10, 3) == -1
    assert test1.index_of(ord("o"), 10, 4) == 13

    # [4]
    # int32_t icu::UnicodeString::indexOf(
    #       const char16_t *srcChars,
    #       int32_t srcLength,
    #       int32_t start
    # )
    assert test1.index_of("ipsum", 2, 10) == 42

    # [5]
    # int32_t icu::UnicodeString::indexOf(
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.index_of("ipsum", 0, 2, 30, 13) == -1
    assert test1.index_of("ipsum", 0, 2, 30, 14) == 42

    # [6]
    # int32_t icu::UnicodeString::indexOf(
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.index_of(test2, 0, 2, 30, 13) == -1
    assert test1.index_of(test2, 0, 2, 30, 14) == 42

    # [7]
    # int32_t icu::UnicodeString::indexOf(const UnicodeString &text)
    assert test1.index_of(test2) == 6
    assert test1.index_of("ipsum") == 6

    # [8]
    # int32_t icu::UnicodeString::indexOf(
    #       const UnicodeString &text,
    #       int32_t start
    # )
    assert test1.index_of(test2, 6) == 6
    assert test1.index_of(test2, 7) == -1
    assert test1.index_of("ipsum", 6) == 6
    assert test1.index_of("ipsum", 7) == -1

    # [9]
    # int32_t icu::UnicodeString::indexOf(
    #       const UnicodeString &text,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.index_of(test2, 0, 11) == 6
    assert test1.index_of(test2, 0, 10) == -1

    # [10]
    # int32_t icu::UnicodeString::indexOf(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.index_of("ipsum", 2, 30, 14) == 42

    # [11]
    # int32_t icu::UnicodeString::indexOf(UChar32 c)
    assert test3.index_of(0xD841) == 4
    assert test3.index_of(0xDC02) == 3

    # [12]
    # int32_t icu::UnicodeString::indexOf(
    #       UChar32 c,
    #       int32_t start
    # )
    assert test3.index_of(0xD841, 10) == 17

    # [13]
    # int32_t icu::UnicodeString::indexOf(
    #       UChar32 c,
    #       int32_t start,
    #       int32_t length
    # )
    if icu.U_ICU_VERSION_MAJOR_NUM >= 68:
        assert test3.index_of(0xD841, 10, 4) == 13
    else:  # U_ICU_VERSION_MAJOR_NUM <= 67
        assert test3.index_of(0xD841, 10, 4) == -1
    assert test3.index_of(0xD841, 10, 8) == 17

    # __contains__()
    assert "ipsum" in test1
    assert test2 in test1


def test_insert() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::insert(
    #       int32_t start,
    #       char16_t srcChar
    # )
    test1 = icu.UnicodeString()
    result = test1.insert(0, 0x63).insert(0, 0x61).insert(1, 0x62)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "abc"

    # [2]
    # UnicodeString &icu::UnicodeString::insert(
    #       int32_t start,
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString()
    result = test1.insert(0, "abc", 2, 1).insert(0, "abc", 0, 1).insert(1, "abc", 1, 1)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "abc"

    # [3]
    # UnicodeString &icu::UnicodeString::insert(
    #       int32_t start,
    #       const UnicodeString &srcText
    # )
    test1 = icu.UnicodeString()
    result = (
        test1.insert(0, icu.UnicodeString("baz"))
        .insert(0, icu.UnicodeString("foo"))
        .insert(3, icu.UnicodeString(" bar "))
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"

    test1 = icu.UnicodeString()
    result = test1.insert(0, "baz").insert(0, "foo").insert(3, " bar ")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"

    # [4]
    # UnicodeString &icu::UnicodeString::insert(
    #       int32_t start,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString()
    test2 = icu.UnicodeString("foo bar baz", -1, icu.US_INV)
    result = test1.insert(0, test2, 8, 3).insert(0, test2, 0, 4).insert(4, test2, 4, 4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"

    # [6]
    # UnicodeString &icu::UnicodeString::insert(
    #       int32_t start,
    #       UChar32 srcChar
    # )
    test1 = icu.UnicodeString()
    result = test1.insert(0, 0x20402).insert(0, 0x20100).insert(2, 0x0074)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    expected = icu.UnicodeString("\\uD840\\uDD00t\\uD841\\uDC02", -1, icu.US_INV).unescape()
    assert test1 == expected
    assert len(test1) == 5

    # [5]
    # UnicodeString &icu::UnicodeString::insert(
    #       int32_t start,
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString()
    result = test1.insert(0, "baz qux", 3).insert(0, "foo bar", 4).insert(4, "bar baz", 4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"


def test_last_index_of() -> None:
    text = (
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
        "sed do eiusmod tempor incididunt ut labore et dolore magna "
        "aliqua."
    )
    test1 = icu.UnicodeString(text, -1, icu.US_INV)
    test2 = icu.UnicodeString("ipsum", -1, icu.US_INV)
    test3 = icu.UnicodeString(
        "\\ud841\\udc02\\u0071\\udc02\\ud841\\u0071\\ud841\\udc02"
        "\\u0071\\u0072\\ud841\\udc02\\u0071\\ud841\\udc02\\u0071"
        "\\udc02\\ud841\\u0073\\u0000"
    ).unescape()

    # [1]
    # int32_t icu::UnicodeString::lastIndexOf(char16_t c)
    assert test1.last_index_of(ord("o")) == 106

    # [2]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       char16_t c,
    #       int32_t start
    # )
    assert test1.last_index_of(ord("o"), 10) == 106

    # [3]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       char16_t c,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.last_index_of(ord("o"), 10, 3) == -1
    assert test1.last_index_of(ord("o"), 10, 4) == 13

    # [4]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       const char16_t *srcChars,
    #       int32_t srcLength,
    #       int32_t start
    # )
    assert test1.last_index_of("ipsum", 2, 10) == 42

    # [5]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.last_index_of("ipsum", 0, 2, 30, 13) == -1
    assert test1.last_index_of("ipsum", 0, 2, 30, 14) == 42

    # [6]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.last_index_of(test2, 0, 2, 30, 13) == -1
    assert test1.last_index_of(test2, 0, 2, 30, 14) == 42

    # [7]
    # int32_t icu::UnicodeString::lastIndexOf(const UnicodeString &text)
    assert test1.last_index_of(test2) == 6
    assert test1.last_index_of("ipsum") == 6

    # [8]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       const UnicodeString &text,
    #       int32_t start
    # )
    assert test1.last_index_of(test2, 6) == 6
    assert test1.last_index_of(test2, 7) == -1
    assert test1.last_index_of("ipsum", 6) == 6
    assert test1.last_index_of("ipsum", 7) == -1

    # [9]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       const UnicodeString &text,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.last_index_of(test2, 0, 11) == 6
    assert test1.last_index_of(test2, 0, 10) == -1

    # [10]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength,
    #       int32_t start,
    #       int32_t length
    # )
    assert test1.last_index_of("ipsum", 2, 30, 14) == 42

    # [11]
    # int32_t icu::UnicodeString::lastIndexOf(UChar32 c)
    assert test3.last_index_of(0xD841) == 17
    assert test3.last_index_of(0xDC02) == 16

    # [12]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       UChar32 c,
    #       int32_t start
    # )
    assert test3.last_index_of(0xD841, 10) == 17

    # [13]
    # int32_t icu::UnicodeString::lastIndexOf(
    #       UChar32 c,
    #       int32_t start,
    #       int32_t length
    # )
    if icu.U_ICU_VERSION_MAJOR_NUM >= 68:
        assert test3.last_index_of(0xD841, 10, 4) == 13
    else:  # U_ICU_VERSION_MAJOR_NUM <= 67
        assert test3.last_index_of(0xD841, 10, 4) == -1
    assert test3.last_index_of(0xD841, 10, 8) == 17


def test_move_index32() -> None:
    test1 = icu.UnicodeString("a\U00010000b\U0010ffff\u2029")

    # int32_t icu::UnicodeString::moveIndex32(
    #       int32_t index,
    #       int32_t delta
    # )
    assert test1.move_index32(1, 2) == 4
    assert test1.move_index32(0, 3) == 4
    assert test1.move_index32(test1.length(), -2) == 4


def test_operator() -> None:
    test1 = icu.UnicodeString("foo bar baz", -1, icu.US_INV)
    test2 = icu.UnicodeString("foo bar baz")
    test3 = icu.UnicodeString("foo bar")

    # UBool icu::UnicodeString::operator!=(const UnicodeString &text)
    # UnicodeString.__ne__(other: UnicodeString|str) -> bool
    assert not (test1 != test2)
    assert test1 != test3

    assert not (test1 != "foo bar baz")
    assert test1 != "foo bar"

    # UBool icu::UnicodeString::operator<(const UnicodeString &text)
    # UnicodeString.__lt__(other: UnicodeString|str) -> bool
    assert not (test1 < test2)
    assert not (test1 < test3)

    assert not (test1 < "foo bar baz")
    assert not (test1 < "foo bar")

    # UBool icu::UnicodeString::operator<=(const UnicodeString &text)
    # UnicodeString.__le__(other: UnicodeString|str) -> bool
    assert test1 <= test2
    assert not (test1 <= test3)

    assert test1 <= "foo bar baz"
    assert not (test1 <= "foo bar")

    # UBool icu::UnicodeString::operator==(const UnicodeString &text)
    # UnicodeString.__eq__(other: UnicodeString|str) -> bool
    assert test1 == test2
    assert not (test1 == test3)

    assert test1 == "foo bar baz"
    assert not (test1 == "foo bar")

    # UBool icu::UnicodeString::operator>(const UnicodeString &text)
    # UnicodeString.__gt__(other: UnicodeString|str) -> bool
    assert not (test1 > test2)
    assert test1 > test3

    assert not (test1 > "foo bar baz")
    assert test1 > "foo bar"

    # UBool icu::UnicodeString::operator>=(const UnicodeString &text)
    # UnicodeString.__ge__(other: UnicodeString|str) -> bool
    assert test1 >= test2
    assert test1 >= test3

    assert test1 >= "foo bar baz"
    assert test1 >= "foo bar"

    # char16_t icu::UnicodeString::operator[](int32_t offset)
    # UnicodeString.__getitem__(index: int) -> str
    assert test1[0] == "f"
    assert test1[1] == "o"
    assert test1[2] == "o"
    assert test1[3] == " "
    assert test1[4] == "b"
    assert test1[5] == "a"
    assert test1[6] == "r"
    assert test1[7] == " "
    assert test1[8] == "b"
    assert test1[9] == "a"
    assert test1[10] == "z"

    assert test1[-3] == "b"  # != U+ffff
    assert test1[-2] == "a"  # != U+ffff
    assert test1[-1] == "z"  # != U+ffff

    with pytest.raises(IndexError):  # IndexError: string index out of range
        _ = test1[test1.length()]  # != U+ffff
    _ = test1[test1.length() - 1]

    # UnicodeString.__getitem__(index: slice) -> UnicodeString
    assert isinstance(test1[:], icu.UnicodeString)
    assert test1[4:7] == "bar"
    assert test1[0:9:4] == "fbb"
    with pytest.raises(ValueError):  # ValueError: slice step cannot be zero
        _ = test1[::0]

    # UnicodeString &icu::UnicodeString::operator+=(
    #       const UnicodeString &srcText
    # )
    # UnicodeString.__iadd__(other: UnicodeString)
    object_id = id(test3)
    test3 += icu.UnicodeString(" baz")
    assert isinstance(test3, icu.UnicodeString)
    assert test3 == test1
    assert id(test3) == object_id

    # UnicodeString.__add__(other: UnicodeString)
    test4 = icu.UnicodeString("foo bar") + icu.UnicodeString(" baz")
    assert isinstance(test4, icu.UnicodeString)
    assert test4 == test1

    # UnicodeString.__iadd__(other: str)
    test5 = icu.UnicodeString("foo bar")
    object_id = id(test5)
    test5 += " baz"
    assert isinstance(test5, icu.UnicodeString)
    assert test5 == test1
    assert id(test5) == object_id

    # UnicodeString.__add__(other: str)
    test6 = icu.UnicodeString("foo bar") + " baz"
    assert isinstance(test6, icu.UnicodeString)
    assert test6 == test1

    # UnicodeString &icu::UnicodeString::operator+=(UChar32 ch)
    # UnicodeString &icu::UnicodeString::operator+=(char16_t ch)
    # UnicodeString.__iadd__(other: int)
    test7 = icu.UnicodeString("a")
    object_id = id(test7)
    test7 += 0x20402
    test7 += 0x62
    assert isinstance(test7, icu.UnicodeString)
    expected = icu.UnicodeString("a\\uD841\\uDC02b", -1, icu.US_INV).unescape()
    assert test7 == expected
    assert id(test7) == object_id


def test_operator_setitem() -> None:
    test1 = icu.UnicodeString("abcd")

    # UnicodeString.__setitem__(index: int, value: str|int)
    test1[0] = "a"
    test1[1] = 0xD83C
    test1[2] = 0xDF08
    test1[3] = "b"
    assert str(test1) == "a\U0001f308b"

    with pytest.raises(IndexError):
        test1[4] = "c"  # index out of range

    test1[-1] = "c"
    assert str(test1) == "a\U0001f308c"

    test1[-4] = "d"
    assert str(test1) == "d\U0001f308c"

    with pytest.raises(IndexError):
        test1[-5] = "d"  # index out of range

    with pytest.raises(ValueError):
        test1[0] = "12"  # multi-character string

    test1[0] = 0  # [0, 0xffff]
    assert test1[0] == "\0"

    with pytest.raises(TypeError):
        test1[0] = -1  # [0, 0xffff]

    test1[0] = 0xFFFF  # [0, 0xffff]
    assert test1[0] == "\uffff"

    with pytest.raises(TypeError):
        test1[0] = 0x10000  # [0, 0xffff]


def test_pad_leading() -> None:
    # UBool icu::UnicodeString::padLeading(
    #       int32_t targetLength,
    #       char16_t padChar = 0x0020
    # )
    test1 = icu.UnicodeString("foo")
    assert test1.pad_leading(10) is True
    assert test1 == "       foo"

    test1 = icu.UnicodeString("foo")
    assert test1.pad_leading(10, ord(".")) is True
    assert test1 == ".......foo"


def test_pad_trailing() -> None:
    # UBool icu::UnicodeString::padTrailing(
    #       int32_t targetLength,
    #       char16_t padChar = 0x0020
    # )
    test1 = icu.UnicodeString("foo")
    assert test1.pad_trailing(10) is True
    assert test1 == "foo       "

    test1 = icu.UnicodeString("foo")
    assert test1.pad_trailing(10, ord(".")) is True
    assert test1 == "foo......."


def test_remove() -> None:
    test1 = icu.UnicodeString("foo bar baz")

    # [2]
    # UnicodeString &icu::UnicodeString::remove(
    #       int32_t start,
    #       int32_t length = (int32_t)INT32_MAX
    # )
    result = test1.remove(3, 4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo baz"

    result = test1.remove(3)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo"

    # [1]
    # UnicodeString &icu::UnicodeString::remove()
    result = test1.remove()
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1.length() == 0
    assert len(test1) == 0

    # UnicodeString &icu::UnicodeString::removeBetween(
    #       int32_t start,
    #       int32_t limit = (int32_t)INT32_MAX
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = test1.remove_between(3, 7)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo baz"

    result = test1.remove_between(3)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo"


def test_replace() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::replace(
    #       int32_t start,
    #       int32_t length,
    #       char16_t srcChar
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = test1.replace(4, 1, ord("B")).replace(8, 1, ord("B")).replace(0, 1, ord("F"))
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "Foo Bar Baz"

    # [2]
    # UnicodeString &icu::UnicodeString::replace(
    #       int32_t start,
    #       int32_t length,
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace(4, 3, "BAR", 0, -1)
        .replace(8, 3, "BAZ", 0, -1)
        .replace(0, 3, "FOO", 0, -1)
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    # [3]
    # UnicodeString &icu::UnicodeString::replace(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace(4, 3, icu.UnicodeString("BAR"))
        .replace(8, 3, icu.UnicodeString("BAZ"))
        .replace(0, 3, icu.UnicodeString("FOO"))
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    test1 = icu.UnicodeString("foo bar baz")
    result = test1.replace(4, 3, "BAR").replace(8, 3, "BAZ").replace(0, 3, "FOO")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    # [4]
    # UnicodeString &icu::UnicodeString::replace(
    #       int32_t start,
    #       int32_t length,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace(4, 3, icu.UnicodeString("BAR"), 0, 3)
        .replace(8, 3, icu.UnicodeString("BAZ"), 0, 3)
        .replace(0, 3, icu.UnicodeString("FOO"), 0, 3)
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    # [5]
    # UnicodeString &icu::UnicodeString::replace(
    #       int32_t start,
    #       int32_t length,
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = test1.replace(4, 3, "BAR", -1).replace(8, 3, "BAZ", -1).replace(0, 3, "FOO", -1)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    # [6]
    # UnicodeString &icu::UnicodeString::replace(
    #       int32_t start,
    #       int32_t length,
    #       UChar32 srcChar
    # )
    test1 = icu.UnicodeString("a\\uD83C\\uDF38b", -1, icu.US_INV).unescape()
    result = test1.replace(1, 2, 0x1F30F).replace(0, 1, 0x1F30D).replace(4, 1, 0x1F30E)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "\U0001f30d\U0001f30f\U0001f30e"


def test_replace_between() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::replaceBetween(
    #       int32_t start,
    #       int32_t limit,
    #       const UnicodeString &srcText
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace_between(4, 7, icu.UnicodeString("BAR"))
        .replace_between(8, 11, icu.UnicodeString("BAZ"))
        .replace_between(0, 3, icu.UnicodeString("FOO"))
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace_between(4, 7, "BAR")
        .replace_between(8, 11, "BAZ")
        .replace_between(0, 3, "FOO")
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    # [2]
    # UnicodeString &icu::UnicodeString::replaceBetween(
    #       int32_t start,
    #       int32_t limit,
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLimit
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace_between(4, 7, icu.UnicodeString("BAR"), 0, 3)
        .replace_between(8, 11, icu.UnicodeString("BAZ"), 0, 3)
        .replace_between(0, 3, icu.UnicodeString("FOO"), 0, 3)
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"

    test1 = icu.UnicodeString("foo bar baz")
    result = (
        test1.replace_between(4, 7, "BAR", 0, 3)
        .replace_between(8, 11, "BAZ", 0, 3)
        .replace_between(0, 3, "FOO", 0, 3)
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "FOO BAR BAZ"


def test_retain_between() -> None:
    # UnicodeString &icu::UnicodeString::retainBetween(
    #       int32_t start,
    #       int32_t limit = INT32_MAX
    # )
    test1 = icu.UnicodeString("foo bar baz")
    result = test1.retain_between(4, 7)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "bar"

    test1 = icu.UnicodeString("foo bar baz")
    result = test1.retain_between(4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "bar baz"


def test_reverse() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::reverse(
    #       int32_t start,
    #       int32_t length
    # )
    test1 = icu.UnicodeString("0123-456-789")
    result = test1.reverse(5, 3)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "0123-654-789"

    # [2]
    # UnicodeString &icu::UnicodeString::reverse(void)
    test1 = icu.UnicodeString("0123456789")
    result = test1.reverse()
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "9876543210"


def test_set_to() -> None:
    # [2]
    # UnicodeString &icu::UnicodeString::setTo(char16_t srcChar)
    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    result = test1.set_to(0x61)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "a"

    # [3]
    # UnicodeString &icu::UnicodeString::setTo(
    #       const char16_t *srcChars,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    result = test1.set_to("foo bar baz", 4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo "

    # [4]
    # UnicodeString &icu::UnicodeString::setTo(const UnicodeString &srcText)
    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    test2 = icu.UnicodeString("foo bar baz")
    result = test1.set_to(test2)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"

    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    result = test1.set_to("foo bar baz")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "foo bar baz"

    # [5]
    # UnicodeString &icu::UnicodeString::setTo(
    #       const UnicodeString &srcText,
    #       int32_t srcStart
    # )
    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    test2 = icu.UnicodeString("foo bar baz")
    result = test1.set_to(test2, 4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "bar baz"

    # [6]
    # UnicodeString &icu::UnicodeString::setTo(
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    test2 = icu.UnicodeString("foo bar baz")
    result = test1.set_to(test2, 4, 4)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "bar "

    # [8]
    # UnicodeString &icu::UnicodeString::setTo(UChar32 srcChar)
    test1 = icu.UnicodeString("a\U0001f338b", -1, icu.US_INV)
    result = test1.set_to(0x1F338)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == "\U0001f338"


def test_starts_with() -> None:
    test1 = icu.UnicodeString("foo bar")
    test2 = icu.UnicodeString("foo")
    test3 = icu.UnicodeString("bar")
    s2 = "foo"
    s3 = "bar"

    # [1]
    # UBool icu::UnicodeString::startsWith(
    #       const char16_t *srcChars,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.starts_with(s2, 0, -1) is True
    assert test1.starts_with(s3, 0, -1) is False

    # [2]
    # UBool icu::UnicodeString::startsWith(
    #       const UnicodeString &srcText,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    assert test1.starts_with(test2, 0, len(test2)) is True
    assert test1.starts_with(test3, 0, len(test3)) is False

    # [3]
    # UBool icu::UnicodeString::startsWith(const UnicodeString &text)
    assert test1.starts_with(test2) is True
    assert test1.starts_with(test3) is False
    assert test1.starts_with("foo") is True
    assert test1.starts_with("bar") is False

    # [4]
    # UBool icu::UnicodeString::startsWith(
    #       ConstChar16Ptr srcChars,
    #       int32_t srcLength
    # )
    assert test1.starts_with(s2, -1) is True
    assert test1.starts_with(s3, -1) is False


def test_swap() -> None:
    test1 = icu.UnicodeString("abcd", 4, icu.US_INV)
    test2 = icu.UnicodeString(100, 0x7A, 100)

    # void icu::UnicodeString::swap(UnicodeString &other)
    test1.swap(test2)
    assert test1.length() == 100
    assert test2.length() == 4
    assert test1 == "z" * 100
    assert test2 == "abcd"


def test_temp_sub_string() -> None:
    # UnicodeString icu::UnicodeString::tempSubString(
    #       int32_t start = 0,
    #       int32_t length = INT32_MAX
    # )
    test1 = icu.UnicodeString("abcd", 4, icu.US_INV)
    test2 = test1.temp_sub_string()
    assert isinstance(test2, icu.UnicodeString)
    assert test2 == "abcd"

    test2 = test1.temp_sub_string(1)
    assert isinstance(test2, icu.UnicodeString)
    assert test2 == "bcd"

    test2 = test1.temp_sub_string(1, 2)
    assert isinstance(test2, icu.UnicodeString)
    assert test2 == "bc"
    test1.append(test2)
    assert test1 == "abcdbc"

    # UnicodeString icu::UnicodeString::tempSubStringBetween(
    #       int32_t start,
    #       int32_t limit = INT32_MAX
    # )
    test1 = icu.UnicodeString("abcd", 4, icu.US_INV)
    test2 = test1.temp_sub_string_between(1)
    assert isinstance(test2, icu.UnicodeString)
    assert test2 == "bcd"

    test2 = test1.temp_sub_string_between(1, 3)
    assert isinstance(test2, icu.UnicodeString)
    assert test2 == "bc"
    test1.append(test2)
    assert test1 == "abcdbc"


def test_to_lower() -> None:
    default_locale = None
    try:
        default_locale = icu.Locale.get_default()

        # [1]
        # UnicodeString &icu::UnicodeString::toLower(const Locale &locale)
        loc = icu.Locale("en", "US")
        test1 = icu.UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        result = test1.to_lower(loc)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x69"  # U+0069: Latin Small Letter I

        test1 = icu.UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        result = test1.to_lower("en_US")
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x69"  # U+0069: Latin Small Letter I

        loc = icu.Locale("tr")
        test1 = icu.UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        result = test1.to_lower(loc)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\u0131"  # U+0131: Latin Small Letter Dotless I

        test1 = icu.UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        result = test1.to_lower("tr")
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\u0131"  # U+0131: Latin Small Letter Dotless I

        # [2]
        # UnicodeString &icu::UnicodeString::toLower(void)
        icu.Locale.set_default(icu.Locale("en", "US"))
        test1 = icu.UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        result = test1.to_lower()
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x69"  # U+0069: Latin Small Letter I
    finally:
        if default_locale:
            icu.Locale.set_default(default_locale)


def test_to_title() -> None:
    # [1]
    # UnicodeString &icu::UnicodeString::toTitle(BreakIterator *titleIter)
    test1 = icu.UnicodeString("sTrA\\u00dfE", -1, icu.US_INV).unescape()
    result = test1.to_title(None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString("Stra\\u00dfe", -1, icu.US_INV).unescape()

    # [2]
    # UnicodeString &icu::UnicodeString::toTitle(
    #       BreakIterator *titleIter,
    #       const Locale &locale
    # )
    src = "«ijs»"
    test1 = icu.UnicodeString(src)
    result = test1.to_title(None, icu.Locale("nl-BE"))
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString("«IJs»")

    test1 = icu.UnicodeString(src)
    result = test1.to_title(None, "nl-BE")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString("«IJs»")

    test1 = icu.UnicodeString(src)
    result = test1.to_title(None, icu.Locale("tr-DE"))
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString("«İjs»")

    test1 = icu.UnicodeString(src)
    result = test1.to_title(None, "tr-DE")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString("«İjs»")

    # [3]
    # UnicodeString &icu::UnicodeString::toTitle(
    #       BreakIterator *titleIter,
    #       const Locale &locale,
    #       uint32_t options
    # )
    src = " john. Smith"
    test1 = icu.UnicodeString(src)
    result = test1.to_title(
        None,
        icu.Locale(""),
        icu.UnicodeString.TITLECASE_WHOLE_STRING | icu.UnicodeString.TITLECASE_NO_LOWERCASE,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString(" John. Smith")

    test1 = icu.UnicodeString(src)
    result = test1.to_title(
        None,
        "",
        icu.UnicodeString.TITLECASE_WHOLE_STRING | icu.UnicodeString.TITLECASE_NO_LOWERCASE,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString(" John. Smith")

    test1 = icu.UnicodeString(src)
    result = test1.to_title(
        None,
        icu.Locale(""),
        icu.UnicodeString.TITLECASE_WHOLE_STRING
        | icu.UnicodeString.TITLECASE_NO_BREAK_ADJUSTMENT,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString(" john. smith")

    test1 = icu.UnicodeString(src)
    result = test1.to_title(
        None,
        "",
        icu.UnicodeString.TITLECASE_WHOLE_STRING
        | icu.UnicodeString.TITLECASE_NO_BREAK_ADJUSTMENT,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert test1 == icu.UnicodeString(" john. smith")


def test_to_upper() -> None:
    default_locale = None
    try:
        default_locale = icu.Locale.get_default()

        # [1]
        # UnicodeString &icu::UnicodeString::toUpper(const Locale &locale)
        loc = icu.Locale("en", "US")
        test1 = icu.UnicodeString("\x69")  # U+0069: Latin Small Letter I
        result = test1.to_upper(loc)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I

        test1 = icu.UnicodeString("\x69")  # U+0069: Latin Small Letter I
        result = test1.to_upper("en_US")
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I

        test1 = icu.UnicodeString("\u0131")  # U+0131: Latin Small Letter Dotless I
        result = test1.to_upper(loc)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I

        test1 = icu.UnicodeString("\u0131")  # U+0131: Latin Small Letter Dotless I
        result = test1.to_upper("en_US")
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I

        loc = icu.Locale("tr")
        test1 = icu.UnicodeString("\x69")  # U+0069: Latin Small Letter I
        result = test1.to_upper(loc)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\u0130"  # U+0130: Latin Capital Letter I with Dot Above

        test1 = icu.UnicodeString("\x69")  # U+0069: Latin Small Letter I
        result = test1.to_upper("tr")
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\u0130"  # U+0130: Latin Capital Letter I with Dot Above

        test1 = icu.UnicodeString("\u0131")  # U+0131: Latin Small Letter Dotless I
        result = test1.to_upper(loc)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I

        test1 = icu.UnicodeString("\u0131")  # U+0131: Latin Small Letter Dotless I
        result = test1.to_upper("tr")
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I

        # [2]
        # UnicodeString &icu::UnicodeString::toUpper(void)
        icu.Locale.set_default(icu.Locale("en", "US"))
        test1 = icu.UnicodeString("\x69")  # U+0069: Latin Small Letter I
        result = test1.to_upper()
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(test1)
        assert test1 == "\x49"  # U+0049: Latin Capital Letter I
    finally:
        if default_locale:
            icu.Locale.set_default(default_locale)


def test_trim() -> None:
    test1 = icu.UnicodeString("  \\U0001f338  ", -1, icu.US_INV).unescape()
    assert len(test1) == 6

    # UnicodeString &icu::UnicodeString::trim(void)
    result = test1.trim().insert(0, "a").append("b")
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(test1)
    assert len(test1) == 4
    assert test1 == "a\U0001f338b"


def test_truncate() -> None:
    test1 = icu.UnicodeString("foo bar baz", -1, icu.US_INV).unescape()

    # UBool icu::UnicodeString::truncate(int32_t targetLength)
    assert test1.truncate(15) is False
    assert len(test1) == 11

    assert test1.truncate(7) is True
    assert len(test1) == 7
    assert test1 == "foo bar"


def test_unescape() -> None:
    # From icu/source/test/intltest/ustrtest.cpp:
    # void UnicodeStringTest::TestUnescape(void)
    s = "abc\\u4567 \\n\\r \\U00101234xyz\\x1\\x{5289}\\x1b"
    src = icu.UnicodeString(s, -1, icu.US_INV)
    out = icu.UnicodeString("abc")
    out.append(0x4567)
    out.append(" ")
    out.append(0xA)
    out.append(0xD)
    out.append(" ")
    out.append(0x00101234)
    out.append("xyz", -1)  # out.append("xyz")
    out.append(1).append(0x5289).append(0x1B)

    # UnicodeString icu::UnicodeString::unescape()
    result1 = src.unescape()
    assert isinstance(result1, icu.UnicodeString)
    assert result1 == out

    result2 = icu.u_unescape(s)
    assert isinstance(result2, str)
    assert result2 == out


def test_unicode_string() -> None:
    # [1]
    # icu::UnicodeString::UnicodeString()
    test1 = icu.UnicodeString()
    assert test1.is_bogus() is False
    assert test1.is_empty() is True
    assert len(test1) == 0

    # [2]
    # icu::UnicodeString::UnicodeString(
    #       int32_t capacity,
    #       UChar32 c,
    #       int32_t count
    # )
    test2 = icu.UnicodeString(20, 0x41, 10)
    assert test2.is_bogus() is False
    assert test2.is_empty() is False
    assert test2 == "A" * 10

    # [3]
    # icu::UnicodeString::UnicodeString(char16_t ch)
    test3 = icu.UnicodeString(0x41)
    assert test3.is_bogus() is False
    assert test3.is_empty() is False
    assert test3 == "A"

    # [4]
    # icu::UnicodeString::UnicodeString(UChar32 ch)
    test4 = icu.UnicodeString(0x1F338)
    assert test4.is_bogus() is False
    assert test4.is_empty() is False
    assert test4 == "\U0001f338"

    # [5]
    # icu::UnicodeString::UnicodeString(const char16_t *text)
    test5 = icu.UnicodeString("foo bar baz")
    assert test5.is_bogus() is False
    assert test5.is_empty() is False
    assert test5 == "foo bar baz"

    # [8]
    # icu::UnicodeString::UnicodeString(const std::nullptr_t text)
    test8 = icu.UnicodeString(None)
    assert test8.is_bogus() is False
    assert test8.is_empty() is True
    assert len(test8) == 0

    # [9]
    # icu::UnicodeString::UnicodeString(
    #       const char16_t *text,
    #       int32_t textLength
    # )
    test9 = icu.UnicodeString("foo bar baz", 4)
    assert test9.is_bogus() is False
    assert test9.is_empty() is False
    assert test9 == "foo "

    # [12]
    # icu::UnicodeString::UnicodeString(
    #       const std::nullptr_t text,
    #       int32_t textLength
    # )
    test12 = icu.UnicodeString(None, 10)
    assert test12.is_bogus() is False
    assert test12.is_empty() is True
    assert len(test12) == 0

    # [20]
    # icu::UnicodeString::UnicodeString(
    #       const char *codepageData,
    #       const char *codepage
    # )
    sjis = b"\x61\x5b\x5c\xb1\x87\x54"
    utf8 = sjis.decode("cp932")
    test20 = icu.UnicodeString(sjis, "ibm-943c")
    assert test20.is_bogus() is False
    assert test20.is_empty() is False
    assert test20 == utf8

    # [21]
    # icu::UnicodeString::UnicodeString(
    #       const char *codepageData,
    #       int32_t dataLength,
    #       const char *codepage
    # )
    utf16be = b"\x00\x62\x00\x61\x00\x72"
    test21 = icu.UnicodeString(utf16be, len(utf16be), "utf-16-be")
    assert test21.is_bogus() is False
    assert test21.is_empty() is False
    assert test21 == "bar"

    utf8 = "a\U0001f338b"
    utf32be = utf8.encode("utf-32-be")
    test21a = icu.UnicodeString(utf32be, len(utf32be), "utf-32-be")
    assert test21a.is_bogus() is False
    assert test21a.is_empty() is False
    assert test21a == utf8

    # [22]
    # icu::UnicodeString::UnicodeString(
    #       const char *src,
    #       int32_t srcLength,
    #       UConverter *cnv,
    #       UErrorCode &errorCode
    # )
    with gc(icu.ucnv_open("ibm-943c"), icu.ucnv_close) as cnv:
        utf8 = sjis.decode("cp932")
        test22 = icu.UnicodeString(sjis, len(sjis), cnv)
        assert test22.is_bogus() is False
        assert test22.is_empty() is False
        assert test22 == utf8

    # [23]
    # icu::UnicodeString::UnicodeString(
    #       const char *src,
    #       int32_t textLength,
    #       enum EInvariant inv
    # )
    test23 = icu.UnicodeString("foo bar baz", -1, icu.US_INV)
    assert test23.is_bogus() is False
    assert test23.is_empty() is False
    assert test23 == "foo bar baz"

    # [24]
    # icu::UnicodeString::UnicodeString(const UnicodeString &that)
    test24 = icu.UnicodeString(test5)
    assert test24.is_bogus() is False
    assert test24.is_empty() is False
    assert test24 == "foo bar baz"

    # [26]
    # icu::UnicodeString::UnicodeString(
    #       const UnicodeString &src,
    #       int32_t srcStart
    # )
    test26 = icu.UnicodeString(test5, 4)
    assert test26.is_bogus() is False
    assert test26.is_empty() is False
    assert test26 == "bar baz"

    # [27]
    # icu::UnicodeString::UnicodeString(
    #       const UnicodeString &src,
    #       int32_t srcStart,
    #       int32_t srcLength
    # )
    test27 = icu.UnicodeString(test5, 4, 3)
    assert test27.is_bogus() is False
    assert test27.is_empty() is False
    assert test27 == "bar"


def test_unicode_string_appendable() -> None:
    assert issubclass(icu.UnicodeStringAppendable, icu.Appendable)

    dest = icu.UnicodeString()
    app = icu.UnicodeStringAppendable(dest)

    # UBool icu::UnicodeStringAppendable::appendCodePoint(UChar32 c)
    assert app.append_code_point(0x61) is True
    assert app.append_code_point(0x1F338) is True

    # UBool icu::UnicodeStringAppendable::appendCodeUnit(char16_t c)
    assert app.append_code_unit(0x62) is True

    # UBool icu::UnicodeStringAppendable::appendString(
    #       const char16_t *s,
    #       int32_t length
    # )
    assert app.append_string("\U0001f338c", -1) is True
    assert app.append_string("defghijklm", 3) is True

    assert dest == "a\U0001f338b\U0001f338cdef"


def test_unicode_string_vector() -> None:
    # [1]
    # UnicodeStringVector.__init__()
    t1 = icu.UnicodeStringVector()
    assert len(t1) == 0

    # [2]
    # UnicodeStringVector.__init__(n: int)
    t2 = icu.UnicodeStringVector(3)
    assert len(t2) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t2)
    assert len(t2[0]) == 0
    assert len(t2[1]) == 0
    assert len(t2[2]) == 0

    # [3]
    # UnicodeStringVector.__init__(other: UnicodeStringVector)
    t2[0].set_to("foo")
    t2[1].set_to("bar")
    t2[2].set_to("baz")
    t3 = icu.UnicodeStringVector(t2)
    assert len(t3) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t3)
    assert t3[0] == "foo"
    assert t3[1] == "bar"
    assert t3[2] == "baz"

    # [4]
    # UnicodeStringVector.__init__(iterable: list[UnicodeString])
    t4 = icu.UnicodeStringVector(
        [
            icu.UnicodeString("foo"),
            icu.UnicodeString("bar"),
            icu.UnicodeString("baz"),
        ]
    )
    assert len(t4) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert t4[0] == "foo"
    assert t4[1] == "bar"
    assert t4[2] == "baz"

    # [5]
    # UnicodeStringVector.__init__(iterable: list[str])
    t5 = icu.UnicodeStringVector(["foo", "bar", "baz"])
    assert len(t5) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t5)
    assert t5[0] == "foo"
    assert t5[1] == "bar"
    assert t5[2] == "baz"


def test_unicode_string_vector_api() -> None:
    t1 = icu.UnicodeStringVector(["foo"])

    # UnicodeStringVector.append(item: UnicodeString|str)
    t1.append(icu.UnicodeString("bar"))
    t1.append("baz")
    assert len(t1) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t1)
    assert list(map(str, t1)) == ["foo", "bar", "baz"]

    # UnicodeStringVector.count(item: UnicodeString|str) -> int
    t2 = icu.UnicodeStringVector(t1)
    t2.append("foo")
    assert t2.count(icu.UnicodeString("foo")) == 2
    assert t2.count("foo") == 2
    assert t2.count(icu.UnicodeString("ba")) == 0
    assert t2.count("ba") == 0

    # [1]
    # UnicodeStringVector.extend(iterable: list[UnicodeString])
    t3 = icu.UnicodeStringVector(t1)
    t3.extend([icu.UnicodeString("qux"), icu.UnicodeString("quux")])
    assert len(t3) == 5
    assert all(isinstance(x, icu.UnicodeString) for x in t3)
    assert list(map(str, t3)) == ["foo", "bar", "baz", "qux", "quux"]

    # [2]
    # UnicodeStringVector.extend(iterable: list[str])
    t3 = icu.UnicodeStringVector(t1)
    t3.extend(["qux", "quux"])
    assert len(t3) == 5
    assert all(isinstance(x, icu.UnicodeString) for x in t3)
    assert list(map(str, t3)) == ["foo", "bar", "baz", "qux", "quux"]

    # UnicodeStringVector.insert(index: int, item: UnicodeString|str)
    t4 = icu.UnicodeStringVector(t1)
    t4.insert(0, icu.UnicodeString("0"))
    assert len(t4) == 4
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert list(map(str, t4)) == ["0", "foo", "bar", "baz"]

    t4.insert(-1, "-1")
    assert len(t4) == 5
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert list(map(str, t4)) == ["0", "foo", "bar", "-1", "baz"]

    with pytest.raises(IndexError):
        t4.insert(5, "5")  # list index out of range

    with pytest.raises(IndexError):
        t4.insert(-6, "-6")  # list index out of range

    # UnicodeStringVector.pop(index: int = -1) -> UnicodeString
    t5 = icu.UnicodeStringVector(["foo", "bar", "baz", "qux", "quux"])
    item = t5.pop()
    assert isinstance(item, icu.UnicodeString)
    assert item == "quux"
    assert len(t5) == 4
    assert list(map(str, t5)) == ["foo", "bar", "baz", "qux"]

    item = t5.pop(0)
    assert isinstance(item, icu.UnicodeString)
    assert item == "foo"
    assert len(t5) == 3
    assert list(map(str, t5)) == ["bar", "baz", "qux"]

    item = t5.pop(-1)
    assert isinstance(item, icu.UnicodeString)
    assert item == "qux"
    assert len(t5) == 2
    assert list(map(str, t5)) == ["bar", "baz"]

    with pytest.raises(IndexError):
        _ = t5.pop(2)  # list index out of range

    with pytest.raises(IndexError):
        _ = t5.pop(-3)  # list index out of range

    # UnicodeStringVector.remove(item: UnicodeString|str)
    t6 = icu.UnicodeStringVector(["foo", "bar", "baz", "foo"])
    t6.remove(icu.UnicodeString("foo"))
    assert len(t6) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t6)
    assert list(map(str, t6)) == ["bar", "baz", "foo"]

    t6.remove("baz")
    assert len(t6) == 2
    assert all(isinstance(x, icu.UnicodeString) for x in t6)
    assert list(map(str, t6)) == ["bar", "foo"]

    with pytest.raises(ValueError):
        t6.remove("baz")  # item not in list

    # UnicodeStringVector.reverse()
    t7 = icu.UnicodeStringVector(t1)
    assert t7.reverse() is None
    assert len(t7) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t7)
    assert list(map(str, t7)) == ["baz", "bar", "foo"]

    # UnicodeStringVector.clear()
    assert len(t1) > 0
    t1.clear()
    assert len(t1) == 0


def test_unicode_string_vector_operators() -> None:
    t1 = icu.UnicodeStringVector(["foo", "bar", "baz"])
    t2 = icu.UnicodeStringVector(t1)
    t3 = icu.UnicodeStringVector(["foo", "bar", "baz", "qux"])

    # UnicodeStringVector.__bool__() -> bool
    assert bool(icu.UnicodeStringVector(1))
    assert not bool(icu.UnicodeStringVector())

    # UnicodeStringVector.__contains__(item: UnicodeString|str) -> bool
    assert icu.UnicodeString("bar") in t1
    assert icu.UnicodeString("ba") not in t1
    assert "bar" in t1
    assert "ba" not in t1

    # UnicodeStringVector.__eq__(other: UnicodeStringVector) -> bool
    assert t1 == t2
    assert not (t1 == t3)

    # UnicodeStringVector.__ne__(other: UnicodeStringVector) -> bool
    assert not (t1 != t2)
    assert t1 != t3

    # [1]
    # UnicodeStringVector.__getitem__(index: int) -> UnicodeString
    item = t3[0]
    assert isinstance(item, icu.UnicodeString)
    assert item == "foo"
    item.set_to("FOO")
    assert t3[0] == "FOO"

    with pytest.raises(IndexError):
        _ = t3[4]  # list index out of range

    assert t3[-1] == t3[3]
    assert t3[-2] == t3[2]
    assert t3[-3] == t3[1]
    assert t3[-4] == t3[0]

    with pytest.raises(IndexError):
        _ = t3[-5]  # list index out of range

    # [2]
    # UnicodeStringVector.__getitem__(index: slice) -> UnicodeStringVector
    assert isinstance(t3[:], icu.UnicodeStringVector)
    t4 = t3[1::2]
    assert len(t4) == 2
    assert t4[0] == "bar"
    assert t4[1] == "qux"

    assert len(t3[0:0]) == 0

    with pytest.raises(ValueError):
        _ = t3[::0]  # slice step cannot be zero

    # [1]
    # UnicodeStringVector.__iadd__(item: UnicodeString|str) -> UnicodeStringVector
    object_id4 = id(t4)
    t4 += icu.UnicodeString("a")
    assert isinstance(t4, icu.UnicodeStringVector)
    assert id(t4) == object_id4
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert len(t4) == 3
    assert list(map(str, t4)) == ["bar", "qux", "a"]

    t4 += "ab"
    assert isinstance(t4, icu.UnicodeStringVector)
    assert id(t4) == object_id4
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert len(t4) == 4
    assert list(map(str, t4)) == ["bar", "qux", "a", "ab"]

    # [2]
    # UnicodeStringVector.__iadd__(iterable: list[UnicodeString]) -> UnicodeStringVector
    t4 += [icu.UnicodeString("abc"), icu.UnicodeString("d")]
    assert isinstance(t4, icu.UnicodeStringVector)
    assert id(t4) == object_id4
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert len(t4) == 6
    assert list(map(str, t4)) == ["bar", "qux", "a", "ab", "abc", "d"]

    t4 += icu.UnicodeStringVector(["e", "fg"])
    assert isinstance(t4, icu.UnicodeStringVector)
    assert id(t4) == object_id4
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert len(t4) == 8
    assert list(map(str, t4)) == [
        "bar",
        "qux",
        "a",
        "ab",
        "abc",
        "d",
        "e",
        "fg",
    ]

    # [3]
    # UnicodeStringVector.__iadd__(iterable: list[str]) -> UnicodeStringVector
    t4 += ["h", "ijk"]
    assert isinstance(t4, icu.UnicodeStringVector)
    assert id(t4) == object_id4
    assert all(isinstance(x, icu.UnicodeString) for x in t4)
    assert len(t4) == 10
    assert list(map(str, t4)) == [
        "bar",
        "qux",
        "a",
        "ab",
        "abc",
        "d",
        "e",
        "fg",
        "h",
        "ijk",
    ]

    # UnicodeStringVector.__iter__() -> Iterator[UnicodeString]
    assert [str(x) for x in iter(t1)] == ["foo", "bar", "baz"]

    # UnicodeStringVector.__repr__() -> str
    assert repr(t1) == "UnicodeStringVector(['foo', 'bar', 'baz'])"

    # UnicodeStringVector.__reversed__() -> UnicodeStringVector
    t5 = icu.UnicodeStringVector(t1)
    t5r = reversed(t5)
    assert isinstance(t5r, icu.UnicodeStringVector)
    assert len(t5) == len(t5r) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in t5r)
    assert list(map(str, t5)) == ["foo", "bar", "baz"]
    assert list(map(str, t5r)) == ["baz", "bar", "foo"]

    # [1]
    # UnicodeStringVector.__setitem__(index: int, item: UnicodeString|str)
    t6 = icu.UnicodeStringVector(t3)
    t6[3] = icu.UnicodeString("QUX")
    assert all(isinstance(x, icu.UnicodeString) for x in t6)
    assert list(map(str, t6)) == ["FOO", "bar", "baz", "QUX"]

    t6[-1] = "qux"
    assert list(map(str, t6)) == ["FOO", "bar", "baz", "qux"]

    with pytest.raises(IndexError):
        t6[4] = "quux"  # list index out of range

    with pytest.raises(IndexError):
        t6[-5] = "quux"  # list index out of range

    # [2]
    # UnicodeStringVector.__setitem__(index: slice, iterable: list[UnicodeString])
    t6[::2] = [icu.UnicodeString("Foo"), icu.UnicodeString("Baz")]
    assert list(map(str, t6)) == ["Foo", "bar", "Baz", "qux"]

    with pytest.raises(ValueError):
        t6[::0] = [icu.UnicodeString("foo")]  # slice step cannot be zero

    with pytest.raises(ValueError):
        t6[:] = [
            icu.UnicodeString("foo")
        ]  # attempt to assign sequence of size 1 to extended slice of size 4

    # [3]
    # UnicodeStringVector.__setitem__(index: slice, iterable: list[str])
    t6[1::2] = ["BAR", "QUX"]
    assert list(map(str, t6)) == ["Foo", "BAR", "Baz", "QUX"]

    with pytest.raises(ValueError):
        t6[::0] = ["foo"]  # slice step cannot be zero

    with pytest.raises(ValueError):
        t6[:] = ["foo"]  # attempt to assign sequence of size 1 to extended slice of size 4

    # [1]
    # UnicodeStringVector.__delitem__(index: int)
    t7 = icu.UnicodeStringVector(list("0123456789"))
    del t7[0]
    assert list(map(str, t7)) == [
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
    ]

    del t7[-1]
    assert list(map(str, t7)) == [
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
    ]

    with pytest.raises(IndexError):
        del t7[8]  # list index out of range

    with pytest.raises(IndexError):
        del t7[-9]  # list index out of range

    # [1]
    # UnicodeStringVector.__delitem__(index: slice)
    del t7[1:3]
    assert list(map(str, t7)) == [
        "1",
        "4",
        "5",
        "6",
        "7",
        "8",
    ]

    del t7[1::2]
    assert list(map(str, t7)) == [
        "1",
        "5",
        "7",
    ]

    with pytest.raises(ValueError):
        del t7[::0]  # slice step cannot be zero

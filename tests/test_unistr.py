import copy

import pytest
from icupy import (
    ICUException, INT32_MAX, Locale, U_ICU_VERSION_MAJOR_NUM, UErrorCode,
    UnicodeString, UnicodeStringVector, US_INV, u_unescape, ucnv_close,
    ucnv_open,
)

from . import gc


def test_append():
    # [1]
    # UnicodeString & append(char16_t srcChar)
    test1 = UnicodeString()
    test1.append(0x61).append(0x62).append(0x63)
    assert test1 == "abc"

    # [2]
    # UnicodeString & append(const char16_t *srcChars,
    #                        int32_t srcStart,
    #                        int32_t srcLength)
    test1 = UnicodeString()
    test1.append("abc", 0, 1).append("abc", 1, 1).append("abc", 2, 1)
    assert test1 == "abc"

    # [3]
    # UnicodeString & append(const UnicodeString &srcText)
    test1 = UnicodeString("foo ", -1, US_INV)
    test1.append(test1).append(test1).append(test1)
    # "foo " + "foo "
    # "foo foo " + "foo foo "
    # "foo foo foo foo " + "foo foo foo foo "
    assert test1 == "foo foo foo foo foo foo foo foo "

    # [4]
    # UnicodeString & append(const UnicodeString &srcText,
    #                        int32_t srcStart,
    #                        int32_t srcLength)
    test1 = UnicodeString()
    test2 = UnicodeString("foo bar baz", -1, US_INV)
    test1.append(test2, 0, 4).append(test2, 4, 4).append(test2, 8, 3)
    assert test1 == "foo bar baz"

    # [6]
    # UnicodeString & append(UChar32 srcChar)
    test1 = UnicodeString()
    test1.append(0x1f338)
    expected = UnicodeString("\\uD83C\\uDF38", -1, US_INV).unescape()
    assert test1 == expected
    assert len(test1) == 2

    test1.append(0x0074)
    expected = UnicodeString("\\uD83C\\uDF38t", -1, US_INV).unescape()
    assert test1 == expected
    assert len(test1) == 3

    # [5]
    # UnicodeString & append(ConstChar16Ptr srcChars,
    #                        int32_t srcLength)
    test1 = UnicodeString()
    test1.append("foo bar", 4).append("bar baz", 4).append("baz qux", 3)
    assert test1 == "foo bar baz"


def test_case_compare():
    s = "\x61\u0308\x62"
    test1 = UnicodeString("a\\u0308b").unescape()
    test2 = UnicodeString(s)

    # [1]
    # int8_t caseCompare(const UnicodeString &text,
    #                    uint32_t options)
    assert test1.case_compare(test2, UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [2]
    # int8_t caseCompare(ConstChar16Ptr srcChars,
    #                    int32_t srcLength,
    #                    uint32_t options)
    assert test1.case_compare(s, -1, UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [3]
    # int8_t caseCompare(int32_t start,
    #                    int32_t length,
    #                    const char16_t *srcChars,
    #                    int32_t srcStart,
    #                    int32_t srcLength,
    #                    uint32_t options)
    assert test1.case_compare(0,
                              len(s),
                              s,
                              0,
                              -1,
                              UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [4]
    # int8_t caseCompare(int32_t start,
    #                    int32_t length,
    #                    const char16_t *srcChars,
    #                    uint32_t options)
    assert test1.case_compare(0,
                              len(s),
                              s,
                              UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [5]
    # int8_t caseCompare(int32_t start,
    #                    int32_t length,
    #                    const UnicodeString &srcText,
    #                    int32_t srcStart,
    #                    int32_t srcLength,
    #                    uint32_t options)
    assert test1.case_compare(0,
                              -1,
                              test2,
                              0,
                              -1,
                              UnicodeString.FOLD_CASE_DEFAULT) == 0

    # [6]
    # int8_t caseCompare(int32_t start,
    #                    int32_t length,
    #                    const UnicodeString &srcText,
    #                    uint32_t options)
    assert test1.case_compare(0,
                              len(test2),
                              test2,
                              UnicodeString.FOLD_CASE_DEFAULT) == 0

    # int8_t caseCompareBetween(int32_t start,
    #                           int32_t limit,
    #                           const UnicodeString &srcText,
    #                           int32_t srcStart,
    #                           int32_t srcLimit,
    #                           uint32_t options)
    assert test1.case_compare_between(0,
                                      len(test2),
                                      test2,
                                      0,
                                      len(test1),
                                      UnicodeString.FOLD_CASE_DEFAULT) == 0


def test_char_at():
    test1 = UnicodeString("a\\uD83C\\uDF38b").unescape()
    assert test1.length() == 4
    assert len(test1) == 4
    assert test1.count_char32() == 3

    assert test1.char_at(0) == 0x61
    assert test1.char_at(1) == 0xd83c
    assert test1.char_at(2) == 0xdf38
    assert test1.char_at(3) == 0x62

    assert test1.char32_at(0) == 0x61
    assert test1.char32_at(1) == 0x1f338
    assert test1.char32_at(2) == 0x1f338
    assert test1.char32_at(3) == 0x62

    assert test1.get_char32_start(0) == 0
    assert test1.get_char32_start(1) == 1
    assert test1.get_char32_start(2) == 1
    assert test1.get_char32_start(3) == 3

    assert test1.get_char32_limit(0) == 0
    assert test1.get_char32_limit(1) == 1
    assert test1.get_char32_limit(2) == 3
    assert test1.get_char32_limit(3) == 3

    assert test1.has_more_char32_than(0, INT32_MAX, -1)
    assert test1.has_more_char32_than(0, INT32_MAX, 0)
    assert test1.has_more_char32_than(0, INT32_MAX, 1)
    assert test1.has_more_char32_than(0, INT32_MAX, 2)
    assert not test1.has_more_char32_than(0, INT32_MAX, 3)

    test1.set_char_at(
        0, ord("A")).set_char_at(
        3, ord("B")).set_char_at(
        1, 0xd83c).set_char_at(
        2, 0xdf63)
    assert test1 == "A\U0001f363B"


def test_clone():
    test1 = UnicodeString(0x1f338)
    test2 = test1.clone()
    assert isinstance(test2, UnicodeString)
    assert id(test1) != id(test2)
    assert test2 == test1

    test3 = copy.copy(test1)
    assert test3 == test1

    test4 = copy.deepcopy(test1)
    assert test4 == test1


def test_compare():
    test1 = UnicodeString("foo bar")
    test2 = UnicodeString("foo bar")
    test3 = UnicodeString("foo bar baz")
    s2 = "foo bar"
    s3 = "foo bar baz"

    # [1]
    # int8_t compare(const UnicodeString &text)
    assert test1.compare(test2) == 0
    assert test1.compare(test3) < 0

    # [2]
    # int8_t compare(ConstChar16Ptr srcChars,
    #                int32_t srcLength)
    assert test1.compare(s2, len(s2)) == 0
    assert test1.compare(s3, len(s3)) < 0

    # [3]
    # int8_t compare(int32_t start,
    #                int32_t length,
    #                const char16_t *srcChars)
    assert test1.compare(0, len(test1), s2) == 0
    assert test1.compare(0, len(test1), s3) == 0

    # [4]
    # int8_t compare(int32_t start,
    #                int32_t length,
    #                const char16_t *srcChars,
    #                int32_t srcStart,
    #                int32_t srcLength)
    assert test1.compare(0, len(test1), s2, 0, -1) == 0
    assert test1.compare(0, len(test1), s3, 0, -1) < 0

    # [5]
    # int8_t compare(int32_t start,
    #                int32_t length,
    #                const UnicodeString &srcText,
    #                int32_t srcStart,
    #                int32_t srcLength)
    assert test1.compare(0, len(test1), test2, 0, len(test2)) == 0
    assert test1.compare(0, len(test1), test3, 0, len(test3)) < 0

    # [6]
    # int8_t compare(int32_t start,
    #                int32_t length,
    #                const UnicodeString &text)
    assert test1.compare(0, len(test1), test2) == 0
    assert test1.compare(0, len(test1), test3) < 0

    # int8_t compareBetween(int32_t start,
    #                       int32_t limit,
    #                       const UnicodeString &srcText,
    #                       int32_t srcStart,
    #                       int32_t srcLimit)
    assert test1.compare_between(0, len(test1), test2, 0, len(test2)) == 0
    assert test1.compare_between(0, len(test1), test3, 0, len(test3)) < 0


def test_compare_code_point_order():
    s = "\x61\u0308\x62"
    test1 = UnicodeString("a\\u0308b").unescape()
    test2 = UnicodeString(s)

    # [1]
    # int8_t compareCodePointOrder(const UnicodeString &text)
    assert test1.compare_code_point_order(test2) == 0

    # [2]
    # int8_t compareCodePointOrder(ConstChar16Ptr srcChars,
    #                              int32_t srcLength)
    assert test1.compare_code_point_order(s, -1) == 0

    # [3]
    # int8_t compareCodePointOrder(int32_t start,
    #                              int32_t length,
    #                              const char16_t *srcChars)
    assert test1.compare_code_point_order(0, len(test1), s) == 0

    # [4]
    # int8_t compareCodePointOrder(int32_t start,
    #                              int32_t length,
    #                              const char16_t *srcChars,
    #                              int32_t srcStart,
    #                              int32_t srcLength)
    assert test1.compare_code_point_order(0, len(test1), s, 0, -1) == 0

    # [5]
    # int8_t compareCodePointOrder(int32_t start,
    #                              int32_t length,
    #                              const UnicodeString &srcText)
    assert test1.compare_code_point_order(0, len(test1), test2) == 0

    # [6]
    # int8_t compareCodePointOrder(int32_t start,
    #                              int32_t length,
    #                              const UnicodeString &srcText,
    #                              int32_t srcStart,
    #                              int32_t srcLength)
    assert test1.compare_code_point_order(0,
                                          len(test2),
                                          test2,
                                          0,
                                          len(test1)) == 0

    # int8_t compareCodePointOrderBetween(int32_t start,
    #                                     int32_t limit,
    #                                     const UnicodeString &srcText,
    #                                     int32_t srcStart,
    #                                     int32_t srcLimit)
    assert test1.compare_code_point_order_between(0,
                                                  len(test2),
                                                  test2,
                                                  0,
                                                  len(test1)) == 0


def test_copy():
    test1 = UnicodeString("abc")
    test1.copy(1, 2, 3)
    assert test1 == "abcb"


def test_ends_with():
    test1 = UnicodeString("foo bar")
    test2 = UnicodeString("foo")
    test3 = UnicodeString("bar")
    s2 = "foo"
    s3 = "bar"

    # [1]
    # UBool endsWith(const char16_t *srcChars,
    #                int32_t srcStart,
    #                int32_t srcLength)
    assert not test1.ends_with(s2, 0, -1)
    assert test1.ends_with(s3, 0, -1)

    # [2]
    # UBool endsWith(const UnicodeString &srcText,
    #                int32_t srcStart,
    #                int32_t srcLength)
    assert not test1.ends_with(test2, 0, len(test2))
    assert test1.ends_with(test3, 0, len(test3))

    # [3]
    # UBool endsWith(const UnicodeString &text)
    assert not test1.ends_with(test2)
    assert test1.ends_with(test3)

    # [4]
    # UBool endsWith(ConstChar16Ptr srcChars,
    #                int32_t srcLength)
    assert not test1.ends_with(s2, -1)
    assert test1.ends_with(s3, -1)


def test_extract():
    # [1]
    # int32_t extract(char *dest,
    #                 int32_t destCapacity,
    #                 UConverter *cnv,
    #                 UErrorCode &errorCode
    # )
    with gc(ucnv_open("ibm-943c"), ucnv_close) as cnv:
        utf8 = "a[\x5C\uFF71\u2160"
        sjis = b"\x61\x5B\x5C\xB1\x87\x54"  # utf8.encode("cp932")
        test1 = UnicodeString(utf8, "utf-8")
        dest = test1.extract(cnv)
        assert isinstance(dest, bytes)
        assert dest == sjis

    src = "a\\uD83C\\uDF38b\\x00c"
    expected = "a\U0001f338b\x00c"
    test2 = UnicodeString(src, -1, US_INV).unescape()
    test3 = UnicodeString()
    test3.set_to_bogus()

    # [2]
    # int32_t extract(Char16Ptr dest,
    #                 int32_t destCapacity,
    #                 UErrorCode &errorCode)
    dest = test2.extract()
    assert isinstance(dest, str)
    assert dest == expected

    with pytest.raises(ICUException) as exc_info:
        _ = test3.extract()
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # [3]
    # void extract(int32_t start,
    #              int32_t length,
    #              Char16Ptr dst,
    #              int32_t dstStart = 0)
    pass  # Same as [7]

    # [4]
    # void extract(int32_t start,
    #              int32_t length,
    #              UnicodeString &target)
    target = UnicodeString("foo")
    test2.extract(3, 3, target)
    assert len(target) == 3
    assert target == UnicodeString("b\x00c", 3)

    test2.extract(3, 2, target)
    assert len(target) == 2
    assert target == UnicodeString("b\x00", 2)

    test3.extract(3, 3, target)
    assert len(target) == 0

    # [5]
    # int32_t extract(int32_t start,
    #                 int32_t startLength,
    #                 char *target,
    #                 const char *codepage = 0)
    #
    # [8]
    # int32_t extract(int32_t start,
    #                 int32_t startLength,
    #                 char *target,
    #                 uint32_t targetLength,
    #                 const char *codepage)
    dest = test2.extract(3, 3, "utf-32-be")
    assert isinstance(dest, bytes)
    assert dest == b"\x00\x00\x00\x62\x00\x00\x00\x00\x00\x00\x00\x63"
    assert dest.decode("utf-32-be") == "b\x00c"

    dest = test2.extract(3, 2, "utf-32-be")
    assert isinstance(dest, bytes)
    assert dest == b"\x00\x00\x00\x62\x00\x00\x00\x00"
    assert dest.decode("utf-32-be") == "b\x00"

    dest = test3.extract(3, 3, "utf-32-be")
    assert isinstance(dest, bytes)
    assert len(dest) == 0

    # [6]
    # int32_t extract(int32_t start,
    #                 int32_t startLength,
    #                 char *target,
    #                 int32_t targetCapacity,
    #                 enum EInvariant inv)
    dest = test2.extract(3, 3, US_INV)
    assert isinstance(dest, bytes)
    assert dest == b"b\x00c"

    dest = test2.extract(3, 2, US_INV)
    assert isinstance(dest, bytes)
    assert dest == b"b\x00"

    dest = test3.extract(3, 3, US_INV)
    assert isinstance(dest, bytes)
    assert len(dest) == 0

    # [7]
    # int32_t extract(int32_t start,
    #                 int32_t startLength,
    #                 char *target,
    #                 uint32_t targetLength)
    dest = test2.extract(3, 3)
    assert isinstance(dest, bytes)
    assert dest == b"b\x00c"

    dest = test2.extract(3, 2)
    assert isinstance(dest, bytes)
    assert dest == b"b\x00"

    dest = test3.extract(3, 3)
    assert len(dest) == 0


def test_extract_between():
    src = "a\\uD83C\\uDF38b\\x00c"
    test1 = UnicodeString(src, -1, US_INV).unescape()
    test2 = test1.clone()
    test2.set_to_bogus()

    # [1]
    # void extractBetween(int32_t start,
    #                     int32_t limit,
    #                     char16_t *dst,
    #                     int32_t dstStart = 0)
    dest = test1.extract_between(3, 6)
    assert isinstance(dest, str)
    assert dest == "b\x00c"

    dest = test1.extract_between(3, 5)
    assert isinstance(dest, str)
    assert dest == "b"

    dest = test2.extract_between(3, 6)
    assert len(dest) == 0

    # [2]
    # void extractBetween(int32_t start,
    #                     int32_t limit,
    #                     UnicodeString &target)
    target = UnicodeString("foo")
    test1.extract_between(3, 6, target)
    assert len(target) == 3
    assert target == UnicodeString("b\x00c", 3)

    test1.extract_between(3, 5, target)
    assert len(target) == 2
    assert target == UnicodeString("b\x00", 2)

    test2.extract_between(3, 6, target)
    assert len(target) == 0


def test_fast_copy_from():
    test1 = UnicodeString(0x1f338)
    test2 = UnicodeString("foo bar baz")
    test3 = test1.fast_copy_from(test2)
    assert test1 == test2 == test3
    assert id(test1) == id(test3)


def test_find_and_replace():
    # [1]
    # UnicodeString & findAndReplace(const UnicodeString &oldText,
    #                                const UnicodeString &newText)
    test1 = UnicodeString("abc")
    test1.find_and_replace(
        UnicodeString("b"), UnicodeString("B")).find_and_replace(
        UnicodeString("c"), UnicodeString("C")).find_and_replace(
        UnicodeString("a"), UnicodeString("A")
    )
    assert test1 == "ABC"

    # [2]
    # UnicodeString & findAndReplace(int32_t start,
    #                                int32_t length,
    #                                const UnicodeString &oldText,
    #                                const UnicodeString &newText)
    test2 = UnicodeString("foo bar baz")
    test2.find_and_replace(
        0, 3, UnicodeString("bar"), UnicodeString("BAR")).find_and_replace(
        8, 11, UnicodeString("baz"), UnicodeString("BAZ")).find_and_replace(
        0, 3, UnicodeString("foo"), UnicodeString("FOO")
    )
    assert test2 == "FOO bar BAZ"

    # [3]
    # UnicodeString& findAndReplace(int32_t start,
    #                               int32_t length,
    #                               const UnicodeString &oldText,
    #                               int32_t oldStart,
    #                               int32_t oldLength,
    #                               const UnicodeString &newText,
    #                               int32_t newStart,
    #                               int32_t newLength)
    test3 = UnicodeString("foo bar baz")
    test3.find_and_replace(
        0, 3, UnicodeString("bar"), 0, 1, UnicodeString("BAR"), 0, 1
    ).find_and_replace(
        8, 11, UnicodeString("baz"), 0, 1, UnicodeString("BAZ"), 0, 1
    ).find_and_replace(
        0, 3, UnicodeString("foo"), 0, 1, UnicodeString("FOO"), 0, 1
    )
    assert test3 == "Foo bar Baz"


def test_fold_case():
    # U+0041: Latin Capital Letter A
    # U+0398: Greek Capital Letter Theta
    # U+0414: Cyrillic Capital Letter De
    # U+00DF: Latin Small Letter Sharp S
    # U+1F9B: Greek Capital Letter Eta with Dasia and Varia and Prosgegrammeni
    test1 = UnicodeString("\x41\u0398\u0414\xdf\u1f9b")
    test1.fold_case()
    # U+0061: Latin Small Letter A
    # U+03B8: Greek Small Letter Theta
    # U+0434: Cyrillic Small Letter De
    # U+1F23: Greek Small Letter Eta with Dasia and Varia
    # U+03B9: Greek Small Letter Iota
    assert test1 == "a\u03b8\u0434ss\u1f23\u03b9"

    # U+0049: Latin Capital Letter I
    test2 = UnicodeString("\x49")
    test2.fold_case(UnicodeString.FOLD_CASE_DEFAULT)
    # U+0069: Latin Small Letter I
    assert test2 == "\x69"

    # U+0049: Latin Capital Letter I
    test3 = UnicodeString("\x49")
    test3.fold_case(UnicodeString.FOLD_CASE_EXCLUDE_SPECIAL_I)
    # U+0131: Latin Small Letter Dotless I
    assert test3 == "\u0131"


def test_from_utf32():
    s1 = "a\U0001f338b"
    test1 = UnicodeString.from_utf32(s1, len(s1))
    assert isinstance(test1, UnicodeString)
    assert test1 == s1

    s2 = test1.to_utf32()
    assert isinstance(s2, str)
    assert s2 == s1


def test_from_utf8():
    b1 = b"a\xF0\x9F\x8C\xB8b"
    expected = "a\U0001f338b"
    test1 = UnicodeString.from_utf8(b1)
    assert isinstance(test1, UnicodeString)
    assert test1 == expected

    b2 = test1.to_utf8()
    assert isinstance(b2, bytes)
    assert b2 == b1

    s3 = test1.to_utf8_string()
    assert isinstance(s3, str)
    assert s3 == expected
    assert str(test1) == s3


def test_get_buffer():
    # [1]
    # const char16_t * getBuffer()
    test1 = UnicodeString("abc", -1, US_INV)
    test1.append(UnicodeString("\\uD83C \\uDF38").unescape())

    p = test1.get_buffer()
    assert p[0] == 0x61
    assert p[1] == 0x62
    assert p[2] == 0x63
    assert p[3] == 0xd83c
    assert p[4] == 0x20
    assert p[5] == 0xdf38

    p = test1.get_terminated_buffer()
    assert p[0] == 0x61
    assert p[1] == 0x62
    assert p[2] == 0x63
    assert p[3] == 0xd83c
    assert p[4] == 0x20
    assert p[5] == 0xdf38
    assert p[6] == 0

    test1.set_to_bogus()
    assert test1.get_buffer() is None
    assert test1.get_terminated_buffer() is None


def test_hash_code():
    test1 = UnicodeString("abc")
    test2 = UnicodeString("abc")

    assert test1.hash_code() == test2.hash_code()

    test1.append("d")
    assert test1.hash_code() != test2.hash_code()

    test2.append("d")
    assert test1.hash_code() == test2.hash_code()


def test_index_of():
    raw_text = ("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
                "sed do eiusmod tempor incididunt ut labore et dolore magna "
                "aliqua.")
    test1 = UnicodeString(raw_text, -1, US_INV)
    test2 = UnicodeString("ipsum", -1, US_INV)
    test3 = UnicodeString(
        "\\ud841\\udc02\\u0071\\udc02\\ud841\\u0071\\ud841\\udc02"
        "\\u0071\\u0072\\ud841\\udc02\\u0071\\ud841\\udc02\\u0071"
        "\\udc02\\ud841\\u0073\\u0000").unescape()

    # [1]
    # int32_t indexOf(char16_t c)
    assert test1.index_of(ord('o')) == 1

    # [2]
    # int32_t indexOf(char16_t c,
    #                 int32_t start)
    assert test1.index_of(ord('o'), 10) == 13

    # [3]
    # int32_t indexOf(char16_t c,
    #                 int32_t start,
    #                 int32_t length)
    assert test1.index_of(ord('o'), 10, 3) == -1
    assert test1.index_of(ord('o'), 10, 4) == 13

    # [4]
    # int32_t indexOf(const char16_t *srcChars,
    #                 int32_t srcLength,
    #                 int32_t start)
    assert test1.index_of("ipsum", 2, 10) == 42

    # [5]
    # int32_t indexOf(const char16_t *srcChars,
    #                 int32_t srcStart,
    #                 int32_t srcLength,
    #                 int32_t start,
    #                 int32_t length)
    assert test1.index_of("ipsum", 0, 2, 30, 13) == -1
    assert test1.index_of("ipsum", 0, 2, 30, 14) == 42

    # [6]
    # int32_t indexOf(const UnicodeString &srcText,
    #                 int32_t srcStart,
    #                 int32_t srcLength,
    #                 int32_t start,
    #                 int32_t length)
    assert test1.index_of(test2, 0, 2, 30, 13) == -1
    assert test1.index_of(test2, 0, 2, 30, 14) == 42

    # [7]
    # int32_t indexOf(const UnicodeString &text)
    assert test1.index_of(test2) == 6

    # [8]
    # int32_t indexOf(const UnicodeString &text,
    #                 int32_t start)
    assert test1.index_of(test2, 6) == 6
    assert test1.index_of(test2, 7) == -1

    # [9]
    # int32_t indexOf(const UnicodeString &text,
    #                 int32_t start,
    #                 int32_t length)
    assert test1.index_of(test2, 0, 11) == 6
    assert test1.index_of(test2, 0, 10) == -1

    # [10]
    # int32_t indexOf(ConstChar16Ptr srcChars,
    #                 int32_t srcLength,
    #                 int32_t start,
    #                 int32_t length)
    assert test1.index_of("ipsum", 2, 30, 14) == 42

    # [11]
    # int32_t indexOf(UChar32 c)
    assert test3.index_of(0xd841) == 4
    assert test3.index_of(0xdc02) == 3

    # [12]
    # int32_t indexOf(UChar32 c,
    #                 int32_t start)
    assert test3.index_of(0xd841, 10) == 17

    # [13]
    # int32_t indexOf(UChar32 c,
    #                 int32_t start,
    #                 int32_t length)
    if U_ICU_VERSION_MAJOR_NUM >= 68:
        assert test3.index_of(0xd841, 10, 4) == 13
    else:  # U_ICU_VERSION_MAJOR_NUM <= 67
        assert test3.index_of(0xd841, 10, 4) == -1
    assert test3.index_of(0xd841, 10, 8) == 17

    # __contains__()
    assert "ipsum" in test1
    assert test2 in test1


def test_insert():
    # [1]
    # UnicodeString & insert(int32_t start,
    #                        char16_t srcChar)
    test1 = UnicodeString()
    test1.insert(0, 0x63).insert(0, 0x61).insert(1, 0x62)
    assert test1 == "abc"

    # [2]
    # UnicodeString & insert(int32_t start,
    #                        const char16_t *srcChars,
    #                        int32_t srcStart,
    #                        int32_t srcLength)
    test1 = UnicodeString()
    test1.insert(0, "abc", 2, 1).insert(0, "abc", 0, 1).insert(1, "abc", 1, 1)
    assert test1 == "abc"

    # [3]
    # UnicodeString & insert(int32_t start,
    #                        const UnicodeString &srcText)
    test1 = UnicodeString()
    test1.insert(
        0, UnicodeString("baz")).insert(
        0, UnicodeString("foo")).insert(
        3, UnicodeString(" bar ")
    )
    assert test1 == "foo bar baz"

    # [4]
    # UnicodeString & insert(int32_t start,
    #                        const UnicodeString &srcText,
    #                        int32_t srcStart,
    #                        int32_t srcLength)
    test1 = UnicodeString()
    test2 = UnicodeString("foo bar baz", -1, US_INV)
    test1.insert(0, test2, 8, 3).insert(0, test2, 0, 4).insert(4, test2, 4, 4)
    assert test1 == "foo bar baz"

    # [6]
    # UnicodeString & insert(int32_t start,
    #                        UChar32 srcChar)
    test1 = UnicodeString()
    test1.insert(0, 0x20402).insert(0, 0x20100).insert(2, 0x0074)
    expected = UnicodeString("\\uD840\\uDD00t\\uD841\\uDC02",
                             -1,
                             US_INV).unescape()
    assert test1 == expected
    assert len(test1) == 5

    # [5]
    # UnicodeString & insert(int32_t start,
    #                        ConstChar16Ptr srcChars,
    #                        int32_t srcLength)
    test1 = UnicodeString()
    test1.insert(
        0, "baz qux", 3).insert(
        0, "foo bar", 4).insert(
        4, "bar baz", 4)
    assert test1 == "foo bar baz"


def test_last_index_of():
    text = ("Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
            "sed do eiusmod tempor incididunt ut labore et dolore magna "
            "aliqua.")
    test1 = UnicodeString(text, -1, US_INV)
    test2 = UnicodeString("ipsum", -1, US_INV)
    test3 = UnicodeString(
        "\\ud841\\udc02\\u0071\\udc02\\ud841\\u0071\\ud841\\udc02"
        "\\u0071\\u0072\\ud841\\udc02\\u0071\\ud841\\udc02\\u0071"
        "\\udc02\\ud841\\u0073\\u0000").unescape()

    # [1]
    # int32_t lastIndexOf(char16_t c)
    assert test1.last_index_of(ord('o')) == 106

    # [2]
    # int32_t lastIndexOf(char16_t c,
    #                     int32_t start)
    assert test1.last_index_of(ord('o'), 10) == 106

    # [3]
    # int32_t lastIndexOf(char16_t c,
    #                     int32_t start,
    #                     int32_t length)
    assert test1.last_index_of(ord('o'), 10, 3) == -1
    assert test1.last_index_of(ord('o'), 10, 4) == 13

    # [4]
    # int32_t lastIndexOf(const char16_t *srcChars,
    #                     int32_t srcLength,
    #                     int32_t start)
    assert test1.last_index_of("ipsum", 2, 10) == 42

    # [5]
    # int32_t lastIndexOf(const char16_t *srcChars,
    #                     int32_t srcStart,
    #                     int32_t srcLength,
    #                     int32_t start,
    #                     int32_t length)
    assert test1.last_index_of("ipsum", 0, 2, 30, 13) == -1
    assert test1.last_index_of("ipsum", 0, 2, 30, 14) == 42

    # [6]
    # int32_t lastIndexOf(const UnicodeString &srcText,
    #                     int32_t srcStart,
    #                     int32_t srcLength,
    #                     int32_t start,
    #                     int32_t length)
    assert test1.last_index_of(test2, 0, 2, 30, 13) == -1
    assert test1.last_index_of(test2, 0, 2, 30, 14) == 42

    # [7]
    # int32_t lastIndexOf(const UnicodeString &text)
    assert test1.last_index_of(test2) == 6

    # [8]
    # int32_t lastIndexOf(const UnicodeString &text,
    #                     int32_t start)
    assert test1.last_index_of(test2, 6) == 6
    assert test1.last_index_of(test2, 7) == -1

    # [9]
    # int32_t lastIndexOf(const UnicodeString &text,
    #                     int32_t start,
    #                     int32_t length)
    assert test1.last_index_of(test2, 0, 11) == 6
    assert test1.last_index_of(test2, 0, 10) == -1

    # [10]
    # int32_t lastIndexOf(ConstChar16Ptr srcChars,
    #                     int32_t srcLength,
    #                     int32_t start,
    #                     int32_t length)
    assert test1.last_index_of("ipsum", 2, 30, 14) == 42

    # [11]
    # int32_t lastIndexOf(UChar32 c)
    assert test3.last_index_of(0xd841) == 17
    assert test3.last_index_of(0xdc02) == 16

    # [12]
    # int32_t lastIndexOf(UChar32 c,
    #                     int32_t start)
    assert test3.last_index_of(0xd841, 10) == 17

    # [13]
    # int32_t lastIndexOf(UChar32 c,
    #                     int32_t start,
    #                     int32_t length)
    if U_ICU_VERSION_MAJOR_NUM >= 68:
        assert test3.last_index_of(0xd841, 10, 4) == 13
    else:  # U_ICU_VERSION_MAJOR_NUM <= 67
        assert test3.last_index_of(0xd841, 10, 4) == -1
    assert test3.last_index_of(0xd841, 10, 8) == 17


def test_move_index32():
    test1 = UnicodeString("a\U00010000b\U0010ffff\u2029")
    assert test1.move_index32(1, 2) == 4
    assert test1.move_index32(0, 3) == 4
    assert test1.move_index32(test1.length(), -2) == 4


def test_operator():
    test1 = UnicodeString("foo bar baz", -1, US_INV)
    test2 = UnicodeString("foo bar baz")
    test3 = UnicodeString("foo bar")

    # operator!=()
    assert not (test1 != test2)
    assert test1 != test3

    assert not (test1 != "foo bar baz")
    assert test1 != "foo bar"

    # operator<()
    assert not (test1 < test2)
    assert not (test1 < test3)

    assert not (test1 < "foo bar baz")
    assert not (test1 < "foo bar")

    # operator<=()
    assert test1 <= test2
    assert not (test1 <= test3)

    assert test1 <= "foo bar baz"
    assert not (test1 <= "foo bar")

    # operator==()
    assert test1 == test2
    assert not (test1 == test3)

    assert test1 == "foo bar baz"
    assert not (test1 == "foo bar")

    # operator>()
    assert not (test1 > test2)
    assert test1 > test3

    assert not (test1 > "foo bar baz")
    assert test1 > "foo bar"

    # operator>=()
    assert test1 >= test2
    assert test1 >= test3

    assert test1 >= "foo bar baz"
    assert test1 >= "foo bar"

    # operator[]()
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

    assert test1[4:7] == "bar"
    assert test1[0:9:4] == "fbb"
    with pytest.raises(ValueError):  # ValueError: slice step cannot be zero
        _ = test1[::0]

    # operator+=(const UnicodeString &srcText)
    test3 += UnicodeString(" baz")
    assert test3 == test1

    test4 = UnicodeString("foo bar") + UnicodeString(" baz")
    assert test4 == test1

    # operator+=(const char16_t *srcChars)
    test5 = UnicodeString("foo bar")
    test5 += " baz"
    assert test5 == test1

    test6 = UnicodeString("foo bar") + " baz"
    assert test6 == test1

    # operator+=(UChar32 ch)
    test7 = UnicodeString("a")
    test7 += 0x20402
    test7 += 0x62
    expected = UnicodeString("a\\uD841\\uDC02b", -1, US_INV).unescape()
    assert test7 == expected


def test_pad_leading():
    test1 = UnicodeString("foo")
    assert test1.pad_leading(10)
    assert test1 == "       foo"

    test1 = UnicodeString("foo")
    assert test1.pad_leading(10, ".")
    assert test1 == ".......foo"


def test_pad_trailing():
    test1 = UnicodeString("foo")
    assert test1.pad_trailing(10)
    assert test1 == "foo       "

    test1 = UnicodeString("foo")
    assert test1.pad_trailing(10, ".")
    assert test1 == "foo......."


def test_remove():
    test1 = UnicodeString("foo bar baz")

    # [2]
    # UnicodeString & remove(int32_t start,
    #                        int32_t length = (int32_t)INT32_MAX)
    test1.remove(3, 4)
    assert test1 == "foo baz"

    test1.remove(3)
    assert test1 == "foo"

    # [1]
    # UnicodeString & remove()
    test1.remove()
    assert test1.length() == 0
    assert len(test1) == 0

    # UnicodeString & removeBetween(int32_t start,
    #                               int32_t limit = (int32_t)INT32_MAX)
    test1 = UnicodeString("foo bar baz")
    test1.remove_between(3, 7)
    assert test1 == "foo baz"

    test1.remove_between(3)
    assert test1 == "foo"


def test_replace():
    # [1]
    # UnicodeString & replace(int32_t start,
    #                         int32_t length,
    #                         char16_t srcChar)
    test1 = UnicodeString("foo bar baz")
    test1.replace(
        4, 1, ord("B")).replace(
        8, 1, ord("B")).replace(
        0, 1, ord("F"))
    assert test1 == "Foo Bar Baz"

    # [2]
    # UnicodeString & replace(int32_t start,
    #                         int32_t length,
    #                         const char16_t *srcChars,
    #                         int32_t srcStart,
    #                         int32_t srcLength)
    test1 = UnicodeString("foo bar baz")
    test1.replace(
        4, 3, "BAR", 0, -1).replace(
        8, 3, "BAZ", 0, -1).replace(
        0, 3, "FOO", 0, -1)
    assert test1 == "FOO BAR BAZ"

    # [3]
    # UnicodeString & replace(int32_t start,
    #                         int32_t length,
    #                         const UnicodeString &srcText)
    test1 = UnicodeString("foo bar baz")
    test1.replace(
        4, 3, UnicodeString("BAR")).replace(
        8, 3, UnicodeString("BAZ")).replace(
        0, 3, UnicodeString("FOO"))
    assert test1 == "FOO BAR BAZ"

    # [4]
    # UnicodeString & replace(int32_t start,
    #                         int32_t length,
    #                         const UnicodeString &srcText,
    #                         int32_t srcStart,
    #                         int32_t srcLength)
    test1 = UnicodeString("foo bar baz")
    test1.replace(
        4, 3, UnicodeString("BAR"), 0, 3).replace(
        8, 3, UnicodeString("BAZ"), 0, 3).replace(
        0, 3, UnicodeString("FOO"), 0, 3)
    assert test1 == "FOO BAR BAZ"

    # [5]
    # UnicodeString & replace(int32_t start,
    #                         int32_t length,
    #                         ConstChar16Ptr srcChars,
    #                         int32_t srcLength)
    test1 = UnicodeString("foo bar baz")
    test1.replace(
        4, 3, "BAR", -1).replace(
        8, 3, "BAZ", -1).replace(
        0, 3, "FOO", -1)
    assert test1 == "FOO BAR BAZ"

    # [6]
    # UnicodeString & replace(int32_t start,
    #                         int32_t length,
    #                         UChar32 srcChar)
    test1 = UnicodeString("a\\uD83C\\uDF38b", -1, US_INV).unescape()
    test1.replace(1, 2, 0x1f30f).replace(0, 1, 0x1f30d).replace(4, 1, 0x1f30e)
    assert test1 == "\U0001f30d\U0001f30f\U0001f30e"


def test_replace_between():
    # [1]
    # UnicodeString & replaceBetween(int32_t start,
    #                                int32_t limit,
    #                                const UnicodeString &srcText)
    test1 = UnicodeString("foo bar baz")
    test1.replace_between(
        4, 7, UnicodeString("BAR")).replace_between(
        8, 11, UnicodeString("BAZ")).replace_between(
        0, 3, UnicodeString("FOO"))
    assert test1 == "FOO BAR BAZ"

    # [2]
    # UnicodeString & replaceBetween(int32_t start,
    #                                int32_t limit,
    #                                const UnicodeString &srcText,
    #                                int32_t srcStart,
    #                                int32_t srcLimit)
    test1 = UnicodeString("foo bar baz")
    test1.replace_between(
        4, 7, UnicodeString("BAR"), 0, 3).replace_between(
        8, 11, UnicodeString("BAZ"), 0, 3).replace_between(
        0, 3, UnicodeString("FOO"), 0, 3)
    assert test1 == "FOO BAR BAZ"


def test_retain_between():
    test1 = UnicodeString("foo bar baz")
    test1.retain_between(4, 7)
    assert test1 == "bar"

    test1 = UnicodeString("foo bar baz")
    test1.retain_between(4)
    assert test1 == "bar baz"


def test_reverse():
    # [1]
    # UnicodeString & reverse(int32_t start,
    #                         int32_t length)
    test1 = UnicodeString("0123-456-789")
    test1.reverse(5, 3)
    assert test1 == "0123-654-789"

    # [2]
    # UnicodeString & reverse(void)
    test1 = UnicodeString("0123456789")
    test1.reverse()
    assert test1 == "9876543210"


def test_set_to():
    # [2]
    # UnicodeString & setTo(char16_t srcChar)
    test1 = UnicodeString("a\U0001f338b", -1, US_INV)
    test1.set_to(0x61)
    assert test1 == "a"

    # [3]
    # UnicodeString & setTo(const char16_t *srcChars,
    #                       int32_t srcLength)
    test1 = UnicodeString("a\U0001f338b", -1, US_INV)
    test1.set_to("foo bar baz", 4)
    assert test1 == "foo "

    # [4]
    # UnicodeString & setTo(const UnicodeString &srcText)
    test1 = UnicodeString("a\U0001f338b", -1, US_INV)
    test2 = UnicodeString("foo bar baz")
    test1.set_to(test2)
    assert test1 == "foo bar baz"

    # [5]
    # UnicodeString & setTo(const UnicodeString &srcText,
    #                       int32_t srcStart)
    test1 = UnicodeString("a\U0001f338b", -1, US_INV)
    test2 = UnicodeString("foo bar baz")
    test1.set_to(test2, 4)
    assert test1 == "bar baz"

    # [6]
    # UnicodeString & setTo(const UnicodeString &srcText,
    #                       int32_t srcStart,
    #                       int32_t srcLength)
    test1 = UnicodeString("a\U0001f338b", -1, US_INV)
    test2 = UnicodeString("foo bar baz")
    test1.set_to(test2, 4, 4)
    assert test1 == "bar "

    # [8]
    # UnicodeString & setTo(UChar32 srcChar)
    test1 = UnicodeString("a\U0001f338b", -1, US_INV)
    test1.set_to(0x1f338)
    assert test1 == "\U0001f338"


def test_starts_with():
    test1 = UnicodeString("foo bar")
    test2 = UnicodeString("foo")
    test3 = UnicodeString("bar")
    s2 = "foo"
    s3 = "bar"

    # [1]
    # UBool startsWith(const char16_t *srcChars,
    #                  int32_t srcStart,
    #                  int32_t srcLength)
    assert test1.starts_with(s2, 0, -1)
    assert not test1.starts_with(s3, 0, -1)

    # [2]
    # UBool startsWith(const UnicodeString &srcText,
    #                  int32_t srcStart,
    #                  int32_t srcLength)
    assert test1.starts_with(test2, 0, len(test2))
    assert not test1.starts_with(test3, 0, len(test3))

    # [3]
    # UBool startsWith(const UnicodeString &text)
    assert test1.starts_with(test2)
    assert not test1.starts_with(test3)

    # [4]
    # UBool startsWith(ConstChar16Ptr srcChars,
    #                  int32_t srcLength)
    assert test1.starts_with(s2, -1)
    assert not test1.starts_with(s3, -1)


def test_swap():
    test1 = UnicodeString("abcd", 4, US_INV)
    test2 = UnicodeString(100, 0x7a, 100)
    test1.swap(test2)
    assert test1.length() == 100
    assert test2.length() == 4
    assert test1 == "z" * 100
    assert test2 == "abcd"


def test_temp_sub_string():
    test1 = UnicodeString("abcd", 4, US_INV)
    test2 = test1.temp_sub_string(1, 2)
    test1.append(test2)
    assert test1 == "abcdbc"

    test1 = UnicodeString("abcd", 4, US_INV)
    test2 = test1.temp_sub_string_between(1, 3)
    test1.append(test2)
    assert test1 == "abcdbc"


def test_to_lower():
    default_locale = None
    try:
        default_locale = Locale.get_default()

        # [1]
        # UnicodeString& toLower(const Locale &locale)
        loc = Locale("en", "US")
        test1 = UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        test1.to_lower(loc)
        assert test1 == '\x69'  # U+0069: Latin Small Letter I

        loc = Locale("tr")
        test1 = UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        test1.to_lower(loc)
        assert test1 == '\u0131'  # U+0131: Latin Small Letter Dotless I

        # [2]
        # UnicodeString& toLower(void)
        Locale.set_default(Locale("en", "US"))
        test1 = UnicodeString("\x49")  # U+0049: Latin Capital Letter I
        test1.to_lower()
        assert test1 == '\x69'  # U+0069: Latin Small Letter I
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_to_title():
    # [1]
    # UnicodeString& toTitle(BreakIterator *titleIter)
    test1 = UnicodeString("sTrA\\u00dfE", -1, US_INV).unescape()
    test1.to_title(None)
    assert test1 == UnicodeString("Stra\\u00dfe", -1, US_INV).unescape()

    # [2]
    # UnicodeString& toTitle(BreakIterator *titleIter,
    #                        const Locale &locale)
    src = "«ijs»"
    test1 = UnicodeString(src)
    test1.to_title(None, Locale("nl-BE"))
    assert test1 == UnicodeString("«IJs»")

    test1 = UnicodeString(src)
    test1.to_title(None, Locale("tr-DE"))
    assert test1 == UnicodeString("«İjs»")

    # [3]
    # UnicodeString& toTitle(BreakIterator *titleIter,
    #                        const Locale &locale,
    #                        uint32_t options)
    src = " john. Smith"
    test1 = UnicodeString(src)
    test1.to_title(None,
                   Locale(""),
                   UnicodeString.TITLECASE_WHOLE_STRING |
                   UnicodeString.TITLECASE_NO_LOWERCASE)
    assert test1 == UnicodeString(" John. Smith")

    test1 = UnicodeString(src)
    test1.to_title(None,
                   Locale(""),
                   UnicodeString.TITLECASE_WHOLE_STRING |
                   UnicodeString.TITLECASE_NO_BREAK_ADJUSTMENT)
    assert test1 == UnicodeString(" john. smith")


def test_to_upper():
    default_locale = None
    try:
        default_locale = Locale.get_default()

        # [1]
        # UnicodeString& toUpper(const Locale &locale)
        loc = Locale("en", "US")
        test1 = UnicodeString("\x69")  # U+0069: Latin Small Letter I
        test1.to_upper(loc)
        assert test1 == '\x49'  # U+0049: Latin Capital Letter I

        test1 = UnicodeString("\u0131")  # U+0131: Latin Small Letter Dotless I
        test1.to_upper(loc)
        assert test1 == '\x49'  # U+0049: Latin Capital Letter I

        loc = Locale("tr")
        test1 = UnicodeString("\x69")  # U+0069: Latin Small Letter I
        test1.to_upper(loc)
        assert test1 == '\u0130'  # U+0130: Latin Capital Letter I with Dot Above

        test1 = UnicodeString("\u0131")  # U+0131: Latin Small Letter Dotless I
        test1.to_upper(loc)
        assert test1 == '\x49'  # U+0049: Latin Capital Letter I

        # [2]
        # UnicodeString& toUpper(void)
        Locale.set_default(Locale("en", "US"))
        test1 = UnicodeString("\x69")  # U+0069: Latin Small Letter I
        test1.to_upper()
        assert test1 == '\x49'  # U+0049: Latin Capital Letter I
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_trim():
    test1 = UnicodeString("  \\U0001f338  ", -1, US_INV).unescape()
    assert len(test1) == 6
    test1.trim().insert(0, "a").append("b")
    assert len(test1) == 4
    assert test1 == "a\U0001f338b"


def test_truncate():
    test1 = UnicodeString("foo bar baz", -1, US_INV).unescape()
    assert not test1.truncate(15)
    assert len(test1) == 11

    assert test1.truncate(7)
    assert len(test1) == 7
    assert test1 == "foo bar"


def test_unescape():
    # From icu/source/test/intltest/ustrtest.cpp:
    # void UnicodeStringTest::TestUnescape(void)
    s = "abc\\u4567 \\n\\r \\U00101234xyz\\x1\\x{5289}\\x1b"
    src = UnicodeString(s, -1, US_INV)
    out = UnicodeString("abc")
    out.append(0x4567)
    out.append(" ")
    out.append(0xA)
    out.append(0xD)
    out.append(" ")
    out.append(0x00101234)
    out.append("xyz", -1)  # out.append("xyz")
    out.append(1).append(0x5289).append(0x1b)
    result1 = src.unescape()
    assert isinstance(result1, UnicodeString)
    assert result1 == out

    result2 = u_unescape(s)
    assert isinstance(result2, str)
    assert result2 == out


def test_unicode_string():
    # [1]
    # UnicodeString()
    test1 = UnicodeString()
    assert not test1.is_bogus()
    assert test1.is_empty()
    assert len(test1) == 0

    # [2]
    # UnicodeString(int32_t capacity,
    #               UChar32 c,
    #               int32_t count)
    test2 = UnicodeString(20, 0x41, 10)
    assert not test2.is_bogus()
    assert not test2.is_empty()
    assert test2 == "A" * 10

    # [3]
    # UnicodeString(char16_t ch)
    test3 = UnicodeString(0x41)
    assert not test3.is_bogus()
    assert not test3.is_empty()
    assert test3 == "A"

    # [4]
    # UnicodeString(UChar32 ch)
    test4 = UnicodeString(0x1f338)
    assert not test4.is_bogus()
    assert not test4.is_empty()
    assert test4 == "\U0001F338"

    # [5]
    # UnicodeString(const char16_t *text)
    test5 = UnicodeString("foo bar baz")
    assert not test5.is_bogus()
    assert not test5.is_empty()
    assert test5 == "foo bar baz"

    # [8]
    # UnicodeString(const std::nullptr_t text)
    test8 = UnicodeString(None)
    assert not test8.is_bogus()
    assert test8.is_empty()
    assert len(test8) == 0

    # [9]
    # UnicodeString(const char16_t *text,
    #               int32_t textLength)
    test9 = UnicodeString("foo bar baz", 4)
    assert not test9.is_bogus()
    assert not test9.is_empty()
    assert test9 == "foo "

    # [12]
    # UnicodeString(const std::nullptr_t text,
    #               int32_t textLength)
    test12 = UnicodeString(None, 10)
    assert not test12.is_bogus()
    assert test12.is_empty()
    assert len(test12) == 0

    # [20]
    # UnicodeString(const char *codepageData,
    #               const char *codepage)
    sjis = b"\x61\x5b\x5c\xb1\x87\x54"
    utf8 = sjis.decode("cp932")
    test20 = UnicodeString(sjis, "ibm-943c")
    assert not test20.is_bogus()
    assert not test20.is_empty()
    assert test20 == utf8

    # [21]
    # UnicodeString(const char *codepageData,
    #               int32_t dataLength,
    #               const char *codepage)
    utf16be = b"\x00\x62\x00\x61\x00\x72"
    test21 = UnicodeString(utf16be, len(utf16be), "utf-16-be")
    assert not test21.is_bogus()
    assert not test21.is_empty()
    assert test21 == "bar"

    utf8 = "a\U0001F338b"
    utf32be = utf8.encode("utf-32-be")
    test21a = UnicodeString(utf32be, len(utf32be), "utf-32-be")
    assert not test21a.is_bogus()
    assert not test21a.is_empty()
    assert test21a == utf8

    # [22]
    # UnicodeString(const char *src,
    #               int32_t srcLength,
    #               UConverter *cnv,
    #               UErrorCode &errorCode
    # )
    with gc(ucnv_open("ibm-943c"), ucnv_close) as cnv:
        utf8 = sjis.decode("cp932")
        test22 = UnicodeString(sjis, len(sjis), cnv)
        assert not test22.is_bogus()
        assert not test22.is_empty()
        assert test22 == utf8

    # [23]
    # UnicodeString(const char *src,
    #               int32_t textLength,
    #               enum EInvariant inv)
    test23 = UnicodeString("foo bar baz", -1, US_INV)
    assert not test23.is_bogus()
    assert not test23.is_empty()
    assert test23 == "foo bar baz"

    # [24]
    # UnicodeString(const UnicodeString &that)
    test24 = UnicodeString(test5)
    assert not test24.is_bogus()
    assert not test24.is_empty()
    assert test24 == "foo bar baz"

    # [26]
    # UnicodeString(const UnicodeString &src,
    #               int32_t srcStart)
    test26 = UnicodeString(test5, 4)
    assert not test26.is_bogus()
    assert not test26.is_empty()
    assert test26 == "bar baz"

    # [27]
    # UnicodeString(const UnicodeString &src,
    #               int32_t srcStart,
    #               int32_t srcLength)
    test27 = UnicodeString(test5, 4, 3)
    assert not test27.is_bogus()
    assert not test27.is_empty()
    assert test27 == "bar"


def test_unicode_string_vector():
    t = UnicodeStringVector(3)
    assert len(t) == 3
    assert isinstance(t[0], UnicodeString)
    assert isinstance(t[1], UnicodeString)
    assert isinstance(t[2], UnicodeString)

    t[0].append("foo", -1)
    t[1].append("bar", -1)
    t[2].append("baz", -1)
    assert t[0] == "foo"
    assert t[1] == "bar"
    assert t[2] == "baz"

    s = [UnicodeString("foo"), UnicodeString("bar"), UnicodeString("baz")]
    t = UnicodeStringVector(s)
    assert len(t) == 3
    assert t[0] == s[0]
    assert t[1] == s[1]
    assert t[2] == s[2]

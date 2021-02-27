import copy

import pytest
from icupy import (
    RegexMatcher, RegexPattern, UParseError, URegexpFlag, UTextVector,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString, UnicodeStringVector,
    utext_close, utext_extract, utext_native_length,
    utext_open_const_unicode_string, utext_open_utf8,
)


def test_clone():
    regex = UnicodeString("\\w+")
    test1 = RegexPattern.compile(regex, 0)
    assert isinstance(test1, RegexPattern)
    test2 = test1.clone()
    assert isinstance(test2, RegexPattern)
    assert test2.pattern() == regex

    test3 = copy.copy(test1)
    assert test3.pattern() == regex

    test4 = copy.deepcopy(test1)
    assert test4.pattern() == regex


def test_compile():
    s = "\\w+"

    # [1]
    # static RegexPattern* RegexPattern::compile(
    #       const UnicodeString &regex,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regex1 = UnicodeString(s)
    test1 = RegexPattern.compile(regex1, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert isinstance(test1, RegexPattern)
    assert test1.pattern() == s
    assert test1.flags() == URegexpFlag.UREGEX_CASE_INSENSITIVE

    # [2]
    # static RegexPattern* RegexPattern::compile(
    #       const UnicodeString &regex,
    #       uint32_t flags,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex2 = UnicodeString(s)
    pe = UParseError()
    test2 = RegexPattern.compile(regex2,
                                 URegexpFlag.UREGEX_CASE_INSENSITIVE,
                                 pe)
    assert isinstance(test2, RegexPattern)
    assert test2.pattern() == s
    assert test2.flags() == URegexpFlag.UREGEX_CASE_INSENSITIVE

    # [3]
    # static RegexPattern* RegexPattern::compile(
    #       const UnicodeString &regex,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex3 = UnicodeString(s)
    pe = UParseError()
    test3 = RegexPattern.compile(regex3, pe)
    assert isinstance(test3, RegexPattern)
    assert test3.pattern() == s
    assert test3.flags() == 0

    # [4]
    # static RegexPattern* RegexPattern::compile(
    #       UText *regex,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regex4 = utext_open_utf8(None, s, -1)
    test4 = RegexPattern.compile(regex4, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert isinstance(test4, RegexPattern)
    assert test4.pattern() == s
    assert test4.flags() == URegexpFlag.UREGEX_CASE_INSENSITIVE
    utext_close(regex4)

    # [5]
    # static RegexPattern* RegexPattern::compile(
    #       UText *regex,
    #       uint32_t flags,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex5 = utext_open_utf8(None, s, -1)
    pe = UParseError()
    test5 = RegexPattern.compile(regex5,
                                 URegexpFlag.UREGEX_CASE_INSENSITIVE,
                                 pe)
    assert isinstance(test5, RegexPattern)
    assert test5.pattern() == s
    assert test5.flags() == URegexpFlag.UREGEX_CASE_INSENSITIVE
    utext_close(regex5)

    # [6]
    # static RegexPattern* RegexPattern::compile(
    #       UText *regex,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex6 = utext_open_utf8(None, s, -1)
    pe = UParseError()
    test6 = RegexPattern.compile(regex6, pe)
    assert isinstance(test6, RegexPattern)
    assert test6.pattern() == s
    assert test6.flags() == 0
    utext_close(regex6)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_group_number_from_name():
    regex = UnicodeString("01(?<A>23(?<B>45)67)(?<C>.*)")
    pattern = RegexPattern.compile(regex, 0)

    # [1]
    # int32_t RegexPattern::groupNumberFromName(
    #       const char *groupName,
    #       int32_t nameLength,
    #       UErrorCode &status
    # )
    assert pattern.group_number_from_name("A", -1) == 1
    assert pattern.group_number_from_name("B", -1) == 2
    assert pattern.group_number_from_name("C", -1) == 3

    # [2]
    # int32_t RegexPattern::groupNumberFromName(
    #       const UnicodeString &groupName,
    #       UErrorCode &status
    # )
    assert pattern.group_number_from_name(UnicodeString("A")) == 1
    assert pattern.group_number_from_name(UnicodeString("B")) == 2
    assert pattern.group_number_from_name(UnicodeString("C")) == 3


def test_matcher():
    regex = UnicodeString("\\w+")
    pattern = RegexPattern.compile(regex, 0)

    # [1]
    # RegexMatcher* RegexPattern::matcher(const UnicodeString &input,
    #                                     UErrorCode &status
    # )
    src1 = UnicodeString("foo bar baz")
    matcher1 = pattern.matcher(src1)
    assert isinstance(matcher1, RegexMatcher)
    assert matcher1.pattern().pattern() == regex
    assert matcher1.input() == src1

    # [2]
    # RegexMatcher* RegexPattern::matcher(UErrorCode &status)
    matcher2 = pattern.matcher()
    assert isinstance(matcher2, RegexMatcher)
    assert matcher2.pattern().pattern() == regex
    assert len(matcher2.input()) == 0


def test_matches():
    regex1 = UnicodeString("[A-Za-z]+")
    src1a = UnicodeString("abc")
    src1b = UnicodeString("123")

    # [1]
    # static UBool RegexPattern::matches(const UnicodeString &regex,
    #                                    const UnicodeString &input,
    #                                    UParseError &pe,
    #                                    UErrorCode &status
    # )
    pe = UParseError()
    assert RegexPattern.matches(regex1, src1a, pe)

    pe = UParseError()
    assert not RegexPattern.matches(regex1, src1b, pe)

    # [2]
    # static UBool RegexPattern::matches(UText *regex,
    #                                    UText *input,
    #                                    UParseError &pe,
    #                                    UErrorCode & 	status
    # )
    regex2 = utext_open_const_unicode_string(None, regex1)
    src2a = utext_open_const_unicode_string(None, src1a)
    pe = UParseError()
    assert RegexPattern.matches(regex2, src2a, pe)

    src2b = utext_open_const_unicode_string(None, src1b)
    pe = UParseError()
    assert not RegexPattern.matches(regex2, src2b, pe)

    utext_close(regex2)
    utext_close(src2a)
    utext_close(src2b)


def test_operator():
    regex = UnicodeString("\\w+")
    test1 = RegexPattern.compile(regex, 0)
    test2 = RegexPattern.compile(regex, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    test3 = RegexPattern.compile(regex, 0)

    assert test1 != test2
    assert not (test1 != test3)

    assert not (test1 == test2)
    assert test1 == test3


def test_pattern_text():
    regex = UnicodeString("\\w+")
    pattern = RegexPattern.compile(regex, 0)

    result1 = pattern.pattern()
    assert isinstance(result1, UnicodeString)
    assert result1 == "\\w+"

    result2 = pattern.pattern_text()
    dest = utext_extract(result2, 0, utext_native_length(result2))
    assert dest == "\\w+"
    utext_close(result2)


def test_regex_pattern():
    # [1]
    # RegexPattern::RegexPattern()
    test1 = RegexPattern()
    pattern1 = test1.pattern()
    assert pattern1.length() == 0

    # [2]
    # RegexPattern::RegexPattern(const RegexPattern &source)
    regex = UnicodeString("\\w+")
    source = RegexPattern.compile(regex, 0)
    assert isinstance(source, RegexPattern)
    test2 = RegexPattern(source)
    assert test2.pattern() == "\\w+"


def test_split():
    regex = UnicodeString("\\s+")
    pattern = RegexPattern.compile(regex, 0)
    src1 = UnicodeString("foo bar baz")

    # [1]
    # int32_t RegexPattern::split(const UnicodeString &input,
    #                             UnicodeString dest[],
    #                             int32_t destCapacity,
    #                             UErrorCode &status
    # )
    dest1 = UnicodeStringVector(10)
    result = pattern.split(src1, dest1)
    assert result == 3
    assert dest1[0] == "foo"
    assert dest1[1] == "bar"
    assert dest1[2] == "baz"

    result = pattern.split(src1, dest1, 2)
    assert result == 2
    assert dest1[0] == "foo"
    assert dest1[1] == "bar baz"

    # [2]
    # int32_t RegexPattern::split(UText *input,
    #                             UText *dest[],
    #                             int32_t destCapacity,
    #                             UErrorCode &status
    # )
    src2 = utext_open_const_unicode_string(None, src1)
    dest2 = UTextVector(10)
    result = pattern.split(src2, dest2)
    assert result == 3
    assert utext_extract(dest2[0], 0, utext_native_length(dest2[0])) == "foo"
    assert utext_extract(dest2[1], 0, utext_native_length(dest2[1])) == "bar"
    assert utext_extract(dest2[2], 0, utext_native_length(dest2[2])) == "baz"

    result = pattern.split(src2, dest2, 2)
    assert result == 2
    assert utext_extract(dest2[0], 0, utext_native_length(dest2[0])) == "foo"
    assert (utext_extract(dest2[1], 0, utext_native_length(dest2[1]))
            == "bar baz")
    utext_close(src2)

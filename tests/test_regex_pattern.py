import copy

import pytest

from icupy import icu


def test_clone():
    regex = icu.UnicodeString("\\w+")
    test1 = icu.RegexPattern.compile(regex, 0)
    assert isinstance(test1, icu.RegexPattern)

    # RegexPattern *icu::RegexPattern::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.RegexPattern)
    assert test2.pattern() == regex

    test3 = copy.copy(test1)
    assert test3.pattern() == regex

    test4 = copy.deepcopy(test1)
    assert test4.pattern() == regex


def test_compile():
    s = "\\w+"

    # [1]
    # static RegexPattern *icu::RegexPattern::compile(
    #       const UnicodeString &regex,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regex1 = icu.UnicodeString(s)
    test1 = icu.RegexPattern.compile(regex1, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert isinstance(test1, icu.RegexPattern)
    assert test1.pattern() == s
    assert test1.flags() == icu.URegexpFlag.UREGEX_CASE_INSENSITIVE

    test1a = icu.RegexPattern.compile(s, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert isinstance(test1a, icu.RegexPattern)
    assert test1a.pattern() == s
    assert test1a.flags() == icu.URegexpFlag.UREGEX_CASE_INSENSITIVE

    # [2]
    # static RegexPattern *icu::RegexPattern::compile(
    #       const UnicodeString &regex,
    #       uint32_t flags,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex2 = icu.UnicodeString(s)
    pe = icu.UParseError()
    test2 = icu.RegexPattern.compile(regex2, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE, pe)
    assert isinstance(test2, icu.RegexPattern)
    assert test2.pattern() == s
    assert test2.flags() == icu.URegexpFlag.UREGEX_CASE_INSENSITIVE

    pe = icu.UParseError()
    test2a = icu.RegexPattern.compile(s, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE, pe)
    assert isinstance(test2a, icu.RegexPattern)
    assert test2a.pattern() == s
    assert test2a.flags() == icu.URegexpFlag.UREGEX_CASE_INSENSITIVE

    # [3]
    # static RegexPattern *icu::RegexPattern::compile(
    #       const UnicodeString &regex,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex3 = icu.UnicodeString(s)
    pe = icu.UParseError()
    test3 = icu.RegexPattern.compile(regex3, pe)
    assert isinstance(test3, icu.RegexPattern)
    assert test3.pattern() == s
    assert test3.flags() == 0

    pe = icu.UParseError()
    test3a = icu.RegexPattern.compile(s, pe)
    assert isinstance(test3a, icu.RegexPattern)
    assert test3a.pattern() == s
    assert test3a.flags() == 0

    # [4]
    # static RegexPattern *icu::RegexPattern::compile(
    #       UText *regex,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regex4 = icu.utext_open_utf8(None, s.encode(), -1)
    test4 = icu.RegexPattern.compile(regex4, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert isinstance(test4, icu.RegexPattern)
    assert test4.pattern() == s
    assert test4.flags() == icu.URegexpFlag.UREGEX_CASE_INSENSITIVE
    icu.utext_close(regex4)

    # [5]
    # static RegexPattern *icu::RegexPattern::compile(
    #       UText *regex,
    #       uint32_t flags,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex5 = icu.utext_open_utf8(None, s.encode(), -1)
    pe = icu.UParseError()
    test5 = icu.RegexPattern.compile(regex5, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE, pe)
    assert isinstance(test5, icu.RegexPattern)
    assert test5.pattern() == s
    assert test5.flags() == icu.URegexpFlag.UREGEX_CASE_INSENSITIVE
    icu.utext_close(regex5)

    # [6]
    # static RegexPattern *icu::RegexPattern::compile(
    #       UText *regex,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex6 = icu.utext_open_utf8(None, s.encode(), -1)
    pe = icu.UParseError()
    test6 = icu.RegexPattern.compile(regex6, pe)
    assert isinstance(test6, icu.RegexPattern)
    assert test6.pattern() == s
    assert test6.flags() == 0
    icu.utext_close(regex6)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_group_number_from_name():
    regex = icu.UnicodeString("01(?<A>23(?<B>45)67)(?<C>.*)")
    pattern = icu.RegexPattern.compile(regex, 0)

    # [1]
    # int32_t icu::RegexPattern::groupNumberFromName(
    #       const char *groupName,
    #       int32_t nameLength,
    #       UErrorCode &status
    # )
    assert pattern.group_number_from_name("A", -1) == 1
    assert pattern.group_number_from_name("B", -1) == 2
    assert pattern.group_number_from_name("C", -1) == 3

    # [2]
    # int32_t icu::RegexPattern::groupNumberFromName(
    #       const UnicodeString &groupName,
    #       UErrorCode &status
    # )
    assert pattern.group_number_from_name(icu.UnicodeString("A")) == 1
    assert pattern.group_number_from_name(icu.UnicodeString("B")) == 2
    assert pattern.group_number_from_name(icu.UnicodeString("C")) == 3

    assert pattern.group_number_from_name("A") == 1
    assert pattern.group_number_from_name("B") == 2
    assert pattern.group_number_from_name("C") == 3


def test_matcher():
    regex = icu.UnicodeString("\\w+")
    pattern = icu.RegexPattern.compile(regex, 0)

    # [1]
    # RegexMatcher *icu::RegexPattern::matcher(
    #       const UnicodeString &input,
    #       UErrorCode &status
    # )
    src1 = icu.UnicodeString("foo bar baz")
    matcher1 = pattern.matcher(src1)
    assert isinstance(matcher1, icu.RegexMatcher)
    assert matcher1.pattern().pattern() == regex
    assert matcher1.input() == src1

    # [2]
    # RegexMatcher *icu::RegexPattern::matcher(UErrorCode &status)
    matcher2 = pattern.matcher()
    assert isinstance(matcher2, icu.RegexMatcher)
    assert matcher2.pattern().pattern() == regex
    assert len(matcher2.input()) == 0


def test_matches():
    regex1 = icu.UnicodeString("[A-Za-z]+")
    src1a = icu.UnicodeString("abc")
    src1b = icu.UnicodeString("123")

    # [1]
    # static UBool icu::RegexPattern::matches(
    #       const UnicodeString &regex,
    #       const UnicodeString &input,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    pe = icu.UParseError()
    assert icu.RegexPattern.matches(regex1, src1a, pe) is True

    pe = icu.UParseError()
    assert icu.RegexPattern.matches(regex1, src1b, pe) is False

    pe = icu.UParseError()
    assert icu.RegexPattern.matches("[A-Za-z]+", src1a, pe) is True

    pe = icu.UParseError()
    assert icu.RegexPattern.matches("[A-Za-z]+", src1b, pe) is False

    # [2]
    # static UBool icu::RegexPattern::matches(
    #       UText *regex,
    #       UText *input,
    #       UParseError &pe,
    #       UErrorCode &status
    # )
    regex2 = icu.utext_open_const_unicode_string(None, regex1)
    src2a = icu.utext_open_const_unicode_string(None, src1a)
    pe = icu.UParseError()
    assert icu.RegexPattern.matches(regex2, src2a, pe) is True

    src2b = icu.utext_open_const_unicode_string(None, src1b)
    pe = icu.UParseError()
    assert icu.RegexPattern.matches(regex2, src2b, pe) is False

    icu.utext_close(regex2)
    icu.utext_close(src2a)
    icu.utext_close(src2b)


def test_operator():
    regex = icu.UnicodeString("\\w+")
    test1 = icu.RegexPattern.compile(regex, 0)
    test2 = icu.RegexPattern.compile(regex, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    test3 = icu.RegexPattern.compile(regex, 0)

    # UBool icu::RegexPattern::operator!=(const RegexPattern &that)
    assert test1 != test2
    assert not (test1 != test3)

    # UBool icu::RegexPattern::operator==(const RegexPattern &that)
    assert not (test1 == test2)
    assert test1 == test3


def test_pattern_text():
    regex = icu.UnicodeString("\\w+")
    pattern = icu.RegexPattern.compile(regex, 0)

    # UnicodeString icu::RegexPattern::pattern()
    result1 = pattern.pattern()
    assert isinstance(result1, icu.UnicodeString)
    assert result1 == "\\w+"

    # UText *icu::RegexPattern::patternText(UErrorCode &status)
    result2 = pattern.pattern_text()
    dest = icu.utext_extract(result2, 0, icu.utext_native_length(result2))
    assert dest == "\\w+"
    icu.utext_close(result2)


def test_regex_pattern():
    # [1]
    # icu::RegexPattern::RegexPattern()
    test1 = icu.RegexPattern()
    pattern1 = test1.pattern()
    assert pattern1.length() == 0

    # [2]
    # icu::RegexPattern::RegexPattern(const RegexPattern &source)
    regex = icu.UnicodeString("\\w+")
    source = icu.RegexPattern.compile(regex, 0)
    assert isinstance(source, icu.RegexPattern)
    test2 = icu.RegexPattern(source)
    assert test2.pattern() == "\\w+"


def test_split():
    regex = icu.UnicodeString("\\s+")
    pattern = icu.RegexPattern.compile(regex, 0)
    src1 = icu.UnicodeString("foo bar baz")

    # [1]
    # int32_t icu::RegexPattern::split(
    #       const UnicodeString &input,
    #       UnicodeString dest[],
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest1 = icu.UnicodeStringVector(10)
    result = pattern.split(src1, dest1)
    assert result == 3
    assert dest1[0] == "foo"
    assert dest1[1] == "bar"
    assert dest1[2] == "baz"

    result = pattern.split(src1, dest1, 2)
    assert result == 2
    assert dest1[0] == "foo"
    assert dest1[1] == "bar baz"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = pattern.split(src1, dest1, 0)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # [2]
    # int32_t icu::RegexPattern::split(
    #       UText *input,
    #       UText *dest[],
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    src2 = icu.utext_open_const_unicode_string(None, src1)
    out = icu.UnicodeStringVector(10)
    dest2 = icu.UTextVector(out)
    result = pattern.split(src2, dest2)
    assert result == 3
    assert icu.utext_extract(dest2[0], 0, icu.utext_native_length(dest2[0])) == "foo"
    assert icu.utext_extract(dest2[1], 0, icu.utext_native_length(dest2[1])) == "bar"
    assert icu.utext_extract(dest2[2], 0, icu.utext_native_length(dest2[2])) == "baz"
    assert out[0] == "foo"
    assert out[1] == "bar"
    assert out[2] == "baz"

    result = pattern.split(src2, dest2, 2)
    assert result == 2
    assert icu.utext_extract(dest2[0], 0, icu.utext_native_length(dest2[0])) == "foo"
    assert icu.utext_extract(dest2[1], 0, icu.utext_native_length(dest2[1])) == "bar baz"
    assert out[0] == "foo"
    assert out[1] == "bar baz"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = pattern.split(src2, dest2, 0)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    icu.utext_close(src2)

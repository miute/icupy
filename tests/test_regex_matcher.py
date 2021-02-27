import pytest
from icupy import (
    ConstVoidPtr, ICUException, RegexMatcher, RegexPattern, UErrorCode,
    URegexFindProgressCallbackPtr, URegexMatchCallbackPtr, URegexpFlag,
    UTextVector, UnicodeString, UnicodeStringVector,
    utext_close, utext_extract, utext_get_native_index, utext_native_length,
    utext_open_const_unicode_string, utext_open_unicode_string,
    utext_open_utf8, utext_replace,
)

from . import gc


def test_api():
    regexp = UnicodeString("\\w+")
    src = UnicodeString("foo bar baz")
    matcher = RegexMatcher(regexp, src, 0)

    s = UnicodeString()
    with gc(utext_open_unicode_string(None, s), utext_close) as dest:
        result = matcher.get_input(dest)
        assert result == dest
        assert utext_extract(dest, 0, utext_native_length(dest)) == src

        result = matcher.get_input(None)
        assert utext_extract(result, 0, utext_native_length(result)) == src
        utext_close(result)

    assert matcher.get_stack_limit() > 0
    matcher.set_stack_limit(0)
    assert matcher.get_stack_limit() == 0

    assert matcher.get_time_limit() == 0
    matcher.set_time_limit(10)
    assert matcher.get_time_limit() == 10

    assert matcher.has_anchoring_bounds()
    result = matcher.use_anchoring_bounds(False)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert not matcher.has_anchoring_bounds()

    assert not matcher.has_transparent_bounds()
    result = matcher.use_transparent_bounds(True)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.has_transparent_bounds()

    matcher.use_anchoring_bounds(True)
    matcher.use_transparent_bounds(False)
    assert matcher.find()  # "foo"
    assert not matcher.hit_end()
    assert not matcher.require_end()

    assert matcher.find()  # "bar"
    assert not matcher.hit_end()
    assert not matcher.require_end()

    assert matcher.find()  # "baz"
    assert matcher.hit_end()
    assert not matcher.require_end()

    assert not matcher.find()
    assert matcher.hit_end()
    assert not matcher.require_end()

    result = matcher.input()
    assert isinstance(result, UnicodeString)
    assert result == src

    result = matcher.input_text()
    assert utext_extract(result, 0, utext_native_length(result)) == src

    pattern = matcher.pattern()
    assert isinstance(pattern, RegexPattern)
    assert pattern.pattern() == regexp


def test_append_replacement():
    regexp = UnicodeString("\\w+")
    src = UnicodeString("foo bar baz")
    matcher = RegexMatcher(regexp, src, 0)

    assert matcher.find()
    assert matcher.find()

    # [1]
    # RegexMatcher& RegexMatcher::appendReplacement(
    #       UnicodeString &dest,
    #       const UnicodeString &replacement,
    #       UErrorCode &status
    # )
    dest1 = UnicodeString()
    replacement1 = UnicodeString("abc")
    result = matcher.append_replacement(dest1, replacement1)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert dest1 == "foo abc"

    matcher.reset()
    assert matcher.find()
    assert matcher.find()

    # [2]
    # RegexMatcher& RegexMatcher::appendReplacement(
    #       UText *dest,
    #       UText *replacement,
    #       UErrorCode &status
    # )
    s2 = UnicodeString()
    dest2 = utext_open_unicode_string(None, s2)
    replacement2 = utext_open_utf8(None, "abc", -1)
    result = matcher.append_replacement(dest2, replacement2)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert utext_extract(dest2, 0, utext_native_length(dest2)) == "foo abc"
    utext_close(dest2)
    utext_close(replacement2)


def test_append_tail():
    regexp = UnicodeString("\\w+")
    src = UnicodeString("foo bar baz")
    matcher = RegexMatcher(regexp, src, 0)

    assert matcher.find()
    assert matcher.find()

    # [1]
    # UnicodeString& RegexMatcher::appendTail(UnicodeString &dest)
    dest1 = UnicodeString()
    replacement1 = UnicodeString("abc")
    matcher.append_replacement(dest1, replacement1)
    result = matcher.append_tail(dest1)
    assert isinstance(result, UnicodeString)
    assert result == dest1
    assert dest1 == "foo abc baz"

    matcher.reset()
    assert matcher.find()
    assert matcher.find()

    # [2]
    # UText* RegexMatcher::appendTail(UText *dest,
    #                                 UErrorCode &status
    # )
    s2 = UnicodeString()
    dest2 = utext_open_unicode_string(None, s2)
    replacement2 = utext_open_utf8(None, "abc", -1)
    matcher.append_replacement(dest2, replacement2)
    result = matcher.append_tail(dest2)
    assert result == dest2
    assert utext_extract(dest2, 0, utext_native_length(dest2)) == "foo abc baz"
    utext_close(dest2)
    utext_close(replacement2)


def test_end():
    regexp = UnicodeString("01(23(45)67)(.*)")
    src = UnicodeString("0123456789")
    matcher = RegexMatcher(regexp, src, 0)

    matcher.looking_at(0)

    # [1]
    # int32_t RegexMatcher::end(int32_t group,
    #                           UErrorCode &status
    # )
    #
    # int64_t RegexMatcher::end64(int32_t group,
    #                             UErrorCode &status
    # )
    assert matcher.end(0) == 10
    assert matcher.end(1) == 8
    assert matcher.end(2) == 6
    assert matcher.end(3) == 10

    assert matcher.end64(0) == 10
    assert matcher.end64(1) == 8
    assert matcher.end64(2) == 6
    assert matcher.end64(3) == 10

    # [2]
    # int32_t RegexMatcher::end(UErrorCode &status)
    #
    # int64_t RegexMatcher::end64(UErrorCode &status)
    assert matcher.end() == 10

    assert matcher.end64() == 10


def test_find():
    regexp = UnicodeString("abc")
    src = UnicodeString(".abc..abc..")
    matcher = RegexMatcher(regexp, src, 0)

    # [1]
    # UBool RegexMatcher::find()
    #
    # [3]
    # UBool RegexMatcher::find(UErrorCode &status)
    # (ICU>=55)
    assert matcher.find()
    assert matcher.start() == 1
    assert matcher.find()
    assert matcher.start() == 6
    assert not matcher.find()

    # [2]
    # UBool RegexMatcher::find(int64_t start,
    #                          UErrorCode &status
    # )
    assert matcher.find(2)
    assert matcher.start() == 6
    assert matcher.find(6)
    assert matcher.start() == 6
    assert not matcher.find(7)


def test_group():
    regexp = UnicodeString("01(23(45)67)(.*)")
    src = UnicodeString("0123456789")
    matcher = RegexMatcher(regexp, src, 0)
    assert matcher.group_count() == 3
    matcher.looking_at(0)

    # [1]
    # UnicodeString RegexMatcher::group(int32_t groupNum,
    #                                   UErrorCode &status
    # )
    result = matcher.group(0)
    assert isinstance(result, UnicodeString)
    assert result == "0123456789"
    assert matcher.group(1) == "234567"
    assert matcher.group(2) == "45"
    assert matcher.group(3) == "89"

    # [2]
    # UText* RegexMatcher::group(int32_t groupNum,
    #                            UText *dest,
    #                            int64_t &group_len,
    #                            UErrorCode &status
    # )
    dest, group_len = matcher.group(0, None)
    assert utext_get_native_index(dest) == 0
    assert group_len == 10
    assert utext_extract(dest, 0, 10) == "0123456789"

    result, group_len = matcher.group(1, dest)
    assert result == dest
    assert utext_get_native_index(dest) == 2
    assert group_len == 6
    assert utext_extract(dest, 2, 2 + 6) == "234567"

    result, group_len = matcher.group(2, dest)
    assert utext_get_native_index(dest) == 4
    assert group_len == 2
    assert utext_extract(dest, 4, 4 + 2) == "45"

    result, group_len = matcher.group(3, dest)
    assert utext_get_native_index(dest) == 8
    assert group_len == 2
    assert utext_extract(dest, 8, 8 + 2) == "89"
    utext_close(dest)

    # [3]
    # UnicodeString RegexMatcher::group(UErrorCode &status)
    result = matcher.group()
    assert isinstance(result, UnicodeString)
    assert result == "0123456789"

    # [4]
    # UText* RegexMatcher::group(UText *dest,
    #                            int64_t &group_len,
    #                            UErrorCode &status
    # )
    dest, group_len = matcher.group(None)
    assert utext_get_native_index(dest) == 0
    assert group_len == 10
    assert utext_extract(dest, 0, 10) == "0123456789"

    result, group_len = matcher.group(dest)
    assert result == dest
    assert utext_get_native_index(dest) == 0
    assert group_len == 10
    assert utext_extract(dest, 0, 10) == "0123456789"
    utext_close(dest)


def test_looking_at():
    regexp = UnicodeString("\\w+")
    src = UnicodeString("foo bar baz")
    matcher = RegexMatcher(regexp, src, 0)

    # [1]
    # UBool RegexMatcher::lookingAt(int64_t startIndex,
    #                               UErrorCode &status
    # )
    assert matcher.looking_at(0)
    assert matcher.looking_at(1)
    assert matcher.looking_at(2)
    assert not matcher.looking_at(3)

    # [2]
    # UBool RegexMatcher::lookingAt(UErrorCode &status)
    assert matcher.looking_at()


def test_matches():
    regexp = UnicodeString("\\w+")
    src = UnicodeString("foo bar baz")
    matcher = RegexMatcher(regexp, src, 0)

    # [1]
    # UBool RegexMatcher::matches(int64_t startIndex,
    #                             UErrorCode &status
    # )
    assert not matcher.matches(0)
    assert not matcher.matches(4)
    assert matcher.matches(8)

    # [2]
    # UBool RegexMatcher::matches(UErrorCode &status)
    assert not matcher.matches()


def test_regex_matcher():
    s = "\\w+"

    # [1]
    # RegexMatcher::RegexMatcher(const UnicodeString &regexp,
    #                            uint32_t flags,
    #                            UErrorCode &status
    # )
    regexp1 = UnicodeString(s)
    test1 = RegexMatcher(regexp1, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern1 = test1.pattern()
    assert pattern1.pattern() == s

    # [2]
    # RegexMatcher::RegexMatcher(UText *regexp,
    #                            uint32_t flags,
    #                            UErrorCode &status
    # )
    regexp2 = utext_open_utf8(None, s, -1)
    test2 = RegexMatcher(regexp2, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern2 = test2.pattern()
    assert pattern2.pattern() == s
    utext_close(regexp2)

    # [3]
    # RegexMatcher::RegexMatcher(const UnicodeString &regexp,
    #                            const UnicodeString &input,
    #                            uint32_t flags,
    #                            UErrorCode &status
    # )
    regexp3 = UnicodeString(s)
    src3 = UnicodeString("foo bar baz")
    test3 = RegexMatcher(regexp3, src3, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern3 = test3.pattern()
    assert pattern3.pattern() == s
    assert test3.input() == "foo bar baz"

    # [4]
    # RegexMatcher::RegexMatcher(UText *regexp,
    #                            UText *input,
    #                            uint32_t flags,
    #                            UErrorCode &status
    # )
    regexp4 = utext_open_utf8(None, s, -1)
    src4 = utext_open_utf8(None, "foo bar baz", -1)
    test4 = RegexMatcher(regexp4, src4, URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern4 = test4.pattern()
    assert pattern4.pattern() == s
    assert test4.input() == "foo bar baz"
    utext_close(regexp4)
    utext_close(src4)


def test_region():
    regexp = UnicodeString("\\w+")
    src = UnicodeString("foo bar baz")
    matcher = RegexMatcher(regexp, src, 0)
    assert matcher.region_start() == 0
    assert matcher.region_start64() == 0
    assert matcher.region_end() == len(src)
    assert matcher.region_end64() == len(src)
    assert matcher.find()
    assert matcher.group() == "foo"

    # [1]
    # RegexMatcher& RegexMatcher::region(
    #       int64_t regionStart,
    #       int64_t regionLimit,
    #       int64_t startIndex,
    #       UErrorCode &status
    # )
    result = matcher.region(3, 7, 5)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.region_start() == 3
    assert matcher.region_start64() == 3
    assert matcher.region_end() == 7
    assert matcher.region_end64() == 7
    assert matcher.find()
    assert matcher.group() == "ar"

    # [2]
    # RegexMatcher& RegexMatcher::region(
    #       int64_t start,
    #       int64_t limit,
    #       UErrorCode &status
    # )
    result = matcher.region(3, 7)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.region_start() == 3
    assert matcher.region_start64() == 3
    assert matcher.region_end() == 7
    assert matcher.region_end64() == 7
    assert matcher.find()
    assert matcher.group() == "bar"


def test_replace_all():
    regexp = UnicodeString("abc")
    src = UnicodeString(".abc..abc..")
    matcher = RegexMatcher(regexp, src, 0)

    # [1]
    # UnicodeString RegexMatcher::replaceAll(
    #       const UnicodeString &replacement,
    #       UErrorCode &status
    # )
    result = matcher.replace_all(UnicodeString("xyz"))
    assert isinstance(result, UnicodeString)
    assert result == ".xyz..xyz.."

    # [2]
    # UText* RegexMatcher::replaceAll(UText *replacement,
    #                                 UText *dest,
    #                                 UErrorCode &status
    # )
    replacement = utext_open_utf8(None, "xyz", -1)
    dest = matcher.replace_all(replacement, None)
    assert utext_extract(dest, 0, utext_native_length(dest)) == ".xyz..xyz.."

    utext_open_utf8(replacement, "ABC", -1)
    utext_replace(dest, 0, utext_native_length(dest), "", -1)
    result = matcher.replace_all(replacement, dest)
    assert result == dest
    assert utext_extract(dest, 0, utext_native_length(dest)) == ".ABC..ABC.."
    utext_close(replacement)
    utext_close(dest)


def test_replace_first():
    regexp = UnicodeString("abc")
    src = UnicodeString(".abc..abc..")
    matcher = RegexMatcher(regexp, src, 0)

    # [1]
    # UnicodeString RegexMatcher::replaceFirst(
    #       const UnicodeString &replacement,
    #       UErrorCode &status
    # )
    result = matcher.replace_first(UnicodeString("xyz"))
    assert isinstance(result, UnicodeString)
    assert result == ".xyz..abc.."

    # [2]
    # UText* RegexMatcher::replaceFirst(UText *replacement,
    #                                   UText *dest,
    #                                   UErrorCode &status
    # )
    replacement = utext_open_utf8(None, "xyz", -1)
    dest = matcher.replace_first(replacement, None)
    assert utext_extract(dest, 0, utext_native_length(dest)) == ".xyz..abc.."

    utext_open_utf8(replacement, "ABC", -1)
    utext_replace(dest, 0, utext_native_length(dest), "", -1)
    result = matcher.replace_first(replacement, dest)
    assert result == dest
    assert utext_extract(dest, 0, utext_native_length(dest)) == ".ABC..abc.."
    utext_close(replacement)
    utext_close(dest)


def test_reset():
    regexp = UnicodeString("\\w+")
    src1 = UnicodeString("foo")
    matcher = RegexMatcher(regexp, src1, 0)
    assert matcher.find()
    assert matcher.group() == "foo"
    assert not matcher.find()

    # [1]
    # RegexMatcher& RegexMatcher::reset()
    result = matcher.reset()
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.find()
    assert matcher.group() == "foo"
    assert not matcher.find()

    # [2]
    # RegexMatcher& RegexMatcher::reset(const UnicodeString &input)
    src2 = UnicodeString("foo bar")
    result = matcher.reset(src2)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.find()
    assert matcher.group() == "foo"
    assert matcher.find()
    assert matcher.group() == "bar"
    assert not matcher.find()

    # [3]
    # RegexMatcher& icu::RegexMatcher::reset(int64_t index,
    #                                        UErrorCode &status
    # )
    result = matcher.reset(4)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.find()
    assert matcher.group() == "bar"
    assert not matcher.find()

    # [4]
    # RegexMatcher& RegexMatcher::reset(UText *input)
    src4 = utext_open_utf8(None, "abc xyz", -1)
    result = matcher.reset(src4)
    assert isinstance(result, RegexMatcher)
    assert result == matcher
    assert matcher.find()
    assert matcher.group() == "abc"
    assert matcher.find()
    assert matcher.group() == "xyz"
    assert not matcher.find()
    utext_close(src4)


def test_set_find_progress_callback():
    result1 = []

    # UBool URegexFindProgressCallback(const void *context,
    #                                  int64_t matchIndex
    # )
    def _find_progress_callback1(_context: object, _match_index: int) -> bool:
        assert _context is None
        nonlocal result1
        result1.append(_match_index)
        return True

    def _find_progress_callback2(_context: object, _match_index: int) -> bool:
        assert isinstance(_context, list)
        _context.append(_match_index)
        if _match_index >= 5:
            return False
        return True

    regexp = UnicodeString("abc")
    src = UnicodeString(".abc..abc..")
    matcher = RegexMatcher(regexp, src, 0)

    # void RegexMatcher::getFindProgressCallback(
    #       URegexFindProgressCallback *&callback,
    #       const void *&context,
    #       UErrorCode &status
    # )
    callback0, context0 = matcher.get_find_progress_callback()
    assert isinstance(callback0, URegexFindProgressCallbackPtr)
    assert isinstance(context0, ConstVoidPtr)

    # void RegexMatcher::setFindProgressCallback(
    #       URegexFindProgressCallback *callback,
    #       const void *context,
    #       UErrorCode &status
    # )
    callback1 = URegexFindProgressCallbackPtr(_find_progress_callback1)
    context1 = ConstVoidPtr(None)
    matcher.set_find_progress_callback(callback1, context1)
    assert matcher.find()
    assert matcher.find()
    assert not matcher.find()
    assert result1 == [1, 5, 6]

    result2 = []
    callback2 = URegexFindProgressCallbackPtr(_find_progress_callback2)
    context2 = ConstVoidPtr(result2)
    matcher.set_find_progress_callback(callback2, context2)
    assert matcher.find(0)
    with pytest.raises(ICUException) as exc_info:
        matcher.find()
    assert exc_info.value.args[0] == UErrorCode.U_REGEX_STOPPED_BY_CALLER
    assert result2 == [1, 5]

    result1.clear()
    result2.clear()
    matcher.set_find_progress_callback(callback0, context0)
    assert matcher.find(0)
    assert matcher.find()
    assert not matcher.find()
    assert len(result1) == len(result2) == 0


def test_set_match_callback():
    result1 = []

    # UBool URegexMatchCallback(const void *context, int32_t steps)
    def _match_callback1(_context: object, _steps: int) -> bool:
        assert _context is None
        nonlocal result1
        result1.append(_steps)
        return True

    def _match_callback2(_context: object, _steps: int) -> bool:
        assert isinstance(_context, list)
        _context.append(_steps)
        if _steps >= 5:
            return False
        return True

    regexp = UnicodeString("((.)+\\2)+x")
    matcher = RegexMatcher(regexp, 0)
    src = UnicodeString("aaaaaaaaaaaaaaaaaaaaaaab")

    # void RegexMatcher::getMatchCallback(
    #       URegexMatchCallback *&callback,
    #       const void *&context,
    #       UErrorCode &status
    # )
    callback0, context0 = matcher.get_match_callback()
    assert isinstance(callback0, URegexMatchCallbackPtr)
    assert isinstance(context0, ConstVoidPtr)

    # void RegexMatcher::setMatchCallback(
    #       URegexMatchCallback *callback,
    #       const void *context,
    #       UErrorCode &status
    # )
    callback1 = URegexMatchCallbackPtr(_match_callback1)
    context1 = ConstVoidPtr(None)
    matcher.set_match_callback(callback1, context1)
    matcher.reset(src)
    assert not matcher.matches()
    assert result1 == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]

    result2 = []
    callback2 = URegexMatchCallbackPtr(_match_callback2)
    context2 = ConstVoidPtr(result2)
    matcher.set_match_callback(callback2, context2)
    matcher.reset(src)
    with pytest.raises(ICUException) as exc_info:
        matcher.matches()
    assert exc_info.value.args[0] == UErrorCode.U_REGEX_STOPPED_BY_CALLER
    assert result2 == [1, 2, 3, 4, 5]

    result1.clear()
    result2.clear()
    matcher.set_match_callback(callback0, context0)
    matcher.reset(src)
    assert not matcher.matches()
    assert len(result1) == len(result2) == 0


def test_split():
    regexp = UnicodeString("\\s+")
    matcher = RegexMatcher(regexp, 0)
    src1 = UnicodeString("foo bar baz")

    # [1]
    # int32_t RegexMatcher::split(const UnicodeString &input,
    #                             UnicodeString dest[],
    #                             int32_t destCapacity,
    #                             UErrorCode &status
    # )
    dest1 = UnicodeStringVector(10)
    result = matcher.split(src1, dest1)
    assert result == 3
    assert dest1[0] == "foo"
    assert dest1[1] == "bar"
    assert dest1[2] == "baz"

    result = matcher.split(src1, dest1, 2)
    assert result == 2
    assert dest1[0] == "foo"
    assert dest1[1] == "bar baz"

    # [2]
    # int32_t RegexMatcher::split(UText *input,
    #                             UText *dest[],
    #                             int32_t destCapacity,
    #                             UErrorCode &status
    # )
    src2 = utext_open_const_unicode_string(None, src1)
    dest2 = UTextVector(10)
    result = matcher.split(src2, dest2)
    assert result == 3
    assert utext_extract(dest2[0], 0, utext_native_length(dest2[0])) == "foo"
    assert utext_extract(dest2[1], 0, utext_native_length(dest2[1])) == "bar"
    assert utext_extract(dest2[2], 0, utext_native_length(dest2[2])) == "baz"

    result = matcher.split(src2, dest2, 2)
    assert result == 2
    assert utext_extract(dest2[0], 0, utext_native_length(dest2[0])) == "foo"
    assert (utext_extract(dest2[1], 0, utext_native_length(dest2[1]))
            == "bar baz")
    utext_close(src2)


def test_start():
    regexp = UnicodeString("01(23(45)67)(.*)")
    src = UnicodeString("0123456789")
    matcher = RegexMatcher(regexp, src, 0)

    matcher.looking_at(0)

    # [1]
    # int32_t RegexMatcher::start(int32_t group,
    #                             UErrorCode &status
    # )
    #
    # int64_t RegexMatcher::start64(int32_t group,
    #                               UErrorCode &status
    # )
    assert matcher.start(0) == 0
    assert matcher.start(1) == 2
    assert matcher.start(2) == 4
    assert matcher.start(3) == 8

    assert matcher.start64(0) == 0
    assert matcher.start64(1) == 2
    assert matcher.start64(2) == 4
    assert matcher.start64(3) == 8

    # [2]
    # int32_t RegexMatcher::start(UErrorCode &status)
    #
    # int64_t RegexMatcher::start64(UErrorCode &status)
    assert matcher.start() == 0

    assert matcher.start64() == 0

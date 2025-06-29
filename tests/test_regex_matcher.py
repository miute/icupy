import pytest

from icupy import icu
from icupy.utils import gc


def test_api() -> None:
    regexp = icu.UnicodeString("\\w+")
    src = icu.UnicodeString("foo bar baz")
    matcher = icu.RegexMatcher(regexp, src, 0)

    s = icu.UnicodeString()
    with gc(icu.utext_open_unicode_string(None, s), icu.utext_close) as dest:
        # UText *icu::RegexMatcher::getInput(
        #       UText *dest,
        #       UErrorCode &status
        # )
        result = matcher.get_input(dest)
        assert result == dest
        assert icu.utext_extract(dest, 0, icu.utext_native_length(dest)) == src

        result = matcher.get_input(None)
        assert icu.utext_extract(result, 0, icu.utext_native_length(result)) == src
        icu.utext_close(result)

    # int32_t icu::RegexMatcher::getStackLimit()
    assert matcher.get_stack_limit() > 0

    # void icu::RegexMatcher::setStackLimit(
    #       int32_t limit,
    #       UErrorCode &status
    # )
    matcher.set_stack_limit(0)
    assert matcher.get_stack_limit() == 0

    # int32_t icu::RegexMatcher::getTimeLimit()
    assert matcher.get_time_limit() == 0

    # void icu::RegexMatcher::setTimeLimit(
    #       int32_t limit,
    #       UErrorCode &status
    # )
    matcher.set_time_limit(10)
    assert matcher.get_time_limit() == 10

    # UBool icu::RegexMatcher::hasAnchoringBounds()
    assert matcher.has_anchoring_bounds() is True

    # RegexMatcher &icu::RegexMatcher::useAnchoringBounds(UBool b)
    result = matcher.use_anchoring_bounds(False)
    assert isinstance(result, icu.RegexMatcher)
    assert result == matcher
    assert matcher.has_anchoring_bounds() is False

    # UBool icu::RegexMatcher::hasTransparentBounds()
    assert matcher.has_transparent_bounds() is False

    # RegexMatcher &icu::RegexMatcher::useTransparentBounds(UBool b)
    result = matcher.use_transparent_bounds(True)
    assert isinstance(result, icu.RegexMatcher)
    assert result == matcher
    assert matcher.has_transparent_bounds() is True

    matcher.use_anchoring_bounds(True)
    matcher.use_transparent_bounds(False)
    assert matcher.find() is True  # "foo"

    # UBool icu::RegexMatcher::hitEnd()
    assert matcher.hit_end() is False

    # UBool icu::RegexMatcher::requireEnd()
    assert matcher.require_end() is False

    assert matcher.find() is True  # "bar"
    assert matcher.hit_end() is False
    assert matcher.require_end() is False

    assert matcher.find() is True  # "baz"
    assert matcher.hit_end() is True
    assert matcher.require_end() is False

    assert matcher.find() is False
    assert matcher.hit_end() is True
    assert matcher.require_end() is False

    # const UnicodeString &icu::RegexMatcher::input()
    result = matcher.input()
    assert isinstance(result, icu.UnicodeString)
    assert result == src

    # UText *icu::RegexMatcher::inputText()
    result = matcher.input_text()
    assert icu.utext_extract(result, 0, icu.utext_native_length(result)) == src

    # const RegexPattern &icu::RegexMatcher::pattern()
    pattern = matcher.pattern()
    assert isinstance(pattern, icu.RegexPattern)
    assert pattern.pattern() == regexp


def test_append_replacement() -> None:
    regexp = icu.UnicodeString("\\w+")
    src = icu.UnicodeString("foo bar baz")
    matcher = icu.RegexMatcher(regexp, src, 0)

    assert matcher.find() is True
    assert matcher.find() is True

    # [1]
    # RegexMatcher &icu::RegexMatcher::appendReplacement(
    #       UnicodeString &dest,
    #       const UnicodeString &replacement,
    #       UErrorCode &status
    # )
    dest1 = icu.UnicodeString()
    replacement1 = icu.UnicodeString("abc")
    result = matcher.append_replacement(dest1, replacement1)
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert dest1 == "foo abc"

    matcher.reset()
    assert matcher.find() is True
    assert matcher.find() is True

    dest1.remove()
    result = matcher.append_replacement(dest1, "abc")
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert dest1 == "foo abc"

    matcher.reset()
    assert matcher.find() is True
    assert matcher.find() is True

    # [2]
    # RegexMatcher &icu::RegexMatcher::appendReplacement(
    #       UText *dest,
    #       UText *replacement,
    #       UErrorCode &status
    # )
    s2 = icu.UnicodeString()
    dest2 = icu.utext_open_unicode_string(None, s2)
    replacement2 = icu.utext_open_utf8(None, b"abc", -1)
    result = matcher.append_replacement(dest2, replacement2)
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert icu.utext_extract(dest2, 0, icu.utext_native_length(dest2)) == "foo abc"
    icu.utext_close(dest2)
    icu.utext_close(replacement2)


def test_append_tail() -> None:
    regexp = icu.UnicodeString("\\w+")
    src = icu.UnicodeString("foo bar baz")
    matcher = icu.RegexMatcher(regexp, src, 0)

    assert matcher.find() is True
    assert matcher.find() is True

    # [1]
    # UnicodeString &icu::RegexMatcher::appendTail(UnicodeString &dest)
    dest1 = icu.UnicodeString()
    replacement1 = icu.UnicodeString("abc")
    matcher.append_replacement(dest1, replacement1)
    result = matcher.append_tail(dest1)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest1)
    assert dest1 == "foo abc baz"

    matcher.reset()
    assert matcher.find() is True
    assert matcher.find() is True

    # [2]
    # UText *icu::RegexMatcher::appendTail(
    #       UText *dest,
    #       UErrorCode &status
    # )
    s2 = icu.UnicodeString()
    dest2 = icu.utext_open_unicode_string(None, s2)
    replacement2 = icu.utext_open_utf8(None, b"abc", -1)
    matcher.append_replacement(dest2, replacement2)
    result = matcher.append_tail(dest2)
    assert result == dest2
    assert icu.utext_extract(dest2, 0, icu.utext_native_length(dest2)) == "foo abc baz"
    icu.utext_close(dest2)
    icu.utext_close(replacement2)


def test_end() -> None:
    regexp = icu.UnicodeString("01(23(45)67)(.*)")
    src = icu.UnicodeString("0123456789")
    matcher = icu.RegexMatcher(regexp, src, 0)

    matcher.looking_at(0)

    # [1]
    # int32_t icu::RegexMatcher::end(
    #       int32_t group,
    #       UErrorCode &status
    # )
    assert matcher.end(0) == 10
    assert matcher.end(1) == 8
    assert matcher.end(2) == 6
    assert matcher.end(3) == 10

    # int64_t icu::RegexMatcher::end64(
    #       int32_t group,
    #       UErrorCode &status
    # )
    assert matcher.end64(0) == 10
    assert matcher.end64(1) == 8
    assert matcher.end64(2) == 6
    assert matcher.end64(3) == 10

    # [2]
    # int32_t icu::RegexMatcher::end(UErrorCode &status)
    assert matcher.end() == 10

    # int64_t icu::RegexMatcher::end64(UErrorCode &status)
    assert matcher.end64() == 10


def test_find() -> None:
    regexp = icu.UnicodeString("abc")
    src = icu.UnicodeString(".abc..abc..")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # [1]
    # UBool icu::RegexMatcher::find()
    #
    # [3]
    # UBool icu::RegexMatcher::find(UErrorCode &status)
    # (ICU>=55)
    assert matcher.find() is True
    assert matcher.start() == 1
    assert matcher.find() is True
    assert matcher.start() == 6
    assert matcher.find() is False

    # [2]
    # UBool icu::RegexMatcher::find(
    #       int64_t start,
    #       UErrorCode &status
    # )
    assert matcher.find(2) is True
    assert matcher.start() == 6
    assert matcher.find(6) is True
    assert matcher.start() == 6
    assert matcher.find(7) is False


def test_group() -> None:
    regexp = icu.UnicodeString("01(23(45)67)(.*)")
    src = icu.UnicodeString("0123456789")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # int32_t icu::RegexMatcher::groupCount()
    assert matcher.group_count() == 3
    matcher.looking_at(0)

    # [1]
    # UnicodeString icu::RegexMatcher::group(
    #       int32_t groupNum,
    #       UErrorCode &status
    # )
    result = matcher.group(0)
    assert isinstance(result, icu.UnicodeString)
    assert result == "0123456789"
    assert matcher.group(1) == "234567"
    assert matcher.group(2) == "45"
    assert matcher.group(3) == "89"

    # [2]
    # UText *icu::RegexMatcher::group(
    #       int32_t groupNum,
    #       UText *dest,
    #       int64_t &group_len,
    #       UErrorCode &status
    # )
    dest, group_len = matcher.group(0, None)
    assert icu.utext_get_native_index(dest) == 0
    assert group_len == 10
    assert icu.utext_extract(dest, 0, 10) == "0123456789"

    result, group_len = matcher.group(1, dest)
    assert result == dest
    assert icu.utext_get_native_index(dest) == 2
    assert group_len == 6
    assert icu.utext_extract(dest, 2, 2 + 6) == "234567"

    result, group_len = matcher.group(2, dest)
    assert icu.utext_get_native_index(dest) == 4
    assert group_len == 2
    assert icu.utext_extract(dest, 4, 4 + 2) == "45"

    result, group_len = matcher.group(3, dest)
    assert icu.utext_get_native_index(dest) == 8
    assert group_len == 2
    assert icu.utext_extract(dest, 8, 8 + 2) == "89"
    icu.utext_close(dest)

    # [3]
    # UnicodeString icu::RegexMatcher::group(UErrorCode &status)
    result = matcher.group()
    assert isinstance(result, icu.UnicodeString)
    assert result == "0123456789"

    # [4]
    # UText *icu::RegexMatcher::group(
    #       UText *dest,
    #       int64_t &group_len,
    #       UErrorCode &status
    # )
    dest, group_len = matcher.group(None)
    assert icu.utext_get_native_index(dest) == 0
    assert group_len == 10
    assert icu.utext_extract(dest, 0, 10) == "0123456789"

    result, group_len = matcher.group(dest)
    assert result == dest
    assert icu.utext_get_native_index(dest) == 0
    assert group_len == 10
    assert icu.utext_extract(dest, 0, 10) == "0123456789"
    icu.utext_close(dest)


def test_looking_at() -> None:
    regexp = icu.UnicodeString("\\w+")
    src = icu.UnicodeString("foo bar baz")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # [1]
    # UBool icu::RegexMatcher::lookingAt(
    #       int64_t startIndex,
    #       UErrorCode &status
    # )
    assert matcher.looking_at(0) is True
    assert matcher.looking_at(1) is True
    assert matcher.looking_at(2) is True
    assert matcher.looking_at(3) is False

    # [2]
    # UBool icu::RegexMatcher::lookingAt(UErrorCode &status)
    assert matcher.looking_at() is True


def test_matches() -> None:
    regexp = icu.UnicodeString("\\w+")
    src = icu.UnicodeString("foo bar baz")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # [1]
    # UBool icu::RegexMatcher::matches(
    #       int64_t startIndex,
    #       UErrorCode &status
    # )
    assert matcher.matches(0) is False
    assert matcher.matches(4) is False
    assert matcher.matches(8) is True

    # [2]
    # UBool icu::RegexMatcher::matches(UErrorCode &status)
    assert matcher.matches() is False


def test_regex_matcher() -> None:
    s = "\\w+"

    # [1]
    # icu::RegexMatcher::RegexMatcher(
    #       const UnicodeString &regexp,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regexp1 = icu.UnicodeString(s)
    test1 = icu.RegexMatcher(regexp1, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern1 = test1.pattern()
    assert pattern1.pattern() == s

    test1a = icu.RegexMatcher(s, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert test1a.pattern().pattern() == s

    # [2]
    # icu::RegexMatcher::RegexMatcher(
    #       UText *regexp,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regexp2 = icu.utext_open_utf8(None, s.encode(), -1)
    test2 = icu.RegexMatcher(regexp2, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern2 = test2.pattern()
    assert pattern2.pattern() == s
    icu.utext_close(regexp2)

    # [3]
    # icu::RegexMatcher::RegexMatcher(
    #       const UnicodeString &regexp,
    #       const UnicodeString &input,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regexp3 = icu.UnicodeString(s)
    src3 = icu.UnicodeString("foo bar baz")
    test3 = icu.RegexMatcher(regexp3, src3, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern3 = test3.pattern()
    assert pattern3.pattern() == s
    assert test3.input() == "foo bar baz"

    test3a = icu.RegexMatcher(s, src3, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    assert test3a.pattern().pattern() == s
    assert test3a.input() == "foo bar baz"

    # [4]
    # icu::RegexMatcher::RegexMatcher(
    #       UText *regexp,
    #       UText *input,
    #       uint32_t flags,
    #       UErrorCode &status
    # )
    regexp4 = icu.utext_open_utf8(None, s.encode(), -1)
    src4 = icu.utext_open_utf8(None, b"foo bar baz", -1)
    test4 = icu.RegexMatcher(regexp4, src4, icu.URegexpFlag.UREGEX_CASE_INSENSITIVE)
    pattern4 = test4.pattern()
    assert pattern4.pattern() == s
    assert test4.input() == "foo bar baz"
    icu.utext_close(regexp4)
    icu.utext_close(src4)


def test_region() -> None:
    regexp = icu.UnicodeString("\\w+")
    src = icu.UnicodeString("foo bar baz")
    matcher = icu.RegexMatcher(regexp, src, 0)
    assert matcher.region_start() == 0
    assert matcher.region_start64() == 0
    assert matcher.region_end() == len(src)
    assert matcher.region_end64() == len(src)
    assert matcher.find() is True
    assert matcher.group() == "foo"

    # [1]
    # RegexMatcher &icu::RegexMatcher::region(
    #       int64_t regionStart,
    #       int64_t regionLimit,
    #       int64_t startIndex,
    #       UErrorCode &status
    # )
    result = matcher.region(3, 7, 5)
    assert isinstance(result, icu.RegexMatcher)
    assert result == matcher
    assert matcher.region_start() == 3
    assert matcher.region_start64() == 3
    assert matcher.region_end() == 7
    assert matcher.region_end64() == 7
    assert matcher.find() is True
    assert matcher.group() == "ar"

    # [2]
    # RegexMatcher &icu::RegexMatcher::region(
    #       int64_t start,
    #       int64_t limit,
    #       UErrorCode &status
    # )
    result = matcher.region(3, 7)
    assert isinstance(result, icu.RegexMatcher)
    assert result == matcher
    assert matcher.region_start() == 3
    assert matcher.region_start64() == 3
    assert matcher.region_end() == 7
    assert matcher.region_end64() == 7
    assert matcher.find() is True
    assert matcher.group() == "bar"


def test_replace_all() -> None:
    regexp = icu.UnicodeString("abc")
    src = icu.UnicodeString(".abc..abc..")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # [1]
    # UnicodeString icu::RegexMatcher::replaceAll(
    #       const UnicodeString &replacement,
    #       UErrorCode &status
    # )
    result = matcher.replace_all(icu.UnicodeString("xyz"))
    assert isinstance(result, icu.UnicodeString)
    assert result == ".xyz..xyz.."

    result = matcher.replace_all("xyz")
    assert isinstance(result, icu.UnicodeString)
    assert result == ".xyz..xyz.."

    # [2]
    # UText *icu::RegexMatcher::replaceAll(
    #       UText *replacement,
    #       UText *dest,
    #       UErrorCode &status
    # )
    replacement = icu.utext_open_utf8(None, b"xyz", -1)
    dest = matcher.replace_all(replacement, None)
    assert icu.utext_extract(dest, 0, icu.utext_native_length(dest)) == ".xyz..xyz.."

    icu.utext_open_utf8(replacement, b"ABC", -1)
    icu.utext_replace(dest, 0, icu.utext_native_length(dest), "", -1)
    result = matcher.replace_all(replacement, dest)
    assert result == dest
    assert icu.utext_extract(dest, 0, icu.utext_native_length(dest)) == ".ABC..ABC.."
    icu.utext_close(replacement)
    icu.utext_close(dest)


def test_replace_first() -> None:
    regexp = icu.UnicodeString("abc")
    src = icu.UnicodeString(".abc..abc..")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # [1]
    # UnicodeString icu::RegexMatcher::replaceFirst(
    #       const UnicodeString &replacement,
    #       UErrorCode &status
    # )
    result = matcher.replace_first(icu.UnicodeString("xyz"))
    assert isinstance(result, icu.UnicodeString)
    assert result == ".xyz..abc.."

    result = matcher.replace_first("xyz")
    assert isinstance(result, icu.UnicodeString)
    assert result == ".xyz..abc.."

    # [2]
    # UText *icu::RegexMatcher::replaceFirst(
    #       UText *replacement,
    #       UText *dest,
    #       UErrorCode &status
    # )
    replacement = icu.utext_open_utf8(None, b"xyz", -1)
    dest = matcher.replace_first(replacement, None)
    assert icu.utext_extract(dest, 0, icu.utext_native_length(dest)) == ".xyz..abc.."

    icu.utext_open_utf8(replacement, b"ABC", -1)
    icu.utext_replace(dest, 0, icu.utext_native_length(dest), "", -1)
    result = matcher.replace_first(replacement, dest)
    assert result == dest
    assert icu.utext_extract(dest, 0, icu.utext_native_length(dest)) == ".ABC..abc.."
    icu.utext_close(replacement)
    icu.utext_close(dest)


def test_reset() -> None:
    regexp = icu.UnicodeString("\\w+")
    src1 = icu.UnicodeString("foo")
    matcher = icu.RegexMatcher(regexp, src1, 0)
    assert matcher.find() is True
    assert matcher.group() == "foo"
    assert matcher.find() is False

    # [1]
    # RegexMatcher &icu::RegexMatcher::reset()
    result = matcher.reset()
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert matcher.find() is True
    assert matcher.group() == "foo"
    assert matcher.find() is False

    # [2]
    # RegexMatcher &icu::RegexMatcher::reset(const UnicodeString &input)
    src2 = icu.UnicodeString("foo bar")
    result = matcher.reset(src2)
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert matcher.find() is True
    assert matcher.group() == "foo"
    assert matcher.find() is True
    assert matcher.group() == "bar"
    assert matcher.find() is False

    # [3]
    # RegexMatcher &icu::RegexMatcher::reset(
    #       int64_t index,
    #       UErrorCode &status
    # )
    result = matcher.reset(4)
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert matcher.find() is True
    assert matcher.group() == "bar"
    assert matcher.find() is False

    # [4]
    # RegexMatcher &icu::RegexMatcher::reset(UText *input)
    src4 = icu.utext_open_utf8(None, b"abc xyz", -1)
    result = matcher.reset(src4)
    assert isinstance(result, icu.RegexMatcher)
    assert id(result) == id(matcher)
    assert matcher.find() is True
    assert matcher.group() == "abc"
    assert matcher.find() is True
    assert matcher.group() == "xyz"
    assert matcher.find() is False
    icu.utext_close(src4)


def test_set_find_progress_callback() -> None:
    result1 = []

    # UBool URegexFindProgressCallback(
    #       const void *context,
    #       int64_t matchIndex
    # )
    def _find_progress_callback1(_context: object, _match_index: int) -> bool:
        assert _context is None
        nonlocal result1
        result1.append(_match_index)
        return True

    def _find_progress_callback2(_context: object, _match_index: int) -> bool:
        assert isinstance(_context, list)
        _context.append(_match_index)
        return _match_index < 5

    regexp = icu.UnicodeString("abc")
    src = icu.UnicodeString(".abc..abc..")
    matcher = icu.RegexMatcher(regexp, src, 0)

    # void icu::RegexMatcher::getFindProgressCallback(
    #       URegexFindProgressCallback *&callback,
    #       const void *&context,
    #       UErrorCode &status
    # )
    callback0, context0 = matcher.get_find_progress_callback()
    assert isinstance(callback0, icu.URegexFindProgressCallbackPtr)
    assert isinstance(context0, icu.ConstVoidPtr)

    # void icu::RegexMatcher::setFindProgressCallback(
    #       URegexFindProgressCallback *callback,
    #       const void *context,
    #       UErrorCode &status
    # )
    callback1 = icu.URegexFindProgressCallbackPtr(_find_progress_callback1)
    context1 = icu.ConstVoidPtr(None)
    matcher.set_find_progress_callback(callback1, context1)
    assert matcher.find() is True
    assert matcher.find() is True
    assert matcher.find() is False
    assert result1 == [1, 5, 6]

    callback1a, context1a = matcher.get_find_progress_callback()
    assert isinstance(callback1a, icu.URegexFindProgressCallbackPtr)
    assert isinstance(context1a, icu.ConstVoidPtr)

    result2: list[int] = []
    callback2 = icu.URegexFindProgressCallbackPtr(_find_progress_callback2)
    context2 = icu.ConstVoidPtr(result2)
    matcher.set_find_progress_callback(callback2, context2)
    assert matcher.find(0) is True
    with pytest.raises(icu.ICUError) as exc_info:
        matcher.find()
    assert exc_info.value.args[0] == icu.UErrorCode.U_REGEX_STOPPED_BY_CALLER
    assert result2 == [1, 5]

    result1.clear()
    result2.clear()
    matcher.set_find_progress_callback(callback0, context0)
    assert matcher.find(0) is True
    assert matcher.find() is True
    assert matcher.find() is False
    assert len(result1) == len(result2) == 0

    result1.clear()
    result2.clear()
    matcher.set_find_progress_callback(callback1a, context1a)
    assert matcher.find(0) is True
    assert matcher.find() is True
    assert matcher.find() is False
    assert result1 == [1, 5, 6]
    assert len(result2) == 0


def test_set_match_callback() -> None:
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
        return _steps < 5

    regexp = icu.UnicodeString("((.)+\\2)+x")
    matcher = icu.RegexMatcher(regexp, 0)
    src = icu.UnicodeString("aaaaaaaaaaaaaaaaaaaaaaab")

    # void icu::RegexMatcher::getMatchCallback(
    #       URegexMatchCallback *&callback,
    #       const void *&context,
    #       UErrorCode &status
    # )
    callback0, context0 = matcher.get_match_callback()
    assert isinstance(callback0, icu.URegexMatchCallbackPtr)
    assert isinstance(context0, icu.ConstVoidPtr)

    # void icu::RegexMatcher::setMatchCallback(
    #       URegexMatchCallback *callback,
    #       const void *context,
    #       UErrorCode &status
    # )
    callback1 = icu.URegexMatchCallbackPtr(_match_callback1)
    context1 = icu.ConstVoidPtr(None)
    matcher.set_match_callback(callback1, context1)
    matcher.reset(src)
    assert not matcher.matches()
    assert result1 == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]

    callback1a, context1a = matcher.get_match_callback()
    assert isinstance(callback1a, icu.URegexMatchCallbackPtr)
    assert isinstance(context1a, icu.ConstVoidPtr)

    result2: list[int] = []
    callback2 = icu.URegexMatchCallbackPtr(_match_callback2)
    context2 = icu.ConstVoidPtr(result2)
    matcher.set_match_callback(callback2, context2)
    matcher.reset(src)
    with pytest.raises(icu.ICUError) as exc_info:
        matcher.matches()
    assert exc_info.value.args[0] == icu.UErrorCode.U_REGEX_STOPPED_BY_CALLER
    assert result2 == [1, 2, 3, 4, 5]

    result1.clear()
    result2.clear()
    matcher.set_match_callback(callback0, context0)
    matcher.reset(src)
    assert not matcher.matches()
    assert len(result1) == len(result2) == 0

    result1.clear()
    result2.clear()
    matcher.set_match_callback(callback1a, context1a)
    matcher.reset(src)
    assert not matcher.matches()
    assert result1 == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]


def test_split() -> None:
    regexp = icu.UnicodeString("\\s+")
    matcher = icu.RegexMatcher(regexp, 0)
    src1 = icu.UnicodeString("foo bar baz")

    # [1]
    # int32_t icu::RegexMatcher::split(
    #       const UnicodeString &input,
    #       UnicodeString dest[],
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    dest1 = icu.UnicodeStringVector(10)
    result = matcher.split(src1, dest1)
    assert result == 3
    assert dest1[0] == "foo"
    assert dest1[1] == "bar"
    assert dest1[2] == "baz"

    result = matcher.split(src1, dest1, 2)
    assert result == 2
    assert dest1[0] == "foo"
    assert dest1[1] == "bar baz"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = matcher.split(src1, dest1, 0)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # [2]
    # int32_t icu::RegexMatcher::split(
    #       UText *input,
    #       UText *dest[],
    #       int32_t destCapacity,
    #       UErrorCode &status
    # )
    src2 = icu.utext_open_const_unicode_string(None, src1)
    out = icu.UnicodeStringVector(10)
    dest2 = icu.UTextVector(out)
    result = matcher.split(src2, dest2)
    assert result == 3
    assert icu.utext_extract(dest2[0], 0, icu.utext_native_length(dest2[0])) == "foo"
    assert icu.utext_extract(dest2[1], 0, icu.utext_native_length(dest2[1])) == "bar"
    assert icu.utext_extract(dest2[2], 0, icu.utext_native_length(dest2[2])) == "baz"
    assert out[0] == "foo"
    assert out[1] == "bar"
    assert out[2] == "baz"

    result = matcher.split(src2, dest2, 2)
    assert result == 2
    assert icu.utext_extract(dest2[0], 0, icu.utext_native_length(dest2[0])) == "foo"
    assert icu.utext_extract(dest2[1], 0, icu.utext_native_length(dest2[1])) == "bar baz"
    assert out[0] == "foo"
    assert out[1] == "bar baz"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = matcher.split(src2, dest2, 0)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    icu.utext_close(src2)


def test_start() -> None:
    regexp = icu.UnicodeString("01(23(45)67)(.*)")
    src = icu.UnicodeString("0123456789")
    matcher = icu.RegexMatcher(regexp, src, 0)

    matcher.looking_at(0)

    # [1]
    # int32_t icu::RegexMatcher::start(
    #       int32_t group,
    #       UErrorCode &status
    # )
    assert matcher.start(0) == 0
    assert matcher.start(1) == 2
    assert matcher.start(2) == 4
    assert matcher.start(3) == 8

    # int64_t icu::RegexMatcher::start64(
    #       int32_t group,
    #       UErrorCode &status
    # )
    assert matcher.start64(0) == 0
    assert matcher.start64(1) == 2
    assert matcher.start64(2) == 4
    assert matcher.start64(3) == 8

    # [2]
    # int32_t icu::RegexMatcher::start(UErrorCode &status)
    assert matcher.start() == 0

    # int64_t icu::RegexMatcher::start64(UErrorCode &status)
    assert matcher.start64() == 0

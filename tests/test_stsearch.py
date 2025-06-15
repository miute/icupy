from __future__ import annotations

import copy

import pytest

from icupy import icu


# from icu4c-71_1-src/icu/source/test/intltest/srchtest.cpp
class _TestSearch(icu.SearchIterator):
    def __init__(
        self,
        pattern: icu.UnicodeString | str,
        text: icu.StringCharacterIterator | icu.UnicodeString | str,
    ) -> None:
        super().__init__(text)
        self._pattern = pattern
        self._offset = 0

    def get_offset(self) -> int:
        # virtual int32_t icu::SearchIterator::getOffset() const
        return self._offset

    def _handle_next(self, position: int) -> int:
        # virtual int32_t icu::SearchIterator::handleNext(
        #       int32_t position,
        #       UErrorCode &status
        # )
        text = self.get_text()
        match: int = text.index_of(self._pattern, position)
        if match < 0:
            self._set_match_not_found()
            match = icu.USEARCH_DONE
            return match
        self._set_match_start(match)
        self._offset = match
        self._set_match_length(len(self._pattern))
        return match

    def _handle_prev(self, position: int) -> int:
        # virtual int32_t icu::SearchIterator::handlePrev(
        #       int32_t position,
        #       UErrorCode &status
        # )
        text = self.get_text()
        match: int = text.last_index_of(self._pattern, 0, position)
        if match < 0:
            self._set_match_not_found()
            match = icu.USEARCH_DONE
            return match
        self._set_match_start(match)
        self._offset = match
        self._set_match_length(len(self._pattern))
        return match

    def set_offset(self, position: int) -> None:
        # virtual void icu::SearchIterator::setOffset(
        #       int32_t position,
        #       UErrorCode &status
        # )
        if position >= 0 and position <= len(self.get_text()):
            self._offset = position


def test_api() -> None:
    pattern = icu.UnicodeString("abab")
    text = icu.UnicodeString("abababab")
    breakiter = icu.BreakIterator.create_character_instance(icu.Locale.get_us())
    coll = icu.Collator.create_instance(icu.Locale.get_us())
    it = icu.StringSearch(pattern, text, coll, breakiter)

    assert icu.USEARCH_DONE == icu.StringSearch.DONE

    # int32_t icu::SearchIterator::first(UErrorCode &status)
    assert it.first() == 0

    # int32_t icu::SearchIterator::getMatchedLength(void)
    assert it.get_matched_length() == 4

    # int32_t icu::SearchIterator::getMatchedStart(void)
    assert it.get_matched_start() == 0

    # void icu::SearchIterator::getMatchedText(UnicodeString &result)
    result = icu.UnicodeString()
    it.get_matched_text(result)
    assert result == "abab"

    # int32_t icu::StringSearch::getOffset(void)
    assert it.get_offset() == 0

    # int32_t icu::SearchIterator::following(
    #       int32_t position,
    #       UErrorCode &status
    # )
    assert it.following(0) == 0
    assert it.following(1) == 2
    assert it.following(3) == 4
    assert it.following(5) == icu.USEARCH_DONE

    # int32_t icu::SearchIterator::last(UErrorCode &status)
    assert it.last() == 4

    # void icu::StringSearch::reset()
    it.reset()

    # int32_t icu::SearchIterator::next(UErrorCode &status)
    assert it.next() == 0
    assert it.next() == 4
    assert it.next() == icu.USEARCH_DONE

    assert list(it) == [0, 4]

    # int32_t icu::SearchIterator::preceding(
    #       int32_t position,
    #       UErrorCode &status
    # )
    assert it.preceding(8) == 4
    assert it.preceding(7) == 2
    assert it.preceding(5) == 0
    assert it.preceding(3) == icu.USEARCH_DONE

    # void icu::StringSearch::setOffset(
    #       int32_t position,
    #       UErrorCode &status
    # )
    it.set_offset(8)

    # int32_t icu::SearchIterator::previous(UErrorCode &status)
    assert it.previous() == 4
    assert it.previous() == 0
    assert it.previous() == icu.USEARCH_DONE

    assert reversed(it) == [4, 0]

    assert it.get_offset() == 0
    it.set_offset(4)
    assert it.get_offset() == 4
    with pytest.raises(icu.ICUError) as exc_info:
        it.set_offset(9)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # USearchAttributeValue icu::SearchIterator::getAttribute(
    #       USearchAttribute attribute
    # )
    assert (
        it.get_attribute(icu.USearchAttribute.USEARCH_OVERLAP)
        == icu.USearchAttributeValue.USEARCH_OFF
    )

    # void icu::SearchIterator::setAttribute(
    #       USearchAttribute attribute,
    #       USearchAttributeValue value,
    #       UErrorCode &status
    # )
    it.set_attribute(
        icu.USearchAttribute.USEARCH_OVERLAP,
        icu.USearchAttributeValue.USEARCH_ON,
    )
    assert (
        it.get_attribute(icu.USearchAttribute.USEARCH_OVERLAP)
        == icu.USearchAttributeValue.USEARCH_ON
    )

    # const BreakIterator *icu::SearchIterator::getBreakIterator(void)
    result = it.get_break_iterator()
    assert isinstance(result, icu.BreakIterator)
    assert result == breakiter

    # void icu::SearchIterator::setBreakIterator(
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    breakiter2 = icu.BreakIterator.create_character_instance(icu.Locale.get_japan())
    assert breakiter2 != breakiter
    it.set_break_iterator(breakiter2)
    assert it.get_break_iterator() == breakiter2

    # RuleBasedCollator *icu::StringSearch::getCollator()
    result = it.get_collator()
    assert isinstance(result, icu.RuleBasedCollator)
    assert result == coll

    # void icu::StringSearch::setCollator(
    #       RuleBasedCollator *coll,
    #       UErrorCode &status
    # )
    coll2 = icu.Collator.create_instance(icu.Locale.get_japan())
    assert coll2 != coll
    it.set_collator(coll2)
    assert it.get_collator() == coll2

    # const UnicodeString &icu::StringSearch::getPattern()
    result = it.get_pattern()
    assert isinstance(result, icu.UnicodeString)
    assert result == pattern

    # void icu::StringSearch::setPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    pattern2 = icu.UnicodeString("ab")
    it.set_pattern(pattern2)
    assert it.get_pattern() == pattern2

    it.set_pattern("abc")
    assert it.get_pattern() == "abc"

    # const UnicodeString &icu::SearchIterator::getText(void)
    result = it.get_text()
    assert isinstance(result, icu.UnicodeString)
    assert result == text


def test_clone() -> None:
    pattern = icu.UnicodeString("fox")
    text = icu.UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    coll = icu.Collator.create_instance(icu.Locale.get_us())
    test1 = icu.StringSearch(pattern, text, coll, breakiter)
    assert test1.next() != icu.USEARCH_DONE

    # StringSearch *icu::StringSearch::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.StringSearch)
    assert test2 != test1
    assert test2.next() != icu.USEARCH_DONE
    assert test2 == test1

    test3 = copy.copy(test1)
    assert test3 != test1
    assert test3.next() != icu.USEARCH_DONE
    assert test3 == test1

    test4 = copy.deepcopy(test1)
    assert test4 != test1
    assert test4.next() != icu.USEARCH_DONE
    assert test4 == test1

    # StringSearch *icu::StringSearch::safeClone()
    test5 = test1.safe_clone()
    assert isinstance(test5, icu.StringSearch)
    assert test5 == test1


def test_operator() -> None:
    pattern = icu.UnicodeString("fox")
    text = icu.UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    coll = icu.Collator.create_instance(icu.Locale.get_us())
    test1 = icu.StringSearch(pattern, text, coll, breakiter)
    test2 = icu.StringSearch(pattern, text, coll, breakiter)

    # UBool icu::SearchIterator::operator!=(const SearchIterator &that)
    assert not (test1 != test2)

    # UBool icu::StringSearch::operator==(const SearchIterator &that)
    assert test1 == test2

    assert test1.next() != icu.USEARCH_DONE
    assert test1 != test2
    assert not (test1 == test2)

    assert test2.next() != icu.USEARCH_DONE
    assert not (test1 != test2)
    assert test1 == test2


def test_set_text() -> None:
    pattern = icu.UnicodeString("a")
    text = icu.UnicodeString("abc")
    coll = icu.Collator.create_instance(icu.Locale.get_us())
    it = icu.StringSearch(pattern, text, coll, None)

    # [1]
    # void icu::StringSearch::setText(
    #       CharacterIterator &text,
    #       UErrorCode &status
    # )
    s = icu.UnicodeString("ab c")
    text = icu.StringCharacterIterator(s)
    it.set_text(text)
    assert it.get_text() == s

    # [2]
    # void icu::StringSearch::setText(
    #       const UnicodeString &text,
    #       UErrorCode &status
    # )
    text = icu.UnicodeString("a bc")
    it.set_text(text)
    assert it.get_text() == text

    text = "a\U0001f338b"
    it.set_text(text)
    assert it.get_text() == text


def test_string_search() -> None:
    assert issubclass(icu.StringSearch, icu.SearchIterator)

    pattern = icu.UnicodeString("fox")
    text1 = icu.UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    coll = icu.Collator.create_instance(icu.Locale.get_us())
    text2 = icu.StringCharacterIterator(text1)

    # [1]
    # icu::StringSearch::StringSearch(
    #       const UnicodeString &pattern,
    #       const UnicodeString &text,
    #       const Locale &locale,
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    test1 = icu.StringSearch(pattern, text1, icu.Locale.get_us(), None)
    assert test1.get_break_iterator() is None
    assert test1.get_collator()
    assert test1.get_pattern() == pattern
    assert test1.get_text() == text1

    test1b = icu.StringSearch(pattern, text1, icu.Locale.get_us(), breakiter)
    assert test1b.get_break_iterator() == breakiter
    assert test1b.get_collator()
    assert test1b.get_pattern() == pattern
    assert test1b.get_text() == text1

    test1c = icu.StringSearch(str(pattern), text1, icu.Locale.get_us(), breakiter)
    assert test1c.get_break_iterator() == breakiter
    assert test1c.get_collator()
    assert test1c.get_pattern() == pattern
    assert test1c.get_text() == text1

    test1d = icu.StringSearch(pattern, text1, "en_US", breakiter)
    assert test1d.get_break_iterator() == breakiter
    assert test1d.get_collator()
    assert test1d.get_pattern() == pattern
    assert test1d.get_text() == text1

    test1e = icu.StringSearch(str(pattern), text1, "en_US", breakiter)
    assert test1e.get_break_iterator() == breakiter
    assert test1e.get_collator()
    assert test1e.get_pattern() == pattern
    assert test1e.get_text() == text1

    test1f = icu.StringSearch(pattern, str(text1), icu.Locale.get_us(), breakiter)
    assert test1f.get_break_iterator() == breakiter
    assert test1f.get_collator()
    assert test1f.get_pattern() == pattern
    assert test1f.get_text() == text1

    test1g = icu.StringSearch(str(pattern), str(text1), icu.Locale.get_us(), breakiter)
    assert test1g.get_break_iterator() == breakiter
    assert test1g.get_collator()
    assert test1g.get_pattern() == pattern
    assert test1g.get_text() == text1

    test1h = icu.StringSearch(pattern, str(text1), "en_US", breakiter)
    assert test1h.get_break_iterator() == breakiter
    assert test1h.get_collator()
    assert test1h.get_pattern() == pattern
    assert test1h.get_text() == text1

    test1i = icu.StringSearch(str(pattern), str(text1), "en_US", breakiter)
    assert test1i.get_break_iterator() == breakiter
    assert test1i.get_collator()
    assert test1i.get_pattern() == pattern
    assert test1i.get_text() == text1

    # [2]
    # icu::StringSearch::StringSearch(
    #       const UnicodeString &pattern,
    #       const UnicodeString &text,
    #       RuleBasedCollator *coll,
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    test2 = icu.StringSearch(pattern, text1, coll, None)
    assert test2.get_break_iterator() is None
    assert test2.get_collator() == coll
    assert test2.get_pattern() == pattern
    assert test2.get_text() == text1

    test2b = icu.StringSearch(pattern, text1, coll, breakiter)
    assert test2b.get_break_iterator() == breakiter
    assert test2b.get_collator() == coll
    assert test2b.get_pattern() == pattern
    assert test2b.get_text() == text1

    test2c = icu.StringSearch(str(pattern), text1, coll, breakiter)
    assert test2c.get_break_iterator() == breakiter
    assert test2c.get_collator() == coll
    assert test2c.get_pattern() == pattern
    assert test2c.get_text() == text1

    test2d = icu.StringSearch(pattern, str(text1), coll, breakiter)
    assert test2d.get_break_iterator() == breakiter
    assert test2d.get_collator() == coll
    assert test2d.get_pattern() == pattern
    assert test2d.get_text() == text1

    test2e = icu.StringSearch(str(pattern), str(text1), coll, breakiter)
    assert test2e.get_break_iterator() == breakiter
    assert test2e.get_collator() == coll
    assert test2e.get_pattern() == pattern
    assert test2e.get_text() == text1

    # [3]
    # icu::StringSearch::StringSearch(
    #       const UnicodeString &pattern,
    #       CharacterIterator &text,
    #       const Locale &locale,
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    test3 = icu.StringSearch(pattern, text2, icu.Locale.get_us(), None)
    assert test3.get_break_iterator() is None
    assert test3.get_collator()
    assert test3.get_pattern() == pattern
    assert test3.get_text() == text1

    test3b = icu.StringSearch(pattern, text2, icu.Locale.get_us(), breakiter)
    assert test3b.get_break_iterator() == breakiter
    assert test3b.get_collator()
    assert test3b.get_pattern() == pattern
    assert test3b.get_text() == text1

    test3c = icu.StringSearch(str(pattern), text2, icu.Locale.get_us(), breakiter)
    assert test3c.get_break_iterator() == breakiter
    assert test3c.get_collator()
    assert test3c.get_pattern() == pattern
    assert test3c.get_text() == text1

    test3d = icu.StringSearch(pattern, text2, "en_US", breakiter)
    assert test3d.get_break_iterator() == breakiter
    assert test3d.get_collator()
    assert test3d.get_pattern() == pattern
    assert test3d.get_text() == text1

    test3e = icu.StringSearch(str(pattern), text2, "en_US", breakiter)
    assert test3e.get_break_iterator() == breakiter
    assert test3e.get_collator()
    assert test3e.get_pattern() == pattern
    assert test3e.get_text() == text1

    # [4]
    # icu::StringSearch::StringSearch(
    #       const UnicodeString &pattern,
    #       CharacterIterator &text,
    #       RuleBasedCollator *coll,
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    test4 = icu.StringSearch(pattern, text2, coll, None)
    assert test4.get_break_iterator() is None
    assert test4.get_collator() == coll
    assert test4.get_pattern() == pattern
    assert test4.get_text() == text1

    test4b = icu.StringSearch(pattern, text2, coll, breakiter)
    assert test4b.get_break_iterator() == breakiter
    assert test4b.get_collator() == coll
    assert test4b.get_pattern() == pattern
    assert test4b.get_text() == text1

    test4c = icu.StringSearch(str(pattern), text2, coll, breakiter)
    assert test4c.get_break_iterator() == breakiter
    assert test4c.get_collator() == coll
    assert test4c.get_pattern() == pattern
    assert test4c.get_text() == text1

    # [5]
    # icu::StringSearch::StringSearch(const StringSearch &that)
    test5 = icu.StringSearch(test2b)
    assert test5.get_break_iterator() == breakiter
    assert test5.get_collator() == coll
    assert test5.get_pattern() == pattern
    assert test5.get_text() == text1


def test_subclass_next() -> None:
    pattern = icu.UnicodeString("abc")
    text = icu.UnicodeString("abc abcd abc")
    si = _TestSearch(pattern, text)
    assert si.get_text() == text

    assert si.first() == 0
    assert si.get_offset() == 0
    assert si.get_matched_start() == 0
    assert si.get_matched_length() == 3

    assert si.next() == 4
    assert si.get_offset() == 4
    assert si.get_matched_start() == 4
    assert si.get_matched_length() == 3

    assert si.next() == 9
    assert si.get_offset() == 9
    assert si.get_matched_start() == 9
    assert si.get_matched_length() == 3

    assert si.next() == icu.USEARCH_DONE
    assert si.get_offset() == len(text)
    assert si.get_matched_start() == icu.USEARCH_DONE
    assert si.get_matched_length() == 0

    si.reset()
    assert si.get_offset() == 0


def test_subclass_previous() -> None:
    pattern = icu.UnicodeString("abc")
    text = icu.UnicodeString("abc abcd abc")
    si = _TestSearch(pattern, text)
    assert si.get_text() == text

    assert si.last() == 9
    assert si.get_offset() == 9
    assert si.get_matched_start() == 9
    assert si.get_matched_length() == 3

    assert si.previous() == 4
    assert si.get_offset() == 4
    assert si.get_matched_start() == 4
    assert si.get_matched_length() == 3

    assert si.previous() == 0
    assert si.get_offset() == 0
    assert si.get_matched_start() == 0
    assert si.get_matched_length() == 3

    assert si.previous() == icu.USEARCH_DONE
    assert si.get_offset() == 0
    assert si.get_matched_start() == icu.USEARCH_DONE
    assert si.get_matched_length() == 0


def test_subclass_set_text() -> None:
    pattern = "abc"
    text = "abc abcd abc"
    si = _TestSearch(pattern, text)
    assert si.get_text() == text

    text2 = icu.StringCharacterIterator(text)
    si.set_text(text2)
    assert si.get_text() == text

    text3 = icu.UnicodeString("foo bar baz")
    si.set_text(text3)
    assert si.get_text() == text3

    si.set_text(text)
    assert si.get_text() == text

    si2 = _TestSearch(pattern, text2)
    assert si2.get_text() == text

    assert si == si2
    assert not (si != si2)

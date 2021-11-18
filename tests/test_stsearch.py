import copy

import pytest
from icupy.icu import (
    BreakIterator, Collator, ICUError, Locale, RuleBasedCollator,
    SearchIterator, StringCharacterIterator, StringSearch, UErrorCode,
    USEARCH_DONE, USearchAttribute, USearchAttributeValue, UnicodeString,
)


def test_api():
    pattern = UnicodeString("abab")
    text = UnicodeString("abababab")
    breakiter = BreakIterator.create_character_instance(Locale.get_us())
    coll = Collator.create_instance(Locale.get_us())
    it = StringSearch(pattern, text, coll, breakiter)

    assert USEARCH_DONE == StringSearch.DONE

    # int32_t icu::SearchIterator::first(UErrorCode &status)
    assert it.first() == 0

    # int32_t icu::SearchIterator::getMatchedLength(void)
    assert it.get_matched_length() == 4

    # int32_t icu::SearchIterator::getMatchedStart(void)
    assert it.get_matched_start() == 0

    # void icu::SearchIterator::getMatchedText(UnicodeString &result)
    result = UnicodeString()
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
    assert it.following(5) == USEARCH_DONE

    # int32_t icu::SearchIterator::last(UErrorCode &status)
    assert it.last() == 4

    # void icu::StringSearch::reset()
    it.reset()

    # int32_t icu::SearchIterator::next(UErrorCode &status)
    assert it.next() == 0
    assert it.next() == 4
    assert it.next() == USEARCH_DONE

    assert list(it) == [0, 4]

    # int32_t icu::SearchIterator::preceding(
    #       int32_t position,
    #       UErrorCode &status
    # )
    assert it.preceding(8) == 4
    assert it.preceding(7) == 2
    assert it.preceding(5) == 0
    assert it.preceding(3) == USEARCH_DONE

    # void icu::StringSearch::setOffset(
    #       int32_t position,
    #       UErrorCode &status
    # )
    it.set_offset(8)

    # int32_t icu::SearchIterator::previous(UErrorCode &status)
    assert it.previous() == 4
    assert it.previous() == 0
    assert it.previous() == USEARCH_DONE

    assert reversed(it) == [4, 0]

    assert it.get_offset() == 0
    it.set_offset(4)
    assert it.get_offset() == 4
    with pytest.raises(ICUError) as exc_info:
        it.set_offset(9)
    assert exc_info.value.args[0] == UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # USearchAttributeValue icu::SearchIterator::getAttribute(
    #       USearchAttribute attribute
    # )
    assert (it.get_attribute(USearchAttribute.USEARCH_OVERLAP)
            == USearchAttributeValue.USEARCH_OFF)

    # void icu::SearchIterator::setAttribute(
    #       USearchAttribute attribute,
    #       USearchAttributeValue value,
    #       UErrorCode &status
    # )
    it.set_attribute(USearchAttribute.USEARCH_OVERLAP,
                     USearchAttributeValue.USEARCH_ON)
    assert (it.get_attribute(USearchAttribute.USEARCH_OVERLAP)
            == USearchAttributeValue.USEARCH_ON)

    # const BreakIterator *icu::SearchIterator::getBreakIterator(void)
    result = it.get_break_iterator()
    assert isinstance(result, BreakIterator)
    assert result == breakiter

    # void icu::SearchIterator::setBreakIterator(
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    breakiter2 = BreakIterator.create_character_instance(Locale.get_japan())
    assert breakiter2 != breakiter
    it.set_break_iterator(breakiter2)
    assert it.get_break_iterator() == breakiter2

    # RuleBasedCollator *icu::StringSearch::getCollator()
    result = it.get_collator()
    assert isinstance(result, RuleBasedCollator)
    assert result == coll

    # void icu::StringSearch::setCollator(
    #       RuleBasedCollator *coll,
    #       UErrorCode &status
    # )
    coll2 = Collator.create_instance(Locale.get_japan())
    assert coll2 != coll
    it.set_collator(coll2)
    assert it.get_collator() == coll2

    # const UnicodeString &icu::StringSearch::getPattern()
    result = it.get_pattern()
    assert isinstance(result, UnicodeString)
    assert result == pattern

    # void icu::StringSearch::setPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    pattern2 = UnicodeString("ab")
    it.set_pattern(pattern2)
    assert it.get_pattern() == pattern2

    it.set_pattern("abc")
    assert it.get_pattern() == "abc"

    # const UnicodeString &icu::SearchIterator::getText(void)
    result = it.get_text()
    assert isinstance(result, UnicodeString)
    assert result == text


def test_clone():
    pattern = UnicodeString("fox")
    text = UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = BreakIterator.create_word_instance(Locale.get_us())
    coll = Collator.create_instance(Locale.get_us())
    test1 = StringSearch(pattern, text, coll, breakiter)
    assert test1.next() != USEARCH_DONE

    # StringSearch *icu::StringSearch::clone()
    test2 = test1.clone()
    assert isinstance(test2, StringSearch)
    assert test2 != test1
    assert test2.next() != USEARCH_DONE
    assert test2 == test1

    test3 = copy.copy(test1)
    assert test3 != test1
    assert test3.next() != USEARCH_DONE
    assert test3 == test1

    test4 = copy.deepcopy(test1)
    assert test4 != test1
    assert test4.next() != USEARCH_DONE
    assert test4 == test1

    # StringSearch *icu::StringSearch::safeClone()
    test5 = test1.safe_clone()
    assert isinstance(test5, StringSearch)
    assert test5 == test1


def test_operator():
    pattern = UnicodeString("fox")
    text = UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = BreakIterator.create_word_instance(Locale.get_us())
    coll = Collator.create_instance(Locale.get_us())
    test1 = StringSearch(pattern, text, coll, breakiter)
    test2 = StringSearch(pattern, text, coll, breakiter)

    # UBool icu::SearchIterator::operator!=(const SearchIterator &that)
    assert not (test1 != test2)

    # UBool icu::StringSearch::operator==(const SearchIterator &that)
    assert test1 == test2

    assert test1.next() != USEARCH_DONE
    assert test1 != test2
    assert not (test1 == test2)

    assert test2.next() != USEARCH_DONE
    assert not (test1 != test2)
    assert test1 == test2


def test_set_text():
    pattern = UnicodeString("a")
    text = UnicodeString("abc")
    coll = Collator.create_instance(Locale.get_us())
    it = StringSearch(pattern, text, coll, None)

    # [1]
    # void icu::StringSearch::setText(
    #       CharacterIterator &text,
    #       UErrorCode &status
    # )
    s = UnicodeString("ab c")
    text = StringCharacterIterator(s)
    it.set_text(text)
    assert it.get_text() == s

    # [2]
    # void icu::StringSearch::setText(
    #       const UnicodeString &text,
    #       UErrorCode &status
    # )
    text = UnicodeString("a bc")
    it.set_text(text)
    assert it.get_text() == text

    text = "a\U0001f338b"
    it.set_text(text)
    assert it.get_text() == text


def test_string_search():
    assert issubclass(StringSearch, SearchIterator)

    pattern = UnicodeString("fox")
    text1 = UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = BreakIterator.create_word_instance(Locale.get_us())
    coll = Collator.create_instance(Locale.get_us())
    text2 = StringCharacterIterator(text1)

    # [1]
    # icu::StringSearch::StringSearch(
    #       const UnicodeString &pattern,
    #       const UnicodeString &text,
    #       const Locale &locale,
    #       BreakIterator *breakiter,
    #       UErrorCode &status
    # )
    test1 = StringSearch(pattern, text1, Locale.get_us(), None)
    assert test1.get_break_iterator() is None
    assert test1.get_collator()
    assert test1.get_pattern() == pattern
    assert test1.get_text() == text1

    test1b = StringSearch(pattern, text1, Locale.get_us(), breakiter)
    assert test1b.get_break_iterator() == breakiter
    assert test1b.get_collator()
    assert test1b.get_pattern() == pattern
    assert test1b.get_text() == text1

    test1c = StringSearch(str(pattern), text1, Locale.get_us(), breakiter)
    assert test1c.get_break_iterator() == breakiter
    assert test1c.get_collator()
    assert test1c.get_pattern() == pattern
    assert test1c.get_text() == text1

    test1d = StringSearch(pattern, text1, "en_US", breakiter)
    assert test1d.get_break_iterator() == breakiter
    assert test1d.get_collator()
    assert test1d.get_pattern() == pattern
    assert test1d.get_text() == text1

    test1e = StringSearch(str(pattern), text1, "en_US", breakiter)
    assert test1e.get_break_iterator() == breakiter
    assert test1e.get_collator()
    assert test1e.get_pattern() == pattern
    assert test1e.get_text() == text1

    test1f = StringSearch(pattern, str(text1), Locale.get_us(), breakiter)
    assert test1f.get_break_iterator() == breakiter
    assert test1f.get_collator()
    assert test1f.get_pattern() == pattern
    assert test1f.get_text() == text1

    test1g = StringSearch(str(pattern), str(text1), Locale.get_us(), breakiter)
    assert test1g.get_break_iterator() == breakiter
    assert test1g.get_collator()
    assert test1g.get_pattern() == pattern
    assert test1g.get_text() == text1

    test1h = StringSearch(pattern, str(text1), "en_US", breakiter)
    assert test1h.get_break_iterator() == breakiter
    assert test1h.get_collator()
    assert test1h.get_pattern() == pattern
    assert test1h.get_text() == text1

    test1i = StringSearch(str(pattern), str(text1), "en_US", breakiter)
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
    test2 = StringSearch(pattern, text1, coll, None)
    assert test2.get_break_iterator() is None
    assert test2.get_collator() == coll
    assert test2.get_pattern() == pattern
    assert test2.get_text() == text1

    test2b = StringSearch(pattern, text1, coll, breakiter)
    assert test2b.get_break_iterator() == breakiter
    assert test2b.get_collator() == coll
    assert test2b.get_pattern() == pattern
    assert test2b.get_text() == text1

    test2c = StringSearch(str(pattern), text1, coll, breakiter)
    assert test2c.get_break_iterator() == breakiter
    assert test2c.get_collator() == coll
    assert test2c.get_pattern() == pattern
    assert test2c.get_text() == text1

    test2d = StringSearch(pattern, str(text1), coll, breakiter)
    assert test2d.get_break_iterator() == breakiter
    assert test2d.get_collator() == coll
    assert test2d.get_pattern() == pattern
    assert test2d.get_text() == text1

    test2e = StringSearch(str(pattern), str(text1), coll, breakiter)
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
    test3 = StringSearch(pattern, text2, Locale.get_us(), None)
    assert test3.get_break_iterator() is None
    assert test3.get_collator()
    assert test3.get_pattern() == pattern
    assert test3.get_text() == text1

    test3b = StringSearch(pattern, text2, Locale.get_us(), breakiter)
    assert test3b.get_break_iterator() == breakiter
    assert test3b.get_collator()
    assert test3b.get_pattern() == pattern
    assert test3b.get_text() == text1

    test3c = StringSearch(str(pattern), text2, Locale.get_us(), breakiter)
    assert test3c.get_break_iterator() == breakiter
    assert test3c.get_collator()
    assert test3c.get_pattern() == pattern
    assert test3c.get_text() == text1

    test3d = StringSearch(pattern, text2, "en_US", breakiter)
    assert test3d.get_break_iterator() == breakiter
    assert test3d.get_collator()
    assert test3d.get_pattern() == pattern
    assert test3d.get_text() == text1

    test3e = StringSearch(str(pattern), text2, "en_US", breakiter)
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
    test4 = StringSearch(pattern, text2, coll, None)
    assert test4.get_break_iterator() is None
    assert test4.get_collator() == coll
    assert test4.get_pattern() == pattern
    assert test4.get_text() == text1

    test4b = StringSearch(pattern, text2, coll, breakiter)
    assert test4b.get_break_iterator() == breakiter
    assert test4b.get_collator() == coll
    assert test4b.get_pattern() == pattern
    assert test4b.get_text() == text1

    test4c = StringSearch(str(pattern), text2, coll, breakiter)
    assert test4c.get_break_iterator() == breakiter
    assert test4c.get_collator() == coll
    assert test4c.get_pattern() == pattern
    assert test4c.get_text() == text1

    # [5]
    # icu::StringSearch::StringSearch(const StringSearch &that)
    test5 = StringSearch(test2b)
    assert test5.get_break_iterator() == breakiter
    assert test5.get_collator() == coll
    assert test5.get_pattern() == pattern
    assert test5.get_text() == text1

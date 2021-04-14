import copy

import pytest
from icupy import (
    BreakIterator, Collator, ICUException, Locale, RuleBasedCollator,
    StringCharacterIterator, StringSearch, UErrorCode, USEARCH_DONE,
    USearchAttribute, USearchAttributeValue, UnicodeString,
)


def test_api():
    pattern = UnicodeString("abab")
    text = UnicodeString("abababab")
    breakiter = BreakIterator.create_character_instance(Locale.get_us())
    coll = Collator.create_instance(Locale.get_us())
    it = StringSearch(pattern, text, coll, breakiter)

    assert USEARCH_DONE == StringSearch.DONE
    assert it.first() == 0
    assert it.get_matched_length() == 4
    assert it.get_matched_start() == 0
    result = UnicodeString()
    it.get_matched_text(result)
    assert result == "abab"
    assert it.get_offset() == 0

    assert it.following(0) == 0
    assert it.following(1) == 2
    assert it.following(3) == 4
    assert it.following(5) == USEARCH_DONE

    assert it.last() == 4

    it.reset()
    assert it.next() == 0
    assert it.next() == 4
    assert it.next() == USEARCH_DONE

    assert list(it) == [0, 4]

    assert it.preceding(8) == 4
    assert it.preceding(7) == 2
    assert it.preceding(5) == 0
    assert it.preceding(3) == USEARCH_DONE

    it.set_offset(8)
    assert it.previous() == 4
    assert it.previous() == 0
    assert it.previous() == USEARCH_DONE

    assert reversed(it) == [4, 0]

    assert it.get_offset() == 0
    it.set_offset(4)
    assert it.get_offset() == 4
    with pytest.raises(ICUException) as exc_info:
        it.set_offset(9)
    assert exc_info.value.args[0] == UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    assert (it.get_attribute(USearchAttribute.USEARCH_OVERLAP)
            == USearchAttributeValue.USEARCH_OFF)
    it.set_attribute(USearchAttribute.USEARCH_OVERLAP,
                     USearchAttributeValue.USEARCH_ON)
    assert (it.get_attribute(USearchAttribute.USEARCH_OVERLAP)
            == USearchAttributeValue.USEARCH_ON)

    result = it.get_break_iterator()
    assert isinstance(result, BreakIterator)
    assert result == breakiter
    breakiter2 = BreakIterator.create_character_instance(Locale.get_france())
    it.set_break_iterator(breakiter2)
    assert it.get_break_iterator() == breakiter2

    result = it.get_collator()
    assert isinstance(result, RuleBasedCollator)
    assert result == coll
    coll2 = Collator.create_instance(Locale.get_france())
    it.set_collator(coll2)
    assert it.get_collator() == coll2

    result = it.get_pattern()
    assert isinstance(result, UnicodeString)
    assert result == pattern
    pattern2 = UnicodeString("ab")
    it.set_pattern(pattern2)
    assert it.get_pattern() == pattern2

    it.set_pattern("abc")
    assert it.get_pattern() == "abc"

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

    assert not (test1 != test2)
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
    # void StringSearch::setText(CharacterIterator &text,
    #                            UErrorCode &status)
    text1 = UnicodeString("ab c")
    chariter = StringCharacterIterator(text1)
    it.set_text(chariter)
    assert it.get_text() == text1

    # [2]
    # void StringSearch::setText(const UnicodeString &text,
    #                            UErrorCode &status)
    text2 = UnicodeString("a bc")
    it.set_text(text2)
    assert it.get_text() == text2


def test_string_search():
    pattern = UnicodeString("fox")
    text1 = UnicodeString("The quick brown fox jumps over the lazy dog.")
    breakiter = BreakIterator.create_word_instance(Locale.get_us())
    coll = Collator.create_instance(Locale.get_us())
    text2 = StringCharacterIterator(text1)

    # [1]
    # StringSearch::StringSearch(const UnicodeString &pattern,
    #                            const UnicodeString &text,
    #                            const Locale &locale,
    #                            BreakIterator *breakiter,
    #                            UErrorCode &status)
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

    test1c = StringSearch("fox", text1, Locale.get_us(), breakiter)
    assert test1c.get_break_iterator() == breakiter
    assert test1c.get_collator()
    assert test1c.get_pattern() == pattern
    assert test1c.get_text() == text1

    test1d = StringSearch(pattern, text1, "en_US", breakiter)
    assert test1d.get_break_iterator() == breakiter
    assert test1d.get_collator()
    assert test1d.get_pattern() == pattern
    assert test1d.get_text() == text1

    test1e = StringSearch("fox", text1, "en_US", breakiter)
    assert test1e.get_break_iterator() == breakiter
    assert test1e.get_collator()
    assert test1e.get_pattern() == pattern
    assert test1e.get_text() == text1

    # [2]
    # StringSearch::StringSearch(const UnicodeString &pattern,
    #                            const UnicodeString &text,
    #                            RuleBasedCollator *coll,
    #                            BreakIterator *breakiter,
    #                            UErrorCode &status)
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

    test2c = StringSearch("fox", text1, coll, breakiter)
    assert test2c.get_break_iterator() == breakiter
    assert test2c.get_collator() == coll
    assert test2c.get_pattern() == pattern
    assert test2c.get_text() == text1

    # [3]
    # StringSearch::StringSearch(const UnicodeString &pattern,
    #                            CharacterIterator &text,
    #                            const Locale &locale,
    #                            BreakIterator *breakiter,
    #                            UErrorCode &status)
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

    test3c = StringSearch("fox", text2, Locale.get_us(), breakiter)
    assert test3c.get_break_iterator() == breakiter
    assert test3c.get_collator()
    assert test3c.get_pattern() == pattern
    assert test3c.get_text() == text1

    test3d = StringSearch(pattern, text2, "en_US", breakiter)
    assert test3d.get_break_iterator() == breakiter
    assert test3d.get_collator()
    assert test3d.get_pattern() == pattern
    assert test3d.get_text() == text1

    test3e = StringSearch("fox", text2, "en_US", breakiter)
    assert test3e.get_break_iterator() == breakiter
    assert test3e.get_collator()
    assert test3e.get_pattern() == pattern
    assert test3e.get_text() == text1

    # [4]
    # StringSearch::StringSearch(const UnicodeString &pattern,
    #                            CharacterIterator &text,
    #                            RuleBasedCollator *coll,
    #                            BreakIterator *breakiter,
    #                            UErrorCode &status)
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

    test4c = StringSearch("fox", text2, coll, breakiter)
    assert test4c.get_break_iterator() == breakiter
    assert test4c.get_collator() == coll
    assert test4c.get_pattern() == pattern
    assert test4c.get_text() == text1

    # [5]
    # StringSearch::StringSearch(const StringSearch &that)
    test5 = StringSearch(test2b)
    assert test5.get_break_iterator() == breakiter
    assert test5.get_collator() == coll
    assert test5.get_pattern() == pattern
    assert test5.get_text() == text1

import pytest

from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 56:
    pytest.skip("ICU4C<56", allow_module_level=True)

import copy

# fmt: off
from icupy.icu import (
    U_ICU_VERSION_MAJOR_NUM, BreakIterator, CharacterIterator,
    FilteredBreakIteratorBuilder, Locale, StringCharacterIterator,
    ULocDataLocaleType, UnicodeString, UWordBreak, utext_close, utext_extract,
    utext_native_length, utext_open_const_unicode_string,
)

# fmt: on


def test_break_iterator_adopt_text():
    where = Locale.get_english()
    builder = FilteredBreakIteratorBuilder.create_instance(where)
    assert isinstance(builder, FilteredBreakIteratorBuilder)
    fbi = builder.build(BreakIterator.create_sentence_instance(where))
    assert isinstance(fbi, BreakIterator)
    src = UnicodeString("hello world")
    it = StringCharacterIterator(src)

    # void icu::BreakIterator::adoptText(CharacterIterator *it)
    fbi.adopt_text(it)
    assert fbi.first() == 0
    assert fbi.next() == 11
    assert fbi.next() == BreakIterator.DONE

    fbi.adopt_text(None)
    assert fbi.first() == 0
    assert fbi.next() == BreakIterator.DONE


def test_break_iterator_api():
    where = Locale.get_english()
    builder = FilteredBreakIteratorBuilder.create_instance(where)
    assert isinstance(builder, FilteredBreakIteratorBuilder)
    fbi = builder.build(BreakIterator.create_word_instance(where))
    assert isinstance(fbi, BreakIterator)
    src = UnicodeString("hello world")
    it = StringCharacterIterator(src)
    fbi.adopt_text(it)

    # int32_t icu::BreakIterator::current(void)
    # int32_t icu::BreakIterator::first(void)
    # int32_t icu::BreakIterator::next(int32_t n)
    # int32_t icu::BreakIterator::next(void)
    assert fbi.first() == 0
    assert fbi.current() == 0
    assert fbi.next() == 5
    assert fbi.current() == 5
    assert fbi.next() == 6
    assert fbi.current() == 6
    assert fbi.next() == 11
    assert fbi.current() == 11
    assert fbi.next() == BreakIterator.DONE
    assert fbi.current() == 11

    assert fbi.first() == 0
    assert fbi.next(1) == 5
    assert fbi.next(1) == 6
    assert fbi.next(1) == 11
    assert fbi.next(-1) == 6

    # int32_t icu::BreakIterator::following(int32_t offset)
    assert fbi.following(0) == 5
    assert fbi.following(5) == 6
    assert fbi.following(6) == 11

    # Locale icu::BreakIterator::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    loc = fbi.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, Locale)
    assert loc == where

    loc = fbi.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert isinstance(loc, Locale)
    assert len(loc.get_name()) == 0

    # int32_t icu::BreakIterator::getRuleStatus()
    assert fbi.first() == 0
    assert fbi.get_rule_status() == UWordBreak.UBRK_WORD_NONE
    assert fbi.next() == 5
    assert fbi.get_rule_status() == UWordBreak.UBRK_WORD_NONE

    # int32_t icu::BreakIterator::getRuleStatusVec(
    #       int32_t *fillInVec,
    #       int32_t capacity,
    #       UErrorCode &status
    # )
    assert fbi.first() == 0
    fill_in_vec = fbi.get_rule_status_vec()
    assert isinstance(fill_in_vec, list)
    # assert len(fill_in_vec) == 0
    assert fill_in_vec == [0]  # correct?

    # UBool icu::BreakIterator::isBoundary(int32_t offset)
    assert fbi.is_boundary(0)
    assert not fbi.is_boundary(1)
    assert not fbi.is_boundary(2)
    assert not fbi.is_boundary(3)
    assert not fbi.is_boundary(4)
    assert fbi.is_boundary(5)
    assert fbi.is_boundary(6)
    assert not fbi.is_boundary(7)
    assert not fbi.is_boundary(8)
    assert not fbi.is_boundary(9)
    assert not fbi.is_boundary(10)
    assert fbi.is_boundary(11)

    # int32_t icu::BreakIterator::last(void)
    # int32_t icu::BreakIterator::previous(void)
    assert fbi.last() == 11
    assert fbi.current() == 11
    assert fbi.previous() == 6
    assert fbi.current() == 6
    assert fbi.previous() == 5
    assert fbi.current() == 5
    assert fbi.previous() == 0
    assert fbi.current() == 0

    # int32_t icu::BreakIterator::preceding(int32_t offset)
    assert fbi.preceding(12) == 11
    assert fbi.preceding(11) == 6
    assert fbi.preceding(6) == 5
    assert fbi.preceding(5) == 0

    # CharacterIterator &icu::BreakIterator::getText(void)
    it = fbi.get_text()
    assert isinstance(it, CharacterIterator)
    dest = UnicodeString()
    it.get_text(dest)
    assert dest == src

    # UText *icu::BreakIterator::getUText(
    #       UText *fillIn,
    #       UErrorCode &status
    # )
    ut = fbi.get_utext(None)
    dest = utext_extract(ut, 0, utext_native_length(ut))
    assert dest == src
    utext_close(ut)


def test_break_iterator_clone():
    where = Locale.get_english()
    builder = FilteredBreakIteratorBuilder.create_instance(where)
    fbi = builder.build(BreakIterator.create_sentence_instance(where))

    # BreakIterator *icu::BreakIterator::clone()
    fbi2 = fbi.clone()
    assert isinstance(fbi2, BreakIterator)


@pytest.mark.xfail(
    reason="FIXME: Filtered BreakIterator.__eq__(BreakIterator) is not work"
)
def test_break_iterator_eq():
    where = Locale.get_english()
    builder = FilteredBreakIteratorBuilder.create_instance(where)
    fbi = builder.build(BreakIterator.create_sentence_instance(where))
    text = UnicodeString("hello world")
    fbi.set_text(text)
    fbi2 = fbi.clone()

    # BreakIterator.__copy__() -> BreakIterator
    # BreakIterator.__deepcopy__(Optional[memo]) -> BreakIterator
    fbi2a = copy.copy(fbi)
    fbi2b = copy.deepcopy(fbi)
    # FIXME: Filtered BreakIterator.__eq__(BreakIterator) is not work.
    assert fbi == fbi2 == fbi2a == fbi2b


@pytest.mark.xfail(
    reason="FIXME: Filtered BreakIterator.__ne__(BreakIterator) is not work"
)
def test_break_iterator_ne():
    where = Locale.get_english()
    builder = FilteredBreakIteratorBuilder.create_instance(where)
    fbi = builder.build(BreakIterator.create_sentence_instance(where))
    text = UnicodeString("hello world")
    fbi.set_text(text)
    fbi2 = fbi.clone()

    # FIXME: Filtered BreakIterator.__ne__(BreakIterator) is not work.
    assert not (fbi != fbi2)


def test_filtered_break_iterator_builder_56():
    # [1]
    # static FilteredBreakIteratorBuilder *
    # icu::FilteredBreakIteratorBuilder::createInstance(
    #       const Locale &where,
    #       UErrorCode &status
    # )
    where = Locale.get_english()
    builder1 = FilteredBreakIteratorBuilder.create_instance(where)
    assert isinstance(builder1, FilteredBreakIteratorBuilder)

    builder1a = FilteredBreakIteratorBuilder.create_instance("en")
    assert isinstance(builder1a, FilteredBreakIteratorBuilder)

    # **Deprecated in ICU 60**
    # [2]
    # static FilteredBreakIteratorBuilder *
    # icu::FilteredBreakIteratorBuilder::createInstance(UErrorCode &status)
    builder2 = FilteredBreakIteratorBuilder.create_instance()
    assert isinstance(builder2, FilteredBreakIteratorBuilder)

    # UBool icu::FilteredBreakIteratorBuilder::suppressBreakAfter(
    #       const UnicodeString &string,
    #       UErrorCode &status
    # )
    assert builder2.suppress_break_after(UnicodeString("Mr.")) is True
    assert builder2.suppress_break_after("Capt.") is True
    assert builder2.suppress_break_after("Mr.") is False
    assert builder2.suppress_break_after(UnicodeString("Capt.")) is False

    # UBool icu::FilteredBreakIteratorBuilder::unsuppressBreakAfter(
    #       const UnicodeString &string,
    #       UErrorCode &status
    # )
    assert builder2.unsuppress_break_after(UnicodeString("Capt.")) is True
    assert builder2.unsuppress_break_after("Mr.") is True
    assert builder2.unsuppress_break_after("Capt.") is False
    assert builder2.unsuppress_break_after(UnicodeString("Mr.")) is False

    # **Deprecated in ICU 60**
    # BreakIterator *icu::FilteredBreakIteratorBuilder::build(
    #       BreakIterator *adoptBreakIterator,
    #       UErrorCode &status
    # )
    builder2.suppress_break_after("Mr.")
    fbi = builder2.build(BreakIterator.create_sentence_instance(where))
    assert isinstance(fbi, BreakIterator)

    # [1]
    # void icu::BreakIterator::setText(const UnicodeString &text)
    text = UnicodeString(
        "In the meantime Mr. Weston arrived with his small ship, which he had "
        "now recovered. Capt. Gorges, who informed the Sgt. here that one "
        "purpose of his going east was to meet with Mr. Weston, took this "
        "opportunity to call him to account for some abuses he had to lay to "
        "his charge."
    )
    fbi.set_text(text)
    assert fbi.next() == 84
    assert fbi.next() == 90
    assert fbi.next() == 278
    assert fbi.next() == BreakIterator.DONE

    text2 = UnicodeString()
    fbi.set_text(text2)
    assert fbi.next() == BreakIterator.DONE

    # [2]
    # void icu::BreakIterator::setText(
    #       UText *text,
    #       UErrorCode &status
    # )
    ut = utext_open_const_unicode_string(None, text)
    fbi.set_text(ut)
    assert fbi.next() == 84
    assert fbi.next() == 90
    assert fbi.next() == 278
    assert fbi.next() == BreakIterator.DONE
    utext_close(ut)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_filtered_break_iterator_builder_60():
    # static FilteredBreakIteratorBuilder *
    # icu::FilteredBreakIteratorBuilder::createEmptyInstance(UErrorCode &status)
    builder = FilteredBreakIteratorBuilder.create_empty_instance()
    assert isinstance(builder, FilteredBreakIteratorBuilder)

    assert builder.suppress_break_after("Mr.") is True
    assert builder.suppress_break_after("Mr.") is False
    assert builder.unsuppress_break_after("Mr.") is True
    assert builder.unsuppress_break_after("Mr.") is False
    assert builder.suppress_break_after("Mr.") is True

    # BreakIterator *
    # icu::FilteredBreakIteratorBuilder::wrapIteratorWithFilter(
    #       BreakIterator *adoptBreakIterator,
    #       UErrorCode &status
    # )
    fbi = builder.wrap_iterator_with_filter(
        BreakIterator.create_sentence_instance(Locale.get_english())
    )
    assert isinstance(fbi, BreakIterator)

    text = UnicodeString(
        "In the meantime Mr. Weston arrived with his small ship, which he had "
        "now recovered. Capt. Gorges, who informed the Sgt. here that one "
        "purpose of his going east was to meet with Mr. Weston, took this "
        "opportunity to call him to account for some abuses he had to lay to "
        "his charge."
    )
    fbi.set_text(text)
    assert fbi.next() == 84
    assert fbi.next() == 90
    assert fbi.next() == 278
    assert fbi.next() == BreakIterator.DONE

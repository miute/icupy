import pytest
from icupy.icu import (
    U_ICU_VERSION_MAJOR_NUM, UnicodeSet, UnicodeSetIterator, UnicodeString,
)


def test_next():
    # [1]
    # icu::UnicodeSetIterator::UnicodeSetIterator(const UnicodeSet &set)
    uniset = UnicodeSet("[a\\U0001abcd{ab}]")
    it = UnicodeSetIterator(uniset)

    # UBool icu::UnicodeSetIterator::next()
    assert it.next()

    # UBool icu::UnicodeSetIterator::isString()
    assert not it.is_string()

    # UChar32 icu::UnicodeSetIterator::getCodepoint()
    assert it.get_codepoint() == ord("a")

    # const UnicodeString &icu::UnicodeSetIterator::getString()
    result = it.get_string()
    assert isinstance(result, UnicodeString)
    assert result == "a"

    assert it.next()
    assert not it.is_string()
    assert it.get_codepoint() == 0x1abcd
    assert it.get_string() == "\U0001abcd"

    assert it.next()
    assert it.is_string()
    assert it.get_string() == "ab"

    assert not it.next()

    # [1]
    # void icu::UnicodeSetIterator::reset()
    it.reset()
    assert it.next()
    assert not it.is_string()
    assert it.get_codepoint() == ord("a")
    assert it.get_string() == "a"


def test_next_range():
    # [2]
    # icu::UnicodeSetIterator::UnicodeSetIterator()
    it = UnicodeSetIterator()

    # UBool icu::UnicodeSetIterator::nextRange()
    assert not it.next_range()

    # [2]
    # void icu::UnicodeSetIterator::reset(const UnicodeSet &set)
    uniset = UnicodeSet("[a0-9{ab}]")
    it.reset(uniset)

    assert it.next_range()
    assert not it.is_string()
    assert it.get_codepoint() == ord("0")

    # UChar32 icu::UnicodeSetIterator::getCodepointEnd()
    assert it.get_codepoint_end() == ord("9")

    assert it.next_range()
    assert not it.is_string()
    assert it.get_codepoint() == ord("a")
    assert it.get_codepoint_end() == ord("a")

    assert it.next_range()
    assert it.is_string()
    assert it.get_string() == "ab"

    assert not it.next_range()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_skip_to_strings():
    uniset = UnicodeSet("[a0-9{ab}]")
    it = UnicodeSetIterator(uniset)

    # UnicodeSetIterator &icu::UnicodeSetIterator::skipToStrings()
    result = it.skip_to_strings()
    assert isinstance(result, UnicodeSetIterator)
    assert id(result) == id(it)

    assert it.next()
    assert it.is_string()
    assert it.get_string() == "ab"

    assert not it.next()

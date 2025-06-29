import pytest

from icupy import icu


def test_next() -> None:
    # [1]
    # icu::UnicodeSetIterator::UnicodeSetIterator(const UnicodeSet &set)
    uniset = icu.UnicodeSet("[a\\U0001abcd{ab}]")
    it = icu.UnicodeSetIterator(uniset)

    # UBool icu::UnicodeSetIterator::next()
    assert it.next() is True

    # UBool icu::UnicodeSetIterator::isString()
    assert it.is_string() is False

    # UChar32 icu::UnicodeSetIterator::getCodepoint()
    assert it.get_codepoint() == ord("a")

    # const UnicodeString &icu::UnicodeSetIterator::getString()
    result = it.get_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "a"

    assert it.next() is True
    assert it.is_string() is False
    assert it.get_codepoint() == 0x1ABCD
    assert it.get_string() == "\U0001abcd"

    assert it.next() is True
    assert it.is_string() is True
    assert it.get_string() == "ab"

    assert it.next() is False

    # [1]
    # void icu::UnicodeSetIterator::reset()
    it.reset()
    assert it.next() is True
    assert it.is_string() is False
    assert it.get_codepoint() == ord("a")
    assert it.get_string() == "a"


def test_next_range() -> None:
    # [2]
    # icu::UnicodeSetIterator::UnicodeSetIterator()
    it = icu.UnicodeSetIterator()

    # UBool icu::UnicodeSetIterator::nextRange()
    assert it.next_range() is False

    # [2]
    # void icu::UnicodeSetIterator::reset(const UnicodeSet &set)
    uniset = icu.UnicodeSet("[a0-9{ab}]")
    it.reset(uniset)

    assert it.next_range() is True
    assert it.is_string() is False
    assert it.get_codepoint() == ord("0")

    # UChar32 icu::UnicodeSetIterator::getCodepointEnd()
    assert it.get_codepoint_end() == ord("9")

    assert it.next_range() is True
    assert it.is_string() is False
    assert it.get_codepoint() == ord("a")
    assert it.get_codepoint_end() == ord("a")

    assert it.next_range() is True
    assert it.is_string() is True
    assert it.get_string() == "ab"

    assert it.next_range() is False


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_skip_to_strings() -> None:
    uniset = icu.UnicodeSet("[a0-9{ab}]")
    it = icu.UnicodeSetIterator(uniset)

    # UnicodeSetIterator &icu::UnicodeSetIterator::skipToStrings()
    result = it.skip_to_strings()
    assert isinstance(result, icu.UnicodeSetIterator)
    assert id(result) == id(it)

    assert it.next()
    assert it.is_string()
    assert it.get_string() == "ab"

    assert not it.next()

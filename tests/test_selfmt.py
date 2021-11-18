import copy

import pytest
from icupy.icu import (
    FieldPosition, FieldPositionIterator, Formattable, ICUError,
    ParsePosition, SelectFormat, UErrorCode, UnicodeString,
)


def test_api():
    # From icu/source/test/intltest/selfmts.cpp
    fmt = SelectFormat("feminine {feminineVerbValue} other{otherVerbValue}")

    # void icu::SelectFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    pattern = UnicodeString(
        "masculine{masculineVerbValue} other{otherVerbValue}")
    fmt.apply_pattern(pattern)
    fmt.apply_pattern(str(pattern))

    # UnicodeString &icu::SelectFormat::toPattern(UnicodeString &appendTo)
    append_to = UnicodeString()
    result = fmt.to_pattern(append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == pattern

    # UBool icu::SelectFormat::operator!=(const Format &other)
    fmt1 = SelectFormat("odd{foo} other{bar}")
    fmt2 = SelectFormat("odd{foo} other{bar}")
    fmt3 = SelectFormat("odd{foo} other{bar2}")
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert fmt2 != fmt3

    # UBool icu::SelectFormat::operator==(const Format &other)
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)


def test_clone():
    fmt1 = SelectFormat("feminine {feminineVerbValue} other{otherVerbValue}")

    # SelectFormat *icu::SelectFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, SelectFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format():
    fmt = SelectFormat("feminine {feminineVerbValue} other{otherVerbValue}")
    append_to = UnicodeString()

    # [1], [2]
    # UnicodeString &icu::SelectFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(
        Formattable(UnicodeString("feminine")),
        append_to,
        pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "feminineVerbValue"

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(
            Formattable(UnicodeString("feminine")),
            append_to,
            pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(
            Formattable(UnicodeString("feminine")),
            append_to,
            None)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(Formattable(UnicodeString("feminine")), append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "feminineVerbValue"

    # [5]
    # UnicodeString &icu::SelectFormat::format(
    #       const UnicodeString &keyword,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(UnicodeString("feminine"), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "feminineVerbValue"

    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format("feminine", append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "feminineVerbValue"


def test_parse_object():
    fmt = SelectFormat("feminine {feminineVerbValue} other{otherVerbValue}")

    # void icu::SelectFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = Formattable()
    parse_pos = ParsePosition()
    fmt.parse_object(UnicodeString("feminineVerbValue"), result, parse_pos)
    assert parse_pos.get_index() == 0
    assert parse_pos.get_error_index() != -1

    result = Formattable()
    parse_pos = ParsePosition()
    fmt.parse_object("feminineVerbValue", result, parse_pos)
    assert parse_pos.get_index() == 0
    assert parse_pos.get_error_index() != -1

    # *U_INVALID_FORMAT_ERROR in ICU 69*
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    with pytest.raises(ICUError) as exc_info:
        fmt.parse_object(UnicodeString("feminineVerbValue"), result)
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    result = Formattable()
    with pytest.raises(ICUError) as exc_info:
        fmt.parse_object("feminineVerbValue", result)
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR


def test_select_format():
    pattern = UnicodeString(
        "feminine {feminineVerbValue} other{otherVerbValue}")

    # [1]
    # icu::SelectFormat::SelectFormat(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt1 = SelectFormat(pattern)

    fmt1a = SelectFormat(str(pattern))
    assert fmt1 == fmt1a

    # [2]
    # icu::SelectFormat::SelectFormat(const SelectFormat &other)
    fmt2 = SelectFormat(fmt1)
    assert fmt1 == fmt2

import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 51:
    pytest.skip("ICU4C<51", allow_module_level=True)

import copy

from icupy.icu import (
    CompactDecimalFormat, DecimalFormat, FieldPosition, FieldPositionIterator,
    Formattable, ICUError, INT32_MAX, INT64_MAX, Locale, ParsePosition,
    UErrorCode, UNumberCompactStyle, UnicodeString,
)


def test_api():
    assert issubclass(CompactDecimalFormat, DecimalFormat)

    # static CompactDecimalFormat *icu::CompactDecimalFormat::createInstance(
    #       const Locale &inLocale,
    #       UNumberCompactStyle style,
    #       UErrorCode &status
    # )
    fmt = CompactDecimalFormat.create_instance(
        Locale.get_us(),
        UNumberCompactStyle.UNUM_SHORT)
    assert isinstance(fmt, CompactDecimalFormat)

    fmt2 = CompactDecimalFormat.create_instance(
        "en-US",
        UNumberCompactStyle.UNUM_SHORT)
    assert isinstance(fmt2, CompactDecimalFormat)
    assert fmt == fmt2

    # icu::CompactDecimalFormat::CompactDecimalFormat(
    #       const CompactDecimalFormat &source
    # )
    fmt3 = CompactDecimalFormat(fmt)
    assert fmt == fmt3


def test_clone():
    fmt1 = CompactDecimalFormat.create_instance(
        Locale.get_us(),
        UNumberCompactStyle.UNUM_SHORT)

    # CompactDecimalFormat *icu::CompactDecimalFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, CompactDecimalFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format():
    fmt = CompactDecimalFormat.create_instance(
        Locale.get_us(),
        UNumberCompactStyle.UNUM_SHORT)
    d = -10456.0037
    s = "-1.045600e+04"
    n32 = INT32_MAX
    n64 = INT64_MAX
    append_to = UnicodeString()

    # [3]
    # UnicodeString &icu::DecimalFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(d, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # [5]
    # UnicodeString &icu::DecimalFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(d, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    append_to.remove()
    result = fmt.format(d, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # [6]
    # UnicodeString &icu::DecimalFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n32, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    # [8]
    # UnicodeString &icu::DecimalFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(n32, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    append_to.remove()
    result = fmt.format(n32, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    # [9]
    # UnicodeString &icu::DecimalFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n64, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # [11]
    # UnicodeString &icu::DecimalFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(n64, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # [12]
    # UnicodeString &icu::DecimalFormat::format(
    #       StringPiece number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(s, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    append_to.remove()
    result = fmt.format(s, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(Formattable(n64), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(Formattable(n64), append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(d, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n32, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n64, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(Formattable(d), append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"


def test_parse():
    fmt = CompactDecimalFormat.create_instance(
        Locale.get_us(),
        UNumberCompactStyle.UNUM_SHORT)
    # n = 1.2e+15
    s = "1200T"

    # *No effect in ICU 69*
    # [1]
    # void icu::CompactDecimalFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       ParsePosition &parsePosition
    # )
    result = Formattable()
    parse_position = ParsePosition()
    fmt.parse(UnicodeString(s), result, parse_position)
    assert parse_position.get_index() == 0

    result = Formattable()
    parse_position = ParsePosition()
    fmt.parse(s, result, parse_position)
    assert parse_position.get_index() == 0

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [2]
    # void icu::CompactDecimalFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    with pytest.raises(ICUError) as exc_info:
        fmt.parse(UnicodeString(s), result)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    result = Formattable()
    with pytest.raises(ICUError) as exc_info:
        fmt.parse(s, result)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

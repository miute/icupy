import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 51:
    pytest.skip("ICU4C<51", allow_module_level=True)

import copy


def test_api():
    assert issubclass(icu.CompactDecimalFormat, icu.DecimalFormat)

    # static CompactDecimalFormat *icu::CompactDecimalFormat::createInstance(
    #       const Locale &inLocale,
    #       UNumberCompactStyle style,
    #       UErrorCode &status
    # )
    fmt = icu.CompactDecimalFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberCompactStyle.UNUM_SHORT
    )
    assert isinstance(fmt, icu.CompactDecimalFormat)

    fmt2 = icu.CompactDecimalFormat.create_instance("en-US", icu.UNumberCompactStyle.UNUM_SHORT)
    assert isinstance(fmt2, icu.CompactDecimalFormat)
    assert fmt == fmt2

    # icu::CompactDecimalFormat::CompactDecimalFormat(
    #       const CompactDecimalFormat &source
    # )
    fmt3 = icu.CompactDecimalFormat(fmt)
    assert fmt == fmt3


def test_clone():
    fmt1 = icu.CompactDecimalFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberCompactStyle.UNUM_SHORT
    )

    # CompactDecimalFormat *icu::CompactDecimalFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.CompactDecimalFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format():
    fmt = icu.CompactDecimalFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberCompactStyle.UNUM_SHORT
    )
    d = -10456.0037
    s = "-1.045600e+04"
    n32 = icu.INT32_MAX
    n64 = icu.INT64_MAX
    append_to = icu.UnicodeString()

    # [3]
    # UnicodeString &icu::DecimalFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(d, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
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
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(d, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    append_to.remove()
    result = fmt.format(d, append_to, None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # [6]
    # UnicodeString &icu::DecimalFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n32, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
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
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(n32, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    append_to.remove()
    result = fmt.format(n32, append_to, None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    # [9]
    # UnicodeString &icu::DecimalFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n64, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
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
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(n64, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
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
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(s, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    append_to.remove()
    result = fmt.format(s, append_to, None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(icu.Formattable(n64), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(icu.Formattable(n64), append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(d, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"

    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n32, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2.1B"

    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n64, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372T"

    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(icu.Formattable(d), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "-10K"


def test_parse():
    fmt = icu.CompactDecimalFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberCompactStyle.UNUM_SHORT
    )
    # n = 1.2e+15
    s = "1200T"

    # *No effect in ICU 69*
    # [1]
    # void icu::CompactDecimalFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       ParsePosition &parsePosition
    # )
    result = icu.Formattable()
    parse_position = icu.ParsePosition()
    fmt.parse(icu.UnicodeString(s), result, parse_position)
    assert parse_position.get_index() == 0

    result = icu.Formattable()
    parse_position = icu.ParsePosition()
    fmt.parse(s, result, parse_position)
    assert parse_position.get_index() == 0

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [2]
    # void icu::CompactDecimalFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse(icu.UnicodeString(s), result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse(s, result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR


def test_parse_object():
    fmt = icu.CompactDecimalFormat.create_instance(
        icu.Locale.get_us(), icu.UNumberCompactStyle.UNUM_SHORT
    )
    fmt.set_maximum_significant_digits(20)
    # n = 1.2e+15
    s = "-10K"

    # *No effect in ICU 69+*
    # void icu::NumberFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt.parse_object(icu.UnicodeString(s), result, parse_pos)
    assert parse_pos.get_index() == 0

    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt.parse_object(s, result, parse_pos)
    assert parse_pos.get_index() == 0

    # *U_INVALID_FORMAT_ERROR in ICU 69+*
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse_object(icu.UnicodeString(s), result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse_object(s, result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

import copy
import sys

import pytest

# fmt: off
from icupy.icu import (
    U_ICU_VERSION_MAJOR_NUM, DateFormat, ErrorCode, FieldPosition,
    FieldPositionIterator, GregorianCalendar, ICUError, Locale, MessagePattern,
    ParsePosition, TimeZone, UErrorCode, UnicodeString, UParseError, u_failure,
    u_get_version, u_success, u_version_from_string, u_version_to_string,
)

# fmt: on


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_constrained_field_position():
    # fmt: off
    from icupy.icu import (
        ConstrainedFieldPosition, ListFormatter, UFieldCategory,
    )

    # fmt: on
    # icu::ConstrainedFieldPosition
    cfpos = ConstrainedFieldPosition()

    # ConstrainedFieldPosition.__repr__() -> str
    fmt = ListFormatter.create_instance(Locale.get_english())
    items = [
        UnicodeString("hello"),
        UnicodeString("wonderful"),
        UnicodeString("world"),
    ]
    fl = fmt.format_strings_to_value(items, len(items))
    assert fl.next_position(cfpos) is True
    assert repr(cfpos) == (
        "<ConstrainedFieldPosition("
        "category=0x1003, field=0, start=0, limit=5"
        ")>"
    )

    # int32_t icu::ConstrainedFieldPosition::getCategory() const
    category = (
        UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
        | UFieldCategory.UFIELD_CATEGORY_LIST
    )
    assert cfpos.get_category() == category

    # int32_t icu::ConstrainedFieldPosition::getField() const
    assert cfpos.get_field() == 0

    # int32_t icu::ConstrainedFieldPosition::getLimit() const
    assert cfpos.get_limit() == 5

    # int32_t icu::ConstrainedFieldPosition::getStart() const
    assert cfpos.get_start() == 0

    # void icu::ConstrainedFieldPosition::reset()
    cfpos.reset()
    assert cfpos.get_field() == 0
    assert cfpos.get_limit() == 0
    assert cfpos.get_start() == 0

    # UBool icu::ConstrainedFieldPosition::matchesField(
    #   	int32_t category,
    #       int32_t field
    # ) const
    assert cfpos.matches_field(0, 0) is True

    # void icu::ConstrainedFieldPosition::constrainCategory(int32_t category)
    category = UFieldCategory.UFIELD_CATEGORY_NUMBER
    cfpos.constrain_category(category)
    assert cfpos.matches_field(0, 0) is False
    assert cfpos.matches_field(category, 0) is True

    # void icu::ConstrainedFieldPosition::constrainField(
    #   	int32_t category,
    #       int32_t field
    # )
    category = UFieldCategory.UFIELD_CATEGORY_NUMBER
    cfpos.constrain_field(category, 1)
    assert cfpos.matches_field(category, 0) is False
    assert cfpos.matches_field(category, 1) is True


def test_field_position():
    # icu::FieldPosition
    # FieldPosition.__repr__() -> str
    fmt = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM, DateFormat.SHORT, Locale.get_english()
    )
    fmt.set_time_zone(TimeZone.get_gmt())
    append_to = UnicodeString()
    pos_iter = FieldPositionIterator()
    fmt.format(1215298800000, append_to, pos_iter)
    # -> 'Jul 5, 2008, 11:00 PM'
    fpos1 = FieldPosition()
    assert pos_iter.next(fpos1) is True  # 'Jul'
    assert (
        repr(fpos1) == "<FieldPosition(field=2, begin_index=0, end_index=3)>"
    )
    assert pos_iter.next(fpos1) is True  # '5'
    assert pos_iter.next(fpos1) is True  # '2008'
    assert pos_iter.next(fpos1) is True  # '11'
    assert pos_iter.next(fpos1) is True  # '00'
    assert pos_iter.next(fpos1) is True  # 'PM'
    assert pos_iter.next(fpos1) is False

    # FieldPosition *icu::FieldPosition::clone()
    # FieldPosition.__copy__() -> FieldPosition
    # FieldPosition.__deepcopy__(Optional[memo]) -> FieldPosition
    # FieldPosition.__eq__(other) -> bool
    # FieldPosition.__ne__(other) -> bool
    fpos2 = FieldPosition()
    fpos1a = fpos1.clone()
    assert isinstance(fpos1a, FieldPosition)
    fpos1b = copy.copy(fpos1)
    fpos1c = copy.deepcopy(fpos1)
    assert fpos1 == fpos1a == fpos1b == fpos1c
    assert fpos1 != fpos2


def test_icu_error():
    assert issubclass(ICUError, Exception)

    fmt = DateFormat.create_date_time_instance(
        DateFormat.SHORT, DateFormat.LONG, Locale.get_english()
    )
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.parse("07/10/96 4:5:0 PM, PDT")
    ex = exc_info.value
    assert isinstance(ex, ICUError)
    assert len(ex.args) == 1
    error_code = ex.args[0]
    assert isinstance(error_code, ErrorCode)

    # const char *icu::ErrorCode::errorName()
    error_name = error_code.error_name
    assert isinstance(error_name, str)
    assert error_name == "U_ILLEGAL_ARGUMENT_ERROR"

    # UErrorCode icu::ErrorCode::get()
    status = error_code.get()
    assert isinstance(status, UErrorCode)
    assert status == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # ErrorCode.__eq__(self, other: UErrorCode) -> bool
    assert error_code == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # UBool icu::ErrorCode::isFailure() const
    assert error_code.is_failure() is True

    # UBool icu::ErrorCode::isSuccess() const
    assert error_code.is_success() is False

    # UErrorCode icu::ErrorCode::reset()
    error_code.reset()
    assert error_code == UErrorCode.U_ZERO_ERROR
    assert error_code.is_failure() is False
    assert error_code.is_success() is True

    # void icu::ErrorCode::set(UErrorCode value)
    error_code.set(UErrorCode.U_MEMORY_ALLOCATION_ERROR)
    assert error_code == UErrorCode.U_MEMORY_ALLOCATION_ERROR
    assert error_code.is_failure() is True
    assert error_code.is_success() is False

    if U_ICU_VERSION_MAJOR_NUM >= 63:
        with pytest.raises(ICUError) as exc_info:
            _ = Locale.for_language_tag("x")
        ex = exc_info.value
        assert isinstance(ex, ICUError)
        assert len(ex.args) == 2
        assert isinstance(ex.args[0], ErrorCode)  # icu::ErrorCode
        assert isinstance(ex.args[1], str)  # An error message

    # ErrorCode.__repr__() -> str
    error_code = ex.args[0]
    assert repr(error_code) == "<ErrorCode(<U_ILLEGAL_ARGUMENT_ERROR: 1>)>"


@pytest.mark.skipif(
    not sys.platform.startswith("win"), reason="On Windows only"
)
def test_import():
    import importlib
    import os
    import tempfile

    import icupy

    old_value = os.environ.get("ICU_ROOT")
    try:
        with tempfile.TemporaryDirectory() as pathname:
            os.environ["ICU_ROOT"] = pathname
            with pytest.raises(FileNotFoundError):
                importlib.reload(icupy)
    finally:
        if old_value is not None:
            os.environ["ICU_ROOT"] = old_value
        else:
            del os.environ["ICU_ROOT"]


def test_parse_position():
    # icu::ParsePosition
    pos1 = ParsePosition()
    pos2 = ParsePosition(1)
    pos3 = ParsePosition(pos1)

    # bool icu::ParsePosition::operator!=(const ParsePosition &that)
    assert pos1 != pos2
    assert not (pos1 != pos3)
    assert pos2 != pos3

    # bool icu::ParsePosition::operator==(const ParsePosition &that)
    assert not (pos1 == pos2)
    assert pos1 == pos3
    assert not (pos2 == pos3)

    # ParsePosition *icu::ParsePosition::clone()
    # ParsePosition.__copy__() -> ParsePosition
    # ParsePosition.__deepcopy__(Optional[memo]) -> ParsePosition
    pos4 = pos1.clone()
    assert isinstance(pos4, ParsePosition)
    pos4a = copy.copy(pos1)
    pos4b = copy.deepcopy(pos1)
    assert pos1 == pos4 == pos4a == pos4b

    # void icu::ParsePosition::setErrorIndex(int32_t ei)
    assert pos1.get_error_index() == -1
    pos1.set_error_index(8)
    assert pos1.get_error_index() == 8

    # void icu::ParsePosition::setIndex(int32_t index)
    assert pos1.get_index() == 0
    pos1.set_index(4)
    assert pos1.get_index() == 4

    # ParsePosition.__repr__() -> str
    fmt = DateFormat.create_date_time_instance(
        DateFormat.SHORT, DateFormat.LONG, Locale.get_english()
    )
    zone = TimeZone.create_time_zone("PST")
    cal = GregorianCalendar(zone)
    cal.clear()
    pos = ParsePosition()
    fmt.parse(UnicodeString("07/10/96 4:5:0 PM PDT"), cal, pos)
    assert pos.get_error_index() == -1
    assert repr(pos) == "<ParsePosition(index=21, error_index=-1)>"

    pos = ParsePosition()
    fmt.parse(UnicodeString("07/10/96 4:5:0 PM JST"), cal, pos)
    assert pos.get_error_index() == 18
    assert repr(pos) == "<ParsePosition(index=0, error_index=18)>"


def test_u_failure():
    assert u_failure(UErrorCode.U_USING_FALLBACK_WARNING) is False
    assert u_failure(UErrorCode.U_ZERO_ERROR) is False
    assert u_failure(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR) is True


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_u_get_data_version():
    from icupy.icu import u_get_data_version

    # void u_getDataVersion(
    #       UVersionInfo dataVersionFillin,
    #       UErrorCode *status
    # )
    data_version = u_get_data_version()
    assert isinstance(data_version, tuple)
    assert len(data_version) == 4
    assert all(isinstance(x, int) for x in data_version)
    assert all(x >= 0 for x in data_version)


def test_u_get_version():
    # void u_getVersion(UVersionInfo versionArray)
    version_array = u_get_version()
    assert isinstance(version_array, tuple)
    assert len(version_array) == 4
    assert all(isinstance(x, int) for x in version_array)
    assert all(x >= 0 for x in version_array)


def test_u_success():
    assert u_success(UErrorCode.U_USING_FALLBACK_WARNING) is True
    assert u_success(UErrorCode.U_ZERO_ERROR) is True
    assert u_success(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR) is False


def test_u_version_from_string():
    # void u_versionFromString(
    #       UVersionInfo versionArray,
    #       const char *versionString
    # )
    version_array = u_version_from_string("123.45.67.89")
    assert isinstance(version_array, tuple)
    assert len(version_array) == 4
    assert version_array[0] == 123
    assert version_array[1] == 45
    assert version_array[2] == 67
    assert version_array[3] == 89


def test_u_version_to_string():
    # void u_versionToString(
    #       const UVersionInfo versionArray,
    #       char *versionString
    # )
    version_string = u_version_to_string([123, 45, 67, 89])
    assert isinstance(version_string, str)
    assert version_string == "123.45.67.89"

    version_string = u_version_to_string((123, 45, 67, 89))
    assert isinstance(version_string, str)
    assert version_string == "123.45.67.89"

    with pytest.raises(TypeError):
        _ = u_version_to_string([1, 2, 3])

    with pytest.raises(TypeError):
        _ = u_version_to_string([1, 2, 3, 4, 5])

    with pytest.raises(TypeError):
        _ = u_version_to_string([256, 0, 0, 0])  # [0, 255]


def test_uparse_error():
    # UParseError.__repr__() -> str
    pattern = UnicodeString("I see {many}")
    parse_error = UParseError()
    _ = MessagePattern(pattern, parse_error)
    assert parse_error.offset == 0
    assert repr(parse_error) == (
        "<UParseError(line=0, offset=0, pre_context='', post_context='')>"
    )

    pattern = UnicodeString("I see {'many'}")
    parse_error = UParseError()
    with pytest.raises(ICUError):
        _ = MessagePattern(pattern, parse_error)
    assert parse_error.offset == 7
    assert repr(parse_error) == (
        "<UParseError("
        "line=0, offset=7, pre_context='I see {', post_context='\\'many\\'}'"
        ")>"
    )

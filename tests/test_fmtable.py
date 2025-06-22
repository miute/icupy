import copy

import pytest

from icupy import icu


def test_api() -> None:
    fmt2 = icu.Formattable(1231027200000.0, icu.Formattable.IS_DATE)
    fmt3 = icu.Formattable(float(icu.INT32_MAX + 1))
    fmt4 = icu.Formattable(icu.INT32_MAX)
    fmt5 = icu.Formattable(icu.INT32_MAX + 1)
    fmt8 = icu.Formattable(icu.UnicodeString("1e-3"))
    array = [fmt2, fmt3, fmt4, fmt5, fmt8]
    fmt10 = icu.Formattable(array)

    # Type icu::Formattable::getType(void)
    assert fmt2.get_type() == icu.Formattable.DATE
    assert fmt3.get_type() == icu.Formattable.DOUBLE
    assert fmt4.get_type() == icu.Formattable.LONG
    assert fmt5.get_type() == icu.Formattable.INT64
    assert fmt8.get_type() == icu.Formattable.STRING
    assert fmt10.get_type() == icu.Formattable.ARRAY

    # [2]
    # const Formattable *icu::Formattable::getArray(
    #       int32_t &count,
    #       UErrorCode &status
    # )
    array2 = fmt10.get_array()
    assert isinstance(array2, list)
    assert array2 == array

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt2.get_array()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    # UDate icu::Formattable::getDate(UErrorCode &status)
    assert fmt2.get_date() == 1231027200000.0

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt3.get_date()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    # StringPiece icu::Formattable::getDecimalNumber(UErrorCode &status)
    result = fmt3.get_decimal_number()
    assert isinstance(result, str)
    assert result == "2.147483648E+9"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt2.get_decimal_number()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_STATE_ERROR

    # double icu::Formattable::getDouble(UErrorCode &status)
    assert fmt3.get_double() == icu.INT32_MAX + 1
    assert fmt4.get_double() == icu.INT32_MAX
    assert fmt5.get_double() == icu.INT32_MAX + 1

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt2.get_double()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    # int64_t icu::Formattable::getInt64(UErrorCode &status)
    assert fmt3.get_int64() == icu.INT32_MAX + 1
    assert fmt4.get_int64() == icu.INT32_MAX
    assert fmt5.get_int64() == icu.INT32_MAX + 1

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt2.get_int64()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    # int32_t icu::Formattable::getLong(UErrorCode &status)
    assert fmt4.get_long() == icu.INT32_MAX

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt5.get_long()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    # const UObject *icu::Formattable::getObject()
    assert fmt2.get_object() is None
    assert fmt10.get_object() is None

    # [1]
    # UnicodeString &icu::Formattable::getString(UErrorCode &status)
    result = fmt8.get_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "1e-3"

    # [4]
    # UnicodeString &icu::Formattable::getString(
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result = icu.UnicodeString()
    output = fmt8.get_string(result)
    assert isinstance(output, icu.UnicodeString)
    assert id(result) == id(output)
    assert output == result == "1e-3"

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt2.get_string()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    # UBool icu::Formattable::isNumeric()
    assert fmt2.is_numeric() is False
    assert fmt3.is_numeric() is True
    assert fmt4.is_numeric() is True
    assert fmt5.is_numeric() is True
    assert fmt8.is_numeric() is False
    assert fmt10.is_numeric() is False

    # UBool icu::Formattable::operator!=(const Formattable &other)
    assert fmt3 != fmt4
    assert fmt3 != fmt5

    # UBool icu::Formattable::operator==(const Formattable &other)
    assert not (fmt3 == fmt4)
    assert not (fmt3 == fmt5)

    # Formattable &icu::Formattable::operator[](int32_t index)
    assert fmt10[0] == fmt2
    assert fmt10[1] == fmt3
    assert fmt10[2] == fmt4
    assert fmt10[3] == fmt5
    assert fmt10[4] == fmt8
    with pytest.raises(IndexError):  # Non-standard
        _ = fmt10[5]
    assert fmt10[-1] == fmt8  # Non-standard

    with pytest.raises(icu.ICUError) as exc_info:  # Non-standard
        _ = fmt2[0]
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    fmt1 = icu.Formattable()

    # void icu::Formattable::setArray(
    #       const Formattable *array,
    #       int32_t count
    # )
    fmt1.set_array(array)
    assert fmt1.get_type() == icu.Formattable.ARRAY
    assert fmt1.get_array() == array
    assert fmt1 == fmt10

    # void icu::Formattable::setDate(UDate d)
    fmt1.set_date(1231027200000.0)
    assert fmt1.get_type() == icu.Formattable.DATE
    assert fmt1.get_date() == 1231027200000.0
    assert fmt1 == fmt2

    # void icu::Formattable::setDecimalNumber(
    #       StringPiece numberString,
    #       UErrorCode &status
    # )
    fmt1.set_decimal_number("1e-3")
    assert fmt1.get_type() == icu.Formattable.DOUBLE
    assert fmt1.get_double() == 1e-3

    # void icu::Formattable::setDouble(double d)
    fmt1.set_double(icu.INT32_MAX + 1)
    assert fmt1.get_type() == icu.Formattable.DOUBLE
    assert fmt1.get_double() == icu.INT32_MAX + 1
    assert fmt1 == fmt3

    # void icu::Formattable::setInt64(int64_t ll)
    fmt1.set_int64(icu.INT32_MAX + 1)
    assert fmt1.get_type() == icu.Formattable.INT64
    assert fmt1.get_int64() == icu.INT32_MAX + 1
    assert fmt1 == fmt5

    # void icu::Formattable::setLong(int32_t l)
    fmt1.set_long(icu.INT32_MAX)
    assert fmt1.get_type() == icu.Formattable.LONG
    assert fmt1.get_long() == icu.INT32_MAX
    assert fmt1 == fmt4

    # void icu::Formattable::setString(const UnicodeString &stringToCopy)
    fmt1.set_string(icu.UnicodeString("1e-3"))
    assert fmt1.get_type() == icu.Formattable.STRING
    assert fmt1.get_string() == "1e-3"
    assert fmt1 == fmt8

    fmt1.set_string("1e+3")
    assert fmt1.get_type() == icu.Formattable.STRING
    assert fmt1.get_string() == "1e+3"
    assert fmt1 != fmt8


def test_clone() -> None:
    fmt2 = icu.Formattable(1231027200000.0, icu.Formattable.IS_DATE)
    fmt3 = icu.Formattable(float(icu.INT32_MAX))
    fmt4 = icu.Formattable(icu.INT32_MAX)
    fmt5 = icu.Formattable(icu.INT32_MAX + 1)
    fmt8 = icu.Formattable(icu.UnicodeString("1e-3"))
    array = [fmt2, fmt3, fmt4, fmt5, fmt8]
    fmt10 = icu.Formattable(array)

    # Formattable *icu::Formattable::clone()
    test1 = fmt10.clone()
    assert isinstance(test1, icu.Formattable)
    assert test1.get_type() == icu.Formattable.ARRAY
    assert test1.get_array() == array

    test2 = copy.copy(fmt10)
    assert test2.get_type() == icu.Formattable.ARRAY
    assert test2.get_array() == array

    test3 = copy.deepcopy(fmt10)
    assert test3.get_type() == icu.Formattable.ARRAY
    assert test3.get_array() == array


def test_formattable() -> None:
    # [1]
    # icu::Formattable::Formattable()
    fmt1 = icu.Formattable()
    assert fmt1.get_type() == icu.Formattable.LONG
    assert fmt1.get_long() == 0

    # [2]
    # icu::Formattable::Formattable(
    #       UDate d,
    #       ISDATE flag
    # )
    fmt2 = icu.Formattable(1231027200000.0, icu.Formattable.IS_DATE)
    assert fmt2.get_type() == icu.Formattable.DATE
    assert fmt2.get_date() == 1231027200000.0

    # [3]
    # icu::Formattable::Formattable(double d)
    fmt3 = icu.Formattable(0.0)
    assert fmt3.get_type() == icu.Formattable.DOUBLE
    assert fmt3.get_double() == 0

    # [4]
    # icu::Formattable::Formattable(int32_t l)
    fmt4 = icu.Formattable(icu.INT32_MAX)
    assert fmt4.get_type() == icu.Formattable.LONG
    assert fmt4.get_long() == icu.INT32_MAX

    # [5]
    # icu::Formattable::Formattable(int64_t ll)
    fmt5 = icu.Formattable(icu.INT32_MAX + 1)
    assert fmt5.get_type() == icu.Formattable.INT64
    assert fmt5.get_int64() == icu.INT32_MAX + 1

    # [7]
    # icu::Formattable::Formattable(
    #       StringPiece number,
    #       UErrorCode &status
    # )
    fmt7 = icu.Formattable("1e-3")
    assert fmt7.get_type() == icu.Formattable.DOUBLE
    assert fmt7.get_double() == 1e-3

    # [8]
    # icu::Formattable::Formattable(const UnicodeString &strToCopy)
    fmt8 = icu.Formattable(icu.UnicodeString("1e-3"))
    assert fmt8.get_type() == icu.Formattable.STRING
    assert fmt8.get_string() == "1e-3"

    # [10]
    # icu::Formattable::Formattable(
    #       const Formattable *arrayToCopy,
    #       int32_t count
    # )
    array = [fmt2, fmt3, fmt4, fmt5, fmt7, fmt8]
    fmt10 = icu.Formattable(array, len(array))
    assert fmt10.get_type() == icu.Formattable.ARRAY
    assert fmt10.get_array() == array

    fmt10a = icu.Formattable([fmt10])
    assert fmt10a.get_type() == icu.Formattable.ARRAY
    assert fmt10a.get_array() == [fmt10]
    assert fmt10a[0].get_type() == icu.Formattable.ARRAY
    assert fmt10a[0].get_array() == array

    # [11]
    # icu::Formattable::Formattable(UObject *objectToAdopt)
    fmt11a = icu.Formattable(icu.Calendar.create_instance("en"))
    assert fmt11a.get_type() == icu.Formattable.OBJECT
    assert isinstance(fmt11a.get_object(), icu.Calendar)

    fmt11b = icu.Formattable(icu.CurrencyAmount(1, "JPY"))
    assert fmt11b.get_type() == icu.Formattable.OBJECT
    assert isinstance(fmt11b.get_object(), icu.CurrencyAmount)

    fmt11c = icu.Formattable(icu.DateInterval(0, 1))
    assert fmt11c.get_type() == icu.Formattable.OBJECT
    assert isinstance(fmt11c.get_object(), icu.DateInterval)

    fmt11d = icu.Formattable(icu.TimeUnitAmount(1, icu.TimeUnit.UTIMEUNIT_DAY))
    assert fmt11d.get_type() == icu.Formattable.OBJECT
    assert isinstance(fmt11d.get_object(), icu.TimeUnitAmount)

    fmt11e = icu.Formattable(icu.TimeZone.create_time_zone("JST"))
    assert fmt11e.get_type() == icu.Formattable.OBJECT
    assert isinstance(fmt11e.get_object(), icu.TimeZone)

    # [12]
    # icu::Formattable::Formattable(const Formattable &)
    fmt12 = icu.Formattable(fmt10)
    assert fmt12.get_type() == icu.Formattable.ARRAY
    assert fmt12.get_array() == array


def test_get_object_upcasting() -> None:
    fmt = icu.Formattable(icu.TimeZone.get_gmt())
    zone = fmt.get_object()
    assert isinstance(zone, icu.SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    fmt = icu.Formattable(icu.TimeZone.create_time_zone("JST"))
    zone = fmt.get_object()
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)

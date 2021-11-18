import copy

import pytest
from icupy.icu import (
    Calendar, CurrencyAmount, DateInterval, Formattable, ICUError,
    INT32_MAX, TimeUnit, TimeUnitAmount, TimeZone, UErrorCode, UnicodeString,
)


def test_api():
    fmt2 = Formattable(1231027200000.0, Formattable.IS_DATE)
    fmt3 = Formattable(float(INT32_MAX + 1))
    fmt4 = Formattable(INT32_MAX)
    fmt5 = Formattable(INT32_MAX + 1)
    fmt8 = Formattable(UnicodeString("1e-3"))
    array = [fmt2, fmt3, fmt4, fmt5, fmt8]
    fmt10 = Formattable(array)

    # Type icu::Formattable::getType(void)
    assert fmt2.get_type() == Formattable.DATE
    assert fmt3.get_type() == Formattable.DOUBLE
    assert fmt4.get_type() == Formattable.LONG
    assert fmt5.get_type() == Formattable.INT64
    assert fmt8.get_type() == Formattable.STRING
    assert fmt10.get_type() == Formattable.ARRAY

    # [2]
    # const Formattable *icu::Formattable::getArray(
    #       int32_t &count,
    #       UErrorCode &status
    # )
    array2 = fmt10.get_array()
    assert isinstance(array2, list)
    assert array2 == array

    with pytest.raises(ICUError) as exc_info:
        _ = fmt2.get_array()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # UDate icu::Formattable::getDate(UErrorCode &status)
    assert fmt2.get_date() == 1231027200000.0

    with pytest.raises(ICUError) as exc_info:
        _ = fmt3.get_date()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # StringPiece icu::Formattable::getDecimalNumber(UErrorCode &status)
    result = fmt3.get_decimal_number()
    assert isinstance(result, str)
    assert result == "2.147483648E+9"

    with pytest.raises(ICUError) as exc_info:
        _ = fmt2.get_decimal_number()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_STATE_ERROR

    # double icu::Formattable::getDouble(UErrorCode &status)
    assert fmt3.get_double() == INT32_MAX + 1
    assert fmt4.get_double() == INT32_MAX
    assert fmt5.get_double() == INT32_MAX + 1

    with pytest.raises(ICUError) as exc_info:
        _ = fmt2.get_double()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # int64_t icu::Formattable::getInt64(UErrorCode &status)
    assert fmt3.get_int64() == INT32_MAX + 1
    assert fmt4.get_int64() == INT32_MAX
    assert fmt5.get_int64() == INT32_MAX + 1

    with pytest.raises(ICUError) as exc_info:
        _ = fmt2.get_int64()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # int32_t icu::Formattable::getLong(UErrorCode &status)
    assert fmt4.get_long() == INT32_MAX

    with pytest.raises(ICUError) as exc_info:
        _ = fmt5.get_long()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # const UObject *icu::Formattable::getObject()
    assert fmt2.get_object() is None
    assert fmt10.get_object() is None

    # [1]
    # UnicodeString &icu::Formattable::getString(UErrorCode &status)
    result = fmt8.get_string()
    assert isinstance(result, UnicodeString)
    assert result == "1e-3"

    # [4]
    # UnicodeString &icu::Formattable::getString(
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result = UnicodeString()
    output = fmt8.get_string(result)
    assert isinstance(output, UnicodeString)
    assert id(result) == id(output)
    assert output == result == "1e-3"

    with pytest.raises(ICUError) as exc_info:
        _ = fmt2.get_string()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # UBool icu::Formattable::isNumeric()
    assert not fmt2.is_numeric()
    assert fmt3.is_numeric()
    assert fmt4.is_numeric()
    assert fmt5.is_numeric()
    assert not fmt8.is_numeric()
    assert not fmt10.is_numeric()

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

    with pytest.raises(ICUError) as exc_info:  # Non-standard
        _ = fmt2[0]
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    fmt1 = Formattable()

    # void icu::Formattable::setArray(
    #       const Formattable *array,
    #       int32_t count
    # )
    fmt1.set_array(array)
    assert fmt1.get_type() == Formattable.ARRAY
    assert fmt1.get_array() == array
    assert fmt1 == fmt10

    # void icu::Formattable::setDate(UDate d)
    fmt1.set_date(1231027200000.0)
    assert fmt1.get_type() == Formattable.DATE
    assert fmt1.get_date() == 1231027200000.0
    assert fmt1 == fmt2

    # void icu::Formattable::setDecimalNumber(
    #       StringPiece numberString,
    #       UErrorCode &status
    # )
    fmt1.set_decimal_number("1e-3")
    assert fmt1.get_type() == Formattable.DOUBLE
    assert fmt1.get_double() == 1e-3

    # void icu::Formattable::setDouble(double d)
    fmt1.set_double(INT32_MAX + 1)
    assert fmt1.get_type() == Formattable.DOUBLE
    assert fmt1.get_double() == INT32_MAX + 1
    assert fmt1 == fmt3

    # void icu::Formattable::setInt64(int64_t ll)
    fmt1.set_int64(INT32_MAX + 1)
    assert fmt1.get_type() == Formattable.INT64
    assert fmt1.get_int64() == INT32_MAX + 1
    assert fmt1 == fmt5

    # void icu::Formattable::setLong(int32_t l)
    fmt1.set_long(INT32_MAX)
    assert fmt1.get_type() == Formattable.LONG
    assert fmt1.get_long() == INT32_MAX
    assert fmt1 == fmt4

    # void icu::Formattable::setString(const UnicodeString &stringToCopy)
    fmt1.set_string(UnicodeString("1e-3"))
    assert fmt1.get_type() == Formattable.STRING
    assert fmt1.get_string() == "1e-3"
    assert fmt1 == fmt8

    fmt1.set_string("1e+3")
    assert fmt1.get_type() == Formattable.STRING
    assert fmt1.get_string() == "1e+3"
    assert fmt1 != fmt8


def test_clone():
    fmt2 = Formattable(1231027200000.0, Formattable.IS_DATE)
    fmt3 = Formattable(float(INT32_MAX))
    fmt4 = Formattable(INT32_MAX)
    fmt5 = Formattable(INT32_MAX + 1)
    fmt8 = Formattable(UnicodeString("1e-3"))
    array = [fmt2, fmt3, fmt4, fmt5, fmt8]
    fmt10 = Formattable(array)

    # Formattable *icu::Formattable::clone()
    test1 = fmt10.clone()
    assert isinstance(test1, Formattable)
    assert test1.get_type() == Formattable.ARRAY
    assert test1.get_array() == array

    test2 = copy.copy(fmt10)
    assert test2.get_type() == Formattable.ARRAY
    assert test2.get_array() == array

    test3 = copy.deepcopy(fmt10)
    assert test3.get_type() == Formattable.ARRAY
    assert test3.get_array() == array


def test_formattable():
    # [1]
    # icu::Formattable::Formattable()
    fmt1 = Formattable()
    assert fmt1.get_type() == Formattable.LONG
    assert fmt1.get_long() == 0

    # [2]
    # icu::Formattable::Formattable(
    #       UDate d,
    #       ISDATE flag
    # )
    fmt2 = Formattable(1231027200000.0, Formattable.IS_DATE)
    assert fmt2.get_type() == Formattable.DATE
    assert fmt2.get_date() == 1231027200000.0

    # [3]
    # icu::Formattable::Formattable(double d)
    fmt3 = Formattable(0.0)
    assert fmt3.get_type() == Formattable.DOUBLE
    assert fmt3.get_double() == 0

    # [4]
    # icu::Formattable::Formattable(int32_t l)
    fmt4 = Formattable(INT32_MAX)
    assert fmt4.get_type() == Formattable.LONG
    assert fmt4.get_long() == INT32_MAX

    # [5]
    # icu::Formattable::Formattable(int64_t ll)
    fmt5 = Formattable(INT32_MAX + 1)
    assert fmt5.get_type() == Formattable.INT64
    assert fmt5.get_int64() == INT32_MAX + 1

    # [7]
    # icu::Formattable::Formattable(
    #       StringPiece number,
    #       UErrorCode &status
    # )
    fmt7 = Formattable("1e-3")
    assert fmt7.get_type() == Formattable.DOUBLE
    assert fmt7.get_double() == 1e-3

    # [8]
    # icu::Formattable::Formattable(const UnicodeString &strToCopy)
    fmt8 = Formattable(UnicodeString("1e-3"))
    assert fmt8.get_type() == Formattable.STRING
    assert fmt8.get_string() == "1e-3"

    # [10]
    # icu::Formattable::Formattable(
    #       const Formattable *arrayToCopy,
    #       int32_t count
    # )
    array = [fmt2, fmt3, fmt4, fmt5, fmt7, fmt8]
    fmt10 = Formattable(array, len(array))
    assert fmt10.get_type() == Formattable.ARRAY
    assert fmt10.get_array() == array

    fmt10a = Formattable([fmt10])
    assert fmt10a.get_type() == Formattable.ARRAY
    assert fmt10a.get_array() == [fmt10]
    assert fmt10a[0].get_type() == Formattable.ARRAY
    assert fmt10a[0].get_array() == array

    # [11]
    # icu::Formattable::Formattable(UObject *objectToAdopt)
    fmt11a = Formattable(Calendar.create_instance("en"))
    assert fmt11a.get_type() == Formattable.OBJECT
    assert isinstance(fmt11a.get_object(), Calendar)

    fmt11b = Formattable(CurrencyAmount(1, "JPY"))
    assert fmt11b.get_type() == Formattable.OBJECT
    assert isinstance(fmt11b.get_object(), CurrencyAmount)

    fmt11c = Formattable(DateInterval(0, 1))
    assert fmt11c.get_type() == Formattable.OBJECT
    assert isinstance(fmt11c.get_object(), DateInterval)

    fmt11d = Formattable(TimeUnitAmount(1, TimeUnit.UTIMEUNIT_DAY))
    assert fmt11d.get_type() == Formattable.OBJECT
    assert isinstance(fmt11d.get_object(), TimeUnitAmount)

    fmt11e = Formattable(TimeZone.create_time_zone("JST"))
    assert fmt11e.get_type() == Formattable.OBJECT
    assert isinstance(fmt11e.get_object(), TimeZone)

    # [12]
    # icu::Formattable::Formattable(const Formattable &)
    fmt12 = Formattable(fmt10)
    assert fmt12.get_type() == Formattable.ARRAY
    assert fmt12.get_array() == array


def test_get_object_upcasting():
    from icupy.icu import BasicTimeZone, SimpleTimeZone

    fmt = Formattable(TimeZone.get_gmt())
    zone = fmt.get_object()
    assert isinstance(zone, SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    fmt = Formattable(TimeZone.create_time_zone("JST"))
    zone = fmt.get_object()
    assert not isinstance(zone, SimpleTimeZone)
    assert isinstance(zone, BasicTimeZone)

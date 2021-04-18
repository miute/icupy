import copy

import pytest
from icupy import (
    Formattable, ICUException, INT32_MAX, UErrorCode, UnicodeString,
)


def test_api():
    fmt2 = Formattable(1231027200000.0, Formattable.IS_DATE)
    fmt3 = Formattable(float(INT32_MAX + 1))
    fmt4 = Formattable(INT32_MAX)
    fmt5 = Formattable(INT32_MAX + 1)
    fmt8 = Formattable(UnicodeString("1e-3"))
    array = [fmt2, fmt3, fmt4, fmt5, fmt8]
    fmt10 = Formattable(array)

    # Formattable::getType
    assert fmt2.get_type() == Formattable.DATE
    assert fmt3.get_type() == Formattable.DOUBLE
    assert fmt4.get_type() == Formattable.LONG
    assert fmt5.get_type() == Formattable.INT64
    assert fmt8.get_type() == Formattable.STRING
    assert fmt10.get_type() == Formattable.ARRAY

    # Formattable::getArray
    array2 = fmt10.get_array()
    assert isinstance(array2, list)
    assert array2 == array

    with pytest.raises(ICUException) as exc_info:
        _ = fmt2.get_array()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # Formattable::getDate
    assert fmt2.get_date() == 1231027200000.0

    with pytest.raises(ICUException) as exc_info:
        _ = fmt3.get_date()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # Formattable::getDecimalNumber
    result = fmt3.get_decimal_number()
    assert isinstance(result, str)
    assert result == "2.147483648E+9"

    with pytest.raises(ICUException) as exc_info:
        _ = fmt2.get_decimal_number()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_STATE_ERROR

    # Formattable::getDouble
    assert fmt3.get_double() == INT32_MAX + 1
    assert fmt4.get_double() == INT32_MAX
    assert fmt5.get_double() == INT32_MAX + 1

    with pytest.raises(ICUException) as exc_info:
        _ = fmt2.get_double()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # Formattable::getInt64
    assert fmt3.get_int64() == INT32_MAX + 1
    assert fmt4.get_int64() == INT32_MAX
    assert fmt5.get_int64() == INT32_MAX + 1

    with pytest.raises(ICUException) as exc_info:
        _ = fmt2.get_int64()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # Formattable::getLong
    assert fmt4.get_long() == INT32_MAX

    with pytest.raises(ICUException) as exc_info:
        _ = fmt5.get_long()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # Formattable::getObject
    assert fmt2.get_object() is None
    assert fmt10.get_object() is None

    # Formattable::getString
    result = fmt8.get_string()
    assert isinstance(result, UnicodeString)
    assert result == "1e-3"

    result = UnicodeString()
    output = fmt8.get_string(result)
    assert isinstance(output, UnicodeString)
    assert output == result == "1e-3"

    with pytest.raises(ICUException) as exc_info:
        _ = fmt2.get_string()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    # Formattable::isNumeric
    assert not fmt2.is_numeric()
    assert fmt3.is_numeric()
    assert fmt4.is_numeric()
    assert fmt5.is_numeric()
    assert not fmt8.is_numeric()
    assert not fmt10.is_numeric()

    # Formattable::operator!=
    assert fmt3 != fmt4
    assert fmt3 != fmt5

    # Formattable::operator==
    assert not (fmt3 == fmt4)
    assert not (fmt3 == fmt5)

    # Formattable::operator[]
    assert fmt10[0] == fmt2
    assert fmt10[1] == fmt3
    assert fmt10[2] == fmt4
    assert fmt10[3] == fmt5
    assert fmt10[4] == fmt8
    with pytest.raises(IndexError):  # Non-standard
        _ = fmt10[5]
    assert fmt10[-1] == fmt8  # Non-standard

    with pytest.raises(ICUException) as exc_info:  # Non-standard
        _ = fmt2[0]
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    fmt1 = Formattable()

    # Formattable::setArray
    fmt1.set_array(array)
    assert fmt1.get_type() == Formattable.ARRAY
    assert fmt1.get_array() == array
    assert fmt1 == fmt10

    # Formattable::setDate
    fmt1.set_date(1231027200000.0)
    assert fmt1.get_type() == Formattable.DATE
    assert fmt1.get_date() == 1231027200000.0
    assert fmt1 == fmt2

    # Formattable::setDecimalNumber
    fmt1.set_decimal_number("1e-3")
    assert fmt1.get_type() == Formattable.DOUBLE
    assert fmt1.get_double() == 1e-3

    # Formattable::setDouble
    fmt1.set_double(INT32_MAX + 1)
    assert fmt1.get_type() == Formattable.DOUBLE
    assert fmt1.get_double() == INT32_MAX + 1
    assert fmt1 == fmt3

    # Formattable::setInt64
    fmt1.set_int64(INT32_MAX + 1)
    assert fmt1.get_type() == Formattable.INT64
    assert fmt1.get_int64() == INT32_MAX + 1
    assert fmt1 == fmt5

    # Formattable::setLong
    fmt1.set_long(INT32_MAX)
    assert fmt1.get_type() == Formattable.LONG
    assert fmt1.get_long() == INT32_MAX
    assert fmt1 == fmt4

    # Formattable::setString
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
    # Formattable::Formattable()
    fmt1 = Formattable()
    assert fmt1.get_type() == Formattable.LONG
    assert fmt1.get_long() == 0

    # [2]
    # Formattable::Formattable(UDate d,
    #                          ISDATE flag)
    fmt2 = Formattable(1231027200000.0, Formattable.IS_DATE)
    assert fmt2.get_type() == Formattable.DATE
    assert fmt2.get_date() == 1231027200000.0

    # [3]
    # Formattable::Formattable(double d)
    fmt3 = Formattable(0.0)
    assert fmt3.get_type() == Formattable.DOUBLE
    assert fmt3.get_double() == 0

    # [4]
    # Formattable::Formattable(int32_t l)
    fmt4 = Formattable(INT32_MAX)
    assert fmt4.get_type() == Formattable.LONG
    assert fmt4.get_long() == INT32_MAX

    # [5]
    # Formattable::Formattable(int64_t ll)
    fmt5 = Formattable(INT32_MAX + 1)
    assert fmt5.get_type() == Formattable.INT64
    assert fmt5.get_int64() == INT32_MAX + 1

    # [7]
    # Formattable::Formattable(StringPiece number,
    #                          UErrorCode &status
    # )
    fmt7 = Formattable("1e-3")
    assert fmt7.get_type() == Formattable.DOUBLE
    assert fmt7.get_double() == 1e-3

    # [8]
    # Formattable::Formattable(const UnicodeString &strToCopy)
    fmt8 = Formattable(UnicodeString("1e-3"))
    assert fmt8.get_type() == Formattable.STRING
    assert fmt8.get_string() == "1e-3"

    # [10]
    # Formattable::Formattable(const Formattable *arrayToCopy,
    #                          int32_t count
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
    # Formattable::Formattable(UObject *objectToAdopt)
    # obj = Locale("en-US")
    # fmt11 = Formattable(obj)
    # assert fmt11.get_type() == Formattable.OBJECT
    # assert fmt11.get_object() == obj

    # [12]
    # Formattable::Formattable(const Formattable &)
    fmt12 = Formattable(fmt10)
    assert fmt12.get_type() == Formattable.ARRAY
    assert fmt12.get_array() == array

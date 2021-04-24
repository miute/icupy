import copy

import pytest
from icupy import (
    Calendar, DateFormat, FieldPosition, FieldPositionIterator,
    Formattable, GregorianCalendar, ICUException, Locale, ParsePosition,
    TimeZone, UDateFormatField, UErrorCode, ULocDataLocaleType,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    # static const Locale *DateFormat::getAvailableLocales(int32_t &count)
    locales = DateFormat.get_available_locales()
    assert isinstance(locales, list)
    assert len(locales) > 0
    assert all(isinstance(x, Locale) for x in locales)

    zone = TimeZone.create_default()
    df = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT,
        Locale.get_english())
    assert isinstance(df, DateFormat)

    cal = df.get_calendar()
    assert isinstance(cal, Calendar)
    assert cal.get_time_zone() == zone

    loc = df.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, Locale)
    assert loc == Locale("en")

    zone2 = df.get_time_zone()
    assert isinstance(zone2, TimeZone)
    assert zone2 == zone

    assert df.is_lenient()

    # DateFormat::operator==()
    df2 = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT,
        Locale.get_english())
    df3 = DateFormat.create_date_time_instance(
        DateFormat.SHORT,
        DateFormat.LONG,
        Locale.get_english())
    assert df == df2
    assert not (df == df3)

    # Format::operator!=()
    assert not (df != df2)
    assert df != df3

    # [1]
    # void DateFormat::parse(const UnicodeString &text,
    #                        Calendar &cal,
    #                        ParsePosition &pos)
    zone2 = TimeZone.create_time_zone("PDT")
    cal2 = GregorianCalendar(zone2)
    cal2.clear()
    pos = ParsePosition(0)
    df3.parse(UnicodeString("07/10/96 4:5:0 PM PDT"), cal2, pos)
    assert pos.get_error_index() == -1
    assert cal2.get_time() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    cal2.clear()
    pos = ParsePosition(0)
    df3.parse("07/10/96 4:5:0 PM PDT", cal2, pos)
    assert pos.get_error_index() == -1
    assert cal2.get_time() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # [2]
    # UDate DateFormat::parse(const UnicodeString &text,
    #                         ParsePosition &pos)
    pos = ParsePosition(0)
    result = df3.parse(UnicodeString("07/10/96 4:5:0 PM PDT"), pos)
    assert pos.get_error_index() == -1
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    pos = ParsePosition(0)
    result = df3.parse("07/10/96 4:5:0 PM PDT", pos)
    assert pos.get_error_index() == -1
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # [3]
    # UDate DateFormat::parse(const UnicodeString &text,
    #                         UErrorCode &status)
    result = df3.parse(UnicodeString("07/10/96 4:5:0 PM PDT"))
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = df3.parse("07/10/96 4:5:0 PM PDT")
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    with pytest.raises(ICUException) as exc_info:
        _ = df3.parse("07/10/96 4:5:0 PM, PDT")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # void DateFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = Formattable()
    parse_pos = ParsePosition(0)
    df3.parse_object(UnicodeString("07/10/96 4:5:0 PM PDT"),
                     result,
                     parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = Formattable()
    parse_pos = ParsePosition(0)
    df3.parse_object("07/10/96 4:5:0 PM PDT",
                     result,
                     parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # void Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    df3.parse_object(UnicodeString("07/10/96 4:5:0 PM PDT"), result)
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = Formattable()
    df3.parse_object("07/10/96 4:5:0 PM PDT", result)
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = Formattable()
    with pytest.raises(ICUException) as exc_info:
        df3.parse_object("07/10/96 4:5:0 PM, PDT", result)
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    zone3 = TimeZone.create_time_zone("JST")
    cal3 = GregorianCalendar(zone3, Locale.get_japanese())
    df.set_calendar(cal3)
    assert df.get_calendar() == cal3

    df.set_lenient(False)
    assert not df.is_lenient()

    assert df.get_time_zone() == zone3
    df.set_time_zone(zone2)
    assert df.get_time_zone() == zone2


def test_clone():
    df = DateFormat.create_instance()

    test1 = df.clone()
    assert isinstance(test1, DateFormat)
    assert test1 == df

    test2 = copy.copy(df)
    assert test2 == df

    test3 = copy.deepcopy(df)
    assert test3 == df


def test_create_instance():
    # static DateFormat *DateFormat::createDateInstance(
    #       EStyle style = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    df10 = DateFormat.create_date_instance()
    assert isinstance(df10, DateFormat)

    df11 = DateFormat.create_date_instance(DateFormat.DEFAULT)
    df12 = DateFormat.create_date_instance(
        DateFormat.DEFAULT,
        Locale.get_default())
    assert df10 == df11 == df12

    # static DateFormat *DateFormat::createDateTimeInstance(
    #       EStyle dateStyle = kDefault,
    #       EStyle timeStyle = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    df20 = DateFormat.create_date_time_instance()
    assert isinstance(df20, DateFormat)

    df21 = DateFormat.create_date_time_instance(DateFormat.DEFAULT)
    df22 = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT)
    df23 = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT,
        Locale.get_default())
    assert df20 == df21 == df22 == df23

    # static DateFormat *DateFormat::createInstance(void)
    df30 = DateFormat.create_instance()
    assert isinstance(df30, DateFormat)

    df24 = DateFormat.create_date_time_instance(
        DateFormat.SHORT,
        DateFormat.SHORT,
        Locale.get_default())
    assert df30 == df24

    # static DateFormat *DateFormat::createTimeInstance(
    #       EStyle style = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    df40 = DateFormat.create_time_instance()
    assert isinstance(df40, DateFormat)

    df41 = DateFormat.create_time_instance(DateFormat.DEFAULT)
    df42 = DateFormat.create_time_instance(
        DateFormat.DEFAULT,
        Locale.get_default())
    assert df40 == df41 == df42


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_create_instance_for_skeleton():
    # [2]
    # static DateFormat *DateFormat::createInstanceForSkeleton(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    df20 = DateFormat.create_instance_for_skeleton(
        UnicodeString("yMMMMd"),
        Locale.get_english())
    assert isinstance(df20, DateFormat)

    df21 = DateFormat.create_instance_for_skeleton(
        "yMMMMd",
        Locale.get_english())
    assert df20 == df21

    # [3]
    # static DateFormat *DateFormat::createInstanceForSkeleton(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    df30 = DateFormat.create_instance_for_skeleton(UnicodeString("yMMMMd"))
    assert isinstance(df30, DateFormat)

    df31 = DateFormat.create_instance_for_skeleton("yMMMMd")
    assert df30 == df31


def test_format():
    date = 1215298800000.0  # 2008-07-05T23:00:00Z
    zone = TimeZone.get_gmt()
    cal = GregorianCalendar(zone)
    cal.set_time(date)
    df = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())
    df.set_time_zone(zone)

    # [1]
    # UnicodeString &DateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition
    # )
    append_to = UnicodeString()
    field_pos = FieldPosition(FieldPosition.DONT_CARE)
    result = df.format(cal, append_to, field_pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [2]
    # UnicodeString &DateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    pos_iter = FieldPositionIterator()
    result = df.format(cal, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_DATE_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_YEAR_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_HOUR1_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MINUTE_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_AM_PM_FIELD

    assert not pos_iter.next(fp)

    append_to = UnicodeString()
    result = df.format(cal, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [3]
    # UnicodeString &DateFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    obj = Formattable(date, Formattable.IS_DATE)
    append_to = UnicodeString()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = df.format(obj, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [6]
    # UnicodeString &DateFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    obj = Formattable(date, Formattable.IS_DATE)
    append_to = UnicodeString()
    pos_iter = FieldPositionIterator()
    result = df.format(obj, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    append_to = UnicodeString()
    result = df.format(obj, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [8]
    # UnicodeString &DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo
    # )
    append_to = UnicodeString()
    result = df.format(date, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [9]
    # UnicodeString &DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition
    # )
    append_to = UnicodeString()
    field_position = FieldPosition(FieldPosition.DONT_CARE)
    result = df.format(date, append_to, field_position)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [10]
    # UnicodeString &DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    pos_iter = FieldPositionIterator()
    result = df.format(date, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    append_to = UnicodeString()
    result = df.format(date, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [3]
    # UnicodeString &Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    obj = Formattable(date, Formattable.IS_DATE)
    append_to = UnicodeString()
    result = df.format(obj, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_boolean_attribute():
    from icupy import UDateFormatBooleanAttribute

    df = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())

    assert df.get_boolean_attribute(
        UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE)

    df.set_boolean_attribute(
        UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE,
        False)
    assert not df.get_boolean_attribute(
        UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_context():
    from icupy import UDisplayContext, UDisplayContextType

    df = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())

    assert (df.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_NONE)

    df.set_context(
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)
    assert (df.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_is_calendar_lenient():
    df = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())

    assert df.is_calendar_lenient()
    cal = df.get_calendar()
    assert cal.is_lenient()

    cal2 = GregorianCalendar()
    cal2.set_lenient(False)
    df.set_calendar(cal2)
    assert not df.is_calendar_lenient()

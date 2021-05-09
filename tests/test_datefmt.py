import copy

import pytest
from icupy import (
    Calendar, DateFormat, FieldPosition, FieldPositionIterator,
    Format, Formattable, GregorianCalendar, ICUException, Locale,
    ParsePosition, TimeZone, UDateFormatField, UErrorCode,
    ULocDataLocaleType, U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    assert issubclass(DateFormat, Format)

    # static const Locale *DateFormat::getAvailableLocales(int32_t &count)
    locales = DateFormat.get_available_locales()
    assert isinstance(locales, list)
    assert len(locales) > 0
    assert all(isinstance(x, Locale) for x in locales)

    zone = TimeZone.create_default()
    fmt = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT,
        Locale.get_english())
    fmt2 = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT,
        Locale.get_english())
    fmt3 = DateFormat.create_date_time_instance(
        DateFormat.SHORT,
        DateFormat.LONG,
        Locale.get_english())

    # Format::operator!=()
    assert not (fmt != fmt2)
    assert fmt != fmt3
    assert fmt2 != fmt3

    # DateFormat::operator==()
    assert fmt == fmt2
    assert not (fmt == fmt3)
    assert not (fmt2 == fmt3)

    # const Calendar *DateFormat::getCalendar(void)
    cal = fmt2.get_calendar()
    assert isinstance(cal, Calendar)
    assert cal.get_time_zone() == zone

    # void DateFormat::setCalendar(const Calendar &newCalendar)
    zone2 = TimeZone.create_time_zone("JST")
    assert zone2 != zone
    new_calendar = GregorianCalendar(zone2, Locale.get_japanese())
    fmt2.set_calendar(new_calendar)
    assert fmt2.get_calendar() == new_calendar

    # Locale Format::getLocale(ULocDataLocaleType type, UErrorCode &status)
    loc = fmt.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, Locale)
    assert loc == Locale("en")

    # const TimeZone &DateFormat::getTimeZone(void)
    zone3 = fmt2.get_time_zone()
    assert isinstance(zone3, TimeZone)
    assert zone3 == zone2

    # void DateFormat::setTimeZone(const TimeZone &zone)
    zone4 = TimeZone.create_time_zone("PST")
    assert zone4 != zone3
    fmt2.set_time_zone(zone4)
    assert fmt2.get_time_zone() == zone4

    # UBool DateFormat::isLenient(void)
    assert fmt2.is_lenient()

    # void DateFormat::setLenient(UBool lenient)
    fmt2.set_lenient(False)
    assert not fmt2.is_lenient()


def test_clone():
    fmt1 = DateFormat.create_instance()

    fmt2 = fmt1.clone()
    assert isinstance(fmt2, DateFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_create_instance():
    # static DateFormat *DateFormat::createDateInstance(
    #       EStyle style = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    fmt1 = DateFormat.create_date_instance()
    assert isinstance(fmt1, DateFormat)

    fmt1a = DateFormat.create_date_instance(DateFormat.DEFAULT)
    fmt1b = DateFormat.create_date_instance(
        DateFormat.DEFAULT,
        Locale.get_default())
    assert fmt1 == fmt1a == fmt1b

    # static DateFormat *DateFormat::createDateTimeInstance(
    #       EStyle dateStyle = kDefault,
    #       EStyle timeStyle = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    fmt2 = DateFormat.create_date_time_instance()
    assert isinstance(fmt2, DateFormat)

    fmt2a = DateFormat.create_date_time_instance(DateFormat.DEFAULT)
    fmt2b = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT)
    fmt2c = DateFormat.create_date_time_instance(
        DateFormat.DEFAULT,
        DateFormat.DEFAULT,
        Locale.get_default())
    assert fmt2 == fmt2a == fmt2b == fmt2c

    # static DateFormat *DateFormat::createInstance(void)
    fmt3 = DateFormat.create_instance()
    assert isinstance(fmt3, DateFormat)

    fmt2d = DateFormat.create_date_time_instance(
        DateFormat.SHORT,
        DateFormat.SHORT,
        Locale.get_default())
    assert fmt3 == fmt2d

    # static DateFormat *DateFormat::createTimeInstance(
    #       EStyle style = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    fmt4 = DateFormat.create_time_instance()
    assert isinstance(fmt4, DateFormat)

    fmt4a = DateFormat.create_time_instance(DateFormat.DEFAULT)
    fmt4b = DateFormat.create_time_instance(
        DateFormat.DEFAULT,
        Locale.get_default())
    assert fmt4 == fmt4a == fmt4b


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_create_instance_for_skeleton():
    # [2]
    # static DateFormat *DateFormat::createInstanceForSkeleton(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt2 = DateFormat.create_instance_for_skeleton(
        UnicodeString("yMMMMd"),
        Locale.get_english())
    assert isinstance(fmt2, DateFormat)

    fmt2a = DateFormat.create_instance_for_skeleton(
        "yMMMMd",
        Locale.get_english())
    assert fmt2 == fmt2a

    # [3]
    # static DateFormat *DateFormat::createInstanceForSkeleton(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    fmt3 = DateFormat.create_instance_for_skeleton(UnicodeString("yMMMMd"))
    assert isinstance(fmt3, DateFormat)

    fmt3a = DateFormat.create_instance_for_skeleton("yMMMMd")
    assert fmt3 == fmt3a


def test_format():
    date = 1215298800000.0  # 2008-07-05T23:00:00Z
    zone = TimeZone.get_gmt()
    cal = GregorianCalendar(zone)
    cal.set_time(date)
    fmt = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())
    fmt.set_time_zone(zone)

    # [1]
    # UnicodeString &DateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition
    # )
    append_to = UnicodeString()
    field_position = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(cal, append_to, field_position)
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
    result = fmt.format(cal, append_to, pos_iter)
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

    append_to.remove()
    result = fmt.format(cal, append_to, None)
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
    result = fmt.format(obj, append_to, pos)
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
    result = fmt.format(obj, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    append_to.remove()
    result = fmt.format(obj, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    # [8]
    # UnicodeString &DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo
    # )
    append_to = UnicodeString()
    result = fmt.format(date, append_to)
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
    result = fmt.format(date, append_to, field_position)
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
    result = fmt.format(date, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    append_to.remove()
    result = fmt.format(date, append_to, None)
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
    result = fmt.format(obj, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Jul 5, 2008, 11:00 PM"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_boolean_attribute():
    from icupy import UDateFormatBooleanAttribute

    fmt = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())

    # UBool DateFormat::getBooleanAttribute(
    #       UDateFormatBooleanAttribute attr,
    #       UErrorCode &status
    # )
    assert fmt.get_boolean_attribute(
        UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE)

    # DateFormat &DateFormat::setBooleanAttribute(
    #       UDateFormatBooleanAttribute attr,
    #       UBool newvalue,
    #       UErrorCode &status
    # )
    result = fmt.set_boolean_attribute(
        UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE,
        False)
    assert isinstance(result, DateFormat)
    assert id(result) == id(fmt)
    assert not fmt.get_boolean_attribute(
        UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_context():
    from icupy import UDisplayContext, UDisplayContextType

    fmt = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())

    # UDisplayContext DateFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_NONE)

    # void DateFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_is_calendar_lenient():
    fmt = DateFormat.create_date_time_instance(
        DateFormat.MEDIUM,
        DateFormat.SHORT,
        Locale.get_english())

    # UBool DateFormat::isCalendarLenient(void)
    assert fmt.is_calendar_lenient()

    cal = fmt.get_calendar()
    assert cal.is_lenient()

    # void DateFormat::setCalendarLenient(UBool lenient)
    fmt.set_calendar_lenient(False)
    assert not fmt.is_calendar_lenient()
    assert not cal.is_lenient()


def test_parse():
    fmt = DateFormat.create_date_time_instance(
        DateFormat.SHORT,
        DateFormat.LONG,
        Locale.get_english())

    # [1]
    # void DateFormat::parse(const UnicodeString &text,
    #                        Calendar &cal,
    #                        ParsePosition &pos)
    zone = TimeZone.create_time_zone("PST")
    cal = GregorianCalendar(zone)
    cal.clear()
    pos = ParsePosition(0)
    fmt.parse(UnicodeString("07/10/96 4:5:0 PM PDT"), cal, pos)
    assert pos.get_error_index() == -1
    assert cal.get_time() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    cal.clear()
    pos = ParsePosition(0)
    fmt.parse("07/10/96 4:5:0 PM PDT", cal, pos)
    assert pos.get_error_index() == -1
    assert cal.get_time() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # [2]
    # UDate DateFormat::parse(const UnicodeString &text,
    #                         ParsePosition &pos)
    pos = ParsePosition(0)
    result = fmt.parse(UnicodeString("07/10/96 4:5:0 PM PDT"), pos)
    assert pos.get_error_index() == -1
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    pos = ParsePosition(0)
    result = fmt.parse("07/10/96 4:5:0 PM PDT", pos)
    assert pos.get_error_index() == -1
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # [3]
    # UDate DateFormat::parse(const UnicodeString &text,
    #                         UErrorCode &status)
    result = fmt.parse(UnicodeString("07/10/96 4:5:0 PM PDT"))
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = fmt.parse("07/10/96 4:5:0 PM PDT")
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    with pytest.raises(ICUException) as exc_info:
        _ = fmt.parse("07/10/96 4:5:0 PM, PDT")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_parse_object():
    fmt = DateFormat.create_date_time_instance(
        DateFormat.SHORT,
        DateFormat.LONG,
        Locale.get_english())

    # void DateFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = Formattable()
    parse_pos = ParsePosition(0)
    fmt.parse_object(UnicodeString("07/10/96 4:5:0 PM PDT"),
                     result,
                     parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = Formattable()
    parse_pos = ParsePosition(0)
    fmt.parse_object("07/10/96 4:5:0 PM PDT",
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
    fmt.parse_object(UnicodeString("07/10/96 4:5:0 PM PDT"), result)
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = Formattable()
    fmt.parse_object("07/10/96 4:5:0 PM PDT", result)
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = Formattable()
    with pytest.raises(ICUException) as exc_info:
        fmt.parse_object("07/10/96 4:5:0 PM, PDT", result)
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

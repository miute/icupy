import copy

import pytest

from icupy import icu


def test_api() -> None:
    assert issubclass(icu.DateFormat, icu.Format)

    # static const Locale *icu::DateFormat::getAvailableLocales(int32_t &count)
    locales = icu.DateFormat.get_available_locales()
    assert isinstance(locales, list)
    assert len(locales) > 0
    assert all(isinstance(x, icu.Locale) for x in locales)

    zone = icu.TimeZone.create_default()
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.DEFAULT,
        icu.DateFormat.DEFAULT,
        icu.Locale.get_english(),
    )
    fmt2 = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.DEFAULT,
        icu.DateFormat.DEFAULT,
        icu.Locale.get_english(),
    )
    fmt3 = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.SHORT, icu.DateFormat.LONG, icu.Locale.get_english()
    )

    # UBool icu::Format::operator!=(const Format &)
    assert not (fmt != fmt2)
    assert fmt != fmt3
    assert fmt2 != fmt3

    # UBool icu::DateFormat::operator==(const Format &)
    assert fmt == fmt2
    assert not (fmt == fmt3)
    assert not (fmt2 == fmt3)

    # const Calendar *icu::DateFormat::getCalendar(void)
    cal = fmt2.get_calendar()
    assert isinstance(cal, icu.Calendar)
    assert cal.get_time_zone() == zone

    # void icu::DateFormat::setCalendar(const Calendar &newCalendar)
    zone2 = icu.TimeZone.create_time_zone("JST")
    assert zone2 != zone
    new_calendar = icu.GregorianCalendar(zone2, icu.Locale.get_japanese())
    fmt2.set_calendar(new_calendar)
    assert fmt2.get_calendar() == new_calendar

    # Locale icu::Format::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    loc = fmt.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert loc == icu.Locale("en")

    # const NumberFormat *icu::DateFormat::getNumberFormat(void)
    numfmt = fmt.get_number_format()
    assert isinstance(numfmt, icu.NumberFormat)

    # void icu::DateFormat::setNumberFormat(
    #       const NumberFormat &newNumberFormat
    # )
    new_number_format = icu.NumberFormat.create_instance()
    new_number_format.set_parse_integer_only(True)
    new_number_format.set_grouping_used(False)
    assert numfmt != new_number_format
    fmt.set_number_format(new_number_format)
    assert fmt.get_number_format() == new_number_format

    # const TimeZone &icu::DateFormat::getTimeZone(void)
    zone3 = fmt2.get_time_zone()
    assert isinstance(zone3, icu.TimeZone)
    assert zone3 == zone2

    # void icu::DateFormat::setTimeZone(const TimeZone &zone)
    zone4 = icu.TimeZone.create_time_zone("PST")
    assert zone4 != zone3
    fmt2.set_time_zone(zone4)
    assert fmt2.get_time_zone() == zone4

    # UBool icu::DateFormat::isLenient(void)
    assert fmt2.is_lenient() is True

    # void icu::DateFormat::setLenient(UBool lenient)
    fmt2.set_lenient(False)
    assert fmt2.is_lenient() is False


def test_clone() -> None:
    fmt1 = icu.DateFormat.create_instance()

    # DateFormat *icu::DateFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.DateFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_create_instance() -> None:
    # static DateFormat *icu::DateFormat::createDateInstance(
    #       EStyle style = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    fmt1 = icu.DateFormat.create_date_instance()
    assert isinstance(fmt1, icu.DateFormat)

    fmt1a = icu.DateFormat.create_date_instance(icu.DateFormat.DEFAULT)
    assert isinstance(fmt1a, icu.DateFormat)

    fmt1b = icu.DateFormat.create_date_instance(
        icu.DateFormat.DEFAULT, icu.Locale.get_default()
    )
    assert isinstance(fmt1b, icu.DateFormat)
    assert fmt1 == fmt1a == fmt1b

    # static DateFormat *icu::DateFormat::createDateTimeInstance(
    #       EStyle dateStyle = kDefault,
    #       EStyle timeStyle = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    fmt2 = icu.DateFormat.create_date_time_instance()
    assert isinstance(fmt2, icu.DateFormat)

    fmt2a = icu.DateFormat.create_date_time_instance(icu.DateFormat.DEFAULT)
    assert isinstance(fmt2a, icu.DateFormat)

    fmt2b = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.DEFAULT, icu.DateFormat.DEFAULT
    )
    assert isinstance(fmt2b, icu.DateFormat)

    fmt2c = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.DEFAULT,
        icu.DateFormat.DEFAULT,
        icu.Locale.get_default(),
    )
    assert isinstance(fmt2c, icu.DateFormat)
    assert fmt2 == fmt2a == fmt2b == fmt2c

    # static DateFormat *icu::DateFormat::createInstance(void)
    fmt3 = icu.DateFormat.create_instance()
    assert isinstance(fmt3, icu.DateFormat)

    fmt2d = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.SHORT, icu.DateFormat.SHORT, icu.Locale.get_default()
    )
    assert isinstance(fmt2d, icu.DateFormat)
    assert fmt3 == fmt2d

    # static DateFormat *icu::DateFormat::createTimeInstance(
    #       EStyle style = kDefault,
    #       const Locale &aLocale = Locale::getDefault()
    # )
    fmt4 = icu.DateFormat.create_time_instance()
    assert isinstance(fmt4, icu.DateFormat)

    fmt4a = icu.DateFormat.create_time_instance(icu.DateFormat.DEFAULT)
    assert isinstance(fmt4a, icu.DateFormat)

    fmt4b = icu.DateFormat.create_time_instance(
        icu.DateFormat.DEFAULT, icu.Locale.get_default()
    )
    assert isinstance(fmt4b, icu.DateFormat)
    assert fmt4 == fmt4a == fmt4b


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_create_instance_for_skeleton() -> None:
    # [2]
    # static DateFormat *icu::DateFormat::createInstanceForSkeleton(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt2 = icu.DateFormat.create_instance_for_skeleton(
        icu.UnicodeString("yMMMMd"), icu.Locale.get_english()
    )
    assert isinstance(fmt2, icu.DateFormat)

    fmt2a = icu.DateFormat.create_instance_for_skeleton("yMMMMd", icu.Locale.get_english())
    assert isinstance(fmt2a, icu.DateFormat)
    assert fmt2 == fmt2a

    # [3]
    # static DateFormat *icu::DateFormat::createInstanceForSkeleton(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    fmt3 = icu.DateFormat.create_instance_for_skeleton(icu.UnicodeString("yMMMMd"))
    assert isinstance(fmt3, icu.DateFormat)

    fmt3a = icu.DateFormat.create_instance_for_skeleton("yMMMMd")
    assert isinstance(fmt3a, icu.DateFormat)
    assert fmt3 == fmt3a


def test_format() -> None:
    date = 1215298800000.0  # 2008-07-05T23:00:00Z
    zone = icu.TimeZone.get_gmt()
    cal = icu.GregorianCalendar(zone)
    cal.set_time(date)
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.MEDIUM, icu.DateFormat.SHORT, icu.Locale.get_english()
    )
    fmt.set_time_zone(zone)

    # [1]
    # UnicodeString &icu::DateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition
    # )
    append_to = icu.UnicodeString()
    field_position = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(cal, append_to, field_position)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [2]
    # UnicodeString &icu::DateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(cal, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    fp = icu.FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_DATE_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_YEAR_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_HOUR1_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_MINUTE_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_AM_PM_FIELD

    assert not pos_iter.next(fp)

    append_to.remove()
    result = fmt.format(cal, append_to, None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [3]
    # UnicodeString &icu::DateFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(date, icu.Formattable.IS_DATE)
    append_to = icu.UnicodeString()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(obj, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [6]
    # UnicodeString &icu::DateFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(date, icu.Formattable.IS_DATE)
    append_to = icu.UnicodeString()
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(obj, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    fp = icu.FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    append_to.remove()
    result = fmt.format(obj, append_to, None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [8]
    # UnicodeString &icu::DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo
    # )
    append_to = icu.UnicodeString()
    result = fmt.format(date, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [9]
    # UnicodeString &icu::DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition
    # )
    append_to = icu.UnicodeString()
    field_position = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(date, append_to, field_position)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [10]
    # UnicodeString &icu::DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos_iter = icu.FieldPositionIterator()
    result = fmt.format(date, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    fp = icu.FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == icu.UDateFormatField.UDAT_MONTH_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 3

    append_to.remove()
    result = fmt.format(date, append_to, None)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )

    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(date, icu.Formattable.IS_DATE)
    append_to = icu.UnicodeString()
    result = fmt.format(obj, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Jul 5, 2008, 11:00 PM",
        "Jul 5, 2008, 11:00\u202fPM",  # ICU>=72
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_boolean_attribute() -> None:
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.MEDIUM, icu.DateFormat.SHORT, icu.Locale.get_english()
    )

    # UBool icu::DateFormat::getBooleanAttribute(
    #       UDateFormatBooleanAttribute attr,
    #       UErrorCode &status
    # )
    assert (
        fmt.get_boolean_attribute(icu.UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE)
        is True
    )

    # DateFormat &icu::DateFormat::setBooleanAttribute(
    #       UDateFormatBooleanAttribute attr,
    #       UBool newvalue,
    #       UErrorCode &status
    # )
    result = fmt.set_boolean_attribute(
        icu.UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE, False
    )
    assert isinstance(result, icu.DateFormat)
    assert id(result) == id(fmt)
    assert (
        fmt.get_boolean_attribute(icu.UDateFormatBooleanAttribute.UDAT_PARSE_ALLOW_WHITESPACE)
        is False
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_context() -> None:
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.MEDIUM, icu.DateFormat.SHORT, icu.Locale.get_english()
    )

    # UDisplayContext icu::DateFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (
        fmt.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
        == icu.UDisplayContext.UDISPCTX_CAPITALIZATION_NONE
    )

    # void icu::DateFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(icu.UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)
    assert (
        fmt.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
        == icu.UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE
    )


def test_get_time_zone_upcasting() -> None:
    fmt = icu.DateFormat.create_date_time_instance()

    fmt.set_time_zone(icu.TimeZone.get_gmt())
    zone = fmt.get_time_zone()
    assert isinstance(zone, icu.SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    fmt.set_time_zone(icu.TimeZone.create_time_zone("JST"))
    zone = fmt.get_time_zone()
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_is_calendar_lenient() -> None:
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.MEDIUM, icu.DateFormat.SHORT, icu.Locale.get_english()
    )

    # UBool icu::DateFormat::isCalendarLenient(void)
    assert fmt.is_calendar_lenient() is True

    cal = fmt.get_calendar()
    assert cal.is_lenient() is True

    # void icu::DateFormat::setCalendarLenient(UBool lenient)
    fmt.set_calendar_lenient(False)
    assert fmt.is_calendar_lenient() is False
    assert cal.is_lenient() is False


def test_parse() -> None:
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.SHORT, icu.DateFormat.LONG, icu.Locale.get_english()
    )

    # [1]
    # void icu::DateFormat::parse(
    #       const UnicodeString &text,
    #       Calendar &cal,
    #       ParsePosition &pos
    # )
    zone = icu.TimeZone.create_time_zone("PST")
    cal = icu.GregorianCalendar(zone)
    cal.clear()
    pos = icu.ParsePosition(0)
    fmt.parse(icu.UnicodeString("07/10/96 4:5:0 PM PDT"), cal, pos)
    assert pos.get_error_index() == -1
    assert cal.get_time() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    cal.clear()
    pos = icu.ParsePosition(0)
    fmt.parse("07/10/96 4:5:0 PM PDT", cal, pos)
    assert pos.get_error_index() == -1
    assert cal.get_time() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # [2]
    # UDate icu::DateFormat::parse(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = icu.ParsePosition(0)
    result = fmt.parse(icu.UnicodeString("07/10/96 4:5:0 PM PDT"), pos)
    assert pos.get_error_index() == -1
    assert isinstance(result, float)
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    pos = icu.ParsePosition(0)
    result = fmt.parse("07/10/96 4:5:0 PM PDT", pos)
    assert pos.get_error_index() == -1
    assert isinstance(result, float)
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # [3]
    # UDate icu::DateFormat::parse(
    #       const UnicodeString &text,
    #       UErrorCode &status
    # )
    result = fmt.parse(icu.UnicodeString("07/10/96 4:5:0 PM PDT"))
    assert isinstance(result, float)
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = fmt.parse("07/10/96 4:5:0 PM PDT")
    assert isinstance(result, float)
    assert result == 837039900000.0  # 1996-07-10T16:05:00-07:00

    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.parse("07/10/96 4:5:0 PM, PDT")
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_parse_object() -> None:
    fmt = icu.DateFormat.create_date_time_instance(
        icu.DateFormat.SHORT, icu.DateFormat.LONG, icu.Locale.get_english()
    )

    # void icu::DateFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = icu.Formattable()
    parse_pos = icu.ParsePosition(0)
    fmt.parse_object(icu.UnicodeString("07/10/96 4:5:0 PM PDT"), result, parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = icu.Formattable()
    parse_pos = icu.ParsePosition(0)
    fmt.parse_object("07/10/96 4:5:0 PM PDT", result, parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    fmt.parse_object(icu.UnicodeString("07/10/96 4:5:0 PM PDT"), result)
    assert result.get_type() == icu.Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = icu.Formattable()
    fmt.parse_object("07/10/96 4:5:0 PM PDT", result)
    assert result.get_type() == icu.Formattable.DATE
    assert result.get_date() == 837039900000.0  # 1996-07-10T16:05:00-07:00

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse_object("07/10/96 4:5:0 PM, PDT", result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

import copy

import pytest
from icupy.icu import (
    DateFormat, DateFormatSymbols, FieldPosition, FieldPositionIterator,
    Format, Formattable, GregorianCalendar, ICUError, Locale,
    ParsePosition, SimpleDateFormat, TimeZone, TimeZoneFormat,
    UCalendarMonths, UDateFormatField, UErrorCode, U_ICU_VERSION_MAJOR_NUM,
    UnicodeString,
)


def test_api():
    fmt1 = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        Locale.get_english())
    fmt2 = fmt1.clone()
    fmt3 = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        Locale.get_french())

    # UBool icu::Format::operator!=(const Format &other)
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert fmt2 != fmt3

    # UBool icu::SimpleDateFormat::operator==(const Format &other)
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)

    # UnicodeString &icu::SimpleDateFormat::toLocalizedPattern(
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result = UnicodeString()
    string = fmt2.to_localized_pattern(result)
    assert isinstance(string, UnicodeString)
    assert id(result) == id(string)
    assert result == "yyyy.MM.dd G 'at' HH:mm:ss z"

    # void icu::SimpleDateFormat::applyLocalizedPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2.apply_localized_pattern(UnicodeString("MMMMdHmm"))
    assert fmt2.to_localized_pattern(result) == "MMMMdHmm"

    fmt2.apply_localized_pattern("yyyy.MM.dd G 'at' HH:mm:ss z")
    assert fmt2.to_localized_pattern(result) == "yyyy.MM.dd G 'at' HH:mm:ss z"

    # UnicodeString &icu::SimpleDateFormat::toPattern(UnicodeString &result)
    string = fmt2.to_pattern(result)
    assert isinstance(string, UnicodeString)
    assert id(result) == id(string)
    assert result == "yyyy.MM.dd G 'at' HH:mm:ss z"

    # void icu::SimpleDateFormat::applyPattern(const UnicodeString &pattern)
    fmt2.apply_pattern(UnicodeString("MMMMdHmm"))
    assert fmt2.to_pattern(result) == "MMMMdHmm"

    fmt2.apply_pattern("yyyy.MM.dd G 'at' HH:mm:ss z")
    assert fmt2.to_pattern(result) == "yyyy.MM.dd G 'at' HH:mm:ss z"

    # UDate icu::SimpleDateFormat::get2DigitYearStart(UErrorCode &status)
    d = fmt2.get_2digit_year_start()
    assert isinstance(d, float)

    # void icu::SimpleDateFormat::set2DigitYearStart(
    #       UDate d,
    #       UErrorCode &status
    # )
    fmt2.set_2digit_year_start(d)

    # const DateFormatSymbols *
    # icu::SimpleDateFormat::getDateFormatSymbols(void)
    sym = fmt2.get_date_format_symbols()
    assert isinstance(sym, DateFormatSymbols)

    # void icu::SimpleDateFormat::setDateFormatSymbols(
    #       const DateFormatSymbols &newFormatSymbols
    # )
    new_format_symbols = DateFormatSymbols(Locale.get_french())
    assert new_format_symbols != sym
    fmt2.set_date_format_symbols(new_format_symbols)
    assert fmt2.get_date_format_symbols() == new_format_symbols


def test_clone():
    fmt1 = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        Locale.get_english())

    # SimpleDateFormat *icu::SimpleDateFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, SimpleDateFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_format():
    locale = Locale.get_english()
    tz = TimeZone.create_time_zone("PST")
    cal = GregorianCalendar(tz, locale)
    cal.set(1996, UCalendarMonths.UCAL_JULY, 10, 15, 8, 56)
    date = cal.get_time()
    obj = Formattable(date, Formattable.IS_DATE)
    fmt = SimpleDateFormat("yyyy.MM.dd G 'at' HH:mm:ss z", locale)
    fmt.set_time_zone(tz)

    # [2]
    # UnicodeString &icu::SimpleDateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos
    # )
    append_to = UnicodeString()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(cal, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [4]
    # UnicodeString &icu::SimpleDateFormat::format(
    #       Calendar &cal,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(cal, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_YEAR_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 4

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MONTH_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_DATE_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_ERA_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_HOUR_OF_DAY0_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_MINUTE_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_SECOND_FIELD

    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_TIMEZONE_FIELD

    assert not pos_iter.next(fp)

    append_to.remove()
    result = fmt.format(cal, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [5]
    # UnicodeString &icu::DateFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(obj, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [8]
    # UnicodeString &icu::DateFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(obj, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_YEAR_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 4

    append_to.remove()
    result = fmt.format(obj, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [9]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(obj, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [10]
    # UnicodeString &icu::DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(date, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [11]
    # UnicodeString &icu::DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition
    # )
    append_to.remove()
    field_position = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(date, append_to, field_position)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    # [12]
    # UnicodeString &icu::DateFormat::format(
    #       UDate date,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt.format(date, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"

    fp = FieldPosition()
    assert pos_iter.next(fp)
    assert fp.get_field() == UDateFormatField.UDAT_YEAR_FIELD
    assert fp.get_begin_index() == 0
    assert fp.get_end_index() == 4

    append_to.remove()
    result = fmt.format(date, append_to, None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1996.07.10 AD at 15:08:56 PDT"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_get_context():
    from icupy.icu import UDisplayContext, UDisplayContextType

    fmt = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        Locale.get_english())

    # UDisplayContext icu::DateFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_NONE)

    # void icu::SimpleDateFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_get_time_zone_format():
    fmt = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        Locale.get_english())

    # *ICU 49 technology preview*
    # const TimeZoneFormat *icu::SimpleDateFormat::getTimeZoneFormat(void)
    tzf = fmt.get_time_zone_format()
    assert isinstance(tzf, TimeZoneFormat)

    tzf2 = TimeZoneFormat.create_instance(Locale.get_french())
    assert tzf2 != tzf

    # *ICU 49 technology preview*
    # void icu::SimpleDateFormat::setTimeZoneFormat(
    #       const TimeZoneFormat &newTimeZoneFormat
    # )
    fmt.set_time_zone_format(tzf2)
    assert fmt.get_time_zone_format() == tzf2


def test_parse():
    locale = Locale.get_english()
    tz = TimeZone.create_time_zone("PST")
    fmt = SimpleDateFormat("yyyy.MM.dd G 'at' HH:mm:ss z", locale)
    date = 837036536000.0  # 1996-07-10T15:08:56-07:00

    # [1]
    # void icu::SimpleDateFormat::parse(
    #       const UnicodeString &text,
    #       Calendar &cal,
    #       ParsePosition &pos
    # )
    cal = GregorianCalendar(tz, locale)
    cal.clear()
    pos = ParsePosition(0)
    fmt.parse(UnicodeString("1996.07.10 AD at 15:08:56 PDT"), cal, pos)
    assert pos.get_error_index() == -1
    assert cal.get_time() == date

    cal.clear()
    pos = ParsePosition(0)
    fmt.parse("1996.07.10 AD at 15:08:56 PDT", cal, pos)
    assert pos.get_error_index() == -1
    assert cal.get_time() == date

    # [3]
    # UDate icu::DateFormat::parse(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = ParsePosition(0)
    result = fmt.parse(UnicodeString("1996.07.10 AD at 15:08:56 PDT"), pos)
    assert pos.get_error_index() == -1
    assert isinstance(result, float)
    assert result == date

    pos = ParsePosition(0)
    result = fmt.parse("1996.07.10 AD at 15:08:56 PDT", pos)
    assert pos.get_error_index() == -1
    assert isinstance(result, float)
    assert result == date

    # [4]
    # UDate icu::DateFormat::parse(
    #       const UnicodeString &text,
    #       UErrorCode &status
    # )
    result = fmt.parse(UnicodeString("1996.07.10 AD at 15:08:56 PDT"))
    assert isinstance(result, float)
    assert result == date

    result = fmt.parse("1996.07.10 AD at 15:08:56 PDT")
    assert isinstance(result, float)
    assert result == date

    with pytest.raises(ICUError) as exc_info:
        _ = fmt.parse("1996.07.10 AD at 15:08:56, PDT")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_parse_object():
    fmt = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        Locale.get_english())
    date = 837036536000.0  # 1996-07-10T15:08:56-07:00

    # void icu::DateFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = Formattable()
    parse_pos = ParsePosition(0)
    fmt.parse_object(
        UnicodeString("1996.07.10 AD at 15:08:56 PDT"),
        result,
        parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == date

    result = Formattable()
    parse_pos = ParsePosition(0)
    fmt.parse_object(
        "1996.07.10 AD at 15:08:56 PDT",
        result,
        parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == date

    # [2]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    fmt.parse_object(
        UnicodeString("1996.07.10 AD at 15:08:56 PDT"),
        result)
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == date

    result = Formattable()
    fmt.parse_object(
        "1996.07.10 AD at 15:08:56 PDT",
        result)
    assert result.get_type() == Formattable.DATE
    assert result.get_date() == date


def test_simple_date_format():
    assert issubclass(DateFormat, Format)
    assert issubclass(SimpleDateFormat, DateFormat)
    locale = Locale.get_english()

    # [1]
    # icu::SimpleDateFormat::SimpleDateFormat(UErrorCode &status)
    fmt1 = SimpleDateFormat()

    # [2]
    # icu::SimpleDateFormat::SimpleDateFormat(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2 = SimpleDateFormat(UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"))
    fmt2a = SimpleDateFormat("yyyy.MM.dd G 'at' HH:mm:ss z")
    assert fmt2 == fmt2a
    assert fmt2 != fmt1

    # [3]
    # icu::SimpleDateFormat::SimpleDateFormat(
    #       const UnicodeString &pattern,
    #       const UnicodeString &override,
    #       UErrorCode &status
    # )
    fmt3 = SimpleDateFormat(
        UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"),
        UnicodeString("y=hebr;d=thai;s=arab"))
    fmt3a = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        UnicodeString("y=hebr;d=thai;s=arab"))
    fmt3b = SimpleDateFormat(
        UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"),
        "y=hebr;d=thai;s=arab")
    fmt3c = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        "y=hebr;d=thai;s=arab")
    assert fmt3 == fmt3a == fmt3b == fmt3c
    assert fmt3 != fmt1

    # [4]
    # icu::SimpleDateFormat::SimpleDateFormat(
    #       const UnicodeString &pattern,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt4 = SimpleDateFormat(
        UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"),
        locale)
    fmt4a = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        locale)
    assert fmt4 == fmt4a
    assert fmt4 != fmt1

    # [5]
    # icu::SimpleDateFormat::SimpleDateFormat(
    #       const UnicodeString &pattern,
    #       const UnicodeString &override,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt5 = SimpleDateFormat(
        UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"),
        UnicodeString("y=hebr;d=thai;s=arab"),
        locale)
    fmt5a = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        UnicodeString("y=hebr;d=thai;s=arab"),
        locale)
    fmt5b = SimpleDateFormat(
        UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"),
        "y=hebr;d=thai;s=arab",
        locale)
    fmt5c = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        "y=hebr;d=thai;s=arab",
        locale)
    assert fmt5 == fmt5a == fmt5b == fmt5c
    assert fmt5 != fmt1

    # [7]
    # icu::SimpleDateFormat::SimpleDateFormat(
    #       const UnicodeString &pattern,
    #       const DateFormatSymbols &formatData,
    #       UErrorCode &status
    # )
    format_data = DateFormatSymbols(locale)
    fmt7 = SimpleDateFormat(
        UnicodeString("yyyy.MM.dd G 'at' HH:mm:ss z"),
        format_data)
    fmt7a = SimpleDateFormat(
        "yyyy.MM.dd G 'at' HH:mm:ss z",
        format_data)
    assert fmt7 == fmt7a
    assert fmt7 != fmt1

    # [8]
    # icu::SimpleDateFormat::SimpleDateFormat(const SimpleDateFormat &)
    fmt8 = SimpleDateFormat(fmt2)
    assert fmt8 == fmt2
    assert fmt8 != fmt1

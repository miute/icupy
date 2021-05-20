import copy

import pytest
from icupy import (
    Calendar, DateFormat, DateInterval, DateIntervalFormat, DateIntervalInfo,
    FieldPosition, Format, Locale, TimeZone, UCalendarDateFields,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    assert issubclass(DateIntervalFormat, Format)

    fmt1 = DateIntervalFormat.create_instance("yMMMd", Locale.get_english())
    fmt2 = DateIntervalFormat.create_instance("yMMMd", "en")
    fmt3 = DateIntervalFormat.create_instance("yMMMd", Locale.get_japanese())

    # DateIntervalFormat::operator!=()
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert fmt2 != fmt3

    # DateIntervalFormat::operator==()
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)

    # const DateFormat *DateIntervalFormat::getDateFormat(void)
    dtfmt = fmt2.get_date_format()
    assert isinstance(dtfmt, DateFormat)

    # const DateIntervalInfo *DateIntervalFormat::getDateIntervalInfo(void)
    dtitvinf = fmt2.get_date_interval_info()
    assert isinstance(dtitvinf, DateIntervalInfo)

    # void DateIntervalFormat::setDateIntervalInfo(
    #       const DateIntervalInfo &newIntervalPatterns,
    #       UErrorCode &status
    # )
    new_interval_patterns = DateIntervalInfo(Locale.get_japanese())
    assert new_interval_patterns != dtitvinf
    fmt2.set_date_interval_info(new_interval_patterns)
    assert fmt2.get_date_interval_info() == new_interval_patterns

    # const TimeZone &DateIntervalFormat::getTimeZone(void)
    zone2 = fmt2.get_time_zone()
    assert isinstance(zone2, TimeZone)

    # void DateIntervalFormat::setTimeZone(const TimeZone &zone)
    zone = TimeZone.create_time_zone("JST")
    assert zone != zone2
    fmt2.set_time_zone(zone)
    assert fmt2.get_time_zone() == zone

    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z
    append_to = UnicodeString()
    field_position = FieldPosition(FieldPosition.DONT_CARE)
    fmt2.format(DateInterval(from_date, to_date), append_to, field_position)
    assert (append_to
            == "2013\u5E744\u670826\u65E5\uFF5E28\u65E5")  # 2013年4月26日～28日


def test_clone():
    fmt1 = DateIntervalFormat.create_instance("yMMMd", Locale.get_english())

    fmt2 = fmt1.clone()
    assert isinstance(fmt2, DateIntervalFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_create_instance():
    dtitvinf = DateIntervalInfo(Locale.get_english())

    # [1]
    # static DateIntervalFormat *DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       const DateIntervalInfo &dtitvinf,
    #       UErrorCode &status
    # )
    fmt1 = DateIntervalFormat.create_instance(
        UnicodeString("yMMMd"),
        dtitvinf)
    assert isinstance(fmt1, DateIntervalFormat)

    fmt1a = DateIntervalFormat.create_instance(
        "yMMMd",
        dtitvinf)
    assert fmt1 == fmt1a

    # [2]
    # static DateIntervalFormat *DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       const DateIntervalInfo &dtitvinf,
    #       UErrorCode &status
    # )
    fmt2 = DateIntervalFormat.create_instance(
        UnicodeString("yMMMd"),
        Locale("en"),
        dtitvinf)
    assert isinstance(fmt2, DateIntervalFormat)

    fmt2a = DateIntervalFormat.create_instance(
        "yMMMd",
        Locale("en"),
        dtitvinf)
    fmt2b = DateIntervalFormat.create_instance(
        UnicodeString("yMMMd"),
        "en",
        dtitvinf)
    fmt2c = DateIntervalFormat.create_instance(
        "yMMMd",
        "en",
        dtitvinf)
    assert fmt2 == fmt2a == fmt2b == fmt2c

    # [3]
    # static DateIntervalFormat *DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt3 = DateIntervalFormat.create_instance(
        UnicodeString("yMMMd"),
        Locale("en"))
    assert isinstance(fmt3, DateIntervalFormat)

    fmt3a = DateIntervalFormat.create_instance(
        "yMMMd",
        Locale("en"))
    fmt3b = DateIntervalFormat.create_instance(
        UnicodeString("yMMMd"),
        "en")
    fmt3c = DateIntervalFormat.create_instance(
        "yMMMd",
        "en")
    assert fmt3 == fmt3a == fmt3b == fmt3c

    # [4]
    # static DateIntervalFormat *DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    fmt4 = DateIntervalFormat.create_instance(UnicodeString("yMMMd"))
    assert isinstance(fmt4, DateIntervalFormat)

    fmt4a = DateIntervalFormat.create_instance("yMMMd")
    assert fmt4 == fmt4a


def test_date_interval():
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z

    # [1]
    # DateInterval::DateInterval(UDate fromDate,
    #                            UDate toDate)
    itv1 = DateInterval(from_date, to_date)

    # [2]
    # DateInterval::DateInterval(const DateInterval &other)
    itv2 = DateInterval(itv1)

    # DateInterval *DateInterval::clone()
    itv3 = itv1.clone()

    # DateInterval::operator!=()
    assert not (itv1 != itv2)
    assert not (itv1 != itv3)
    assert not (itv2 != itv3)

    # DateInterval::operator==()
    assert itv1 == itv2 == itv3

    # UDate DateInterval::getFromDate()
    assert itv2.get_from_date() == from_date

    # UDate DateInterval::getToDate()
    assert itv2.get_to_date() == to_date


def test_date_interval_info():
    # [2]
    # DateIntervalInfo::DateIntervalInfo(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    dtitvinf2 = DateIntervalInfo(Locale("en"))
    dtitvinf2a = DateIntervalInfo("en")
    dtitvinf2b = DateIntervalInfo(Locale("ja"))

    # [3]
    # DateIntervalInfo::DateIntervalInfo(const DateIntervalInfo &)
    dtitvinf3 = DateIntervalInfo(dtitvinf2)

    # DateIntervalInfo *DateIntervalInfo::clone()
    dtitvinf4 = dtitvinf2.clone()
    assert isinstance(dtitvinf4, DateIntervalInfo)

    # DateIntervalInfo::operator!=()
    assert not (dtitvinf2 != dtitvinf2a)
    assert dtitvinf2 != dtitvinf2b
    assert not (dtitvinf2 != dtitvinf3)
    assert not (dtitvinf2 != dtitvinf4)

    # DateIntervalInfo::operator==()
    assert dtitvinf2 == dtitvinf2a == dtitvinf3 == dtitvinf4
    assert not (dtitvinf2 == dtitvinf2b)

    # UBool DateIntervalInfo::getDefaultOrder()
    assert not dtitvinf2.get_default_order()

    # UnicodeString &DateIntervalInfo::getFallbackIntervalPattern(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    pattern = dtitvinf3.get_fallback_interval_pattern(result)
    assert isinstance(pattern, UnicodeString)
    assert id(result) == id(pattern)
    assert result == "{0} \u2013 {1}"

    # void DateIntervalInfo::setFallbackIntervalPattern(
    #       const UnicodeString &fallbackPattern,
    #       UErrorCode &status
    # )
    dtitvinf3.set_fallback_interval_pattern(UnicodeString("{0} ~ {1}"))
    assert dtitvinf3.get_fallback_interval_pattern(result) == "{0} ~ {1}"

    dtitvinf3.set_fallback_interval_pattern("{0} \u2013 {1}")
    assert dtitvinf3.get_fallback_interval_pattern(result) == "{0} \u2013 {1}"

    # UnicodeString &DateIntervalInfo::getIntervalPattern(
    #       const UnicodeString &skeleton,
    #       UCalendarDateFields field,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result = UnicodeString()
    pattern = dtitvinf3.get_interval_pattern(
        UnicodeString("Hm"),
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result)
    assert isinstance(pattern, UnicodeString)
    assert id(result) == id(pattern)
    assert result == "HH:mm \u2013 HH:mm"

    pattern = dtitvinf3.get_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result)
    assert isinstance(pattern, UnicodeString)
    assert id(result) == id(pattern)
    assert result == "HH:mm \u2013 HH:mm"

    # void DateIntervalInfo::setIntervalPattern(
    #       const UnicodeString &skeleton,
    #       UCalendarDateFields lrgDiffCalUnit,
    #       const UnicodeString &intervalPattern,
    #       UErrorCode &status
    # )
    dtitvinf3.set_interval_pattern(
        UnicodeString("Hm"),
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        UnicodeString("yyyy MMM d HH:mm ~ HH:mm"))
    assert dtitvinf3.get_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result) == "yyyy MMM d HH:mm ~ HH:mm"

    dtitvinf3.set_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        UnicodeString("HH:mm ~ HH:mm"))
    assert dtitvinf3.get_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result) == "HH:mm ~ HH:mm"

    dtitvinf3.set_interval_pattern(
        UnicodeString("Hm"),
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        "yyyy MMM d HH:mm ~ HH:mm")
    assert dtitvinf3.get_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result) == "yyyy MMM d HH:mm ~ HH:mm"

    dtitvinf3.set_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        "HH:mm ~ HH:mm")
    assert dtitvinf3.get_interval_pattern(
        "Hm",
        UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result) == "HH:mm ~ HH:mm"


def test_format():
    en_locale = Locale.get_english()
    fmt = DateIntervalFormat.create_instance("yMMMd", en_locale)
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z

    # [1]
    # UnicodeString &DateIntervalFormat::format(
    #       Calendar &fromCalendar,
    #       Calendar &toCalendar,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition,
    #       UErrorCode &status
    # )
    zone = TimeZone.get_gmt()
    from_calendar = Calendar.create_instance(zone, en_locale)
    from_calendar.set_time(from_date)
    to_calendar = Calendar.create_instance(zone, en_locale)
    to_calendar.set_time(to_date)
    append_to = UnicodeString()
    field_position = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(
        from_calendar,
        to_calendar,
        append_to,
        field_position)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Apr 26 \u2013 28, 2013"

    # [2]
    # UnicodeString &DateIntervalFormat::format(
    #       const DateInterval *dtInterval,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition,
    #       UErrorCode &status
    # )
    dt_interval = DateInterval(from_date, to_date)
    append_to = UnicodeString()
    field_position = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(dt_interval, append_to, field_position)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Apr 26 \u2013 28, 2013"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_format_to_value():
    from icupy import FormattedDateInterval

    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z

    en_locale = Locale.get_english()
    fmt = DateIntervalFormat.create_instance("yMMMd", en_locale)
    zone = TimeZone.get_gmt()
    from_calendar = Calendar.create_instance(zone, en_locale)
    from_calendar.set_time(from_date)
    to_calendar = Calendar.create_instance(zone, en_locale)
    to_calendar.set_time(to_date)

    # [1]
    # FormattedDateInterval DateIntervalFormat::formatToValue(
    #       Calendar &fromCalendar,
    #       Calendar &toCalendar,
    #       UErrorCode &status
    # )
    dtitv1 = fmt.format_to_value(from_calendar, to_calendar)
    assert isinstance(dtitv1, FormattedDateInterval)
    assert dtitv1.to_temp_string() == "Apr 26 \u2013 28, 2013"

    # [2]
    # FormattedDateInterval DateIntervalFormat::formatToValue(
    #       const DateInterval &dtInterval,
    #       UErrorCode &status
    # )
    dt_interval = DateInterval(from_date, to_date)
    dtitv2 = fmt.format_to_value(dt_interval)
    assert isinstance(dtitv2, FormattedDateInterval)
    assert dtitv2.to_temp_string() == "Apr 26 \u2013 28, 2013"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_formatted_date_interval():
    from icupy import (
        ConstrainedFieldPosition, FormattedDateInterval, UDateFormatField,
        UFieldCategory, UnicodeStringAppendable,
    )

    fmt = DateIntervalFormat.create_instance("yMMMd", Locale.get_english())
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z
    dtitv = fmt.format_to_value(DateInterval(from_date, to_date))
    assert isinstance(dtitv, FormattedDateInterval)

    # Appendable &FormattedDateInterval::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    dest = UnicodeString()
    appendable = UnicodeStringAppendable(dest)
    appendable.append_code_point(0x1f338)
    result = dtitv.append_to(appendable)
    assert isinstance(result, UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert dest == "\U0001f338Apr 26 \u2013 28, 2013"

    # UBool FormattedDateInterval::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = ConstrainedFieldPosition()
    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == UDateFormatField.UDAT_MONTH_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 3)

    assert dtitv.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_DATE_INTERVAL_SPAN)
    assert cfpos.get_field() == 0

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == UDateFormatField.UDAT_DATE_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (4, 6)

    assert dtitv.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_DATE_INTERVAL_SPAN)
    assert cfpos.get_field() == 1

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == UDateFormatField.UDAT_DATE_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (9, 11)

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == UDateFormatField.UDAT_YEAR_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (13, 17)

    assert not dtitv.next_position(cfpos)

    # UnicodeString FormattedDateInterval::toString(UErrorCode &status)
    result = dtitv.to_string()
    assert isinstance(result, UnicodeString)
    assert result == "Apr 26 \u2013 28, 2013"

    # UnicodeString FormattedDateInterval::toTempString(UErrorCode &status)
    result = dtitv.to_temp_string()
    assert isinstance(result, UnicodeString)
    assert result == "Apr 26 \u2013 28, 2013"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_get_context():
    from icupy import UDisplayContext, UDisplayContextType

    fmt = DateIntervalFormat.create_instance("yMMMd", Locale.get_english())

    # UDisplayContext DateIntervalFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_NONE)

    # void DateIntervalFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)

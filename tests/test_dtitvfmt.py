import copy

import pytest

from icupy import icu


def test_api() -> None:
    assert issubclass(icu.DateIntervalFormat, icu.Format)

    fmt1 = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale.get_english())
    fmt2 = icu.DateIntervalFormat.create_instance("yMMMd", "en")
    fmt3 = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale.get_japanese())

    # UBool icu::DateIntervalFormat::operator!=(const Format &)
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert fmt2 != fmt3

    # UBool icu::DateIntervalFormat::operator==(const Format &)
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)

    # const DateFormat *icu::DateIntervalFormat::getDateFormat(void)
    dtfmt = fmt2.get_date_format()
    assert isinstance(dtfmt, icu.DateFormat)

    # const DateIntervalInfo *icu::DateIntervalFormat::getDateIntervalInfo()
    dtitvinf = fmt2.get_date_interval_info()
    assert isinstance(dtitvinf, icu.DateIntervalInfo)

    # Locale icu::Format::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    loc = fmt1.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, icu.Locale)

    # void icu::DateIntervalFormat::setDateIntervalInfo(
    #       const DateIntervalInfo &newIntervalPatterns,
    #       UErrorCode &status
    # )
    new_interval_patterns = icu.DateIntervalInfo(icu.Locale.get_japanese())
    assert new_interval_patterns != dtitvinf
    fmt2.set_date_interval_info(new_interval_patterns)
    assert fmt2.get_date_interval_info() == new_interval_patterns

    # const TimeZone &icu::DateIntervalFormat::getTimeZone(void)
    zone2 = fmt2.get_time_zone()
    assert isinstance(zone2, icu.TimeZone)

    # void icu::DateIntervalFormat::setTimeZone(const TimeZone &zone)
    zone = icu.TimeZone.create_time_zone("JST")
    assert zone != zone2
    fmt2.set_time_zone(zone)
    assert fmt2.get_time_zone() == zone


def test_clone() -> None:
    fmt1 = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale.get_english())

    # DateIntervalFormat *icu::DateIntervalFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.DateIntervalFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_create_instance() -> None:
    dtitvinf = icu.DateIntervalInfo(icu.Locale.get_english())

    # [1]
    # static DateIntervalFormat *icu::DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       const DateIntervalInfo &dtitvinf,
    #       UErrorCode &status
    # )
    fmt1 = icu.DateIntervalFormat.create_instance(icu.UnicodeString("yMMMd"), dtitvinf)
    assert isinstance(fmt1, icu.DateIntervalFormat)

    fmt1a = icu.DateIntervalFormat.create_instance("yMMMd", dtitvinf)
    assert isinstance(fmt1a, icu.DateIntervalFormat)
    assert fmt1 == fmt1a

    # [2]
    # static DateIntervalFormat *icu::DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       const DateIntervalInfo &dtitvinf,
    #       UErrorCode &status
    # )
    fmt2 = icu.DateIntervalFormat.create_instance(
        icu.UnicodeString("yMMMd"), icu.Locale("en"), dtitvinf
    )
    assert isinstance(fmt2, icu.DateIntervalFormat)

    fmt2a = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale("en"), dtitvinf)
    assert isinstance(fmt2a, icu.DateIntervalFormat)

    fmt2b = icu.DateIntervalFormat.create_instance(icu.UnicodeString("yMMMd"), "en", dtitvinf)
    assert isinstance(fmt2b, icu.DateIntervalFormat)

    fmt2c = icu.DateIntervalFormat.create_instance("yMMMd", "en", dtitvinf)
    assert isinstance(fmt2c, icu.DateIntervalFormat)
    assert fmt2 == fmt2a == fmt2b == fmt2c

    # [3]
    # static DateIntervalFormat *icu::DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt3 = icu.DateIntervalFormat.create_instance(icu.UnicodeString("yMMMd"), icu.Locale("en"))
    assert isinstance(fmt3, icu.DateIntervalFormat)

    fmt3a = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale("en"))
    assert isinstance(fmt3a, icu.DateIntervalFormat)

    fmt3b = icu.DateIntervalFormat.create_instance(icu.UnicodeString("yMMMd"), "en")
    assert isinstance(fmt3b, icu.DateIntervalFormat)

    fmt3c = icu.DateIntervalFormat.create_instance("yMMMd", "en")
    assert isinstance(fmt3c, icu.DateIntervalFormat)
    assert fmt3 == fmt3a == fmt3b == fmt3c

    # [4]
    # static DateIntervalFormat *icu::DateIntervalFormat::createInstance(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    fmt4 = icu.DateIntervalFormat.create_instance(icu.UnicodeString("yMMMd"))
    assert isinstance(fmt4, icu.DateIntervalFormat)

    fmt4a = icu.DateIntervalFormat.create_instance("yMMMd")
    assert isinstance(fmt4a, icu.DateIntervalFormat)
    assert fmt4 == fmt4a


def test_date_interval() -> None:
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z

    # [1]
    # icu::DateInterval::DateInterval(
    #       UDate fromDate,
    #       UDate toDate
    # )
    itv1 = icu.DateInterval(from_date, to_date)

    # [2]
    # icu::DateInterval::DateInterval(const DateInterval &other)
    itv2 = icu.DateInterval(itv1)

    # DateInterval *icu::DateInterval::clone()
    itv3 = itv1.clone()
    assert isinstance(itv3, icu.DateInterval)

    # DateInterval.__copy__() -> DateInterval
    # DateInterval.__deepcopy__(Optional[memo]) -> DateInterval
    itv3a = copy.copy(itv3)
    itv3b = copy.deepcopy(itv3)
    assert itv3 == itv3a == itv3b

    # UBool icu::DateInterval::operator!=(const DateInterval &)
    assert not (itv1 != itv2)
    assert not (itv1 != itv3)
    assert not (itv2 != itv3)

    # UBool icu::DateInterval::operator==(const DateInterval &)
    assert itv1 == itv2 == itv3

    # UDate icu::DateInterval::getFromDate()
    assert itv2.get_from_date() == from_date

    # UDate icu::DateInterval::getToDate()
    assert itv2.get_to_date() == to_date


def test_date_interval_info() -> None:
    # [2]
    # icu::DateIntervalInfo::DateIntervalInfo(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    dtitvinf2 = icu.DateIntervalInfo(icu.Locale("en"))
    dtitvinf2a = icu.DateIntervalInfo("en")
    dtitvinf2b = icu.DateIntervalInfo(icu.Locale("ja"))

    # [3]
    # icu::DateIntervalInfo::DateIntervalInfo(const DateIntervalInfo &)
    dtitvinf3 = icu.DateIntervalInfo(dtitvinf2)

    # icu::DateIntervalInfo *DateIntervalInfo::clone()
    dtitvinf4 = dtitvinf2.clone()
    assert isinstance(dtitvinf4, icu.DateIntervalInfo)

    # DateIntervalInfo.__copy__() -> DateIntervalInfo
    # DateIntervalInfo.__deepcopy__(Optional[memo]) -> DateIntervalInfo
    dtitvinf4a = copy.copy(dtitvinf2)
    dtitvinf4b = copy.deepcopy(dtitvinf2)
    assert dtitvinf4 == dtitvinf4a == dtitvinf4b

    # UBool icu::DateIntervalInfo::operator!=(const DateIntervalInfo &)
    assert not (dtitvinf2 != dtitvinf2a)
    assert dtitvinf2 != dtitvinf2b
    assert not (dtitvinf2 != dtitvinf3)
    assert not (dtitvinf2 != dtitvinf4)

    # UBool icu::DateIntervalInfo::operator==(const DateIntervalInfo &)
    assert dtitvinf2 == dtitvinf2a == dtitvinf3 == dtitvinf4
    assert not (dtitvinf2 == dtitvinf2b)

    # UBool icu::DateIntervalInfo::getDefaultOrder()
    assert dtitvinf2.get_default_order() is False

    # UnicodeString &icu::DateIntervalInfo::getFallbackIntervalPattern(
    #       UnicodeString &result
    # )
    result = icu.UnicodeString()
    pattern = dtitvinf3.get_fallback_interval_pattern(result)
    assert isinstance(pattern, icu.UnicodeString)
    assert id(result) == id(pattern)
    assert result in (
        "{0} \u2013 {1}",
        "{0}\u2009\u2013\u2009{1}",  # ICU>=72
    )

    # void icu::DateIntervalInfo::setFallbackIntervalPattern(
    #       const UnicodeString &fallbackPattern,
    #       UErrorCode &status
    # )
    dtitvinf3.set_fallback_interval_pattern(icu.UnicodeString("{0} ~ {1}"))
    assert dtitvinf3.get_fallback_interval_pattern(result) == "{0} ~ {1}"

    dtitvinf3.set_fallback_interval_pattern("{0} \u2013 {1}")
    assert dtitvinf3.get_fallback_interval_pattern(result) == "{0} \u2013 {1}"

    # UnicodeString &icu::DateIntervalInfo::getIntervalPattern(
    #       const UnicodeString &skeleton,
    #       UCalendarDateFields field,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result = icu.UnicodeString()
    pattern = dtitvinf3.get_interval_pattern(
        icu.UnicodeString("Hm"),
        icu.UCalendarDateFields.UCAL_HOUR_OF_DAY,
        result,
    )
    assert isinstance(pattern, icu.UnicodeString)
    assert id(result) == id(pattern)
    assert result in (
        "HH:mm \u2013 HH:mm",
        "HH:mm\u2009\u2013\u2009HH:mm",  # ICU>=72
    )

    pattern = dtitvinf3.get_interval_pattern(
        "Hm", icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, result
    )
    assert isinstance(pattern, icu.UnicodeString)
    assert id(result) == id(pattern)
    assert result in (
        "HH:mm \u2013 HH:mm",
        "HH:mm\u2009\u2013\u2009HH:mm",  # ICU>=72
    )

    # void icu::DateIntervalInfo::setIntervalPattern(
    #       const UnicodeString &skeleton,
    #       UCalendarDateFields lrgDiffCalUnit,
    #       const UnicodeString &intervalPattern,
    #       UErrorCode &status
    # )
    dtitvinf3.set_interval_pattern(
        icu.UnicodeString("Hm"),
        icu.UCalendarDateFields.UCAL_HOUR_OF_DAY,
        icu.UnicodeString("yyyy MMM d HH:mm ~ HH:mm"),
    )
    assert (
        dtitvinf3.get_interval_pattern("Hm", icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, result)
        == "yyyy MMM d HH:mm ~ HH:mm"
    )

    dtitvinf3.set_interval_pattern(
        "Hm",
        icu.UCalendarDateFields.UCAL_HOUR_OF_DAY,
        icu.UnicodeString("HH:mm ~ HH:mm"),
    )
    assert (
        dtitvinf3.get_interval_pattern("Hm", icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, result)
        == "HH:mm ~ HH:mm"
    )

    dtitvinf3.set_interval_pattern(
        icu.UnicodeString("Hm"),
        icu.UCalendarDateFields.UCAL_HOUR_OF_DAY,
        "yyyy MMM d HH:mm ~ HH:mm",
    )
    assert (
        dtitvinf3.get_interval_pattern("Hm", icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, result)
        == "yyyy MMM d HH:mm ~ HH:mm"
    )

    dtitvinf3.set_interval_pattern(
        "Hm", icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, "HH:mm ~ HH:mm"
    )
    assert (
        dtitvinf3.get_interval_pattern("Hm", icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, result)
        == "HH:mm ~ HH:mm"
    )


def test_format() -> None:
    en_locale = icu.Locale.get_english()
    fmt = icu.DateIntervalFormat.create_instance("yMMMd", en_locale)
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z

    # [1]
    # UnicodeString &icu::DateIntervalFormat::format(
    #       Calendar &fromCalendar,
    #       Calendar &toCalendar,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition,
    #       UErrorCode &status
    # )
    zone = icu.TimeZone.get_gmt()
    from_calendar = icu.Calendar.create_instance(zone, en_locale)
    from_calendar.set_time(from_date)
    to_calendar = icu.Calendar.create_instance(zone, en_locale)
    to_calendar.set_time(to_date)
    append_to = icu.UnicodeString()
    field_position = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(from_calendar, to_calendar, append_to, field_position)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )

    # [2]
    # UnicodeString &icu::DateIntervalFormat::format(
    #       const DateInterval *dtInterval,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition,
    #       UErrorCode &status
    # )
    dt_interval = icu.DateInterval(from_date, to_date)
    append_to = icu.UnicodeString()
    field_position = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(dt_interval, append_to, field_position)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )

    # [3]
    # UnicodeString &icu::DateIntervalFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &fieldPosition,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    field_position = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(
        icu.Formattable(icu.DateInterval(from_date, to_date)),
        append_to,
        field_position,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )

    # [5]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(
            icu.Formattable(icu.DateInterval(from_date, to_date)),
            append_to,
            pos_iter,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [6]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    result = fmt.format(icu.Formattable(icu.DateInterval(from_date, to_date)), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_format_to_value() -> None:
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z

    en_locale = icu.Locale.get_english()
    fmt = icu.DateIntervalFormat.create_instance("yMMMd", en_locale)
    zone = icu.TimeZone.get_gmt()
    from_calendar = icu.Calendar.create_instance(zone, en_locale)
    from_calendar.set_time(from_date)
    to_calendar = icu.Calendar.create_instance(zone, en_locale)
    to_calendar.set_time(to_date)

    # [1]
    # FormattedDateInterval icu::DateIntervalFormat::formatToValue(
    #       Calendar &fromCalendar,
    #       Calendar &toCalendar,
    #       UErrorCode &status
    # )
    dtitv1 = fmt.format_to_value(from_calendar, to_calendar)
    assert isinstance(dtitv1, icu.FormattedDateInterval)
    assert dtitv1.to_temp_string() in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )

    # [2]
    # FormattedDateInterval icu::DateIntervalFormat::formatToValue(
    #       const DateInterval &dtInterval,
    #       UErrorCode &status
    # )
    dt_interval = icu.DateInterval(from_date, to_date)
    dtitv2 = fmt.format_to_value(dt_interval)
    assert isinstance(dtitv2, icu.FormattedDateInterval)
    assert dtitv2.to_temp_string() in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_formatted_date_interval() -> None:
    assert issubclass(icu.FormattedDateInterval, icu.FormattedValue)

    # icu::FormattedDateInterval::FormattedDateInterval()
    dtitv = icu.FormattedDateInterval()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = dtitv.to_string()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_STATE_ERROR

    fmt = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale.get_english())
    from_date = 1366934400000.0  # 2013-04-26T00:00:00Z
    to_date = 1367107200000.0  # 2013-04-28T00:00:00Z
    dtitv = fmt.format_to_value(icu.DateInterval(from_date, to_date))
    assert isinstance(dtitv, icu.FormattedDateInterval)

    # Appendable &icu::FormattedDateInterval::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    dest = icu.UnicodeString()
    appendable = icu.UnicodeStringAppendable(dest)
    appendable.append_code_point(0x1F338)
    result = dtitv.append_to(appendable)
    assert isinstance(result, icu.UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert dest in (
        "\U0001f338Apr 26 \u2013 28, 2013",
        "\U0001f338Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )

    # UBool icu::FormattedDateInterval::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = icu.ConstrainedFieldPosition()
    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == icu.UDateFormatField.UDAT_MONTH_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 3)

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_DATE_INTERVAL_SPAN
    assert cfpos.get_field() == 0

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == icu.UDateFormatField.UDAT_DATE_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (4, 6)

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_DATE_INTERVAL_SPAN
    assert cfpos.get_field() == 1

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == icu.UDateFormatField.UDAT_DATE_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (9, 11)

    assert dtitv.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_DATE
    assert cfpos.get_field() == icu.UDateFormatField.UDAT_YEAR_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (13, 17)

    assert not dtitv.next_position(cfpos)

    # UnicodeString icu::FormattedDateInterval::toString(UErrorCode &status)
    result = dtitv.to_string()
    assert isinstance(result, icu.UnicodeString)
    assert result in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )

    # UnicodeString icu::FormattedDateInterval::toTempString(
    #       UErrorCode &status
    # )
    result = dtitv.to_temp_string()
    assert isinstance(result, icu.UnicodeString)
    assert result in (
        "Apr 26 \u2013 28, 2013",
        "Apr 26\u2009\u2013\u200928, 2013",  # ICU>=72
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_get_context() -> None:
    fmt = icu.DateIntervalFormat.create_instance("yMMMd", icu.Locale.get_english())

    # UDisplayContext icu::DateIntervalFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (
        fmt.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
        == icu.UDisplayContext.UDISPCTX_CAPITALIZATION_NONE
    )

    # void icu::DateIntervalFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(icu.UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)
    assert (
        fmt.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
        == icu.UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE
    )


def test_get_time_zone_upcasting() -> None:
    fmt = icu.DateIntervalFormat.create_instance("yMMMd")

    fmt.set_time_zone(icu.TimeZone.get_gmt())
    zone = fmt.get_time_zone()
    assert isinstance(zone, icu.SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    fmt.set_time_zone(icu.TimeZone.create_time_zone("JST"))
    zone = fmt.get_time_zone()
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)

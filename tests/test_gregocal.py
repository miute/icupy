import copy

import pytest

from icupy import icu

HOUR = icu.U_MILLIS_PER_HOUR


def test_api() -> None:
    # static const Locale *icu::Calendar::getAvailableLocales(
    #       int32_t &count
    # )
    result = icu.Calendar.get_available_locales()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, icu.Locale) for x in result)

    # static StringEnumeration *icu::Calendar::getKeywordValuesForLocale(
    #       const char *key,
    #       const Locale &locale,
    #       UBool commonlyUsed,
    #       UErrorCode &status
    # )
    it1 = icu.Calendar.get_keyword_values_for_locale("calendar", icu.Locale("he"), True)
    assert isinstance(it1, icu.StringEnumeration)
    assert "hebrew" in it1

    it2 = icu.Calendar.get_keyword_values_for_locale("calendar", "he", True)
    assert isinstance(it2, icu.StringEnumeration)
    assert "hebrew" in it2

    # static UDate icu::Calendar::getNow(void)
    result = icu.Calendar.get_now()
    assert isinstance(result, float)
    assert result > 0

    when = 1215300615000.0  # 2008-07-05T23:30:15Z
    when2 = 1215304215000.0  # 2008-07-06T00:30:15Z
    when3 = 1215097200000.0  # 2008-07-04T00:00:00Z
    zone = icu.TimeZone.get_gmt()
    zone2 = icu.TimeZone.create_default()
    cal = icu.Calendar.create_instance("en")
    assert isinstance(cal, icu.GregorianCalendar)

    # const TimeZone &icu::Calendar::getTimeZone(void)
    result = cal.get_time_zone()
    assert isinstance(result, icu.TimeZone)
    assert result == zone2

    # void icu::Calendar::setTimeZone(const TimeZone &zone)
    cal.set_time_zone(zone)
    assert cal.get_time_zone() == zone

    # void icu::Calendar::setTime(
    #       UDate date,
    #       UErrorCode &status
    # )
    cal.set_time(when)

    # GregorianCalendar.__repr__() -> str
    assert repr(cal) == "<GregorianCalendar('2008-07-05T23:30:15.000Z')>"

    # UDate icu::Calendar::getTime(UErrorCode &status)
    assert cal.get_time() == when

    # void icu::Calendar::add(
    #       UCalendarDateFields field,
    #       int32_t amount,
    #       UErrorCode &status
    # )
    cal2 = cal.clone()
    cal2.add(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, 1)
    assert cal2.get_time() == when2  # 2008-07-06T00:30:15Z

    # UBool icu::Calendar::after(
    #       const Calendar &when,
    #       UErrorCode &status
    # )
    assert cal.after(cal2) is False
    assert cal2.after(cal) is True

    # UBool icu::Calendar::before(
    #       const Calendar &when,
    #       UErrorCode &status
    # )
    assert cal.before(cal2) is True
    assert cal2.before(cal) is False

    # [2]
    # void icu::Calendar::clear(UCalendarDateFields field)
    cal3 = cal2.clone()
    cal3.clear(icu.UCalendarDateFields.UCAL_SECOND)
    assert cal3.get_time() == 1215304200000.0  # 2008-07-06T00:30:00Z

    # [3]
    # void icu::Calendar::clear(void)
    cal3.clear()
    assert cal3.get_time() == 0

    cal4 = icu.GregorianCalendar(zone, "en")
    cal4.set_time(when)

    # UBool icu::Calendar::equals(
    #       const Calendar &when,
    #       UErrorCode &status
    # )
    assert cal.equals(cal2) is False
    assert cal.equals(cal3) is False
    assert cal.equals(cal4) is True

    # int32_t icu::Calendar::fieldDifference(
    #       UDate when,
    #       UCalendarDateFields field,
    #       UErrorCode &status
    # )
    assert cal.field_difference(when2, icu.UCalendarDateFields.UCAL_HOUR_OF_DAY) == 1
    assert cal.field_difference(when2, icu.UCalendarDateFields.UCAL_MINUTE) == 0

    year = 2008
    month = icu.UCalendarMonths.UCAL_JULY
    date = 5
    hour = 23
    minute = 30
    second = 15

    # [2]
    # void icu::Calendar::set(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date
    # )
    cal3.set(year, month, date)

    # int32_t icu::Calendar::get(
    #       UCalendarDateFields field,
    #       UErrorCode &status
    # )
    assert cal3.get(icu.UCalendarDateFields.UCAL_ERA) == icu.GregorianCalendar.AD
    assert cal3.get(icu.UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(icu.UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(icu.UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY) == 0
    assert cal3.get(icu.UCalendarDateFields.UCAL_MINUTE) == 0
    assert cal3.get(icu.UCalendarDateFields.UCAL_SECOND) == 0

    # [3]
    # void icu::Calendar::set(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       int32_t hour,
    #       int32_t minute
    # )
    cal3.set(year, month, date, hour, minute)
    assert cal3.get(icu.UCalendarDateFields.UCAL_ERA) == icu.GregorianCalendar.AD
    assert cal3.get(icu.UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(icu.UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(icu.UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY) == hour
    assert cal3.get(icu.UCalendarDateFields.UCAL_MINUTE) == minute
    assert cal3.get(icu.UCalendarDateFields.UCAL_SECOND) == 0

    # [4]
    # void icu::Calendar::set(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       int32_t hour,
    #       int32_t minute,
    #       int32_t second
    # )
    cal3.set(year, month, date, hour - 1, minute, second)
    assert cal3.get(icu.UCalendarDateFields.UCAL_ERA) == icu.GregorianCalendar.AD
    assert cal3.get(icu.UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(icu.UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(icu.UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY) == hour - 1
    assert cal3.get(icu.UCalendarDateFields.UCAL_MINUTE) == minute
    assert cal3.get(icu.UCalendarDateFields.UCAL_SECOND) == second

    # [5]
    # void icu::Calendar::set(
    #       UCalendarDateFields field,
    #       int32_t value
    # )
    cal3.set(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, hour)
    assert cal3.get(icu.UCalendarDateFields.UCAL_ERA) == icu.GregorianCalendar.AD
    assert cal3.get(icu.UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(icu.UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(icu.UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY) == hour
    assert cal3.get(icu.UCalendarDateFields.UCAL_MINUTE) == minute
    assert cal3.get(icu.UCalendarDateFields.UCAL_SECOND) == second

    # int32_t icu::Calendar::getActualMaximum(
    #       UCalendarDateFields field,
    #       UErrorCode &status
    # )
    assert cal.get_actual_maximum(icu.UCalendarDateFields.UCAL_DAY_OF_MONTH) == 31

    # int32_t icu::Calendar::getLeastMaximum(UCalendarDateFields field)
    assert cal.get_least_maximum(icu.UCalendarDateFields.UCAL_DAY_OF_MONTH) == 28

    # int32_t icu::Calendar::getMaximum(UCalendarDateFields field)
    assert cal.get_maximum(icu.UCalendarDateFields.UCAL_DAY_OF_MONTH) == 31

    # int32_t icu::Calendar::getActualMinimum(
    #       UCalendarDateFields field,
    #       UErrorCode &status
    # )
    assert cal.get_actual_minimum(icu.UCalendarDateFields.UCAL_DAY_OF_MONTH) == 1

    # int32_t icu::Calendar::getGreatestMinimum(UCalendarDateFields field)
    assert cal.get_greatest_minimum(icu.UCalendarDateFields.UCAL_DAY_OF_MONTH) == 1

    # int32_t icu::Calendar::getMinimum(UCalendarDateFields field)
    assert cal.get_minimum(icu.UCalendarDateFields.UCAL_DAY_OF_MONTH) == 1

    # UCalendarWeekdayType icu::Calendar::getDayOfWeekType(
    #       UCalendarDaysOfWeek dayOfWeek,
    #       UErrorCode &status
    # )
    assert (
        cal.get_day_of_week_type(icu.UCalendarDaysOfWeek.UCAL_SUNDAY)
        == icu.UCalendarWeekdayType.UCAL_WEEKEND
    )

    # UCalendarDaysOfWeek icu::Calendar::getFirstDayOfWeek(UErrorCode &status)
    assert cal2.get_first_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_SUNDAY

    # void icu::Calendar::setFirstDayOfWeek(UCalendarDaysOfWeek value)
    cal2.set_first_day_of_week(icu.UCalendarDaysOfWeek.UCAL_MONDAY)
    assert cal2.get_first_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_MONDAY

    # Locale icu::Calendar::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    result = cal.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("en")

    # uint8_t icu::Calendar::getMinimalDaysInFirstWeek(void)
    assert cal2.get_minimal_days_in_first_week() == 1

    # void icu::Calendar::setMinimalDaysInFirstWeek(uint8_t value)
    cal2.set_minimal_days_in_first_week(7)
    assert cal2.get_minimal_days_in_first_week() == 7

    # int32_t icu::Calendar::getWeekendTransition(
    #       UCalendarDaysOfWeek dayOfWeek,
    #       UErrorCode &status
    # )
    assert cal.get_weekend_transition(icu.UCalendarDaysOfWeek.UCAL_SUNDAY) == 24 * HOUR

    # UBool icu::Calendar::inDaylightTime(UErrorCode &status)
    assert cal.in_daylight_time() is False
    assert cal4.in_daylight_time() is False

    # UBool icu::Calendar::isEquivalentTo(const Calendar &other)
    assert cal.is_equivalent_to(cal2) is False
    assert cal.is_equivalent_to(cal3) is True
    assert cal.is_equivalent_to(cal4) is True

    # UBool icu::Calendar::isLenient(void)
    assert cal2.is_lenient() is True

    # void icu::Calendar::setLenient(UBool lenient)
    cal2.set_lenient(False)
    assert cal2.is_lenient() is False

    # UBool icu::Calendar::isSet(UCalendarDateFields field)
    assert cal2.is_set(icu.UCalendarDateFields.UCAL_YEAR) is True
    assert cal2.is_set(icu.UCalendarDateFields.UCAL_MONTH) is True
    assert cal2.is_set(icu.UCalendarDateFields.UCAL_DATE) is True

    # [1]
    # UBool icu::Calendar::isWeekend(
    #       UDate date,
    #       UErrorCode &status
    # )
    assert cal2.is_weekend(when) is True
    assert cal2.is_weekend(when3) is False

    # [2]
    # UBool icu::Calendar::isWeekend(void)
    assert cal2.is_weekend() is True

    # UBool icu::Calendar::operator!=(const Calendar &that)
    assert cal != cal2
    assert cal != cal3
    assert cal != cal4

    # UBool icu::Calendar::operator==(const Calendar &that)
    assert not (cal == cal2)
    assert not (cal == cal3)
    assert not (cal == cal4)

    # TimeZone *icu::Calendar::orphanTimeZone(void)
    zone4 = cal2.orphan_time_zone()
    assert isinstance(zone4, icu.TimeZone)
    assert zone4 != zone2
    assert cal2.get_time_zone() == zone2

    # [3]
    # void icu::Calendar::roll(
    #       UCalendarDateFields field,
    #       int32_t amount,
    #       UErrorCode &status
    # )
    cal3.roll(icu.UCalendarDateFields.UCAL_HOUR_OF_DAY, 1)
    # 2008-07-05T23:30:15Z -> 2008-07-05T00:30:15Z
    assert cal3.get_time() == 1215217815000.0  # 2008-07-05T00:30:15Z

    # UDate icu::GregorianCalendar::getGregorianChange(void)
    assert cal4.get_gregorian_change() == -12219292800000.0  # 1582-10-15T00:00:00Z

    # void icu::GregorianCalendar::setGregorianChange(
    #       UDate date,
    #       UErrorCode &success
    # )
    cal4.set_gregorian_change(-2208988800000.0)  # 1900-01-01T00:00:00Z
    assert cal4.get_gregorian_change() == -2208988800000.0

    # UBool icu::GregorianCalendar::isLeapYear(int32_t year)
    assert cal4.is_leap_year(2008) is True
    assert cal4.is_leap_year(2009) is False


def test_calendar_create_instance() -> None:
    locale1 = icu.Locale.get_english()
    locale2 = icu.Locale.get_default()
    zone1 = icu.SimpleTimeZone(8 * HOUR, "s2")
    zone2 = icu.TimeZone.create_default()

    # [1]
    # static Calendar *icu::Calendar::createInstance(
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal1 = icu.Calendar.create_instance(locale1)
    assert isinstance(cal1, icu.GregorianCalendar)
    assert cal1.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal1.get_time_zone() == zone2

    assert (
        icu.Calendar.create_instance("en").get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
        == locale1
    )

    # [2]
    # static Calendar *icu::Calendar::createInstance(
    #       const TimeZone &zone,
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal2 = icu.Calendar.create_instance(zone1, locale1)
    assert isinstance(cal2, icu.GregorianCalendar)
    assert cal2.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal2.get_time_zone() == zone1

    assert (
        icu.Calendar.create_instance(zone1, "en").get_locale(
            icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE
        )
        == locale1
    )

    # [3]
    # static Calendar *icu::Calendar::createInstance(
    #       const TimeZone &zone,
    #       UErrorCode &success
    # )
    cal3 = icu.Calendar.create_instance(zone1)
    assert isinstance(cal3, icu.GregorianCalendar)
    assert cal3.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal3.get_time_zone() == zone1

    # [6]
    # static Calendar *icu::Calendar::createInstance(UErrorCode &success)
    cal6 = icu.Calendar.create_instance()
    assert isinstance(cal6, icu.GregorianCalendar)
    assert cal6.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal6.get_time_zone() == zone2


def test_clone() -> None:
    cal1 = icu.GregorianCalendar(icu.TimeZone.get_gmt())
    cal1.set_time(1215298800000.0)  # 2008-07-05T23:00:00Z

    # Calendar *icu::Calendar::clone()
    cal2 = cal1.clone()
    assert isinstance(cal2, icu.GregorianCalendar)

    cal3 = copy.copy(cal1)

    cal4 = copy.deepcopy(cal1)
    assert cal1 == cal2 == cal3 == cal4


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_repeated_wall_time_option() -> None:
    zone = icu.SimpleTimeZone(8 * HOUR, "s2")
    cal = icu.Calendar.create_instance(zone)

    # UCalendarWallTimeOption icu::Calendar::getRepeatedWallTimeOption(void)
    assert cal.get_repeated_wall_time_option() == icu.UCalendarWallTimeOption.UCAL_WALLTIME_LAST

    # void icu::Calendar::setRepeatedWallTimeOption(
    #       UCalendarWallTimeOption option
    # )
    cal.set_repeated_wall_time_option(icu.UCalendarWallTimeOption.UCAL_WALLTIME_FIRST)
    assert (
        cal.get_repeated_wall_time_option() == icu.UCalendarWallTimeOption.UCAL_WALLTIME_FIRST
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_skipped_wall_time_option() -> None:
    zone = icu.SimpleTimeZone(8 * HOUR, "s2")
    cal = icu.Calendar.create_instance(zone)

    # UCalendarWallTimeOption icu::Calendar::getSkippedWallTimeOption(void)
    assert cal.get_skipped_wall_time_option() == icu.UCalendarWallTimeOption.UCAL_WALLTIME_LAST

    # void icu::Calendar::setSkippedWallTimeOption(
    #       UCalendarWallTimeOption option
    # )
    cal.set_skipped_wall_time_option(icu.UCalendarWallTimeOption.UCAL_WALLTIME_FIRST)
    assert cal.get_skipped_wall_time_option() == icu.UCalendarWallTimeOption.UCAL_WALLTIME_FIRST


def test_get_time_zone_upcasting() -> None:
    cal = icu.Calendar.create_instance()

    cal.set_time_zone(icu.TimeZone.get_gmt())
    zone = cal.get_time_zone()
    assert isinstance(zone, icu.SimpleTimeZone)

    zone = cal.orphan_time_zone()
    assert isinstance(zone, icu.SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    cal.set_time_zone(icu.TimeZone.create_time_zone("JST"))
    zone = cal.get_time_zone()
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)

    zone = cal.orphan_time_zone()
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_type() -> None:
    # const char *icu::Calendar::getType()
    cal1 = icu.Calendar.create_instance("ja")
    result = cal1.get_type()
    assert isinstance(result, str)
    assert result == "gregorian"

    cal2 = icu.Calendar.create_instance("ja@calendar=japanese")
    result = cal2.get_type()
    assert isinstance(result, str)
    assert result == "japanese"


def test_gregorian_calendar() -> None:
    assert issubclass(icu.GregorianCalendar, icu.Calendar)
    locale1 = icu.Locale.get_english()
    locale2 = icu.Locale.get_default()
    zone1 = icu.SimpleTimeZone(8 * HOUR, "s2")
    zone2 = icu.TimeZone.create_default()
    zone3 = icu.TimeZone.create_time_zone("Asia/Tokyo")
    offset = zone3.get_raw_offset() - zone2.get_raw_offset()

    # [1]
    # icu::GregorianCalendar::GregorianCalendar(UErrorCode &success)
    cal1 = icu.GregorianCalendar()
    assert cal1.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal1.get_time_zone() == zone2

    # [3]
    # icu::GregorianCalendar::GregorianCalendar(
    #       const TimeZone &zone,
    #       UErrorCode &success
    # )
    cal3 = icu.GregorianCalendar(zone1)
    assert cal3.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal3.get_time_zone() == zone1

    # [4]
    # icu::GregorianCalendar::GregorianCalendar(
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal4 = icu.GregorianCalendar(locale1)
    assert cal4.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal4.get_time_zone() == zone2

    # [6]
    # icu::GregorianCalendar::GregorianCalendar(
    #       const TimeZone &zone,
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal6 = icu.GregorianCalendar(zone1, locale1)
    assert cal6.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal6.get_time_zone() == zone1

    # [7]
    # icu::GregorianCalendar::GregorianCalendar(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       UErrorCode &success
    # )
    cal7 = icu.GregorianCalendar(2008, icu.UCalendarMonths.UCAL_JULY, 5)
    assert cal7.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal7.get_time_zone() == zone2
    assert cal7.get_time() - offset == 1215183600000.0  # 2008-07-05T00:00:00Z

    # [8]
    # icu::GregorianCalendar::GregorianCalendar(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       int32_t hour,
    #       int32_t minute,
    #       UErrorCode &success
    # )
    cal8 = icu.GregorianCalendar(2008, icu.UCalendarMonths.UCAL_JULY, 5, 23, 30)
    assert cal8.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal8.get_time_zone() == zone2
    assert cal8.get_time() - offset == 1215268200000.0  # 2008-07-05T23:30:00Z

    # [9]
    # icu::GregorianCalendar::GregorianCalendar(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       int32_t hour,
    #       int32_t minute,
    #       int32_t second,
    #       UErrorCode &success
    # )
    cal9 = icu.GregorianCalendar(2008, icu.UCalendarMonths.UCAL_JULY, 5, 23, 30, 15)
    assert cal9.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal9.get_time_zone() == zone2
    assert cal9.get_time() - offset == 1215268215000.0  # 2008-07-05T23:30:15Z

    # [10]
    # icu::GregorianCalendar::GregorianCalendar(
    #       const GregorianCalendar &source
    # )
    cal10 = icu.GregorianCalendar(cal9)
    assert cal10.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal10.get_time_zone() == zone2
    assert cal10.get_time() - offset == 1215268215000.0  # 2008-07-05T23:30:15Z


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 73, reason="ICU4C<73")
def test_in_temporal_leap_year() -> None:
    gc = icu.GregorianCalendar()
    gc.clear()
    gc.set(2024, icu.UCalendarMonths.UCAL_JANUARY, 1)

    # virtual bool icu::Calendar::inTemporalLeapYear(UErrorCode &status) const
    loc = icu.Locale(icu.Locale.get_root())
    loc.set_keyword_value("calendar", "japanese")
    cal = icu.Calendar.create_instance(loc)
    cal.set_time(gc.get_time())
    assert cal.in_temporal_leap_year()

    gc.set(2023, icu.UCalendarMonths.UCAL_JANUARY, 1)
    cal.set_time(gc.get_time())
    assert not cal.in_temporal_leap_year()


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 73, reason="ICU4C<73")
def test_set_temporal_month_code() -> None:
    gc = icu.GregorianCalendar()
    gc.clear()
    gc.set(2022, icu.UCalendarMonths.UCAL_JANUARY, 11)

    # virtual const char * icu::Calendar::getTemporalMonthCode(UErrorCode &status) const
    loc = icu.Locale(icu.Locale.get_root())
    loc.set_keyword_value("calendar", "hebrew")
    cal = icu.Calendar.create_instance(loc)
    cal.set_time(gc.get_time())
    code = cal.get_temporal_month_code()
    assert isinstance(code, str)
    assert code == "M05"

    gc.set(2022, icu.UCalendarMonths.UCAL_FEBRUARY, 12)
    cal.set_time(gc.get_time())
    code = cal.get_temporal_month_code()
    assert isinstance(code, str)
    assert code == "M05L"

    # virtual void icu::Calendar::setTemporalMonthCode(
    #       const char *temporalMonth,
    #       UErrorCode &status
    # )
    cal.set_temporal_month_code("M05")
    code = cal.get_temporal_month_code()
    assert isinstance(code, str)
    assert code == "M05"

    with pytest.raises(TypeError):
        cal.set_temporal_month_code(None)

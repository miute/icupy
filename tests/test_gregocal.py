import copy

import pytest
from icupy import (
    Calendar, GregorianCalendar, Locale, SimpleTimeZone, StringEnumeration,
    TimeZone, UCalendarDateFields, UCalendarDaysOfWeek, UCalendarMonths,
    UCalendarWallTimeOption, UCalendarWeekdayType, ULocDataLocaleType,
    U_ICU_VERSION_MAJOR_NUM, U_MILLIS_PER_HOUR as HOUR,
)


def test_api():
    result = Calendar.get_available_locales()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, Locale) for x in result)

    it1 = Calendar.get_keyword_values_for_locale(
        "calendar",
        Locale("he"),
        True)
    assert isinstance(it1, StringEnumeration)
    assert "hebrew" in it1
    it2 = Calendar.get_keyword_values_for_locale(
        "calendar",
        "he",
        True)
    assert isinstance(it2, StringEnumeration)
    assert "hebrew" in it2

    result = Calendar.get_now()
    assert isinstance(result, float)
    assert result > 0

    when = 1215300615000.0  # 2008-07-05T23:30:15Z
    when2 = 1215304215000.0  # 2008-07-06T00:30:15Z
    when3 = 1215097200000.0  # 2008-07-04T00:00:00Z
    zone = TimeZone.get_gmt()
    zone2 = TimeZone.create_default()
    cal = Calendar.create_instance("en")
    assert isinstance(cal, GregorianCalendar)
    assert cal.get_time_zone() == zone2

    cal.set_time_zone(zone)
    assert cal.get_time_zone() == zone

    cal.set_time(when)
    assert cal.get_time() == when

    cal2 = cal.clone()
    cal2.add(UCalendarDateFields.UCAL_HOUR_OF_DAY, 1)
    assert cal2.get_time() == when2  # 2008-07-06T00:30:15Z

    assert not cal.after(cal2)
    assert cal2.after(cal)

    assert cal.before(cal2)
    assert not cal2.before(cal)

    cal3 = cal2.clone()
    cal3.clear(UCalendarDateFields.UCAL_SECOND)
    assert cal3.get_time() == 1215304200000.0  # 2008-07-06T00:30:00Z
    cal3.clear()
    assert cal3.get_time() == 0

    cal4 = GregorianCalendar(zone, "en")
    cal4.set_time(when)

    assert not cal.equals(cal2)
    assert not cal.equals(cal3)
    assert cal.equals(cal4)

    assert cal.field_difference(
        when2,
        UCalendarDateFields.UCAL_HOUR_OF_DAY) == 1
    assert cal.field_difference(
        when2,
        UCalendarDateFields.UCAL_MINUTE) == 0

    year = 2008
    month = UCalendarMonths.UCAL_JULY
    date = 5
    hour = 23
    minute = 30
    second = 15
    cal3.set(year, month, date)
    assert cal3.get(UCalendarDateFields.UCAL_ERA) == GregorianCalendar.AD
    assert cal3.get(UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(UCalendarDateFields.UCAL_HOUR_OF_DAY) == 0
    assert cal3.get(UCalendarDateFields.UCAL_MINUTE) == 0
    assert cal3.get(UCalendarDateFields.UCAL_SECOND) == 0

    cal3.set(year, month, date, hour, minute)
    assert cal3.get(UCalendarDateFields.UCAL_ERA) == GregorianCalendar.AD
    assert cal3.get(UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(UCalendarDateFields.UCAL_HOUR_OF_DAY) == hour
    assert cal3.get(UCalendarDateFields.UCAL_MINUTE) == minute
    assert cal3.get(UCalendarDateFields.UCAL_SECOND) == 0

    cal3.set(year, month, date, hour, minute, second)
    assert cal3.get(UCalendarDateFields.UCAL_ERA) == GregorianCalendar.AD
    assert cal3.get(UCalendarDateFields.UCAL_YEAR) == year
    assert cal3.get(UCalendarDateFields.UCAL_MONTH) == month
    assert cal3.get(UCalendarDateFields.UCAL_DATE) == date
    assert cal3.get(UCalendarDateFields.UCAL_HOUR_OF_DAY) == hour
    assert cal3.get(UCalendarDateFields.UCAL_MINUTE) == minute
    assert cal3.get(UCalendarDateFields.UCAL_SECOND) == second

    assert cal.get_actual_maximum(UCalendarDateFields.UCAL_DAY_OF_MONTH) == 31
    assert cal.get_least_maximum(UCalendarDateFields.UCAL_DAY_OF_MONTH) == 28
    assert cal.get_maximum(UCalendarDateFields.UCAL_DAY_OF_MONTH) == 31

    assert cal.get_actual_minimum(UCalendarDateFields.UCAL_DAY_OF_MONTH) == 1
    assert cal.get_greatest_minimum(UCalendarDateFields.UCAL_DAY_OF_MONTH) == 1
    assert cal.get_minimum(UCalendarDateFields.UCAL_DAY_OF_MONTH) == 1

    assert (cal.get_day_of_week_type(UCalendarDaysOfWeek.UCAL_SUNDAY)
            == UCalendarWeekdayType.UCAL_WEEKEND)

    assert cal2.get_first_day_of_week() == UCalendarDaysOfWeek.UCAL_SUNDAY
    cal2.set_first_day_of_week(UCalendarDaysOfWeek.UCAL_MONDAY)
    assert cal2.get_first_day_of_week() == UCalendarDaysOfWeek.UCAL_MONDAY

    result = cal.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert isinstance(result, Locale)
    assert result == Locale("en")

    assert cal2.get_minimal_days_in_first_week() == 1
    cal2.set_minimal_days_in_first_week(7)
    assert cal2.get_minimal_days_in_first_week() == 7

    result = cal.get_time_zone()
    assert isinstance(result, TimeZone)
    assert result == zone

    assert (cal.get_weekend_transition(UCalendarDaysOfWeek.UCAL_SUNDAY)
            == 24 * HOUR)

    assert not cal.in_daylight_time()
    assert not cal4.in_daylight_time()

    assert not cal.is_equivalent_to(cal2)
    assert cal.is_equivalent_to(cal3)
    assert cal.is_equivalent_to(cal4)

    assert cal2.is_lenient()
    cal2.set_lenient(False)
    assert not cal2.is_lenient()

    assert cal2.is_set(UCalendarDateFields.UCAL_YEAR)
    assert cal2.is_set(UCalendarDateFields.UCAL_MONTH)
    assert cal2.is_set(UCalendarDateFields.UCAL_DATE)

    assert cal2.is_weekend(when)
    assert not cal2.is_weekend(when3)
    assert cal2.is_weekend()

    assert cal != cal2
    assert cal != cal3
    assert cal != cal4
    assert not (cal == cal2)
    assert not (cal == cal3)
    assert not (cal == cal4)

    zone4 = cal2.orphan_time_zone()
    assert isinstance(zone4, TimeZone)
    assert zone4 != zone2
    assert cal2.get_time_zone() == zone2

    cal3.roll(UCalendarDateFields.UCAL_HOUR_OF_DAY, 1)
    # 2008-07-05T23:30:15 -> 2008-07-05T00:30:15Z
    assert cal3.get_time() == 1215217815000.0  # 2008-07-05T00:30:15Z

    # 1582-10-15T00:00:00Z
    assert cal4.get_gregorian_change() == -12219292800000.0
    cal4.set_gregorian_change(-2208988800000.0)  # 1900-01-01T00:00:00Z
    assert cal4.get_gregorian_change() == -2208988800000.0

    assert cal4.is_leap_year(2008)
    assert not cal4.is_leap_year(2009)


def test_calendar_create_instance():
    locale1 = Locale.get_english()
    locale2 = Locale.get_default()
    zone1 = SimpleTimeZone(8 * HOUR, "s2")
    zone2 = TimeZone.create_default()

    # [1]
    # static Calendar *Calendar::createInstance(
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal1 = Calendar.create_instance(locale1)
    assert isinstance(cal1, GregorianCalendar)
    assert cal1.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal1.get_time_zone() == zone2

    assert Calendar.create_instance("en").get_locale(
        ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1

    # [2]
    # static Calendar *Calendar::createInstance(
    #       const TimeZone &zone,
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal2 = Calendar.create_instance(zone1, locale1)
    assert isinstance(cal2, GregorianCalendar)
    assert cal2.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal2.get_time_zone() == zone1

    assert Calendar.create_instance(zone1, "en").get_locale(
        ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1

    # [3]
    # static Calendar *Calendar::createInstance(
    #       const TimeZone &zone,
    #       UErrorCode &success
    # )
    cal3 = Calendar.create_instance(zone1)
    assert isinstance(cal3, GregorianCalendar)
    assert cal3.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal3.get_time_zone() == zone1

    # [6]
    # static Calendar *Calendar::createInstance(UErrorCode &success)
    cal6 = Calendar.create_instance()
    assert isinstance(cal6, GregorianCalendar)
    assert cal6.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal6.get_time_zone() == zone2


def test_clone():
    cal1 = GregorianCalendar(TimeZone.get_gmt())
    cal1.set_time(1215298800000.0)  # 2008-07-05T23:00:00Z

    cal2 = cal1.clone()
    assert isinstance(cal2, GregorianCalendar)

    cal3 = copy.copy(cal1)

    cal4 = copy.deepcopy(cal1)
    assert cal1 == cal2 == cal3 == cal4


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_repeated_wall_time_option():
    zone = SimpleTimeZone(8 * HOUR, "s2")
    cal = Calendar.create_instance(zone)
    assert (cal.get_repeated_wall_time_option()
            == UCalendarWallTimeOption.UCAL_WALLTIME_LAST)

    cal.set_repeated_wall_time_option(
        UCalendarWallTimeOption.UCAL_WALLTIME_FIRST)
    assert (cal.get_repeated_wall_time_option()
            == UCalendarWallTimeOption.UCAL_WALLTIME_FIRST)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_skipped_wall_time_option():
    zone = SimpleTimeZone(8 * HOUR, "s2")
    cal = Calendar.create_instance(zone)
    assert (cal.get_skipped_wall_time_option()
            == UCalendarWallTimeOption.UCAL_WALLTIME_LAST)

    cal.set_skipped_wall_time_option(
        UCalendarWallTimeOption.UCAL_WALLTIME_FIRST)
    assert (cal.get_skipped_wall_time_option()
            == UCalendarWallTimeOption.UCAL_WALLTIME_FIRST)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_type():
    cal1 = Calendar.create_instance("ja")
    result = cal1.get_type()
    assert isinstance(result, str)
    assert result == "gregorian"

    cal2 = Calendar.create_instance("ja@calendar=japanese")
    result = cal2.get_type()
    assert isinstance(result, str)
    assert result == "japanese"


def test_gregorian_calendar():
    assert issubclass(GregorianCalendar, Calendar)
    locale1 = Locale.get_english()
    locale2 = Locale.get_default()
    zone1 = SimpleTimeZone(8 * HOUR, "s2")
    zone2 = TimeZone.create_default()

    # [1]
    # GregorianCalendar::GregorianCalendar(UErrorCode &success)
    cal1 = GregorianCalendar()
    assert cal1.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal1.get_time_zone() == zone2

    # [3]
    # GregorianCalendar::GregorianCalendar(
    #       const TimeZone &zone,
    #       UErrorCode &success
    # )
    cal3 = GregorianCalendar(zone1)
    assert cal3.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal3.get_time_zone() == zone1

    # [4]
    # GregorianCalendar::GregorianCalendar(
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal4 = GregorianCalendar(locale1)
    assert cal4.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal4.get_time_zone() == zone2

    # [6]
    # GregorianCalendar::GregorianCalendar(
    #       const TimeZone &zone,
    #       const Locale &aLocale,
    #       UErrorCode &success
    # )
    cal6 = GregorianCalendar(zone1, locale1)
    assert cal6.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == locale1
    assert cal6.get_time_zone() == zone1

    # [7]
    # GregorianCalendar::GregorianCalendar(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       UErrorCode &success
    # )
    cal7 = GregorianCalendar(2008, UCalendarMonths.UCAL_JULY, 5)
    assert cal7.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal7.get_time_zone() == zone2
    assert cal7.get_time() == 1215183600000.0  # 2008-07-05T00:00:00Z

    # [8]
    # GregorianCalendar::GregorianCalendar(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       int32_t hour,
    #       int32_t minute,
    #       UErrorCode &success
    # )
    cal8 = GregorianCalendar(2008, UCalendarMonths.UCAL_JULY, 5, 23, 30)
    assert cal8.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal8.get_time_zone() == zone2
    assert cal8.get_time() == 1215268200000.0  # 2008-07-05T23:30:00Z

    # [9]
    # GregorianCalendar::GregorianCalendar(
    #       int32_t year,
    #       int32_t month,
    #       int32_t date,
    #       int32_t hour,
    #       int32_t minute,
    #       int32_t second,
    #       UErrorCode &success
    # )
    cal9 = GregorianCalendar(2008, UCalendarMonths.UCAL_JULY, 5, 23, 30, 15)
    assert cal9.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal9.get_time_zone() == zone2
    assert cal9.get_time() == 1215268215000.0  # 2008-07-05T23:30:15Z

    # [10]
    # GregorianCalendar::GregorianCalendar(const GregorianCalendar &source)
    cal10 = GregorianCalendar(cal9)
    assert cal10.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE) == locale2
    assert cal10.get_time_zone() == zone2
    assert cal10.get_time() == 1215268215000.0  # 2008-07-05T23:30:15Z

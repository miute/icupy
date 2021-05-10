from icupy import (
    AnnualTimeZoneRule, DateTimeRule, InitialTimeZoneRule,
    TimeArrayTimeZoneRule, TimeZoneRule, TimeZoneTransition,
    UCalendarDaysOfWeek, UCalendarMonths, U_MILLIS_PER_HOUR as HOUR,
    UnicodeString,
)


def test_annual_time_zone_rule():
    assert issubclass(AnnualTimeZoneRule, TimeZoneRule)
    assert isinstance(AnnualTimeZoneRule.MAX_YEAR, int)
    assert AnnualTimeZoneRule.MAX_YEAR > 0

    dr = DateTimeRule(
        UCalendarMonths.UCAL_FEBRUARY,
        11,
        1 * HOUR,
        DateTimeRule.WALL_TIME)

    # [1]
    # AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings,
    #       const DateTimeRule &dateTimeRule,
    #       int32_t startYear,
    #       int32_t endYear
    # )
    ar1 = AnnualTimeZoneRule(
        UnicodeString("a1"),
        -1 * HOUR,
        1 * HOUR,
        dr,
        1966,
        AnnualTimeZoneRule.MAX_YEAR)

    # [2]
    # AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings,
    #       DateTimeRule *dateTimeRule,
    #       int32_t startYear,
    #       int32_t endYear
    # )
    ar2 = AnnualTimeZoneRule(
        UnicodeString("a2"),
        -1 * HOUR,
        1 * HOUR,
        dr.clone(),
        1966,
        AnnualTimeZoneRule.MAX_YEAR)

    # [3]
    # AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const AnnualTimeZoneRule &source
    # )
    ar3 = AnnualTimeZoneRule(ar1)

    ar4 = ar1.clone()

    name = UnicodeString()
    result = ar1.get_name(name)
    assert isinstance(result, UnicodeString)
    assert result == name
    assert result == "a1"

    assert ar1.get_raw_offset() == -1 * HOUR
    assert ar1.get_dst_savings() == 1 * HOUR

    assert ar1.get_end_year() == AnnualTimeZoneRule.MAX_YEAR
    assert ar1.get_rule() == dr
    assert ar1.get_start_year() == 1966

    # UBool AnnualTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result1 = ar1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert not available
    assert isinstance(result1, (int, float))

    # UBool AnnualTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result2 = ar1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result2, (int, float))

    # UBool AnnualTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result3 = ar1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available
    assert isinstance(result3, (int, float))

    # UBool AnnualTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result4 = ar1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available
    assert isinstance(result4, (int, float))

    # UBool AnnualTimeZoneRule::getStartInYear(
    #       int32_t year,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result5 = ar1.get_start_in_year(1966, -1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result5, (int, float))

    assert ar1.is_equivalent_to(ar2)
    assert ar1.is_equivalent_to(ar3)
    assert ar1.is_equivalent_to(ar4)

    assert not (ar1 != ar2)
    assert not (ar1 != ar3)
    assert not (ar1 != ar4)
    assert not (ar2 != ar3)
    assert not (ar2 != ar4)
    assert not (ar3 != ar4)

    assert ar1 == ar2 == ar3 == ar4


def test_date_time_rule():
    # [1]
    # DateTimeRule::DateTimeRule(int32_t month,
    #                            int32_t dayOfMonth,
    #                            int32_t millisInDay,
    #                            TimeRuleType timeType)
    dr1 = DateTimeRule(
        UCalendarMonths.UCAL_FEBRUARY,
        11,
        1 * HOUR,
        DateTimeRule.WALL_TIME)
    assert dr1.get_date_rule_type() == DateTimeRule.DOM
    assert dr1.get_rule_day_of_month() == 11
    assert dr1.get_rule_day_of_week() == 0
    assert dr1.get_rule_millis_in_day() == 1 * HOUR
    assert dr1.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY
    assert dr1.get_rule_week_in_month() == 0
    assert dr1.get_time_rule_type() == DateTimeRule.WALL_TIME

    # [2]
    # DateTimeRule::DateTimeRule(int32_t month,
    #                            int32_t weekInMonth,
    #                            int32_t dayOfWeek,
    #                            int32_t millisInDay,
    #                            TimeRuleType timeType)
    dr2 = DateTimeRule(
        UCalendarMonths.UCAL_FEBRUARY,
        -1,
        UCalendarDaysOfWeek.UCAL_SATURDAY,
        1 * HOUR,
        DateTimeRule.WALL_TIME)
    assert dr2.get_date_rule_type() == DateTimeRule.DOW
    assert dr2.get_rule_day_of_month() == 0
    assert dr2.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_SATURDAY
    assert dr2.get_rule_millis_in_day() == 1 * HOUR
    assert dr2.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY
    assert dr2.get_rule_week_in_month() == -1
    assert dr2.get_time_rule_type() == DateTimeRule.WALL_TIME

    # [3]
    # DateTimeRule::DateTimeRule(int32_t month,
    #                            int32_t dayOfMonth,
    #                            int32_t dayOfWeek,
    #                            UBool after,
    #                            int32_t millisInDay,
    #                            TimeRuleType timeType)
    dr3 = DateTimeRule(
        UCalendarMonths.UCAL_FEBRUARY,
        11,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        True,
        1 * HOUR,
        DateTimeRule.WALL_TIME)
    assert dr3.get_date_rule_type() == DateTimeRule.DOW_GEQ_DOM
    assert dr3.get_rule_day_of_month() == 11
    assert dr3.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_SUNDAY
    assert dr3.get_rule_millis_in_day() == 1 * HOUR
    assert dr3.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY
    assert dr3.get_rule_week_in_month() == 0
    assert dr3.get_time_rule_type() == DateTimeRule.WALL_TIME

    # [4]
    # DateTimeRule::DateTimeRule(const DateTimeRule &source)
    dr4 = DateTimeRule(dr1)
    assert dr4.get_date_rule_type() == DateTimeRule.DOM
    assert dr4.get_rule_day_of_month() == 11
    assert dr4.get_rule_day_of_week() == 0
    assert dr4.get_rule_millis_in_day() == 1 * HOUR
    assert dr4.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY
    assert dr4.get_rule_week_in_month() == 0
    assert dr4.get_time_rule_type() == DateTimeRule.WALL_TIME

    dr5 = dr1.clone()

    assert dr1 != dr2
    assert dr1 != dr3
    assert not (dr1 != dr4)
    assert not (dr1 != dr5)
    assert dr2 != dr3
    assert dr2 != dr4
    assert dr2 != dr5
    assert dr3 != dr4
    assert dr3 != dr5
    assert not (dr4 != dr5)

    assert not (dr1 == dr2)
    assert not (dr1 == dr3)
    assert dr1 == dr4
    assert dr1 == dr5
    assert not (dr2 == dr3)
    assert not (dr2 == dr4)
    assert not (dr2 == dr5)
    assert not (dr3 == dr4)
    assert not (dr3 == dr5)
    assert dr4 == dr5


def test_initial_time_zone_rule():
    assert issubclass(InitialTimeZoneRule, TimeZoneRule)

    # [1]
    # InitialTimeZoneRule::InitialTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings
    # )
    ir1 = InitialTimeZoneRule(UnicodeString("i1"), -1 * HOUR, 1 * HOUR)

    # [2]
    # InitialTimeZoneRule::InitialTimeZoneRule(
    #       const InitialTimeZoneRule &source
    # )
    ir2 = InitialTimeZoneRule(ir1)

    ir3 = ir1.clone()

    name = UnicodeString()
    result = ir1.get_name(name)
    assert isinstance(result, UnicodeString)
    assert result == name
    assert result == "i1"

    assert ir1.get_raw_offset() == -1 * HOUR
    assert ir1.get_dst_savings() == 1 * HOUR

    # UBool InitialTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, _ = ir1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert not available

    # UBool InitialTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, _ = ir1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert not available

    # UBool InitialTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, _ = ir1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert not available

    # UBool InitialTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, _ = ir1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert not available

    assert ir1.is_equivalent_to(ir2)
    assert ir1.is_equivalent_to(ir3)

    assert not (ir1 != ir2)
    assert not (ir1 != ir3)
    assert not (ir2 != ir3)

    assert ir1 == ir2 == ir3


def test_time_array_time_zone_rule():
    assert issubclass(TimeArrayTimeZoneRule, TimeZoneRule)
    start_times = [0.0, 10000000.0]

    # [1]
    # TimeArrayTimeZoneRule::TimeArrayTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings,
    #       const UDate *startTimes,
    #       int32_t numStartTimes,
    #       DateTimeRule::TimeRuleType timeRuleType
    # )
    tr1 = TimeArrayTimeZoneRule(
        UnicodeString("t1"),
        -1 * HOUR,
        1 * HOUR,
        start_times,
        len(start_times),
        DateTimeRule.UTC_TIME)

    # [2]
    # TimeArrayTimeZoneRule::TimeArrayTimeZoneRule(
    #       const TimeArrayTimeZoneRule &source
    # )
    tr2 = TimeArrayTimeZoneRule(tr1)

    tr3 = tr1.clone()

    name = UnicodeString()
    result = tr1.get_name(name)
    assert isinstance(result, UnicodeString)
    assert result == name
    assert result == "t1"

    assert tr1.get_raw_offset() == -1 * HOUR
    assert tr1.get_dst_savings() == 1 * HOUR

    assert tr1.count_start_times() == len(start_times)

    # UBool TimeArrayTimeZoneRule::getStartTimeAt(
    #       int32_t index,
    #       UDate &result
    # )
    valid, result = tr1.get_start_time_at(0)
    assert (valid, result) == (True, start_times[0])
    valid, result = tr1.get_start_time_at(1)
    assert (valid, result) == (True, start_times[1])
    valid, result = tr1.get_start_time_at(2)
    assert not valid

    assert tr1.get_time_type() == DateTimeRule.UTC_TIME

    # UBool TimeArrayTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result1 = tr1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result1, (int, float))

    # UBool TimeArrayTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result2 = tr1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result2, (int, float))

    # UBool TimeArrayTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result3 = tr1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available
    assert isinstance(result3, (int, float))

    # UBool TimeArrayTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result4 = tr1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert not available
    assert isinstance(result4, (int, float))

    assert tr1.is_equivalent_to(tr2)
    assert tr1.is_equivalent_to(tr3)

    assert not (tr1 != tr2)
    assert not (tr1 != tr3)
    assert not (tr2 != tr3)

    assert tr1 == tr2 == tr3


def test_time_zone_transition():
    dr1 = DateTimeRule(
        UCalendarMonths.UCAL_JULY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        DateTimeRule.WALL_TIME)
    dr2 = DateTimeRule(
        UCalendarMonths.UCAL_JANUARY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        DateTimeRule.WALL_TIME)
    tzr1 = AnnualTimeZoneRule(
        "from",
        0,
        0,
        dr1,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    tzr2 = AnnualTimeZoneRule(
        "to",
        0,
        0,
        dr2,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    time = 1230681600000.0  # 2008-12-31T00:00:00

    # [1]
    # TimeZoneTransition::TimeZoneTransition(
    #       UDate time,
    #       const TimeZoneRule &from,
    #       const TimeZoneRule &to
    # )
    tzt1 = TimeZoneTransition(time, tzr1, tzr2)

    # [2]
    # TimeZoneTransition::TimeZoneTransition()
    tzt2 = TimeZoneTransition()

    # [3]
    # TimeZoneTransition::TimeZoneTransition(
    #       const TimeZoneTransition &source
    # )
    tzt3 = TimeZoneTransition(tzt2)

    tzt4 = tzt1.clone()

    from_ = tzt1.get_from()
    assert isinstance(from_, AnnualTimeZoneRule)
    assert from_ == tzr1
    assert from_ != tzr2
    assert tzt2.get_from() is None

    assert tzt1.get_time() == time
    assert tzt2.get_time() == 0

    to = tzt1.get_to()
    assert isinstance(to, AnnualTimeZoneRule)
    assert to != tzr1
    assert to == tzr2
    assert tzt2.get_to() is None

    assert tzt1 != tzt2
    assert tzt1 != tzt3
    assert not (tzt1 != tzt4)
    assert not (tzt2 != tzt3)
    assert tzt2 != tzt4
    assert tzt3 != tzt4

    assert not (tzt1 == tzt2)
    assert not (tzt1 == tzt3)
    assert tzt1 == tzt4
    assert tzt2 == tzt3
    assert not (tzt2 == tzt4)
    assert not (tzt3 == tzt4)

    tzt2.set_from(from_)
    tzt2.set_to(to)
    tzt2.set_time(time)
    assert tzt1 == tzt2

    tzt3.set_from(from_)
    tzt3.set_to(to)
    tzt3.set_time(time)
    assert tzt1 == tzt3

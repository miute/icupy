from icupy.icu import (
    AnnualTimeZoneRule, DateTimeRule, InitialTimeZoneRule,
    TimeArrayTimeZoneRule, TimeZoneRule, TimeZoneTransition,
    UCalendarDaysOfWeek, UCalendarMonths, U_MILLIS_PER_HOUR as HOUR,
    UnicodeString,
)


def test_annual_time_zone_rule():
    assert issubclass(AnnualTimeZoneRule, TimeZoneRule)
    assert isinstance(AnnualTimeZoneRule.MAX_YEAR, int)
    assert AnnualTimeZoneRule.MAX_YEAR > 0

    date_time_rule = DateTimeRule(
        UCalendarMonths.UCAL_FEBRUARY,
        11,
        1 * HOUR,
        DateTimeRule.WALL_TIME)

    # [1]
    # icu::AnnualTimeZoneRule::AnnualTimeZoneRule(
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
        date_time_rule,
        1966,
        AnnualTimeZoneRule.MAX_YEAR)

    # [2]
    # icu::AnnualTimeZoneRule::AnnualTimeZoneRule(
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
        date_time_rule.clone(),
        1966,
        AnnualTimeZoneRule.MAX_YEAR)

    # [3]
    # icu::AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const AnnualTimeZoneRule &source
    # )
    ar3 = AnnualTimeZoneRule(ar1)

    # AnnualTimeZoneRule *icu::AnnualTimeZoneRule::clone()
    ar4 = ar1.clone()
    assert isinstance(ar4, AnnualTimeZoneRule)

    # UnicodeString &icu::TimeZoneRule::getName(UnicodeString &name)
    name = UnicodeString()
    result = ar1.get_name(name)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(name)
    assert result == "a1"

    # int32_t icu::TimeZoneRule::getRawOffset(void)
    assert ar1.get_raw_offset() == -1 * HOUR

    # int32_t icu::TimeZoneRule::getDSTSavings(void)
    assert ar1.get_dst_savings() == 1 * HOUR

    # int32_t icu::AnnualTimeZoneRule::getEndYear(void)
    assert ar1.get_end_year() == AnnualTimeZoneRule.MAX_YEAR

    # const DateTimeRule *icu::AnnualTimeZoneRule::getRule(void)
    assert ar1.get_rule() == date_time_rule

    # int32_t icu::AnnualTimeZoneRule::getStartYear(void)
    assert ar1.get_start_year() == 1966

    # UBool icu::AnnualTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result1 = ar1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert not available
    assert isinstance(result1, (int, float))

    # UBool icu::AnnualTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result2 = ar1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result2, (int, float))

    # UBool icu::AnnualTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result3 = ar1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available
    assert isinstance(result3, (int, float))

    # UBool icu::AnnualTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result4 = ar1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available
    assert isinstance(result4, (int, float))

    # UBool icu::AnnualTimeZoneRule::getStartInYear(
    #       int32_t year,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result5 = ar1.get_start_in_year(1966, -1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result5, (int, float))

    # UBool icu::AnnualTimeZoneRule::isEquivalentTo(const TimeZoneRule &that)
    assert ar1.is_equivalent_to(ar2)
    assert ar1.is_equivalent_to(ar3)
    assert ar1.is_equivalent_to(ar4)

    # UBool icu::AnnualTimeZoneRule::operator!=(const TimeZoneRule &that)
    assert not (ar1 != ar2)
    assert not (ar1 != ar3)
    assert not (ar1 != ar4)
    assert not (ar2 != ar3)
    assert not (ar2 != ar4)
    assert not (ar3 != ar4)

    # UBool icu::AnnualTimeZoneRule::operator==(const TimeZoneRule &that)
    assert ar1 == ar2 == ar3 == ar4


def test_date_time_rule():
    # [1]
    # icu::DateTimeRule::DateTimeRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t millisInDay,
    #       TimeRuleType timeType
    # )
    dr1 = DateTimeRule(
        UCalendarMonths.UCAL_FEBRUARY,
        11,
        1 * HOUR,
        DateTimeRule.WALL_TIME)

    # DateRuleType icu::DateTimeRule::getDateRuleType(void)
    assert dr1.get_date_rule_type() == DateTimeRule.DOM

    # int32_t icu::DateTimeRule::getRuleDayOfMonth(void)
    assert dr1.get_rule_day_of_month() == 11

    # int32_t icu::DateTimeRule::getRuleDayOfWeek(void)
    assert dr1.get_rule_day_of_week() == 0

    # int32_t icu::DateTimeRule::getRuleMillisInDay(void)
    assert dr1.get_rule_millis_in_day() == 1 * HOUR

    # int32_t icu::DateTimeRule::getRuleMonth(void)
    assert dr1.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY

    # int32_t icu::DateTimeRule::getRuleWeekInMonth(void)
    assert dr1.get_rule_week_in_month() == 0

    # TimeRuleType icu::DateTimeRule::getTimeRuleType(void)
    assert dr1.get_time_rule_type() == DateTimeRule.WALL_TIME

    # [2]
    # icu::DateTimeRule::DateTimeRule(
    #       int32_t month,
    #       int32_t weekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t millisInDay,
    #       TimeRuleType timeType
    # )
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
    # icu::DateTimeRule::DateTimeRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t dayOfWeek,
    #       UBool after,
    #       int32_t millisInDay,
    #       TimeRuleType timeType
    # )
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
    # icu::DateTimeRule::DateTimeRule(const DateTimeRule &source)
    dr4 = DateTimeRule(dr1)
    assert dr4.get_date_rule_type() == DateTimeRule.DOM
    assert dr4.get_rule_day_of_month() == 11
    assert dr4.get_rule_day_of_week() == 0
    assert dr4.get_rule_millis_in_day() == 1 * HOUR
    assert dr4.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY
    assert dr4.get_rule_week_in_month() == 0
    assert dr4.get_time_rule_type() == DateTimeRule.WALL_TIME

    # DateTimeRule *icu::DateTimeRule::clone()
    dr5 = dr1.clone()
    assert isinstance(dr5, DateTimeRule)

    # UBool icu::DateTimeRule::operator!=(const DateTimeRule &that)
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

    # UBool icu::DateTimeRule::operator==(const DateTimeRule &that)
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
    # icu::InitialTimeZoneRule::InitialTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings
    # )
    ir1 = InitialTimeZoneRule(UnicodeString("i1"), -1 * HOUR, 1 * HOUR)

    # [2]
    # icu::InitialTimeZoneRule::InitialTimeZoneRule(
    #       const InitialTimeZoneRule &source
    # )
    ir2 = InitialTimeZoneRule(ir1)

    # InitialTimeZoneRule *icu::InitialTimeZoneRule::clone()
    ir3 = ir1.clone()
    assert isinstance(ir3, InitialTimeZoneRule)

    # UnicodeString &icu::TimeZoneRule::getName(UnicodeString &name)
    name = UnicodeString()
    result = ir1.get_name(name)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(name)
    assert result == "i1"

    # int32_t icu::TimeZoneRule::getRawOffset(void)
    assert ir1.get_raw_offset() == -1 * HOUR

    # int32_t icu::TimeZoneRule::getDSTSavings(void)
    assert ir1.get_dst_savings() == 1 * HOUR

    # UBool icu::InitialTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, _ = ir1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert not available

    # UBool icu::InitialTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, _ = ir1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert not available

    # UBool icu::InitialTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, _ = ir1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert not available

    # UBool icu::InitialTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, _ = ir1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert not available

    # UBool icu::InitialTimeZoneRule::isEquivalentTo(const TimeZoneRule &that)
    assert ir1.is_equivalent_to(ir2)
    assert ir1.is_equivalent_to(ir3)

    # UBool icu::InitialTimeZoneRule::operator!=(const TimeZoneRule &that)
    assert not (ir1 != ir2)
    assert not (ir1 != ir3)
    assert not (ir2 != ir3)

    # UBool icu::InitialTimeZoneRule::operator==(const TimeZoneRule &that)
    assert ir1 == ir2 == ir3


def test_time_array_time_zone_rule():
    assert issubclass(TimeArrayTimeZoneRule, TimeZoneRule)
    start_times = [0.0, 10000000.0]

    # [1]
    # icu::TimeArrayTimeZoneRule::TimeArrayTimeZoneRule(
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
    # icu::TimeArrayTimeZoneRule::TimeArrayTimeZoneRule(
    #       const TimeArrayTimeZoneRule &source
    # )
    tr2 = TimeArrayTimeZoneRule(tr1)

    # TimeArrayTimeZoneRule *icu::TimeArrayTimeZoneRule::clone()
    tr3 = tr1.clone()
    assert isinstance(tr3, TimeArrayTimeZoneRule)

    # UnicodeString &icu::TimeZoneRule::getName(UnicodeString &name)
    name = UnicodeString()
    result = tr1.get_name(name)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(name)
    assert result == "t1"

    # int32_t icu::TimeZoneRule::getRawOffset(void)
    assert tr1.get_raw_offset() == -1 * HOUR

    # int32_t icu::TimeZoneRule::getDSTSavings(void)
    assert tr1.get_dst_savings() == 1 * HOUR

    # int32_t icu::TimeArrayTimeZoneRule::countStartTimes(void)
    assert tr1.count_start_times() == len(start_times)

    # UBool icu::TimeArrayTimeZoneRule::getStartTimeAt(
    #       int32_t index,
    #       UDate &result
    # )
    valid, result = tr1.get_start_time_at(0)
    assert (valid, result) == (True, start_times[0])

    valid, result = tr1.get_start_time_at(1)
    assert (valid, result) == (True, start_times[1])

    valid, result = tr1.get_start_time_at(2)
    assert not valid

    # DateTimeRule::TimeRuleType icu::TimeArrayTimeZoneRule::getTimeType(void)
    assert tr1.get_time_type() == DateTimeRule.UTC_TIME

    # UBool icu::TimeArrayTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result1 = tr1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result1, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result2 = tr1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available
    assert isinstance(result2, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result3 = tr1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available
    assert isinstance(result3, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result4 = tr1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert not available
    assert isinstance(result4, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::isEquivalentTo(
    #       const TimeZoneRule &that
    # )
    assert tr1.is_equivalent_to(tr2)
    assert tr1.is_equivalent_to(tr3)

    # UBool icu::TimeArrayTimeZoneRule::operator!=(const TimeZoneRule &that)
    assert not (tr1 != tr2)
    assert not (tr1 != tr3)
    assert not (tr2 != tr3)

    # UBool icu::TimeArrayTimeZoneRule::operator==(const TimeZoneRule &that)
    assert tr1 == tr2 == tr3


def test_time_zone_transition():
    date_time_rule1 = DateTimeRule(
        UCalendarMonths.UCAL_JULY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        DateTimeRule.WALL_TIME)
    date_time_rule2 = DateTimeRule(
        UCalendarMonths.UCAL_JANUARY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        DateTimeRule.WALL_TIME)
    from_ = AnnualTimeZoneRule(
        "from",
        0,
        0,
        date_time_rule1,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    to = AnnualTimeZoneRule(
        "to",
        0,
        0,
        date_time_rule2,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    time = 1230681600000.0  # 2008-12-31T00:00:00

    # [1]
    # icu::TimeZoneTransition::TimeZoneTransition(
    #       UDate time,
    #       const TimeZoneRule &from,
    #       const TimeZoneRule &to
    # )
    tzt1 = TimeZoneTransition(time, from_, to)

    # [2]
    # icu::TimeZoneTransition::TimeZoneTransition()
    tzt2 = TimeZoneTransition()

    # [3]
    # icu::TimeZoneTransition::TimeZoneTransition(
    #       const TimeZoneTransition &source
    # )
    tzt3 = TimeZoneTransition(tzt2)

    # TimeZoneTransition *icu::TimeZoneTransition::clone()
    tzt4 = tzt1.clone()
    assert isinstance(tzt4, TimeZoneTransition)

    # const TimeZoneRule *icu::TimeZoneTransition::getFrom(void)
    result = tzt1.get_from()
    assert isinstance(result, AnnualTimeZoneRule)
    assert result == from_

    assert tzt2.get_from() is None

    # UDate icu::TimeZoneTransition::getTime(void)
    assert tzt1.get_time() == time
    assert tzt2.get_time() == 0

    # const TimeZoneRule *icu::TimeZoneTransition::getTo(void)
    result = tzt1.get_to()
    assert isinstance(result, AnnualTimeZoneRule)
    assert result == to

    assert tzt2.get_to() is None

    # UBool icu::TimeZoneTransition::operator!=(const TimeZoneTransition &that)
    assert tzt1 != tzt2
    assert tzt1 != tzt3
    assert not (tzt1 != tzt4)
    assert not (tzt2 != tzt3)
    assert tzt2 != tzt4
    assert tzt3 != tzt4

    # UBool icu::TimeZoneTransition::operator==(const TimeZoneTransition &that)
    assert not (tzt1 == tzt2)
    assert not (tzt1 == tzt3)
    assert tzt1 == tzt4
    assert tzt2 == tzt3
    assert not (tzt2 == tzt4)
    assert not (tzt3 == tzt4)

    # void icu::TimeZoneTransition::setFrom(const TimeZoneRule &from)
    tzt2.set_from(from_)

    # void icu::TimeZoneTransition::setTo(const TimeZoneRule &to)
    tzt2.set_to(to)

    # void icu::TimeZoneTransition::setTime(UDate time)
    tzt2.set_time(time)
    assert tzt1 == tzt2

    tzt3.set_from(from_)
    tzt3.set_to(to)
    tzt3.set_time(time)
    assert tzt1 == tzt3

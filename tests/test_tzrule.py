import copy

from icupy import icu
from icupy.icu import U_MILLIS_PER_HOUR as HOUR


def test_annual_time_zone_rule() -> None:
    assert issubclass(icu.AnnualTimeZoneRule, icu.TimeZoneRule)
    assert isinstance(icu.AnnualTimeZoneRule.MAX_YEAR, int)
    assert icu.AnnualTimeZoneRule.MAX_YEAR > 0

    date_time_rule = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_FEBRUARY,
        11,
        1 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )

    # [1]
    # icu::AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings,
    #       const DateTimeRule &dateTimeRule,
    #       int32_t startYear,
    #       int32_t endYear
    # )
    ar1 = icu.AnnualTimeZoneRule(
        icu.UnicodeString("a1"),
        -1 * HOUR,
        1 * HOUR,
        date_time_rule,
        1966,
        icu.AnnualTimeZoneRule.MAX_YEAR,
    )

    # [2]
    # icu::AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings,
    #       DateTimeRule *dateTimeRule,
    #       int32_t startYear,
    #       int32_t endYear
    # )
    ar2 = icu.AnnualTimeZoneRule(
        icu.UnicodeString("a2"),
        -1 * HOUR,
        1 * HOUR,
        date_time_rule.clone(),
        1966,
        icu.AnnualTimeZoneRule.MAX_YEAR,
    )

    # [3]
    # icu::AnnualTimeZoneRule::AnnualTimeZoneRule(
    #       const AnnualTimeZoneRule &source
    # )
    ar3 = icu.AnnualTimeZoneRule(ar1)

    # AnnualTimeZoneRule *icu::AnnualTimeZoneRule::clone()
    ar4 = ar1.clone()
    assert isinstance(ar4, icu.AnnualTimeZoneRule)

    # AnnualTimeZoneRule.__copy__() -> AnnualTimeZoneRule
    # AnnualTimeZoneRule.__deepcopy__(Optional[memo]) -> AnnualTimeZoneRule
    ar4a = copy.copy(ar4)
    ar4b = copy.deepcopy(ar4)
    assert ar4 == ar4a == ar4b

    # UnicodeString &icu::TimeZoneRule::getName(UnicodeString &name)
    name = icu.UnicodeString()
    result = ar1.get_name(name)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "a1"

    # int32_t icu::TimeZoneRule::getRawOffset(void)
    assert ar1.get_raw_offset() == -1 * HOUR

    # int32_t icu::TimeZoneRule::getDSTSavings(void)
    assert ar1.get_dst_savings() == 1 * HOUR

    # int32_t icu::AnnualTimeZoneRule::getEndYear(void)
    assert ar1.get_end_year() == icu.AnnualTimeZoneRule.MAX_YEAR

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
    assert available is False
    assert isinstance(result1, (int, float))

    # UBool icu::AnnualTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result2 = ar1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available is True
    assert isinstance(result2, (int, float))

    # UBool icu::AnnualTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result3 = ar1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available is True
    assert isinstance(result3, (int, float))

    # UBool icu::AnnualTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result4 = ar1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available is True
    assert isinstance(result4, (int, float))

    # UBool icu::AnnualTimeZoneRule::getStartInYear(
    #       int32_t year,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result5 = ar1.get_start_in_year(1966, -1 * HOUR, 1 * HOUR)
    assert available is True
    assert isinstance(result5, (int, float))

    # UBool icu::AnnualTimeZoneRule::isEquivalentTo(const TimeZoneRule &that)
    assert ar1.is_equivalent_to(ar2) is True
    assert ar1.is_equivalent_to(ar3) is True
    assert ar1.is_equivalent_to(ar4) is True

    # UBool icu::AnnualTimeZoneRule::operator!=(const TimeZoneRule &that)
    assert not (ar1 != ar2)
    assert not (ar1 != ar3)
    assert not (ar1 != ar4)
    assert not (ar2 != ar3)
    assert not (ar2 != ar4)
    assert not (ar3 != ar4)

    # UBool icu::AnnualTimeZoneRule::operator==(const TimeZoneRule &that)
    assert ar1 == ar2 == ar3 == ar4

    # AnnualTimeZoneRule.__repr__() -> str
    assert repr(ar1) == (
        "<AnnualTimeZoneRule("
        "name='a1', raw_offset=-3600000, dst_savings=3600000, "
        "start_year=1966, end_year=2147483647"
        ")>"
    )


def test_date_time_rule() -> None:
    # [1]
    # icu::DateTimeRule::DateTimeRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t millisInDay,
    #       TimeRuleType timeType
    # )
    dr1 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_FEBRUARY,
        11,
        1 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )

    # DateRuleType icu::DateTimeRule::getDateRuleType(void)
    assert dr1.get_date_rule_type() == icu.DateTimeRule.DOM

    # int32_t icu::DateTimeRule::getRuleDayOfMonth(void)
    assert dr1.get_rule_day_of_month() == 11

    # int32_t icu::DateTimeRule::getRuleDayOfWeek(void)
    assert dr1.get_rule_day_of_week() == 0

    # int32_t icu::DateTimeRule::getRuleMillisInDay(void)
    assert dr1.get_rule_millis_in_day() == 1 * HOUR

    # int32_t icu::DateTimeRule::getRuleMonth(void)
    assert dr1.get_rule_month() == icu.UCalendarMonths.UCAL_FEBRUARY

    # int32_t icu::DateTimeRule::getRuleWeekInMonth(void)
    assert dr1.get_rule_week_in_month() == 0

    # TimeRuleType icu::DateTimeRule::getTimeRuleType(void)
    assert dr1.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [2]
    # icu::DateTimeRule::DateTimeRule(
    #       int32_t month,
    #       int32_t weekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t millisInDay,
    #       TimeRuleType timeType
    # )
    dr2 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_FEBRUARY,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_SATURDAY,
        1 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )
    assert dr2.get_date_rule_type() == icu.DateTimeRule.DOW
    assert dr2.get_rule_day_of_month() == 0
    assert dr2.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_SATURDAY
    assert dr2.get_rule_millis_in_day() == 1 * HOUR
    assert dr2.get_rule_month() == icu.UCalendarMonths.UCAL_FEBRUARY
    assert dr2.get_rule_week_in_month() == -1
    assert dr2.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [3]
    # icu::DateTimeRule::DateTimeRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t dayOfWeek,
    #       UBool after,
    #       int32_t millisInDay,
    #       TimeRuleType timeType
    # )
    dr3 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_FEBRUARY,
        11,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        True,
        1 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )
    assert dr3.get_date_rule_type() == icu.DateTimeRule.DOW_GEQ_DOM
    assert dr3.get_rule_day_of_month() == 11
    assert dr3.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_SUNDAY
    assert dr3.get_rule_millis_in_day() == 1 * HOUR
    assert dr3.get_rule_month() == icu.UCalendarMonths.UCAL_FEBRUARY
    assert dr3.get_rule_week_in_month() == 0
    assert dr3.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [4]
    # icu::DateTimeRule::DateTimeRule(const DateTimeRule &source)
    dr4 = icu.DateTimeRule(dr1)
    assert dr4.get_date_rule_type() == icu.DateTimeRule.DOM
    assert dr4.get_rule_day_of_month() == 11
    assert dr4.get_rule_day_of_week() == 0
    assert dr4.get_rule_millis_in_day() == 1 * HOUR
    assert dr4.get_rule_month() == icu.UCalendarMonths.UCAL_FEBRUARY
    assert dr4.get_rule_week_in_month() == 0
    assert dr4.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # DateTimeRule *icu::DateTimeRule::clone()
    dr5 = dr1.clone()
    assert isinstance(dr5, icu.DateTimeRule)

    # DateTimeRule.__copy__() -> DateTimeRule
    # DateTimeRule.__deepcopy__(Optional[memo]) -> DateTimeRule
    dr5a = copy.copy(dr1)
    dr5b = copy.deepcopy(dr1)
    assert dr5 == dr5a == dr5b

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

    # DateTimeRule.__repr__() -> str
    assert repr(dr1) == (
        "<DateTimeRule("
        "date_rule_type=0, rule_month=1, rule_day_of_month=11, "
        "rule_week_in_month=0, rule_day_of_week=0, rule_millis_in_day=3600000, "
        "time_rule_type=0"
        ")>"
    )


def test_initial_time_zone_rule() -> None:
    assert issubclass(icu.InitialTimeZoneRule, icu.TimeZoneRule)

    # [1]
    # icu::InitialTimeZoneRule::InitialTimeZoneRule(
    #       const UnicodeString &name,
    #       int32_t rawOffset,
    #       int32_t dstSavings
    # )
    ir1 = icu.InitialTimeZoneRule(icu.UnicodeString("i1"), -1 * HOUR, 1 * HOUR)

    # [2]
    # icu::InitialTimeZoneRule::InitialTimeZoneRule(
    #       const InitialTimeZoneRule &source
    # )
    ir2 = icu.InitialTimeZoneRule(ir1)

    # InitialTimeZoneRule *icu::InitialTimeZoneRule::clone()
    ir3 = ir1.clone()
    assert isinstance(ir3, icu.InitialTimeZoneRule)

    # InitialTimeZoneRule.__copy__() -> InitialTimeZoneRule
    # InitialTimeZoneRule.__deepcopy__(Optional[memo]) -> InitialTimeZoneRule
    ir3a = copy.copy(ir3)
    ir3b = copy.deepcopy(ir3)
    assert ir3 == ir3a == ir3b

    # UnicodeString &icu::TimeZoneRule::getName(UnicodeString &name)
    name = icu.UnicodeString()
    result = ir1.get_name(name)
    assert isinstance(result, icu.UnicodeString)
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
    assert available is False

    # UBool icu::InitialTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, _ = ir1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available is False

    # UBool icu::InitialTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, _ = ir1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available is False

    # UBool icu::InitialTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, _ = ir1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available is False

    # UBool icu::InitialTimeZoneRule::isEquivalentTo(const TimeZoneRule &that)
    assert ir1.is_equivalent_to(ir2) is True
    assert ir1.is_equivalent_to(ir3) is True

    # UBool icu::InitialTimeZoneRule::operator!=(const TimeZoneRule &that)
    assert not (ir1 != ir2)
    assert not (ir1 != ir3)
    assert not (ir2 != ir3)

    # UBool icu::InitialTimeZoneRule::operator==(const TimeZoneRule &that)
    assert ir1 == ir2 == ir3

    # InitialTimeZoneRule.__repr__() -> str
    assert repr(ir1) == (
        "<InitialTimeZoneRule(name='i1', raw_offset=-3600000, dst_savings=3600000)>"
    )


def test_time_array_time_zone_rule() -> None:
    assert issubclass(icu.TimeArrayTimeZoneRule, icu.TimeZoneRule)
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
    tr1 = icu.TimeArrayTimeZoneRule(
        icu.UnicodeString("t1"),
        -1 * HOUR,
        1 * HOUR,
        start_times,
        len(start_times),
        icu.DateTimeRule.UTC_TIME,
    )

    # [2]
    # icu::TimeArrayTimeZoneRule::TimeArrayTimeZoneRule(
    #       const TimeArrayTimeZoneRule &source
    # )
    tr2 = icu.TimeArrayTimeZoneRule(tr1)

    # TimeArrayTimeZoneRule *icu::TimeArrayTimeZoneRule::clone()
    tr3 = tr1.clone()
    assert isinstance(tr3, icu.TimeArrayTimeZoneRule)

    # TimeArrayTimeZoneRule.__copy__() -> TimeArrayTimeZoneRule
    # TimeArrayTimeZoneRule.__deepcopy__(Optional[memo])
    #  -> TimeArrayTimeZoneRule
    tr3a = copy.copy(tr3)
    tr3b = copy.deepcopy(tr3)
    assert tr3 == tr3a == tr3b

    # UnicodeString &icu::TimeZoneRule::getName(UnicodeString &name)
    name = icu.UnicodeString()
    result = tr1.get_name(name)
    assert isinstance(result, icu.UnicodeString)
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
    assert valid is False

    # DateTimeRule::TimeRuleType icu::TimeArrayTimeZoneRule::getTimeType(void)
    assert tr1.get_time_type() == icu.DateTimeRule.UTC_TIME

    # UBool icu::TimeArrayTimeZoneRule::getFinalStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result1 = tr1.get_final_start(-1 * HOUR, 1 * HOUR)
    assert available is True
    assert isinstance(result1, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::getFirstStart(
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UDate &result
    # )
    available, result2 = tr1.get_first_start(-1 * HOUR, 1 * HOUR)
    assert available is True
    assert isinstance(result2, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::getNextStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result3 = tr1.get_next_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available is True
    assert isinstance(result3, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::getPreviousStart(
    #       UDate base,
    #       int32_t prevRawOffset,
    #       int32_t prevDSTSavings,
    #       UBool inclusive,
    #       UDate &result
    # )
    available, result4 = tr1.get_previous_start(0, -1 * HOUR, 1 * HOUR, False)
    assert available is False
    assert isinstance(result4, (int, float))

    # UBool icu::TimeArrayTimeZoneRule::isEquivalentTo(
    #       const TimeZoneRule &that
    # )
    assert tr1.is_equivalent_to(tr2) is True
    assert tr1.is_equivalent_to(tr3) is True

    # UBool icu::TimeArrayTimeZoneRule::operator!=(const TimeZoneRule &that)
    assert not (tr1 != tr2)
    assert not (tr1 != tr3)
    assert not (tr2 != tr3)

    # UBool icu::TimeArrayTimeZoneRule::operator==(const TimeZoneRule &that)
    assert tr1 == tr2 == tr3

    # TimeArrayTimeZoneRule.__repr__() -> str
    assert repr(tr1) == (
        "<TimeArrayTimeZoneRule("
        "name='t1', raw_offset=-3600000, dst_savings=3600000, "
        "start_times=[0, 1e+07]"
        ")>"
    )


def test_time_zone_transition() -> None:
    date_time_rule1 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_JULY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.DateTimeRule.WALL_TIME,
    )
    date_time_rule2 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_JANUARY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.DateTimeRule.WALL_TIME,
    )
    from_ = icu.AnnualTimeZoneRule(
        "from", 0, 0, date_time_rule1, 0, icu.AnnualTimeZoneRule.MAX_YEAR
    )
    to = icu.AnnualTimeZoneRule("to", 0, 0, date_time_rule2, 0, icu.AnnualTimeZoneRule.MAX_YEAR)
    time = 1230681600000.0  # 2008-12-31T00:00:00

    # [1]
    # icu::TimeZoneTransition::TimeZoneTransition(
    #       UDate time,
    #       const TimeZoneRule &from,
    #       const TimeZoneRule &to
    # )
    tzt1 = icu.TimeZoneTransition(time, from_, to)

    # [2]
    # icu::TimeZoneTransition::TimeZoneTransition()
    tzt2 = icu.TimeZoneTransition()

    # [3]
    # icu::TimeZoneTransition::TimeZoneTransition(
    #       const TimeZoneTransition &source
    # )
    tzt3 = icu.TimeZoneTransition(tzt2)

    # TimeZoneTransition *icu::TimeZoneTransition::clone()
    tzt4 = tzt1.clone()
    assert isinstance(tzt4, icu.TimeZoneTransition)

    # TimeZoneTransition.__copy__() -> TimeZoneTransition
    # TimeZoneTransition.__deepcopy__(Optional[memo]) -> TimeZoneTransition
    tzt4a = copy.copy(tzt4)
    tzt4b = copy.deepcopy(tzt4)
    assert tzt4 == tzt4a == tzt4b

    # const TimeZoneRule *icu::TimeZoneTransition::getFrom(void)
    result = tzt1.get_from()
    assert isinstance(result, icu.AnnualTimeZoneRule)
    assert result == from_

    assert tzt2.get_from() is None

    # UDate icu::TimeZoneTransition::getTime(void)
    assert tzt1.get_time() == time
    assert tzt2.get_time() == 0

    # const TimeZoneRule *icu::TimeZoneTransition::getTo(void)
    result = tzt1.get_to()
    assert isinstance(result, icu.AnnualTimeZoneRule)
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

import copy

import pytest

from icupy import icu

HOUR = icu.U_MILLIS_PER_HOUR


def test_basic_time_zone() -> None:
    assert issubclass(icu.BasicTimeZone, icu.TimeZone)

    zone1 = icu.TimeZone.create_time_zone("America/Los_Angeles")
    assert isinstance(zone1, icu.BasicTimeZone)  # OlsonTimeZone
    assert not isinstance(zone1, icu.RuleBasedTimeZone)
    assert not isinstance(zone1, icu.SimpleTimeZone)
    assert not isinstance(zone1, icu.VTimeZone)

    # BasicTimeZone *icu::BasicTimeZone::clone()
    zone1a = zone1.clone()
    assert isinstance(zone1a, icu.BasicTimeZone)
    assert zone1a == zone1

    zone1b = copy.copy(zone1)
    assert zone1b == zone1

    zone1c = copy.deepcopy(zone1)
    assert zone1c == zone1

    # int32_t icu::BasicTimeZone::countTransitionRules(UErrorCode &status)
    assert zone1.count_transition_rules() == 4

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    result = icu.TimeZoneTransition()
    assert zone1.get_next_transition(base, False, result) is True
    assert result.get_time() == 1236506400000.0  # 2009-03-08T10:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone1.get_previous_transition(base, False, result) is True
    assert result.get_time() == 1225616400000.0  # 2008-11-02T09:00:00Z

    # void icu::BasicTimeZone::getSimpleRulesNear(
    #       UDate date,
    #       InitialTimeZoneRule *&initial,
    #       AnnualTimeZoneRule *&std,
    #       AnnualTimeZoneRule *&dst,
    #       UErrorCode &status
    # )
    initial, std, dst = zone1.get_simple_rules_near(0)
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == -8 * HOUR
    assert initial.get_dst_savings() == 1 * HOUR
    assert isinstance(std, icu.AnnualTimeZoneRule)
    assert std.get_raw_offset() == -8 * HOUR
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_OCTOBER
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, icu.AnnualTimeZoneRule)
    assert dst.get_raw_offset() == -8 * HOUR
    assert dst.get_dst_savings() == 1 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_APRIL
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone1.get_time_zone_rules()
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert isinstance(trsrules, list)
    assert len(trsrules) == 4
    assert any(isinstance(x, icu.AnnualTimeZoneRule) for x in trsrules)

    # UBool icu::BasicTimeZone::hasEquivalentTransitions(
    #       const BasicTimeZone &tz,
    #       UDate start,
    #       UDate end,
    #       UBool ignoreDstAmount,
    #       UErrorCode &ec
    # )
    # From icu/source/test/intltest/tzrulets.cpp:
    # TimeZoneRuleTest::TestHasEquivalentTransitions()
    zone5 = icu.TimeZone.create_time_zone("America/New_York")
    zone6 = icu.TimeZone.create_time_zone("America/Indiana/Indianapolis")
    start = 1104537600000.0  # 2005-01-01T00:00:00Z
    end = 1262304000000.0  # 2010-01-01T00:00:00Z
    assert zone5.has_equivalent_transitions(zone6, start, end, True) is False
    start = 1136073600000.0  # 2006-01-01T00:00:00Z
    assert zone5.has_equivalent_transitions(zone6, start, end, True) is True


def test_basic_time_zone_get_offset() -> None:
    zone = icu.TimeZone.create_time_zone("America/Los_Angeles")
    assert isinstance(zone, icu.BasicTimeZone)

    # [1]
    # void icu::TimeZone::getOffset(
    #       UDate date,
    #       UBool local,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &ec
    # )
    raw_offset, dst_offset = zone.get_offset(0, False)
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_basic_time_zone_get_offset_from_local() -> None:
    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = icu.TimeZone.create_time_zone("America/Los_Angeles")
    assert isinstance(zone, icu.BasicTimeZone)

    # void icu::BasicTimeZone::getOffsetFromLocal(
    #       UDate date,
    #       UTimeZoneLocalOption nonExistingTimeOpt,
    #       UTimeZoneLocalOption duplicatedTimeOpt,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &status
    # )
    date = 1143941400000  # 2006-04-02T01:30:00Z
    raw_offset, dst_offset = zone.get_offset_from_local(
        date,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER,
    )
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


def test_rule_based_time_zone() -> None:
    assert issubclass(icu.RuleBasedTimeZone, icu.BasicTimeZone)
    assert issubclass(icu.RuleBasedTimeZone, icu.TimeZone)
    initial_rule1 = icu.InitialTimeZoneRule("STD", 0, 0)
    initial_rule2 = icu.InitialTimeZoneRule("STD", 5 * HOUR, 0)

    # [1]
    # icu::RuleBasedTimeZone::RuleBasedTimeZone(
    #       const UnicodeString &id,
    #       InitialTimeZoneRule *initialRule
    # )
    zone1 = icu.RuleBasedTimeZone(icu.UnicodeString("r1"), initial_rule1)
    dr11 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_JULY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.DateTimeRule.WALL_TIME,
    )
    tzr11 = icu.AnnualTimeZoneRule("r1(STD)", 0, 0, dr11, 0, icu.AnnualTimeZoneRule.MAX_YEAR)
    zone1.add_transition_rule(tzr11)
    dr12 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_JANUARY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.DateTimeRule.WALL_TIME,
    )
    tzr12 = icu.AnnualTimeZoneRule(
        "r1(DST)", 0, 1 * HOUR, dr12, 0, icu.AnnualTimeZoneRule.MAX_YEAR
    )
    zone1.add_transition_rule(tzr12)
    zone1.complete()

    zone2 = icu.RuleBasedTimeZone(icu.UnicodeString("r2"), initial_rule2)
    dr21 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        5 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )
    tzr21 = icu.AnnualTimeZoneRule("r2(STD)", 5 * HOUR, 0, dr21, 2000, 2010)
    zone2.add_transition_rule(tzr21)
    dr22 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_MARCH,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )
    tzr22 = icu.AnnualTimeZoneRule("r2(DST)", 5 * HOUR, 1 * HOUR, dr22, 2000, 2010)
    zone2.add_transition_rule(tzr22)
    dr23 = icu.DateTimeRule(icu.UCalendarMonths.UCAL_JANUARY, 1, 0, icu.DateTimeRule.WALL_TIME)
    tzr23 = icu.AnnualTimeZoneRule(
        "r2(STD)", 6 * HOUR, 0, dr23, 2011, icu.AnnualTimeZoneRule.MAX_YEAR
    )
    zone2.add_transition_rule(tzr23)
    dr24 = icu.DateTimeRule(icu.UCalendarMonths.UCAL_JANUARY, 1, 1, icu.DateTimeRule.WALL_TIME)
    tzr24 = icu.AnnualTimeZoneRule(
        "r2(DST)", 6 * HOUR, 0, dr24, 2011, icu.AnnualTimeZoneRule.MAX_YEAR
    )
    zone2.add_transition_rule(tzr24)
    zone2.complete()

    # [2]
    # icu::RuleBasedTimeZone::RuleBasedTimeZone(
    #       const RuleBasedTimeZone &source
    # )
    zone3 = icu.RuleBasedTimeZone(zone2)

    # RuleBasedTimeZone *icu::RuleBasedTimeZone::clone()
    zone2a = zone2.clone()
    assert isinstance(zone2a, icu.RuleBasedTimeZone)
    assert zone2a == zone2

    zone2b = copy.copy(zone2)
    assert zone2b == zone2

    zone2c = copy.deepcopy(zone2)
    assert zone2c == zone2

    # UBool icu::TimeZone::operator!=(const TimeZone &that)
    assert zone1 != zone2
    assert zone1 != zone3
    assert not (zone2 != zone3)

    # UBool icu::RuleBasedTimeZone::operator==(const TimeZone &that)
    assert not (zone1 == zone2)
    assert not (zone1 == zone3)
    assert zone2 == zone3

    # int32_t icu::BasicTimeZone::countTransitionRules(UErrorCode &status)
    assert zone1.count_transition_rules() == 2
    assert zone2.count_transition_rules() == 4

    # int32_t icu::TimeZone::getDSTSavings()
    assert zone1.get_dst_savings() == 1 * HOUR
    assert zone2.get_dst_savings() == 0

    # UnicodeString &icu::TimeZone::getID(UnicodeString &ID)
    id_ = icu.UnicodeString()
    result = zone1.get_id(id_)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(id_)
    assert result == "r1"

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    tzt = icu.TimeZoneTransition()
    assert zone1.get_next_transition(base, False, tzt) is True
    assert tzt.get_time() == 1231027200000.0  # 2009-01-04T00:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone1.get_previous_transition(base, False, tzt) is True
    assert tzt.get_time() == 1215298800000.0  # 2008-07-05T23:00:00Z

    # int32_t icu::TimeZone::getRawOffset(void)
    assert zone1.get_raw_offset() == 0
    assert zone2.get_raw_offset() == 6 * HOUR

    # void icu::BasicTimeZone::getSimpleRulesNear(
    #       UDate date,
    #       InitialTimeZoneRule *&initial,
    #       AnnualTimeZoneRule *&std,
    #       AnnualTimeZoneRule *&dst,
    #       UErrorCode &status
    # )
    initial, std, dst = zone1.get_simple_rules_near(0)
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == 0
    assert initial.get_dst_savings() == 1 * HOUR
    assert isinstance(std, icu.AnnualTimeZoneRule)
    assert std.get_raw_offset() == 0
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JULY
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, icu.AnnualTimeZoneRule)
    assert dst.get_raw_offset() == 0
    assert dst.get_dst_savings() == 1 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JANUARY
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone2.get_time_zone_rules()
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == 5 * HOUR
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 4
    assert all(isinstance(x, icu.AnnualTimeZoneRule) for x in trsrules)
    assert trsrules[0] == tzr21
    assert trsrules[1] == tzr22
    assert trsrules[2] == tzr23
    assert trsrules[3] == tzr24

    # UBool icu::BasicTimeZone::hasEquivalentTransitions(
    #       const BasicTimeZone &tz,
    #       UDate start,
    #       UDate end,
    #       UBool ignoreDstAmount,
    #       UErrorCode &ec
    # )
    start = 1215298800000.0  # 2008-07-05T23:00:00Z
    end = 1231027200000.0  # 2009-01-04T00:00:00Z
    assert zone1.has_equivalent_transitions(zone2, start, end, False) is False
    assert zone2.has_equivalent_transitions(zone1, start, end, False) is False

    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    assert zone1.has_same_rules(zone2) is False
    assert zone2.has_same_rules(zone3) is True

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone1.set_id(icu.UnicodeString("abc"))
    assert zone1.get_id(id_) == "abc"
    zone1.set_id("ABC")
    assert zone1.get_id(id_) == "ABC"

    # void icu::TimeZone::setRawOffset(int32_t offsetMillis)
    zone1.set_raw_offset(-5 * HOUR)  # Nothing to do
    assert zone1.get_raw_offset() == 0

    # UBool icu::TimeZone::useDaylightTime(void)
    assert zone1.use_daylight_time() is True
    assert zone2.use_daylight_time() is False


def test_rule_based_time_zone_get_offset() -> None:
    initial_rule = icu.InitialTimeZoneRule("r1(STD)", 8 * HOUR, 0)
    zone = icu.RuleBasedTimeZone("r1", initial_rule)
    dr1 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )
    tzr1 = icu.AnnualTimeZoneRule(
        "r1(STD)", 8 * HOUR, 0, dr1, 0, icu.AnnualTimeZoneRule.MAX_YEAR
    )
    zone.add_transition_rule(tzr1)
    dr2 = icu.DateTimeRule(
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        True,
        2 * HOUR,
        icu.DateTimeRule.WALL_TIME,
    )
    tzr2 = icu.AnnualTimeZoneRule(
        "r1(DST)",
        8 * HOUR,
        int(0.5 * HOUR),
        dr2,
        0,
        icu.AnnualTimeZoneRule.MAX_YEAR,
    )
    zone.add_transition_rule(tzr2)
    zone.complete()

    # [1]
    # void icu::RuleBasedTimeZone::getOffset(
    #       UDate date,
    #       UBool local,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &ec
    # )
    raw_offset, dst_offset = zone.get_offset(0, False)
    assert raw_offset == 8 * HOUR
    assert dst_offset == 0

    # [2]
    # int32_t icu::RuleBasedTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t millis,
    #       int32_t monthLength,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
            28,
        )
        == 8.5 * HOUR
    )

    # [3]
    # int32_t icu::RuleBasedTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t millis,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
        )
        == 8.5 * HOUR
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_rule_based_time_zone_get_offset_from_local() -> None:
    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = icu.RuleBasedTimeZone(
        "Rule based Pacific Time",
        icu.InitialTimeZoneRule("Pacific Standard Time", -8 * HOUR, 0),
    )
    zone.add_transition_rule(
        icu.AnnualTimeZoneRule(
            "Pacific Daylight Time",
            -8 * HOUR,
            1 * HOUR,
            icu.DateTimeRule(
                icu.UCalendarMonths.UCAL_APRIL,
                1,
                icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
                2 * HOUR,
                icu.DateTimeRule.WALL_TIME,
            ),
            2000,
            icu.AnnualTimeZoneRule.MAX_YEAR,
        )
    )
    zone.add_transition_rule(
        icu.AnnualTimeZoneRule(
            "Pacific Standard Time",
            -8 * HOUR,
            0,
            icu.DateTimeRule(
                icu.UCalendarMonths.UCAL_OCTOBER,
                -1,
                icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
                2 * HOUR,
                icu.DateTimeRule.WALL_TIME,
            ),
            2000,
            icu.AnnualTimeZoneRule.MAX_YEAR,
        )
    )
    zone.complete()

    # void icu::RuleBasedTimeZone::getOffsetFromLocal(
    #       UDate date,
    #       UTimeZoneLocalOption nonExistingTimeOpt,
    #       UTimeZoneLocalOption duplicatedTimeOpt,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &status
    # )
    date = 1143941400000  # 2006-04-02T01:30:00Z
    raw_offset, dst_offset = zone.get_offset_from_local(
        date,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER,
    )
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


def test_simple_time_zone() -> None:
    assert issubclass(icu.SimpleTimeZone, icu.BasicTimeZone)
    assert issubclass(icu.SimpleTimeZone, icu.TimeZone)

    # [2]
    # icu::SimpleTimeZone::SimpleTimeZone(
    #       int32_t rawOffsetGMT,
    #       const UnicodeString &ID
    # )
    zone2 = icu.SimpleTimeZone(8 * HOUR, icu.UnicodeString("s2"))

    # [3]
    # icu::SimpleTimeZone::SimpleTimeZone(
    #       int32_t rawOffsetGMT,
    #       const UnicodeString &ID,
    #       int8_t savingsStartMonth,
    #       int8_t savingsStartDayOfWeekInMonth,
    #       int8_t savingsStartDayOfWeek,
    #       int32_t savingsStartTime,
    #       int8_t savingsEndMonth,
    #       int8_t savingsEndDayOfWeekInMonth,
    #       int8_t savingsEndDayOfWeek,
    #       int32_t savingsEndTime,
    #       UErrorCode &status
    # )
    zone3 = icu.SimpleTimeZone(
        0,
        icu.UnicodeString("s3"),
        icu.UCalendarMonths.UCAL_JANUARY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.UCalendarMonths.UCAL_JULY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
    )

    # [4]
    # icu::SimpleTimeZone::SimpleTimeZone(
    #       int32_t rawOffsetGMT,
    #       const UnicodeString &ID,
    #       int8_t savingsStartMonth,
    #       int8_t savingsStartDayOfWeekInMonth,
    #       int8_t savingsStartDayOfWeek,
    #       int32_t savingsStartTime,
    #       int8_t savingsEndMonth,
    #       int8_t savingsEndDayOfWeekInMonth,
    #       int8_t savingsEndDayOfWeek,
    #       int32_t savingsEndTime,
    #       int32_t savingsDST,
    #       UErrorCode &status
    # )
    zone4 = icu.SimpleTimeZone(
        -5 * HOUR,
        icu.UnicodeString("s4"),
        icu.UCalendarMonths.UCAL_MARCH,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.UCalendarMonths.UCAL_SEPTEMBER,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        int(0.5 * HOUR),
    )

    # [5]
    # icu::SimpleTimeZone::SimpleTimeZone(
    #       int32_t rawOffsetGMT,
    #       const UnicodeString &ID,
    #       int8_t savingsStartMonth,
    #       int8_t savingsStartDayOfWeekInMonth,
    #       int8_t savingsStartDayOfWeek,
    #       int32_t savingsStartTime,
    #       TimeMode savingsStartTimeMode,
    #       int8_t savingsEndMonth,
    #       int8_t savingsEndDayOfWeekInMonth,
    #       int8_t savingsEndDayOfWeek,
    #       int32_t savingsEndTime,
    #       TimeMode savingsEndTimeMode,
    #       int32_t savingsDST,
    #       UErrorCode &status
    # )
    zone5 = icu.SimpleTimeZone(
        8 * HOUR,
        icu.UnicodeString("s5"),
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR),
    )

    # [1]
    # icu::SimpleTimeZone::SimpleTimeZone(const SimpleTimeZone &source)
    zone1 = icu.SimpleTimeZone(zone5)

    # SimpleTimeZone *icu::SimpleTimeZone::clone()
    zone1a = zone1.clone()
    assert isinstance(zone1a, icu.SimpleTimeZone)
    assert zone1a == zone1

    zone1b = copy.copy(zone1)
    assert zone1b == zone1

    zone1c = copy.deepcopy(zone1)
    assert zone1c == zone1

    # UBool icu::TimeZone::operator!=(const TimeZone &that)
    assert zone1 != zone2
    assert zone1 != zone3
    assert zone1 != zone4
    assert not (zone1 != zone5)
    assert zone2 != zone3
    assert zone2 != zone4
    assert zone2 != zone5
    assert zone3 != zone4
    assert zone3 != zone5
    assert zone4 != zone5

    # UBool icu::SimpleTimeZone::operator==(const TimeZone &that)
    assert not (zone1 == zone2)
    assert not (zone1 == zone3)
    assert not (zone1 == zone4)
    assert zone1 == zone5
    assert not (zone2 == zone3)
    assert not (zone2 == zone4)
    assert not (zone2 == zone5)
    assert not (zone3 == zone4)
    assert not (zone3 == zone5)
    assert not (zone4 == zone5)

    # int32_t icu::BasicTimeZone::countTransitionRules(UErrorCode &status)
    assert zone1.count_transition_rules() == 2
    assert zone2.count_transition_rules() == 0
    assert zone3.count_transition_rules() == 2
    assert zone4.count_transition_rules() == 2
    assert zone5.count_transition_rules() == 2

    # int32_t icu::TimeZone::getDSTSavings()
    assert zone1.get_dst_savings() == 0.5 * HOUR
    assert zone2.get_dst_savings() == 1 * HOUR
    assert zone3.get_dst_savings() == 1 * HOUR
    assert zone4.get_dst_savings() == 0.5 * HOUR
    assert zone5.get_dst_savings() == 0.5 * HOUR

    # UnicodeString &icu::TimeZone::getID(UnicodeString &ID)
    id_ = icu.UnicodeString()
    result = zone1.get_id(id_)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(id_)
    assert result == "s5"

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    tzt = icu.TimeZoneTransition()
    assert zone3.get_next_transition(base, False, tzt) is True
    assert tzt.get_time() == 1231027200000.0  # 2009-01-04T00:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone3.get_previous_transition(base, False, tzt) is True
    assert tzt.get_time() == 1215298800000.0  # 2008-07-05T23:00:00Z

    # int32_t icu::TimeZone::getRawOffset(void)
    assert zone1.get_raw_offset() == 8 * HOUR
    assert zone2.get_raw_offset() == 8 * HOUR
    assert zone3.get_raw_offset() == 0
    assert zone4.get_raw_offset() == -5 * HOUR
    assert zone5.get_raw_offset() == 8 * HOUR

    # void icu::BasicTimeZone::getSimpleRulesNear(
    #       UDate date,
    #       InitialTimeZoneRule *&initial,
    #       AnnualTimeZoneRule *&std,
    #       AnnualTimeZoneRule *&dst,
    #       UErrorCode &status
    # )
    initial, std, dst = zone2.get_simple_rules_near(0)
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == 8 * HOUR
    assert initial.get_dst_savings() == 0
    assert std is None
    assert dst is None

    initial, std, dst = zone4.get_simple_rules_near(0)
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == -5 * HOUR
    assert initial.get_dst_savings() == 0.5 * HOUR
    assert isinstance(std, icu.AnnualTimeZoneRule)
    assert std.get_raw_offset() == -5 * HOUR
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_SEPTEMBER
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, icu.AnnualTimeZoneRule)
    assert dst.get_raw_offset() == -5 * HOUR
    assert dst.get_dst_savings() == 0.5 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_MARCH
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone2.get_time_zone_rules()
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == 8 * HOUR
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 0

    initial, trsrules = zone4.get_time_zone_rules()
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == -5 * HOUR
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    assert all(isinstance(x, icu.AnnualTimeZoneRule) for x in trsrules)
    assert trsrules[0].get_raw_offset() == -5 * HOUR
    assert trsrules[0].get_dst_savings() == 0
    dr = trsrules[0].get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_SEPTEMBER
    assert dr.get_rule_day_of_month() == 0
    assert trsrules[1].get_raw_offset() == -5 * HOUR
    assert trsrules[1].get_dst_savings() == 0.5 * HOUR
    dr = trsrules[1].get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_MARCH
    assert dr.get_rule_day_of_month() == 0

    # UBool icu::BasicTimeZone::hasEquivalentTransitions(
    #       const BasicTimeZone &tz,
    #       UDate start,
    #       UDate end,
    #       UBool ignoreDstAmount,
    #       UErrorCode &ec
    # )
    start = 1215298800000.0  # 2008-07-05T23:00:00Z
    end = 1231027200000.0  # 2009-01-04T00:00:00Z
    assert zone5.has_equivalent_transitions(zone4, start, end, False) is False
    assert zone5.has_equivalent_transitions(zone1, start, end, False) is True

    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    assert zone5.has_same_rules(zone4) is False
    assert zone5.has_same_rules(zone1) is True

    # void icu::SimpleTimeZone::setDSTSavings(
    #       int32_t millisSavedDuringDST,
    #       UErrorCode &status
    # )
    zone1.set_dst_savings(1 * HOUR)
    assert zone1.get_dst_savings() == 1 * HOUR
    zone2.set_dst_savings(int(0.5 * HOUR))
    assert zone2.get_dst_savings() == 0.5 * HOUR

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone1.set_id(icu.UnicodeString("abc"))
    assert zone1.get_id(id_) == "abc"
    zone1.set_id("ABC")
    assert zone1.get_id(id_) == "ABC"

    # void icu::TimeZone::setRawOffset(int32_t offsetMillis)
    zone1.set_raw_offset(0)
    assert zone1.get_raw_offset() == 0
    zone2.set_raw_offset(-5 * HOUR)
    assert zone2.get_raw_offset() == -5 * HOUR

    _, trsrules = zone1.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    assert trsrules[0].get_start_year() == 0
    assert trsrules[1].get_start_year() == 0
    zone1.set_start_year(1966)
    _, trsrules = zone1.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    assert trsrules[0].get_start_year() == 1966
    assert trsrules[1].get_start_year() == 1966

    # UBool icu::TimeZone::useDaylightTime(void)
    assert zone1.use_daylight_time() is True
    assert zone2.use_daylight_time() is False
    assert zone3.use_daylight_time() is True
    assert zone4.use_daylight_time() is True
    assert zone5.use_daylight_time() is True


def test_simple_time_zone_get_offset() -> None:
    zone = icu.SimpleTimeZone(
        8 * HOUR,
        "s5",
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR),
    )

    # [1]
    # void icu::SimpleTimeZone::getOffset(
    #       UDate date,
    #       UBool local,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &ec
    # )
    raw_offset, dst_offset = zone.get_offset(0, False)
    assert raw_offset == 8 * HOUR
    assert dst_offset == 0

    # [2]
    # int32_t icu::SimpleTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t millis,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
        )
        == 8.5 * HOUR
    )

    # [3]
    # int32_t icu::SimpleTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t milliseconds,
    #       int32_t monthLength,
    #       int32_t prevMonthLength,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
            28,
            28,
        )
        == 8.5 * HOUR
    )

    # [4]
    # int32_t icu::SimpleTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t milliseconds,
    #       int32_t monthLength,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
            28,
        )
        == 8.5 * HOUR
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_simple_time_zone_get_offset_from_local() -> None:
    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = icu.SimpleTimeZone(
        -8 * HOUR,
        "Simple Pacific Time",
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
    )

    # void icu::SimpleTimeZone::getOffsetFromLocal(
    #       UDate date,
    #       UTimeZoneLocalOption nonExistingTimeOpt,
    #       UTimeZoneLocalOption duplicatedTimeOpt,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &status
    # )
    date = 1143941400000  # 2006-04-02T01:30:00Z
    raw_offset, dst_offset = zone.get_offset_from_local(
        date,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER,
    )
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


def test_simple_time_zone_set_end_rule() -> None:
    zone = icu.SimpleTimeZone(
        8 * HOUR,
        "s5",
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR),
    )

    # [1]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       TimeMode mode,
    #       UBool after,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        icu.UCalendarMonths.UCAL_DECEMBER,
        31,
        icu.UCalendarDaysOfWeek.UCAL_SATURDAY,
        6 * HOUR,
        icu.SimpleTimeZone.UTC_TIME,
        True,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_SATURDAY
    assert dr.get_rule_millis_in_day() == 6 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_DECEMBER
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.UTC_TIME

    # [2]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       UBool after,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        icu.UCalendarMonths.UCAL_OCTOBER,
        31,
        icu.UCalendarDaysOfWeek.UCAL_FRIDAY,
        5 * HOUR,
        True,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_FRIDAY
    assert dr.get_rule_millis_in_day() == 5 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_OCTOBER
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [3]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       TimeMode mode,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        icu.UCalendarMonths.UCAL_SEPTEMBER,
        30,
        4 * HOUR,
        icu.SimpleTimeZone.UTC_TIME,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 30
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 4 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_SEPTEMBER
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.UTC_TIME

    # [4]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_end_rule(icu.UCalendarMonths.UCAL_AUGUST, 31, 3 * HOUR)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 3 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_AUGUST
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [5]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfWeekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       TimeMode mode,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        icu.UCalendarMonths.UCAL_JULY,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_THURSDAY,
        2 * HOUR,
        icu.SimpleTimeZone.UTC_TIME,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_THURSDAY
    assert dr.get_rule_millis_in_day() == 2 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JULY
    assert dr.get_rule_week_in_month() == -1
    assert dr.get_time_rule_type() == icu.DateTimeRule.UTC_TIME

    # [6]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfWeekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        icu.UCalendarMonths.UCAL_JUNE,
        2,
        icu.UCalendarDaysOfWeek.UCAL_WEDNESDAY,
        1 * HOUR,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_WEDNESDAY
    assert dr.get_rule_millis_in_day() == 1 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JUNE
    assert dr.get_rule_week_in_month() == 2
    assert dr.get_time_rule_type() == icu.DateTimeRule.WALL_TIME


def test_simple_time_zone_set_start_rule() -> None:
    zone = icu.SimpleTimeZone(
        8 * HOUR,
        "s5",
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR),
    )

    # [1]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       TimeMode mode,
    #       UBool after,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        icu.UCalendarMonths.UCAL_JANUARY,
        31,
        icu.UCalendarDaysOfWeek.UCAL_SATURDAY,
        1 * HOUR,
        icu.SimpleTimeZone.UTC_TIME,
        True,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_SATURDAY
    assert dr.get_rule_millis_in_day() == 1 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JANUARY
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.UTC_TIME

    # [2]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       UBool after,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        icu.UCalendarMonths.UCAL_FEBRUARY,
        28,
        icu.UCalendarDaysOfWeek.UCAL_FRIDAY,
        2 * HOUR,
        True,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 28
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_FRIDAY
    assert dr.get_rule_millis_in_day() == 2 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_FEBRUARY
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [3]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       TimeMode mode,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        icu.UCalendarMonths.UCAL_MARCH,
        31,
        3 * HOUR,
        icu.SimpleTimeZone.UTC_TIME,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 3 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_MARCH
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.UTC_TIME

    # [4]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_start_rule(icu.UCalendarMonths.UCAL_APRIL, 30, 4 * HOUR)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 30
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 4 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_APRIL
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == icu.DateTimeRule.WALL_TIME

    # [5]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfWeekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       TimeMode mode,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        icu.UCalendarMonths.UCAL_MAY,
        -1,
        icu.UCalendarDaysOfWeek.UCAL_THURSDAY,
        5 * HOUR,
        icu.SimpleTimeZone.UTC_TIME,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_THURSDAY
    assert dr.get_rule_millis_in_day() == 5 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_MAY
    assert dr.get_rule_week_in_month() == -1
    assert dr.get_time_rule_type() == icu.DateTimeRule.UTC_TIME

    # [6]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfWeekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        icu.UCalendarMonths.UCAL_JUNE,
        2,
        icu.UCalendarDaysOfWeek.UCAL_WEDNESDAY,
        6 * HOUR,
    )
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == icu.DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == icu.UCalendarDaysOfWeek.UCAL_WEDNESDAY
    assert dr.get_rule_millis_in_day() == 6 * HOUR
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JUNE
    assert dr.get_rule_week_in_month() == 2
    assert dr.get_time_rule_type() == icu.DateTimeRule.WALL_TIME


def test_time_zone() -> None:
    id_ = icu.UnicodeString("PST")

    # static TimeZone *icu::TimeZone::createDefault(void)
    zone1 = icu.TimeZone.create_default()
    assert isinstance(zone1, icu.BasicTimeZone)

    # static TimeZone *icu::TimeZone::createTimeZone(const UnicodeString &ID)
    zone2 = icu.TimeZone.create_time_zone(id_)
    assert isinstance(zone2, icu.BasicTimeZone)

    zone3 = icu.TimeZone.create_time_zone("PST")
    assert isinstance(zone3, icu.BasicTimeZone)
    assert not (zone2 != zone3)
    assert zone2 == zone3

    # static const TimeZone *icu::TimeZone::getGMT(void)
    zone4 = icu.TimeZone.get_gmt()
    assert isinstance(zone4, icu.BasicTimeZone)

    # static void icu::TimeZone::setDefault(const TimeZone &zone)
    icu.TimeZone.set_default(zone1)

    # static int32_t icu::TimeZone::countEquivalentIDs(const UnicodeString &id)
    n = icu.TimeZone.count_equivalent_ids(id_)
    assert n > 0
    ids = [icu.TimeZone.get_equivalent_id(id_, i) for i in range(n)]
    assert all(isinstance(x, icu.UnicodeString) for x in ids)
    assert "America/Los_Angeles" in ids

    assert icu.TimeZone.count_equivalent_ids("PST") == n
    id2 = icu.TimeZone.get_equivalent_id("PST", 0)
    assert ids[0] == id2

    # static StringEnumeration *icu::TimeZone::createTimeZoneIDEnumeration(
    #       USystemTimeZoneType zoneType,
    #       const char *region,
    #       const int32_t *rawOffset,
    #       UErrorCode &ec
    # )
    it5 = icu.TimeZone.create_time_zone_id_enumeration(
        icu.USystemTimeZoneType.UCAL_ZONE_TYPE_ANY, None, None
    )
    assert isinstance(it5, icu.StringEnumeration)
    assert len(it5) > 0

    it6 = icu.TimeZone.create_time_zone_id_enumeration(
        icu.USystemTimeZoneType.UCAL_ZONE_TYPE_ANY, "US", None
    )
    assert isinstance(it6, icu.StringEnumeration)
    assert len(it6) > 0

    raw_offset = -8 * HOUR
    it7 = icu.TimeZone.create_time_zone_id_enumeration(
        icu.USystemTimeZoneType.UCAL_ZONE_TYPE_ANY, None, raw_offset
    )
    assert isinstance(it7, icu.StringEnumeration)
    assert len(it7) > 0

    it8 = icu.TimeZone.create_time_zone_id_enumeration(
        icu.USystemTimeZoneType.UCAL_ZONE_TYPE_ANY, "US", raw_offset
    )
    assert isinstance(it8, icu.StringEnumeration)
    assert len(it8) > 0

    assert len(it5) > len(it6) > len(it7) > len(it8)
    assert "America/Los_Angeles" in it8

    # [1]
    # static UnicodeString &icu::TimeZone::getCanonicalID(
    #       const UnicodeString &id,
    #       UnicodeString &canonicalID,
    #       UBool &isSystemID,
    #       UErrorCode &status
    # )
    canonical_id = icu.UnicodeString()
    result, is_system_id = icu.TimeZone.get_canonical_id(id_, canonical_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(canonical_id)
    assert canonical_id == "America/Los_Angeles"
    assert is_system_id is True

    canonical_id.remove()
    result, is_system_id = icu.TimeZone.get_canonical_id("PST", canonical_id)
    assert id(result) == id(canonical_id)
    assert canonical_id == "America/Los_Angeles"
    assert is_system_id is True

    locale = icu.Locale.get_english()

    # [1]
    # UnicodeString &icu::TimeZone::getDisplayName(
    #       const Locale &locale,
    #       UnicodeString &result
    # )
    result = icu.UnicodeString()
    output = zone2.get_display_name(locale, result)
    assert isinstance(output, icu.UnicodeString)
    assert id(output) == id(result)
    assert result == "Pacific Standard Time"
    assert zone2.get_display_name("en", result) == "Pacific Standard Time"

    # [2]
    # UnicodeString &icu::TimeZone::getDisplayName(
    #       UBool inDaylight,
    #       EDisplayType style,
    #       const Locale &locale,
    #       UnicodeString &result
    # )
    output = zone2.get_display_name(True, icu.TimeZone.LONG, locale, result)
    assert isinstance(output, icu.UnicodeString)
    assert id(output) == id(result)
    assert result == "Pacific Daylight Time"
    assert (
        zone2.get_display_name(True, icu.TimeZone.LONG, "en", result) == "Pacific Daylight Time"
    )

    # [3]
    # UnicodeString &icu::TimeZone::getDisplayName(
    #       UBool inDaylight,
    #       EDisplayType style,
    #       UnicodeString &result
    # )
    output = zone2.get_display_name(True, icu.TimeZone.LONG_GMT, result)
    assert isinstance(output, icu.UnicodeString)
    assert id(output) == id(result)
    assert result == "GMT-07:00"

    # [4]
    # UnicodeString &icu::TimeZone::getDisplayName(UnicodeString &result)
    output = zone2.get_display_name(result)
    assert isinstance(output, icu.UnicodeString)
    assert id(output) == id(result)
    assert len(result) > 0

    # UnicodeString &icu::TimeZone::getID(UnicodeString &ID)
    id2 = icu.UnicodeString()
    result = zone2.get_id(id2)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(id2)
    assert id2 == "PST"

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone2.set_id(icu.UnicodeString("America/Los_Angeles"))
    zone2.get_id(id2)
    assert id2 == "America/Los_Angeles"

    zone2.set_id("US(PST)")
    zone2.get_id(id2)
    assert id2 == "US(PST)"

    # [1]
    # static int32_t icu::TimeZone::getRegion(
    #       const UnicodeString &id,
    #       char *region,
    #       int32_t capacity,
    #       UErrorCode &status
    # )
    region = icu.TimeZone.get_region(id_)
    assert isinstance(region, str)
    assert region == "US"
    assert icu.TimeZone.get_region("PST") == "US"

    # static const char *icu::TimeZone::getTZDataVersion(UErrorCode &status)
    result = icu.TimeZone.get_tz_data_version()
    assert isinstance(result, str)
    assert len(result) > 0  # e.g., "2020d"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM > 69, reason="ICU4C>69")
def test_time_zone_create_enumeration_69() -> None:
    # **Deprecated in ICU 70**
    # [1]
    # static StringEnumeration *icu::TimeZone::createEnumeration()
    it1 = icu.TimeZone.create_enumeration()
    assert isinstance(it1, icu.StringEnumeration)
    assert len(it1) > 0

    # **Deprecated in ICU 70**
    # [2]
    # static StringEnumeration *icu::TimeZone::createEnumeration(
    #       const char *country
    # )
    it2 = icu.TimeZone.create_enumeration(None)
    assert isinstance(it2, icu.StringEnumeration)
    assert len(it2) > 0

    it3 = icu.TimeZone.create_enumeration("US")
    assert isinstance(it3, icu.StringEnumeration)
    assert len(it3) > 0

    # **Deprecated in ICU 70**
    # [3]
    # static StringEnumeration *icu::TimeZone::createEnumeration(
    #       int32_t rawOffset
    # )
    it4 = icu.TimeZone.create_enumeration(-8 * HOUR)
    assert isinstance(it4, icu.StringEnumeration)
    assert len(it4) > 0

    assert len(it1) == len(it2)
    assert len(it1) > len(it3) > len(it4)
    assert "America/Los_Angeles" in it3
    assert "America/Los_Angeles" in it4


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_time_zone_create_enumeration_70() -> None:
    # [4]
    # static StringEnumeration *icu::TimeZone::createEnumeration(
    #       UErrorCode &status
    # )
    it1 = icu.TimeZone.create_enumeration()
    assert isinstance(it1, icu.StringEnumeration)
    assert len(it1) > 0

    # static StringEnumeration *icu::TimeZone::createEnumerationForRawOffset(
    #       int32_t rawOffset,
    #       UErrorCode &status
    # )
    it4 = icu.TimeZone.create_enumeration_for_raw_offset(-8 * HOUR)
    assert isinstance(it4, icu.StringEnumeration)
    assert len(it4) > 0

    # static StringEnumeration *icu::TimeZone::createEnumerationForRegion(
    #       const char *region,
    #       UErrorCode &status
    # )
    it2 = icu.TimeZone.create_enumeration_for_region(None)
    assert isinstance(it2, icu.StringEnumeration)
    assert len(it2) > 0

    it3 = icu.TimeZone.create_enumeration_for_region("US")
    assert isinstance(it3, icu.StringEnumeration)
    assert len(it3) > 0

    assert len(it1) == len(it2)
    assert len(it1) > len(it3) > len(it4)
    assert "America/Los_Angeles" in it3
    assert "America/Los_Angeles" in it4


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_time_zone_detect_host_time_zone() -> None:
    # static TimeZone *icu::TimeZone::detectHostTimeZone()
    zone = icu.TimeZone.detect_host_time_zone()
    assert isinstance(zone, icu.BasicTimeZone)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_time_zone_get_iana_id() -> None:
    # static UnicodeString &
    # icu::TimeZone::getIanaID(
    #       const UnicodeString &id,
    #       UnicodeString &ianaID,
    #       UErrorCode &status
    # )
    iana_id = icu.UnicodeString()
    result = icu.TimeZone.get_iana_id(icu.UnicodeString("Asia/Calcutta"), iana_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(iana_id)
    assert iana_id == "Asia/Kolkata"

    result = icu.TimeZone.get_iana_id("Asia/Calcutta", iana_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(iana_id)
    assert iana_id == "Asia/Kolkata"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_time_zone_get_unknown() -> None:
    # static const TimeZone &icu::TimeZone::getUnknown()
    zone = icu.TimeZone.get_unknown()
    assert isinstance(zone, icu.BasicTimeZone)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_time_zone_get_windows_id() -> None:
    # static UnicodeString &icu::TimeZone::getWindowsID(
    #       const UnicodeString &id,
    #       UnicodeString &winid,
    #       UErrorCode &status
    # )
    id_ = icu.UnicodeString("America/New_York")
    winid = icu.UnicodeString()
    result = icu.TimeZone.get_windows_id(id_, winid)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(winid)
    assert winid == "Eastern Standard Time"
    assert icu.TimeZone.get_windows_id("America/New_York", winid) == "Eastern Standard Time"

    # static UnicodeString &icu::TimeZone::getIDForWindowsID(
    #       const UnicodeString &winid,
    #       const char *region,
    #       UnicodeString &id,
    #       UErrorCode &status
    # )
    id_ = icu.UnicodeString()
    result = icu.TimeZone.get_id_for_windows_id(winid, None, id_)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(id_)
    assert id_ == "America/New_York"
    assert (
        icu.TimeZone.get_id_for_windows_id("Eastern Standard Time", None, id_)
        == "America/New_York"
    )

    id_ = icu.UnicodeString()
    result = icu.TimeZone.get_id_for_windows_id(winid, "CA", id_)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(id_)
    assert id_ == "America/Toronto"
    assert (
        icu.TimeZone.get_id_for_windows_id("Eastern Standard Time", "CA", id_)
        == "America/Toronto"
    )


def test_time_zone_has_same_rules() -> None:
    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    id_ = "Europe/Moscow"
    otz = icu.TimeZone.create_time_zone(id_)
    vtz1 = icu.VTimeZone.create_vtime_zone_by_id(id_)
    assert otz.has_same_rules(vtz1) is False
    assert vtz1.has_same_rules(otz) is True

    stz1 = icu.SimpleTimeZone(28800000, "Asia/Singapore")
    stz1.set_start_year(1970)
    stz1.set_start_rule(0, 1, 0)
    stz1.set_end_rule(1, 1, 0)
    vtz2 = icu.VTimeZone.create_vtime_zone_from_basic_time_zone(stz1)
    stz2 = stz1.clone()
    assert vtz2.has_same_rules(stz2) is True


def test_vtime_zone() -> None:
    assert issubclass(icu.VTimeZone, icu.BasicTimeZone)
    assert issubclass(icu.VTimeZone, icu.TimeZone)

    # static VTimeZone *icu::VTimeZone::createVTimeZoneByID(
    #       const UnicodeString &ID
    # )
    id1 = icu.UnicodeString("America/New_York")
    zone1 = icu.VTimeZone.create_vtime_zone_by_id(id1)
    assert isinstance(zone1, icu.VTimeZone)
    zone1.set_tzurl(icu.UnicodeString("http://source.icu-project.org/timezone"))
    zone1.set_last_modified(1215298800000.0)  # 2008-07-05T23:00:00Z
    vtzdata = icu.UnicodeString()
    zone1.write(vtzdata)

    # static VTimeZone *icu::VTimeZone::createVTimeZone(
    #       const UnicodeString &vtzdata,
    #       UErrorCode &status
    # )
    zone2 = icu.VTimeZone.create_vtime_zone(vtzdata)
    assert isinstance(zone2, icu.VTimeZone)

    # static VTimeZone *icu::VTimeZone::createVTimeZoneFromBasicTimeZone(
    #       const BasicTimeZone &basicTZ,
    #       UErrorCode &status
    # )
    basic_tz = icu.SimpleTimeZone(
        0,
        "s3",
        icu.UCalendarMonths.UCAL_JANUARY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        icu.UCalendarMonths.UCAL_JULY,
        1,
        icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
    )
    zone3 = icu.VTimeZone.create_vtime_zone_from_basic_time_zone(basic_tz)
    assert isinstance(zone3, icu.VTimeZone)

    # icu::VTimeZone::VTimeZone(const VTimeZone &source)
    zone4 = icu.VTimeZone(zone3)

    # VTimeZone *icu::VTimeZone::clone()
    zone1a = zone1.clone()
    assert isinstance(zone1a, icu.VTimeZone)
    assert zone1a == zone1

    zone1b = copy.copy(zone1)
    assert zone1b == zone1

    zone1c = copy.deepcopy(zone1)
    assert zone1c == zone1

    # UBool icu::TimeZone::operator!=(const TimeZone &that)
    assert zone1 != zone2
    assert zone1 != zone3
    assert zone1 != zone4
    assert zone2 != zone3
    assert zone2 != zone4
    assert not (zone3 != zone4)

    # UBool icu::VTimeZone::operator==(const TimeZone &that)
    assert not (zone1 == zone2)
    assert not (zone1 == zone3)
    assert not (zone1 == zone4)
    assert not (zone2 == zone3)
    assert not (zone2 == zone4)
    assert zone3 == zone4

    # int32_t icu::BasicTimeZone::countTransitionRules(UErrorCode &status)
    assert zone1.count_transition_rules() == 4
    assert zone2.count_transition_rules() == 17
    assert zone3.count_transition_rules() == 2
    assert zone4.count_transition_rules() == 2

    # int32_t icu::TimeZone::getDSTSavings()
    assert zone1.get_dst_savings() == 1 * HOUR
    assert zone2.get_dst_savings() == 1 * HOUR
    assert zone3.get_dst_savings() == 1 * HOUR

    # UnicodeString &icu::TimeZone::getID(UnicodeString &ID)
    id_ = icu.UnicodeString()
    result = zone1.get_id(id_)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(id_)
    assert len(result) == 0

    # UBool icu::VTimeZone::getLastModified(UDate &lastModified)
    assert zone1.get_last_modified() == (True, 1215298800000.0)
    assert zone2.get_last_modified() == (True, 1215298800000.0)
    result, _ = zone3.get_last_modified()
    assert result is False

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    tzt = icu.TimeZoneTransition()
    assert zone3.get_next_transition(base, False, tzt) is True
    assert tzt.get_time() == 1231027200000.0  # 2009-01-04T00:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone3.get_previous_transition(base, False, tzt) is True
    assert tzt.get_time() == 1215298800000.0  # 2008-07-05T23:00:00Z

    # int32_t icu::TimeZone::getRawOffset(void)
    assert zone1.get_raw_offset() == -5 * HOUR
    assert zone2.get_raw_offset() == -5 * HOUR
    assert zone3.get_raw_offset() == 0

    # void icu::BasicTimeZone::getSimpleRulesNear(
    #       UDate date,
    #       InitialTimeZoneRule *&initial,
    #       AnnualTimeZoneRule *&std,
    #       AnnualTimeZoneRule *&dst,
    #       UErrorCode &status
    # )
    initial, std, dst = zone1.get_simple_rules_near(0)
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == -5 * HOUR
    assert initial.get_dst_savings() == 1 * HOUR
    assert isinstance(std, icu.AnnualTimeZoneRule)
    assert std.get_raw_offset() == -5 * HOUR
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_OCTOBER
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, icu.AnnualTimeZoneRule)
    assert dst.get_raw_offset() == -5 * HOUR
    assert dst.get_dst_savings() == 1 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_APRIL
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone3.get_time_zone_rules()
    assert isinstance(initial, icu.InitialTimeZoneRule)
    assert initial.get_raw_offset() == 0
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    assert all(isinstance(x, icu.AnnualTimeZoneRule) for x in trsrules)
    assert trsrules[0].get_raw_offset() == 0
    assert trsrules[0].get_dst_savings() == 0
    dr = trsrules[0].get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JULY
    assert dr.get_rule_day_of_month() == 0
    assert trsrules[1].get_raw_offset() == 0
    assert trsrules[1].get_dst_savings() == 1 * HOUR
    dr = trsrules[1].get_rule()
    assert dr.get_rule_month() == icu.UCalendarMonths.UCAL_JANUARY
    assert dr.get_rule_day_of_month() == 0

    # UBool icu::VTimeZone::getTZURL(UnicodeString &url)
    url = icu.UnicodeString()
    assert zone1.get_tzurl(url) is True
    assert url == "http://source.icu-project.org/timezone"
    assert zone3.get_tzurl(url) is False

    # UBool icu::BasicTimeZone::hasEquivalentTransitions(
    #       const BasicTimeZone &tz,
    #       UDate start,
    #       UDate end,
    #       UBool ignoreDstAmount,
    #       UErrorCode &ec
    # )
    start = 1215298800000.0  # 2008-07-05T23:00:00Z
    end = 1231027200000.0  # 2009-01-04T00:00:00Z
    assert zone1.has_equivalent_transitions(zone3, start, end, False) is False
    assert zone3.has_equivalent_transitions(zone1, start, end, False) is False

    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    assert zone1.has_same_rules(zone2) is False
    assert zone2.has_same_rules(zone3) is False

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone1.set_id(icu.UnicodeString("abc"))
    assert zone1.get_id(id_) == "abc"
    zone1.set_id("ABC")
    assert zone1.get_id(id_) == "ABC"

    # void icu::TimeZone::setRawOffset(int32_t offsetMillis)
    zone1.set_raw_offset(5 * HOUR)  # Nothing to do
    assert zone1.get_raw_offset() == -5 * HOUR

    # void icu::VTimeZone::setTZURL(const UnicodeString &url)
    zone1.set_tzurl("http://www.example.com")
    assert zone1.get_tzurl(url) is True
    assert url == "http://www.example.com"

    # UBool icu::TimeZone::useDaylightTime(void)
    assert zone1.use_daylight_time() is True
    assert zone2.use_daylight_time() is True
    assert zone3.use_daylight_time() is True

    # [1]
    # void icu::VTimeZone::write(
    #       UDate start,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result1 = icu.UnicodeString()
    zone1.write(1231027200000.0, result1)
    assert len(result1) > 0

    # [2]
    # void icu::VTimeZone::write(
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result2 = icu.UnicodeString()
    zone1.write(result2)
    assert len(result2) > 0

    # void icu::VTimeZone::writeSimple(
    #       UDate time,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result3 = icu.UnicodeString()
    zone1.write_simple(1231027200000.0, result3)
    assert len(result3) > 0


def test_vtime_zone_get_offset() -> None:
    basic_tz = icu.SimpleTimeZone(
        8 * HOUR,
        "s5",
        icu.UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        icu.UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        icu.SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR),
    )
    zone = icu.VTimeZone.create_vtime_zone_from_basic_time_zone(basic_tz)

    # [1]
    # void icu::VTimeZone::getOffset(
    #       UDate date,
    #       UBool local,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &ec
    # )
    raw_offset, dst_offset = zone.get_offset(0, False)
    assert raw_offset == 8 * HOUR
    assert dst_offset == 0

    # [2]
    # int32_t icu::VTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t millis,
    #       int32_t monthLength,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
            28,
        )
        == 8.5 * HOUR
    )

    # [3]
    # int32_t icu::VTimeZone::getOffset(
    #       uint8_t era,
    #       int32_t year,
    #       int32_t month,
    #       int32_t day,
    #       uint8_t dayOfWeek,
    #       int32_t millis,
    #       UErrorCode &status
    # )
    assert (
        zone.get_offset(
            icu.GregorianCalendar.AD,
            2008,
            icu.UCalendarMonths.UCAL_JULY,
            6,
            icu.UCalendarDaysOfWeek.UCAL_SUNDAY,
            0,
        )
        == 8.5 * HOUR
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_vtime_zone_get_offset_from_local() -> None:
    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = icu.VTimeZone.create_vtime_zone_by_id("America/Los_Angeles")

    # void icu::VTimeZone::getOffsetFromLocal(
    #       UDate date,
    #       UTimeZoneLocalOption nonExistingTimeOpt,
    #       UTimeZoneLocalOption duplicatedTimeOpt,
    #       int32_t &rawOffset,
    #       int32_t &dstOffset,
    #       UErrorCode &status
    # )
    date = 1143941400000  # 2006-04-02T01:30:00Z
    raw_offset, dst_offset = zone.get_offset_from_local(
        date,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        icu.UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER,
    )
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0

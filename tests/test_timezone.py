import copy

import pytest
from icupy.icu import (
    AnnualTimeZoneRule, BasicTimeZone, DateTimeRule, GregorianCalendar,
    InitialTimeZoneRule, Locale, RuleBasedTimeZone, SimpleTimeZone,
    StringEnumeration, TimeZone, TimeZoneTransition, UCalendarDaysOfWeek,
    UCalendarMonths, USystemTimeZoneType, U_ICU_VERSION_MAJOR_NUM,
    U_MILLIS_PER_HOUR as HOUR, UnicodeString, VTimeZone,
)


def test_basic_time_zone():
    assert issubclass(BasicTimeZone, TimeZone)

    zone1 = TimeZone.create_time_zone("America/Los_Angeles")
    assert isinstance(zone1, BasicTimeZone)  # OlsonTimeZone
    assert not isinstance(zone1, RuleBasedTimeZone)
    assert not isinstance(zone1, SimpleTimeZone)
    assert not isinstance(zone1, VTimeZone)

    # BasicTimeZone *icu::BasicTimeZone::clone()
    zone1a = zone1.clone()
    assert isinstance(zone1a, BasicTimeZone)
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
    result = TimeZoneTransition()
    assert zone1.get_next_transition(base, False, result)
    assert result.get_time() == 1236506400000.0  # 2009-03-08T10:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone1.get_previous_transition(base, False, result)
    assert result.get_time() == 1225616400000.0  # 2008-11-02T09:00:00Z

    # void icu::BasicTimeZone::getSimpleRulesNear(
    #       UDate date,
    #       InitialTimeZoneRule *&initial,
    #       AnnualTimeZoneRule *&std,
    #       AnnualTimeZoneRule *&dst,
    #       UErrorCode &status
    # )
    initial, std, dst = zone1.get_simple_rules_near(0)
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == -8 * HOUR
    assert initial.get_dst_savings() == 1 * HOUR
    assert isinstance(std, AnnualTimeZoneRule)
    assert std.get_raw_offset() == -8 * HOUR
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_OCTOBER
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, AnnualTimeZoneRule)
    assert dst.get_raw_offset() == -8 * HOUR
    assert dst.get_dst_savings() == 1 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_APRIL
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone1.get_time_zone_rules()
    assert isinstance(initial, InitialTimeZoneRule)
    assert isinstance(trsrules, list)
    assert len(trsrules) == 4
    assert any(isinstance(x, AnnualTimeZoneRule) for x in trsrules)

    # UBool icu::BasicTimeZone::hasEquivalentTransitions(
    #       const BasicTimeZone &tz,
    #       UDate start,
    #       UDate end,
    #       UBool ignoreDstAmount,
    #       UErrorCode &ec
    # )
    # From icu/source/test/intltest/tzrulets.cpp:
    # TimeZoneRuleTest::TestHasEquivalentTransitions()
    zone5 = TimeZone.create_time_zone("America/New_York")
    zone6 = TimeZone.create_time_zone("America/Indiana/Indianapolis")
    start = 1104537600000.0  # 2005-01-01T00:00:00Z
    end = 1262304000000.0  # 2010-01-01T00:00:00Z
    assert not zone5.has_equivalent_transitions(zone6, start, end, True)
    start = 1136073600000.0  # 2006-01-01T00:00:00Z
    assert zone5.has_equivalent_transitions(zone6, start, end, True)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_basic_time_zone_get_offset_from_local():
    from icupy.icu import UTimeZoneLocalOption

    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = TimeZone.create_time_zone("America/Los_Angeles")
    assert isinstance(zone, BasicTimeZone)

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
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER)
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


def test_rule_based_time_zone():
    assert issubclass(RuleBasedTimeZone, BasicTimeZone)
    assert issubclass(RuleBasedTimeZone, TimeZone)
    initial_rule1 = InitialTimeZoneRule("STD", 0, 0)
    initial_rule2 = InitialTimeZoneRule("STD", 5 * HOUR, 0)

    # [1]
    # icu::RuleBasedTimeZone::RuleBasedTimeZone(
    #       const UnicodeString &id,
    #       InitialTimeZoneRule *initialRule
    # )
    zone1 = RuleBasedTimeZone(UnicodeString("r1"), initial_rule1)
    dr11 = DateTimeRule(
        UCalendarMonths.UCAL_JULY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        DateTimeRule.WALL_TIME)
    tzr11 = AnnualTimeZoneRule(
        "r1(STD)",
        0,
        0,
        dr11,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    zone1.add_transition_rule(tzr11)
    dr12 = DateTimeRule(
        UCalendarMonths.UCAL_JANUARY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        DateTimeRule.WALL_TIME)
    tzr12 = AnnualTimeZoneRule(
        "r1(DST)",
        0,
        1 * HOUR,
        dr12,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    zone1.add_transition_rule(tzr12)
    zone1.complete()

    zone2 = RuleBasedTimeZone(UnicodeString("r2"), initial_rule2)
    dr21 = DateTimeRule(
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        5 * HOUR,
        DateTimeRule.WALL_TIME)
    tzr21 = AnnualTimeZoneRule(
        "r2(STD)",
        5 * HOUR,
        0,
        dr21,
        2000,
        2010)
    zone2.add_transition_rule(tzr21)
    dr22 = DateTimeRule(
        UCalendarMonths.UCAL_MARCH,
        -1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
        DateTimeRule.WALL_TIME)
    tzr22 = AnnualTimeZoneRule(
        "r2(DST)",
        5 * HOUR,
        1 * HOUR,
        dr22,
        2000,
        2010)
    zone2.add_transition_rule(tzr22)
    dr23 = DateTimeRule(
        UCalendarMonths.UCAL_JANUARY,
        1,
        0,
        DateTimeRule.WALL_TIME)
    tzr23 = AnnualTimeZoneRule(
        "r2(STD)",
        6 * HOUR,
        0,
        dr23,
        2011,
        AnnualTimeZoneRule.MAX_YEAR)
    zone2.add_transition_rule(tzr23)
    dr24 = DateTimeRule(
        UCalendarMonths.UCAL_JANUARY,
        1,
        1,
        DateTimeRule.WALL_TIME)
    tzr24 = AnnualTimeZoneRule(
        "r2(DST)",
        6 * HOUR,
        0,
        dr24,
        2011,
        AnnualTimeZoneRule.MAX_YEAR)
    zone2.add_transition_rule(tzr24)
    zone2.complete()

    # [2]
    # icu::RuleBasedTimeZone::RuleBasedTimeZone(
    #       const RuleBasedTimeZone &source
    # )
    zone3 = RuleBasedTimeZone(zone2)

    # RuleBasedTimeZone *icu::RuleBasedTimeZone::clone()
    zone2a = zone2.clone()
    assert isinstance(zone2a, RuleBasedTimeZone)
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
    id_ = UnicodeString()
    result = zone1.get_id(id_)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(id_)
    assert result == "r1"

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    tzt = TimeZoneTransition()
    assert zone1.get_next_transition(base, False, tzt)
    assert tzt.get_time() == 1231027200000.0  # 2009-01-04T00:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone1.get_previous_transition(base, False, tzt)
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
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == 0
    assert initial.get_dst_savings() == 1 * HOUR
    assert isinstance(std, AnnualTimeZoneRule)
    assert std.get_raw_offset() == 0
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JULY
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, AnnualTimeZoneRule)
    assert dst.get_raw_offset() == 0
    assert dst.get_dst_savings() == 1 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JANUARY
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone2.get_time_zone_rules()
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == 5 * HOUR
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 4
    assert all(isinstance(x, AnnualTimeZoneRule) for x in trsrules)
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
    assert not zone1.has_equivalent_transitions(zone2, start, end, False)
    assert not zone2.has_equivalent_transitions(zone1, start, end, False)

    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    assert not zone1.has_same_rules(zone2)
    assert zone2.has_same_rules(zone3)

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone1.set_id(UnicodeString("abc"))
    assert zone1.get_id(id_) == "abc"
    zone1.set_id("ABC")
    assert zone1.get_id(id_) == "ABC"

    # void icu::TimeZone::setRawOffset(int32_t offsetMillis)
    zone1.set_raw_offset(-5 * HOUR)  # Nothing to do
    assert zone1.get_raw_offset() == 0

    # UBool icu::TimeZone::useDaylightTime(void)
    assert zone1.use_daylight_time()
    assert not zone2.use_daylight_time()


def test_rule_based_time_zone_get_offset():
    initial_rule = InitialTimeZoneRule(
        "r1(STD)",
        8 * HOUR,
        0)
    zone = RuleBasedTimeZone("r1", initial_rule)
    dr1 = DateTimeRule(
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
        DateTimeRule.WALL_TIME)
    tzr1 = AnnualTimeZoneRule(
        "r1(STD)",
        8 * HOUR,
        0,
        dr1,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
    zone.add_transition_rule(tzr1)
    dr2 = DateTimeRule(
        UCalendarMonths.UCAL_APRIL,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        True,
        2 * HOUR,
        DateTimeRule.WALL_TIME)
    tzr2 = AnnualTimeZoneRule(
        "r1(DST)",
        8 * HOUR,
        int(0.5 * HOUR),
        dr2,
        0,
        AnnualTimeZoneRule.MAX_YEAR)
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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        28) == 8.5 * HOUR

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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0) == 8.5 * HOUR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_rule_based_time_zone_get_offset_from_local():
    from icupy.icu import UTimeZoneLocalOption

    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = RuleBasedTimeZone(
        "Rule based Pacific Time",
        InitialTimeZoneRule("Pacific Standard Time", -8 * HOUR, 0)
    )
    zone.add_transition_rule(
        AnnualTimeZoneRule(
            "Pacific Daylight Time",
            -8 * HOUR,
            1 * HOUR,
            DateTimeRule(
                UCalendarMonths.UCAL_APRIL,
                1,
                UCalendarDaysOfWeek.UCAL_SUNDAY,
                2 * HOUR,
                DateTimeRule.WALL_TIME
            ),
            2000,
            AnnualTimeZoneRule.MAX_YEAR
        )
    )
    zone.add_transition_rule(
        AnnualTimeZoneRule(
            "Pacific Standard Time",
            -8 * HOUR,
            0,
            DateTimeRule(
                UCalendarMonths.UCAL_OCTOBER,
                -1,
                UCalendarDaysOfWeek.UCAL_SUNDAY,
                2 * HOUR,
                DateTimeRule.WALL_TIME
            ),
            2000,
            AnnualTimeZoneRule.MAX_YEAR
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
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER)
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


def test_simple_time_zone():
    assert issubclass(SimpleTimeZone, BasicTimeZone)
    assert issubclass(SimpleTimeZone, TimeZone)

    # [2]
    # icu::SimpleTimeZone::SimpleTimeZone(
    #       int32_t rawOffsetGMT,
    #       const UnicodeString &ID
    # )
    zone2 = SimpleTimeZone(8 * HOUR, UnicodeString("s2"))

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
    zone3 = SimpleTimeZone(
        0,
        UnicodeString("s3"),
        UCalendarMonths.UCAL_JANUARY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        UCalendarMonths.UCAL_JULY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0)

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
    zone4 = SimpleTimeZone(
        -5 * HOUR,
        UnicodeString("s4"),
        UCalendarMonths.UCAL_MARCH,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        UCalendarMonths.UCAL_SEPTEMBER,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        int(0.5 * HOUR))

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
    zone5 = SimpleTimeZone(
        8 * HOUR,
        UnicodeString("s5"),
        UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR))

    # [1]
    # icu::SimpleTimeZone::SimpleTimeZone(const SimpleTimeZone &source)
    zone1 = SimpleTimeZone(zone5)

    # SimpleTimeZone *icu::SimpleTimeZone::clone()
    zone1a = zone1.clone()
    assert isinstance(zone1a, SimpleTimeZone)
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
    id_ = UnicodeString()
    result = zone1.get_id(id_)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(id_)
    assert result == "s5"

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    tzt = TimeZoneTransition()
    assert zone3.get_next_transition(base, False, tzt)
    assert tzt.get_time() == 1231027200000.0  # 2009-01-04T00:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone3.get_previous_transition(base, False, tzt)
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
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == 8 * HOUR
    assert initial.get_dst_savings() == 0
    assert std is None
    assert dst is None

    initial, std, dst = zone4.get_simple_rules_near(0)
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == -5 * HOUR
    assert initial.get_dst_savings() == 0.5 * HOUR
    assert isinstance(std, AnnualTimeZoneRule)
    assert std.get_raw_offset() == -5 * HOUR
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_SEPTEMBER
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, AnnualTimeZoneRule)
    assert dst.get_raw_offset() == -5 * HOUR
    assert dst.get_dst_savings() == 0.5 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_MARCH
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone2.get_time_zone_rules()
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == 8 * HOUR
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 0

    initial, trsrules = zone4.get_time_zone_rules()
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == -5 * HOUR
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    assert all(isinstance(x, AnnualTimeZoneRule) for x in trsrules)
    assert trsrules[0].get_raw_offset() == -5 * HOUR
    assert trsrules[0].get_dst_savings() == 0
    dr = trsrules[0].get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_SEPTEMBER
    assert dr.get_rule_day_of_month() == 0
    assert trsrules[1].get_raw_offset() == -5 * HOUR
    assert trsrules[1].get_dst_savings() == 0.5 * HOUR
    dr = trsrules[1].get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_MARCH
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
    assert not zone5.has_equivalent_transitions(zone4, start, end, False)
    assert zone5.has_equivalent_transitions(zone1, start, end, False)

    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    assert not zone5.has_same_rules(zone4)
    assert zone5.has_same_rules(zone1)

    # void icu::SimpleTimeZone::setDSTSavings(
    #       int32_t millisSavedDuringDST,
    #       UErrorCode &status
    # )
    zone1.set_dst_savings(1 * HOUR)
    assert zone1.get_dst_savings() == 1 * HOUR
    zone2.set_dst_savings(int(0.5 * HOUR))
    assert zone2.get_dst_savings() == 0.5 * HOUR

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone1.set_id(UnicodeString("abc"))
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
    assert zone1.use_daylight_time()
    assert not zone2.use_daylight_time()
    assert zone3.use_daylight_time()
    assert zone4.use_daylight_time()
    assert zone5.use_daylight_time()


def test_simple_time_zone_get_offset():
    zone = SimpleTimeZone(
        8 * HOUR,
        "s5",
        UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR))

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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0) == 8.5 * HOUR

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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        28,
        28) == 8.5 * HOUR

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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        28) == 8.5 * HOUR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_simple_time_zone_get_offset_from_local():
    from icupy.icu import UTimeZoneLocalOption

    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = SimpleTimeZone(
        -8 * HOUR,
        "Simple Pacific Time",
        UCalendarMonths.UCAL_APRIL,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR,
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        2 * HOUR)

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
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER)
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0


def test_simple_time_zone_set_end_rule():
    zone = SimpleTimeZone(
        8 * HOUR,
        "s5",
        UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR))

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
        UCalendarMonths.UCAL_DECEMBER,
        31,
        UCalendarDaysOfWeek.UCAL_SATURDAY,
        6 * HOUR,
        SimpleTimeZone.UTC_TIME,
        True)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_SATURDAY
    assert dr.get_rule_millis_in_day() == 6 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_DECEMBER
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.UTC_TIME

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
        UCalendarMonths.UCAL_OCTOBER,
        31,
        UCalendarDaysOfWeek.UCAL_FRIDAY,
        5 * HOUR,
        True)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_FRIDAY
    assert dr.get_rule_millis_in_day() == 5 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_OCTOBER
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.WALL_TIME

    # [3]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       TimeMode mode,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        UCalendarMonths.UCAL_SEPTEMBER,
        30,
        4 * HOUR,
        SimpleTimeZone.UTC_TIME)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 30
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 4 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_SEPTEMBER
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.UTC_TIME

    # [4]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        UCalendarMonths.UCAL_AUGUST,
        31,
        3 * HOUR)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 3 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_AUGUST
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.WALL_TIME

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
        UCalendarMonths.UCAL_JULY,
        -1,
        UCalendarDaysOfWeek.UCAL_THURSDAY,
        2 * HOUR,
        SimpleTimeZone.UTC_TIME)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_THURSDAY
    assert dr.get_rule_millis_in_day() == 2 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JULY
    assert dr.get_rule_week_in_month() == -1
    assert dr.get_time_rule_type() == DateTimeRule.UTC_TIME

    # [6]
    # void icu::SimpleTimeZone::setEndRule(
    #       int32_t month,
    #       int32_t dayOfWeekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_end_rule(
        UCalendarMonths.UCAL_JUNE,
        2,
        UCalendarDaysOfWeek.UCAL_WEDNESDAY,
        1 * HOUR)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[0].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_WEDNESDAY
    assert dr.get_rule_millis_in_day() == 1 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JUNE
    assert dr.get_rule_week_in_month() == 2
    assert dr.get_time_rule_type() == DateTimeRule.WALL_TIME


def test_simple_time_zone_set_start_rule():
    zone = SimpleTimeZone(
        8 * HOUR,
        "s5",
        UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR))

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
        UCalendarMonths.UCAL_JANUARY,
        31,
        UCalendarDaysOfWeek.UCAL_SATURDAY,
        1 * HOUR,
        SimpleTimeZone.UTC_TIME,
        True)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_SATURDAY
    assert dr.get_rule_millis_in_day() == 1 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JANUARY
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.UTC_TIME

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
        UCalendarMonths.UCAL_FEBRUARY,
        28,
        UCalendarDaysOfWeek.UCAL_FRIDAY,
        2 * HOUR,
        True)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW_GEQ_DOM
    assert dr.get_rule_day_of_month() == 28
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_FRIDAY
    assert dr.get_rule_millis_in_day() == 2 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_FEBRUARY
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.WALL_TIME

    # [3]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       TimeMode mode,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        UCalendarMonths.UCAL_MARCH,
        31,
        3 * HOUR,
        SimpleTimeZone.UTC_TIME)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 31
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 3 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_MARCH
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.UTC_TIME

    # [4]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfMonth,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        UCalendarMonths.UCAL_APRIL,
        30,
        4 * HOUR)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOM
    assert dr.get_rule_day_of_month() == 30
    assert dr.get_rule_day_of_week() == 0
    assert dr.get_rule_millis_in_day() == 4 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_APRIL
    assert dr.get_rule_week_in_month() == 0
    assert dr.get_time_rule_type() == DateTimeRule.WALL_TIME

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
        UCalendarMonths.UCAL_MAY,
        -1,
        UCalendarDaysOfWeek.UCAL_THURSDAY,
        5 * HOUR,
        SimpleTimeZone.UTC_TIME)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_THURSDAY
    assert dr.get_rule_millis_in_day() == 5 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_MAY
    assert dr.get_rule_week_in_month() == -1
    assert dr.get_time_rule_type() == DateTimeRule.UTC_TIME

    # [6]
    # void icu::SimpleTimeZone::setStartRule(
    #       int32_t month,
    #       int32_t dayOfWeekInMonth,
    #       int32_t dayOfWeek,
    #       int32_t time,
    #       UErrorCode &status
    # )
    zone.set_start_rule(
        UCalendarMonths.UCAL_JUNE,
        2,
        UCalendarDaysOfWeek.UCAL_WEDNESDAY,
        6 * HOUR)
    _, trsrules = zone.get_time_zone_rules()
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    dr = trsrules[1].get_rule()
    assert dr.get_date_rule_type() == DateTimeRule.DOW
    assert dr.get_rule_day_of_month() == 0
    assert dr.get_rule_day_of_week() == UCalendarDaysOfWeek.UCAL_WEDNESDAY
    assert dr.get_rule_millis_in_day() == 6 * HOUR
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JUNE
    assert dr.get_rule_week_in_month() == 2
    assert dr.get_time_rule_type() == DateTimeRule.WALL_TIME


def test_time_zone():
    id_ = UnicodeString("PST")

    # static TimeZone *icu::TimeZone::createDefault(void)
    zone1 = TimeZone.create_default()
    assert isinstance(zone1, BasicTimeZone)

    # static TimeZone *icu::TimeZone::createTimeZone(const UnicodeString &ID)
    zone2 = TimeZone.create_time_zone(id_)
    assert isinstance(zone2, BasicTimeZone)

    zone3 = TimeZone.create_time_zone("PST")
    assert isinstance(zone3, BasicTimeZone)
    assert not (zone2 != zone3)
    assert zone2 == zone3

    # static const TimeZone *icu::TimeZone::getGMT(void)
    zone4 = TimeZone.get_gmt()
    assert isinstance(zone4, BasicTimeZone)

    # static void icu::TimeZone::setDefault(const TimeZone &zone)
    TimeZone.set_default(zone1)

    # static int32_t icu::TimeZone::countEquivalentIDs(const UnicodeString &id)
    n = TimeZone.count_equivalent_ids(id_)
    assert n > 0
    ids = [TimeZone.get_equivalent_id(id_, i) for i in range(n)]
    assert all(isinstance(x, UnicodeString) for x in ids)
    assert "America/Los_Angeles" in ids

    assert TimeZone.count_equivalent_ids("PST") == n
    id2 = TimeZone.get_equivalent_id("PST", 0)
    assert ids[0] == id2

    # static StringEnumeration *icu::TimeZone::createTimeZoneIDEnumeration(
    #       USystemTimeZoneType zoneType,
    #       const char *region,
    #       const int32_t *rawOffset,
    #       UErrorCode &ec
    # )
    it5 = TimeZone.create_time_zone_id_enumeration(
        USystemTimeZoneType.UCAL_ZONE_TYPE_ANY,
        None,
        None)
    assert isinstance(it5, StringEnumeration)
    assert len(it5) > 0

    it6 = TimeZone.create_time_zone_id_enumeration(
        USystemTimeZoneType.UCAL_ZONE_TYPE_ANY,
        "US",
        None)
    assert isinstance(it6, StringEnumeration)
    assert len(it6) > 0

    raw_offset = -8 * HOUR
    it7 = TimeZone.create_time_zone_id_enumeration(
        USystemTimeZoneType.UCAL_ZONE_TYPE_ANY,
        None,
        raw_offset)
    assert isinstance(it7, StringEnumeration)
    assert len(it7) > 0

    it8 = TimeZone.create_time_zone_id_enumeration(
        USystemTimeZoneType.UCAL_ZONE_TYPE_ANY,
        "US",
        raw_offset)
    assert isinstance(it8, StringEnumeration)
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
    canonical_id = UnicodeString()
    result, is_system_id = TimeZone.get_canonical_id(id_, canonical_id)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(canonical_id)
    assert canonical_id == "America/Los_Angeles"
    assert is_system_id

    canonical_id.remove()
    result, is_system_id = TimeZone.get_canonical_id("PST", canonical_id)
    assert id(result) == id(canonical_id)
    assert canonical_id == "America/Los_Angeles"
    assert is_system_id

    locale = Locale.get_english()

    # [1]
    # UnicodeString &icu::TimeZone::getDisplayName(
    #       const Locale &locale,
    #       UnicodeString &result
    # )
    result = UnicodeString()
    output = zone2.get_display_name(locale, result)
    assert isinstance(output, UnicodeString)
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
    output = zone2.get_display_name(True, TimeZone.LONG, locale, result)
    assert isinstance(output, UnicodeString)
    assert id(output) == id(result)
    assert result == "Pacific Daylight Time"
    assert (zone2.get_display_name(True, TimeZone.LONG, "en", result)
            == "Pacific Daylight Time")

    # [3]
    # UnicodeString &icu::TimeZone::getDisplayName(
    #       UBool inDaylight,
    #       EDisplayType style,
    #       UnicodeString &result
    # )
    output = zone2.get_display_name(True, TimeZone.LONG_GMT, result)
    assert isinstance(output, UnicodeString)
    assert id(output) == id(result)
    assert result == "GMT-07:00"

    # [4]
    # UnicodeString &icu::TimeZone::getDisplayName(UnicodeString &result)
    output = zone2.get_display_name(result)
    assert isinstance(output, UnicodeString)
    assert id(output) == id(result)
    assert len(result) > 0

    # UnicodeString &icu::TimeZone::getID(UnicodeString &ID)
    id2 = UnicodeString()
    result = zone2.get_id(id2)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(id2)
    assert id2 == "PST"

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone2.set_id(UnicodeString("America/Los_Angeles"))
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
    region = TimeZone.get_region(id_)
    assert isinstance(region, str)
    assert region == "US"
    assert TimeZone.get_region("PST") == "US"

    # static const char *icu::TimeZone::getTZDataVersion(UErrorCode &status)
    result = TimeZone.get_tz_data_version()
    assert isinstance(result, str)
    assert len(result) > 0  # e.g., "2020d"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM > 69, reason="ICU4C>69")
def test_time_zone_create_enumeration_69():
    # **Deprecated in ICU 70**
    # [1]
    # static StringEnumeration *icu::TimeZone::createEnumeration()
    it1 = TimeZone.create_enumeration()
    assert isinstance(it1, StringEnumeration)
    assert len(it1) > 0

    # **Deprecated in ICU 70**
    # [2]
    # static StringEnumeration *icu::TimeZone::createEnumeration(
    #       const char *country
    # )
    it2 = TimeZone.create_enumeration(None)
    assert isinstance(it2, StringEnumeration)
    assert len(it2) > 0

    it3 = TimeZone.create_enumeration("US")
    assert isinstance(it3, StringEnumeration)
    assert len(it3) > 0

    # **Deprecated in ICU 70**
    # [3]
    # static StringEnumeration *icu::TimeZone::createEnumeration(
    #       int32_t rawOffset
    # )
    it4 = TimeZone.create_enumeration(-8 * HOUR)
    assert isinstance(it4, StringEnumeration)
    assert len(it4) > 0

    assert len(it1) == len(it2)
    assert len(it1) > len(it3) > len(it4)
    assert "America/Los_Angeles" in it3
    assert "America/Los_Angeles" in it4


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_time_zone_create_enumeration_70():
    # [4]
    # static StringEnumeration *icu::TimeZone::createEnumeration(
    #       UErrorCode &status
    # )
    it1 = TimeZone.create_enumeration()
    assert isinstance(it1, StringEnumeration)
    assert len(it1) > 0

    # static StringEnumeration *icu::TimeZone::createEnumerationForRawOffset(
    #       int32_t rawOffset,
    #       UErrorCode &status
    # )
    it4 = TimeZone.create_enumeration_for_raw_offset(-8 * HOUR)
    assert isinstance(it4, StringEnumeration)
    assert len(it4) > 0

    # static StringEnumeration *icu::TimeZone::createEnumerationForRegion(
    #       const char *region,
    #       UErrorCode &status
    # )
    it2 = TimeZone.create_enumeration_for_region(None)
    assert isinstance(it2, StringEnumeration)
    assert len(it2) > 0

    it3 = TimeZone.create_enumeration_for_region("US")
    assert isinstance(it3, StringEnumeration)
    assert len(it3) > 0

    assert len(it1) == len(it2)
    assert len(it1) > len(it3) > len(it4)
    assert "America/Los_Angeles" in it3
    assert "America/Los_Angeles" in it4


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_time_zone_detect_host_time_zone():
    # static TimeZone *icu::TimeZone::detectHostTimeZone()
    zone = TimeZone.detect_host_time_zone()
    assert isinstance(zone, BasicTimeZone)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_time_zone_get_unknown():
    # static const TimeZone &icu::TimeZone::getUnknown()
    zone = TimeZone.get_unknown()
    assert isinstance(zone, BasicTimeZone)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_time_zone_get_windows_id():
    # static UnicodeString &icu::TimeZone::getWindowsID(
    #       const UnicodeString &id,
    #       UnicodeString &winid,
    #       UErrorCode &status
    # )
    id_ = UnicodeString("America/New_York")
    winid = UnicodeString()
    result = TimeZone.get_windows_id(id_, winid)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(winid)
    assert winid == "Eastern Standard Time"
    assert (TimeZone.get_windows_id("America/New_York", winid)
            == "Eastern Standard Time")

    # static UnicodeString &icu::TimeZone::getIDForWindowsID(
    #       const UnicodeString &winid,
    #       const char *region,
    #       UnicodeString &id,
    #       UErrorCode &status
    # )
    id_ = UnicodeString()
    result = TimeZone.get_id_for_windows_id(winid, None, id_)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(id_)
    assert id_ == "America/New_York"
    assert (TimeZone.get_id_for_windows_id("Eastern Standard Time", None, id_)
            == "America/New_York")

    id_ = UnicodeString()
    result = TimeZone.get_id_for_windows_id(winid, "CA", id_)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(id_)
    assert id_ == "America/Toronto"
    assert (TimeZone.get_id_for_windows_id("Eastern Standard Time", "CA", id_)
            == "America/Toronto")


def test_time_zone_has_same_rules():
    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    id_ = "Europe/Moscow"
    otz = TimeZone.create_time_zone(id_)
    vtz1 = VTimeZone.create_vtime_zone_by_id(id_)
    assert not otz.has_same_rules(vtz1)
    assert vtz1.has_same_rules(otz)

    stz1 = SimpleTimeZone(28800000, "Asia/Singapore")
    stz1.set_start_year(1970)
    stz1.set_start_rule(0, 1, 0)
    stz1.set_end_rule(1, 1, 0)
    vtz2 = VTimeZone.create_vtime_zone_from_basic_time_zone(stz1)
    stz2 = stz1.clone()
    assert vtz2.has_same_rules(stz2)


def test_vtime_zone():
    assert issubclass(VTimeZone, BasicTimeZone)
    assert issubclass(VTimeZone, TimeZone)

    # static VTimeZone *icu::VTimeZone::createVTimeZoneByID(
    #       const UnicodeString &ID
    # )
    id1 = UnicodeString("America/New_York")
    zone1 = VTimeZone.create_vtime_zone_by_id(id1)
    assert isinstance(zone1, VTimeZone)
    zone1.set_tzurl(UnicodeString("http://source.icu-project.org/timezone"))
    zone1.set_last_modified(1215298800000.0)  # 2008-07-05T23:00:00Z
    vtzdata = UnicodeString()
    zone1.write(vtzdata)

    # static VTimeZone *icu::VTimeZone::createVTimeZone(
    #       const UnicodeString &vtzdata,
    #       UErrorCode &status
    # )
    zone2 = VTimeZone.create_vtime_zone(vtzdata)
    assert isinstance(zone2, VTimeZone)

    # static VTimeZone *icu::VTimeZone::createVTimeZoneFromBasicTimeZone(
    #       const BasicTimeZone &basicTZ,
    #       UErrorCode &status
    # )
    basic_tz = SimpleTimeZone(
        0,
        "s3",
        UCalendarMonths.UCAL_JANUARY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        UCalendarMonths.UCAL_JULY,
        1,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0)
    zone3 = VTimeZone.create_vtime_zone_from_basic_time_zone(basic_tz)
    assert isinstance(zone3, VTimeZone)

    # icu::VTimeZone::VTimeZone(const VTimeZone &source)
    zone4 = VTimeZone(zone3)

    # VTimeZone *icu::VTimeZone::clone()
    zone1a = zone1.clone()
    assert isinstance(zone1a, VTimeZone)
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
    id_ = UnicodeString()
    result = zone1.get_id(id_)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(id_)
    assert len(result) == 0

    # UBool icu::VTimeZone::getLastModified(UDate &lastModified)
    assert zone1.get_last_modified() == (True, 1215298800000.0)
    assert zone2.get_last_modified() == (True, 1215298800000.0)
    result, _ = zone3.get_last_modified()
    assert not result

    # UBool icu::BasicTimeZone::getNextTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    base = 1230681600000.0  # 2008-12-31T00:00:00Z
    tzt = TimeZoneTransition()
    assert zone3.get_next_transition(base, False, tzt)
    assert tzt.get_time() == 1231027200000.0  # 2009-01-04T00:00:00Z

    # UBool icu::BasicTimeZone::getPreviousTransition(
    #       UDate base,
    #       UBool inclusive,
    #       TimeZoneTransition &result
    # )
    assert zone3.get_previous_transition(base, False, tzt)
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
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == -5 * HOUR
    assert initial.get_dst_savings() == 1 * HOUR
    assert isinstance(std, AnnualTimeZoneRule)
    assert std.get_raw_offset() == -5 * HOUR
    assert std.get_dst_savings() == 0
    dr = std.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_OCTOBER
    assert dr.get_rule_day_of_month() == 0
    assert isinstance(dst, AnnualTimeZoneRule)
    assert dst.get_raw_offset() == -5 * HOUR
    assert dst.get_dst_savings() == 1 * HOUR
    dr = dst.get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_APRIL
    assert dr.get_rule_day_of_month() == 0

    # void icu::BasicTimeZone::getTimeZoneRules(
    #       const InitialTimeZoneRule *&initial,
    #       const TimeZoneRule *trsrules[],
    #       int32_t &trscount,
    #       UErrorCode &status
    # )
    initial, trsrules = zone3.get_time_zone_rules()
    assert isinstance(initial, InitialTimeZoneRule)
    assert initial.get_raw_offset() == 0
    assert initial.get_dst_savings() == 0
    assert isinstance(trsrules, list)
    assert len(trsrules) == 2
    assert all(isinstance(x, AnnualTimeZoneRule) for x in trsrules)
    assert trsrules[0].get_raw_offset() == 0
    assert trsrules[0].get_dst_savings() == 0
    dr = trsrules[0].get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JULY
    assert dr.get_rule_day_of_month() == 0
    assert trsrules[1].get_raw_offset() == 0
    assert trsrules[1].get_dst_savings() == 1 * HOUR
    dr = trsrules[1].get_rule()
    assert dr.get_rule_month() == UCalendarMonths.UCAL_JANUARY
    assert dr.get_rule_day_of_month() == 0

    # UBool icu::VTimeZone::getTZURL(UnicodeString &url)
    url = UnicodeString()
    assert zone1.get_tzurl(url)
    assert url == "http://source.icu-project.org/timezone"
    assert not zone3.get_tzurl(url)

    # UBool icu::BasicTimeZone::hasEquivalentTransitions(
    #       const BasicTimeZone &tz,
    #       UDate start,
    #       UDate end,
    #       UBool ignoreDstAmount,
    #       UErrorCode &ec
    # )
    start = 1215298800000.0  # 2008-07-05T23:00:00Z
    end = 1231027200000.0  # 2009-01-04T00:00:00Z
    assert not zone1.has_equivalent_transitions(zone3, start, end, False)
    assert not zone3.has_equivalent_transitions(zone1, start, end, False)

    # UBool icu::TimeZone::hasSameRules(const TimeZone &other)
    assert not zone1.has_same_rules(zone2)
    assert not zone2.has_same_rules(zone3)

    # void icu::TimeZone::setID(const UnicodeString &ID)
    zone1.set_id(UnicodeString("abc"))
    assert zone1.get_id(id_) == "abc"
    zone1.set_id("ABC")
    assert zone1.get_id(id_) == "ABC"

    # void icu::TimeZone::setRawOffset(int32_t offsetMillis)
    zone1.set_raw_offset(5 * HOUR)  # Nothing to do
    assert zone1.get_raw_offset() == -5 * HOUR

    # void icu::VTimeZone::setTZURL(const UnicodeString &url)
    zone1.set_tzurl("http://www.example.com")
    assert zone1.get_tzurl(url)
    assert url == "http://www.example.com"

    # UBool icu::TimeZone::useDaylightTime(void)
    assert zone1.use_daylight_time()
    assert zone2.use_daylight_time()
    assert zone3.use_daylight_time()

    # [1]
    # void icu::VTimeZone::write(
    #       UDate start,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result1 = UnicodeString()
    zone1.write(1231027200000.0, result1)
    assert len(result1) > 0

    # [2]
    # void icu::VTimeZone::write(
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result2 = UnicodeString()
    zone1.write(result2)
    assert len(result2) > 0

    # void icu::VTimeZone::writeSimple(
    #       UDate time,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result3 = UnicodeString()
    zone1.write_simple(1231027200000.0, result3)
    assert len(result3) > 0


def test_vtime_zone_get_offset():
    basic_tz = SimpleTimeZone(
        8 * HOUR,
        "s5",
        UCalendarMonths.UCAL_APRIL,
        1,
        -1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        UCalendarMonths.UCAL_OCTOBER,
        -1,
        1,
        2 * HOUR,
        SimpleTimeZone.WALL_TIME,
        int(0.5 * HOUR))
    zone = VTimeZone.create_vtime_zone_from_basic_time_zone(basic_tz)

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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0,
        28) == 8.5 * HOUR

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
    assert zone.get_offset(
        GregorianCalendar.AD,
        2008,
        UCalendarMonths.UCAL_JULY,
        6,
        UCalendarDaysOfWeek.UCAL_SUNDAY,
        0) == 8.5 * HOUR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_vtime_zone_get_offset_from_local():
    from icupy.icu import UTimeZoneLocalOption

    # From icu/source/test/intltest/tzoffloc.cpp:
    # TimeZoneOffsetLocalTest::TestGetOffsetAroundTransition()
    zone = VTimeZone.create_vtime_zone_by_id("America/Los_Angeles")

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
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_FORMER,
        UTimeZoneLocalOption.UCAL_TZ_LOCAL_STANDARD_LATTER)
    assert raw_offset == -8 * HOUR
    assert dst_offset == 0

import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 62:
    pytest.skip("ICU4C<62", allow_module_level=True)

import copy


def test_api() -> None:
    assert issubclass(icu.TimeUnit, icu.MeasureUnit)

    # static TimeUnit *icu::TimeUnit::createInstance(
    #       UTimeUnitFields timeUnitField,
    #       UErrorCode &status
    # )
    unit1 = icu.TimeUnit.create_instance(icu.TimeUnit.UTIMEUNIT_MONTH)
    assert isinstance(unit1, icu.TimeUnit)

    unit2 = icu.TimeUnit.create_instance(icu.TimeUnit.UTIMEUNIT_WEEK)
    assert isinstance(unit2, icu.TimeUnit)

    # icu::TimeUnit::TimeUnit(const TimeUnit &other)
    unit3 = icu.TimeUnit(unit1)

    # UBool icu::MeasureUnit::operator!=(const UObject &other)
    assert unit1 != unit2
    assert not (unit1 != unit3)
    assert unit2 != unit3

    # UBool icu::MeasureUnit::operator==(const UObject &other)
    assert not (unit1 == unit2)
    assert unit1 == unit3
    assert not (unit2 == unit3)

    # FIXME: MeasureUnit.__eq__(self, other: UObject) is not work.
    # unit4 = MeasureUnit.create_month()
    # assert unit1 == unit4

    # UTimeUnitFields icu::TimeUnit::getTimeUnitField()
    assert unit3.get_time_unit_field() == icu.TimeUnit.UTIMEUNIT_MONTH
    assert unit2.get_time_unit_field() == icu.TimeUnit.UTIMEUNIT_WEEK

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unit(
    #       const MeasureUnit &unit
    # )
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())
    assert fmt.unit(unit1).format_int(1).to_string() == "1 mth"
    assert fmt.unit(unit2).format_int(1).to_string() == "1 wk"


def test_clone() -> None:
    unit1 = icu.TimeUnit.create_instance(icu.TimeUnit.UTIMEUNIT_MONTH)

    # TimeUnit *icu::TimeUnit::clone()
    unit2 = unit1.clone()
    assert isinstance(unit2, icu.TimeUnit)
    assert unit2 == unit1

    unit3 = copy.copy(unit1)
    assert unit3 == unit1

    unit4 = copy.deepcopy(unit1)
    assert unit4 == unit1

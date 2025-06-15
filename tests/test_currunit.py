import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 62:
    pytest.skip("ICU4C<62", allow_module_level=True)

import copy


def test_api() -> None:
    assert issubclass(icu.CurrencyUnit, icu.MeasureUnit)

    # [2]
    # icu::CurrencyUnit::CurrencyUnit(
    #       ConstChar16Ptr isoCode,
    #       UErrorCode &ec
    # )
    unit1 = icu.CurrencyUnit("USD")
    unit2 = icu.CurrencyUnit("JPY")

    # [4]
    # icu::CurrencyUnit::CurrencyUnit(const CurrencyUnit &other)
    unit3 = icu.CurrencyUnit(unit1)

    # UBool icu::MeasureUnit::operator!=(const UObject &other)
    assert unit1 != unit2
    assert not (unit1 != unit3)
    assert unit2 != unit3

    # UBool icu::MeasureUnit::operator==(const UObject &other)
    assert not (unit1 == unit2)
    assert unit1 == unit3
    assert not (unit2 == unit3)

    # const char16_t *icu::CurrencyUnit::getISOCurrency()
    iso_code = unit3.get_iso_currency()
    assert isinstance(iso_code, str)
    assert iso_code == "USD"

    iso_code = unit2.get_iso_currency()
    assert isinstance(iso_code, str)
    assert iso_code == "JPY"

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unit(
    #       const MeasureUnit &unit
    # )
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())
    assert fmt.unit(unit1).format_double(100).to_string() == "$100.00"
    assert fmt.unit(unit2).format_double(100).to_string() == "\xa5100"  # ¥100


def test_clone() -> None:
    unit1 = icu.CurrencyUnit("USD")

    # CurrencyUnit *icu::CurrencyUnit::clone()
    unit2 = unit1.clone()
    assert isinstance(unit2, icu.CurrencyUnit)
    assert unit2 == unit1

    unit3 = copy.copy(unit1)
    assert unit3 == unit1

    unit4 = copy.deepcopy(unit1)
    assert unit4 == unit1

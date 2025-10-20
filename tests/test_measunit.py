import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 62:
    pytest.skip("ICU4C<62", allow_module_level=True)

import copy


def test_api() -> None:
    # [1]
    # static int32_t icu::MeasureUnit::getAvailable(
    #       const char *type,
    #       MeasureUnit *destArray,
    #       int32_t destCapacity,
    #       UErrorCode &errorCode
    # )
    dest_array = icu.MeasureUnit.get_available("currency")
    assert isinstance(dest_array, list)
    assert len(dest_array) > 0
    assert all(isinstance(x, icu.MeasureUnit) for x in dest_array)
    assert all(x.get_type() == "currency" for x in dest_array)

    # [2]
    # static int32_t icu::MeasureUnit::getAvailable(
    #       MeasureUnit *destArray,
    #       int32_t destCapacity,
    #       UErrorCode &errorCode
    # )
    dest_array = icu.MeasureUnit.get_available()
    assert isinstance(dest_array, list)
    assert len(dest_array) > 0
    assert all(isinstance(x, icu.MeasureUnit) for x in dest_array)

    # static StringEnumeration *icu::MeasureUnit::getAvailableTypes(
    #       UErrorCode &errorCode
    # )
    types = icu.MeasureUnit.get_available_types()
    assert isinstance(types, icu.StringEnumeration)
    assert len(types) > 0
    assert "currency" in types

    # const char *icu::MeasureUnit::getSubtype()
    unit = icu.MeasureUnit.create_square_kilometer()
    result = unit.get_subtype()
    assert isinstance(result, str)
    assert result == "square-kilometer"

    # const char *icu::MeasureUnit::getType()
    result = unit.get_type()
    assert isinstance(result, str)
    assert result == "area"

    # UBool icu::MeasureUnit::operator!=(const UObject &other)
    unit1 = icu.MeasureUnit.get_kilometer()
    unit2 = unit1.clone()
    unit3 = icu.MeasureUnit.get_kilometer_per_hour()
    assert not (unit1 != unit2)
    assert unit1 != unit3
    assert unit2 != unit3

    # UBool icu::MeasureUnit::operator==(const UObject &other)
    assert unit1 == unit2
    assert not (unit1 == unit3)
    assert not (unit2 == unit3)


def test_clone() -> None:
    unit1 = icu.MeasureUnit.get_kilometer()

    # MeasureUnit *icu::MeasureUnit::clone()
    unit2 = unit1.clone()
    assert isinstance(unit2, icu.MeasureUnit)
    assert unit2 == unit1

    unit3 = copy.copy(unit1)
    assert unit3 == unit1

    unit4 = copy.deepcopy(unit1)
    assert unit4 == unit1


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_for_identifier() -> None:
    # static MeasureUnit icu::MeasureUnit::forIdentifier(
    #       StringPiece identifier,
    #       UErrorCode &status
    # )
    unit = icu.MeasureUnit.for_identifier("furlong-per-nanosecond")
    assert isinstance(unit, icu.MeasureUnit)

    # const char *icu::MeasureUnit::getIdentifier()
    result = unit.get_identifier()
    assert isinstance(result, str)
    assert result == "furlong-per-nanosecond"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_get_complexity() -> None:
    # UMeasureUnitComplexity icu::MeasureUnit::getComplexity(
    #       UErrorCode &status
    # )
    unit = icu.MeasureUnit.get_meter()
    assert unit.get_complexity() == icu.UMeasureUnitComplexity.UMEASURE_UNIT_SINGLE

    unit = icu.MeasureUnit.for_identifier("meter-per-second")
    assert unit.get_complexity() == icu.UMeasureUnitComplexity.UMEASURE_UNIT_COMPOUND

    unit = icu.MeasureUnit.for_identifier("hour-and-minute")
    assert unit.get_complexity() == icu.UMeasureUnitComplexity.UMEASURE_UNIT_MIXED


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_get_dimensionality() -> None:
    # int32_t icu::MeasureUnit::getDimensionality(UErrorCode &status)
    unit = icu.MeasureUnit.create_cubic_meter()
    assert unit.get_dimensionality() == 3

    unit = icu.MeasureUnit.create_meter()
    assert unit.get_dimensionality() == 1


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_get_prefix() -> None:
    # UMeasurePrefix icu::MeasureUnit::getPrefix(UErrorCode &status)
    unit = icu.MeasureUnit.get_kilometer()
    assert unit.get_prefix() == icu.UMeasurePrefix.UMEASURE_PREFIX_KILO


def test_measure_unit_53() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_acre()).format_int(1).to_string()) == "1 ac"

    assert (
        fmt.unit(icu.MeasureUnit.create_arc_minute()).format_int(1).to_string()
    ) == "1 arcmin"

    assert (
        fmt.unit(icu.MeasureUnit.create_arc_second()).format_int(1).to_string()
    ) == "1 arcsec"

    assert (
        fmt.unit(icu.MeasureUnit.create_celsius()).format_int(1).to_string()
    ) == "1\xb0C"  # 1°C

    assert (fmt.unit(icu.MeasureUnit.create_centimeter()).format_int(1).to_string()) == "1 cm"

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_kilometer()).format_int(1).to_string()
    ) == "1 km\xb3"  # 1 km³

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_mile()).format_int(1).to_string()
    ) == "1 mi\xb3"  # 1 mi³

    assert (fmt.unit(icu.MeasureUnit.create_day()).format_int(1).to_string()) == "1 day"

    assert (fmt.unit(icu.MeasureUnit.create_degree()).format_int(1).to_string()) == "1 deg"

    assert (
        fmt.unit(icu.MeasureUnit.create_fahrenheit()).format_int(1).to_string()
    ) == "1\xb0F"  # 1°F

    assert (fmt.unit(icu.MeasureUnit.create_foot()).format_int(1).to_string()) == "1 ft"

    assert (fmt.unit(icu.MeasureUnit.create_gforce()).format_int(1).to_string()) == "1 G"

    assert (fmt.unit(icu.MeasureUnit.create_gram()).format_int(1).to_string()) == "1 g"

    assert (fmt.unit(icu.MeasureUnit.create_hectare()).format_int(1).to_string()) == "1 ha"

    assert (fmt.unit(icu.MeasureUnit.create_hectopascal()).format_int(1).to_string()) == "1 hPa"

    assert (fmt.unit(icu.MeasureUnit.create_horsepower()).format_int(1).to_string()) == "1 hp"

    assert (fmt.unit(icu.MeasureUnit.create_hour()).format_int(1).to_string()) == "1 hr"

    assert (fmt.unit(icu.MeasureUnit.create_inch()).format_int(1).to_string()) == "1 in"

    assert (fmt.unit(icu.MeasureUnit.create_inch_hg()).format_int(1).to_string()) == "1 inHg"

    assert (fmt.unit(icu.MeasureUnit.create_kilogram()).format_int(1).to_string()) == "1 kg"

    assert (fmt.unit(icu.MeasureUnit.create_kilometer()).format_int(1).to_string()) == "1 km"

    assert (
        fmt.unit(icu.MeasureUnit.create_kilometer_per_hour()).format_int(1).to_string()
    ) == "1 km/h"

    assert (fmt.unit(icu.MeasureUnit.create_kilowatt()).format_int(1).to_string()) == "1 kW"

    assert (fmt.unit(icu.MeasureUnit.create_light_year()).format_int(1).to_string()) == "1 ly"

    assert (fmt.unit(icu.MeasureUnit.create_liter()).format_int(1).to_string()) == "1 L"

    assert (fmt.unit(icu.MeasureUnit.create_meter()).format_int(1).to_string()) == "1 m"

    assert (
        fmt.unit(icu.MeasureUnit.create_meter_per_second()).format_int(1).to_string()
    ) == "1 m/s"

    assert (fmt.unit(icu.MeasureUnit.create_mile()).format_int(1).to_string()) == "1 mi"

    assert (
        fmt.unit(icu.MeasureUnit.create_mile_per_hour()).format_int(1).to_string()
    ) == "1 mph"

    assert (fmt.unit(icu.MeasureUnit.create_millibar()).format_int(1).to_string()) == "1 mbar"

    assert (fmt.unit(icu.MeasureUnit.create_millimeter()).format_int(1).to_string()) == "1 mm"

    assert (fmt.unit(icu.MeasureUnit.create_millisecond()).format_int(1).to_string()) == "1 ms"

    assert (fmt.unit(icu.MeasureUnit.create_minute()).format_int(1).to_string()) == "1 min"

    assert (fmt.unit(icu.MeasureUnit.create_month()).format_int(1).to_string()) == "1 mth"

    assert (fmt.unit(icu.MeasureUnit.create_ounce()).format_int(1).to_string()) == "1 oz"

    assert (fmt.unit(icu.MeasureUnit.create_picometer()).format_int(1).to_string()) == "1 pm"

    assert (fmt.unit(icu.MeasureUnit.create_pound()).format_int(1).to_string()) == "1 lb"

    assert (fmt.unit(icu.MeasureUnit.create_second()).format_int(1).to_string()) == "1 sec"

    assert (
        fmt.unit(icu.MeasureUnit.create_square_foot()).format_int(1).to_string()
    ) == "1 sq ft"

    assert (
        fmt.unit(icu.MeasureUnit.create_square_kilometer()).format_int(1).to_string()
    ) == "1 km\xb2"  # 1 km²

    assert (
        fmt.unit(icu.MeasureUnit.create_square_meter()).format_int(1).to_string()
    ) == "1 m\xb2"  # 1 m²

    assert (
        fmt.unit(icu.MeasureUnit.create_square_mile()).format_int(1).to_string()
    ) == "1 sq mi"

    assert (fmt.unit(icu.MeasureUnit.create_watt()).format_int(1).to_string()) == "1 W"

    assert (fmt.unit(icu.MeasureUnit.create_week()).format_int(1).to_string()) == "1 wk"

    assert (fmt.unit(icu.MeasureUnit.create_yard()).format_int(1).to_string()) == "1 yd"

    assert (fmt.unit(icu.MeasureUnit.create_year()).format_int(1).to_string()) == "1 yr"


def test_measure_unit_54() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_acre_foot()).format_int(1).to_string()) == "1 ac ft"

    assert (fmt.unit(icu.MeasureUnit.create_ampere()).format_int(1).to_string()) == "1 A"

    assert (
        fmt.unit(icu.MeasureUnit.create_astronomical_unit()).format_int(1).to_string()
    ) == "1 au"

    assert (fmt.unit(icu.MeasureUnit.create_bit()).format_int(1).to_string()) == "1 bit"

    assert (fmt.unit(icu.MeasureUnit.create_bushel()).format_int(1).to_string()) == "1 bu"

    assert (fmt.unit(icu.MeasureUnit.create_byte()).format_int(1).to_string()) == "1 byte"

    assert (fmt.unit(icu.MeasureUnit.create_calorie()).format_int(1).to_string()) == "1 cal"

    assert (fmt.unit(icu.MeasureUnit.create_carat()).format_int(1).to_string()) == "1 CD"

    assert (fmt.unit(icu.MeasureUnit.create_centiliter()).format_int(1).to_string()) == "1 cL"

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_centimeter()).format_int(1).to_string()
    ) == "1 cm\xb3"  # 1 cm³

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_foot()).format_int(1).to_string()
    ) == "1 ft\xb3"  # 1 ft³

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_inch()).format_int(1).to_string()
    ) == "1 in\xb3"  # 1 in³

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_meter()).format_int(1).to_string()
    ) == "1 m\xb3"  # 1 m³

    assert (
        fmt.unit(icu.MeasureUnit.create_cubic_yard()).format_int(1).to_string()
    ) == "1 yd\xb3"  # 1 yd³

    assert (fmt.unit(icu.MeasureUnit.create_cup()).format_int(1).to_string()) == "1 c"

    assert (fmt.unit(icu.MeasureUnit.create_deciliter()).format_int(1).to_string()) == "1 dL"

    assert (fmt.unit(icu.MeasureUnit.create_decimeter()).format_int(1).to_string()) == "1 dm"

    assert (fmt.unit(icu.MeasureUnit.create_fathom()).format_int(1).to_string()) == "1 fth"

    assert (
        fmt.unit(icu.MeasureUnit.create_fluid_ounce()).format_int(1).to_string()
    ) == "1 fl oz"

    assert (fmt.unit(icu.MeasureUnit.create_foodcalorie()).format_int(1).to_string()) == "1 Cal"

    assert (fmt.unit(icu.MeasureUnit.create_furlong()).format_int(1).to_string()) == "1 fur"

    assert (fmt.unit(icu.MeasureUnit.create_gallon()).format_int(1).to_string()) == "1 gal"

    assert (fmt.unit(icu.MeasureUnit.create_gigabit()).format_int(1).to_string()) == "1 Gb"

    assert (fmt.unit(icu.MeasureUnit.create_gigabyte()).format_int(1).to_string()) == "1 GB"

    assert (fmt.unit(icu.MeasureUnit.create_gigahertz()).format_int(1).to_string()) == "1 GHz"

    assert (fmt.unit(icu.MeasureUnit.create_gigawatt()).format_int(1).to_string()) == "1 GW"

    assert (fmt.unit(icu.MeasureUnit.create_hectoliter()).format_int(1).to_string()) == "1 hL"

    assert (fmt.unit(icu.MeasureUnit.create_hertz()).format_int(1).to_string()) == "1 Hz"

    assert (fmt.unit(icu.MeasureUnit.create_joule()).format_int(1).to_string()) == "1 J"

    assert (fmt.unit(icu.MeasureUnit.create_karat()).format_int(1).to_string()) == "1 kt"

    assert (fmt.unit(icu.MeasureUnit.create_kelvin()).format_int(1).to_string()) == "1 K"

    assert (fmt.unit(icu.MeasureUnit.create_kilobit()).format_int(1).to_string()) == "1 kb"

    assert (fmt.unit(icu.MeasureUnit.create_kilobyte()).format_int(1).to_string()) == "1 kB"

    assert (
        fmt.unit(icu.MeasureUnit.create_kilocalorie()).format_int(1).to_string()
    ) == "1 kcal"

    assert (fmt.unit(icu.MeasureUnit.create_kilohertz()).format_int(1).to_string()) == "1 kHz"

    assert (fmt.unit(icu.MeasureUnit.create_kilojoule()).format_int(1).to_string()) == "1 kJ"

    assert (
        fmt.unit(icu.MeasureUnit.create_kilowatt_hour()).format_int(1).to_string()
    ) == "1 kWh"

    assert (
        fmt.unit(icu.MeasureUnit.create_liter_per_kilometer()).format_int(1).to_string()
    ) == "1 L/km"

    assert (fmt.unit(icu.MeasureUnit.create_lux()).format_int(1).to_string()) == "1 lx"

    assert (fmt.unit(icu.MeasureUnit.create_megabit()).format_int(1).to_string()) == "1 Mb"

    assert (fmt.unit(icu.MeasureUnit.create_megabyte()).format_int(1).to_string()) == "1 MB"

    assert (fmt.unit(icu.MeasureUnit.create_megahertz()).format_int(1).to_string()) == "1 MHz"

    assert (fmt.unit(icu.MeasureUnit.create_megaliter()).format_int(1).to_string()) == "1 ML"

    assert (fmt.unit(icu.MeasureUnit.create_megawatt()).format_int(1).to_string()) == "1 MW"

    assert (
        fmt.unit(icu.MeasureUnit.create_meter_per_second_squared()).format_int(1).to_string()
    ) == "1 m/s\xb2"  # 1 m/s²

    assert (fmt.unit(icu.MeasureUnit.create_metric_ton()).format_int(1).to_string()) == "1 t"

    assert (fmt.unit(icu.MeasureUnit.create_microgram()).format_int(1).to_string()) in (
        "1 \xb5g",
        "1 \u03bcg",
    )  # 1 µg

    assert (fmt.unit(icu.MeasureUnit.create_micrometer()).format_int(1).to_string()) in (
        "1 \xb5m",
        "1 \u03bcm",
    )  # 1 µm

    assert (fmt.unit(icu.MeasureUnit.create_microsecond()).format_int(1).to_string()) in (
        "1 \xb5s",
        "1 \u03bcs",
    )  # 1 µs

    assert (
        fmt.unit(icu.MeasureUnit.create_mile_per_gallon()).format_int(1).to_string()
    ) == "1 mpg"

    assert (fmt.unit(icu.MeasureUnit.create_milliampere()).format_int(1).to_string()) == "1 mA"

    assert (fmt.unit(icu.MeasureUnit.create_milligram()).format_int(1).to_string()) == "1 mg"

    assert (fmt.unit(icu.MeasureUnit.create_milliliter()).format_int(1).to_string()) == "1 mL"

    assert (
        fmt.unit(icu.MeasureUnit.create_millimeter_of_mercury()).format_int(1).to_string()
    ) == "1 mmHg"

    assert (fmt.unit(icu.MeasureUnit.create_milliwatt()).format_int(1).to_string()) == "1 mW"

    assert (fmt.unit(icu.MeasureUnit.create_nanometer()).format_int(1).to_string()) == "1 nm"

    assert (fmt.unit(icu.MeasureUnit.create_nanosecond()).format_int(1).to_string()) == "1 ns"

    assert (
        fmt.unit(icu.MeasureUnit.create_nautical_mile()).format_int(1).to_string()
    ) == "1 nmi"

    assert (
        fmt.unit(icu.MeasureUnit.create_ohm()).format_int(1).to_string()
    ) == "1 \u03a9"  # 1 Ω

    assert (fmt.unit(icu.MeasureUnit.create_ounce_troy()).format_int(1).to_string()) == "1 oz t"

    assert (fmt.unit(icu.MeasureUnit.create_parsec()).format_int(1).to_string()) == "1 pc"

    assert (fmt.unit(icu.MeasureUnit.create_pint()).format_int(1).to_string()) == "1 pt"

    assert (
        fmt.unit(icu.MeasureUnit.create_pound_per_square_inch()).format_int(1).to_string()
    ) == "1 psi"

    assert (fmt.unit(icu.MeasureUnit.create_quart()).format_int(1).to_string()) == "1 qt"

    assert (fmt.unit(icu.MeasureUnit.create_radian()).format_int(1).to_string()) == "1 rad"

    assert (
        fmt.unit(icu.MeasureUnit.create_square_centimeter()).format_int(1).to_string()
    ) == "1 cm\xb2"  # 1 cm²

    assert (
        fmt.unit(icu.MeasureUnit.create_square_inch()).format_int(1).to_string()
    ) == "1 in\xb2"  # 1 in²

    assert (
        fmt.unit(icu.MeasureUnit.create_square_yard()).format_int(1).to_string()
    ) == "1 yd\xb2"  # 1 yd²

    assert (fmt.unit(icu.MeasureUnit.create_stone()).format_int(1).to_string()) == "1 st"

    assert (fmt.unit(icu.MeasureUnit.create_tablespoon()).format_int(1).to_string()) == "1 tbsp"

    assert (fmt.unit(icu.MeasureUnit.create_teaspoon()).format_int(1).to_string()) == "1 tsp"

    assert (fmt.unit(icu.MeasureUnit.create_terabit()).format_int(1).to_string()) == "1 Tb"

    assert (fmt.unit(icu.MeasureUnit.create_terabyte()).format_int(1).to_string()) == "1 TB"

    assert (fmt.unit(icu.MeasureUnit.create_ton()).format_int(1).to_string()) == "1 tn"

    assert (fmt.unit(icu.MeasureUnit.create_volt()).format_int(1).to_string()) == "1 V"


def test_measure_unit_56() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_century()).format_int(1).to_string()) == "1 c"

    assert (fmt.unit(icu.MeasureUnit.create_cup_metric()).format_int(1).to_string()) == "1 mc"

    assert (
        fmt.unit(icu.MeasureUnit.create_generic_temperature()).format_int(1).to_string()
    ) == "1\xb0"  # 1°

    assert (fmt.unit(icu.MeasureUnit.create_knot()).format_int(1).to_string()) == "1 kn"

    assert (
        fmt.unit(icu.MeasureUnit.create_liter_per_100_kilometers()).format_int(1).to_string()
    ) == "1 L/100 km"

    assert (
        fmt.unit(icu.MeasureUnit.create_mile_scandinavian()).format_int(1).to_string()
    ) == "1 smi"

    assert (fmt.unit(icu.MeasureUnit.create_pint_metric()).format_int(1).to_string()) == "1 mpt"

    assert (
        fmt.unit(icu.MeasureUnit.create_revolution_angle()).format_int(1).to_string()
    ) == "1 rev"


def test_measure_unit_57() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (
        fmt.unit(icu.MeasureUnit.create_gallon_imperial()).format_int(1).to_string()
    ) == "1 gal Imp."

    assert (
        fmt.unit(icu.MeasureUnit.create_mile_per_gallon_imperial()).format_int(1).to_string()
    ) == "1 mpg Imp."

    if icu.U_ICU_VERSION_MAJOR_NUM > 70:
        assert (
            fmt.unit(icu.MeasureUnit.create_milligram_per_deciliter()).format_int(1).to_string()
        ) == "1 mg/dL"
    else:
        with pytest.raises(icu.ICUError) as exc_info:
            _ = (
                fmt.unit(icu.MeasureUnit.create_milligram_per_deciliter())
                .format_int(1)
                .to_string()
            )
        assert exc_info.value.args[0] == icu.UErrorCode.U_MISSING_RESOURCE_ERROR

    assert (
        fmt.unit(icu.MeasureUnit.create_millimole_per_liter()).format_int(1).to_string()
    ) == "1 mmol/L"

    assert (
        fmt.unit(icu.MeasureUnit.create_part_per_million()).format_int(1).to_string()
    ) == "1 ppm"


def test_measure_unit_59() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_point()).format_int(1).to_string()) == "1 pt"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_measure_unit_63() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_atmosphere()).format_int(1).to_string()) == "1 atm"

    assert (fmt.unit(icu.MeasureUnit.create_percent()).format_int(1).to_string()) == "1%"

    assert (
        fmt.unit(icu.MeasureUnit.create_permille()).format_int(1).to_string()
    ) == "1\u2030"  # 1‰

    assert (fmt.unit(icu.MeasureUnit.create_petabyte()).format_int(1).to_string()) == "1 PB"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_measure_unit_64() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_barrel()).format_int(1).to_string()) == "1 bbl"

    assert (
        fmt.unit(icu.MeasureUnit.create_british_thermal_unit()).format_int(1).to_string()
    ) == "1 Btu"

    assert (fmt.unit(icu.MeasureUnit.create_dalton()).format_int(1).to_string()) == "1 Da"

    assert (fmt.unit(icu.MeasureUnit.create_day_person()).format_int(1).to_string()) == "1 day"

    assert (fmt.unit(icu.MeasureUnit.create_dunam()).format_int(1).to_string()) == "1 dunam"

    assert (
        fmt.unit(icu.MeasureUnit.create_earth_mass()).format_int(1).to_string()
    ) == "1 M\u2295"  # 1 M⊕

    assert (fmt.unit(icu.MeasureUnit.create_electronvolt()).format_int(1).to_string()) == "1 eV"

    assert (
        fmt.unit(icu.MeasureUnit.create_fluid_ounce_imperial()).format_int(1).to_string()
    ) == "1 fl oz Imp."

    assert (fmt.unit(icu.MeasureUnit.create_kilopascal()).format_int(1).to_string()) == "1 kPa"

    assert (fmt.unit(icu.MeasureUnit.create_megapascal()).format_int(1).to_string()) == "1 MPa"

    assert (fmt.unit(icu.MeasureUnit.create_mole()).format_int(1).to_string()) == "1 mol"

    assert (
        fmt.unit(icu.MeasureUnit.create_month_person()).format_int(1).to_string()
    ) == "1 mth"

    assert (fmt.unit(icu.MeasureUnit.create_newton()).format_int(1).to_string()) == "1 N"

    assert (
        fmt.unit(icu.MeasureUnit.create_newton_meter()).format_int(1).to_string()
    ) == "1 N\u22c5m"  # 1 N⋅m

    assert (
        fmt.unit(icu.MeasureUnit.create_permyriad()).format_int(1).to_string()
    ) == "1\u2031"  # 1‱

    assert (
        fmt.unit(icu.MeasureUnit.create_pound_foot()).format_int(1).to_string()
    ) == "1 lbf\u22c5ft"  # 1 lb⋅ft

    assert (fmt.unit(icu.MeasureUnit.create_pound_force()).format_int(1).to_string()) == "1 lbf"

    assert (
        fmt.unit(icu.MeasureUnit.create_solar_luminosity()).format_int(1).to_string()
    ) == "1 L\u2609"  # 1 L☉

    assert (
        fmt.unit(icu.MeasureUnit.create_solar_mass()).format_int(1).to_string()
    ) == "1 M\u2609"  # 1 M☉

    assert (
        fmt.unit(icu.MeasureUnit.create_solar_radius()).format_int(1).to_string()
    ) == "1 R\u2609"  # 1 R☉

    assert (fmt.unit(icu.MeasureUnit.create_week_person()).format_int(1).to_string()) == "1 wk"

    assert (fmt.unit(icu.MeasureUnit.create_year_person()).format_int(1).to_string()) == "1 yr"

    assert (fmt.unit(icu.MeasureUnit.get_acre()).format_int(1).to_string()) == "1 ac"

    assert (fmt.unit(icu.MeasureUnit.get_acre_foot()).format_int(1).to_string()) == "1 ac ft"

    assert (fmt.unit(icu.MeasureUnit.get_ampere()).format_int(1).to_string()) == "1 A"

    assert (fmt.unit(icu.MeasureUnit.get_arc_minute()).format_int(1).to_string()) == "1 arcmin"

    assert (fmt.unit(icu.MeasureUnit.get_arc_second()).format_int(1).to_string()) == "1 arcsec"

    assert (
        fmt.unit(icu.MeasureUnit.get_astronomical_unit()).format_int(1).to_string()
    ) == "1 au"

    assert (fmt.unit(icu.MeasureUnit.get_atmosphere()).format_int(1).to_string()) == "1 atm"

    assert (fmt.unit(icu.MeasureUnit.get_barrel()).format_int(1).to_string()) == "1 bbl"

    assert (fmt.unit(icu.MeasureUnit.get_bit()).format_int(1).to_string()) == "1 bit"

    assert (
        fmt.unit(icu.MeasureUnit.get_british_thermal_unit()).format_int(1).to_string()
    ) == "1 Btu"

    assert (fmt.unit(icu.MeasureUnit.get_bushel()).format_int(1).to_string()) == "1 bu"

    assert (fmt.unit(icu.MeasureUnit.get_byte()).format_int(1).to_string()) == "1 byte"

    assert (fmt.unit(icu.MeasureUnit.get_calorie()).format_int(1).to_string()) == "1 cal"

    assert (fmt.unit(icu.MeasureUnit.get_carat()).format_int(1).to_string()) == "1 CD"

    assert (
        fmt.unit(icu.MeasureUnit.get_celsius()).format_int(1).to_string()
    ) == "1\xb0C"  # 1°C

    assert (fmt.unit(icu.MeasureUnit.get_centiliter()).format_int(1).to_string()) == "1 cL"

    assert (fmt.unit(icu.MeasureUnit.get_centimeter()).format_int(1).to_string()) == "1 cm"

    assert (fmt.unit(icu.MeasureUnit.get_century()).format_int(1).to_string()) == "1 c"

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_centimeter()).format_int(1).to_string()
    ) == "1 cm\xb3"  # 1 cm³

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_foot()).format_int(1).to_string()
    ) == "1 ft\xb3"  # 1 ft³

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_inch()).format_int(1).to_string()
    ) == "1 in\xb3"  # 1 in³

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_kilometer()).format_int(1).to_string()
    ) == "1 km\xb3"  # 1 km³

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_meter()).format_int(1).to_string()
    ) == "1 m\xb3"  # 1 m³

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_mile()).format_int(1).to_string()
    ) == "1 mi\xb3"  # 1 mi³

    assert (
        fmt.unit(icu.MeasureUnit.get_cubic_yard()).format_int(1).to_string()
    ) == "1 yd\xb3"  # 1 yd³

    assert (fmt.unit(icu.MeasureUnit.get_cup()).format_int(1).to_string()) == "1 c"

    assert (fmt.unit(icu.MeasureUnit.get_cup_metric()).format_int(1).to_string()) == "1 mc"

    assert (fmt.unit(icu.MeasureUnit.get_dalton()).format_int(1).to_string()) == "1 Da"

    assert (fmt.unit(icu.MeasureUnit.get_day()).format_int(1).to_string()) == "1 day"

    assert (fmt.unit(icu.MeasureUnit.get_day_person()).format_int(1).to_string()) == "1 day"

    assert (fmt.unit(icu.MeasureUnit.get_deciliter()).format_int(1).to_string()) == "1 dL"

    assert (fmt.unit(icu.MeasureUnit.get_decimeter()).format_int(1).to_string()) == "1 dm"

    assert (fmt.unit(icu.MeasureUnit.get_degree()).format_int(1).to_string()) == "1 deg"

    assert (fmt.unit(icu.MeasureUnit.get_dunam()).format_int(1).to_string()) == "1 dunam"

    assert (
        fmt.unit(icu.MeasureUnit.get_earth_mass()).format_int(1).to_string()
    ) == "1 M\u2295"  # 1 M⊕

    assert (fmt.unit(icu.MeasureUnit.get_electronvolt()).format_int(1).to_string()) == "1 eV"

    assert (
        fmt.unit(icu.MeasureUnit.get_fahrenheit()).format_int(1).to_string()
    ) == "1\xb0F"  # 1°F

    assert (fmt.unit(icu.MeasureUnit.get_fathom()).format_int(1).to_string()) == "1 fth"

    assert (fmt.unit(icu.MeasureUnit.get_fluid_ounce()).format_int(1).to_string()) == "1 fl oz"

    assert (
        fmt.unit(icu.MeasureUnit.get_fluid_ounce_imperial()).format_int(1).to_string()
    ) == "1 fl oz Imp."

    assert (fmt.unit(icu.MeasureUnit.get_foodcalorie()).format_int(1).to_string()) == "1 Cal"

    assert (fmt.unit(icu.MeasureUnit.get_foot()).format_int(1).to_string()) == "1 ft"

    assert (fmt.unit(icu.MeasureUnit.get_furlong()).format_int(1).to_string()) == "1 fur"

    assert (fmt.unit(icu.MeasureUnit.get_gallon()).format_int(1).to_string()) == "1 gal"

    assert (
        fmt.unit(icu.MeasureUnit.get_gallon_imperial()).format_int(1).to_string()
    ) == "1 gal Imp."

    assert (
        fmt.unit(icu.MeasureUnit.get_generic_temperature()).format_int(1).to_string()
    ) == "1\xb0"  # 1°

    assert (fmt.unit(icu.MeasureUnit.get_gforce()).format_int(1).to_string()) == "1 G"

    assert (fmt.unit(icu.MeasureUnit.get_gigabit()).format_int(1).to_string()) == "1 Gb"

    assert (fmt.unit(icu.MeasureUnit.get_gigabyte()).format_int(1).to_string()) == "1 GB"

    assert (fmt.unit(icu.MeasureUnit.get_gigahertz()).format_int(1).to_string()) == "1 GHz"

    assert (fmt.unit(icu.MeasureUnit.get_gigawatt()).format_int(1).to_string()) == "1 GW"

    assert (fmt.unit(icu.MeasureUnit.get_gram()).format_int(1).to_string()) == "1 g"

    assert (fmt.unit(icu.MeasureUnit.get_hectare()).format_int(1).to_string()) == "1 ha"

    assert (fmt.unit(icu.MeasureUnit.get_hectoliter()).format_int(1).to_string()) == "1 hL"

    assert (fmt.unit(icu.MeasureUnit.get_hectopascal()).format_int(1).to_string()) == "1 hPa"

    assert (fmt.unit(icu.MeasureUnit.get_hertz()).format_int(1).to_string()) == "1 Hz"

    assert (fmt.unit(icu.MeasureUnit.get_horsepower()).format_int(1).to_string()) == "1 hp"

    assert (fmt.unit(icu.MeasureUnit.get_hour()).format_int(1).to_string()) == "1 hr"

    assert (fmt.unit(icu.MeasureUnit.get_inch()).format_int(1).to_string()) == "1 in"

    assert (fmt.unit(icu.MeasureUnit.get_inch_hg()).format_int(1).to_string()) == "1 inHg"

    assert (fmt.unit(icu.MeasureUnit.get_joule()).format_int(1).to_string()) == "1 J"

    assert (fmt.unit(icu.MeasureUnit.get_karat()).format_int(1).to_string()) == "1 kt"

    assert (fmt.unit(icu.MeasureUnit.get_kelvin()).format_int(1).to_string()) == "1 K"

    assert (fmt.unit(icu.MeasureUnit.get_kilobit()).format_int(1).to_string()) == "1 kb"

    assert (fmt.unit(icu.MeasureUnit.get_kilobyte()).format_int(1).to_string()) == "1 kB"

    assert (fmt.unit(icu.MeasureUnit.get_kilocalorie()).format_int(1).to_string()) == "1 kcal"

    assert (fmt.unit(icu.MeasureUnit.get_kilogram()).format_int(1).to_string()) == "1 kg"

    assert (fmt.unit(icu.MeasureUnit.get_kilohertz()).format_int(1).to_string()) == "1 kHz"

    assert (fmt.unit(icu.MeasureUnit.get_kilojoule()).format_int(1).to_string()) == "1 kJ"

    assert (fmt.unit(icu.MeasureUnit.get_kilometer()).format_int(1).to_string()) == "1 km"

    assert (
        fmt.unit(icu.MeasureUnit.get_kilometer_per_hour()).format_int(1).to_string()
    ) == "1 km/h"

    assert (fmt.unit(icu.MeasureUnit.get_kilopascal()).format_int(1).to_string()) == "1 kPa"

    assert (fmt.unit(icu.MeasureUnit.get_kilowatt()).format_int(1).to_string()) == "1 kW"

    assert (fmt.unit(icu.MeasureUnit.get_kilowatt_hour()).format_int(1).to_string()) == "1 kWh"

    assert (fmt.unit(icu.MeasureUnit.get_knot()).format_int(1).to_string()) == "1 kn"

    assert (fmt.unit(icu.MeasureUnit.get_light_year()).format_int(1).to_string()) == "1 ly"

    assert (fmt.unit(icu.MeasureUnit.get_liter()).format_int(1).to_string()) == "1 L"

    assert (
        fmt.unit(icu.MeasureUnit.get_liter_per_100_kilometers()).format_int(1).to_string()
    ) == "1 L/100 km"

    assert (
        fmt.unit(icu.MeasureUnit.get_liter_per_kilometer()).format_int(1).to_string()
    ) == "1 L/km"

    assert (fmt.unit(icu.MeasureUnit.get_lux()).format_int(1).to_string()) == "1 lx"

    assert (fmt.unit(icu.MeasureUnit.get_megabit()).format_int(1).to_string()) == "1 Mb"

    assert (fmt.unit(icu.MeasureUnit.get_megabyte()).format_int(1).to_string()) == "1 MB"

    assert (fmt.unit(icu.MeasureUnit.get_megahertz()).format_int(1).to_string()) == "1 MHz"

    assert (fmt.unit(icu.MeasureUnit.get_megaliter()).format_int(1).to_string()) == "1 ML"

    assert (fmt.unit(icu.MeasureUnit.get_megapascal()).format_int(1).to_string()) == "1 MPa"

    assert (fmt.unit(icu.MeasureUnit.get_megawatt()).format_int(1).to_string()) == "1 MW"

    assert (fmt.unit(icu.MeasureUnit.get_meter()).format_int(1).to_string()) == "1 m"

    assert (
        fmt.unit(icu.MeasureUnit.get_meter_per_second()).format_int(1).to_string()
    ) == "1 m/s"

    assert (
        fmt.unit(icu.MeasureUnit.get_meter_per_second_squared()).format_int(1).to_string()
    ) == "1 m/s\xb2"  # 1 m/s²

    assert (fmt.unit(icu.MeasureUnit.get_metric_ton()).format_int(1).to_string()) == "1 t"

    assert (fmt.unit(icu.MeasureUnit.get_microgram()).format_int(1).to_string()) in (
        "1 \xb5g",
        "1 \u03bcg",
    )  # 1 µg

    assert (fmt.unit(icu.MeasureUnit.get_micrometer()).format_int(1).to_string()) in (
        "1 \xb5m",
        "1 \u03bcm",
    )  # 1 µm

    assert (fmt.unit(icu.MeasureUnit.get_microsecond()).format_int(1).to_string()) in (
        "1 \xb5s",
        "1 \u03bcs",
    )  # 1 µs

    assert (fmt.unit(icu.MeasureUnit.get_mile()).format_int(1).to_string()) == "1 mi"

    assert (
        fmt.unit(icu.MeasureUnit.get_mile_per_gallon()).format_int(1).to_string()
    ) == "1 mpg"

    assert (
        fmt.unit(icu.MeasureUnit.get_mile_per_gallon_imperial()).format_int(1).to_string()
    ) == "1 mpg Imp."

    assert (fmt.unit(icu.MeasureUnit.get_mile_per_hour()).format_int(1).to_string()) == "1 mph"

    assert (
        fmt.unit(icu.MeasureUnit.get_mile_scandinavian()).format_int(1).to_string()
    ) == "1 smi"

    assert (fmt.unit(icu.MeasureUnit.get_milliampere()).format_int(1).to_string()) == "1 mA"

    assert (fmt.unit(icu.MeasureUnit.get_millibar()).format_int(1).to_string()) == "1 mbar"

    assert (fmt.unit(icu.MeasureUnit.get_milligram()).format_int(1).to_string()) == "1 mg"

    if icu.U_ICU_VERSION_MAJOR_NUM > 70:
        assert (
            fmt.unit(icu.MeasureUnit.get_milligram_per_deciliter()).format_int(1).to_string()
        ) == "1 mg/dL"
    else:
        with pytest.raises(icu.ICUError) as exc_info:
            _ = (
                fmt.unit(icu.MeasureUnit.get_milligram_per_deciliter())
                .format_int(1)
                .to_string()
            )
        assert exc_info.value.args[0] == icu.UErrorCode.U_MISSING_RESOURCE_ERROR

    assert (fmt.unit(icu.MeasureUnit.get_milliliter()).format_int(1).to_string()) == "1 mL"

    assert (fmt.unit(icu.MeasureUnit.get_millimeter()).format_int(1).to_string()) == "1 mm"

    assert (
        fmt.unit(icu.MeasureUnit.get_millimeter_of_mercury()).format_int(1).to_string()
    ) == "1 mmHg"

    assert (
        fmt.unit(icu.MeasureUnit.get_millimole_per_liter()).format_int(1).to_string()
    ) == "1 mmol/L"

    assert (fmt.unit(icu.MeasureUnit.get_millisecond()).format_int(1).to_string()) == "1 ms"

    assert (fmt.unit(icu.MeasureUnit.get_milliwatt()).format_int(1).to_string()) == "1 mW"

    assert (fmt.unit(icu.MeasureUnit.get_minute()).format_int(1).to_string()) == "1 min"

    assert (fmt.unit(icu.MeasureUnit.get_mole()).format_int(1).to_string()) == "1 mol"

    assert (fmt.unit(icu.MeasureUnit.get_month()).format_int(1).to_string()) == "1 mth"

    assert (fmt.unit(icu.MeasureUnit.get_month_person()).format_int(1).to_string()) == "1 mth"

    assert (fmt.unit(icu.MeasureUnit.get_nanometer()).format_int(1).to_string()) == "1 nm"

    assert (fmt.unit(icu.MeasureUnit.get_nanosecond()).format_int(1).to_string()) == "1 ns"

    assert (fmt.unit(icu.MeasureUnit.get_nautical_mile()).format_int(1).to_string()) == "1 nmi"

    assert (fmt.unit(icu.MeasureUnit.get_newton()).format_int(1).to_string()) == "1 N"

    assert (
        fmt.unit(icu.MeasureUnit.get_newton_meter()).format_int(1).to_string()
    ) == "1 N\u22c5m"  # 1 N⋅m

    assert (fmt.unit(icu.MeasureUnit.get_ohm()).format_int(1).to_string()) == "1 \u03a9"  # 1 Ω

    assert (fmt.unit(icu.MeasureUnit.get_ounce()).format_int(1).to_string()) == "1 oz"

    assert (fmt.unit(icu.MeasureUnit.get_ounce_troy()).format_int(1).to_string()) == "1 oz t"

    assert (fmt.unit(icu.MeasureUnit.get_parsec()).format_int(1).to_string()) == "1 pc"

    assert (
        fmt.unit(icu.MeasureUnit.get_part_per_million()).format_int(1).to_string()
    ) == "1 ppm"

    assert (fmt.unit(icu.MeasureUnit.get_percent()).format_int(1).to_string()) == "1%"

    assert (
        fmt.unit(icu.MeasureUnit.get_permille()).format_int(1).to_string()
    ) == "1\u2030"  # 1‰

    assert (
        fmt.unit(icu.MeasureUnit.get_permyriad()).format_int(1).to_string()
    ) == "1\u2031"  # 1‱

    assert (fmt.unit(icu.MeasureUnit.get_petabyte()).format_int(1).to_string()) == "1 PB"

    assert (fmt.unit(icu.MeasureUnit.get_picometer()).format_int(1).to_string()) == "1 pm"

    assert (fmt.unit(icu.MeasureUnit.get_pint()).format_int(1).to_string()) == "1 pt"

    assert (fmt.unit(icu.MeasureUnit.get_pint_metric()).format_int(1).to_string()) == "1 mpt"

    assert (fmt.unit(icu.MeasureUnit.get_point()).format_int(1).to_string()) == "1 pt"

    assert (fmt.unit(icu.MeasureUnit.get_pound()).format_int(1).to_string()) == "1 lb"

    assert (
        fmt.unit(icu.MeasureUnit.get_pound_foot()).format_int(1).to_string()
    ) == "1 lbf\u22c5ft"  # 1 lb⋅ft

    assert (fmt.unit(icu.MeasureUnit.get_pound_force()).format_int(1).to_string()) == "1 lbf"

    assert (
        fmt.unit(icu.MeasureUnit.get_pound_per_square_inch()).format_int(1).to_string()
    ) == "1 psi"

    assert (fmt.unit(icu.MeasureUnit.get_quart()).format_int(1).to_string()) == "1 qt"

    assert (fmt.unit(icu.MeasureUnit.get_radian()).format_int(1).to_string()) == "1 rad"

    assert (
        fmt.unit(icu.MeasureUnit.get_revolution_angle()).format_int(1).to_string()
    ) == "1 rev"

    assert (fmt.unit(icu.MeasureUnit.get_second()).format_int(1).to_string()) == "1 sec"

    assert (
        fmt.unit(icu.MeasureUnit.get_solar_luminosity()).format_int(1).to_string()
    ) == "1 L\u2609"  # 1 L☉

    assert (
        fmt.unit(icu.MeasureUnit.get_solar_mass()).format_int(1).to_string()
    ) == "1 M\u2609"  # 1 M☉

    assert (
        fmt.unit(icu.MeasureUnit.get_solar_radius()).format_int(1).to_string()
    ) == "1 R\u2609"  # 1 R☉

    assert (
        fmt.unit(icu.MeasureUnit.get_square_centimeter()).format_int(1).to_string()
    ) == "1 cm\xb2"  # 1 cm²

    assert (fmt.unit(icu.MeasureUnit.get_square_foot()).format_int(1).to_string()) == "1 sq ft"

    assert (
        fmt.unit(icu.MeasureUnit.get_square_inch()).format_int(1).to_string()
    ) == "1 in\xb2"  # 1 in²

    assert (
        fmt.unit(icu.MeasureUnit.get_square_kilometer()).format_int(1).to_string()
    ) == "1 km\xb2"  # 1 km²

    assert (
        fmt.unit(icu.MeasureUnit.get_square_meter()).format_int(1).to_string()
    ) == "1 m\xb2"  # 1 m²

    assert (fmt.unit(icu.MeasureUnit.get_square_mile()).format_int(1).to_string()) == "1 sq mi"

    assert (
        fmt.unit(icu.MeasureUnit.get_square_yard()).format_int(1).to_string()
    ) == "1 yd\xb2"  # 1 yd²

    assert (fmt.unit(icu.MeasureUnit.get_stone()).format_int(1).to_string()) == "1 st"

    assert (fmt.unit(icu.MeasureUnit.get_tablespoon()).format_int(1).to_string()) == "1 tbsp"

    assert (fmt.unit(icu.MeasureUnit.get_teaspoon()).format_int(1).to_string()) == "1 tsp"

    assert (fmt.unit(icu.MeasureUnit.get_terabit()).format_int(1).to_string()) == "1 Tb"

    assert (fmt.unit(icu.MeasureUnit.get_terabyte()).format_int(1).to_string()) == "1 TB"

    assert (fmt.unit(icu.MeasureUnit.get_ton()).format_int(1).to_string()) == "1 tn"

    assert (fmt.unit(icu.MeasureUnit.get_volt()).format_int(1).to_string()) == "1 V"

    assert (fmt.unit(icu.MeasureUnit.get_watt()).format_int(1).to_string()) == "1 W"

    assert (fmt.unit(icu.MeasureUnit.get_week()).format_int(1).to_string()) == "1 wk"

    assert (fmt.unit(icu.MeasureUnit.get_week_person()).format_int(1).to_string()) == "1 wk"

    assert (fmt.unit(icu.MeasureUnit.get_yard()).format_int(1).to_string()) == "1 yd"

    assert (fmt.unit(icu.MeasureUnit.get_year()).format_int(1).to_string()) == "1 yr"

    assert (fmt.unit(icu.MeasureUnit.get_year_person()).format_int(1).to_string()) == "1 yr"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 65, reason="ICU4C<65")
def test_measure_unit_65() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_bar()).format_int(1).to_string()) == "1 bar"

    assert (fmt.unit(icu.MeasureUnit.create_decade()).format_int(1).to_string()) == "1 dec"

    assert (
        fmt.unit(icu.MeasureUnit.create_dot_per_centimeter()).format_int(1).to_string()
    ) == "1 dpcm"

    assert (
        fmt.unit(icu.MeasureUnit.create_dot_per_inch()).format_int(1).to_string()
    ) == "1 dpi"

    assert (fmt.unit(icu.MeasureUnit.create_em()).format_int(1).to_string()) == "1 em"

    assert (fmt.unit(icu.MeasureUnit.create_megapixel()).format_int(1).to_string()) == "1 MP"

    assert (fmt.unit(icu.MeasureUnit.create_pascal()).format_int(1).to_string()) == "1 Pa"

    assert (fmt.unit(icu.MeasureUnit.create_pixel()).format_int(1).to_string()) == "1 px"

    assert (
        fmt.unit(icu.MeasureUnit.create_pixel_per_centimeter()).format_int(1).to_string()
    ) == "1 ppcm"

    assert (
        fmt.unit(icu.MeasureUnit.create_pixel_per_inch()).format_int(1).to_string()
    ) == "1 ppi"

    assert (
        fmt.unit(icu.MeasureUnit.create_therm_us()).format_int(1).to_string()
    ) == "1 US therm"

    assert (fmt.unit(icu.MeasureUnit.get_bar()).format_int(1).to_string()) == "1 bar"

    assert (fmt.unit(icu.MeasureUnit.get_decade()).format_int(1).to_string()) == "1 dec"

    assert (
        fmt.unit(icu.MeasureUnit.get_dot_per_centimeter()).format_int(1).to_string()
    ) == "1 dpcm"

    assert (fmt.unit(icu.MeasureUnit.get_dot_per_inch()).format_int(1).to_string()) == "1 dpi"

    assert (fmt.unit(icu.MeasureUnit.get_em()).format_int(1).to_string()) == "1 em"

    assert (fmt.unit(icu.MeasureUnit.get_megapixel()).format_int(1).to_string()) == "1 MP"

    assert (fmt.unit(icu.MeasureUnit.get_pascal()).format_int(1).to_string()) == "1 Pa"

    assert (fmt.unit(icu.MeasureUnit.get_pixel()).format_int(1).to_string()) == "1 px"

    assert (
        fmt.unit(icu.MeasureUnit.get_pixel_per_centimeter()).format_int(1).to_string()
    ) == "1 ppcm"

    assert (fmt.unit(icu.MeasureUnit.get_pixel_per_inch()).format_int(1).to_string()) == "1 ppi"

    assert (fmt.unit(icu.MeasureUnit.get_therm_us()).format_int(1).to_string()) == "1 US therm"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_measure_unit_68() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (fmt.unit(icu.MeasureUnit.create_candela()).format_int(1).to_string()) == "1 cd"

    assert (fmt.unit(icu.MeasureUnit.create_dessert_spoon()).format_int(1).to_string()) in (
        "1 dstspn",  # ICU4C<74
        "1 dsp",  # ICU4C>=74
    )

    assert (
        fmt.unit(icu.MeasureUnit.create_dessert_spoon_imperial()).format_int(1).to_string()
    ) in (
        "1 dstspn Imp",  # ICU4C<74
        "1 dsp-Imp.",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.create_dot()).format_int(1).to_string()) in (
        "1 dot",  # ICU4C<69
        "1 px",  # ICU4C>=69
    )

    assert (fmt.unit(icu.MeasureUnit.create_dram()).format_int(1).to_string()) in (
        "1 dram fl",  # ICU4C<74
        "1 dram",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.create_drop()).format_int(1).to_string()) in (
        "1 drop",  # ICU4C<74
        "1 dr",  # ICU4C>=74
    )

    assert (
        fmt.unit(icu.MeasureUnit.create_earth_radius()).format_int(1).to_string()
    ) == "1 R\u2295"  # 1 R⊕

    assert (fmt.unit(icu.MeasureUnit.create_grain()).format_int(1).to_string()) in (
        "1 grain",  # ICU4C<74
        "1 gr",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.create_jigger()).format_int(1).to_string()) == "1 jigger"

    assert (fmt.unit(icu.MeasureUnit.create_lumen()).format_int(1).to_string()) == "1 lm"

    assert (fmt.unit(icu.MeasureUnit.create_pinch()).format_int(1).to_string()) in (
        "1 pinch",  # ICU4C<74
        "1 pn",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.create_quart_imperial()).format_int(1).to_string()) in (
        "1 qt Imp.",  # ICU4C<74
        "1 qt-Imp.",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.get_candela()).format_int(1).to_string()) == "1 cd"

    assert (fmt.unit(icu.MeasureUnit.get_dessert_spoon()).format_int(1).to_string()) in (
        "1 dstspn",  # ICU4C<74
        "1 dsp",  # ICU4C>=74
    )

    assert (
        fmt.unit(icu.MeasureUnit.get_dessert_spoon_imperial()).format_int(1).to_string()
    ) in (
        "1 dstspn Imp",  # ICU4C<74
        "1 dsp-Imp.",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.get_dot()).format_int(1).to_string()) in (
        "1 dot",  # ICU4C<69
        "1 px",  # ICU4C>=69
    )

    assert (fmt.unit(icu.MeasureUnit.get_dram()).format_int(1).to_string()) in (
        "1 dram fl",  # ICU4C<69
        "1 dram",  # ICU4C>=69
    )

    assert (fmt.unit(icu.MeasureUnit.get_drop()).format_int(1).to_string()) in (
        "1 drop",  # ICU4C<74
        "1 dr",  # ICU4C>=74
    )

    assert (
        fmt.unit(icu.MeasureUnit.get_earth_radius()).format_int(1).to_string()
    ) == "1 R\u2295"  # 1 R⊕

    assert (fmt.unit(icu.MeasureUnit.get_grain()).format_int(1).to_string()) in (
        "1 grain",  # ICU4C<74
        "1 gr",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.get_jigger()).format_int(1).to_string()) == "1 jigger"

    assert (fmt.unit(icu.MeasureUnit.get_lumen()).format_int(1).to_string()) == "1 lm"

    assert (fmt.unit(icu.MeasureUnit.get_pinch()).format_int(1).to_string()) in (
        "1 pinch",  # ICU4C<74
        "1 pn",  # ICU4C>=74
    )

    assert (fmt.unit(icu.MeasureUnit.get_quart_imperial()).format_int(1).to_string()) in (
        "1 qt Imp.",  # ICU4C<74
        "1 qt-Imp.",  # ICU4C>=74
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_measure_unit_69() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    assert (
        fmt.unit(icu.MeasureUnit.create_milligram_ofglucose_per_deciliter())
        .format_int(1)
        .to_string()
    ) == "1 mg/dL"

    assert (
        fmt.unit(icu.MeasureUnit.get_milligram_ofglucose_per_deciliter())
        .format_int(1)
        .to_string()
    ) == "1 mg/dL"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_measure_unit_70() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    # static MeasureUnit *icu::MeasureUnit::createItem(UErrorCode &status)
    assert (fmt.unit(icu.MeasureUnit.create_item()).format_int(1).to_string()) == "1 item"

    # static MeasureUnit *icu::MeasureUnit::createKilowattHourPer100Kilometer(
    #       UErrorCode &status
    # )
    assert (
        fmt.unit(icu.MeasureUnit.create_kilowatt_hour_per_100_kilometer())
        .format_int(1)
        .to_string()
    ) == "1 kWh/100km"

    # static MeasureUnit icu::MeasureUnit::getItem()
    assert (fmt.unit(icu.MeasureUnit.get_item()).format_int(1).to_string()) == "1 item"

    # static MeasureUnit icu::MeasureUnit::getKilowattHourPer100Kilometer()
    assert (
        fmt.unit(icu.MeasureUnit.get_kilowatt_hour_per_100_kilometer())
        .format_int(1)
        .to_string()
    ) == "1 kWh/100km"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_measure_unit_72() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    # static MeasureUnit *icu::MeasureUnit::createQuarter(UErrorCode &status)
    assert (fmt.unit(icu.MeasureUnit.create_quarter()).format_int(1).to_string()) == "1 qtr"

    # static MeasureUnit *icu::MeasureUnit::createTonne(UErrorCode &status)
    assert (fmt.unit(icu.MeasureUnit.create_tonne()).format_int(1).to_string()) == "1 t"

    # static MeasureUnit icu::MeasureUnit::getQuarter()
    assert (fmt.unit(icu.MeasureUnit.get_quarter()).format_int(1).to_string()) == "1 qtr"

    # static MeasureUnit icu::MeasureUnit::getTonne()
    assert (fmt.unit(icu.MeasureUnit.get_tonne()).format_int(1).to_string()) == "1 t"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 73, reason="ICU4C<73")
def test_measure_unit_73() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    # static MeasureUnit * icu::MeasureUnit::createBeaufort(UErrorCode &status)
    assert (fmt.unit(icu.MeasureUnit.create_beaufort()).format_int(1).to_string()) == "B 1"

    # static MeasureUnit icu::MeasureUnit::getBeaufort()
    assert (fmt.unit(icu.MeasureUnit.get_beaufort()).format_int(1).to_string()) == "B 1"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_measure_unit_74() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    # static MeasureUnit *
    # icu::MeasureUnit::createGasolineEnergyDensity(UErrorCode&)
    assert (
        fmt.unit(icu.MeasureUnit.create_gasoline_energy_density()).format_int(1).to_string()
        == "1 gas-equiv"
    )

    # static MeasureUnit icu::MeasureUnit::getGasolineEnergyDensity()
    assert (
        fmt.unit(icu.MeasureUnit.get_gasoline_energy_density()).format_int(1).to_string()
        == "1 gas-equiv"
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 76, reason="ICU4C<76")
def test_measure_unit_76() -> None:
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    # static MeasureUnit icu::MeasureUnit::getLightSpeed()
    assert fmt.unit(icu.MeasureUnit.get_light_speed()).format_int(1).to_string() == "1 light"

    # static MeasureUnit icu::MeasureUnit::getNight()
    assert fmt.unit(icu.MeasureUnit.get_night()).format_int(1).to_string() == "1 night"

    # static MeasureUnit* icu::MeasureUnit::createLightSpeed(UErrorCode &status)
    assert fmt.unit(icu.MeasureUnit.create_light_speed()).format_int(1).to_string() == "1 light"

    # static MeasureUnit* icu::MeasureUnit::createNight(UErrorCode &status)
    assert fmt.unit(icu.MeasureUnit.create_night()).format_int(1).to_string() == "1 night"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 77, reason="ICU4C<77")
def test_measure_unit_77() -> None:
    unit = icu.MeasureUnit.for_identifier("meter-per-second")

    # MeasureUnit icu::MeasureUnit::withConstantDenominator(
    #       uint64_t denominator,
    #       UErrorCode &status
    # ) const
    unit = unit.with_constant_denominator(100)

    # uint64_t icu::MeasureUnit::getConstantDenominator(UErrorCode &status) const
    assert unit.get_constant_denominator() == 100

    assert unit.get_complexity() == icu.UMeasureUnitComplexity.UMEASURE_UNIT_COMPOUND


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM != 77, reason="ICU4C!=77")
def test_measure_unit_77_removed() -> None:
    # static MeasureUnit* icu::MeasureUnit::createPortionPer1E9(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_portion_per_1e9()

    # static MeasureUnit icu::MeasureUnit::getPortionPer1E9()
    unit2 = icu.MeasureUnit.get_portion_per_1e9()
    assert unit1 == unit2


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 78, reason="ICU4C<78")
def test_measure_unit_78() -> None:
    # static MeasureUnit* icu::MeasureUnit::createBecquerel(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_becquerel()

    # static MeasureUnit icu::MeasureUnit::getBecquerel()
    unit2 = icu.MeasureUnit.get_becquerel()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createBritishThermalUnitIt(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_british_thermal_unit_it()

    # static MeasureUnit icu::MeasureUnit::getBritishThermalUnitIt()
    unit2 = icu.MeasureUnit.get_british_thermal_unit_it()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createBuJp(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_bu_jp()

    # static MeasureUnit icu::MeasureUnit::getBuJp()
    unit2 = icu.MeasureUnit.get_bu_jp()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createCalorieIt(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_calorie_it()

    # static MeasureUnit icu::MeasureUnit::getCalorieIt()
    unit2 = icu.MeasureUnit.get_calorie_it()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createChain(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_chain()

    # static MeasureUnit icu::MeasureUnit::getChain()
    unit2 = icu.MeasureUnit.get_chain()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createCho(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_cho()

    # static MeasureUnit icu::MeasureUnit::getCho()
    unit2 = icu.MeasureUnit.get_cho()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createCoulomb(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_coulomb()

    # static MeasureUnit icu::MeasureUnit::getCoulomb()
    unit2 = icu.MeasureUnit.get_coulomb()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createCupImperial(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_cup_imperial()

    # static MeasureUnit icu::MeasureUnit::getCupImperial()
    unit2 = icu.MeasureUnit.get_cup_imperial()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createCupJp(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_cup_jp()

    # static MeasureUnit icu::MeasureUnit::getCupJp()
    unit2 = icu.MeasureUnit.get_cup_jp()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createFarad(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_farad()

    # static MeasureUnit icu::MeasureUnit::getFarad()
    unit2 = icu.MeasureUnit.get_farad()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createFluidOunceMetric(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_fluid_ounce_metric()

    # static MeasureUnit icu::MeasureUnit::getFluidOunceMetric()
    unit2 = icu.MeasureUnit.get_fluid_ounce_metric()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createFortnight(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_fortnight()

    # static MeasureUnit icu::MeasureUnit::getFortnight()
    unit2 = icu.MeasureUnit.get_fortnight()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createFun(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_fun()

    # static MeasureUnit icu::MeasureUnit::getFun()
    unit2 = icu.MeasureUnit.get_fun()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createGray(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_gray()

    # static MeasureUnit icu::MeasureUnit::getGray()
    unit2 = icu.MeasureUnit.get_gray()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createHenry(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_henry()

    # static MeasureUnit icu::MeasureUnit::getHenry()
    unit2 = icu.MeasureUnit.get_henry()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createJoJp(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_jo_jp()

    # static MeasureUnit icu::MeasureUnit::getJoJp()
    unit2 = icu.MeasureUnit.get_jo_jp()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createKatal(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_katal()

    # static MeasureUnit icu::MeasureUnit::getKatal()
    unit2 = icu.MeasureUnit.get_katal()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createKen(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_ken()

    # static MeasureUnit icu::MeasureUnit::getKen()
    unit2 = icu.MeasureUnit.get_ken()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createKilogramForce(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_kilogram_force()

    # static MeasureUnit icu::MeasureUnit::getKilogramForce()
    unit2 = icu.MeasureUnit.get_kilogram_force()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createKoku(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_koku()

    # static MeasureUnit icu::MeasureUnit::getKoku()
    unit2 = icu.MeasureUnit.get_koku()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createKosaji(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_kosaji()

    # static MeasureUnit icu::MeasureUnit::getKosaji()
    unit2 = icu.MeasureUnit.get_kosaji()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createOfglucose(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_ofglucose()

    # static MeasureUnit icu::MeasureUnit::getOfglucose()
    unit2 = icu.MeasureUnit.get_ofglucose()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createOfhg(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_ofhg()

    # static MeasureUnit icu::MeasureUnit::getOfhg()
    unit2 = icu.MeasureUnit.get_ofhg()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createOsaji(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_osaji()

    # static MeasureUnit icu::MeasureUnit::getOsaji()
    unit2 = icu.MeasureUnit.get_osaji()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createPart(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_part()

    # static MeasureUnit icu::MeasureUnit::getPart()
    unit2 = icu.MeasureUnit.get_part()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createPartPer1E6(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_part_per_1e6()

    # static MeasureUnit icu::MeasureUnit::getPartPer1E6()
    unit2 = icu.MeasureUnit.get_part_per_1e6()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createPartPer1E9(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_part_per_1e9()

    # static MeasureUnit icu::MeasureUnit::getPartPer1E9()
    unit2 = icu.MeasureUnit.get_part_per_1e9()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createPintImperial(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_pint_imperial()

    # static MeasureUnit icu::MeasureUnit::getPintImperial()
    unit2 = icu.MeasureUnit.get_pint_imperial()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createRankine(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_rankine()

    # static MeasureUnit icu::MeasureUnit::getRankine()
    unit2 = icu.MeasureUnit.get_rankine()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createRiJp(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_ri_jp()

    # static MeasureUnit icu::MeasureUnit::getRiJp()
    unit2 = icu.MeasureUnit.get_ri_jp()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createRin(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_rin()

    # static MeasureUnit icu::MeasureUnit::getRin()
    unit2 = icu.MeasureUnit.get_rin()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createRod(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_rod()

    # static MeasureUnit icu::MeasureUnit::getRod()
    unit2 = icu.MeasureUnit.get_rod()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSai(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_sai()

    # static MeasureUnit icu::MeasureUnit::getSai()
    unit2 = icu.MeasureUnit.get_sai()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSeJp(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_se_jp()

    # static MeasureUnit icu::MeasureUnit::getSeJp()
    unit2 = icu.MeasureUnit.get_se_jp()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createShaku(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_shaku()

    # static MeasureUnit icu::MeasureUnit::getShaku()
    unit2 = icu.MeasureUnit.get_shaku()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createShakuCloth(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_shaku_cloth()

    # static MeasureUnit icu::MeasureUnit::getShakuCloth()
    unit2 = icu.MeasureUnit.get_shaku_cloth()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createShakuLength(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_shaku_length()

    # static MeasureUnit icu::MeasureUnit::getShakuLength()
    unit2 = icu.MeasureUnit.get_shaku_length()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSiemens(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_siemens()

    # static MeasureUnit icu::MeasureUnit::getSiemens()
    unit2 = icu.MeasureUnit.get_siemens()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSievert(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_sievert()

    # static MeasureUnit icu::MeasureUnit::getSievert()
    unit2 = icu.MeasureUnit.get_sievert()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSlug(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_slug()

    # static MeasureUnit icu::MeasureUnit::getSlug()
    unit2 = icu.MeasureUnit.get_slug()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSteradian(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_steradian()

    # static MeasureUnit icu::MeasureUnit::getSteradian()
    unit2 = icu.MeasureUnit.get_steradian()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createSun(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_sun()

    # static MeasureUnit icu::MeasureUnit::getSun()
    unit2 = icu.MeasureUnit.get_sun()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createTesla(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_tesla()

    # static MeasureUnit icu::MeasureUnit::getTesla()
    unit2 = icu.MeasureUnit.get_tesla()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createToJp(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_to_jp()

    # static MeasureUnit icu::MeasureUnit::getToJp()
    unit2 = icu.MeasureUnit.get_to_jp()
    assert unit1 == unit2

    # static MeasureUnit* icu::MeasureUnit::createWeber(UErrorCode &status)
    unit1 = icu.MeasureUnit.create_weber()

    # static MeasureUnit icu::MeasureUnit::getWeber()
    unit2 = icu.MeasureUnit.get_weber()
    assert unit1 == unit2


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_product() -> None:
    # MeasureUnit icu::MeasureUnit::product(
    #       const MeasureUnit &other,
    #       UErrorCode &status
    # )
    unit = icu.MeasureUnit.for_identifier("kilowatt")
    unit2 = unit.product(icu.MeasureUnit.for_identifier("hour-per-day"))
    assert isinstance(unit2, icu.MeasureUnit)
    assert unit2.get_identifier() in (
        "hour-kilowatt-per-day",  # ICU4C<69
        "kilowatt-hour-per-day",  # ICU4C>=69
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_reciprocal() -> None:
    # MeasureUnit icu::MeasureUnit::reciprocal(UErrorCode &status)
    unit = icu.MeasureUnit.for_identifier("meter-per-second")
    unit2 = unit.reciprocal()
    assert isinstance(unit2, icu.MeasureUnit)
    assert unit2.get_identifier() == "second-per-meter"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_split_to_single_units() -> None:
    unit = icu.MeasureUnit.create_newton_meter()

    # std::pair<LocalArray<MeasureUnit>, int32_t>
    # icu::MeasureUnit::splitToSingleUnits(UErrorCode &status)
    units = unit.split_to_single_units()
    assert isinstance(units, list)
    assert len(units) == 2
    assert all(isinstance(x, icu.MeasureUnit) for x in units)
    assert units[0].get_identifier() == "newton"
    assert units[1].get_identifier() == "meter"

    unit = icu.MeasureUnit.create_kilometer()
    units = unit.split_to_single_units()
    assert isinstance(units, list)
    assert len(units) == 1
    assert all(isinstance(x, icu.MeasureUnit) for x in units)
    assert units[0].get_identifier() == "kilometer"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_with_dimensionality() -> None:
    unit = icu.MeasureUnit.create_kilometer()

    # MeasureUnit icu::MeasureUnit::withDimensionality(
    #       int32_t dimensionality,
    #       UErrorCode &status
    # )
    unit2 = unit.with_dimensionality(2)
    assert isinstance(unit2, icu.MeasureUnit)
    assert unit2.get_identifier() == "square-kilometer"

    unit3 = unit.with_dimensionality(3)
    assert isinstance(unit3, icu.MeasureUnit)
    assert unit3.get_identifier() == "cubic-kilometer"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_with_prefix() -> None:
    unit = icu.MeasureUnit.create_byte()

    # MeasureUnit icu::MeasureUnit::withPrefix(
    #       UMeasurePrefix prefix,
    #       UErrorCode &status
    # )
    unit2 = unit.with_prefix(icu.UMeasurePrefix.UMEASURE_PREFIX_KILO)
    assert isinstance(unit2, icu.MeasureUnit)
    assert unit2.get_identifier() == "kilobyte"

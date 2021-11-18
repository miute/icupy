import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 62:
    pytest.skip("ICU4C<62", allow_module_level=True)

import copy

from icupy.icu import (
    ICUError, Locale, MeasureUnit, StringEnumeration, UErrorCode,
)
from icupy.icu.number import NumberFormatter


def test_api():
    # [1]
    # static int32_t icu::MeasureUnit::getAvailable(
    #       const char *type,
    #       MeasureUnit *destArray,
    #       int32_t destCapacity,
    #       UErrorCode &errorCode
    # )
    dest_array = MeasureUnit.get_available("currency")
    assert isinstance(dest_array, list)
    assert len(dest_array) > 0
    assert all(isinstance(x, MeasureUnit) for x in dest_array)
    assert all(x.get_type() == "currency" for x in dest_array)

    # [2]
    # static int32_t icu::MeasureUnit::getAvailable(
    #       MeasureUnit *destArray,
    #       int32_t destCapacity,
    #       UErrorCode &errorCode
    # )
    dest_array = MeasureUnit.get_available()
    assert isinstance(dest_array, list)
    assert len(dest_array) > 0
    assert all(isinstance(x, MeasureUnit) for x in dest_array)

    # static StringEnumeration *icu::MeasureUnit::getAvailableTypes(
    #       UErrorCode &errorCode
    # )
    types = MeasureUnit.get_available_types()
    assert isinstance(types, StringEnumeration)
    assert len(types) > 0
    assert "currency" in types

    # const char *icu::MeasureUnit::getSubtype()
    unit = MeasureUnit.create_square_kilometer()
    result = unit.get_subtype()
    assert isinstance(result, str)
    assert result == "square-kilometer"

    # const char *icu::MeasureUnit::getType()
    result = unit.get_type()
    assert isinstance(result, str)
    assert result == "area"

    # UBool icu::MeasureUnit::operator!=(const UObject &other)
    unit1 = MeasureUnit.get_kilometer()
    unit2 = unit1.clone()
    unit3 = MeasureUnit.get_kilometer_per_hour()
    assert not (unit1 != unit2)
    assert unit1 != unit3
    assert unit2 != unit3

    # UBool icu::MeasureUnit::operator==(const UObject &other)
    assert unit1 == unit2
    assert not (unit1 == unit3)
    assert not (unit2 == unit3)


def test_clone():
    unit1 = MeasureUnit.get_kilometer()

    # MeasureUnit *icu::MeasureUnit::clone()
    unit2 = unit1.clone()
    assert isinstance(unit2, MeasureUnit)
    assert unit2 == unit1

    unit3 = copy.copy(unit1)
    assert unit3 == unit1

    unit4 = copy.deepcopy(unit1)
    assert unit4 == unit1


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_for_identifier():
    # static MeasureUnit icu::MeasureUnit::forIdentifier(
    #       StringPiece identifier,
    #       UErrorCode &status
    # )
    unit = MeasureUnit.for_identifier("furlong-per-nanosecond")
    assert isinstance(unit, MeasureUnit)

    # const char *icu::MeasureUnit::getIdentifier()
    result = unit.get_identifier()
    assert isinstance(result, str)
    assert result == "furlong-per-nanosecond"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_get_complexity():
    from icupy.icu import UMeasureUnitComplexity

    # UMeasureUnitComplexity icu::MeasureUnit::getComplexity(
    #       UErrorCode &status
    # )
    unit = MeasureUnit.get_meter()
    assert (unit.get_complexity()
            == UMeasureUnitComplexity.UMEASURE_UNIT_SINGLE)

    unit = MeasureUnit.for_identifier("meter-per-second")
    assert (unit.get_complexity()
            == UMeasureUnitComplexity.UMEASURE_UNIT_COMPOUND)

    unit = MeasureUnit.for_identifier("hour-and-minute")
    assert (unit.get_complexity()
            == UMeasureUnitComplexity.UMEASURE_UNIT_MIXED)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_get_dimensionality():
    # int32_t icu::MeasureUnit::getDimensionality(UErrorCode &status)
    unit = MeasureUnit.create_cubic_meter()
    assert unit.get_dimensionality() == 3

    unit = MeasureUnit.create_meter()
    assert unit.get_dimensionality() == 1


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_get_prefix():
    from icupy.icu import UMeasurePrefix

    # UMeasurePrefix icu::MeasureUnit::getPrefix(UErrorCode &status)
    unit = MeasureUnit.get_kilometer()
    assert unit.get_prefix() == UMeasurePrefix.UMEASURE_PREFIX_KILO


def test_measure_unit_53():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_acre())
            .format_int(1)
            .to_string()) == "1 ac"

    assert (fmt.unit(MeasureUnit.create_arc_minute())
            .format_int(1)
            .to_string()) == "1 arcmin"

    assert (fmt.unit(MeasureUnit.create_arc_second())
            .format_int(1)
            .to_string()) == "1 arcsec"

    assert (fmt.unit(MeasureUnit.create_celsius())
            .format_int(1)
            .to_string()) == "1\xb0C"  # 1°C

    assert (fmt.unit(MeasureUnit.create_centimeter())
            .format_int(1)
            .to_string()) == "1 cm"

    assert (fmt.unit(MeasureUnit.create_cubic_kilometer())
            .format_int(1)
            .to_string()) == "1 km\xb3"  # 1 km³

    assert (fmt.unit(MeasureUnit.create_cubic_mile())
            .format_int(1)
            .to_string()) == "1 mi\xb3"  # 1 mi³

    assert (fmt.unit(MeasureUnit.create_day())
            .format_int(1)
            .to_string()) == "1 day"

    assert (fmt.unit(MeasureUnit.create_degree())
            .format_int(1)
            .to_string()) == "1 deg"

    assert (fmt.unit(MeasureUnit.create_fahrenheit())
            .format_int(1)
            .to_string()) == "1\xb0F"  # 1°F

    assert (fmt.unit(MeasureUnit.create_foot())
            .format_int(1)
            .to_string()) == "1 ft"

    assert (fmt.unit(MeasureUnit.create_gforce())
            .format_int(1)
            .to_string()) == "1 G"

    assert (fmt.unit(MeasureUnit.create_gram())
            .format_int(1)
            .to_string()) == "1 g"

    assert (fmt.unit(MeasureUnit.create_hectare())
            .format_int(1)
            .to_string()) == "1 ha"

    assert (fmt.unit(MeasureUnit.create_hectopascal())
            .format_int(1)
            .to_string()) == "1 hPa"

    assert (fmt.unit(MeasureUnit.create_horsepower())
            .format_int(1)
            .to_string()) == "1 hp"

    assert (fmt.unit(MeasureUnit.create_hour())
            .format_int(1)
            .to_string()) == "1 hr"

    assert (fmt.unit(MeasureUnit.create_inch())
            .format_int(1)
            .to_string()) == "1 in"

    assert (fmt.unit(MeasureUnit.create_inch_hg())
            .format_int(1)
            .to_string()) == "1 inHg"

    assert (fmt.unit(MeasureUnit.create_kilogram())
            .format_int(1)
            .to_string()) == "1 kg"

    assert (fmt.unit(MeasureUnit.create_kilometer())
            .format_int(1)
            .to_string()) == "1 km"

    assert (fmt.unit(MeasureUnit.create_kilometer_per_hour())
            .format_int(1)
            .to_string()) == "1 km/h"

    assert (fmt.unit(MeasureUnit.create_kilowatt())
            .format_int(1)
            .to_string()) == "1 kW"

    assert (fmt.unit(MeasureUnit.create_light_year())
            .format_int(1)
            .to_string()) == "1 ly"

    assert (fmt.unit(MeasureUnit.create_liter())
            .format_int(1)
            .to_string()) == "1 L"

    assert (fmt.unit(MeasureUnit.create_meter())
            .format_int(1)
            .to_string()) == "1 m"

    assert (fmt.unit(MeasureUnit.create_meter_per_second())
            .format_int(1)
            .to_string()) == "1 m/s"

    assert (fmt.unit(MeasureUnit.create_mile())
            .format_int(1)
            .to_string()) == "1 mi"

    assert (fmt.unit(MeasureUnit.create_mile_per_hour())
            .format_int(1)
            .to_string()) == "1 mph"

    assert (fmt.unit(MeasureUnit.create_millibar())
            .format_int(1)
            .to_string()) == "1 mbar"

    assert (fmt.unit(MeasureUnit.create_millimeter())
            .format_int(1)
            .to_string()) == "1 mm"

    assert (fmt.unit(MeasureUnit.create_millisecond())
            .format_int(1)
            .to_string()) == "1 ms"

    assert (fmt.unit(MeasureUnit.create_minute())
            .format_int(1)
            .to_string()) == "1 min"

    assert (fmt.unit(MeasureUnit.create_month())
            .format_int(1)
            .to_string()) == "1 mth"

    assert (fmt.unit(MeasureUnit.create_ounce())
            .format_int(1)
            .to_string()) == "1 oz"

    assert (fmt.unit(MeasureUnit.create_picometer())
            .format_int(1)
            .to_string()) == "1 pm"

    assert (fmt.unit(MeasureUnit.create_pound())
            .format_int(1)
            .to_string()) == "1 lb"

    assert (fmt.unit(MeasureUnit.create_second())
            .format_int(1)
            .to_string()) == "1 sec"

    assert (fmt.unit(MeasureUnit.create_square_foot())
            .format_int(1)
            .to_string()) == "1 sq ft"

    assert (fmt.unit(MeasureUnit.create_square_kilometer())
            .format_int(1)
            .to_string()) == "1 km\xb2"  # 1 km²

    assert (fmt.unit(MeasureUnit.create_square_meter())
            .format_int(1)
            .to_string()) == "1 m\xb2"  # 1 m²

    assert (fmt.unit(MeasureUnit.create_square_mile())
            .format_int(1)
            .to_string()) == "1 sq mi"

    assert (fmt.unit(MeasureUnit.create_watt())
            .format_int(1)
            .to_string()) == "1 W"

    assert (fmt.unit(MeasureUnit.create_week())
            .format_int(1)
            .to_string()) == "1 wk"

    assert (fmt.unit(MeasureUnit.create_yard())
            .format_int(1)
            .to_string()) == "1 yd"

    assert (fmt.unit(MeasureUnit.create_year())
            .format_int(1)
            .to_string()) == "1 yr"


def test_measure_unit_54():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_acre_foot())
            .format_int(1)
            .to_string()) == "1 ac ft"

    assert (fmt.unit(MeasureUnit.create_ampere())
            .format_int(1)
            .to_string()) == "1 A"

    assert (fmt.unit(MeasureUnit.create_astronomical_unit())
            .format_int(1)
            .to_string()) == "1 au"

    assert (fmt.unit(MeasureUnit.create_bit())
            .format_int(1)
            .to_string()) == "1 bit"

    assert (fmt.unit(MeasureUnit.create_bushel())
            .format_int(1)
            .to_string()) == "1 bu"

    assert (fmt.unit(MeasureUnit.create_byte())
            .format_int(1)
            .to_string()) == "1 byte"

    assert (fmt.unit(MeasureUnit.create_calorie())
            .format_int(1)
            .to_string()) == "1 cal"

    assert (fmt.unit(MeasureUnit.create_carat())
            .format_int(1)
            .to_string()) == "1 CD"

    assert (fmt.unit(MeasureUnit.create_centiliter())
            .format_int(1)
            .to_string()) == "1 cL"

    assert (fmt.unit(MeasureUnit.create_cubic_centimeter())
            .format_int(1)
            .to_string()) == "1 cm\xb3"  # 1 cm³

    assert (fmt.unit(MeasureUnit.create_cubic_foot())
            .format_int(1)
            .to_string()) == "1 ft\xb3"  # 1 ft³

    assert (fmt.unit(MeasureUnit.create_cubic_inch())
            .format_int(1)
            .to_string()) == "1 in\xb3"  # 1 in³

    assert (fmt.unit(MeasureUnit.create_cubic_meter())
            .format_int(1)
            .to_string()) == "1 m\xb3"  # 1 m³

    assert (fmt.unit(MeasureUnit.create_cubic_yard())
            .format_int(1)
            .to_string()) == "1 yd\xb3"  # 1 yd³

    assert (fmt.unit(MeasureUnit.create_cup())
            .format_int(1)
            .to_string()) == "1 c"

    assert (fmt.unit(MeasureUnit.create_deciliter())
            .format_int(1)
            .to_string()) == "1 dL"

    assert (fmt.unit(MeasureUnit.create_decimeter())
            .format_int(1)
            .to_string()) == "1 dm"

    assert (fmt.unit(MeasureUnit.create_fathom())
            .format_int(1)
            .to_string()) == "1 fth"

    assert (fmt.unit(MeasureUnit.create_fluid_ounce())
            .format_int(1)
            .to_string()) == "1 fl oz"

    assert (fmt.unit(MeasureUnit.create_foodcalorie())
            .format_int(1)
            .to_string()) == "1 Cal"

    assert (fmt.unit(MeasureUnit.create_furlong())
            .format_int(1)
            .to_string()) == "1 fur"

    assert (fmt.unit(MeasureUnit.create_gallon())
            .format_int(1)
            .to_string()) == "1 gal"

    assert (fmt.unit(MeasureUnit.create_gigabit())
            .format_int(1)
            .to_string()) == "1 Gb"

    assert (fmt.unit(MeasureUnit.create_gigabyte())
            .format_int(1)
            .to_string()) == "1 GB"

    assert (fmt.unit(MeasureUnit.create_gigahertz())
            .format_int(1)
            .to_string()) == "1 GHz"

    assert (fmt.unit(MeasureUnit.create_gigawatt())
            .format_int(1)
            .to_string()) == "1 GW"

    assert (fmt.unit(MeasureUnit.create_hectoliter())
            .format_int(1)
            .to_string()) == "1 hL"

    assert (fmt.unit(MeasureUnit.create_hertz())
            .format_int(1)
            .to_string()) == "1 Hz"

    assert (fmt.unit(MeasureUnit.create_joule())
            .format_int(1)
            .to_string()) == "1 J"

    assert (fmt.unit(MeasureUnit.create_karat())
            .format_int(1)
            .to_string()) == "1 kt"

    assert (fmt.unit(MeasureUnit.create_kelvin())
            .format_int(1)
            .to_string()) == "1 K"

    assert (fmt.unit(MeasureUnit.create_kilobit())
            .format_int(1)
            .to_string()) == "1 kb"

    assert (fmt.unit(MeasureUnit.create_kilobyte())
            .format_int(1)
            .to_string()) == "1 kB"

    assert (fmt.unit(MeasureUnit.create_kilocalorie())
            .format_int(1)
            .to_string()) == "1 kcal"

    assert (fmt.unit(MeasureUnit.create_kilohertz())
            .format_int(1)
            .to_string()) == "1 kHz"

    assert (fmt.unit(MeasureUnit.create_kilojoule())
            .format_int(1)
            .to_string()) == "1 kJ"

    assert (fmt.unit(MeasureUnit.create_kilowatt_hour())
            .format_int(1)
            .to_string()) == "1 kWh"

    assert (fmt.unit(MeasureUnit.create_liter_per_kilometer())
            .format_int(1)
            .to_string()) == "1 L/km"

    assert (fmt.unit(MeasureUnit.create_lux())
            .format_int(1)
            .to_string()) == "1 lx"

    assert (fmt.unit(MeasureUnit.create_megabit())
            .format_int(1)
            .to_string()) == "1 Mb"

    assert (fmt.unit(MeasureUnit.create_megabyte())
            .format_int(1)
            .to_string()) == "1 MB"

    assert (fmt.unit(MeasureUnit.create_megahertz())
            .format_int(1)
            .to_string()) == "1 MHz"

    assert (fmt.unit(MeasureUnit.create_megaliter())
            .format_int(1)
            .to_string()) == "1 ML"

    assert (fmt.unit(MeasureUnit.create_megawatt())
            .format_int(1)
            .to_string()) == "1 MW"

    assert (fmt.unit(MeasureUnit.create_meter_per_second_squared())
            .format_int(1)
            .to_string()) == "1 m/s\xb2"  # 1 m/s²

    assert (fmt.unit(MeasureUnit.create_metric_ton())
            .format_int(1)
            .to_string()) == "1 t"

    assert (fmt.unit(MeasureUnit.create_microgram())
            .format_int(1)
            .to_string()) in ("1 \xb5g", "1 \u03bcg")  # 1 µg

    assert (fmt.unit(MeasureUnit.create_micrometer())
            .format_int(1)
            .to_string()) in ("1 \xb5m", "1 \u03bcm")  # 1 µm

    assert (fmt.unit(MeasureUnit.create_microsecond())
            .format_int(1)
            .to_string()) in ("1 \xb5s", "1 \u03bcs")  # 1 µs

    assert (fmt.unit(MeasureUnit.create_mile_per_gallon())
            .format_int(1)
            .to_string()) == "1 mpg"

    assert (fmt.unit(MeasureUnit.create_milliampere())
            .format_int(1)
            .to_string()) == "1 mA"

    assert (fmt.unit(MeasureUnit.create_milligram())
            .format_int(1)
            .to_string()) == "1 mg"

    assert (fmt.unit(MeasureUnit.create_milliliter())
            .format_int(1)
            .to_string()) == "1 mL"

    assert (fmt.unit(MeasureUnit.create_millimeter_of_mercury())
            .format_int(1)
            .to_string()) == "1 mmHg"

    assert (fmt.unit(MeasureUnit.create_milliwatt())
            .format_int(1)
            .to_string()) == "1 mW"

    assert (fmt.unit(MeasureUnit.create_nanometer())
            .format_int(1)
            .to_string()) == "1 nm"

    assert (fmt.unit(MeasureUnit.create_nanosecond())
            .format_int(1)
            .to_string()) == "1 ns"

    assert (fmt.unit(MeasureUnit.create_nautical_mile())
            .format_int(1)
            .to_string()) == "1 nmi"

    assert (fmt.unit(MeasureUnit.create_ohm())
            .format_int(1)
            .to_string()) == "1 \u03a9"  # 1 Ω

    assert (fmt.unit(MeasureUnit.create_ounce_troy())
            .format_int(1)
            .to_string()) == "1 oz t"

    assert (fmt.unit(MeasureUnit.create_parsec())
            .format_int(1)
            .to_string()) == "1 pc"

    assert (fmt.unit(MeasureUnit.create_pint())
            .format_int(1)
            .to_string()) == "1 pt"

    assert (fmt.unit(MeasureUnit.create_pound_per_square_inch())
            .format_int(1)
            .to_string()) == "1 psi"

    assert (fmt.unit(MeasureUnit.create_quart())
            .format_int(1)
            .to_string()) == "1 qt"

    assert (fmt.unit(MeasureUnit.create_radian())
            .format_int(1)
            .to_string()) == "1 rad"

    assert (fmt.unit(MeasureUnit.create_square_centimeter())
            .format_int(1)
            .to_string()) == "1 cm\xb2"  # 1 cm²

    assert (fmt.unit(MeasureUnit.create_square_inch())
            .format_int(1)
            .to_string()) == "1 in\xb2"  # 1 in²

    assert (fmt.unit(MeasureUnit.create_square_yard())
            .format_int(1)
            .to_string()) == "1 yd\xb2"  # 1 yd²

    assert (fmt.unit(MeasureUnit.create_stone())
            .format_int(1)
            .to_string()) == "1 st"

    assert (fmt.unit(MeasureUnit.create_tablespoon())
            .format_int(1)
            .to_string()) == "1 tbsp"

    assert (fmt.unit(MeasureUnit.create_teaspoon())
            .format_int(1)
            .to_string()) == "1 tsp"

    assert (fmt.unit(MeasureUnit.create_terabit())
            .format_int(1)
            .to_string()) == "1 Tb"

    assert (fmt.unit(MeasureUnit.create_terabyte())
            .format_int(1)
            .to_string()) == "1 TB"

    assert (fmt.unit(MeasureUnit.create_ton())
            .format_int(1)
            .to_string()) == "1 tn"

    assert (fmt.unit(MeasureUnit.create_volt())
            .format_int(1)
            .to_string()) == "1 V"


def test_measure_unit_56():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_century())
            .format_int(1)
            .to_string()) == "1 c"

    assert (fmt.unit(MeasureUnit.create_cup_metric())
            .format_int(1)
            .to_string()) == "1 mc"

    assert (fmt.unit(MeasureUnit.create_generic_temperature())
            .format_int(1)
            .to_string()) == "1\xb0"  # 1°

    assert (fmt.unit(MeasureUnit.create_knot())
            .format_int(1)
            .to_string()) == "1 kn"

    assert (fmt.unit(MeasureUnit.create_liter_per_100_kilometers())
            .format_int(1)
            .to_string()) == "1 L/100 km"

    assert (fmt.unit(MeasureUnit.create_mile_scandinavian())
            .format_int(1)
            .to_string()) == "1 smi"

    assert (fmt.unit(MeasureUnit.create_pint_metric())
            .format_int(1)
            .to_string()) == "1 mpt"

    assert (fmt.unit(MeasureUnit.create_revolution_angle())
            .format_int(1)
            .to_string()) == "1 rev"


def test_measure_unit_57():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_gallon_imperial())
            .format_int(1)
            .to_string()) == "1 gal Imp."

    assert (fmt.unit(MeasureUnit.create_mile_per_gallon_imperial())
            .format_int(1)
            .to_string()) == "1 mpg Imp."

    try:
        assert (fmt.unit(MeasureUnit.create_milligram_per_deciliter())
                .format_int(1)
                .to_string()) == "1 mg/dL"
    except ICUError as ex:
        assert ex.args[0] == UErrorCode.U_MISSING_RESOURCE_ERROR  # ICU 69.1

    assert (fmt.unit(MeasureUnit.create_millimole_per_liter())
            .format_int(1)
            .to_string()) == "1 mmol/L"

    assert (fmt.unit(MeasureUnit.create_part_per_million())
            .format_int(1)
            .to_string()) == "1 ppm"


def test_measure_unit_59():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_point())
            .format_int(1)
            .to_string()) == "1 pt"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_measure_unit_63():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_atmosphere())
            .format_int(1)
            .to_string()) == "1 atm"

    assert (fmt.unit(MeasureUnit.create_percent())
            .format_int(1)
            .to_string()) == "1%"

    assert (fmt.unit(MeasureUnit.create_permille())
            .format_int(1)
            .to_string()) == "1\u2030"  # 1‰

    assert (fmt.unit(MeasureUnit.create_petabyte())
            .format_int(1)
            .to_string()) == "1 PB"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_measure_unit_64():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_barrel())
            .format_int(1)
            .to_string()) == "1 bbl"

    assert (fmt.unit(MeasureUnit.create_british_thermal_unit())
            .format_int(1)
            .to_string()) == "1 Btu"

    assert (fmt.unit(MeasureUnit.create_dalton())
            .format_int(1)
            .to_string()) == "1 Da"

    assert (fmt.unit(MeasureUnit.create_day_person())
            .format_int(1)
            .to_string()) == "1 day"

    assert (fmt.unit(MeasureUnit.create_dunam())
            .format_int(1)
            .to_string()) == "1 dunam"

    assert (fmt.unit(MeasureUnit.create_earth_mass())
            .format_int(1)
            .to_string()) == "1 M\u2295"  # 1 M⊕

    assert (fmt.unit(MeasureUnit.create_electronvolt())
            .format_int(1)
            .to_string()) == "1 eV"

    assert (fmt.unit(MeasureUnit.create_fluid_ounce_imperial())
            .format_int(1)
            .to_string()) == "1 fl oz Imp."

    assert (fmt.unit(MeasureUnit.create_kilopascal())
            .format_int(1)
            .to_string()) == "1 kPa"

    assert (fmt.unit(MeasureUnit.create_megapascal())
            .format_int(1)
            .to_string()) == "1 MPa"

    assert (fmt.unit(MeasureUnit.create_mole())
            .format_int(1)
            .to_string()) == "1 mol"

    assert (fmt.unit(MeasureUnit.create_month_person())
            .format_int(1)
            .to_string()) == "1 mth"

    assert (fmt.unit(MeasureUnit.create_newton())
            .format_int(1)
            .to_string()) == "1 N"

    assert (fmt.unit(MeasureUnit.create_newton_meter())
            .format_int(1)
            .to_string()) == "1 N\u22c5m"  # 1 N⋅m

    assert (fmt.unit(MeasureUnit.create_permyriad())
            .format_int(1)
            .to_string()) == "1\u2031"  # 1‱

    assert (fmt.unit(MeasureUnit.create_pound_foot())
            .format_int(1)
            .to_string()) == "1 lbf\u22c5ft"  # 1 lb⋅ft

    assert (fmt.unit(MeasureUnit.create_pound_force())
            .format_int(1)
            .to_string()) == "1 lbf"

    assert (fmt.unit(MeasureUnit.create_solar_luminosity())
            .format_int(1)
            .to_string()) == "1 L\u2609"  # 1 L☉

    assert (fmt.unit(MeasureUnit.create_solar_mass())
            .format_int(1)
            .to_string()) == "1 M\u2609"  # 1 M☉

    assert (fmt.unit(MeasureUnit.create_solar_radius())
            .format_int(1)
            .to_string()) == "1 R\u2609"  # 1 R☉

    assert (fmt.unit(MeasureUnit.create_week_person())
            .format_int(1)
            .to_string()) == "1 wk"

    assert (fmt.unit(MeasureUnit.create_year_person())
            .format_int(1)
            .to_string()) == "1 yr"

    assert (fmt.unit(MeasureUnit.get_acre())
            .format_int(1)
            .to_string()) == "1 ac"

    assert (fmt.unit(MeasureUnit.get_acre_foot())
            .format_int(1)
            .to_string()) == "1 ac ft"

    assert (fmt.unit(MeasureUnit.get_ampere())
            .format_int(1)
            .to_string()) == "1 A"

    assert (fmt.unit(MeasureUnit.get_arc_minute())
            .format_int(1)
            .to_string()) == "1 arcmin"

    assert (fmt.unit(MeasureUnit.get_arc_second())
            .format_int(1)
            .to_string()) == "1 arcsec"

    assert (fmt.unit(MeasureUnit.get_astronomical_unit())
            .format_int(1)
            .to_string()) == "1 au"

    assert (fmt.unit(MeasureUnit.get_atmosphere())
            .format_int(1)
            .to_string()) == "1 atm"

    assert (fmt.unit(MeasureUnit.get_barrel())
            .format_int(1)
            .to_string()) == "1 bbl"

    assert (fmt.unit(MeasureUnit.get_bit())
            .format_int(1)
            .to_string()) == "1 bit"

    assert (fmt.unit(MeasureUnit.get_british_thermal_unit())
            .format_int(1)
            .to_string()) == "1 Btu"

    assert (fmt.unit(MeasureUnit.get_bushel())
            .format_int(1)
            .to_string()) == "1 bu"

    assert (fmt.unit(MeasureUnit.get_byte())
            .format_int(1)
            .to_string()) == "1 byte"

    assert (fmt.unit(MeasureUnit.get_calorie())
            .format_int(1)
            .to_string()) == "1 cal"

    assert (fmt.unit(MeasureUnit.get_carat())
            .format_int(1)
            .to_string()) == "1 CD"

    assert (fmt.unit(MeasureUnit.get_celsius())
            .format_int(1)
            .to_string()) == "1\xb0C"  # 1°C

    assert (fmt.unit(MeasureUnit.get_centiliter())
            .format_int(1)
            .to_string()) == "1 cL"

    assert (fmt.unit(MeasureUnit.get_centimeter())
            .format_int(1)
            .to_string()) == "1 cm"

    assert (fmt.unit(MeasureUnit.get_century())
            .format_int(1)
            .to_string()) == "1 c"

    assert (fmt.unit(MeasureUnit.get_cubic_centimeter())
            .format_int(1)
            .to_string()) == "1 cm\xb3"  # 1 cm³

    assert (fmt.unit(MeasureUnit.get_cubic_foot())
            .format_int(1)
            .to_string()) == "1 ft\xb3"  # 1 ft³

    assert (fmt.unit(MeasureUnit.get_cubic_inch())
            .format_int(1)
            .to_string()) == "1 in\xb3"  # 1 in³

    assert (fmt.unit(MeasureUnit.get_cubic_kilometer())
            .format_int(1)
            .to_string()) == "1 km\xb3"  # 1 km³

    assert (fmt.unit(MeasureUnit.get_cubic_meter())
            .format_int(1)
            .to_string()) == "1 m\xb3"  # 1 m³

    assert (fmt.unit(MeasureUnit.get_cubic_mile())
            .format_int(1)
            .to_string()) == "1 mi\xb3"  # 1 mi³

    assert (fmt.unit(MeasureUnit.get_cubic_yard())
            .format_int(1)
            .to_string()) == "1 yd\xb3"  # 1 yd³

    assert (fmt.unit(MeasureUnit.get_cup())
            .format_int(1)
            .to_string()) == "1 c"

    assert (fmt.unit(MeasureUnit.get_cup_metric())
            .format_int(1)
            .to_string()) == "1 mc"

    assert (fmt.unit(MeasureUnit.get_dalton())
            .format_int(1)
            .to_string()) == "1 Da"

    assert (fmt.unit(MeasureUnit.get_day())
            .format_int(1)
            .to_string()) == "1 day"

    assert (fmt.unit(MeasureUnit.get_day_person())
            .format_int(1)
            .to_string()) == "1 day"

    assert (fmt.unit(MeasureUnit.get_deciliter())
            .format_int(1)
            .to_string()) == "1 dL"

    assert (fmt.unit(MeasureUnit.get_decimeter())
            .format_int(1)
            .to_string()) == "1 dm"

    assert (fmt.unit(MeasureUnit.get_degree())
            .format_int(1)
            .to_string()) == "1 deg"

    assert (fmt.unit(MeasureUnit.get_dunam())
            .format_int(1)
            .to_string()) == "1 dunam"

    assert (fmt.unit(MeasureUnit.get_earth_mass())
            .format_int(1)
            .to_string()) == "1 M\u2295"  # 1 M⊕

    assert (fmt.unit(MeasureUnit.get_electronvolt())
            .format_int(1)
            .to_string()) == "1 eV"

    assert (fmt.unit(MeasureUnit.get_fahrenheit())
            .format_int(1)
            .to_string()) == "1\xb0F"  # 1°F

    assert (fmt.unit(MeasureUnit.get_fathom())
            .format_int(1)
            .to_string()) == "1 fth"

    assert (fmt.unit(MeasureUnit.get_fluid_ounce())
            .format_int(1)
            .to_string()) == "1 fl oz"

    assert (fmt.unit(MeasureUnit.get_fluid_ounce_imperial())
            .format_int(1)
            .to_string()) == "1 fl oz Imp."

    assert (fmt.unit(MeasureUnit.get_foodcalorie())
            .format_int(1)
            .to_string()) == "1 Cal"

    assert (fmt.unit(MeasureUnit.get_foot())
            .format_int(1)
            .to_string()) == "1 ft"

    assert (fmt.unit(MeasureUnit.get_furlong())
            .format_int(1)
            .to_string()) == "1 fur"

    assert (fmt.unit(MeasureUnit.get_gallon())
            .format_int(1)
            .to_string()) == "1 gal"

    assert (fmt.unit(MeasureUnit.get_gallon_imperial())
            .format_int(1)
            .to_string()) == "1 gal Imp."

    assert (fmt.unit(MeasureUnit.get_generic_temperature())
            .format_int(1)
            .to_string()) == "1\xb0"  # 1°

    assert (fmt.unit(MeasureUnit.get_gforce())
            .format_int(1)
            .to_string()) == "1 G"

    assert (fmt.unit(MeasureUnit.get_gigabit())
            .format_int(1)
            .to_string()) == "1 Gb"

    assert (fmt.unit(MeasureUnit.get_gigabyte())
            .format_int(1)
            .to_string()) == "1 GB"

    assert (fmt.unit(MeasureUnit.get_gigahertz())
            .format_int(1)
            .to_string()) == "1 GHz"

    assert (fmt.unit(MeasureUnit.get_gigawatt())
            .format_int(1)
            .to_string()) == "1 GW"

    assert (fmt.unit(MeasureUnit.get_gram())
            .format_int(1)
            .to_string()) == "1 g"

    assert (fmt.unit(MeasureUnit.get_hectare())
            .format_int(1)
            .to_string()) == "1 ha"

    assert (fmt.unit(MeasureUnit.get_hectoliter())
            .format_int(1)
            .to_string()) == "1 hL"

    assert (fmt.unit(MeasureUnit.get_hectopascal())
            .format_int(1)
            .to_string()) == "1 hPa"

    assert (fmt.unit(MeasureUnit.get_hertz())
            .format_int(1)
            .to_string()) == "1 Hz"

    assert (fmt.unit(MeasureUnit.get_horsepower())
            .format_int(1)
            .to_string()) == "1 hp"

    assert (fmt.unit(MeasureUnit.get_hour())
            .format_int(1)
            .to_string()) == "1 hr"

    assert (fmt.unit(MeasureUnit.get_inch())
            .format_int(1)
            .to_string()) == "1 in"

    assert (fmt.unit(MeasureUnit.get_inch_hg())
            .format_int(1)
            .to_string()) == "1 inHg"

    assert (fmt.unit(MeasureUnit.get_joule())
            .format_int(1)
            .to_string()) == "1 J"

    assert (fmt.unit(MeasureUnit.get_karat())
            .format_int(1)
            .to_string()) == "1 kt"

    assert (fmt.unit(MeasureUnit.get_kelvin())
            .format_int(1)
            .to_string()) == "1 K"

    assert (fmt.unit(MeasureUnit.get_kilobit())
            .format_int(1)
            .to_string()) == "1 kb"

    assert (fmt.unit(MeasureUnit.get_kilobyte())
            .format_int(1)
            .to_string()) == "1 kB"

    assert (fmt.unit(MeasureUnit.get_kilocalorie())
            .format_int(1)
            .to_string()) == "1 kcal"

    assert (fmt.unit(MeasureUnit.get_kilogram())
            .format_int(1)
            .to_string()) == "1 kg"

    assert (fmt.unit(MeasureUnit.get_kilohertz())
            .format_int(1)
            .to_string()) == "1 kHz"

    assert (fmt.unit(MeasureUnit.get_kilojoule())
            .format_int(1)
            .to_string()) == "1 kJ"

    assert (fmt.unit(MeasureUnit.get_kilometer())
            .format_int(1)
            .to_string()) == "1 km"

    assert (fmt.unit(MeasureUnit.get_kilometer_per_hour())
            .format_int(1)
            .to_string()) == "1 km/h"

    assert (fmt.unit(MeasureUnit.get_kilopascal())
            .format_int(1)
            .to_string()) == "1 kPa"

    assert (fmt.unit(MeasureUnit.get_kilowatt())
            .format_int(1)
            .to_string()) == "1 kW"

    assert (fmt.unit(MeasureUnit.get_kilowatt_hour())
            .format_int(1)
            .to_string()) == "1 kWh"

    assert (fmt.unit(MeasureUnit.get_knot())
            .format_int(1)
            .to_string()) == "1 kn"

    assert (fmt.unit(MeasureUnit.get_light_year())
            .format_int(1)
            .to_string()) == "1 ly"

    assert (fmt.unit(MeasureUnit.get_liter())
            .format_int(1)
            .to_string()) == "1 L"

    assert (fmt.unit(MeasureUnit.get_liter_per_100_kilometers())
            .format_int(1)
            .to_string()) == "1 L/100 km"

    assert (fmt.unit(MeasureUnit.get_liter_per_kilometer())
            .format_int(1)
            .to_string()) == "1 L/km"

    assert (fmt.unit(MeasureUnit.get_lux())
            .format_int(1)
            .to_string()) == "1 lx"

    assert (fmt.unit(MeasureUnit.get_megabit())
            .format_int(1)
            .to_string()) == "1 Mb"

    assert (fmt.unit(MeasureUnit.get_megabyte())
            .format_int(1)
            .to_string()) == "1 MB"

    assert (fmt.unit(MeasureUnit.get_megahertz())
            .format_int(1)
            .to_string()) == "1 MHz"

    assert (fmt.unit(MeasureUnit.get_megaliter())
            .format_int(1)
            .to_string()) == "1 ML"

    assert (fmt.unit(MeasureUnit.get_megapascal())
            .format_int(1)
            .to_string()) == "1 MPa"

    assert (fmt.unit(MeasureUnit.get_megawatt())
            .format_int(1)
            .to_string()) == "1 MW"

    assert (fmt.unit(MeasureUnit.get_meter())
            .format_int(1)
            .to_string()) == "1 m"

    assert (fmt.unit(MeasureUnit.get_meter_per_second())
            .format_int(1)
            .to_string()) == "1 m/s"

    assert (fmt.unit(MeasureUnit.get_meter_per_second_squared())
            .format_int(1)
            .to_string()) == "1 m/s\xb2"  # 1 m/s²

    assert (fmt.unit(MeasureUnit.get_metric_ton())
            .format_int(1)
            .to_string()) == "1 t"

    assert (fmt.unit(MeasureUnit.get_microgram())
            .format_int(1)
            .to_string()) in ("1 \xb5g", "1 \u03bcg")  # 1 µg

    assert (fmt.unit(MeasureUnit.get_micrometer())
            .format_int(1)
            .to_string()) in ("1 \xb5m", "1 \u03bcm")  # 1 µm

    assert (fmt.unit(MeasureUnit.get_microsecond())
            .format_int(1)
            .to_string()) in ("1 \xb5s", "1 \u03bcs")  # 1 µs

    assert (fmt.unit(MeasureUnit.get_mile())
            .format_int(1)
            .to_string()) == "1 mi"

    assert (fmt.unit(MeasureUnit.get_mile_per_gallon())
            .format_int(1)
            .to_string()) == "1 mpg"

    assert (fmt.unit(MeasureUnit.get_mile_per_gallon_imperial())
            .format_int(1)
            .to_string()) == "1 mpg Imp."

    assert (fmt.unit(MeasureUnit.get_mile_per_hour())
            .format_int(1)
            .to_string()) == "1 mph"

    assert (fmt.unit(MeasureUnit.get_mile_scandinavian())
            .format_int(1)
            .to_string()) == "1 smi"

    assert (fmt.unit(MeasureUnit.get_milliampere())
            .format_int(1)
            .to_string()) == "1 mA"

    assert (fmt.unit(MeasureUnit.get_millibar())
            .format_int(1)
            .to_string()) == "1 mbar"

    assert (fmt.unit(MeasureUnit.get_milligram())
            .format_int(1)
            .to_string()) == "1 mg"

    try:
        assert (fmt.unit(MeasureUnit.get_milligram_per_deciliter())
                .format_int(1)
                .to_string()) == "1 mg/dL"
    except ICUError as ex:
        assert ex.args[0] == UErrorCode.U_MISSING_RESOURCE_ERROR  # ICU 69.1

    assert (fmt.unit(MeasureUnit.get_milliliter())
            .format_int(1)
            .to_string()) == "1 mL"

    assert (fmt.unit(MeasureUnit.get_millimeter())
            .format_int(1)
            .to_string()) == "1 mm"

    assert (fmt.unit(MeasureUnit.get_millimeter_of_mercury())
            .format_int(1)
            .to_string()) == "1 mmHg"

    assert (fmt.unit(MeasureUnit.get_millimole_per_liter())
            .format_int(1)
            .to_string()) == "1 mmol/L"

    assert (fmt.unit(MeasureUnit.get_millisecond())
            .format_int(1)
            .to_string()) == "1 ms"

    assert (fmt.unit(MeasureUnit.get_milliwatt())
            .format_int(1)
            .to_string()) == "1 mW"

    assert (fmt.unit(MeasureUnit.get_minute())
            .format_int(1)
            .to_string()) == "1 min"

    assert (fmt.unit(MeasureUnit.get_mole())
            .format_int(1)
            .to_string()) == "1 mol"

    assert (fmt.unit(MeasureUnit.get_month())
            .format_int(1)
            .to_string()) == "1 mth"

    assert (fmt.unit(MeasureUnit.get_month_person())
            .format_int(1)
            .to_string()) == "1 mth"

    assert (fmt.unit(MeasureUnit.get_nanometer())
            .format_int(1)
            .to_string()) == "1 nm"

    assert (fmt.unit(MeasureUnit.get_nanosecond())
            .format_int(1)
            .to_string()) == "1 ns"

    assert (fmt.unit(MeasureUnit.get_nautical_mile())
            .format_int(1)
            .to_string()) == "1 nmi"

    assert (fmt.unit(MeasureUnit.get_newton())
            .format_int(1)
            .to_string()) == "1 N"

    assert (fmt.unit(MeasureUnit.get_newton_meter())
            .format_int(1)
            .to_string()) == "1 N\u22c5m"  # 1 N⋅m

    assert (fmt.unit(MeasureUnit.get_ohm())
            .format_int(1)
            .to_string()) == "1 \u03a9"  # 1 Ω

    assert (fmt.unit(MeasureUnit.get_ounce())
            .format_int(1)
            .to_string()) == "1 oz"

    assert (fmt.unit(MeasureUnit.get_ounce_troy())
            .format_int(1)
            .to_string()) == "1 oz t"

    assert (fmt.unit(MeasureUnit.get_parsec())
            .format_int(1)
            .to_string()) == "1 pc"

    assert (fmt.unit(MeasureUnit.get_part_per_million())
            .format_int(1)
            .to_string()) == "1 ppm"

    assert (fmt.unit(MeasureUnit.get_percent())
            .format_int(1)
            .to_string()) == "1%"

    assert (fmt.unit(MeasureUnit.get_permille())
            .format_int(1)
            .to_string()) == "1\u2030"  # 1‰

    assert (fmt.unit(MeasureUnit.get_permyriad())
            .format_int(1)
            .to_string()) == "1\u2031"  # 1‱

    assert (fmt.unit(MeasureUnit.get_petabyte())
            .format_int(1)
            .to_string()) == "1 PB"

    assert (fmt.unit(MeasureUnit.get_picometer())
            .format_int(1)
            .to_string()) == "1 pm"

    assert (fmt.unit(MeasureUnit.get_pint())
            .format_int(1)
            .to_string()) == "1 pt"

    assert (fmt.unit(MeasureUnit.get_pint_metric())
            .format_int(1)
            .to_string()) == "1 mpt"

    assert (fmt.unit(MeasureUnit.get_point())
            .format_int(1)
            .to_string()) == "1 pt"

    assert (fmt.unit(MeasureUnit.get_pound())
            .format_int(1)
            .to_string()) == "1 lb"

    assert (fmt.unit(MeasureUnit.get_pound_foot())
            .format_int(1)
            .to_string()) == "1 lbf\u22c5ft"  # 1 lb⋅ft

    assert (fmt.unit(MeasureUnit.get_pound_force())
            .format_int(1)
            .to_string()) == "1 lbf"

    assert (fmt.unit(MeasureUnit.get_pound_per_square_inch())
            .format_int(1)
            .to_string()) == "1 psi"

    assert (fmt.unit(MeasureUnit.get_quart())
            .format_int(1)
            .to_string()) == "1 qt"

    assert (fmt.unit(MeasureUnit.get_radian())
            .format_int(1)
            .to_string()) == "1 rad"

    assert (fmt.unit(MeasureUnit.get_revolution_angle())
            .format_int(1)
            .to_string()) == "1 rev"

    assert (fmt.unit(MeasureUnit.get_second())
            .format_int(1)
            .to_string()) == "1 sec"

    assert (fmt.unit(MeasureUnit.get_solar_luminosity())
            .format_int(1)
            .to_string()) == "1 L\u2609"  # 1 L☉

    assert (fmt.unit(MeasureUnit.get_solar_mass())
            .format_int(1)
            .to_string()) == "1 M\u2609"  # 1 M☉

    assert (fmt.unit(MeasureUnit.get_solar_radius())
            .format_int(1)
            .to_string()) == "1 R\u2609"  # 1 R☉

    assert (fmt.unit(MeasureUnit.get_square_centimeter())
            .format_int(1)
            .to_string()) == "1 cm\xb2"  # 1 cm²

    assert (fmt.unit(MeasureUnit.get_square_foot())
            .format_int(1)
            .to_string()) == "1 sq ft"

    assert (fmt.unit(MeasureUnit.get_square_inch())
            .format_int(1)
            .to_string()) == "1 in\xb2"  # 1 in²

    assert (fmt.unit(MeasureUnit.get_square_kilometer())
            .format_int(1)
            .to_string()) == "1 km\xb2"  # 1 km²

    assert (fmt.unit(MeasureUnit.get_square_meter())
            .format_int(1)
            .to_string()) == "1 m\xb2"  # 1 m²

    assert (fmt.unit(MeasureUnit.get_square_mile())
            .format_int(1)
            .to_string()) == "1 sq mi"

    assert (fmt.unit(MeasureUnit.get_square_yard())
            .format_int(1)
            .to_string()) == "1 yd\xb2"  # 1 yd²

    assert (fmt.unit(MeasureUnit.get_stone())
            .format_int(1)
            .to_string()) == "1 st"

    assert (fmt.unit(MeasureUnit.get_tablespoon())
            .format_int(1)
            .to_string()) == "1 tbsp"

    assert (fmt.unit(MeasureUnit.get_teaspoon())
            .format_int(1)
            .to_string()) == "1 tsp"

    assert (fmt.unit(MeasureUnit.get_terabit())
            .format_int(1)
            .to_string()) == "1 Tb"

    assert (fmt.unit(MeasureUnit.get_terabyte())
            .format_int(1)
            .to_string()) == "1 TB"

    assert (fmt.unit(MeasureUnit.get_ton())
            .format_int(1)
            .to_string()) == "1 tn"

    assert (fmt.unit(MeasureUnit.get_volt())
            .format_int(1)
            .to_string()) == "1 V"

    assert (fmt.unit(MeasureUnit.get_watt())
            .format_int(1)
            .to_string()) == "1 W"

    assert (fmt.unit(MeasureUnit.get_week())
            .format_int(1)
            .to_string()) == "1 wk"

    assert (fmt.unit(MeasureUnit.get_week_person())
            .format_int(1)
            .to_string()) == "1 wk"

    assert (fmt.unit(MeasureUnit.get_yard())
            .format_int(1)
            .to_string()) == "1 yd"

    assert (fmt.unit(MeasureUnit.get_year())
            .format_int(1)
            .to_string()) == "1 yr"

    assert (fmt.unit(MeasureUnit.get_year_person())
            .format_int(1)
            .to_string()) == "1 yr"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 65, reason="ICU4C<65")
def test_measure_unit_65():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_bar())
            .format_int(1)
            .to_string()) == "1 bar"

    assert (fmt.unit(MeasureUnit.create_decade())
            .format_int(1)
            .to_string()) == "1 dec"

    assert (fmt.unit(MeasureUnit.create_dot_per_centimeter())
            .format_int(1)
            .to_string()) == "1 dpcm"

    assert (fmt.unit(MeasureUnit.create_dot_per_inch())
            .format_int(1)
            .to_string()) == "1 dpi"

    assert (fmt.unit(MeasureUnit.create_em())
            .format_int(1)
            .to_string()) == "1 em"

    assert (fmt.unit(MeasureUnit.create_megapixel())
            .format_int(1)
            .to_string()) == "1 MP"

    assert (fmt.unit(MeasureUnit.create_pascal())
            .format_int(1)
            .to_string()) == "1 Pa"

    assert (fmt.unit(MeasureUnit.create_pixel())
            .format_int(1)
            .to_string()) == "1 px"

    assert (fmt.unit(MeasureUnit.create_pixel_per_centimeter())
            .format_int(1)
            .to_string()) == "1 ppcm"

    assert (fmt.unit(MeasureUnit.create_pixel_per_inch())
            .format_int(1)
            .to_string()) == "1 ppi"

    assert (fmt.unit(MeasureUnit.create_therm_us())
            .format_int(1)
            .to_string()) == "1 US therm"

    assert (fmt.unit(MeasureUnit.get_bar())
            .format_int(1)
            .to_string()) == "1 bar"

    assert (fmt.unit(MeasureUnit.get_decade())
            .format_int(1)
            .to_string()) == "1 dec"

    assert (fmt.unit(MeasureUnit.get_dot_per_centimeter())
            .format_int(1)
            .to_string()) == "1 dpcm"

    assert (fmt.unit(MeasureUnit.get_dot_per_inch())
            .format_int(1)
            .to_string()) == "1 dpi"

    assert (fmt.unit(MeasureUnit.get_em())
            .format_int(1)
            .to_string()) == "1 em"

    assert (fmt.unit(MeasureUnit.get_megapixel())
            .format_int(1)
            .to_string()) == "1 MP"

    assert (fmt.unit(MeasureUnit.get_pascal())
            .format_int(1)
            .to_string()) == "1 Pa"

    assert (fmt.unit(MeasureUnit.get_pixel())
            .format_int(1)
            .to_string()) == "1 px"

    assert (fmt.unit(MeasureUnit.get_pixel_per_centimeter())
            .format_int(1)
            .to_string()) == "1 ppcm"

    assert (fmt.unit(MeasureUnit.get_pixel_per_inch())
            .format_int(1)
            .to_string()) == "1 ppi"

    assert (fmt.unit(MeasureUnit.get_therm_us())
            .format_int(1)
            .to_string()) == "1 US therm"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_measure_unit_68():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_candela())
            .format_int(1)
            .to_string()) == "1 cd"

    assert (fmt.unit(MeasureUnit.create_dessert_spoon())
            .format_int(1)
            .to_string()) == "1 dstspn"

    assert (fmt.unit(MeasureUnit.create_dessert_spoon_imperial())
            .format_int(1)
            .to_string()) == "1 dstspn Imp"

    assert (fmt.unit(MeasureUnit.create_dot())
            .format_int(1)
            .to_string()) in ("1 dot",  # ICU4C<69
                              "1 px")  # ICU4C>=69

    assert (fmt.unit(MeasureUnit.create_dram())
            .format_int(1)
            .to_string()) == "1 dram fl"

    assert (fmt.unit(MeasureUnit.create_drop())
            .format_int(1)
            .to_string()) == "1 drop"

    assert (fmt.unit(MeasureUnit.create_earth_radius())
            .format_int(1)
            .to_string()) == "1 R\u2295"  # 1 R⊕

    assert (fmt.unit(MeasureUnit.create_grain())
            .format_int(1)
            .to_string()) == "1 grain"

    assert (fmt.unit(MeasureUnit.create_jigger())
            .format_int(1)
            .to_string()) == "1 jigger"

    assert (fmt.unit(MeasureUnit.create_lumen())
            .format_int(1)
            .to_string()) == "1 lm"

    assert (fmt.unit(MeasureUnit.create_pinch())
            .format_int(1)
            .to_string()) == "1 pinch"

    assert (fmt.unit(MeasureUnit.create_quart_imperial())
            .format_int(1)
            .to_string()) == "1 qt Imp."

    assert (fmt.unit(MeasureUnit.get_candela())
            .format_int(1)
            .to_string()) == "1 cd"

    assert (fmt.unit(MeasureUnit.get_dessert_spoon())
            .format_int(1)
            .to_string()) == "1 dstspn"

    assert (fmt.unit(MeasureUnit.get_dessert_spoon_imperial())
            .format_int(1)
            .to_string()) == "1 dstspn Imp"

    assert (fmt.unit(MeasureUnit.get_dot())
            .format_int(1)
            .to_string()) in ("1 dot",  # ICU4C<69
                              "1 px")  # ICU4C>=69

    assert (fmt.unit(MeasureUnit.get_dram())
            .format_int(1)
            .to_string()) == "1 dram fl"

    assert (fmt.unit(MeasureUnit.get_drop())
            .format_int(1)
            .to_string()) == "1 drop"

    assert (fmt.unit(MeasureUnit.get_earth_radius())
            .format_int(1)
            .to_string()) == "1 R\u2295"  # 1 R⊕

    assert (fmt.unit(MeasureUnit.get_grain())
            .format_int(1)
            .to_string()) == "1 grain"

    assert (fmt.unit(MeasureUnit.get_jigger())
            .format_int(1)
            .to_string()) == "1 jigger"

    assert (fmt.unit(MeasureUnit.get_lumen())
            .format_int(1)
            .to_string()) == "1 lm"

    assert (fmt.unit(MeasureUnit.get_pinch())
            .format_int(1)
            .to_string()) == "1 pinch"

    assert (fmt.unit(MeasureUnit.get_quart_imperial())
            .format_int(1)
            .to_string()) == "1 qt Imp."


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_measure_unit_69():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    assert (fmt.unit(MeasureUnit.create_milligram_of_glucose_per_deciliter())
            .format_int(1)
            .to_string()) == "1 mg/dL"

    assert (fmt.unit(MeasureUnit.get_milligram_of_glucose_per_deciliter())
            .format_int(1)
            .to_string()) == "1 mg/dL"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_measure_unit_70():
    fmt = NumberFormatter.with_locale(Locale.get_us())

    # static MeasureUnit *icu::MeasureUnit::createItem(UErrorCode &status)
    assert (fmt.unit(MeasureUnit.create_item())
            .format_int(1)
            .to_string()) == "1 item"

    # static MeasureUnit *icu::MeasureUnit::createKilowattHourPer100Kilometer(
    #       UErrorCode &status
    # )
    assert (fmt.unit(MeasureUnit.create_kilowatt_hour_per_100_kilometer())
            .format_int(1)
            .to_string()) == "1 kWh/100km"

    # static MeasureUnit icu::MeasureUnit::getItem()
    assert (fmt.unit(MeasureUnit.get_item())
            .format_int(1)
            .to_string()) == "1 item"

    # static MeasureUnit icu::MeasureUnit::getKilowattHourPer100Kilometer()
    assert (fmt.unit(MeasureUnit.get_kilowatt_hour_per_100_kilometer())
            .format_int(1)
            .to_string()) == "1 kWh/100km"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_product():
    # MeasureUnit icu::MeasureUnit::product(
    #       const MeasureUnit &other,
    #       UErrorCode &status
    # )
    unit = MeasureUnit.for_identifier("kilowatt")
    unit2 = unit.product(MeasureUnit.for_identifier("hour-per-day"))
    assert isinstance(unit2, MeasureUnit)
    assert unit2.get_identifier() in ("hour-kilowatt-per-day",  # ICU4C<69
                                      "kilowatt-hour-per-day")  # ICU4C>=69


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_reciprocal():
    # MeasureUnit icu::MeasureUnit::reciprocal(UErrorCode &status)
    unit = MeasureUnit.for_identifier("meter-per-second")
    unit2 = unit.reciprocal()
    assert isinstance(unit2, MeasureUnit)
    assert unit2.get_identifier() == "second-per-meter"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_split_to_single_units():
    unit = MeasureUnit.create_newton_meter()

    # std::pair<LocalArray<MeasureUnit>, int32_t>
    # icu::MeasureUnit::splitToSingleUnits(UErrorCode &status)
    units = unit.split_to_single_units()
    assert isinstance(units, list)
    assert len(units) == 2
    assert all(isinstance(x, MeasureUnit) for x in units)
    assert units[0].get_identifier() == "newton"
    assert units[1].get_identifier() == "meter"

    unit = MeasureUnit.create_kilometer()
    units = unit.split_to_single_units()
    assert isinstance(units, list)
    assert len(units) == 1
    assert all(isinstance(x, MeasureUnit) for x in units)
    assert units[0].get_identifier() == "kilometer"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_with_dimensionality():
    unit = MeasureUnit.create_kilometer()

    # MeasureUnit icu::MeasureUnit::withDimensionality(
    #       int32_t dimensionality,
    #       UErrorCode &status
    # )
    unit2 = unit.with_dimensionality(2)
    assert isinstance(unit2, MeasureUnit)
    assert unit2.get_identifier() == "square-kilometer"

    unit3 = unit.with_dimensionality(3)
    assert isinstance(unit3, MeasureUnit)
    assert unit3.get_identifier() == "cubic-kilometer"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_with_prefix():
    from icupy.icu import UMeasurePrefix

    unit = MeasureUnit.create_byte()

    # MeasureUnit icu::MeasureUnit::withPrefix(
    #       UMeasurePrefix prefix,
    #       UErrorCode &status
    # )
    unit2 = unit.with_prefix(UMeasurePrefix.UMEASURE_PREFIX_KILO)
    assert isinstance(unit2, MeasureUnit)
    assert unit2.get_identifier() == "kilobyte"

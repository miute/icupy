#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/measunit.h>
#include <unicode/strenum.h>

using namespace icu;

void init_measunit(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  //
  // UMeasurePrefix
  //
  py::enum_<UMeasurePrefix>(m, "UMeasurePrefix", py::arithmetic(),
                            "Enumeration for SI and binary prefixes, e.g. "
                            "\"kilo-\", \"nano-\", \"mebi-\".\n\n"
                            "Enum values should be treated as opaque: use *umeas_get_prefix_power()* and "
                            "*umeas_get_prefix_base()* to find their corresponding values.")
      .value("UMEASURE_PREFIX_ONE", UMEASURE_PREFIX_ONE,
             "The absence of an SI or binary prefix.\n\n  "
             "The integer representation of this enum value is an arbitrary implementation detail and should not be "
             "relied upon: use *umeas_get_prefix_power()* to obtain meaningful values.")
      .value("UMEASURE_PREFIX_YOTTA", UMEASURE_PREFIX_YOTTA, "SI prefix: yotta, 10^24.")
      .value("UMEASURE_PREFIX_ZETTA", UMEASURE_PREFIX_ZETTA, "SI prefix: zetta, 10^21.")
      .value("UMEASURE_PREFIX_EXA", UMEASURE_PREFIX_EXA, "SI prefix: exa, 10^18.")
      .value("UMEASURE_PREFIX_PETA", UMEASURE_PREFIX_PETA, "SI prefix: peta, 10^15.")
      .value("UMEASURE_PREFIX_TERA", UMEASURE_PREFIX_TERA, "SI prefix: tera, 10^12.")
      .value("UMEASURE_PREFIX_GIGA", UMEASURE_PREFIX_GIGA, "SI prefix: giga, 10^9.")
      .value("UMEASURE_PREFIX_MEGA", UMEASURE_PREFIX_MEGA, "SI prefix: mega, 10^6.")
      .value("UMEASURE_PREFIX_KILO", UMEASURE_PREFIX_KILO, "SI prefix: kilo, 10^3.")
      .value("UMEASURE_PREFIX_HECTO", UMEASURE_PREFIX_HECTO, "SI prefix: hecto, 10^2.")
      .value("UMEASURE_PREFIX_DEKA", UMEASURE_PREFIX_DEKA, "SI prefix: deka, 10^1.")
      .value("UMEASURE_PREFIX_DECI", UMEASURE_PREFIX_DECI, "SI prefix: deci, 10^-1.")
      .value("UMEASURE_PREFIX_CENTI", UMEASURE_PREFIX_CENTI, "SI prefix: centi, 10^-2.")
      .value("UMEASURE_PREFIX_MILLI", UMEASURE_PREFIX_MILLI, "SI prefix: milli, 10^-3.")
      .value("UMEASURE_PREFIX_MICRO", UMEASURE_PREFIX_MICRO, "SI prefix: micro, 10^-6.")
      .value("UMEASURE_PREFIX_NANO", UMEASURE_PREFIX_NANO, "SI prefix: nano, 10^-9.")
      .value("UMEASURE_PREFIX_PICO", UMEASURE_PREFIX_PICO, "SI prefix: pico, 10^-12.")
      .value("UMEASURE_PREFIX_FEMTO", UMEASURE_PREFIX_FEMTO, "SI prefix: femto, 10^-15.")
      .value("UMEASURE_PREFIX_ATTO", UMEASURE_PREFIX_ATTO, "SI prefix: atto, 10^-18.")
      .value("UMEASURE_PREFIX_ZEPTO", UMEASURE_PREFIX_ZEPTO, "SI prefix: zepto, 10^-21.")
      .value("UMEASURE_PREFIX_YOCTO", UMEASURE_PREFIX_YOCTO, "SI prefix: yocto, 10^-24.")
      .value("UMEASURE_PREFIX_KIBI", UMEASURE_PREFIX_KIBI, "Binary prefix: kibi, 1024^1.")
      .value("UMEASURE_PREFIX_MEBI", UMEASURE_PREFIX_MEBI, "Binary prefix: mebi, 1024^2.")
      .value("UMEASURE_PREFIX_GIBI", UMEASURE_PREFIX_GIBI, "Binary prefix: gibi, 1024^3.")
      .value("UMEASURE_PREFIX_TEBI", UMEASURE_PREFIX_TEBI, "Binary prefix: tebi, 1024^4.")
      .value("UMEASURE_PREFIX_PEBI", UMEASURE_PREFIX_PEBI, "Binary prefix: pebi, 1024^5.")
      .value("UMEASURE_PREFIX_EXBI", UMEASURE_PREFIX_EXBI, "Binary prefix: exbi, 1024^6.")
      .value("UMEASURE_PREFIX_ZEBI", UMEASURE_PREFIX_ZEBI, "Binary prefix: zebi, 1024^7.")
      .value("UMEASURE_PREFIX_YOBI", UMEASURE_PREFIX_YOBI, "Binary prefix: yobi, 1024^8.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  //
  // UMeasureUnitComplexity
  //
  py::enum_<UMeasureUnitComplexity>(m, "UMeasureUnitComplexity", py::arithmetic(),
                                    "Enumeration for unit complexity.\n\n"
                                    "There are three levels:\n\n"
                                    "* SINGLE: A single unit, optionally with a power and/or SI or binary prefix. "
                                    "Examples: hectare, square-kilometer, kilojoule, per-second, mebibyte.\n"
                                    "* COMPOUND: A unit composed of the product of multiple single units. Examples: "
                                    "meter-per-second, kilowatt-hour, kilogram-meter-per-square-second.\n"
                                    "* MIXED: A unit composed of the sum of multiple single units. Examples: "
                                    "foot+inch, hour+minute+second, degree+arcminute+arcsecond.\n\n"
                                    "The complexity determines which operations are available. For example, you cannot "
                                    "set the power or prefix of a compound unit.")
      .value("UMEASURE_UNIT_SINGLE", UMEASURE_UNIT_SINGLE, "A single unit, like kilojoule.")
      .value("UMEASURE_UNIT_COMPOUND", UMEASURE_UNIT_COMPOUND, "A compound unit, like meter-per-second.")
      .value("UMEASURE_UNIT_MIXED", UMEASURE_UNIT_MIXED, "A mixed unit, like hour+minute.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  //
  // icu::MeasureUnit
  //
  py::class_<MeasureUnit, UObject> mu(m, "MeasureUnit");

  mu.def(py::init<const MeasureUnit &>(), py::arg("other"));

  mu.def("__copy__", &MeasureUnit::clone);

  mu.def(
      "__deepcopy__", [](const MeasureUnit &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  mu.def(
      "__eq__", [](const MeasureUnit &self, const UObject &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def(
      "__ne__", [](const MeasureUnit &self, const UObject &other) { return self != other; }, py::is_operator(),
      py::arg("other"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  mu.def("clone", &MeasureUnit::clone);

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_acre", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createAcre(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_acre_foot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createAcreFoot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_ampere", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createAmpere(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_arc_minute", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createArcMinute(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_arc_second", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createArcSecond(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_astronomical_unit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createAstronomicalUnit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  mu.def_static("create_atmosphere", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createAtmosphere(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_bar", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createBar(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_barrel", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createBarrel(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  mu.def_static("create_beaufort", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createBeaufort(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_bit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createBit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_british_thermal_unit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createBritishThermalUnit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_bushel", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createBushel(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_byte", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createByte(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_calorie", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCalorie(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_candela", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCandela(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_carat", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCarat(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_celsius", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCelsius(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_centiliter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCentiliter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_centimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCentimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_century", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCentury(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_cubic_centimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicCentimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_cubic_foot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicFoot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_cubic_inch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicInch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_cubic_kilometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicKilometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_cubic_meter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicMeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_cubic_mile", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicMile(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_cubic_yard", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCubicYard(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_cup", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCup(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_cup_metric", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createCupMetric(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_dalton", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDalton(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_day", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDay(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_day_person", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDayPerson(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_decade", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDecade(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_deciliter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDeciliter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_decimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDecimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_degree", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDegree(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_dessert_spoon", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDessertSpoon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_dessert_spoon_imperial", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDessertSpoonImperial(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_dot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_dot_per_centimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDotPerCentimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_dot_per_inch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDotPerInch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_dram", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDram(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_drop", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDrop(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_dunam", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createDunam(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_earth_mass", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createEarthMass(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_earth_radius", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createEarthRadius(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_electronvolt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createElectronvolt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_em", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createEm(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_fahrenheit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFahrenheit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_fathom", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFathom(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_fluid_ounce", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFluidOunce(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_fluid_ounce_imperial", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFluidOunceImperial(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_foodcalorie", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFoodcalorie(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_foot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFoot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_furlong", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createFurlong(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_gallon", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGallon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  mu.def_static("create_gallon_imperial", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGallonImperial(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 74)
  mu.def_static("create_gasoline_energy_density", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGasolineEnergyDensity(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 74)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_generic_temperature", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGenericTemperature(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_gforce", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGForce(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_gigabit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGigabit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_gigabyte", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGigabyte(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_gigahertz", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGigahertz(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_gigawatt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGigawatt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_grain", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGrain(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_gram", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createGram(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_hectare", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createHectare(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_hectoliter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createHectoliter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_hectopascal", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createHectopascal(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_hertz", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createHertz(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_horsepower", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createHorsepower(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_hour", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createHour(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_inch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createInch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_inch_hg", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createInchHg(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("create_item", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createItem(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_jigger", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createJigger(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_joule", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createJoule(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_karat", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKarat(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_kelvin", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKelvin(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_kilobit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilobit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_kilobyte", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilobyte(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_kilocalorie", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilocalorie(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_kilogram", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilogram(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_kilohertz", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilohertz(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_kilojoule", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilojoule(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_kilometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_kilometer_per_hour", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilometerPerHour(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_kilopascal", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilopascal(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_kilowatt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilowatt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_kilowatt_hour", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilowattHour(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("create_kilowatt_hour_per_100_kilometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKilowattHourPer100Kilometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_knot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createKnot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_light_year", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createLightYear(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_liter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createLiter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_liter_per_100_kilometers", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createLiterPer100Kilometers(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_liter_per_kilometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createLiterPerKilometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_lumen", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createLumen(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_lux", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createLux(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_megabit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegabit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_megabyte", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegabyte(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_megahertz", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegahertz(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_megaliter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegaliter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_megapascal", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegapascal(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_megapixel", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegapixel(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_megawatt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMegawatt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_meter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_meter_per_second", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMeterPerSecond(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_meter_per_second_squared", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMeterPerSecondSquared(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_metric_ton", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMetricTon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_microgram", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMicrogram(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_micrometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMicrometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_microsecond", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMicrosecond(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_mile", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMile(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_mile_per_gallon", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilePerGallon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  mu.def_static("create_mile_per_gallon_imperial", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilePerGallonImperial(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_mile_per_hour", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilePerHour(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_mile_scandinavian", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMileScandinavian(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_milliampere", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilliampere(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_millibar", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMillibar(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_milligram", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilligram(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  mu.def_static("create_milligram_of_glucose_per_deciliter", // Not a typo
                []() {
                  ErrorCode error_code;
                  auto result = MeasureUnit::createMilligramOfglucosePerDeciliter(error_code);
                  if (error_code.isFailure()) {
                    throw icupy::ICUError(error_code);
                  }
                  return result;
                })
      .def_static("create_milligram_ofglucose_per_deciliter", []() {
        ErrorCode error_code;
        auto result = MeasureUnit::createMilligramOfglucosePerDeciliter(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  mu.def_static("create_milligram_per_deciliter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilligramPerDeciliter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_milliliter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilliliter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_millimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMillimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_millimeter_of_mercury", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMillimeterOfMercury(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  mu.def_static("create_millimole_per_liter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMillimolePerLiter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_millisecond", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMillisecond(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_milliwatt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMilliwatt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_minute", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMinute(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_mole", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMole(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_month", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMonth(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_month_person", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createMonthPerson(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_nanometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createNanometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_nanosecond", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createNanosecond(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_nautical_mile", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createNauticalMile(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_newton", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createNewton(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_newton_meter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createNewtonMeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_ohm", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createOhm(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_ounce", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createOunce(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_ounce_troy", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createOunceTroy(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_parsec", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createParsec(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  mu.def_static("create_part_per_million", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPartPerMillion(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_pascal", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPascal(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  mu.def_static("create_percent", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPercent(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_permille", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPermille(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_permyriad", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPermyriad(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  mu.def_static("create_petabyte", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPetabyte(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_picometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPicometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_pinch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPinch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_pint", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPint(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_pint_metric", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPintMetric(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_pixel", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPixel(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_pixel_per_centimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPixelPerCentimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_pixel_per_inch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPixelPerInch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  mu.def_static("create_point", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPoint(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_pound", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPound(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_pound_foot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPoundFoot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_pound_force", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPoundForce(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_pound_per_square_inch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createPoundPerSquareInch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_quart", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createQuart(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  mu.def_static("create_quarter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createQuarter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_quart_imperial", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createQuartImperial(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_radian", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createRadian(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_revolution_angle", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createRevolutionAngle(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_second", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSecond(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_solar_luminosity", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSolarLuminosity(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_solar_mass", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSolarMass(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_solar_radius", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSolarRadius(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_square_centimeter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareCentimeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_square_foot", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareFoot(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_square_inch", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareInch(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_square_kilometer", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareKilometer(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_square_meter", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareMeter(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_square_mile", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareMile(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_square_yard", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createSquareYard(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_stone", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createStone(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_tablespoon", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createTablespoon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_teaspoon", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createTeaspoon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_terabit", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createTerabit(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_terabyte", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createTerabyte(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_therm_us", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createThermUs(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_ton", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createTon(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  mu.def_static("create_tonne", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createTonne(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_volt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createVolt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_watt", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createWatt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_week", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createWeek(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_week_person", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createWeekPerson(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_yard", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createYard(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  mu.def_static("create_year", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createYear(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_year_person", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::createYearPerson(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def_static(
      "for_identifier",
      [](const char *identifier) {
        ErrorCode error_code;
        auto result = MeasureUnit::forIdentifier(identifier, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("identifier"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_acre", &MeasureUnit::getAcre);

  mu.def_static("get_acre_foot", &MeasureUnit::getAcreFoot);

  mu.def_static("get_ampere", &MeasureUnit::getAmpere);

  mu.def_static("get_arc_minute", &MeasureUnit::getArcMinute);

  mu.def_static("get_arc_second", &MeasureUnit::getArcSecond);

  mu.def_static("get_astronomical_unit", &MeasureUnit::getAstronomicalUnit);

  mu.def_static("get_atmosphere", &MeasureUnit::getAtmosphere);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static(
        "get_available",
        [](const char *type) {
          ErrorCode error_code;
          int32_t capacity = MeasureUnit::getAvailable(type, nullptr, 0, error_code);
          auto result = std::vector<MeasureUnit>(capacity);
          error_code.reset();
          MeasureUnit::getAvailable(type, result.data(), capacity, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("type_"))
      .def_static("get_available", []() {
        ErrorCode error_code;
        int32_t capacity = MeasureUnit::getAvailable(nullptr, 0, error_code);
        auto result = std::vector<MeasureUnit>(capacity);
        error_code.reset();
        MeasureUnit::getAvailable(result.data(), capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  mu.def_static("get_available_types", []() {
    ErrorCode error_code;
    auto result = MeasureUnit::getAvailableTypes(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_bar", &MeasureUnit::getBar);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_barrel", &MeasureUnit::getBarrel);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 73)
  mu.def_static("get_beaufort", &MeasureUnit::getBeaufort);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_bit", &MeasureUnit::getBit);

  mu.def_static("get_british_thermal_unit", &MeasureUnit::getBritishThermalUnit);

  mu.def_static("get_bushel", &MeasureUnit::getBushel);

  mu.def_static("get_byte", &MeasureUnit::getByte);

  mu.def_static("get_calorie", &MeasureUnit::getCalorie);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_candela", &MeasureUnit::getCandela);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_carat", &MeasureUnit::getCarat);

  mu.def_static("get_celsius", &MeasureUnit::getCelsius);

  mu.def_static("get_centiliter", &MeasureUnit::getCentiliter);

  mu.def_static("get_centimeter", &MeasureUnit::getCentimeter);

  mu.def_static("get_century", &MeasureUnit::getCentury);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def("get_complexity", [](const MeasureUnit &self) {
    ErrorCode error_code;
    auto result = self.getComplexity(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_cubic_centimeter", &MeasureUnit::getCubicCentimeter);

  mu.def_static("get_cubic_foot", &MeasureUnit::getCubicFoot);

  mu.def_static("get_cubic_inch", &MeasureUnit::getCubicInch);

  mu.def_static("get_cubic_kilometer", &MeasureUnit::getCubicKilometer);

  mu.def_static("get_cubic_meter", &MeasureUnit::getCubicMeter);

  mu.def_static("get_cubic_mile", &MeasureUnit::getCubicMile);

  mu.def_static("get_cubic_yard", &MeasureUnit::getCubicYard);

  mu.def_static("get_cup", &MeasureUnit::getCup);

  mu.def_static("get_cup_metric", &MeasureUnit::getCupMetric);

  mu.def_static("get_dalton", &MeasureUnit::getDalton);

  mu.def_static("get_day", &MeasureUnit::getDay);

  mu.def_static("get_day_person", &MeasureUnit::getDayPerson);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_decade", &MeasureUnit::getDecade);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_deciliter", &MeasureUnit::getDeciliter);

  mu.def_static("get_decimeter", &MeasureUnit::getDecimeter);

  mu.def_static("get_degree", &MeasureUnit::getDegree);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_dessert_spoon", &MeasureUnit::getDessertSpoon);

  mu.def_static("get_dessert_spoon_imperial", &MeasureUnit::getDessertSpoonImperial);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def("get_dimensionality", [](const MeasureUnit &self) {
    ErrorCode error_code;
    auto result = self.getDimensionality(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_dot", &MeasureUnit::getDot);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_dot_per_centimeter", &MeasureUnit::getDotPerCentimeter);

  mu.def_static("get_dot_per_inch", &MeasureUnit::getDotPerInch);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_dram", &MeasureUnit::getDram);

  mu.def_static("get_drop", &MeasureUnit::getDrop);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_dunam", &MeasureUnit::getDunam);

  mu.def_static("get_earth_mass", &MeasureUnit::getEarthMass);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_earth_radius", &MeasureUnit::getEarthRadius);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_electronvolt", &MeasureUnit::getElectronvolt);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_em", &MeasureUnit::getEm);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_fahrenheit", &MeasureUnit::getFahrenheit);

  mu.def_static("get_fathom", &MeasureUnit::getFathom);

  mu.def_static("get_fluid_ounce", &MeasureUnit::getFluidOunce);

  mu.def_static("get_fluid_ounce_imperial", &MeasureUnit::getFluidOunceImperial);

  mu.def_static("get_foodcalorie", &MeasureUnit::getFoodcalorie);

  mu.def_static("get_foot", &MeasureUnit::getFoot);

  mu.def_static("get_furlong", &MeasureUnit::getFurlong);

  mu.def_static("get_gallon", &MeasureUnit::getGallon);

  mu.def_static("get_gallon_imperial", &MeasureUnit::getGallonImperial);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 74)
  mu.def_static("get_gasoline_energy_density", &MeasureUnit::getGasolineEnergyDensity);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 74)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_generic_temperature", &MeasureUnit::getGenericTemperature);

  mu.def_static("get_gforce", &MeasureUnit::getGForce);

  mu.def_static("get_gigabit", &MeasureUnit::getGigabit);

  mu.def_static("get_gigabyte", &MeasureUnit::getGigabyte);

  mu.def_static("get_gigahertz", &MeasureUnit::getGigahertz);

  mu.def_static("get_gigawatt", &MeasureUnit::getGigawatt);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_grain", &MeasureUnit::getGrain);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_gram", &MeasureUnit::getGram);

  mu.def_static("get_hectare", &MeasureUnit::getHectare);

  mu.def_static("get_hectoliter", &MeasureUnit::getHectoliter);

  mu.def_static("get_hectopascal", &MeasureUnit::getHectopascal);

  mu.def_static("get_hertz", &MeasureUnit::getHertz);

  mu.def_static("get_horsepower", &MeasureUnit::getHorsepower);

  mu.def_static("get_hour", &MeasureUnit::getHour);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def("get_identifier", &MeasureUnit::getIdentifier);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_inch", &MeasureUnit::getInch);

  mu.def_static("get_inch_hg", &MeasureUnit::getInchHg);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("get_item", &MeasureUnit::getItem);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_jigger", &MeasureUnit::getJigger);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_joule", &MeasureUnit::getJoule);

  mu.def_static("get_karat", &MeasureUnit::getKarat);

  mu.def_static("get_kelvin", &MeasureUnit::getKelvin);

  mu.def_static("get_kilobit", &MeasureUnit::getKilobit);

  mu.def_static("get_kilobyte", &MeasureUnit::getKilobyte);

  mu.def_static("get_kilocalorie", &MeasureUnit::getKilocalorie);

  mu.def_static("get_kilogram", &MeasureUnit::getKilogram);

  mu.def_static("get_kilohertz", &MeasureUnit::getKilohertz);

  mu.def_static("get_kilojoule", &MeasureUnit::getKilojoule);

  mu.def_static("get_kilometer", &MeasureUnit::getKilometer);

  mu.def_static("get_kilometer_per_hour", &MeasureUnit::getKilometerPerHour);

  mu.def_static("get_kilopascal", &MeasureUnit::getKilopascal);

  mu.def_static("get_kilowatt", &MeasureUnit::getKilowatt);

  mu.def_static("get_kilowatt_hour", &MeasureUnit::getKilowattHour);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("get_kilowatt_hour_per_100_kilometer", &MeasureUnit::getKilowattHourPer100Kilometer);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_knot", &MeasureUnit::getKnot);

  mu.def_static("get_light_year", &MeasureUnit::getLightYear);

  mu.def_static("get_liter", &MeasureUnit::getLiter);

  mu.def_static("get_liter_per_100_kilometers", &MeasureUnit::getLiterPer100Kilometers);

  mu.def_static("get_liter_per_kilometer", &MeasureUnit::getLiterPerKilometer);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_lumen", &MeasureUnit::getLumen);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_lux", &MeasureUnit::getLux);

  mu.def_static("get_megabit", &MeasureUnit::getMegabit);

  mu.def_static("get_megabyte", &MeasureUnit::getMegabyte);

  mu.def_static("get_megahertz", &MeasureUnit::getMegahertz);

  mu.def_static("get_megaliter", &MeasureUnit::getMegaliter);

  mu.def_static("get_megapascal", &MeasureUnit::getMegapascal);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_megapixel", &MeasureUnit::getMegapixel);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_megawatt", &MeasureUnit::getMegawatt);

  mu.def_static("get_meter", &MeasureUnit::getMeter);

  mu.def_static("get_meter_per_second", &MeasureUnit::getMeterPerSecond);

  mu.def_static("get_meter_per_second_squared", &MeasureUnit::getMeterPerSecondSquared);

  mu.def_static("get_metric_ton", &MeasureUnit::getMetricTon);

  mu.def_static("get_microgram", &MeasureUnit::getMicrogram);

  mu.def_static("get_micrometer", &MeasureUnit::getMicrometer);

  mu.def_static("get_microsecond", &MeasureUnit::getMicrosecond);

  mu.def_static("get_mile", &MeasureUnit::getMile);

  mu.def_static("get_mile_per_gallon", &MeasureUnit::getMilePerGallon);

  mu.def_static("get_mile_per_gallon_imperial", &MeasureUnit::getMilePerGallonImperial);

  mu.def_static("get_mile_per_hour", &MeasureUnit::getMilePerHour);

  mu.def_static("get_mile_scandinavian", &MeasureUnit::getMileScandinavian);

  mu.def_static("get_milliampere", &MeasureUnit::getMilliampere);

  mu.def_static("get_millibar", &MeasureUnit::getMillibar);

  mu.def_static("get_milligram", &MeasureUnit::getMilligram);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  mu.def_static("get_milligram_of_glucose_per_deciliter", &MeasureUnit::getMilligramOfglucosePerDeciliter)
      .def_static("get_milligram_ofglucose_per_deciliter", &MeasureUnit::getMilligramOfglucosePerDeciliter);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_milligram_per_deciliter", &MeasureUnit::getMilligramPerDeciliter);

  mu.def_static("get_milliliter", &MeasureUnit::getMilliliter);

  mu.def_static("get_millimeter", &MeasureUnit::getMillimeter);

  mu.def_static("get_millimeter_of_mercury", &MeasureUnit::getMillimeterOfMercury);

  mu.def_static("get_millimole_per_liter", &MeasureUnit::getMillimolePerLiter);

  mu.def_static("get_millisecond", &MeasureUnit::getMillisecond);

  mu.def_static("get_milliwatt", &MeasureUnit::getMilliwatt);

  mu.def_static("get_minute", &MeasureUnit::getMinute);

  mu.def_static("get_mole", &MeasureUnit::getMole);

  mu.def_static("get_month", &MeasureUnit::getMonth);

  mu.def_static("get_month_person", &MeasureUnit::getMonthPerson);

  mu.def_static("get_nanometer", &MeasureUnit::getNanometer);

  mu.def_static("get_nanosecond", &MeasureUnit::getNanosecond);

  mu.def_static("get_nautical_mile", &MeasureUnit::getNauticalMile);

  mu.def_static("get_newton", &MeasureUnit::getNewton);

  mu.def_static("get_newton_meter", &MeasureUnit::getNewtonMeter);

  mu.def_static("get_ohm", &MeasureUnit::getOhm);

  mu.def_static("get_ounce", &MeasureUnit::getOunce);

  mu.def_static("get_ounce_troy", &MeasureUnit::getOunceTroy);

  mu.def_static("get_parsec", &MeasureUnit::getParsec);

  mu.def_static("get_part_per_million", &MeasureUnit::getPartPerMillion);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_pascal", &MeasureUnit::getPascal);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_percent", &MeasureUnit::getPercent);

  mu.def_static("get_permille", &MeasureUnit::getPermille);

  mu.def_static("get_permyriad", &MeasureUnit::getPermyriad);

  mu.def_static("get_petabyte", &MeasureUnit::getPetabyte);

  mu.def_static("get_picometer", &MeasureUnit::getPicometer);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_pinch", &MeasureUnit::getPinch);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_pint", &MeasureUnit::getPint);

  mu.def_static("get_pint_metric", &MeasureUnit::getPintMetric);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_pixel", &MeasureUnit::getPixel);

  mu.def_static("get_pixel_per_centimeter", &MeasureUnit::getPixelPerCentimeter);

  mu.def_static("get_pixel_per_inch", &MeasureUnit::getPixelPerInch);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_point", &MeasureUnit::getPoint);

  mu.def_static("get_pound", &MeasureUnit::getPound);

  mu.def_static("get_pound_foot", &MeasureUnit::getPoundFoot);

  mu.def_static("get_pound_force", &MeasureUnit::getPoundForce);

  mu.def_static("get_pound_per_square_inch", &MeasureUnit::getPoundPerSquareInch);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  mu.def("get_prefix", [](const MeasureUnit &self) {
    ErrorCode error_code;
    auto result = self.getPrefix(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_quart", &MeasureUnit::getQuart);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  mu.def_static("get_quarter", &MeasureUnit::getQuarter);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("get_quart_imperial", &MeasureUnit::getQuartImperial);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_radian", &MeasureUnit::getRadian);

  mu.def_static("get_revolution_angle", &MeasureUnit::getRevolutionAngle);

  mu.def_static("get_second", &MeasureUnit::getSecond);

  mu.def_static("get_solar_luminosity", &MeasureUnit::getSolarLuminosity);

  mu.def_static("get_solar_mass", &MeasureUnit::getSolarMass);

  mu.def_static("get_solar_radius", &MeasureUnit::getSolarRadius);

  mu.def_static("get_square_centimeter", &MeasureUnit::getSquareCentimeter);

  mu.def_static("get_square_foot", &MeasureUnit::getSquareFoot);

  mu.def_static("get_square_inch", &MeasureUnit::getSquareInch);

  mu.def_static("get_square_kilometer", &MeasureUnit::getSquareKilometer);

  mu.def_static("get_square_meter", &MeasureUnit::getSquareMeter);

  mu.def_static("get_square_mile", &MeasureUnit::getSquareMile);

  mu.def_static("get_square_yard", &MeasureUnit::getSquareYard);

  mu.def_static("get_stone", &MeasureUnit::getStone);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def("get_subtype", &MeasureUnit::getSubtype);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_tablespoon", &MeasureUnit::getTablespoon);

  mu.def_static("get_teaspoon", &MeasureUnit::getTeaspoon);

  mu.def_static("get_terabit", &MeasureUnit::getTerabit);

  mu.def_static("get_terabyte", &MeasureUnit::getTerabyte);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("get_therm_us", &MeasureUnit::getThermUs);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_ton", &MeasureUnit::getTon);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  mu.def_static("get_tonne", &MeasureUnit::getTonne);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def("get_type", &MeasureUnit::getType);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("get_volt", &MeasureUnit::getVolt);

  mu.def_static("get_watt", &MeasureUnit::getWatt);

  mu.def_static("get_week", &MeasureUnit::getWeek);

  mu.def_static("get_week_person", &MeasureUnit::getWeekPerson);

  mu.def_static("get_yard", &MeasureUnit::getYard);

  mu.def_static("get_year", &MeasureUnit::getYear);

  mu.def_static("get_year_person", &MeasureUnit::getYearPerson);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def(
      "product",
      [](const MeasureUnit &self, const MeasureUnit &other) {
        ErrorCode error_code;
        auto result = self.product(other, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("other"));

  mu.def("reciprocal", [](const MeasureUnit &self) {
    ErrorCode error_code;
    auto result = self.reciprocal(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def("split_to_single_units", [](const MeasureUnit &self) {
    ErrorCode error_code;
    auto pair = self.splitToSingleUnits(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    std::vector<MeasureUnit> result(pair.second);
    for (int32_t i = 0; i < pair.second; ++i) {
      result[i] = pair.first[i];
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def(
      "with_dimensionality",
      [](const MeasureUnit &self, int32_t dimensionality) {
        ErrorCode error_code;
        auto result = self.withDimensionality(dimensionality, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("dimensionality"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  mu.def(
      "with_prefix",
      [](const MeasureUnit &self, UMeasurePrefix prefix) {
        ErrorCode error_code;
        auto result = self.withPrefix(prefix, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("prefix"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
}

#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/measunit.h>
#include <unicode/strenum.h>

using namespace icu;

void init_measunit(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
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

  // icu::MeasureUnit
  py::class_<MeasureUnit, UObject> mu(m, "MeasureUnit");
  mu.def(py::init<const MeasureUnit &>(), py::arg("other"));
  mu.def("__copy__", &MeasureUnit::clone)
      .def(
          "__deepcopy__", [](const MeasureUnit &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const MeasureUnit &self, const UObject &other) { return self == other; }, py::is_operator(),
          py::arg("other"))
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
      .def(
          "__ne__", [](const MeasureUnit &self, const UObject &other) { return self != other; }, py::is_operator(),
          py::arg("other"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
      ;
  mu.def("clone", &MeasureUnit::clone);

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  mu.def_static("create_acre", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createAcre(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_arc_minute", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createArcMinute(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_arc_second", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createArcSecond(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_celsius", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCelsius(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_centimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCentimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_kilometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicKilometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_mile", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicMile(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_day", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDay(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_degree", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDegree(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_fahrenheit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFahrenheit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_foot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFoot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gforce", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGForce(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gram", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGram(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_hectare", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createHectare(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_hectopascal", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createHectopascal(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_horsepower", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createHorsepower(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_hour", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createHour(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_inch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createInch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_inch_hg", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createInchHg(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilogram", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilogram(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilometer_per_hour", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilometerPerHour(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilowatt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilowatt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_light_year", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createLightYear(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_liter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createLiter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_meter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_meter_per_second", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMeterPerSecond(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_mile", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMile(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_mile_per_hour", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilePerHour(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_millibar", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMillibar(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_millimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMillimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_millisecond", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMillisecond(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_minute", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMinute(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_month", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMonth(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_ounce", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createOunce(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_picometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPicometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pound", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPound(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_second", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSecond(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_foot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareFoot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_kilometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareKilometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_meter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareMeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_mile", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareMile(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_watt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createWatt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_week", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createWeek(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_yard", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createYard(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_year", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createYear(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static(
        "get_available",
        [](const char *type) {
          UErrorCode error_code = U_ZERO_ERROR;
          int32_t capacity = MeasureUnit::getAvailable(type, nullptr, 0, error_code);
          auto result = std::vector<MeasureUnit>(capacity);
          error_code = U_ZERO_ERROR;
          MeasureUnit::getAvailable(type, result.data(), capacity, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("type_"))
      .def_static("get_available", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        int32_t capacity = MeasureUnit::getAvailable(nullptr, 0, error_code);
        auto result = std::vector<MeasureUnit>(capacity);
        error_code = U_ZERO_ERROR;
        MeasureUnit::getAvailable(result.data(), capacity, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  mu.def_static("get_available_types", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::getAvailableTypes(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def("get_subtype", &MeasureUnit::getSubtype);
  mu.def("get_type", &MeasureUnit::getType);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  mu.def_static("create_acre_foot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createAcreFoot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_ampere", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createAmpere(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_astronomical_unit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createAstronomicalUnit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_bit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createBit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_bushel", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createBushel(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_byte", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createByte(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_calorie", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCalorie(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_carat", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCarat(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_centiliter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCentiliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_centimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicCentimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_foot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicFoot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_inch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicInch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_meter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicMeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cubic_yard", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCubicYard(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cup", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCup(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_deciliter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDeciliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_decimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDecimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_fathom", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFathom(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_fluid_ounce", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFluidOunce(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_foodcalorie", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFoodcalorie(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_furlong", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFurlong(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gallon", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGallon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gigabit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGigabit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gigabyte", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGigabyte(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gigahertz", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGigahertz(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_gigawatt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGigawatt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_hectoliter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createHectoliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_hertz", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createHertz(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_joule", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createJoule(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_karat", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKarat(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kelvin", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKelvin(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilobit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilobit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilobyte", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilobyte(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilocalorie", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilocalorie(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilohertz", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilohertz(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilojoule", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilojoule(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilowatt_hour", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilowattHour(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_liter_per_kilometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createLiterPerKilometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_lux", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createLux(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megabit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegabit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megabyte", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegabyte(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megahertz", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegahertz(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megaliter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegaliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megawatt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegawatt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_meter_per_second_squared", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMeterPerSecondSquared(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_metric_ton", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMetricTon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_microgram", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMicrogram(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_micrometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMicrometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_microsecond", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMicrosecond(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_mile_per_gallon", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilePerGallon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_milliampere", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilliampere(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_milligram", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilligram(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_milliliter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilliliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_millimeter_of_mercury", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMillimeterOfMercury(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_milliwatt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilliwatt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_nanometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createNanometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_nanosecond", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createNanosecond(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_nautical_mile", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createNauticalMile(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_ohm", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createOhm(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_ounce_troy", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createOunceTroy(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_parsec", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createParsec(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pint", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPint(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pound_per_square_inch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPoundPerSquareInch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_quart", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createQuart(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_radian", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createRadian(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_centimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareCentimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_inch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareInch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_square_yard", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSquareYard(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_stone", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createStone(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_tablespoon", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createTablespoon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_teaspoon", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createTeaspoon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_terabit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createTerabit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_terabyte", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createTerabyte(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_ton", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createTon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_volt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createVolt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  mu.def_static("create_century", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCentury(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_cup_metric", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCupMetric(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_generic_temperature", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGenericTemperature(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_knot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKnot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_liter_per_100_kilometers", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createLiterPer100Kilometers(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_mile_scandinavian", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMileScandinavian(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pint_metric", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPintMetric(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_revolution_angle", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createRevolutionAngle(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

#if (U_ICU_VERSION_MAJOR_NUM >= 57)
  mu.def_static("create_gallon_imperial", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGallonImperial(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_mile_per_gallon_imperial", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilePerGallonImperial(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_milligram_per_deciliter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilligramPerDeciliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_millimole_per_liter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMillimolePerLiter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_part_per_million", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPartPerMillion(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 57)

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  mu.def_static("create_point", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPoint(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  mu.def_static("create_atmosphere", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createAtmosphere(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_percent", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPercent(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_permille", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPermille(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_petabyte", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPetabyte(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  mu.def_static("create_barrel", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createBarrel(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_british_thermal_unit", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createBritishThermalUnit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dalton", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDalton(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_day_person", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDayPerson(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dunam", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDunam(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_earth_mass", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createEarthMass(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_electronvolt", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createElectronvolt(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_fluid_ounce_imperial", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createFluidOunceImperial(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_kilopascal", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilopascal(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megapascal", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegapascal(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_mole", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMole(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_month_person", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMonthPerson(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_newton", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createNewton(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_newton_meter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createNewtonMeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_permyriad", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPermyriad(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pound_foot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPoundFoot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pound_force", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPoundForce(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_solar_luminosity", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSolarLuminosity(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_solar_mass", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSolarMass(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_solar_radius", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createSolarRadius(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_week_person", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createWeekPerson(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_year_person", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createYearPerson(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("get_acre", &MeasureUnit::getAcre);
  mu.def_static("get_acre_foot", &MeasureUnit::getAcreFoot);
  mu.def_static("get_ampere", &MeasureUnit::getAmpere);
  mu.def_static("get_arc_minute", &MeasureUnit::getArcMinute);
  mu.def_static("get_arc_second", &MeasureUnit::getArcSecond);
  mu.def_static("get_astronomical_unit", &MeasureUnit::getAstronomicalUnit);
  mu.def_static("get_atmosphere", &MeasureUnit::getAtmosphere);
  mu.def_static("get_barrel", &MeasureUnit::getBarrel);
  mu.def_static("get_bit", &MeasureUnit::getBit);
  mu.def_static("get_british_thermal_unit", &MeasureUnit::getBritishThermalUnit);
  mu.def_static("get_bushel", &MeasureUnit::getBushel);
  mu.def_static("get_byte", &MeasureUnit::getByte);
  mu.def_static("get_calorie", &MeasureUnit::getCalorie);
  mu.def_static("get_carat", &MeasureUnit::getCarat);
  mu.def_static("get_celsius", &MeasureUnit::getCelsius);
  mu.def_static("get_centiliter", &MeasureUnit::getCentiliter);
  mu.def_static("get_centimeter", &MeasureUnit::getCentimeter);
  mu.def_static("get_century", &MeasureUnit::getCentury);
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
  mu.def_static("get_deciliter", &MeasureUnit::getDeciliter);
  mu.def_static("get_decimeter", &MeasureUnit::getDecimeter);
  mu.def_static("get_degree", &MeasureUnit::getDegree);
  mu.def_static("get_dunam", &MeasureUnit::getDunam);
  mu.def_static("get_earth_mass", &MeasureUnit::getEarthMass);
  mu.def_static("get_electronvolt", &MeasureUnit::getElectronvolt);
  mu.def_static("get_fahrenheit", &MeasureUnit::getFahrenheit);
  mu.def_static("get_fathom", &MeasureUnit::getFathom);
  mu.def_static("get_fluid_ounce", &MeasureUnit::getFluidOunce);
  mu.def_static("get_fluid_ounce_imperial", &MeasureUnit::getFluidOunceImperial);
  mu.def_static("get_foodcalorie", &MeasureUnit::getFoodcalorie);
  mu.def_static("get_foot", &MeasureUnit::getFoot);
  mu.def_static("get_furlong", &MeasureUnit::getFurlong);
  mu.def_static("get_gallon", &MeasureUnit::getGallon);
  mu.def_static("get_gallon_imperial", &MeasureUnit::getGallonImperial);
  mu.def_static("get_generic_temperature", &MeasureUnit::getGenericTemperature);
  mu.def_static("get_gforce", &MeasureUnit::getGForce);
  mu.def_static("get_gigabit", &MeasureUnit::getGigabit);
  mu.def_static("get_gigabyte", &MeasureUnit::getGigabyte);
  mu.def_static("get_gigahertz", &MeasureUnit::getGigahertz);
  mu.def_static("get_gigawatt", &MeasureUnit::getGigawatt);
  mu.def_static("get_gram", &MeasureUnit::getGram);
  mu.def_static("get_hectare", &MeasureUnit::getHectare);
  mu.def_static("get_hectoliter", &MeasureUnit::getHectoliter);
  mu.def_static("get_hectopascal", &MeasureUnit::getHectopascal);
  mu.def_static("get_hertz", &MeasureUnit::getHertz);
  mu.def_static("get_horsepower", &MeasureUnit::getHorsepower);
  mu.def_static("get_hour", &MeasureUnit::getHour);
  mu.def_static("get_inch", &MeasureUnit::getInch);
  mu.def_static("get_inch_hg", &MeasureUnit::getInchHg);
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
  mu.def_static("get_knot", &MeasureUnit::getKnot);
  mu.def_static("get_light_year", &MeasureUnit::getLightYear);
  mu.def_static("get_liter", &MeasureUnit::getLiter);
  mu.def_static("get_liter_per_100_kilometers", &MeasureUnit::getLiterPer100Kilometers);
  mu.def_static("get_liter_per_kilometer", &MeasureUnit::getLiterPerKilometer);
  mu.def_static("get_lux", &MeasureUnit::getLux);
  mu.def_static("get_megabit", &MeasureUnit::getMegabit);
  mu.def_static("get_megabyte", &MeasureUnit::getMegabyte);
  mu.def_static("get_megahertz", &MeasureUnit::getMegahertz);
  mu.def_static("get_megaliter", &MeasureUnit::getMegaliter);
  mu.def_static("get_megapascal", &MeasureUnit::getMegapascal);
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
  mu.def_static("get_percent", &MeasureUnit::getPercent);
  mu.def_static("get_permille", &MeasureUnit::getPermille);
  mu.def_static("get_permyriad", &MeasureUnit::getPermyriad);
  mu.def_static("get_petabyte", &MeasureUnit::getPetabyte);
  mu.def_static("get_picometer", &MeasureUnit::getPicometer);
  mu.def_static("get_pint", &MeasureUnit::getPint);
  mu.def_static("get_pint_metric", &MeasureUnit::getPintMetric);
  mu.def_static("get_point", &MeasureUnit::getPoint);
  mu.def_static("get_pound", &MeasureUnit::getPound);
  mu.def_static("get_pound_foot", &MeasureUnit::getPoundFoot);
  mu.def_static("get_pound_force", &MeasureUnit::getPoundForce);
  mu.def_static("get_pound_per_square_inch", &MeasureUnit::getPoundPerSquareInch);
  mu.def_static("get_quart", &MeasureUnit::getQuart);
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
  mu.def_static("get_tablespoon", &MeasureUnit::getTablespoon);
  mu.def_static("get_teaspoon", &MeasureUnit::getTeaspoon);
  mu.def_static("get_terabit", &MeasureUnit::getTerabit);
  mu.def_static("get_terabyte", &MeasureUnit::getTerabyte);
  mu.def_static("get_ton", &MeasureUnit::getTon);
  mu.def_static("get_volt", &MeasureUnit::getVolt);
  mu.def_static("get_watt", &MeasureUnit::getWatt);
  mu.def_static("get_week", &MeasureUnit::getWeek);
  mu.def_static("get_week_person", &MeasureUnit::getWeekPerson);
  mu.def_static("get_yard", &MeasureUnit::getYard);
  mu.def_static("get_year", &MeasureUnit::getYear);
  mu.def_static("get_year_person", &MeasureUnit::getYearPerson);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  mu.def_static("create_bar", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createBar(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_decade", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDecade(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dot_per_centimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDotPerCentimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dot_per_inch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDotPerInch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_em", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createEm(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_megapixel", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMegapixel(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pascal", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPascal(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pixel", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPixel(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pixel_per_centimeter", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPixelPerCentimeter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pixel_per_inch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPixelPerInch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_therm_us", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createThermUs(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("get_bar", &MeasureUnit::getBar);
  mu.def_static("get_decade", &MeasureUnit::getDecade);
  mu.def_static("get_dot_per_centimeter", &MeasureUnit::getDotPerCentimeter);
  mu.def_static("get_dot_per_inch", &MeasureUnit::getDotPerInch);
  mu.def_static("get_em", &MeasureUnit::getEm);
  mu.def_static("get_megapixel", &MeasureUnit::getMegapixel);
  mu.def_static("get_pascal", &MeasureUnit::getPascal);
  mu.def_static("get_pixel", &MeasureUnit::getPixel);
  mu.def_static("get_pixel_per_centimeter", &MeasureUnit::getPixelPerCentimeter);
  mu.def_static("get_pixel_per_inch", &MeasureUnit::getPixelPerInch);
  mu.def_static("get_therm_us", &MeasureUnit::getThermUs);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  mu.def_static(
      "for_identifier",
      [](const char *identifier) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = MeasureUnit::forIdentifier(identifier, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("identifier"));
  mu.def("get_complexity", [](const MeasureUnit &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getComplexity(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def("get_dimensionality", [](const MeasureUnit &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getDimensionality(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def("get_identifier", &MeasureUnit::getIdentifier);
  mu.def(
      "product",
      [](const MeasureUnit &self, const MeasureUnit &other) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.product(other, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("other"));
  mu.def("reciprocal", [](const MeasureUnit &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.reciprocal(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def(
      "with_dimensionality",
      [](const MeasureUnit &self, int32_t dimensionality) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.withDimensionality(dimensionality, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("dimensionality"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  mu.def_static("create_candela", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createCandela(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dessert_spoon", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDessertSpoon(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dessert_spoon_imperial", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDessertSpoonImperial(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dot", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDot(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_dram", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDram(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_drop", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createDrop(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_earth_radius", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createEarthRadius(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_grain", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createGrain(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("create_item", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createItem(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("create_jigger", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createJigger(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("create_kilowatt_hour_per_100_kilometer", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createKilowattHourPer100Kilometer(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("create_lumen", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createLumen(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_pinch", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createPinch(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("create_quart_imperial", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createQuartImperial(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("get_candela", &MeasureUnit::getCandela);
  mu.def_static("get_dessert_spoon", &MeasureUnit::getDessertSpoon);
  mu.def_static("get_dessert_spoon_imperial", &MeasureUnit::getDessertSpoonImperial);
  mu.def_static("get_dot", &MeasureUnit::getDot);
  mu.def_static("get_dram", &MeasureUnit::getDram);
  mu.def_static("get_drop", &MeasureUnit::getDrop);
  mu.def_static("get_earth_radius", &MeasureUnit::getEarthRadius);
  mu.def_static("get_grain", &MeasureUnit::getGrain);
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("get_item", &MeasureUnit::getItem);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("get_jigger", &MeasureUnit::getJigger);
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("get_kilowatt_hour_per_100_kilometer", &MeasureUnit::getKilowattHourPer100Kilometer);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
  mu.def_static("get_lumen", &MeasureUnit::getLumen);
  mu.def_static("get_pinch", &MeasureUnit::getPinch);
  mu.def_static("get_quart_imperial", &MeasureUnit::getQuartImperial);
  mu.def("split_to_single_units", [](const MeasureUnit &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto pair = self.splitToSingleUnits(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    std::vector<MeasureUnit> result(pair.second);
    for (int32_t i = 0; i < pair.second; ++i) {
      result[i] = pair.first[i];
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  mu.def_static("create_milligram_of_glucose_per_deciliter", []() { // Not a typo
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = MeasureUnit::createMilligramOfglucosePerDeciliter(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def_static("get_milligram_of_glucose_per_deciliter", &MeasureUnit::getMilligramOfglucosePerDeciliter);
  mu.def("get_prefix", [](const MeasureUnit &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getPrefix(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  mu.def(
      "with_prefix",
      [](const MeasureUnit &self, UMeasurePrefix prefix) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.withPrefix(prefix, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("prefix"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
}

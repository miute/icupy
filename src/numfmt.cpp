#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/numfmt.h>

using namespace icu;

void init_numfmt(py::module & /*m*/, py::class_<NumberFormat, Format> &nf) {
  //
  // icu::NumberFormat::EAlignmentFields
  //
  py::enum_<NumberFormat::EAlignmentFields>(
      nf, "EAlignmentFields", py::arithmetic(),
      "Alignment Field constants used to construct a *FieldPosition* object.\n\n"
      "Signifies that the position of the integer part or fraction part of a formatted number should be returned.\n\n"
      "Note: as of ICU 4.4, the values in this enum have been extended to support identification of all number format "
      "fields, not just those pertaining to alignment.\n\n"
      "These constants are provided for backwards compatibility only. Please use the C style constants defined in the "
      "header file unum.h.")
      .value("INTEGER_FIELD", NumberFormat::kIntegerField,
             "These constants are provided for backwards compatibility only.\n\n  "
             "Please use the constants defined in the header file unum.h.")
      .value("FRACTION_FIELD", NumberFormat::kFractionField)
      .value("DECIMAL_SEPARATOR_FIELD", NumberFormat::kDecimalSeparatorField)
      .value("EXPONENT_SYMBOL_FIELD", NumberFormat::kExponentSymbolField)
      .value("EXPONENT_SIGN_FIELD", NumberFormat::kExponentSignField)
      .value("EXPONENT_FIELD", NumberFormat::kExponentField)
      .value("GROUPING_SEPARATOR_FIELD", NumberFormat::kGroupingSeparatorField)
      .value("CURRENCY_FIELD", NumberFormat::kCurrencyField)
      .value("PERCENT_FIELD", NumberFormat::kPercentField)
      .value("PERMILL_FIELD", NumberFormat::kPermillField)
      .value("SIGN_FIELD", NumberFormat::kSignField)
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("MEASURE_UNIT_FIELD", NumberFormat::kMeasureUnitField)
      .value("COMPACT_FIELD", NumberFormat::kCompactField)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
      .export_values();

  //
  // icu::NumberFormat::ERoundingMode
  //
  py::enum_<NumberFormat::ERoundingMode>(
      nf, "ERoundingMode", py::arithmetic(),
      "Rounding mode.\n\n"
      "For more detail on rounding modes, see: "
      "https://unicode-org.github.io/icu/userguide/format_parse/numbers/rounding-modes")
      .value("ROUND_CEILING", NumberFormat::kRoundCeiling, "Round towards positive infinity.")
      .value("ROUND_FLOOR", NumberFormat::kRoundFloor, "Round towards negative infinity.")
      .value("ROUND_DOWN", NumberFormat::kRoundDown, "Round towards zero.")
      .value("ROUND_UP", NumberFormat::kRoundUp, "Round away from zero.")
      .value("ROUND_HALF_EVEN", NumberFormat::kRoundHalfEven,
             "Round towards the nearest integer, or towards the nearest even integer if equidistant.")
      .value("ROUND_HALF_DOWN", NumberFormat::kRoundHalfDown,
             "Round towards the nearest integer, or towards zero if equidistant.")
      .value("ROUND_HALF_UP", NumberFormat::kRoundHalfUp,
             "Round towards the nearest integer, or away from zero if equidistant.")
      .value("ROUND_UNNECESSARY", NumberFormat::kRoundUnnecessary,
             "Return *U_FORMAT_INEXACT_ERROR* if number does not format exactly.")
      .export_values();

  //
  // icu::NumberFormat
  //
  nf.def_static(
        "create_currency_instance",
        [](const _LocaleVariant &in_locale) {
          ErrorCode error_code;
          auto result = NumberFormat::createCurrencyInstance(VARIANT_TO_LOCALE(in_locale), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static("create_currency_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createCurrencyInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
        "create_instance",
        [](const _LocaleVariant &desired_locale, UNumberFormatStyle style) {
          ErrorCode error_code;
          auto result = NumberFormat::createInstance(VARIANT_TO_LOCALE(desired_locale), style, error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("desired_locale"), py::arg("style"))
      .def_static(
          "create_instance",
          [](const _LocaleVariant &in_locale) {
            ErrorCode error_code;
            auto result = NumberFormat::createInstance(VARIANT_TO_LOCALE(in_locale), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static("create_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
        "create_percent_instance",
        [](const _LocaleVariant &in_locale) {
          ErrorCode error_code;
          auto result = NumberFormat::createPercentInstance(VARIANT_TO_LOCALE(in_locale), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static("create_percent_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createPercentInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
        "create_scientific_instance",
        [](const _LocaleVariant &in_locale) {
          ErrorCode error_code;
          auto result = NumberFormat::createScientificInstance(VARIANT_TO_LOCALE(in_locale), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static("create_scientific_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createScientificInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = NumberFormat::getAvailableLocales(count);
        std::vector<const Locale *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);

  nf.def(
      "get_context",
      [](const NumberFormat &self, UDisplayContextType type) {
        ErrorCode error_code;
        auto result = self.getContext(type, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));

  nf.def("get_currency", &NumberFormat::getCurrency);

  nf.def("get_maximum_fraction_digits", &NumberFormat::getMaximumFractionDigits);

  nf.def("get_maximum_integer_digits", &NumberFormat::getMaximumIntegerDigits);

  nf.def("get_minimum_fraction_digits", &NumberFormat::getMinimumFractionDigits);

  nf.def("get_minimum_integer_digits", &NumberFormat::getMinimumIntegerDigits);

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  nf.def("get_rounding_mode", &NumberFormat::getRoundingMode);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  nf.def("is_grouping_used", &NumberFormat::isGroupingUsed);

  nf.def("is_lenient", &NumberFormat::isLenient);

  nf.def("is_parse_integer_only", &NumberFormat::isParseIntegerOnly);

  nf.def(
        "parse_object",
        [](const NumberFormat &self, const _UnicodeStringVariant &source, Formattable &result,
           ParsePosition &parse_pos) { self.parseObject(VARIANT_TO_UNISTR(source), result, parse_pos); },
        py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          "parse_object",
          [](const Format &self, const _UnicodeStringVariant &source, Formattable &result) {
            ErrorCode error_code;
            self.parseObject(VARIANT_TO_UNISTR(source), result, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
}

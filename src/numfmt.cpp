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
#if (U_ICU_VERSION_MAJOR_NUM >= 73)
      .value("ROUND_HALF_ODD", NumberFormat::kRoundHalfOdd, "Rounds ties toward the odd number.")
      .value("ROUND_HALF_CEILING", NumberFormat::kRoundHalfCeiling, "Rounds ties toward +infinity (U+221E).")
      .value("ROUND_HALF_FLOOR", NumberFormat::kRoundHalfFloor, "Rounds ties toward -infinity (U+221E).")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 73)
      .export_values();

  //
  // icu::NumberFormat
  //
  nf.def("__copy__", &NumberFormat::clone);

  nf.def(
      "___deepcopy__", [](const NumberFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  nf.def("clone", &NumberFormat::clone);

  nf.def_static(
        "create_currency_instance",
        [](const icupy::LocaleVariant &in_locale) {
          ErrorCode error_code;
          auto result = NumberFormat::createCurrencyInstance(icupy::to_locale(in_locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static("create_currency_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createCurrencyInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
        "create_instance",
        [](const icupy::LocaleVariant &desired_locale, UNumberFormatStyle style) {
          ErrorCode error_code;
          auto result = NumberFormat::createInstance(icupy::to_locale(desired_locale), style, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("desired_locale"), py::arg("style"))
      .def_static(
          "create_instance",
          [](const icupy::LocaleVariant &in_locale) {
            ErrorCode error_code;
            auto result = NumberFormat::createInstance(icupy::to_locale(in_locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static("create_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
        "create_percent_instance",
        [](const icupy::LocaleVariant &in_locale) {
          ErrorCode error_code;
          auto result = NumberFormat::createPercentInstance(icupy::to_locale(in_locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static("create_percent_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createPercentInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  nf.def_static(
        "create_scientific_instance",
        [](const icupy::LocaleVariant &in_locale) {
          ErrorCode error_code;
          auto result = NumberFormat::createScientificInstance(icupy::to_locale(in_locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static("create_scientific_instance", []() {
        ErrorCode error_code;
        auto result = NumberFormat::createScientificInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  nf.def(
        // [1] icu::NumberFormat::format
        // [2] icu::Format::format
        "format",
        [](const NumberFormat &self, const Formattable &obj, UnicodeString &append_to,
           FieldPosition &pos) -> UnicodeString & {
          ErrorCode error_code;
          auto &result = self.format(obj, append_to, pos, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [3] icu::Format::format
          // [4] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [5] icu::Format::format
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [8] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, double number, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"))
      .def(
          // [9] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, double number, UnicodeString &append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [11] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, double number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [12] icu::NumberFormat::format
          // [16] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, int64_t number, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"))
      .def(
          // [13] icu::NumberFormat::format
          // [17] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, int64_t number, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [15] icu::NumberFormat::format
          // [19] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, int64_t number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [20] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, const char *number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(StringPiece(number), append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number").none(false), py::arg("append_to"), py::arg("pos_iter"));

  nf.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = NumberFormat::getAvailableLocales(count);
        std::vector<const Locale *> result(count, nullptr);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  nf.def(
      "get_context",
      [](const NumberFormat &self, UDisplayContextType type) {
        ErrorCode error_code;
        auto result = self.getContext(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  nf.def("get_currency", &NumberFormat::getCurrency);

  nf.def("get_maximum_fraction_digits", &NumberFormat::getMaximumFractionDigits);

  nf.def("get_maximum_integer_digits", &NumberFormat::getMaximumIntegerDigits);

  nf.def("get_minimum_fraction_digits", &NumberFormat::getMinimumFractionDigits);

  nf.def("get_minimum_integer_digits", &NumberFormat::getMinimumIntegerDigits);

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  nf.def("get_rounding_mode", &NumberFormat::getRoundingMode);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  nf.def("is_grouping_used", [](const NumberFormat &self) -> py::bool_ { return self.isGroupingUsed(); });

  nf.def("is_lenient", [](const NumberFormat &self) -> py::bool_ { return self.isLenient(); });

  nf.def("is_parse_integer_only", [](const NumberFormat &self) -> py::bool_ { return self.isParseIntegerOnly(); });

  nf.def(
        "parse",
        [](const NumberFormat &self, const icupy::UnicodeStringVariant &text, Formattable &result,
           ParsePosition &parse_position) { self.parse(icupy::to_unistr(text), result, parse_position); },
        py::arg("text"), py::arg("result"), py::arg("parse_position"))
      .def(
          "parse",
          [](const NumberFormat &self, const icupy::UnicodeStringVariant &text, Formattable &result) {
            ErrorCode error_code;
            self.parse(icupy::to_unistr(text), result, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("result"));

  nf.def(
      "parse_currency",
      [](const NumberFormat &self, const icupy::UnicodeStringVariant &text, ParsePosition &pos) {
        return self.parseCurrency(icupy::to_unistr(text), pos);
      },
      py::arg("text"), py::arg("pos"));

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  nf.def(
      "set_context",
      [](NumberFormat &self, UDisplayContext value) {
        ErrorCode error_code;
        self.setContext(value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  nf.def(
      "set_currency",
      [](NumberFormat &self, const char16_t *the_currency) {
        ErrorCode error_code;
        self.setCurrency(the_currency, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("the_currency"));

  nf.def(
      "set_grouping_used", [](NumberFormat &self, py::bool_ new_value) { self.setGroupingUsed(new_value); },
      py::arg("new_value"));

  nf.def(
      "set_lenient", [](NumberFormat &self, py::bool_ enable) { self.setLenient(enable); }, py::arg("enable"));

  nf.def("set_maximum_fraction_digits", &NumberFormat::setMaximumFractionDigits, py::arg("new_value"));

  nf.def("set_maximum_integer_digits", &NumberFormat::setMaximumIntegerDigits, py::arg("new_value"));

  nf.def("set_minimum_fraction_digits", &NumberFormat::setMinimumFractionDigits, py::arg("new_value"));

  nf.def("set_minimum_integer_digits", &NumberFormat::setMinimumIntegerDigits, py::arg("new_value"));

  nf.def(
      "set_parse_integer_only", [](NumberFormat &self, py::bool_ value) { self.setParseIntegerOnly(value); },
      py::arg("value"));

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  nf.def("set_rounding_mode", &NumberFormat::setRoundingMode, py::arg("rounding_mode"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
}

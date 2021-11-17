#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/numfmt.h>

using namespace icu;

void init_numfmt(py::module & /*m*/, py::class_<NumberFormat, Format> &nf) {
  // icu::NumberFormat
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

  nf.def(
      "__eq__", [](const NumberFormat &self, const Format &other) { return self == other; }, py::is_operator(),
      py::arg("other"));
  nf.def("__copy__", &NumberFormat::clone)
      .def(
          "__deepcopy__", [](const NumberFormat &self, py::dict) { return self.clone(); }, py::arg("memo"));
  nf.def("clone", &NumberFormat::clone);
  nf.def_static(
        "create_currency_instance",
        [](const Locale &in_locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberFormat::createCurrencyInstance(in_locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static(
          // const char *in_locale -> const Locale &in_locale
          "create_currency_instance",
          [](const char *in_locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createCurrencyInstance(in_locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static("create_currency_instance", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = NumberFormat::createCurrencyInstance(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  nf.def_static(
        "create_instance",
        [](const Locale &desired_locale, UNumberFormatStyle style) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberFormat::createInstance(desired_locale, style, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("desired_locale"), py::arg("style"))
      .def_static(
          // const char *desired_locale -> const Locale &desired_locale
          "create_instance",
          [](const char *desired_locale, UNumberFormatStyle style) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createInstance(desired_locale, style, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("desired_locale"), py::arg("style"))
      .def_static(
          "create_instance",
          [](const Locale &in_locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createInstance(in_locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static(
          // const char *in_locale -> const Locale &in_locale
          "create_instance",
          [](const char *in_locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createInstance(in_locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static("create_instance", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = NumberFormat::createInstance(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  nf.def_static(
        "create_percent_instance",
        [](const Locale &in_locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberFormat::createPercentInstance(in_locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static(
          // const char *in_locale -> const Locale &in_locale
          "create_percent_instance",
          [](const char *in_locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createPercentInstance(in_locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static(
          // const char *in_locale -> const Locale &in_locale
          "create_percent_instance", []() {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createPercentInstance(error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          });
  nf.def_static(
        "create_scientific_instance",
        [](const Locale &in_locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberFormat::createScientificInstance(in_locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("in_locale"))
      .def_static(
          // const char *in_locale -> const Locale &in_locale
          "create_scientific_instance",
          [](const char *in_locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormat::createScientificInstance(in_locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("in_locale"))
      .def_static("create_scientific_instance", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = NumberFormat::createScientificInstance(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  nf.def(
        // [1] NumberFormat::format
        // [2] Format::format
        "format",
        [](const NumberFormat &self, const Formattable &obj, UnicodeString &append_to,
           FieldPosition &pos) -> UnicodeString & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.format(obj, append_to, pos, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [3] Format::format
          // [4] NumberFormat::format
          "format",
          [](const NumberFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [5] Format::format
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [8] NumberFormat::format
          "format",
          [](const NumberFormat &self, double number, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(number, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("append_to"))
      .def(
          // [9] NumberFormat::format
          "format", py::overload_cast<double, UnicodeString &, FieldPosition &>(&NumberFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos"))
      .def(
          // [11] NumberFormat::format
          "format",
          [](const NumberFormat &self, double number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [12] NumberFormat::format
          "format", py::overload_cast<int32_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [13] NumberFormat::format
          "format", py::overload_cast<int32_t, UnicodeString &, FieldPosition &>(&NumberFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [15] NumberFormat::format
          "format",
          [](const NumberFormat &self, int32_t number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [16] NumberFormat::format
          "format", py::overload_cast<int64_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [17] NumberFormat::format
          "format", py::overload_cast<int64_t, UnicodeString &, FieldPosition &>(&NumberFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [19] NumberFormat::format
          "format",
          [](const NumberFormat &self, int64_t number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [20] NumberFormat::format
          "format",
          [](const NumberFormat &self, const char *number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(StringPiece(number), append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"));
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
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getContext(type, error_code);
        if (U_FAILURE(error_code)) {
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
  nf.def("parse",
         py::overload_cast<const UnicodeString &, Formattable &, ParsePosition &>(&NumberFormat::parse, py::const_),
         py::arg("text"), py::arg("result"), py::arg("parse_position"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const NumberFormat &self, const char16_t *text, Formattable &result, ParsePosition &parse_position) {
            self.parse(text, result, parse_position);
          },
          py::arg("text"), py::arg("result"), py::arg("parse_position"))
      .def(
          "parse",
          [](const NumberFormat &self, const UnicodeString &text, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parse(text, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("result"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const NumberFormat &self, const char16_t *text, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parse(text, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("result"));
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  nf.def("parse_currency",
         py::overload_cast<const UnicodeString &, ParsePosition &>(&NumberFormat::parseCurrency, py::const_),
         py::arg("text"), py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse_currency",
          [](const NumberFormat &self, const char16_t *text, ParsePosition &pos) {
            return self.parseCurrency(text, pos);
          },
          py::arg("text"), py::arg("pos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
  nf.def("parse_object",
         py::overload_cast<const UnicodeString &, Formattable &, ParsePosition &>(&NumberFormat::parseObject,
                                                                                  py::const_),
         py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const NumberFormat &self, const char16_t *source, Formattable &result, ParsePosition &parse_pos) {
            self.parseObject(source, result, parse_pos);
          },
          py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          "parse_object",
          [](const Format &self, const UnicodeString &source, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parseObject(source, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const Format &self, const char16_t *source, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parseObject(source, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
}

#include "main.hpp"
#include <unicode/currpinf.h>
#include <unicode/decimfmt.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <unicode/numberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_decimfmt(py::module & /*m*/, py::class_<DecimalFormat, NumberFormat> &df) {
  // icu::DecimalFormat
  py::enum_<DecimalFormat::EPadPosition>(df, "EPadPosition", py::arithmetic(), "Pad position.")
      .value("PAD_BEFORE_PREFIX", DecimalFormat::kPadBeforePrefix)
      .value("PAD_AFTER_PREFIX", DecimalFormat::kPadAfterPrefix)
      .value("PAD_BEFORE_SUFFIX", DecimalFormat::kPadBeforeSuffix)
      .value("PAD_AFTER_SUFFIX", DecimalFormat::kPadAfterSuffix)
      .export_values();

  df.def(
        // [1] DecimalFormat::DecimalFormat
        py::init([]() {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = std::make_unique<DecimalFormat>(error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        }))
      .def(
          // [2] DecimalFormat::DecimalFormat
          py::init([](const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormat>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormat>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // [6] DecimalFormat::DecimalFormat
          py::init([](const UnicodeString &pattern, const DecimalFormatSymbols &symbols) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormat>(pattern, symbols, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("symbols"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, const DecimalFormatSymbols &symbols) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<DecimalFormat>(pattern, symbols, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("symbols"))
      .def(
          // [7] DecimalFormat::DecimalFormat
          py::init<const DecimalFormat &>(), py::arg("source"));
  df.def("__copy__", &DecimalFormat::clone)
      .def(
          "__deepcopy__", [](const DecimalFormat &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const DecimalFormat &self, const Format &other) { return self == other; }, py::is_operator(),
          py::arg("other"));
  // FIXME: Implement "void icu::DecimalFormat::adoptCurrencyPluralInfo(CurrencyPluralInfo *toAdopt)".
  // FIXME: Implement "void icu::DecimalFormat::adoptDecimalFormatSymbols(DecimalFormatSymbols *symbolsToAdopt)".
  df.def(
        "apply_localized_pattern",
        [](DecimalFormat &self, const UnicodeString &pattern) {
          UErrorCode error_code = U_ZERO_ERROR;
          self.applyLocalizedPattern(pattern, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_localized_pattern",
          [](DecimalFormat &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyLocalizedPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"))
      .def(
          "apply_localized_pattern",
          [](DecimalFormat &self, const UnicodeString &pattern, UParseError &parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyLocalizedPattern(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_localized_pattern",
          [](DecimalFormat &self, const char16_t *pattern, UParseError &parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyLocalizedPattern(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"));
  df.def(
        "apply_pattern",
        [](DecimalFormat &self, const UnicodeString &pattern) {
          UErrorCode error_code = U_ZERO_ERROR;
          self.applyPattern(pattern, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_pattern",
          [](DecimalFormat &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"))
      .def(
          "apply_pattern",
          [](DecimalFormat &self, const UnicodeString &pattern, UParseError &parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyPattern(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_pattern",
          [](DecimalFormat &self, const char16_t *pattern, UParseError &parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyPattern(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"));
  df.def("are_significant_digits_used", &DecimalFormat::areSignificantDigitsUsed);
  df.def("clone", &DecimalFormat::clone);
  df.def(
        // [1] NumberFormat::format
        "format",
        [](const DecimalFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          // [2] NumberFormat::format
          "format",
          [](const DecimalFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          // [7] NumberFormat::format
          "format", py::overload_cast<double, UnicodeString &>(&NumberFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"))
      .def(
          // [8] DecimalFormat::format
          // [9] NumberFormat::format
          "format", py::overload_cast<double, UnicodeString &, FieldPosition &>(&DecimalFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos"))

      .def(
          // [12] NumberFormat::format
          // [13] DecimalFormat::format
          "format",
          [](const DecimalFormat &self, double number, UnicodeString &append_to,
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
          // [14] NumberFormat::format
          "format", py::overload_cast<int32_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [15] DecimalFormat::format
          // [16] NumberFormat::format
          "format", py::overload_cast<int32_t, UnicodeString &, FieldPosition &>(&DecimalFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [19] NumberFormat::format
          // [20] DecimalFormat::format
          "format",
          [](const DecimalFormat &self, int32_t number, UnicodeString &append_to,
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
          // [21] NumberFormat::format
          "format", py::overload_cast<int64_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [22] NumberFormat::format
          // [23] DecimalFormat::format
          "format", py::overload_cast<int64_t, UnicodeString &, FieldPosition &>(&DecimalFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [26] NumberFormat::format
          // [27] DecimalFormat::format
          "format",
          [](const DecimalFormat &self, int64_t number, UnicodeString &append_to,
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
          // [28] NumberFormat::format
          // [29] DecimalFormat::format
          "format",
          [](const DecimalFormat &self, const char *number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(StringPiece(number), append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  df.def(
      "get_attribute",
      [](const DecimalFormat &self, UNumberFormatAttribute attr) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getAttribute(attr, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  df.def("get_currency_plural_info", &DecimalFormat::getCurrencyPluralInfo, py::return_value_policy::reference);
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("get_currency_usage", &DecimalFormat::getCurrencyUsage);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("get_decimal_format_symbols", &DecimalFormat::getDecimalFormatSymbols, py::return_value_policy::reference);
  df.def("get_format_width", &DecimalFormat::getFormatWidth);
  df.def("get_grouping_size", &DecimalFormat::getGroupingSize);
  df.def("get_maximum_significant_digits", &DecimalFormat::getMaximumSignificantDigits);
  df.def("get_minimum_exponent_digits", &DecimalFormat::getMinimumExponentDigits);
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("get_minimum_grouping_digits", &DecimalFormat::getMinimumGroupingDigits);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("get_minimum_significant_digits", &DecimalFormat::getMinimumSignificantDigits);
  df.def("get_multiplier", &DecimalFormat::getMultiplier);
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  df.def("get_multiplier_scale", &DecimalFormat::getMultiplierScale);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  df.def("get_negative_prefix", &DecimalFormat::getNegativePrefix, py::arg("result"));
  df.def("get_negative_suffix", &DecimalFormat::getNegativeSuffix, py::arg("result"));
  df.def("get_pad_character_string", &DecimalFormat::getPadCharacterString);
  df.def("get_pad_position", &DecimalFormat::getPadPosition);
  df.def("get_positive_prefix", &DecimalFormat::getPositivePrefix, py::arg("result"));
  df.def("get_positive_suffix", &DecimalFormat::getPositiveSuffix, py::arg("result"));
  df.def("get_rounding_increment", &DecimalFormat::getRoundingIncrement);
  df.def("get_rounding_mode", &DecimalFormat::getRoundingMode);
  df.def("get_secondary_grouping_size", &DecimalFormat::getSecondaryGroupingSize);
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("is_decimal_pattern_match_required", &DecimalFormat::isDecimalPatternMatchRequired);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("is_decimal_separator_always_shown", &DecimalFormat::isDecimalSeparatorAlwaysShown);
  df.def("is_exponent_sign_always_shown", &DecimalFormat::isExponentSignAlwaysShown);
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("is_format_fail_if_more_than_max_digits", &DecimalFormat::isFormatFailIfMoreThanMaxDigits);
  df.def("is_parse_case_sensitive", &DecimalFormat::isParseCaseSensitive);
  df.def("is_parse_no_exponent", &DecimalFormat::isParseNoExponent);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("is_scientific_notation", &DecimalFormat::isScientificNotation);
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("is_sign_always_shown", &DecimalFormat::isSignAlwaysShown);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("parse",
         py::overload_cast<const UnicodeString &, Formattable &, ParsePosition &>(&DecimalFormat::parse, py::const_),
         py::arg("text"), py::arg("result"), py::arg("parse_position"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse",
          [](const DecimalFormat &self, const char16_t *text, Formattable &result, ParsePosition &parse_position) {
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
  df.def("parse_currency",
         py::overload_cast<const UnicodeString &, ParsePosition &>(&DecimalFormat::parseCurrency, py::const_),
         py::arg("text"), py::arg("pos"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "parse_currency",
          [](const DecimalFormat &self, const char16_t *text, ParsePosition &pos) {
            return self.parseCurrency(text, pos);
          },
          py::arg("text"), py::arg("pos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  df.def(
      "set_attribute",
      [](DecimalFormat &self, UNumberFormatAttribute attr, int32_t new_value) -> DecimalFormat & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.setAttribute(attr, new_value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"), py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  df.def(
      "set_currency",
      [](DecimalFormat &self, const char16_t *the_currency) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setCurrency(the_currency, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("the_currency"));
  df.def("set_currency_plural_info", &DecimalFormat::setCurrencyPluralInfo, py::arg("info"));
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def(
      "set_currency_usage",
      [](DecimalFormat &self, UCurrencyUsage new_usage) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setCurrencyUsage(new_usage, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("new_usage"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("set_decimal_format_symbols", &DecimalFormat::setDecimalFormatSymbols, py::arg("symbols"));
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("set_decimal_pattern_match_required", &DecimalFormat::setDecimalPatternMatchRequired, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("set_decimal_separator_always_shown", &DecimalFormat::setDecimalSeparatorAlwaysShown, py::arg("new_value"));
  df.def("set_exponent_sign_always_shown", &DecimalFormat::setExponentSignAlwaysShown, py::arg("exp_sign_always"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_format_fail_if_more_than_max_digits", &DecimalFormat::setFormatFailIfMoreThanMaxDigits, py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_format_width", &DecimalFormat::setFormatWidth, py::arg("width"));
  df.def("set_grouping_size", &DecimalFormat::setGroupingSize, py::arg("new_value"));
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("set_grouping_used", &DecimalFormat::setGroupingUsed, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("set_lenient", &DecimalFormat::setLenient, py::arg("enable"));
  df.def("set_maximum_fraction_digits", &DecimalFormat::setMaximumFractionDigits, py::arg("new_value"));
  df.def("set_maximum_integer_digits", &DecimalFormat::setMaximumIntegerDigits, py::arg("new_value"));
  df.def("set_maximum_significant_digits", &DecimalFormat::setMaximumSignificantDigits, py::arg("max_"));
  df.def("set_minimum_exponent_digits", &DecimalFormat::setMinimumExponentDigits, py::arg("min_exp_dig"));
  df.def("set_minimum_fraction_digits", &DecimalFormat::setMinimumFractionDigits, py::arg("new_value"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_minimum_grouping_digits", &DecimalFormat::setMinimumGroupingDigits, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_minimum_integer_digits", &DecimalFormat::setMinimumIntegerDigits, py::arg("new_value"));
  df.def("set_minimum_significant_digits", &DecimalFormat::setMinimumSignificantDigits, py::arg("min_"));
  df.def("set_multiplier", &DecimalFormat::setMultiplier, py::arg("new_value"));
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  df.def("set_multiplier_scale", &DecimalFormat::setMultiplierScale, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  df.def("set_negative_prefix", &DecimalFormat::setNegativePrefix, py::arg("new_value"))
      .def(
          "set_negative_prefix",
          [](DecimalFormat &self, const char16_t *new_value) { self.setNegativePrefix(new_value); },
          py::arg("new_value"));
  df.def("set_negative_suffix", &DecimalFormat::setNegativeSuffix, py::arg("new_value"))
      .def(
          "set_negative_suffix",
          [](DecimalFormat &self, const char16_t *new_value) { self.setNegativeSuffix(new_value); },
          py::arg("new_value"));
  df.def("set_pad_character", &DecimalFormat::setPadCharacter, py::arg("pad_char"))
      .def(
          // const char16_t *pad_char -> const UnicodeString &pad_char
          "set_pad_character", [](DecimalFormat &self, const char16_t *pad_char) { self.setPadCharacter(pad_char); },
          py::arg("pad_char"));
  df.def("set_pad_position", &DecimalFormat::setPadPosition, py::arg("pad_pos"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_parse_case_sensitive", &DecimalFormat::setParseCaseSensitive, py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def("set_parse_integer_only", &DecimalFormat::setParseIntegerOnly, py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_parse_no_exponent", &DecimalFormat::setParseNoExponent, py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_positive_prefix", &DecimalFormat::setPositivePrefix, py::arg("new_value"))
      .def(
          "set_positive_prefix",
          [](DecimalFormat &self, const char16_t *new_value) { self.setPositivePrefix(new_value); },
          py::arg("new_value"));
  df.def("set_positive_suffix", &DecimalFormat::setPositiveSuffix, py::arg("new_value"))
      .def(
          "set_positive_suffix",
          [](DecimalFormat &self, const char16_t *new_value) { self.setPositiveSuffix(new_value); },
          py::arg("new_value"));
  df.def("set_rounding_increment", &DecimalFormat::setRoundingIncrement, py::arg("new_value"));
  df.def("set_rounding_mode", &DecimalFormat::setRoundingMode, py::arg("rounding_mode"));
  df.def("set_scientific_notation", &DecimalFormat::setScientificNotation, py::arg("use_scientific"));
  df.def("set_secondary_grouping_size", &DecimalFormat::setSecondaryGroupingSize, py::arg("new_value"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_sign_always_shown", &DecimalFormat::setSignAlwaysShown, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_significant_digits_used", &DecimalFormat::setSignificantDigitsUsed, py::arg("use_significant_digits"));
  df.def("to_localized_pattern", &DecimalFormat::toLocalizedPattern, py::arg("result"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def(
      "to_number_formatter",
      [](const DecimalFormat &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.toNumberFormatter(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("to_pattern", &DecimalFormat::toPattern, py::arg("result"));
}

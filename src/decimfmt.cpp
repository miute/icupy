#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/currpinf.h>
#include <unicode/decimfmt.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <unicode/numberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_decimfmt(py::module & /*m*/, py::class_<DecimalFormat, NumberFormat> &df) {
  //
  // icu::DecimalFormat::EPadPosition
  //
  py::enum_<DecimalFormat::EPadPosition>(df, "EPadPosition", py::arithmetic(), "Pad position.")
      .value("PAD_BEFORE_PREFIX", DecimalFormat::kPadBeforePrefix)
      .value("PAD_AFTER_PREFIX", DecimalFormat::kPadAfterPrefix)
      .value("PAD_BEFORE_SUFFIX", DecimalFormat::kPadBeforeSuffix)
      .value("PAD_AFTER_SUFFIX", DecimalFormat::kPadAfterSuffix)
      .export_values();

  //
  // icu::DecimalFormat
  //
  df.def(
        // [1] DecimalFormat::DecimalFormat
        py::init([]() {
          ErrorCode error_code;
          auto result = std::make_unique<DecimalFormat>(error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        }))
      .def(
          // [2] DecimalFormat::DecimalFormat
          py::init([](const icupy::UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result = std::make_unique<DecimalFormat>(icupy::to_unistr(pattern), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // [6] DecimalFormat::DecimalFormat
          py::init([](const icupy::UnicodeStringVariant &pattern, const DecimalFormatSymbols &symbols) {
            ErrorCode error_code;
            auto result = std::make_unique<DecimalFormat>(icupy::to_unistr(pattern), symbols, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("symbols"))
      .def(
          // [7] DecimalFormat::DecimalFormat
          py::init<const DecimalFormat &>(), py::arg("other"));

  df.def("__copy__", &DecimalFormat::clone);

  df.def(
      "__deepcopy__", [](const DecimalFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  // FIXME: Implement "void icu::DecimalFormat::adoptCurrencyPluralInfo(CurrencyPluralInfo *toAdopt)".
  // FIXME: Implement "void icu::DecimalFormat::adoptDecimalFormatSymbols(DecimalFormatSymbols *symbolsToAdopt)".

  df.def(
        "apply_localized_pattern",
        [](DecimalFormat &self, const icupy::UnicodeStringVariant &pattern) {
          ErrorCode error_code;
          self.applyLocalizedPattern(icupy::to_unistr(pattern), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          "apply_localized_pattern",
          [](DecimalFormat &self, const icupy::UnicodeStringVariant &pattern, UParseError &parse_error) {
            ErrorCode error_code;
            self.applyLocalizedPattern(icupy::to_unistr(pattern), parse_error, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"));

  df.def(
        "apply_pattern",
        [](DecimalFormat &self, const icupy::UnicodeStringVariant &pattern) {
          ErrorCode error_code;
          self.applyPattern(icupy::to_unistr(pattern), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          "apply_pattern",
          [](DecimalFormat &self, const icupy::UnicodeStringVariant &pattern, UParseError &parse_error) {
            ErrorCode error_code;
            self.applyPattern(icupy::to_unistr(pattern), parse_error, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"));

  df.def("are_significant_digits_used",
         [](const DecimalFormat &self) -> py::bool_ { return self.areSignificantDigitsUsed(); });

  df.def("clone", &DecimalFormat::clone);

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  df.def(
      "get_attribute",
      [](const DecimalFormat &self, UNumberFormatAttribute attr) {
        ErrorCode error_code;
        auto result = self.getAttribute(attr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
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

  df.def("get_secondary_grouping_size", &DecimalFormat::getSecondaryGroupingSize);

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def("is_decimal_pattern_match_required",
         [](const DecimalFormat &self) -> py::bool_ { return self.isDecimalPatternMatchRequired(); });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  df.def("is_decimal_separator_always_shown",
         [](const DecimalFormat &self) -> py::bool_ { return self.isDecimalSeparatorAlwaysShown(); });

  df.def("is_exponent_sign_always_shown",
         [](const DecimalFormat &self) -> py::bool_ { return self.isExponentSignAlwaysShown(); });

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("is_format_fail_if_more_than_max_digits",
         [](const DecimalFormat &self) -> py::bool_ { return self.isFormatFailIfMoreThanMaxDigits(); });

  df.def("is_parse_case_sensitive", [](const DecimalFormat &self) -> py::bool_ { return self.isParseCaseSensitive(); });

  df.def("is_parse_no_exponent", [](const DecimalFormat &self) -> py::bool_ { return self.isParseNoExponent(); });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  df.def("is_scientific_notation", [](const DecimalFormat &self) -> py::bool_ { return self.isScientificNotation(); });

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("is_sign_always_shown", [](const DecimalFormat &self) -> py::bool_ { return self.isSignAlwaysShown(); });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  df.def(
      "set_attribute",
      [](DecimalFormat &self, UNumberFormatAttribute attr, int32_t new_value) -> DecimalFormat & {
        ErrorCode error_code;
        auto &result = self.setAttribute(attr, new_value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"), py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  df.def("set_currency_plural_info", &DecimalFormat::setCurrencyPluralInfo, py::arg("info"));

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def(
      "set_currency_usage",
      [](DecimalFormat &self, UCurrencyUsage new_usage) {
        ErrorCode error_code;
        self.setCurrencyUsage(new_usage, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("new_usage"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  df.def("set_decimal_format_symbols", &DecimalFormat::setDecimalFormatSymbols, py::arg("symbols"));

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  df.def(
      "set_decimal_pattern_match_required",
      [](DecimalFormat &self, py::bool_ new_value) { self.setDecimalPatternMatchRequired(new_value); },
      py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  df.def(
      "set_decimal_separator_always_shown",
      [](DecimalFormat &self, py::bool_ new_value) { self.setDecimalSeparatorAlwaysShown(new_value); },
      py::arg("new_value"));

  df.def(
      "set_exponent_sign_always_shown",
      [](DecimalFormat &self, py::bool_ new_value) { self.setExponentSignAlwaysShown(new_value); },
      py::arg("exp_sign_always"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def(
      "set_format_fail_if_more_than_max_digits",
      [](DecimalFormat &self, py::bool_ value) { self.setFormatFailIfMoreThanMaxDigits(value); }, py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  df.def("set_format_width", &DecimalFormat::setFormatWidth, py::arg("width"));

  df.def("set_grouping_size", &DecimalFormat::setGroupingSize, py::arg("new_value"));

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  df.def(
      "set_grouping_used", [](DecimalFormat &self, py::bool_ new_value) { self.setGroupingUsed(new_value); },
      py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  df.def("set_maximum_significant_digits", &DecimalFormat::setMaximumSignificantDigits, py::arg("max_"));

  df.def("set_minimum_exponent_digits", &DecimalFormat::setMinimumExponentDigits, py::arg("min_exp_dig"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def("set_minimum_grouping_digits", &DecimalFormat::setMinimumGroupingDigits, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  df.def("set_minimum_significant_digits", &DecimalFormat::setMinimumSignificantDigits, py::arg("min_"));

  df.def("set_multiplier", &DecimalFormat::setMultiplier, py::arg("new_value"));

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  df.def("set_multiplier_scale", &DecimalFormat::setMultiplierScale, py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  df.def(
      "set_negative_prefix",
      [](DecimalFormat &self, const icupy::UnicodeStringVariant &new_value) {
        self.setNegativePrefix(icupy::to_unistr(new_value));
      },
      py::arg("new_value"));

  df.def(
      "set_negative_suffix",
      [](DecimalFormat &self, const icupy::UnicodeStringVariant &new_value) {
        self.setNegativeSuffix(icupy::to_unistr(new_value));
      },
      py::arg("new_value"));

  df.def(
      "set_pad_character",
      [](DecimalFormat &self, const icupy::UnicodeStringVariant &pad_char) {
        self.setPadCharacter(icupy::to_unistr(pad_char));
      },
      py::arg("pad_char"));

  df.def("set_pad_position", &DecimalFormat::setPadPosition, py::arg("pad_pos"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def(
      "set_parse_case_sensitive", [](DecimalFormat &self, py::bool_ value) { self.setParseCaseSensitive(value); },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def(
      "set_parse_no_exponent", [](DecimalFormat &self, py::bool_ value) { self.setParseNoExponent(value); },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  df.def(
      "set_positive_prefix",
      [](DecimalFormat &self, const icupy::UnicodeStringVariant &new_value) {
        self.setPositivePrefix(icupy::to_unistr(new_value));
      },
      py::arg("new_value"));

  df.def(
      "set_positive_suffix",
      [](DecimalFormat &self, const icupy::UnicodeStringVariant &new_value) {
        self.setPositiveSuffix(icupy::to_unistr(new_value));
      },
      py::arg("new_value"));

  df.def("set_rounding_increment", &DecimalFormat::setRoundingIncrement, py::arg("new_value"));

  df.def(
      "set_scientific_notation",
      [](DecimalFormat &self, py::bool_ use_scientific) { self.setScientificNotation(use_scientific); },
      py::arg("use_scientific"));

  df.def("set_secondary_grouping_size", &DecimalFormat::setSecondaryGroupingSize, py::arg("new_value"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def(
      "set_sign_always_shown", [](DecimalFormat &self, py::bool_ new_value) { self.setSignAlwaysShown(new_value); },
      py::arg("new_value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  df.def(
      "set_significant_digits_used",
      [](DecimalFormat &self, py::bool_ use_significant_digits) {
        self.setSignificantDigitsUsed(use_significant_digits);
      },
      py::arg("use_significant_digits"));

  df.def("to_localized_pattern", &DecimalFormat::toLocalizedPattern, py::arg("result"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  df.def(
      "to_number_formatter",
      [](const DecimalFormat &self) {
        ErrorCode error_code;
        auto result = self.toNumberFormatter(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  df.def("to_pattern", &DecimalFormat::toPattern, py::arg("result"));
}

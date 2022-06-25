#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/rbnf.h>

using namespace icu;

void init_rbnf(py::module &m) {
  //
  // icu::URBNFRuleSetTag
  //
  py::enum_<URBNFRuleSetTag>(m, "URBNFRuleSetTag", py::arithmetic(), "Tags for the predefined rulesets.")
      .value("URBNF_SPELLOUT", URBNF_SPELLOUT)
      .value("URBNF_ORDINAL", URBNF_ORDINAL)
      .value("URBNF_DURATION", URBNF_DURATION)
      .value("URBNF_NUMBERING_SYSTEM", URBNF_NUMBERING_SYSTEM)
      .export_values();

  //
  // icu::RuleBasedNumberFormat
  //
  py::class_<RuleBasedNumberFormat, NumberFormat> rbnf(m, "RuleBasedNumberFormat");

  rbnf.def(
          // [1] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const _UnicodeStringVariant &rules, UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(VARIANT_TO_UNISTR(rules), perror, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("perror"))
      .def(
          // [2] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const _UnicodeStringVariant &rules, const _UnicodeStringVariant &localizations,
                      UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(VARIANT_TO_UNISTR(rules),
                                                                  VARIANT_TO_UNISTR(localizations), perror, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("localizations"), py::arg("perror"))
      .def(
          // [3] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const _UnicodeStringVariant &rules, const Locale &locale, UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(VARIANT_TO_UNISTR(rules), locale, perror, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("locale"), py::arg("perror"))
      .def(
          // [4] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const _UnicodeStringVariant &rules, const _UnicodeStringVariant &localizations,
                      const _LocaleVariant &locale, UParseError &perror) {
            ErrorCode error_code;
            auto result =
                std::make_unique<RuleBasedNumberFormat>(VARIANT_TO_UNISTR(rules), VARIANT_TO_UNISTR(localizations),
                                                        VARIANT_TO_LOCALE(locale), perror, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("localizations"), py::arg("locale"), py::arg("perror"))
      .def(
          // [5] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](URBNFRuleSetTag tag, const _LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(tag, VARIANT_TO_LOCALE(locale), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("tag"), py::arg("locale"))
      .def(
          // [6] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init<const RuleBasedNumberFormat &>(), py::arg("other"));

  rbnf.def("__copy__", &RuleBasedNumberFormat::clone);

  rbnf.def(
      "__deepcopy__", [](const RuleBasedNumberFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  rbnf.def(
      "__eq__", [](const RuleBasedNumberFormat &self, const Format &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  // FIXME: Implement "void icu::RuleBasedNumberFormat::adoptDecimalFormatSymbols(DecimalFormatSymbols
  //  *symbolsToAdopt)".

  rbnf.def("clone", &RuleBasedNumberFormat::clone);

  rbnf.def(
          // [1] NumberFormat::format
          // [2] Format::format
          "format",
          [](const NumberFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos, error_code);
            if (error_code.isFailure()) {
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
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [5] Format::format
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [9] RuleBasedNumberFormat::format
          "format",
          [](const RuleBasedNumberFormat &self, double number, const _UnicodeStringVariant &rule_set_name,
             UnicodeString &to_append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, VARIANT_TO_UNISTR(rule_set_name), to_append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("rule_set_name"), py::arg("to_append_to"), py::arg("pos"))
      .def(
          // [10] NumberFormat::format
          "format", py::overload_cast<double, UnicodeString &>(&NumberFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"))
      .def(
          // [11] NumberFormat::format
          // [14] RuleBasedNumberFormat::format
          "format",
          py::overload_cast<double, UnicodeString &, FieldPosition &>(&RuleBasedNumberFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos"))
      .def(
          // [13] NumberFormat::format
          "format",
          [](const NumberFormat &self, double number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [15] RuleBasedNumberFormat::format
          "format",
          [](const RuleBasedNumberFormat &self, int32_t number, const _UnicodeStringVariant &rule_set_name,
             UnicodeString &to_append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, VARIANT_TO_UNISTR(rule_set_name), to_append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("rule_set_name"), py::arg("to_append_to"), py::arg("pos"))
      .def(
          // [16] NumberFormat::format
          "format", py::overload_cast<int32_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [17] NumberFormat::format
          // [20] RuleBasedNumberFormat::format
          "format",
          py::overload_cast<int32_t, UnicodeString &, FieldPosition &>(&RuleBasedNumberFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [19] NumberFormat::format
          "format",
          [](const NumberFormat &self, int32_t number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [21] RuleBasedNumberFormat::format
          "format",
          [](const RuleBasedNumberFormat &self, int64_t number, const _UnicodeStringVariant &rule_set_name,
             UnicodeString &to_append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, VARIANT_TO_UNISTR(rule_set_name), to_append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("rule_set_name"), py::arg("to_append_to"), py::arg("pos"))
      .def(
          // [22] NumberFormat::format
          "format", py::overload_cast<int64_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [23] NumberFormat::format
          // [26] RuleBasedNumberFormat::format
          "format",
          py::overload_cast<int64_t, UnicodeString &, FieldPosition &>(&RuleBasedNumberFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [25] NumberFormat::format
          "format",
          [](const NumberFormat &self, int64_t number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [27] NumberFormat::format
          "format",
          [](const NumberFormat &self, char *number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(StringPiece(number), append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"));

  rbnf.def("get_default_rule_set_name", &RuleBasedNumberFormat::getDefaultRuleSetName);

  rbnf.def("get_number_of_rule_set_display_name_locales", &RuleBasedNumberFormat::getNumberOfRuleSetDisplayNameLocales);

  rbnf.def("get_number_of_rule_set_names", &RuleBasedNumberFormat::getNumberOfRuleSetNames);

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  rbnf.def("get_rounding_mode", &RuleBasedNumberFormat::getRoundingMode);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  rbnf.def("get_rules", &RuleBasedNumberFormat::getRules);

  rbnf.def(
          "get_rule_set_display_name",
          [](RuleBasedNumberFormat &self, const _UnicodeStringVariant &rule_set_name, const _LocaleVariant &locale) {
            return self.getRuleSetDisplayName(VARIANT_TO_UNISTR(rule_set_name), VARIANT_TO_LOCALE(locale));
          },
          py::arg("rule_set_name"), py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"))
      .def(
          "get_rule_set_display_name",
          [](RuleBasedNumberFormat &self, int32_t index, const _LocaleVariant &locale) {
            return self.getRuleSetDisplayName(index, VARIANT_TO_LOCALE(locale));
          },
          py::arg("index"), py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"));

  rbnf.def(
      "get_rule_set_display_name_locale",
      [](const RuleBasedNumberFormat &self, int32_t index) {
        ErrorCode error_code;
        auto result = self.getRuleSetDisplayNameLocale(index, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("index"));

  rbnf.def("get_rule_set_name", &RuleBasedNumberFormat::getRuleSetName, py::arg("index"));

  rbnf.def("is_lenient", &RuleBasedNumberFormat::isLenient);

  rbnf.def(
          // [1] RuleBasedNumberFormat::parse
          // [2] NumberFormat::parse
          "parse",
          [](const RuleBasedNumberFormat &self, const _UnicodeStringVariant &text, Formattable &result,
             ParsePosition &parse_position) { self.parse(VARIANT_TO_UNISTR(text), result, parse_position); },
          py::arg("text"), py::arg("result"), py::arg("parse_position"))
      .def(
          // [3] NumberFormat::parse
          "parse",
          [](const NumberFormat &self, const _UnicodeStringVariant &text, Formattable &result) {
            ErrorCode error_code;
            self.parse(VARIANT_TO_UNISTR(text), result, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("result"));

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  rbnf.def(
      "set_context",
      [](RuleBasedNumberFormat &self, UDisplayContext value) {
        ErrorCode error_code;
        self.setContext(value, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("value"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  rbnf.def("set_decimal_format_symbols", &RuleBasedNumberFormat::setDecimalFormatSymbols, py::arg("symbols"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  rbnf.def(
      "set_default_rule_set",
      [](RuleBasedNumberFormat &self, const _UnicodeStringVariant &rule_set_name) {
        ErrorCode error_code;
        self.setDefaultRuleSet(VARIANT_TO_UNISTR(rule_set_name), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("rule_set_name"));

  rbnf.def("set_lenient", &RuleBasedNumberFormat::setLenient, py::arg("enabled"));

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  rbnf.def("set_rounding_mode", &RuleBasedNumberFormat::setRoundingMode, py::arg("rounding_mode"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
}

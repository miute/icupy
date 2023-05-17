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
          py::init([](const icupy::UnicodeStringVariant &rules, UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(icupy::to_unistr(rules), perror, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("perror"))
      .def(
          // [2] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const icupy::UnicodeStringVariant &rules, const icupy::UnicodeStringVariant &localizations,
                      UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(icupy::to_unistr(rules),
                                                                  icupy::to_unistr(localizations), perror, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("localizations"), py::arg("perror"))
      .def(
          // [3] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const icupy::UnicodeStringVariant &rules, const Locale &locale, UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(icupy::to_unistr(rules), locale, perror, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("locale"), py::arg("perror"))
      .def(
          // [4] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](const icupy::UnicodeStringVariant &rules, const icupy::UnicodeStringVariant &localizations,
                      const icupy::LocaleVariant &locale, UParseError &perror) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(
                icupy::to_unistr(rules), icupy::to_unistr(localizations), icupy::to_locale(locale), perror, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("localizations"), py::arg("locale"), py::arg("perror"))
      .def(
          // [5] RuleBasedNumberFormat::RuleBasedNumberFormat
          py::init([](URBNFRuleSetTag tag, const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedNumberFormat>(tag, icupy::to_locale(locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
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

  // FIXME: Implement "void icu::RuleBasedNumberFormat::adoptDecimalFormatSymbols(DecimalFormatSymbols
  //  *symbolsToAdopt)".

  rbnf.def("clone", &RuleBasedNumberFormat::clone);

  rbnf.def(
          // [1] icu::Format::format
          // [2] icu::NumberFormat::format
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
          // [9] icu::RuleBasedNumberFormat::format
          "format",
          [](const RuleBasedNumberFormat &self, double number, const icupy::UnicodeStringVariant &rule_set_name,
             UnicodeString &to_append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, icupy::to_unistr(rule_set_name), to_append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("rule_set_name"), py::arg("to_append_to"), py::arg("pos"))
      .def(
          // [10] icu::NumberFormat::format
          "format", py::overload_cast<double, UnicodeString &>(&NumberFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"))
      .def(
          // [11] icu::NumberFormat::format
          // [14] icu::RuleBasedNumberFormat::format
          "format",
          py::overload_cast<double, UnicodeString &, FieldPosition &>(&RuleBasedNumberFormat::format, py::const_),
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos"))
      .def(
          // [13] icu::NumberFormat::format
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
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [15] icu::RuleBasedNumberFormat::format
          // [21] icu::RuleBasedNumberFormat::format
          "format",
          [](const RuleBasedNumberFormat &self, int64_t number, const icupy::UnicodeStringVariant &rule_set_name,
             UnicodeString &to_append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, icupy::to_unistr(rule_set_name), to_append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("rule_set_name"), py::arg("to_append_to"), py::arg("pos"))
      .def(
          // [16] icu::NumberFormat::format
          // [22] icu::NumberFormat::format
          "format", py::overload_cast<int64_t, UnicodeString &>(&NumberFormat::format, py::const_), py::arg("number"),
          py::arg("append_to"))
      .def(
          // [17] icu::NumberFormat::format
          // [20] icu::RuleBasedNumberFormat::format
          // [23] icu::NumberFormat::format
          // [26] icu::RuleBasedNumberFormat::format
          "format",
          py::overload_cast<int64_t, UnicodeString &, FieldPosition &>(&RuleBasedNumberFormat::format, py::const_),
          py::arg("number"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [19] icu::NumberFormat::format
          // [25] icu::NumberFormat::format
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
          // [27] icu::NumberFormat::format
          "format",
          [](const NumberFormat &self, char *number, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(StringPiece(number), append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos_iter"));

  rbnf.def("get_default_rule_set_name", &RuleBasedNumberFormat::getDefaultRuleSetName);

  rbnf.def("get_number_of_rule_set_display_name_locales", &RuleBasedNumberFormat::getNumberOfRuleSetDisplayNameLocales);

  rbnf.def("get_number_of_rule_set_names", &RuleBasedNumberFormat::getNumberOfRuleSetNames);

  rbnf.def("get_rules", &RuleBasedNumberFormat::getRules);

  rbnf.def(
          "get_rule_set_display_name",
          [](RuleBasedNumberFormat &self, const icupy::UnicodeStringVariant &rule_set_name,
             const icupy::LocaleVariant &locale) {
            return self.getRuleSetDisplayName(icupy::to_unistr(rule_set_name), icupy::to_locale(locale));
          },
          py::arg("rule_set_name"), py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"))
      .def(
          "get_rule_set_display_name",
          [](RuleBasedNumberFormat &self, int32_t index, const icupy::LocaleVariant &locale) {
            return self.getRuleSetDisplayName(index, icupy::to_locale(locale));
          },
          py::arg("index"), py::arg_v("locale", Locale::getDefault(), "icupy.icu.Locale.get_default()"));

  rbnf.def(
      "get_rule_set_display_name_locale",
      [](const RuleBasedNumberFormat &self, int32_t index) {
        ErrorCode error_code;
        auto result = self.getRuleSetDisplayNameLocale(index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("index"));

  rbnf.def("get_rule_set_name", &RuleBasedNumberFormat::getRuleSetName, py::arg("index"));

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  rbnf.def("set_decimal_format_symbols", &RuleBasedNumberFormat::setDecimalFormatSymbols, py::arg("symbols"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  rbnf.def(
      "set_default_rule_set",
      [](RuleBasedNumberFormat &self, const icupy::UnicodeStringVariant &rule_set_name) {
        ErrorCode error_code;
        self.setDefaultRuleSet(icupy::to_unistr(rule_set_name), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("rule_set_name"));
}

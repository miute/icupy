#include "main.hpp"
#include "utextptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/rbbi.h>

using namespace icu;

void init_rbbi(py::module &m) {
  // icu::BreakIterator
  py::class_<BreakIterator, UObject> bi(m, "BreakIterator");

  py::enum_<decltype(BreakIterator::DONE)>(bi, "BreakIterator", py::arithmetic())
      .value("DONE", BreakIterator::DONE,
             "*DONE* is returned by *previous()* and *next()* after all valid boundaries have been returned.")
      .export_values();

  bi.def("__copy__", &BreakIterator::clone)
      .def(
          "__deepcopy__", [](const BreakIterator &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const BreakIterator &self, const BreakIterator &other) { return self == other; },
          py::arg("other"))
      .def("__iter__",
           [](BreakIterator &self) -> BreakIterator & {
             self.first();
             return self;
           })
      .def(
          "__ne__", [](const BreakIterator &self, const BreakIterator &other) { return self != other; },
          py::arg("other"))
      .def("__next__",
           [](BreakIterator &self) {
             auto n = self.next();
             if (n == BreakIterator::DONE) {
               throw py::stop_iteration();
             }
             return n;
           })
      .def("__reversed__", [](BreakIterator &self) {
        std::vector<int32_t> result;
        for (auto n = self.last(); n != 0 && n != BreakIterator::DONE; n = self.previous()) {
          result.push_back(n);
        }
        return result;
      });
  bi.def(
      "adopt_text", [](BreakIterator &self, CharacterIterator *it) { self.adoptText(it ? it->clone() : nullptr); },
      py::arg("it"));
  bi.def("clone", &BreakIterator::clone);
  bi.def_static(
      "create_character_instance",
      [](const _LocaleVariant &where) {
        ErrorCode error_code;
        auto it = BreakIterator::createCharacterInstance(VARIANT_TO_LOCALE(where), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return it;
      },
      py::arg("where"));
  bi.def_static(
      "create_line_instance",
      [](const _LocaleVariant &where) {
        ErrorCode error_code;
        auto it = BreakIterator::createLineInstance(VARIANT_TO_LOCALE(where), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return it;
      },
      py::arg("where"));
  bi.def_static(
      "create_sentence_instance",
      [](const _LocaleVariant &where) {
        ErrorCode error_code;
        auto it = BreakIterator::createSentenceInstance(VARIANT_TO_LOCALE(where), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return it;
      },
      py::arg("where"));
#ifndef U_HIDE_DEPRECATED_API
  bi.def_static(
      "create_title_instance",
      [](const _LocaleVariant &where) {
        ErrorCode error_code;
        auto it = BreakIterator::createTitleInstance(VARIANT_TO_LOCALE(where), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return it;
      },
      py::arg("where"));
#endif // U_HIDE_DEPRECATED_API
  bi.def_static(
      "create_word_instance",
      [](const _LocaleVariant &where) {
        ErrorCode error_code;
        auto it = BreakIterator::createWordInstance(VARIANT_TO_LOCALE(where), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return it;
      },
      py::arg("where"));
  bi.def("current", &BreakIterator::current);
  bi.def("first", &BreakIterator::first);
  bi.def("following", &BreakIterator::following, py::arg("offset"));
  bi.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = BreakIterator::getAvailableLocales(count);
        std::vector<const Locale *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  bi.def_static(
        "get_display_name",
        [](const _LocaleVariant &object_locale, const _LocaleVariant &display_locale,
           UnicodeString &name) -> UnicodeString & {
          return BreakIterator::getDisplayName(VARIANT_TO_LOCALE(object_locale), VARIANT_TO_LOCALE(display_locale),
                                               name);
        },
        py::arg("object_locale"), py::arg("display_locale"), py::arg("name"))
      .def_static(
          "get_display_name",
          [](const _LocaleVariant &object_locale, UnicodeString &name) -> UnicodeString & {
            return BreakIterator::getDisplayName(VARIANT_TO_LOCALE(object_locale), name);
          },
          py::arg("object_locale"), py::arg("name"));
  bi.def(
      "get_locale",
      [](const BreakIterator &self, ULocDataLocaleType type) {
        ErrorCode error_code;
        auto result = self.getLocale(type, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  bi.def("get_rule_status", &BreakIterator::getRuleStatus);
  bi.def("get_rule_status_vec", [](BreakIterator &self) {
    ErrorCode error_code;
    std::vector<int32_t> result(self.getRuleStatusVec(nullptr, 0, error_code));
    error_code.reset();
    self.getRuleStatusVec(result.data(), static_cast<int32_t>(result.size()), error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
  bi.def("get_text", &BreakIterator::getText, py::return_value_policy::reference);
  bi.def(
      "get_utext",
      [](const BreakIterator &self, std::optional<_UTextPtr> &fill_in) {
        ErrorCode error_code;
        auto p = self.getUText(fill_in.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::keep_alive<0, 1>(), py::arg("fill_in"));
  bi.def("is_boundary", &BreakIterator::isBoundary, py::arg("offset"));
  bi.def("last", &BreakIterator::last);
  bi.def("next", py::overload_cast<int32_t>(&BreakIterator::next), py::arg("n"))
      .def("next", py::overload_cast<>(&BreakIterator::next));
  bi.def("preceding", &BreakIterator::preceding, py::arg("offset"));
  bi.def("previous", &BreakIterator::previous);
  // FIXME: Implement "BreakIterator& BreakIterator::refreshInputText(UText *input, UErrorCode &status)".
  // TODO: Implement "static URegistryKey BreakIterator::registerInstance(BreakIterator *toAdopt, const Locale &locale,
  // UBreakIteratorType kind,	UErrorCode &status)".
  bi.def("set_text", py::overload_cast<const UnicodeString &>(&BreakIterator::setText), py::arg("text"))
      .def(
          "set_text",
          [](BreakIterator &self, _UTextPtr &text) {
            ErrorCode error_code;
            self.setText(text, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"));
  // TODO: Implement "static UBool BreakIterator::unregister(URegistryKey key, UErrorCode &status)".

  // icu::RuleBasedBreakIterator
  py::class_<RuleBasedBreakIterator, BreakIterator> rbbi(m, "RuleBasedBreakIterator");
  rbbi.def(
          // [2] RuleBasedBreakIterator::RuleBasedBreakIterator
          py::init<const RuleBasedBreakIterator &>(), py::arg("that"))
      .def(
          // [3] RuleBasedBreakIterator::RuleBasedBreakIterator
          py::init([](const _UnicodeStringVariant &rules, UParseError &parse_error) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedBreakIterator>(VARIANT_TO_UNISTR(rules), parse_error, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("parse_error"))
      .def(
          // [4] RuleBasedBreakIterator::RuleBasedBreakIterator
          py::init([](const std::vector<uint8_t> &compiled_rules, uint32_t rule_length) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedBreakIterator>(compiled_rules.data(), rule_length, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::keep_alive<0, 1>(), py::arg("compiled_rules"), py::arg("rule_length"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  rbbi.def(
      "adopt_text",
      [](RuleBasedBreakIterator &self, CharacterIterator *it) { self.adoptText(it ? it->clone() : nullptr); },
      py::arg("it"));
  rbbi.def("clone", &RuleBasedBreakIterator::clone);
  rbbi.def("current", &RuleBasedBreakIterator::current);
  rbbi.def("first", &RuleBasedBreakIterator::first);
  rbbi.def("following", &RuleBasedBreakIterator::following, py::arg("offset"));
  rbbi.def("get_binary_rules", [](RuleBasedBreakIterator &self) {
    uint32_t length = 0;
    auto p = self.getBinaryRules(length);
    std::vector<uint8_t> result(p, p + length);
    return result;
  });
  rbbi.def("get_rules", &RuleBasedBreakIterator::getRules);
  rbbi.def("get_text", &RuleBasedBreakIterator::getText, py::return_value_policy::reference);
  rbbi.def(
      "get_utext",
      [](const RuleBasedBreakIterator &self, std::optional<_UTextPtr> &fill_in) {
        ErrorCode error_code;
        auto p = self.getUText(fill_in.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::keep_alive<0, 1>(), py::arg("fill_in"));
  rbbi.def("hash_code", &RuleBasedBreakIterator::hashCode);
  rbbi.def("is_boundary", &RuleBasedBreakIterator::isBoundary, py::arg("offset"));
  rbbi.def("last", &RuleBasedBreakIterator::last);
  rbbi.def("next", py::overload_cast<int32_t>(&RuleBasedBreakIterator::next), py::arg("n"))
      .def("next", py::overload_cast<>(&RuleBasedBreakIterator::next));
  rbbi.def("preceding", &RuleBasedBreakIterator::preceding, py::arg("offset"));
  rbbi.def("previous", &RuleBasedBreakIterator::previous);
  // FIXME: Implement "BreakIterator& refreshInputText(UText *input, UErrorCode &status)".
  rbbi.def("set_text", py::overload_cast<const UnicodeString &>(&RuleBasedBreakIterator::setText), py::arg("text"))
      .def(
          "set_text",
          [](RuleBasedBreakIterator &self, _UTextPtr &text) {
            ErrorCode error_code;
            self.setText(text, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"));
}

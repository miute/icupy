#include "main.hpp"
#include <memory>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/rbbi.h>

using namespace icu;

void init_rbbi(py::module &m) {
  // BreakIterator
  py::class_<BreakIterator> bi(m, "BreakIterator");

  py::enum_<decltype(BreakIterator::DONE)>(bi, "BreakIterator", py::arithmetic())
      .value("DONE", BreakIterator::DONE)
      .export_values();

  bi.def("__copy__", &BreakIterator::clone)
      .def("__deepcopy__", [](const BreakIterator &self, py::dict) { return self.clone(); })
      .def("__iter__",
           [](BreakIterator &self) -> BreakIterator & {
             self.first();
             return self;
           })
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
  bi.def_static(
      "create_character_instance",
      [](const Locale &where) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto it = BreakIterator::createCharacterInstance(where, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return it;
      },
      py::arg("where"));
  bi.def_static(
      "create_line_instance",
      [](const Locale &where) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto it = BreakIterator::createLineInstance(where, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return it;
      },
      py::arg("where"));
  bi.def_static(
      "create_sentence_instance",
      [](const Locale &where) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto it = BreakIterator::createSentenceInstance(where, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return it;
      },
      py::arg("where"));
#ifndef U_HIDE_DEPRECATED_API
  bi.def_static(
      "create_title_instance",
      [](const Locale &where) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto it = BreakIterator::createTitleInstance(where, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return it;
      },
      py::arg("where"));
#endif // U_HIDE_DEPRECATED_API
  bi.def_static(
      "create_word_instance",
      [](const Locale &where) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto it = BreakIterator::createWordInstance(where, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return it;
      },
      py::arg("where"));
  bi.def_static("get_available_locales", py::overload_cast<>(&BreakIterator::getAvailableLocales));
  bi.def_static("get_display_name",
                py::overload_cast<const Locale &, const Locale &, UnicodeString &>(&BreakIterator::getDisplayName),
                py::arg("object_locale"), py::arg("display_locale"), py::arg("name"))
      .def_static("get_display_name",
                  py::overload_cast<const Locale &, UnicodeString &>(&BreakIterator::getDisplayName),
                  py::arg("object_locale"), py::arg("name"));
  bi.def(
      "get_locale",
      [](const BreakIterator &self, ULocDataLocaleType type) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getLocale(type, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return result;
      },
      py::arg("type_"));
  bi.def("get_rule_status", &BreakIterator::getRuleStatus);
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  bi.def("get_rule_status_vec", [](BreakIterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    std::vector<int32_t> result(self.getRuleStatusVec(NULL, 0, error_code));
    error_code = U_ZERO_ERROR;
    self.getRuleStatusVec(result.data(), (int32_t)result.size(), error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
  // TODO: Implement "static URegistryKey registerInstance(BreakIterator *toAdopt, const Locale &locale,
  // UBreakIteratorType kind,	UErrorCode &status)".
  // TODO: Implement "static UBool unregister(URegistryKey key, UErrorCode &status)".

  // RuleBasedBreakIterator
  py::class_<RuleBasedBreakIterator, BreakIterator> rbbi(m, "RuleBasedBreakIterator");
  rbbi.def(py::init<const RuleBasedBreakIterator &>(), py::arg("that"))
      .def(py::init([](const UnicodeString &rules, UParseError &parse_error) {
             UErrorCode error_code = U_ZERO_ERROR;
             std::unique_ptr<RuleBasedBreakIterator> result(new RuleBasedBreakIterator(rules, parse_error, error_code));
             if (U_FAILURE(error_code)) {
               throw ICUException(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("parse_error"))
      .def(py::init([](const std::vector<uint8_t> &compiled_rules, uint32_t rule_length) {
             UErrorCode error_code = U_ZERO_ERROR;
             std::unique_ptr<RuleBasedBreakIterator> result(
                 new RuleBasedBreakIterator(compiled_rules.data(), rule_length, error_code));
             if (U_FAILURE(error_code)) {
               throw ICUException(error_code);
             }
             return result;
           }),
           py::arg("compiled_rules"), py::arg("rule_length"))
      .def(py::self != py::self)
      .def(py::self == py::self);
  rbbi.def(
      "adopt_text",
      [](RuleBasedBreakIterator &self, CharacterIterator *it) { self.adoptText(it ? it->clone() : NULL); },
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
  // TODO: Implement "UText* getUText(UText *fillIn, UErrorCode &status)".
  rbbi.def("hash_code", &RuleBasedBreakIterator::hashCode);
  rbbi.def("is_boundary", &RuleBasedBreakIterator::isBoundary, py::arg("offset"));
  rbbi.def("last", &RuleBasedBreakIterator::last);
  rbbi.def("next", py::overload_cast<int32_t>(&RuleBasedBreakIterator::next), py::arg("n"))
      .def("next", py::overload_cast<>(&RuleBasedBreakIterator::next));
  rbbi.def("preceding", &RuleBasedBreakIterator::preceding, py::arg("offset"));
  rbbi.def("previous", &RuleBasedBreakIterator::previous);
  // TODO: Implement "BreakIterator& refreshInputText(UText *input, UErrorCode &status)".
  // TODO: Implement "void setText(UText *text, UErrorCode &status)".
  rbbi.def("set_text", py::overload_cast<const UnicodeString &>(&RuleBasedBreakIterator::setText), py::arg("text"));
}

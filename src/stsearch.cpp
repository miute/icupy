#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/stsearch.h>

using namespace icu;

class PySearchIterator : public SearchIterator {
public:
  PySearchIterator(const PySearchIterator &other) : SearchIterator(other) {}
  PySearchIterator() : SearchIterator() {}
  PySearchIterator(const UnicodeString &text, BreakIterator *breakiter = nullptr) : SearchIterator(text, breakiter) {}
  PySearchIterator(CharacterIterator &text, BreakIterator *breakiter = nullptr) : SearchIterator(text, breakiter) {}

  int32_t getOffset(void) const override {
    PYBIND11_OVERRIDE_PURE_NAME(int32_t, SearchIterator, "get_offset", getOffset);
  }

  int32_t handleNext(int32_t position, UErrorCode & /*status*/) override {
    PYBIND11_OVERRIDE_PURE_NAME(int32_t, SearchIterator, "_handle_next", handleNext, position);
  }

  int32_t handlePrev(int32_t position, UErrorCode & /*status*/) override {
    PYBIND11_OVERRIDE_PURE_NAME(int32_t, SearchIterator, "_handle_prev", handlePrev, position);
  }

  SearchIterator *safeClone(void) const override {
    PYBIND11_OVERRIDE_PURE_NAME(SearchIterator *, SearchIterator, "safe_clone", safeClone);
  }

  void setOffset(int32_t position, UErrorCode & /*status*/) override {
    PYBIND11_OVERRIDE_PURE_NAME(void, SearchIterator, "set_offset", setOffset, position);
  }

  using SearchIterator::setMatchLength;
  using SearchIterator::setMatchNotFound;
  using SearchIterator::setMatchStart;
};

void init_stsearch(py::module &m) {
  //
  // icu::SearchIterator
  //
  py::class_<SearchIterator, UObject, PySearchIterator> si(m, "SearchIterator");

  si.def(py::init<const PySearchIterator &>(), py::arg("other"))
      .def(py::init<>())
      .def(py::init([](const icupy::UnicodeStringVariant &text, BreakIterator *breakiter) {
             return std::make_unique<PySearchIterator>(icupy::to_unistr(text), breakiter);
           }),
           py::arg("text"), py::arg("breakiter") = nullptr)
      .def(py::init([](CharacterIterator &text, BreakIterator *breakiter) {
             return std::make_unique<PySearchIterator>(text, breakiter);
           }),
           py::arg("text"), py::arg("breakiter") = nullptr);

  si.def(
      "__eq__", [](const SearchIterator &self, const SearchIterator &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  si.def("__iter__", [](SearchIterator &self) -> SearchIterator & {
    self.reset();
    return self;
  });

  si.def(
      "__ne__", [](const SearchIterator &self, const SearchIterator &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  si.def("__next__", [](SearchIterator &self) {
    ErrorCode error_code;
    auto n = self.next(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    } else if (n == USEARCH_DONE) {
      throw py::stop_iteration();
    }
    return n;
  });

  si.def("__reversed__", [](SearchIterator &self) {
    std::vector<int32_t> result;
    ErrorCode error_code;
    for (auto n = self.last(error_code); n != USEARCH_DONE; n = self.previous(error_code)) {
      if (error_code.isFailure()) {
        throw icupy::ICUError(error_code);
      }
      result.push_back(n);
    }
    return result;
  });

  si.def("first", [](SearchIterator &self) {
    ErrorCode error_code;
    auto result = self.first(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  si.def(
      "following",
      [](SearchIterator &self, int32_t position) {
        ErrorCode error_code;
        auto result = self.following(position, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("position"));

  si.def("get_attribute", &SearchIterator::getAttribute, py::arg("attribute"));

  si.def("get_break_iterator", &SearchIterator::getBreakIterator);

  si.def("get_matched_length", &SearchIterator::getMatchedLength);

  si.def("get_matched_start", &SearchIterator::getMatchedStart);

  si.def("get_matched_text", &SearchIterator::getMatchedText, py::arg("result"));

  si.def("get_text", &SearchIterator::getText);

  si.def("last", [](SearchIterator &self) {
    ErrorCode error_code;
    auto result = self.last(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  si.def("next", [](SearchIterator &self) {
    ErrorCode error_code;
    auto result = self.next(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  si.def(
      "preceding",
      [](SearchIterator &self, int32_t position) {
        ErrorCode error_code;
        auto result = self.preceding(position, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("position"));

  si.def("previous", [](SearchIterator &self) {
    ErrorCode error_code;
    auto result = self.previous(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  si.def("reset", &SearchIterator::reset);

  si.def(
      "set_attribute",
      [](SearchIterator &self, USearchAttribute attribute, USearchAttributeValue value) {
        ErrorCode error_code;
        self.setAttribute(attribute, value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("attribute"), py::arg("value"));

  si.def(
      "set_break_iterator",
      [](SearchIterator &self, BreakIterator *breakiter) {
        ErrorCode error_code;
        self.setBreakIterator(breakiter, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("breakiter"));

  si.def("_set_match_length", &PySearchIterator::setMatchLength, py::arg("length"));

  si.def("_set_match_not_found", &PySearchIterator::setMatchNotFound);

  si.def("_set_match_start", &PySearchIterator::setMatchStart, py::arg("position"));

  si.def(
        "set_text",
        [](SearchIterator &self, CharacterIterator &text) {
          ErrorCode error_code;
          self.setText(text, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
        },
        py::arg("text"))
      .def(
          "set_text",
          [](SearchIterator &self, const icupy::UnicodeStringVariant &text) {
            ErrorCode error_code;
            self.setText(icupy::to_unistr(text), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"));

  //
  // icu::StringSearch
  //
  py::class_<StringSearch, SearchIterator> ss(m, "StringSearch");

  ss.def(
        // [1] StringSearch::StringSearch
        py::init([](const icupy::UnicodeStringVariant &pattern, const icupy::UnicodeStringVariant &text,
                    const icupy::LocaleVariant &locale, BreakIterator *breakiter) {
          ErrorCode error_code;
          auto result = std::make_unique<StringSearch>(icupy::to_unistr(pattern), icupy::to_unistr(text),
                                                       icupy::to_locale(locale), breakiter, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        }),
        py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // [2] StringSearch::StringSearch
          py::init([](const icupy::UnicodeStringVariant &pattern, const icupy::UnicodeStringVariant &text,
                      RuleBasedCollator *coll, BreakIterator *breakiter) {
            ErrorCode error_code;
            auto result = std::make_unique<StringSearch>(icupy::to_unistr(pattern), icupy::to_unistr(text), coll,
                                                         breakiter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // [3] StringSearch::StringSearch
          py::init([](const icupy::UnicodeStringVariant &pattern, CharacterIterator &text,
                      const icupy::LocaleVariant &locale, BreakIterator *breakiter) {
            ErrorCode error_code;
            auto result = std::make_unique<StringSearch>(icupy::to_unistr(pattern), text, icupy::to_locale(locale),
                                                         breakiter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // [4] StringSearch::StringSearch
          py::init([](const icupy::UnicodeStringVariant &pattern, CharacterIterator &text, RuleBasedCollator *coll,
                      BreakIterator *breakiter) {
            ErrorCode error_code;
            auto result = std::make_unique<StringSearch>(icupy::to_unistr(pattern), text, coll, breakiter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // [5] StringSearch::StringSearch
          py::init<const StringSearch &>(), py::arg("other"));

  ss.def("__copy__", &StringSearch::clone);

  ss.def(
      "__deepcopy__", [](const StringSearch &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  ss.def("clone", &StringSearch::clone);

  ss.def("get_collator", &StringSearch::getCollator, py::return_value_policy::reference);

  ss.def("get_offset", &StringSearch::getOffset);

  ss.def("get_pattern", &StringSearch::getPattern);

  ss.def("safe_clone", &StringSearch::safeClone);

  ss.def(
      "set_collator",
      [](StringSearch &self, RuleBasedCollator *coll) {
        ErrorCode error_code;
        self.setCollator(coll, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("coll"));

  ss.def(
      "set_offset",
      [](StringSearch &self, int32_t position) {
        ErrorCode error_code;
        self.setOffset(position, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("position"));

  ss.def(
      "set_pattern",
      [](StringSearch &self, const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.setPattern(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("pattern"));

  si.def_property_readonly_static("DONE", [](const py::object &) -> int32_t { return USEARCH_DONE; });
}

#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/stsearch.h>

using namespace icu;

class PySearchIterator : SearchIterator {
public:
  using SearchIterator::SearchIterator;

  int32_t getOffset(void) const override { PYBIND11_OVERLOAD_PURE(int32_t, SearchIterator, getOffset); }

  int32_t handleNext(int32_t position, UErrorCode &status) override {
    PYBIND11_OVERLOAD_PURE(int32_t, SearchIterator, position, status);
  }

  int32_t handlePrev(int32_t position, UErrorCode &status) override {
    PYBIND11_OVERLOAD_PURE(int32_t, SearchIterator, handlePrev, position, status);
  }

  SearchIterator *safeClone(void) const override {
    PYBIND11_OVERLOAD_PURE(SearchIterator *, SearchIterator, safeClone);
  }

  void setOffset(int32_t position, UErrorCode &status) override {
    PYBIND11_OVERRIDE_PURE(void, SearchIterator, setOffset, position, status);
  }
};

void init_stsearch(py::module &m) {
  // icu::SearchIterator
  py::class_<SearchIterator, UObject, PySearchIterator> si(m, "SearchIterator");
  si.def("__iter__",
         [](SearchIterator &self) -> SearchIterator & {
           self.reset();
           return self;
         })
      .def("__next__",
           [](SearchIterator &self) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto n = self.next(error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             } else if (n == USEARCH_DONE) {
               throw py::stop_iteration();
             }
             return n;
           })
      .def("__reversed__", [](SearchIterator &self) {
        std::vector<int32_t> result;
        UErrorCode error_code = U_ZERO_ERROR;
        for (auto n = self.last(error_code); n != USEARCH_DONE; n = self.previous(error_code)) {
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          result.push_back(n);
        }
        return result;
      });
  si.def("first", [](SearchIterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.first(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  si.def(
      "following",
      [](SearchIterator &self, int32_t position) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.following(position, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
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
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.last(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  si.def("next", [](SearchIterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.next(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  si.def(
      "preceding",
      [](SearchIterator &self, int32_t position) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.preceding(position, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("position"));
  si.def("previous", [](SearchIterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.previous(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  si.def(
      "set_attribute",
      [](SearchIterator &self, USearchAttribute attribute, USearchAttributeValue value) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setAttribute(attribute, value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("attribute"), py::arg("value"));
  si.def(
      "set_break_iterator",
      [](SearchIterator &self, BreakIterator *breakiter) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setBreakIterator(breakiter, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("breakiter"));

  // icu::StringSearch
  py::class_<StringSearch, SearchIterator> ss(m, "StringSearch");
  ss.def(
        // [1] StringSearch::StringSearch
        py::init([](const UnicodeString &pattern, const UnicodeString &text, const Locale &locale,
                    BreakIterator *breakiter) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        }),
        py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init(
              [](const char16_t *pattern, const UnicodeString &text, const Locale &locale, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          py::init(
              [](const UnicodeString &pattern, const char16_t *text, const Locale &locale, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const UnicodeString &pattern, const UnicodeString &text, const char *locale,
                      BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *text -> const UnicodeString &text
          py::init([](const char16_t *pattern, const char16_t *text, const Locale &locale, BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char *locale -> const Locale &locale
          py::init(
              [](const char16_t *pattern, const UnicodeString &text, const char *locale, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          // const char *locale -> const Locale &locale
          py::init(
              [](const UnicodeString &pattern, const char16_t *text, const char *locale, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *text -> const UnicodeString &text
          // const char *locale -> const Locale &locale
          py::init([](const char16_t *pattern, const char16_t *text, const char *locale, BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // [2] StringSearch::StringSearch
          py::init([](const UnicodeString &pattern, const UnicodeString &text, RuleBasedCollator *coll,
                      BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, coll, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, const UnicodeString &text, RuleBasedCollator *coll,
                      BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, coll, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          py::init([](const UnicodeString &pattern, const char16_t *text, RuleBasedCollator *coll,
                      BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, coll, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char16_t *text -> const UnicodeString &text
          py::init(
              [](const char16_t *pattern, const char16_t *text, RuleBasedCollator *coll, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, coll, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // [3] StringSearch::StringSearch
          py::init([](const UnicodeString &pattern, CharacterIterator &text, const Locale &locale,
                      BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init(
              [](const char16_t *pattern, CharacterIterator &text, const Locale &locale, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char *locale -> const Locale &locale
          py::init(
              [](const UnicodeString &pattern, CharacterIterator &text, const char *locale, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          // const char *locale -> const Locale &locale
          py::init([](const char16_t *pattern, CharacterIterator &text, const char *locale, BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, locale, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"), py::arg("breakiter"))
      .def(
          // [4] StringSearch::StringSearch
          py::init([](const UnicodeString &pattern, CharacterIterator &text, RuleBasedCollator *coll,
                      BreakIterator *breakiter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<StringSearch>(pattern, text, coll, breakiter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init(
              [](const char16_t *pattern, CharacterIterator &text, RuleBasedCollator *coll, BreakIterator *breakiter) {
                UErrorCode error_code = U_ZERO_ERROR;
                auto result = std::make_unique<StringSearch>(pattern, text, coll, breakiter, error_code);
                if (U_FAILURE(error_code)) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("text"), py::arg("coll"), py::arg("breakiter"))
      .def(
          // [5] StringSearch::StringSearch
          py::init<const StringSearch &>(), py::arg("that"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  ss.def("__copy__", &StringSearch::clone)
      .def(
          "__deepcopy__", [](const StringSearch &self, py::dict) { return self.clone(); }, py::arg("memo"));
  ss.def("clone", &StringSearch::clone);
  ss.def("get_collator", &StringSearch::getCollator, py::return_value_policy::reference);
  ss.def("get_offset", &StringSearch::getOffset);
  ss.def("get_pattern", &StringSearch::getPattern);
  ss.def("reset", &StringSearch::reset);
  ss.def("safe_clone", &StringSearch::safeClone);
  ss.def(
      "set_collator",
      [](StringSearch &self, RuleBasedCollator *coll) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setCollator(coll, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("coll"));
  ss.def(
      "set_offset",
      [](StringSearch &self, int32_t position) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setOffset(position, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("position"));
  ss.def(
        "set_pattern",
        [](StringSearch &self, const UnicodeString &pattern) {
          UErrorCode error_code = U_ZERO_ERROR;
          self.setPattern(pattern, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          "set_pattern",
          [](StringSearch &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"));
  ss.def(
        "set_text",
        [](StringSearch &self, CharacterIterator &text) {
          UErrorCode error_code = U_ZERO_ERROR;
          self.setText(text, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
        },
        py::arg("text"))
      .def(
          "set_text",
          [](StringSearch &self, const UnicodeString &text) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setText(text, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "set_text",
          [](StringSearch &self, const char16_t *text) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setText(text, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("text"));

  si.def_property_readonly_static("DONE", [](py::object) -> int32_t { return USEARCH_DONE; });
}

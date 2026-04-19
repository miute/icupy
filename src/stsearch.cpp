#include "main.hpp"
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/stsearch.h>

using namespace icu;

class PySearchIterator : public SearchIterator {
public:
  PySearchIterator(const PySearchIterator &other) : SearchIterator(other) {}

  PySearchIterator() : SearchIterator() {}

  PySearchIterator(const UnicodeString &text,
                   BreakIterator *breakiter = nullptr)
      : SearchIterator(text, breakiter) {}

  PySearchIterator(CharacterIterator &text, BreakIterator *breakiter = nullptr)
      : SearchIterator(text, breakiter) {}

  int32_t getOffset(void) const override {
    PYBIND11_OVERRIDE_PURE_NAME(int32_t, SearchIterator, "get_offset",
                                getOffset);
  }

  int32_t handleNext(int32_t position, UErrorCode & /* status */) override {
    PYBIND11_OVERRIDE_PURE_NAME(int32_t, SearchIterator, "_handle_next",
                                handleNext, position);
  }

  int32_t handlePrev(int32_t position, UErrorCode & /* status */) override {
    PYBIND11_OVERRIDE_PURE_NAME(int32_t, SearchIterator, "_handle_prev",
                                handlePrev, position);
  }

  SearchIterator *safeClone(void) const override {
    PYBIND11_OVERRIDE_PURE_NAME(SearchIterator *, SearchIterator, "safe_clone",
                                safeClone);
  }

  void setOffset(int32_t position, UErrorCode & /* status */) override {
    PYBIND11_OVERRIDE_PURE_NAME(void, SearchIterator, "set_offset", setOffset,
                                position);
  }

  using SearchIterator::setMatchLength;
  using SearchIterator::setMatchNotFound;
  using SearchIterator::setMatchStart;
};

void init_stsearch(py::module &m) {
  //
  // class icu::SearchIterator
  //
  py::class_<SearchIterator, UObject, PySearchIterator> si(m, "SearchIterator",
                                                           R"doc(
      Abstract base class that provides methods to search for a pattern within
      a text string.

      Note:
          Subclasses must implement the following abstract methods:
          :meth:`.get_offset`, :meth:`._handle_next`,
          :meth:`._handle_prev`, and :meth:`.set_offset`.

      See Also:
          :class:`StringSearch`

      Example:

          .. code-block:: python

              # Search iterator using regular expressions
              from icupy import icu
              class RegexSearch(icu.SearchIterator):
                  def __init__(
                      self,
                      regexp: icu.UnicodeString | str,
                      text: icu.UnicodeString,
                      flags: int = 0,
                  ) -> None:
                      icu.SearchIterator.__init__(self, text)
                      self._matcher = icu.RegexMatcher(regexp, text, flags)
                      self._offset = 0
                  def _handle_next(self, position: int) -> int:
                      # Forward search logic
                      if not self._matcher.find(position):
                          self._set_match_not_found()
                          return icu.USEARCH_DONE
                      start = self._matcher.start()
                      end = self._matcher.end()
                      self._set_match_start(start)
                      self._set_match_length(end - start)
                      self._offset = end
                      return start
                  def _handle_prev(self, position: int) -> int:
                      # Backward search logic
                      last_start = icu.USEARCH_DONE
                      last_length = 0
                      # The RegexMatcher does not support backward searches,
                      # so it performs a full search.
                      self._matcher.reset()
                      while self._matcher.find():
                          start = self._matcher.start()
                          end = self._matcher.end()
                          if end > position:
                              break
                          last_start = start
                          last_length = end - start
                      if last_start == icu.USEARCH_DONE:
                          self._set_match_not_found()
                      else:
                          self._set_match_start(last_start)
                          self._set_match_length(last_length)
                          self._offset = last_start
                      return last_start
                  def get_offset(self) -> int:
                      return self._offset
                  def set_offset(self, position: int) -> None:
                      if position < 0 or position > len(self.get_text()):
                          raise icu.ICUError(icu.U_INDEX_OUTOFBOUNDS_ERROR)
                      self._offset = position

              text = icu.UnicodeString("Apple 123 Banana 456 Cherry 789")
              it = RegexSearch("\\d+", text)
              [text[start:end] for start, end in it]  # ['123', '456', '789']
              [text[start:end] for start, end in reversed(it)]  # ['789', '456', '123']
      )doc");

  si.def(py::init<const PySearchIterator &>(), py::arg("other"), R"doc(
      Initialize a ``SearchIterator`` instance from another ``SearchIterator``.
      )doc")
      .def(py::init<>(), R"doc(
      Initialize a ``SearchIterator`` instance with default values.

      .. important::

          Only available in subclasses.
      )doc")
      .def(py::init([](const icupy::UnicodeStringVariant &text,
                       std::optional<BreakIterator *> &breakiter) {
             return std::make_unique<PySearchIterator>(
                 icupy::to_unistr(text), breakiter.value_or(nullptr));
           }),
           py::arg("text"), py::arg("breakiter") = std::nullopt, R"doc(
      Initialize a ``SearchIterator`` instance with the specified text and
      break iterator.

      .. important::

          Only available in subclasses.
      )doc")
      .def(py::init([](CharacterIterator &text,
                       std::optional<BreakIterator *> &breakiter) {
             return std::make_unique<PySearchIterator>(
                 text, breakiter.value_or(nullptr));
           }),
           py::arg("text"), py::arg("breakiter") = std::nullopt, R"doc(
      Initialize a ``SearchIterator`` instance with the specified text and
      break iterator.

      .. important::

          Only available in subclasses.
      )doc");

  si.def(
      "__eq__",
      [](const SearchIterator &self, const SearchIterator &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  si.def(
      "__iter__",
      [](SearchIterator &self) -> SearchIterator & {
        self.reset();
        return self;
      },
      R"doc(
      Return an iterator object.

      See Also:
          :meth:`.__next__`
      )doc");

  si.def(
      "__ne__",
      [](const SearchIterator &self, const SearchIterator &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  si.def(
      "__next__",
      [](SearchIterator &self) {
        ErrorCode error_code;
        auto index = self.next(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        } else if (index == USEARCH_DONE) {
          throw py::stop_iteration();
        }
        auto length = self.getMatchedLength();
        return std::make_tuple(index, index + length);
      },
      R"doc(
      Return a tuple of the starting and ending indices of the next match.
      )doc");

  si.def(
      "__reversed__",
      [](SearchIterator &self) {
        ErrorCode error_code;
        auto index = self.last(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        std::vector<std::tuple<int32_t, int32_t>> result;
        int32_t length;
        while (index != USEARCH_DONE) {
          length = self.getMatchedLength();
          result.push_back(std::make_tuple(index, index + length));
          index = self.preceding(index, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
        }
        return result;
      },
      R"doc(
      Return a reversed iterator object.

      See Also:
          :meth:`.__iter__`
          :meth:`.__next__`
      )doc");

  si.def(
      "first",
      [](SearchIterator &self) {
        ErrorCode error_code;
        auto result = self.first(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the character index of the first match; :attr:`USEARCH_DONE`
      otherwise.

      If a match is found, the iterator is adjusted so that the current index
      points to the position of the match.
      )doc");

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
      py::arg("position"), R"doc(
      Return the character index of the first match following *position*;
      :attr:`USEARCH_DONE` otherwise.

      If a match is found, the iterator is adjusted so that the current index
      points to the position of the match.
      )doc");

  si.def("get_attribute", &SearchIterator::getAttribute, py::arg("attribute"),
         R"doc(
      Return the value of the specified search attribute.

      See Also:
          :meth:`.set_attribute`
      )doc");

  si.def(
      "get_break_iterator",
      [](const SearchIterator &self) -> std::optional<const BreakIterator *> {
        return self.getBreakIterator();
      },
      R"doc(
      Return the break iterator used by this search iterator.

      See Also:
          :meth:`.set_break_iterator`
      )doc");

  si.def("get_matched_length", &SearchIterator::getMatchedLength, R"doc(
      Return the length of the matched text.

      Note:
          This call returns a valid result only if one of the calls to
          :meth:`.first`, :meth:`.next`, :meth:`.previous`, or :meth:`.last`
          has completed successfully.

      See Also:
          :meth:`.first`
          :meth:`.get_matched_start`
          :meth:`.get_matched_text`
          :meth:`.last`
          :meth:`.next`
          :meth:`.previous`
      )doc");

  si.def("get_matched_start", &SearchIterator::getMatchedStart, R"doc(
      Return the starting index of the matched text.

      Note:
          This call returns a valid result only if one of the calls to
          :meth:`.first`, :meth:`.next`, :meth:`.previous`, or :meth:`.last`
          has completed successfully.

      See Also:
          :meth:`.first`
          :meth:`.get_matched_length`
          :meth:`.get_matched_text`
          :meth:`.last`
          :meth:`.next`
          :meth:`.previous`
      )doc");

  si.def("get_matched_text", &SearchIterator::getMatchedText, py::arg("result"),
         R"doc(
      Copy the matched text into *result*.

      Note:
          This call returns a valid result only if one of the calls to
          :meth:`.first`, :meth:`.next`, :meth:`.previous`, or :meth:`.last`
          has completed successfully.

      See Also:
          :meth:`.first`
          :meth:`.last`
          :meth:`.next`
          :meth:`.previous`
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  si.def("get_offset", &SearchIterator::getOffset,
         R"doc(
      Return the current index in the text being searched.

      If the iteration has passed the end of the text (or the beginning, in the
      case of a reverse search), return :attr:`USEARCH_DONE`.

      Important:
          Subclasses must implement the following abstract methods:
          :meth:`.get_offset`, :meth:`._handle_next`,
          :meth:`._handle_prev`, and :meth:`.set_offset`.

      See Also:
           :meth:`.set_offset`
      )doc");

  si.def(
      "get_text",
      [](const SearchIterator &self) -> const UnicodeString & {
        return self.getText();
      },
      R"doc(
      Return the text being searched.

      See Also:
          :meth:`.set_text`
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  si.def(
      "_handle_next",
      [](PySearchIterator &self, int32_t position) {
        ErrorCode error_code;
        return self.handleNext(position, error_code);
      },
      py::arg("position"),
      R"doc(
      Framework method that performs the actual forward search in subclasses.

      If a match is found, the implementation must return an index indicating
      the start of the match, call :meth:`._set_match_start` with that index as
      an argument, and call :meth:`._set_match_length` with the number of
      characters in the target text that make up the matching string as an
      argument. If no match is found, it must return :attr:`USEARCH_DONE` and
      call :meth:`._set_match_not_found`.

      Important:
          Subclasses must implement the following abstract methods:
          :meth:`.get_offset`, :meth:`._handle_next`,
          :meth:`._handle_prev`, and :meth:`.set_offset`.

      See Also:
          :meth:`._handle_prev`
          :meth:`._set_match_length`
          :meth:`._set_match_not_found`
          :meth:`._set_match_start`
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  si.def(
      "_handle_prev",
      [](PySearchIterator &self, int32_t position) {
        ErrorCode error_code;
        return self.handlePrev(position, error_code);
      },
      py::arg("position"),
      R"doc(
      Framework method that performs the actual backward search in subclasses.

      If a match is found, the implementation must return an index indicating
      the start of the match, call :meth:`._set_match_start` with that index as
      an argument, and call :meth:`._set_match_length` with the number of
      characters in the target text that make up the matching string as an
      argument. If no match is found, it must return :attr:`USEARCH_DONE` and
      call :meth:`._set_match_not_found`.

      Important:
          Subclasses must implement the following abstract methods:
          :meth:`.get_offset`, :meth:`._handle_next`,
          :meth:`._handle_prev`, and :meth:`.set_offset`.

      See Also:
          :meth:`._handle_next`
          :meth:`._set_match_length`
          :meth:`._set_match_not_found`
          :meth:`._set_match_start`
      )doc");

  si.def(
      "last",
      [](SearchIterator &self) {
        ErrorCode error_code;
        auto result = self.last(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the character index of the last match; :attr:`USEARCH_DONE`
      otherwise.

      If a match is found, the iterator is adjusted so that the current index
      points to the position of the match.
      )doc");

  si.def(
      "next",
      [](SearchIterator &self) {
        ErrorCode error_code;
        auto result = self.next(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the character index of the next match; :attr:`USEARCH_DONE`
      otherwise.

      If a match is found, the iterator is adjusted so that the current index
      points to the position of the match.
      )doc");

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
      py::arg("position"), R"doc(
      Return the character index of the first match preceding *position*;
      :attr:`USEARCH_DONE` otherwise.

      If a match is found, the iterator is adjusted so that the current index
      points to the position of the match.
      )doc");

  si.def(
      "previous",
      [](SearchIterator &self) {
        ErrorCode error_code;
        auto result = self.previous(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the character index of the previous match; :attr:`USEARCH_DONE`
      otherwise.

      If a match is found, the iterator is adjusted so that the current index
      points to the position of the match.
      )doc");

  si.def("reset", &SearchIterator::reset, R"doc(
      Reset the search iterator to the initial state.
      )doc");

  si.def(
      "set_attribute",
      [](SearchIterator &self, USearchAttribute attribute,
         USearchAttributeValue value) {
        ErrorCode error_code;
        self.setAttribute(attribute, value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("attribute"), py::arg("value"), R"doc(
      Set the specified search attribute to the specified value.

      See Also:
          :meth:`.get_attribute`
      )doc");

  si.def(
      "set_break_iterator",
      [](SearchIterator &self, std::optional<BreakIterator *> &breakiter) {
        ErrorCode error_code;
        self.setBreakIterator(breakiter.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("breakiter"), R"doc(
      Set the break iterator to be used by this search iterator.

      ``BreakIterator`` is used to limit the points at which matches are
      detected. If a match is found but its start or end index does not fall
      within the boundaries defined by ``BreakIterator``, that match is
      rejected and the search continues for another match. If *breakiter* is
      `None`, no breaks are detected.

      See Also:
          :meth:`.get_break_iterator`
      )doc");

  si.def("_set_match_length", &PySearchIterator::setMatchLength,
         py::arg("length"), R"doc(
      Set the length of the matched text.

      Important:
          :meth:`._handle_next` and :meth:`._handle_prev` of the subclasses
          must call :meth:`._set_match_length` whenever a match is found within
          the target text.
      )doc");

  si.def("_set_match_not_found", &PySearchIterator::setMatchNotFound, R"doc(
      Set the state of the search iterator to indicate that no match was found.

      Important:
          :meth:`._handle_next` and :meth:`._handle_prev` of the subclasses
          must call :meth:`._set_match_not_found` if a match is not found
          within the target text.
      )doc");

  si.def("_set_match_start", &PySearchIterator::setMatchStart,
         py::arg("position"), R"doc(
      Set the starting index of the matched text.

      Important:
          :meth:`._handle_next` and :meth:`._handle_prev` of the subclasses
          must call :meth:`._set_match_start` whenever a match is found within
          the target text.
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  si.def(
      "set_offset",
      [](SearchIterator &self, int32_t position) {
        ErrorCode error_code;
        self.setOffset(position, error_code);
      },
      py::arg("position"), R"doc(
      Set the current index in the text being searched to *position*.

      Important:
          Subclasses must implement the following abstract methods:
          :meth:`.get_offset`, :meth:`._handle_next`,
          :meth:`._handle_prev`, and :meth:`.set_offset`.

      See Also:
          :meth:`.get_offset`
      )doc");

  si.def(
        "set_text",
        [](SearchIterator &self, CharacterIterator &text) {
          ErrorCode error_code;
          self.setText(text, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
        },
        py::arg("text"), R"doc(
      Set the text to be searched.

      The iteration is reset to the start.

      .. seealso::

          :meth:`.get_text`
      )doc")
      .def(
          "set_text",
          [](SearchIterator &self, const icupy::UnicodeStringVariant &text) {
            ErrorCode error_code;
            self.setText(icupy::to_unistr(text), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), R"doc(
      Set the text to be searched.

      The iteration is reset to the start.

      .. seealso::

          :meth:`.get_text`
      )doc");

  //
  // class icu::StringSearch
  //
  py::class_<StringSearch, SearchIterator> ss(m, "StringSearch");

  ss.def(
        // [1] StringSearch::StringSearch
        py::init([](const icupy::UnicodeStringVariant &pattern,
                    const icupy::UnicodeStringVariant &text,
                    const icupy::LocaleVariant &locale,
                    std::optional<BreakIterator *> &breakiter) {
          ErrorCode error_code;
          auto result = std::make_unique<StringSearch>(
              icupy::to_unistr(pattern), icupy::to_unistr(text),
              icupy::to_locale(locale), breakiter.value_or(nullptr),
              error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        }),
        py::arg("pattern"), py::arg("text"), py::arg("locale"),
        py::arg("breakiter") = std::nullopt)
      .def(
          // [2] StringSearch::StringSearch
          py::init([](const icupy::UnicodeStringVariant &pattern,
                      const icupy::UnicodeStringVariant &text,
                      RuleBasedCollator *coll,
                      std::optional<BreakIterator *> &breakiter) {
            ErrorCode error_code;
            auto result = std::make_unique<StringSearch>(
                icupy::to_unistr(pattern), icupy::to_unistr(text), coll,
                breakiter.value_or(nullptr), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll").none(false),
          py::arg("breakiter") = std::nullopt)
      .def(
          // [3] StringSearch::StringSearch
          py::init([](const icupy::UnicodeStringVariant &pattern,
                      CharacterIterator &text,
                      const icupy::LocaleVariant &locale,
                      std::optional<BreakIterator *> &breakiter) {
            ErrorCode error_code;
            auto result = std::make_unique<StringSearch>(
                icupy::to_unistr(pattern), text, icupy::to_locale(locale),
                breakiter.value_or(nullptr), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("locale"),
          py::arg("breakiter") = std::nullopt)
      .def(
          // [4] StringSearch::StringSearch
          py::init([](const icupy::UnicodeStringVariant &pattern,
                      CharacterIterator &text, RuleBasedCollator *coll,
                      std::optional<BreakIterator *> &breakiter) {
            ErrorCode error_code;
            auto result = std::make_unique<StringSearch>(
                icupy::to_unistr(pattern), text, coll,
                breakiter.value_or(nullptr), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("text"), py::arg("coll").none(false),
          py::arg("breakiter") = std::nullopt)
      .def(
          // [5] StringSearch::StringSearch
          py::init<const StringSearch &>(), py::arg("other"));

  ss.def("__copy__", &StringSearch::clone);

  ss.def(
      "__deepcopy__",
      [](const StringSearch &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"));

  ss.def("clone", &StringSearch::clone);

  ss.def("get_collator", &StringSearch::getCollator,
         py::return_value_policy::reference);

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
      py::arg("coll").none(false));

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

  si.def_property_readonly_static(
      "DONE", [](const py::object &) -> int32_t { return USEARCH_DONE; });
}

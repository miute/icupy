#include "main.hpp"
#include "utextptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/rbbi.h>

using namespace icu;

void init_rbbi(py::module &m) {
  //
  // class icu::BreakIterator
  //
  py::class_<BreakIterator, UObject> bi(m, "BreakIterator", R"doc(
      Abstract base class for identifying boundaries (boundary positions)
      within text. It scans text while maintaining the current position and
      returns the index of characters that form boundaries.

      - Line boundary analysis determines where a text string can be broken
        when line-wrapping. The mechanism correctly handles punctuation and
        hyphenated words.
      - Sentence boundary analysis allows selection with correct interpretation
        of periods within numbers and abbreviations, and trailing punctuation
        marks such as quotation marks and parentheses.
      - Word boundary analysis is used by search and replace functions, as well
        as within text editing applications that allow the user to select words
        with a double click. Word selection provides correct interpretation of
        punctuation marks within and following words. Characters that are not
        part of a word, such as symbols or punctuation marks, have word-breaks
        on both sides.
      - Character boundary analysis allows users to interact with characters as
        they expect to, for example, when moving the cursor through a text
        string. Character boundary analysis provides correct navigation of
        through character strings, regardless of how the character is stored.
        For example, an accented character might be stored as a base character
        and a diacritical mark. What users consider to be a character can
        differ between languages.

      The text boundary positions are found according to the rules described
      in Unicode Standard Annex #29, Text Boundaries, and Unicode Standard
      Annex #14, Line Breaking Properties. These are available at
      http://www.unicode.org/reports/tr14/ and
      http://www.unicode.org/reports/tr29/.

      For more information, see the ICU User Guide:
      `Boundary Analysis <https://unicode-org.github.io/icu/userguide/boundaryanalysis/>`__.

      See Also:
          :class:`RuleBasedBreakIterator`

      Examples:
          BreakIterator for sentence-breaks:

          >>> from icupy import icu
          >>> bi = icu.BreakIterator.create_sentence_instance(icu.ULOC_US)
          >>> src = icu.UnicodeString(
          ...     "Alice was beginning to get very tired of sitting by her sister on the bank. "
          ...     "She had nothing to do. The book had no pictures or conversations in it."
          ... )
          >>> bi.set_text(src)
          >>> start = bi.first()
          >>> while (end := bi.next()) != icu.BreakIterator.DONE:
          ...     print(src[start:end])
          ...     start = end
          ...
          Alice was beginning to get very tired of sitting by her sister on the bank.
          She had nothing to do.
          The book had no pictures or conversations in it.

          BreakIterator for word-breaks:

          >>> from icupy import icu
          >>> bi = icu.BreakIterator.create_word_instance(icu.ULOC_US)
          >>> src = icu.UnicodeString("Alice was beginning to get very tired of sitting by her sister on the bank.")
          >>> bi.set_text(src)
          >>> result: list[str] = []
          >>> start = bi.first()
          >>> while (end := bi.next()) != icu.BreakIterator.DONE:
          ...     if bi.get_rule_status() != icu.UBRK_WORD_NONE:
          ...         result.append(src[start:end])
          ...     start = end
          ...
          >>> result
          ['Alice', 'was', 'beginning', 'to', 'get', 'very', 'tired', 'of', 'sitting', 'by', 'her', 'sister', 'on', 'the', 'bank']
      )doc");

  py::enum_<decltype(BreakIterator::DONE)>(bi, "BreakIterator",
                                           py::arithmetic())
      .value("DONE", BreakIterator::DONE, R"doc(
             DONE is returned by :meth:`~BreakIterator.previous` and
             :meth:`~BreakIterator.next` after all valid boundaries have been
             returned.
             )doc")
      .export_values();

  bi.def("__copy__", &BreakIterator::clone,
         R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  bi.def(
      "__deepcopy__",
      [](const BreakIterator &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"),
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  bi.def(
      "__eq__",
      [](const BreakIterator &self, const BreakIterator &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  bi.def(
      "__iter__",
      [](BreakIterator &self) -> BreakIterator & {
        self.first();
        return self;
      },
      R"doc(
      Return an iterator object.
      )doc");

  bi.def(
      "__ne__",
      [](const BreakIterator &self, const BreakIterator &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  bi.def(
      "__next__",
      [](BreakIterator &self) {
        auto n = self.next();
        if (n == BreakIterator::DONE) {
          throw py::stop_iteration();
        }
        return n;
      },
      R"doc(
      Return the next boundary from the iterator.
      )doc");

  bi.def(
      "__reversed__",
      [](BreakIterator &self) {
        std::vector<int32_t> result;
        for (auto n = self.last(); n != 0 && n != BreakIterator::DONE;
             n = self.previous()) {
          result.push_back(n);
        }
        return result;
      },
      R"doc(
      Return a reversed iterator object.
      )doc");

  bi.def(
      "adopt_text",
      [](BreakIterator &self, CharacterIterator *it) {
        self.adoptText(it->clone());
      },
      py::arg("it").none(false), R"doc(
      Change the text over which this operates.

      The text boundary is reset to the start.

      .. note::

          An instance of ``CharacterIterator`` will be copied: changes made to
          the iterator object after passing it to ``BreakIterator`` will not be
          reflected.

      .. note::

          :meth:`.set_text` provides similar functionality to this function,
          and is more efficient.

      See Also:
          :meth:`.set_text`
      )doc");

  bi.def("clone", &BreakIterator::clone,
         R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");

  bi.def_static(
      "create_character_instance",
      [](const icupy::LocaleVariant &where) {
        ErrorCode error_code;
        auto result = BreakIterator::createCharacterInstance(
            icupy::to_locale(where), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("where"), R"doc(
      Create a ``BreakIterator`` for character-breaks using specified locale.

      Return an instance of a ``BreakIterator`` implementing character breaks.
      Character breaks are boundaries of combining character sequences.
      )doc");

  bi.def_static(
      "create_line_instance",
      [](const icupy::LocaleVariant &where) {
        ErrorCode error_code;
        auto result = BreakIterator::createLineInstance(icupy::to_locale(where),
                                                        error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("where"), R"doc(
      Create a ``BreakIterator`` for line-breaks using specified locale.

      Return an instance of a ``BreakIterator`` implementing line breaks.
      Line breaks are logically possible line breaks, actual line breaks are
      usually determined based on display width. LineBreak is useful for word
      wrapping text.
      )doc");

  bi.def_static(
      "create_sentence_instance",
      [](const icupy::LocaleVariant &where) {
        ErrorCode error_code;
        auto result = BreakIterator::createSentenceInstance(
            icupy::to_locale(where), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("where"), R"doc(
      Create a ``BreakIterator`` for sentence-breaks using specified locale.

      Return an instance of a ``BreakIterator`` implementing sentence breaks.
      )doc");

  bi.def_static(
      "create_title_instance",
      [](const icupy::LocaleVariant &where) {
        ErrorCode error_code;
        auto result = BreakIterator::createTitleInstance(
            icupy::to_locale(where), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("where"), R"doc(
      Create a ``BreakIterator`` for title-casing breaks using the specified
      locale.

      Return an instance of a ``BreakIterator`` implementing title breaks.

      The iterator returned locates title boundaries as described for
      Unicode 3.2 only. For Unicode 4.0 and above title boundary iteration,
      please use a word boundary iterator. See :meth:`.create_word_instance`.

      .. deprecated:: ICU64
          Use :meth:`.create_word_instance` instead.
      )doc");

  bi.def_static(
      "create_word_instance",
      [](const icupy::LocaleVariant &where) {
        ErrorCode error_code;
        auto result = BreakIterator::createWordInstance(icupy::to_locale(where),
                                                        error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("where"), R"doc(
      Create a ``BreakIterator`` for word-breaks using the given locale.

      Return an instance of a ``BreakIterator`` implementing word breaks.
      WordBreak is useful for word selection (ex. double click).
      )doc");

  bi.def("current", &BreakIterator::current, R"doc(
      Return the character index of the current iterator position within the
      text.
      )doc");

  bi.def("first", &BreakIterator::first, R"doc(
      Set the current iteration position to the beginning of the text,
      and return the current character index of the text.
      )doc");

  bi.def("following", &BreakIterator::following, py::arg("offset"), R"doc(
      Advance the iterator to the first boundary following the specified
      offset, and return the current character index of the text.

      The value returned is always greater than the offset or the value
      ``BreakIterator.DONE``.
      )doc");

  bi.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = BreakIterator::getAvailableLocales(count);
        std::vector<const Locale *> result(count, nullptr);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return the set of locales for which break iterators are installed.
      )doc");

  bi.def_static(
        "get_display_name",
        [](const icupy::LocaleVariant &object_locale,
           const icupy::LocaleVariant &display_locale,
           UnicodeString &name) -> UnicodeString & {
          return BreakIterator::getDisplayName(icupy::to_locale(object_locale),
                                               icupy::to_locale(display_locale),
                                               name);
        },
        py::arg("object_locale"), py::arg("display_locale"), py::arg("name"),
        R"doc(
      Return name of the object for the desired locale, in the desired
      language.
      )doc")
      .def_static(
          "get_display_name",
          [](const icupy::LocaleVariant &object_locale,
             UnicodeString &name) -> UnicodeString & {
            return BreakIterator::getDisplayName(
                icupy::to_locale(object_locale), name);
          },
          py::arg("object_locale"), py::arg("name"), R"doc(
      Return name of the object for the desired locale, in the language of the
      default locale.
      )doc");

  bi.def(
      "get_locale",
      [](const BreakIterator &self, ULocDataLocaleType type) {
        ErrorCode error_code;
        auto result = self.getLocale(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type"), R"doc(
      Return the locale for this break iterator.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  bi.def("get_rule_status", &BreakIterator::getRuleStatus, R"doc(
      For RuleBasedBreakIterators, return the status tag from the break rule
      that determined the boundary at the current iteration position.

      For break iterator types that do not support a rule status, a default
      value of 0 is returned.

      See Also:
          :class:`ULineBreakTag`
          :class:`USentenceBreakTag`
          :class:`UWordBreak`
      )doc");

  bi.def(
      "get_rule_status_vec",
      [](BreakIterator &self) {
        ErrorCode error_code;
        const auto capacity = self.getRuleStatusVec(nullptr, 0, error_code);
        std::vector<int32_t> result(capacity, 0);
        error_code.reset();
        self.getRuleStatusVec(result.data(), capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      For RuleBasedBreakIterators, return the status (tag) values from the
      break rule(s) that determined the boundary at the current iteration
      position.

      For break iterator types that do not support rule status, no values are
      returned.

      See Also:
          :class:`ULineBreakTag`
          :class:`USentenceBreakTag`
          :class:`UWordBreak`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)

  bi.def("get_text", &BreakIterator::getText,
         py::return_value_policy::reference, R"doc(
      Return a :class:`CharacterIterator` over the text being analyzed.
      )doc");

  bi.def(
      "get_utext",
      [](const BreakIterator &self, std::optional<icupy::UTextPtr> &fill_in) {
        ErrorCode error_code;
        auto p = self.getUText(fill_in.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p);
      },
      py::keep_alive<0, 1>(), py::arg("fill_in") = std::nullopt, R"doc(
      Return a :class:`UText` for the text being analyzed.

      The returned :class:`UText` is a shallow clone of the :class:`UText` used
      internally by the break iterator implementation.

      If *fill_in* is specified, it stores the shallow clone of the
      :class:`UText` used internally by the break iterator implementation.
      )doc");

  bi.def(
      "is_boundary",
      [](BreakIterator &self, int32_t offset) -> py::bool_ {
        return self.isBoundary(offset);
      },
      py::arg("offset"), R"doc(
      Return ``True`` if the specified position is a boundary position,
      ``False`` otherwise.

      The current position of the iterator is set to the first boundary
      position at or following the specified offset.
      )doc");

  bi.def("last", &BreakIterator::last, R"doc(
      Set the iterator position to the index immediately BEYOND the last
      character in the text being scanned,
      and return the current character index of the text.
      )doc");

  bi.def("next", py::overload_cast<int32_t>(&BreakIterator::next), py::arg("n"),
         R"doc(
      Set the iterator position to the nth boundary from the current boundary,
      and return the current character index of the text or
      ``BreakIterator.DONE`` if all boundaries have been returned.

      If *n* is negative, move to the previous boundary; if *n* is positive,
      move to the following boundary.
      )doc")
      .def("next", py::overload_cast<>(&BreakIterator::next), R"doc(
      Advance the iterator to the boundary following the current boundary,
      and return the current character index of the text or
      ``BreakIterator.DONE`` if all boundaries have been returned.
      )doc");

  bi.def("preceding", &BreakIterator::preceding, py::arg("offset"), R"doc(
      Set the iterator position to the first boundary preceding the specified
      offset, and return the current character index of the text.

      The value returned is always smaller than the offset or the value
      ``BreakIterator.DONE``.
      )doc");

  bi.def("previous", &BreakIterator::previous, R"doc(
      Set the iterator position to the boundary preceding the current boundary,
      and return the current character index of the text or
      ``BreakIterator.DONE`` if all boundaries have been returned.
      )doc");

  // FIXME: Implement "BreakIterator& BreakIterator::refreshInputText(
  //  UText *input, UErrorCode &status)".

  // TODO: Implement "static URegistryKey BreakIterator::registerInstance(
  //  BreakIterator *toAdopt, const Locale &locale, UBreakIteratorType kind,
  //  UErrorCode &status)".

  bi.def("set_text",
         py::overload_cast<const UnicodeString &>(&BreakIterator::setText),
         py::arg("text"), R"doc(
      Change the text over which this operates.

      The text boundary is reset to the start.

      .. important::

          *text* must outlive the break iterator object.
      )doc")
      .def(
          "set_text",
          [](BreakIterator &self, icupy::UTextPtr &text) {
            ErrorCode error_code;
            self.setText(text, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), R"doc(
      Change the text over which this operates.

      The text boundary is reset to the start.

      .. important::

          This function makes a shallow clone of the specified :class:`UText`.
          The caller can immediately close or reuse the :class:`UText` passed
          as a parameter, but the underlying text itself must outlive the break
          iterator object.
      )doc");

  // TODO: Implement "static UBool BreakIterator::unregister(URegistryKey key,
  //  UErrorCode &status)".

  //
  // class icu::RuleBasedBreakIterator
  //
  py::class_<RuleBasedBreakIterator, BreakIterator> rbbi(
      m, "RuleBasedBreakIterator", R"doc(
      Subclass of :class:`BreakIterator` that determines text boundaries based
      on a rule set.

      For more information, see the ICU User Guide:
      `Break Rules <https://unicode-org.github.io/icu/userguide/boundaryanalysis/break-rules.html>`__.
      )doc");

  rbbi.def(
          // [2] RuleBasedBreakIterator::RuleBasedBreakIterator
          py::init<const RuleBasedBreakIterator &>(), py::arg("other"), R"doc(
      Initialize a ``RuleBasedBreakIterator`` instance from another
      ``RuleBasedBreakIterator``.
      )doc")
      .def(
          // [3] RuleBasedBreakIterator::RuleBasedBreakIterator
          py::init([](const icupy::UnicodeStringVariant &rules,
                      UParseError &parse_error) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedBreakIterator>(
                icupy::to_unistr(rules), parse_error, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("parse_error"), R"doc(
      Initialize a ``RuleBasedBreakIterator`` instance from a set of rules
      specified as a string.

      If an error occurs while parsing the rules string, the offset into the
      rules string at which the error occurred will be saved into the
      :class:`UParseError`.
      )doc")
      .def(
          // [4] RuleBasedBreakIterator::RuleBasedBreakIterator
          py::init([](const py::buffer &compiled_rules, uint32_t rule_length) {
            auto info = compiled_rules.request();
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedBreakIterator>(
                reinterpret_cast<uint8_t *>(info.ptr), rule_length, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::keep_alive<1, 2>(), py::arg("compiled_rules"),
          py::arg("rule_length"), R"doc(
      Initialize a ``RuleBasedBreakIterator`` instance from a set of
      precompiled binary rules.

      .. important::

          *compiled_rules* must outlive the break iterator object.

      .. note::

          The compiled rules are not compatible across different major versions
          of ICU. The compiled rules are compatible only between machines with
          the same byte ordering (little or big endian) and the same base
          character set family (ASCII or EBCDIC).

      .. seealso::

          :meth:`.get_binary_rules`
      )doc");

  rbbi.def("__hash__", &RuleBasedBreakIterator::hashCode, R"doc(
      Return the hash value of this object.

      This is equivalent to :meth:`hash_code`.
      )doc");

  rbbi.def("clone", &RuleBasedBreakIterator::clone,
           R"doc(
      Return a copy of this object.

      See Also:
          :meth:`~BreakIterator.__copy__`
          :meth:`~BreakIterator.__deepcopy__`
      )doc");

  rbbi.def(
      "get_binary_rules",
      [](RuleBasedBreakIterator &self) {
        uint32_t length;
        auto p = self.getBinaryRules(length);
        return py::memoryview::from_memory(const_cast<uint8_t *>(p),
                                           sizeof(uint8_t) * length, true);
      },
      py::keep_alive<0, 1>(),
      R"doc(
      Return the binary form of compiled break rules.

      .. note::

          The compiled rules are not compatible across different major versions
          of ICU. The compiled rules are compatible only between machines with
          the same byte ordering (little or big endian) and the same base
          character set family (ASCII or EBCDIC).
      )doc");

  rbbi.def("get_rules", &RuleBasedBreakIterator::getRules, R"doc(
      Return the description used to create this break iterator.
      )doc");

  rbbi.def("hash_code", &RuleBasedBreakIterator::hashCode, R"doc(
      Return a hash code for this :class:`BreakIterator`.

      See Also:
          :meth:`.__hash__`
      )doc");

  // FIXME: Implement "BreakIterator& refreshInputText(UText *input,
  //  UErrorCode &status)".
}

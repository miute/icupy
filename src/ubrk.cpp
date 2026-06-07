#include "main.hpp"
#include <pybind11/native_enum.h>
#include <unicode/ubrk.h>

void init_ubrk(py::module &m) {
  //
  // enum UBreakIteratorType
  //
  py::native_enum<UBreakIteratorType>(m, "UBreakIteratorType", "enum.IntEnum",
                                      R"doc(
The possible types of text boundaries.
      )doc")
      .value("UBRK_CHARACTER", UBRK_CHARACTER, R"doc(
             Character breaks.
             )doc")
      .value("UBRK_WORD", UBRK_WORD, R"doc(
             Word breaks.
             )doc")
      .value("UBRK_LINE", UBRK_LINE, R"doc(
             Line breaks.
             )doc")
      .value("UBRK_SENTENCE", UBRK_SENTENCE, R"doc(
             Sentence breaks.
             )doc")
      .export_values()
      .finalize();

  //
  // enum ULineBreakTag
  //
  py::native_enum<ULineBreakTag>(m, "ULineBreakTag", "enum.IntEnum", R"doc(
Enum constants for the line break tags returned by
:meth:`BreakIterator.get_rule_status`.

A range of values is defined for each category of word, to allow for further
subdivisions of a category in future releases.
Applications should check for tag values falling within the range, rather than
for single individual values.

The numeric values of all of these constants are stable (will not change).
      )doc")
      .value("UBRK_LINE_SOFT", UBRK_LINE_SOFT,
             R"doc(
             Tag value for soft line breaks, positions at which a line break
             is acceptable but not required.
             )doc")
      .value("UBRK_LINE_SOFT_LIMIT", UBRK_LINE_SOFT_LIMIT, R"doc(
             Upper bound for soft line breaks.
             )doc")
      .value("UBRK_LINE_HARD", UBRK_LINE_HARD, R"doc(
             :attr:`UBRK_LINE_HARD` is same as :attr:`UBRK_LINE_SOFT_LIMIT`.

             :attr:`UBRK_LINE_SOFT_LIMIT`:
             upper bound for soft line breaks.

             :attr:`UBRK_LINE_HARD`:
             tag value for a hard, or mandatory line break.
             )doc")
      .value("UBRK_LINE_HARD_LIMIT", UBRK_LINE_HARD_LIMIT, R"doc(
             Upper bound for hard line breaks.
             )doc")
      .export_values()
      .finalize();

  //
  // enum USentenceBreakTag
  //
  py::native_enum<USentenceBreakTag>(m, "USentenceBreakTag", "enum.IntEnum",
                                     R"doc(
Enum constants for the sentence break tags returned by
:meth:`BreakIterator.get_rule_status`.

A range of values is defined for each category of sentence, to allow for
further subdivisions of a category in future releases.
Applications should check for tag values falling within the range, rather than
for single individual values.

The numeric values of all of these constants are stable (will not change).
      )doc")
      .value("UBRK_SENTENCE_TERM", UBRK_SENTENCE_TERM, R"doc(
             Tag value for for sentences ending with a sentence terminator
             ('.', '?', '!', etc.) character, possibly followed by a hard
             separator (CR, LF, PS, etc.)
             )doc")
      .value("UBRK_SENTENCE_TERM_LIMIT", UBRK_SENTENCE_TERM_LIMIT, R"doc(
             Upper bound for tags for sentences ended by sentence terminators.
             )doc")
      .value("UBRK_SENTENCE_SEP", UBRK_SENTENCE_SEP, R"doc(
             :attr:`UBRK_SENTENCE_SEP` is same as
             :attr:`UBRK_SENTENCE_TERM_LIMIT`.

             :attr:`UBRK_SENTENCE_SEP`:
             tag value for for sentences that do not contain an ending sentence
             terminator ('.', '?', '!', etc.) character, but are ended only by
             a hard separator (CR, LF, PS, etc.) or end of input.

             :attr:`UBRK_SENTENCE_TERM_LIMIT`:
             upper bound for tags for sentences ended by sentence terminators.
             )doc")
      .value("UBRK_SENTENCE_SEP_LIMIT", UBRK_SENTENCE_SEP_LIMIT, R"doc(
             Upper bound for tags for sentences ended by a separator.
             )doc")
      .export_values()
      .finalize();

  //
  // enum UWordBreak
  //
  py::native_enum<UWordBreak>(m, "UWordBreak", "enum.IntEnum", R"doc(
Enum constants for the word break tags returned by
:meth:`BreakIterator.get_rule_status`.

A range of values is defined for each category of word, to allow for further
subdivisions of a category in future releases.
Applications should check for tag values falling within the range, rather than
for single individual values.

The numeric values of all of these constants are stable (will not change).
      )doc")
      .value("UBRK_WORD_NONE", UBRK_WORD_NONE, R"doc(
             Tag value for "words" that do not fit into any of other
             categories.

             Includes spaces and most punctuation.
             )doc")
      .value("UBRK_WORD_NONE_LIMIT", UBRK_WORD_NONE_LIMIT, R"doc(
             Upper bound for tags for uncategorized words.
             )doc")
      .value("UBRK_WORD_NUMBER", UBRK_WORD_NUMBER, R"doc(
             :attr:`UBRK_WORD_NUMBER` is same as :attr:`UBRK_WORD_NONE_LIMIT`.

             :attr:`UBRK_WORD_NUMBER`:
             tag value for words that appear to be numbers, lower limit.

             :attr:`UBRK_WORD_NONE_LIMIT`:
             upper bound for tags for uncategorized words.
             )doc")
      .value("UBRK_WORD_NUMBER_LIMIT", UBRK_WORD_NUMBER_LIMIT, R"doc(
             Tag value for words that appear to be numbers, upper limit.
             )doc")
      .value("UBRK_WORD_LETTER", UBRK_WORD_LETTER, R"doc(
             :attr:`UBRK_WORD_LETTER` is same as
             :attr:`UBRK_WORD_NUMBER_LIMIT`.

             :attr:`UBRK_WORD_LETTER`:
             tag value for words that contain letters, excluding hiragana,
             katakana or ideographic characters, lower limit.

             :attr:`UBRK_WORD_NUMBER_LIMIT`:
             upper bound for tags for words that appear to be numbers.
             )doc")
      .value("UBRK_WORD_LETTER_LIMIT", UBRK_WORD_LETTER_LIMIT, R"doc(
             Tag value for words containing letters, upper limit.
             )doc")
      .value("UBRK_WORD_KANA", UBRK_WORD_KANA, R"doc(
             :attr:`UBRK_WORD_KANA` is same as :attr:`UBRK_WORD_LETTER_LIMIT`.

             :attr:`UBRK_WORD_KANA`:
             tag value for words containing kana characters, lower limit.

             :attr:`UBRK_WORD_LETTER_LIMIT`:
             upper bound for tags for words that contain letters.
             )doc")
      .value("UBRK_WORD_KANA_LIMIT", UBRK_WORD_KANA_LIMIT, R"doc(
             Tag value for words containing kana characters, upper limit.
             )doc")
      .value("UBRK_WORD_IDEO", UBRK_WORD_IDEO, R"doc(
             :attr:`UBRK_WORD_IDEO` is same as :attr:`UBRK_WORD_KANA_LIMIT`.

             :attr:`UBRK_WORD_IDEO`:
             tag value for words containing ideographic characters, lower limit.

             :attr:`UBRK_WORD_KANA_LIMIT`:
             upper bound for tags for words that contain kana characters.
             )doc")
      .value("UBRK_WORD_IDEO_LIMIT", UBRK_WORD_IDEO_LIMIT, R"doc(
             Tag value for words containing ideographic characters, upper limit.
             )doc")
      .export_values()
      .finalize();

  m.attr("UBRK_DONE") = UBRK_DONE;
}

#include "main.hpp"
#include <unicode/ubrk.h>

void init_ubrk(py::module &m) {
  //
  // enum UBreakIteratorType
  //
  py::enum_<UBreakIteratorType>(m, "UBreakIteratorType", py::arithmetic(),
                                "The possible types of text boundaries.")
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
      .export_values();

  //
  // enum ULineBreakTag
  //
  py::enum_<ULineBreakTag>(m, "ULineBreakTag", py::arithmetic(), R"doc(
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
             Tag value for a hard, or mandatory line break.
             )doc")
      .value("UBRK_LINE_HARD_LIMIT", UBRK_LINE_HARD_LIMIT, R"doc(
             Upper bound for hard line breaks.
             )doc")
      .export_values();

  //
  // enum USentenceBreakTag
  //
  py::enum_<USentenceBreakTag>(m, "USentenceBreakTag", py::arithmetic(), R"doc(
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
             Tag value for for sentences that do not contain an ending sentence
             terminator ('.', '?', '!', etc.) character, but are ended only by
             a hard separator (CR, LF, PS, etc.) or end of input.
             )doc")
      .value("UBRK_SENTENCE_SEP_LIMIT", UBRK_SENTENCE_SEP_LIMIT, R"doc(
             Upper bound for tags for sentences ended by a separator.
             )doc")
      .export_values();

  //
  // enum UWordBreak
  //
  py::enum_<UWordBreak>(m, "UWordBreak", py::arithmetic(), R"doc(
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
             Tag value for words that appear to be numbers, lower limit.
             )doc")
      .value("UBRK_WORD_NUMBER_LIMIT", UBRK_WORD_NUMBER_LIMIT, R"doc(
             Tag value for words that appear to be numbers, upper limit.
             )doc")
      .value("UBRK_WORD_LETTER", UBRK_WORD_LETTER, R"doc(
             Tag value for words that contain letters, excluding hiragana,
             katakana or ideographic characters, lower limit.
             )doc")
      .value("UBRK_WORD_LETTER_LIMIT", UBRK_WORD_LETTER_LIMIT, R"doc(
             Tag value for words containing letters, upper limit.
             )doc")
      .value("UBRK_WORD_KANA", UBRK_WORD_KANA, R"doc(
             Tag value for words containing kana characters, lower limit.
             )doc")
      .value("UBRK_WORD_KANA_LIMIT", UBRK_WORD_KANA_LIMIT, R"doc(
             Tag value for words containing kana characters, upper limit.
             )doc")
      .value("UBRK_WORD_IDEO", UBRK_WORD_IDEO, R"doc(
             Tag value for words containing ideographic characters, lower limit.
             )doc")
      .value("UBRK_WORD_IDEO_LIMIT", UBRK_WORD_IDEO_LIMIT, R"doc(
             Tag value for words containing ideographic characters, upper limit.
             )doc")
      .export_values();

  m.attr("UBRK_DONE") = UBRK_DONE;
}

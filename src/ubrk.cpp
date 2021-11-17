#include "main.hpp"
#include <unicode/ubrk.h>

void init_ubrk(py::module &m) {
  py::enum_<UBreakIteratorType>(m, "UBreakIteratorType", py::arithmetic(), "The possible types of text boundaries.")
      .value("UBRK_CHARACTER", UBRK_CHARACTER, "Character breaks.")
      .value("UBRK_WORD", UBRK_WORD, "Word breaks.")
      .value("UBRK_LINE", UBRK_LINE, "Line breaks.")
      .value("UBRK_SENTENCE", UBRK_SENTENCE, "Sentence breaks.")
      .export_values();

  py::enum_<ULineBreakTag>(m, "ULineBreakTag", py::arithmetic(),
                           "Enum constants for the line break tags returned by *get_rule_status()*.\n\n"
                           "A range of values is defined for each category of word, to allow for further subdivisions "
                           "of a category in future releases. Applications should check for tag values falling within "
                           "the range, rather than for single individual values.\n\n"
                           "The numeric values of all of these constants are stable (will not change).")
      .value("UBRK_LINE_SOFT", UBRK_LINE_SOFT,
             "Tag value for soft line breaks, positions at which a line break is acceptable but not required.")
      .value("UBRK_LINE_SOFT_LIMIT", UBRK_LINE_SOFT_LIMIT, "Upper bound for soft line breaks.")
      .value("UBRK_LINE_HARD", UBRK_LINE_HARD, "Tag value for a hard, or mandatory line break.")
      .value("UBRK_LINE_HARD_LIMIT", UBRK_LINE_HARD_LIMIT, "Upper bound for hard line breaks.")
      .export_values();

  py::enum_<USentenceBreakTag>(m, "USentenceBreakTag", py::arithmetic(),
                               "Enum constants for the sentence break tags returned by *get_rule_status()*.\n\n"
                               "A range of values is defined for each category of sentence, to allow for further "
                               "subdivisions of a category in future releases. Applications should check for tag "
                               "values falling within the range, rather than for single individual values.\n\n"
                               "The numeric values of all of these constants are stable (will not change).")
      .value("UBRK_SENTENCE_TERM", UBRK_SENTENCE_TERM,
             "Tag value for for sentences ending with a sentence terminator ('.', '?', '!', etc.) character, possibly "
             "followed by a hard separator (CR, LF, PS, etc.)")
      .value("UBRK_SENTENCE_TERM_LIMIT", UBRK_SENTENCE_TERM_LIMIT,
             "Upper bound for tags for sentences ended by sentence terminators.")
      .value("UBRK_SENTENCE_SEP", UBRK_SENTENCE_SEP,
             "Tag value for for sentences that do not contain an ending sentence terminator ('.', '?', '!', etc.) "
             "character, but are ended only by a hard separator (CR, LF, PS, etc.) or end of input.")
      .value("UBRK_SENTENCE_SEP_LIMIT", UBRK_SENTENCE_SEP_LIMIT,
             "Upper bound for tags for sentences ended by a separator.")
      .export_values();

  py::enum_<UWordBreak>(m, "UWordBreak", py::arithmetic(),
                        "Enum constants for the word break tags returned by *get_rule_status()*.\n\n"
                        "A range of values is defined for each category of word, to allow for further subdivisions of "
                        "a category in future releases. Applications should check for tag values falling within the "
                        "range, rather than for single individual values.\n\n"
                        "The numeric values of all of these constants are stable (will not change).")
      .value("UBRK_WORD_NONE", UBRK_WORD_NONE,
             "Tag value for \"words\" that do not fit into any of other categories.\n\n  "
             "Includes spaces and most punctuation.")
      .value("UBRK_WORD_NONE_LIMIT", UBRK_WORD_NONE_LIMIT, "Upper bound for tags for uncategorized words.")
      .value("UBRK_WORD_NUMBER", UBRK_WORD_NUMBER, "Tag value for words that appear to be numbers, lower limit.")
      .value("UBRK_WORD_NUMBER_LIMIT", UBRK_WORD_NUMBER_LIMIT,
             "Tag value for words that appear to be numbers, upper limit.")
      .value("UBRK_WORD_LETTER", UBRK_WORD_LETTER,
             "Tag value for words that contain letters, excluding hiragana, katakana or ideographic characters, lower "
             "limit.")
      .value("UBRK_WORD_LETTER_LIMIT", UBRK_WORD_LETTER_LIMIT, "Tag value for words containing letters, upper limit.")
      .value("UBRK_WORD_KANA", UBRK_WORD_KANA, "Tag value for words containing kana characters, lower limit.")
      .value("UBRK_WORD_KANA_LIMIT", UBRK_WORD_KANA_LIMIT,
             "Tag value for words containing kana characters, upper limit.")
      .value("UBRK_WORD_IDEO", UBRK_WORD_IDEO, "Tag value for words containing ideographic characters, lower limit.")
      .value("UBRK_WORD_IDEO_LIMIT", UBRK_WORD_IDEO_LIMIT,
             "Tag value for words containing ideographic characters, upper limit.")
      .export_values();

  m.attr("UBRK_DONE") = UBRK_DONE;
}

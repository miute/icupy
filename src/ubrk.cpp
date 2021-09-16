#include "main.hpp"
#include <unicode/ubrk.h>

void init_ubrk(py::module &m) {
  py::enum_<UBreakIteratorType>(m, "UBreakIteratorType", py::arithmetic())
      .value("UBRK_CHARACTER", UBRK_CHARACTER)
      .value("UBRK_WORD", UBRK_WORD)
      .value("UBRK_LINE", UBRK_LINE)
      .value("UBRK_SENTENCE", UBRK_SENTENCE)
      .export_values();

  py::enum_<ULineBreakTag>(m, "ULineBreakTag", py::arithmetic())
      .value("UBRK_LINE_SOFT", UBRK_LINE_SOFT)
      .value("UBRK_LINE_SOFT_LIMIT", UBRK_LINE_SOFT_LIMIT)
      .value("UBRK_LINE_HARD", UBRK_LINE_HARD)
      .value("UBRK_LINE_HARD_LIMIT", UBRK_LINE_HARD_LIMIT)
      .export_values();

  py::enum_<USentenceBreakTag>(m, "USentenceBreakTag", py::arithmetic())
      .value("UBRK_SENTENCE_TERM", UBRK_SENTENCE_TERM)
      .value("UBRK_SENTENCE_TERM_LIMIT", UBRK_SENTENCE_TERM_LIMIT)
      .value("UBRK_SENTENCE_SEP", UBRK_SENTENCE_SEP)
      .value("UBRK_SENTENCE_SEP_LIMIT", UBRK_SENTENCE_SEP_LIMIT)
      .export_values();

  py::enum_<UWordBreak>(m, "UWordBreak", py::arithmetic())
      .value("UBRK_WORD_NONE", UBRK_WORD_NONE)
      .value("UBRK_WORD_NONE_LIMIT", UBRK_WORD_NONE_LIMIT)
      .value("UBRK_WORD_NUMBER", UBRK_WORD_NUMBER)
      .value("UBRK_WORD_NUMBER_LIMIT", UBRK_WORD_NUMBER_LIMIT)
      .value("UBRK_WORD_LETTER", UBRK_WORD_LETTER)
      .value("UBRK_WORD_LETTER_LIMIT", UBRK_WORD_LETTER_LIMIT)
      .value("UBRK_WORD_KANA", UBRK_WORD_KANA)
      .value("UBRK_WORD_KANA_LIMIT", UBRK_WORD_KANA_LIMIT)
      .value("UBRK_WORD_IDEO", UBRK_WORD_IDEO)
      .value("UBRK_WORD_IDEO_LIMIT", UBRK_WORD_IDEO_LIMIT)
      .export_values();

  m.attr("UBRK_DONE") = UBRK_DONE;
}

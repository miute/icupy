#include "main.hpp"
#include "usetptr.hpp"
#include <memory>

_ConstUSetPtr::_ConstUSetPtr(const USet *p) : p_(p) {}
_ConstUSetPtr::~_ConstUSetPtr() {}
const USet *_ConstUSetPtr::get() const { return p_; }

_USetPtr::_USetPtr(USet *p) : p_(p) {}
_USetPtr::~_USetPtr() {}
USet *_USetPtr::get() const { return p_; }

void init_uset(py::module &m) {
  py::enum_<USetSpanCondition>(
      m, "USetSpanCondition", py::arithmetic(),
      "Argument values for whether *span()* and similar functions continue while the current character is contained "
      "vs. not contained in the set.\n\n"
      "The functionality is straightforward for sets with only single code points, without strings (which is the "
      "common case):\n\n"
      "* USET_SPAN_CONTAINED and USET_SPAN_SIMPLE work the same.\n"
      "* USET_SPAN_CONTAINED and USET_SPAN_SIMPLE are inverses of USET_SPAN_NOT_CONTAINED.\n"
      "* span() and span_back() partition any string the same way when alternating between "
      "span(USET_SPAN_NOT_CONTAINED) and span(either \"contained\" condition).\n"
      "* Using a complemented (inverted) set and the opposite span conditions yields the same results.\n\n"
      "When a set contains multi-code point strings, then these statements may not be true, depending on the strings "
      "in the set (for example, whether they overlap with each other) and the string that is processed. For a set with "
      "strings:\n\n"
      "* The complement of the set contains the opposite set of code points, but the same set of strings. Therefore, "
      "complementing both the set and the span conditions may yield different results.\n"
      "* When starting spans at different positions in a string (span(s, ...) vs. span(s+1, ...)) the ends of the "
      "spans may be different because a set string may start before the later position.\n"
      "* span(USET_SPAN_SIMPLE) may be shorter than span(USET_SPAN_CONTAINED) because it will not recursively try all "
      "possible paths. For example, with a set which contains the three strings \"xy\", \"xya\" and \"ax\", "
      "span(\"xyax\", USET_SPAN_CONTAINED) will return 4 but span(\"xyax\", USET_SPAN_SIMPLE) will return 3. "
      "span(USET_SPAN_SIMPLE) will never be longer than span(USET_SPAN_CONTAINED).\n"
      "* With either \"contained\" condition, span() and spanBack() may partition a string in different ways. For "
      "example, with a set which contains the two strings \"ab\" and \"ba\", and when processing the string \"aba\", "
      "span() will yield contained/not-contained boundaries of { 0, 2, 3 } while span_back() will yield boundaries of "
      "{ 0, 1, 3 }.\n\n"
      "Note: If it is important to get the same boundaries whether iterating forward or backward through a string, "
      "then either only span() should be used and the boundaries cached for backward operation, or an ICU "
      "BreakIterator could be used.\n\n"
      "Note: Unpaired surrogates are treated like surrogate code points. Similarly, set strings match only on code "
      "point boundaries, never in the middle of a surrogate pair. Illegal UTF-8 sequences are treated like U+FFFD. "
      "When processing UTF-8 strings, malformed set strings (strings with unpaired surrogates which cannot be "
      "converted to UTF-8) are ignored.")
      .value("USET_SPAN_NOT_CONTAINED", USET_SPAN_NOT_CONTAINED,
             "Continues a *span()* while there is no set element at the current position.\n\n  "
             "Increments by one code point at a time. Stops before the first set element (character or string). (For "
             "code points only, this is like *while not contains(current)*).\n\n  "
             "When *span()* returns, the substring between where it started and the position it returned consists only "
             "of characters that are not in the set, and none of its strings overlap with the span.")
      .value("USET_SPAN_CONTAINED", USET_SPAN_CONTAINED,
             "Spans the longest substring that is a concatenation of set elements (characters or strings).\n\n  "
             "(For characters only, this is like *while contains(current)*).\n\n  "
             "When *span()* returns, the substring between where it started and the position it returned consists "
             "only of set elements (characters or strings) that are in the set.\n\n  "
             "If a set contains strings, then the span will be the longest substring for which there exists at least "
             "one non-overlapping concatenation of set elements (characters or strings). This is equivalent to a POSIX "
             "regular expression for (OR of each set element). (Java/ICU/Perl regex stops at the first match of an "
             "OR.)")
      .value("USET_SPAN_SIMPLE", USET_SPAN_SIMPLE,
             "Continues a *span()* while there is a set element at the current position.\n\n  "
             "Increments by the longest matching element at each position. (For characters only, this is like *while "
             "contains(current)*).\n\n  "
             "When *span()* returns, the substring between where it started and the position it returned consists only "
             "of set elements (characters or strings) that are in the set.\n\n  "
             "If a set only contains single characters, then this is the same as *USET_SPAN_CONTAINED*.\n\n  "
             "If a set contains strings, then the span will be the longest substring with a match at each position "
             "with the longest single set element (character or string).\n\n  "
             "Use this span condition together with other longest-match algorithms, such as ICU converters "
             "(*ucnv_get_unicode_set()*).")
#ifndef U_HIDE_DEPRECATED_API
      .value("USET_SPAN_CONDITION_COUNT", USET_SPAN_CONDITION_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::class_<_ConstUSetPtr>(m, "_ConstUSetPtr");

  py::class_<_USetPtr>(m, "_USetPtr");

  py::class_<USerializedSet>(m, "USerializedSet").def(py::init<>());

  m.attr("USET_IGNORE_SPACE") = int32_t{USET_IGNORE_SPACE};
  m.attr("USET_CASE_INSENSITIVE") = int32_t{USET_CASE_INSENSITIVE};
  m.attr("USET_ADD_CASE_MAPPINGS") = int32_t{USET_ADD_CASE_MAPPINGS};

  m.attr("USET_SERIALIZED_STATIC_ARRAY_CAPACITY") = int32_t{USET_SERIALIZED_STATIC_ARRAY_CAPACITY};
}

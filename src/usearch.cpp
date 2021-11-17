#include "main.hpp"
#include <unicode/usearch.h>

void init_usearch(py::module &m) {
  py::enum_<USearchAttribute>(m, "USearchAttribute", py::arithmetic())
      .value("USEARCH_OVERLAP", USEARCH_OVERLAP, "Option for overlapping matches.")
#ifndef U_HIDE_DEPRECATED_API
      .value("USEARCH_CANONICAL_MATCH", USEARCH_CANONICAL_MATCH, "**Deprecated:** ICU 53")
#endif // U_HIDE_DEPRECATED_API
      .value("USEARCH_ELEMENT_COMPARISON", USEARCH_ELEMENT_COMPARISON,
             "Option to control how collation elements are compared.\n\n  "
             "The default value will be *USEARCH_STANDARD_ELEMENT_COMPARISON*.")
#ifndef U_HIDE_DEPRECATED_API
      .value("USEARCH_ATTRIBUTE_COUNT", USEARCH_ATTRIBUTE_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<USearchAttributeValue>(m, "USearchAttributeValue", py::arithmetic())
      .value("USEARCH_DEFAULT", USEARCH_DEFAULT, "Default value for any *USearchAttribute*.")
      .value("USEARCH_OFF", USEARCH_OFF, "Value for *USEARCH_OVERLAP* and *USEARCH_CANONICAL_MATCH*.")
      .value("USEARCH_ON", USEARCH_ON, "Value for *USEARCH_OVERLAP* and *USEARCH_CANONICAL_MATCH*.")
      .value("USEARCH_STANDARD_ELEMENT_COMPARISON", USEARCH_STANDARD_ELEMENT_COMPARISON,
             "Value (default) for *USEARCH_ELEMENT_COMPARISON*; standard collation element comparison at the specified "
             "collator strength.")
      .value("USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD", USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD,
             "Value for *USEARCH_ELEMENT_COMPARISON*; collation element comparison is modified to effectively provide "
             "behavior between the specified strength and strength - 1.\n\n  "
             "Collation elements in the pattern that have the base weight for the specified strength are treated as "
             "\"wildcards\" that match an element with any other weight at that collation level in the searched text. "
             "For example, with a secondary-strength English collator, a plain 'e' in the pattern will match a plain e "
             "or an e with any diacritic in the searched text, but an e with diacritic in the pattern will only match "
             "an e with the same diacritic in the searched text.\n\n  "
             "This supports \"asymmetric search\" as described in UTS #10 Unicode Collation Algorithm.")
      .value("USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD", USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD,
             "Value for *USEARCH_ELEMENT_COMPARISON*.\n\n  "
             "Collation element comparison is modified to effectively provide behavior between the specified strength "
             "and strength - 1. Collation elements in either the pattern or the searched text that have the base "
             "weight for the specified strength are treated as \"wildcards\" that match an element with any other "
             "weight at that collation level. For example, with a secondary-strength English collator, a plain 'e' in "
             "the pattern will match a plain e or an e with any diacritic in the searched text, but an e with "
             "diacritic in the pattern will only match an e with the same diacritic or a plain e in the searched "
             "text.\n\n  "
             "This option is similar to \"asymmetric search\" as described in UTS #10 Unicode Collation Algorithm, but "
             "also allows unmarked characters in the searched text to match marked or unmarked versions of that "
             "character in the pattern.")
#ifndef U_HIDE_DEPRECATED_API
      .value("USEARCH_ATTRIBUTE_VALUE_COUNT", USEARCH_ATTRIBUTE_VALUE_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  m.attr("USEARCH_DONE") = USEARCH_DONE;
}

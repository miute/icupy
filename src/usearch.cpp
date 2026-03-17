#include "main.hpp"
#include <unicode/usearch.h>

void init_usearch(py::module &m) {
  //
  // enum USearchAttribute
  //
  py::enum_<USearchAttribute>(m, "USearchAttribute", py::arithmetic(), R"doc(
Enum constants for the text searching attributes.
)doc")
      .value("USEARCH_OVERLAP", USEARCH_OVERLAP, R"doc(
             Option for overlapping matches.
             )doc")
      .value("USEARCH_CANONICAL_MATCH", USEARCH_CANONICAL_MATCH, R"doc(
             Deprecated: ICU 53
             )doc")
      .value("USEARCH_ELEMENT_COMPARISON", USEARCH_ELEMENT_COMPARISON, R"doc(
             Option to control how collation elements are compared.

             The default value will be
             :attr:`~USearchAttributeValue.USEARCH_STANDARD_ELEMENT_COMPARISON`.
             )doc")
      .value("USEARCH_ATTRIBUTE_COUNT", USEARCH_ATTRIBUTE_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum USearchAttributeValue
  //
  py::enum_<USearchAttributeValue>(m, "USearchAttributeValue", py::arithmetic(),
                                   R"doc(
Enum constants for the text searching attribute values.
)doc")
      .value("USEARCH_DEFAULT", USEARCH_DEFAULT, R"doc(
             Default value for any :class:`USearchAttribute`.
             )doc")
      .value("USEARCH_OFF", USEARCH_OFF, R"doc(
             Value for :attr:`~USearchAttribute.USEARCH_OVERLAP` and
             :attr:`~USearchAttribute.USEARCH_CANONICAL_MATCH`.
             )doc")
      .value("USEARCH_ON", USEARCH_ON, R"doc(
             Value for :attr:`~USearchAttribute.USEARCH_OVERLAP` and
             :attr:`~USearchAttribute.USEARCH_CANONICAL_MATCH`.
             )doc")
      .value("USEARCH_STANDARD_ELEMENT_COMPARISON",
             USEARCH_STANDARD_ELEMENT_COMPARISON, R"doc(
             Value (default) for
             :attr:`~USearchAttribute.USEARCH_ELEMENT_COMPARISON`; standard
             collation element comparison at the specified collator strength.
             )doc")
      .value("USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD",
             USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD, R"doc(
             Value for :attr:`~USearchAttribute.USEARCH_ELEMENT_COMPARISON`;
             collation element comparison is modified to effectively provide
             behavior between the specified strength and strength - 1.

             Collation elements in the pattern that have the base weight for
             the specified strength are treated as "wildcards" that match an
             element with any other weight at that collation level in the
             searched text.
             For example, with a secondary-strength English collator, a plain
             'e' in the pattern will match a plain e or an e with any diacritic
             in the searched text, but an e with diacritic in the pattern will
             only match an e with the same diacritic in the searched text.

             This supports "asymmetric search" as described in UTS #10
             Unicode Collation Algorithm.
             )doc")
      .value("USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD",
             USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD, R"doc(
             Value for :attr:`~USearchAttribute.USEARCH_ELEMENT_COMPARISON`.

             Collation element comparison is modified to effectively provide
             behavior between the specified strength and strength - 1.
             Collation elements in either the pattern or the searched text that
             have the base weight for the specified strength are treated as
             "wildcards" that match an element with any other weight at that
             collation level. For example, with a secondary-strength English
             collator, a plain 'e' in the pattern will match a plain e or an e
             with any diacritic in the searched text, but an e with diacritic
             in the pattern will only match an e with the same diacritic or a
             plain e in the searched text.

             This option is similar to "asymmetric search" as described in
             UTS #10 Unicode Collation Algorithm, but also allows unmarked
             characters in the searched text to match marked or unmarked
             versions of that character in the pattern.
             )doc")
      .value("USEARCH_ATTRIBUTE_VALUE_COUNT", USEARCH_ATTRIBUTE_VALUE_COUNT,
             R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  m.attr("USEARCH_DONE") = USEARCH_DONE;
}

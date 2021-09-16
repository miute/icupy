#include "main.hpp"
#include <unicode/usearch.h>

void init_usearch(py::module &m) {
  py::enum_<USearchAttribute>(m, "USearchAttribute", py::arithmetic())
      .value("USEARCH_OVERLAP", USEARCH_OVERLAP)
#ifndef U_HIDE_DEPRECATED_API
      .value("USEARCH_CANONICAL_MATCH", USEARCH_CANONICAL_MATCH)
#endif // U_HIDE_DEPRECATED_API
      .value("USEARCH_ELEMENT_COMPARISON", USEARCH_ELEMENT_COMPARISON)
#ifndef U_HIDE_DEPRECATED_API
      .value("USEARCH_ATTRIBUTE_COUNT", USEARCH_ATTRIBUTE_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<USearchAttributeValue>(m, "USearchAttributeValue", py::arithmetic())
      .value("USEARCH_DEFAULT", USEARCH_DEFAULT)
      .value("USEARCH_OFF", USEARCH_OFF)
      .value("USEARCH_ON", USEARCH_ON)
      .value("USEARCH_STANDARD_ELEMENT_COMPARISON", USEARCH_STANDARD_ELEMENT_COMPARISON)
      .value("USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD", USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD)
      .value("USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD", USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD)
#ifndef U_HIDE_DEPRECATED_API
      .value("USEARCH_ATTRIBUTE_VALUE_COUNT", USEARCH_ATTRIBUTE_VALUE_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  m.attr("USEARCH_DONE") = USEARCH_DONE;
}

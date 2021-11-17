#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
#include <unicode/unumberrangeformatter.h>
#else
#include <unicode/numberrangeformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_unumberrangeformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UNumberRangeCollapse>(m, "UNumberRangeCollapse", py::arithmetic(),
                                  "Defines how to merge fields that are identical across the range sign.")
      .value("UNUM_RANGE_COLLAPSE_AUTO", UNUM_RANGE_COLLAPSE_AUTO,
             "Use locale data and heuristics to determine how much of the string to collapse.\n\n  "
             "Could end up collapsing none, some, or all repeated pieces in a locale-sensitive way.\n\n  "
             "The heuristics used for this option are subject to change over time.")
      .value("UNUM_RANGE_COLLAPSE_NONE", UNUM_RANGE_COLLAPSE_NONE,
             "Do not collapse any part of the number.\n\n  "
             "Example: \"3.2 thousand kilograms \xe2\x80\x93 5.3 thousand kilograms\"")
      .value("UNUM_RANGE_COLLAPSE_UNIT", UNUM_RANGE_COLLAPSE_UNIT,
             "Collapse the unit part of the number, but not the notation, if present.\n\n  "
             "Example: \"3.2 thousand \xe2\x80\x93 5.3 thousand kilograms\"")
      .value("UNUM_RANGE_COLLAPSE_ALL", UNUM_RANGE_COLLAPSE_ALL,
             "Collapse any field that is equal across the range sign.\n\n  "
             "May introduce ambiguity on the magnitude of the number. Example: \"3.2 \xe2\x80\x93 5.3 thousand "
             "kilograms\"")
      .export_values();

  py::enum_<UNumberRangeIdentityFallback>(
      m, "UNumberRangeIdentityFallback", py::arithmetic(),
      "Defines the behavior when the two numbers in the range are identical after rounding.\n\n"
      "To programmatically detect when the identity fallback is used, compare the lower and upper BigDecimals via "
      "*FormattedNumber*.")
      .value("UNUM_IDENTITY_FALLBACK_SINGLE_VALUE", UNUM_IDENTITY_FALLBACK_SINGLE_VALUE,
             "Show the number as a single value rather than a range.\n\n  "
             "Example: \"$5\"")
      .value("UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE",
             UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE,
             "Show the number using a locale-sensitive approximation pattern.\n\n  "
             "If the numbers were the same before rounding, show the single value. Example: \"~$5\" or \"$5\"")
      .value("UNUM_IDENTITY_FALLBACK_APPROXIMATELY", UNUM_IDENTITY_FALLBACK_APPROXIMATELY,
             "Show the number using a locale-sensitive approximation pattern.\n\n  "
             "Use the range pattern always, even if the inputs are the same. Example: \"~$5\"")
      .value("UNUM_IDENTITY_FALLBACK_RANGE", UNUM_IDENTITY_FALLBACK_RANGE,
             "Show the number as the range of two equal values.\n\n  "
             "Use the range pattern always, even if the inputs are the same. Example (with RangeCollapse.NONE): \"$5 "
             "\xe2\x80\x93 $5\"")
      .export_values();

  py::enum_<UNumberRangeIdentityResult>(
      m, "UNumberRangeIdentityResult", py::arithmetic(),
      "Used in the result class *FormattedNumberRange* to indicate to the user whether the numbers formatted in the "
      "range were equal or not, and whether or not the identity fallback was applied.")
      .value("UNUM_IDENTITY_RESULT_EQUAL_BEFORE_ROUNDING", UNUM_IDENTITY_RESULT_EQUAL_BEFORE_ROUNDING,
             "Used to indicate that the two numbers in the range were equal, even before any rounding rules were "
             "applied.")
      .value("UNUM_IDENTITY_RESULT_EQUAL_AFTER_ROUNDING", UNUM_IDENTITY_RESULT_EQUAL_AFTER_ROUNDING,
             "Used to indicate that the two numbers in the range were equal, but only after rounding rules were "
             "applied.")
      .value("UNUM_IDENTITY_RESULT_NOT_EQUAL", UNUM_IDENTITY_RESULT_NOT_EQUAL,
             "Used to indicate that the two numbers in the range were not equal, even after rounding rules were "
             "applied.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

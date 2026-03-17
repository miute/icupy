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
  //
  // enum UNumberRangeCollapse
  //
  py::enum_<UNumberRangeCollapse>(
      m, "UNumberRangeCollapse", py::arithmetic(),
      "Define how to merge fields that are identical across the range sign.")
      .value("UNUM_RANGE_COLLAPSE_AUTO", UNUM_RANGE_COLLAPSE_AUTO, R"doc(
             Use locale data and heuristics to determine how much of the
             string to collapse.

             Could end up collapsing none, some, or all repeated pieces in a
             locale-sensitive way.

             The heuristics used for this option are subject to change over
             time.
             )doc")
      .value("UNUM_RANGE_COLLAPSE_NONE", UNUM_RANGE_COLLAPSE_NONE, R"doc(
             Do not collapse any part of the number.

             Example: "3.2 thousand kilograms – 5.3 thousand kilograms"
             )doc")
      .value("UNUM_RANGE_COLLAPSE_UNIT", UNUM_RANGE_COLLAPSE_UNIT, R"doc(
             Collapse the unit part of the number, but not the notation, if
             present.

             Example: "3.2 thousand – 5.3 thousand kilograms"
             )doc")
      .value("UNUM_RANGE_COLLAPSE_ALL", UNUM_RANGE_COLLAPSE_ALL, R"doc(
             Collapse any field that is equal across the range sign.

             May introduce ambiguity on the magnitude of the number. Example:
             "3.2 – 5.3 thousand kilograms"
             )doc")
      .export_values();

  //
  // enum UNumberRangeIdentityFallback
  //
  py::enum_<UNumberRangeIdentityFallback>(m, "UNumberRangeIdentityFallback",
                                          py::arithmetic(), R"doc(
Define the behavior when the two numbers in the range are identical after
rounding.

To programmatically detect when the identity fallback is used, compare the
lower and upper BigDecimals via :class:`number.FormattedNumber`.
      )doc")
      .value("UNUM_IDENTITY_FALLBACK_SINGLE_VALUE",
             UNUM_IDENTITY_FALLBACK_SINGLE_VALUE, R"doc(
             Show the number as a single value rather than a range.

             Example: "$5"
             )doc")
      .value("UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE",
             UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE, R"doc(
             Show the number using a locale-sensitive approximation pattern.

             If the numbers were the same before rounding, show the single
             value. Example: "~$5" or "$5"
             )doc")
      .value("UNUM_IDENTITY_FALLBACK_APPROXIMATELY",
             UNUM_IDENTITY_FALLBACK_APPROXIMATELY, R"doc(
             Show the number using a locale-sensitive approximation pattern.

             Use the range pattern always, even if the inputs are the same.
             Example: "~$5"
             )doc")
      .value("UNUM_IDENTITY_FALLBACK_RANGE", UNUM_IDENTITY_FALLBACK_RANGE,
             R"doc(
             Show the number as the range of two equal values.

             Use the range pattern always, even if the inputs are the same.
             Example (with RangeCollapse.NONE): "$5 – $5"
             )doc")
      .export_values();

  //
  // enum UNumberRangeIdentityResult
  //
  py::enum_<UNumberRangeIdentityResult>(m, "UNumberRangeIdentityResult",
                                        py::arithmetic(), R"doc(
Used in the result class :class:`~number.FormattedNumberRange` to indicate to
the user whether the numbers formatted in the range were equal or not, and
whether or not the identity fallback was applied.
      )doc")
      .value("UNUM_IDENTITY_RESULT_EQUAL_BEFORE_ROUNDING",
             UNUM_IDENTITY_RESULT_EQUAL_BEFORE_ROUNDING, R"doc(
             Used to indicate that the two numbers in the range were equal,
             even before any rounding rules were applied.
             )doc")
      .value("UNUM_IDENTITY_RESULT_EQUAL_AFTER_ROUNDING",
             UNUM_IDENTITY_RESULT_EQUAL_AFTER_ROUNDING, R"doc(
             Used to indicate that the two numbers in the range were equal,
             but only after rounding rules were applied.
             )doc")
      .value("UNUM_IDENTITY_RESULT_NOT_EQUAL", UNUM_IDENTITY_RESULT_NOT_EQUAL,
             R"doc(
             Used to indicate that the two numbers in the range were not
             equal, even after rounding rules were applied.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

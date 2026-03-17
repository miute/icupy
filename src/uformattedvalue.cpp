#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <unicode/uformattedvalue.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

void init_uformattedvalue(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // enum UFieldCategory
  //
  py::enum_<UFieldCategory>(m, "UFieldCategory", py::arithmetic(), R"doc(
All possible field categories in ICU.

Every entry in this enum corresponds to another enum that exists in ICU.

In the APIs that take a :class:`UFieldCategory`, an ``int32_t`` type is used.
Field categories having any of the top four bits turned on are reserved as
private-use for external APIs implementing :class:`FormattedValue`. This means
that categories 2^28 and higher or below zero (with the highest bit turned on)
are private-use and will not be used by ICU in the future.
      )doc")
      .value("UFIELD_CATEGORY_UNDEFINED", UFIELD_CATEGORY_UNDEFINED, R"doc(
             For an undefined field category.
             )doc")
      .value("UFIELD_CATEGORY_DATE", UFIELD_CATEGORY_DATE, R"doc(
             For fields in :class:`UDateFormatField` (udat.h), from ICU 3.0.
             )doc")
      .value("UFIELD_CATEGORY_NUMBER", UFIELD_CATEGORY_NUMBER, R"doc(
             For fields in :class:`UNumberFormatFields` (unum.h), from ICU 49.
             )doc")
      .value("UFIELD_CATEGORY_LIST", UFIELD_CATEGORY_LIST, R"doc(
             For fields in :class:`UListFormatterField` (ulistformatter.h),
             from ICU 63.
             )doc")
      .value("UFIELD_CATEGORY_RELATIVE_DATETIME",
             UFIELD_CATEGORY_RELATIVE_DATETIME, R"doc(
             For fields in :class:`URelativeDateTimeFormatterField`
             (ureldatefmt.h), from ICU 64.
             )doc")
      .value("UFIELD_CATEGORY_LIST_SPAN", UFIELD_CATEGORY_LIST_SPAN, R"doc(
             Category for spans in a list.
             )doc")
      .value("UFIELD_CATEGORY_DATE_INTERVAL_SPAN",
             UFIELD_CATEGORY_DATE_INTERVAL_SPAN, R"doc(
             Category for spans in a date interval.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UFIELD_CATEGORY_NUMBER_RANGE_SPAN",
             UFIELD_CATEGORY_NUMBER_RANGE_SPAN, R"doc(
             Category for spans in a number range.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

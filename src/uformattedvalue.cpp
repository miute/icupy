#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <unicode/uformattedvalue.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

void init_uformattedvalue(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  py::enum_<UFieldCategory>(
      m, "UFieldCategory", py::arithmetic(),
      "All possible field categories in ICU.\n\n"
      "Every entry in this enum corresponds to another enum that exists in ICU.\n\n"
      "In the APIs that take a *UFieldCategory*, an int32_t type is used. Field categories having any of the top four "
      "bits turned on are reserved as private-use for external APIs implementing *FormattedValue*. This means that "
      "categories 2^28 and higher or below zero (with the highest bit turned on) are private-use and will not be used "
      "by ICU in the future.")
      .value("UFIELD_CATEGORY_UNDEFINED", UFIELD_CATEGORY_UNDEFINED, "For an undefined field category.")
      .value("UFIELD_CATEGORY_DATE", UFIELD_CATEGORY_DATE, "For fields in *UDateFormatField* (udat.h), from ICU 3.0.")
      .value("UFIELD_CATEGORY_NUMBER", UFIELD_CATEGORY_NUMBER,
             "For fields in *UNumberFormatFields* (unum.h), from ICU 49.")
      .value("UFIELD_CATEGORY_LIST", UFIELD_CATEGORY_LIST,
             "For fields in *UListFormatterField* (ulistformatter.h), from ICU 63.")
      .value("UFIELD_CATEGORY_RELATIVE_DATETIME", UFIELD_CATEGORY_RELATIVE_DATETIME,
             "For fields in *URelativeDateTimeFormatterField* (ureldatefmt.h), from ICU 64.")
      .value("UFIELD_CATEGORY_LIST_SPAN", UFIELD_CATEGORY_LIST_SPAN, "Category for spans in a list.")
      .value("UFIELD_CATEGORY_DATE_INTERVAL_SPAN", UFIELD_CATEGORY_DATE_INTERVAL_SPAN,
             "Category for spans in a date interval.")
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UFIELD_CATEGORY_NUMBER_RANGE_SPAN", UFIELD_CATEGORY_NUMBER_RANGE_SPAN,
             "Category for spans in a number range.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

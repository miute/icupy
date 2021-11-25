#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
#include <unicode/uregion.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

void init_uregion(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  py::enum_<URegionType>(m, "URegionType", py::arithmetic(),
                         "*URegionType* is an enumeration defining the different types of regions.\n\n"
                         "Current possible values are *URGN_WORLD*, *URGN_CONTINENT*, *URGN_SUBCONTINENT*, "
                         "*URGN_TERRITORY*, *URGN_GROUPING*, *URGN_DEPRECATED*, and *URGN_UNKNOWN*.")
      .value("URGN_UNKNOWN", URGN_UNKNOWN, "Type representing the unknown region.")
      .value("URGN_TERRITORY", URGN_TERRITORY, "Type representing a territory.")
      .value("URGN_WORLD", URGN_WORLD, "Type representing the whole world.")
      .value("URGN_CONTINENT", URGN_CONTINENT, "Type representing a continent.")
      .value("URGN_SUBCONTINENT", URGN_SUBCONTINENT, "Type representing a sub-continent.")
      .value("URGN_GROUPING", URGN_GROUPING,
             "Type representing a grouping of territories that is not to be used in the normal "
             "WORLD/CONTINENT/SUBCONTINENT/TERRITORY containment tree.")
      .value("URGN_DEPRECATED", URGN_DEPRECATED,
             "Type representing a region whose code has been deprecated, usually due to a country splitting into "
             "multiple territories or changing its name.")
#ifndef U_HIDE_DEPRECATED_API
      .value("URGN_LIMIT", URGN_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

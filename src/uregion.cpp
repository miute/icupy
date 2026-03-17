#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
#include <unicode/uregion.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

void init_uregion(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  //
  // enum URegionType
  //
  py::enum_<URegionType>(m, "URegionType", py::arithmetic(), R"doc(
URegionType is an enumeration defining the different types of regions.
      )doc")
      .value("URGN_UNKNOWN", URGN_UNKNOWN, R"doc(
             Type representing the unknown region.
             )doc")
      .value("URGN_TERRITORY", URGN_TERRITORY, R"doc(
             Type representing a territory.
             )doc")
      .value("URGN_WORLD", URGN_WORLD, R"doc(
             Type representing the whole world.
             )doc")
      .value("URGN_CONTINENT", URGN_CONTINENT, R"doc(
             Type representing a continent.
             )doc")
      .value("URGN_SUBCONTINENT", URGN_SUBCONTINENT, R"doc(
             Type representing a sub-continent.
             )doc")
      .value("URGN_GROUPING", URGN_GROUPING, R"doc(
             Type representing a grouping of territories that is not to be
             used in the normal
             WORLD/CONTINENT/SUBCONTINENT/TERRITORY containment tree.
             )doc")
      .value("URGN_DEPRECATED", URGN_DEPRECATED, R"doc(
             Type representing a region whose code has been deprecated,
             usually due to a country splitting into multiple territories or
             changing its name.
             )doc")
      .value("URGN_LIMIT", URGN_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

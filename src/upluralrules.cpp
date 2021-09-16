#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <unicode/upluralrules.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

void init_upluralrules(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  py::enum_<UPluralType>(m, "UPluralType", py::arithmetic())
      .value("UPLURAL_TYPE_CARDINAL", UPLURAL_TYPE_CARDINAL)
      .value("UPLURAL_TYPE_ORDINAL", UPLURAL_TYPE_ORDINAL)
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

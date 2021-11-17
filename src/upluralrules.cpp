#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <unicode/upluralrules.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

void init_upluralrules(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  py::enum_<UPluralType>(m, "UPluralType", py::arithmetic(), "Type of plurals and PluralRules.")
      .value("UPLURAL_TYPE_CARDINAL", UPLURAL_TYPE_CARDINAL, "Plural rules for cardinal numbers: 1 file vs. 2 files.")
      .value("UPLURAL_TYPE_ORDINAL", UPLURAL_TYPE_ORDINAL,
             "Plural rules for ordinal numbers: 1st file, 2nd file, 3rd file, 4th file, etc.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

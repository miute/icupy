#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <unicode/upluralrules.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

void init_upluralrules(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // enum UPluralType
  //
  py::enum_<UPluralType>(m, "UPluralType", py::arithmetic(), R"doc(
Type of PluralRules.
      )doc")
      .value("UPLURAL_TYPE_CARDINAL", UPLURAL_TYPE_CARDINAL, R"doc(
             Plural rules for cardinal numbers: 1 file vs. 2 files.
             )doc")
      .value("UPLURAL_TYPE_ORDINAL", UPLURAL_TYPE_ORDINAL, R"doc(
             Plural rules for ordinal numbers: 1st file, 2nd file, 3rd file,
             4th file, etc.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

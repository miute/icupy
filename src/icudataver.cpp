#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
#include "uversion.hpp"
#include <pybind11/stl.h>
#include <unicode/icudataver.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

using namespace icu;

void init_icudataver(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def(
      "u_get_data_version",
      []() {
        UVersionInfo data_version_fillin{};
        ErrorCode error_code;
        u_getDataVersion(data_version_fillin, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return icupy::VersionInfo(data_version_fillin);
      },
      R"doc(
      Return the ICU data version information.
      )doc");

  m.attr("U_ICU_DATA_KEY") = U_ICU_DATA_KEY;
  m.attr("U_ICU_VERSION_BUNDLE") = U_ICU_VERSION_BUNDLE;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
}

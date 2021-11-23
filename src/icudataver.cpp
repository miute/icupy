#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
#include <pybind11/stl.h>
#include <unicode/errorcode.h>
#include <unicode/icudataver.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

using namespace icu;

void init_icudataver(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def("u_get_data_version", []() {
    UVersionInfo info;
    ErrorCode error_code;
    u_getDataVersion(info, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    std::vector<uint8_t> result(info, info + sizeof(info));
    return result;
  });

  m.attr("U_ICU_DATA_KEY") = U_ICU_DATA_KEY;
  m.attr("U_ICU_VERSION_BUNDLE") = U_ICU_VERSION_BUNDLE;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
#include <algorithm>
#include <pybind11/stl.h>
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
      throw icupy::ICUError(error_code);
    }
    py::tuple result(U_MAX_VERSION_LENGTH);
    int n = 0;
    std::for_each(std::begin(info), std::end(info), [&](auto v) { result[n++] = v; });
    return result;
  });

  m.attr("U_ICU_DATA_KEY") = U_ICU_DATA_KEY;
  m.attr("U_ICU_VERSION_BUNDLE") = U_ICU_VERSION_BUNDLE;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
}

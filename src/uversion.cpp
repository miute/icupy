#include "main.hpp"
#include <algorithm>
#include <pybind11/stl.h>
#include <unicode/uversion.h>

void init_uversion(py::module &m) {
  m.def("u_get_version", []() {
    UVersionInfo info;
    u_getVersion(info);
    py::tuple result(U_MAX_VERSION_LENGTH);
    int n = 0;
    std::for_each(std::begin(info), std::end(info), [&](auto v) { result[n++] = v; });
    return result;
  });

  m.def(
      "u_version_from_string",
      [](const char *version_string) {
        UVersionInfo info;
        u_versionFromString(info, version_string);
        py::tuple result(U_MAX_VERSION_LENGTH);
        int n = 0;
        std::for_each(std::begin(info), std::end(info), [&](auto v) { result[n++] = v; });
        return result;
      },
      py::arg("version_string"));

  m.def(
      "u_version_to_string",
      [](const std::array<uint8_t, sizeof(UVersionInfo)> &version_array) {
        UVersionInfo info;
        std::copy(version_array.begin(), version_array.end(), info);
        char buf[U_MAX_VERSION_STRING_LENGTH];
        u_versionToString(info, buf);
        return py::str(buf);
      },
      py::arg("version_array"));

  m.attr("U_MAX_VERSION_LENGTH") = U_MAX_VERSION_LENGTH;
  m.attr("U_VERSION_DELIMITER") = U_VERSION_DELIMITER;

  // <unicode/uvernum.h>
  m.attr("U_COPYRIGHT_STRING") = U_COPYRIGHT_STRING;
  m.attr("U_ICU_VERSION") = U_ICU_VERSION;
  m.attr("U_ICU_VERSION_BUILDLEVEL_NUM") = U_ICU_VERSION_BUILDLEVEL_NUM;
  m.attr("U_ICU_VERSION_MAJOR_NUM") = U_ICU_VERSION_MAJOR_NUM;
  m.attr("U_ICU_VERSION_MINOR_NUM") = U_ICU_VERSION_MINOR_NUM;
  m.attr("U_ICU_VERSION_PATCHLEVEL_NUM") = U_ICU_VERSION_PATCHLEVEL_NUM;
  m.attr("U_ICU_VERSION_SHORT") = U_ICU_VERSION_SHORT;
  m.attr("UCOL_BUILDER_VERSION") = UCOL_BUILDER_VERSION;
  m.attr("UCOL_RUNTIME_VERSION") = UCOL_RUNTIME_VERSION;
}

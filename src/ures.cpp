#include "main.hpp"
#include "uresptr.hpp"
#include <memory>

_UResourceBundlePtr::_UResourceBundlePtr(UResourceBundle *p) : p_(p) {}
_UResourceBundlePtr::~_UResourceBundlePtr() {}
UResourceBundle *_UResourceBundlePtr::get() const { return p_; }

void init_ures(py::module &m) {
  py::enum_<UResType>(m, "UResType", py::arithmetic())
      .value("URES_NONE", URES_NONE)
      .value("URES_STRING", URES_STRING)
      .value("URES_BINARY", URES_BINARY)
      .value("URES_TABLE", URES_TABLE)
      .value("URES_ALIAS", URES_ALIAS)
      .value("URES_INT", URES_INT)
      .value("URES_ARRAY", URES_ARRAY)
      .value("URES_INT_VECTOR", URES_INT_VECTOR)
#ifndef U_HIDE_DEPRECATED_API
      .value("RES_NONE", RES_NONE)
      .value("RES_STRING", RES_STRING)
      .value("RES_BINARY", RES_BINARY)
      .value("RES_TABLE", RES_TABLE)
      .value("RES_ALIAS", RES_ALIAS)
      .value("RES_INT", RES_INT)
      .value("RES_ARRAY", RES_ARRAY)
      .value("RES_INT_VECTOR", RES_INT_VECTOR)
      .value("RES_RESERVED", RES_RESERVED)
      .value("URES_LIMIT", URES_LIMIT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::class_<_UResourceBundlePtr>(m, "_UResourceBundlePtr");

  m.def(
      "ures_close", [](_UResourceBundlePtr &resource_bundle) { ures_close(resource_bundle); },
      py::arg("resource_bundle"));
  m.def(
      "ures_open",
      [](const char *package_name, const char *locale) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto resource_bundle = ures_open(package_name, locale, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UResourceBundlePtr>(resource_bundle);
      },
      py::arg("package_name"), py::arg("locale"));
  m.def(
      "ures_open_direct",
      [](const char *package_name, const char *locale) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto resource_bundle = ures_openDirect(package_name, locale, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UResourceBundlePtr>(resource_bundle);
      },
      py::arg("package_name"), py::arg("locale"));
}

#include "main.hpp"
#include "uresptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/stl.h>

using namespace icu;

_UResourceBundlePtr::_UResourceBundlePtr(UResourceBundle *p) : p_(p) {}

_UResourceBundlePtr::~_UResourceBundlePtr() {}

UResourceBundle *_UResourceBundlePtr::get() const { return p_; }

void init_ures(py::module &m) {
  //
  // enum UResType
  //
  py::enum_<UResType>(m, "UResType", py::arithmetic(),
                      "Numeric constants for types of resource items.")
      .value("URES_NONE", URES_NONE,
             "Resource type constant for \"no resource\".")
      .value("URES_STRING", URES_STRING,
             "Resource type constant for 16-bit Unicode strings.")
      .value("URES_BINARY", URES_BINARY,
             "Resource type constant for binary data.")
      .value("URES_TABLE", URES_TABLE,
             "Resource type constant for tables of key-value pairs.")
      .value("URES_ALIAS", URES_ALIAS,
             "Resource type constant for aliases; internally stores a string "
             "which identifies the actual resource "
             "storing the data (can be in a different resource bundle).\n\n  "
             "Resolved internally before delivering the actual resource "
             "through the API.")
      .value("URES_INT", URES_INT,
             "Resource type constant for a single 28-bit integer, interpreted "
             "as signed or unsigned by the "
             "*ures_get_int()* or *ures_get_uint()* function.")
      .value("URES_ARRAY", URES_ARRAY,
             "Resource type constant for arrays of resources.")
      .value("URES_INT_VECTOR", URES_INT_VECTOR,
             "Resource type constant for vectors of 32-bit integers.")
      .value("RES_NONE", RES_NONE,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_STRING", RES_STRING,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_BINARY", RES_BINARY,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_TABLE", RES_TABLE,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_ALIAS", RES_ALIAS,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_INT", RES_INT,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_ARRAY", RES_ARRAY,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_INT_VECTOR", RES_INT_VECTOR,
             "**Deprecated:** ICU 2.6 Use the URES_ constant instead.")
      .value("RES_RESERVED", RES_RESERVED, "**Deprecated:** ICU 2.6 Not used.")
      .value("URES_LIMIT", URES_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, "
             "see ICU ticket #12420.")
      .export_values();

  //
  // struct UResourceBundle
  //
  py::class_<_UResourceBundlePtr>(m, "_UResourceBundlePtr");

  //
  // Functions
  //
  m.def(
      "ures_close",
      [](_UResourceBundlePtr &resource_bundle) { ures_close(resource_bundle); },
      py::arg("resource_bundle"));

  m.def(
      "ures_open",
      [](const std::optional<std::string> &package_name,
         const std::optional<std::string> &locale) {
        ErrorCode error_code;
        auto resource_bundle =
            ures_open(package_name ? package_name->data() : nullptr,
                      locale ? locale->data() : nullptr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UResourceBundlePtr>(resource_bundle);
      },
      py::arg("package_name") = std::nullopt, py::arg("locale") = std::nullopt);

  m.def(
      "ures_open_direct",
      [](const std::optional<std::string> &package_name,
         const std::optional<std::string> &locale) {
        ErrorCode error_code;
        auto resource_bundle =
            ures_openDirect(package_name ? package_name->data() : nullptr,
                            locale ? locale->data() : nullptr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UResourceBundlePtr>(resource_bundle);
      },
      py::arg("package_name") = std::nullopt, py::arg("locale") = std::nullopt);
}

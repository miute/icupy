#include "main.hpp"
#include "uresptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/stl.h>

using namespace icu;

namespace icupy {

UResourceBundlePtr::UResourceBundlePtr(UResourceBundle *p) : p_(p) {}

UResourceBundlePtr::~UResourceBundlePtr() {}

UResourceBundle *UResourceBundlePtr::get() const { return p_; }

} // namespace icupy

void init_ures(py::module &m) {
  //
  // enum UResType
  //
  py::enum_<UResType>(m, "UResType", py::arithmetic(), R"doc(
Numeric constants for types of resource items.

See Also:
    :meth:`ResourceBundle.get_type`
      )doc")
      .value("URES_NONE", URES_NONE, R"doc(
             Resource type constant for "no resource".
             )doc")
      .value("URES_STRING", URES_STRING, R"doc(
             Resource type constant for 16-bit Unicode strings.
             )doc")
      .value("URES_BINARY", URES_BINARY, R"doc(
             Resource type constant for binary data.
             )doc")
      .value("URES_TABLE", URES_TABLE, R"doc(
             Resource type constant for tables of key-value pairs.
             )doc")
      .value("URES_ALIAS", URES_ALIAS, R"doc(
             Resource type constant for aliases; internally stores a string
             which identifies the actual resource storing the data (can be in
             a different resource bundle).

             Resolved internally before delivering the actual resource
             through the API.
             )doc")
      .value("URES_INT", URES_INT, R"doc(
             Resource type constant for a single 28-bit integer, interpreted
             as signed or unsigned by the :meth:`ResourceBundle.get_int` or
             :meth:`ResourceBundle.get_uint` function.
             )doc")
      .value("URES_ARRAY", URES_ARRAY, R"doc(
             Resource type constant for arrays of resources.
             )doc")
      .value("URES_INT_VECTOR", URES_INT_VECTOR, R"doc(
             Resource type constant for vectors of 32-bit integers.
             )doc")
      .value("RES_NONE", RES_NONE, R"doc(
             Deprecated: ICU 2.6 Use the URES_NONE constant instead.
             )doc")
      .value("RES_STRING", RES_STRING, R"doc(
             Deprecated: ICU 2.6 Use the URES_STRING constant instead.
             )doc")
      .value("RES_BINARY", RES_BINARY, R"doc(
             Deprecated: ICU 2.6 Use the URES_BINARY constant instead.
             )doc")
      .value("RES_TABLE", RES_TABLE, R"doc(
             Deprecated: ICU 2.6 Use the URES_TABLE constant instead.
             )doc")
      .value("RES_ALIAS", RES_ALIAS, R"doc(
             Deprecated: ICU 2.6 Use the URES_ALIAS constant instead.
             )doc")
      .value("RES_INT", RES_INT, R"doc(
             Deprecated: ICU 2.6 Use the URES_INT constant instead.
             )doc")
      .value("RES_ARRAY", RES_ARRAY, R"doc(
             Deprecated: ICU 2.6 Use the URES_ARRAY constant instead.
             )doc")
      .value("RES_INT_VECTOR", RES_INT_VECTOR, R"doc(
             Deprecated: ICU 2.6 Use the URES_INT_VECTOR constant instead.
             )doc")
      .value("RES_RESERVED", RES_RESERVED,
             R"doc(
             Deprecated: ICU 2.6 Not used.
             )doc")
      .value("URES_LIMIT", URES_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // struct UResourceBundle
  //
  py::class_<icupy::UResourceBundlePtr>(m, "UResourceBundle", R"doc(
       UResourceBundle structure.

       See Also:
           :func:`ures_close`
           :func:`ures_open`
           :func:`ures_open_direct`
           :meth:`ResourceBundle.__init__`
       )doc");

  //
  // Functions
  //
  m.def(
      "ures_close",
      [](icupy::UResourceBundlePtr &resource_bundle) {
        ures_close(resource_bundle);
      },
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
        return std::make_unique<icupy::UResourceBundlePtr>(resource_bundle);
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
        return std::make_unique<icupy::UResourceBundlePtr>(resource_bundle);
      },
      py::arg("package_name") = std::nullopt, py::arg("locale") = std::nullopt);
}

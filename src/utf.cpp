#include "main.hpp"
#include <unicode/utf.h>

void init_utf(py::module &m) {
  m.def(
      "u_is_bmp", [](uint32_t c) -> py::bool_ { return U_IS_BMP(c); },
      py::arg("c"));

#if (U_ICU_VERSION_MAJOR_NUM >= 78)
  m.def(
      "u_is_code_point",
      [](uint32_t c) -> py::bool_ { return U_IS_CODE_POINT(c); }, py::arg("c"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)

  m.def(
      "u_is_lead", [](uint32_t c) -> py::bool_ { return U_IS_LEAD(c); },
      py::arg("c"));

#if (U_ICU_VERSION_MAJOR_NUM >= 78)
  m.def(
      "u_is_scalar_value",
      [](uint32_t c) -> py::bool_ { return U_IS_SCALAR_VALUE(c); },
      py::arg("c"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)

  m.def(
      "u_is_supplementary",
      [](uint32_t c) -> py::bool_ { return U_IS_SUPPLEMENTARY(c); },
      py::arg("c"));

  m.def(
      "u_is_surrogate",
      [](uint32_t c) -> py::bool_ { return U_IS_SURROGATE(c); }, py::arg("c"));

  m.def(
      "u_is_surrogate_lead",
      [](uint32_t c) -> py::bool_ { return U_IS_SURROGATE_LEAD(c); },
      py::arg("c"));

  m.def(
      "u_is_surrogate_trail",
      [](uint32_t c) -> py::bool_ { return U_IS_SURROGATE_TRAIL(c); },
      py::arg("c"));

  m.def(
      "u_is_trail", [](uint32_t c) -> py::bool_ { return U_IS_TRAIL(c); },
      py::arg("c"));

  m.def(
      "u_is_unicode_char",
      [](uint32_t c) -> py::bool_ { return U_IS_UNICODE_CHAR(c); },
      py::arg("c"));

  m.def(
      "u_is_unicode_nonchar",
      [](uint32_t c) -> py::bool_ { return U_IS_UNICODE_NONCHAR(c); },
      py::arg("c"));
}

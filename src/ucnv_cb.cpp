#include "main.hpp"
#include <unicode/ucnv_cb.h>

void init_ucnv_cb(py::module &m) {
  m.def(
      "ucnv_cb_from_u_write_bytes",
      [](UConverterFromUnicodeArgs *args, const char *source, int32_t length, int32_t offset_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_cbFromUWriteBytes(args, source, length, offset_index, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("source"), py::arg("length"), py::arg("offset_index"));
  m.def(
      "ucnv_cb_from_u_write_sub",
      [](UConverterFromUnicodeArgs *args, int32_t offset_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_cbFromUWriteSub(args, offset_index, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("offset_index"));
  m.def(
      "ucnv_cb_to_u_write_sub",
      [](UConverterToUnicodeArgs *args, int32_t offset_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_cbToUWriteSub(args, offset_index, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("offset_index"));
  m.def(
      "ucnv_cb_to_u_write_uchars",
      [](UConverterToUnicodeArgs *args, const UChar *source, int32_t length, int32_t offset_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_cbToUWriteUChars(args, source, length, offset_index, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("source"), py::arg("length"), py::arg("offset_index"));
}

#include "main.hpp"
#include <unicode/ucnv_cb.h>

using namespace icu;

void init_ucnv_cb(py::module &m) {
  // TODO: Remove ucnv_cb_from_u_write_bytes() in a future release.
  m.def(
      "ucnv_cb_from_u_write_bytes",
      [](UConverterFromUnicodeArgs *args, const char *source, int32_t length, int32_t offset_index) {
        PyErr_WarnEx(nullptr,
                     "ucnv_cb_from_u_write_bytes() is deprecated and will be removed in a future release. "
                     "Use ucnv_cb_from_uwrite_bytes() instead.",
                     1);
        ErrorCode error_code;
        ucnv_cbFromUWriteBytes(args, source, length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("source"), py::arg("length"), py::arg("offset_index"));

  // TODO: Remove ucnv_cb_from_u_write_sub() in a future release.
  m.def(
      "ucnv_cb_from_u_write_sub",
      [](UConverterFromUnicodeArgs *args, int32_t offset_index) {
        PyErr_WarnEx(nullptr,
                     "ucnv_cb_from_u_write_sub() is deprecated and will be removed in a future release. "
                     "Use ucnv_cb_from_uwrite_sub() instead.",
                     1);
        ErrorCode error_code;
        ucnv_cbFromUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("offset_index"));

  // TODO: Remove ucnv_cb_to_u_write_sub() in a future release.
  m.def(
      "ucnv_cb_to_u_write_sub",
      [](UConverterToUnicodeArgs *args, int32_t offset_index) {
        PyErr_WarnEx(nullptr,
                     "ucnv_cb_to_u_write_sub() is deprecated and will be removed in a future release. "
                     "Use ucnv_cb_to_uwrite_sub() instead.",
                     1);
        ErrorCode error_code;
        ucnv_cbToUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("offset_index"));

  // TODO: Remove ucnv_cb_to_u_write_uchars() in a future release.
  m.def(
      "ucnv_cb_to_u_write_uchars",
      [](UConverterToUnicodeArgs *args, const UChar *source, int32_t length, int32_t offset_index) {
        PyErr_WarnEx(nullptr,
                     "ucnv_cb_to_u_write_uchars() is deprecated and will be removed in a future release. "
                     "Use ucnv_cb_to_uwrite_uchars() instead.",
                     1);
        ErrorCode error_code;
        ucnv_cbToUWriteUChars(args, source, length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("source"), py::arg("length"), py::arg("offset_index"));

  m.def(
      "ucnv_cb_from_uwrite_bytes",
      [](UConverterFromUnicodeArgs *args, const char *source, int32_t length, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbFromUWriteBytes(args, source, length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("source"), py::arg("length"), py::arg("offset_index"));

  m.def(
      "ucnv_cb_from_uwrite_sub",
      [](UConverterFromUnicodeArgs *args, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbFromUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("offset_index"));

  m.def(
      "ucnv_cb_to_uwrite_sub",
      [](UConverterToUnicodeArgs *args, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbToUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("offset_index"));

  m.def(
      "ucnv_cb_to_uwrite_uchars",
      [](UConverterToUnicodeArgs *args, const UChar *source, int32_t length, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbToUWriteUChars(args, source, length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args"), py::arg("source"), py::arg("length"), py::arg("offset_index"));
}

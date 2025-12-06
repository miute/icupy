#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/ucnv_cb.h>
#include <variant>

using namespace icu;

void init_ucnv_cb(py::module &m) {
  // TODO: Remove ucnv_cb_from_u_write_bytes() in a future release.
  m.def(
      "ucnv_cb_from_u_write_bytes",
      [](UConverterFromUnicodeArgs *args, const icupy::CharPtrVariant &source, int32_t length, int32_t offset_index) {
        PyErr_WarnEx(nullptr,
                     "ucnv_cb_from_u_write_bytes() is deprecated and will be removed in a future release. "
                     "Use ucnv_cb_from_uwrite_bytes() instead.",
                     1);
        const auto v = icupy::CharPtr(source);
        if (length == -1) {
          length = static_cast<int32_t>(v.size());
        }
        ErrorCode error_code;
        ucnv_cbFromUWriteBytes(args, v, length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("source"), py::arg("length"), py::arg("offset_index"));

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
      py::arg("args").none(false), py::arg("offset_index"));

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
      py::arg("args").none(false), py::arg("offset_index"));

  // TODO: Remove ucnv_cb_to_u_write_uchars() in a future release.
  m.def(
      "ucnv_cb_to_u_write_uchars",
      [](UConverterToUnicodeArgs *args, const std::u16string &source, int32_t length, int32_t offset_index) {
        PyErr_WarnEx(nullptr,
                     "ucnv_cb_to_u_write_uchars() is deprecated and will be removed in a future release. "
                     "Use ucnv_cb_to_uwrite_uchars() instead.",
                     1);
        ErrorCode error_code;
        ucnv_cbToUWriteUChars(args, source.data(), length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("source"), py::arg("length"), py::arg("offset_index"));

  m.def(
      "ucnv_cb_from_uwrite_bytes",
      [](UConverterFromUnicodeArgs *args, const icupy::CharPtrVariant &source, int32_t length, int32_t offset_index) {
        const auto v = icupy::CharPtr(source);
        if (length == -1) {
          length = static_cast<int32_t>(v.size());
        }
        ErrorCode error_code;
        ucnv_cbFromUWriteBytes(args, v, length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("source"), py::arg("length"), py::arg("offset_index"));

  m.def(
      "ucnv_cb_from_uwrite_sub",
      [](UConverterFromUnicodeArgs *args, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbFromUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("offset_index"));

  m.def(
      "ucnv_cb_to_uwrite_sub",
      [](UConverterToUnicodeArgs *args, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbToUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("offset_index"));

  m.def(
      "ucnv_cb_to_uwrite_uchars",
      [](UConverterToUnicodeArgs *args, const std::u16string &source, int32_t length, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbToUWriteUChars(args, source.data(), length, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("source"), py::arg("length"), py::arg("offset_index"));
}

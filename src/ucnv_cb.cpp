#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/ucnv_cb.h>

using namespace icu;

void init_ucnv_cb(py::module &m) {
  m.def(
      "ucnv_cb_from_uwrite_bytes",
      [](UConverterFromUnicodeArgs *args, const icupy::CharPtrVariant &source,
         int32_t length, int32_t offset_index) {
        const auto source_value = icupy::CharPtr(source);
        if (length == -1) {
          length = static_cast<int32_t>(source_value.size());
        }
        ErrorCode error_code;
        ucnv_cbFromUWriteBytes(args, source_value, length, offset_index,
                               error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("source"), py::arg("length"),
      py::arg("offset_index"), R"doc(
      Write out the specified output bytes to the target byte buffer or to
      converter internal buffers.

      .. important::

          ONLY used by From Unicode callback functions.

      See Also:
          :class:`UConverterFromUCallback`
          :func:`ucnv_cb_from_uwrite_sub`
      )doc");

  m.def(
      "ucnv_cb_from_uwrite_sub",
      [](UConverterFromUnicodeArgs *args, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbFromUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("offset_index"), R"doc(
      Write out the correct substitution character sequence to the target.

      .. important::

          ONLY used by From Unicode callback functions.

      See Also:
          :class:`UConverterFromUCallback`
          :func:`ucnv_cb_from_uwrite_bytes`
      )doc");

  m.def(
      "ucnv_cb_to_uwrite_sub",
      [](UConverterToUnicodeArgs *args, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbToUWriteSub(args, offset_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("offset_index"), R"doc(
      Write out the Unicode substitution character (U+FFFD).

      .. important::

          ONLY used by To Unicode callback functions.

      See Also:
          :class:`UConverterToUCallback`
          :func:`ucnv_cb_to_uwrite_uchars`
      )doc");

  m.def(
      "ucnv_cb_to_uwrite_uchars",
      [](UConverterToUnicodeArgs *args, const std::u16string &source,
         int32_t length, int32_t offset_index) {
        ErrorCode error_code;
        ucnv_cbToUWriteUChars(args, source.data(), length, offset_index,
                              error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("args").none(false), py::arg("source"), py::arg("length"),
      py::arg("offset_index"), R"doc(
      Write out the specified characters to the target ``UChar`` buffer.

      .. important::

          ONLY used by To Unicode callback functions.

      See Also:
          :class:`UConverterToUCallback`
          :func:`ucnv_cb_to_uwrite_sub`
      )doc");
}

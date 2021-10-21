#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
#include "ubiditransformptr.hpp"
#include <unicode/ushape.h>

_UBiDiTransformPtr::_UBiDiTransformPtr(UBiDiTransform *p) : p_(p) {}
_UBiDiTransformPtr::~_UBiDiTransformPtr() {}
UBiDiTransform *_UBiDiTransformPtr::get() const { return p_; }
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

void init_ubiditransform(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  py::enum_<UBiDiMirroring>(m, "UBiDiMirroring", py::arithmetic())
      .value("UBIDI_MIRRORING_OFF", UBIDI_MIRRORING_OFF,
             "Constant indicating that character mirroring should not be performed. This is the default.")
      .value("UBIDI_MIRRORING_ON", UBIDI_MIRRORING_ON,
             "Constant indicating that character mirroring should be performed. This corresponds to calling "
             "ubidi_writeReordered or ubidi_writeReverse with the UBIDI_DO_MIRRORING option bit set.")
      .export_values();

  py::enum_<UBiDiOrder>(m, "UBiDiOrder", py::arithmetic())
      .value("UBIDI_LOGICAL", UBIDI_LOGICAL, "Constant indicating a logical order. This is the default for input text.")
      .value("UBIDI_VISUAL", UBIDI_VISUAL, "Constant indicating a visual order. This is a default for output text.")
      .export_values();

  py::class_<_UBiDiTransformPtr>(m, "_UBiDiTransformPtr");

  m.def(
      "ubiditransform_close", [](_UBiDiTransformPtr &bidi_transform) { ubiditransform_close(bidi_transform); },
      py::arg("bidi_transform"));
  m.def("ubiditransform_open", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto bidi_transform = ubiditransform_open(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_UBiDiTransformPtr>(bidi_transform);
  });
  m.def(
      "ubiditransform_transform",
      [](_UBiDiTransformPtr &bidi_transform, const char16_t *src, int32_t src_length, UBiDiLevel in_para_level,
         UBiDiOrder in_order, UBiDiLevel out_para_level, UBiDiOrder out_order, UBiDiMirroring do_mirroring,
         uint32_t shaping_options) {
        int32_t dest_size = src_length == -1 ? u_strlen(src) : src_length;
        if (shaping_options & U_SHAPE_LETTERS_UNSHAPE) {
          dest_size *= 2;
        }
        std::u16string result(dest_size, u'\0');
        UErrorCode error_code = U_ZERO_ERROR;
        dest_size =
            ubiditransform_transform(bidi_transform, src, src_length, result.data(), dest_size, in_para_level, in_order,
                                     out_para_level, out_order, do_mirroring, shaping_options, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        result.resize(dest_size);
        return result;
      },
      py::arg("bidi_transform"), py::arg("src"), py::arg("src_length"), py::arg("in_para_level"), py::arg("in_order"),
      py::arg("out_para_level"), py::arg("out_order"), py::arg("do_mirroring"), py::arg("shaping_options"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
}

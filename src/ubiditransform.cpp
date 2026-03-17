#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
#include "ubiditransformptr.hpp"
#include <unicode/ushape.h>

using namespace icu;

namespace icupy {

//
// struct UBiDiTransform
//
UBiDiTransformPtr::UBiDiTransformPtr(UBiDiTransform *p) : p_(p) {}

UBiDiTransformPtr::~UBiDiTransformPtr() {}

UBiDiTransform *UBiDiTransformPtr::get() const { return p_; }

} // namespace icupy

#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

void init_ubiditransform(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  //
  // enum UBiDiMirroring
  //
  py::enum_<UBiDiMirroring>(m, "UBiDiMirroring", py::arithmetic(), R"doc(
UBiDiMirroring indicates whether or not characters with the "mirrored" property
in RTL runs should be replaced with their mirror-image counterparts.

See Also:
    :func:`ubidi_set_reordering_options`
    :func:`ubidi_write_reordered`
    :func:`ubidi_write_reverse`
      )doc")
      .value("UBIDI_MIRRORING_OFF", UBIDI_MIRRORING_OFF, R"doc(
             Constant indicating that character mirroring should not be
             performed.

             This is the default.
             )doc")
      .value("UBIDI_MIRRORING_ON", UBIDI_MIRRORING_ON, R"doc(
             Constant indicating that character mirroring should be performed.

             This corresponds to calling :func:`ubidi_write_reordered` or
             :func:`ubidi_write_reverse` with the
             :attr:`UBIDI_DO_MIRRORING` option bit set.
             )doc")
      .export_values();

  //
  // enum UBiDiOrder
  //
  py::enum_<UBiDiOrder>(m, "UBiDiOrder", py::arithmetic(), R"doc(
UBiDiOrder indicates the order of text.

This bidi transformation engine supports all possible combinations (4 in total)
of input and output text order:

- <logical input, visual output>: unless the output direction is RTL,
  this corresponds to a normal operation of the Bidi algorithm as described
  in the Unicode Technical Report and implemented by UBiDi when the reordering
  mode is set to :attr:`~UBiDiReorderingMode.UBIDI_REORDER_DEFAULT`.
  Visual RTL mode is not supported by UBiDi and is accomplished through
  reversing a visual LTR string,
- <visual input, logical output>: unless the input direction is RTL,
  this corresponds to an "inverse bidi algorithm" in UBiDi with the reordering
  mode set to :attr:`~UBiDiReorderingMode.UBIDI_REORDER_INVERSE_LIKE_DIRECT`.
  Visual RTL mode is not supported by UBiDi and is accomplished through
  reversing a visual LTR string,
- <logical input, logical output>: if the input and output base directions
  mismatch, this corresponds to the UBiDi implementation with the reordering
  mode set to :attr:`~UBiDiReorderingMode.UBIDI_REORDER_RUNS_ONLY`;
  and if the input and output base directions are identical, the
  transformation engine will only handle character mirroring and Arabic shaping
  operations without reordering,
- <visual input, visual output>: this reordering mode is not supported by the
  UBiDi engine; it implies character mirroring, Arabic shaping, and - if the
  input/output base directions mismatch - string reverse operations.

See Also:
    :func:`ubidi_set_inverse`
    :func:`ubidi_set_reordering_mode`
      )doc")
      .value("UBIDI_LOGICAL", UBIDI_LOGICAL, R"doc(
             Constant indicating a logical order.

             This is the default for input text.
             )doc")
      .value("UBIDI_VISUAL", UBIDI_VISUAL, R"doc(
             Constant indicating a visual order.

             This is a default for output text.
             )doc")
      .export_values();

  //
  // struct UBiDiTransform
  //
  py::class_<icupy::UBiDiTransformPtr>(m, "UBiDiTransform", R"doc(
    UBiDiTransform structure that stores information used by the layout
    transformation engine.

    See Also:
        :func:`ubiditransform_close`
        :func:`ubiditransform_open`
    )doc");

  //
  // Functions
  //
  m.def(
      "ubiditransform_close",
      [](icupy::UBiDiTransformPtr &bidi_transform) {
        ubiditransform_close(bidi_transform);
      },
      py::arg("bidi_transform"));

  m.def("ubiditransform_open", []() {
    ErrorCode error_code;
    auto bidi_transform = ubiditransform_open(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::make_unique<icupy::UBiDiTransformPtr>(bidi_transform);
  });

  m.def(
      "ubiditransform_transform",
      [](icupy::UBiDiTransformPtr &bidi_transform, const std::u16string &src,
         int32_t src_length, UBiDiLevel in_para_level, UBiDiOrder in_order,
         UBiDiLevel out_para_level, UBiDiOrder out_order,
         UBiDiMirroring do_mirroring, uint32_t shaping_options) {
        auto dest_size = src_length;
        if (dest_size < 0) {
          dest_size = static_cast<int32_t>(src.size());
        }
        if (shaping_options & U_SHAPE_LETTERS_UNSHAPE) {
          dest_size *= 2;
        }
        std::u16string result(dest_size, u'\0');
        ErrorCode error_code;
        dest_size = ubiditransform_transform(
            bidi_transform, src.data(), src_length, result.data(), dest_size,
            in_para_level, in_order, out_para_level, out_order, do_mirroring,
            shaping_options, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        result.resize(dest_size);
        return result;
      },
      py::arg("bidi_transform"), py::arg("src"), py::arg("src_length"),
      py::arg("in_para_level"), py::arg("in_order"), py::arg("out_para_level"),
      py::arg("out_order"), py::arg("do_mirroring"),
      py::arg("shaping_options"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
}

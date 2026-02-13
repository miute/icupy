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
  py::enum_<UBiDiMirroring>(
      m, "UBiDiMirroring", py::arithmetic(),
      "*UBiDiMirroring* indicates whether or not characters with the "
      "\"mirrored\" property in "
      "RTL runs should be replaced with their mirror-image counterparts.")
      .value("UBIDI_MIRRORING_OFF", UBIDI_MIRRORING_OFF,
             "0: Constant indicating that character mirroring should not be "
             "performed.\n\n  "
             "This is the default.")
      .value("UBIDI_MIRRORING_ON", UBIDI_MIRRORING_ON,
             "1: Constant indicating that character mirroring should be "
             "performed.\n\n  "
             "This corresponds to calling *ubidi_write_reordered* or "
             "*ubidi_write_reverse* with the "
             "*UBIDI_DO_MIRRORING* option bit set.")
      .export_values();

  //
  // enum UBiDiOrder
  //
  py::enum_<UBiDiOrder>(
      m, "UBiDiOrder", py::arithmetic(),
      "*UBiDiOrder* indicates the order of text.\n\n"
      "This bidi transformation engine supports all possible combinations (4 "
      "in total) of input and output text "
      "order:\n\n"
      "* <logical input, visual output>: unless the output direction is RTL, "
      "this corresponds to a normal operation of "
      "the Bidi algorithm as described in the Unicode Technical Report and "
      "implemented by *UBiDi* when the reordering "
      "mode is set to *UBIDI_REORDER_DEFAULT*. Visual RTL mode is not "
      "supported by *UBiDi* and is accomplished through "
      "reversing a visual LTR string,\n"
      "* <visual input, logical output>: unless the input direction is RTL, "
      "this corresponds to an \"inverse bidi "
      "algorithm\" in *UBiDi* with the reordering mode set to "
      "*UBIDI_REORDER_INVERSE_LIKE_DIRECT*. Visual RTL mode is "
      "not not supported by *UBiDi* and is accomplished through reversing a "
      "visual LTR string,\n"
      "* <logical input, logical output>: if the input and output base "
      "directions mismatch, this corresponds to the "
      "*UBiDi* implementation with the reordering mode set to "
      "*UBIDI_REORDER_RUNS_ONLY*; and if the input and output "
      "base directions are identical, the transformation engine will only "
      "handle character mirroring and Arabic "
      "shaping operations without reordering,\n"
      "* <visual input, visual output>: this reordering mode is not supported "
      "by the *UBiDi* engine; it implies "
      "character mirroring, Arabic shaping, and - if the input/output base "
      "directions mismatch - string reverse "
      "operations.")
      .value("UBIDI_LOGICAL", UBIDI_LOGICAL,
             "0: Constant indicating a logical order.\n\n  "
             "This is the default for input text.")
      .value("UBIDI_VISUAL", UBIDI_VISUAL,
             "1: Constant indicating a visual order.\n\n  "
             "This is a default for output text.")
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

import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 58:
    pytest.skip("ICU4C<58", allow_module_level=True)

from icupy.utils import gc


def test_api():
    # UBiDiTransform *ubiditransform_open(UErrorCode *pErrorCode)
    # void ubiditransform_close(UBiDiTransform *pBidiTransform)
    with gc(icu.ubiditransform_open(), icu.ubiditransform_close) as transform:
        # From icu/source/test/cintltst/cbiditransformtst.c
        in_text = (
            "a[b]c \\u05d0(\\u05d1\\u05d2 \\u05d3)\\u05d4 1 d \\u0630 "
            "23\\u0660 e\\u06314 f \\ufeaf \\u0661\\u0662"
        )
        out_text = (
            "a[b]c 1 \\u05d4(\\u05d3 \\u05d2\\u05d1)\\u05d0 d 23\\u0660 "
            "\\u0630 e4\\u0631 f \\u0661\\u0662 \\ufeaf"
        )
        src = icu.u_unescape(in_text)
        expected = icu.u_unescape(out_text)

        # uint32_t ubiditransform_transform(
        #       UBiDiTransform *pBiDiTransform,
        #       const UChar *src,
        #       int32_t srcLength,
        #       UChar *dest,
        #       int32_t destSize,
        #       UBiDiLevel inParaLevel,
        #       UBiDiOrder inOrder,
        #       UBiDiLevel outParaLevel,
        #       UBiDiOrder outOrder,
        #       UBiDiMirroring doMirroring,
        #       uint32_t shapingOptions,
        #       UErrorCode *pErrorCode
        # )
        dest = icu.ubiditransform_transform(
            transform,
            src,
            len(src),
            icu.UBIDI_LTR,
            icu.UBiDiOrder.UBIDI_LOGICAL,
            icu.UBIDI_LTR,
            icu.UBiDiOrder.UBIDI_VISUAL,
            icu.UBIDI_MIRRORING_ON,
            0,
        )
        assert isinstance(dest, str)
        assert dest == expected

        out_text = (
            "a[b]c 1 \\u05d4)\\u05d3 \\u05d2\\u05d1(\\u05d0 d "
            "\\u0662\\u0663\\u0660 \\u0630 e\\u0664\\u0631 f \\u0661\\u0662 "
            "\\ufeaf"
        )
        expected = icu.u_shape_arabic(
            icu.u_unescape(out_text),
            -1,
            icu.U_SHAPE_DIGITS_ALEN2AN_INIT_LR | icu.U_SHAPE_LETTERS_UNSHAPE,
        )
        dest = icu.ubiditransform_transform(
            transform,
            src,
            -1,
            icu.UBIDI_LTR,
            icu.UBiDiOrder.UBIDI_LOGICAL,
            icu.UBIDI_LTR,
            icu.UBiDiOrder.UBIDI_VISUAL,
            icu.UBIDI_MIRRORING_OFF,
            icu.U_SHAPE_DIGITS_ALEN2AN_INIT_LR | icu.U_SHAPE_LETTERS_UNSHAPE,
        )
        assert isinstance(dest, str)
        assert dest == expected

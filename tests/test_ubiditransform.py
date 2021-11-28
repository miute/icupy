import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 58:
    pytest.skip("ICU4C<58", allow_module_level=True)

from icupy.icu import (
    UBIDI_LTR, UBIDI_MIRRORING_OFF, UBIDI_MIRRORING_ON, UBiDiOrder,
    U_SHAPE_DIGITS_ALEN2AN_INIT_LR, U_SHAPE_LETTERS_UNSHAPE,
    u_shape_arabic, u_unescape, ubiditransform_close, ubiditransform_open,
    ubiditransform_transform,
)
from icupy.utils import gc


def test_api():
    # UBiDiTransform *ubiditransform_open(UErrorCode *pErrorCode)
    # void ubiditransform_close(UBiDiTransform *pBidiTransform)
    with gc(ubiditransform_open(), ubiditransform_close) as transform:
        # From icu/source/test/cintltst/cbiditransformtst.c
        in_text = (
            "a[b]c \\u05d0(\\u05d1\\u05d2 \\u05d3)\\u05d4 1 d \\u0630 "
            "23\\u0660 e\\u06314 f \\ufeaf \\u0661\\u0662"
        )
        out_text = (
            "a[b]c 1 \\u05d4(\\u05d3 \\u05d2\\u05d1)\\u05d0 d 23\\u0660 "
            "\\u0630 e4\\u0631 f \\u0661\\u0662 \\ufeaf"
        )
        src = u_unescape(in_text)
        expected = u_unescape(out_text)

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
        dest = ubiditransform_transform(
            transform,
            src,
            len(src),
            UBIDI_LTR,
            UBiDiOrder.UBIDI_LOGICAL,
            UBIDI_LTR,
            UBiDiOrder.UBIDI_VISUAL,
            UBIDI_MIRRORING_ON,
            0)
        assert isinstance(dest, str)
        assert dest == expected

        out_text = (
            "a[b]c 1 \\u05d4)\\u05d3 \\u05d2\\u05d1(\\u05d0 d "
            "\\u0662\\u0663\\u0660 \\u0630 e\\u0664\\u0631 f \\u0661\\u0662 "
            "\\ufeaf"
        )
        expected = u_shape_arabic(
            u_unescape(out_text),
            -1,
            U_SHAPE_DIGITS_ALEN2AN_INIT_LR | U_SHAPE_LETTERS_UNSHAPE)
        dest = ubiditransform_transform(
            transform,
            src,
            -1,
            UBIDI_LTR,
            UBiDiOrder.UBIDI_LOGICAL,
            UBIDI_LTR,
            UBiDiOrder.UBIDI_VISUAL,
            UBIDI_MIRRORING_OFF,
            U_SHAPE_DIGITS_ALEN2AN_INIT_LR | U_SHAPE_LETTERS_UNSHAPE)
        assert isinstance(dest, str)
        assert dest == expected

from icupy.icu import (
    U_SHAPE_DIGITS_AN2EN, U_SHAPE_DIGITS_EN2AN,
    U_SHAPE_DIGIT_TYPE_AN, U_SHAPE_DIGIT_TYPE_AN_EXTENDED,
    u_shape_arabic,
)


def test_api():
    # From icu/source/test/cintltst/cbiditst.c
    source = "\x31\u0627\x32\u06f3\x61\x34"

    # int32_t u_shapeArabic(
    #       const UChar *source,
    #       int32_t sourceLength,
    #       UChar *dest,
    #       int32_t destSize,
    #       uint32_t options,
    #       UErrorCode *pErrorCode
    # )
    # european -> arabic
    options = U_SHAPE_DIGITS_EN2AN | U_SHAPE_DIGIT_TYPE_AN
    dest = u_shape_arabic(source, len(source), options)
    assert isinstance(dest, str)
    assert dest == "\u0661\u0627\u0662\u06f3\x61\u0664"

    # arabic -> european
    options = U_SHAPE_DIGITS_AN2EN | U_SHAPE_DIGIT_TYPE_AN_EXTENDED
    dest = u_shape_arabic(source, -1, options)
    assert isinstance(dest, str)
    assert dest == "\x31\u0627\x32\x33\x61\x34"

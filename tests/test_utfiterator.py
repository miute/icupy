import pytest

from icupy import icu


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 78, reason="ICU4C<78")
def test_all_code_points() -> None:
    # template<typename CP32>
    # class U_HEADER_ONLY_NAMESPACE::AllCodePoints< CP32 >
    it = icu.header.AllCodePoints()
    for c in it:
        assert icu.u_is_code_point(c)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 78, reason="ICU4C<78")
def test_all_scalar_values() -> None:
    # template<typename CP32>
    # class U_HEADER_ONLY_NAMESPACE::AllScalarValues< CP32 >
    it = icu.header.AllScalarValues()
    for c in it:
        assert icu.u_is_scalar_value(c)


@pytest.mark.parametrize(("c", "expected"), [(0xFFFF, True), (0xFFFF + 1, False)])
def test_u_is_bmp(c: int, expected: bool) -> None:
    # #define U_IS_BMP(c)	    ((uint32_t)(c)<=0xffff)
    assert icu.u_is_bmp(c) is expected  # (U+0000..U+ffff)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 78, reason="ICU4C<78")
@pytest.mark.parametrize(("c", "expected"), [(0x10FFFF, True), (0x10FFFF + 1, False)])
def test_u_is_code_point(c: int, expected: bool) -> None:
    # #define U_IS_CODE_POINT(c)	((uint32_t)(c)<=0x10ffff)
    assert icu.u_is_code_point(c) is expected  # (U+0000..U+10FFFF)


@pytest.mark.parametrize(
    ("c", "expected"),
    [(0xD800 - 1, False), (0xD800, True), (0xDBFF, True), (0xDBFF + 1, False)],
)
def test_u_is_lead(c: int, expected: bool) -> None:
    # #define U_IS_LEAD(c)      (((c)&0xfffffc00)==0xd800)
    assert icu.u_is_lead(c) is expected  # (U+d800..U+dbff)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 78, reason="ICU4C<78")
@pytest.mark.parametrize(
    ("c", "expected"),
    [
        (0xD800 - 1, True),
        (0xD800, False),
        (0xE000 - 1, False),
        (0xE000, True),
        (0x10FFFF, True),
        (0x10FFFF + 1, False),
    ],
)
def test_u_is_scalar_value(c: int, expected: bool) -> None:
    # #define U_IS_SCALAR_VALUE(c)  ((uint32_t)(c)<0xd800 || (0xe000<=(c) && (c)<=0x10ffff))
    assert icu.u_is_scalar_value(c) is expected


@pytest.mark.parametrize(
    ("c", "expected"),
    [
        (0x10000 - 1, False),
        (0x10000, True),
        (0x10FFFF - 1, True),
        (0x10FFFF, True),
        (0x10FFFF + 1, False),
    ],
)
def test_u_is_supplementary(c: int, expected: bool) -> None:
    # #define U_IS_SUPPLEMENTARY(c) ((uint32_t)((c)-0x10000)<=0xfffff)
    assert icu.u_is_supplementary(c) is expected  # (U+10000..U+10ffff)


@pytest.mark.parametrize(
    ("c", "expected"),
    [(0xD800 - 1, False), (0xD800, True), (0xDFFF, True), (0xDFFF + 1, False)],
)
def test_u_is_surrogate(c: int, expected: bool) -> None:
    # #define U_IS_SURROGATE(c)     (((c)&0xfffff800)==0xd800)
    assert icu.u_is_surrogate(c) is expected  # (U+d800..U+dfff)


@pytest.mark.parametrize(
    ("c", "expected"),
    [(0xD800 - 1, False), (0xD800, True), (0xDBFF, True), (0xDBFF + 1, False)],
)
def test_u_is_surrogate_lead(c: int, expected: bool) -> None:
    # #define U_IS_SURROGATE_LEAD(c)    (((c)&0x400)==0)
    assert icu.u_is_surrogate_lead(c) is expected  # (U+d800..U+dbff)


@pytest.mark.parametrize(
    ("c", "expected"),
    [(0xDC00 - 1, False), (0xDC00, True), (0xDFFF, True), (0xDFFF + 1, False)],
)
def test_u_is_surrogate_trail(c: int, expected: bool) -> None:
    # #define U_IS_SURROGATE_TRAIL(c)   (((c)&0x400)!=0)
    assert icu.u_is_surrogate_trail(c) is expected  # (U+dc00..U+dfff)


@pytest.mark.parametrize(
    ("c", "expected"),
    [(0xDC00 - 1, False), (0xDC00, True), (0xDFFF, True), (0xDFFF + 1, False)],
)
def test_u_is_trail(c: int, expected: bool) -> None:
    # #define U_IS_TRAIL(c)     (((c)&0xfffffc00)==0xdc00)
    assert icu.u_is_trail(c) is expected  # (U+dc00..U+dfff)


@pytest.mark.parametrize(
    ("c", "expected"),
    [
        (0xD800 - 1, True),
        (0xD800, False),
        (0xE000 - 1, False),
        (0xE000, True),
        (0x10FFFF, False),
        (0x10FFFF + 1, False),
    ],
)
def test_u_is_unicode_char(c: int, expected: bool) -> None:
    # #define U_IS_UNICODE_CHAR(c)
    #       ((uint32_t)(c)<0xd800 || \
    #           (0xe000<=(c) && (c)<=0x10ffff && !U_IS_UNICODE_NONCHAR(c)))
    assert icu.u_is_unicode_char(c) is expected


@pytest.mark.parametrize(
    ("c", "expected"),
    [
        (0xFDD0 - 1, False),
        (0xFDD0, True),
        (0xFDEF, True),
        (0xFDEF + 1, False),
        (0x10FFFE - 1, False),
        (0x10FFFE, True),
        (0x10FFFF, True),
        (0x10FFFF + 1, False),
    ],
)
def test_u_is_unicode_nonchar(c: int, expected: bool) -> None:
    # #define U_IS_UNICODE_NONCHAR(c)
    #       ((c)>=0xfdd0 && \
    #           ((c)<=0xfdef || ((c)&0xfffe)==0xfffe) && (c)<=0x10ffff)
    assert icu.u_is_unicode_nonchar(c) is expected

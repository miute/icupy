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


def test_u16_back() -> None:
    # [trail, trail, lead, trail, 0, ...]
    lead, trail = 0xD83D, 0xDE08
    s = icu.UnicodeString(trail).append(trail).append(lead).append(trail)
    p = s.get_terminated_buffer()

    # #define U16_BACK_1(s, start, i)
    # start: 0 <= start < buffer_size
    # i: 0 < i < buffer_size
    with pytest.raises(IndexError):
        icu.u16_back_1(p, -1, 1)  # start < 0
    assert icu.u16_back_1(p, len(p) - 1, len(p) - 1) >= 0
    with pytest.raises(IndexError):
        icu.u16_back_1(p, len(p), len(p) - 1)  # start >= buffer_size
    with pytest.raises(IndexError):
        icu.u16_back_1(p, 0, 0)  # i <= 0
    assert icu.u16_back_1(p, 0, 1) == 0
    assert icu.u16_back_1(p, 0, 2) == 1
    assert icu.u16_back_1(p, 0, 3) == 2
    assert icu.u16_back_1(p, 0, 4) == 2
    assert icu.u16_back_1(p, 0, 5) == 4
    assert icu.u16_back_1(p, 0, len(p) - 1) >= 0  # undefined
    with pytest.raises(IndexError):
        icu.u16_back_1(p, 0, len(p))  # i >= buffer_size

    # #define U16_BACK_N(s, start, i, n)
    with pytest.raises(IndexError):
        icu.u16_back_n(p, -1, 1, 2)  # start < 0
    assert icu.u16_back_n(p, len(p) - 1, len(p) - 1, 2) >= 0
    with pytest.raises(IndexError):
        icu.u16_back_n(p, len(p), len(p) - 1, 2)  # start >= buffer_size
    with pytest.raises(IndexError):
        icu.u16_back_n(p, 0, 0, 2)  # i <= 0
    assert icu.u16_back_n(p, 0, 1, 2) == 0
    assert icu.u16_back_n(p, 0, 2, 2) == 0
    assert icu.u16_back_n(p, 0, 3, 2) == 1
    assert icu.u16_back_n(p, 0, 4, 2) == 1
    assert icu.u16_back_n(p, 0, 5, 2) == 2
    assert icu.u16_back_n(p, 0, len(p) - 1, 2) >= 0  # undefined
    with pytest.raises(IndexError):
        icu.u16_back_n(p, 0, len(p), 2)  # i >= buffer_size


def test_u16_fwd() -> None:
    # [lead, lead, trail, lead, 0, ...]
    lead, trail = 0xD83D, 0xDE08
    s = icu.UnicodeString(lead).append(lead).append(trail).append(lead)
    p = s.get_terminated_buffer()

    # #define U16_FWD_1(s, i, limit)
    # i: 0 <= i < buffer_size - 1
    # limit: 0 <= limit <= buffer_size - 1
    with pytest.raises(IndexError):
        icu.u16_fwd_1(p, -1, -1)  # i < 0
    assert icu.u16_fwd_1(p, 0, -1) == 1
    assert icu.u16_fwd_1(p, 1, -1) == 3
    assert icu.u16_fwd_1(p, 2, -1) == 3
    assert icu.u16_fwd_1(p, 3, -1) == 4
    with pytest.raises(IndexError):
        icu.u16_fwd_1(p, 4, -1)  # i >= limit

    with pytest.raises(IndexError):
        icu.u16_fwd_1(p, 0, 0)  # i >= limit
    assert icu.u16_fwd_1(p, 0, len(p) - 1) >= 0  # undefined
    with pytest.raises(IndexError):
        icu.u16_fwd_1(p, 0, len(p))  # limit >= buffer_size
    assert icu.u16_fwd_1(p, 0, 4) == 1
    assert icu.u16_fwd_1(p, 1, 4) == 3
    assert icu.u16_fwd_1(p, 2, 4) == 3
    assert icu.u16_fwd_1(p, 3, 4) == 4
    with pytest.raises(IndexError):
        icu.u16_fwd_1(p, 4, 4)  # i >= limit

    # #define U16_FWD_N(s, i, limit, n)
    with pytest.raises(IndexError):
        icu.u16_fwd_n(p, -1, -1, 2)  # i < 0
    assert icu.u16_fwd_n(p, 0, -1, 2) == 3
    assert icu.u16_fwd_n(p, 1, -1, 2) == 4
    assert icu.u16_fwd_n(p, 2, -1, 2) == 4
    assert icu.u16_fwd_n(p, 3, -1, 2) >= 0  # undefined
    with pytest.raises(IndexError):
        icu.u16_fwd_n(p, 4, -1, 2)  # i >= limit

    with pytest.raises(IndexError):
        icu.u16_fwd_n(p, 0, 0, 2)  # i >= limit
    assert icu.u16_fwd_n(p, 0, len(p) - 1, 2) >= 0  # undefined
    with pytest.raises(IndexError):
        icu.u16_fwd_n(p, 0, len(p), 2)  # limit >= buffer_size
    assert icu.u16_fwd_n(p, 0, 4, 2) == 3
    assert icu.u16_fwd_n(p, 1, 4, 2) == 4
    assert icu.u16_fwd_n(p, 2, 4, 2) == 4
    assert icu.u16_fwd_n(p, 3, 4, 2) >= 0  # undefined
    with pytest.raises(IndexError):
        icu.u16_fwd_n(p, 4, 4, 2)  # i >= limit


def test_u16_get() -> None:
    # [0x61, lead, trail, lead, trail, 0, ...]
    lead, trail = 0xD83D, 0xDE08
    s = icu.UnicodeString("a")
    s.append(lead).append(trail).append(lead).append(trail)
    p = s.get_terminated_buffer()

    # #define U16_GET(s, start, i, limit)
    with pytest.raises(IndexError):
        icu.u16_get(p, 0, -1, -1)  # i < 0
    assert icu.u16_get(p, 0, 0, -1) == 0x61
    assert icu.u16_get(p, 0, 1, -1) == 0x1F608
    assert icu.u16_get(p, 0, 2, -1) == 0x1F608
    assert icu.u16_get(p, 0, 3, -1) == 0x1F608
    assert icu.u16_get(p, 0, 4, -1) == 0x1F608
    with pytest.raises(IndexError):
        icu.u16_get(p, 0, 5, -1)  # i >= limit

    with pytest.raises(IndexError):
        icu.u16_get(p, 0, 0, 0)  # i >= limit
    assert icu.u16_get(p, 0, 0, 4) == 0x61
    assert icu.u16_get(p, 0, 1, 4) == 0x1F608
    assert icu.u16_get(p, 0, 2, 4) == 0x1F608
    assert icu.u16_get(p, 0, 3, 4) == lead
    with pytest.raises(IndexError):
        icu.u16_get(p, 0, 4, 4)  # i >= limit
    assert icu.u16_get(p, 0, 0, len(p) - 1) == 0x61
    with pytest.raises(IndexError):
        icu.u16_get(p, 0, 0, len(p))  # limit >= buffer_size

    # #define U16_GET_OR_FFFD(s, start, i, limit)
    with pytest.raises(IndexError):
        icu.u16_get_or_fffd(p, 0, -1, -1)  # i < 0
    assert icu.u16_get_or_fffd(p, 0, 0, -1) == 0x61
    assert icu.u16_get_or_fffd(p, 0, 1, -1) == 0x1F608
    assert icu.u16_get_or_fffd(p, 0, 2, -1) == 0x1F608
    assert icu.u16_get_or_fffd(p, 0, 3, -1) == 0x1F608
    assert icu.u16_get_or_fffd(p, 0, 4, -1) == 0x1F608
    with pytest.raises(IndexError):
        icu.u16_get_or_fffd(p, 0, 5, -1)  # i >= limit

    with pytest.raises(IndexError):
        icu.u16_get_or_fffd(p, 0, 0, 0)  # i >= limit
    assert icu.u16_get_or_fffd(p, 0, 0, 4) == 0x61
    assert icu.u16_get_or_fffd(p, 0, 1, 4) == 0x1F608
    assert icu.u16_get_or_fffd(p, 0, 2, 4) == 0x1F608
    assert icu.u16_get_or_fffd(p, 0, 3, 4) == 0xFFFD
    with pytest.raises(IndexError):
        icu.u16_get_or_fffd(p, 0, 4, 4)  # i >= limit
    assert icu.u16_get_or_fffd(p, 0, 0, len(p) - 1) == 0x61
    with pytest.raises(IndexError):
        icu.u16_get_or_fffd(p, 0, 0, len(p))  # limit >= buffer_size

    # #define U16_GET_SUPPLEMENTARY(lead, trail)
    assert icu.u16_get_supplementary(lead, trail) == 0x1F608


def test_u16_is() -> None:
    lead, trail = 0xD83D, 0xDE08

    # #define U16_IS_LEAD(c)
    assert icu.u16_is_lead(0x61) is False
    assert icu.u16_is_lead(lead) is True
    assert icu.u16_is_lead(trail) is False

    # #define U16_IS_SINGLE(c)
    assert icu.u16_is_single(0x61) is True
    assert icu.u16_is_single(lead) is False
    assert icu.u16_is_single(trail) is False

    # #define U16_IS_SURROGATE(c)
    assert icu.u16_is_surrogate(0x61) is False
    assert icu.u16_is_surrogate(lead) is True
    assert icu.u16_is_surrogate(trail) is True

    # #define U16_IS_SURROGATE_LEAD(c)
    assert icu.u16_is_surrogate_lead(lead) is True
    assert icu.u16_is_surrogate_lead(trail) is False

    # #define U16_IS_SURROGATE_TRAIL(c)
    assert icu.u16_is_surrogate_trail(lead) is False
    assert icu.u16_is_surrogate_trail(trail) is True

    # #define U16_IS_TRAIL(c)
    assert icu.u16_is_trail(0x61) is False
    assert icu.u16_is_trail(lead) is False
    assert icu.u16_is_trail(trail) is True

    # #define U16_LEAD(supplementary)
    assert icu.u16_lead(0x1F608) == lead

    # #define U16_LENGTH(c)
    assert icu.u16_length(0x61) == 1
    assert icu.u16_length(0x1F608) == 2

    # #define U16_TRAIL(supplementary)
    assert icu.u16_trail(0x1F608) == trail


def test_u16_next() -> None:
    # [0x61, lead, trail, lead, trail, 0, ...]
    lead, trail = 0xD83D, 0xDE08
    s = icu.UnicodeString("a")
    s.append(lead).append(trail).append(lead).append(trail)
    p = s.get_terminated_buffer()

    # #define U16_NEXT(s, i, limit, c)
    with pytest.raises(IndexError):
        icu.u16_next(p, -1, -1)  # i < 0
    assert icu.u16_next(p, 0, -1) == (0x61, 1)
    assert icu.u16_next(p, 1, -1) == (0x1F608, 3)
    assert icu.u16_next(p, 2, -1) == (trail, 3)
    assert icu.u16_next(p, 3, -1) == (0x1F608, 5)
    assert icu.u16_next(p, 4, -1) == (trail, 5)
    with pytest.raises(IndexError):
        icu.u16_next(p, 5, -1)  # i >= limit

    with pytest.raises(IndexError):
        icu.u16_next(p, 0, 0)  # i >= limit
    assert icu.u16_next(p, 0, len(p) - 1)  # undefined
    with pytest.raises(IndexError):
        icu.u16_next(p, 0, len(p))  # limit >= buffer_size
    assert icu.u16_next(p, 0, 4) == (0x61, 1)
    assert icu.u16_next(p, 1, 4) == (0x1F608, 3)
    assert icu.u16_next(p, 2, 4) == (trail, 3)
    assert icu.u16_next(p, 3, 4) == (lead, 4)
    with pytest.raises(IndexError):
        icu.u16_next(p, 4, 4)  # i >= limit

    # #define U16_NEXT_OR_FFFD(s, i, limit, c)
    with pytest.raises(IndexError):
        icu.u16_next_or_fffd(p, -1, -1)  # i < 0
    assert icu.u16_next_or_fffd(p, 0, -1) == (0x61, 1)
    assert icu.u16_next_or_fffd(p, 1, -1) == (0x1F608, 3)
    assert icu.u16_next_or_fffd(p, 2, -1) == (0xFFFD, 3)
    assert icu.u16_next_or_fffd(p, 3, -1) == (0x1F608, 5)
    assert icu.u16_next_or_fffd(p, 4, -1) == (0xFFFD, 5)
    with pytest.raises(IndexError):
        icu.u16_next_or_fffd(p, 5, -1)  # i >= limit

    with pytest.raises(IndexError):
        icu.u16_next_or_fffd(p, 0, 0)  # i >= limit
    assert icu.u16_next_or_fffd(p, 0, len(p) - 1)  # undefined
    with pytest.raises(IndexError):
        icu.u16_next_or_fffd(p, 0, len(p))  # limit >= buffer_size
    assert icu.u16_next_or_fffd(p, 0, 4) == (0x61, 1)
    assert icu.u16_next_or_fffd(p, 1, 4) == (0x1F608, 3)
    assert icu.u16_next_or_fffd(p, 2, 4) == (0xFFFD, 3)
    assert icu.u16_next_or_fffd(p, 3, 4) == (0xFFFD, 4)
    with pytest.raises(IndexError):
        icu.u16_next_or_fffd(p, 4, 4)  # i >= limit


def test_u16_prev() -> None:
    # [0x61, lead, trail, lead, trail, 0, ...]
    lead, trail = 0xD83D, 0xDE08
    s = icu.UnicodeString("a")
    s.append(lead).append(trail).append(lead).append(trail)
    p = s.get_terminated_buffer()

    # #define U16_PREV(s, start, i, c)
    # start: 0 <= start < buffer_size
    # i: 0 < i < buffer_size
    with pytest.raises(IndexError):
        icu.u16_prev(p, -1, 1)  # start < 0
    assert icu.u16_prev(p, len(p) - 1, len(p) - 1)
    with pytest.raises(IndexError):
        icu.u16_prev(p, len(p), len(p) - 1)  # start >= buffer_size
    with pytest.raises(IndexError):
        icu.u16_prev(p, 0, 0)  # i <= 0
    assert icu.u16_prev(p, 0, 1) == (0x61, 0)
    assert icu.u16_prev(p, 0, 2) == (lead, 1)
    assert icu.u16_prev(p, 0, 3) == (0x1F608, 1)
    assert icu.u16_prev(p, 0, 4) == (lead, 3)
    assert icu.u16_prev(p, 0, 5) == (0x1F608, 3)
    assert icu.u16_prev(p, 0, 6) == (0, 5)
    assert icu.u16_prev(p, 0, len(p) - 1)  # undefined
    with pytest.raises(IndexError):
        icu.u16_prev(p, 0, len(p))  # i >= buffer_size

    # #define U16_PREV_OR_FFFD(s, start, i, c)
    with pytest.raises(IndexError):
        icu.u16_prev_or_fffd(p, -1, 1)  # start < 0
    assert icu.u16_prev_or_fffd(p, len(p) - 1, len(p) - 1)
    with pytest.raises(IndexError):
        icu.u16_prev_or_fffd(p, len(p), len(p) - 1)  # start >= buffer_size
    with pytest.raises(IndexError):
        icu.u16_prev_or_fffd(p, 0, 0)  # i <= 0
    assert icu.u16_prev_or_fffd(p, 0, 1) == (0x61, 0)
    assert icu.u16_prev_or_fffd(p, 0, 2) == (0xFFFD, 1)
    assert icu.u16_prev_or_fffd(p, 0, 3) == (0x1F608, 1)
    assert icu.u16_prev_or_fffd(p, 0, 4) == (0xFFFD, 3)
    assert icu.u16_prev_or_fffd(p, 0, 5) == (0x1F608, 3)
    assert icu.u16_prev_or_fffd(p, 0, 6) == (0, 5)
    assert icu.u16_prev_or_fffd(p, 0, len(p) - 1)  # undefined
    with pytest.raises(IndexError):
        icu.u16_prev_or_fffd(p, 0, len(p))  # i >= buffer_size

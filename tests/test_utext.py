import pytest
from icupy.icu import (
    ICUError, StringCharacterIterator, UErrorCode, UTextVector,
    U_SENTINEL, UnicodeString,
    utext_char32_at, utext_clone, utext_close, utext_copy, utext_current32,
    utext_equals, utext_extract, utext_freeze, utext_get_native_index,
    utext_get_previous_native_index, utext_has_meta_data,
    utext_is_length_expensive, utext_is_writable, utext_move_index32,
    utext_native_length, utext_next32, utext_next32_from,
    utext_open_character_iterator, utext_open_const_unicode_string,
    utext_open_replaceable, utext_open_uchars, utext_open_unicode_string,
    utext_open_utf8, utext_previous32, utext_previous32_from, utext_replace,
    utext_set_native_index,
)
from icupy.utils import gc


def test_api():
    s = "\x41\U0001f338\x42"
    with gc(utext_open_uchars(None, s, -1), utext_close) as ut:
        assert ut.magic
        assert ut.flags
        assert ut.provider_properties
        assert ut.p_funcs
        assert utext_char32_at(ut, 0) == 0x41
        assert utext_char32_at(ut, 1) == 0x1f338
        assert utext_char32_at(ut, 2) == 0x1f338
        assert utext_char32_at(ut, 3) == 0x42
        assert utext_char32_at(ut, 4) == U_SENTINEL

        # int32_t utext_extract(UText *ut,
        #                       int64_t nativeStart,
        #                       int64_t nativeLimit,
        #                       UChar *dest,
        #                       int32_t destCapacity,
        #                       UErrorCode *status
        # )
        dest = utext_extract(ut, 1, 3)
        assert isinstance(dest, str)
        assert dest == "\U0001f338"

        utext_set_native_index(ut, 0)
        assert utext_current32(ut) == 0x41
        assert utext_get_native_index(ut) == 0
        assert utext_get_previous_native_index(ut) == 0

        assert not utext_has_meta_data(ut)
        assert not utext_is_length_expensive(ut)
        assert not utext_is_writable(ut)

        utext_move_index32(ut, 1)
        assert utext_native_length(ut) == 4
        assert utext_next32(ut) == 0x1f338
        assert utext_next32(ut) == 0x42
        assert utext_next32(ut) == U_SENTINEL

        assert utext_next32_from(ut, 2) == 0x1f338
        assert utext_next32(ut) == 0x42

        assert utext_previous32(ut) == 0x42
        assert utext_previous32(ut) == 0x1f338
        assert utext_previous32(ut) == 0x41
        assert utext_previous32(ut) == U_SENTINEL

        assert utext_previous32_from(ut, 3) == 0x1f338
        assert utext_previous32(ut) == 0x41


def test_clone_uchars():
    s1 = "ABC"
    s2 = "abc"
    ut1 = utext_open_uchars(None, s1, -1)
    assert not utext_is_writable(ut1)

    # Deep clone
    ut2 = utext_clone(None, ut1, True, True)
    assert ut2 is not None
    assert ut2 != ut1
    assert not utext_is_writable(ut2)
    assert utext_extract(ut2, 0, utext_native_length(ut2)) == "ABC"
    assert utext_close(ut2) is None

    ut2 = utext_open_uchars(None, s2, -1)
    ut3 = utext_clone(ut2, ut1, True, True)
    assert ut3 is not None
    assert ut3 == ut2
    assert ut3 != ut1
    assert not utext_is_writable(ut3)
    assert utext_extract(ut3, 0, utext_native_length(ut3)) == "ABC"
    assert utext_close(ut2) is None
    assert utext_close(ut3) == ut2

    ut2 = utext_open_uchars(None, s2, -1)
    utext_clone(ut2, ut1, True, False)
    assert ut2 != ut1
    assert not utext_is_writable(ut2)
    assert utext_extract(ut2, 0, utext_native_length(ut2)) == "ABC"
    assert utext_close(ut2) is None

    # Shallow clone
    ut4 = utext_clone(None, ut1, False, True)
    assert ut4 is not None
    assert ut4 != ut1
    assert not utext_is_writable(ut4)
    assert utext_extract(ut4, 0, utext_native_length(ut4)) == "ABC"
    assert utext_close(ut4) is None

    ut5 = utext_open_uchars(None, s2, -1)
    ut6 = utext_clone(ut5, ut1, False, True)
    assert ut6 is not None
    assert ut6 == ut5
    assert ut6 != ut1
    assert not utext_is_writable(ut6)
    assert utext_extract(ut6, 0, utext_native_length(ut6)) == "ABC"
    assert utext_close(ut5) is None
    assert utext_close(ut6) == ut5

    ut5 = utext_open_uchars(None, s2, -1)
    utext_clone(ut5, ut1, False, False)
    assert ut5 != ut1
    assert not utext_is_writable(ut5)
    assert utext_extract(ut5, 0, utext_native_length(ut5)) == "ABC"
    assert utext_close(ut5) is None

    assert utext_close(ut1) is None


def test_clone_unicode_string():
    s1 = UnicodeString("ABC")
    s2 = UnicodeString("abc")
    ut1 = utext_open_unicode_string(None, s1)
    assert utext_is_writable(ut1)

    # Deep clone
    ut2 = utext_clone(None, ut1, True, True)
    assert ut2 is not None
    assert ut2 != ut1
    assert not utext_is_writable(ut2)
    assert utext_extract(ut2, 0, utext_native_length(ut2)) == "ABC"
    assert utext_close(ut2) is None

    ut2 = utext_open_unicode_string(None, s2)
    ut3 = utext_clone(ut2, ut1, True, True)
    assert ut3 is not None
    assert ut3 == ut2
    assert ut3 != ut1
    assert not utext_is_writable(ut3)
    assert utext_extract(ut3, 0, utext_native_length(ut3)) == "ABC"
    assert utext_close(ut2) is None
    assert utext_close(ut3) == ut2

    ut2 = utext_open_unicode_string(None, s2)
    utext_clone(ut2, ut1, True, False)
    assert ut2 != ut1
    assert utext_is_writable(ut2)
    assert utext_extract(ut2, 0, utext_native_length(ut2)) == "ABC"
    assert utext_close(ut2) is None

    # Shallow clone
    ut4 = utext_clone(None, ut1, False, True)
    assert ut4 is not None
    assert ut4 != ut1
    assert not utext_is_writable(ut4)
    assert utext_extract(ut4, 0, utext_native_length(ut4)) == "ABC"
    assert utext_close(ut4) is None

    ut5 = utext_open_unicode_string(None, s2)
    ut6 = utext_clone(ut5, ut1, False, True)
    assert ut6 is not None
    assert ut6 == ut5
    assert ut6 != ut1
    assert not utext_is_writable(ut6)
    assert utext_extract(ut6, 0, utext_native_length(ut6)) == "ABC"
    assert utext_close(ut5) is None
    assert utext_close(ut6) == ut5

    ut5 = utext_open_unicode_string(None, s2)
    utext_clone(ut5, ut1, False, False)
    assert ut5 != ut1
    assert utext_is_writable(ut5)
    assert utext_extract(ut5, 0, utext_native_length(ut5)) == "ABC"
    assert utext_close(ut5) is None

    assert utext_close(ut1) is None


def test_copy():
    s1 = "ABC"
    with gc(utext_open_uchars(None, s1, -1), utext_close) as ut1:
        assert not utext_is_writable(ut1)

        with pytest.raises(ICUError) as exc_info:
            utext_copy(ut1, 0, 2, 2, False)
        assert exc_info.value.args[0] == UErrorCode.U_NO_WRITE_PERMISSION

    s2 = UnicodeString(s1)
    with gc(utext_open_unicode_string(None, s2), utext_close) as ut2:
        assert utext_is_writable(ut2)

        # Copy: |AB|C -> AB|AB|C
        utext_copy(ut2, 0, 2, 2, False)
        assert utext_extract(ut2, 0, utext_native_length(ut2)) == "ABABC"

        # Move: A|BA|BC -> ABC|BA|
        utext_copy(ut2, 1, 3, 5, True)
        assert utext_extract(ut2, 0, utext_native_length(ut2)) == "ABCBA"


def test_equals():
    s1 = "ABC"
    with gc(utext_open_uchars(None, s1, -1), utext_close) as ut1:
        with gc(utext_clone(None, ut1, False, True), utext_close) as ut2:
            utext_set_native_index(ut1, 0)
            utext_set_native_index(ut2, 0)
            assert utext_equals(ut1, ut2)

            assert utext_next32(ut1) != U_SENTINEL
            assert not utext_equals(ut1, ut2)

            assert utext_next32(ut2) != U_SENTINEL
            assert utext_equals(ut1, ut2)


def test_freeze():
    s = UnicodeString("ABC")
    with gc(utext_open_unicode_string(None, s), utext_close) as ut:
        assert utext_is_writable(ut)
        utext_freeze(ut)
        assert not utext_is_writable(ut)


def test_open_character_iterator():
    src1 = UnicodeString("\x41\U0001f338\x42")
    src2 = UnicodeString("\x61\U0001f338\x62")
    it1 = StringCharacterIterator(src1)
    it2 = StringCharacterIterator(src2)
    with gc(utext_open_character_iterator(None, it1), utext_close) as ut1:
        assert not utext_is_writable(ut1)

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # Reset
        ut2 = utext_open_character_iterator(ut1, it2)
        assert ut2 == ut1

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "a\U0001f338b"

        dest2 = utext_extract(ut2, 0, utext_native_length(ut2))
        assert dest2 == "a\U0001f338b"

        # Keep-alive test
        utext_open_character_iterator(ut1, it1)
        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"


def test_open_const_unicode_string():
    s1 = UnicodeString("\x41\U0001f338\x42")
    s2 = UnicodeString("\x61\U0001f338\x62")
    with gc(utext_open_const_unicode_string(None, s1), utext_close) as ut1:
        assert not utext_is_writable(ut1)

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # Reset
        ut2 = utext_open_const_unicode_string(ut1, s2)
        assert ut2 == ut1

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "a\U0001f338b"

        dest2 = utext_extract(ut2, 0, utext_native_length(ut2))
        assert dest2 == "a\U0001f338b"

        # Keep-alive test
        utext_open_const_unicode_string(ut1, s1)
        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"


def test_open_replaceable():
    s1 = UnicodeString("\x41\U0001f338\x42")
    s2 = UnicodeString("\x61\U0001f338\x62")
    with gc(utext_open_replaceable(None, s1), utext_close) as ut1:
        assert utext_is_writable(ut1)

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # Reset
        ut2 = utext_open_replaceable(ut1, s2)
        assert ut2 == ut1

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "a\U0001f338b"

        dest2 = utext_extract(ut2, 0, utext_native_length(ut2))
        assert dest2 == "a\U0001f338b"

        # Keep-alive test
        utext_open_replaceable(ut1, s1)
        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"


def test_open_uchars():
    s1 = "\x41\U0001f338\x42"
    s2 = "\x61\U0001f338\x62"
    with gc(utext_open_uchars(None, s1, -1), utext_close) as ut1:
        assert not utext_is_writable(ut1)

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # Reset
        ut2 = utext_open_uchars(ut1, s2, -1)
        assert ut2 == ut1

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "a\U0001f338b"

        dest2 = utext_extract(ut2, 0, utext_native_length(ut2))
        assert dest2 == "a\U0001f338b"

        # Keep-alive test
        utext_open_uchars(ut1, s1, -1)
        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # NULL string
        utext_open_uchars(ut1, None, -1)
        dest1 = utext_extract(ut1, 0, 4)
        assert len(dest1) == 0


def test_open_unicode_string():
    s1 = UnicodeString("\x41\U0001f338\x42")
    s2 = UnicodeString("\x61\U0001f338\x62")
    with gc(utext_open_unicode_string(None, s1), utext_close) as ut1:
        assert utext_is_writable(ut1)

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # Reset
        ut2 = utext_open_unicode_string(ut1, s2)
        assert ut2 == ut1

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "a\U0001f338b"

        dest2 = utext_extract(ut2, 0, utext_native_length(ut2))
        assert dest2 == "a\U0001f338b"

        # Keep-alive test
        utext_open_unicode_string(ut1, s1)
        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"


def test_open_utf8():
    s1 = "\x41\U0001f338\x42"
    s2 = "\x61\U0001f338\x62"
    with gc(utext_open_utf8(None, s1, -1), utext_close) as ut1:
        assert not utext_is_writable(ut1)

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # Reset
        ut2 = utext_open_utf8(ut1, s2, -1)
        assert ut2 == ut1

        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "a\U0001f338b"

        dest2 = utext_extract(ut2, 0, utext_native_length(ut2))
        assert dest2 == "a\U0001f338b"

        # Keep-alive test
        utext_open_utf8(ut1, s1, -1)
        dest1 = utext_extract(ut1, 0, utext_native_length(ut1))
        assert dest1 == "A\U0001f338B"

        # NULL string
        utext_open_utf8(ut1, None, -1)
        dest1 = utext_extract(ut1, 0, 4)
        assert len(dest1) == 0


def test_replace():
    s1 = "ABC"
    with gc(utext_open_uchars(None, s1, -1), utext_close) as ut1:
        assert not utext_is_writable(ut1)

        with pytest.raises(ICUError) as exc_info:
            utext_replace(ut1, 1, 2, "abc", -1)
        assert exc_info.value.args[0] == UErrorCode.U_NO_WRITE_PERMISSION

    s2 = UnicodeString(s1)
    with gc(utext_open_unicode_string(None, s2), utext_close) as ut2:
        assert utext_is_writable(ut2)

        assert utext_replace(ut2, 1, 2, "abc", -1) == 2
        assert utext_extract(ut2, 0, utext_native_length(ut2)) == "AabcC"


def test_utext_vector():
    t = UTextVector(3)
    assert len(t) == 3

    dest1 = utext_extract(t[0], 0, utext_native_length(t[0]))
    dest2 = utext_extract(t[1], 0, utext_native_length(t[1]))
    dest3 = utext_extract(t[2], 0, utext_native_length(t[2]))
    assert len(dest1) == 0
    assert len(dest2) == 0
    assert len(dest3) == 0

    assert utext_is_writable(t[0])
    assert utext_is_writable(t[1])
    assert utext_is_writable(t[2])

    utext_replace(t[0], 0, utext_native_length(t[0]), "foo", -1)
    utext_replace(t[1], 0, utext_native_length(t[1]), "bar", -1)
    utext_replace(t[2], 0, utext_native_length(t[2]), "baz", -1)

    assert utext_extract(t[0], 0, utext_native_length(t[0])) == "foo"
    assert utext_extract(t[1], 0, utext_native_length(t[1])) == "bar"
    assert utext_extract(t[2], 0, utext_native_length(t[2])) == "baz"

    # ut0, ut1, ut2 = t[0], t[1], t[2]
    # del t
    # assert ut0.magic == 0
    # assert ut1.magic == 0
    # assert ut2.magic == 0

    s = [UnicodeString("foo"), UnicodeString("bar"), UnicodeString("baz")]
    t = UTextVector(s)
    assert len(t) == 3

    assert utext_extract(t[0], 0, utext_native_length(t[0])) == s[0]
    assert utext_extract(t[1], 0, utext_native_length(t[1])) == s[1]
    assert utext_extract(t[2], 0, utext_native_length(t[2])) == s[2]

    assert utext_is_writable(t[0])
    assert utext_is_writable(t[1])
    assert utext_is_writable(t[2])

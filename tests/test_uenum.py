import pytest
from icupy import (
    Locale, U_ICU_VERSION_MAJOR_NUM,
    uenum_close, uenum_count, uenum_next, uenum_open_char_strings_enumeration,
    uenum_open_from_string_enumeration, uenum_open_uchar_strings_enumeration,
    uenum_reset, uenum_unext,
)

from . import gc


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_open_char_strings_enumeration():
    strings = ["abc", "\uff11\uff12", "\U0001f338\U0001f339"]
    with gc(uenum_open_char_strings_enumeration(strings, len(strings)),
            uenum_close) as en:
        assert uenum_count(en) == 3
        assert uenum_next(en) == strings[0]
        assert uenum_next(en) == strings[1]
        assert uenum_next(en) == strings[2]
        assert uenum_next(en) is None
        uenum_reset(en)
        assert uenum_next(en) == strings[0]

    with gc(uenum_open_char_strings_enumeration(strings),
            uenum_close) as en:
        assert uenum_count(en) == 3
        assert uenum_next(en) == strings[0]
        assert uenum_next(en) == strings[1]
        assert uenum_next(en) == strings[2]
        assert uenum_next(en) is None
        uenum_reset(en)
        assert uenum_next(en) == strings[0]


def test_open_from_string_enumeration():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    adopted = loc.create_keywords()
    with gc(uenum_open_from_string_enumeration(adopted), uenum_close) as en:
        assert uenum_count(en) == 2
        assert uenum_next(en) == "calendar"
        assert uenum_next(en) == "collation"
        assert uenum_next(en) is None
        uenum_reset(en)
        assert uenum_next(en) == "calendar"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_open_uchar_strings_enumeration():
    strings = ["abc", "\uff11\uff12", "\U0001f338\U0001f339"]
    with gc(uenum_open_uchar_strings_enumeration(strings, len(strings)),
            uenum_close) as en:
        assert uenum_count(en) == 3
        assert uenum_unext(en) == strings[0]
        assert uenum_unext(en) == strings[1]
        assert uenum_unext(en) == strings[2]
        assert uenum_unext(en) is None
        uenum_reset(en)
        assert uenum_unext(en) == strings[0]

    with gc(uenum_open_uchar_strings_enumeration(strings),
            uenum_close) as en:
        assert uenum_count(en) == 3
        assert uenum_unext(en) == strings[0]
        assert uenum_unext(en) == strings[1]
        assert uenum_unext(en) == strings[2]
        assert uenum_unext(en) is None
        uenum_reset(en)
        assert uenum_unext(en) == strings[0]

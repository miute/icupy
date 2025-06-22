import pytest

from icupy import icu
from icupy.utils import gc


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_open_char_strings_enumeration() -> None:
    strings = ["abc", "\uff11\uff12", "\U0001f338\U0001f339"]
    with gc(
        icu.uenum_open_char_strings_enumeration(strings, len(strings)),
        icu.uenum_close,
    ) as en:
        assert icu.uenum_count(en) == 3
        assert icu.uenum_next(en) == strings[0]
        assert icu.uenum_next(en) == strings[1]
        assert icu.uenum_next(en) == strings[2]
        assert icu.uenum_next(en) is None
        icu.uenum_reset(en)
        assert icu.uenum_next(en) == strings[0]

    with gc(icu.uenum_open_char_strings_enumeration(strings), icu.uenum_close) as en:
        assert icu.uenum_count(en) == 3
        assert icu.uenum_next(en) == strings[0]
        assert icu.uenum_next(en) == strings[1]
        assert icu.uenum_next(en) == strings[2]
        assert icu.uenum_next(en) is None
        icu.uenum_reset(en)
        assert icu.uenum_next(en) == strings[0]


def test_open_from_string_enumeration() -> None:
    loc = icu.Locale("de@calendar=buddhist;collation=phonebook")
    adopted = loc.create_keywords()
    with gc(icu.uenum_open_from_string_enumeration(adopted), icu.uenum_close) as en:
        assert icu.uenum_count(en) == 2
        assert icu.uenum_next(en) == "calendar"
        assert icu.uenum_next(en) == "collation"
        assert icu.uenum_next(en) is None
        icu.uenum_reset(en)
        assert icu.uenum_next(en) == "calendar"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 50, reason="ICU4C<50")
def test_open_uchar_strings_enumeration() -> None:
    strings = ["abc", "\uff11\uff12", "\U0001f338\U0001f339"]
    with gc(
        icu.uenum_open_uchar_strings_enumeration(strings, len(strings)),
        icu.uenum_close,
    ) as en:
        assert icu.uenum_count(en) == 3
        assert icu.uenum_unext(en) == strings[0]
        assert icu.uenum_unext(en) == strings[1]
        assert icu.uenum_unext(en) == strings[2]
        assert icu.uenum_unext(en) is None
        icu.uenum_reset(en)
        assert icu.uenum_unext(en) == strings[0]

    with gc(icu.uenum_open_uchar_strings_enumeration(strings), icu.uenum_close) as en:
        assert icu.uenum_count(en) == 3
        assert icu.uenum_unext(en) == strings[0]
        assert icu.uenum_unext(en) == strings[1]
        assert icu.uenum_unext(en) == strings[2]
        assert icu.uenum_unext(en) is None
        icu.uenum_reset(en)
        assert icu.uenum_unext(en) == strings[0]

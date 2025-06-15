import copy
from collections.abc import Iterable

from icupy import icu


def test_clone() -> None:
    # StringEnumeration *icu::Locale::createKeywords(UErrorCode &status)
    loc = icu.Locale("de@calendar=buddhist;collation=phonebook")
    it1 = loc.create_keywords()
    assert isinstance(it1, icu.StringEnumeration)

    # int32_t icu::StringEnumeration::count(UErrorCode &status)
    assert it1.count() == len(it1) == 2

    # const char *icu::StringEnumeration::next(
    #       int32_t *resultLength,
    #       UErrorCode &status
    # )
    assert it1.next() is not None

    # StringEnumeration *icu::StringEnumeration::clone()
    it2 = it1.clone()
    assert isinstance(it2, icu.StringEnumeration)
    assert len(it2) == 2
    assert it2.next() is not None
    assert it2.next() is None

    t2 = [x for x in iter(it2)]
    assert len(t2) == 2
    assert "calendar" in t2
    assert "collation" in t2

    it3 = copy.copy(it1)
    t3 = [x for x in iter(it3)]
    assert len(t3) == 2
    assert "calendar" in t3
    assert "collation" in t3

    it4 = copy.deepcopy(it1)
    t4 = [x for x in iter(it4)]
    assert len(t4) == 2
    assert "calendar" in t4
    assert "collation" in t4


def test_next() -> None:
    loc = icu.Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, icu.StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

    # const char *icu::StringEnumeration::next(
    #       int32_t *resultLength,
    #       UErrorCode &status
    # )
    result = it.next()
    assert isinstance(result, str)
    assert result in ["calendar", "collation"]

    result = it.next()
    assert isinstance(result, str)
    assert result in ["calendar", "collation"]

    result = it.next()
    assert result is None

    assert "calendar" in it
    assert "collation" in it

    t = [x for x in iter(it)]
    assert len(t) == 2
    assert all(isinstance(x, str) for x in t)
    assert "calendar" in t
    assert "collation" in t


def test_operator() -> None:
    loc1 = icu.Locale("de@calendar=buddhist;collation=phonebook")
    it1 = loc1.create_keywords()
    assert isinstance(it1, icu.StringEnumeration)

    loc2 = loc1.clone()
    it2 = loc2.create_keywords()
    assert isinstance(it2, icu.StringEnumeration)

    # UBool icu::StringEnumeration::operator!=(const StringEnumeration &that)
    assert not (it1 != it2)

    # UBool icu::StringEnumeration::operator==(const StringEnumeration &that)
    assert it1 == it2


def test_reset() -> None:
    loc = icu.Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, icu.StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

    _ = it.next()
    _ = it.next()
    assert it.next() is None

    # void icu::StringEnumeration::reset(UErrorCode &status)
    it.reset()
    assert it.next() is not None


def test_snext() -> None:
    loc = icu.Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, icu.StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

    # const UnicodeString *icu::StringEnumeration::snext(UErrorCode &status)
    result = it.snext()
    assert isinstance(result, icu.UnicodeString)
    assert str(result) in ["calendar", "collation"]

    result = it.snext()
    assert isinstance(result, icu.UnicodeString)
    assert str(result) in ["calendar", "collation"]

    result = it.snext()
    assert result is None

    assert icu.UnicodeString("calendar", -1) in it
    assert icu.UnicodeString("collation", -1) in it

    it.reset()
    y: Iterable[icu.UnicodeString] = iter(it.snext, None)
    t: list[icu.UnicodeString] = [x.clone() for x in y]
    assert len(t) == 2
    assert all(isinstance(x, icu.UnicodeString) for x in t)
    t2 = [str(x) for x in t]
    assert "calendar" in t2
    assert "collation" in t2


def test_unext() -> None:
    loc = icu.Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, icu.StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

    # const char16_t *icu::StringEnumeration::unext(
    #       int32_t *resultLength,
    #       UErrorCode &status
    # )
    result = it.unext()
    assert isinstance(result, str)
    assert result in ["calendar", "collation"]

    result = it.unext()
    assert isinstance(result, str)
    assert result in ["calendar", "collation"]

    result = it.unext()
    assert result is None

    assert "calendar" in it
    assert "collation" in it

    it.reset()
    y: Iterable[icu.UnicodeString] = iter(it.unext, None)
    t = list(y)
    assert len(t) == 2
    assert all(isinstance(x, str) for x in t)
    assert "calendar" in t
    assert "collation" in t

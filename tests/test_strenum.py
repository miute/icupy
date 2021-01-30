import copy

from icupy import Locale, UnicodeString, StringEnumeration


def test_clone():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    it1 = loc.create_keywords()
    assert isinstance(it1, StringEnumeration)
    assert len(it1) == 2
    assert it1.next() is not None

    it2 = it1.clone()
    assert isinstance(it2, StringEnumeration)
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


def test_next():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

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


def test_operator():
    loc1 = Locale("de@calendar=buddhist;collation=phonebook")
    it1 = loc1.create_keywords()
    assert isinstance(it1, StringEnumeration)

    loc2 = loc1.clone()
    it2 = loc2.create_keywords()
    assert isinstance(it2, StringEnumeration)

    assert not (it1 != it2)
    assert it1 == it2


def test_reset():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

    _ = it.next()
    _ = it.next()
    result = it.next()
    assert result is None

    it.reset()
    result = it.next()
    assert result is not None


def test_snext():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

    result = it.snext()
    assert isinstance(result, UnicodeString)
    assert str(result) in ["calendar", "collation"]

    result = it.snext()
    assert isinstance(result, UnicodeString)
    assert str(result) in ["calendar", "collation"]

    result = it.snext()
    assert result is None

    assert UnicodeString("calendar", -1) in it
    assert UnicodeString("collation", -1) in it

    it.reset()
    t = [x.clone() for x in iter(it.snext, None)]
    assert len(t) == 2
    assert all(isinstance(x, UnicodeString) for x in t)
    t2 = [str(x) for x in t]
    assert "calendar" in t2
    assert "collation" in t2


def test_unext():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    it = loc.create_keywords()
    assert isinstance(it, StringEnumeration)
    assert it.count() == 2
    assert len(it) == 2

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
    t = [x for x in iter(it.unext, None)]
    assert len(t) == 2
    assert all(isinstance(x, str) for x in t)
    assert "calendar" in t
    assert "collation" in t

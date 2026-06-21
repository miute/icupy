import pytest

from icupy import icu


def test_append() -> None:
    # UnicodeStringList.append(value: UnicodeString | str) -> None
    usl = icu.UnicodeStringList()
    usl.append(icu.UnicodeString("a"))
    usl.append("b")
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b"]


def test_clear() -> None:
    # UnicodeStringList.clear() -> None
    usl = icu.UnicodeStringList(["a", "b", "c"])
    assert len(usl) == 3
    usl.clear()
    assert len(usl) == 0
    assert list(usl) == []


def test_count() -> None:
    # UnicodeStringList.count(value: UnicodeString | str) -> int
    usl = icu.UnicodeStringList(["a", "b", "c", "a"])
    assert usl.count("a") == 2
    assert usl.count("b") == 1
    assert usl.count("c") == 1
    assert usl.count("d") == 0
    assert usl.count(icu.UnicodeString("a")) == 2
    assert usl.count(icu.UnicodeString("b")) == 1
    assert usl.count(icu.UnicodeString("c")) == 1
    assert usl.count(icu.UnicodeString("d")) == 0


def test_extend() -> None:
    # UnicodeStringList.extend(other: Sequence[UnicodeString]) -> None
    # UnicodeStringList.extend(other: Sequence[str]) -> None
    usl = icu.UnicodeStringList()
    usl.extend([icu.UnicodeString("a"), icu.UnicodeString("b")])
    usl.extend(["c", "d"])
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c", "d"]

    # UnicodeStringList.extend(other: UnicodeStringList) -> None
    other = icu.UnicodeStringList(["e", "f"])
    usl.extend(other)
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c", "d", "e", "f"]


def test_index() -> None:
    # UnicodeStringList.index(value: UnicodeString | str, start: int = 0, stop: int = -1) -> int
    usl = icu.UnicodeStringList(["a", "b", "c"])
    assert usl.index("a") == 0
    assert usl.index("b") == 1
    assert usl.index("c") == 2
    assert usl.index(icu.UnicodeString("a")) == 0
    assert usl.index(icu.UnicodeString("b")) == 1
    assert usl.index(icu.UnicodeString("c")) == 2

    assert usl.index("b", 1) == 1
    assert usl.index("b", 1, 1) == 1
    assert usl.index("b", 1, 2) == 1

    with pytest.raises(ValueError):
        usl.index("d")

    with pytest.raises(ValueError):
        usl.index("b", 2)


def test_init() -> None:
    # UnicodeStringList()
    usl = icu.UnicodeStringList()
    assert len(usl) == 0
    assert list(usl) == []

    # UnicodeStringList(size: SupportsInt | SupportsIndex)
    usl = icu.UnicodeStringList(3)
    assert len(usl) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["", "", ""]

    # UnicodeStringList(iterable: Sequence[str])
    usl = icu.UnicodeStringList(["a", "b", "c"])
    assert len(usl) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c"]

    # UnicodeStringList(iterable: Sequence[UnicodeString])
    usl = icu.UnicodeStringList(
        [
            icu.UnicodeString("a"),
            icu.UnicodeString("b"),
            icu.UnicodeString("c"),
        ]
    )
    assert len(usl) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c"]

    # UnicodeStringList(other: UnicodeStringList)
    usl2 = icu.UnicodeStringList(usl)
    assert len(usl2) == 3
    assert all(isinstance(x, icu.UnicodeString) for x in usl2)
    assert list(usl2) == ["a", "b", "c"]


def test_insert() -> None:
    # UnicodeStringList.insert(index: SupportsInt | SupportsIndex, value: UnicodeString | str) -> None
    usl = icu.UnicodeStringList(["a", "c"])
    usl.insert(1, "b")
    assert list(usl) == ["a", "b", "c"]

    usl.insert(0, "z")
    assert list(usl) == ["z", "a", "b", "c"]

    usl.insert(4, "d")
    assert list(usl) == ["z", "a", "b", "c", "d"]

    usl.insert(10, "e")
    assert list(usl) == ["z", "a", "b", "c", "d", "e"]

    usl.insert(-1, "y")
    assert list(usl) == ["z", "a", "b", "c", "d", "y", "e"]

    usl.insert(-2, "x")
    assert list(usl) == ["z", "a", "b", "c", "d", "x", "y", "e"]

    usl.insert(-10, "w")
    assert list(usl) == ["w", "z", "a", "b", "c", "d", "x", "y", "e"]


def test_operator_add() -> None:
    # UnicodeStringList.__add__(other: UnicodeStringList) -> UnicodeStringList
    usl = icu.UnicodeStringList(["a", "b", "c"])
    other = icu.UnicodeStringList(["d", "e", "f"])
    result = usl + other
    assert isinstance(result, icu.UnicodeStringList)
    assert len(result) == 6
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(result) == ["a", "b", "c", "d", "e", "f"]

    # UnicodeStringList.__add__(other: Sequence[UnicodeString]) -> UnicodeStringList
    usl = icu.UnicodeStringList(["a", "b", "c"])
    other = [
        icu.UnicodeString("d"),
        icu.UnicodeString("e"),
        icu.UnicodeString("f"),
    ]
    result = usl + other
    assert isinstance(result, icu.UnicodeStringList)
    assert len(result) == 6
    assert all(isinstance(x, icu.UnicodeString) for x in result)
    assert list(result) == ["a", "b", "c", "d", "e", "f"]

    # UnicodeStringList.__add__(other: Sequence[str]) -> UnicodeStringList
    usl = icu.UnicodeStringList(["a", "b", "c"])
    other = ["d", "e", "f"]
    result = usl + other
    assert isinstance(result, icu.UnicodeStringList)
    assert len(result) == 6
    assert all(isinstance(x, icu.UnicodeString) for x in result)
    assert list(result) == ["a", "b", "c", "d", "e", "f"]


def test_operator_contains() -> None:
    # UnicodeStringList.__contains__(value: UnicodeString | str) -> bool
    usl = icu.UnicodeStringList(["a", "b", "c"])
    assert "a" in usl
    assert "b" in usl
    assert "c" in usl
    assert "d" not in usl

    assert icu.UnicodeString("a") in usl
    assert icu.UnicodeString("d") not in usl


def test_operator_delitem() -> None:
    # UnicodeStringList.__delitem__(index: SupportsInt | SupportsIndex) -> None
    usl = icu.UnicodeStringList(["a", "b", "c"])
    del usl[0]
    assert list(usl) == ["b", "c"]

    usl = icu.UnicodeStringList(["a", "b", "c"])
    del usl[-1]
    assert list(usl) == ["a", "b"]

    with pytest.raises(IndexError):
        del usl[3]

    with pytest.raises(IndexError):
        del usl[-4]

    # UnicodeStringList.__delitem__(slice: slice) -> None
    usl = icu.UnicodeStringList(list("01234"))
    del usl[1:4]
    assert list(usl) == ["0", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    del usl[-3:-1]
    assert list(usl) == ["0", "1", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    del usl[0:5:2]
    assert list(usl) == ["1", "3"]

    usl = icu.UnicodeStringList(list("01234"))
    del usl[4:0:-2]
    assert list(usl) == ["0", "1", "3"]

    usl = icu.UnicodeStringList(list("01234"))
    del usl[2:2]
    assert list(usl) == ["0", "1", "2", "3", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    del usl[:]
    assert len(usl) == 0

    usl = icu.UnicodeStringList(list("01234"))
    with pytest.raises(ValueError):
        del usl[::0]


def test_operator_getitem() -> None:
    # UnicodeStringList.__getitem__(index: SupportsInt | SupportsIndex) -> UnicodeString
    usl = icu.UnicodeStringList(["a", "b", "c"])
    item = usl[0]
    assert isinstance(item, icu.UnicodeString)
    assert item == "a"
    item.append("d")
    assert item == "ad"

    assert usl[0] == "ad"
    assert usl[1] == "b"
    assert usl[2] == "c"

    # UnicodeStringList.__getitem__(slice: slice) -> UnicodeStringList
    usl = icu.UnicodeStringList(list("01234"))
    sub = usl[1:4]
    assert isinstance(sub, icu.UnicodeStringList)
    assert len(sub) == 3
    assert list(sub) == ["1", "2", "3"]

    sub = usl[-3:-1]
    assert isinstance(sub, icu.UnicodeStringList)
    assert len(sub) == 2
    assert list(sub) == ["2", "3"]

    sub = usl[:5:2]
    assert isinstance(sub, icu.UnicodeStringList)
    assert len(sub) == 3
    assert list(sub) == ["0", "2", "4"]

    sub = usl[4::-2]
    assert isinstance(sub, icu.UnicodeStringList)
    assert len(sub) == 3
    assert list(sub) == ["4", "2", "0"]

    sub = usl[2:2]
    assert isinstance(sub, icu.UnicodeStringList)
    assert len(sub) == 0

    sub = usl[:]
    assert isinstance(sub, icu.UnicodeStringList)
    assert len(sub) == 5
    assert list(sub) == ["0", "1", "2", "3", "4"]

    with pytest.raises(ValueError):
        _ = usl[::0]


def test_operator_iadd() -> None:
    # UnicodeStringList.__iadd__(other: UnicodeStringList) -> UnicodeStringList
    usl = icu.UnicodeStringList(["a", "b", "c"])
    usl += icu.UnicodeStringList(["d", "e", "f"])
    assert isinstance(usl, icu.UnicodeStringList)
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c", "d", "e", "f"]

    # UnicodeStringList.__iadd__(other: Sequence[UnicodeString]) -> UnicodeStringList
    usl = icu.UnicodeStringList(["a", "b", "c"])
    usl += [
        icu.UnicodeString("d"),
        icu.UnicodeString("e"),
        icu.UnicodeString("f"),
    ]
    assert isinstance(usl, icu.UnicodeStringList)
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c", "d", "e", "f"]

    # UnicodeStringList.__iadd__(other: Sequence[str]) -> UnicodeStringList
    usl = icu.UnicodeStringList(["a", "b", "c"])
    usl += ["d", "e", "f"]
    assert isinstance(usl, icu.UnicodeStringList)
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["a", "b", "c", "d", "e", "f"]


def test_operator_setitem() -> None:
    # UnicodeStringList.__setitem__(index: SupportsInt | SupportsIndex, value: UnicodeString | str)
    usl = icu.UnicodeStringList(3)
    usl[0] = "a"
    usl[1] = icu.UnicodeString("b")
    usl[2] = "c"
    assert usl[0] == "a"
    assert usl[1] == "b"
    assert usl[2] == "c"

    # UnicodeStringList.__setitem__(slice: slice, values: UnicodeStringList)
    usl = icu.UnicodeStringList(list("01234"))
    usl[1:4] = icu.UnicodeStringList(["10", "20", "30"])
    assert list(usl) == ["0", "10", "20", "30", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    usl[1:4] = icu.UnicodeStringList(["99"])
    assert list(usl) == ["0", "99", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    usl[-3:-1] = icu.UnicodeStringList(["20", "30"])
    assert list(usl) == ["0", "1", "20", "30", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    usl[0:5:2] = icu.UnicodeStringList(["10", "20", "30"])
    assert list(usl) == ["10", "1", "20", "3", "30"]

    usl = icu.UnicodeStringList(list("01234"))
    usl[4:0:-2] = icu.UnicodeStringList(["40", "20"])
    assert list(usl) == ["0", "1", "20", "3", "40"]

    usl = icu.UnicodeStringList(list("01234"))
    usl[2:2] = icu.UnicodeStringList(["a", "b"])
    assert list(usl) == ["0", "1", "a", "b", "2", "3", "4"]

    usl = icu.UnicodeStringList(list("01234"))
    usl[:] = icu.UnicodeStringList(["a", "b"])
    assert list(usl) == ["a", "b"]

    usl = icu.UnicodeStringList(list("01234"))
    with pytest.raises(ValueError):
        usl[0:5:2] = icu.UnicodeStringList(["10", "20"])

    usl = icu.UnicodeStringList(list("01234"))
    with pytest.raises(ValueError):
        usl[::0] = icu.UnicodeStringList(["1", "2", "3"])

    # Test assigning empty sequence to a slice
    usl = icu.UnicodeStringList(list("01234"))
    usl[1:4] = icu.UnicodeStringList([])
    assert list(usl) == ["0", "4"]

    # Test assigning different sized sequence to a slice (step = 1)
    usl = icu.UnicodeStringList(list("01234"))
    usl[1:4] = icu.UnicodeStringList(["a", "b"])
    assert list(usl) == ["0", "a", "b", "4"]

    # Test assigning empty sequence to starting slice
    usl = icu.UnicodeStringList(list("01234"))
    usl[0:2] = icu.UnicodeStringList([])
    assert list(usl) == ["2", "3", "4"]


def test_pop() -> None:
    usl = icu.UnicodeStringList(["a", "b", "c"])
    value = usl.pop()
    assert isinstance(value, icu.UnicodeString)
    assert value == "c"
    assert list(usl) == ["a", "b"]

    usl = icu.UnicodeStringList(["a", "b", "c"])
    value = usl.pop(0)
    assert isinstance(value, icu.UnicodeString)
    assert value == "a"
    assert list(usl) == ["b", "c"]

    usl = icu.UnicodeStringList(["a", "b", "c"])
    value = usl.pop(-1)
    assert isinstance(value, icu.UnicodeString)
    assert value == "c"
    assert list(usl) == ["a", "b"]

    usl = icu.UnicodeStringList(["a", "b", "c"])
    with pytest.raises(IndexError):
        usl.pop(3)

    usl = icu.UnicodeStringList(["a", "b", "c"])
    with pytest.raises(IndexError):
        usl.pop(-4)


def test_remove() -> None:
    # UnicodeStringList.remove(value: UnicodeString | str) -> None
    usl = icu.UnicodeStringList(["a", "b", "c", "b"])
    usl.remove("b")
    assert list(usl) == ["a", "c", "b"]

    usl.remove(icu.UnicodeString("b"))
    assert list(usl) == ["a", "c"]

    with pytest.raises(ValueError):
        usl.remove("b")


def test_reverse() -> None:
    # UnicodeStringList.reverse() -> None
    usl = icu.UnicodeStringList(["a", "b", "c"])
    usl.reverse()
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == ["c", "b", "a"]

    usl = icu.UnicodeStringList()
    usl.reverse()
    assert all(isinstance(x, icu.UnicodeString) for x in usl)
    assert list(usl) == []


def test_sort() -> None:
    # UnicodeStringList.sort(*, coll: Collator | None = None, reverse: bool = False) -> None
    usl = icu.UnicodeStringList()
    usl.sort()
    assert list(usl) == []

    t = ["a", "ä", "A", "á", "Z", "Å", "å"]

    usl = icu.UnicodeStringList(t)
    usl.sort()
    assert list(usl) == ["A", "Z", "a", "Å", "á", "ä", "å"]

    usl = icu.UnicodeStringList(t)
    usl.sort(reverse=True)
    assert list(usl) == ["å", "ä", "á", "Å", "a", "Z", "A"]

    coll = icu.Collator.create_instance(icu.Locale("en_US"))

    coll.set_attribute(icu.UCOL_STRENGTH, icu.UCOL_TERTIARY)  # default
    usl = icu.UnicodeStringList(t)
    usl.sort(coll=coll)
    assert list(usl) == ["a", "A", "á", "å", "Å", "ä", "Z"]

    usl = icu.UnicodeStringList(t)
    usl.sort(coll=coll, reverse=True)
    assert list(usl) == ["Z", "ä", "Å", "å", "á", "A", "a"]

    coll.set_attribute(icu.UCOL_STRENGTH, icu.UCOL_SECONDARY)  # accent sensitive
    usl = icu.UnicodeStringList(t)
    usl.sort(coll=coll)
    assert list(usl) == ["a", "A", "á", "Å", "å", "ä", "Z"]

    coll.set_attribute(icu.UCOL_STRENGTH, icu.UCOL_PRIMARY)  # base letters only
    usl = icu.UnicodeStringList(t)
    usl.sort(coll=coll)
    assert list(usl) == ["a", "ä", "A", "á", "Å", "å", "Z"]

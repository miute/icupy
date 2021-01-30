from icupy import StringCharacterIterator, UnicodeString


def test_iter():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    assert list(it) == ["a", "\U0001f338", "b"]

    t = [it.first()] + [
        c for c in iter(it.next, StringCharacterIterator.DONE)]
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]

    t = [it.first32()] + [
        c for c in iter(it.next32, StringCharacterIterator.DONE)]
    assert t == [0x61, 0x1f338, 0x62]

    assert reversed(it) == ["b", "\U0001f338", "a"]

    src = UnicodeString("\\uD83C\\uDF38").unescape()
    it = StringCharacterIterator(src)
    assert list(it) == ["\U0001f338"]
    assert reversed(it) == ["\U0001f338"]

    src = UnicodeString()
    it = StringCharacterIterator(src)
    assert list(it) == []
    assert reversed(it) == []


def test_move():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    pos = it.move(1, StringCharacterIterator.START)
    assert pos == 1
    pos = it.move(1, StringCharacterIterator.CURRENT)
    assert pos == 2
    pos = it.move(1, StringCharacterIterator.END)
    assert pos == 4


def test_move32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    pos = it.move32(1, StringCharacterIterator.START)
    assert pos == 1
    pos = it.move32(1, StringCharacterIterator.CURRENT)
    assert pos == 3
    pos = it.move32(1, StringCharacterIterator.END)
    assert pos == 4


def test_next():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()
    it.set_to_start()
    while it.has_next():
        current = it.current()
        c = it.next_post_inc()
        assert c == current
        t.append(c)
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]

    t.clear()
    c = it.first_post_inc()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.next_post_inc()
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]

    t.clear()
    c = it.first()
    while c != StringCharacterIterator.DONE:
        current = it.current()
        assert c == current
        t.append(c)
        c = it.next()
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]


def test_next32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()
    it.set_to_start()
    while it.has_next():
        current = it.current32()
        c = it.next32_post_inc()
        assert c == current
        t.append(c)
    assert t == [0x61, 0x1f338, 0x62]

    t.clear()
    c = it.first32_post_inc()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.next32_post_inc()
    assert t == [0x61, 0x1f338, 0x62]

    t.clear()
    c = it.first32()
    while c != StringCharacterIterator.DONE:
        current = it.current32()
        assert c == current
        t.append(c)
        c = it.next32()
    assert t == [0x61, 0x1f338, 0x62]


def test_previous():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()
    it.set_to_end()
    while it.has_previous():
        c = it.previous()
        current = it.current()
        assert c == current
        t.append(c)
    assert t == [0x62, 0xdf38, 0xd83c, 0x61]

    t.clear()
    c = it.last()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.previous()
    assert t == [0x62, 0xdf38, 0xd83c, 0x61]


def test_previous32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()
    it.set_to_end()
    while it.has_previous():
        c = it.previous32()
        current = it.current32()
        assert c == current
        t.append(c)
    assert t == [0x62, 0x1f338, 0x61]

    t.clear()
    c = it.last32()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.previous32()
    assert t == [0x62, 0x1f338, 0x61]


def test_set_index():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    c = it.set_index(0)
    assert c == 0x61
    c = it.set_index(1)
    assert c == 0xd83c
    c = it.set_index(2)
    assert c == 0xdf38
    c = it.set_index(3)
    assert c == 0x62


def test_set_index32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    c = it.set_index32(0)
    assert c == 0x61
    c = it.set_index32(1)
    assert c == 0x1f338
    c = it.set_index32(2)
    assert c == 0x1f338
    c = it.set_index32(3)
    assert c == 0x62


def test_set_text():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    it.set_text(UnicodeString("foo bar baz"))
    dest = UnicodeString()
    it.get_text(dest)
    assert dest == "foo bar baz"


def test_string_character_iterator():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    expected = "a\U0001f338b"
    dest = UnicodeString()

    # [1]
    # StringCharacterIterator(const UnicodeString &textStr)
    it1 = StringCharacterIterator(src)
    assert it1.get_length() == 4
    assert len(it1) == 4
    dest.remove()
    it1.get_text(dest)
    assert dest == expected

    # [2]
    # StringCharacterIterator(const UnicodeString &textStr,
    #                         int32_t textPos
    # )
    it2 = StringCharacterIterator(src, 2)
    assert it2.get_length() == 4
    assert len(it2) == 4
    dest.remove()
    it2.get_text(dest)
    assert dest == expected
    assert it2.current() == 0xdf38

    # [3]
    # StringCharacterIterator(const UnicodeString &textStr,
    #                         int32_t textBegin,
    #                         int32_t textEnd,
    #                         int32_t textPos
    # )
    it3 = StringCharacterIterator(src, 1, 3, 0)
    assert it3.get_length() == 4
    assert len(it3) == 4
    dest.remove()
    it3.get_text(dest)
    assert dest == expected
    assert it3.current() == 0xd83c
    c = it3.next()
    assert c == 0xdf38
    c = it3.next()
    assert c == StringCharacterIterator.DONE
    c = it3.first()
    assert c == 0xd83c

    # [4]
    # StringCharacterIterator(const StringCharacterIterator &that)
    it1.set_index(2)
    it4 = StringCharacterIterator(it1)
    assert it4.get_length() == 4
    assert len(it4) == 4
    dest.remove()
    it4.get_text(dest)
    assert dest == expected
    assert it4.get_index() == 2
    assert it4.current() == 0xdf38

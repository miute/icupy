from icupy.icu import (
    CharacterIterator, ForwardCharacterIterator, StringCharacterIterator,
    UCharCharacterIterator, UnicodeString,
)


def test_iter():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    # int32_t icu::CharacterIterator::getLength(void)
    assert it.get_length() == len(it) == 4

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

    # int32_t icu::CharacterIterator::move(
    #       int32_t delta,
    #       EOrigin origin
    # )
    assert it.move(1, StringCharacterIterator.START) == 1
    assert it.move(1, StringCharacterIterator.CURRENT) == 2
    assert it.move(1, StringCharacterIterator.END) == 4


def test_move32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    # int32_t icu::CharacterIterator::move32(
    #       int32_t delta,
    #       EOrigin origin
    # )
    assert it.move32(1, StringCharacterIterator.START) == 1
    assert it.move32(1, StringCharacterIterator.CURRENT) == 3
    assert it.move32(1, StringCharacterIterator.END) == 4


def test_next():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()

    # int32_t icu::CharacterIterator::setToStart()
    assert it.set_to_start() == 0

    # UBool icu::ForwardCharacterIterator::hasNext()
    while it.has_next():
        # char16_t icu::CharacterIterator::current(void)
        current = it.current()

        # char16_t icu::ForwardCharacterIterator::nextPostInc(void)
        c = it.next_post_inc()
        assert c == current
        t.append(c)
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]

    # char16_t icu::CharacterIterator::firstPostInc(void)
    c = it.first_post_inc()
    t.clear()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.next_post_inc()
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]

    # char16_t icu::CharacterIterator::first(void)
    c = it.first()
    t.clear()
    while c != StringCharacterIterator.DONE:
        current = it.current()
        assert c == current
        t.append(c)

        # char16_t icu::CharacterIterator::next(void)
        c = it.next()
    assert t == [0x61, 0xd83c, 0xdf38, 0x62]


def test_next32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()
    it.set_to_start()
    while it.has_next():
        # UChar32 icu::CharacterIterator::current32(void)
        current = it.current32()

        # UChar32 icu::ForwardCharacterIterator::next32PostInc(void)
        c = it.next32_post_inc()
        assert c == current
        t.append(c)
    assert t == [0x61, 0x1f338, 0x62]

    # UChar32 icu::CharacterIterator::first32PostInc(void)
    c = it.first32_post_inc()
    t.clear()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.next32_post_inc()
    assert t == [0x61, 0x1f338, 0x62]

    # UChar32 icu::CharacterIterator::first32(void)
    c = it.first32()
    t.clear()
    while c != StringCharacterIterator.DONE:
        current = it.current32()
        assert c == current
        t.append(c)

        # UChar32 icu::CharacterIterator::next32(void)
        c = it.next32()
    assert t == [0x61, 0x1f338, 0x62]


def test_previous():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    t = list()

    # int32_t icu::CharacterIterator::setToEnd()
    assert it.set_to_end() == 4

    # UBool icu::CharacterIterator::hasPrevious()
    while it.has_previous():
        # char16_t icu::CharacterIterator::previous(void)
        c = it.previous()
        current = it.current()
        assert c == current
        t.append(c)
    assert t == [0x62, 0xdf38, 0xd83c, 0x61]

    # char16_t icu::CharacterIterator::last(void)
    c = it.last()
    t.clear()
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
        # UChar32 icu::CharacterIterator::previous32(void)
        c = it.previous32()
        current = it.current32()
        assert c == current
        t.append(c)
    assert t == [0x62, 0x1f338, 0x61]

    # UChar32 icu::CharacterIterator::last32(void)
    c = it.last32()
    t.clear()
    while c != StringCharacterIterator.DONE:
        t.append(c)
        c = it.previous32()
    assert t == [0x62, 0x1f338, 0x61]


def test_set_index():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    # int32_t icu::CharacterIterator::getIndex(void)
    assert it.get_index() == 0

    # char16_t icu::CharacterIterator::setIndex(int32_t position)
    assert it.set_index(0) == 0x61
    assert it.get_index() == 0

    assert it.set_index(1) == 0xd83c
    assert it.get_index() == 1

    assert it.set_index(2) == 0xdf38
    assert it.get_index() == 2

    assert it.set_index(3) == 0x62
    assert it.get_index() == 3

    # int32_t icu::CharacterIterator::endIndex(void)
    assert it.end_index() == 4

    # int32_t icu::CharacterIterator::startIndex(void)
    assert it.start_index() == 0


def test_set_index32():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    # UChar32 icu::CharacterIterator::setIndex32(int32_t position)
    assert it.set_index32(0) == 0x61
    assert it.get_index() == 0

    assert it.set_index32(1) == 0x1f338
    assert it.get_index() == 1

    assert it.set_index32(2) == 0x1f338
    assert it.get_index() == 1

    assert it.set_index32(3) == 0x62
    assert it.get_index() == 3


def test_set_text():
    src = UnicodeString("a\\uD83C\\uDF38b").unescape()
    it = StringCharacterIterator(src)

    # [2]
    # void icu::StringCharacterIterator::setText(const UnicodeString &newText)
    it.set_text(UnicodeString("foo bar baz"))

    # void icu::CharacterIterator::getText(UnicodeString &result)
    result = UnicodeString()
    it.get_text(result)
    assert result == "foo bar baz"

    new_text = "a\U0001f338b"
    it.set_text(new_text)
    it.get_text(result)
    assert result == new_text


def test_string_character_iterator():
    assert issubclass(CharacterIterator, ForwardCharacterIterator)
    assert issubclass(UCharCharacterIterator, CharacterIterator)
    assert issubclass(StringCharacterIterator, UCharCharacterIterator)

    text_str = UnicodeString("a\\uD83C\\uDF38b").unescape()
    expected = "a\U0001f338b"
    dest = UnicodeString()

    # [1]
    # icu::StringCharacterIterator::StringCharacterIterator(
    #       const UnicodeString &textStr
    # )
    it1 = StringCharacterIterator(text_str)
    assert it1.get_length() == 4
    assert len(it1) == 4
    dest.remove()
    it1.get_text(dest)
    assert dest == expected

    it1a = StringCharacterIterator("a\U0001f338b")
    assert it1a.get_length() == 4
    assert len(it1a) == 4
    dest.remove()
    it1a.get_text(dest)
    assert dest == expected

    # [2]
    # icu::StringCharacterIterator::StringCharacterIterator(
    #       const UnicodeString &textStr,
    #       int32_t textPos
    # )
    it2 = StringCharacterIterator(text_str, 2)
    assert it2.get_length() == 4
    assert len(it2) == 4
    dest.remove()
    it2.get_text(dest)
    assert dest == expected
    assert it2.current() == 0xdf38

    it2a = StringCharacterIterator("a\U0001f338b", 2)
    assert it2a.get_length() == 4
    assert len(it2a) == 4
    dest.remove()
    it2a.get_text(dest)
    assert dest == expected
    assert it2a.current() == 0xdf38

    # [3]
    # icu::StringCharacterIterator::StringCharacterIterator(
    #       const UnicodeString &textStr,
    #       int32_t textBegin,
    #       int32_t textEnd,
    #       int32_t textPos
    # )
    it3 = StringCharacterIterator(text_str, 1, 3, 0)
    assert it3.get_length() == 4
    assert len(it3) == 4
    dest.remove()
    it3.get_text(dest)
    assert dest == expected
    assert it3.current() == 0xd83c
    assert it3.next() == 0xdf38
    assert it3.next() == StringCharacterIterator.DONE
    assert it3.first() == 0xd83c

    it3a = StringCharacterIterator("a\U0001f338b", 1, 3, 0)
    assert it3a.get_length() == 4
    assert len(it3a) == 4
    dest.remove()
    it3a.get_text(dest)
    assert dest == expected
    assert it3a.current() == 0xd83c
    assert it3a.next() == 0xdf38
    assert it3a.next() == StringCharacterIterator.DONE
    assert it3a.first() == 0xd83c

    # [4]
    # icu::StringCharacterIterator::StringCharacterIterator(
    #       const StringCharacterIterator &that
    # )
    it1.set_index(2)
    it4 = StringCharacterIterator(it1)
    assert it4.get_length() == 4
    assert len(it4) == 4
    dest.remove()
    it4.get_text(dest)
    assert dest == expected
    assert it4.get_index() == 2
    assert it4.current() == 0xdf38

from icupy.icu import CanonicalIterator, UnicodeString


def test_api():
    # From icu/source/test/intltest/canittst.cpp

    # icu::CanonicalIterator::CanonicalIterator(
    #       const UnicodeString &source,
    #       UErrorCode &status
    # )
    source = UnicodeString("ljubav")
    it = CanonicalIterator(source)

    # UnicodeString icu::CanonicalIterator::getSource()
    result = it.get_source()
    assert isinstance(result, UnicodeString)
    assert result == source

    # str -> UnicodeString
    source = "ljubav"
    it = CanonicalIterator(source)
    assert it.get_source() == source

    # void icu::CanonicalIterator::setSource(
    #       const UnicodeString &newSource,
    #       UErrorCode &status
    # )
    new_source = UnicodeString("\\u010d\\u017E").unescape()
    expected = [
        "c\u030cz\u030c",
        "c\u030c\u017e",
        "\u010dz\u030c",
        "\u010d\u017e",
    ]
    it.set_source(new_source)

    # __iter__() and __next__()
    assert len(list(it)) == len(expected)
    assert all(isinstance(x, UnicodeString) for x in it)
    assert all(x in it for x in expected)

    # void icu::CanonicalIterator::reset()
    it.reset()

    # UnicodeString icu::CanonicalIterator::next()
    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert result.is_bogus()
    assert len(expected) == 0

    # str -> UnicodeString
    new_source = "x\u0307\u0327"
    expected = ["x\u0307\u0327", "x\u0327\u0307", "\u1e8b\u0327"]
    it.set_source(new_source)
    assert len(list(it)) == len(expected)
    assert all(isinstance(x, UnicodeString) for x in it)
    assert all(x in it for x in expected)

    it.reset()
    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert not result.is_bogus()
    assert result in expected
    expected.remove(result)

    result = it.next()
    assert isinstance(result, UnicodeString)
    assert result.is_bogus()
    assert len(expected) == 0

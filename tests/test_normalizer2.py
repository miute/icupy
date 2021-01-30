from icupy import (
    FilteredNormalizer2, Normalizer2, UnicodeSet, UnicodeString,
    UNormalization2Mode, UNormalizationCheckResult, US_INV,
)


def test_api():
    n2 = Normalizer2.get_nfc_instance()

    # UnicodeString& append(UnicodeString &first,
    #                       const UnicodeString &second,
    #                       UErrorCode &errorCode
    # )
    first = UnicodeString("A", -1, US_INV)
    second = UnicodeString("\\u030A", -1, US_INV).unescape()
    third = UnicodeString("B", -1, US_INV)
    result = n2.append(first, second).append(third)
    assert result == first == "\xC5B"

    assert n2.compose_pair(ord("A"), 0x030a) == 0xc5

    assert n2.get_combining_class(0x3099) == 8

    decomposition = UnicodeString()
    assert n2.get_decomposition(0xc5, decomposition)
    assert decomposition == UnicodeString("A\\u030A", -1, US_INV).unescape()

    decomposition.remove()
    assert n2.get_raw_decomposition(0xc5, decomposition)
    assert decomposition == UnicodeString("A\\u030A", -1, US_INV).unescape()

    # U+304C: Hiragana Letter Ga
    # U+304B: Hiragana Letter Ka
    # U+3099: Combining Katakana-Hiragana Voiced Sound Mark
    assert n2.has_boundary_after(0x304c)
    assert not n2.has_boundary_after(0x304b)
    assert not n2.has_boundary_after(0x3099)

    # U+0103: Latin Small Letter A with Breve
    # U+0306: Combining Breve
    assert n2.has_boundary_before(0x0103)
    assert n2.has_boundary_before(ord("a"))
    assert not n2.has_boundary_before(0x0306)

    assert n2.is_inert(0x304c)
    assert not n2.is_inert(0x304b)
    assert not n2.is_inert(0x3099)

    # UBool isNormalized(const UnicodeString &s,
    #                    UErrorCode &errorCode
    # )
    assert n2.is_normalized(UnicodeString(0x304c))
    assert not n2.is_normalized(
        UnicodeString("\\u304B\\u3099", -1, US_INV).unescape())

    # [1]
    # UnicodeString normalize(const UnicodeString &src,
    #                         UErrorCode &errorCode
    # )
    src = UnicodeString("a\\u0306", -1, US_INV).unescape()
    result = n2.normalize(src)
    assert src != result
    assert result == "\u0103"

    # [2]
    # UnicodeString& normalize(const UnicodeString &src,
    #                          UnicodeString &dest,
    #                          UErrorCode &errorCode
    # )
    dest = UnicodeString()
    result = n2.normalize(src, dest)
    assert src != result
    assert result == dest == "\u0103"

    # UnicodeString& normalizeSecondAndAppend(
    #       UnicodeString &first,
    #       const UnicodeString &second,
    #       UErrorCode &errorCode
    # )
    first = UnicodeString("A", -1, US_INV)
    second = UnicodeString("a\\u0306", -1, US_INV).unescape()
    third = UnicodeString("b")
    result = n2.normalize_second_and_append(first, second).append(third)
    assert result == first == "A\u0103b"

    # UNormalizationCheckResult quickCheck(
    #       const UnicodeString &s,
    #       UErrorCode &errorCode
    # )
    result = n2.quick_check(UnicodeString(0x304c))
    assert result == UNormalizationCheckResult.UNORM_YES
    result = n2.quick_check(
        UnicodeString("\\u304B\\u3099", -1, US_INV).unescape())
    assert result == UNormalizationCheckResult.UNORM_MAYBE

    # int32_t spanQuickCheckYes(const UnicodeString &s,
    #                           UErrorCode &errorCode
    # )
    assert n2.span_quick_check_yes(UnicodeString(0x304c))
    assert not n2.span_quick_check_yes(
        UnicodeString("\\u304B\\u3099", -1, US_INV).unescape())


def test_filtered_normalizer2():
    n2 = Normalizer2.get_nfc_instance()
    filter = UnicodeSet(UnicodeString("[^\\u00a0-\\u00ff]", -1, US_INV))
    fn2 = FilteredNormalizer2(n2, filter)
    decomposition = UnicodeString()

    assert n2.get_decomposition(0xe4, decomposition)
    assert decomposition == UnicodeString("a\\u0308", -1, US_INV).unescape()
    assert n2.get_decomposition(0x100, decomposition)
    assert decomposition == UnicodeString("A\\u0304", -1, US_INV).unescape()

    assert not fn2.get_decomposition(0xe4, decomposition)
    assert fn2.get_decomposition(0x100, decomposition)
    assert decomposition == UnicodeString("A\\u0304", -1, US_INV).unescape()


def test_get_instance():
    nfkc1 = Normalizer2.get_instance(
        None,
        "nfkc",
        UNormalization2Mode.UNORM2_COMPOSE)
    nfc = Normalizer2.get_nfc_instance()
    nfd = Normalizer2.get_nfd_instance()
    nfkc_cf = Normalizer2.get_nfkc_casefold_instance()
    nfkc2 = Normalizer2.get_nfkc_instance()
    nfkd = Normalizer2.get_nfkd_instance()

    source = UnicodeString("\\u1E9B\\u0323\\xDF", -1, US_INV).unescape()

    result = nfkc1.normalize(source)
    assert result == UnicodeString("\\u1E69\\xDF", -1, US_INV).unescape()

    result = nfkc2.normalize(source)
    assert result == UnicodeString("\\u1E69\\xDF", -1, US_INV).unescape()

    result = nfkc_cf.normalize(source)
    assert result == UnicodeString("\\u1E69ss", -1, US_INV).unescape()

    result = nfc.normalize(source)
    assert result == UnicodeString("\\u1E9B\\u0323\\xDF",
                                   -1,
                                   US_INV).unescape()

    result = nfd.normalize(source)
    assert result == UnicodeString("\\u017F\\u0323\\u0307\\xDF",
                                   -1,
                                   US_INV).unescape()

    result = nfkd.normalize(source)
    assert result == UnicodeString("s\\u0323\\u0307\\xDF",
                                   -1,
                                   US_INV).unescape()

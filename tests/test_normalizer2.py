import pytest

# fmt: off
from icupy.icu import (
    U_ICU_VERSION_MAJOR_NUM, US_INV, FilteredNormalizer2, Normalizer2,
    UnicodeSet, UnicodeString, UNormalization2Mode, UNormalizationCheckResult,
)

# fmt: on


def test_api():
    # static const Normalizer2 *icu::Normalizer2::getInstance(
    #       const char *packageName,
    #       const char *name,
    #       UNormalization2Mode mode,
    #       UErrorCode &errorCode
    # )
    n2 = Normalizer2.get_instance(
        None, "nfc", UNormalization2Mode.UNORM2_COMPOSE
    )

    # UnicodeString &icu::Normalizer2::append(
    #       UnicodeString &first,
    #       const UnicodeString &second,
    #       UErrorCode &errorCode
    # )
    first = UnicodeString("A", -1, US_INV)
    second = UnicodeString("\\u030A", -1, US_INV).unescape()
    third = UnicodeString("B", -1, US_INV)
    result = n2.append(first, second).append(third)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(first)
    assert result == first == "\xC5B"

    first = UnicodeString("A", -1, US_INV)
    result = n2.append(first, "\u030A").append(third)
    assert result == first == "\xC5B"

    # UBool icu::Normalizer2::getDecomposition(
    #       UChar32 c,
    #       UnicodeString &decomposition
    # )
    decomposition = UnicodeString()
    assert n2.get_decomposition(0xC5, decomposition)
    assert decomposition == UnicodeString("A\\u030A", -1, US_INV).unescape()

    # UBool icu::Normalizer2::hasBoundaryAfter(UChar32 c)
    # U+304C: Hiragana Letter Ga
    # U+304B: Hiragana Letter Ka
    # U+3099: Combining Katakana-Hiragana Voiced Sound Mark
    assert n2.has_boundary_after(0x304C)
    assert not n2.has_boundary_after(0x304B)
    assert not n2.has_boundary_after(0x3099)

    # UBool icu::Normalizer2::hasBoundaryBefore(UChar32 c)
    # U+0103: Latin Small Letter A with Breve
    # U+0306: Combining Breve
    assert n2.has_boundary_before(0x0103)
    assert n2.has_boundary_before(ord("a"))
    assert not n2.has_boundary_before(0x0306)

    # UBool icu::Normalizer2::isInert(UChar32 c)
    assert n2.is_inert(0x304C)
    assert not n2.is_inert(0x304B)
    assert not n2.is_inert(0x3099)

    # UBool icu::Normalizer2::isNormalized(
    #       const UnicodeString &s,
    #       UErrorCode &errorCode
    # )
    assert n2.is_normalized(UnicodeString(0x304C))
    assert not n2.is_normalized(
        UnicodeString("\\u304B\\u3099", -1, US_INV).unescape()
    )

    assert n2.is_normalized("\u304c")
    assert not n2.is_normalized("\u304B\u3099")

    # [1]
    # UnicodeString icu::Normalizer2::normalize(
    #       const UnicodeString &src,
    #       UErrorCode &errorCode
    # )
    src = UnicodeString("a\\u0306", -1, US_INV).unescape()
    result = n2.normalize(src)
    assert isinstance(result, UnicodeString)
    assert result == "\u0103"

    result = n2.normalize("a\u0306")
    assert result == "\u0103"

    # [2]
    # UnicodeString &icu::Normalizer2::normalize(
    #       const UnicodeString &src,
    #       UnicodeString &dest,
    #       UErrorCode &errorCode
    # )
    dest = UnicodeString()
    result = n2.normalize(src, dest)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(dest)
    assert result == dest == "\u0103"

    dest.remove()
    result = n2.normalize("a\u0306", dest)
    assert result == dest == "\u0103"

    # UnicodeString &icu::Normalizer2::normalizeSecondAndAppend(
    #       UnicodeString &first,
    #       const UnicodeString &second,
    #       UErrorCode &errorCode
    # )
    first = UnicodeString("A", -1, US_INV)
    second = UnicodeString("a\\u0306", -1, US_INV).unescape()
    third = UnicodeString("b")
    result = n2.normalize_second_and_append(first, second).append(third)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(first)
    assert result == first == "A\u0103b"

    first = UnicodeString("A", -1, US_INV)
    result = n2.normalize_second_and_append(first, "a\u0306").append(third)
    assert result == first == "A\u0103b"

    # UNormalizationCheckResult icu::Normalizer2::quickCheck(
    #       const UnicodeString &s,
    #       UErrorCode &errorCode
    # )
    result = n2.quick_check(UnicodeString(0x304C))
    assert result == UNormalizationCheckResult.UNORM_YES
    result = n2.quick_check(
        UnicodeString("\\u304B\\u3099", -1, US_INV).unescape()
    )
    assert result == UNormalizationCheckResult.UNORM_MAYBE

    result = n2.quick_check("\u304c")
    assert result == UNormalizationCheckResult.UNORM_YES
    result = n2.quick_check("\u304B\u3099")
    assert result == UNormalizationCheckResult.UNORM_MAYBE

    # int32_t icu::Normalizer2::spanQuickCheckYes(
    #       const UnicodeString &s,
    #       UErrorCode &errorCode
    # )
    assert n2.span_quick_check_yes(UnicodeString(0x304C))
    assert not n2.span_quick_check_yes(
        UnicodeString("\\u304B\\u3099", -1, US_INV).unescape()
    )

    assert n2.span_quick_check_yes("\u304c")
    assert not n2.span_quick_check_yes("\u304B\u3099")


def test_filtered_normalizer2():
    # icu::FilteredNormalizer2::FilteredNormalizer2(
    #       const Normalizer2 &n2,
    #       const UnicodeSet &filterSet
    # )
    n2 = Normalizer2.get_instance(
        None, "nfc", UNormalization2Mode.UNORM2_COMPOSE
    )
    filter_set = UnicodeSet(UnicodeString("[^\\u00a0-\\u00ff]", -1, US_INV))
    fn2 = FilteredNormalizer2(n2, filter_set)

    decomposition = UnicodeString()
    assert n2.get_decomposition(0xE4, decomposition)
    assert decomposition == UnicodeString("a\\u0308", -1, US_INV).unescape()

    assert n2.get_decomposition(0x100, decomposition)
    assert decomposition == UnicodeString("A\\u0304", -1, US_INV).unescape()

    assert not fn2.get_decomposition(0xE4, decomposition)

    assert fn2.get_decomposition(0x100, decomposition)
    assert decomposition == UnicodeString("A\\u0304", -1, US_INV).unescape()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_get_instance():
    # static const Normalizer2 *icu::Normalizer2::getInstance(
    #       const char *packageName,
    #       const char *name,
    #       UNormalization2Mode mode,
    #       UErrorCode &errorCode
    # )
    nfkc1 = Normalizer2.get_instance(
        None, "nfkc", UNormalization2Mode.UNORM2_COMPOSE
    )
    assert isinstance(nfkc1, Normalizer2)

    # static const Normalizer2 *icu::Normalizer2::getNFCInstance(
    #       UErrorCode &errorCode
    # )
    nfc = Normalizer2.get_nfc_instance()
    assert isinstance(nfc, Normalizer2)

    # static const Normalizer2 *icu::Normalizer2::getNFDInstance(
    #       UErrorCode &errorCode
    # )
    nfd = Normalizer2.get_nfd_instance()
    assert isinstance(nfd, Normalizer2)

    # static const Normalizer2 *icu::Normalizer2::getNFKCCasefoldInstance(
    #       UErrorCode &errorCode
    # )
    nfkc_cf = Normalizer2.get_nfkc_casefold_instance()
    assert isinstance(nfkc_cf, Normalizer2)

    # static const Normalizer2 *icu::Normalizer2::getNFKCInstance(
    #       UErrorCode &errorCode
    # )
    nfkc2 = Normalizer2.get_nfkc_instance()
    assert isinstance(nfkc2, Normalizer2)

    # static const Normalizer2 *icu::Normalizer2::getNFKDInstance(
    #       UErrorCode &errorCode
    # )
    nfkd = Normalizer2.get_nfkd_instance()
    assert isinstance(nfkd, Normalizer2)

    source = UnicodeString("\\u1E9B\\u0323\\xDF", -1, US_INV).unescape()

    result = nfkc1.normalize(source)
    assert result == UnicodeString("\\u1E69\\xDF", -1, US_INV).unescape()

    result = nfkc2.normalize(source)
    assert result == UnicodeString("\\u1E69\\xDF", -1, US_INV).unescape()

    result = nfkc_cf.normalize(source)
    assert result == UnicodeString("\\u1E69ss", -1, US_INV).unescape()

    result = nfc.normalize(source)
    assert (
        result == UnicodeString("\\u1E9B\\u0323\\xDF", -1, US_INV).unescape()
    )

    result = nfd.normalize(source)
    assert (
        result
        == UnicodeString("\\u017F\\u0323\\u0307\\xDF", -1, US_INV).unescape()
    )

    result = nfkd.normalize(source)
    assert (
        result == UnicodeString("s\\u0323\\u0307\\xDF", -1, US_INV).unescape()
    )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_icu_49():
    n2 = Normalizer2.get_nfc_instance()

    # UChar32 icu::Normalizer2::composePair(
    #       UChar32 a,
    #       UChar32 b
    # )
    assert n2.compose_pair(ord("A"), 0x030A) == 0xC5

    # uint8_t icu::Normalizer2::getCombiningClass(UChar32 c)
    assert n2.get_combining_class(0x3099) == 8

    # UBool icu::Normalizer2::getRawDecomposition(
    #       UChar32 c,
    #       UnicodeString &decomposition
    # )
    decomposition = UnicodeString()
    assert n2.get_raw_decomposition(0xC5, decomposition)
    assert decomposition == UnicodeString("A\\u030A", -1, US_INV).unescape()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_icu_60():
    from icupy.icu import Edits

    # From icu/source/test/intltest/tstnorm.cpp
    #  BasicNormalizerTest::TestComposeUTF8WithEdits()
    nfkc_cf = Normalizer2.get_nfkc_casefold_instance()
    src = (
        "  AÄA\u0308A\u0308\u00ad\u0323Ä\u0323,"
        "\u00ad\u1100\u1161가\u11A8가\u3133  "
    ).encode()
    expected = "  aääạ\u0308ạ\u0308,가각갃  ".encode()

    # void icu::Normalizer2::normalizeUTF8(
    #       uint32_t options,
    #       StringPiece src,
    #       ByteSink &sink,
    #       Edits *edits,
    #       UErrorCode &errorCode
    # ) const
    result = nfkc_cf.normalize_utf8(0, src)
    assert isinstance(result, bytes)
    assert result == expected

    edits = Edits()
    assert not edits.has_changes()
    result = nfkc_cf.normalize_utf8(0, src, edits)
    assert isinstance(result, bytes)
    assert result == expected
    assert edits.has_changes()
    assert edits.number_of_changes() == 9
    it = edits.get_fine_iterator()
    result = []
    while True:
        if not it.next():
            break
        result.append((it.has_change(), it.old_length(), it.new_length()))
    assert result == [
        (False, 2, 2),  # 2 spaces
        (True, 1, 1),  # A→a
        (True, 2, 2),  # Ä→ä
        (True, 3, 2),  # A\u0308→ä
        (True, 7, 5),  # A\u0308\u00ad\u0323→ạ\u0308 removes the soft hyphen
        (True, 4, 5),  # Ä\u0323→ạ\u0308
        (False, 1, 1),  # comma
        (True, 2, 0),  # U+00AD soft hyphen maps to empty
        (True, 6, 3),  # \u1100\u1161→가
        (True, 6, 3),  # 가\u11A8→각
        (True, 6, 3),  # 가\u3133→갃
        (False, 2, 2),  # 2 spaces
    ]

    # UBool icu::Normalizer2::isNormalizedUTF8(
    #       StringPiece s,
    #       UErrorCode &errorCode
    # ) const
    assert nfkc_cf.is_normalized_utf8(src) is False
    assert nfkc_cf.is_normalized_utf8(expected) is True


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_icu_74():
    # static const Normalizer2 *
    # icu::Normalizer2::getNFKCSimpleCasefoldInstance(UErrorCode&)
    n2 = Normalizer2.get_nfkc_simple_casefold_instance()
    assert isinstance(n2, Normalizer2)
    assert n2.normalize("aA\u0308 ßẞ \u1F80\u1F88") == "aä ßß \u1F80\u1F80"

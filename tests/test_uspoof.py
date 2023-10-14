from pathlib import Path

import pytest

# fmt: off
from icupy.icu import (
    U_ICU_VERSION_MAJOR_NUM, UnicodeSet, UnicodeString, UParseError,
    USpoofChecks, uspoof_are_confusable, uspoof_are_confusable_unicode_string,
    uspoof_are_confusable_utf8, uspoof_check, uspoof_check_unicode_string,
    uspoof_check_utf8, uspoof_clone, uspoof_close, uspoof_get_allowed_chars,
    uspoof_get_allowed_locales, uspoof_get_allowed_unicode_set,
    uspoof_get_checks, uspoof_get_skeleton, uspoof_get_skeleton_unicode_string,
    uspoof_get_skeleton_utf8, uspoof_open, uspoof_open_from_serialized,
    uspoof_open_from_source, uspoof_serialize, uspoof_set_allowed_chars,
    uspoof_set_allowed_locales, uspoof_set_allowed_unicode_set,
    uspoof_set_checks,
)
from icupy.utils import gc

# fmt: on


def test_api():
    # From icu/source/test/cintltst/spooftest.c

    # USpoofChecker *uspoof_open(UErrorCode *status)
    # void uspoof_close(USpoofChecker *sc)
    with gc(uspoof_open(), uspoof_close) as sc:
        sc_mixed = "\x73\u0441"  # "sc", with Cyrillic 'c'
        sc_latin = "\x73\x63"  # "sc", plain ascii

        # int32_t uspoof_areConfusable(
        #       const USpoofChecker *sc,
        #       const UChar *id1,
        #       int32_t length1,
        #       const UChar *id2,
        #       int32_t length2,
        #       UErrorCode *status
        # )
        assert (
            uspoof_are_confusable(sc, sc_latin, -1, sc_mixed, -1)
            == USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )
        assert (
            uspoof_are_confusable(
                sc, sc_latin, len(sc_latin), sc_mixed, len(sc_mixed)
            )
            == USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )

        s1 = UnicodeString("cxs")
        s2 = UnicodeString(
            "\\u0441\\u0445\\u0455"
        ).unescape()  # Cyrillic "cxs"

        # int32_t uspoof_areConfusableUnicodeString(
        #       const USpoofChecker *sc,
        #       const icu::UnicodeString &s1,
        #       const icu::UnicodeString &s2,
        #       UErrorCode *status
        # )
        assert uspoof_are_confusable_unicode_string(sc, s1, s2) == (
            USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )
        assert uspoof_are_confusable_unicode_string(sc, "cxs", s2) == (
            USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )
        assert uspoof_are_confusable_unicode_string(
            sc, s1, "\u0441\u0445\u0455"
        ) == (
            USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )
        assert uspoof_are_confusable_unicode_string(
            sc, "cxs", "\u0441\u0445\u0455"
        ) == (
            USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )

        b_sc_latin = sc_latin.encode()
        b_sc_mixed = sc_mixed.encode()

        # int32_t uspoof_areConfusableUTF8(
        #       const USpoofChecker *sc,
        #       const char *id1,
        #       int32_t length1,
        #       const char *id2,
        #       int32_t length2,
        #       UErrorCode *status
        # )
        assert (
            uspoof_are_confusable_utf8(sc, b_sc_latin, -1, b_sc_mixed, -1)
            == USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )
        assert (
            uspoof_are_confusable_utf8(
                sc, b_sc_latin, len(b_sc_latin), b_sc_mixed, len(b_sc_mixed)
            )
            == USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )

        # int32_t uspoof_check(
        #       const USpoofChecker *sc,
        #       const UChar *id,
        #       int32_t length,
        #       int32_t *position,
        #       UErrorCode *status
        # )
        assert (
            uspoof_check(sc, sc_mixed, -1) == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            uspoof_check(sc, sc_mixed, len(sc_mixed))
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            uspoof_check(sc, sc_mixed)  # length is optional
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )

        # int32_t uspoof_checkUnicodeString(
        #       const USpoofChecker *sc,
        #       const icu::UnicodeString &id,
        #       int32_t *position,
        #       UErrorCode *status
        # )
        assert (
            uspoof_check_unicode_string(sc, UnicodeString(sc_mixed))
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            uspoof_check_unicode_string(sc, sc_mixed)
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )

        # int32_t uspoof_checkUTF8(
        #       const USpoofChecker *sc,
        #       const char *id,
        #       int32_t length,
        #       int32_t *position,
        #       UErrorCode *status
        # )
        assert (
            uspoof_check_utf8(sc, b_sc_mixed, -1)
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            uspoof_check_utf8(sc, b_sc_mixed, len(b_sc_mixed))
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            uspoof_check_utf8(sc, b_sc_mixed)  # length is optional
            == USpoofChecks.USPOOF_SINGLE_SCRIPT
        )

        # USpoofChecker *uspoof_clone(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        sc2 = uspoof_clone(sc)
        uspoof_close(sc2)

        # const USet *uspoof_getAllowedChars(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        uset = uspoof_get_allowed_chars(sc)
        uniset1 = UnicodeSet.from_uset(uset)
        assert uniset1.size() > 0

        # const char *uspoof_getAllowedLocales(
        #       USpoofChecker *sc,
        #       UErrorCode *status
        # )
        result = uspoof_get_allowed_locales(sc)
        assert isinstance(result, str)

        # const icu::UnicodeSet *uspoof_getAllowedUnicodeSet(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        uniset2 = uspoof_get_allowed_unicode_set(sc)
        assert isinstance(uniset2, UnicodeSet)
        assert uniset1 == uniset2

        # int32_t uspoof_getChecks(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        assert uspoof_get_checks(sc) == USpoofChecks.USPOOF_ALL_CHECKS

        lll_latin_a = "\x6c\x49\x31"  # lI1, all ASCII
        lll_skel = "\x6c\x6c\x6c"

        # int32_t uspoof_getSkeleton(
        #       const USpoofChecker *sc,
        #       uint32_t type,
        #       const UChar *id,
        #       int32_t length,
        #       UChar *dest,
        #       int32_t destCapacity,
        #       UErrorCode *status
        # )
        type_ = USpoofChecks.USPOOF_ANY_CASE  # Deprecated in ICU 58
        dest = uspoof_get_skeleton(sc, type_, lll_latin_a, -1)
        assert isinstance(dest, str)
        assert dest == lll_skel

        dest = uspoof_get_skeleton(sc, type_, lll_latin_a, len(lll_latin_a))
        assert isinstance(dest, str)
        assert dest == lll_skel

        # icu::UnicodeString &uspoof_getSkeletonUnicodeString(
        #       const USpoofChecker *sc,
        #       uint32_t type,
        #       const icu::UnicodeString &id,
        #       icu::UnicodeString &dest,
        #       UErrorCode *status
        # )
        dest = UnicodeString()
        result = uspoof_get_skeleton_unicode_string(
            sc, type_, UnicodeString(lll_latin_a), dest
        )
        assert isinstance(result, UnicodeString)
        assert id(result) == id(dest)
        assert dest == lll_skel

        result = uspoof_get_skeleton_unicode_string(
            sc, type_, lll_latin_a, dest
        )
        assert isinstance(result, UnicodeString)
        assert id(result) == id(dest)
        assert dest == lll_skel

        b_lll_latin_a = lll_latin_a.encode()

        # int32_t uspoof_getSkeletonUTF8(
        #       const USpoofChecker *sc,
        #       uint32_t type,
        #       const char *id,
        #       int32_t length,
        #       char *dest,
        #       int32_t destCapacity,
        #       UErrorCode *status
        # )
        dest = uspoof_get_skeleton_utf8(sc, type_, b_lll_latin_a, -1)
        assert isinstance(dest, bytes)
        assert dest.decode() == lll_skel

        dest = uspoof_get_skeleton_utf8(
            sc, type_, b_lll_latin_a, len(b_lll_latin_a)
        )
        assert isinstance(dest, bytes)
        assert dest.decode() == lll_skel

        # void uspoof_setAllowedLocales(
        #       USpoofChecker *sc,
        #       const char *localesList,
        #       UErrorCode *status
        # )
        uspoof_set_allowed_locales(sc, "en, ru_RU")
        assert uspoof_get_allowed_locales(sc) == "en, ru_RU"

        # void uspoof_setAllowedUnicodeSet(
        #       USpoofChecker *sc,
        #       const icu::UnicodeSet *chars,
        #       UErrorCode *status
        # )
        uspoof_set_allowed_unicode_set(sc, UnicodeSet(0, 0x10FFFF))
        assert uspoof_get_allowed_unicode_set(sc) == UnicodeSet(0, 0x10FFFF)

        # void uspoof_setChecks(
        #       USpoofChecker *sc,
        #       int32_t checks,
        #       UErrorCode *status
        # )
        uspoof_set_checks(
            sc,
            USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_ANY_CASE,
        )
        assert (
            uspoof_get_checks(sc)
            == USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | USpoofChecks.USPOOF_ANY_CASE
        )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51():
    # fmt: off
    from icupy.icu import (
        URestrictionLevel, uspoof_get_inclusion_set,
        uspoof_get_inclusion_unicode_set, uspoof_get_recommended_set,
        uspoof_get_recommended_unicode_set, uspoof_get_restriction_level,
        uspoof_set_restriction_level,
    )

    # fmt: on
    # const USet *uspoof_getInclusionSet(UErrorCode *status)
    uset = uspoof_get_inclusion_set()
    uniset1 = UnicodeSet.from_uset(uset)
    assert uniset1.size() > 0

    # const icu::UnicodeSet *uspoof_getInclusionUnicodeSet(UErrorCode *status)
    uniset2 = uspoof_get_inclusion_unicode_set()
    assert isinstance(uniset2, UnicodeSet)
    assert uniset1 == uniset2

    # const USet *uspoof_getRecommendedSet(UErrorCode *status)
    uset = uspoof_get_recommended_set()
    uniset3 = UnicodeSet.from_uset(uset)
    assert uniset3.size() > 0

    # const icu::UnicodeSet *uspoof_getRecommendedUnicodeSet(
    #       UErrorCode *status
    # )
    uniset4 = uspoof_get_recommended_unicode_set()
    assert isinstance(uniset4, UnicodeSet)
    assert uniset3 == uniset4

    with gc(uspoof_open(), uspoof_close) as sc:
        # URestrictionLevel uspoof_getRestrictionLevel(const USpoofChecker *sc)
        assert (
            uspoof_get_restriction_level(sc)
            == URestrictionLevel.USPOOF_HIGHLY_RESTRICTIVE
        )

        # void uspoof_setRestrictionLevel(
        #       USpoofChecker *sc,
        #       URestrictionLevel restrictionLevel
        # )
        uspoof_set_restriction_level(
            sc, URestrictionLevel.USPOOF_MODERATELY_RESTRICTIVE
        )
        assert (
            uspoof_get_restriction_level(sc)
            == URestrictionLevel.USPOOF_MODERATELY_RESTRICTIVE
        )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 58, reason="ICU4C<58")
def test_api_58():
    # From icu/source/test/intltest/itspoof.cpp
    # fmt: off
    from icupy.icu import (
        URestrictionLevel, uspoof_check2, uspoof_check2_unicode_string,
        uspoof_check2_utf8, uspoof_close_check_result,
        uspoof_get_check_result_checks, uspoof_get_check_result_numerics,
        uspoof_get_check_result_restriction_level, uspoof_open_check_result,
    )

    # fmt: on
    # USpoofCheckResult *uspoof_openCheckResult(UErrorCode *status)
    # void uspoof_closeCheckResult(USpoofCheckResult *checkResult)
    with gc(uspoof_open(), uspoof_close) as sc, gc(
        uspoof_open_check_result(), uspoof_close_check_result
    ) as check_result:
        id_ = "m\u0307"

        # int32_t uspoof_check2(
        #       const USpoofChecker *sc,
        #       const UChar *id,
        #       int32_t length,
        #       USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert uspoof_check2(sc, id_, -1, check_result) == 0
        assert uspoof_check2(sc, id_, -1, None) == 0
        assert uspoof_check2(sc, id_, -1) == 0  # checkResult is optional
        assert uspoof_check2(sc, id_) == 0  # length is optional

        assert uspoof_check2(sc, id_, len(id_), check_result) == 0
        assert uspoof_check2(sc, id_, len(id_), None) == 0
        assert uspoof_check2(sc, id_, len(id_)) == 0  # checkResult is optional

        # int32_t uspoof_check2UnicodeString(
        #       const USpoofChecker *sc,
        #       const icu::UnicodeString &id,
        #       USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert (
            uspoof_check2_unicode_string(sc, UnicodeString(id_), check_result)
            == 0
        )
        assert uspoof_check2_unicode_string(sc, UnicodeString(id_), None) == 0
        assert (
            uspoof_check2_unicode_string(sc, UnicodeString(id_)) == 0
        )  # checkResult is optional

        assert uspoof_check2_unicode_string(sc, id_, check_result) == 0
        assert uspoof_check2_unicode_string(sc, id_, None) == 0
        assert (
            uspoof_check2_unicode_string(sc, id_) == 0
        )  # checkResult is optional

        b_id = id_.encode()

        # int32_t uspoof_check2UTF8(
        #       const USpoofChecker *sc,
        #       const char *id,
        #       int32_t length,
        #       USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert uspoof_check2_utf8(sc, b_id, -1, check_result) == 0
        assert uspoof_check2_utf8(sc, b_id, -1, None) == 0
        assert uspoof_check2_utf8(sc, b_id, -1) == 0  # checkResult is optional
        assert uspoof_check2_utf8(sc, b_id) == 0  # length is optional

        assert uspoof_check2_utf8(sc, b_id, len(b_id), check_result) == 0
        assert uspoof_check2_utf8(sc, b_id, len(b_id), None) == 0
        assert (
            uspoof_check2_utf8(sc, b_id, len(b_id)) == 0
        )  # checkResult is optional

        # int32_t uspoof_getCheckResultChecks(
        #       const USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert uspoof_get_check_result_checks(check_result) == 0

        assert (
            uspoof_check2_utf8(sc, "i\u0307".encode(), -1, check_result)
            == USpoofChecks.USPOOF_HIDDEN_OVERLAY
        )
        assert (
            uspoof_get_check_result_checks(check_result)
            == USpoofChecks.USPOOF_HIDDEN_OVERLAY
        )

        # const USet *uspoof_getCheckResultNumerics(
        #       const USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        uspoof_set_checks(
            sc,
            USpoofChecks.USPOOF_MIXED_NUMBERS
            | USpoofChecks.USPOOF_RESTRICTION_LEVEL,
        )
        assert (
            uspoof_check2_unicode_string(
                sc, UnicodeString("\u0967"), check_result
            )
            == 0
        )
        uset = uspoof_get_check_result_numerics(check_result)
        uniset = UnicodeSet.from_uset(uset)
        assert uniset == UnicodeSet("[\u0966]")

        # URestrictionLevel uspoof_getCheckResultRestrictionLevel(
        #       const USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert (
            uspoof_get_check_result_restriction_level(check_result)
            == URestrictionLevel.USPOOF_SINGLE_SCRIPT_RESTRICTIVE
        )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_are_bidi_confusable():
    # From icu/source/test/intltest/itspoof.cpp
    # fmt: off
    from icupy.icu import (
        USPOOF_SINGLE_SCRIPT_CONFUSABLE, UBiDiDirection,
        uspoof_are_bidi_confusable, uspoof_are_bidi_confusable_unicode_string,
        uspoof_are_bidi_confusable_utf8,
    )

    # fmt: on
    id1 = "J-2"
    id2 = "J\u200F2\u2013\u200F"

    with gc(uspoof_open(), uspoof_close) as sc:
        # uint32_t uspoof_areBidiConfusable(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const UChar *id1,
        #       int32_t length1,
        #       const UChar *id2,
        #       int32_t length2,
        #       UErrorCode *status
        # )
        result = uspoof_are_bidi_confusable(
            sc, UBiDiDirection.UBIDI_LTR, id1, -1, id2, -1
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = uspoof_are_bidi_confusable(
            sc, UBiDiDirection.UBIDI_LTR, id1, len(id1), id2, len(id2)
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        # uint32_t uspoof_areBidiConfusableUnicodeString(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const icu::UnicodeString &s1,
        #       const icu::UnicodeString &s2,
        #       UErrorCode *status
        # )
        result = uspoof_are_bidi_confusable_unicode_string(
            sc,
            UBiDiDirection.UBIDI_LTR,
            UnicodeString(id1),
            UnicodeString(id2),
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = uspoof_are_bidi_confusable_unicode_string(
            sc,
            UBiDiDirection.UBIDI_LTR,
            id1,
            UnicodeString(id2),
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = uspoof_are_bidi_confusable_unicode_string(
            sc,
            UBiDiDirection.UBIDI_LTR,
            UnicodeString(id1),
            id2,
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = uspoof_are_bidi_confusable_unicode_string(
            sc,
            UBiDiDirection.UBIDI_LTR,
            id1,
            id2,
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        b1 = id1.encode()
        b2 = id2.encode()

        # uint32_t uspoof_areBidiConfusableUTF8(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const char *id1,
        #       int32_t length1,
        #       const char *id2,
        #       int32_t length2,
        #       UErrorCode *status
        # )
        result = uspoof_are_bidi_confusable_utf8(
            sc, UBiDiDirection.UBIDI_LTR, b1, -1, b2, -1
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = uspoof_are_bidi_confusable_utf8(
            sc, UBiDiDirection.UBIDI_LTR, b1, len(b1), b2, len(b2)
        )
        assert isinstance(result, int)
        assert result == USPOOF_SINGLE_SCRIPT_CONFUSABLE


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_get_bidi_skeleton():
    # From icu/source/test/intltest/itspoof.cpp
    # fmt: off
    from icupy.icu import (
        UBiDiDirection, uspoof_get_bidi_skeleton,
        uspoof_get_bidi_skeleton_unicode_string, uspoof_get_bidi_skeleton_utf8,
    )

    # fmt: on
    id_ = "A1<שׂ"

    with gc(uspoof_open(), uspoof_close) as sc:
        # int32_t uspoof_getBidiSkeleton(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const UChar *id,
        #       int32_t length,
        #       UChar *dest,
        #       int32_t destCapacity,
        #       UErrorCode *status
        # )
        dest = uspoof_get_bidi_skeleton(sc, UBiDiDirection.UBIDI_LTR, id_, -1)
        assert isinstance(dest, str)
        assert dest == "Al<ש\u0307"

        dest = uspoof_get_bidi_skeleton(
            sc, UBiDiDirection.UBIDI_LTR, id_, len(id_)
        )
        assert isinstance(dest, str)
        assert dest == "Al<ש\u0307"

        dest = UnicodeString()

        # icu::UnicodeString &uspoof_getBidiSkeletonUnicodeString(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const icu::UnicodeString &id,
        #       icu::UnicodeString &dest,
        #       UErrorCode *status
        # )
        result = uspoof_get_bidi_skeleton_unicode_string(
            sc, UBiDiDirection.UBIDI_LTR, UnicodeString(id_), dest
        )
        assert isinstance(result, UnicodeString)
        assert id(result) == id(dest)
        assert dest == "Al<ש\u0307"

        result = uspoof_get_bidi_skeleton_unicode_string(
            sc, UBiDiDirection.UBIDI_LTR, id_, dest
        )
        assert isinstance(result, UnicodeString)
        assert id(result) == id(dest)
        assert dest == "Al<ש\u0307"

        b = id_.encode()

        # int32_t uspoof_getBidiSkeletonUTF8(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const char *id,
        #       int32_t length,
        #       char *dest,
        #       int32_t destCapacity,
        #       UErrorCode *status
        # )
        dest = uspoof_get_bidi_skeleton_utf8(
            sc, UBiDiDirection.UBIDI_LTR, b, -1
        )
        assert isinstance(dest, bytes)
        assert dest.decode() == "Al<ש\u0307"

        dest = uspoof_get_bidi_skeleton_utf8(
            sc, UBiDiDirection.UBIDI_LTR, b, len(b)
        )
        assert isinstance(dest, bytes)
        assert dest.decode() == "Al<ש\u0307"


def test_open_from_serialized():
    with gc(uspoof_open(), uspoof_close) as sc:
        # int32_t uspoof_serialize(
        #       USpoofChecker *sc,
        #       void *data,
        #       int32_t capacity,
        #       UErrorCode *status
        # )
        data = uspoof_serialize(sc)
        assert isinstance(data, bytes)
        assert len(data) > 0

        # USpoofChecker *uspoof_openFromSerialized(
        #       const void *data,
        #       int32_t length,
        #       int32_t *pActualLength,
        #       UErrorCode *pErrorCode
        # )
        with gc(
            uspoof_open_from_serialized(data, len(data)), uspoof_close
        ) as sc2:
            uniset1 = uspoof_get_allowed_unicode_set(sc)
            uniset2 = uspoof_get_allowed_unicode_set(sc2)
            assert uniset1 == uniset2

        with gc(
            uspoof_open_from_serialized(data),  # length is optional
            uspoof_close,
        ) as sc2:
            uniset1 = uspoof_get_allowed_unicode_set(sc)
            uniset2 = uspoof_get_allowed_unicode_set(sc2)
            assert uniset1 == uniset2


def test_open_from_source():
    parent = Path(__file__).resolve().parent
    path1 = parent / "confusables.txt"
    path2 = parent / "confusablesWholeScript.txt"
    try:
        with open(path1, "rb") as fp1, open(path2, "rb") as fp2:
            confusables = fp1.read()
            confusables_whole_script = fp2.read()
            good_latin = "\x75\x7a"

            # USpoofChecker *uspoof_openFromSource(
            #       const char *confusables,
            #       int32_t confusablesLen,
            #       const char *confusablesWholeScript,
            #       int32_t confusablesWholeScriptLen,
            #       int32_t *errType,
            #       UParseError *pe,
            #       UErrorCode *status
            # )
            pe = UParseError()
            with gc(
                uspoof_open_from_source(
                    confusables,
                    len(confusables),
                    confusables_whole_script,
                    len(confusables_whole_script),
                    pe,
                ),
                uspoof_close,
            ) as sc:
                assert pe.offset == 0
                assert uspoof_check(sc, good_latin, -1) == 0

            pe = UParseError()
            with gc(
                uspoof_open_from_source(
                    confusables, -1, confusables_whole_script, -1, pe
                ),
                uspoof_close,
            ) as sc:
                assert pe.offset == 0
                assert uspoof_check(sc, good_latin, -1) == 0

            pe = UParseError()
            with gc(
                uspoof_open_from_source(
                    confusables, -1, None, 0, pe  # Deprecated in ICU 58
                ),
                uspoof_close,
            ) as sc:
                assert pe.offset == 0
                assert uspoof_check(sc, good_latin, -1) == 0
    except FileNotFoundError:
        pytest.skip(
            "confusables.txt or confusablesWholeScript.txt, "
            "or both files are not found (not an error). "
            "You need to copy them from "
            "<icu4c>/icu/source/data/unidata/"
        )


def test_set_allowed_chars():
    with gc(uspoof_open(), uspoof_close) as sc:
        # void uspoof_setAllowedChars(
        #       USpoofChecker *sc,
        #       const USet *chars,
        #       UErrorCode *status
        # )
        uniset = UnicodeSet(0x41, 0x5A)
        chars = uniset.to_uset()
        uspoof_set_allowed_chars(sc, chars)  # USet*
        uniset4 = uspoof_get_allowed_unicode_set(sc)
        assert uniset4 == UnicodeSet("[A-Z]")


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_set_allowed_chars_63():
    from icupy.icu import UProperty, u_get_binary_property_set

    with gc(uspoof_open(), uspoof_close) as sc:
        # const USet *u_getBinaryPropertySet(
        #       UProperty property,
        #       UErrorCode *pErrorCode
        # )
        chars = u_get_binary_property_set(UProperty.UCHAR_POSIX_BLANK)

        # void uspoof_setAllowedChars(
        #       USpoofChecker *sc,
        #       const USet *chars,
        #       UErrorCode *status
        # )
        uspoof_set_allowed_chars(sc, chars)  # const USet*
        uniset = uspoof_get_allowed_unicode_set(sc)
        assert repr(uniset) == (
            "<UnicodeSet('["
            "\\u0009\\ \\u00A0\\u1680\\u2000-\\u200A\\u202F\\u205F\\u3000"
            "]')>"
        )

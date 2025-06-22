from pathlib import Path

import pytest

from icupy import icu
from icupy.utils import gc


def test_api() -> None:
    # From icu/source/test/cintltst/spooftest.c

    # USpoofChecker *uspoof_open(UErrorCode *status)
    # void uspoof_close(USpoofChecker *sc)
    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
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
            icu.uspoof_are_confusable(sc, sc_latin, -1, sc_mixed, -1)
            == icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )
        assert (
            icu.uspoof_are_confusable(sc, sc_latin, len(sc_latin), sc_mixed, len(sc_mixed))
            == icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )

        s1 = icu.UnicodeString("cxs")
        s2 = icu.UnicodeString("\\u0441\\u0445\\u0455").unescape()  # Cyrillic "cxs"

        # int32_t uspoof_areConfusableUnicodeString(
        #       const USpoofChecker *sc,
        #       const icu::UnicodeString &s1,
        #       const icu::UnicodeString &s2,
        #       UErrorCode *status
        # )
        assert icu.uspoof_are_confusable_unicode_string(sc, s1, s2) == (
            icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )
        assert icu.uspoof_are_confusable_unicode_string(sc, "cxs", s2) == (
            icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )
        assert icu.uspoof_are_confusable_unicode_string(sc, s1, "\u0441\u0445\u0455") == (
            icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
        )
        assert icu.uspoof_are_confusable_unicode_string(sc, "cxs", "\u0441\u0445\u0455") == (
            icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
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
            icu.uspoof_are_confusable_utf8(sc, b_sc_latin, -1, b_sc_mixed, -1)
            == icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )
        assert (
            icu.uspoof_are_confusable_utf8(
                sc, b_sc_latin, len(b_sc_latin), b_sc_mixed, len(b_sc_mixed)
            )
            == icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
        )

        # int32_t uspoof_check(
        #       const USpoofChecker *sc,
        #       const UChar *id,
        #       int32_t length,
        #       int32_t *position,
        #       UErrorCode *status
        # )
        assert icu.uspoof_check(sc, sc_mixed, -1) == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        assert (
            icu.uspoof_check(sc, sc_mixed, len(sc_mixed))
            == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            icu.uspoof_check(sc, sc_mixed) == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )  # length is optional

        # int32_t uspoof_checkUnicodeString(
        #       const USpoofChecker *sc,
        #       const icu::UnicodeString &id,
        #       int32_t *position,
        #       UErrorCode *status
        # )
        assert (
            icu.uspoof_check_unicode_string(sc, icu.UnicodeString(sc_mixed))
            == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            icu.uspoof_check_unicode_string(sc, sc_mixed)
            == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )

        # int32_t uspoof_checkUTF8(
        #       const USpoofChecker *sc,
        #       const char *id,
        #       int32_t length,
        #       int32_t *position,
        #       UErrorCode *status
        # )
        assert (
            icu.uspoof_check_utf8(sc, b_sc_mixed, -1) == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            icu.uspoof_check_utf8(sc, b_sc_mixed, len(b_sc_mixed))
            == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )
        assert (
            icu.uspoof_check_utf8(sc, b_sc_mixed) == icu.USpoofChecks.USPOOF_SINGLE_SCRIPT
        )  # length is optional

        # USpoofChecker *uspoof_clone(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        sc2 = icu.uspoof_clone(sc)
        icu.uspoof_close(sc2)

        # const USet *uspoof_getAllowedChars(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        uset = icu.uspoof_get_allowed_chars(sc)
        uniset1 = icu.UnicodeSet.from_uset(uset)
        assert uniset1.size() > 0

        # const char *uspoof_getAllowedLocales(
        #       USpoofChecker *sc,
        #       UErrorCode *status
        # )
        result = icu.uspoof_get_allowed_locales(sc)
        assert isinstance(result, str)

        # const icu::UnicodeSet *uspoof_getAllowedUnicodeSet(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        uniset2 = icu.uspoof_get_allowed_unicode_set(sc)
        assert isinstance(uniset2, icu.UnicodeSet)
        assert uniset1 == uniset2

        # int32_t uspoof_getChecks(
        #       const USpoofChecker *sc,
        #       UErrorCode *status
        # )
        assert icu.uspoof_get_checks(sc) == icu.USpoofChecks.USPOOF_ALL_CHECKS

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
        type_ = icu.USpoofChecks.USPOOF_ANY_CASE  # Deprecated in ICU 58
        dest = icu.uspoof_get_skeleton(sc, type_, lll_latin_a, -1)
        assert isinstance(dest, str)
        assert dest == lll_skel

        dest = icu.uspoof_get_skeleton(sc, type_, lll_latin_a, len(lll_latin_a))
        assert isinstance(dest, str)
        assert dest == lll_skel

        # icu::UnicodeString &uspoof_getSkeletonUnicodeString(
        #       const USpoofChecker *sc,
        #       uint32_t type,
        #       const icu::UnicodeString &id,
        #       icu::UnicodeString &dest,
        #       UErrorCode *status
        # )
        dest = icu.UnicodeString()
        result = icu.uspoof_get_skeleton_unicode_string(
            sc, type_, icu.UnicodeString(lll_latin_a), dest
        )
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(dest)
        assert dest == lll_skel

        result = icu.uspoof_get_skeleton_unicode_string(sc, type_, lll_latin_a, dest)
        assert isinstance(result, icu.UnicodeString)
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
        dest = icu.uspoof_get_skeleton_utf8(sc, type_, b_lll_latin_a, -1)
        assert isinstance(dest, bytes)
        assert dest.decode() == lll_skel

        dest = icu.uspoof_get_skeleton_utf8(sc, type_, b_lll_latin_a, len(b_lll_latin_a))
        assert isinstance(dest, bytes)
        assert dest.decode() == lll_skel

        # void uspoof_setAllowedLocales(
        #       USpoofChecker *sc,
        #       const char *localesList,
        #       UErrorCode *status
        # )
        icu.uspoof_set_allowed_locales(sc, "en, ru_RU")
        assert icu.uspoof_get_allowed_locales(sc) == "en, ru_RU"

        # void uspoof_setAllowedUnicodeSet(
        #       USpoofChecker *sc,
        #       const icu::UnicodeSet *chars,
        #       UErrorCode *status
        # )
        icu.uspoof_set_allowed_unicode_set(sc, icu.UnicodeSet(0, 0x10FFFF))
        assert icu.uspoof_get_allowed_unicode_set(sc) == icu.UnicodeSet(0, 0x10FFFF)

        # void uspoof_setChecks(
        #       USpoofChecker *sc,
        #       int32_t checks,
        #       UErrorCode *status
        # )
        icu.uspoof_set_checks(
            sc,
            icu.USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_ANY_CASE,
        )
        assert (
            icu.uspoof_get_checks(sc)
            == icu.USpoofChecks.USPOOF_WHOLE_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_MIXED_SCRIPT_CONFUSABLE
            | icu.USpoofChecks.USPOOF_ANY_CASE
        )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51() -> None:
    # const USet *uspoof_getInclusionSet(UErrorCode *status)
    uset = icu.uspoof_get_inclusion_set()
    uniset1 = icu.UnicodeSet.from_uset(uset)
    assert uniset1.size() > 0

    # const icu::UnicodeSet *uspoof_getInclusionUnicodeSet(UErrorCode *status)
    uniset2 = icu.uspoof_get_inclusion_unicode_set()
    assert isinstance(uniset2, icu.UnicodeSet)
    assert uniset1 == uniset2

    # const USet *uspoof_getRecommendedSet(UErrorCode *status)
    uset = icu.uspoof_get_recommended_set()
    uniset3 = icu.UnicodeSet.from_uset(uset)
    assert uniset3.size() > 0

    # const icu::UnicodeSet *uspoof_getRecommendedUnicodeSet(
    #       UErrorCode *status
    # )
    uniset4 = icu.uspoof_get_recommended_unicode_set()
    assert isinstance(uniset4, icu.UnicodeSet)
    assert uniset3 == uniset4

    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
        # URestrictionLevel uspoof_getRestrictionLevel(const USpoofChecker *sc)
        assert (
            icu.uspoof_get_restriction_level(sc)
            == icu.URestrictionLevel.USPOOF_HIGHLY_RESTRICTIVE
        )

        # void uspoof_setRestrictionLevel(
        #       USpoofChecker *sc,
        #       URestrictionLevel restrictionLevel
        # )
        icu.uspoof_set_restriction_level(
            sc, icu.URestrictionLevel.USPOOF_MODERATELY_RESTRICTIVE
        )
        assert (
            icu.uspoof_get_restriction_level(sc)
            == icu.URestrictionLevel.USPOOF_MODERATELY_RESTRICTIVE
        )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 58, reason="ICU4C<58")
def test_api_58() -> None:
    # From icu/source/test/intltest/itspoof.cpp

    # USpoofCheckResult *uspoof_openCheckResult(UErrorCode *status)
    # void uspoof_closeCheckResult(USpoofCheckResult *checkResult)
    with (
        gc(icu.uspoof_open(), icu.uspoof_close) as sc,
        gc(icu.uspoof_open_check_result(), icu.uspoof_close_check_result) as check_result,
    ):
        id_ = "m\u0307"

        # int32_t uspoof_check2(
        #       const USpoofChecker *sc,
        #       const UChar *id,
        #       int32_t length,
        #       USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert icu.uspoof_check2(sc, id_, -1, check_result) == 0
        assert icu.uspoof_check2(sc, id_, -1, None) == 0
        assert icu.uspoof_check2(sc, id_, -1) == 0  # checkResult is optional
        assert icu.uspoof_check2(sc, id_) == 0  # length is optional

        assert icu.uspoof_check2(sc, id_, len(id_), check_result) == 0
        assert icu.uspoof_check2(sc, id_, len(id_), None) == 0
        assert icu.uspoof_check2(sc, id_, len(id_)) == 0  # checkResult is optional

        # int32_t uspoof_check2UnicodeString(
        #       const USpoofChecker *sc,
        #       const icu::UnicodeString &id,
        #       USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert icu.uspoof_check2_unicode_string(sc, icu.UnicodeString(id_), check_result) == 0
        assert icu.uspoof_check2_unicode_string(sc, icu.UnicodeString(id_), None) == 0
        assert (
            icu.uspoof_check2_unicode_string(sc, icu.UnicodeString(id_)) == 0
        )  # checkResult is optional

        assert icu.uspoof_check2_unicode_string(sc, id_, check_result) == 0
        assert icu.uspoof_check2_unicode_string(sc, id_, None) == 0
        assert icu.uspoof_check2_unicode_string(sc, id_) == 0  # checkResult is optional

        b_id = id_.encode()

        # int32_t uspoof_check2UTF8(
        #       const USpoofChecker *sc,
        #       const char *id,
        #       int32_t length,
        #       USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert icu.uspoof_check2_utf8(sc, b_id, -1, check_result) == 0
        assert icu.uspoof_check2_utf8(sc, b_id, -1, None) == 0
        assert icu.uspoof_check2_utf8(sc, b_id, -1) == 0  # checkResult is optional
        assert icu.uspoof_check2_utf8(sc, b_id) == 0  # length is optional

        assert icu.uspoof_check2_utf8(sc, b_id, len(b_id), check_result) == 0
        assert icu.uspoof_check2_utf8(sc, b_id, len(b_id), None) == 0
        assert icu.uspoof_check2_utf8(sc, b_id, len(b_id)) == 0  # checkResult is optional

        # int32_t uspoof_getCheckResultChecks(
        #       const USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert icu.uspoof_get_check_result_checks(check_result) == 0

        assert (
            icu.uspoof_check2_utf8(sc, "i\u0307".encode(), -1, check_result)
            == icu.USpoofChecks.USPOOF_HIDDEN_OVERLAY
        )
        assert (
            icu.uspoof_get_check_result_checks(check_result)
            == icu.USpoofChecks.USPOOF_HIDDEN_OVERLAY
        )

        # const USet *uspoof_getCheckResultNumerics(
        #       const USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        icu.uspoof_set_checks(
            sc,
            icu.USpoofChecks.USPOOF_MIXED_NUMBERS | icu.USpoofChecks.USPOOF_RESTRICTION_LEVEL,
        )
        assert (
            icu.uspoof_check2_unicode_string(sc, icu.UnicodeString("\u0967"), check_result) == 0
        )
        uset = icu.uspoof_get_check_result_numerics(check_result)
        uniset = icu.UnicodeSet.from_uset(uset)
        assert uniset == icu.UnicodeSet("[\u0966]")

        # URestrictionLevel uspoof_getCheckResultRestrictionLevel(
        #       const USpoofCheckResult *checkResult,
        #       UErrorCode *status
        # )
        assert (
            icu.uspoof_get_check_result_restriction_level(check_result)
            == icu.URestrictionLevel.USPOOF_SINGLE_SCRIPT_RESTRICTIVE
        )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_are_bidi_confusable() -> None:
    # From icu/source/test/intltest/itspoof.cpp
    id1 = "J-2"
    id2 = "J\u200f2\u2013\u200f"

    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
        # uint32_t uspoof_areBidiConfusable(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const UChar *id1,
        #       int32_t length1,
        #       const UChar *id2,
        #       int32_t length2,
        #       UErrorCode *status
        # )
        result = icu.uspoof_are_bidi_confusable(
            sc, icu.UBiDiDirection.UBIDI_LTR, id1, -1, id2, -1
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = icu.uspoof_are_bidi_confusable(
            sc, icu.UBiDiDirection.UBIDI_LTR, id1, len(id1), id2, len(id2)
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

        # uint32_t uspoof_areBidiConfusableUnicodeString(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const icu::UnicodeString &s1,
        #       const icu::UnicodeString &s2,
        #       UErrorCode *status
        # )
        result = icu.uspoof_are_bidi_confusable_unicode_string(
            sc,
            icu.UBiDiDirection.UBIDI_LTR,
            icu.UnicodeString(id1),
            icu.UnicodeString(id2),
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = icu.uspoof_are_bidi_confusable_unicode_string(
            sc,
            icu.UBiDiDirection.UBIDI_LTR,
            id1,
            icu.UnicodeString(id2),
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = icu.uspoof_are_bidi_confusable_unicode_string(
            sc,
            icu.UBiDiDirection.UBIDI_LTR,
            icu.UnicodeString(id1),
            id2,
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = icu.uspoof_are_bidi_confusable_unicode_string(
            sc,
            icu.UBiDiDirection.UBIDI_LTR,
            id1,
            id2,
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

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
        result = icu.uspoof_are_bidi_confusable_utf8(
            sc, icu.UBiDiDirection.UBIDI_LTR, b1, -1, b2, -1
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE

        result = icu.uspoof_are_bidi_confusable_utf8(
            sc, icu.UBiDiDirection.UBIDI_LTR, b1, len(b1), b2, len(b2)
        )
        assert isinstance(result, int)
        assert result == icu.USPOOF_SINGLE_SCRIPT_CONFUSABLE


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 74, reason="ICU4C<74")
def test_get_bidi_skeleton() -> None:
    # From icu/source/test/intltest/itspoof.cpp
    id_ = "A1<שׂ"

    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
        # int32_t uspoof_getBidiSkeleton(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const UChar *id,
        #       int32_t length,
        #       UChar *dest,
        #       int32_t destCapacity,
        #       UErrorCode *status
        # )
        dest = icu.uspoof_get_bidi_skeleton(sc, icu.UBiDiDirection.UBIDI_LTR, id_, -1)
        assert isinstance(dest, str)
        assert dest == "Al<ש\u0307"

        dest = icu.uspoof_get_bidi_skeleton(sc, icu.UBiDiDirection.UBIDI_LTR, id_, len(id_))
        assert isinstance(dest, str)
        assert dest == "Al<ש\u0307"

        dest = icu.UnicodeString()

        # icu::UnicodeString &uspoof_getBidiSkeletonUnicodeString(
        #       const USpoofChecker *sc,
        #       UBiDiDirection direction,
        #       const icu::UnicodeString &id,
        #       icu::UnicodeString &dest,
        #       UErrorCode *status
        # )
        result = icu.uspoof_get_bidi_skeleton_unicode_string(
            sc, icu.UBiDiDirection.UBIDI_LTR, icu.UnicodeString(id_), dest
        )
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(dest)
        assert dest == "Al<ש\u0307"

        result = icu.uspoof_get_bidi_skeleton_unicode_string(
            sc, icu.UBiDiDirection.UBIDI_LTR, id_, dest
        )
        assert isinstance(result, icu.UnicodeString)
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
        dest = icu.uspoof_get_bidi_skeleton_utf8(sc, icu.UBiDiDirection.UBIDI_LTR, b, -1)
        assert isinstance(dest, bytes)
        assert dest.decode() == "Al<ש\u0307"

        dest = icu.uspoof_get_bidi_skeleton_utf8(sc, icu.UBiDiDirection.UBIDI_LTR, b, len(b))
        assert isinstance(dest, bytes)
        assert dest.decode() == "Al<ש\u0307"


def test_open_from_serialized() -> None:
    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
        # int32_t uspoof_serialize(
        #       USpoofChecker *sc,
        #       void *data,
        #       int32_t capacity,
        #       UErrorCode *status
        # )
        data = icu.uspoof_serialize(sc)
        assert isinstance(data, bytes)
        assert len(data) > 0

        # USpoofChecker *uspoof_openFromSerialized(
        #       const void *data,
        #       int32_t length,
        #       int32_t *pActualLength,
        #       UErrorCode *pErrorCode
        # )
        with gc(icu.uspoof_open_from_serialized(data, len(data)), icu.uspoof_close) as sc2:
            uniset1 = icu.uspoof_get_allowed_unicode_set(sc)
            uniset2 = icu.uspoof_get_allowed_unicode_set(sc2)
            assert uniset1 == uniset2

        with gc(
            icu.uspoof_open_from_serialized(data),  # length is optional
            icu.uspoof_close,
        ) as sc2:
            uniset1 = icu.uspoof_get_allowed_unicode_set(sc)
            uniset2 = icu.uspoof_get_allowed_unicode_set(sc2)
            assert uniset1 == uniset2


def test_open_from_source() -> None:
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
            pe = icu.UParseError()
            with gc(
                icu.uspoof_open_from_source(
                    confusables,
                    len(confusables),
                    confusables_whole_script,
                    len(confusables_whole_script),
                    pe,
                ),
                icu.uspoof_close,
            ) as sc:
                assert pe.offset == 0
                assert icu.uspoof_check(sc, good_latin, -1) == 0

            pe = icu.UParseError()
            with gc(
                icu.uspoof_open_from_source(confusables, -1, confusables_whole_script, -1, pe),
                icu.uspoof_close,
            ) as sc:
                assert pe.offset == 0
                assert icu.uspoof_check(sc, good_latin, -1) == 0

            pe = icu.UParseError()
            with gc(
                icu.uspoof_open_from_source(
                    confusables,
                    -1,
                    None,
                    0,
                    pe,  # Deprecated in ICU 58
                ),
                icu.uspoof_close,
            ) as sc:
                assert pe.offset == 0
                assert icu.uspoof_check(sc, good_latin, -1) == 0
    except FileNotFoundError:
        pytest.skip(
            "confusables.txt or confusablesWholeScript.txt, "
            "or both files are not found (not an error). "
            "You need to copy them from "
            "<icu4c>/icu/source/data/unidata/"
        )


def test_set_allowed_chars() -> None:
    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
        # void uspoof_setAllowedChars(
        #       USpoofChecker *sc,
        #       const USet *chars,
        #       UErrorCode *status
        # )
        uniset = icu.UnicodeSet(0x41, 0x5A)
        chars = uniset.to_uset()
        icu.uspoof_set_allowed_chars(sc, chars)  # USet*
        uniset4 = icu.uspoof_get_allowed_unicode_set(sc)
        assert uniset4 == icu.UnicodeSet("[A-Z]")


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_set_allowed_chars_63() -> None:
    with gc(icu.uspoof_open(), icu.uspoof_close) as sc:
        # const USet *u_getBinaryPropertySet(
        #       UProperty property,
        #       UErrorCode *pErrorCode
        # )
        chars = icu.u_get_binary_property_set(icu.UProperty.UCHAR_POSIX_BLANK)

        # void uspoof_setAllowedChars(
        #       USpoofChecker *sc,
        #       const USet *chars,
        #       UErrorCode *status
        # )
        icu.uspoof_set_allowed_chars(sc, chars)  # const USet*
        uniset = icu.uspoof_get_allowed_unicode_set(sc)
        assert repr(uniset) == (
            "<UnicodeSet('["
            "\\u0009\\ \\u00A0\\u1680\\u2000-\\u200A\\u202F\\u205F\\u3000"
            "]')>"
        )

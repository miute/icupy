import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 65:
    pytest.skip("ICU4C<65", allow_module_level=True)

from icupy.icu import (
    ErrorCode, Locale, LocaleMatcher, UErrorCode, ULocMatchDemotion,
    ULocMatchFavorSubtag,
)


def test_builder():
    locales = [Locale("fr"), Locale("en_GB"), Locale("en")]

    # icu::LocaleMatcher::Builder::Builder()
    #
    # Builder &icu::LocaleMatcher::Builder::addSupportedLocale(
    #       const Locale &locale
    # )
    #
    # LocaleMatcher icu::LocaleMatcher::Builder::build(UErrorCode &errorCode)
    matcher = (LocaleMatcher.Builder()
               .add_supported_locale(locales[0])
               .add_supported_locale("en_GB")
               .add_supported_locale(locales[2])
               .build())
    assert isinstance(matcher, LocaleMatcher)

    # [1]
    # const Locale *icu::LocaleMatcher::getBestMatch(
    #       const Locale &desiredLocale,
    #       UErrorCode &errorCode
    # )
    assert matcher.get_best_match(Locale("en_US")) == Locale("en")
    assert matcher.get_best_match("en_US") == Locale("en")

    # [2]
    # Builder &icu::LocaleMatcher::Builder::setSupportedLocales(
    #       Locale::Iterator &locales
    # )
    #
    # Builder &icu::LocaleMatcher::Builder::setDefaultLocale(
    #       const Locale *defaultLocale
    # )
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales(locales)
               .set_default_locale(Locale.get_german())
               .build())
    assert matcher.get_best_match(Locale("ja_JP")) == Locale("de")
    assert matcher.get_best_match("ja_JP") == Locale("de")

    # Builder &icu::LocaleMatcher::Builder::setDemotionPerDesiredLocale(
    #       ULocMatchDemotion demotion
    # )
    supported = [Locale("fr"), Locale("de-CH"), Locale("it")]
    desired = [Locale("fr-CH"), Locale("de-CH"), Locale("it")]
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales(supported)
               .set_demotion_per_desired_locale(
        ULocMatchDemotion.ULOCMATCH_DEMOTION_NONE)
               .build())

    # [2]
    # const Locale *icu::LocaleMatcher::getBestMatch(
    #       Locale::Iterator &desiredLocales,
    #       UErrorCode &errorCode
    # )
    assert matcher.get_best_match(desired) == Locale("de_CH")

    # Builder &icu::LocaleMatcher::Builder::setFavorSubtag(
    #       ULocMatchFavorSubtag subtag
    # )
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales(locales)
               .set_favor_subtag(ULocMatchFavorSubtag.ULOCMATCH_FAVOR_SCRIPT)
               .build())
    assert matcher.get_best_match(Locale("ja")) == Locale("fr")
    assert matcher.get_best_match("ja") == Locale("fr")

    # Builder &icu::LocaleMatcher::Builder::setSupportedLocalesFromListString(
    #       StringPiece locales
    # )
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales_from_list_string(
        " el, fr;q=0.555555, en-GB ; q = 0.88  , el; q =0, en;q=0.88 , fr ")
               .build())
    assert matcher.get_best_match_for_list_string(
        "el, fr, fr;q=0, en-GB") == Locale("en_GB")
    assert matcher.get_best_match(Locale("en_US")) == Locale("en")

    # UBool icu::LocaleMatcher::Builder::copyErrorTo(UErrorCode &outErrorCode)
    bld = LocaleMatcher.Builder().set_supported_locales(locales)
    bld.build()
    out_error_code = ErrorCode()
    assert not bld.copy_error_to(out_error_code)
    assert out_error_code.get() == UErrorCode.U_ZERO_ERROR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_builder_set_direction():
    from icupy.icu import ULocMatchDirection

    # Builder &icu::LocaleMatcher::Builder::setDirection(
    #       ULocMatchDirection direction
    # )
    supported = [Locale("ar"), Locale("nn")]
    desired = [Locale("arz-EG"), Locale("nb-DK")]
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales(supported)
               .set_direction(
        ULocMatchDirection.ULOCMATCH_DIRECTION_ONLY_TWO_WAY)
               .build())
    assert matcher.get_best_match(desired) == Locale("nn")


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_builder_set_max_distance():
    # Builder &icu::LocaleMatcher::Builder::setMaxDistance(
    #       const Locale &desired,
    #       const Locale &supported
    # )
    matcher = (LocaleMatcher.Builder()
               .set_max_distance(Locale("de-AT"), Locale.get_german())
               .build())
    assert matcher.is_match(Locale("de-LU"), Locale.get_german())
    assert not matcher.is_match(Locale("da"), Locale("no"))
    assert not matcher.is_match(Locale.get_chinese(),
                                Locale.get_traditional_chinese())

    matcher = (LocaleMatcher.Builder()
               .set_max_distance("de-AT", Locale("de"))
               .build())
    assert not matcher.is_match("da", Locale("no"))

    matcher = (LocaleMatcher.Builder()
               .set_max_distance(Locale("de-AT"), "de")
               .build())
    assert matcher.is_match(Locale("de-LU"), "de")

    matcher = (LocaleMatcher.Builder()
               .set_max_distance("de-AT", "de")
               .build())
    assert not matcher.is_match("zh", "zh_TW")


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_builder_set_no_default_locale():
    # Builder &icu::LocaleMatcher::Builder::setNoDefaultLocale()
    locales = [Locale("fr"), Locale("en_GB"), Locale("en")]
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales(locales)
               .set_no_default_locale()
               .build())
    assert matcher.get_best_match(Locale("en_GB")) == Locale("en_GB")
    assert matcher.get_best_match("en_GB") == Locale("en_GB")
    assert matcher.get_best_match(Locale("ja_JP")) is None
    assert matcher.get_best_match("ja_JP") is None


def test_get_best_match_result():
    locales = [Locale("fr"), Locale("en-GB")]
    matcher = (LocaleMatcher.Builder()
               .set_supported_locales(locales)
               .add_supported_locale(Locale.get_english())
               .set_default_locale(Locale.get_german())
               .build())

    # [1]
    # Result icu::LocaleMatcher::getBestMatchResult(
    #       const Locale &desiredLocale,
    #       UErrorCode &errorCode
    # )
    desired_locale = Locale("en_GB")
    result = matcher.get_best_match_result(desired_locale)
    assert result.get_desired_index() == 0
    assert result.get_desired_locale() == desired_locale
    assert result.get_supported_index() == 1
    assert result.get_supported_locale() == Locale("en_GB")
    resolved = result.make_resolved_locale()
    assert isinstance(resolved, Locale)
    assert resolved == desired_locale

    desired_locale = Locale("ja_JP")
    result = matcher.get_best_match_result(desired_locale)
    assert result.get_desired_index() == -1
    assert result.get_desired_locale() is None
    assert result.get_supported_index() == -1
    assert result.get_supported_locale() == Locale("de")
    assert result.make_resolved_locale() == Locale("de")

    # [2]
    # Result icu::LocaleMatcher::getBestMatchResult(
    #       Locale::Iterator &desiredLocales,
    #       UErrorCode &errorCode
    # )
    desired_locales = [Locale("ja-JP"), Locale("en-US")]
    result = matcher.get_best_match_result(desired_locales)
    assert result.get_desired_index() == 1
    assert result.get_desired_locale() == desired_locales[1]
    assert result.get_supported_index() == 2
    assert result.get_supported_locale() == Locale("en")
    assert result.make_resolved_locale() == Locale("en_US")

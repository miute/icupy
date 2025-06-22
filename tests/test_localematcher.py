import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 65:
    pytest.skip("ICU4C<65", allow_module_level=True)


def test_builder() -> None:
    locales = [icu.Locale("fr"), icu.Locale("en_GB"), icu.Locale("en")]

    # icu::LocaleMatcher::Builder::Builder()
    #
    # Builder &icu::LocaleMatcher::Builder::addSupportedLocale(
    #       const Locale &locale
    # )
    #
    # LocaleMatcher icu::LocaleMatcher::Builder::build(UErrorCode &errorCode)
    matcher = (
        icu.LocaleMatcher.Builder()
        .add_supported_locale(locales[0])
        .add_supported_locale("en_GB")
        .add_supported_locale(locales[2])
        .build()
    )
    assert isinstance(matcher, icu.LocaleMatcher)

    # [1]
    # const Locale *icu::LocaleMatcher::getBestMatch(
    #       const Locale &desiredLocale,
    #       UErrorCode &errorCode
    # )
    assert matcher.get_best_match(icu.Locale("en_US")) == icu.Locale("en")
    assert matcher.get_best_match("en_US") == icu.Locale("en")

    # [2]
    # Builder &icu::LocaleMatcher::Builder::setSupportedLocales(
    #       Locale::Iterator &locales
    # )
    #
    # Builder &icu::LocaleMatcher::Builder::setDefaultLocale(
    #       const Locale *defaultLocale
    # )
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales(locales)
        .set_default_locale(icu.Locale.get_german())
        .build()
    )
    assert matcher.get_best_match(icu.Locale("ja_JP")) == icu.Locale("de")
    assert matcher.get_best_match("ja_JP") == icu.Locale("de")

    # Builder &icu::LocaleMatcher::Builder::setDemotionPerDesiredLocale(
    #       ULocMatchDemotion demotion
    # )
    supported = [icu.Locale("fr"), icu.Locale("de-CH"), icu.Locale("it")]
    desired = [icu.Locale("fr-CH"), icu.Locale("de-CH"), icu.Locale("it")]
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales(supported)
        .set_demotion_per_desired_locale(icu.ULocMatchDemotion.ULOCMATCH_DEMOTION_NONE)
        .build()
    )

    # [2]
    # const Locale *icu::LocaleMatcher::getBestMatch(
    #       Locale::Iterator &desiredLocales,
    #       UErrorCode &errorCode
    # )
    assert matcher.get_best_match(desired) == icu.Locale("de_CH")

    # Builder &icu::LocaleMatcher::Builder::setFavorSubtag(
    #       ULocMatchFavorSubtag subtag
    # )
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales(locales)
        .set_favor_subtag(icu.ULocMatchFavorSubtag.ULOCMATCH_FAVOR_SCRIPT)
        .build()
    )
    assert matcher.get_best_match(icu.Locale("ja")) == icu.Locale("fr")
    assert matcher.get_best_match("ja") == icu.Locale("fr")

    # Builder &icu::LocaleMatcher::Builder::setSupportedLocalesFromListString(
    #       StringPiece locales
    # )
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales_from_list_string(
            " el, fr;q=0.555555, en-GB ; q = 0.88  , el; q =0, en;q=0.88 , fr "
        )
        .build()
    )
    assert matcher.get_best_match_for_list_string("el, fr, fr;q=0, en-GB") == icu.Locale(
        "en_GB"
    )
    assert matcher.get_best_match(icu.Locale("en_US")) == icu.Locale("en")

    # UBool icu::LocaleMatcher::Builder::copyErrorTo(UErrorCode &outErrorCode)
    bld = icu.LocaleMatcher.Builder().set_supported_locales(locales)
    bld.build()
    out_error_code = icu.ErrorCode()
    assert bld.copy_error_to(out_error_code) is False
    assert out_error_code.get() == icu.UErrorCode.U_ZERO_ERROR


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_builder_set_direction() -> None:
    # Builder &icu::LocaleMatcher::Builder::setDirection(
    #       ULocMatchDirection direction
    # )
    supported = [icu.Locale("ar"), icu.Locale("nn")]
    desired = [icu.Locale("arz-EG"), icu.Locale("nb-DK")]
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales(supported)
        .set_direction(icu.ULocMatchDirection.ULOCMATCH_DIRECTION_ONLY_TWO_WAY)
        .build()
    )
    assert matcher.get_best_match(desired) == icu.Locale("nn")


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_builder_set_max_distance() -> None:
    # Builder &icu::LocaleMatcher::Builder::setMaxDistance(
    #       const Locale &desired,
    #       const Locale &supported
    # )
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_max_distance(icu.Locale("de-AT"), icu.Locale.get_german())
        .build()
    )
    assert matcher.is_match(icu.Locale("de-LU"), icu.Locale.get_german()) is True
    assert matcher.is_match(icu.Locale("da"), icu.Locale("no")) is False
    assert (
        matcher.is_match(icu.Locale.get_chinese(), icu.Locale.get_traditional_chinese())
        is False
    )

    matcher = icu.LocaleMatcher.Builder().set_max_distance("de-AT", icu.Locale("de")).build()
    assert matcher.is_match("da", icu.Locale("no")) is False

    matcher = icu.LocaleMatcher.Builder().set_max_distance(icu.Locale("de-AT"), "de").build()
    assert matcher.is_match(icu.Locale("de-LU"), "de") is True

    matcher = icu.LocaleMatcher.Builder().set_max_distance("de-AT", "de").build()
    assert matcher.is_match("zh", "zh_TW") is False


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_builder_set_no_default_locale() -> None:
    # Builder &icu::LocaleMatcher::Builder::setNoDefaultLocale()
    locales = [icu.Locale("fr"), icu.Locale("en_GB"), icu.Locale("en")]
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales(locales)
        .set_no_default_locale()
        .build()
    )
    assert matcher.get_best_match(icu.Locale("en_GB")) == icu.Locale("en_GB")
    assert matcher.get_best_match("en_GB") == icu.Locale("en_GB")
    assert matcher.get_best_match(icu.Locale("ja_JP")) is None
    assert matcher.get_best_match("ja_JP") is None


def test_get_best_match_result() -> None:
    locales = [icu.Locale("fr"), icu.Locale("en-GB")]
    matcher = (
        icu.LocaleMatcher.Builder()
        .set_supported_locales(locales)
        .add_supported_locale(icu.Locale.get_english())
        .set_default_locale(icu.Locale.get_german())
        .build()
    )

    # [1]
    # Result icu::LocaleMatcher::getBestMatchResult(
    #       const Locale &desiredLocale,
    #       UErrorCode &errorCode
    # )
    desired_locale = icu.Locale("en_GB")
    result = matcher.get_best_match_result(desired_locale)
    assert isinstance(result, icu.LocaleMatcher.Result)
    assert result.get_desired_index() == 0
    assert result.get_desired_locale() == desired_locale
    assert result.get_supported_index() == 1
    assert result.get_supported_locale() == icu.Locale("en_GB")
    resolved = result.make_resolved_locale()
    assert isinstance(resolved, icu.Locale)
    assert resolved == desired_locale

    desired_locale = icu.Locale("ja_JP")
    result = matcher.get_best_match_result(desired_locale)
    assert isinstance(result, icu.LocaleMatcher.Result)
    assert result.get_desired_index() == -1
    assert result.get_desired_locale() is None
    assert result.get_supported_index() == -1
    assert result.get_supported_locale() == icu.Locale("de")
    assert result.make_resolved_locale() == icu.Locale("de")

    # [2]
    # Result icu::LocaleMatcher::getBestMatchResult(
    #       Locale::Iterator &desiredLocales,
    #       UErrorCode &errorCode
    # )
    desired_locales = [icu.Locale("ja-JP"), icu.Locale("en-US")]
    result = matcher.get_best_match_result(desired_locales)
    assert isinstance(result, icu.LocaleMatcher.Result)
    assert result.get_desired_index() == 1
    assert result.get_desired_locale() == desired_locales[1]
    assert result.get_supported_index() == 2
    assert result.get_supported_locale() == icu.Locale("en")
    assert result.make_resolved_locale() == icu.Locale("en_US")

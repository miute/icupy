import pytest

from icupy import icu


def test_api():
    assert icu.uscript_breaks_between_letters(icu.UScriptCode.USCRIPT_JAPANESE) is True
    assert icu.uscript_breaks_between_letters(icu.UScriptCode.USCRIPT_LATIN) is False

    # int32_t uscript_getCode(const char *nameOrAbbrOrLocale,
    #                         UScriptCode *fillIn,
    #                         int32_t capacity,
    #                         UErrorCode *err
    # )
    fill_in = icu.uscript_get_code("ja_JP")
    assert isinstance(fill_in, list)
    assert len(fill_in) == 3
    assert icu.UScriptCode.USCRIPT_HAN in fill_in
    assert icu.UScriptCode.USCRIPT_HIRAGANA in fill_in
    assert icu.UScriptCode.USCRIPT_KATAKANA in fill_in

    assert icu.uscript_get_name(icu.UScriptCode.USCRIPT_JAPANESE) == "Jpan"
    assert icu.uscript_get_name(icu.UScriptCode.USCRIPT_MALAYALAM) == "Malayalam"
    assert icu.uscript_get_name(icu.UScriptCode.USCRIPT_INVALID_CODE) is None

    # int32_t uscript_getSampleString(UScriptCode script,
    #                                 UChar *dest,
    #                                 int32_t capacity,
    #                                 UErrorCode *pErrorCode
    # )
    dest = icu.uscript_get_sample_string(icu.UScriptCode.USCRIPT_JAPANESE)
    assert isinstance(dest, str)
    assert len(dest) > 0

    result = icu.uscript_get_sample_unicode_string(icu.UScriptCode.USCRIPT_JAPANESE)
    assert isinstance(result, icu.UnicodeString)
    assert dest == result

    # UScriptCode uscript_getScript(UChar32 codepoint, UErrorCode *err)
    assert icu.uscript_get_script(0x0D02) == icu.UScriptCode.USCRIPT_MALAYALAM
    assert icu.uscript_get_script(0) == icu.UScriptCode.USCRIPT_COMMON
    with pytest.raises(icu.ICUError) as exc_info:
        _ = icu.uscript_get_script(-1)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # int32_t uscript_getScriptExtensions(UChar32 c,
    #                                     UScriptCode *scripts,
    #                                     int32_t capacity,
    #                                     UErrorCode *errorCode
    # )
    scripts = icu.uscript_get_script_extensions(0x3031)
    assert isinstance(scripts, list)
    assert len(scripts) == 2
    assert icu.UScriptCode.USCRIPT_HIRAGANA in scripts
    assert icu.UScriptCode.USCRIPT_KATAKANA in scripts

    scripts = icu.uscript_get_script_extensions(-1)
    assert isinstance(scripts, list)
    assert len(scripts) == 1
    assert icu.UScriptCode.USCRIPT_UNKNOWN in scripts

    assert icu.uscript_get_short_name(icu.UScriptCode.USCRIPT_JAPANESE) == "Jpan"
    assert icu.uscript_get_short_name(icu.UScriptCode.USCRIPT_MALAYALAM) == "Mlym"
    assert icu.uscript_get_short_name(icu.UScriptCode.USCRIPT_INVALID_CODE) is None

    assert (
        icu.uscript_get_usage(icu.UScriptCode.USCRIPT_HIRAGANA)
        == icu.UScriptUsage.USCRIPT_USAGE_RECOMMENDED
    )
    assert (
        icu.uscript_get_usage(icu.UScriptCode.USCRIPT_ADLAM)
        == icu.UScriptUsage.USCRIPT_USAGE_LIMITED_USE
    )

    assert icu.uscript_has_script(0x3031, icu.UScriptCode.USCRIPT_HIRAGANA) is True
    assert icu.uscript_has_script(0x3031, icu.UScriptCode.USCRIPT_HAN) is False

    assert icu.uscript_is_cased(icu.UScriptCode.USCRIPT_LATIN) is True
    assert icu.uscript_is_cased(icu.UScriptCode.USCRIPT_HIRAGANA) is False

    assert icu.uscript_is_right_to_left(icu.UScriptCode.USCRIPT_ARABIC) is True
    assert icu.uscript_is_right_to_left(icu.UScriptCode.USCRIPT_LATIN) is False

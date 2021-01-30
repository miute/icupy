import pytest

from icupy import (
    ICUException, UErrorCode, UnicodeString, UScriptCode, UScriptUsage,
    uscript_breaks_between_letters, uscript_get_code, uscript_get_name,
    uscript_get_sample_string, uscript_get_sample_unicode_string,
    uscript_get_script, uscript_get_script_extensions, uscript_get_short_name,
    uscript_get_usage, uscript_has_script, uscript_is_cased,
    uscript_is_right_to_left,
)


def test_api():
    assert uscript_breaks_between_letters(UScriptCode.USCRIPT_JAPANESE)
    assert not uscript_breaks_between_letters(UScriptCode.USCRIPT_LATIN)

    # int32_t uscript_getCode(const char *nameOrAbbrOrLocale,
    #                         UScriptCode *fillIn,
    #                         int32_t capacity,
    #                         UErrorCode *err
    # )
    fill_in = uscript_get_code("ja_JP")
    assert isinstance(fill_in, list)
    assert len(fill_in) == 3
    assert UScriptCode.USCRIPT_HAN in fill_in
    assert UScriptCode.USCRIPT_HIRAGANA in fill_in
    assert UScriptCode.USCRIPT_KATAKANA in fill_in

    assert uscript_get_name(UScriptCode.USCRIPT_JAPANESE) == "Jpan"
    assert uscript_get_name(UScriptCode.USCRIPT_MALAYALAM) == "Malayalam"
    assert uscript_get_name(UScriptCode.USCRIPT_INVALID_CODE) is None

    # int32_t uscript_getSampleString(UScriptCode script,
    #                                 UChar *dest,
    #                                 int32_t capacity,
    #                                 UErrorCode *pErrorCode
    # )
    dest = uscript_get_sample_string(UScriptCode.USCRIPT_JAPANESE)
    assert isinstance(dest, str)
    assert len(dest) > 0

    result = uscript_get_sample_unicode_string(UScriptCode.USCRIPT_JAPANESE)
    assert isinstance(result, UnicodeString)
    assert dest == result

    # UScriptCode uscript_getScript(UChar32 codepoint, UErrorCode *err)
    assert uscript_get_script(0x0d02) == UScriptCode.USCRIPT_MALAYALAM
    assert uscript_get_script(0) == UScriptCode.USCRIPT_COMMON
    with pytest.raises(ICUException) as exc_info:
        _ = uscript_get_script(-1)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # int32_t uscript_getScriptExtensions(UChar32 c,
    #                                     UScriptCode *scripts,
    #                                     int32_t capacity,
    #                                     UErrorCode *errorCode
    # )
    scripts = uscript_get_script_extensions(0x3031)
    assert isinstance(scripts, list)
    assert len(scripts) == 2
    assert UScriptCode.USCRIPT_HIRAGANA in scripts
    assert UScriptCode.USCRIPT_KATAKANA in scripts

    scripts = uscript_get_script_extensions(-1)
    assert isinstance(scripts, list)
    assert len(scripts) == 1
    assert UScriptCode.USCRIPT_UNKNOWN in scripts

    assert uscript_get_short_name(UScriptCode.USCRIPT_JAPANESE) == "Jpan"
    assert uscript_get_short_name(UScriptCode.USCRIPT_MALAYALAM) == "Mlym"
    assert uscript_get_short_name(UScriptCode.USCRIPT_INVALID_CODE) is None

    assert uscript_get_usage(
        UScriptCode.USCRIPT_HIRAGANA
    ) == UScriptUsage.USCRIPT_USAGE_RECOMMENDED
    assert uscript_get_usage(
        UScriptCode.USCRIPT_ADLAM
    ) == UScriptUsage.USCRIPT_USAGE_LIMITED_USE

    assert uscript_has_script(0x3031, UScriptCode.USCRIPT_HIRAGANA)
    assert not uscript_has_script(0x3031, UScriptCode.USCRIPT_HAN)

    assert uscript_is_cased(UScriptCode.USCRIPT_LATIN)
    assert not uscript_is_cased(UScriptCode.USCRIPT_HIRAGANA)

    assert uscript_is_right_to_left(UScriptCode.USCRIPT_ARABIC)
    assert not uscript_is_right_to_left(UScriptCode.USCRIPT_LATIN)

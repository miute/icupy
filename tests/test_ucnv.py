from functools import partial
from pathlib import Path
from typing import Any

import pytest

from icupy import icu
from icupy.utils import gc


def test_api() -> None:
    assert icu.ucnv_compare_names("UTF-8", "utf8") == 0
    assert icu.ucnv_compare_names("UTF-8", "utf80") != 0

    count = icu.ucnv_count_available()
    assert count > 0
    names = [icu.ucnv_get_available_name(n) for n in range(count)]

    name = icu.UnicodeString("ISO_2022", -1)
    name.append(icu.UCNV_LOCALE_OPTION_STRING, -1).append(icu.Locale.JAPANESE, -1)
    name.append(icu.UCNV_VERSION_OPTION_STRING, -1).append("4", -1)
    encoding = str(name)  # "ISO_2022,locale=ja,version=4"
    assert encoding in names

    count = icu.ucnv_count_aliases(encoding)
    assert count > 0
    aliases1 = [icu.ucnv_get_alias(encoding, n) for n in range(count)]
    assert "JIS8" in aliases1
    aliases2 = icu.ucnv_get_aliases(encoding)
    assert isinstance(aliases2, list)
    assert sorted(aliases1) == sorted(aliases2)

    count = icu.ucnv_count_standards()
    assert count > 0
    standards = [icu.ucnv_get_standard(n) for n in range(count)]
    assert "IANA" in standards

    # const char *ucnv_detectUnicodeSignature(const char *source,
    #                                         int32_t sourceLength,
    #                                         int32_t *signatureLength,
    #                                         UErrorCode *pErrorCode
    # )
    source = b"\xef\xbb\xbf\x41\x42\x43"
    result = icu.ucnv_detect_unicode_signature(source, len(source))
    assert result == "UTF-8"
    result = icu.ucnv_detect_unicode_signature(source, 2)
    assert result is None

    result = icu.ucnv_get_canonical_name("utf8", "IANA")
    assert result == "UTF-8"

    result = icu.ucnv_get_default_name()
    assert isinstance(result, str)
    assert len(result) > 0

    result = icu.ucnv_get_standard_name("ibm-943_P15A-2003", "IANA")
    assert result == "Shift_JIS"

    with gc(icu.ucnv_open("ibm-943"), icu.ucnv_close) as cnv:
        assert icu.ucnv_get_ccsid(cnv) == 943

        result = icu.ucnv_get_display_name(cnv, icu.Locale.ENGLISH)
        assert result == "ibm-943_P130-1999"

        # UnicodeString::UnicodeString(const char *src,
        #                              int32_t srcLength,
        #                              UConverter *cnv,
        #                              UErrorCode &errorCode
        # )
        sjis = b"\x61\x5b\x5c\xb1\x87\x54"
        utf8 = sjis.decode("cp932")
        test1 = icu.UnicodeString(sjis, -1, cnv)  # ibm-943 to utf-8
        source = test1.extract()
        assert isinstance(source, str)

        # void ucnv_fixFileSeparator(const UConverter *cnv,
        #                            UChar *source,
        #                            int32_t sourceLen
        # )
        result = icu.ucnv_fix_file_separator(cnv, source, icu.u_strlen(source))
        assert isinstance(result, str)
        assert source != utf8
        assert result == utf8

        # int32_t UnicodeString::extract(char *dest,
        #                                int32_t destCapacity,
        #                                UConverter *cnv,
        #                                UErrorCode &errorCode
        # )
        dest = test1.extract(cnv)  # utf-8 to ibm-943
        assert isinstance(dest, bytes)
        assert dest.decode("cp932") == utf8

        assert icu.ucnv_get_name(cnv) == "ibm-943_P130-1999"
        assert icu.ucnv_get_platform(cnv) == icu.UConverterPlatform.UCNV_IBM
        assert icu.ucnv_get_subst_chars(cnv) == b"\xfc\xfc"
        assert icu.ucnv_get_type(cnv) == icu.UConverterType.UCNV_MBCS

        uniset = icu.UnicodeSet()
        icu.ucnv_get_unicode_set(
            cnv, uniset.to_uset(), icu.UConverterUnicodeSet.UCNV_ROUNDTRIP_SET
        )
        assert uniset.size() > 0

        assert icu.ucnv_is_ambiguous(cnv) is True
        assert icu.ucnv_is_fixed_width(cnv) is False
        assert icu.ucnv_uses_fallback(cnv) is False
        icu.ucnv_set_fallback(cnv, True)
        assert icu.ucnv_uses_fallback(cnv) is True

        test2 = icu.UnicodeString("a\uff71b\U0001f338c", -1)
        dest = test2.extract(cnv)  # utf-8 to ibm-943
        # dest.decode("cp932") raises UnicodeDecodeError.
        assert dest == b"a\xb1b\xfc\xfcc"

        sub = b"?"
        icu.ucnv_set_subst_chars(cnv, sub, len(sub))
        assert icu.ucnv_get_subst_chars(cnv) == sub

        dest = test2.extract(cnv)  # utf-8 to ibm-943
        assert dest.decode("cp932") == "a\uff71b?c"

        sub2 = "<??>"
        icu.ucnv_set_subst_string(cnv, sub2, len(sub2))
        assert icu.ucnv_get_subst_chars(cnv) == sub2.encode()

        dest = test2.extract(cnv)  # utf-8 to ibm-943
        assert dest.decode("cp932") == "a\uff71b<??>c"

        icu.ucnv_reset(cnv)
        icu.ucnv_reset_from_unicode(cnv)
        icu.ucnv_reset_to_unicode(cnv)

    en = icu.ucnv_open_all_names()
    assert icu.uenum_count(en) > 0
    names2 = iter(partial(icu.uenum_next, en), None)
    assert encoding in names2
    icu.uenum_close(en)

    en = icu.ucnv_open_standard_names("ibm-943_P15A-2003", "IANA")
    assert icu.uenum_count(en) > 0
    standards2 = iter(partial(icu.uenum_next, en), None)
    assert "Shift_JIS" in standards2
    icu.uenum_close(en)

    with gc(
        icu.ucnv_open_ccsid(943, icu.UConverterPlatform.UCNV_IBM),
        icu.ucnv_close,
    ) as cnv:
        assert icu.ucnv_get_ccsid(cnv) == 943

        result = icu.ucnv_get_display_name(cnv, icu.Locale.ENGLISH)
        assert result == "ibm-943_P130-1999"

    icu.ucnv_flush_cache()


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 71, reason="ICU4C<71")
def test_clone() -> None:
    with gc(icu.ucnv_open("ibm-943c"), icu.ucnv_close) as cnv1:
        test1 = icu.UnicodeString("a\uff71b\U0001f338c", -1)

        context = icu.ConstVoidPtr(icu.UCNV_ESCAPE_XML_DEC)
        action = icu.UConverterFromUCallbackEscape(context)
        icu.ucnv_set_from_ucall_back(cnv1, action)

        # UConverter *ucnv_clone(
        #       const UConverter *cnv,
        #       UErrorCode *status
        # )
        with gc(icu.ucnv_clone(cnv1), icu.ucnv_close) as cnv2:
            dest = test1.extract(cnv1)  # utf-8 to ibm-943c
            assert dest == b"a\xb1b&#127800;c"

            dest = test1.extract(cnv2)  # utf-8 to ibm-943c
            assert dest == b"a\xb1b&#127800;c"


def test_open_package() -> None:
    path = Path(__file__).joinpath("..", "testdata").resolve()
    cnv = None
    try:
        cnv = icu.ucnv_open_package(str(path), "test3")
    except icu.ICUError as ex:
        if ex.args[0] != icu.UErrorCode.U_FILE_ACCESS_ERROR:
            raise
        pytest.skip(
            "testdata.dat is not found (not an error). "
            "You need to build a test data from the source. "
            "See also <icu4c>/icu/source/test/testdata/"
        )

    with gc(cnv, icu.ucnv_close) as cnv:
        assert icu.ucnv_get_name(cnv) == "test3"
        assert icu.ucnv_get_subst_chars(cnv) == b"\xff"


def test_set_from_u_call_back() -> None:
    """Set the custom From Unicode error callback function."""

    # typedef void( * UConverterFromUCallback) (
    #       const void *context,
    #       UConverterFromUnicodeArgs *args,
    #       const UChar *codeUnits,
    #       int32_t length,
    #       UChar32 codePoint,
    #       UConverterCallbackReason reason,
    #       UErrorCode *pErrorCode
    # )
    def _from_unicode_cb(
        _info: dict[str, Any],
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.ErrorCode,
    ) -> None:
        if not isinstance(_info, dict):
            _error_code.set(icu.U_ILLEGAL_ARGUMENT_ERROR)
            return
        if _reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
            _error_code.set(icu.U_ZERO_ERROR)
            _calls = _info.setdefault("numCalls", 0)
            _info["numCalls"] = _calls + 1
            _t = _info.setdefault("reason", [])
            _t.append((_code_units, _length, _code_point, _reason))
            _source = "".join([f"%u{ord(c):04X}" for c in _code_units])
            icu.ucnv_cb_from_uwrite_bytes(_args, _source, len(_source), 0)

    def _from_unicode_stop_cb(
        _info: dict[str, Any],
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.ErrorCode,
    ) -> None:
        _ = _args, _code_units, _length, _code_point
        assert isinstance(_info, dict)
        if _reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL]:
            _error_code.set(icu.U_INTERNAL_PROGRAM_ERROR)
            _calls = _info.setdefault("numCalls", 0)
            _info["numCalls"] = _calls + 1

    def _from_unicode_skip_cb(
        _info: object,
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.ErrorCode,
    ) -> None:
        _ = _args, _code_units, _length, _code_point
        assert _info is None
        if _reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL]:
            # do nothing
            _error_code.set(icu.U_ZERO_ERROR)

    # U+20AC: UCNV_UNASSIGNED
    # U+D800: UCNV_ILLEGAL
    s = icu.UnicodeString("A€\\uD800B").unescape()
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        # Default behavior
        icu.ucnv_set_subst_string(cnv, "[?]")
        assert s.extract(cnv) == b"A[?][?]B"

        # Get the default callback function
        action1 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action1, icu.UConverterFromUCallback)

        # Set a new callback function
        info: dict[str, Any] = {}
        context2 = icu.ConstVoidPtr(info)
        action2 = icu.UConverterFromUCallback(_from_unicode_cb, context2)
        assert action2.context().value() == info
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action2)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        assert s.extract(cnv) == b"A%u20AC%uD800B"
        assert info["numCalls"] > 0
        reason = info["reason"]
        assert reason[0] == ("€", 1, 0x20AC, icu.UCNV_UNASSIGNED)
        assert reason[1] == ("\ud800", 1, 0xD800, icu.UCNV_ILLEGAL)

        # Set a new callback function
        action3 = icu.UConverterFromUCallback(_from_unicode_stop_cb, context2)
        assert action3.context().value() == info
        old_action2 = icu.ucnv_set_from_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterFromUCallback)
        assert old_action2.context().value() == info

        info.clear()
        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_INTERNAL_PROGRAM_ERROR
        assert info["numCalls"] > 0

        # Set the old callback function
        icu.ucnv_set_from_ucall_back(cnv, old_action2)
        info.clear()
        assert s.extract(cnv) == b"A%u20AC%uD800B"
        assert info["numCalls"] > 0
        reason = info["reason"]
        assert reason[0] == ("€", 1, 0x20AC, icu.UCNV_UNASSIGNED)
        assert reason[1] == ("\ud800", 1, 0xD800, icu.UCNV_ILLEGAL)

        # Set the old callback function
        icu.ucnv_set_from_ucall_back(cnv, action1)
        info.clear()
        assert s.extract(cnv) == b"A[?][?]B"
        assert len(info) == 0

        # Set a new callback function
        action4 = icu.UConverterFromUCallback(_from_unicode_skip_cb)
        assert action4.context() is None
        icu.ucnv_set_from_ucall_back(cnv, action4)
        assert s.extract(cnv) == b"AB"


@pytest.mark.parametrize(
    ("option", "expected"),
    [
        (None, b"A%U20AC%UD800B"),
        (icu.UCNV_ESCAPE_ICU, b"A%U20AC%UD800B"),
        (icu.UCNV_ESCAPE_JAVA, b"A\\u20AC\\uD800B"),
        (icu.UCNV_ESCAPE_C, b"A\\u20AC\\uD800B"),
        (icu.UCNV_ESCAPE_XML_DEC, b"A&#8364;&#55296;B"),
        (icu.UCNV_ESCAPE_XML_HEX, b"A&#x20AC;&#xD800;B"),
    ],
)
def test_set_from_u_call_back_escape(option: str | None, expected: bytes) -> None:
    """Set the UCNV_FROM_U_CALLBACK_ESCAPE callback function."""

    # U+20AC: UCNV_UNASSIGNED
    # U+D800: UCNV_ILLEGAL
    s = icu.UnicodeString("A€\\uD800B").unescape()
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        context = icu.ConstVoidPtr(option)
        action = icu.UConverterFromUCallbackEscape(context)
        assert action.context() is None
        assert action.option() == option
        old_action = icu.ucnv_set_from_ucall_back(cnv, action)
        assert isinstance(old_action, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None
        assert action2.option() == option

        assert s.extract(cnv) == expected

    # Backward compatibility
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        context = icu.ConstVoidPtr(option)
        action = icu.UConverterFromUCallback(icu.UCNV_FROM_U_CALLBACK_ESCAPE, context)
        assert action.context().value() == option
        old_action = icu.ucnv_set_from_ucall_back(cnv, action)
        assert isinstance(old_action, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context().value() == option

        assert s.extract(cnv) == expected


def test_set_from_u_call_back_skip() -> None:
    """Set the UCNV_FROM_U_CALLBACK_SKIP callback function."""

    # U+20AC: UCNV_UNASSIGNED
    # U+D800: UCNV_ILLEGAL
    s = icu.UnicodeString("A€\\uD800B").unescape()
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterFromUCallbackSkip()
        assert action1.context() is None
        assert action1.option() is None
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None
        assert action2.option() is None

        assert s.extract(cnv) == b"AB"

        # Set a new callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        context3 = icu.ConstVoidPtr(icu.UCNV_SKIP_STOP_ON_ILLEGAL)
        action3 = icu.UConverterFromUCallbackSkip(context3)
        assert action3.context() is None
        assert action3.option() == icu.UCNV_SKIP_STOP_ON_ILLEGAL
        old_action2 = icu.ucnv_set_from_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterFromUCallback)
        assert old_action2.context() is None
        assert old_action2.option() is None

        action4 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterFromUCallback)
        assert action4.context() is None
        assert action4.option() == icu.UCNV_SKIP_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_from_ucall_back(cnv, old_action2)
        assert s.extract(cnv) == b"AB"

        # Set the old callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        icu.ucnv_set_from_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

    # Backward compatibility
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterFromUCallback(icu.UCNV_FROM_U_CALLBACK_SKIP)
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None

        assert s.extract(cnv) == b"AB"

        # Set a new callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        context3 = icu.ConstVoidPtr(icu.UCNV_SKIP_STOP_ON_ILLEGAL)
        action3 = icu.UConverterFromUCallback(icu.UCNV_FROM_U_CALLBACK_SKIP, context3)
        old_action2 = icu.ucnv_set_from_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterFromUCallback)
        assert old_action2.context() is None

        action4 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterFromUCallback)
        assert action4.context().value() == icu.UCNV_SKIP_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_from_ucall_back(cnv, old_action2)
        assert s.extract(cnv) == b"AB"

        # Set the old callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        icu.ucnv_set_from_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND


def test_set_from_u_call_back_stop() -> None:
    """Set the UCNV_FROM_U_CALLBACK_STOP callback function."""

    s = icu.UnicodeString("A€\\uD800B").unescape()
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        # U+20AC: UCNV_UNASSIGNED
        s = icu.UnicodeString("A€B").unescape()

        # Set a new callback function: option = None
        action1 = icu.UConverterFromUCallbackStop()
        assert action1.context() is None
        assert action1.option() is None
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None
        assert action2.option() is None

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_INVALID_CHAR_FOUND

        # U+D800: UCNV_ILLEGAL
        s = icu.UnicodeString("A\\uD800B").unescape()

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

    # Backward compatibility
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        # U+20AC: UCNV_UNASSIGNED
        s = icu.UnicodeString("A€B").unescape()

        action1 = icu.UConverterFromUCallback(icu.UCNV_FROM_U_CALLBACK_STOP)
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_INVALID_CHAR_FOUND

        # U+D800: UCNV_ILLEGAL
        s = icu.UnicodeString("A\\uD800B").unescape()

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND


def test_set_from_u_call_back_substitute() -> None:
    """Set the UCNV_FROM_U_CALLBACK_SUBSTITUTE callback function."""

    # U+20AC: UCNV_UNASSIGNED
    # U+D800: UCNV_ILLEGAL
    s = icu.UnicodeString("A€\\uD800B").unescape()
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        icu.ucnv_set_subst_chars(cnv, "?")

        # Set a new callback function: option = None
        action1 = icu.UConverterFromUCallbackSubstitute()
        assert action1.context() is None
        assert action1.option() is None
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None
        assert action2.option() is None

        assert s.extract(cnv) == b"A??B"

        # Set a new callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        context1 = icu.ConstVoidPtr(icu.UCNV_SUB_STOP_ON_ILLEGAL)
        action3 = icu.UConverterFromUCallbackSubstitute(context1)
        old_action2 = icu.ucnv_set_from_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterFromUCallback)
        assert old_action2.context() is None
        assert old_action2.option() is None

        action4 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterFromUCallback)
        assert action4.context() is None
        assert action4.option() == icu.UCNV_SUB_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_from_ucall_back(cnv, old_action2)
        assert s.extract(cnv) == b"A??B"

        # Set the old callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        icu.ucnv_set_from_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

    # Backward compatibility
    with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
        icu.ucnv_set_subst_chars(cnv, "?")

        # Set a new callback function: option = None
        action1 = icu.UConverterFromUCallback(icu.UCNV_FROM_U_CALLBACK_SUBSTITUTE)
        old_action1 = icu.ucnv_set_from_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterFromUCallback)

        action2 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterFromUCallback)
        assert action2.context() is None

        assert s.extract(cnv) == b"A??B"

        # Set a new callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        context3 = icu.ConstVoidPtr(icu.UCNV_SUB_STOP_ON_ILLEGAL)
        action3 = icu.UConverterFromUCallback(icu.UCNV_FROM_U_CALLBACK_SUBSTITUTE, context3)
        old_action2 = icu.ucnv_set_from_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterFromUCallback)
        assert old_action2.context() is None

        action4 = icu.ucnv_get_from_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterFromUCallback)
        assert action4.context().value() == icu.UCNV_SUB_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_from_ucall_back(cnv, old_action2)
        assert s.extract(cnv) == b"A??B"

        # Set the old callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        icu.ucnv_set_from_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            s.extract(cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND


def test_set_to_u_call_back() -> None:
    """Set the custom To Unicode error callback function."""

    # typedef void(* UConverterToUCallback)(
    #       const void *context,
    #       UConverterToUnicodeArgs *args,
    #       const char *codeUnits,
    #       int32_t length,
    #       UConverterCallbackReason reason,
    #       UErrorCode *pErrorCode
    # )
    def _to_unicode_cb(
        _info: dict[str, Any],
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.ErrorCode,
    ) -> None:
        if not isinstance(_info, dict):
            _error_code.set(icu.U_ILLEGAL_ARGUMENT_ERROR)
            return
        if _reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
            assert icu.ucnv_get_name(_args.converter).startswith("ibm-943")
            _error_code.set(icu.U_ZERO_ERROR)
            _calls = _info.setdefault("numCalls", 0)
            _info["numCalls"] = _calls + 1
            _t = _info.setdefault("reason", [])
            _t.append((_code_units, _length, _reason))
            _source = "".join([f"%{b:02X}" for b in _code_units])
            icu.ucnv_cb_to_uwrite_uchars(_args, _source, len(_source), 0)

    def _to_unicode_skip_cb(
        _info: object,
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.ErrorCode,
    ) -> None:
        _ = _args, _code_units, _length
        assert _info is None
        if _reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
            # do nothing
            _error_code.set(icu.U_ZERO_ERROR)

    # Invalid Shift-JIS characters
    src = (
        b"a"
        b"\xeb\x40"  # UCNV_UNASSIGNED
        b"\xfa\x30"  # UCNV_ILLEGAL
        b"b"
    )
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        # Default behavior
        assert icu.ucnv_get_subst_chars(cnv) == b"\xfc\xfc"
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\ufffd\x1a\x30b"

        # Get the default callback function
        action1 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action1, icu.UConverterToUCallback)

        # Set a new callback function
        info: dict[str, Any] = {}
        context2 = icu.ConstVoidPtr(info)
        action2 = icu.UConverterToUCallback(_to_unicode_cb, context2)
        assert action2.context().value() == info
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action2)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        assert len(info) == 0
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a%EB%40%FA\x30b"
        assert info["numCalls"] > 0
        reason = info["reason"]
        assert reason[0] == (b"\xeb\x40", 2, icu.UCNV_UNASSIGNED)
        assert reason[1] == (b"\xfa", 1, icu.UCNV_ILLEGAL)

        # Set the old callback function
        old_action2 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action2, icu.UConverterToUCallback)
        info.clear()
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\ufffd\x1a\x30b"
        assert len(info) == 0

        # Set a new callback function
        old_action3 = icu.ucnv_set_to_ucall_back(cnv, old_action2)
        assert isinstance(old_action3, icu.UConverterToUCallback)
        info.clear()
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a%EB%40%FA\x30b"
        assert info["numCalls"] > 0
        reason = info["reason"]
        assert reason[0] == (b"\xeb\x40", 2, icu.UCNV_UNASSIGNED)
        assert reason[1] == (b"\xfa", 1, icu.UCNV_ILLEGAL)

        # Set a new callback function
        action4 = icu.UConverterToUCallback(_to_unicode_skip_cb)
        assert action4.context() is None
        icu.ucnv_set_to_ucall_back(cnv, action4)
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\x30b"


@pytest.mark.parametrize(
    ("option", "expected"),
    [
        (None, "a%XEB%X40%XFA\x30b"),
        (icu.UCNV_ESCAPE_ICU, "a%XEB%X40%XFA\x30b"),
        (icu.UCNV_ESCAPE_JAVA, "a%XEB%X40%XFA\x30b"),
        (icu.UCNV_ESCAPE_C, "a\\xEB\\x40\\xFA\x30b"),
        (icu.UCNV_ESCAPE_XML_DEC, "a&#235;&#64;&#250;\x30b"),
        (icu.UCNV_ESCAPE_XML_HEX, "a&#xEB;&#x40;&#xFA;\x30b"),
        (icu.UCNV_ESCAPE_UNICODE, "a%XEB%X40%XFA\x30b"),
    ],
)
def test_set_to_u_call_back_escape(option: str | None, expected: str) -> None:
    """Set the UCNV_TO_U_CALLBACK_ESCAPE callback function."""

    # Invalid Shift-JIS characters
    src = (
        b"a"
        b"\xeb\x40"  # UCNV_UNASSIGNED
        b"\xfa\x30"  # UCNV_ILLEGAL
        b"b"
    )
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        context = icu.ConstVoidPtr(option)
        action = icu.UConverterToUCallbackEscape(context)
        assert action.context() is None
        assert action.option() == option
        old_action = icu.ucnv_set_to_ucall_back(cnv, action)
        assert isinstance(old_action, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None
        assert action2.option() == option

        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == expected

    # Backward compatibility
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        context = icu.ConstVoidPtr(option)
        action = icu.UConverterToUCallback(icu.UCNV_TO_U_CALLBACK_ESCAPE, context)
        assert action.context().value() == option
        old_action = icu.ucnv_set_to_ucall_back(cnv, action)
        assert isinstance(old_action, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context().value() == option

        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == expected


def test_set_to_u_call_back_skip() -> None:
    """Set the UCNV_TO_U_CALLBACK_SKIP callback function."""

    # Invalid Shift-JIS characters
    src = (
        b"a"
        b"\xeb\x40"  # UCNV_UNASSIGNED
        b"\xfa\x30"  # UCNV_ILLEGAL
        b"b"
    )
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterToUCallbackSkip()
        assert action1.context() is None
        assert action1.option() is None
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None
        assert action2.option() is None

        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\x30b"

        # Set a new callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        context3 = icu.ConstVoidPtr(icu.UCNV_SKIP_STOP_ON_ILLEGAL)
        action3 = icu.UConverterToUCallbackSkip(context3)
        assert action3.context() is None
        assert action3.option() == icu.UCNV_SKIP_STOP_ON_ILLEGAL
        old_action2 = icu.ucnv_set_to_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterToUCallback)
        assert old_action2.context() is None
        assert old_action2.option() is None

        action4 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterToUCallback)
        assert action4.context() is None
        assert action4.option() == icu.UCNV_SKIP_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_to_ucall_back(cnv, old_action2)
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\x30b"

        # Set the old callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        icu.ucnv_set_to_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

    # Backward compatibility
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterToUCallback(icu.UCNV_TO_U_CALLBACK_SKIP)
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None

        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\x30b"

        # Set a new callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        context3 = icu.ConstVoidPtr(icu.UCNV_SKIP_STOP_ON_ILLEGAL)
        action3 = icu.UConverterToUCallback(icu.UCNV_TO_U_CALLBACK_SKIP, context3)
        old_action2 = icu.ucnv_set_to_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterToUCallback)
        assert old_action2.context() is None

        action4 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterToUCallback)
        assert action4.context().value() == icu.UCNV_SKIP_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_to_ucall_back(cnv, old_action2)
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\x30b"

        # Set the old callback function: option = UCNV_SKIP_STOP_ON_ILLEGAL
        icu.ucnv_set_to_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND


def test_set_to_u_call_back_stop() -> None:
    """Set the UCNV_TO_U_CALLBACK_STOP callback function."""

    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterToUCallbackStop()
        assert action1.context() is None
        assert action1.option() is None
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None
        assert action2.option() is None

        src = (
            b"a"
            b"\xeb\x40"  # UCNV_UNASSIGNED
            b"b"
        )
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_INVALID_CHAR_FOUND

        src = (
            b"a"
            b"\xfa\x30"  # UCNV_ILLEGAL
            b"b"
        )
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

    # Backward compatibility
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        action1 = icu.UConverterToUCallback(icu.UCNV_TO_U_CALLBACK_STOP)
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None

        src = (
            b"a"
            b"\xeb\x40"  # UCNV_UNASSIGNED
            b"b"
        )
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_INVALID_CHAR_FOUND

        src = (
            b"a"
            b"\xfa\x30"  # UCNV_ILLEGAL
            b"b"
        )
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND


def test_set_to_u_call_back_substitute() -> None:
    """Set the UCNV_TO_U_CALLBACK_SUBSTITUTE callback function."""

    # Invalid Shift-JIS characters
    src = (
        b"a"
        b"\xeb\x40"  # UCNV_UNASSIGNED
        b"\xfa\x30"  # UCNV_ILLEGAL
        b"b"
    )
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterToUCallbackSubstitute()
        assert action1.context() is None
        assert action1.option() is None
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None
        assert action2.option() is None

        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\ufffd\x1a\x30b"

        # Set a new callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        context1 = icu.ConstVoidPtr(icu.UCNV_SUB_STOP_ON_ILLEGAL)
        action3 = icu.UConverterToUCallbackSubstitute(context1)
        old_action2 = icu.ucnv_set_to_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterToUCallback)
        assert old_action2.context() is None
        assert old_action2.option() is None

        action4 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterToUCallback)
        assert action4.context() is None
        assert action4.option() == icu.UCNV_SUB_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_to_ucall_back(cnv, old_action2)
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\ufffd\x1a\x30b"

        # Set the old callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        icu.ucnv_set_to_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

    # Backward compatibility
    with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
        # Set a new callback function: option = None
        action1 = icu.UConverterToUCallback(icu.UCNV_TO_U_CALLBACK_SUBSTITUTE)
        old_action1 = icu.ucnv_set_to_ucall_back(cnv, action1)
        assert isinstance(old_action1, icu.UConverterToUCallback)

        action2 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action2, icu.UConverterToUCallback)
        assert action2.context() is None

        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\ufffd\x1a\x30b"

        # Set a new callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        context3 = icu.ConstVoidPtr(icu.UCNV_SUB_STOP_ON_ILLEGAL)
        action3 = icu.UConverterToUCallback(icu.UCNV_TO_U_CALLBACK_SUBSTITUTE, context3)
        old_action2 = icu.ucnv_set_to_ucall_back(cnv, action3)
        assert isinstance(old_action2, icu.UConverterToUCallback)
        assert old_action2.context() is None

        action4 = icu.ucnv_get_to_ucall_back(cnv)
        assert isinstance(action4, icu.UConverterToUCallback)
        assert action4.context().value() == icu.UCNV_SUB_STOP_ON_ILLEGAL

        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

        # Set the old callback function: option = None
        icu.ucnv_set_to_ucall_back(cnv, old_action2)
        s = icu.UnicodeString(src, -1, cnv)
        assert str(s) == "a\ufffd\x1a\x30b"

        # Set the old callback function: option = UCNV_SUB_STOP_ON_ILLEGAL
        icu.ucnv_set_to_ucall_back(cnv, action4)
        with pytest.raises(icu.ICUError) as exc_info:
            _ = icu.UnicodeString(src, -1, cnv)
        assert exc_info.value.error_code == icu.U_ILLEGAL_CHAR_FOUND

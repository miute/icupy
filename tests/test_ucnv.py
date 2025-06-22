from functools import partial
from pathlib import Path

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

        sub = "<??>"
        icu.ucnv_set_subst_string(cnv, sub, len(sub))
        assert icu.ucnv_get_subst_chars(cnv) == sub.encode()

        dest = test2.extract(cnv)  # utf-8 to ibm-943
        assert dest.decode("cp932") == "a\uff71b<??>c"

        icu.ucnv_reset(cnv)
        icu.ucnv_reset_from_unicode(cnv)
        icu.ucnv_reset_to_unicode(cnv)

    en = icu.ucnv_open_all_names()
    assert icu.uenum_count(en) > 0
    names = iter(partial(icu.uenum_next, en), None)
    assert encoding in names
    icu.uenum_close(en)

    en = icu.ucnv_open_standard_names("ibm-943_P15A-2003", "IANA")
    assert icu.uenum_count(en) > 0
    standards = iter(partial(icu.uenum_next, en), None)
    assert "Shift_JIS" in standards
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
        icu.ucnv_set_from_ucall_back(cnv1, icu.UCNV_FROM_U_CALLBACK_ESCAPE, context)

        # UConverter *ucnv_clone(
        #       const UConverter *cnv,
        #       UErrorCode *status
        # )
        with gc(icu.ucnv_clone(cnv1), icu.ucnv_close) as cnv2:
            dest = test1.extract(cnv1)  # utf-8 to ibm-943c
            assert dest == b"a\xb1b&#127800;c"

            dest = test1.extract(cnv2)  # utf-8 to ibm-943c
            assert dest == b"a\xb1b&#127800;c"


def test_deprecated_api() -> None:
    def _from_u_callback1(
        _context: object,
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        if _reason == icu.UConverterCallbackReason.UCNV_UNASSIGNED:
            _source = "?"
            icu.ucnv_cb_from_u_write_bytes(_args, _source, len(_source), 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _from_u_callback2(
        _context: object,
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        if _reason == icu.UConverterCallbackReason.UCNV_UNASSIGNED:
            icu.ucnv_cb_from_u_write_sub(_args, 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _to_u_callback1(
        _context: object,
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        if _reason == icu.UConverterCallbackReason.UCNV_ILLEGAL:
            icu.ucnv_cb_to_u_write_sub(_args, 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _to_u_callback2(
        _context: object,
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        if _reason == icu.UConverterCallbackReason.UCNV_ILLEGAL:
            _source = "".join([f"\\x{x:02x}" for x in _code_units])
            icu.ucnv_cb_to_u_write_uchars(_args, _source, len(_source), 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    with gc(icu.ucnv_open("ibm-943c"), icu.ucnv_close) as cnv:
        test1 = icu.UnicodeString("a\uff71b\U0001f338c", -1)

        # ucnv_get_from_u_call_back() is deprecated
        with pytest.warns(RuntimeWarning):
            old_action1, old_context1 = icu.ucnv_get_from_u_call_back(cnv)

        # ucnv_cb_from_u_write_bytes() is deprecated
        action5 = icu.UConverterFromUCallbackPtr(_from_u_callback1)
        context5 = icu.ConstVoidPtr(None)
        icu.ucnv_set_from_ucall_back(cnv, action5, context5)
        with pytest.warns(RuntimeWarning):
            _ = test1.extract(cnv)  # utf-8 to ibm-943c

        # ucnv_cb_from_u_write_sub() is deprecated
        action6 = icu.UConverterFromUCallbackPtr(_from_u_callback2)
        context6 = icu.ConstVoidPtr("foo bar baz")
        icu.ucnv_set_from_ucall_back(cnv, action6, context6)
        with pytest.warns(RuntimeWarning):
            _ = test1.extract(cnv)  # utf-8 to ibm-943c

        # ucnv_set_from_u_call_back() is deprecated
        with pytest.warns(RuntimeWarning):
            icu.ucnv_set_from_u_call_back(cnv, old_action1, old_context1)

    with gc(icu.ucnv_open("utf-8"), icu.ucnv_close) as cnv:
        utf8 = (
            b"\x61"
            b"\xfe"  # Impossible bytes
            b"\xc0\xaf"  # Overlong sequences
            b"\xed\xa0\x80"  # Illegal code positions
            b"\x62"
        )

        # ucnv_get_to_u_call_back() is deprecated
        with pytest.warns(RuntimeWarning):
            old_action1, old_context1 = icu.ucnv_get_to_u_call_back(cnv)

        # ucnv_cb_to_u_write_sub() is deprecated
        action5 = icu.UConverterToUCallbackPtr(_to_u_callback1)
        context5 = icu.ConstVoidPtr(None)
        icu.ucnv_set_to_ucall_back(cnv, action5, context5)
        with pytest.warns(RuntimeWarning):
            _ = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8

        # ucnv_cb_to_u_write_uchars() is deprecated
        action6 = icu.UConverterToUCallbackPtr(_to_u_callback2)
        context6 = icu.ConstVoidPtr("foo bar baz")
        icu.ucnv_set_to_ucall_back(cnv, action6, context6)
        with pytest.warns(RuntimeWarning):
            _ = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8

        # ucnv_set_to_u_call_back() is deprecated
        with pytest.warns(RuntimeWarning):
            icu.ucnv_set_to_u_call_back(cnv, old_action1, old_context1)


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


def test_set_from_ucall_back() -> None:
    result1, result2, result3 = [], [], []

    # void (* UConverterFromUCallback)(const void *context,
    #                                  UConverterFromUnicodeArgs *args,
    #                                  const UChar *codeUnits,
    #                                  int32_t length,
    #                                  UChar32 codePoint,
    #                                  UConverterCallbackReason reason,
    #                                  UErrorCode *pErrorCode)
    def _from_u_callback1(
        _context: object,
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        assert _context is None
        nonlocal result1
        result1.append((_reason, _error_code, _code_units))
        assert icu.ucnv_get_name(_args.converter) == "ibm-943_P15A-2003"
        if _reason == icu.UConverterCallbackReason.UCNV_UNASSIGNED:
            _source = "?"
            icu.ucnv_cb_from_uwrite_bytes(_args, _source, len(_source), 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _from_u_callback2(
        _context: object,
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        assert isinstance(_context, str)
        assert _context == "foo bar baz"
        nonlocal result2
        result2.append((_reason, _error_code, _code_units))
        assert icu.ucnv_get_name(_args.converter) == "ibm-943_P15A-2003"
        if _reason == icu.UConverterCallbackReason.UCNV_UNASSIGNED:
            icu.ucnv_cb_from_uwrite_sub(_args, 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _from_u_callback3(
        _context: object,
        _args: icu.UConverterFromUnicodeArgs,
        _code_units: str,
        _length: int,
        _code_point: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        assert isinstance(_context, dict)
        assert _context["key"] == "foo bar baz"
        nonlocal result3
        result3.append((_reason, _error_code, _code_units))
        assert icu.ucnv_get_name(_args.converter) == "ibm-943_P15A-2003"
        if _reason == icu.UConverterCallbackReason.UCNV_UNASSIGNED:
            if _code_point >= 0xFFFF:
                _fmt = "\\U{:08x}"
            elif _code_point >= 0xFF:
                _fmt = "\\u{:04x}"
            else:
                _fmt = "\\x{:02x}"
            _source = _fmt.format(_code_point)
            icu.ucnv_cb_from_uwrite_bytes(_args, _source, len(_source), 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    with gc(icu.ucnv_open("ibm-943c"), icu.ucnv_close) as cnv:
        assert icu.ucnv_get_subst_chars(cnv) == b"\xfc\xfc"

        test1 = icu.UnicodeString("a\uff71b\U0001f338c", -1)

        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b\xfc\xfcc"

        # void ucnv_getFromUCallBack(const UConverter *converter,
        #                            UConverterFromUCallback *action,
        #                            const void **context
        # )
        old_action1, old_context1 = icu.ucnv_get_from_ucall_back(cnv)

        # void ucnv_setFromUCallBack(UConverter *converter,
        #                            UConverterFromUCallback newAction,
        #                            const void *newContext,
        #                            UConverterFromUCallback *oldAction,
        #                            const void **oldContext,
        #                            UErrorCode *err
        # )
        context2 = icu.ConstVoidPtr(None)
        icu.ucnv_set_from_ucall_back(cnv, icu.UCNV_FROM_U_CALLBACK_ESCAPE, context2)  # %UXXXX
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b%UD83C%UDF38c"

        context3 = icu.ConstVoidPtr(icu.UCNV_ESCAPE_C)
        old_action3, old_context3 = icu.ucnv_set_from_ucall_back(
            cnv, icu.UCNV_FROM_U_CALLBACK_ESCAPE, context3
        )  # \\uXXXX
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b\\U0001F338c"

        context4 = icu.ConstVoidPtr(icu.UCNV_ESCAPE_XML_HEX)
        icu.ucnv_set_from_ucall_back(cnv, icu.UCNV_FROM_U_CALLBACK_ESCAPE, context4)  # &#xXXXX;
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b&#x1F338;c"

        action5 = icu.UConverterFromUCallbackPtr(_from_u_callback1)
        context5 = icu.ConstVoidPtr(None)
        icu.ucnv_set_from_ucall_back(cnv, action5, context5)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result1) > 0
        assert dest == b"a\xb1b?c"

        action5a, context5a = icu.ucnv_get_from_ucall_back(cnv)

        action6 = icu.UConverterFromUCallbackPtr(_from_u_callback2)
        context6 = icu.ConstVoidPtr("foo bar baz")
        icu.ucnv_set_from_ucall_back(cnv, action6, context6)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result2) > 0
        assert dest == b"a\xb1b\xfc\xfcc"

        action7 = icu.UConverterFromUCallbackPtr(_from_u_callback3)
        d = {"key": "foo bar baz"}
        context7 = icu.ConstVoidPtr(d)
        old_action7, old_context7 = icu.ucnv_set_from_ucall_back(cnv, action7, context7)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result3) > 0
        assert dest == b"a\xb1b\\U0001f338c"

        icu.ucnv_set_from_ucall_back(cnv, old_action7, old_context7)
        result1.clear()
        result2.clear()
        result3.clear()
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result2) > 0
        assert dest == b"a\xb1b\xfc\xfcc"

        icu.ucnv_set_from_ucall_back(cnv, old_action3, old_context3)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b%UD83C%UDF38c"

        icu.ucnv_set_from_ucall_back(cnv, old_action1, old_context1)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b\xfc\xfcc"

        icu.ucnv_set_from_ucall_back(cnv, action5a, context5a)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result1) > 0
        assert dest == b"a\xb1b?c"


def test_set_to_ucall_back() -> None:
    result1, result2, result3 = [], [], []

    # void (* UConverterToUCallback)(const void *context,
    #                                UConverterToUnicodeArgs *args,
    #                                const char *codeUnits,
    #                                int32_t length,
    #                                UConverterCallbackReason reason,
    #                                UErrorCode *pErrorCode)
    def _to_u_callback1(
        _context: object,
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        assert _context is None
        nonlocal result1
        result1.append((_reason, _error_code, _code_units))
        assert icu.ucnv_get_name(_args.converter) == "UTF-8"
        if _reason == icu.UConverterCallbackReason.UCNV_ILLEGAL:
            icu.ucnv_cb_to_uwrite_sub(_args, 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _to_u_callback2(
        _context: object,
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        assert isinstance(_context, str)
        assert _context == "foo bar baz"
        nonlocal result2
        result2.append((_reason, _error_code, _code_units))
        assert icu.ucnv_get_name(_args.converter) == "UTF-8"
        if _reason == icu.UConverterCallbackReason.UCNV_ILLEGAL:
            _source = "".join([f"\\x{x:02x}" for x in _code_units])
            icu.ucnv_cb_to_uwrite_uchars(_args, _source, len(_source), 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    def _to_u_callback3(
        _context: object,
        _args: icu.UConverterToUnicodeArgs,
        _code_units: bytes,
        _length: int,
        _reason: icu.UConverterCallbackReason,
        _error_code: icu.UErrorCode,
    ) -> icu.UErrorCode:
        assert isinstance(_context, dict)
        assert _context["key"] == "foo bar baz"
        nonlocal result3
        result3.append((_reason, _error_code, _code_units))
        assert icu.ucnv_get_name(_args.converter) == "UTF-8"
        if _reason == icu.UConverterCallbackReason.UCNV_ILLEGAL:
            _source = "".join([f"%x{x:02X}" for x in _code_units])
            icu.ucnv_cb_to_uwrite_uchars(_args, _source, len(_source), 0)
            _error_code = icu.UErrorCode.U_ZERO_ERROR
        return _error_code

    with gc(icu.ucnv_open("utf-8"), icu.ucnv_close) as cnv:
        assert icu.ucnv_get_subst_chars(cnv) == b"\xef\xbf\xbd"

        # Illegal UTF-8 characters
        utf8 = (
            b"\x61"
            b"\xfe"  # Impossible bytes
            b"\xc0\xaf"  # Overlong sequences
            b"\xed\xa0\x80"  # Illegal code positions
            b"\x62"
        )

        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (
            dest.encode() == b"a"
            b"\xef\xbf\xbd"
            b"\xef\xbf\xbd\xef\xbf\xbd"
            b"\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"
            b"b"
        )

        # void ucnv_getToUCallBack(const UConverter *converter,
        #                          UConverterToUCallback *action,
        #                          const void **context
        # )
        old_action1, old_context1 = icu.ucnv_get_to_ucall_back(cnv)

        # void ucnv_setToUCallBack(UConverter *converter,
        #                          UConverterToUCallback newAction,
        #                          const void *newContext,
        #                          UConverterToUCallback *oldAction,
        #                          const void **oldContext,
        #                          UErrorCode *err
        # )
        context2 = icu.ConstVoidPtr(None)
        icu.ucnv_set_to_ucall_back(cnv, icu.UCNV_TO_U_CALLBACK_ESCAPE, context2)  # %XNN
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert dest == "a%XFE%XC0%XAF%XED%XA0%X80b"

        context3 = icu.ConstVoidPtr(icu.UCNV_ESCAPE_C)
        old_action3, old_context3 = icu.ucnv_set_to_ucall_back(
            cnv, icu.UCNV_TO_U_CALLBACK_ESCAPE, context3
        )  # \\xXX
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert dest == "a\\xFE\\xC0\\xAF\\xED\\xA0\\x80b"

        context4 = icu.ConstVoidPtr(icu.UCNV_ESCAPE_XML_HEX)
        icu.ucnv_set_to_ucall_back(cnv, icu.UCNV_TO_U_CALLBACK_ESCAPE, context4)  # &#xXX;
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert dest == "a&#xFE;&#xC0;&#xAF;&#xED;&#xA0;&#x80;b"

        action5 = icu.UConverterToUCallbackPtr(_to_u_callback1)
        context5 = icu.ConstVoidPtr(None)
        icu.ucnv_set_to_ucall_back(cnv, action5, context5)
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result1) > 0
        dest = test1.extract()
        assert (
            dest.encode() == b"a"
            b"\xef\xbf\xbd"
            b"\xef\xbf\xbd\xef\xbf\xbd"
            b"\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"
            b"b"
        )

        action6 = icu.UConverterToUCallbackPtr(_to_u_callback2)
        context6 = icu.ConstVoidPtr("foo bar baz")
        icu.ucnv_set_to_ucall_back(cnv, action6, context6)
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result2) > 0
        dest = test1.extract()
        assert dest == "a\\xfe\\xc0\\xaf\\xed\\xa0\\x80b"

        action6a, context6a = icu.ucnv_get_to_ucall_back(cnv)

        action7 = icu.UConverterToUCallbackPtr(_to_u_callback3)
        d = {"key": "foo bar baz"}
        context7 = icu.ConstVoidPtr(d)
        old_action7, old_context7 = icu.ucnv_set_to_ucall_back(cnv, action7, context7)
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result3) > 0
        dest = test1.extract()
        assert dest == "a%xFE%xC0%xAF%xED%xA0%x80b"

        icu.ucnv_set_to_ucall_back(cnv, old_action7, old_context7)
        result1.clear()
        result2.clear()
        result3.clear()
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result2) > 0
        dest = test1.extract()
        assert dest == "a\\xfe\\xc0\\xaf\\xed\\xa0\\x80b"

        icu.ucnv_set_to_ucall_back(cnv, old_action3, old_context3)
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert dest == "a%XFE%XC0%XAF%XED%XA0%X80b"

        icu.ucnv_set_to_ucall_back(cnv, old_action1, old_context1)
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (
            dest.encode() == b"a"
            b"\xef\xbf\xbd"
            b"\xef\xbf\xbd\xef\xbf\xbd"
            b"\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"
            b"b"
        )

        icu.ucnv_set_to_ucall_back(cnv, action6a, context6a)
        test1 = icu.UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result2) > 0
        dest = test1.extract()
        assert dest == "a\\xfe\\xc0\\xaf\\xed\\xa0\\x80b"

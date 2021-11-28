from functools import partial
from pathlib import Path

import pytest
from icupy.icu import (
    ConstVoidPtr, ICUError, Locale, UErrorCode, UnicodeSet, UnicodeString,
    u_strlen,
    # ucnv
    UCNV_LOCALE_OPTION_STRING, UCNV_VERSION_OPTION_STRING,
    UConverterPlatform, UConverterType, UConverterUnicodeSet,
    ucnv_close, ucnv_compare_names, ucnv_count_aliases, ucnv_count_available,
    ucnv_count_standards, ucnv_detect_unicode_signature,
    ucnv_fix_file_separator, ucnv_flush_cache,
    ucnv_get_alias, ucnv_get_aliases, ucnv_get_available_name,
    ucnv_get_canonical_name, ucnv_get_ccsid, ucnv_get_default_name,
    ucnv_get_display_name, ucnv_get_from_ucall_back, ucnv_get_name,
    ucnv_get_platform, ucnv_get_standard, ucnv_get_standard_name,
    ucnv_get_subst_chars, ucnv_get_to_ucall_back, ucnv_get_type,
    ucnv_get_unicode_set,
    ucnv_is_ambiguous, ucnv_is_fixed_width,
    ucnv_open, ucnv_open_all_names, ucnv_open_ccsid, ucnv_open_package,
    ucnv_open_standard_names, ucnv_reset,
    ucnv_reset_from_unicode, ucnv_reset_to_unicode,
    ucnv_set_fallback, ucnv_set_from_ucall_back, ucnv_set_subst_chars,
    ucnv_set_subst_string, ucnv_set_to_ucall_back, ucnv_uses_fallback,
    # ucnv_err
    UCNV_ESCAPE_C, UCNV_ESCAPE_XML_HEX,
    UCNV_FROM_U_CALLBACK_ESCAPE, UCNV_TO_U_CALLBACK_ESCAPE,
    UConverterCallbackReason,
    UConverterFromUCallbackPtr, UConverterFromUnicodeArgs,
    UConverterToUCallbackPtr, UConverterToUnicodeArgs,
    # ucnv_cb
    ucnv_cb_from_u_write_bytes, ucnv_cb_from_u_write_sub,
    ucnv_cb_to_u_write_sub, ucnv_cb_to_u_write_uchars,
    # uenum
    uenum_close, uenum_count, uenum_next,
)
from icupy.utils import gc


def test_api():
    assert ucnv_compare_names("UTF-8", "utf8") == 0
    assert ucnv_compare_names("UTF-8", "utf80") != 0

    count = ucnv_count_available()
    assert count > 0
    names = [ucnv_get_available_name(n) for n in range(count)]

    name = UnicodeString("ISO_2022", -1)
    name.append(UCNV_LOCALE_OPTION_STRING, -1).append(Locale.JAPANESE, -1)
    name.append(UCNV_VERSION_OPTION_STRING, -1).append("4", -1)
    encoding = str(name)  # "ISO_2022,locale=ja,version=4"
    assert encoding in names

    count = ucnv_count_aliases(encoding)
    assert count > 0
    aliases1 = [ucnv_get_alias(encoding, n) for n in range(count)]
    assert "JIS8" in aliases1
    aliases2 = ucnv_get_aliases(encoding)
    assert isinstance(aliases2, list)
    assert sorted(aliases1) == sorted(aliases2)

    count = ucnv_count_standards()
    assert count > 0
    standards = [ucnv_get_standard(n) for n in range(count)]
    assert "IANA" in standards

    # const char *ucnv_detectUnicodeSignature(const char *source,
    #                                         int32_t sourceLength,
    #                                         int32_t *signatureLength,
    #                                         UErrorCode *pErrorCode
    # )
    source = b"\xEF\xBB\xBF\x41\x42\x43"
    result = ucnv_detect_unicode_signature(source, len(source))
    assert result == "UTF-8"
    result = ucnv_detect_unicode_signature(source, 2)
    assert result is None

    result = ucnv_get_canonical_name("utf8", "IANA")
    assert result == "UTF-8"

    result = ucnv_get_default_name()
    assert isinstance(result, str)
    assert len(result) > 0

    result = ucnv_get_standard_name("ibm-943_P15A-2003", "IANA")
    assert result == "Shift_JIS"

    with gc(ucnv_open("ibm-943"), ucnv_close) as cnv:
        assert ucnv_get_ccsid(cnv) == 943

        result = ucnv_get_display_name(cnv, Locale.ENGLISH)
        assert result == "ibm-943_P130-1999"

        # UnicodeString::UnicodeString(const char *src,
        #                              int32_t srcLength,
        #                              UConverter *cnv,
        #                              UErrorCode &errorCode
        # )
        sjis = b"\x61\x5B\x5C\xB1\x87\x54"
        utf8 = sjis.decode("cp932")
        test1 = UnicodeString(sjis, -1, cnv)  # ibm-943 to utf-8
        source = test1.extract()
        assert isinstance(source, str)

        # void ucnv_fixFileSeparator(const UConverter *cnv,
        #                            UChar *source,
        #                            int32_t sourceLen
        # )
        result = ucnv_fix_file_separator(cnv, source, u_strlen(source))
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

        assert ucnv_get_name(cnv) == "ibm-943_P130-1999"
        assert ucnv_get_platform(cnv) == UConverterPlatform.UCNV_IBM
        assert ucnv_get_subst_chars(cnv) == b"\xFC\xFC"
        assert ucnv_get_type(cnv) == UConverterType.UCNV_MBCS

        uniset = UnicodeSet()
        ucnv_get_unicode_set(cnv,
                             uniset.to_uset(),
                             UConverterUnicodeSet.UCNV_ROUNDTRIP_SET)
        assert uniset.size() > 0

        assert ucnv_is_ambiguous(cnv)
        assert not ucnv_is_fixed_width(cnv)
        assert not ucnv_uses_fallback(cnv)
        ucnv_set_fallback(cnv, True)
        assert ucnv_uses_fallback(cnv)

        test2 = UnicodeString("a\uFF71b\U0001f338c", -1)
        dest = test2.extract(cnv)  # utf-8 to ibm-943
        # dest.decode("cp932") raises UnicodeDecodeError.
        assert dest == b"a\xB1b\xFC\xFCc"

        sub = b"?"
        ucnv_set_subst_chars(cnv, sub, len(sub))
        assert ucnv_get_subst_chars(cnv) == sub

        dest = test2.extract(cnv)  # utf-8 to ibm-943
        assert dest.decode("cp932") == "a\uFF71b?c"

        sub = "<??>"
        ucnv_set_subst_string(cnv, sub, len(sub))
        assert ucnv_get_subst_chars(cnv) == sub.encode()

        dest = test2.extract(cnv)  # utf-8 to ibm-943
        assert dest.decode("cp932") == "a\uFF71b<??>c"

        ucnv_reset(cnv)
        ucnv_reset_from_unicode(cnv)
        ucnv_reset_to_unicode(cnv)

    en = ucnv_open_all_names()
    assert uenum_count(en) > 0
    names = iter(partial(uenum_next, en), None)
    assert encoding in names
    uenum_close(en)

    en = ucnv_open_standard_names("ibm-943_P15A-2003", "IANA")
    assert uenum_count(en) > 0
    standards = iter(partial(uenum_next, en), None)
    assert "Shift_JIS" in standards
    uenum_close(en)

    with gc(ucnv_open_ccsid(943, UConverterPlatform.UCNV_IBM),
            ucnv_close) as cnv:
        assert ucnv_get_ccsid(cnv) == 943

        result = ucnv_get_display_name(cnv, Locale.ENGLISH)
        assert result == "ibm-943_P130-1999"

    ucnv_flush_cache()


def test_open_package():
    path = Path(__file__).joinpath("..", "testdata").resolve()
    cnv = None
    try:
        cnv = ucnv_open_package(str(path), "test3")
    except ICUError as ex:
        if ex.args[0] != UErrorCode.U_FILE_ACCESS_ERROR:
            raise
        pytest.skip("testdata.dat is not found (not an error). "
                    "You need to build a test data from the source. "
                    "See also <icu4c>/icu/source/test/testdata/")

    with gc(cnv, ucnv_close) as cnv:
        assert ucnv_get_name(cnv) == "test3"
        assert ucnv_get_subst_chars(cnv) == b"\xFF"


def test_set_from_ucall_back():
    result1, result2, result3 = [], [], []

    # void (* UConverterFromUCallback)(const void *context,
    #                                  UConverterFromUnicodeArgs *args,
    #                                  const UChar *codeUnits,
    #                                  int32_t length,
    #                                  UChar32 codePoint,
    #                                  UConverterCallbackReason reason,
    #                                  UErrorCode *pErrorCode)
    def _from_u_callback1(_context: object, _args: UConverterFromUnicodeArgs,
                          _code_units: str, _length: int, _code_point: int,
                          _reason: UConverterCallbackReason,
                          _error_code: UErrorCode) -> UErrorCode:
        assert _context is None
        nonlocal result1
        result1.append((_reason, _error_code, _code_units))
        assert ucnv_get_name(_args.converter) == "ibm-943_P15A-2003"
        if _reason == UConverterCallbackReason.UCNV_UNASSIGNED:
            _source = "?"
            ucnv_cb_from_u_write_bytes(_args, _source, len(_source), 0)
            _error_code = UErrorCode.U_ZERO_ERROR
        return _error_code

    def _from_u_callback2(_context: object, _args: UConverterFromUnicodeArgs,
                          _code_units: str, _length: int, _code_point: int,
                          _reason: UConverterCallbackReason,
                          _error_code: UErrorCode) -> UErrorCode:
        assert isinstance(_context, str)
        assert _context == "foo bar baz"
        nonlocal result2
        result2.append((_reason, _error_code, _code_units))
        assert ucnv_get_name(_args.converter) == "ibm-943_P15A-2003"
        if _reason == UConverterCallbackReason.UCNV_UNASSIGNED:
            ucnv_cb_from_u_write_sub(_args, 0)
            _error_code = UErrorCode.U_ZERO_ERROR
        return _error_code

    def _from_u_callback3(_context: object, _args: UConverterFromUnicodeArgs,
                          _code_units: str, _length: int, _code_point: int,
                          _reason: UConverterCallbackReason,
                          _error_code: UErrorCode) -> UErrorCode:
        assert isinstance(_context, dict)
        assert _context["key"] == "foo bar baz"
        nonlocal result3
        result3.append((_reason, _error_code, _code_units))
        assert ucnv_get_name(_args.converter) == "ibm-943_P15A-2003"
        if _reason == UConverterCallbackReason.UCNV_UNASSIGNED:
            if _code_point >= 0xffff:
                _fmt = "\\U{:08x}"
            elif _code_point >= 0xff:
                _fmt = "\\u{:04x}"
            else:
                _fmt = "\\x{:02x}"
            _source = _fmt.format(_code_point)
            ucnv_cb_from_u_write_bytes(_args, _source, len(_source), 0)
            _error_code = UErrorCode.U_ZERO_ERROR
        return _error_code

    with gc(ucnv_open("ibm-943c"), ucnv_close) as cnv:
        assert ucnv_get_subst_chars(cnv) == b"\xfc\xfc"

        test1 = UnicodeString("a\uFF71b\U0001f338c", -1)

        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b\xfc\xfcc"

        # void ucnv_getFromUCallBack(const UConverter *converter,
        #                            UConverterFromUCallback *action,
        #                            const void **context
        # )
        old_action1, old_context1 = ucnv_get_from_ucall_back(cnv)

        # void ucnv_setFromUCallBack(UConverter *converter,
        #                            UConverterFromUCallback newAction,
        #                            const void *newContext,
        #                            UConverterFromUCallback *oldAction,
        #                            const void **oldContext,
        #                            UErrorCode *err
        # )
        context2 = ConstVoidPtr(None)
        ucnv_set_from_ucall_back(
            cnv,
            UCNV_FROM_U_CALLBACK_ESCAPE,
            context2)  # %UXXXX
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b%UD83C%UDF38c"

        context3 = ConstVoidPtr(UCNV_ESCAPE_C)
        old_action3, old_context3 = ucnv_set_from_ucall_back(
            cnv,
            UCNV_FROM_U_CALLBACK_ESCAPE,
            context3)  # \\uXXXX
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b\\U0001F338c"

        context4 = ConstVoidPtr(UCNV_ESCAPE_XML_HEX)
        ucnv_set_from_ucall_back(
            cnv,
            UCNV_FROM_U_CALLBACK_ESCAPE,
            context4)  # &#xXXXX;
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b&#x1F338;c"

        action5 = UConverterFromUCallbackPtr(_from_u_callback1)
        context5 = ConstVoidPtr(None)
        ucnv_set_from_ucall_back(
            cnv,
            action5,
            context5)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result1) > 0
        assert dest == b"a\xb1b?c"

        action6 = UConverterFromUCallbackPtr(_from_u_callback2)
        context6 = ConstVoidPtr("foo bar baz")
        ucnv_set_from_ucall_back(
            cnv,
            action6,
            context6)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result2) > 0
        assert dest == b"a\xb1b\xfc\xfcc"

        action7 = UConverterFromUCallbackPtr(_from_u_callback3)
        d = {"key": "foo bar baz"}
        context7 = ConstVoidPtr(d)
        old_action7, old_context7 = ucnv_set_from_ucall_back(
            cnv,
            action7,
            context7)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result3) > 0
        assert dest == b"a\xb1b\\U0001f338c"

        ucnv_set_from_ucall_back(
            cnv,
            old_action7,
            old_context7)
        result1.clear()
        result2.clear()
        result3.clear()
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert len(result2) > 0
        assert dest == b"a\xb1b\xfc\xfcc"

        ucnv_set_from_ucall_back(
            cnv,
            old_action3,
            old_context3)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b%UD83C%UDF38c"

        ucnv_set_from_ucall_back(
            cnv,
            old_action1,
            old_context1)
        dest = test1.extract(cnv)  # utf-8 to ibm-943c
        assert dest == b"a\xb1b\xfc\xfcc"


def test_set_to_ucall_back():
    result1, result2, result3 = [], [], []

    # void (* UConverterToUCallback)(const void *context,
    #                                UConverterToUnicodeArgs *args,
    #                                const char *codeUnits,
    #                                int32_t length,
    #                                UConverterCallbackReason reason,
    #                                UErrorCode *pErrorCode)
    def _to_u_callback1(_context: object, _args: UConverterToUnicodeArgs,
                        _code_units: bytes, _length: int,
                        _reason: UConverterCallbackReason,
                        _error_code: UErrorCode) -> UErrorCode:
        assert _context is None
        nonlocal result1
        result1.append((_reason, _error_code, _code_units))
        assert ucnv_get_name(_args.converter) == "UTF-8"
        if _reason == UConverterCallbackReason.UCNV_ILLEGAL:
            ucnv_cb_to_u_write_sub(_args, 0)
            _error_code = UErrorCode.U_ZERO_ERROR
        return _error_code

    def _to_u_callback2(_context: object, _args: UConverterToUnicodeArgs,
                        _code_units: bytes, _length: int,
                        _reason: UConverterCallbackReason,
                        _error_code: UErrorCode) -> UErrorCode:
        assert isinstance(_context, str)
        assert _context == "foo bar baz"
        nonlocal result2
        result2.append((_reason, _error_code, _code_units))
        assert ucnv_get_name(_args.converter) == "UTF-8"
        if _reason == UConverterCallbackReason.UCNV_ILLEGAL:
            _source = "".join(["\\x{:02x}".format(x) for x in _code_units])
            ucnv_cb_to_u_write_uchars(_args, _source, len(_source), 0)
            _error_code = UErrorCode.U_ZERO_ERROR
        return _error_code

    def _to_u_callback3(_context: object, _args: UConverterToUnicodeArgs,
                        _code_units: bytes, _length: int,
                        _reason: UConverterCallbackReason,
                        _error_code: UErrorCode) -> UErrorCode:
        assert isinstance(_context, dict)
        assert _context["key"] == "foo bar baz"
        nonlocal result3
        result3.append((_reason, _error_code, _code_units))
        assert ucnv_get_name(_args.converter) == "UTF-8"
        if _reason == UConverterCallbackReason.UCNV_ILLEGAL:
            _source = "".join(["%x{:02X}".format(x) for x in _code_units])
            ucnv_cb_to_u_write_uchars(_args, _source, len(_source), 0)
            _error_code = UErrorCode.U_ZERO_ERROR
        return _error_code

    with gc(ucnv_open("utf-8"), ucnv_close) as cnv:
        assert ucnv_get_subst_chars(cnv) == b"\xef\xbf\xbd"

        # Illegal UTF-8 characters
        utf8 = (b"\x61"
                b"\xfe"  # Impossible bytes
                b"\xc0\xaf"  # Overlong sequences
                b"\xed\xa0\x80"  # Illegal code positions
                b"\x62")

        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (dest.encode() == b"a"
                                 b"\xef\xbf\xbd"
                                 b"\xef\xbf\xbd\xef\xbf\xbd"
                                 b"\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"
                                 b"b")

        # void ucnv_getToUCallBack(const UConverter *converter,
        #                          UConverterToUCallback *action,
        #                          const void **context
        # )
        old_action1, old_context1 = ucnv_get_to_ucall_back(cnv)

        # void ucnv_setToUCallBack(UConverter *converter,
        #                          UConverterToUCallback newAction,
        #                          const void *newContext,
        #                          UConverterToUCallback *oldAction,
        #                          const void **oldContext,
        #                          UErrorCode *err
        # )
        context2 = ConstVoidPtr(None)
        ucnv_set_to_ucall_back(
            cnv,
            UCNV_TO_U_CALLBACK_ESCAPE,
            context2)  # %XNN
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (dest == "a"
                        "%XFE"
                        "%XC0%XAF"
                        "%XED%XA0%X80"
                        "b")

        context3 = ConstVoidPtr(UCNV_ESCAPE_C)
        old_action3, old_context3 = ucnv_set_to_ucall_back(
            cnv,
            UCNV_TO_U_CALLBACK_ESCAPE,
            context3)  # \\xXX
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (dest == "a"
                        "\\xFE"
                        "\\xC0\\xAF"
                        "\\xED\\xA0\\x80"
                        "b")

        context4 = ConstVoidPtr(UCNV_ESCAPE_XML_HEX)
        ucnv_set_to_ucall_back(
            cnv,
            UCNV_TO_U_CALLBACK_ESCAPE,
            context4)  # &#xXX;
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (dest == "a"
                        "&#xFE;"
                        "&#xC0;&#xAF;"
                        "&#xED;&#xA0;&#x80;"
                        "b")

        action5 = UConverterToUCallbackPtr(_to_u_callback1)
        context5 = ConstVoidPtr(None)
        ucnv_set_to_ucall_back(
            cnv,
            action5,
            context5)
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result1) > 0
        dest = test1.extract()
        assert (dest.encode() == b"a"
                                 b"\xef\xbf\xbd"
                                 b"\xef\xbf\xbd\xef\xbf\xbd"
                                 b"\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"
                                 b"b")

        action6 = UConverterToUCallbackPtr(_to_u_callback2)
        context6 = ConstVoidPtr("foo bar baz")
        ucnv_set_to_ucall_back(
            cnv,
            action6,
            context6)
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result2) > 0
        dest = test1.extract()
        assert (dest == "a"
                        "\\xfe"
                        "\\xc0\\xaf"
                        "\\xed\\xa0\\x80"
                        "b")

        action7 = UConverterToUCallbackPtr(_to_u_callback3)
        d = {"key": "foo bar baz"}
        context7 = ConstVoidPtr(d)
        old_action7, old_context7 = ucnv_set_to_ucall_back(
            cnv,
            action7,
            context7)
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result3) > 0
        dest = test1.extract()
        assert (dest == "a"
                        "%xFE"
                        "%xC0%xAF"
                        "%xED%xA0%x80"
                        "b")

        ucnv_set_to_ucall_back(
            cnv,
            old_action7,
            old_context7)
        result1.clear()
        result2.clear()
        result3.clear()
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        assert len(result2) > 0
        dest = test1.extract()
        assert (dest == "a"
                        "\\xfe"
                        "\\xc0\\xaf"
                        "\\xed\\xa0\\x80"
                        "b")

        ucnv_set_to_ucall_back(
            cnv,
            old_action3,
            old_context3)
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (dest == "a"
                        "%XFE"
                        "%XC0%XAF"
                        "%XED%XA0%X80"
                        "b")

        ucnv_set_to_ucall_back(
            cnv,
            old_action1,
            old_context1)
        test1 = UnicodeString(utf8, -1, cnv)  # utf-8 to utf-8
        dest = test1.extract()
        assert (dest.encode() == b"a"
                                 b"\xef\xbf\xbd"
                                 b"\xef\xbf\xbd\xef\xbf\xbd"
                                 b"\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd"
                                 b"b")

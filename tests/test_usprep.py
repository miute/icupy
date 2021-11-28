from pathlib import Path

import pytest
from icupy.icu import (
    ICUError, UErrorCode, UParseError, USPREP_ALLOW_UNASSIGNED,
    USPREP_DEFAULT, UStringPrepProfileType,
    u_unescape, usprep_close, usprep_open, usprep_open_by_type,
    usprep_prepare,
)
from icupy.utils import gc


def test_open():
    path = Path(__file__).resolve().parent / "testdata"
    try:
        # UStringPrepProfile *usprep_open(
        #       const char *path,
        #       const char *fileName,
        #       UErrorCode *status
        # )
        #
        # void usprep_close(UStringPrepProfile *profile)
        with gc(usprep_open(str(path), "nfscis"), usprep_close) as prep:
            _ = usprep_prepare(prep, "", 0, USPREP_DEFAULT, None)
    except ICUError as ex:
        if ex.args[0] != UErrorCode.U_FILE_ACCESS_ERROR:
            raise
        pytest.skip("testdata.dat is not found (not an error). "
                    "You need to build a test data from the source. "
                    "See also <icu4c>/icu/source/test/testdata/")


def test_open_by_type():
    # From icu/source/test/cintltst/spreptst.c

    # UStringPrepProfile *usprep_openByType(
    #       UStringPrepProfileType type,
    #       UErrorCode *status
    # )
    #
    # void usprep_close(UStringPrepProfile *profile)
    with gc(usprep_open_by_type(
            UStringPrepProfileType.USPREP_RFC4013_SASLPREP),
            usprep_close) as prep:
        # int32_t usprep_prepare(
        #       const UStringPrepProfile *prep,
        #       const UChar *src,
        #       int32_t srcLength,
        #       UChar *dest,
        #       int32_t destCapacity,
        #       int32_t options,
        #       UParseError *parseError,
        #       UErrorCode *status
        # )
        src = u_unescape("user:\\u00A0\\u0AC6\\u1680\\u00ADpassword1")
        expected = u_unescape("user: \\u0AC6 password1")
        parse_error = UParseError()
        dest = usprep_prepare(
            prep,
            src,
            len(src),
            USPREP_ALLOW_UNASSIGNED,
            parse_error)
        assert isinstance(dest, str)
        assert dest == expected

    with gc(usprep_open_by_type(UStringPrepProfileType.USPREP_RFC4011_MIB),
            usprep_close) as prep:
        src = u_unescape("Policy\\u034F\\u200DBase\\u0020d\\u1806\\u200C")
        expected = u_unescape("PolicyBase d")
        dest = usprep_prepare(
            prep,
            src,
            -1,
            USPREP_ALLOW_UNASSIGNED,
            None)
        assert isinstance(dest, str)
        assert dest == expected

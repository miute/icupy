from pathlib import Path

import pytest

from icupy import icu
from icupy.utils import gc


def test_open() -> None:
    path = Path(__file__).resolve().parent / "testdata"
    try:
        # UStringPrepProfile *usprep_open(
        #       const char *path,
        #       const char *fileName,
        #       UErrorCode *status
        # )
        #
        # void usprep_close(UStringPrepProfile *profile)
        with gc(icu.usprep_open(str(path), "nfscis"), icu.usprep_close) as prep:
            _ = icu.usprep_prepare(prep, "", 0, icu.USPREP_DEFAULT, None)
    except icu.ICUError as ex:
        if ex.args[0] != icu.UErrorCode.U_FILE_ACCESS_ERROR:
            raise
        pytest.skip(
            "testdata.dat is not found (not an error). "
            "You need to build a test data from the source. "
            "See also <icu4c>/icu/source/test/testdata/"
        )


def test_open_by_type() -> None:
    # From icu/source/test/cintltst/spreptst.c

    # UStringPrepProfile *usprep_openByType(
    #       UStringPrepProfileType type,
    #       UErrorCode *status
    # )
    #
    # void usprep_close(UStringPrepProfile *profile)
    with gc(
        icu.usprep_open_by_type(icu.UStringPrepProfileType.USPREP_RFC4013_SASLPREP),
        icu.usprep_close,
    ) as prep:
        # int32_t icu.usprep_prepare(
        #       const UStringPrepProfile *prep,
        #       const UChar *src,
        #       int32_t srcLength,
        #       UChar *dest,
        #       int32_t destCapacity,
        #       int32_t options,
        #       UParseError *parseError,
        #       UErrorCode *status
        # )
        src = icu.u_unescape("user:\\u00A0\\u0AC6\\u1680\\u00ADpassword1")
        expected = icu.u_unescape("user: \\u0AC6 password1")
        parse_error = icu.UParseError()
        dest = icu.usprep_prepare(prep, src, len(src), icu.USPREP_ALLOW_UNASSIGNED, parse_error)
        assert isinstance(dest, str)
        assert dest == expected

    with gc(
        icu.usprep_open_by_type(icu.UStringPrepProfileType.USPREP_RFC4011_MIB),
        icu.usprep_close,
    ) as prep:
        src = icu.u_unescape("Policy\\u034F\\u200DBase\\u0020d\\u1806\\u200C")
        expected = icu.u_unescape("PolicyBase d")
        dest = icu.usprep_prepare(prep, src, -1, icu.USPREP_ALLOW_UNASSIGNED, None)
        assert isinstance(dest, str)
        assert dest == expected

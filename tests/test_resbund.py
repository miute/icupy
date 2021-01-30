import copy
from pathlib import Path

import pytest

from icupy import (
    ICUException, Locale, ResourceBundle, UErrorCode, ULocDataLocaleType,
    UnicodeString, UResType,
    ures_close, ures_open, ures_open_direct,
)


def _int28_to_uint28(n: int) -> int:
    return n if n >= 0 else 2 ** 28 + n


def test_api():
    rb = ures_open_direct(None, "metadata")
    test1 = ResourceBundle(rb)
    ures_close(rb)

    assert test1.get_type() == UResType.URES_TABLE
    assert test1.get_size() > 0
    assert len(test1) == test1.get_size()
    assert test1.get_key() is None
    assert test1.get_name() == "metadata"

    test2 = test1.get("alias")
    assert test2.get_type() == UResType.URES_TABLE
    assert test2.get_size() > 0
    assert len(test2) == test2.get_size()
    assert test2.get_key() == "alias"
    assert test2.get_name() == "metadata"

    test3 = test2.get("language")
    assert test3.get_type() == UResType.URES_TABLE
    assert test3.get_size() > 0
    assert len(test3) == test3.get_size()
    assert test3.get_key() == "language"
    assert test3.get_name() == "metadata"

    test4 = test3.get("sh")
    assert test4.get_type() == UResType.URES_TABLE
    assert test4.get_size() >= 2
    assert len(test4) == test4.get_size()
    assert test4.get_key() == "sh"
    assert test4.get_name() == "metadata"

    result = test4.get_string_ex("replacement")
    assert isinstance(result, UnicodeString)
    assert result == "sr_Latn"

    result = test4.get_string_ex("reason")
    assert isinstance(result, UnicodeString)
    assert result == "legacy"

    result1 = test4.get_string_ex(0)
    assert isinstance(result1, UnicodeString)
    result2 = test4.get_string_ex(1)
    assert isinstance(result2, UnicodeString)
    assert result1 != result2
    assert str(result1) in ["sr_Latn", "legacy"]
    assert str(result2) in ["sr_Latn", "legacy"]


def test_api2():
    path = Path(__file__).joinpath("..", "testdata").resolve()
    rb = None
    try:
        rb = ures_open_direct(str(path), "testtypes")
    except ICUException as ex:
        if ex.args[0] != UErrorCode.U_MISSING_RESOURCE_ERROR:
            raise
        pytest.skip("testdata.dat is not found (not an error). "
                    "You need to build a test data from the source. "
                    "See also <icu4c>/icu/source/test/testdata/")

    test1 = ResourceBundle(rb)
    ures_close(rb)

    # "testtypes"
    assert test1.get_type() == UResType.URES_TABLE
    assert test1.get_size() > 0
    assert test1.get_key() is None
    assert test1.get_name() == "testtypes"

    resources = list()
    while test1.has_next():
        result = test1.get_next()
        assert isinstance(result, ResourceBundle)
        resources.append(result)

    assert not test1.has_next()
    with pytest.raises(ICUException) as exc_info:
        _ = test1.get_next()
    assert exc_info.value.args[0] == UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    test1.reset_iterator()
    assert test1.has_next()

    for index, expected in enumerate(resources):
        result = test1.get(index)
        assert isinstance(result, ResourceBundle)
        assert result.get_key() is not None
        assert result.get_key() == expected.get_key()

    resources2 = [x for x in test1]
    assert len(resources2) == len(resources)
    for res1, res2 in zip(resources, resources2):
        assert res1.get_key() is not None
        assert res1.get_key() == res2.get_key()

    # "testtypes" > "binarytest"
    test2 = test1.get("binarytest")
    assert test2.get_type() == UResType.URES_BINARY
    result = test2.get_binary()
    assert isinstance(result, list)
    assert len(result) == 15
    assert result == [
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
        0x0b, 0x0c, 0x0d, 0x0e
    ]

    # "testtypes" > "emptybin"
    test3 = test1.get("emptybin")
    assert test3.get_type() == UResType.URES_BINARY
    result = test3.get_binary()
    assert isinstance(result, list)
    assert len(result) == 0

    # "testtypes" > "minusone"
    test4 = test1.get("minusone")
    assert test4.get_type() == UResType.URES_INT
    result = test4.get_int()
    assert isinstance(result, int)
    assert result == -1

    result = test4.get_uint()
    assert isinstance(result, int)
    assert result == _int28_to_uint28(-1)

    # "testtypes" > "emptyint"
    test5 = test1.get("emptyint")
    assert test5.get_type() == UResType.URES_INT
    result = test5.get_int()
    assert isinstance(result, int)
    assert result == 0

    result = test5.get_uint()
    assert isinstance(result, int)
    assert result == 0

    # "testtypes" > "integerarray"
    test6 = test1.get("integerarray")
    assert test6.get_type() == UResType.URES_INT_VECTOR
    result = test6.get_int_vector()
    assert isinstance(result, list)
    assert result == [1, 2, 3, -3, 4, 5, 6, 7]

    # "testtypes" > "emptyintv"
    test7 = test1.get("emptyintv")
    assert test7.get_type() == UResType.URES_INT_VECTOR
    result = test7.get_int_vector()
    assert isinstance(result, list)
    assert len(result) == 0

    # "testtypes" > "menu" > "file"
    test8 = test1.get("menu")
    test9 = test8.get("file")

    result = test9.get_next_string()
    assert isinstance(result, UnicodeString)
    assert str(result) in ["Open", "Save", "Exit"]

    result = test9.get_next_string()
    assert isinstance(result, UnicodeString)
    assert str(result) in ["Open", "Save", "Exit"]

    result = test9.get_next_string()
    assert isinstance(result, UnicodeString)
    assert str(result) in ["Open", "Save", "Exit"]

    with pytest.raises(ICUException) as exc_info:
        _ = test9.get_next_string()
    assert exc_info.value.args[0] == UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # "testtypes" > "zerotest"
    test10 = test1.get("zerotest")
    assert test10.get_type() == UResType.URES_STRING
    result = test10.get_string()
    assert isinstance(result, UnicodeString)
    assert len(result) == 7
    assert str(result) == "abc\u0000def"

    # "testtypes" > "emptystring"
    test11 = test1.get("emptystring")
    assert test11.get_type() == UResType.URES_STRING
    result = test11.get_string()
    assert isinstance(result, UnicodeString)
    assert len(result) == 0

    # "default"
    test12 = ResourceBundle(str(path), Locale.get_us())
    version_info = test12.get_version()
    assert isinstance(version_info, list)
    assert len(version_info) == 4
    assert version_info == [44, 0, 0, 0]

    test13 = ResourceBundle(str(path), Locale("te_IN"))
    version_info = test13.get_version()
    assert isinstance(version_info, list)
    assert len(version_info) == 4
    assert version_info == [55, 0, 0, 0]


def test_clone():
    test1 = ResourceBundle(None, Locale.get_us())
    test2 = test1.clone()
    assert test1.get_name() == test2.get_name()
    assert test1.get_key() == test2.get_key()

    test3 = copy.copy(test1)
    assert test3.get_name() == test1.get_name()
    assert test3.get_key() == test1.get_key()

    test4 = copy.deepcopy(test1)
    assert test4.get_name() == test1.get_name()
    assert test4.get_key() == test1.get_key()


def test_resource_bundle():
    default_locale = Locale.get_default()
    if str(default_locale) == "ja_JP":
        res_locale = Locale.get_us()
    else:
        res_locale = Locale.get_japan()
    assert default_locale != res_locale

    # [1]
    # ResourceBundle(const UnicodeString &packageName,
    #                const Locale &locale,
    #                UErrorCode &err
    # )
    test1 = ResourceBundle(UnicodeString(), res_locale)
    assert test1.get_type() == UResType.URES_TABLE
    assert test1.get_size() >= 0
    assert test1.get_key() is None
    assert (test1.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
            == res_locale)
    assert (test1.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
            == res_locale)
    assert test1.get_name() == res_locale.get_name()

    # [2]
    # ResourceBundle(const UnicodeString &packageName,
    #                UErrorCode &err
    # )
    test2 = ResourceBundle(UnicodeString())
    assert test2.get_type() == UResType.URES_TABLE
    assert test2.get_size() >= 0
    assert test2.get_key() is None
    assert (test2.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
            == default_locale)
    assert (test2.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
            == default_locale)
    assert test2.get_name() == default_locale.get_name()

    # [3]
    # ResourceBundle(UErrorCode &err)
    test3 = ResourceBundle()
    assert test3.get_type() == UResType.URES_TABLE
    assert test3.get_size() >= 0
    assert test3.get_key() is None
    assert (test3.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
            == default_locale)
    assert (test3.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
            == default_locale)
    assert test3.get_name() == default_locale.get_name()

    # [4]
    # ResourceBundle(const char *packageName,
    #                const Locale &locale,
    #                UErrorCode &err
    # )
    test4 = ResourceBundle(None, res_locale)
    assert test4.get_type() == UResType.URES_TABLE
    assert test4.get_size() >= 0
    assert test4.get_key() is None
    assert (test4.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
            == res_locale)
    assert (test4.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
            == res_locale)
    assert test4.get_name() == res_locale.get_name()

    # [5]
    # ResourceBundle(const ResourceBundle &original)
    test5 = ResourceBundle(test1)
    assert test5.get_type() == UResType.URES_TABLE
    assert test5.get_size() >= 0
    assert test5.get_key() is None
    assert (test5.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
            == res_locale)
    assert (test5.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
            == res_locale)
    assert test5.get_name() == res_locale.get_name()

    # [6]
    # ResourceBundle(UResourceBundle *res,
    #                UErrorCode &status
    # )
    rb = ures_open(None, str(res_locale))
    test6 = ResourceBundle(rb)
    ures_close(rb)
    assert test6.get_type() == UResType.URES_TABLE
    assert test6.get_size() >= 0
    assert test6.get_key() is None
    assert (test6.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
            == res_locale)
    assert (test6.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
            == res_locale)
    assert test6.get_name() == res_locale.get_name()

import copy
from pathlib import Path

import pytest

from icupy import icu


def _int28_to_uint28(n: int) -> int:
    return n if n >= 0 else 2**28 + n


def test_api() -> None:
    # UResourceBundle *ures_openDirect(
    #       const char *packageName,
    #       const char *locale,
    #       UErrorCode *status
    # )
    rb = icu.ures_open_direct(None, "metadata")
    test1 = icu.ResourceBundle(rb)
    icu.ures_close(rb)

    # UResType icu::ResourceBundle::getType(void)
    assert test1.get_type() == icu.UResType.URES_TABLE

    # int32_t icu::ResourceBundle::getSize(void)
    assert test1.get_size() > 0
    assert len(test1) == test1.get_size()

    # const char *icu::ResourceBundle::getKey(void)
    assert test1.get_key() is None

    # const char *icu::ResourceBundle::getName(void)
    assert test1.get_name() == "metadata"

    # [1]
    # ResourceBundle icu::ResourceBundle::get(
    #       const char *key,
    #       UErrorCode &status
    # )
    test2 = test1.get("alias")
    assert isinstance(test2, icu.ResourceBundle)
    assert test2.get_type() == icu.UResType.URES_TABLE
    assert test2.get_size() > 0
    assert len(test2) == test2.get_size()
    assert test2.get_key() == "alias"
    assert test2.get_name() == "metadata"

    test3 = test2.get("language")
    assert isinstance(test3, icu.ResourceBundle)
    assert test3.get_type() == icu.UResType.URES_TABLE
    assert test3.get_size() > 0
    assert len(test3) == test3.get_size()
    assert test3.get_key() == "language"
    assert test3.get_name() == "metadata"

    test4 = test3.get("sh")
    assert isinstance(test4, icu.ResourceBundle)
    assert test4.get_type() == icu.UResType.URES_TABLE
    assert test4.get_size() >= 2
    assert len(test4) == test4.get_size()
    assert test4.get_key() == "sh"
    assert test4.get_name() == "metadata"

    # [1]
    # UnicodeString icu::ResourceBundle::getStringEx(
    #       const char *key,
    #       UErrorCode &status
    # )
    result = test4.get_string_ex("replacement")
    assert isinstance(result, icu.UnicodeString)
    assert result == "sr_Latn"

    result = test4.get_string_ex("reason")
    assert isinstance(result, icu.UnicodeString)
    assert result == "legacy"

    # [2]
    # UnicodeString icu::ResourceBundle::getStringEx(
    #       int32_t index,
    #       UErrorCode &status
    # )
    result1 = test4.get_string_ex(0)
    assert isinstance(result1, icu.UnicodeString)
    result2 = test4.get_string_ex(1)
    assert isinstance(result2, icu.UnicodeString)
    assert result1 != result2
    assert str(result1) in ["sr_Latn", "legacy"]
    assert str(result2) in ["sr_Latn", "legacy"]


def test_api2() -> None:
    path = Path(__file__).resolve().parent / "testdata"
    rb = None
    try:
        rb = icu.ures_open_direct(str(path), "testtypes")
    except icu.ICUError as ex:
        if ex.args[0] != icu.UErrorCode.U_MISSING_RESOURCE_ERROR:
            raise
        pytest.skip(
            "testdata.dat is not found (not an error). "
            "You need to build a test data from the source. "
            "See also <icu4c>/icu/source/test/testdata/"
        )

    test1 = icu.ResourceBundle(rb)
    icu.ures_close(rb)

    # "testtypes"
    assert test1.get_type() == icu.UResType.URES_TABLE
    assert test1.get_size() > 0
    assert test1.get_key() is None
    assert test1.get_name() == "testtypes"

    resources = list()

    # UBool icu::ResourceBundle::hasNext(void)
    while test1.has_next():
        # ResourceBundle icu::ResourceBundle::getNext(UErrorCode &status)
        result = test1.get_next()
        assert isinstance(result, icu.ResourceBundle)
        resources.append(result)

    assert test1.has_next() is False
    with pytest.raises(icu.ICUError) as exc_info:
        _ = test1.get_next()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # void icu::ResourceBundle::resetIterator(void)
    test1.reset_iterator()
    assert test1.has_next() is True

    for index, expected in enumerate(resources):
        # [2]
        # ResourceBundle icu::ResourceBundle::get(
        #       int32_t index,
        #       UErrorCode &status
        # )
        result = test1.get(index)
        assert isinstance(result, icu.ResourceBundle)
        assert result.get_key() is not None
        assert result.get_key() == expected.get_key()

    resources2 = [x for x in test1]
    assert len(resources2) == len(resources)
    for res1, res2 in zip(resources, resources2):
        assert res1.get_key() is not None
        assert res1.get_key() == res2.get_key()

    # "testtypes" > "binarytest"
    test2 = test1.get("binarytest")
    assert isinstance(test2, icu.ResourceBundle)
    assert test2.get_type() == icu.UResType.URES_BINARY

    # const uint8_t *icu::ResourceBundle::getBinary(
    #       int32_t &len,
    #       UErrorCode &status
    # )
    result = test2.get_binary()
    assert isinstance(result, bytes)
    assert len(result) == 15
    assert result == b"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e"

    # "testtypes" > "emptybin"
    test3 = test1.get("emptybin")
    assert isinstance(test3, icu.ResourceBundle)
    assert test3.get_type() == icu.UResType.URES_BINARY
    result = test3.get_binary()
    assert isinstance(result, bytes)
    assert len(result) == 0

    # "testtypes" > "minusone"
    test4 = test1.get("minusone")
    assert isinstance(test4, icu.ResourceBundle)
    assert test4.get_type() == icu.UResType.URES_INT

    # int32_t icu::ResourceBundle::getInt(UErrorCode &status)
    result = test4.get_int()
    assert isinstance(result, int)
    assert result == -1

    # uint32_t icu::ResourceBundle::getUInt(UErrorCode &status)
    result = test4.get_uint()
    assert isinstance(result, int)
    assert result == _int28_to_uint28(-1)

    # "testtypes" > "emptyint"
    test5 = test1.get("emptyint")
    assert isinstance(test5, icu.ResourceBundle)
    assert test5.get_type() == icu.UResType.URES_INT
    result = test5.get_int()
    assert isinstance(result, int)
    assert result == 0

    result = test5.get_uint()
    assert isinstance(result, int)
    assert result == 0

    # "testtypes" > "integerarray"
    test6 = test1.get("integerarray")
    assert isinstance(test6, icu.ResourceBundle)
    assert test6.get_type() == icu.UResType.URES_INT_VECTOR

    # const int32_t *icu::ResourceBundle::getIntVector(
    #       int32_t &len,
    #       UErrorCode &status
    # )
    result = test6.get_int_vector()
    assert isinstance(result, list)
    assert result == [1, 2, 3, -3, 4, 5, 6, 7]

    # "testtypes" > "emptyintv"
    test7 = test1.get("emptyintv")
    assert isinstance(test7, icu.ResourceBundle)
    assert test7.get_type() == icu.UResType.URES_INT_VECTOR
    result = test7.get_int_vector()
    assert isinstance(result, list)
    assert len(result) == 0

    # "testtypes" > "menu" > "file"
    test8 = test1.get("menu")
    assert isinstance(test8, icu.ResourceBundle)
    test9 = test8.get("file")
    assert isinstance(test9, icu.ResourceBundle)

    # [2]
    # UnicodeString icu::ResourceBundle::getNextString(
    #       const char **key,
    #       UErrorCode &status
    # )
    result = test9.get_next_string()
    assert isinstance(result, icu.UnicodeString)
    assert str(result) in ["Open", "Save", "Exit"]

    result = test9.get_next_string()
    assert isinstance(result, icu.UnicodeString)
    assert str(result) in ["Open", "Save", "Exit"]

    result = test9.get_next_string()
    assert isinstance(result, icu.UnicodeString)
    assert str(result) in ["Open", "Save", "Exit"]

    with pytest.raises(icu.ICUError) as exc_info:
        _ = test9.get_next_string()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # "testtypes" > "zerotest"
    test10 = test1.get("zerotest")
    assert isinstance(test10, icu.ResourceBundle)
    assert test10.get_type() == icu.UResType.URES_STRING

    # UnicodeString icu::ResourceBundle::getString(UErrorCode &status)
    result = test10.get_string()
    assert isinstance(result, icu.UnicodeString)
    assert len(result) == 7
    assert str(result) == "abc\u0000def"

    # "testtypes" > "emptystring"
    test11 = test1.get("emptystring")
    assert isinstance(test11, icu.ResourceBundle)
    assert test11.get_type() == icu.UResType.URES_STRING
    result = test11.get_string()
    assert isinstance(result, icu.UnicodeString)
    assert len(result) == 0

    # "default"
    test12 = icu.ResourceBundle(str(path), icu.Locale.get_us())

    # void icu::ResourceBundle::getVersion(UVersionInfo versionInfo)
    version_info = test12.get_version()
    assert isinstance(version_info, tuple)
    assert len(version_info) == 4
    assert version_info == (44, 0, 0, 0)

    test13 = icu.ResourceBundle(str(path), icu.Locale("te_IN"))
    version_info = test13.get_version()
    assert isinstance(version_info, tuple)
    assert len(version_info) == 4
    assert version_info == (55, 0, 0, 0)


def test_clone() -> None:
    test1 = icu.ResourceBundle(None, icu.Locale.get_us())

    # ResourceBundle *icu::ResourceBundle::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.ResourceBundle)
    assert test1.get_name() == test2.get_name()
    assert test1.get_key() == test2.get_key()

    test3 = copy.copy(test1)
    assert test3.get_name() == test1.get_name()
    assert test3.get_key() == test1.get_key()

    test4 = copy.deepcopy(test1)
    assert test4.get_name() == test1.get_name()
    assert test4.get_key() == test1.get_key()


def test_resource_bundle() -> None:
    default_locale = icu.Locale.get_default()
    if str(default_locale) == "ja_JP":
        res_locale = icu.Locale.get_us()
    else:
        res_locale = icu.Locale.get_japan()
    assert default_locale != res_locale

    # [1]
    # icu::ResourceBundle::ResourceBundle(
    #       const UnicodeString &packageName,
    #       const Locale &locale,
    #       UErrorCode &err
    # )
    test1 = icu.ResourceBundle(icu.UnicodeString(), res_locale)
    assert test1.get_type() == icu.UResType.URES_TABLE
    assert test1.get_size() >= 0
    assert test1.get_key() is None
    assert test1.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == res_locale
    assert test1.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == res_locale
    assert test1.get_name() == res_locale.get_name()

    test1b = icu.ResourceBundle(icu.UnicodeString(), str(res_locale))
    assert test1b.get_type() == icu.UResType.URES_TABLE
    assert test1b.get_size() >= 0
    assert test1b.get_key() is None
    assert test1b.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == res_locale
    assert test1b.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == res_locale
    assert test1b.get_name() == res_locale.get_name()

    # [2]
    # icu::ResourceBundle::ResourceBundle(
    #       const UnicodeString &packageName,
    #       UErrorCode &err
    # )
    test2 = icu.ResourceBundle(icu.UnicodeString())
    assert test2.get_type() == icu.UResType.URES_TABLE
    assert test2.get_size() >= 0
    assert test2.get_key() is None
    assert test2.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == default_locale
    assert test2.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == default_locale
    assert test2.get_name() == default_locale.get_name()

    test2a = icu.ResourceBundle("")
    assert test2a.get_type() == icu.UResType.URES_TABLE
    assert test2a.get_size() >= 0
    assert test2a.get_key() is None
    assert test2a.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == default_locale
    assert test2a.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == default_locale
    assert test2a.get_name() == default_locale.get_name()

    # [3]
    # icu::ResourceBundle::ResourceBundle(UErrorCode &err)
    test3 = icu.ResourceBundle()
    assert test3.get_type() == icu.UResType.URES_TABLE
    assert test3.get_size() >= 0
    assert test3.get_key() is None
    assert test3.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == default_locale
    assert test3.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == default_locale
    assert test3.get_name() == default_locale.get_name()

    # [4]
    # icu::ResourceBundle::ResourceBundle(
    #       const char *packageName,
    #       const Locale &locale,
    #       UErrorCode &err
    # )
    test4 = icu.ResourceBundle(None, res_locale)
    assert test4.get_type() == icu.UResType.URES_TABLE
    assert test4.get_size() >= 0
    assert test4.get_key() is None
    assert test4.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == res_locale
    assert test4.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == res_locale
    assert test4.get_name() == res_locale.get_name()

    test4a = icu.ResourceBundle(None, str(res_locale))
    assert test4a.get_type() == icu.UResType.URES_TABLE
    assert test4a.get_size() >= 0
    assert test4a.get_key() is None
    assert test4a.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == res_locale
    assert test4a.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == res_locale
    assert test4a.get_name() == res_locale.get_name()

    # [5]
    # icu::ResourceBundle::ResourceBundle(const ResourceBundle &original)
    test5 = icu.ResourceBundle(test1)
    assert test5.get_type() == icu.UResType.URES_TABLE
    assert test5.get_size() >= 0
    assert test5.get_key() is None
    assert test5.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == res_locale
    assert test5.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == res_locale
    assert test5.get_name() == res_locale.get_name()

    # [6]
    # icu::ResourceBundle::ResourceBundle(
    #       UResourceBundle *res,
    #       UErrorCode &status
    # )
    rb = icu.ures_open(None, str(res_locale))
    test6 = icu.ResourceBundle(rb)
    icu.ures_close(rb)
    assert test6.get_type() == icu.UResType.URES_TABLE
    assert test6.get_size() >= 0
    assert test6.get_key() is None
    assert test6.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE) == res_locale
    assert test6.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE) == res_locale
    assert test6.get_name() == res_locale.get_name()

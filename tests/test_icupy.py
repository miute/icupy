import pytest
from icupy.icu import (
    ErrorCode, ICUError, UErrorCode, U_ICU_VERSION_MAJOR_NUM,
    u_failure, u_get_version, u_success, u_version_from_string,
    u_version_to_string,
)


def test_icu_error():
    assert issubclass(ICUError, Exception)

    with pytest.raises(ICUError) as exc_info:
        _ = u_version_to_string([1, 2, 3])
    ex = exc_info.value
    assert isinstance(ex, ICUError)
    assert len(ex.args) == 1
    error_code = ex.args[0]
    assert isinstance(error_code, ErrorCode)

    # const char *icu::ErrorCode::errorName()
    error_name = error_code.error_name
    assert isinstance(error_name, str)
    assert error_name == "U_ILLEGAL_ARGUMENT_ERROR"

    # UErrorCode icu::ErrorCode::get()
    status = error_code.get()
    assert isinstance(status, UErrorCode)
    assert status == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # ErrorCode.__eq__(self, other: UErrorCode) -> bool
    assert error_code == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    if U_ICU_VERSION_MAJOR_NUM >= 63:
        from icupy.icu import Locale

        with pytest.raises(ICUError) as exc_info:
            _ = Locale.for_language_tag("x")
        ex = exc_info.value
        assert isinstance(ex, ICUError)
        assert len(ex.args) == 2
        assert isinstance(ex.args[0], ErrorCode)  # icu::ErrorCode
        assert isinstance(ex.args[1], str)  # An error message


# TODO: Remove test_icu_exception().
def test_icu_exception():
    """Backward compatibility test.
    ICUException is deprecated. Use ICUError instead.
    """

    from icupy import ICUException

    assert issubclass(ICUException, Exception)

    with pytest.raises(ICUException) as exc_info:
        _ = u_version_to_string([1, 2, 3])
    ex = exc_info.value
    assert isinstance(ex, ICUException)
    assert len(ex.args) == 1
    error_code = ex.args[0]
    assert isinstance(error_code, ErrorCode)

    # const char *icu::ErrorCode::errorName()
    error_name = error_code.error_name
    assert isinstance(error_name, str)
    assert error_name == "U_ILLEGAL_ARGUMENT_ERROR"

    # UErrorCode icu::ErrorCode::get()
    status = error_code.get()
    assert isinstance(status, UErrorCode)
    assert status == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # ErrorCode.__eq__(self, other: UErrorCode) -> bool
    assert error_code == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    if U_ICU_VERSION_MAJOR_NUM >= 63:
        from icupy.icu import Locale

        with pytest.raises(ICUException) as exc_info:
            _ = Locale.for_language_tag("x")
        ex = exc_info.value
        assert isinstance(ex, ICUException)
        assert len(ex.args) == 2
        assert isinstance(ex.args[0], ErrorCode)  # icu::ErrorCode
        assert isinstance(ex.args[1], str)  # Error message


def test_u_failure():
    assert not u_failure(UErrorCode.U_USING_FALLBACK_WARNING)
    assert not u_failure(UErrorCode.U_ZERO_ERROR)
    assert u_failure(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_u_get_data_version():
    from icupy.icu import u_get_data_version

    # void u_getDataVersion(
    #       UVersionInfo dataVersionFillin,
    #       UErrorCode *status
    # )
    data_version = u_get_data_version()
    assert isinstance(data_version, list)
    assert len(data_version) == 4
    assert all(isinstance(x, int) for x in data_version)
    assert all(x >= 0 for x in data_version)


def test_u_get_version():
    # void u_getVersion(UVersionInfo versionArray)
    version_array = u_get_version()
    assert isinstance(version_array, list)
    assert len(version_array) == 4
    assert all(isinstance(x, int) for x in version_array)
    assert all(x >= 0 for x in version_array)


def test_u_success():
    assert u_success(UErrorCode.U_USING_FALLBACK_WARNING)
    assert u_success(UErrorCode.U_ZERO_ERROR)
    assert not u_success(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR)


def test_u_version_from_string():
    # void u_versionFromString(
    #       UVersionInfo versionArray,
    #       const char *versionString
    # )
    version_array = u_version_from_string("123.45.67.89")
    assert isinstance(version_array, list)
    assert len(version_array) == 4
    assert version_array[0] == 123
    assert version_array[1] == 45
    assert version_array[2] == 67
    assert version_array[3] == 89


def test_u_version_to_string():
    # void u_versionToString(
    #       const UVersionInfo versionArray,
    #       char *versionString
    # )
    version_string = u_version_to_string([123, 45, 67, 89])
    assert isinstance(version_string, str)
    assert version_string == "123.45.67.89"

    with pytest.raises(ICUError) as exc_info:
        _ = u_version_to_string([1, 2, 3])
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    with pytest.raises(ICUError) as exc_info:
        _ = u_version_to_string([1, 2, 3, 4, 5])
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    with pytest.raises(TypeError):
        _ = u_version_to_string([256, 0, 0, 0])  # [0, 255]

import pytest

from icupy import (
    ICUException, UErrorCode, u_failure, u_get_version, u_success,
    u_version_from_string, u_version_to_string,
)


def test_u_failure():
    assert not u_failure(UErrorCode.U_USING_FALLBACK_WARNING)
    assert not u_failure(UErrorCode.U_ZERO_ERROR)
    assert u_failure(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR)


def test_u_get_version():
    # void u_getVersion(UVersionInfo versionArray)
    version_array = u_get_version()
    assert isinstance(version_array, list)
    assert len(version_array) == 4
    assert version_array[0] > 0
    assert version_array[1] >= 0
    assert version_array[2] >= 0
    assert version_array[3] >= 0


def test_u_success():
    assert u_success(UErrorCode.U_USING_FALLBACK_WARNING)
    assert u_success(UErrorCode.U_ZERO_ERROR)
    assert not u_success(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR)


def test_u_version_from_string():
    # void u_versionFromString(UVersionInfo versionArray,
    #                          const char *versionString
    # )
    version_array = u_version_from_string("123.45.67.89")
    assert isinstance(version_array, list)
    assert len(version_array) == 4
    assert version_array[0] == 123
    assert version_array[1] == 45
    assert version_array[2] == 67
    assert version_array[3] == 89


def test_u_version_to_string():
    # void u_versionToString(const UVersionInfo versionArray,
    #                        char *versionString
    # )
    version_string = u_version_to_string([123, 45, 67, 89])
    assert isinstance(version_string, str)
    assert version_string == "123.45.67.89"

    with pytest.raises(ICUException) as exc_info:
        _ = u_version_to_string([1, 2, 3])
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    with pytest.raises(ICUException) as exc_info:
        _ = u_version_to_string([1, 2, 3, 4, 5])
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    with pytest.raises(TypeError):
        _ = u_version_to_string([256, 0, 0, 0])  # [0, 255]

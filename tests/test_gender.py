import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)

from icupy.icu import GenderInfo, Locale, UGender


def test_api():
    # static const GenderInfo *icu::GenderInfo::getInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    gi = GenderInfo.get_instance(Locale.get_us())
    assert isinstance(gi, GenderInfo)

    # UGender icu::GenderInfo::getListGender(
    #       const UGender *genders,
    #       int32_t length,
    #       UErrorCode &status
    # )
    assert gi.get_list_gender([], 0) == UGender.UGENDER_OTHER

    assert gi.get_list_gender(
        [UGender.UGENDER_MALE],
        1) == UGender.UGENDER_MALE

    assert gi.get_list_gender(
        [UGender.UGENDER_FEMALE, UGender.UGENDER_MALE],
        2) == UGender.UGENDER_OTHER

    gi = GenderInfo.get_instance("fr")
    assert isinstance(gi, GenderInfo)

    # length is optional
    assert gi.get_list_gender([]) == UGender.UGENDER_OTHER

    assert gi.get_list_gender(
        [UGender.UGENDER_MALE]) == UGender.UGENDER_MALE

    assert gi.get_list_gender(
        [UGender.UGENDER_FEMALE, UGender.UGENDER_MALE]
    ) == UGender.UGENDER_MALE

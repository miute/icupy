import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)


def test_api():
    # static const GenderInfo *icu::GenderInfo::getInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    gi = icu.GenderInfo.get_instance(icu.Locale.get_us())
    assert isinstance(gi, icu.GenderInfo)

    # UGender icu::GenderInfo::getListGender(
    #       const UGender *genders,
    #       int32_t length,
    #       UErrorCode &status
    # )
    assert gi.get_list_gender([], 0) == icu.UGender.UGENDER_OTHER

    assert gi.get_list_gender([icu.UGender.UGENDER_MALE], 1) == icu.UGender.UGENDER_MALE

    assert (
        gi.get_list_gender([icu.UGender.UGENDER_FEMALE, icu.UGender.UGENDER_MALE], 2)
        == icu.UGender.UGENDER_OTHER
    )

    gi = icu.GenderInfo.get_instance("fr")
    assert isinstance(gi, icu.GenderInfo)

    # length is optional
    assert gi.get_list_gender([]) == icu.UGender.UGENDER_OTHER

    assert gi.get_list_gender([icu.UGender.UGENDER_MALE]) == icu.UGender.UGENDER_MALE

    assert (
        gi.get_list_gender([icu.UGender.UGENDER_FEMALE, icu.UGender.UGENDER_MALE])
        == icu.UGender.UGENDER_MALE
    )

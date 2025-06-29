import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)

import copy


def test_api() -> None:
    # static TimeZoneNames *icu::TimeZoneNames::createInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    tzn1 = icu.TimeZoneNames.create_instance(icu.Locale("en"))
    assert isinstance(tzn1, icu.TimeZoneNames)

    tzn2 = icu.TimeZoneNames.create_instance("en")
    assert isinstance(tzn2, icu.TimeZoneNames)

    tzn3 = icu.TimeZoneNames.create_instance("ja")

    # UBool icu::TimeZoneNames::operator!=(const TimeZoneNames &other)
    assert not (tzn1 != tzn2)
    assert tzn1 != tzn3
    assert tzn2 != tzn3

    # UBool icu::TimeZoneNames::operator==(const TimeZoneNames &other)
    assert tzn1 == tzn2
    assert not (tzn1 == tzn3)
    assert not (tzn2 == tzn3)

    # [1]
    # StringEnumeration *icu::TimeZoneNames::getAvailableMetaZoneIDs(
    #       const UnicodeString &tzID,
    #       UErrorCode &status
    # )
    it1 = tzn1.get_available_meta_zone_ids(icu.UnicodeString("America/Los_Angeles"))
    assert isinstance(it1, icu.StringEnumeration)
    assert len(it1) > 0
    assert "America_Pacific" in it1

    it2 = tzn1.get_available_meta_zone_ids("America/Los_Angeles")
    assert isinstance(it2, icu.StringEnumeration)
    assert it2 == it1

    # [2]
    # StringEnumeration *icu::TimeZoneNames::getAvailableMetaZoneIDs(
    #       UErrorCode &status
    # )
    it3 = tzn1.get_available_meta_zone_ids()
    assert isinstance(it3, icu.StringEnumeration)
    assert len(it3) > 0
    assert "America_Pacific" in it3

    date = 1358208000000.0  # 2013-01-15T00:00:00Z
    name = icu.UnicodeString()

    # UnicodeString &icu::TimeZoneNames::getDisplayName(
    #       const UnicodeString &tzID,
    #       UTimeZoneNameType type,
    #       UDate date,
    #       UnicodeString &name
    # )
    result = tzn1.get_display_name(
        icu.UnicodeString("America/Los_Angeles"),
        icu.UTimeZoneNameType.UTZNM_LONG_STANDARD,
        date,
        name,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"

    result = tzn1.get_display_name(
        "America/Los_Angeles",
        icu.UTimeZoneNameType.UTZNM_LONG_DAYLIGHT,
        date,
        name,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Daylight Time"

    # UnicodeString &icu::TimeZoneNames::getExemplarLocationName(
    #       const UnicodeString &tzID,
    #       UnicodeString &name
    # )
    result = tzn1.get_exemplar_location_name(icu.UnicodeString("America/Los_Angeles"), name)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Los Angeles"

    result = tzn1.get_exemplar_location_name("Asia/Tokyo", name)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Tokyo"

    # UnicodeString &icu::TimeZoneNames::getMetaZoneDisplayName(
    #       const UnicodeString &mzID,
    #       UTimeZoneNameType type,
    #       UnicodeString &name
    # )
    result = tzn1.get_meta_zone_display_name(
        icu.UnicodeString("America_Pacific"),
        icu.UTimeZoneNameType.UTZNM_LONG_STANDARD,
        name,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"

    result = tzn1.get_meta_zone_display_name(
        "America_Pacific", icu.UTimeZoneNameType.UTZNM_LONG_DAYLIGHT, name
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Daylight Time"

    mz_id = icu.UnicodeString()

    # UnicodeString &icu::TimeZoneNames::getMetaZoneID(
    #       const UnicodeString &tzID,
    #       UDate date,
    #       UnicodeString &mzID
    # )
    result = tzn1.get_meta_zone_id(icu.UnicodeString("America/Los_Angeles"), date, mz_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(mz_id)
    assert result == "America_Pacific"

    result = tzn1.get_meta_zone_id("America/Los_Angeles", date, mz_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(mz_id)
    assert result == "America_Pacific"

    tz_id = icu.UnicodeString()

    # UnicodeString &icu::TimeZoneNames::getReferenceZoneID(
    #       const UnicodeString &mzID,
    #       const char *region,
    #       UnicodeString &tzID
    # )
    result = tzn1.get_reference_zone_id(icu.UnicodeString("America_Pacific"), "001", tz_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(tz_id)
    assert result == "America/Los_Angeles"

    result = tzn1.get_reference_zone_id("America_Pacific", "001", tz_id)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(tz_id)
    assert result == "America/Los_Angeles"

    # UnicodeString &icu::TimeZoneNames::getTimeZoneDisplayName(
    #       const UnicodeString &tzID,
    #       UTimeZoneNameType type,
    #       UnicodeString &name
    # )
    result = tzn1.get_time_zone_display_name(
        icu.UnicodeString("America/Los_Angeles"),
        icu.UTimeZoneNameType.UTZNM_LONG_STANDARD,
        name,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result.is_bogus()

    result = tzn1.get_time_zone_display_name(
        "America/Los_Angeles", icu.UTimeZoneNameType.UTZNM_LONG_STANDARD, name
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result.is_bogus()


def test_clone() -> None:
    tzn1 = icu.TimeZoneNames.create_instance(icu.Locale("en"))

    # TimeZoneNames *icu::TimeZoneNames::clone()
    tzn2 = tzn1.clone()
    assert isinstance(tzn2, icu.TimeZoneNames)
    assert tzn2 == tzn1

    tzn3 = copy.copy(tzn1)
    assert tzn3 == tzn1

    tzn4 = copy.deepcopy(tzn1)
    assert tzn4 == tzn1


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_create_tzdb_instance() -> None:
    # static TimeZoneNames *icu::TimeZoneNames::createTZDBInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    tzn1 = icu.TimeZoneNames.create_tzdb_instance(icu.Locale("en"))
    assert isinstance(tzn1, icu.TimeZoneNames)

    tzn2 = icu.TimeZoneNames.create_tzdb_instance("en")
    assert isinstance(tzn2, icu.TimeZoneNames)

    # UBool icu::TZDBTimeZoneNames::operator==(...) is not implemented (ICU 69)
    # assert tzn1 == tzn2

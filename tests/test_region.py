import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 51:
    pytest.skip("ICU4C<51", allow_module_level=True)

from icupy.icu import Region, StringEnumeration, URegionType


def test_api():
    world = Region.get_instance("001")
    continent = Region.get_instance("039")  # Southern Europe
    territory = Region.get_instance("IT")  # Italy

    # UBool icu::Region::contains(const Region &other)
    assert not territory.contains(continent)
    assert continent.contains(territory)

    # __contains__(self, item: Region)
    assert continent not in territory
    assert territory in continent

    # [1]
    # const Region *icu::Region::getContainingRegion()
    result = world.get_containing_region()
    assert result is None

    result = territory.get_containing_region()
    assert isinstance(result, Region)
    assert result.contains(territory)

    # [2]
    # const Region *icu::Region::getContainingRegion(URegionType type)
    result = world.get_containing_region(URegionType.URGN_CONTINENT)
    assert result is None

    result = territory.get_containing_region(URegionType.URGN_CONTINENT)
    assert isinstance(result, Region)

    # int32_t icu::Region::getNumericCode()
    assert world.get_numeric_code() == 1
    assert continent.get_numeric_code() == 39
    assert territory.get_numeric_code() == 380

    # const char *icu::Region::getRegionCode()
    result = world.get_region_code()
    assert isinstance(result, str)
    assert result == "001"

    result = continent.get_region_code()
    assert isinstance(result, str)
    assert result == "039"

    result = territory.get_region_code()
    assert isinstance(result, str)
    assert result == "IT"

    # URegionType icu::Region::getType()
    assert world.get_type() == URegionType.URGN_WORLD
    assert continent.get_type() == URegionType.URGN_SUBCONTINENT
    assert territory.get_type() == URegionType.URGN_TERRITORY

    # bool icu::Region::operator!=(const Region &that)
    region = Region.get_instance(380)
    assert continent != territory
    assert continent != region
    assert not (territory != region)

    # bool icu::Region::operator==(const Region &that)
    assert not (continent == territory)
    assert not (continent == region)
    assert territory == region


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_api_55():
    # static StringEnumeration *icu::Region::getAvailable(
    #       URegionType type,
    #       UErrorCode &status
    # )
    territories = Region.get_available(URegionType.URGN_GROUPING)
    assert isinstance(territories, StringEnumeration)
    assert len(territories) > 0

    world = Region.get_instance("001")

    # [1]
    # StringEnumeration *icu::Region::getContainedRegions(UErrorCode &status)
    territories = world.get_contained_regions()
    assert isinstance(territories, StringEnumeration)
    assert len(territories) > 0

    # [2]
    # StringEnumeration *icu::Region::getContainedRegions(
    #       URegionType type,
    #       UErrorCode &status
    # )
    territories = world.get_contained_regions(URegionType.URGN_TERRITORY)
    assert isinstance(territories, StringEnumeration)
    assert len(territories) > 0

    # StringEnumeration *icu::Region::getPreferredValues(UErrorCode &status)
    territories = world.get_preferred_values()
    assert territories is None

    grouping = Region.get_instance("SU")
    territories = grouping.get_preferred_values()
    assert isinstance(territories, StringEnumeration)
    assert len(territories) > 0


def test_get_instance():
    # [1]
    # static const Region *icu::Region::getInstance(
    #       const char *region_code,
    #       UErrorCode &status
    # )
    region = Region.get_instance("EU")
    assert isinstance(region, Region)

    # [2]
    # static const Region *icu::Region::getInstance(
    #       int32_t code,
    #       UErrorCode &status
    # )
    region = Region.get_instance(967)
    assert isinstance(region, Region)

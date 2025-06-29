import copy

import pytest

from icupy import icu


def test_api() -> None:
    gen1 = icu.DateTimePatternGenerator.create_instance(icu.Locale("en_US"))
    gen2 = icu.DateTimePatternGenerator.create_instance("en_US")
    gen3 = icu.DateTimePatternGenerator.create_instance(icu.Locale("en_US@calendar=japanese"))

    # UBool icu::DateTimePatternGenerator::operator!=(
    #       const DateTimePatternGenerator &other
    # )
    assert not (gen1 != gen2)
    assert gen1 != gen3
    assert gen2 != gen3

    # UBool icu::DateTimePatternGenerator::operator==(
    #       const DateTimePatternGenerator &other
    # )
    assert gen1 == gen2
    assert not (gen1 == gen3)
    assert not (gen2 == gen3)

    # const UnicodeString &icu::DateTimePatternGenerator::getAppendItemFormat(
    #       UDateTimePatternField field
    # )
    result = gen2.get_append_item_format(icu.UDateTimePatternField.UDATPG_ERA_FIELD)
    assert isinstance(result, icu.UnicodeString)
    assert result == "{0} {1}"

    # void icu::DateTimePatternGenerator::setAppendItemFormat(
    #       UDateTimePatternField field,
    #       const UnicodeString &value
    # )
    gen2.set_append_item_format(
        icu.UDateTimePatternField.UDATPG_ERA_FIELD,
        icu.UnicodeString("{0}, {1}"),
    )
    assert gen2.get_append_item_format(icu.UDateTimePatternField.UDATPG_ERA_FIELD) == "{0}, {1}"

    gen2.set_append_item_format(icu.UDateTimePatternField.UDATPG_ERA_FIELD, "{0} {1}")
    assert gen2.get_append_item_format(icu.UDateTimePatternField.UDATPG_ERA_FIELD) == "{0} {1}"

    # const UnicodeString &icu::DateTimePatternGenerator::getAppendItemName(
    #       UDateTimePatternField field
    # )
    result = gen2.get_append_item_name(icu.UDateTimePatternField.UDATPG_ERA_FIELD)
    assert isinstance(result, icu.UnicodeString)
    assert result == "era"

    # void icu::DateTimePatternGenerator::setAppendItemName(
    #       UDateTimePatternField field,
    #       const UnicodeString &value
    # )
    gen2.set_append_item_name(
        icu.UDateTimePatternField.UDATPG_ERA_FIELD, icu.UnicodeString("epoch")
    )
    assert gen2.get_append_item_name(icu.UDateTimePatternField.UDATPG_ERA_FIELD) == "epoch"

    gen2.set_append_item_name(icu.UDateTimePatternField.UDATPG_ERA_FIELD, "era")
    assert gen2.get_append_item_name(icu.UDateTimePatternField.UDATPG_ERA_FIELD) == "era"

    # UnicodeString icu::DateTimePatternGenerator::getBaseSkeleton(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    result = gen1.get_base_skeleton(icu.UnicodeString("HH:mm"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "Hm"

    result = gen1.get_base_skeleton("HH:mm")
    assert isinstance(result, icu.UnicodeString)
    assert result == "Hm"

    # StringEnumeration *icu::DateTimePatternGenerator::getBaseSkeletons(
    #       UErrorCode &status
    # )
    it = gen1.get_base_skeletons()
    assert isinstance(it, icu.StringEnumeration)
    assert len(it) > 0

    # [1]
    # UnicodeString icu::DateTimePatternGenerator::getBestPattern(
    #       const UnicodeString &skeleton,
    #       UDateTimePatternMatchOptions options,
    #       UErrorCode &status
    # )
    result = gen1.get_best_pattern(
        icu.UnicodeString("MMMMdjmm"),
        icu.UDateTimePatternMatchOptions.UDATPG_MATCH_NO_OPTIONS,
    )
    assert isinstance(result, icu.UnicodeString)
    assert result in [
        "MMMM d, h:mm a",  # ICU<=70
        "MMMM d 'at' h:mm a",  # ICU>=71
        "MMMM d 'at' h:mm\u202fa",  # ICU>=72
    ]

    result = gen1.get_best_pattern(
        "MMMMdjmm", icu.UDateTimePatternMatchOptions.UDATPG_MATCH_NO_OPTIONS
    )
    assert isinstance(result, icu.UnicodeString)
    assert result in [
        "MMMM d, h:mm a",  # ICU<=70
        "MMMM d 'at' h:mm a",  # ICU>=71
        "MMMM d 'at' h:mm\u202fa",  # ICU>=72
    ]

    # [2]
    # UnicodeString icu::DateTimePatternGenerator::getBestPattern(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    result = gen1.get_best_pattern(icu.UnicodeString("MMMMdjmm"))
    assert isinstance(result, icu.UnicodeString)
    assert result in [
        "MMMM d, h:mm a",  # ICU<=70
        "MMMM d 'at' h:mm a",  # ICU>=71
        "MMMM d 'at' h:mm\u202fa",  # ICU>=72
    ]

    result = gen1.get_best_pattern("MMMMdjmm")
    assert isinstance(result, icu.UnicodeString)
    assert result in [
        "MMMM d, h:mm a",  # ICU<=70
        "MMMM d 'at' h:mm a",  # ICU>=71
        "MMMM d 'at' h:mm\u202fa",  # ICU>=72
    ]

    # const UnicodeString &icu::DateTimePatternGenerator::getDateTimeFormat()
    result = gen2.get_date_time_format()
    assert isinstance(result, icu.UnicodeString)
    assert result == "{1}, {0}"

    # void icu::DateTimePatternGenerator::setDateTimeFormat(
    #       const UnicodeString &dateTimeFormat
    # )
    gen2.set_date_time_format(icu.UnicodeString("{1} 'at' {0}"))
    assert gen2.get_date_time_format() == "{1} 'at' {0}"

    gen2.set_date_time_format("{1}, {0}")
    assert gen2.get_date_time_format() == "{1}, {0}"

    # const UnicodeString &icu::DateTimePatternGenerator::getDecimal()
    result = gen2.get_decimal()
    assert isinstance(result, icu.UnicodeString)
    assert result == "."

    # void icu::DateTimePatternGenerator::setDecimal(
    #       const UnicodeString &decimal
    # )
    gen2.set_decimal(icu.UnicodeString(","))
    assert gen2.get_decimal() == ","

    gen2.set_decimal(".")
    assert gen2.get_decimal() == "."

    # const UnicodeString &
    # icu::DateTimePatternGenerator::getPatternForSkeleton(
    #       const UnicodeString &skeleton
    # )
    result = gen1.get_pattern_for_skeleton(icu.UnicodeString("yMMMd"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "MMM d, y"

    result = gen1.get_pattern_for_skeleton("yMMMd")
    assert isinstance(result, icu.UnicodeString)
    assert result == "MMM d, y"

    # UnicodeString icu::DateTimePatternGenerator::getSkeleton(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    result = gen1.get_skeleton(icu.UnicodeString("MMM d, y"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "yMMMd"

    result = gen1.get_skeleton("MMM d, y")
    assert isinstance(result, icu.UnicodeString)
    assert result == "yMMMd"

    # StringEnumeration *icu::DateTimePatternGenerator::getSkeletons(
    #       UErrorCode &status
    # )
    it = gen1.get_skeletons()
    assert isinstance(it, icu.StringEnumeration)
    assert len(it) > 0

    # [1]
    # UnicodeString icu::DateTimePatternGenerator::replaceFieldTypes(
    #       const UnicodeString &pattern,
    #       const UnicodeString &skeleton,
    #       UDateTimePatternMatchOptions options,
    #       UErrorCode &status
    # )
    result = gen1.replace_field_types(
        icu.UnicodeString("d-M H:m"),
        icu.UnicodeString("MMMMddHHmm"),
        icu.UDateTimePatternMatchOptions.UDATPG_MATCH_ALL_FIELDS_LENGTH,
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM HH:mm"

    result = gen1.replace_field_types(
        "d-M H:m",
        icu.UnicodeString("MMMMddHHmm"),
        icu.UDateTimePatternMatchOptions.UDATPG_MATCH_ALL_FIELDS_LENGTH,
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM HH:mm"

    result = gen1.replace_field_types(
        icu.UnicodeString("d-M H:m"),
        "MMMMddHHmm",
        icu.UDateTimePatternMatchOptions.UDATPG_MATCH_ALL_FIELDS_LENGTH,
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM HH:mm"

    result = gen1.replace_field_types(
        "d-M H:m",
        "MMMMddHHmm",
        icu.UDateTimePatternMatchOptions.UDATPG_MATCH_ALL_FIELDS_LENGTH,
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM HH:mm"

    # [2]
    # UnicodeString icu::DateTimePatternGenerator::replaceFieldTypes(
    #       const UnicodeString &pattern,
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    result = gen1.replace_field_types(
        icu.UnicodeString("d-M H:m"), icu.UnicodeString("MMMMddHHmm")
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM H:m"

    result = gen1.replace_field_types("d-M H:m", icu.UnicodeString("MMMMddHHmm"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM H:m"

    result = gen1.replace_field_types(icu.UnicodeString("d-M H:m"), "MMMMddHHmm")
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM H:m"

    result = gen1.replace_field_types("d-M H:m", "MMMMddHHmm")
    assert isinstance(result, icu.UnicodeString)
    assert result == "dd-MMMM H:m"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 71, reason="ICU4C<71")
def test_api_71() -> None:
    dtpg = icu.DateTimePatternGenerator.create_instance(icu.Locale("en_US"))

    # const UnicodeString &icu::DateTimePatternGenerator::getDateTimeFormat(
    #       UDateFormatStyle style,
    #       UErrorCode &status
    # )
    result = dtpg.get_date_time_format(icu.UDateFormatStyle.UDAT_FULL)
    assert isinstance(result, icu.UnicodeString)
    assert result == "{1} 'at' {0}"

    # void icu::DateTimePatternGenerator::setDateTimeFormat(
    #       UDateFormatStyle style,
    #       const UnicodeString &dateTimeFormat,
    #       UErrorCode &status
    # )
    dtpg.set_date_time_format(icu.UDateFormatStyle.UDAT_FULL, icu.UnicodeString("{1}, {0}"))
    assert dtpg.get_date_time_format(icu.UDateFormatStyle.UDAT_FULL) == "{1}, {0}"

    dtpg.set_date_time_format(icu.UDateFormatStyle.UDAT_FULL, "{1} 'at' {0}")
    assert dtpg.get_date_time_format(icu.UDateFormatStyle.UDAT_FULL) == "{1} 'at' {0}"


def test_clone() -> None:
    gen1 = icu.DateTimePatternGenerator.create_instance("en_US")

    # DateTimePatternGenerator *icu::DateTimePatternGenerator::clone()
    gen2 = gen1.clone()
    assert isinstance(gen2, icu.DateTimePatternGenerator)
    assert gen2 == gen1

    gen3 = copy.copy(gen1)
    assert gen3 == gen1

    gen4 = copy.deepcopy(gen1)
    assert gen4 == gen1


def test_create_empty_instance() -> None:
    # static DateTimePatternGenerator *
    # icu::DateTimePatternGenerator::createEmptyInstance(UErrorCode &status)
    gen = icu.DateTimePatternGenerator.create_empty_instance()
    assert isinstance(gen, icu.DateTimePatternGenerator)

    # UDateTimePatternConflict icu::DateTimePatternGenerator::addPattern(
    #       const UnicodeString &pattern,
    #       UBool override,
    #       UnicodeString &conflictingPattern,
    #       UErrorCode &status
    # )
    conflicting_pattern = icu.UnicodeString()
    result = gen.add_pattern(icu.UnicodeString("MMMMd"), True, conflicting_pattern)
    assert result == icu.UDateTimePatternConflict.UDATPG_NO_CONFLICT
    assert len(conflicting_pattern) == 0

    result = gen.add_pattern("HH:mm", True, conflicting_pattern)
    assert result == icu.UDateTimePatternConflict.UDATPG_NO_CONFLICT
    assert len(conflicting_pattern) == 0

    result = gen.add_pattern("MMMMMdd", True, conflicting_pattern)
    assert result == icu.UDateTimePatternConflict.UDATPG_NO_CONFLICT
    assert len(conflicting_pattern) == 0

    # StringEnumeration *icu::DateTimePatternGenerator::getBaseSkeletons(
    #       UErrorCode &status
    # )
    it1 = gen.get_base_skeletons()
    assert isinstance(it1, icu.StringEnumeration)
    assert list(it1) == ["Hm", "MMMMd", "MMMMMd"]

    # StringEnumeration *icu::DateTimePatternGenerator::getSkeletons(
    #       UErrorCode &status
    # )
    it2 = gen.get_skeletons()
    assert isinstance(it2, icu.StringEnumeration)
    assert list(it2) == ["HHmm", "MMMMd", "MMMMMdd"]


def test_create_instance() -> None:
    # [1]
    # static DateTimePatternGenerator *
    # icu::DateTimePatternGenerator::createInstance(
    #       const Locale &uLocale,
    #       UErrorCode &status
    # )
    gen1 = icu.DateTimePatternGenerator.create_instance(icu.Locale("en_US"))
    assert isinstance(gen1, icu.DateTimePatternGenerator)

    gen1a = icu.DateTimePatternGenerator.create_instance("en_US")
    assert isinstance(gen1a, icu.DateTimePatternGenerator)

    # [2]
    # static DateTimePatternGenerator *
    # icu::DateTimePatternGenerator::createInstance(UErrorCode &status)
    gen2 = icu.DateTimePatternGenerator.create_instance()
    assert isinstance(gen2, icu.DateTimePatternGenerator)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_get_default_hour_cycle() -> None:
    gen = icu.DateTimePatternGenerator.create_instance(icu.Locale("en_US"))

    # UDateFormatHourCycle icu::DateTimePatternGenerator::getDefaultHourCycle(
    #       UErrorCode &status
    # )
    assert gen.get_default_hour_cycle() == icu.UDateFormatHourCycle.UDAT_HOUR_CYCLE_12


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 61, reason="ICU4C<61")
def test_get_field_display_name() -> None:
    gen = icu.DateTimePatternGenerator.create_instance(icu.Locale("en_US"))

    # UnicodeString icu::DateTimePatternGenerator::getFieldDisplayName(
    #       UDateTimePatternField field,
    #       UDateTimePGDisplayWidth width
    # )
    result = gen.get_field_display_name(
        icu.UDateTimePatternField.UDATPG_ERA_FIELD,
        icu.UDateTimePGDisplayWidth.UDATPG_ABBREVIATED,
    )
    assert isinstance(result, icu.UnicodeString)
    assert result == "era"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 56, reason="ICU4C<56")
def test_static_get_skeleton() -> None:
    # static UnicodeString
    # icu::DateTimePatternGenerator::staticGetBaseSkeleton(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    result = icu.DateTimePatternGenerator.static_get_base_skeleton(icu.UnicodeString("d-M H:m"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "MdHm"

    result = icu.DateTimePatternGenerator.static_get_base_skeleton("d-M H:m")
    assert isinstance(result, icu.UnicodeString)
    assert result == "MdHm"

    # static UnicodeString icu::DateTimePatternGenerator::staticGetSkeleton(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    result = icu.DateTimePatternGenerator.static_get_skeleton(icu.UnicodeString("d-M H:m"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "MdHm"

    result = icu.DateTimePatternGenerator.static_get_skeleton("d-M H:m")
    assert isinstance(result, icu.UnicodeString)
    assert result == "MdHm"

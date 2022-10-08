import pytest

from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 72:
    pytest.skip("ICU4C<72", allow_module_level=True)

# fmt: off
from icupy.icu import (
    DisplayOptions, UDisplayOptionsCapitalization,
    UDisplayOptionsDisplayLength, UDisplayOptionsGrammaticalCase,
    UDisplayOptionsNameStyle, UDisplayOptionsNounClass,
    UDisplayOptionsPluralCategory, UDisplayOptionsSubstituteHandling,
    udispopt_from_grammatical_case_identifier,
    udispopt_from_noun_class_identifier,
    udispopt_from_plural_category_identifier,
    udispopt_get_grammatical_case_identifier,
    udispopt_get_noun_class_identifier,
    udispopt_get_plural_category_identifier,
)

# fmt: on


def test_api():
    # UDisplayOptionsGrammaticalCase
    # udispopt_fromGrammaticalCaseIdentifier(const char *identifier)
    result = udispopt_from_grammatical_case_identifier("")
    assert isinstance(result, UDisplayOptionsGrammaticalCase)
    assert (
        result
        == UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_UNDEFINED
    )

    with pytest.raises(TypeError):
        _ = udispopt_from_grammatical_case_identifier(None)

    # UDisplayOptionsNounClass
    # udispopt_fromNounClassIdentifier(const char *identifier)
    result = udispopt_from_noun_class_identifier("")
    assert isinstance(result, UDisplayOptionsNounClass)
    assert result == UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_UNDEFINED

    with pytest.raises(TypeError):
        _ = udispopt_from_noun_class_identifier(None)

    # UDisplayOptionsPluralCategory
    # udispopt_fromPluralCategoryIdentifier(const char *identifier)
    result = udispopt_from_plural_category_identifier("")
    assert isinstance(result, UDisplayOptionsPluralCategory)
    assert (
        result
        == UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_UNDEFINED
    )

    with pytest.raises(TypeError):
        _ = udispopt_from_plural_category_identifier(None)

    # const char *
    # udispopt_getGrammaticalCaseIdentifier(
    #       UDisplayOptionsGrammaticalCase grammaticalCase
    # )
    result = udispopt_get_grammatical_case_identifier(
        UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_UNDEFINED
    )
    assert isinstance(result, str)

    # const char *
    # udispopt_getNounClassIdentifier(UDisplayOptionsNounClass nounClass)
    result = udispopt_get_noun_class_identifier(
        UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_UNDEFINED
    )
    assert isinstance(result, str)

    # const char *
    # udispopt_getPluralCategoryIdentifier(UDisplayOptionsPluralCategory pluralCategory)
    result = udispopt_get_plural_category_identifier(
        UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_UNDEFINED
    )
    assert isinstance(result, str)


def test_builder():
    # static icu::DisplayOptions::Builder icu::DisplayOptions::builder()
    bld = DisplayOptions.builder()
    assert isinstance(bld, DisplayOptions.Builder)

    # DisplayOptions Builder::build()
    ops = bld.build()
    assert isinstance(ops, DisplayOptions)

    # UDisplayOptionsCapitalization icu::DisplayOptions::getCapitalization()
    assert (
        ops.get_capitalization()
        == UDisplayOptionsCapitalization.UDISPOPT_CAPITALIZATION_UNDEFINED
    )

    # UDisplayOptionsDisplayLength icu::DisplayOptions::getDisplayLength()
    assert (
        ops.get_display_length()
        == UDisplayOptionsDisplayLength.UDISPOPT_DISPLAY_LENGTH_UNDEFINED
    )

    # UDisplayOptionsGrammaticalCase icu::DisplayOptions::getGrammaticalCase()
    assert (
        ops.get_grammatical_case()
        == UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_UNDEFINED
    )

    # UDisplayOptionsNameStyle icu::DisplayOptions::getNameStyle()
    assert (
        ops.get_name_style()
        == UDisplayOptionsNameStyle.UDISPOPT_NAME_STYLE_UNDEFINED
    )

    # UDisplayOptionsNounClass icu::DisplayOptions::getNounClass()
    assert (
        ops.get_noun_class()
        == UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_UNDEFINED
    )

    # UDisplayOptionsPluralCategory icu::DisplayOptions::getPluralCategory()
    assert (
        ops.get_plural_category()
        == UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_UNDEFINED
    )

    # UDisplayOptionsSubstituteHandling icu::DisplayOptions::getSubstituteHandling()
    assert (
        ops.get_substitute_handling()
        == UDisplayOptionsSubstituteHandling.UDISPOPT_SUBSTITUTE_HANDLING_UNDEFINED
    )

    # Builder &Builder::setCapitalization(
    #       UDisplayOptionsCapitalization capitalization
    # )
    result = bld.set_capitalization(
        UDisplayOptionsCapitalization.UDISPOPT_CAPITALIZATION_BEGINNING_OF_SENTENCE
    )
    assert id(result) == id(bld)

    # Builder &Builder::setDisplayLength(
    #       UDisplayOptionsDisplayLength displayLength
    # )
    result = bld.set_display_length(
        UDisplayOptionsDisplayLength.UDISPOPT_DISPLAY_LENGTH_FULL
    )
    assert id(result) == id(bld)

    # Builder &Builder::setGrammaticalCase(
    #       UDisplayOptionsGrammaticalCase grammaticalCase
    # )
    result = bld.set_grammatical_case(
        UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_ABLATIVE
    )
    assert id(result) == id(bld)

    # Builder &Builder::setNameStyle(
    #       UDisplayOptionsNameStyle nameStyle
    # )
    result = bld.set_name_style(
        UDisplayOptionsNameStyle.UDISPOPT_NAME_STYLE_STANDARD_NAMES
    )
    assert id(result) == id(bld)

    # Builder &Builder::setNounClass(
    #       UDisplayOptionsNounClass nounClass
    # )
    result = bld.set_noun_class(
        UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_PERSONAL
    )
    assert id(result) == id(bld)

    # Builder &Builder::setPluralCategory(
    #       UDisplayOptionsPluralCategory pluralCategory
    # )
    result = bld.set_plural_category(
        UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_FEW
    )
    assert id(result) == id(bld)

    # Builder &Builder::setSubstituteHandling(
    #       UDisplayOptionsSubstituteHandling substituteHandling
    # )
    result = bld.set_substitute_handling(
        UDisplayOptionsSubstituteHandling.UDISPOPT_SUBSTITUTE_HANDLING_NO_SUBSTITUTE
    )
    assert id(result) == id(bld)

    ops2 = bld.build()
    assert id(ops2) != id(ops)
    assert (
        ops2.get_capitalization()
        == UDisplayOptionsCapitalization.UDISPOPT_CAPITALIZATION_BEGINNING_OF_SENTENCE
    )
    assert (
        ops2.get_display_length()
        == UDisplayOptionsDisplayLength.UDISPOPT_DISPLAY_LENGTH_FULL
    )
    assert (
        ops2.get_grammatical_case()
        == UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_ABLATIVE
    )
    assert (
        ops2.get_name_style()
        == UDisplayOptionsNameStyle.UDISPOPT_NAME_STYLE_STANDARD_NAMES
    )
    assert (
        ops2.get_noun_class()
        == UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_PERSONAL
    )
    assert (
        ops2.get_plural_category()
        == UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_FEW
    )
    assert (
        ops2.get_substitute_handling()
        == UDisplayOptionsSubstituteHandling.UDISPOPT_SUBSTITUTE_HANDLING_NO_SUBSTITUTE
    )

    # Builder icu::DisplayOptions::copyToBuilder()
    bld2 = ops2.copy_to_builder()
    assert isinstance(bld2, DisplayOptions.Builder)

    ops3 = bld2.build()
    assert id(ops3) != id(ops2)
    assert id(ops3) != id(ops)
    assert (
        ops3.get_capitalization()
        == UDisplayOptionsCapitalization.UDISPOPT_CAPITALIZATION_BEGINNING_OF_SENTENCE
    )
    assert (
        ops3.get_display_length()
        == UDisplayOptionsDisplayLength.UDISPOPT_DISPLAY_LENGTH_FULL
    )
    assert (
        ops3.get_grammatical_case()
        == UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_ABLATIVE
    )
    assert (
        ops3.get_name_style()
        == UDisplayOptionsNameStyle.UDISPOPT_NAME_STYLE_STANDARD_NAMES
    )
    assert (
        ops3.get_noun_class()
        == UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_PERSONAL
    )
    assert (
        ops3.get_plural_category()
        == UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_FEW
    )
    assert (
        ops3.get_substitute_handling()
        == UDisplayOptionsSubstituteHandling.UDISPOPT_SUBSTITUTE_HANDLING_NO_SUBSTITUTE
    )

    # icu::DisplayOptions::DisplayOptions(const DisplayOptions &other)
    ops4 = DisplayOptions(ops2)
    assert (
        ops4.get_capitalization()
        == UDisplayOptionsCapitalization.UDISPOPT_CAPITALIZATION_BEGINNING_OF_SENTENCE
    )
    assert (
        ops4.get_display_length()
        == UDisplayOptionsDisplayLength.UDISPOPT_DISPLAY_LENGTH_FULL
    )
    assert (
        ops4.get_grammatical_case()
        == UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_ABLATIVE
    )
    assert (
        ops4.get_name_style()
        == UDisplayOptionsNameStyle.UDISPOPT_NAME_STYLE_STANDARD_NAMES
    )
    assert (
        ops4.get_noun_class()
        == UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_PERSONAL
    )
    assert (
        ops4.get_plural_category()
        == UDisplayOptionsPluralCategory.UDISPOPT_PLURAL_CATEGORY_FEW
    )
    assert (
        ops4.get_substitute_handling()
        == UDisplayOptionsSubstituteHandling.UDISPOPT_SUBSTITUTE_HANDLING_NO_SUBSTITUTE
    )

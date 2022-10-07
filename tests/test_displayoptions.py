import pytest

from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 72:
    pytest.skip("ICU4C<72", allow_module_level=True)

# fmt: off
from icupy.icu import (
    UDisplayOptionsGrammaticalCase, UDisplayOptionsNounClass,
    UDisplayOptionsPluralCategory, udispopt_from_grammatical_case_identifier,
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

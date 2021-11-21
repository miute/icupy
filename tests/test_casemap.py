import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 59:
    pytest.skip("ICU4C<59", allow_module_level=True)

from icupy.icu import (
    BreakIterator, CaseMap, Edits,
    U_EDITS_NO_RESET, U_FOLD_CASE_EXCLUDE_SPECIAL_I, U_OMIT_UNCHANGED_TEXT,
    U_TITLECASE_NO_BREAK_ADJUSTMENT, U_TITLECASE_NO_LOWERCASE,
    UnicodeString,
)


# From icu/source/test/intltest/strcase.cpp
def test_api():
    # static int32_t icu::CaseMap::fold(
    #       uint32_t options,
    #       const char16_t *src,
    #       int32_t srcLength,
    #       char16_t *dest,
    #       int32_t destCapacity,
    #       Edits *edits,
    #       UErrorCode &errorCode
    # )
    options = (U_OMIT_UNCHANGED_TEXT
               | U_EDITS_NO_RESET
               | U_FOLD_CASE_EXCLUDE_SPECIAL_I)
    src = "IßtanBul"
    edits = Edits()
    dest = CaseMap.fold(options, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "ıssb"

    dest = CaseMap.fold(options, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "ıssb"

    dest = CaseMap.fold(options, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "ıssb"

    dest = CaseMap.fold(options, src)  # src_length is optional
    assert isinstance(dest, str)
    assert dest == "ıssb"

    # static int32_t icu::CaseMap::toLower(
    #       const char *locale,
    #       uint32_t options,
    #       const char16_t *src,
    #       int32_t srcLength,
    #       char16_t *dest,
    #       int32_t destCapacity,
    #       Edits *edits,
    #       UErrorCode &errorCode
    # )
    locale = "tr"
    options = U_OMIT_UNCHANGED_TEXT
    src = "IstanBul"
    edits = Edits()
    dest = CaseMap.to_lower(locale, options, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "ıb"

    dest = CaseMap.to_lower(locale, options, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "ıb"

    dest = CaseMap.to_lower(locale, options, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "ıb"

    dest = CaseMap.to_lower(locale, options, src)  # src_length is optional
    assert isinstance(dest, str)
    assert dest == "ıb"

    # static int32_t icu::CaseMap::toTitle(
    #       const char *locale,
    #       uint32_t options,
    #       BreakIterator *iter,
    #       const char16_t *src,
    #       int32_t srcLength,
    #       char16_t *dest,
    #       int32_t destCapacity,
    #       Edits *edits,
    #       UErrorCode &errorCode
    # )
    locale = "nl"
    options = (U_OMIT_UNCHANGED_TEXT
               | U_TITLECASE_NO_BREAK_ADJUSTMENT
               | U_TITLECASE_NO_LOWERCASE)
    it = BreakIterator.create_word_instance(locale)
    src = "IjssEL IglOo"
    it.set_text(UnicodeString(src))
    it.first()
    edits = Edits()
    dest = CaseMap.to_title(locale, options, it, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "J"

    dest = CaseMap.to_title(
        locale, options, None, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "J"

    dest = CaseMap.to_title(locale, options, None, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "J"

    dest = CaseMap.to_title(
        locale, options, None, src)  # src_length is optional
    assert isinstance(dest, str)
    assert dest == "J"

    # static int32_t icu::CaseMap::toUpper(
    #       const char *locale,
    #       uint32_t options,
    #       const char16_t *src,
    #       int32_t srcLength,
    #       char16_t *dest,
    #       int32_t destCapacity,
    #       Edits *edits,
    #       UErrorCode &errorCode
    # )
    locale = "el"
    options = U_OMIT_UNCHANGED_TEXT
    src = "Πατάτα"
    edits = Edits()
    dest = CaseMap.to_upper(locale, options, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

    dest = CaseMap.to_upper(locale, options, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

    dest = CaseMap.to_upper(locale, options, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

    dest = CaseMap.to_upper(locale, options, src)  # src_length is optional
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 59:
    pytest.skip("ICU4C<59", allow_module_level=True)


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
    options = (
        icu.U_OMIT_UNCHANGED_TEXT | icu.U_EDITS_NO_RESET | icu.U_FOLD_CASE_EXCLUDE_SPECIAL_I
    )
    src = "IßtanBul"
    edits = icu.Edits()
    dest = icu.CaseMap.fold(options, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "ıssb"

    dest = icu.CaseMap.fold(options, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "ıssb"

    dest = icu.CaseMap.fold(options, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "ıssb"

    dest = icu.CaseMap.fold(options, src)  # src_length is optional
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
    options = icu.U_OMIT_UNCHANGED_TEXT
    src = "IstanBul"
    edits = icu.Edits()
    dest = icu.CaseMap.to_lower(locale, options, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "ıb"

    dest = icu.CaseMap.to_lower(locale, options, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "ıb"

    dest = icu.CaseMap.to_lower(locale, options, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "ıb"

    dest = icu.CaseMap.to_lower(locale, options, src)  # src_length is optional
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
    options = (
        icu.U_OMIT_UNCHANGED_TEXT
        | icu.U_TITLECASE_NO_BREAK_ADJUSTMENT
        | icu.U_TITLECASE_NO_LOWERCASE
    )
    it = icu.BreakIterator.create_word_instance(locale)
    src = "IjssEL IglOo"
    it.set_text(icu.UnicodeString(src))
    it.first()
    edits = icu.Edits()
    dest = icu.CaseMap.to_title(locale, options, it, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "J"

    dest = icu.CaseMap.to_title(locale, options, None, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "J"

    dest = icu.CaseMap.to_title(locale, options, None, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "J"

    dest = icu.CaseMap.to_title(locale, options, None, src)  # src_length is optional
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
    options = icu.U_OMIT_UNCHANGED_TEXT
    src = "Πατάτα"
    edits = icu.Edits()
    dest = icu.CaseMap.to_upper(locale, options, src, -1, edits)
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

    dest = icu.CaseMap.to_upper(locale, options, src, -1, None)  # edits can be None
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

    dest = icu.CaseMap.to_upper(locale, options, src, -1)  # edits is optional
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

    dest = icu.CaseMap.to_upper(locale, options, src)  # src_length is optional
    assert isinstance(dest, str)
    assert dest == "ΑΤΑΤΑ"

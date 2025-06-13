import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)

import copy


def test_api():
    rules = icu.PluralRules.create_rules("odd: n mod 2 is 1")
    fmt = icu.PluralFormat(rules)

    # void icu::PluralFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    pattern = icu.UnicodeString("odd {# is odd.} other{# is even.}")
    fmt.apply_pattern(pattern)

    fmt.apply_pattern(str(pattern))

    # UnicodeString &icu::PluralFormat::toPattern(UnicodeString &appendTo)
    append_to = icu.UnicodeString()
    result = fmt.to_pattern(append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == pattern

    # void icu::PluralFormat::setNumberFormat(
    #       const NumberFormat *format,
    #       UErrorCode &status
    # )
    numfmt = icu.NumberFormat.create_instance()
    fmt.set_number_format(numfmt)

    # UBool icu::PluralFormat::operator!=(const Format &other)
    fmt1 = icu.PluralFormat(icu.Locale("en"), rules)
    fmt2 = icu.PluralFormat(icu.Locale("en"), rules)
    fmt3 = icu.PluralFormat(icu.Locale("fr"), rules)
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert fmt2 != fmt3

    # UBool icu::PluralFormat::operator==(const Format &other)
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)


def test_clone():
    fmt1 = icu.PluralFormat(icu.Locale.get_english())

    # PluralFormat *icu::PluralFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.PluralFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format():
    # From icu/source/test/intltest/plurfmts.cpp
    pattern = "one{#st file}two{#nd file}few{#rd file}other{#th file}"
    fmt = icu.PluralFormat(
        icu.Locale.get_english(), icu.UPluralType.UPLURAL_TYPE_ORDINAL, pattern
    )
    append_to = icu.UnicodeString()

    # [1], [2]
    # UnicodeString &icu::PluralFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(icu.Formattable(3), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "3rd file"

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(icu.Formattable(3), append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(icu.Formattable(3), append_to, None)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(icu.Formattable(3), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "3rd file"

    # [5]
    # UnicodeString icu::PluralFormat::format(
    #       double number,
    #       UErrorCode &status
    # )
    result = fmt.format(3.0)
    assert isinstance(result, icu.UnicodeString)
    assert result == "3rd file"

    # [6]
    # UnicodeString &icu::PluralFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(3.0, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "3rd file"

    # [7]
    # UnicodeString icu::PluralFormat::format(
    #       int32_t number,
    #       UErrorCode &status
    # )
    result = fmt.format(3)
    assert isinstance(result, icu.UnicodeString)
    assert result == "3rd file"

    # [8]
    # UnicodeString &icu::PluralFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(3, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "3rd file"


def test_parse_object():
    pattern = "one{#st file}two{#nd file}few{#rd file}other{#th file}"
    fmt = icu.PluralFormat(
        icu.Locale.get_english(), icu.UPluralType.UPLURAL_TYPE_ORDINAL, pattern
    )

    # *No effect in ICU 69*
    # void icu::PluralFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    source = icu.UnicodeString("3rd file")
    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt.parse_object(source, result, parse_pos)
    assert parse_pos.get_index() == 0

    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt.parse_object(str(source), result, parse_pos)
    assert parse_pos.get_index() == 0

    # *U_INVALID_FORMAT_ERROR in ICU 69*
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse_object(source, result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt.parse_object(str(source), result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR


def test_plural_format():
    locale = icu.Locale.get_default()
    rules = icu.PluralRules.create_default_rules()
    pattern = icu.UnicodeString("other{#}")

    # [1]
    # icu::PluralFormat::PluralFormat(UErrorCode &status)
    fmt1 = icu.PluralFormat()

    # [2]
    # icu::PluralFormat::PluralFormat(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt2 = icu.PluralFormat(locale)
    assert fmt1 == fmt2

    # [3]
    # icu::PluralFormat::PluralFormat(
    #       const PluralRules &rules,
    #       UErrorCode &status
    # )
    fmt3 = icu.PluralFormat(rules)

    # [4]
    # icu::PluralFormat::PluralFormat(
    #       const Locale &locale,
    #       const PluralRules &rules,
    #       UErrorCode &status
    # )
    fmt4 = icu.PluralFormat(locale, rules)
    assert fmt3 == fmt4

    fmt4a = icu.PluralFormat(str(locale), rules)
    assert fmt4 == fmt4a

    # [5]
    # icu::PluralFormat::PluralFormat(
    #       const Locale &locale,
    #       UPluralType type,
    #       UErrorCode &status
    # )
    fmt5 = icu.PluralFormat(locale, icu.UPluralType.UPLURAL_TYPE_CARDINAL)

    fmt5a = icu.PluralFormat(str(locale), icu.UPluralType.UPLURAL_TYPE_CARDINAL)
    assert fmt5 == fmt5a

    # [6]
    # icu::PluralFormat::PluralFormat(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt6 = icu.PluralFormat(pattern)

    fmt6a = icu.PluralFormat(str(pattern))
    assert fmt6 == fmt6a

    # [7]
    # icu::PluralFormat::PluralFormat(
    #       const Locale &locale,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt7 = icu.PluralFormat(locale, pattern)
    assert fmt6 == fmt7

    fmt7a = icu.PluralFormat(str(locale), pattern)
    assert fmt7 == fmt7a

    fmt7b = icu.PluralFormat(locale, str(pattern))
    assert fmt7 == fmt7b

    fmt7c = icu.PluralFormat(str(locale), str(pattern))
    assert fmt7 == fmt7c

    # [8]
    # icu::PluralFormat::PluralFormat(
    #       const PluralRules &rules,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt8 = icu.PluralFormat(rules, pattern)

    fmt8a = icu.PluralFormat(rules, str(pattern))
    assert fmt8 == fmt8a

    # [9]
    # icu::PluralFormat::PluralFormat(
    #       const Locale &locale,
    #       const PluralRules &rules,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt9 = icu.PluralFormat(locale, rules, pattern)

    fmt9a = icu.PluralFormat(str(locale), rules, pattern)
    assert fmt9 == fmt9a

    fmt9b = icu.PluralFormat(locale, rules, str(pattern))
    assert fmt9 == fmt9b

    fmt9c = icu.PluralFormat(str(locale), rules, str(pattern))
    assert fmt9 == fmt9c

    # [10]
    # icu::PluralFormat::PluralFormat(
    #       const Locale &locale,
    #       UPluralType type,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt10 = icu.PluralFormat(locale, icu.UPluralType.UPLURAL_TYPE_CARDINAL, pattern)

    fmt10a = icu.PluralFormat(str(locale), icu.UPluralType.UPLURAL_TYPE_CARDINAL, pattern)
    assert fmt10 == fmt10a

    fmt10b = icu.PluralFormat(locale, icu.UPluralType.UPLURAL_TYPE_CARDINAL, str(pattern))
    assert fmt10 == fmt10b

    fmt10c = icu.PluralFormat(str(locale), icu.UPluralType.UPLURAL_TYPE_CARDINAL, str(pattern))
    assert fmt10 == fmt10c

    # [11]
    # icu::PluralFormat::PluralFormat(const PluralFormat &other)
    fmt11 = icu.PluralFormat(fmt1)
    assert fmt1 == fmt11

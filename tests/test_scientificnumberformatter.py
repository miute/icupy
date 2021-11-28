import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 55:
    pytest.skip("ICU4C<55", allow_module_level=True)

import copy

from icupy.icu import (
    Formattable, Locale, NumberFormat, ScientificNumberFormatter, UnicodeString,
)


def test_clone():
    decfmt = NumberFormat.create_scientific_instance("en")
    decfmt.apply_pattern("0.00E+0")
    fmt1 = ScientificNumberFormatter.create_superscript_instance(decfmt)
    append_to = UnicodeString()

    # ScientificNumberFormatter *icu::ScientificNumberFormatter::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, ScientificNumberFormatter)

    append_to.remove()
    fmt2.format(Formattable(6.02e23), append_to)
    assert append_to == "6.02\u00d710\u207a\u00b2\u00b3"

    fmt3 = copy.copy(fmt1)
    append_to.remove()
    fmt3.format(Formattable(6.02e23), append_to)
    assert append_to == "6.02\u00d710\u207a\u00b2\u00b3"

    fmt4 = copy.deepcopy(fmt1)
    append_to.remove()
    fmt4.format(Formattable(6.02e23), append_to)
    assert append_to == "6.02\u00d710\u207a\u00b2\u00b3"


def test_create_markup_instance():
    # [1]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createMarkupInstance(
    #       const Locale &locale,
    #       const UnicodeString &beginMarkup,
    #       const UnicodeString &endMarkup,
    #       UErrorCode &status
    # )
    fmt = ScientificNumberFormatter.create_markup_instance(
        Locale.get_english(),
        UnicodeString("<sup>"),
        UnicodeString("</sup>"))
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        "en",
        UnicodeString("<sup>"),
        UnicodeString("</sup>"))
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        Locale.get_english(),
        "<sup>",
        UnicodeString("</sup>"))
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        Locale.get_english(),
        UnicodeString("<sup>"),
        "</sup>")
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        "en",
        "<sup>",
        UnicodeString("</sup>"))
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        "en",
        UnicodeString("<sup>"),
        "</sup>")
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        Locale.get_english(),
        "<sup>",
        "</sup>")
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        "en",
        "<sup>",
        "</sup>")
    assert isinstance(fmt, ScientificNumberFormatter)

    decfmt = NumberFormat.create_scientific_instance("en")
    decfmt.apply_pattern("0.00E+0")

    # [2]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createMarkupInstance(
    #       DecimalFormat *fmtToAdopt,
    #       const UnicodeString &beginMarkup,
    #       const UnicodeString &endMarkup,
    #       UErrorCode &status
    # )
    fmt = ScientificNumberFormatter.create_markup_instance(
        decfmt,
        UnicodeString("<sup>"),
        UnicodeString("</sup>"))
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        decfmt,
        "<sup>",
        UnicodeString("</sup>"))
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        decfmt,
        UnicodeString("<sup>"),
        "</sup>")
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_markup_instance(
        decfmt,
        "<sup>",
        "</sup>")
    assert isinstance(fmt, ScientificNumberFormatter)


def test_create_superscript_instance():
    # [1]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createSuperscriptInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt = ScientificNumberFormatter.create_superscript_instance(
        Locale.get_english())
    assert isinstance(fmt, ScientificNumberFormatter)

    fmt = ScientificNumberFormatter.create_superscript_instance("en")
    assert isinstance(fmt, ScientificNumberFormatter)

    decfmt = NumberFormat.create_scientific_instance("en")
    decfmt.apply_pattern("0.00E+0")

    # [2]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createSuperscriptInstance(
    #       DecimalFormat *fmtToAdopt,
    #       UErrorCode &status
    # )
    fmt = ScientificNumberFormatter.create_superscript_instance(decfmt)
    assert isinstance(fmt, ScientificNumberFormatter)


def test_format():
    fmt = ScientificNumberFormatter.create_markup_instance(
        "en",
        "<sup>",
        "</sup>")

    # UnicodeString &icu::ScientificNumberFormatter::format(
    #       const Formattable &number,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = UnicodeString("String: ")
    result = fmt.format(Formattable(1.23456e-78), append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "String: 1.23456\u00d710<sup>-78</sup>"

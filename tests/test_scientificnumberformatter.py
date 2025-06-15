import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 55:
    pytest.skip("ICU4C<55", allow_module_level=True)

import copy


def test_clone() -> None:
    decfmt = icu.NumberFormat.create_scientific_instance("en")
    decfmt.apply_pattern("0.00E+0")
    fmt1 = icu.ScientificNumberFormatter.create_superscript_instance(decfmt)
    append_to = icu.UnicodeString()

    # ScientificNumberFormatter *icu::ScientificNumberFormatter::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.ScientificNumberFormatter)

    append_to.remove()
    fmt2.format(icu.Formattable(6.02e23), append_to)
    assert append_to == "6.02\u00d710\u207a\u00b2\u00b3"

    fmt3 = copy.copy(fmt1)
    append_to.remove()
    fmt3.format(icu.Formattable(6.02e23), append_to)
    assert append_to == "6.02\u00d710\u207a\u00b2\u00b3"

    fmt4 = copy.deepcopy(fmt1)
    append_to.remove()
    fmt4.format(icu.Formattable(6.02e23), append_to)
    assert append_to == "6.02\u00d710\u207a\u00b2\u00b3"


def test_create_markup_instance() -> None:
    # [1]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createMarkupInstance(
    #       const Locale &locale,
    #       const UnicodeString &beginMarkup,
    #       const UnicodeString &endMarkup,
    #       UErrorCode &status
    # )
    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        icu.Locale.get_english(),
        icu.UnicodeString("<sup>"),
        icu.UnicodeString("</sup>"),
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        "en", icu.UnicodeString("<sup>"), icu.UnicodeString("</sup>")
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        icu.Locale.get_english(), "<sup>", icu.UnicodeString("</sup>")
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        icu.Locale.get_english(), icu.UnicodeString("<sup>"), "</sup>"
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        "en", "<sup>", icu.UnicodeString("</sup>")
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        "en", icu.UnicodeString("<sup>"), "</sup>"
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        icu.Locale.get_english(), "<sup>", "</sup>"
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance("en", "<sup>", "</sup>")
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    decfmt = icu.NumberFormat.create_scientific_instance("en")
    decfmt.apply_pattern("0.00E+0")

    # [2]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createMarkupInstance(
    #       DecimalFormat *fmtToAdopt,
    #       const UnicodeString &beginMarkup,
    #       const UnicodeString &endMarkup,
    #       UErrorCode &status
    # )
    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        decfmt, icu.UnicodeString("<sup>"), icu.UnicodeString("</sup>")
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        decfmt, "<sup>", icu.UnicodeString("</sup>")
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(
        decfmt, icu.UnicodeString("<sup>"), "</sup>"
    )
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_markup_instance(decfmt, "<sup>", "</sup>")
    assert isinstance(fmt, icu.ScientificNumberFormatter)


def test_create_superscript_instance() -> None:
    # [1]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createSuperscriptInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt = icu.ScientificNumberFormatter.create_superscript_instance(icu.Locale.get_english())
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    fmt = icu.ScientificNumberFormatter.create_superscript_instance("en")
    assert isinstance(fmt, icu.ScientificNumberFormatter)

    decfmt = icu.NumberFormat.create_scientific_instance("en")
    decfmt.apply_pattern("0.00E+0")

    # [2]
    # static ScientificNumberFormatter *
    # icu::ScientificNumberFormatter::createSuperscriptInstance(
    #       DecimalFormat *fmtToAdopt,
    #       UErrorCode &status
    # )
    fmt = icu.ScientificNumberFormatter.create_superscript_instance(decfmt)
    assert isinstance(fmt, icu.ScientificNumberFormatter)


def test_format() -> None:
    fmt = icu.ScientificNumberFormatter.create_markup_instance("en", "<sup>", "</sup>")

    # UnicodeString &icu::ScientificNumberFormatter::format(
    #       const Formattable &number,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString("String: ")
    result = fmt.format(icu.Formattable(1.23456e-78), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "String: 1.23456\u00d710<sup>-78</sup>"

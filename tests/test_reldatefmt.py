import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 53:
    pytest.skip("ICU4C<53", allow_module_level=True)

from icupy.icu import (
    DecimalFormat, Locale, NumberFormat, RelativeDateTimeFormatter,
    UDateAbsoluteUnit, UDateDirection, UDateRelativeUnit, UnicodeString,
)


def test_api():
    fmt = RelativeDateTimeFormatter("en")

    # UnicodeString &icu::RelativeDateTimeFormatter::combineDateAndTime(
    #       const UnicodeString &relativeDateString,
    #       const UnicodeString &timeString,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    result = fmt.combine_date_and_time(
        UnicodeString("yesterday"),
        UnicodeString("3:50"),
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "yesterday, 3:50"

    append_to.remove()
    result = fmt.combine_date_and_time(
        "yesterday",
        UnicodeString("3:50"),
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "yesterday, 3:50"

    append_to.remove()
    result = fmt.combine_date_and_time(
        UnicodeString("yesterday"),
        "3:50",
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "yesterday, 3:50"

    append_to.remove()
    result = fmt.combine_date_and_time(
        "yesterday",
        "3:50",
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "yesterday, 3:50"

    # const NumberFormat &icu::RelativeDateTimeFormatter::getNumberFormat()
    numfmt = fmt.get_number_format()
    assert isinstance(numfmt, NumberFormat)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_api_54():
    from icupy.icu import UDateRelativeDateTimeFormatterStyle, UDisplayContext

    fmt = RelativeDateTimeFormatter(
        "en",
        None,
        UDateRelativeDateTimeFormatterStyle.UDAT_STYLE_NARROW,
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)

    # UDisplayContext
    # icu::RelativeDateTimeFormatter::getCapitalizationContext()
    result = fmt.get_capitalization_context()
    assert isinstance(result, UDisplayContext)
    assert (result
            == UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE)

    # UDateRelativeDateTimeFormatterStyle
    # icu::RelativeDateTimeFormatter::getFormatStyle()
    result = fmt.get_format_style()
    assert isinstance(result, UDateRelativeDateTimeFormatterStyle)
    assert result == UDateRelativeDateTimeFormatterStyle.UDAT_STYLE_NARROW


def test_format():
    fmt = RelativeDateTimeFormatter("en")

    # [2]
    # UnicodeString &icu::RelativeDateTimeFormatter::format(
    #       double quantity,
    #       UDateDirection direction,
    #       UDateRelativeUnit unit,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    result = fmt.format(
        0.5,
        UDateDirection.UDAT_DIRECTION_LAST,
        UDateRelativeUnit.UDAT_RELATIVE_SECONDS,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "0.5 seconds ago"

    append_to.remove()
    result = fmt.format(
        1,
        UDateDirection.UDAT_DIRECTION_LAST,
        UDateRelativeUnit.UDAT_RELATIVE_SECONDS,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1 second ago"

    # [3]
    # UnicodeString &icu::RelativeDateTimeFormatter::format(
    #       UDateDirection direction,
    #       UDateAbsoluteUnit unit,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(
        UDateDirection.UDAT_DIRECTION_NEXT,
        UDateAbsoluteUnit.UDAT_ABSOLUTE_DAY,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "tomorrow"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 57, reason="ICU4C<57")
def test_format_57():
    from icupy.icu import URelativeDateTimeUnit

    fmt = RelativeDateTimeFormatter("en")

    # [1]
    # UnicodeString &icu::RelativeDateTimeFormatter::format(
    #       double offset,
    #       URelativeDateTimeUnit unit,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    result = fmt.format(
        0.5,
        URelativeDateTimeUnit.UDAT_REL_UNIT_SECOND,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "in 0.5 seconds"

    append_to.remove()
    result = fmt.format(
        1,
        URelativeDateTimeUnit.UDAT_REL_UNIT_SECOND,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "in 1 second"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 57, reason="ICU4C<57")
def test_format_numeric():
    from icupy.icu import URelativeDateTimeUnit

    fmt = RelativeDateTimeFormatter("en-US")

    # UnicodeString &icu::RelativeDateTimeFormatter::formatNumeric(
    #       double offset,
    #       URelativeDateTimeUnit unit,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    result = fmt.format_numeric(
        0.5,
        URelativeDateTimeUnit.UDAT_REL_UNIT_SECOND,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "in 0.5 seconds"

    append_to.remove()
    result = fmt.format_numeric(
        1,
        URelativeDateTimeUnit.UDAT_REL_UNIT_SECOND,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "in 1 second"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_format_numeric_to_value():
    from icupy.icu import (
        ConstrainedFieldPosition, FormattedRelativeDateTime,
        FormattedValue, UFieldCategory, UNumberFormatFields,
        URelativeDateTimeFormatterField, URelativeDateTimeUnit,
        UnicodeStringAppendable,
    )

    assert issubclass(FormattedRelativeDateTime, FormattedValue)
    fmt = RelativeDateTimeFormatter("en-US")

    # FormattedRelativeDateTime
    # icu::RelativeDateTimeFormatter::formatNumericToValue(
    #       double offset,
    #       URelativeDateTimeUnit unit,
    #       UErrorCode &status
    # )
    fv = fmt.format_numeric_to_value(
        1.5,
        URelativeDateTimeUnit.UDAT_REL_UNIT_WEEK)
    assert isinstance(fv, FormattedRelativeDateTime)

    # Appendable &icu::FormattedRelativeDateTime::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = UnicodeString("x")
    appendable = UnicodeStringAppendable(s)
    result = fv.append_to(appendable)
    assert isinstance(result, UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "xin 1.5 weeks"

    # UBool icu::FormattedRelativeDateTime::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = ConstrainedFieldPosition()
    assert fv.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_RELATIVE_DATETIME)
    assert (cfpos.get_field()
            == URelativeDateTimeFormatterField.UDAT_REL_LITERAL_FIELD)
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 2)

    assert fv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (3, 4)

    assert fv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert (cfpos.get_field()
            == UNumberFormatFields.UNUM_DECIMAL_SEPARATOR_FIELD)
    assert (cfpos.get_start(), cfpos.get_limit()) == (4, 5)

    assert fv.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_FRACTION_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (5, 6)

    assert fv.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_RELATIVE_DATETIME)
    assert (cfpos.get_field()
            == URelativeDateTimeFormatterField.UDAT_REL_NUMERIC_FIELD)
    assert (cfpos.get_start(), cfpos.get_limit()) == (3, 6)

    assert fv.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_RELATIVE_DATETIME)
    assert (cfpos.get_field()
            == URelativeDateTimeFormatterField.UDAT_REL_LITERAL_FIELD)
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 12)

    assert not fv.next_position(cfpos)

    # UnicodeString icu::FormattedRelativeDateTime::toString(
    #       UErrorCode &status
    # )
    result = fv.to_string()
    assert isinstance(result, UnicodeString)
    assert result == "in 1.5 weeks"

    # UnicodeString icu::FormattedRelativeDateTime::toTempString(
    #       UErrorCode &status
    # )
    result = fv.to_temp_string()
    assert isinstance(result, UnicodeString)
    assert result == "in 1.5 weeks"

    fv = fmt.format_numeric_to_value(
        1,
        URelativeDateTimeUnit.UDAT_REL_UNIT_WEEK)
    assert isinstance(fv, FormattedRelativeDateTime)
    assert fv.to_temp_string() == "in 1 week"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_format_to_value():
    from icupy.icu import FormattedRelativeDateTime, URelativeDateTimeUnit

    fmt = RelativeDateTimeFormatter("en-US")

    # [1]
    # FormattedRelativeDateTime icu::RelativeDateTimeFormatter::formatToValue(
    #       double offset,
    #       URelativeDateTimeUnit unit,
    #       UErrorCode &status
    # )
    fv = fmt.format_to_value(1.0, URelativeDateTimeUnit.UDAT_REL_UNIT_DAY)
    assert isinstance(fv, FormattedRelativeDateTime)
    assert fv.to_temp_string() == "tomorrow"

    fv = fmt.format_to_value(3, URelativeDateTimeUnit.UDAT_REL_UNIT_DAY)
    assert isinstance(fv, FormattedRelativeDateTime)
    assert fv.to_temp_string() == "in 3 days"

    # [2]
    # FormattedRelativeDateTime icu::RelativeDateTimeFormatter::formatToValue(
    #       double quantity,
    #       UDateDirection direction,
    #       UDateRelativeUnit unit,
    #       UErrorCode &status
    # )
    fv = fmt.format_to_value(
        12.0,
        UDateDirection.UDAT_DIRECTION_LAST,
        UDateRelativeUnit.UDAT_RELATIVE_HOURS)
    assert isinstance(fv, FormattedRelativeDateTime)
    assert fv.to_temp_string() == "12 hours ago"

    fv = fmt.format_to_value(
        12,
        UDateDirection.UDAT_DIRECTION_LAST,
        UDateRelativeUnit.UDAT_RELATIVE_HOURS)
    assert isinstance(fv, FormattedRelativeDateTime)
    assert fv.to_temp_string() == "12 hours ago"

    # [3]
    # FormattedRelativeDateTime icu::RelativeDateTimeFormatter::formatToValue(
    #       UDateDirection direction,
    #       UDateAbsoluteUnit unit,
    #       UErrorCode &status
    # )
    fv = fmt.format_to_value(
        UDateDirection.UDAT_DIRECTION_NEXT,
        UDateAbsoluteUnit.UDAT_ABSOLUTE_MONDAY)
    assert isinstance(fv, FormattedRelativeDateTime)
    assert fv.to_temp_string() == "next Monday"


def test_relative_date_time_formatter():
    # [1]
    # icu::RelativeDateTimeFormatter::RelativeDateTimeFormatter(
    #       UErrorCode &status
    # )
    fmt1 = RelativeDateTimeFormatter()

    # [2]
    # icu::RelativeDateTimeFormatter::RelativeDateTimeFormatter(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    _ = RelativeDateTimeFormatter(Locale("en"))
    _ = RelativeDateTimeFormatter("en")

    # [3]
    # icu::RelativeDateTimeFormatter::RelativeDateTimeFormatter(
    #       const Locale &locale,
    #       NumberFormat *nfToAdopt,
    #       UErrorCode &status
    # )
    _ = RelativeDateTimeFormatter(Locale("en"), DecimalFormat("0"))
    _ = RelativeDateTimeFormatter(Locale("en"), None)
    _ = RelativeDateTimeFormatter("en", DecimalFormat("0"))
    _ = RelativeDateTimeFormatter("en", None)

    # [5]
    # icu::RelativeDateTimeFormatter::RelativeDateTimeFormatter(
    #       const RelativeDateTimeFormatter &other
    # )
    _ = RelativeDateTimeFormatter(fmt1)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_relative_date_time_formatter_54():
    from icupy.icu import UDateRelativeDateTimeFormatterStyle, UDisplayContext

    # [4]
    # icu::RelativeDateTimeFormatter::RelativeDateTimeFormatter(
    #       const Locale &locale,
    #       NumberFormat *nfToAdopt,
    #       UDateRelativeDateTimeFormatterStyle style,
    #       UDisplayContext capitalizationContext,
    #       UErrorCode &status
    # )
    _ = RelativeDateTimeFormatter(
        Locale("en"),
        DecimalFormat("0"),
        UDateRelativeDateTimeFormatterStyle.UDAT_STYLE_LONG,
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE)

    _ = RelativeDateTimeFormatter(
        Locale("en"),
        None,
        UDateRelativeDateTimeFormatterStyle.UDAT_STYLE_LONG,
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE)

    _ = RelativeDateTimeFormatter(
        "en",
        DecimalFormat("0"),
        UDateRelativeDateTimeFormatterStyle.UDAT_STYLE_LONG,
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE)

    _ = RelativeDateTimeFormatter(
        "en",
        None,
        UDateRelativeDateTimeFormatterStyle.UDAT_STYLE_LONG,
        UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE)

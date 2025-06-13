import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 63:
    pytest.skip("ICU4C<63", allow_module_level=True)

import copy


def test_formatted_number_range_63():
    fmt = (
        icu.number.NumberRangeFormatter.with_locale("en-US")
        .number_formatter_first(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
        )
        .number_formatter_second(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_kilometer())
        )
    )
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)

    num = fmt.format_formattable_range(icu.Formattable(3), icu.Formattable(3000))
    assert isinstance(num, icu.number.FormattedNumberRange)

    # Appendable &icu::number::FormattedNumberRange::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = icu.UnicodeString()
    appendable = icu.UnicodeStringAppendable(s)
    result = num.append_to(appendable)
    assert isinstance(result, icu.UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "3 m \u2013 3,000 km"  # 3 m – 3,000 km

    # UNumberRangeIdentityResult
    # icu::number::FormattedNumberRange::getIdentityResult(UErrorCode &status)
    assert (
        num.get_identity_result()
        == icu.UNumberRangeIdentityResult.UNUM_IDENTITY_RESULT_NOT_EQUAL
    )

    # UnicodeString icu::number::FormattedNumberRange::toString(
    #       UErrorCode &status
    # )
    result = num.to_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "3 m \u2013 3,000 km"  # 3 m – 3,000 km


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_formatted_number_range_64():
    assert issubclass(icu.number.FormattedNumberRange, icu.FormattedValue)

    # icu::number::FormattedNumberRange::FormattedNumberRange()
    num = icu.number.FormattedNumberRange()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = num.to_string()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_STATE_ERROR

    fmt = (
        icu.number.NumberRangeFormatter.with_locale("en-US")
        .number_formatter_first(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
        )
        .number_formatter_second(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_kilometer())
        )
    )
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)

    num = fmt.format_formattable_range(icu.Formattable(3), icu.Formattable(3000))
    assert isinstance(num, icu.number.FormattedNumberRange)

    # UnicodeString icu::number::FormattedNumberRange::toTempString(
    #       UErrorCode &status
    # )
    result = num.to_temp_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "3 m \u2013 3,000 km"  # 3 m – 3,000 km


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_formatted_number_range_68():
    fmt = (
        icu.number.NumberRangeFormatter.with_locale("en-US")
        .number_formatter_first(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
        )
        .number_formatter_second(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_kilometer())
        )
    )
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)

    num = fmt.format_formattable_range(icu.Formattable(3), icu.Formattable(3000))
    assert isinstance(num, icu.number.FormattedNumberRange)

    # template<typename StringClass>
    # std::pair<StringClass, StringClass>
    # icu::number::FormattedNumberRange::getDecimalNumbers(UErrorCode &status)
    pair = num.get_decimal_numbers()
    assert isinstance(pair, tuple)
    assert len(pair) == 2
    assert all(isinstance(x, str) for x in pair)
    assert pair[0] == "3"
    assert pair[1] == "3E+3"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_formatted_number_range_69():
    fmt = (
        icu.number.NumberRangeFormatter.with_locale("en-US")
        .number_formatter_first(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
        )
        .number_formatter_second(
            icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_kilometer())
        )
    )
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)

    # "3 m – 3,000 km"
    num = fmt.format_formattable_range(icu.Formattable(3), icu.Formattable(3000))
    assert isinstance(num, icu.number.FormattedNumberRange)

    # UBool icu::number::FormattedNumberRange::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = icu.ConstrainedFieldPosition()
    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER_RANGE_SPAN
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 3)  # "3 m"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 1)  # "3"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_MEASURE_UNIT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (2, 3)  # "m"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER_RANGE_SPAN
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_FRACTION_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (6, 14)  # "3,000 km"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_GROUPING_SEPARATOR_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 8)  # ","

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (6, 11)  # "3,000"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_MEASURE_UNIT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (12, 14)  # "km"

    assert not num.next_position(cfpos)


def test_localized_number_range_formatter_63():
    # [1]
    # icu::number::LocalizedNumberRangeFormatter::LocalizedNumberRangeFormatter()
    fmt = icu.number.LocalizedNumberRangeFormatter()

    # [2]
    # icu::number::LocalizedNumberRangeFormatter::LocalizedNumberRangeFormatter(
    #       const LocalizedNumberRangeFormatter &other
    # )
    fmt2 = icu.number.LocalizedNumberRangeFormatter(fmt)

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::collapse(
    #       UNumberRangeCollapse collapse
    # )
    result = fmt2.collapse(icu.UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_UNIT)
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    # template<typename Derived>
    # UBool icu::number::NumberRangeFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = icu.ErrorCode()
    assert fmt2.copy_error_to(out_error_code) is False
    assert out_error_code.get() == icu.UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::identityFallback(
    #       UNumberRangeIdentityFallback identityFallback
    # )
    result = fmt2.identity_fallback(
        icu.UNumberRangeIdentityFallback.UNUM_IDENTITY_FALLBACK_RANGE
    )
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterBoth(
    #       const UnlocalizedNumberFormatter &formatter
    # )
    result = fmt2.number_formatter_both(icu.number.NumberFormatter.with_())
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterFirst(
    #       const UnlocalizedNumberFormatter &formatterFirst
    # )
    result = fmt2.number_formatter_first(icu.number.NumberFormatter.with_())
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterSecond(
    #       const UnlocalizedNumberFormatter &formatterSecond
    # )
    result = fmt2.number_formatter_second(icu.number.NumberFormatter.with_())
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    # FormattedNumberRange
    # icu::number::LocalizedNumberRangeFormatter::formatFormattableRange(
    #       const Formattable &first,
    #       const Formattable &second,
    #       UErrorCode &status
    # )
    formatter = icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_kilometer())
    result = fmt2.number_formatter_both(formatter).format_formattable_range(
        icu.Formattable(4), icu.Formattable(6)
    )
    assert isinstance(result, icu.number.FormattedNumberRange)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_localized_number_range_formatter_64():
    formatter = icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
    fmt = (
        icu.number.NumberRangeFormatter.with_locale("en-US")
        .collapse(icu.UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_NONE)
        .number_formatter_both(formatter)
    )
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)
    assert (
        fmt.format_formattable_range(icu.Formattable(1), icu.Formattable(5)).to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberRangeFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, icu.number.LocalizedNumberRangeFormatter)
    assert (
        fmt2.format_formattable_range(icu.Formattable(1), icu.Formattable(5)).to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt3 = copy.copy(fmt)
    assert (
        fmt3.format_formattable_range(icu.Formattable(1), icu.Formattable(5)).to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt4 = copy.deepcopy(fmt)
    assert (
        fmt4.format_formattable_range(icu.Formattable(1), icu.Formattable(5)).to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 75, reason="ICU4C<75")
def test_localized_number_range_formatter_75():
    # UnlocalizedNumberRangeFormatter
    # icu::number::LocalizedNumberRangeFormatter::withoutLocale() const &
    unf2 = (
        icu.number.NumberRangeFormatter.with_()
        .identity_fallback(icu.UNumberRangeIdentityFallback.UNUM_IDENTITY_FALLBACK_RANGE)
        .locale("ar-EG")
        .without_locale()
    )
    assert isinstance(unf2, icu.number.UnlocalizedNumberRangeFormatter)
    res2 = unf2.locale("ja-JP").format_formattable_range(icu.Formattable(5), icu.Formattable(5))
    assert res2.to_temp_string() == "5\uff5e5"


def test_number_range_formatter():
    # static UnlocalizedNumberRangeFormatter
    # icu::number::NumberRangeFormatter::with()
    fmt = icu.number.NumberRangeFormatter.with_()
    assert isinstance(fmt, icu.number.UnlocalizedNumberRangeFormatter)

    # static LocalizedNumberRangeFormatter
    # icu::number::NumberRangeFormatter::withLocale(const Locale &locale)
    fmt = icu.number.NumberRangeFormatter.with_locale(icu.Locale.get_us())
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)

    fmt = icu.number.NumberRangeFormatter.with_locale("en-US")
    assert isinstance(fmt, icu.number.LocalizedNumberRangeFormatter)


def test_unlocalized_number_range_formatter_63():
    # [1]
    # icu::number::UnlocalizedNumberRangeFormatter::UnlocalizedNumberRangeFormatter()
    fmt = icu.number.UnlocalizedNumberRangeFormatter()

    # [2]
    # icu::number::UnlocalizedNumberRangeFormatter::UnlocalizedNumberRangeFormatter(
    #       const UnlocalizedNumberRangeFormatter &other
    # )
    fmt2 = icu.number.UnlocalizedNumberRangeFormatter(fmt)

    # LocalizedNumberRangeFormatter
    # icu::number::UnlocalizedNumberRangeFormatter::locale(
    #       const Locale &locale
    # )
    result = fmt2.locale(icu.Locale.get_us())
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    result = fmt2.locale("en-US")
    assert isinstance(result, icu.number.LocalizedNumberRangeFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::collapse(
    #       UNumberRangeCollapse collapse
    # )
    result = fmt2.collapse(icu.UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_UNIT)
    assert isinstance(result, icu.number.UnlocalizedNumberRangeFormatter)

    # template<typename Derived>
    # UBool icu::number::NumberRangeFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = icu.ErrorCode()
    assert fmt2.copy_error_to(out_error_code) is False
    assert out_error_code.get() == icu.UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::identityFallback(
    #       UNumberRangeIdentityFallback identityFallback
    # )
    result = fmt2.identity_fallback(
        icu.UNumberRangeIdentityFallback.UNUM_IDENTITY_FALLBACK_RANGE
    )
    assert isinstance(result, icu.number.UnlocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterBoth(
    #       const UnlocalizedNumberFormatter &formatter
    # )
    result = fmt2.number_formatter_both(icu.number.NumberFormatter.with_())
    assert isinstance(result, icu.number.UnlocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterFirst(
    #       const UnlocalizedNumberFormatter &formatterFirst
    # )
    result = fmt2.number_formatter_first(icu.number.NumberFormatter.with_())
    assert isinstance(result, icu.number.UnlocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterSecond(
    #       const UnlocalizedNumberFormatter &formatterSecond
    # )
    result = fmt2.number_formatter_second(icu.number.NumberFormatter.with_())
    assert isinstance(result, icu.number.UnlocalizedNumberRangeFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_unlocalized_number_range_formatter_64():
    formatter = icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
    fmt = (
        icu.number.NumberRangeFormatter.with_()
        .collapse(icu.UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_NONE)
        .number_formatter_both(formatter)
    )
    assert isinstance(fmt, icu.number.UnlocalizedNumberRangeFormatter)
    assert (
        fmt.locale("en-US")
        .format_formattable_range(icu.Formattable(1), icu.Formattable(5))
        .to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberRangeFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, icu.number.UnlocalizedNumberRangeFormatter)
    assert (
        fmt2.locale("en-US")
        .format_formattable_range(icu.Formattable(1), icu.Formattable(5))
        .to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt3 = copy.copy(fmt)
    assert (
        fmt3.locale("en-US")
        .format_formattable_range(icu.Formattable(1), icu.Formattable(5))
        .to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt4 = copy.deepcopy(fmt)
    assert (
        fmt4.locale("en-US")
        .format_formattable_range(icu.Formattable(1), icu.Formattable(5))
        .to_string()
    ) == "1 m \u2013 5 m"  # 1 m - 5 m

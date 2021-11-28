import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 63:
    pytest.skip("ICU4C<63", allow_module_level=True)

import copy

from icupy.icu import (
    ErrorCode, Formattable, Locale, MeasureUnit, UErrorCode,
    UNumberRangeCollapse, UNumberRangeIdentityFallback,
    UNumberRangeIdentityResult, UnicodeString, UnicodeStringAppendable,
)
from icupy.icu.number import (
    FormattedNumberRange, LocalizedNumberRangeFormatter, NumberFormatter,
    NumberRangeFormatter, UnlocalizedNumberRangeFormatter,
)


def test_formatted_number_range_63():
    fmt = (NumberRangeFormatter.with_locale("en-US")
           .number_formatter_first(NumberFormatter.with_()
                                   .unit(MeasureUnit.create_meter()))
           .number_formatter_second(NumberFormatter.with_()
                                    .unit(MeasureUnit.create_kilometer())))
    assert isinstance(fmt, LocalizedNumberRangeFormatter)

    num = fmt.format_formattable_range(Formattable(3), Formattable(3000))
    assert isinstance(num, FormattedNumberRange)

    # Appendable &icu::number::FormattedNumberRange::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = UnicodeString()
    appendable = UnicodeStringAppendable(s)
    result = num.append_to(appendable)
    assert isinstance(result, UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "3 m \u2013 3,000 km"  # 3 m – 3,000 km

    # UNumberRangeIdentityResult
    # icu::number::FormattedNumberRange::getIdentityResult(UErrorCode &status)
    assert (num.get_identity_result()
            == UNumberRangeIdentityResult.UNUM_IDENTITY_RESULT_NOT_EQUAL)

    # UnicodeString icu::number::FormattedNumberRange::toString(
    #       UErrorCode &status
    # )
    result = num.to_string()
    assert isinstance(result, UnicodeString)
    assert result == "3 m \u2013 3,000 km"  # 3 m – 3,000 km


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_formatted_number_range_64():
    from icupy.icu import FormattedValue

    assert issubclass(FormattedNumberRange, FormattedValue)

    fmt = (NumberRangeFormatter.with_locale("en-US")
           .number_formatter_first(NumberFormatter.with_()
                                   .unit(MeasureUnit.create_meter()))
           .number_formatter_second(NumberFormatter.with_()
                                    .unit(MeasureUnit.create_kilometer())))
    assert isinstance(fmt, LocalizedNumberRangeFormatter)

    num = fmt.format_formattable_range(Formattable(3), Formattable(3000))
    assert isinstance(num, FormattedNumberRange)

    # UnicodeString icu::number::FormattedNumberRange::toTempString(
    #       UErrorCode &status
    # )
    result = num.to_temp_string()
    assert isinstance(result, UnicodeString)
    assert result == "3 m \u2013 3,000 km"  # 3 m – 3,000 km


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_formatted_number_range_68():
    fmt = (NumberRangeFormatter.with_locale("en-US")
           .number_formatter_first(NumberFormatter.with_()
                                   .unit(MeasureUnit.create_meter()))
           .number_formatter_second(NumberFormatter.with_()
                                    .unit(MeasureUnit.create_kilometer())))
    assert isinstance(fmt, LocalizedNumberRangeFormatter)

    num = fmt.format_formattable_range(Formattable(3), Formattable(3000))
    assert isinstance(num, FormattedNumberRange)

    # template<typename StringClass>
    # std::pair<StringClass, StringClass>
    # icu::number::FormattedNumberRange::getDecimalNumbers(UErrorCode &status)
    pair = num.get_decimal_numbers()
    assert isinstance(pair, tuple)
    assert len(pair) == 2
    assert all(isinstance(x, str) for x in pair)
    assert pair[0] == "3"
    assert pair[1] == "3E+3"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_formatted_number_range_69():
    from icupy.icu import (
        ConstrainedFieldPosition, UFieldCategory, UNumberFormatFields,
    )

    fmt = (NumberRangeFormatter.with_locale("en-US")
           .number_formatter_first(NumberFormatter.with_()
                                   .unit(MeasureUnit.create_meter()))
           .number_formatter_second(NumberFormatter.with_()
                                    .unit(MeasureUnit.create_kilometer())))
    assert isinstance(fmt, LocalizedNumberRangeFormatter)

    # "3 m – 3,000 km"
    num = fmt.format_formattable_range(Formattable(3), Formattable(3000))
    assert isinstance(num, FormattedNumberRange)

    # UBool icu::number::FormattedNumberRange::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = ConstrainedFieldPosition()
    assert num.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_NUMBER_RANGE_SPAN)
    assert cfpos.get_field() == UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 3)  # "3 m"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 1)  # "3"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_MEASURE_UNIT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (2, 3)  # "m"

    assert num.next_position(cfpos)
    assert (cfpos.get_category()
            == UFieldCategory.UFIELD_CATEGORY_NUMBER_RANGE_SPAN)
    assert cfpos.get_field() == UNumberFormatFields.UNUM_FRACTION_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (6, 14)  # "3,000 km"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert (cfpos.get_field()
            == UNumberFormatFields.UNUM_GROUPING_SEPARATOR_FIELD)
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 8)  # ","

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (6, 11)  # "3,000"

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_MEASURE_UNIT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (12, 14)  # "km"

    assert not num.next_position(cfpos)


def test_localized_number_range_formatter_63():
    # [1]
    # icu::number::LocalizedNumberRangeFormatter::LocalizedNumberRangeFormatter()
    fmt = LocalizedNumberRangeFormatter()

    # [2]
    # icu::number::LocalizedNumberRangeFormatter::LocalizedNumberRangeFormatter(
    #       const LocalizedNumberRangeFormatter &other
    # )
    fmt2 = LocalizedNumberRangeFormatter(fmt)

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::collapse(
    #       UNumberRangeCollapse collapse
    # )
    result = fmt2.collapse(UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_UNIT)
    assert isinstance(result, LocalizedNumberRangeFormatter)

    # template<typename Derived>
    # UBool icu::number::NumberRangeFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = ErrorCode()
    assert not fmt2.copy_error_to(out_error_code)
    assert out_error_code.get() == UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::identityFallback(
    #       UNumberRangeIdentityFallback identityFallback
    # )
    result = fmt2.identity_fallback(
        UNumberRangeIdentityFallback.UNUM_IDENTITY_FALLBACK_RANGE)
    assert isinstance(result, LocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterBoth(
    #       const UnlocalizedNumberFormatter &formatter
    # )
    result = fmt2.number_formatter_both(NumberFormatter.with_())
    assert isinstance(result, LocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterFirst(
    #       const UnlocalizedNumberFormatter &formatterFirst
    # )
    result = fmt2.number_formatter_first(NumberFormatter.with_())
    assert isinstance(result, LocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterSecond(
    #       const UnlocalizedNumberFormatter &formatterSecond
    # )
    result = fmt2.number_formatter_second(NumberFormatter.with_())
    assert isinstance(result, LocalizedNumberRangeFormatter)

    # FormattedNumberRange
    # icu::number::LocalizedNumberRangeFormatter::formatFormattableRange(
    #       const Formattable &first,
    #       const Formattable &second,
    #       UErrorCode &status
    # )
    formatter = NumberFormatter.with_().unit(MeasureUnit.create_kilometer())
    result = (fmt2.number_formatter_both(formatter)
              .format_formattable_range(Formattable(4), Formattable(6)))
    assert isinstance(result, FormattedNumberRange)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_localized_number_range_formatter_64():
    formatter = NumberFormatter.with_().unit(MeasureUnit.create_meter())
    fmt = (NumberRangeFormatter.with_locale("en-US")
           .collapse(UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_NONE)
           .number_formatter_both(formatter))
    assert isinstance(fmt, LocalizedNumberRangeFormatter)
    assert (fmt.format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberRangeFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, LocalizedNumberRangeFormatter)
    assert (fmt2.format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt3 = copy.copy(fmt)
    assert (fmt3.format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt4 = copy.deepcopy(fmt)
    assert (fmt4.format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m


def test_number_range_formatter():
    # static UnlocalizedNumberRangeFormatter
    # icu::number::NumberRangeFormatter::with()
    fmt = NumberRangeFormatter.with_()
    assert isinstance(fmt, UnlocalizedNumberRangeFormatter)

    # static LocalizedNumberRangeFormatter
    # icu::number::NumberRangeFormatter::withLocale(const Locale &locale)
    fmt = NumberRangeFormatter.with_locale(Locale.get_us())
    assert isinstance(fmt, LocalizedNumberRangeFormatter)

    fmt = NumberRangeFormatter.with_locale("en-US")
    assert isinstance(fmt, LocalizedNumberRangeFormatter)


def test_unlocalized_number_range_formatter_63():
    # [1]
    # icu::number::UnlocalizedNumberRangeFormatter::UnlocalizedNumberRangeFormatter()
    fmt = UnlocalizedNumberRangeFormatter()

    # [2]
    # icu::number::UnlocalizedNumberRangeFormatter::UnlocalizedNumberRangeFormatter(
    #       const UnlocalizedNumberRangeFormatter &other
    # )
    fmt2 = UnlocalizedNumberRangeFormatter(fmt)

    # LocalizedNumberRangeFormatter
    # icu::number::UnlocalizedNumberRangeFormatter::locale(
    #       const Locale &locale
    # )
    result = fmt2.locale(Locale.get_us())
    assert isinstance(result, LocalizedNumberRangeFormatter)

    result = fmt2.locale("en-US")
    assert isinstance(result, LocalizedNumberRangeFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::collapse(
    #       UNumberRangeCollapse collapse
    # )
    result = fmt2.collapse(UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_UNIT)
    assert isinstance(result, UnlocalizedNumberRangeFormatter)

    # template<typename Derived>
    # UBool icu::number::NumberRangeFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = ErrorCode()
    assert not fmt2.copy_error_to(out_error_code)
    assert out_error_code.get() == UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::identityFallback(
    #       UNumberRangeIdentityFallback identityFallback
    # )
    result = fmt2.identity_fallback(
        UNumberRangeIdentityFallback.UNUM_IDENTITY_FALLBACK_RANGE)
    assert isinstance(result, UnlocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterBoth(
    #       const UnlocalizedNumberFormatter &formatter
    # )
    result = fmt2.number_formatter_both(NumberFormatter.with_())
    assert isinstance(result, UnlocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterFirst(
    #       const UnlocalizedNumberFormatter &formatterFirst
    # )
    result = fmt2.number_formatter_first(NumberFormatter.with_())
    assert isinstance(result, UnlocalizedNumberRangeFormatter)

    # [2]
    # template<typename Derived>
    # Derived icu::number::NumberRangeFormatterSettings<Derived>::numberFormatterSecond(
    #       const UnlocalizedNumberFormatter &formatterSecond
    # )
    result = fmt2.number_formatter_second(NumberFormatter.with_())
    assert isinstance(result, UnlocalizedNumberRangeFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_unlocalized_number_range_formatter_64():
    formatter = NumberFormatter.with_().unit(MeasureUnit.create_meter())
    fmt = (NumberRangeFormatter.with_()
           .collapse(UNumberRangeCollapse.UNUM_RANGE_COLLAPSE_NONE)
           .number_formatter_both(formatter))
    assert isinstance(fmt, UnlocalizedNumberRangeFormatter)
    assert (fmt.locale("en-US")
            .format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberRangeFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, UnlocalizedNumberRangeFormatter)
    assert (fmt2.locale("en-US")
            .format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt3 = copy.copy(fmt)
    assert (fmt3.locale("en-US")
            .format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

    fmt4 = copy.deepcopy(fmt)
    assert (fmt4.locale("en-US")
            .format_formattable_range(Formattable(1), Formattable(5))
            .to_string()) == "1 m \u2013 5 m"  # 1 m - 5 m

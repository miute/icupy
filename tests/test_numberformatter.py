import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 60:
    pytest.skip("ICU4C<60", allow_module_level=True)

import copy


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_formatted_number_62():
    fmt = (
        icu.number.NumberFormatter.with_()
        .notation(icu.number.Notation.compact_short())
        .unit(icu.MeasureUnit.get_kelvin())
        .locale("en-US")
    )
    num = fmt.format_double(65000)
    assert isinstance(num, icu.number.FormattedNumber)

    # Appendable &icu::number::FormattedNumber::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = icu.UnicodeString()
    appendable = icu.UnicodeStringAppendable(s)
    result = num.append_to(appendable)
    assert isinstance(result, icu.UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "65K K"

    # UnicodeString icu::number::FormattedNumber::toString(UErrorCode &status)
    result = num.to_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "65K K"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_formatted_number_64():
    assert issubclass(icu.number.FormattedNumber, icu.FormattedValue)

    # icu::number::FormattedNumber::FormattedNumber()
    num = icu.number.FormattedNumber()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = num.to_string()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_STATE_ERROR

    fmt = (
        icu.number.NumberFormatter.with_()
        .notation(icu.number.Notation.compact_short())
        .unit(icu.MeasureUnit.get_kelvin())
        .locale("en-US")
    )
    num = fmt.format_double(65000)
    assert isinstance(num, icu.number.FormattedNumber)

    # UBool icu::number::FormattedNumber::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = icu.ConstrainedFieldPosition()
    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 2)

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_COMPACT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (2, 3)

    assert num.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == icu.UNumberFormatFields.UNUM_MEASURE_UNIT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (4, 5)

    assert not num.next_position(cfpos)

    # UnicodeString icu::number::FormattedNumber::toTempString(
    #       UErrorCode &status
    # )
    result = num.to_temp_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "65K K"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 65, reason="ICU4C<65")
def test_formatted_number_65():
    fmt = (
        icu.number.NumberFormatter.with_()
        .notation(icu.number.Notation.compact_short())
        .unit(icu.MeasureUnit.get_kelvin())
        .locale("en-US")
    )
    num = fmt.format_double(65000)
    assert isinstance(num, icu.number.FormattedNumber)

    # template<typename StringClass>
    # StringClass icu::number::FormattedNumber::toDecimalNumber(
    #       UErrorCode &status
    # )
    result = num.to_decimal_number()
    assert isinstance(result, str)
    assert result == "65"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_formatted_number_68():
    fmt = (
        icu.number.NumberFormatter.with_()
        .usage("person")
        .unit(icu.MeasureUnit.get_kilogram())
        .locale("en-GB")
    )
    num = fmt.format_double(80)
    assert isinstance(num, icu.number.FormattedNumber)

    # MeasureUnit icu::number::FormattedNumber::getOutputUnit(
    #       UErrorCode &status
    # )
    unit = num.get_output_unit()
    assert isinstance(unit, icu.MeasureUnit)
    assert unit.get_identifier() == "stone-and-pound"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM != 71, reason="ICU4C!=71")
def test_formatted_number_get_noun_class_71():
    fmt = (
        icu.number.NumberFormatter.with_()
        .unit(icu.MeasureUnit.for_identifier("cubic-meter"))
        .locale("fr")
    )
    num = fmt.format_double(1.1)
    assert isinstance(num, icu.number.FormattedNumber)

    # NounClass icu::number::FormattedNumber::getNounClass(UErrorCode &status)
    assert num.get_noun_class() == icu.NounClass.MASCULINE


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_formatted_number_get_noun_class_72():
    fmt = (
        icu.number.NumberFormatter.with_()
        .unit(icu.MeasureUnit.for_identifier("cubic-meter"))
        .locale("fr")
    )
    num = fmt.format_double(1.1)
    assert isinstance(num, icu.number.FormattedNumber)

    # UDisplayOptionsNounClass
    # icu::number::FormattedNumber::getNounClass(UErrorCode &status)
    assert num.get_noun_class() == icu.UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_MASCULINE


def test_integer_width():
    # icu::number::IntegerWidth
    assert isinstance(icu.number.IntegerWidth.zero_fill_to(3), icu.number.IntegerWidth)
    assert isinstance(
        icu.number.IntegerWidth.zero_fill_to(3).truncate_at(3),
        icu.number.IntegerWidth,
    )


def test_localized_number_formatter_60():
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)

    # [2]
    # icu::number::LocalizedNumberFormatter::LocalizedNumberFormatter(
    #       const LocalizedNumberFormatter &other
    # )
    fmt2 = icu.number.LocalizedNumberFormatter(fmt)

    # FormattedNumber icu::number::LocalizedNumberFormatter::formatDecimal(
    #       StringPiece value,
    #       UErrorCode &status
    # )
    num = fmt2.format_decimal("9.87E-3")
    assert isinstance(num, icu.number.FormattedNumber)

    # FormattedNumber icu::number::LocalizedNumberFormatter::formatDouble(
    #       double value,
    #       UErrorCode &status
    # )
    num = fmt2.format_double(9.87e-3)
    assert isinstance(num, icu.number.FormattedNumber)

    # FormattedNumber icu::number::LocalizedNumberFormatter::formatInt(
    #       int64_t value,
    #       UErrorCode &status
    # )
    num = fmt2.format_int(icu.INT32_MAX + 1)
    assert isinstance(num, icu.number.FormattedNumber)

    # UBool icu::number::NumberFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = icu.ErrorCode()
    assert fmt2.copy_error_to(out_error_code) is False
    assert out_error_code.get() == icu.UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::decimal(
    #       UNumberDecimalSeparatorDisplay style
    # )
    result = fmt2.decimal(icu.UNumberDecimalSeparatorDisplay.UNUM_DECIMAL_SEPARATOR_AUTO)
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::integerWidth(
    #       const IntegerWidth &style
    # )
    result = fmt2.integer_width(icu.number.IntegerWidth.zero_fill_to(2))
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::notation(
    #       const Notation &notation
    # )
    result = fmt2.notation(icu.number.Notation.compact_long())
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::symbols(
    #       const DecimalFormatSymbols &symbols
    # )
    result = fmt.symbols(icu.DecimalFormatSymbols(icu.Locale.get_us()))
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unit(
    #       const icu::MeasureUnit &unit
    # )
    result = fmt.unit(icu.MeasureUnit.create_meter_per_second())
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unitWidth(
    #       UNumberUnitWidth width
    # )
    result = fmt.unit_width(icu.UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
    assert isinstance(result, icu.number.LocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 61, reason="ICU4C<61")
def test_localized_number_formatter_61():
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::grouping(
    #       UNumberGroupingStrategy strategy
    # )
    result = fmt.grouping(icu.UNumberGroupingStrategy.UNUM_GROUPING_MIN2)
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::perUnit(
    #       const icu::MeasureUnit &perUnit
    # )
    result = fmt.per_unit(icu.MeasureUnit.get_meter())
    assert isinstance(result, icu.number.LocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_localized_number_formatter_62():
    # [1]
    # icu::number::LocalizedNumberFormatter::LocalizedNumberFormatter()
    fmt = icu.number.LocalizedNumberFormatter()

    # Format *icu::number::LocalizedNumberFormatter::toFormat(
    #       UErrorCode &status
    # )
    fmt2 = (
        fmt.notation(icu.number.Notation.engineering())
        .unit(icu.MeasureUnit.get_kelvin())
        .to_format()
    )
    assert isinstance(fmt2, icu.Format)

    obj = icu.Formattable(65000)
    append_to = icu.UnicodeString()
    fmt2.format(obj, append_to)
    assert append_to == "65E3 K"

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::precision(
    #       const Precision &precision
    # )
    result = fmt.precision(icu.number.Precision.fixed_fraction(2))
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::roundingMode(
    #       UNumberFormatRoundingMode roundingMode
    # )
    result = fmt.rounding_mode(icu.UNumberFormatRoundingMode.UNUM_ROUND_HALFEVEN)
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::scale(
    #       const Scale &scale
    # )
    result = fmt.scale(icu.number.Scale.power_of_ten(2))
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::sign(
    #       UNumberSignDisplay style
    # )
    result = fmt.sign(icu.UNumberSignDisplay.UNUM_SIGN_AUTO)
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # UnicodeString icu::number::NumberFormatterSettings<Derived>::toSkeleton(
    #       UErrorCode &status
    # )
    result = fmt.unit(icu.MeasureUnit.get_meter_per_second()).to_skeleton()
    assert isinstance(result, icu.UnicodeString)
    assert result in (
        "unit/meter-per-second",
        "measure-unit/speed-meter-per-second",
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_localized_number_formatter_64():
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us()).notation(
        icu.number.Notation.engineering()
    )
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)
    assert fmt.format_double(0.8765).to_string() == "876.5E-3"

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, icu.number.LocalizedNumberFormatter)
    assert fmt2.format_double(0.8765).to_string() == "876.5E-3"

    fmt3 = copy.copy(fmt)
    assert fmt3.format_double(0.8765).to_string() == "876.5E-3"

    fmt4 = copy.deepcopy(fmt)
    assert fmt4.format_double(0.8765).to_string() == "876.5E-3"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_localized_number_formatter_68():
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::usage(
    #       StringPiece usage
    # )
    result = fmt.usage("person")
    assert isinstance(result, icu.number.LocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_localized_number_formatter_72():
    bld = icu.DisplayOptions.builder()
    display_options = bld.set_grammatical_case(
        icu.UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_DATIVE
    ).build()

    # icu::number::LocalizedNumberFormatter
    # icu::number::NumberFormatterSettings<LocalizedNumberFormatter>::displayOptions(
    #       const DisplayOptions &displayOptions
    # )
    fmt = (
        icu.number.NumberFormatter.with_locale("de")
        .unit(icu.MeasureUnit.for_identifier("meter-and-centimeter"))
        .unit_width(icu.UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
        .display_options(display_options)
    )
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)

    num = fmt.format_double(2.1)
    assert num.to_temp_string() == "2 Metern, 10 Zentimetern"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 75, reason="ICU4C<75")
def test_localized_number_formatter_75():
    # UnlocalizedNumberFormatter
    # icu::number::LocalizedNumberFormatter::withoutLocale() const &
    lnf2 = (
        icu.number.NumberFormatter.with_()
        .notation(icu.number.Notation.compact_long())
        .locale("fr")
        .unit_width(icu.UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
    )
    unf2 = lnf2.without_locale()
    assert isinstance(unf2, icu.number.UnlocalizedNumberFormatter)
    f = unf2.unit(icu.MeasureUnit.create_meter())
    # l1 = f.threshold(0).locale("ja-JP")
    l1 = f.locale("ja-JP")
    result1 = l1.format_double(2)
    actual1 = result1.to_string()
    assert actual1 == "2 メートル"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_localized_number_formatter_to_format():
    # [1]
    # icu::number::LocalizedNumberFormatter::LocalizedNumberFormatter()
    fmt = icu.number.LocalizedNumberFormatter()

    # Format *icu::number::LocalizedNumberFormatter::toFormat(
    #       UErrorCode &status
    # )
    fmt2 = (
        fmt.notation(icu.number.Notation.engineering())
        .unit(icu.MeasureUnit.get_kelvin())
        .to_format()
    )
    assert isinstance(fmt2, icu.Format)

    # bool icu::Format::operator!=(const Format &other)
    fmt3 = fmt2.clone()
    assert fmt2 != fmt
    assert not (fmt2 != fmt3)

    # bool icu::Format::operator==(const Format &other)
    assert not (fmt2 == fmt)
    assert fmt2 == fmt3

    # [1]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(65000)
    append_to = icu.UnicodeString()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt2.format(obj, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    # [2]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(65000)
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    result = fmt2.format(obj, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    append_to.remove()
    pos_iter = None
    result = fmt2.format(obj, append_to, pos_iter)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(65000)
    append_to.remove()
    result = fmt2.format(obj, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    # [1]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    source = icu.UnicodeString("65E3 K")
    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt2.parse_object(source, result, parse_pos)
    assert parse_pos.get_index() == 0
    assert parse_pos.get_error_index() == 0

    source = "65E3 K"
    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt2.parse_object(source, result, parse_pos)
    assert parse_pos.get_index() == 0
    assert parse_pos.get_error_index() == 0

    # [2]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    source = icu.UnicodeString("65E3 K")
    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt2.parse_object(source, result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    source = "65E3 K"
    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt2.parse_object(source, result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_no_unit():
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale.get_us())

    unit = icu.NoUnit.base()
    assert isinstance(unit, icu.MeasureUnit)
    assert fmt.unit(unit).format_int(1).to_string() == "1"

    unit = icu.NoUnit.percent()
    assert isinstance(unit, icu.MeasureUnit)
    assert fmt.unit(unit).format_int(1).to_string() == "1%"

    unit = icu.NoUnit.permille()
    assert isinstance(unit, icu.MeasureUnit)
    assert fmt.unit(unit).format_int(1).to_string() == "1\u2030"  # 1‰


def test_number_formatter_60():
    # static UnlocalizedNumberFormatter icu::number::NumberFormatter::with()
    fmt = icu.number.NumberFormatter.with_()
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    # static LocalizedNumberFormatter icu::number::NumberFormatter::withLocale(
    #       const Locale &locale
    # )
    fmt = icu.number.NumberFormatter.with_locale(icu.Locale("en_US"))
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)

    fmt = icu.number.NumberFormatter.with_locale("en_US")
    assert isinstance(fmt, icu.number.LocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_number_formatter_62():
    # [1]
    # static UnlocalizedNumberFormatter
    # icu::number::NumberFormatter::forSkeleton(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    fmt = icu.number.NumberFormatter.for_skeleton(
        icu.UnicodeString("measure-unit/speed-meter-per-second")
    )
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    fmt = icu.number.NumberFormatter.for_skeleton("measure-unit/speed-meter-per-second")
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_number_formatter_64():
    # [2]
    # static UnlocalizedNumberFormatter
    # icu::number::NumberFormatter::forSkeleton(
    #       const UnicodeString &skeleton,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = icu.UParseError()
    fmt = icu.number.NumberFormatter.for_skeleton(
        icu.UnicodeString("measure-unit/speed-meter-per-second"), perror
    )
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    perror = icu.UParseError()
    fmt = icu.number.NumberFormatter.for_skeleton("measure-unit/speed-meter-per-second", perror)
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)


def test_notation():
    # icu::number::Notation
    assert isinstance(icu.number.Notation.compact_long(), icu.number.Notation)
    assert isinstance(icu.number.Notation.compact_short(), icu.number.Notation)
    assert isinstance(icu.number.Notation.engineering(), icu.number.ScientificNotation)
    assert isinstance(icu.number.Notation.scientific(), icu.number.ScientificNotation)
    assert isinstance(icu.number.Notation.simple(), icu.number.Notation)

    # icu::number::ScientificNotation
    assert issubclass(icu.number.ScientificNotation, icu.number.Notation)

    assert isinstance(
        (
            icu.number.Notation.engineering().with_exponent_sign_display(
                icu.UNumberSignDisplay.UNUM_SIGN_ALWAYS
            )
        ),
        icu.number.ScientificNotation,
    )
    assert isinstance(
        icu.number.Notation.engineering().with_min_exponent_digits(2),
        icu.number.ScientificNotation,
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_precision_60_62():
    # icu::number::Precision
    assert isinstance(
        icu.number.Precision.currency(icu.UCurrencyUsage.UCURR_USAGE_STANDARD),
        icu.number.CurrencyPrecision,
    )
    assert isinstance(icu.number.Precision.fixed_fraction(3), icu.number.FractionPrecision)
    assert isinstance(icu.number.Precision.increment(0.5), icu.number.IncrementPrecision)
    assert isinstance(icu.number.Precision.integer(), icu.number.FractionPrecision)
    assert isinstance(icu.number.Precision.max_fraction(3), icu.number.FractionPrecision)
    assert isinstance(icu.number.Precision.min_fraction(1), icu.number.FractionPrecision)
    assert isinstance(
        icu.number.Precision.min_max_fraction(1, 3),
        icu.number.FractionPrecision,
    )
    assert isinstance(icu.number.Precision.unlimited(), icu.number.Precision)

    # icu::number::CurrencyPrecision
    assert issubclass(icu.number.CurrencyPrecision, icu.number.Precision)

    assert isinstance(
        (
            icu.number.Precision.currency(
                icu.UCurrencyUsage.UCURR_USAGE_STANDARD
            ).with_currency(icu.CurrencyUnit("USD"))
        ),
        icu.number.Precision,
    )

    # icu::number::FractionPrecision
    assert issubclass(icu.number.FractionPrecision, icu.number.Precision)

    assert isinstance(
        icu.number.Precision.fixed_fraction(2).with_max_digits(2),
        icu.number.Precision,
    )
    assert isinstance(
        icu.number.Precision.fixed_fraction(2).with_min_digits(3),
        icu.number.Precision,
    )

    # icu::number::IncrementPrecision
    assert issubclass(icu.number.IncrementPrecision, icu.number.Precision)

    assert isinstance(
        icu.number.Precision.increment(0.5).with_min_fraction(1),
        icu.number.Precision,
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_precision_62():
    # icu::number::Precision
    assert isinstance(icu.number.Precision.fixed_significant_digits(3), icu.number.Precision)
    assert isinstance(icu.number.Precision.max_significant_digits(4), icu.number.Precision)
    assert isinstance(
        icu.number.Precision.min_max_significant_digits(3, 4),
        icu.number.Precision,
    )
    assert isinstance(icu.number.Precision.min_significant_digits(3), icu.number.Precision)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_precision_69():
    # icu::number::Precision
    assert isinstance(
        (
            icu.number.Precision.fixed_fraction(2).trailing_zero_display(
                icu.UNumberTrailingZeroDisplay.UNUM_TRAILING_ZERO_AUTO
            )
        ),
        icu.number.Precision,
    )

    # icu::number::FractionPrecision
    assert isinstance(
        (
            icu.number.Precision.fixed_fraction(1).with_significant_digits(
                3,
                3,
                icu.UNumberRoundingPriority.UNUM_ROUNDING_PRIORITY_RELAXED,
            )
        ),
        icu.number.Precision,
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 71, reason="ICU4C<71")
def test_precision_71():
    # static IncrementPrecision icu::number::Precision::incrementExact(
    #       uint64_t mantissa,
    #       int16_t magnitude
    # )
    assert isinstance(
        icu.number.Precision.increment_exact(50, -2),
        icu.number.IncrementPrecision,
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_scale():
    # icu::number::Scale
    assert isinstance(icu.number.Scale.by_decimal("5.2"), icu.number.Scale)
    assert isinstance(icu.number.Scale.by_double(5.2), icu.number.Scale)
    assert isinstance(icu.number.Scale.by_double_and_power_of_ten(5.2, 3), icu.number.Scale)
    assert isinstance(icu.number.Scale.none(), icu.number.Scale)
    assert isinstance(icu.number.Scale.power_of_ten(2), icu.number.Scale)


def test_unlocalized_number_formatter_60():
    fmt = icu.number.NumberFormatter.with_()
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    # [2]
    # icu::number::UnlocalizedNumberFormatter::UnlocalizedNumberFormatter(
    #       const UnlocalizedNumberFormatter &other
    # )
    fmt2 = icu.number.UnlocalizedNumberFormatter(fmt)

    # LocalizedNumberFormatter icu::number::UnlocalizedNumberFormatter::locale(
    #       const icu::Locale &locale
    # )
    result = fmt2.locale(icu.Locale.get_us())
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    result = fmt2.locale("en-US")
    assert isinstance(result, icu.number.LocalizedNumberFormatter)

    # UBool icu::number::NumberFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = icu.ErrorCode()
    assert fmt2.copy_error_to(out_error_code) is False
    assert out_error_code.get() == icu.UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::decimal(
    #       UNumberDecimalSeparatorDisplay style
    # )
    result = fmt2.decimal(icu.UNumberDecimalSeparatorDisplay.UNUM_DECIMAL_SEPARATOR_AUTO)
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::integerWidth(
    #       const IntegerWidth &style
    # )
    result = fmt2.integer_width(icu.number.IntegerWidth.zero_fill_to(2))
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::notation(
    #       const Notation &notation
    # )
    result = fmt2.notation(icu.number.Notation.compact_long())
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::symbols(
    #       const DecimalFormatSymbols &symbols
    # )
    result = fmt.symbols(icu.DecimalFormatSymbols(icu.Locale.get_us()))
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unit(
    #       const icu::MeasureUnit &unit
    # )
    result = fmt.unit(icu.MeasureUnit.create_meter_per_second())
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unitWidth(
    #       UNumberUnitWidth width
    # )
    result = fmt.unit_width(icu.UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 61, reason="ICU4C<61")
def test_unlocalized_number_formatter_61():
    fmt = icu.number.NumberFormatter.with_()
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::grouping(
    #       UNumberGroupingStrategy strategy
    # )
    result = fmt.grouping(icu.UNumberGroupingStrategy.UNUM_GROUPING_MIN2)
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::perUnit(
    #       const icu::MeasureUnit &perUnit
    # )
    result = fmt.per_unit(icu.MeasureUnit.get_meter())
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_unlocalized_number_formatter_62():
    # [1]
    # icu::number::UnlocalizedNumberFormatter::UnlocalizedNumberFormatter()
    fmt = icu.number.UnlocalizedNumberFormatter()

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::precision(
    #       const Precision &precision
    # )
    result = fmt.precision(icu.number.Precision.fixed_fraction(2))
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::roundingMode(
    #       UNumberFormatRoundingMode roundingMode
    # )
    result = fmt.rounding_mode(icu.UNumberFormatRoundingMode.UNUM_ROUND_HALFEVEN)
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::scale(
    #       const Scale &scale
    # )
    result = fmt.scale(icu.number.Scale.power_of_ten(2))
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::sign(
    #       UNumberSignDisplay style
    # )
    result = fmt.sign(icu.UNumberSignDisplay.UNUM_SIGN_AUTO)
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # UnicodeString icu::number::NumberFormatterSettings<Derived>::toSkeleton(
    #       UErrorCode &status
    # )
    result = fmt.unit(icu.MeasureUnit.get_meter_per_second()).to_skeleton()
    assert isinstance(result, icu.UnicodeString)
    assert result in (
        "unit/meter-per-second",
        "measure-unit/speed-meter-per-second",
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_unlocalized_number_formatter_64():
    fmt = icu.number.NumberFormatter.with_().notation(icu.number.Notation.engineering())
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)
    assert fmt.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, icu.number.UnlocalizedNumberFormatter)
    assert fmt2.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"

    fmt3 = copy.copy(fmt)
    assert fmt3.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"

    fmt4 = copy.deepcopy(fmt)
    assert fmt4.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_unlocalized_number_formatter_68():
    fmt = icu.number.NumberFormatter.with_()
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::usage(
    #       StringPiece usage
    # )
    result = fmt.usage("person")
    assert isinstance(result, icu.number.UnlocalizedNumberFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_unlocalized_number_formatter_72():
    bld = icu.DisplayOptions.builder()
    display_options = bld.set_grammatical_case(
        icu.UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_DATIVE
    ).build()

    # icu::number::UnlocalizedNumberFormatter
    # icu::number::NumberFormatterSettings<UnlocalizedNumberFormatter>::displayOptions(
    #       const DisplayOptions &displayOptions
    # )
    fmt = (
        icu.number.NumberFormatter.with_()
        .unit(icu.MeasureUnit.for_identifier("meter-and-centimeter"))
        .unit_width(icu.UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
        .display_options(display_options)
    )
    assert isinstance(fmt, icu.number.UnlocalizedNumberFormatter)

    num = fmt.locale("de").format_double(2.1)
    assert num.to_temp_string() == "2 Metern, 10 Zentimetern"

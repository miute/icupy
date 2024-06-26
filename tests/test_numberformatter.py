import pytest

from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 60:
    pytest.skip("ICU4C<60", allow_module_level=True)

import copy

# fmt: off
from icupy.icu import (
    INT32_MAX, CurrencyUnit, DecimalFormatSymbols, ErrorCode, Format,
    Formattable, Locale, MeasureUnit, UCurrencyUsage, UErrorCode,
    UnicodeString, UnicodeStringAppendable, UNumberDecimalSeparatorDisplay,
    UNumberFormatRoundingMode, UNumberSignDisplay, UNumberUnitWidth,
    UParseError,
)
from icupy.icu.number import (
    FormattedNumber, IntegerWidth, LocalizedNumberFormatter, Notation,
    NumberFormatter, ScientificNotation, UnlocalizedNumberFormatter,
)

# fmt: on


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_formatted_number_62():
    fmt = (
        NumberFormatter.with_()
        .notation(Notation.compact_short())
        .unit(MeasureUnit.get_kelvin())
        .locale("en-US")
    )
    num = fmt.format_double(65000)
    assert isinstance(num, FormattedNumber)

    # Appendable &icu::number::FormattedNumber::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = UnicodeString()
    appendable = UnicodeStringAppendable(s)
    result = num.append_to(appendable)
    assert isinstance(result, UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "65K K"

    # UnicodeString icu::number::FormattedNumber::toString(UErrorCode &status)
    result = num.to_string()
    assert isinstance(result, UnicodeString)
    assert result == "65K K"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_formatted_number_64():
    # fmt: off
    from icupy.icu import (
        ConstrainedFieldPosition, FormattedValue, ICUError, UFieldCategory,
        UNumberFormatFields,
    )

    # fmt: on

    assert issubclass(FormattedNumber, FormattedValue)

    # icu::number::FormattedNumber::FormattedNumber()
    num = FormattedNumber()
    with pytest.raises(ICUError) as exc_info:
        _ = num.to_string()
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_STATE_ERROR

    fmt = (
        NumberFormatter.with_()
        .notation(Notation.compact_short())
        .unit(MeasureUnit.get_kelvin())
        .locale("en-US")
    )
    num = fmt.format_double(65000)
    assert isinstance(num, FormattedNumber)

    # UBool icu::number::FormattedNumber::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = ConstrainedFieldPosition()
    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_INTEGER_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 2)

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_COMPACT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (2, 3)

    assert num.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_NUMBER
    assert cfpos.get_field() == UNumberFormatFields.UNUM_MEASURE_UNIT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (4, 5)

    assert not num.next_position(cfpos)

    # UnicodeString icu::number::FormattedNumber::toTempString(
    #       UErrorCode &status
    # )
    result = num.to_temp_string()
    assert isinstance(result, UnicodeString)
    assert result == "65K K"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 65, reason="ICU4C<65")
def test_formatted_number_65():
    fmt = (
        NumberFormatter.with_()
        .notation(Notation.compact_short())
        .unit(MeasureUnit.get_kelvin())
        .locale("en-US")
    )
    num = fmt.format_double(65000)
    assert isinstance(num, FormattedNumber)

    # template<typename StringClass>
    # StringClass icu::number::FormattedNumber::toDecimalNumber(
    #       UErrorCode &status
    # )
    result = num.to_decimal_number()
    assert isinstance(result, str)
    assert result == "65"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_formatted_number_68():
    fmt = (
        NumberFormatter.with_()
        .usage("person")
        .unit(MeasureUnit.get_kilogram())
        .locale("en-GB")
    )
    num = fmt.format_double(80)
    assert isinstance(num, FormattedNumber)

    # MeasureUnit icu::number::FormattedNumber::getOutputUnit(
    #       UErrorCode &status
    # )
    unit = num.get_output_unit()
    assert isinstance(unit, MeasureUnit)
    assert unit.get_identifier() == "stone-and-pound"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM != 71, reason="ICU4C!=71")
def test_formatted_number_get_noun_class_71():
    from icupy.icu import NounClass

    fmt = (
        NumberFormatter.with_()
        .unit(MeasureUnit.for_identifier("cubic-meter"))
        .locale("fr")
    )
    num = fmt.format_double(1.1)
    assert isinstance(num, FormattedNumber)

    # NounClass icu::number::FormattedNumber::getNounClass(UErrorCode &status)
    assert num.get_noun_class() == NounClass.MASCULINE


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_formatted_number_get_noun_class_72():
    from icupy.icu import UDisplayOptionsNounClass

    fmt = (
        NumberFormatter.with_()
        .unit(MeasureUnit.for_identifier("cubic-meter"))
        .locale("fr")
    )
    num = fmt.format_double(1.1)
    assert isinstance(num, FormattedNumber)

    # UDisplayOptionsNounClass
    # icu::number::FormattedNumber::getNounClass(UErrorCode &status)
    assert (
        num.get_noun_class()
        == UDisplayOptionsNounClass.UDISPOPT_NOUN_CLASS_MASCULINE
    )


def test_integer_width():
    # icu::number::IntegerWidth
    assert isinstance(IntegerWidth.zero_fill_to(3), IntegerWidth)
    assert isinstance(
        IntegerWidth.zero_fill_to(3).truncate_at(3), IntegerWidth
    )


def test_localized_number_formatter_60():
    fmt = NumberFormatter.with_locale(Locale.get_us())
    assert isinstance(fmt, LocalizedNumberFormatter)

    # [2]
    # icu::number::LocalizedNumberFormatter::LocalizedNumberFormatter(
    #       const LocalizedNumberFormatter &other
    # )
    fmt2 = LocalizedNumberFormatter(fmt)

    # FormattedNumber icu::number::LocalizedNumberFormatter::formatDecimal(
    #       StringPiece value,
    #       UErrorCode &status
    # )
    num = fmt2.format_decimal("9.87E-3")
    assert isinstance(num, FormattedNumber)

    # FormattedNumber icu::number::LocalizedNumberFormatter::formatDouble(
    #       double value,
    #       UErrorCode &status
    # )
    num = fmt2.format_double(9.87e-3)
    assert isinstance(num, FormattedNumber)

    # FormattedNumber icu::number::LocalizedNumberFormatter::formatInt(
    #       int64_t value,
    #       UErrorCode &status
    # )
    num = fmt2.format_int(INT32_MAX + 1)
    assert isinstance(num, FormattedNumber)

    # UBool icu::number::NumberFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = ErrorCode()
    assert fmt2.copy_error_to(out_error_code) is False
    assert out_error_code.get() == UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::decimal(
    #       UNumberDecimalSeparatorDisplay style
    # )
    result = fmt2.decimal(
        UNumberDecimalSeparatorDisplay.UNUM_DECIMAL_SEPARATOR_AUTO
    )
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::integerWidth(
    #       const IntegerWidth &style
    # )
    result = fmt2.integer_width(IntegerWidth.zero_fill_to(2))
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::notation(
    #       const Notation &notation
    # )
    result = fmt2.notation(Notation.compact_long())
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::symbols(
    #       const DecimalFormatSymbols &symbols
    # )
    result = fmt.symbols(DecimalFormatSymbols(Locale.get_us()))
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unit(
    #       const icu::MeasureUnit &unit
    # )
    result = fmt.unit(MeasureUnit.create_meter_per_second())
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unitWidth(
    #       UNumberUnitWidth width
    # )
    result = fmt.unit_width(UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
    assert isinstance(result, LocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 61, reason="ICU4C<61")
def test_localized_number_formatter_61():
    from icupy.icu import UNumberGroupingStrategy

    fmt = NumberFormatter.with_locale(Locale.get_us())
    assert isinstance(fmt, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::grouping(
    #       UNumberGroupingStrategy strategy
    # )
    result = fmt.grouping(UNumberGroupingStrategy.UNUM_GROUPING_MIN2)
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::perUnit(
    #       const icu::MeasureUnit &perUnit
    # )
    result = fmt.per_unit(MeasureUnit.get_meter())
    assert isinstance(result, LocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_localized_number_formatter_62():
    from icupy.icu.number import Precision, Scale

    # [1]
    # icu::number::LocalizedNumberFormatter::LocalizedNumberFormatter()
    fmt = LocalizedNumberFormatter()

    # Format *icu::number::LocalizedNumberFormatter::toFormat(
    #       UErrorCode &status
    # )
    fmt2 = (
        fmt.notation(Notation.engineering())
        .unit(MeasureUnit.get_kelvin())
        .to_format()
    )
    assert isinstance(fmt2, Format)

    obj = Formattable(65000)
    append_to = UnicodeString()
    fmt2.format(obj, append_to)
    assert append_to == "65E3 K"

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::precision(
    #       const Precision &precision
    # )
    result = fmt.precision(Precision.fixed_fraction(2))
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::roundingMode(
    #       UNumberFormatRoundingMode roundingMode
    # )
    result = fmt.rounding_mode(UNumberFormatRoundingMode.UNUM_ROUND_HALFEVEN)
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::scale(
    #       const Scale &scale
    # )
    result = fmt.scale(Scale.power_of_ten(2))
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::sign(
    #       UNumberSignDisplay style
    # )
    result = fmt.sign(UNumberSignDisplay.UNUM_SIGN_AUTO)
    assert isinstance(result, LocalizedNumberFormatter)

    # template<typename Derived>
    # UnicodeString icu::number::NumberFormatterSettings<Derived>::toSkeleton(
    #       UErrorCode &status
    # )
    result = fmt.unit(MeasureUnit.get_meter_per_second()).to_skeleton()
    assert isinstance(result, UnicodeString)
    assert result in (
        "unit/meter-per-second",
        "measure-unit/speed-meter-per-second",
    )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_localized_number_formatter_64():
    fmt = NumberFormatter.with_locale(Locale.get_us()).notation(
        Notation.engineering()
    )
    assert isinstance(fmt, LocalizedNumberFormatter)
    assert fmt.format_double(0.8765).to_string() == "876.5E-3"

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, LocalizedNumberFormatter)
    assert fmt2.format_double(0.8765).to_string() == "876.5E-3"

    fmt3 = copy.copy(fmt)
    assert fmt3.format_double(0.8765).to_string() == "876.5E-3"

    fmt4 = copy.deepcopy(fmt)
    assert fmt4.format_double(0.8765).to_string() == "876.5E-3"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_localized_number_formatter_68():
    fmt = NumberFormatter.with_locale(Locale.get_us())
    assert isinstance(fmt, LocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::usage(
    #       StringPiece usage
    # )
    result = fmt.usage("person")
    assert isinstance(result, LocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_localized_number_formatter_72():
    from icupy.icu import DisplayOptions, UDisplayOptionsGrammaticalCase

    bld = DisplayOptions.builder()
    display_options = bld.set_grammatical_case(
        UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_DATIVE
    ).build()

    # icu::number::LocalizedNumberFormatter
    # icu::number::NumberFormatterSettings<LocalizedNumberFormatter>::displayOptions(
    #       const DisplayOptions &displayOptions
    # )
    fmt = (
        NumberFormatter.with_locale("de")
        .unit(MeasureUnit.for_identifier("meter-and-centimeter"))
        .unit_width(UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
        .display_options(display_options)
    )
    assert isinstance(fmt, LocalizedNumberFormatter)

    num = fmt.format_double(2.1)
    assert num.to_temp_string() == "2 Metern, 10 Zentimetern"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 75, reason="ICU4C<75")
def test_localized_number_formatter_75():
    # UnlocalizedNumberFormatter
    # icu::number::LocalizedNumberFormatter::withoutLocale() const &
    lnf2 = (
        NumberFormatter.with_()
        .notation(Notation.compact_long())
        .locale("fr")
        .unit_width(UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
    )
    unf2 = lnf2.without_locale()
    assert isinstance(unf2, UnlocalizedNumberFormatter)
    f = unf2.unit(MeasureUnit.create_meter())
    # l1 = f.threshold(0).locale("ja-JP")
    l1 = f.locale("ja-JP")
    result1 = l1.format_double(2)
    actual1 = result1.to_string()
    assert actual1 == "2 メートル"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_localized_number_formatter_to_format():
    # fmt: off
    from icupy.icu import (
        FieldPosition, FieldPositionIterator, ICUError, ParsePosition,
    )

    # fmt: on
    # [1]
    # icu::number::LocalizedNumberFormatter::LocalizedNumberFormatter()
    fmt = LocalizedNumberFormatter()

    # Format *icu::number::LocalizedNumberFormatter::toFormat(
    #       UErrorCode &status
    # )
    fmt2 = (
        fmt.notation(Notation.engineering())
        .unit(MeasureUnit.get_kelvin())
        .to_format()
    )
    assert isinstance(fmt2, Format)

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
    obj = Formattable(65000)
    append_to = UnicodeString()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt2.format(obj, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    # [2]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    obj = Formattable(65000)
    append_to.remove()
    pos_iter = FieldPositionIterator()
    result = fmt2.format(obj, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    append_to.remove()
    pos_iter = None
    result = fmt2.format(obj, append_to, pos_iter)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    obj = Formattable(65000)
    append_to.remove()
    result = fmt2.format(obj, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert append_to == "65E3 K"

    # [1]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    source = UnicodeString("65E3 K")
    result = Formattable()
    parse_pos = ParsePosition()
    fmt2.parse_object(source, result, parse_pos)
    assert parse_pos.get_index() == 0
    assert parse_pos.get_error_index() == 0

    source = "65E3 K"
    result = Formattable()
    parse_pos = ParsePosition()
    fmt2.parse_object(source, result, parse_pos)
    assert parse_pos.get_index() == 0
    assert parse_pos.get_error_index() == 0

    # [2]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    source = UnicodeString("65E3 K")
    result = Formattable()
    with pytest.raises(ICUError) as exc_info:
        fmt2.parse_object(source, result)
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR

    source = "65E3 K"
    result = Formattable()
    with pytest.raises(ICUError) as exc_info:
        fmt2.parse_object(source, result)
    assert exc_info.value.args[0] == UErrorCode.U_INVALID_FORMAT_ERROR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_no_unit():
    from icupy.icu import NoUnit

    fmt = NumberFormatter.with_locale(Locale.get_us())

    unit = NoUnit.base()
    assert isinstance(unit, MeasureUnit)
    assert fmt.unit(unit).format_int(1).to_string() == "1"

    unit = NoUnit.percent()
    assert isinstance(unit, MeasureUnit)
    assert fmt.unit(unit).format_int(1).to_string() == "1%"

    unit = NoUnit.permille()
    assert isinstance(unit, MeasureUnit)
    assert fmt.unit(unit).format_int(1).to_string() == "1\u2030"  # 1‰


def test_number_formatter_60():
    # static UnlocalizedNumberFormatter icu::number::NumberFormatter::with()
    fmt = NumberFormatter.with_()
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    # static LocalizedNumberFormatter icu::number::NumberFormatter::withLocale(
    #       const Locale &locale
    # )
    fmt = NumberFormatter.with_locale(Locale("en_US"))
    assert isinstance(fmt, LocalizedNumberFormatter)

    fmt = NumberFormatter.with_locale("en_US")
    assert isinstance(fmt, LocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_number_formatter_62():
    # [1]
    # static UnlocalizedNumberFormatter
    # icu::number::NumberFormatter::forSkeleton(
    #       const UnicodeString &skeleton,
    #       UErrorCode &status
    # )
    fmt = NumberFormatter.for_skeleton(
        UnicodeString("measure-unit/speed-meter-per-second")
    )
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    fmt = NumberFormatter.for_skeleton("measure-unit/speed-meter-per-second")
    assert isinstance(fmt, UnlocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_number_formatter_64():
    # [2]
    # static UnlocalizedNumberFormatter
    # icu::number::NumberFormatter::forSkeleton(
    #       const UnicodeString &skeleton,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = UParseError()
    fmt = NumberFormatter.for_skeleton(
        UnicodeString("measure-unit/speed-meter-per-second"), perror
    )
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    perror = UParseError()
    fmt = NumberFormatter.for_skeleton(
        "measure-unit/speed-meter-per-second", perror
    )
    assert isinstance(fmt, UnlocalizedNumberFormatter)


def test_notation():
    # icu::number::Notation
    assert isinstance(Notation.compact_long(), Notation)
    assert isinstance(Notation.compact_short(), Notation)
    assert isinstance(Notation.engineering(), ScientificNotation)
    assert isinstance(Notation.scientific(), ScientificNotation)
    assert isinstance(Notation.simple(), Notation)

    # icu::number::ScientificNotation
    assert issubclass(ScientificNotation, Notation)

    assert isinstance(
        (
            Notation.engineering().with_exponent_sign_display(
                UNumberSignDisplay.UNUM_SIGN_ALWAYS
            )
        ),
        ScientificNotation,
    )
    assert isinstance(
        Notation.engineering().with_min_exponent_digits(2), ScientificNotation
    )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_precision_60_62():
    # fmt: off
    from icupy.icu.number import (
        CurrencyPrecision, FractionPrecision, IncrementPrecision, Precision,
    )

    # fmt: on
    # icu::number::Precision
    assert isinstance(
        Precision.currency(UCurrencyUsage.UCURR_USAGE_STANDARD),
        CurrencyPrecision,
    )
    assert isinstance(Precision.fixed_fraction(3), FractionPrecision)
    assert isinstance(Precision.increment(0.5), IncrementPrecision)
    assert isinstance(Precision.integer(), FractionPrecision)
    assert isinstance(Precision.max_fraction(3), FractionPrecision)
    assert isinstance(Precision.min_fraction(1), FractionPrecision)
    assert isinstance(Precision.min_max_fraction(1, 3), FractionPrecision)
    assert isinstance(Precision.unlimited(), Precision)

    # icu::number::CurrencyPrecision
    assert issubclass(CurrencyPrecision, Precision)

    assert isinstance(
        (
            Precision.currency(
                UCurrencyUsage.UCURR_USAGE_STANDARD
            ).with_currency(CurrencyUnit("USD"))
        ),
        Precision,
    )

    # icu::number::FractionPrecision
    assert issubclass(FractionPrecision, Precision)

    assert isinstance(
        Precision.fixed_fraction(2).with_max_digits(2), Precision
    )
    assert isinstance(
        Precision.fixed_fraction(2).with_min_digits(3), Precision
    )

    # icu::number::IncrementPrecision
    assert issubclass(IncrementPrecision, Precision)

    assert isinstance(Precision.increment(0.5).with_min_fraction(1), Precision)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_precision_62():
    from icupy.icu.number import Precision

    # icu::number::Precision
    assert isinstance(Precision.fixed_significant_digits(3), Precision)
    assert isinstance(Precision.max_significant_digits(4), Precision)
    assert isinstance(Precision.min_max_significant_digits(3, 4), Precision)
    assert isinstance(Precision.min_significant_digits(3), Precision)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 69, reason="ICU4C<69")
def test_precision_69():
    from icupy.icu import UNumberRoundingPriority, UNumberTrailingZeroDisplay
    from icupy.icu.number import Precision

    # icu::number::Precision
    assert isinstance(
        (
            Precision.fixed_fraction(2).trailing_zero_display(
                UNumberTrailingZeroDisplay.UNUM_TRAILING_ZERO_AUTO
            )
        ),
        Precision,
    )

    # icu::number::FractionPrecision
    assert isinstance(
        (
            Precision.fixed_fraction(1).with_significant_digits(
                3, 3, UNumberRoundingPriority.UNUM_ROUNDING_PRIORITY_RELAXED
            )
        ),
        Precision,
    )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 71, reason="ICU4C<71")
def test_precision_71():
    from icupy.icu.number import IncrementPrecision, Precision

    # static IncrementPrecision icu::number::Precision::incrementExact(
    #       uint64_t mantissa,
    #       int16_t magnitude
    # )
    assert isinstance(Precision.increment_exact(50, -2), IncrementPrecision)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_scale():
    from icupy.icu.number import Scale

    # icu::number::Scale
    assert isinstance(Scale.by_decimal("5.2"), Scale)
    assert isinstance(Scale.by_double(5.2), Scale)
    assert isinstance(Scale.by_double_and_power_of_ten(5.2, 3), Scale)
    assert isinstance(Scale.none(), Scale)
    assert isinstance(Scale.power_of_ten(2), Scale)


def test_unlocalized_number_formatter_60():
    fmt = NumberFormatter.with_()
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    # [2]
    # icu::number::UnlocalizedNumberFormatter::UnlocalizedNumberFormatter(
    #       const UnlocalizedNumberFormatter &other
    # )
    fmt2 = UnlocalizedNumberFormatter(fmt)

    # LocalizedNumberFormatter icu::number::UnlocalizedNumberFormatter::locale(
    #       const icu::Locale &locale
    # )
    result = fmt2.locale(Locale.get_us())
    assert isinstance(result, LocalizedNumberFormatter)

    result = fmt2.locale("en-US")
    assert isinstance(result, LocalizedNumberFormatter)

    # UBool icu::number::NumberFormatterSettings<Derived>::copyErrorTo(
    #       UErrorCode &outErrorCode
    # )
    out_error_code = ErrorCode()
    assert fmt2.copy_error_to(out_error_code) is False
    assert out_error_code.get() == UErrorCode.U_ZERO_ERROR

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::decimal(
    #       UNumberDecimalSeparatorDisplay style
    # )
    result = fmt2.decimal(
        UNumberDecimalSeparatorDisplay.UNUM_DECIMAL_SEPARATOR_AUTO
    )
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::integerWidth(
    #       const IntegerWidth &style
    # )
    result = fmt2.integer_width(IntegerWidth.zero_fill_to(2))
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::notation(
    #       const Notation &notation
    # )
    result = fmt2.notation(Notation.compact_long())
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::symbols(
    #       const DecimalFormatSymbols &symbols
    # )
    result = fmt.symbols(DecimalFormatSymbols(Locale.get_us()))
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unit(
    #       const icu::MeasureUnit &unit
    # )
    result = fmt.unit(MeasureUnit.create_meter_per_second())
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::unitWidth(
    #       UNumberUnitWidth width
    # )
    result = fmt.unit_width(UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
    assert isinstance(result, UnlocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 61, reason="ICU4C<61")
def test_unlocalized_number_formatter_61():
    from icupy.icu import UNumberGroupingStrategy

    fmt = NumberFormatter.with_()
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::grouping(
    #       UNumberGroupingStrategy strategy
    # )
    result = fmt.grouping(UNumberGroupingStrategy.UNUM_GROUPING_MIN2)
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::perUnit(
    #       const icu::MeasureUnit &perUnit
    # )
    result = fmt.per_unit(MeasureUnit.get_meter())
    assert isinstance(result, UnlocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_unlocalized_number_formatter_62():
    from icupy.icu.number import Precision, Scale

    # [1]
    # icu::number::UnlocalizedNumberFormatter::UnlocalizedNumberFormatter()
    fmt = UnlocalizedNumberFormatter()

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::precision(
    #       const Precision &precision
    # )
    result = fmt.precision(Precision.fixed_fraction(2))
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::roundingMode(
    #       UNumberFormatRoundingMode roundingMode
    # )
    result = fmt.rounding_mode(UNumberFormatRoundingMode.UNUM_ROUND_HALFEVEN)
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::scale(
    #       const Scale &scale
    # )
    result = fmt.scale(Scale.power_of_ten(2))
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::sign(
    #       UNumberSignDisplay style
    # )
    result = fmt.sign(UNumberSignDisplay.UNUM_SIGN_AUTO)
    assert isinstance(result, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # UnicodeString icu::number::NumberFormatterSettings<Derived>::toSkeleton(
    #       UErrorCode &status
    # )
    result = fmt.unit(MeasureUnit.get_meter_per_second()).to_skeleton()
    assert isinstance(result, UnicodeString)
    assert result in (
        "unit/meter-per-second",
        "measure-unit/speed-meter-per-second",
    )


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_unlocalized_number_formatter_64():
    fmt = NumberFormatter.with_().notation(Notation.engineering())
    assert isinstance(fmt, UnlocalizedNumberFormatter)
    assert fmt.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"

    # template<typename Derived>
    # LocalPointer<Derived>
    # icu::number::NumberFormatterSettings<Derived>::clone()
    fmt2 = fmt.clone()
    assert isinstance(fmt2, UnlocalizedNumberFormatter)
    assert fmt2.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"

    fmt3 = copy.copy(fmt)
    assert fmt3.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"

    fmt4 = copy.deepcopy(fmt)
    assert fmt4.locale("en-US").format_double(0.8765).to_string() == "876.5E-3"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 68, reason="ICU4C<68")
def test_unlocalized_number_formatter_68():
    fmt = NumberFormatter.with_()
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    # template<typename Derived>
    # Derived icu::number::NumberFormatterSettings<Derived>::usage(
    #       StringPiece usage
    # )
    result = fmt.usage("person")
    assert isinstance(result, UnlocalizedNumberFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 72, reason="ICU4C<72")
def test_unlocalized_number_formatter_72():
    from icupy.icu import DisplayOptions, UDisplayOptionsGrammaticalCase

    bld = DisplayOptions.builder()
    display_options = bld.set_grammatical_case(
        UDisplayOptionsGrammaticalCase.UDISPOPT_GRAMMATICAL_CASE_DATIVE
    ).build()

    # icu::number::UnlocalizedNumberFormatter
    # icu::number::NumberFormatterSettings<UnlocalizedNumberFormatter>::displayOptions(
    #       const DisplayOptions &displayOptions
    # )
    fmt = (
        NumberFormatter.with_()
        .unit(MeasureUnit.for_identifier("meter-and-centimeter"))
        .unit_width(UNumberUnitWidth.UNUM_UNIT_WIDTH_FULL_NAME)
        .display_options(display_options)
    )
    assert isinstance(fmt, UnlocalizedNumberFormatter)

    num = fmt.locale("de").format_double(2.1)
    assert num.to_temp_string() == "2 Metern, 10 Zentimetern"

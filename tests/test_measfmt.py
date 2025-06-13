import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 53:
    pytest.skip("ICU4C<53", allow_module_level=True)

import copy


def test_api():
    # [1]
    # icu::MeasureFormat::MeasureFormat(
    #       const Locale &locale,
    #       UMeasureFormatWidth width,
    #       UErrorCode &status
    # )
    fmt1 = icu.MeasureFormat(icu.Locale.get_us(), icu.UMeasureFormatWidth.UMEASFMT_WIDTH_SHORT)
    fmt2 = icu.MeasureFormat("en-US", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_SHORT)
    fmt3 = icu.MeasureFormat(icu.Locale.get_us(), icu.UMeasureFormatWidth.UMEASFMT_WIDTH_NARROW)

    # [3]
    # icu::MeasureFormat::MeasureFormat(const MeasureFormat &other)
    fmt4 = icu.MeasureFormat(fmt1)

    # UBool icu::Format::operator!=(const Format &other)
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert not (fmt1 != fmt4)

    # UBool icu::MeasureFormat::operator==(const Format &other)
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert fmt1 == fmt4

    # [1]
    # static MeasureFormat *icu::MeasureFormat::createCurrencyFormat(
    #       const Locale &locale,
    #       UErrorCode &ec
    # )
    fmt4 = icu.MeasureFormat.create_currency_format(icu.Locale.get_us())
    assert isinstance(fmt4, icu.MeasureFormat)

    fmt5 = icu.MeasureFormat.create_currency_format("en-US")
    assert isinstance(fmt5, icu.MeasureFormat)
    assert fmt4 == fmt5

    # [2]
    # static MeasureFormat *icu::MeasureFormat::createCurrencyFormat(
    #       UErrorCode &ec
    # )
    fmt6 = icu.MeasureFormat.create_currency_format()
    assert isinstance(fmt6, icu.MeasureFormat)

    # Locale icu::Format::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    loc = fmt1.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert loc == icu.Locale("en-US")


def test_clone():
    fmt1 = icu.MeasureFormat("en-US", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_SHORT)

    # MeasureFormat *icu::MeasureFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.MeasureFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_currency_amount():
    assert issubclass(icu.CurrencyAmount, icu.Measure)

    # [1]
    # icu::CurrencyAmount::CurrencyAmount(
    #       const Formattable &amount,
    #       ConstChar16Ptr isoCode,
    #       UErrorCode &ec
    # )
    camt1 = icu.CurrencyAmount(icu.Formattable(1), "USD")
    camt2 = icu.CurrencyAmount(icu.Formattable("1.0"), "USD")
    assert camt1 == camt2

    # [2]
    # icu::CurrencyAmount::CurrencyAmount(
    #       double amount,
    #       ConstChar16Ptr isoCode,
    #       UErrorCode &ec
    # )
    camt3 = icu.CurrencyAmount(1, "USD")
    assert camt1 != camt3

    # [3]
    # icu::CurrencyAmount::CurrencyAmount(const CurrencyAmount &other)
    camt4 = icu.CurrencyAmount(camt1)
    assert camt1 == camt4

    # CurrencyAmount *icu::CurrencyAmount::clone()
    camt5 = camt1.clone()
    assert isinstance(camt5, icu.CurrencyAmount)
    assert camt1 == camt5

    # CurrencyAmount.__copy__() -> CurrencyAmount
    # CurrencyAmount.__deepcopy__(Optional[memo]) -> CurrencyAmount
    camt5a = copy.copy(camt1)
    camt5b = copy.deepcopy(camt1)
    assert camt5 == camt5a == camt5b

    # const CurrencyUnit &icu::CurrencyAmount::getCurrency()
    unit = camt1.get_currency()
    assert isinstance(unit, icu.CurrencyUnit)
    assert unit.get_iso_currency() == "USD"

    # const char16_t *icu::CurrencyAmount::getISOCurrency()
    iso_code = camt1.get_iso_currency()
    assert isinstance(iso_code, str)
    assert iso_code == "USD"


def test_format():
    fmt1 = icu.MeasureFormat("en-US", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_WIDE)
    fmt2 = icu.MeasureFormat.create_currency_format("en-US")

    # [2]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt1.format(
        icu.Formattable(icu.Measure(icu.Formattable(3.5), icu.MeasureUnit.create_foot())),
        append_to,
        pos,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "3.5 feet"

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt1.format(
        icu.Formattable(icu.CurrencyAmount(icu.Formattable(1), "USD")),
        append_to,
        pos,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1.00 US dollars"

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt2.format(
        icu.Formattable(icu.CurrencyAmount(icu.Formattable(1), "USD")),
        append_to,
        pos,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "$1.00"

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt1.format(
        icu.Formattable(icu.TimeUnitAmount(icu.Formattable(2), icu.TimeUnit.UTIMEUNIT_DAY)),
        append_to,
        pos,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2 days"

    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt1.format(
            icu.Formattable(icu.Measure(icu.Formattable(3.5), icu.MeasureUnit.create_foot())),
            append_to,
            pos_iter,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt1.format(
            icu.Formattable(icu.CurrencyAmount(icu.Formattable(1), "USD")),
            append_to,
            pos_iter,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt2.format(
            icu.Formattable(icu.CurrencyAmount(icu.Formattable(1), "USD")),
            append_to,
            pos_iter,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt1.format(
            icu.Formattable(icu.TimeUnitAmount(icu.Formattable(2), icu.TimeUnit.UTIMEUNIT_DAY)),
            append_to,
            pos_iter,
        )
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    result = fmt1.format(
        icu.Formattable(icu.Measure(icu.Formattable(3.5), icu.MeasureUnit.create_foot())),
        append_to,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "3.5 feet"

    append_to.remove()
    result = fmt1.format(
        icu.Formattable(icu.CurrencyAmount(icu.Formattable(1), "USD")),
        append_to,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "1.00 US dollars"

    append_to.remove()
    result = fmt2.format(
        icu.Formattable(icu.CurrencyAmount(icu.Formattable(1), "USD")),
        append_to,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "$1.00"

    append_to.remove()
    result = fmt1.format(
        icu.Formattable(icu.TimeUnitAmount(icu.Formattable(2), icu.TimeUnit.UTIMEUNIT_DAY)),
        append_to,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "2 days"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 55, reason="ICU4C<55")
def test_format_measure_per_unit():
    fmt = icu.MeasureFormat("en-US", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_SHORT)

    # UnicodeString &icu::MeasureFormat::formatMeasurePerUnit(
    #       const Measure &measure,
    #       const MeasureUnit &perUnit,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format_measure_per_unit(
        icu.Measure(icu.Formattable(50), icu.MeasureUnit.create_pound_force()),
        icu.MeasureUnit.create_square_inch(),
        append_to,
        pos,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in ("50 lbf/in\xb2", "50 psi")  # 50 lbf/in²


def test_format_measures():
    fmt = icu.MeasureFormat("fr-FR", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_NARROW)

    # UnicodeString &icu::MeasureFormat::formatMeasures(
    #       const Measure *measures,
    #       int32_t measureCount,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    measures = [
        icu.Measure(icu.Formattable(5), icu.MeasureUnit.create_hour()),
        icu.Measure(icu.Formattable(10), icu.MeasureUnit.create_minute()),
    ]
    append_to = icu.UnicodeString()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format_measures(measures, len(measures), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "5h 10min"

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format_measures(measures, -1, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "5h 10min"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 58, reason="ICU4C<58")
def test_get_unit_display_name():
    fmt1 = icu.MeasureFormat("en", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_WIDE)
    fmt2 = icu.MeasureFormat("ja", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_WIDE)

    # UnicodeString icu::MeasureFormat::getUnitDisplayName(
    #       const MeasureUnit &unit,
    #       UErrorCode &status
    # )
    result = fmt1.get_unit_display_name(icu.MeasureUnit.create_year())
    assert isinstance(result, icu.UnicodeString)
    assert result == "years"

    result = fmt2.get_unit_display_name(icu.MeasureUnit.create_year())
    assert isinstance(result, icu.UnicodeString)
    assert result == "\u5e74"  # 年


def test_parse_object():
    fmt1 = icu.MeasureFormat("en-US", icu.UMeasureFormatWidth.UMEASFMT_WIDTH_WIDE)
    fmt2 = icu.MeasureFormat.create_currency_format("en-US")

    # [1]
    # void icu::MeasureFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &reslt,
    #       ParsePosition &pos
    # )
    result = icu.Formattable()
    pos = icu.ParsePosition(0)
    fmt1.parse_object(icu.UnicodeString("3.5 feet"), result, pos)
    assert pos.get_index() == 0

    result = icu.Formattable()
    pos = icu.ParsePosition(0)
    fmt1.parse_object("3.5 feet", result, pos)
    assert pos.get_index() == 0

    result = icu.Formattable()
    pos = icu.ParsePosition(0)
    fmt1.parse_object("1.00 US dollars", result, pos)
    assert pos.get_index() == 0

    result = icu.Formattable()
    pos = icu.ParsePosition(0)
    fmt2.parse_object("$1.00", result, pos)
    assert pos.get_index() != 0
    assert pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.OBJECT
    camt = result.get_object()
    assert isinstance(camt, icu.CurrencyAmount)
    num = camt.get_number()
    assert num.get_type() == icu.Formattable.LONG
    assert num.get_long() == 1
    assert camt.get_iso_currency() == "USD"

    result = icu.Formattable()
    pos = icu.ParsePosition(0)
    fmt1.parse_object("2 days", result, pos)
    assert pos.get_index() == 0

    # [3]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt1.parse_object(icu.UnicodeString("3.5 feet"), result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt1.parse_object("3.5 feet", result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt1.parse_object("1.00 US dollars", result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR

    result = icu.Formattable()
    fmt2.parse_object("$1.00", result)
    assert result.get_type() == icu.Formattable.OBJECT
    camt = result.get_object()
    assert isinstance(camt, icu.CurrencyAmount)
    num = camt.get_number()
    assert num.get_type() == icu.Formattable.LONG
    assert num.get_long() == 1
    assert camt.get_iso_currency() == "USD"

    result = icu.Formattable()
    with pytest.raises(icu.ICUError) as exc_info:
        fmt1.parse_object("2 days", result)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_FORMAT_ERROR


def test_measure():
    # [1]
    # icu::Measure::Measure(const Formattable &number,
    #                       MeasureUnit *adoptedUnit,
    #                       UErrorCode &ec)
    meas1 = icu.Measure(icu.Formattable(100), icu.MeasureUnit.create_meter())
    meas2 = icu.Measure(icu.Formattable("100.0"), icu.MeasureUnit.create_meter())
    meas3 = icu.Measure(icu.Formattable(0.1), icu.MeasureUnit.create_kilometer())
    assert meas1 == meas2
    assert meas1 != meas3
    assert meas2 != meas3

    # [2]
    # icu::Measure::Measure(const Measure &other)
    meas4 = icu.Measure(meas1)
    assert meas1 == meas4

    # Measure *icu::Measure::clone()
    meas5 = meas4.clone()
    assert isinstance(meas5, icu.Measure)

    # Measure.__copy__() -> Measure
    # Measure.__deepcopy__(Optional[memo]) -> Measure
    meas5a = copy.copy(meas4)
    meas5b = copy.deepcopy(meas4)
    assert meas1 == meas5 == meas5a == meas5b

    # const Formattable &icu::Measure::getNumber()
    num = meas1.get_number()
    assert isinstance(num, icu.Formattable)
    assert num.get_type() == icu.Formattable.LONG
    assert num.get_long() == 100

    # const MeasureUnit &icu::Measure::getUnit()
    unit = meas1.get_unit()
    assert isinstance(unit, icu.MeasureUnit)


def test_time_unit_amount():
    assert issubclass(icu.TimeUnitAmount, icu.Measure)

    # [1]
    # icu::TimeUnitAmount::TimeUnitAmount(
    #       const Formattable &number,
    #       TimeUnit::UTimeUnitFields timeUnitField,
    #       UErrorCode &status
    # )
    tuamt1 = icu.TimeUnitAmount(icu.Formattable(1), icu.TimeUnit.UTIMEUNIT_DAY)

    # [2]
    # icu::TimeUnitAmount::TimeUnitAmount(
    #       double amount,
    #       TimeUnit::UTimeUnitFields timeUnitField,
    #       UErrorCode &status
    # )
    # tuamt2 =
    _ = icu.TimeUnitAmount(1, icu.TimeUnit.UTIMEUNIT_DAY)
    # FIXME: TimeUnitAmount.__eq__(self, other: UObject) is not work.
    # assert tuamt1 == tuamt2

    # icu::TimeUnitAmount::TimeUnitAmount(const TimeUnitAmount &other)
    tuamt3 = icu.TimeUnitAmount(tuamt1)
    assert tuamt1 == tuamt3
    # assert tuamt2 == tuamt3

    # TimeUnitAmount *icu::TimeUnitAmount::clone()
    tuamt4 = tuamt1.clone()
    assert isinstance(tuamt4, icu.TimeUnitAmount)
    assert tuamt1 == tuamt4
    # assert tuamt2 == tuamt4

    # TimeUnitAmount.__copy__() -> TimeUnitAmount
    # TimeUnitAmount.__deepcopy__(Optional[memo]) -> TimeUnitAmount
    tuamt4a = copy.copy(tuamt1)
    tuamt4b = copy.deepcopy(tuamt1)
    assert tuamt4 == tuamt4a == tuamt4b

    # const TimeUnit &icu::TimeUnitAmount::getTimeUnit()
    unit = tuamt1.get_time_unit()
    assert isinstance(unit, icu.TimeUnit)
    assert unit.get_time_unit_field() == icu.TimeUnit.UTIMEUNIT_DAY

    # TimeUnit::UTimeUnitFields icu::TimeUnitAmount::getTimeUnitField()
    assert tuamt1.get_time_unit_field() == icu.TimeUnit.UTIMEUNIT_DAY

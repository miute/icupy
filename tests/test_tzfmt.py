import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)

import copy

from icupy.icu import U_MILLIS_PER_HOUR as HOUR


def test_api() -> None:
    assert issubclass(icu.TimeZoneFormat, icu.Format)

    # static TimeZoneFormat *icu::TimeZoneFormat::createInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt1 = icu.TimeZoneFormat.create_instance(icu.Locale("en"))
    fmt2 = icu.TimeZoneFormat.create_instance("en")
    fmt3 = icu.TimeZoneFormat.create_instance(icu.Locale("ja"))
    assert isinstance(fmt1, icu.TimeZoneFormat)
    assert isinstance(fmt2, icu.TimeZoneFormat)
    assert isinstance(fmt3, icu.TimeZoneFormat)

    # UBool icu::Format::operator!=(const Format &other)
    assert not (fmt1 != fmt2)
    assert fmt1 != fmt3
    assert fmt2 != fmt3

    # UBool icu::TimeZoneFormat::operator==(const Format &other)
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)

    # UnicodeString &icu::TimeZoneFormat::formatOffsetLocalizedGMT(
    #       int32_t offset,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    result = icu.UnicodeString()
    string = fmt1.format_offset_localized_gmt(int(-8.5 * HOUR), result)
    assert isinstance(string, icu.UnicodeString)
    assert id(result) == id(string)
    assert result == "GMT-08:30"

    # uint32_t icu::TimeZoneFormat::getDefaultParseOptions(void)
    assert (
        fmt2.get_default_parse_options()
        == icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_NONE
    )

    # void icu::TimeZoneFormat::setDefaultParseOptions(uint32_t flags)
    fmt2.set_default_parse_options(
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES
    )
    assert (
        fmt2.get_default_parse_options()
        == icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES
    )

    # UnicodeString &icu::TimeZoneFormat::getGMTOffsetDigits(
    #       UnicodeString &digits
    # )
    digits = icu.UnicodeString()
    result = fmt2.get_gmt_offset_digits(digits)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(digits)
    assert result == "0123456789"

    # void icu::TimeZoneFormat::setGMTOffsetDigits(
    #       const UnicodeString &digits,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_offset_digits(icu.UnicodeString("abcdefghij"))
    assert fmt2.get_gmt_offset_digits(digits) == "abcdefghij"

    fmt2.set_gmt_offset_digits("0123456789")
    assert fmt2.get_gmt_offset_digits(digits) == "0123456789"

    # UnicodeString &icu::TimeZoneFormat::getGMTOffsetPattern(
    #       UTimeZoneFormatGMTOffsetPatternType type,
    #       UnicodeString &pattern
    # )
    pattern = icu.UnicodeString()
    result = fmt2.get_gmt_offset_pattern(
        icu.UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM, pattern
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(pattern)
    assert result == "+HH:mm"

    # void icu::TimeZoneFormat::setGMTOffsetPattern(
    #       UTimeZoneFormatGMTOffsetPatternType type,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_offset_pattern(
        icu.UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        icu.UnicodeString("+H:mm"),
    )
    assert (
        fmt2.get_gmt_offset_pattern(
            icu.UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
            pattern,
        )
        == "+H:mm"
    )

    fmt2.set_gmt_offset_pattern(
        icu.UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        "+HH:mm",
    )
    assert (
        fmt2.get_gmt_offset_pattern(
            icu.UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
            pattern,
        )
        == "+HH:mm"
    )

    # UnicodeString &icu::TimeZoneFormat::getGMTPattern(UnicodeString &pattern)
    result = fmt2.get_gmt_pattern(pattern)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(pattern)
    assert result == "GMT{0}"

    # void icu::TimeZoneFormat::setGMTPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_pattern(icu.UnicodeString("GMT {0}"))
    assert fmt2.get_gmt_pattern(pattern) == "GMT {0}"

    fmt2.set_gmt_pattern("GMT{0}")
    assert fmt2.get_gmt_pattern(pattern) == "GMT{0}"

    # UnicodeString &icu::TimeZoneFormat::getGMTZeroFormat(
    #       UnicodeString &gmtZeroFormat
    # )
    gmt_zero_format = icu.UnicodeString()
    result = fmt2.get_gmt_zero_format(gmt_zero_format)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(gmt_zero_format)
    assert result == "GMT"

    # void icu::TimeZoneFormat::setGMTZeroFormat(
    #       const UnicodeString &gmtZeroFormat,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_zero_format(icu.UnicodeString("UTC"))
    assert fmt2.get_gmt_zero_format(gmt_zero_format) == "UTC"

    fmt2.set_gmt_zero_format("GMT")
    assert fmt2.get_gmt_zero_format(gmt_zero_format) == "GMT"

    # const TimeZoneNames *icu::TimeZoneFormat::getTimeZoneNames()
    tzn = fmt2.get_time_zone_names()
    assert isinstance(tzn, icu.TimeZoneNames)

    # void icu::TimeZoneFormat::setTimeZoneNames(const TimeZoneNames &tznames)
    tznames = icu.TimeZoneNames.create_instance("ja")
    assert tznames != tzn
    fmt2.set_time_zone_names(tznames)
    assert fmt2.get_time_zone_names() == tznames

    # int32_t icu::TimeZoneFormat::parseOffsetISO8601(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = icu.ParsePosition(0)
    result = fmt1.parse_offset_iso8601(icu.UnicodeString("-08:30"), pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR

    pos = icu.ParsePosition(0)
    result = fmt1.parse_offset_iso8601("-08:30", pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR

    # int32_t icu::TimeZoneFormat::parseOffsetLocalizedGMT(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = icu.ParsePosition(0)
    result = fmt1.parse_offset_localized_gmt(icu.UnicodeString("GMT-8:30"), pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR

    pos = icu.ParsePosition(0)
    result = fmt1.parse_offset_localized_gmt("GMT-8:30", pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR


def test_clone() -> None:
    # TimeZoneFormat *icu::TimeZoneFormat::clone()
    fmt1 = icu.TimeZoneFormat.create_instance("en")

    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.TimeZoneFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_format() -> None:
    fmt = icu.TimeZoneFormat.create_instance("en")
    zone = icu.TimeZone.create_time_zone("Asia/Tokyo")
    assert zone.get_raw_offset() == 9 * HOUR
    assert zone.get_dst_savings() == 0
    obj = icu.Formattable(zone)
    tz = icu.TimeZone.create_time_zone("America/Los_Angeles")
    date = 1358208000000.0  # 2013-01-15T00:00:00Z

    # [1]
    # UnicodeString &icu::TimeZoneFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to = icu.UnicodeString()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(obj, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "GMT+09:00"

    # [3]
    # UnicodeString &icu::TimeZoneFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(obj, append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::TimeZoneFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(obj, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "GMT+09:00"

    # [5]
    # UnicodeString &icu::TimeZoneFormat::format(
    #       UTimeZoneFormatStyle style,
    #       const TimeZone &tz,
    #       UDate date,
    #       UnicodeString &name,
    #       UTimeZoneFormatTimeType *timeType = NULL
    # )
    name = icu.UnicodeString()
    time_type = icu.UTimeZoneFormatTimeType(0)
    result = fmt.format(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_LONG,
        tz,
        date,
        name,
        time_type,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"
    assert time_type == icu.UTimeZoneFormatTimeType.UTZFMT_TIME_TYPE_STANDARD

    name.remove()
    result = fmt.format(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_LONG,
        tz,
        date,
        name,
        None,
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"

    name.remove()
    result = fmt.format(icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_LONG, tz, date, name)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_icu_51() -> None:
    fmt = icu.TimeZoneFormat.create_instance("en")
    offset = int(-8.5 * HOUR)
    result = icu.UnicodeString()

    # UnicodeString &icu::TimeZoneFormat::formatOffsetISO8601Basic(
    #       int32_t offset,
    #       UBool useUtcIndicator,
    #       UBool isShort,
    #       UBool ignoreSeconds,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    string = fmt.format_offset_iso8601_basic(offset, True, False, True, result)
    assert isinstance(string, icu.UnicodeString)
    assert id(result) == id(string)
    assert result == "-0830"

    # UnicodeString &icu::TimeZoneFormat::formatOffsetISO8601Extended(
    #       int32_t offset,
    #       UBool useUtcIndicator,
    #       UBool isShort,
    #       UBool ignoreSeconds,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    string = fmt.format_offset_iso8601_extended(offset, True, False, True, result)
    assert isinstance(string, icu.UnicodeString)
    assert id(result) == id(string)
    assert result == "-08:30"

    # UnicodeString &icu::TimeZoneFormat::formatOffsetShortLocalizedGMT(
    #       int32_t offset,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    string = fmt.format_offset_short_localized_gmt(offset, result)
    assert isinstance(string, icu.UnicodeString)
    assert id(result) == id(string)
    assert result == "GMT-8:30"

    # int32_t icu::TimeZoneFormat::parseOffsetShortLocalizedGMT(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = icu.ParsePosition(0)
    result = fmt.parse_offset_short_localized_gmt(icu.UnicodeString("GMT-8:30"), pos)
    assert pos.get_error_index() == -1
    assert result == offset

    pos = icu.ParsePosition(0)
    result = fmt.parse_offset_short_localized_gmt("GMT-8:30", pos)
    assert pos.get_error_index() == -1
    assert result == offset


def test_parse() -> None:
    fmt = icu.TimeZoneFormat.create_instance("en")
    tzid = icu.UnicodeString()

    # [1]
    # TimeZone *icu::TimeZoneFormat::parse(
    #       UTimeZoneFormatStyle style,
    #       const UnicodeString &text,
    #       ParsePosition &pos,
    #       int32_t parseOptions,
    #       UTimeZoneFormatTimeType *timeType = NULL
    # )
    pos = icu.ParsePosition(0)
    time_type = icu.UTimeZoneFormatTimeType(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        icu.UnicodeString("EST"),
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
        time_type,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR
    assert time_type == icu.UTimeZoneFormatTimeType.UTZFMT_TIME_TYPE_STANDARD

    pos = icu.ParsePosition(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        icu.UnicodeString("EST"),
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
        None,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = icu.ParsePosition(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        icu.UnicodeString("EST"),
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = icu.ParsePosition(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        "EST",
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = icu.ParsePosition(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        "AQTST",
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
    )
    assert pos.get_error_index() != -1
    assert tz is None

    # [2]
    # TimeZone *icu::TimeZoneFormat::parse(
    #       UTimeZoneFormatStyle style,
    #       const UnicodeString &text,
    #       ParsePosition &pos,
    #       UTimeZoneFormatTimeType *timeType = NULL
    # )
    pos = icu.ParsePosition(0)
    time_type = icu.UTimeZoneFormatTimeType(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        icu.UnicodeString("CST"),
        pos,
        time_type,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR
    assert time_type == icu.UTimeZoneFormatTimeType.UTZFMT_TIME_TYPE_STANDARD

    pos = icu.ParsePosition(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        icu.UnicodeString("CST"),
        pos,
        None,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = icu.ParsePosition(0)
    tz = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        icu.UnicodeString("CST"),
        pos,
    )
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = icu.ParsePosition(0)
    tz = fmt.parse(icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT, "CST", pos)
    assert pos.get_error_index() == -1
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = icu.ParsePosition(0)
    tz = fmt.parse(icu.UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT, "AQTST", pos)
    assert pos.get_error_index() != -1
    assert tz is None


def test_parse_upcasting() -> None:
    fmt = icu.TimeZoneFormat.create_instance("en")

    pos = icu.ParsePosition(0)
    zone = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "GMT+900",
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_NONE,
    )
    assert pos.get_index() != 0
    assert isinstance(zone, icu.SimpleTimeZone)

    pos = icu.ParsePosition(0)
    zone = fmt.parse(icu.UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT, "GMT+900", pos)
    assert pos.get_index() != 0
    assert isinstance(zone, icu.SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    pos = icu.ParsePosition(0)
    zone = fmt.parse(
        icu.UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "GMT",
        pos,
        icu.UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_NONE,
    )
    assert pos.get_index() != 0
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)

    pos = icu.ParsePosition(0)
    zone = fmt.parse(icu.UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT, "GMT", pos)
    assert pos.get_index() != 0
    assert not isinstance(zone, icu.SimpleTimeZone)
    assert isinstance(zone, icu.BasicTimeZone)

    # NULL
    pos = icu.ParsePosition(0)
    zone = fmt.parse(icu.UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT, "EST", pos)
    assert pos.get_index() == 0
    assert zone is None


def test_parse_object() -> None:
    fmt = icu.TimeZoneFormat.create_instance("en")

    # void icu::TimeZoneFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = icu.Formattable()
    parse_pos = icu.ParsePosition(0)
    fmt.parse_object(icu.UnicodeString("America/Los_Angeles"), result, parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_raw_offset() == -8 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    result = icu.Formattable()
    parse_pos = icu.ParsePosition(0)
    fmt.parse_object("America/Los_Angeles", result, parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_raw_offset() == -8 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    # [2]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    fmt.parse_object(icu.UnicodeString("Asia/Tokyo"), result)
    assert result.get_type() == icu.Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_raw_offset() == 9 * HOUR
    assert tz.get_dst_savings() == 0

    result = icu.Formattable()
    fmt.parse_object("Asia/Tokyo", result)
    assert result.get_type() == icu.Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, icu.TimeZone)
    assert tz.get_raw_offset() == 9 * HOUR
    assert tz.get_dst_savings() == 0

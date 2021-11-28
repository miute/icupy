import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)

import copy

from icupy.icu import (
    FieldPosition, FieldPositionIterator, Format, Formattable, ICUError,
    Locale, ParsePosition, TimeZone, TimeZoneFormat, TimeZoneNames,
    UErrorCode, UTimeZoneFormatGMTOffsetPatternType,
    UTimeZoneFormatParseOption, UTimeZoneFormatStyle, UTimeZoneFormatTimeType,
    U_MILLIS_PER_HOUR as HOUR, UnicodeString,
)


def test_api():
    assert issubclass(TimeZoneFormat, Format)

    # static TimeZoneFormat *icu::TimeZoneFormat::createInstance(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt1 = TimeZoneFormat.create_instance(Locale("en"))
    fmt2 = TimeZoneFormat.create_instance("en")
    fmt3 = TimeZoneFormat.create_instance(Locale("ja"))
    assert isinstance(fmt1, TimeZoneFormat)
    assert isinstance(fmt2, TimeZoneFormat)
    assert isinstance(fmt3, TimeZoneFormat)

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
    result = UnicodeString()
    string = fmt1.format_offset_localized_gmt(int(-8.5 * HOUR), result)
    assert isinstance(string, UnicodeString)
    assert id(result) == id(string)
    assert result == "GMT-08:30"

    # uint32_t icu::TimeZoneFormat::getDefaultParseOptions(void)
    assert (fmt2.get_default_parse_options()
            == UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_NONE)

    # void icu::TimeZoneFormat::setDefaultParseOptions(uint32_t flags)
    fmt2.set_default_parse_options(
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES)
    assert (fmt2.get_default_parse_options()
            == UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES)

    # UnicodeString &icu::TimeZoneFormat::getGMTOffsetDigits(
    #       UnicodeString &digits
    # )
    digits = UnicodeString()
    result = fmt2.get_gmt_offset_digits(digits)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(digits)
    assert result == "0123456789"

    # void icu::TimeZoneFormat::setGMTOffsetDigits(
    #       const UnicodeString &digits,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_offset_digits(UnicodeString("abcdefghij"))
    assert fmt2.get_gmt_offset_digits(digits) == "abcdefghij"

    fmt2.set_gmt_offset_digits("0123456789")
    assert fmt2.get_gmt_offset_digits(digits) == "0123456789"

    # UnicodeString &icu::TimeZoneFormat::getGMTOffsetPattern(
    #       UTimeZoneFormatGMTOffsetPatternType type,
    #       UnicodeString &pattern
    # )
    pattern = UnicodeString()
    result = fmt2.get_gmt_offset_pattern(
        UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        pattern)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(pattern)
    assert result == "+HH:mm"

    # void icu::TimeZoneFormat::setGMTOffsetPattern(
    #       UTimeZoneFormatGMTOffsetPatternType type,
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_offset_pattern(
        UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        UnicodeString("+H:mm"))
    assert fmt2.get_gmt_offset_pattern(
        UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        pattern) == "+H:mm"

    fmt2.set_gmt_offset_pattern(
        UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        "+HH:mm")
    assert fmt2.get_gmt_offset_pattern(
        UTimeZoneFormatGMTOffsetPatternType.UTZFMT_PAT_POSITIVE_HM,
        pattern) == "+HH:mm"

    # UnicodeString &icu::TimeZoneFormat::getGMTPattern(UnicodeString &pattern)
    result = fmt2.get_gmt_pattern(pattern)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(pattern)
    assert result == "GMT{0}"

    # void icu::TimeZoneFormat::setGMTPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_pattern(UnicodeString("GMT {0}"))
    assert fmt2.get_gmt_pattern(pattern) == "GMT {0}"

    fmt2.set_gmt_pattern("GMT{0}")
    assert fmt2.get_gmt_pattern(pattern) == "GMT{0}"

    # UnicodeString &icu::TimeZoneFormat::getGMTZeroFormat(
    #       UnicodeString &gmtZeroFormat
    # )
    gmt_zero_format = UnicodeString()
    result = fmt2.get_gmt_zero_format(gmt_zero_format)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(gmt_zero_format)
    assert result == "GMT"

    # void icu::TimeZoneFormat::setGMTZeroFormat(
    #       const UnicodeString &gmtZeroFormat,
    #       UErrorCode &status
    # )
    fmt2.set_gmt_zero_format(UnicodeString("UTC"))
    assert fmt2.get_gmt_zero_format(gmt_zero_format) == "UTC"

    fmt2.set_gmt_zero_format("GMT")
    assert fmt2.get_gmt_zero_format(gmt_zero_format) == "GMT"

    # const TimeZoneNames *icu::TimeZoneFormat::getTimeZoneNames()
    tzn = fmt2.get_time_zone_names()
    assert isinstance(tzn, TimeZoneNames)

    # void icu::TimeZoneFormat::setTimeZoneNames(const TimeZoneNames &tznames)
    tznames = TimeZoneNames.create_instance("ja")
    assert tznames != tzn
    fmt2.set_time_zone_names(tznames)
    assert fmt2.get_time_zone_names() == tznames

    # int32_t icu::TimeZoneFormat::parseOffsetISO8601(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = ParsePosition(0)
    result = fmt1.parse_offset_iso8601(UnicodeString("-08:30"), pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR

    pos = ParsePosition(0)
    result = fmt1.parse_offset_iso8601("-08:30", pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR

    # int32_t icu::TimeZoneFormat::parseOffsetLocalizedGMT(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = ParsePosition(0)
    result = fmt1.parse_offset_localized_gmt(UnicodeString("GMT-8:30"), pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR

    pos = ParsePosition(0)
    result = fmt1.parse_offset_localized_gmt("GMT-8:30", pos)
    assert pos.get_error_index() == -1
    assert result == -8.5 * HOUR


def test_clone():
    # TimeZoneFormat *icu::TimeZoneFormat::clone()
    fmt1 = TimeZoneFormat.create_instance("en")

    fmt2 = fmt1.clone()
    assert isinstance(fmt2, TimeZoneFormat)
    assert fmt2 == fmt1

    fmt3 = copy.copy(fmt1)
    assert fmt3 == fmt1

    fmt4 = copy.deepcopy(fmt1)
    assert fmt4 == fmt1


def test_format():
    fmt = TimeZoneFormat.create_instance("en")
    zone = TimeZone.create_time_zone("Asia/Tokyo")
    assert zone.get_raw_offset() == 9 * HOUR
    assert zone.get_dst_savings() == 0
    obj = Formattable(zone)
    tz = TimeZone.create_time_zone("America/Los_Angeles")
    date = 1358208000000.0  # 2013-01-15T00:00:00Z

    # [1]
    # UnicodeString &icu::TimeZoneFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to = UnicodeString()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(obj, append_to, pos)
    assert isinstance(result, UnicodeString)
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
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(obj, append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::TimeZoneFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(obj, append_to)
    assert isinstance(result, UnicodeString)
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
    name = UnicodeString()
    time_type = UTimeZoneFormatTimeType(0)
    result = fmt.format(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_LONG,
        tz,
        date,
        name,
        time_type)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"
    assert time_type == UTimeZoneFormatTimeType.UTZFMT_TIME_TYPE_STANDARD

    name.remove()
    result = fmt.format(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_LONG,
        tz,
        date,
        name,
        None)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"

    name.remove()
    result = fmt.format(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_LONG,
        tz,
        date,
        name)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(name)
    assert result == "Pacific Standard Time"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_icu_51():
    fmt = TimeZoneFormat.create_instance("en")
    offset = int(-8.5 * HOUR)
    result = UnicodeString()

    # UnicodeString &icu::TimeZoneFormat::formatOffsetISO8601Basic(
    #       int32_t offset,
    #       UBool useUtcIndicator,
    #       UBool isShort,
    #       UBool ignoreSeconds,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    string = fmt.format_offset_iso8601_basic(
        offset,
        True,
        False,
        True,
        result)
    assert isinstance(string, UnicodeString)
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
    string = fmt.format_offset_iso8601_extended(
        offset,
        True,
        False,
        True,
        result)
    assert isinstance(string, UnicodeString)
    assert id(result) == id(string)
    assert result == "-08:30"

    # UnicodeString &icu::TimeZoneFormat::formatOffsetShortLocalizedGMT(
    #       int32_t offset,
    #       UnicodeString &result,
    #       UErrorCode &status
    # )
    string = fmt.format_offset_short_localized_gmt(
        offset,
        result)
    assert isinstance(string, UnicodeString)
    assert id(result) == id(string)
    assert result == "GMT-8:30"

    # int32_t icu::TimeZoneFormat::parseOffsetShortLocalizedGMT(
    #       const UnicodeString &text,
    #       ParsePosition &pos
    # )
    pos = ParsePosition(0)
    result = fmt.parse_offset_short_localized_gmt(
        UnicodeString("GMT-8:30"),
        pos)
    assert pos.get_error_index() == -1
    assert result == offset

    pos = ParsePosition(0)
    result = fmt.parse_offset_short_localized_gmt(
        "GMT-8:30",
        pos)
    assert pos.get_error_index() == -1
    assert result == offset


def test_parse():
    fmt = TimeZoneFormat.create_instance("en")
    tzid = UnicodeString()

    # [1]
    # TimeZone *icu::TimeZoneFormat::parse(
    #       UTimeZoneFormatStyle style,
    #       const UnicodeString &text,
    #       ParsePosition &pos,
    #       int32_t parseOptions,
    #       UTimeZoneFormatTimeType *timeType = NULL
    # )
    pos = ParsePosition(0)
    time_type = UTimeZoneFormatTimeType(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        UnicodeString("EST"),
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
        time_type)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR
    assert time_type == UTimeZoneFormatTimeType.UTZFMT_TIME_TYPE_STANDARD

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        UnicodeString("EST"),
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES,
        None)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        UnicodeString("EST"),
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        "EST",
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/New_York"
    assert tz.get_raw_offset() == -5 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        "AQTST",
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_ALL_STYLES)
    assert pos.get_error_index() != -1
    assert tz is None

    # [2]
    # TimeZone *icu::TimeZoneFormat::parse(
    #       UTimeZoneFormatStyle style,
    #       const UnicodeString &text,
    #       ParsePosition &pos,
    #       UTimeZoneFormatTimeType *timeType = NULL
    # )
    pos = ParsePosition(0)
    time_type = UTimeZoneFormatTimeType(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        UnicodeString("CST"),
        pos,
        time_type)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR
    assert time_type == UTimeZoneFormatTimeType.UTZFMT_TIME_TYPE_STANDARD

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        UnicodeString("CST"),
        pos,
        None)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        UnicodeString("CST"),
        pos)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        "CST",
        pos)
    assert pos.get_error_index() == -1
    assert isinstance(tz, TimeZone)
    assert tz.get_id(tzid) == "America/Chicago"
    assert tz.get_raw_offset() == -6 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    pos = ParsePosition(0)
    tz = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_SPECIFIC_SHORT,
        "AQTST",
        pos)
    assert pos.get_error_index() != -1
    assert tz is None


def test_parse_upcasting():
    from icupy.icu import BasicTimeZone, SimpleTimeZone

    fmt = TimeZoneFormat.create_instance("en")

    pos = ParsePosition(0)
    zone = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "GMT+900",
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_NONE)
    assert pos.get_index() != 0
    assert isinstance(zone, SimpleTimeZone)

    pos = ParsePosition(0)
    zone = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "GMT+900",
        pos)
    assert pos.get_index() != 0
    assert isinstance(zone, SimpleTimeZone)

    # TimeZone -> BasicTimeZone
    pos = ParsePosition(0)
    zone = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "GMT",
        pos,
        UTimeZoneFormatParseOption.UTZFMT_PARSE_OPTION_NONE)
    assert pos.get_index() != 0
    assert not isinstance(zone, SimpleTimeZone)
    assert isinstance(zone, BasicTimeZone)

    pos = ParsePosition(0)
    zone = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "GMT",
        pos)
    assert pos.get_index() != 0
    assert not isinstance(zone, SimpleTimeZone)
    assert isinstance(zone, BasicTimeZone)

    # NULL
    pos = ParsePosition(0)
    zone = fmt.parse(
        UTimeZoneFormatStyle.UTZFMT_STYLE_LOCALIZED_GMT,
        "EST",
        pos)
    assert pos.get_index() == 0
    assert zone is None


def test_parse_object():
    fmt = TimeZoneFormat.create_instance("en")

    # void icu::TimeZoneFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = Formattable()
    parse_pos = ParsePosition(0)
    fmt.parse_object(UnicodeString("America/Los_Angeles"),
                     result,
                     parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, TimeZone)
    assert tz.get_raw_offset() == -8 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    result = Formattable()
    parse_pos = ParsePosition(0)
    fmt.parse_object("America/Los_Angeles",
                     result,
                     parse_pos)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, TimeZone)
    assert tz.get_raw_offset() == -8 * HOUR
    assert tz.get_dst_savings() == 1 * HOUR

    # [2]
    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    fmt.parse_object(UnicodeString("Asia/Tokyo"), result)
    assert result.get_type() == Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, TimeZone)
    assert tz.get_raw_offset() == 9 * HOUR
    assert tz.get_dst_savings() == 0

    result = Formattable()
    fmt.parse_object("Asia/Tokyo", result)
    assert result.get_type() == Formattable.OBJECT
    tz = result.get_object()
    assert isinstance(tz, TimeZone)
    assert tz.get_raw_offset() == 9 * HOUR
    assert tz.get_dst_savings() == 0

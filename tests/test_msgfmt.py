import copy

import pytest

from icupy import icu


def test_api() -> None:
    # static UnicodeString icu::MessageFormat::autoQuoteApostrophe(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    result = icu.MessageFormat.auto_quote_apostrophe(icu.UnicodeString("'"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "''"

    result = icu.MessageFormat.auto_quote_apostrophe("'")
    assert isinstance(result, icu.UnicodeString)
    assert result == "''"

    pattern = icu.UnicodeString(
        "At {1,time,::jmm} on {1,date,::dMMMM}, there was {2} on planet {0,number}.",
    )
    fmt = icu.MessageFormat(pattern, icu.Locale.get_us())

    # UMessagePatternApostropheMode icu::MessageFormat::getApostropheMode()
    assert (
        fmt.get_apostrophe_mode()
        == icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL
    )

    # Format *icu::MessageFormat::getFormat(
    #       const UnicodeString &formatName,
    #       UErrorCode &status
    # )
    result = fmt.get_format(icu.UnicodeString("0"))
    assert isinstance(result, icu.DecimalFormat)

    result = fmt.get_format(icu.UnicodeString("1"))
    assert isinstance(result, icu.SimpleDateFormat)

    result = fmt.get_format(icu.UnicodeString("2"))
    assert result is None

    result = fmt.get_format("0")
    assert isinstance(result, icu.DecimalFormat)

    result = fmt.get_format("1")
    assert isinstance(result, icu.SimpleDateFormat)

    result = fmt.get_format("2")
    assert result is None

    # StringEnumeration *icu::MessageFormat::getFormatNames(UErrorCode &status)
    it = fmt.get_format_names()
    assert isinstance(it, icu.StringEnumeration)
    assert list(it) == ["1", "1", "2", "0"]

    # const Format **icu::MessageFormat::getFormats(int32_t &count)
    formats = fmt.get_formats()
    assert isinstance(formats, list)
    assert len(formats) == 4
    assert isinstance(formats[0], icu.SimpleDateFormat)
    assert isinstance(formats[1], icu.SimpleDateFormat)
    assert formats[2] is None
    assert isinstance(formats[3], icu.DecimalFormat)

    # const Locale &icu::MessageFormat::getLocale(void)
    result = fmt.get_locale()
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("en", "US")

    # Locale icu::Format::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    loc = fmt.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert loc == icu.Locale("en-US")

    # void icu::MessageFormat::setLocale(const Locale &theLocale)
    fmt.set_locale(icu.Locale.get_france())
    assert fmt.get_locale() == icu.Locale("fr", "FR")

    fmt.set_locale("en-US")
    assert fmt.get_locale() == icu.Locale("en", "US")

    # UnicodeString &icu::MessageFormat::toPattern(UnicodeString &appendTo)
    append_to = icu.UnicodeString()
    result = fmt.to_pattern(append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == pattern

    # UBool icu::MessageFormat::usesNamedArguments()
    assert fmt.uses_named_arguments() is False

    # UBool icu::MessageFormat::operator==(const Format &other)
    fmt1 = icu.MessageFormat("{0} {1}", icu.Locale.get_us())
    fmt2 = icu.MessageFormat("{0} {1}", icu.Locale.get_us())
    fmt3 = icu.MessageFormat("{0} {1}", icu.Locale.get_france())
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)


def test_apply_pattern() -> None:
    fmt1 = icu.MessageFormat("{0} {1}")
    fmt2 = fmt1.clone()
    assert fmt1 == fmt2

    # [1]
    # void icu::MessageFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt1.apply_pattern(icu.UnicodeString("{1} {0}"))
    assert fmt1 != fmt2

    fmt2.apply_pattern("{1} {0}")
    assert fmt1 == fmt2

    # [2]
    # void icu::MessageFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UMessagePatternApostropheMode aposMode,
    #       UParseError *parseError,
    #       UErrorCode &status
    # )
    pattern = icu.UnicodeString("ab{0,choice,0#1'2''3'''4''''.}yz")
    parse_error = icu.UParseError()
    fmt1.apply_pattern(
        pattern,
        icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED,
        parse_error,
    )
    assert (
        fmt1.get_apostrophe_mode()
        == icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED
    )
    assert fmt1 != fmt2

    parse_error = icu.UParseError()
    fmt2.apply_pattern(
        str(pattern),
        icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED,
        parse_error,
    )
    assert (
        fmt2.get_apostrophe_mode()
        == icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED
    )
    assert fmt1 == fmt2

    fmt1.apply_pattern(
        pattern,
        icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL,
        None,
    )
    assert (
        fmt1.get_apostrophe_mode()
        == icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL
    )
    assert fmt1 != fmt2

    fmt2.apply_pattern(
        str(pattern),
        icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL,
        None,
    )
    assert (
        fmt2.get_apostrophe_mode()
        == icu.UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL
    )
    assert fmt1 == fmt2

    # [3]
    # void icu::MessageFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = icu.UParseError()
    fmt1.apply_pattern(icu.UnicodeString("{1} {0}"), parse_error)
    assert fmt1 != fmt2

    parse_error = icu.UParseError()
    fmt2.apply_pattern("{1} {0}", parse_error)
    assert fmt1 == fmt2


def test_clone() -> None:
    fmt1 = icu.MessageFormat("{0} {1}")

    # MessageFormat *icu::MessageFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.MessageFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format() -> None:
    fmt = icu.MessageFormat(
        "At {1,time,::jmm} on {1,date,::dMMMM}, there was {2} on planet {0,number}.",
        icu.Locale.get_us(),
    )
    formats = fmt.get_formats()
    tz = icu.TimeZone.get_gmt()
    formats[0].set_time_zone(tz)
    formats[1].set_time_zone(tz)
    date = 1637685775000.0  # 2021-11-23T16:42:55Z
    append_to = icu.UnicodeString()

    # [1], [2]
    # UnicodeString &icu::MessageFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(
        [
            icu.Formattable(7),
            icu.Formattable(date, icu.Formattable.IS_DATE),
            icu.Formattable(icu.UnicodeString("a disturbance in the Force")),
        ]
    )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(obj, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.",
        "At 4:42\u202fPM on November 23, "
        "there was a disturbance in the Force on planet 7.",  # ICU>=72
    )

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(
        [
            icu.Formattable(7),
            icu.Formattable(date, icu.Formattable.IS_DATE),
            icu.Formattable(icu.UnicodeString("a disturbance in the Force")),
        ]
    )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(obj, append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(obj, append_to, None)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    obj = icu.Formattable(
        [
            icu.Formattable(7),
            icu.Formattable(date, icu.Formattable.IS_DATE),
            icu.Formattable(icu.UnicodeString("a disturbance in the Force")),
        ]
    )
    append_to.remove()
    result = fmt.format(obj, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.",
        "At 4:42\u202fPM on November 23, "
        "there was a disturbance in the Force on planet 7.",  # ICU>=72
    )

    # [5]
    # UnicodeString &icu::MessageFormat::format(
    #       const Formattable *source,
    #       int32_t count,
    #       UnicodeString &appendTo,
    #       FieldPosition &ignore,
    #       UErrorCode &status
    # )
    source = [
        icu.Formattable(7),
        icu.Formattable(date, icu.Formattable.IS_DATE),
        icu.Formattable(icu.UnicodeString("a disturbance in the Force")),
    ]
    append_to.remove()
    ignore = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(source, len(source), append_to, ignore)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.",
        "At 4:42\u202fPM on November 23, "
        "there was a disturbance in the Force on planet 7.",  # ICU>=72
    )

    append_to.remove()
    ignore = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(source, -1, append_to, ignore)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.",
        "At 4:42\u202fPM on November 23, "
        "there was a disturbance in the Force on planet 7.",  # ICU>=72
    )

    # *NotImplemented*
    # [6]
    # static UnicodeString &icu::MessageFormat::format(
    #       const UnicodeString &pattern,
    #       const Formattable *arguments,
    #       int32_t count,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )

    # [7]
    # UnicodeString &icu::MessageFormat::format(
    #       const UnicodeString *argumentNames,
    #       const Formattable *arguments,
    #       int32_t count,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    argument_names = [
        icu.UnicodeString("1"),
        icu.UnicodeString("2"),
        icu.UnicodeString("0"),
    ]
    arguments = [
        icu.Formattable(date, icu.Formattable.IS_DATE),
        icu.Formattable(icu.UnicodeString("a disturbance in the Force")),
        icu.Formattable(7),
    ]
    append_to.remove()
    result = fmt.format(argument_names, arguments, len(argument_names), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.",
        "At 4:42\u202fPM on November 23, "
        "there was a disturbance in the Force on planet 7.",  # ICU>=72
    )

    append_to.remove()
    result = fmt.format(argument_names, arguments, -1, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result in (
        "At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.",
        "At 4:42\u202fPM on November 23, "
        "there was a disturbance in the Force on planet 7.",  # ICU>=72
    )


def test_message_format() -> None:
    pattern = icu.UnicodeString("{0} {1}")
    new_locale = icu.Locale.get_us()

    # [1]
    # icu::MessageFormat::MessageFormat(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt1 = icu.MessageFormat(pattern)

    fmt1a = icu.MessageFormat(str(pattern))
    assert fmt1 == fmt1a

    # [2]
    # icu::MessageFormat::MessageFormat(
    #       const UnicodeString &pattern,
    #       const Locale &newLocale,
    #       UErrorCode &status
    # )
    fmt2 = icu.MessageFormat(pattern, new_locale)

    fmt2a = icu.MessageFormat(str(pattern), new_locale)
    assert fmt2 == fmt2a

    fmt2b = icu.MessageFormat(pattern, "en-US")
    assert fmt2 == fmt2b

    fmt2c = icu.MessageFormat(str(pattern), "en-US")
    assert fmt2 == fmt2c

    # [3]
    # icu::MessageFormat::MessageFormat(
    #       const UnicodeString &pattern,
    #       const Locale &newLocale,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = icu.UParseError()
    fmt3 = icu.MessageFormat(pattern, new_locale, parse_error)

    parse_error = icu.UParseError()
    fmt3a = icu.MessageFormat(str(pattern), new_locale, parse_error)
    assert fmt3 == fmt3a

    parse_error = icu.UParseError()
    fmt3b = icu.MessageFormat(pattern, "en-US", parse_error)
    assert fmt3 == fmt3b

    parse_error = icu.UParseError()
    fmt3c = icu.MessageFormat(str(pattern), "en-US", parse_error)
    assert fmt3 == fmt3c

    # [4]
    # icu::MessageFormat::MessageFormat(const MessageFormat &)
    fmt4 = icu.MessageFormat(fmt1)
    assert fmt1 == fmt4


def test_parse() -> None:
    # From icu/source/test/intltest/tmsgfmt.cpp
    fmt = icu.MessageFormat("{0} =sep= {1}", icu.Locale.get_us())
    s = "abc =sep= def"

    # [1]
    # Formattable *icu::MessageFormat::parse(
    #       const UnicodeString &source,
    #       int32_t &count,
    #       UErrorCode &status
    # )
    objs = fmt.parse(icu.UnicodeString(s))
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, icu.Formattable) for x in objs)
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    objs = fmt.parse(s)
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, icu.Formattable) for x in objs)
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    # [2]
    # Formattable *icu::MessageFormat::parse(
    #       const UnicodeString &source,
    #       ParsePosition &pos,
    #       int32_t &count
    # )
    pos = icu.ParsePosition()
    objs = fmt.parse(icu.UnicodeString(s), pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, icu.Formattable) for x in objs)
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    pos = icu.ParsePosition()
    objs = fmt.parse(s, pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, icu.Formattable) for x in objs)
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    # void icu::MessageFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &pos
    # )
    result = icu.Formattable()
    pos = icu.ParsePosition()
    fmt.parse_object(icu.UnicodeString(s), result, pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    result = icu.Formattable()
    pos = icu.ParsePosition()
    fmt.parse_object(s, result, pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    fmt.parse_object(icu.UnicodeString(s), result)
    assert result.get_type() == icu.Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"

    result = icu.Formattable()
    fmt.parse_object(s, result)
    assert result.get_type() == icu.Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == icu.Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == icu.Formattable.STRING
    assert objs[1].get_string() == "def"


def test_set_format() -> None:
    fmt = icu.MessageFormat("{0,date} {1,time}", icu.Locale.get_us())
    dtfmt0 = icu.SimpleDateFormat("yyyy.MM.dd")
    dtfmt1 = icu.SimpleDateFormat("HH:mm:ss z")
    date = 1637685775000.0  # 2021-11-23T16:42:55Z
    obj = icu.Formattable(
        [
            icu.Formattable(date, icu.Formattable.IS_DATE),
            icu.Formattable(date, icu.Formattable.IS_DATE),
        ]
    )
    append_to = icu.UnicodeString()

    # [1]
    # void icu::MessageFormat::setFormat(
    #       const UnicodeString &formatName,
    #       const Format &format,
    #       UErrorCode &status
    # )
    tz = icu.TimeZone.get_gmt()
    dtfmt0.set_time_zone(tz)
    dtfmt1.set_time_zone(tz)

    fmt.set_format(icu.UnicodeString("0"), dtfmt0)
    fmt.set_format("1", dtfmt1)

    append_to.remove()
    assert fmt.format(obj, append_to) == "2021.11.23 16:42:55 GMT"

    # [2]
    # void icu::MessageFormat::setFormat(
    #       int32_t formatNumber,
    #       const Format &format
    # )
    tz = icu.TimeZone.create_time_zone("PST")
    dtfmt0.set_time_zone(tz)
    dtfmt1.set_time_zone(tz)

    fmt.set_format(0, dtfmt0)
    fmt.set_format(1, dtfmt1)

    append_to.remove()
    assert fmt.format(obj, append_to) in (
        "2021.11.23 08:42:55 GMT-8",
        "2021.11.23 08:42:55 PST",
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_set_format_62() -> None:
    fmt = icu.MessageFormat("{0,number,currency}", icu.Locale.get_us())
    obj = icu.Formattable([icu.Formattable(444444.55)])
    append_to = icu.UnicodeString()
    assert fmt.format(obj, append_to) == "$444,444.55"

    numfmt = (
        icu.number.NumberFormatter.for_skeleton("currency/USD unit-width-iso-code")
        .locale(icu.Locale.get_us())
        .to_format()
    )
    fmt.set_format(0, numfmt)
    append_to.remove()
    assert fmt.format(obj, append_to) == "USD\xa0444,444.55"


def test_set_formats() -> None:
    fmt = icu.MessageFormat(
        "At {1,time,::jmm} on {1,date,::dMMMM}, there was {2} on planet {0,number}.",
        icu.Locale.get_us(),
    )
    formats = fmt.get_formats()

    # void icu::MessageFormat::setFormats(
    #       const Format **newFormats,
    #       int32_t cnt
    # )
    new_formats = [
        icu.SimpleDateFormat("HH:mm:ss z"),
        icu.SimpleDateFormat("yyyy.MM.dd"),
        None,
        formats[3].clone(),
    ]
    tz = icu.TimeZone.get_gmt()
    new_formats[0].set_time_zone(tz)
    new_formats[1].set_time_zone(tz)
    fmt.set_formats(new_formats, len(new_formats))

    date = 1637685775000.0  # 2021-11-23T16:42:55Z
    obj = icu.Formattable(
        [
            icu.Formattable(7),
            icu.Formattable(date, icu.Formattable.IS_DATE),
            icu.Formattable(icu.UnicodeString("a disturbance in the Force")),
        ]
    )
    append_to = icu.UnicodeString()
    assert fmt.format(obj, append_to) == (
        "At 16:42:55 GMT on 2021.11.23, there was a disturbance in the Force on planet 7."
    )

    tz = icu.TimeZone.create_time_zone("PST")
    new_formats[0].set_time_zone(tz)
    new_formats[1].set_time_zone(tz)
    fmt.set_formats(new_formats)  # cnt is optional

    append_to.remove()
    assert fmt.format(obj, append_to) in (
        "At 08:42:55 GMT-8 on 2021.11.23, there was a disturbance in the Force on planet 7.",
        "At 08:42:55 PST on 2021.11.23, there was a disturbance in the Force on planet 7.",
    )

import copy

import pytest
from icupy.icu import (
    DecimalFormat, FieldPosition, FieldPositionIterator, Formattable,
    ICUError, Locale, MessageFormat, ParsePosition, SimpleDateFormat,
    StringEnumeration, TimeZone, UErrorCode, UMessagePatternApostropheMode,
    UParseError, U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    # static UnicodeString icu::MessageFormat::autoQuoteApostrophe(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    result = MessageFormat.auto_quote_apostrophe(UnicodeString("'"))
    assert isinstance(result, UnicodeString)
    assert result == "''"

    result = MessageFormat.auto_quote_apostrophe("'")
    assert isinstance(result, UnicodeString)
    assert result == "''"

    pattern = UnicodeString(
        "At {1,time,::jmm} on {1,date,::dMMMM}, "
        "there was {2} on planet {0,number}.",
    )
    fmt = MessageFormat(pattern, Locale.get_us())

    # UMessagePatternApostropheMode icu::MessageFormat::getApostropheMode()
    assert (fmt.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL)

    # Format *icu::MessageFormat::getFormat(
    #       const UnicodeString &formatName,
    #       UErrorCode &status
    # )
    result = fmt.get_format(UnicodeString("0"))
    assert isinstance(result, DecimalFormat)

    result = fmt.get_format(UnicodeString("1"))
    assert isinstance(result, SimpleDateFormat)

    result = fmt.get_format(UnicodeString("2"))
    assert result is None

    result = fmt.get_format("0")
    assert isinstance(result, DecimalFormat)

    result = fmt.get_format("1")
    assert isinstance(result, SimpleDateFormat)

    result = fmt.get_format("2")
    assert result is None

    # StringEnumeration *icu::MessageFormat::getFormatNames(UErrorCode &status)
    it = fmt.get_format_names()
    assert isinstance(it, StringEnumeration)
    assert list(it) == ["1", "1", "2", "0"]

    # const Format **icu::MessageFormat::getFormats(int32_t &count)
    formats = fmt.get_formats()
    assert isinstance(formats, list)
    assert len(formats) == 4
    assert isinstance(formats[0], SimpleDateFormat)
    assert isinstance(formats[1], SimpleDateFormat)
    assert formats[2] is None
    assert isinstance(formats[3], DecimalFormat)

    # const Locale &icu::MessageFormat::getLocale(void)
    result = fmt.get_locale()
    assert isinstance(result, Locale)
    assert result == Locale("en", "US")

    # void icu::MessageFormat::setLocale(const Locale &theLocale)
    fmt.set_locale(Locale.get_france())
    assert fmt.get_locale() == Locale("fr", "FR")

    fmt.set_locale("en-US")
    assert fmt.get_locale() == Locale("en", "US")

    # UnicodeString &icu::MessageFormat::toPattern(UnicodeString &appendTo)
    append_to = UnicodeString()
    result = fmt.to_pattern(append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == pattern

    # UBool icu::MessageFormat::usesNamedArguments()
    assert not fmt.uses_named_arguments()

    # UBool icu::MessageFormat::operator==(const Format &other)
    fmt1 = MessageFormat("{0} {1}", Locale.get_us())
    fmt2 = MessageFormat("{0} {1}", Locale.get_us())
    fmt3 = MessageFormat("{0} {1}", Locale.get_france())
    assert fmt1 == fmt2
    assert not (fmt1 == fmt3)
    assert not (fmt2 == fmt3)


def test_apply_pattern():
    fmt1 = MessageFormat("{0} {1}")
    fmt2 = fmt1.clone()
    assert fmt1 == fmt2

    # [1]
    # void icu::MessageFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt1.apply_pattern(UnicodeString("{1} {0}"))
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
    pattern = UnicodeString("ab{0,choice,0#1'2''3'''4''''.}yz")
    parse_error = UParseError()
    fmt1.apply_pattern(
        pattern,
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED,
        parse_error)
    assert (fmt1.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED)
    assert fmt1 != fmt2

    parse_error = UParseError()
    fmt2.apply_pattern(
        str(pattern),
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED,
        parse_error)
    assert (fmt2.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED)
    assert fmt1 == fmt2

    fmt1.apply_pattern(
        pattern,
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL,
        None)
    assert (fmt1.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL)
    assert fmt1 != fmt2

    fmt2.apply_pattern(
        str(pattern),
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL,
        None)
    assert (fmt2.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL)
    assert fmt1 == fmt2

    # [3]
    # void icu::MessageFormat::applyPattern(
    #       const UnicodeString &pattern,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = UParseError()
    fmt1.apply_pattern(UnicodeString("{1} {0}"), parse_error)
    assert fmt1 != fmt2

    parse_error = UParseError()
    fmt2.apply_pattern("{1} {0}", parse_error)
    assert fmt1 == fmt2


def test_clone():
    fmt1 = MessageFormat("{0} {1}")

    # MessageFormat *icu::MessageFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, MessageFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format():
    fmt = MessageFormat(
        "At {1,time,::jmm} on {1,date,::dMMMM}, "
        "there was {2} on planet {0,number}.",
        Locale.get_us())
    formats = fmt.get_formats()
    tz = TimeZone.get_gmt()
    formats[0].set_time_zone(tz)
    formats[1].set_time_zone(tz)
    date = 1637685775000.0  # 2021-11-23T16:42:55Z
    append_to = UnicodeString()

    # [1], [2]
    # UnicodeString &icu::MessageFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    obj = Formattable([
        Formattable(7),
        Formattable(date, Formattable.IS_DATE),
        Formattable(UnicodeString("a disturbance in the Force")),
    ])
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(obj, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "At 4:42 PM on November 23, "
        "there was a disturbance in the Force on planet 7."
    )

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [3]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    obj = Formattable([
        Formattable(7),
        Formattable(date, Formattable.IS_DATE),
        Formattable(UnicodeString("a disturbance in the Force")),
    ])
    append_to.remove()
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(obj, append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    append_to.remove()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(obj, append_to, None)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # [4]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    obj = Formattable([
        Formattable(7),
        Formattable(date, Formattable.IS_DATE),
        Formattable(UnicodeString("a disturbance in the Force")),
    ])
    append_to.remove()
    result = fmt.format(obj, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "At 4:42 PM on November 23, "
        "there was a disturbance in the Force on planet 7."
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
        Formattable(7),
        Formattable(date, Formattable.IS_DATE),
        Formattable(UnicodeString("a disturbance in the Force")),
    ]
    append_to.remove()
    ignore = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(source, len(source), append_to, ignore)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "At 4:42 PM on November 23, "
        "there was a disturbance in the Force on planet 7."
    )

    append_to.remove()
    ignore = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(source, -1, append_to, ignore)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "At 4:42 PM on November 23, "
        "there was a disturbance in the Force on planet 7."
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
        UnicodeString("1"),
        UnicodeString("2"),
        UnicodeString("0"),
    ]
    arguments = [
        Formattable(date, Formattable.IS_DATE),
        Formattable(UnicodeString("a disturbance in the Force")),
        Formattable(7),
    ]
    append_to.remove()
    result = fmt.format(
        argument_names,
        arguments,
        len(argument_names),
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "At 4:42 PM on November 23, "
        "there was a disturbance in the Force on planet 7."
    )

    append_to.remove()
    result = fmt.format(
        argument_names,
        arguments,
        -1,
        append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "At 4:42 PM on November 23, "
        "there was a disturbance in the Force on planet 7."
    )


def test_message_format():
    pattern = UnicodeString("{0} {1}")
    new_locale = Locale.get_us()

    # [1]
    # icu::MessageFormat::MessageFormat(
    #       const UnicodeString &pattern,
    #       UErrorCode &status
    # )
    fmt1 = MessageFormat(pattern)

    fmt1a = MessageFormat(str(pattern))
    assert fmt1 == fmt1a

    # [2]
    # icu::MessageFormat::MessageFormat(
    #       const UnicodeString &pattern,
    #       const Locale &newLocale,
    #       UErrorCode &status
    # )
    fmt2 = MessageFormat(pattern, new_locale)

    fmt2a = MessageFormat(str(pattern), new_locale)
    assert fmt2 == fmt2a

    fmt2b = MessageFormat(pattern, "en-US")
    assert fmt2 == fmt2b

    fmt2c = MessageFormat(str(pattern), "en-US")
    assert fmt2 == fmt2c

    # [3]
    # icu::MessageFormat::MessageFormat(
    #       const UnicodeString &pattern,
    #       const Locale &newLocale,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = UParseError()
    fmt3 = MessageFormat(pattern, new_locale, parse_error)

    parse_error = UParseError()
    fmt3a = MessageFormat(str(pattern), new_locale, parse_error)
    assert fmt3 == fmt3a

    parse_error = UParseError()
    fmt3b = MessageFormat(pattern, "en-US", parse_error)
    assert fmt3 == fmt3b

    parse_error = UParseError()
    fmt3c = MessageFormat(str(pattern), "en-US", parse_error)
    assert fmt3 == fmt3c

    # [4]
    # icu::MessageFormat::MessageFormat(const MessageFormat &)
    fmt4 = MessageFormat(fmt1)
    assert fmt1 == fmt4


def test_parse():
    # From icu/source/test/intltest/tmsgfmt.cpp
    fmt = MessageFormat("{0} =sep= {1}", Locale.get_us())
    s = "abc =sep= def"

    # [1]
    # Formattable *icu::MessageFormat::parse(
    #       const UnicodeString &source,
    #       int32_t &count,
    #       UErrorCode &status
    # )
    objs = fmt.parse(UnicodeString(s))
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, Formattable) for x in objs)
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    objs = fmt.parse(s)
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, Formattable) for x in objs)
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    # [2]
    # Formattable *icu::MessageFormat::parse(
    #       const UnicodeString &source,
    #       ParsePosition &pos,
    #       int32_t &count
    # )
    pos = ParsePosition()
    objs = fmt.parse(UnicodeString(s), pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, Formattable) for x in objs)
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    pos = ParsePosition()
    objs = fmt.parse(s, pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert isinstance(objs, list)
    assert len(objs) == 2
    assert all(isinstance(x, Formattable) for x in objs)
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    # void icu::MessageFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &pos
    # )
    result = Formattable()
    pos = ParsePosition()
    fmt.parse_object(UnicodeString(s), result, pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert result.get_type() == Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    result = Formattable()
    pos = ParsePosition()
    fmt.parse_object(s, result, pos)
    assert pos.get_index() > 0
    assert pos.get_error_index() == -1
    assert result.get_type() == Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    fmt.parse_object(UnicodeString(s), result)
    assert result.get_type() == Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"

    result = Formattable()
    fmt.parse_object(s, result)
    assert result.get_type() == Formattable.ARRAY
    objs = result.get_array()
    assert len(objs) == 2
    assert objs[0].get_type() == Formattable.STRING
    assert objs[0].get_string() == "abc"
    assert objs[1].get_type() == Formattable.STRING
    assert objs[1].get_string() == "def"


def test_set_format():
    fmt = MessageFormat("{0,date} {1,time}", Locale.get_us())
    dtfmt0 = SimpleDateFormat("yyyy.MM.dd")
    dtfmt1 = SimpleDateFormat("HH:mm:ss z")
    date = 1637685775000.0  # 2021-11-23T16:42:55Z
    obj = Formattable([
        Formattable(date, Formattable.IS_DATE),
        Formattable(date, Formattable.IS_DATE),
    ])
    append_to = UnicodeString()

    # [1]
    # void icu::MessageFormat::setFormat(
    #       const UnicodeString &formatName,
    #       const Format &format,
    #       UErrorCode &status
    # )
    tz = TimeZone.get_gmt()
    dtfmt0.set_time_zone(tz)
    dtfmt1.set_time_zone(tz)

    fmt.set_format(UnicodeString("0"), dtfmt0)
    fmt.set_format("1", dtfmt1)

    append_to.remove()
    assert fmt.format(obj, append_to) == "2021.11.23 16:42:55 GMT"

    # [2]
    # void icu::MessageFormat::setFormat(
    #       int32_t formatNumber,
    #       const Format &format
    # )
    tz = TimeZone.create_time_zone("PST")
    dtfmt0.set_time_zone(tz)
    dtfmt1.set_time_zone(tz)

    fmt.set_format(0, dtfmt0)
    fmt.set_format(1, dtfmt1)

    append_to.remove()
    assert fmt.format(obj, append_to) in ("2021.11.23 08:42:55 GMT-8",
                                          "2021.11.23 08:42:55 PST")


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 62, reason="ICU4C<62")
def test_set_format_62():
    from icupy.icu.number import NumberFormatter

    fmt = MessageFormat("{0,number,currency}", Locale.get_us())
    obj = Formattable([
        Formattable(444444.55)
    ])
    append_to = UnicodeString()
    assert fmt.format(obj, append_to) == "$444,444.55"

    numfmt = (
        NumberFormatter.for_skeleton("currency/USD unit-width-iso-code")
            .locale(Locale.get_us())
            .to_format()
    )
    fmt.set_format(0, numfmt)
    append_to.remove()
    assert fmt.format(obj, append_to) == "USD\xa0444,444.55"


def test_set_formats():
    fmt = MessageFormat(
        "At {1,time,::jmm} on {1,date,::dMMMM}, "
        "there was {2} on planet {0,number}.",
        Locale.get_us())
    formats = fmt.get_formats()

    # void icu::MessageFormat::setFormats(
    #       const Format **newFormats,
    #       int32_t cnt
    # )
    new_formats = [
        SimpleDateFormat("HH:mm:ss z"),
        SimpleDateFormat("yyyy.MM.dd"),
        None,
        formats[3].clone(),
    ]
    tz = TimeZone.get_gmt()
    new_formats[0].set_time_zone(tz)
    new_formats[1].set_time_zone(tz)
    fmt.set_formats(new_formats, len(new_formats))

    date = 1637685775000.0  # 2021-11-23T16:42:55Z
    obj = Formattable([
        Formattable(7),
        Formattable(date, Formattable.IS_DATE),
        Formattable(UnicodeString("a disturbance in the Force")),
    ])
    append_to = UnicodeString()
    assert fmt.format(obj, append_to) == (
        "At 16:42:55 GMT on 2021.11.23, "
        "there was a disturbance in the Force on planet 7."
    )

    tz = TimeZone.create_time_zone("PST")
    new_formats[0].set_time_zone(tz)
    new_formats[1].set_time_zone(tz)
    fmt.set_formats(new_formats)  # cnt is optional

    append_to.remove()
    assert fmt.format(obj, append_to) in (
        "At 08:42:55 GMT-8 on 2021.11.23, "
        "there was a disturbance in the Force on planet 7.",
        "At 08:42:55 PST on 2021.11.23, "
        "there was a disturbance in the Force on planet 7.",
    )

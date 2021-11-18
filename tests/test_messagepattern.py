from icupy.icu import (
    MessagePattern, UMSGPAT_NO_NUMERIC_VALUE, UMessagePatternApostropheMode,
    UMessagePatternArgType, UMessagePatternPartType, UParseError,
    UnicodeString,
)


def test_api():
    pattern = "I don't '{know}' {gender,select,female{h''er}other{h'im}}."
    mp1 = MessagePattern(pattern, None)

    # UnicodeString icu::MessagePattern::autoQuoteApostropheDeep()
    result = mp1.auto_quote_apostrophe_deep()
    assert isinstance(result, UnicodeString)
    assert (result
            == "I don''t '{know}' {gender,select,female{h''er}other{h''im}}.")

    # int32_t icu::MessagePattern::countParts()
    assert mp1.count_parts() == 16

    # UMessagePatternApostropheMode icu::MessagePattern::getApostropheMode()
    assert (mp1.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL)

    # void icu::MessagePattern::clear()
    mp1.clear()
    assert mp1.count_parts() == 0

    # void icu::MessagePattern::clearPatternAndSetApostropheMode(
    #       UMessagePatternApostropheMode mode
    # )
    mp1 = MessagePattern(pattern, None)
    mp1.clear_pattern_and_set_apostrophe_mode(
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED)
    assert mp1.count_parts() == 0
    assert (mp1.get_apostrophe_mode()
            == UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED)

    # int32_t icu::MessagePattern::getLimitPartIndex(int32_t start)
    mp1 = MessagePattern(pattern, None)
    assert mp1.get_limit_part_index(0) == 15

    # const Part &icu::MessagePattern::getPart(int32_t i)
    part = mp1.get_part(4)
    assert isinstance(part, MessagePattern.Part)

    # double icu::MessagePattern::getNumericValue(const Part &part)
    assert mp1.get_numeric_value(part) == UMSGPAT_NO_NUMERIC_VALUE

    # UMessagePatternPartType icu::MessagePattern::getPartType(int32_t i)
    assert (mp1.get_part_type(4)
            == UMessagePatternPartType.UMSGPAT_PART_TYPE_ARG_START)

    # int32_t icu::MessagePattern::getPatternIndex(int32_t partIndex)
    assert mp1.get_pattern_index(4) == 17

    # const UnicodeString &icu::MessagePattern::getPatternString()
    result = mp1.get_pattern_string()
    assert isinstance(result, UnicodeString)
    assert result == pattern

    # double icu::MessagePattern::getPluralOffset(int32_t pluralStart)
    pattern2 = ("{0,plural,offset:1 "
                "one{another meter}"
                "other{{0,number,00.#} meters}}")
    mp2 = MessagePattern(pattern2, None)
    assert (mp2.get_part_type(3)
            == UMessagePatternPartType.UMSGPAT_PART_TYPE_ARG_INT)
    assert mp2.get_plural_offset(3) == 1

    # UnicodeString icu::MessagePattern::getSubstring(const Part &part)
    part = mp2.get_part(3)
    result = mp2.get_substring(part)
    assert isinstance(result, UnicodeString)
    assert result == "1"

    # int32_t icu::MessagePattern::hashCode()
    assert mp1.hash_code()
    assert mp1.hash_code() != mp2.hash_code()

    # UBool icu::MessagePattern::hasNamedArguments()
    assert mp1.has_named_arguments()
    assert not mp2.has_named_arguments()

    # UBool icu::MessagePattern::hasNumberedArguments()
    assert not mp1.has_numbered_arguments()
    assert mp2.has_numbered_arguments()

    # UBool icu::MessagePattern::operator!=(const MessagePattern &other)
    mp3 = MessagePattern(pattern, None)
    assert mp1 != mp2
    assert not (mp1 != mp3)
    assert mp2 != mp3

    # UBool icu::MessagePattern::operator==(const MessagePattern &other)
    assert not (mp1 == mp2)
    assert mp1 == mp3
    assert not (mp2 == mp3)

    # UBool icu::MessagePattern::partSubstringMatches(
    #       const Part &part,
    #       const UnicodeString &s
    # )
    assert mp2.part_substring_matches(part, UnicodeString("1"))
    assert mp2.part_substring_matches(part, "1")

    # static int32_t icu::MessagePattern::validateArgumentName(
    #       const UnicodeString &name
    # )
    assert MessagePattern.validate_argument_name(UnicodeString("3")) == 3
    assert MessagePattern.validate_argument_name("3") == 3


def test_message_pattern():
    # [1]
    # icu::MessagePattern::MessagePattern(UErrorCode &errorCode)
    mp1 = MessagePattern()
    assert mp1.count_parts() == 0

    # [2]
    # icu::MessagePattern::MessagePattern(
    #       UMessagePatternApostropheMode mode,
    #       UErrorCode &errorCode
    # )
    mp2 = MessagePattern(
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_OPTIONAL)
    assert mp1 == mp2
    assert mp2.count_parts() == 0

    mp2a = MessagePattern(
        UMessagePatternApostropheMode.UMSGPAT_APOS_DOUBLE_REQUIRED)
    assert mp1 != mp2a

    # [3]
    # icu::MessagePattern::MessagePattern(
    #       const UnicodeString &pattern,
    #       UParseError *parseError,
    #       UErrorCode &errorCode
    # )
    pattern = UnicodeString("I see {many}")
    parse_error = UParseError()
    mp3 = MessagePattern(pattern, parse_error)
    assert parse_error.offset == 0
    assert mp3.count_parts() == 5

    mp3a = MessagePattern(pattern, None)
    assert mp3 == mp3a

    parse_error = UParseError()
    mp3b = MessagePattern(str(pattern), parse_error)
    assert parse_error.offset == 0
    assert mp3 == mp3b

    mp3c = MessagePattern(str(pattern), None)
    assert mp3 == mp3c

    # [4]
    # icu::MessagePattern::MessagePattern(const MessagePattern &other)
    mp4 = MessagePattern(mp3)
    assert mp3 == mp4
    assert mp4.count_parts() == 5


def test_parse():
    pattern = UnicodeString(
        "At {1,time,::jmm} on {1,date,::dMMMM}, "
        "there was {2} on planet {0,number}.")

    # MessagePattern &icu::MessagePattern::parse(
    #       const UnicodeString &pattern,
    #       UParseError *parseError,
    #       UErrorCode &errorCode
    # )
    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse(pattern, parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 19

    mp = MessagePattern()
    result = mp.parse(pattern, None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 19

    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse(str(pattern), parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 19

    mp = MessagePattern()
    result = mp.parse(str(pattern), None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 19


def test_parse_choice_style():
    pattern = UnicodeString(
        "0#are no files|1#is one file|1<are {0, number} files")

    # MessagePattern &icu::MessagePattern::parseChoiceStyle(
    #       const UnicodeString &pattern,
    #       UParseError *parseError,
    #       UErrorCode &errorCode
    # )
    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse_choice_style(pattern, parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16

    mp = MessagePattern()
    result = mp.parse_choice_style(pattern, None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16

    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse_choice_style(str(pattern), parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16

    mp = MessagePattern()
    result = mp.parse_choice_style(str(pattern), None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16


def test_parse_plural_style():
    pattern = UnicodeString(
        "one{#st file}two{#nd file}few{#rd file}other{#th file}")

    # MessagePattern &icu::MessagePattern::parsePluralStyle(
    #       const UnicodeString &pattern,
    #       UParseError *parseError,
    #       UErrorCode &errorCode
    # )
    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse_plural_style(pattern, parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16

    mp = MessagePattern()
    result = mp.parse_plural_style(pattern, None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16

    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse_plural_style(str(pattern), parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16

    mp = MessagePattern()
    result = mp.parse_plural_style(str(pattern), None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 16


def test_parse_select_style():
    pattern = UnicodeString("female{h''er}other{h'im}")

    # MessagePattern &icu::MessagePattern::parseSelectStyle(
    #       const UnicodeString &pattern,
    #       UParseError *parseError,
    #       UErrorCode &errorCode
    # )
    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse_select_style(pattern, parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 8

    mp = MessagePattern()
    result = mp.parse_select_style(pattern, None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 8

    mp = MessagePattern()
    parse_error = UParseError()
    result = mp.parse_select_style(str(pattern), parse_error)
    assert parse_error.offset == 0
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 8

    mp = MessagePattern()
    result = mp.parse_select_style(str(pattern), None)
    assert isinstance(result, MessagePattern)
    assert id(result) == id(mp)
    assert mp.count_parts() == 8


def test_part():
    pattern = "I don't '{know}' {gender,select,female{h''er}other{h'im}}."
    mp3 = MessagePattern(pattern, None)

    part = mp3.get_part(4)
    assert isinstance(part, MessagePattern.Part)

    # UMessagePatternArgType icu::MessagePattern::Part::getArgType()
    assert (part.get_arg_type()
            == UMessagePatternArgType.UMSGPAT_ARG_TYPE_SELECT)

    # int32_t icu::MessagePattern::Part::getIndex(void)
    assert part.get_index() == 17

    # int32_t icu::MessagePattern::Part::getLength(void)
    assert part.get_length() == 1

    # int32_t icu::MessagePattern::Part::getLimit()
    assert part.get_limit() == 18

    # UMessagePatternPartType icu::MessagePattern::Part::getType()
    assert (part.get_type()
            == UMessagePatternPartType.UMSGPAT_PART_TYPE_ARG_START)

    # int32_t icu::MessagePattern::Part::getValue()
    assert part.get_value() == 4

    # int32_t icu::MessagePattern::Part::hashCode(void)
    assert part.hash_code()

    # static UBool icu::MessagePattern::Part::hasNumericValue(
    #       UMessagePatternPartType type
    # )
    assert not MessagePattern.Part.has_numeric_value(
        UMessagePatternPartType.UMSGPAT_PART_TYPE_ARG_START)

    assert MessagePattern.Part.has_numeric_value(
        UMessagePatternPartType.UMSGPAT_PART_TYPE_ARG_INT)

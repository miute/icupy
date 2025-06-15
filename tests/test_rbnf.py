import copy

import pytest

from icupy import icu


def test_api() -> None:
    # From icu/source/test/intltest/itrbnf.cpp
    rules = (
        "%main:"
        " 0: no; 1: some; 100: a lot; 1000: tons;\n"
        "%other:"
        " 0: nada; 1: yah, some; 100: plenty; 1000: more'n you'll ever need"
    )
    localizations = (
        "<"
        " <%main, %other>, <en, Main, Other>, <fr, leMain, leOther>,"
        " <de, 'das Main', 'etwas anderes'>"
        ">"
    )
    perror = icu.UParseError()
    fmt = icu.RuleBasedNumberFormat(rules, localizations, perror)

    # UnicodeString icu::RuleBasedNumberFormat::getDefaultRuleSetName()
    result = fmt.get_default_rule_set_name()
    assert isinstance(result, icu.UnicodeString)
    assert result == "%main"

    # void icu::RuleBasedNumberFormat::setDefaultRuleSet(
    #       const UnicodeString &ruleSetName,
    #       UErrorCode &status
    # )
    fmt.set_default_rule_set(icu.UnicodeString("%other"))
    assert fmt.get_default_rule_set_name() == "%other"

    fmt.set_default_rule_set("%main")
    assert fmt.get_default_rule_set_name() == "%main"

    # int32_t icu::RuleBasedNumberFormat::getNumberOfRuleSetDisplayNameLocales(
    #       void
    # )
    assert fmt.get_number_of_rule_set_display_name_locales() == 3

    # int32_t icu::RuleBasedNumberFormat::getNumberOfRuleSetNames()
    assert fmt.get_number_of_rule_set_names() == 2

    # UnicodeString icu::RuleBasedNumberFormat::getRules()
    result = fmt.get_rules()
    assert isinstance(result, icu.UnicodeString)
    assert len(result) > 0

    # Locale icu::RuleBasedNumberFormat::getRuleSetDisplayNameLocale(
    #       int32_t index,
    #       UErrorCode &status
    # )
    result = fmt.get_rule_set_display_name_locale(1)
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("fr")

    # UnicodeString icu::RuleBasedNumberFormat::getRuleSetName(int32_t index)
    result = fmt.get_rule_set_name(1)
    assert isinstance(result, icu.UnicodeString)
    assert result == "%other"

    # UBool icu::RuleBasedNumberFormat::isLenient(void)
    assert not fmt.is_lenient()

    # void icu::RuleBasedNumberFormat::setLenient(UBool enabled)
    fmt.set_lenient(True)
    assert fmt.is_lenient()

    # UBool icu::RuleBasedNumberFormat::operator==(const Format &other)
    fmt2 = fmt.clone()
    fmt3 = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())
    assert fmt == fmt2
    assert not (fmt == fmt3)
    assert not (fmt2 == fmt3)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_api_49() -> None:
    fmt = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_ORDINAL, icu.Locale.get_us())

    # void icu::RuleBasedNumberFormat::setDecimalFormatSymbols(
    #       const DecimalFormatSymbols &symbols
    # )
    symbols = icu.DecimalFormatSymbols(icu.Locale.get_us())
    symbols.set_symbol(
        icu.DecimalFormatSymbols.GROUPING_SEPARATOR_SYMBOL,
        icu.UnicodeString("&"),
        True,
    )
    fmt.set_decimal_format_symbols(symbols)
    append_to = icu.UnicodeString()
    assert fmt.format(1001, append_to) == "1&001st"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_api_53() -> None:
    fmt = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())

    # UDisplayContext icu::NumberFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (
        fmt.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
        == icu.UDisplayContext.UDISPCTX_CAPITALIZATION_NONE
    )

    # void icu::RuleBasedNumberFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(icu.UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_STANDALONE)
    assert (
        fmt.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
        == icu.UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_STANDALONE
    )


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_api_60() -> None:
    fmt = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())

    # ERoundingMode icu::RuleBasedNumberFormat::getRoundingMode(void)
    assert fmt.get_rounding_mode() == icu.RuleBasedNumberFormat.ROUND_UNNECESSARY

    # void icu::RuleBasedNumberFormat::setRoundingMode(
    #       ERoundingMode roundingMode
    # )
    fmt.set_rounding_mode(icu.RuleBasedNumberFormat.ROUND_HALF_EVEN)
    assert fmt.get_rounding_mode() == icu.RuleBasedNumberFormat.ROUND_HALF_EVEN


def test_clone() -> None:
    fmt1 = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())

    # RuleBasedNumberFormat *icu::RuleBasedNumberFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, icu.RuleBasedNumberFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format() -> None:
    fmt = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())
    d = -10456.0037
    s = "-1.045600e+04"
    n32 = icu.INT32_MAX
    n64 = icu.INT64_MAX
    append_to = icu.UnicodeString()
    rule_set_name = icu.UnicodeString("%spellout-numbering")

    # [1], [2]
    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(icu.Formattable(d), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six point zero zero three seven")

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [3], [4]
    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(icu.Formattable(d), append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [5]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(icu.Formattable(d), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six point zero zero three seven")

    # [9]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       double number,
    #       const UnicodeString &ruleSetName,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(d, rule_set_name, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six point zero zero three seven")

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(d, str(rule_set_name), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six point zero zero three seven")

    # [10]
    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(d, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six point zero zero three seven")

    # [11], [14]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       double number,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(d, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six point zero zero three seven")

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [13]
    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(d, append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [15]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int32_t number,
    #       const UnicodeString &ruleSetName,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n32, rule_set_name, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "two billion one hundred forty-seven million"
        " four hundred eighty-three thousand"
        " six hundred forty-seven"
    )

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n32, str(rule_set_name), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "two billion one hundred forty-seven million"
        " four hundred eighty-three thousand"
        " six hundred forty-seven"
    )

    # [16]
    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n32, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "two billion one hundred forty-seven million"
        " four hundred eighty-three thousand"
        " six hundred forty-seven"
    )

    # [17], [20]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int32_t number,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n32, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == (
        "two billion one hundred forty-seven million"
        " four hundred eighty-three thousand"
        " six hundred forty-seven"
    )

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [19]
    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(n32, append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # [21]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int64_t number,
    #       const UnicodeString &ruleSetName,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n64, rule_set_name, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n64, str(rule_set_name), append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    # [22]
    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n64, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    # [23], [26]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int64_t number,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = icu.FieldPosition(icu.FieldPosition.DONT_CARE)
    result = fmt.format(n64, append_to, pos)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [25]
    # UnicodeString &format(
    #       int64_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(n64, append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [27]
    # UnicodeString &icu::NumberFormat::format(
    #       StringPiece number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = icu.FieldPositionIterator()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fmt.format(s, append_to, pos_iter)
    assert exc_info.value.args[0] == icu.UErrorCode.U_UNSUPPORTED_ERROR


def test_get_rule_set_display_name() -> None:
    # From icu/source/test/intltest/itrbnf.cpp
    rules = (
        "%main:"
        " 0: no; 1: some; 100: a lot; 1000: tons;\n"
        "%other:"
        " 0: nada; 1: yah, some; 100: plenty; 1000: more'n you'll ever need"
    )
    localizations = (
        "<"
        " <%main, %other>, <en, Main, Other>, <fr, leMain, leOther>,"
        " <de, 'das Main', 'etwas anderes'>"
        ">"
    )
    perror = icu.UParseError()
    fmt = icu.RuleBasedNumberFormat(rules, localizations, perror)

    # [1]
    # UnicodeString icu::RuleBasedNumberFormat::getRuleSetDisplayName(
    #       const UnicodeString &ruleSetName,
    #       const Locale &locale = Locale::getDefault()
    # )
    result = fmt.get_rule_set_display_name(icu.UnicodeString("%main"))
    assert isinstance(result, icu.UnicodeString)
    assert result in ["%main", "Main", "leMain", "das Main"]

    result = fmt.get_rule_set_display_name("%main")
    assert isinstance(result, icu.UnicodeString)
    assert result in ["%main", "Main", "leMain", "das Main"]

    result = fmt.get_rule_set_display_name(icu.UnicodeString("%main"), icu.Locale("de_DE_FOO"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name("%main", icu.Locale("de_DE_FOO"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name(icu.UnicodeString("%main"), "de_DE_FOO")
    assert isinstance(result, icu.UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name("%main", "de_DE_FOO")
    assert isinstance(result, icu.UnicodeString)
    assert result == "das Main"

    # [2]
    # UnicodeString icu::RuleBasedNumberFormat::getRuleSetDisplayName(
    #       int32_t index,
    #       const Locale &locale = Locale::getDefault()
    # )
    result = fmt.get_rule_set_display_name(0)
    assert isinstance(result, icu.UnicodeString)
    assert result in ["%main", "Main", "leMain", "das Main"]

    result = fmt.get_rule_set_display_name(0, icu.Locale("de_DE_FOO"))
    assert isinstance(result, icu.UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name(0, "de_DE_FOO")
    assert isinstance(result, icu.UnicodeString)
    assert result == "das Main"


def test_parse() -> None:
    fmt = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())
    d = -10456.0037
    s = "minus ten thousand four hundred fifty-six point zero zero three seven"

    # [1], [2]
    # void icu::RuleBasedNumberFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       ParsePosition &parsePosition
    # )
    result = icu.Formattable()
    parse_position = icu.ParsePosition()
    fmt.parse(icu.UnicodeString(s), result, parse_position)
    assert parse_position.get_index() > 0
    assert parse_position.get_error_index() == -1
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d

    result = icu.Formattable()
    parse_position = icu.ParsePosition()
    fmt.parse(s, result, parse_position)
    assert parse_position.get_index() > 0
    assert parse_position.get_error_index() == -1
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d

    # [3]
    # void icu::NumberFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    fmt.parse(icu.UnicodeString(s), result)
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d

    result = icu.Formattable()
    fmt.parse(s, result)
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d


def test_parse_object() -> None:
    fmt = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())
    d = -10456.0037
    s = "minus ten thousand four hundred fifty-six point zero zero three seven"

    # void icu::NumberFormat::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       ParsePosition &parse_pos
    # )
    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt.parse_object(icu.UnicodeString(s), result, parse_pos)
    assert parse_pos.get_index() == len(s)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d

    result = icu.Formattable()
    parse_pos = icu.ParsePosition()
    fmt.parse_object(s, result, parse_pos)
    assert parse_pos.get_index() == len(s)
    assert parse_pos.get_error_index() == -1
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d

    # void icu::Format::parseObject(
    #       const UnicodeString &source,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = icu.Formattable()
    fmt.parse_object(icu.UnicodeString(s), result)
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d

    result = icu.Formattable()
    fmt.parse_object(s, result)
    assert result.get_type() == icu.Formattable.DOUBLE
    assert result.get_double() == d


def test_rule_based_number_format() -> None:
    assert issubclass(icu.RuleBasedNumberFormat, icu.NumberFormat)

    rules = icu.UnicodeString("=#,##0=;1,000,000: <##0.###< million;")

    # [1]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = icu.UParseError()
    fmt1 = icu.RuleBasedNumberFormat(rules, perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt1a = icu.RuleBasedNumberFormat(str(rules), perror)
    assert perror.offset == 0
    assert fmt1 == fmt1a

    # [2]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       const UnicodeString &localizations,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = icu.UParseError()
    fmt2 = icu.RuleBasedNumberFormat(rules, icu.UnicodeString(), perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt2a = icu.RuleBasedNumberFormat(str(rules), icu.UnicodeString(), perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt2b = icu.RuleBasedNumberFormat(rules, "", perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt2c = icu.RuleBasedNumberFormat(str(rules), "", perror)
    assert perror.offset == 0
    assert fmt2 == fmt2a == fmt2b == fmt2c

    # [3]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       const Locale &locale,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = icu.UParseError()
    fmt3 = icu.RuleBasedNumberFormat(rules, icu.Locale.get_default(), perror)
    assert perror.offset == 0
    assert fmt1 == fmt3

    perror = icu.UParseError()
    fmt3a = icu.RuleBasedNumberFormat(str(rules), icu.Locale.get_default(), perror)
    assert perror.offset == 0
    assert fmt3 == fmt3a

    # [4]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       const UnicodeString &localizations,
    #       const Locale &locale,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = icu.UParseError()
    fmt4 = icu.RuleBasedNumberFormat(
        rules, icu.UnicodeString(), icu.Locale.get_default(), perror
    )
    assert perror.offset == 0
    assert fmt2 == fmt4

    perror = icu.UParseError()
    fmt4a = icu.RuleBasedNumberFormat(
        str(rules), icu.UnicodeString(), icu.Locale.get_default(), perror
    )
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt4b = icu.RuleBasedNumberFormat(rules, "", icu.Locale.get_default(), perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt4c = icu.RuleBasedNumberFormat(
        rules, icu.UnicodeString(), str(icu.Locale.get_default()), perror
    )
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt4d = icu.RuleBasedNumberFormat(str(rules), "", icu.Locale.get_default(), perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt4e = icu.RuleBasedNumberFormat(rules, "", str(icu.Locale.get_default()), perror)
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt4f = icu.RuleBasedNumberFormat(
        str(rules), icu.UnicodeString(), str(icu.Locale.get_default()), perror
    )
    assert perror.offset == 0

    perror = icu.UParseError()
    fmt4g = icu.RuleBasedNumberFormat(str(rules), "", str(icu.Locale.get_default()), perror)
    assert perror.offset == 0
    assert fmt4 == fmt4a == fmt4b == fmt4c == fmt4d == fmt4e == fmt4f == fmt4g

    # [5]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       URBNFRuleSetTag tag,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt5 = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, icu.Locale.get_us())

    fmt5a = icu.RuleBasedNumberFormat(icu.URBNFRuleSetTag.URBNF_SPELLOUT, "en-US")
    assert fmt5 == fmt5a

    # [6]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const RuleBasedNumberFormat &rhs
    # )
    fmt6 = icu.RuleBasedNumberFormat(fmt5)
    assert fmt5 == fmt6

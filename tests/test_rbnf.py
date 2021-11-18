import copy

import pytest
from icupy.icu import (
    FieldPosition, FieldPositionIterator, Formattable, ICUError,
    INT32_MAX, INT64_MAX, Locale, NumberFormat, ParsePosition,
    RuleBasedNumberFormat, UErrorCode, UParseError, URBNFRuleSetTag,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
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
    perror = UParseError()
    fmt = RuleBasedNumberFormat(rules, localizations, perror)

    # UnicodeString icu::RuleBasedNumberFormat::getDefaultRuleSetName()
    result = fmt.get_default_rule_set_name()
    assert isinstance(result, UnicodeString)
    assert result == "%main"

    # void icu::RuleBasedNumberFormat::setDefaultRuleSet(
    #       const UnicodeString &ruleSetName,
    #       UErrorCode &status
    # )
    fmt.set_default_rule_set(UnicodeString("%other"))
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
    assert isinstance(result, UnicodeString)
    assert len(result) > 0

    # Locale icu::RuleBasedNumberFormat::getRuleSetDisplayNameLocale(
    #       int32_t index,
    #       UErrorCode &status
    # )
    result = fmt.get_rule_set_display_name_locale(1)
    assert isinstance(result, Locale)
    assert result == Locale("fr")

    # UnicodeString icu::RuleBasedNumberFormat::getRuleSetName(int32_t index)
    result = fmt.get_rule_set_name(1)
    assert isinstance(result, UnicodeString)
    assert result == "%other"

    # UBool icu::RuleBasedNumberFormat::isLenient(void)
    assert not fmt.is_lenient()

    # void icu::RuleBasedNumberFormat::setLenient(UBool enabled)
    fmt.set_lenient(True)
    assert fmt.is_lenient()

    # UBool icu::RuleBasedNumberFormat::operator==(const Format &other)
    fmt2 = fmt.clone()
    fmt3 = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())
    assert fmt == fmt2
    assert not (fmt == fmt3)
    assert not (fmt2 == fmt3)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_api_49():
    from icupy.icu import DecimalFormatSymbols

    fmt = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_ORDINAL,
        Locale.get_us())

    # void icu::RuleBasedNumberFormat::setDecimalFormatSymbols(
    #       const DecimalFormatSymbols &symbols
    # )
    symbols = DecimalFormatSymbols(Locale.get_us())
    symbols.set_symbol(
        DecimalFormatSymbols.GROUPING_SEPARATOR_SYMBOL,
        UnicodeString("&"),
        True)
    fmt.set_decimal_format_symbols(symbols)
    append_to = UnicodeString()
    assert fmt.format(1001, append_to) == "1&001st"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 53, reason="ICU4C<53")
def test_api_53():
    from icupy.icu import UDisplayContext, UDisplayContextType

    fmt = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())

    # UDisplayContext icu::NumberFormat::getContext(
    #       UDisplayContextType type,
    #       UErrorCode &status
    # )
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_NONE)

    # void icu::RuleBasedNumberFormat::setContext(
    #       UDisplayContext value,
    #       UErrorCode &status
    # )
    fmt.set_context(UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_STANDALONE)
    assert (fmt.get_context(UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION)
            == UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_STANDALONE)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_api_60():
    fmt = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())

    # ERoundingMode icu::RuleBasedNumberFormat::getRoundingMode(void)
    assert fmt.get_rounding_mode() == RuleBasedNumberFormat.ROUND_UNNECESSARY

    # void icu::RuleBasedNumberFormat::setRoundingMode(
    #       ERoundingMode roundingMode
    # )
    fmt.set_rounding_mode(RuleBasedNumberFormat.ROUND_HALF_EVEN)
    assert fmt.get_rounding_mode() == RuleBasedNumberFormat.ROUND_HALF_EVEN


def test_clone():
    fmt1 = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())

    # RuleBasedNumberFormat *icu::RuleBasedNumberFormat::clone()
    fmt2 = fmt1.clone()
    assert isinstance(fmt2, RuleBasedNumberFormat)
    assert fmt1 == fmt2

    fmt3 = copy.copy(fmt1)
    assert fmt1 == fmt3

    fmt4 = copy.deepcopy(fmt1)
    assert fmt1 == fmt4


def test_format():
    fmt = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())
    d = -10456.0037
    s = "-1.045600e+04"
    n32 = INT32_MAX
    n64 = INT64_MAX
    append_to = UnicodeString()
    rule_set_name = UnicodeString("%spellout-numbering")

    # [1], [2]
    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(Formattable(d), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six"
                      " point zero zero three seven")

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [3], [4]
    # UnicodeString &icu::NumberFormat::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(Formattable(d), append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # [5]
    # UnicodeString &icu::Format::format(
    #       const Formattable &obj,
    #       UnicodeString &appendTo,
    #       UErrorCode &status
    # )
    append_to.remove()
    result = fmt.format(Formattable(d), append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six"
                      " point zero zero three seven")

    # [9]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       double number,
    #       const UnicodeString &ruleSetName,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(d, rule_set_name, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six"
                      " point zero zero three seven")

    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(d, str(rule_set_name), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six"
                      " point zero zero three seven")

    # [10]
    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(d, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six"
                      " point zero zero three seven")

    # [11], [14]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       double number,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(d, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("minus ten thousand four hundred fifty-six"
                      " point zero zero three seven")

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [13]
    # UnicodeString &icu::NumberFormat::format(
    #       double number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(d, append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # [15]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int32_t number,
    #       const UnicodeString &ruleSetName,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n32, rule_set_name, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("two billion one hundred forty-seven million"
                      " four hundred eighty-three thousand"
                      " six hundred forty-seven")

    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n32, str(rule_set_name), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("two billion one hundred forty-seven million"
                      " four hundred eighty-three thousand"
                      " six hundred forty-seven")

    # [16]
    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n32, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("two billion one hundred forty-seven million"
                      " four hundred eighty-three thousand"
                      " six hundred forty-seven")

    # [17], [20]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int32_t number,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n32, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == ("two billion one hundred forty-seven million"
                      " four hundred eighty-three thousand"
                      " six hundred forty-seven")

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [19]
    # UnicodeString &icu::NumberFormat::format(
    #       int32_t number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(n32, append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # [21]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int64_t number,
    #       const UnicodeString &ruleSetName,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n64, rule_set_name, append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n64, str(rule_set_name), append_to, pos)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    # [22]
    # UnicodeString &icu::NumberFormat::format(
    #       int64_t number,
    #       UnicodeString &appendTo
    # )
    append_to.remove()
    result = fmt.format(n64, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "9,223,372,036,854,775,807"

    # [23], [26]
    # UnicodeString &icu::RuleBasedNumberFormat::format(
    #       int64_t number,
    #       UnicodeString &toAppendTo,
    #       FieldPosition &pos
    # )
    append_to.remove()
    pos = FieldPosition(FieldPosition.DONT_CARE)
    result = fmt.format(n64, append_to, pos)
    assert isinstance(result, UnicodeString)
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
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(n64, append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR

    # *U_UNSUPPORTED_ERROR in ICU 69*
    # [27]
    # UnicodeString &icu::NumberFormat::format(
    #       StringPiece number,
    #       UnicodeString &appendTo,
    #       FieldPositionIterator *posIter,
    #       UErrorCode &status
    # )
    append_to.remove()
    pos_iter = FieldPositionIterator()
    with pytest.raises(ICUError) as exc_info:
        _ = fmt.format(s, append_to, pos_iter)
    assert exc_info.value.args[0] == UErrorCode.U_UNSUPPORTED_ERROR


def test_get_rule_set_display_name():
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
    perror = UParseError()
    fmt = RuleBasedNumberFormat(rules, localizations, perror)

    # [1]
    # UnicodeString icu::RuleBasedNumberFormat::getRuleSetDisplayName(
    #       const UnicodeString &ruleSetName,
    #       const Locale &locale = Locale::getDefault()
    # )
    result = fmt.get_rule_set_display_name(UnicodeString("%main"))
    assert isinstance(result, UnicodeString)
    assert result in ["%main", "Main", "leMain", "das Main"]

    result = fmt.get_rule_set_display_name("%main")
    assert isinstance(result, UnicodeString)
    assert result in ["%main", "Main", "leMain", "das Main"]

    result = fmt.get_rule_set_display_name(
        UnicodeString("%main"),
        Locale("de_DE_FOO"))
    assert isinstance(result, UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name(
        "%main",
        Locale("de_DE_FOO"))
    assert isinstance(result, UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name(
        UnicodeString("%main"),
        "de_DE_FOO")
    assert isinstance(result, UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name(
        "%main",
        "de_DE_FOO")
    assert isinstance(result, UnicodeString)
    assert result == "das Main"

    # [2]
    # UnicodeString icu::RuleBasedNumberFormat::getRuleSetDisplayName(
    #       int32_t index,
    #       const Locale &locale = Locale::getDefault()
    # )
    result = fmt.get_rule_set_display_name(0)
    assert isinstance(result, UnicodeString)
    assert result in ["%main", "Main", "leMain", "das Main"]

    result = fmt.get_rule_set_display_name(0, Locale("de_DE_FOO"))
    assert isinstance(result, UnicodeString)
    assert result == "das Main"

    result = fmt.get_rule_set_display_name(0, "de_DE_FOO")
    assert isinstance(result, UnicodeString)
    assert result == "das Main"


def test_parse():
    fmt = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())
    d = -10456.0037
    s = ("minus ten thousand four hundred fifty-six"
         " point zero zero three seven")

    # [1], [2]
    # void icu::RuleBasedNumberFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       ParsePosition &parsePosition
    # )
    result = Formattable()
    parse_position = ParsePosition()
    fmt.parse(UnicodeString(s), result, parse_position)
    assert parse_position.get_index() > 0
    assert parse_position.get_error_index() == -1
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

    result = Formattable()
    parse_position = ParsePosition()
    fmt.parse(s, result, parse_position)
    assert parse_position.get_index() > 0
    assert parse_position.get_error_index() == -1
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

    # [3]
    # void icu::NumberFormat::parse(
    #       const UnicodeString &text,
    #       Formattable &result,
    #       UErrorCode &status
    # )
    result = Formattable()
    fmt.parse(UnicodeString(s), result)
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d

    result = Formattable()
    fmt.parse(s, result)
    assert result.get_type() == Formattable.DOUBLE
    assert result.get_double() == d


def test_rule_based_number_format():
    assert issubclass(RuleBasedNumberFormat, NumberFormat)

    rules = UnicodeString("=#,##0=;1,000,000: <##0.###< million;")

    # [1]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = UParseError()
    fmt1 = RuleBasedNumberFormat(rules, perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt1a = RuleBasedNumberFormat(str(rules), perror)
    assert perror.offset == 0
    assert fmt1 == fmt1a

    # [2]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       const UnicodeString &localizations,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = UParseError()
    fmt2 = RuleBasedNumberFormat(rules, UnicodeString(), perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt2a = RuleBasedNumberFormat(str(rules), UnicodeString(), perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt2b = RuleBasedNumberFormat(rules, "", perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt2c = RuleBasedNumberFormat(str(rules), "", perror)
    assert perror.offset == 0
    assert fmt2 == fmt2a == fmt2b == fmt2c

    # [3]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const UnicodeString &rules,
    #       const Locale &locale,
    #       UParseError &perror,
    #       UErrorCode &status
    # )
    perror = UParseError()
    fmt3 = RuleBasedNumberFormat(
        rules,
        Locale.get_default(),
        perror)
    assert perror.offset == 0
    assert fmt1 == fmt3

    perror = UParseError()
    fmt3a = RuleBasedNumberFormat(
        str(rules),
        Locale.get_default(),
        perror)
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
    perror = UParseError()
    fmt4 = RuleBasedNumberFormat(
        rules,
        UnicodeString(),
        Locale.get_default(),
        perror)
    assert perror.offset == 0
    assert fmt2 == fmt4

    perror = UParseError()
    fmt4a = RuleBasedNumberFormat(
        str(rules),
        UnicodeString(),
        Locale.get_default(),
        perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt4b = RuleBasedNumberFormat(
        rules,
        "",
        Locale.get_default(),
        perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt4c = RuleBasedNumberFormat(
        rules,
        UnicodeString(),
        str(Locale.get_default()),
        perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt4d = RuleBasedNumberFormat(
        str(rules),
        "",
        Locale.get_default(),
        perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt4e = RuleBasedNumberFormat(
        rules,
        "",
        str(Locale.get_default()),
        perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt4f = RuleBasedNumberFormat(
        str(rules),
        UnicodeString(),
        str(Locale.get_default()),
        perror)
    assert perror.offset == 0

    perror = UParseError()
    fmt4g = RuleBasedNumberFormat(
        str(rules),
        "",
        str(Locale.get_default()),
        perror)
    assert perror.offset == 0
    assert fmt4 == fmt4a == fmt4b == fmt4c == fmt4d == fmt4e == fmt4f == fmt4g

    # [5]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       URBNFRuleSetTag tag,
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    fmt5 = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        Locale.get_us())

    fmt5a = RuleBasedNumberFormat(
        URBNFRuleSetTag.URBNF_SPELLOUT,
        "en-US")
    assert fmt5 == fmt5a

    # [6]
    # icu::RuleBasedNumberFormat::RuleBasedNumberFormat(
    #       const RuleBasedNumberFormat &rhs
    # )
    fmt6 = RuleBasedNumberFormat(fmt5)
    assert fmt5 == fmt6

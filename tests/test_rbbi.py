import copy

import pytest

from icupy import icu


def test_adopt_text() -> None:
    bi = icu.BreakIterator.create_line_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    it = icu.StringCharacterIterator(src)

    # void icu::BreakIterator::adoptText(CharacterIterator *it)
    bi.adopt_text(it)
    assert bi.first() == 0
    assert bi.next() == 4
    assert bi.next() == 8
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE

    bi.adopt_text(None)
    assert bi.first() == 0
    assert bi.next() == icu.BreakIterator.DONE


def test_clone() -> None:
    bi1 = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    bi1.set_text(src)
    bi1.next()

    # BreakIterator *icu::BreakIterator::clone()
    bi2 = bi1.clone()
    assert isinstance(bi2, icu.BreakIterator)
    assert id(bi1) != id(bi2)
    assert bi1 == bi2
    assert bi1.current() == bi2.current()

    bi3 = copy.copy(bi1)
    assert bi1 == bi3
    assert bi1.current() == bi3.current()

    bi4 = copy.deepcopy(bi1)
    assert bi1 == bi4
    assert bi1.current() == bi4.current()


def test_create_character_instance() -> None:
    # static BreakIterator *icu::BreakIterator::createCharacterInstance(
    #       const Locale &where,
    #       UErrorCode &status
    # )
    bi = icu.BreakIterator.create_character_instance(icu.Locale.get_us())
    assert isinstance(bi, icu.RuleBasedBreakIterator)
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 1
    assert bi.next() == 2
    assert bi.next() == 3
    assert bi.next() == 4
    assert bi.next() == 5
    assert bi.next() == 6
    assert bi.next() == 7
    assert bi.next() == 8
    assert bi.next() == 9
    assert bi.next() == 10
    assert bi.next() == 11
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE

    assert list(bi) == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
    assert reversed(bi) == [12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

    bi = icu.BreakIterator.create_character_instance("en_US")
    bi.set_text(src)
    assert list(bi) == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
    assert reversed(bi) == [12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]

    loc = bi.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "en_US"


def test_create_line_instance() -> None:
    # static BreakIterator *icu::BreakIterator::createLineInstance(
    #       const Locale &where,
    #       UErrorCode &status
    # )
    bi = icu.BreakIterator.create_line_instance(icu.Locale.get_us())
    assert isinstance(bi, icu.RuleBasedBreakIterator)
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 4
    assert bi.next() == 8
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE

    assert list(bi) == [4, 8, 12]
    assert reversed(bi) == [12, 8, 4]

    bi = icu.BreakIterator.create_line_instance("en_US")
    bi.set_text(src)
    assert list(bi) == [4, 8, 12]
    assert reversed(bi) == [12, 8, 4]

    loc = bi.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "en_US"


def test_create_sentence_instance() -> None:
    # static BreakIterator *icu::BreakIterator::createSentenceInstance(
    #       const Locale &where,
    #       UErrorCode &status
    # )
    bi = icu.BreakIterator.create_sentence_instance(icu.Locale.get_us())
    assert isinstance(bi, icu.RuleBasedBreakIterator)
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE

    assert list(bi) == [12]
    assert reversed(bi) == [12]

    bi = icu.BreakIterator.create_sentence_instance("en_US")
    bi.set_text(src)
    assert list(bi) == [12]
    assert reversed(bi) == [12]

    loc = bi.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "en_US"


def test_create_title_instance() -> None:
    # **Deprecated in ICU 64**
    # static BreakIterator *icu::BreakIterator::createTitleInstance(
    #       const Locale &where,
    #       UErrorCode &status
    # )
    bi = icu.BreakIterator.create_title_instance(icu.Locale.get_us())
    assert isinstance(bi, icu.RuleBasedBreakIterator)
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 4
    assert bi.next() == 8
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE

    assert list(bi) == [4, 8, 12]
    assert reversed(bi) == [12, 8, 4]

    bi = icu.BreakIterator.create_title_instance("en_US")
    bi.set_text(src)
    assert list(bi) == [4, 8, 12]
    assert reversed(bi) == [12, 8, 4]

    loc = bi.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "en_US"


def test_create_word_instance() -> None:
    # static BreakIterator *icu::BreakIterator::createWordInstance(
    #       const Locale &where,
    #       UErrorCode &status
    # )
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    assert isinstance(bi, icu.RuleBasedBreakIterator)
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 3
    assert bi.next() == 4
    assert bi.next() == 7
    assert bi.next() == 8
    assert bi.next() == 11
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE

    assert list(bi) == [3, 4, 7, 8, 11, 12]
    assert reversed(bi) == [12, 11, 8, 7, 4, 3]

    bi = icu.BreakIterator.create_word_instance("en_US")
    bi.set_text(src)
    assert list(bi) == [3, 4, 7, 8, 11, 12]
    assert reversed(bi) == [12, 11, 8, 7, 4, 3]

    loc = bi.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "en_US"


def test_following() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    # int32_t icu::BreakIterator::following(int32_t offset)
    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.following(0) == 3
    assert bi.following(3) == 4
    assert bi.following(4) == 7
    assert bi.following(7) == 8
    assert bi.following(8) == 11
    assert bi.following(11) == 12
    assert bi.following(12) == icu.BreakIterator.DONE


def test_get_available_locales() -> None:
    # [1]
    # static const Locale *icu::BreakIterator::getAvailableLocales(
    #       int32_t &count
    # )
    it = icu.BreakIterator.get_available_locales()
    assert isinstance(it, list)
    assert len(it) > 0
    assert all(isinstance(x, icu.Locale) for x in it)
    assert icu.Locale("ja") in it


def test_get_display_name() -> None:
    default_locale = None
    try:
        default_locale = icu.Locale.get_default()
        us_locale = icu.Locale.get_us()
        icu.Locale.set_default(us_locale)
        name = icu.UnicodeString()

        # [1]
        # static UnicodeString &icu::BreakIterator::getDisplayName(
        #       const Locale &objectLocale,
        #       const Locale &displayLocale,
        #       UnicodeString &name
        # )
        result = icu.BreakIterator.get_display_name(icu.Locale.get_france(), us_locale, name)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name)
        assert name == "French (France)"

        name.remove()
        result = icu.BreakIterator.get_display_name("fr_FR", icu.Locale("en_US"), name)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name)
        assert name == "French (France)"

        name.remove()
        result = icu.BreakIterator.get_display_name(icu.Locale("fr_FR"), "en_US", name)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name)
        assert name == "French (France)"

        name.remove()
        result = icu.BreakIterator.get_display_name("fr_FR", "en_US", name)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name)
        assert name == "French (France)"

        # [2]
        # static UnicodeString &icu::BreakIterator::getDisplayName(
        #       const Locale &objectLocale,
        #       UnicodeString &name
        # )
        name.remove()
        result = icu.BreakIterator.get_display_name(us_locale, name)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name)
        assert name == "English (United States)"

        name.remove()
        result = icu.BreakIterator.get_display_name("en_US", name)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(name)
        assert name == "English (United States)"
    finally:
        if default_locale:
            icu.Locale.set_default(default_locale)


def test_get_locale() -> None:
    bi1 = icu.BreakIterator.create_word_instance(icu.Locale.get_english())
    bi2 = icu.BreakIterator.create_word_instance(icu.Locale.get_french())

    # Locale icu::BreakIterator::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    loc = bi1.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert loc.get_name() == "en"

    loc = bi1.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert len(loc.get_name()) == 0

    loc = bi2.get_locale(icu.ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert loc.get_name() == "fr"

    loc = bi2.get_locale(icu.ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert isinstance(loc, icu.Locale)
    assert len(loc.get_name()) == 0


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_get_rule_status() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz!")
    bi.set_text(src)
    assert bi.first() == 0

    # int32_t icu::BreakIterator::getRuleStatus()
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_NONE
    assert bi.next() == 3
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_LETTER
    assert bi.next() == 4
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_NONE
    assert bi.next() == 7
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_LETTER
    assert bi.next() == 8
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_NONE
    assert bi.next() == 11
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_LETTER
    assert bi.next() == 12
    assert bi.get_rule_status() == icu.UWordBreak.UBRK_WORD_NONE
    assert bi.next() == icu.BreakIterator.DONE


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_get_rule_status_vec() -> None:
    # From icu/source/test/intltest/rbbiapts.cpp:
    # void RBBIAPITest::TestRuleStatusVec()
    rules = icu.UnicodeString(
        "[A-N]{100}; \n"
        "[a-w]{200}; \n"
        "[\\p{L}]{300}; \n"
        "[\\p{N}]{400}; \n"
        "[0-5]{500}; \n"
        "!.*;\n",
        -1,
        icu.UnicodeString.INVARIANT,
    )
    parse_error = icu.UParseError()
    bi = icu.RuleBasedBreakIterator(rules, parse_error)
    src = icu.UnicodeString("Aapz5?")
    bi.set_text(src)

    # int32_t icu::BreakIterator::getRuleStatusVec(
    #       int32_t *fillInVec,
    #       int32_t capacity,
    #       UErrorCode &status
    # )
    assert bi.next() == 1
    values = bi.get_rule_status_vec()
    assert isinstance(values, list)
    assert values == [100, 300]

    assert bi.next() == 2
    values = bi.get_rule_status_vec()
    assert isinstance(values, list)
    assert values == [200, 300]

    assert bi.next() == 3
    values = bi.get_rule_status_vec()
    assert isinstance(values, list)
    assert values == [200, 300]

    assert bi.next() == 4
    values = bi.get_rule_status_vec()
    assert isinstance(values, list)
    assert values == [300]

    assert bi.next() == 5
    values = bi.get_rule_status_vec()
    assert isinstance(values, list)
    assert values == [400, 500]

    assert bi.next() == 6
    values = bi.get_rule_status_vec()
    assert isinstance(values, list)
    assert values == [0]


def test_get_rules() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())

    # const UnicodeString &icu::RuleBasedBreakIterator::getRules(void)
    rules = bi.get_rules()
    assert isinstance(rules, icu.UnicodeString)
    assert rules.length() > 0


def test_get_text() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())

    # CharacterIterator &icu::BreakIterator::getText(void)
    # it = bi.get_text()  # Segmentation fault with Clang 12
    # assert it is None

    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)
    it = bi.get_text()
    assert isinstance(it, icu.CharacterIterator)

    dest = icu.UnicodeString()
    it.get_text(dest)
    assert dest == src


def test_get_utext() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())

    # UText *icu::BreakIterator::getUText(
    #       UText *fillIn,
    #       UErrorCode &status
    # )
    ut1 = bi.get_utext(None)
    dest = icu.utext_extract(ut1, 0, icu.utext_native_length(ut1))
    assert len(dest) == 0
    icu.utext_close(ut1)

    src1 = icu.UnicodeString("ABC")
    bi.set_text(src1)
    ut1 = bi.get_utext(None)
    assert icu.utext_extract(ut1, 0, icu.utext_native_length(ut1)) == "ABC"

    src2 = icu.UnicodeString("abc")
    bi.set_text(src2)
    ut2 = bi.get_utext(ut1)
    assert ut2 == ut1
    assert icu.utext_extract(ut1, 0, icu.utext_native_length(ut1)) == "abc"
    icu.utext_close(ut1)


def test_hash_code() -> None:
    bi1 = icu.BreakIterator.create_character_instance(icu.Locale.get_us())
    bi2 = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    bi3 = icu.BreakIterator.create_word_instance(icu.Locale.get_us())

    # int32_t icu::RuleBasedBreakIterator::hashCode(void)
    assert bi1.hash_code() != bi2.hash_code()
    assert bi2.hash_code() == bi3.hash_code()

    assert hash(bi1) == bi1.hash_code()
    assert hash(bi2) == bi2.hash_code()
    assert hash(bi3) == bi3.hash_code()

    src = icu.UnicodeString("foo bar baz.")
    bi1.set_text(src)
    assert bi1.hash_code() != bi2.hash_code()
    assert bi2.hash_code() == bi3.hash_code()

    assert hash(bi1) == bi1.hash_code()
    assert hash(bi2) == bi2.hash_code()
    assert hash(bi3) == bi3.hash_code()

    bi1.next()
    assert bi1.hash_code() != bi2.hash_code()
    assert bi2.hash_code() == bi3.hash_code()

    assert hash(bi1) == bi1.hash_code()
    assert hash(bi2) == bi2.hash_code()
    assert hash(bi3) == bi3.hash_code()


def test_is_boundary() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    # UBool icu::BreakIterator::isBoundary(int32_t offset)
    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.is_boundary(0) is True
    assert bi.is_boundary(1) is False
    assert bi.is_boundary(2) is False
    assert bi.is_boundary(3) is True
    assert bi.is_boundary(4) is True
    assert bi.is_boundary(5) is False
    assert bi.is_boundary(6) is False
    assert bi.is_boundary(7) is True
    assert bi.is_boundary(8) is True
    assert bi.is_boundary(9) is False
    assert bi.is_boundary(10) is False
    assert bi.is_boundary(11) is True
    assert bi.is_boundary(12) is True


def test_next() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    # [1]
    # int32_t icu::BreakIterator::next(int32_t n)
    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.first() == 0
    assert bi.next(3) == 7
    assert bi.next(3) == 12
    assert bi.next(3) == icu.BreakIterator.DONE
    assert bi.current() == 12
    assert bi.next(-3) == 7
    assert bi.next(-3) == 0
    assert bi.next(-3) == icu.BreakIterator.DONE
    assert bi.current() == 0

    # [2]
    # int32_t icu::BreakIterator::next(void)
    assert bi.first() == 0
    assert bi.next() == 3
    assert bi.next() == 4
    assert bi.next() == 7
    assert bi.next() == 8
    assert bi.next() == 11
    assert bi.next() == 12
    assert bi.next() == icu.BreakIterator.DONE


def test_operator() -> None:
    bi1 = icu.BreakIterator.create_character_instance(icu.Locale.get_us())
    bi2 = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    bi3 = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")

    # UBool icu::BreakIterator::operator!=(const BreakIterator &rhs)
    assert bi1 != bi2
    assert bi1 != bi3

    # UBool icu::BreakIterator::operator==(const BreakIterator &)
    assert bi2 == bi3

    bi2.next()
    assert bi2 != bi3
    assert not (bi2 == bi3)

    bi3.next()
    assert not (bi2 != bi3)
    assert bi2 == bi3

    bi2.set_text(src)
    assert bi2 != bi3
    assert not (bi2 == bi3)

    bi3.set_text(src)
    assert not (bi2 != bi3)
    assert bi2 == bi3


def test_preceding() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    # int32_t icu::BreakIterator::preceding(int32_t offset)
    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.preceding(12) == 11
    assert bi.preceding(11) == 8
    assert bi.preceding(8) == 7
    assert bi.preceding(7) == 4
    assert bi.preceding(4) == 3
    assert bi.preceding(3) == 0
    assert bi.preceding(0) == icu.BreakIterator.DONE


def test_previous() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src = icu.UnicodeString("foo bar baz.")
    bi.set_text(src)

    # int32_t icu::BreakIterator::previous(void)
    assert bi.last() == 12
    assert bi.previous() == 11
    assert bi.previous() == 8
    assert bi.previous() == 7
    assert bi.previous() == 4
    assert bi.previous() == 3
    assert bi.previous() == 0
    assert bi.previous() == icu.BreakIterator.DONE


def test_rule_based_break_iterator() -> None:
    src = icu.UnicodeString("foo.")

    # [1]
    # icu::RuleBasedBreakIterator::RuleBasedBreakIterator()
    # NotImplemented

    # [3]
    # icu::RuleBasedBreakIterator::RuleBasedBreakIterator(
    #       const UnicodeString &rules,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    rules = icu.UnicodeString(
        "$dictionary = [a-z]; \n"
        "!!forward; \n"
        "$dictionary $dictionary; \n"
        "!!reverse; \n"
        "$dictionary $dictionary; \n"
    )
    parse_error = icu.UParseError()
    bi3 = icu.RuleBasedBreakIterator(rules, parse_error)
    bi3.set_text(src)
    assert bi3.first() == 0
    assert bi3.next() == 2
    assert bi3.next() == 3
    assert bi3.next() == 4
    assert bi3.next() == icu.BreakIterator.DONE
    assert bi3.current() == 4

    bi3a = icu.RuleBasedBreakIterator(
        "$dictionary = [a-z]; \n"
        "!!forward; \n"
        "$dictionary $dictionary; \n"
        "!!reverse; \n"
        "$dictionary $dictionary; \n",
        parse_error,
    )
    bi3a.set_text(src)
    assert bi3a.first() == 0
    assert bi3a.next() == 2
    assert bi3a.next() == 3
    assert bi3a.next() == 4
    assert bi3a.next() == icu.BreakIterator.DONE
    assert bi3a.current() == 4
    assert bi3 == bi3a

    # [4]
    # icu::RuleBasedBreakIterator::RuleBasedBreakIterator(
    #       const uint8_t *compiledRules,
    #       uint32_t ruleLength,
    #       UErrorCode &status
    # )
    binary_rules = bi3.get_binary_rules()
    assert isinstance(binary_rules, bytes)
    assert len(binary_rules) > 0
    bi4 = icu.RuleBasedBreakIterator(binary_rules, len(binary_rules))
    bi4.set_text(src)
    assert bi4.first() == 0
    assert bi4.next() == 2
    assert bi4.next() == 3
    assert bi4.next() == 4
    assert bi4.next() == icu.BreakIterator.DONE
    assert bi4.current() == 4

    # [2]
    # icu::RuleBasedBreakIterator::RuleBasedBreakIterator(
    #       const RuleBasedBreakIterator &that
    # )
    bi2 = icu.RuleBasedBreakIterator(bi3)
    assert bi3.current() == 4
    assert bi2.current() == 4
    assert bi2.first() == 0
    assert bi2.next() == 2
    assert bi2.next() == 3
    assert bi2.next() == 4
    assert bi2.next() == icu.BreakIterator.DONE
    assert bi2.current() == 4

    # [5]
    # icu::RuleBasedBreakIterator::RuleBasedBreakIterator(
    #       UDataMemory *image,
    #       UErrorCode &status
    # )
    # NotImplemented


def test_set_text() -> None:
    bi = icu.BreakIterator.create_word_instance(icu.Locale.get_us())
    src1 = icu.UnicodeString("foo bar baz.")
    src2 = icu.UnicodeString("lorem ipsum")

    # [1]
    # void icu::BreakIterator::setText(const UnicodeString &newText)
    bi.set_text(src1)
    assert bi.current() == 0
    assert bi.next() == 3

    bi.set_text(src2)
    assert bi.current() == 0
    assert bi.next() == 5

    # [2]
    # void icu::BreakIterator::setText(
    #       UText *text,
    #       UErrorCode &status
    # )
    ut = icu.utext_open_const_unicode_string(None, src1)
    bi.set_text(ut)
    assert bi.current() == 0
    assert bi.next() == 3

    icu.utext_open_const_unicode_string(ut, src2)
    bi.set_text(ut)
    assert bi.current() == 0
    assert bi.next() == 5
    icu.utext_close(ut)

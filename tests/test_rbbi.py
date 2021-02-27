import copy

from icupy import (
    BreakIterator, CharacterIterator, Locale, RuleBasedBreakIterator,
    StringCharacterIterator, StringEnumeration, ULocDataLocaleType,
    UnicodeString, UParseError, UWordBreak,
    utext_close, utext_extract, utext_native_length,
    utext_open_const_unicode_string,
)


def test_adopt_text():
    bi = BreakIterator.create_line_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    it = StringCharacterIterator(src)
    bi.adopt_text(it)
    assert bi.first() == 0
    assert bi.next() == 4
    assert bi.next() == 8
    assert bi.next() == 12
    assert bi.next() == BreakIterator.DONE

    bi.adopt_text(None)
    assert bi.first() == 0
    assert bi.next() == BreakIterator.DONE


def test_clone():
    bi1 = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi1.set_text(src)
    bi1.next()

    bi2 = bi1.clone()
    assert isinstance(bi2, BreakIterator)
    assert id(bi1) != id(bi2)
    assert bi1 == bi2
    assert bi1.current() == bi2.current()

    bi3 = copy.copy(bi1)
    assert bi1 == bi3
    assert bi1.current() == bi3.current()

    bi4 = copy.deepcopy(bi1)
    assert bi1 == bi4
    assert bi1.current() == bi4.current()


def test_create_character_instance():
    bi = BreakIterator.create_character_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
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
    assert bi.next() == BreakIterator.DONE

    assert list(bi) == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
    assert reversed(bi) == [12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1]


def test_create_line_instance():
    bi = BreakIterator.create_line_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 4
    assert bi.next() == 8
    assert bi.next() == 12
    assert bi.next() == BreakIterator.DONE

    assert list(bi) == [4, 8, 12]
    assert reversed(bi) == [12, 8, 4]


def test_create_sentence_instance():
    bi = BreakIterator.create_sentence_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 12
    assert bi.next() == BreakIterator.DONE

    assert list(bi) == [12]
    assert reversed(bi) == [12]


def test_create_word_instance():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.first() == 0
    assert bi.next() == 3
    assert bi.next() == 4
    assert bi.next() == 7
    assert bi.next() == 8
    assert bi.next() == 11
    assert bi.next() == 12
    assert bi.next() == BreakIterator.DONE

    assert list(bi) == [3, 4, 7, 8, 11, 12]
    assert reversed(bi) == [12, 11, 8, 7, 4, 3]


def test_following():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.following(0) == 3
    assert bi.following(3) == 4
    assert bi.following(4) == 7
    assert bi.following(7) == 8
    assert bi.following(8) == 11
    assert bi.following(11) == 12
    assert bi.following(12) == BreakIterator.DONE


def test_get_available_locales():
    # [2]
    # static StringEnumeration* BreakIterator::getAvailableLocales()
    it = BreakIterator.get_available_locales()
    assert isinstance(it, StringEnumeration)
    assert len(it) > 0
    assert all(isinstance(x, str) for x in it)
    assert "ja" in it
    assert "und" not in it


def test_get_display_name():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        us_locale = Locale.get_us()
        Locale.set_default(us_locale)
        dest = UnicodeString()

        # [1]
        # static UnicodeString& getDisplayName(const Locale &objectLocale,
        #                                      const Locale &displayLocale,
        #                                      UnicodeString &name
        # )
        result = BreakIterator.get_display_name(
            Locale.get_france(),
            us_locale,
            dest)
        assert id(result) == id(dest)
        assert dest == "French (France)"

        # [2]
        # static UnicodeString& getDisplayName(const Locale &objectLocale,
        #                                      UnicodeString &name
        # )
        result = BreakIterator.get_display_name(us_locale, dest)
        assert id(result) == id(dest)
        assert dest == "English (United States)"
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_locale():
    bi1 = BreakIterator.create_word_instance(Locale.get_english())
    bi2 = BreakIterator.create_word_instance(Locale.get_french())

    loc = bi1.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "en"

    loc = bi1.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert len(loc.get_name()) == 0

    loc = bi2.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert loc.get_name() == "fr"

    loc = bi2.get_locale(ULocDataLocaleType.ULOC_ACTUAL_LOCALE)
    assert len(loc.get_name()) == 0


def test_get_rule_status():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz!")
    bi.set_text(src)
    assert bi.first() == 0
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_NONE
    assert bi.next() == 3
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_LETTER
    assert bi.next() == 4
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_NONE
    assert bi.next() == 7
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_LETTER
    assert bi.next() == 8
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_NONE
    assert bi.next() == 11
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_LETTER
    assert bi.next() == 12
    assert bi.get_rule_status() == UWordBreak.UBRK_WORD_NONE
    assert bi.next() == BreakIterator.DONE


def test_get_rule_status_vec():
    # From icu/source/test/intltest/rbbiapts.cpp:
    # void RBBIAPITest::TestRuleStatusVec()
    rules = UnicodeString("[A-N]{100}; \n"
                          "[a-w]{200}; \n"
                          "[\\p{L}]{300}; \n"
                          "[\\p{N}]{400}; \n"
                          "[0-5]{500}; \n"
                          "!.*;\n",
                          -1,
                          UnicodeString.EInvariant.INVARIANT)
    parse_error = UParseError()
    bi = RuleBasedBreakIterator(rules, parse_error)
    src = UnicodeString("Aapz5?")
    bi.set_text(src)

    assert bi.next() == 1
    values = bi.get_rule_status_vec()
    assert values == [100, 300]

    assert bi.next() == 2
    values = bi.get_rule_status_vec()
    assert values == [200, 300]

    assert bi.next() == 3
    values = bi.get_rule_status_vec()
    assert values == [200, 300]

    assert bi.next() == 4
    values = bi.get_rule_status_vec()
    assert values == [300]

    assert bi.next() == 5
    values = bi.get_rule_status_vec()
    assert values == [400, 500]

    assert bi.next() == 6
    values = bi.get_rule_status_vec()
    assert values == [0]


def test_get_rules():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    rules = bi.get_rules()
    assert isinstance(rules, UnicodeString)
    assert rules.length() > 0


def test_get_text():
    bi = BreakIterator.create_word_instance(Locale.get_us())

    it = bi.get_text()
    assert it is None

    src = UnicodeString("foo bar baz.")
    bi.set_text(src)
    it = bi.get_text()
    assert isinstance(it, CharacterIterator)
    dest = UnicodeString()
    it.get_text(dest)
    assert dest == src


def test_get_utext():
    bi = BreakIterator.create_word_instance(Locale.get_us())

    ut1 = bi.get_utext(None)
    dest = utext_extract(ut1, 0, utext_native_length(ut1))
    assert len(dest) == 0
    utext_close(ut1)

    src1 = UnicodeString("ABC")
    bi.set_text(src1)
    ut1 = bi.get_utext(None)
    assert utext_extract(ut1, 0, utext_native_length(ut1)) == "ABC"

    src2 = UnicodeString("abc")
    bi.set_text(src2)
    ut2 = bi.get_utext(ut1)
    assert ut2 == ut1
    assert utext_extract(ut1, 0, utext_native_length(ut1)) == "abc"
    utext_close(ut1)


def test_hash_code():
    bi1 = BreakIterator.create_character_instance(Locale.get_us())
    bi2 = BreakIterator.create_word_instance(Locale.get_us())
    bi3 = BreakIterator.create_word_instance(Locale.get_us())

    assert bi1.hash_code() != bi2.hash_code()
    assert bi2.hash_code() == bi3.hash_code()

    src = UnicodeString("foo bar baz.")
    bi1.set_text(src)
    assert bi1.hash_code() != bi2.hash_code()
    assert bi2.hash_code() == bi3.hash_code()

    bi1.next()
    assert bi1.hash_code() != bi2.hash_code()
    assert bi2.hash_code() == bi3.hash_code()


def test_is_boundary():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.is_boundary(0)
    assert not bi.is_boundary(1)
    assert not bi.is_boundary(2)
    assert bi.is_boundary(3)
    assert bi.is_boundary(4)
    assert not bi.is_boundary(5)
    assert not bi.is_boundary(6)
    assert bi.is_boundary(7)
    assert bi.is_boundary(8)
    assert not bi.is_boundary(9)
    assert not bi.is_boundary(10)
    assert bi.is_boundary(11)
    assert bi.is_boundary(12)


def test_next():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.first() == 0
    assert bi.next(3) == 7
    assert bi.next(3) == 12
    assert bi.next(3) == BreakIterator.DONE
    assert bi.current() == 12
    assert bi.next(-3) == 7
    assert bi.next(-3) == 0
    assert bi.next(-3) == BreakIterator.DONE
    assert bi.current() == 0


def test_operator():
    bi1 = BreakIterator.create_character_instance(Locale.get_us())
    bi2 = BreakIterator.create_word_instance(Locale.get_us())
    bi3 = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    assert bi1 != bi2
    assert bi1 != bi3
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


def test_preceding():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    # [0, 3, 4, 7, 8, 11, 12]
    assert bi.preceding(12) == 11
    assert bi.preceding(11) == 8
    assert bi.preceding(8) == 7
    assert bi.preceding(7) == 4
    assert bi.preceding(4) == 3
    assert bi.preceding(3) == 0
    assert bi.preceding(0) == BreakIterator.DONE


def test_previous():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src = UnicodeString("foo bar baz.")
    bi.set_text(src)

    assert bi.last() == 12
    assert bi.previous() == 11
    assert bi.previous() == 8
    assert bi.previous() == 7
    assert bi.previous() == 4
    assert bi.previous() == 3
    assert bi.previous() == 0
    assert bi.previous() == BreakIterator.DONE


def test_rule_based_break_iterator():
    src = UnicodeString("foo.")

    # [1]
    # RuleBasedBreakIterator()
    pass  # NotImplemented

    # [3]
    # RuleBasedBreakIterator(const UnicodeString &rules,
    #                        UParseError &parseError,
    #                        UErrorCode &status
    # )
    rules = UnicodeString("$dictionary = [a-z]; \n"
                          "!!forward; \n"
                          "$dictionary $dictionary; \n"
                          "!!reverse; \n"
                          "$dictionary $dictionary; \n")
    parse_error = UParseError()
    bi3 = RuleBasedBreakIterator(rules, parse_error)
    bi3.set_text(src)
    assert bi3.first() == 0
    assert bi3.next() == 2
    assert bi3.next() == 3
    assert bi3.next() == 4
    assert bi3.next() == BreakIterator.DONE
    assert bi3.current() == 4

    # [4]
    # RuleBasedBreakIterator(const uint8_t *compiledRules,
    #                        uint32_t ruleLength,
    #                        UErrorCode &status
    # )
    binary_rules = bi3.get_binary_rules()
    assert isinstance(binary_rules, list)
    assert len(binary_rules) > 0
    bi4 = RuleBasedBreakIterator(binary_rules, len(binary_rules))
    bi4.set_text(src)
    assert bi4.first() == 0
    assert bi4.next() == 2
    assert bi4.next() == 3
    assert bi4.next() == 4
    assert bi4.next() == BreakIterator.DONE
    assert bi4.current() == 4

    # [2]
    # RuleBasedBreakIterator(const RuleBasedBreakIterator &that)
    bi2 = RuleBasedBreakIterator(bi3)
    assert bi3.current() == 4
    assert bi2.current() == 4
    assert bi2.first() == 0
    assert bi2.next() == 2
    assert bi2.next() == 3
    assert bi2.next() == 4
    assert bi2.next() == BreakIterator.DONE
    assert bi2.current() == 4

    # [5]
    # RuleBasedBreakIterator(UDataMemory *image,
    #                        UErrorCode &status
    # )
    pass  # NotImplemented


def test_set_text():
    bi = BreakIterator.create_word_instance(Locale.get_us())
    src1 = UnicodeString("foo bar baz.")
    src2 = UnicodeString("lorem ipsum")

    # [1]
    # void BreakIterator::setText(const UnicodeString &newText)
    bi.set_text(src1)
    assert bi.current() == 0
    assert bi.next() == 3

    bi.set_text(src2)
    assert bi.current() == 0
    assert bi.next() == 5

    # [2]
    # void BreakIterator::setText(UText *text,
    #                             UErrorCode &status
    # )
    ut = utext_open_const_unicode_string(None, src1)
    bi.set_text(ut)
    assert bi.current() == 0
    assert bi.next() == 3

    utext_open_const_unicode_string(ut, src2)
    bi.set_text(ut)
    assert bi.current() == 0
    assert bi.next() == 5
    utext_close(ut)

import copy

import pytest

from icupy import (
    ICUException, Locale, StringEnumeration, U_ICU_VERSION_MAJOR_NUM,
    UErrorCode, UnicodeString
)


def test_add_likely_subtags():
    loc = Locale("sr")
    loc.add_likely_subtags()
    assert loc.get_name() == "sr_Cyrl_RS"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_canonicalize():
    loc = Locale.for_language_tag("aa-Deva-IN-SAAHO-u-ca-japanese")
    # "aa_Deva_IN_SAAHO@calendar=japanese"
    loc.canonicalize()
    # "ssy_Deva_IN@calendar=japanese"
    assert loc.to_language_tag() == "ssy-Deva-IN-u-ca-japanese"


def test_clone():
    test1 = Locale.get_us()
    test2 = test1.clone()
    assert isinstance(test2, Locale)
    assert id(test1) != id(test2)
    assert test1.get_name() == test2.get_name()

    test3 = copy.copy(test1)
    assert test1.get_name() == test3.get_name()

    test4 = copy.deepcopy(test1)
    assert test1.get_name() == test4.get_name()


def test_constants():
    loc = Locale.get_canada()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en_CA"
    assert Locale.CANADA == "en_CA"

    loc = Locale.get_canada_french()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "fr_CA"
    assert Locale.CANADA_FRENCH == "fr_CA"

    loc = Locale.get_china()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_CN"
    assert Locale.CHINA == "zh_CN"

    loc = Locale.get_chinese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh"
    assert Locale.CHINESE == "zh"

    loc = Locale.get_english()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en"
    assert Locale.ENGLISH == "en"

    loc = Locale.get_france()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "fr_FR"
    assert Locale.FRANCE == "fr_FR"

    loc = Locale.get_french()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "fr"
    assert Locale.FRENCH == "fr"

    loc = Locale.get_german()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "de"
    assert Locale.GERMAN == "de"

    loc = Locale.get_germany()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "de_DE"
    assert Locale.GERMANY == "de_DE"

    loc = Locale.get_italian()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "it"
    assert Locale.ITALIAN == "it"

    loc = Locale.get_italy()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "it_IT"
    assert Locale.ITALY == "it_IT"

    loc = Locale.get_japan()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ja_JP"
    assert Locale.JAPAN == "ja_JP"

    loc = Locale.get_japanese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ja"
    assert Locale.JAPANESE == "ja"

    loc = Locale.get_korea()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ko_KR"
    assert Locale.KOREA == "ko_KR"

    loc = Locale.get_korean()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ko"
    assert Locale.KOREAN == "ko"

    loc = Locale.get_prc()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_CN"
    assert Locale.PRC == "zh_CN"

    loc = Locale.get_root()
    assert isinstance(loc, Locale)
    assert len(loc.get_name()) == 0

    loc = Locale.get_simplified_chinese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_CN"
    assert Locale.SIMPLIFIED_CHINESE == "zh_CN"

    loc = Locale.get_taiwan()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_TW"
    assert Locale.TAIWAN == "zh_TW"

    loc = Locale.get_traditional_chinese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_TW"
    assert Locale.TRADITIONAL_CHINESE == "zh_TW"

    loc = Locale.get_uk()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en_GB"
    assert Locale.UK == "en_GB"

    loc = Locale.get_us()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en_US"
    assert Locale.US == "en_US"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_create_canonical():
    loc = Locale.create_canonical("aa-Deva-IN-SAAHO-u-ca-japanese")
    assert loc.to_language_tag() == "ssy-Deva-IN-u-ca-japanese"


def test_create_from_name():
    loc = Locale.create_from_name(None)
    assert loc == Locale.get_default()

    loc = Locale.create_from_name("x-klingon-latn-zx.utf32be@special")
    assert loc.get_language() == "x-klingon"
    assert loc.get_script() == "Latn"
    assert loc.get_country() == "ZX"
    assert len(loc.get_variant()) == 0


def test_create_keywords():
    # StringEnumeration* createKeywords(UErrorCode &status)
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    result = loc.create_keywords()
    assert isinstance(result, StringEnumeration)
    assert len(result) == 2
    t = [x for x in result]
    assert "calendar" in t
    assert "collation" in t

    loc = Locale("de")
    result = loc.create_keywords()
    assert result is None

    loc.set_to_bogus()
    result = loc.create_keywords()
    assert result is None


def test_create_unicode_keywords():
    # StringEnumeration* createUnicodeKeywords(UErrorCode &status)
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    result = loc.create_unicode_keywords()
    assert isinstance(result, StringEnumeration)
    assert len(result) == 2
    t = [x for x in result]
    assert "ca" in t
    assert "co" in t

    loc = Locale("de")
    result = loc.create_unicode_keywords()
    assert result is None

    loc.set_to_bogus()
    result = loc.create_unicode_keywords()
    assert result is None


def test_for_language_tag():
    loc = Locale.for_language_tag("af-t-ar-i0-handwrit-u-ca-coptic-x-foo")
    assert loc.get_name() == "af@calendar=coptic;t=ar-i0-handwrit;x=foo"


def test_get_available_locales():
    result = Locale.get_available_locales()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, Locale) for x in result)
    assert Locale.get_default() in result
    assert Locale.get_root() not in result


def test_get_base_name():
    loc = Locale("de_DE@calendar=buddhist;collation=phonebook")
    assert loc.get_base_name() == "de_DE"


def test_get_country():
    loc = Locale("de_DE@calendar=buddhist;collation=phonebook")
    assert loc.get_country() == "DE"


def test_get_default():
    loc = Locale.get_default()
    assert isinstance(loc, Locale)
    assert loc == Locale.create_from_name(None)
    assert len(loc.get_name()) > 0


def test_get_display_country():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        loc = Locale.get_us()
        Locale.set_default(loc)
        display_locale = Locale.get_france()
        dest = UnicodeString()

        # [1]
        # UnicodeString& getDisplayCountry(const Locale &displayLocale,
        #                                  UnicodeString &dispCountry
        # )
        result = loc.get_display_country(display_locale, dest)
        assert dest == UnicodeString("\\u00C9tats-Unis").unescape()
        assert id(result) == id(dest)

        # [2]
        # UnicodeString& getDisplayCountry(UnicodeString &dispCountry)
        dest.remove()
        result = loc.get_display_country(dest)
        assert dest == "United States"
        assert id(result) == id(dest)
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_display_language():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        loc = Locale.get_us()
        Locale.set_default(loc)
        display_locale = Locale.get_france()
        dest = UnicodeString()

        # [1]
        # UnicodeString& getDisplayLanguage(const Locale &displayLocale,
        #                                   UnicodeString &dispLang
        # )
        result = loc.get_display_language(display_locale, dest)
        assert dest == "anglais"
        assert id(result) == id(dest)

        # [2]
        # UnicodeString& getDisplayLanguage(UnicodeString &dispLang)
        dest.remove()
        result = loc.get_display_language(dest)
        assert dest == "English"
        assert id(result) == id(dest)
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_display_name():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        loc = Locale.get_us()
        Locale.set_default(loc)
        display_locale = Locale.get_france()
        dest = UnicodeString()

        # [1]
        # UnicodeString& getDisplayName(const Locale &displayLocale,
        #                               UnicodeString &name
        # )
        result = loc.get_display_name(display_locale, dest)
        assert dest == UnicodeString("anglais (\\u00C9tats-Unis)").unescape()
        assert id(result) == id(dest)

        # [2]
        # UnicodeString& getDisplayName(UnicodeString &name)
        dest.remove()
        result = loc.get_display_name(dest)
        assert dest == "English (United States)"
        assert id(result) == id(dest)
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_display_script():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        loc = Locale.get_us()
        Locale.set_default(loc)
        loc = Locale("zh", "Hans", "CN")
        display_locale = Locale.get_france()
        dest = UnicodeString()

        # [1]
        # UnicodeString& getDisplayScript(const Locale &displayLocale,
        #                                 UnicodeString &dispScript
        # )
        result = loc.get_display_script(display_locale, dest)
        assert dest == UnicodeString("sinogrammes simplifi\\u00E9s").unescape()
        assert id(result) == id(dest)

        # [2]
        # UnicodeString& getDisplayScript(UnicodeString &dispScript)
        dest.remove()
        result = loc.get_display_script(dest)
        assert dest == "Simplified Han"
        assert id(result) == id(dest)
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_display_variant():
    default_locale = None
    try:
        default_locale = Locale.get_default()
        loc = Locale.get_us()
        Locale.set_default(loc)
        loc = Locale("no", "NO", "NY")
        display_locale = Locale.get_france()
        dest = UnicodeString()

        # [1]
        # UnicodeString& getDisplayVariant(const Locale &displayLocale,
        #                                  UnicodeString &dispVar
        # )
        result = loc.get_display_variant(display_locale, dest)
        assert dest == "NY"
        assert id(result) == id(dest)

        # [2]
        # UnicodeString& getDisplayVariant(UnicodeString &dispVar)
        dest.remove()
        result = loc.get_display_variant(dest)
        assert dest == "NY"
        assert id(result) == id(dest)
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_iso3_country():
    loc = Locale.get_us()
    assert loc.get_iso3_country() == "USA"


def test_get_iso3_language():
    loc = Locale.get_us()
    assert loc.get_iso3_language() == "eng"


def test_get_iso_countries():
    result = Locale.get_iso_countries()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, str) for x in result)
    assert all(len(x) == 2 for x in result)


def test_get_iso_languages():
    result = Locale.get_iso_languages()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, str) for x in result)
    assert all(2 <= len(x) <= 3 for x in result)


def test_get_keyword_value():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    value = loc.get_keyword_value("calendar")
    assert value == "buddhist"

    value = loc.get_keyword_value("collation")
    assert value == "phonebook"

    value = loc.get_keyword_value("currency")
    assert len(value) == 0

    loc.set_to_bogus()
    with pytest.raises(ICUException) as exc_info:
        _ = loc.get_keyword_value("calendar")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_get_keywords():
    # void getKeywords(OutputIterator iterator,
    #                  UErrorCode &status
    # )
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    result = loc.get_keywords()
    assert isinstance(result, set)
    assert len(result) == 2
    assert "calendar" in result
    assert "collation" in result

    if U_ICU_VERSION_MAJOR_NUM <= 64:
        # See https://unicode-org.atlassian.net/browse/ICU-20573
        pytest.skip("ICU4C<=64")

    loc = Locale("de")
    result = loc.get_keywords()
    assert len(result) == 0

    loc.set_to_bogus()
    result = loc.get_keywords()
    assert len(result) == 0


def test_get_lcid():
    loc = Locale("en", "US")
    assert loc.get_lcid() == 0x409


def test_get_name():
    loc = Locale("de", "DE")
    loc.set_keyword_value("collation", "phonebook")
    loc.set_keyword_value("calendar", "buddhist")
    assert loc.get_name() == "de_DE@calendar=buddhist;collation=phonebook"
    assert str(loc) == loc.get_name()


def test_get_script():
    loc = Locale("no", "NO", "NY")
    assert len(loc.get_script()) == 0

    loc = Locale("zh", "Hans", "CN")
    assert loc.get_script() == "Hans"


def test_get_unicode_keyword_value():
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    value = loc.get_unicode_keyword_value("ca")
    assert value == "buddhist"

    value = loc.get_unicode_keyword_value("co")
    assert value == "phonebk"

    with pytest.raises(ICUException) as exc_info:
        _ = loc.get_unicode_keyword_value("cu")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    loc.set_to_bogus()
    with pytest.raises(ICUException) as exc_info:
        _ = loc.get_unicode_keyword_value("ca")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


def test_get_unicode_keywords():
    # void getUnicodeKeywords(OutputIterator iterator,
    #                         UErrorCode &status
    # )
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    result = loc.get_unicode_keywords()
    assert isinstance(result, set)
    assert len(result) == 2
    assert "ca" in result
    assert "co" in result

    if U_ICU_VERSION_MAJOR_NUM <= 64:
        # See https://unicode-org.atlassian.net/browse/ICU-20573
        pytest.skip("ICU4C<=64")

    loc = Locale("de")
    result = loc.get_unicode_keywords()
    assert len(result) == 0

    loc.set_to_bogus()
    result = loc.get_unicode_keywords()
    assert len(result) == 0


def test_get_variant():
    loc = Locale("no", "NO", "NY")
    assert loc.get_variant() == "NY"

    loc = Locale("zh", "Hans", "CN")
    assert len(loc.get_variant()) == 0


def test_hash_code():
    test1 = Locale("en", "US")
    test2 = Locale("en", "US")

    assert test1.hash_code() == test2.hash_code()

    test1.set_keyword_value("currency", "USD")
    assert test1.hash_code() != test2.hash_code()

    test2.set_keyword_value("currency", "USD")
    assert test1.hash_code() == test2.hash_code()


def test_is_bogus():
    loc = Locale("en", "US")
    assert not loc.is_bogus()

    loc.set_to_bogus()
    assert loc.is_bogus()


def test_is_right_to_left():
    loc = Locale("ar")
    assert loc.is_right_to_left()

    loc = Locale("zh")
    assert not loc.is_right_to_left()


def test_locale():
    # [1]
    # Locale()
    test1 = Locale()
    assert test1 == Locale.get_default()

    # [2]
    # Locale(const char *language,
    #        const char *country=0,
    #        const char *variant=0,
    #        const char *keywordsAndValues=0
    # )
    test20 = Locale(None)
    assert test20 == test1

    test21 = Locale("")
    assert test21 != test1

    test22 = Locale("en")
    assert test22.get_name() == "en"

    test23 = Locale("en", "US")
    assert test23.get_name() == "en_US"

    test24 = Locale("no", "NO", "NY")
    assert test24.get_name() == "no_NO_NY"

    test25 = Locale("no", variant="NY", country="NO")
    assert test25.get_name() == test24.get_name()

    test26 = Locale("no", "NO", "NY", "currency=NOK")
    assert test26.get_name() == "no_NO_NY@currency=NOK"

    # [3]
    # Locale(const Locale &other)
    test3 = Locale(test26)
    assert test3.get_name() == test26.get_name()


def test_minimize_subtags():
    loc = Locale("zh_Hant_TW")
    loc.minimize_subtags()
    assert loc.get_name() == "zh_TW"


def test_operator():
    test1 = Locale("en", "US")
    test2 = Locale("en", "US")
    test3 = Locale("en", "GB")

    # operator!=()
    assert not (test1 != test2)
    assert test1 != test3

    # operator==()
    assert test1 == test2
    assert not (test1 == test3)


def test_set_default():
    default_locale = None
    try:
        default_locale = Locale.get_default()

        new_locale = Locale.get_japan()
        if new_locale == default_locale:
            new_locale = Locale.get_us()
        Locale.set_default(new_locale)
        assert Locale.get_default() == new_locale

        Locale.set_default(None)
        assert Locale.get_default() == new_locale
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_set_keyword_value():
    loc = Locale("de", "DE")
    loc.set_keyword_value("collation", "phonebook")
    loc.set_keyword_value("calendar", "buddhist")
    assert loc.get_name() == "de_DE@calendar=buddhist;collation=phonebook"

    with pytest.raises(ICUException) as exc_info:
        loc.set_keyword_value(None, None)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    loc.set_keyword_value("calendar", None)
    assert loc.get_name() == "de_DE@collation=phonebook"

    loc.set_keyword_value("collation", "")
    assert loc.get_name() == "de_DE"


def test_set_unicode_keyword_value():
    loc = Locale("de", "DE")
    loc.set_unicode_keyword_value("co", "phonebk")
    loc.set_unicode_keyword_value("ca", "buddhist")
    assert loc.get_name() == "de_DE@calendar=buddhist;collation=phonebook"

    with pytest.raises(ICUException) as exc_info:
        loc.set_unicode_keyword_value(None, None)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    loc.set_unicode_keyword_value("ca", None)
    assert loc.get_name() == "de_DE@collation=phonebook"

    loc.set_unicode_keyword_value("co", "")
    assert loc.get_name() == "de_DE"


def test_to_language_tag():
    loc = Locale("af@calendar=coptic;t=ar-i0-handwrit;x=foo")
    assert loc.to_language_tag() == "af-t-ar-i0-handwrit-u-ca-coptic-x-foo"

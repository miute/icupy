import copy

import pytest
from icupy.icu import (
    ICUError, Locale, StringEnumeration, UErrorCode,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_add_likely_subtags():
    # void icu::Locale::addLikelySubtags(UErrorCode &status)
    loc = Locale("sr")
    loc.add_likely_subtags()
    assert loc.get_name() == "sr_Cyrl_RS"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_canonicalize():
    # "aa_Deva_IN_SAAHO@calendar=japanese"
    loc = Locale.for_language_tag("aa-Deva-IN-SAAHO-u-ca-japanese")

    # void icu::Locale::canonicalize(UErrorCode &status)
    loc.canonicalize()

    # "ssy_Deva_IN@calendar=japanese"
    assert loc.to_language_tag() == "ssy-Deva-IN-u-ca-japanese"


def test_clone():
    test1 = Locale.get_us()

    # Locale *icu::Locale::clone()
    test2 = test1.clone()
    assert isinstance(test2, Locale)
    assert id(test1) != id(test2)
    assert test1.get_name() == test2.get_name()

    test3 = copy.copy(test1)
    assert test1.get_name() == test3.get_name()

    test4 = copy.deepcopy(test1)
    assert test1.get_name() == test4.get_name()


def test_constants():
    # static const Locale &icu::Locale::getCanada(void)
    loc = Locale.get_canada()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en_CA"
    assert Locale.CANADA == "en_CA"

    # static const Locale &icu::Locale::getCanadaFrench(void)
    loc = Locale.get_canada_french()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "fr_CA"
    assert Locale.CANADA_FRENCH == "fr_CA"

    # static const Locale &icu::Locale::getChina(void)
    loc = Locale.get_china()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_CN"
    assert Locale.CHINA == "zh_CN"

    # static const Locale &icu::Locale::getChinese(void)
    loc = Locale.get_chinese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh"
    assert Locale.CHINESE == "zh"

    # static const Locale &icu::Locale::getEnglish(void)
    loc = Locale.get_english()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en"
    assert Locale.ENGLISH == "en"

    # static const Locale &icu::Locale::getFrance(void)
    loc = Locale.get_france()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "fr_FR"
    assert Locale.FRANCE == "fr_FR"

    # static const Locale &icu::Locale::getFrench(void)
    loc = Locale.get_french()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "fr"
    assert Locale.FRENCH == "fr"

    # static const Locale &icu::Locale::getGerman(void)
    loc = Locale.get_german()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "de"
    assert Locale.GERMAN == "de"

    # static const Locale &icu::Locale::getGermany(void)
    loc = Locale.get_germany()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "de_DE"
    assert Locale.GERMANY == "de_DE"

    # static const Locale &icu::Locale::getItalian(void)
    loc = Locale.get_italian()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "it"
    assert Locale.ITALIAN == "it"

    # static const Locale &icu::Locale::getItaly(void)
    loc = Locale.get_italy()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "it_IT"
    assert Locale.ITALY == "it_IT"

    # static const Locale &icu::Locale::getJapan(void)
    loc = Locale.get_japan()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ja_JP"
    assert Locale.JAPAN == "ja_JP"

    # static const Locale &icu::Locale::getJapanese(void)
    loc = Locale.get_japanese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ja"
    assert Locale.JAPANESE == "ja"

    # static const Locale &icu::Locale::getKorea(void)
    loc = Locale.get_korea()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ko_KR"
    assert Locale.KOREA == "ko_KR"

    # static const Locale &icu::Locale::getKorean(void)
    loc = Locale.get_korean()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "ko"
    assert Locale.KOREAN == "ko"

    # static const Locale &icu::Locale::getPRC(void)
    loc = Locale.get_prc()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_CN"
    assert Locale.PRC == "zh_CN"

    # static const Locale &icu::Locale::getRoot(void)
    loc = Locale.get_root()
    assert isinstance(loc, Locale)
    assert len(loc.get_name()) == 0

    # static const Locale &icu::Locale::getSimplifiedChinese(void)
    loc = Locale.get_simplified_chinese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_CN"
    assert Locale.SIMPLIFIED_CHINESE == "zh_CN"

    # static const Locale &icu::Locale::getTaiwan(void)
    loc = Locale.get_taiwan()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_TW"
    assert Locale.TAIWAN == "zh_TW"

    # static const Locale &icu::Locale::getTraditionalChinese(void)
    loc = Locale.get_traditional_chinese()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "zh_TW"
    assert Locale.TRADITIONAL_CHINESE == "zh_TW"

    # static const Locale &icu::Locale::getUK(void)
    loc = Locale.get_uk()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en_GB"
    assert Locale.UK == "en_GB"

    # static const Locale &icu::Locale::getUS(void)
    loc = Locale.get_us()
    assert isinstance(loc, Locale)
    assert loc.get_name() == "en_US"
    assert Locale.US == "en_US"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_create_canonical():
    # static Locale icu::Locale::createCanonical(const char *name)
    loc = Locale.create_canonical("aa-Deva-IN-SAAHO-u-ca-japanese")
    assert isinstance(loc, Locale)
    assert loc.to_language_tag() == "ssy-Deva-IN-u-ca-japanese"


def test_create_from_name():
    # static Locale icu::Locale::createFromName(const char *name)
    loc = Locale.create_from_name(None)
    assert isinstance(loc, Locale)
    assert loc == Locale.get_default()

    loc = Locale.create_from_name("x-klingon-latn-zx.utf32be@special")
    assert isinstance(loc, Locale)
    assert loc.get_language() == "x-klingon"
    assert loc.get_script() == "Latn"
    assert loc.get_country() == "ZX"
    assert len(loc.get_variant()) == 0


def test_create_keywords():
    # StringEnumeration *icu::Locale::createKeywords(UErrorCode &status)
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


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_create_unicode_keywords():
    # StringEnumeration *icu::Locale::createUnicodeKeywords(UErrorCode &status)
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


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_for_language_tag():
    # static Locale icu::Locale::forLanguageTag(
    #       StringPiece tag,
    #       UErrorCode &status
    # )
    loc = Locale.for_language_tag("af-t-ar-i0-handwrit-u-ca-coptic-x-foo")
    assert isinstance(loc, Locale)
    assert loc.get_name() == "af@calendar=coptic;t=ar-i0-handwrit;x=foo"


def test_get_available_locales():
    # static const Locale *icu::Locale::getAvailableLocales(int32_t &count)
    result = Locale.get_available_locales()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, Locale) for x in result)
    assert Locale.get_default() in result
    assert Locale.get_root() not in result


def test_get_base_name():
    # const char *icu::Locale::getBaseName()
    loc = Locale("de_DE@calendar=buddhist;collation=phonebook")
    assert loc.get_base_name() == "de_DE"


def test_get_country():
    # const char *icu::Locale::getCountry()
    loc = Locale("de_DE@calendar=buddhist;collation=phonebook")
    assert loc.get_country() == "DE"


def test_get_default():
    # static const Locale &icu::Locale::getDefault(void)
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
        disp_country = UnicodeString()

        # [1]
        # UnicodeString &icu::Locale::getDisplayCountry(
        #       const Locale &displayLocale,
        #       UnicodeString &dispCountry
        # )
        result = loc.get_display_country(display_locale, disp_country)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_country)
        assert disp_country == UnicodeString("\\u00C9tats-Unis").unescape()

        disp_country.remove()
        result = loc.get_display_country("fr", disp_country)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_country)
        assert disp_country == UnicodeString("\\u00C9tats-Unis").unescape()

        # [2]
        # UnicodeString &icu::Locale::getDisplayCountry(
        #       UnicodeString &dispCountry
        # )
        disp_country.remove()
        result = loc.get_display_country(disp_country)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_country)
        assert disp_country == "United States"
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
        disp_lang = UnicodeString()

        # [1]
        # UnicodeString &icu::Locale::getDisplayLanguage(
        #       const Locale &displayLocale,
        #       UnicodeString &dispLang
        # )
        result = loc.get_display_language(display_locale, disp_lang)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_lang)
        assert disp_lang == "anglais"

        disp_lang.remove()
        result = loc.get_display_language("fr", disp_lang)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_lang)
        assert disp_lang == "anglais"

        # [2]
        # UnicodeString &icu::Locale::getDisplayLanguage(
        #       UnicodeString &dispLang
        # )
        disp_lang.remove()
        result = loc.get_display_language(disp_lang)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_lang)
        assert disp_lang == "English"
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
        name = UnicodeString()

        # [1]
        # UnicodeString &icu::Locale::getDisplayName(
        #       const Locale &displayLocale,
        #       UnicodeString &name
        # )
        result = loc.get_display_name(display_locale, name)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(name)
        assert name == UnicodeString("anglais (\\u00C9tats-Unis)").unescape()

        name.remove()
        result = loc.get_display_name("fr", name)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(name)
        assert name == UnicodeString("anglais (\\u00C9tats-Unis)").unescape()

        # [2]
        # UnicodeString &icu::Locale::getDisplayName(UnicodeString &name)
        name.remove()
        result = loc.get_display_name(name)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(name)
        assert name == "English (United States)"
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
        disp_script = UnicodeString()

        # [1]
        # UnicodeString &icu::Locale::getDisplayScript(
        #       const Locale &displayLocale,
        #       UnicodeString &dispScript
        # )
        result = loc.get_display_script(display_locale, disp_script)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_script)
        assert (disp_script
                == UnicodeString("sinogrammes simplifi\\u00E9s").unescape())

        disp_script.remove()
        result = loc.get_display_script("fr", disp_script)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_script)
        assert (disp_script
                == UnicodeString("sinogrammes simplifi\\u00E9s").unescape())

        # [2]
        # UnicodeString &icu::Locale::getDisplayScript(
        #       UnicodeString &dispScript
        # )
        disp_script.remove()
        result = loc.get_display_script(disp_script)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_script)
        assert disp_script == "Simplified Han"
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
        disp_var = UnicodeString()

        # [1]
        # UnicodeString &icu::Locale::getDisplayVariant(
        #       const Locale &displayLocale,
        #       UnicodeString &dispVar
        # )
        result = loc.get_display_variant(display_locale, disp_var)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_var)
        assert disp_var == "NY"

        disp_var.remove()
        result = loc.get_display_variant("fr", disp_var)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_var)
        assert disp_var == "NY"

        # [2]
        # UnicodeString &icu::Locale::getDisplayVariant(UnicodeString &dispVar)
        disp_var.remove()
        result = loc.get_display_variant(disp_var)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(disp_var)
        assert disp_var == "NY"
    finally:
        if default_locale:
            Locale.set_default(default_locale)


def test_get_iso3_country():
    # const char *icu::Locale::getISO3Country()
    loc = Locale.get_us()
    assert loc.get_iso3_country() == "USA"


def test_get_iso3_language():
    # const char *icu::Locale::getISO3Language()
    loc = Locale.get_us()
    assert loc.get_iso3_language() == "eng"


def test_get_iso_countries():
    # static const char * const *icu::Locale::getISOCountries()
    result = Locale.get_iso_countries()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, str) for x in result)
    assert all(len(x) == 2 for x in result)


def test_get_iso_languages():
    # static const char * const *icu::Locale::getISOLanguages()
    result = Locale.get_iso_languages()
    assert isinstance(result, list)
    assert len(result) > 0
    assert all(isinstance(x, str) for x in result)
    assert all(2 <= len(x) <= 3 for x in result)


def test_get_keyword_value():
    loc = Locale("de@calendar=buddhist;collation=phonebook")

    # [3]
    # template<typename StringClass>
    # StringClass icu::Locale::getKeywordValue(
    #       StringPiece keywordName,
    #       UErrorCode &status
    # )
    value = loc.get_keyword_value("calendar")
    assert isinstance(value, str)
    assert value == "buddhist"

    value = loc.get_keyword_value("collation")
    assert isinstance(value, str)
    assert value == "phonebook"

    value = loc.get_keyword_value("currency")
    assert isinstance(value, str)
    assert len(value) == 0

    loc.set_to_bogus()
    with pytest.raises(ICUError) as exc_info:
        _ = loc.get_keyword_value("calendar")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_get_keywords():
    loc = Locale("de@calendar=buddhist;collation=phonebook")

    # void icu::Locale::getKeywords(
    #       OutputIterator iterator,
    #       UErrorCode &status
    # )
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
    # uint32_t icu::Locale::getLCID(void)
    loc = Locale("en", "US")
    assert loc.get_lcid() == 0x409


def test_get_name():
    # const char *icu::Locale::getName()
    loc = Locale("de", "DE")
    loc.set_keyword_value("collation", "phonebook")
    loc.set_keyword_value("calendar", "buddhist")
    assert loc.get_name() == "de_DE@calendar=buddhist;collation=phonebook"
    assert str(loc) == loc.get_name()


def test_get_script():
    # const char *icu::Locale::getScript()
    loc = Locale("no", "NO", "NY")
    assert len(loc.get_script()) == 0

    loc = Locale("zh", "Hans", "CN")
    assert loc.get_script() == "Hans"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_get_unicode_keyword_value():
    # [2]
    # template<typename StringClass>
    # StringClass icu::Locale::getUnicodeKeywordValue(
    #       StringPiece keywordName,
    #       UErrorCode &status
    # )
    loc = Locale("de@calendar=buddhist;collation=phonebook")
    value = loc.get_unicode_keyword_value("ca")
    assert isinstance(value, str)
    assert value == "buddhist"

    value = loc.get_unicode_keyword_value("co")
    assert isinstance(value, str)
    assert value == "phonebk"

    with pytest.raises(ICUError) as exc_info:
        _ = loc.get_unicode_keyword_value("cu")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    loc.set_to_bogus()
    with pytest.raises(ICUError) as exc_info:
        _ = loc.get_unicode_keyword_value("ca")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_get_unicode_keywords():
    # void icu::Locale::getUnicodeKeywords(
    #       OutputIterator iterator,
    #       UErrorCode &status
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
    # const char *icu::Locale::getVariant()
    loc = Locale("no", "NO", "NY")
    assert loc.get_variant() == "NY"

    loc = Locale("zh", "Hans", "CN")
    assert len(loc.get_variant()) == 0


def test_hash_code():
    # int32_t icu::Locale::hashCode(void)
    test1 = Locale("en", "US")
    test2 = Locale("en", "US")

    assert test1.hash_code() == test2.hash_code()

    test1.set_keyword_value("currency", "USD")
    assert test1.hash_code() != test2.hash_code()

    test2.set_keyword_value("currency", "USD")
    assert test1.hash_code() == test2.hash_code()


def test_is_bogus():
    # UBool icu::Locale::isBogus(void)
    loc = Locale("en", "US")
    assert not loc.is_bogus()

    # void icu::Locale::setToBogus()
    loc.set_to_bogus()
    assert loc.is_bogus()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_is_right_to_left():
    # UBool icu::Locale::isRightToLeft()
    loc = Locale("ar")
    assert loc.is_right_to_left()

    loc = Locale("zh")
    assert not loc.is_right_to_left()


def test_locale():
    # [1]
    # icu::Locale::Locale()
    test1 = Locale()
    assert test1 == Locale.get_default()

    # [2]
    # icu::Locale::Locale(
    #       const char *language,
    #       const char *country=0,
    #       const char *variant=0,
    #       const char *keywordsAndValues=0
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
    # icu::Locale::Locale(const Locale &other)
    test3 = Locale(test26)
    assert test3.get_name() == test26.get_name()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_minimize_subtags():
    # void icu::Locale::minimizeSubtags(UErrorCode &status)
    loc = Locale("zh_Hant_TW")
    loc.minimize_subtags()
    assert loc.get_name() == "zh_TW"


def test_operator():
    test1 = Locale("en", "US")
    test2 = Locale("en", "US")
    test3 = Locale("en", "GB")

    # UBool icu::Locale::operator!=(const Locale &other)
    assert not (test1 != test2)
    assert test1 != test3

    # UBool icu::Locale::operator==(const Locale &other)
    assert test1 == test2
    assert not (test1 == test3)


def test_set_default():
    default_locale = None
    try:
        default_locale = Locale.get_default()

        new_locale = Locale.get_japan()
        if new_locale == default_locale:
            new_locale = Locale.get_us()

        # static void icu::Locale::setDefault(
        #       const Locale &newLocale,
        #       UErrorCode &success
        # )
        Locale.set_default(new_locale)
        assert Locale.get_default() == new_locale

        Locale.set_default(None)
        assert Locale.get_default() == new_locale
    finally:
        if default_locale:
            Locale.set_default(default_locale)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 49, reason="ICU4C<49")
def test_set_keyword_value():
    # [1]
    # void icu::Locale::setKeywordValue(
    #       const char *keywordName,
    #       const char *keywordValue,
    #       UErrorCode &status
    # )
    loc = Locale("de", "DE")
    loc.set_keyword_value("collation", "phonebook")
    loc.set_keyword_value("calendar", "buddhist")
    assert loc.get_name() == "de_DE@calendar=buddhist;collation=phonebook"

    with pytest.raises(ICUError) as exc_info:
        loc.set_keyword_value(None, None)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    loc.set_keyword_value("calendar", None)
    assert loc.get_name() == "de_DE@collation=phonebook"

    loc.set_keyword_value("collation", "")
    assert loc.get_name() == "de_DE"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_set_unicode_keyword_value():
    # void icu::Locale::setUnicodeKeywordValue(
    #       StringPiece keywordName,
    #       StringPiece keywordValue,
    #       UErrorCode &status
    # )
    loc = Locale("de", "DE")
    loc.set_unicode_keyword_value("co", "phonebk")
    loc.set_unicode_keyword_value("ca", "buddhist")
    assert loc.get_name() == "de_DE@calendar=buddhist;collation=phonebook"

    with pytest.raises(ICUError) as exc_info:
        loc.set_unicode_keyword_value(None, None)
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    loc.set_unicode_keyword_value("ca", None)
    assert loc.get_name() == "de_DE@collation=phonebook"

    loc.set_unicode_keyword_value("co", "")
    assert loc.get_name() == "de_DE"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_to_language_tag():
    # [2]
    # template<typename StringClass>
    # StringClass icu::Locale::toLanguageTag(UErrorCode &status)
    loc = Locale("af@calendar=coptic;t=ar-i0-handwrit;x=foo")
    assert loc.to_language_tag() == "af-t-ar-i0-handwrit-u-ca-coptic-x-foo"

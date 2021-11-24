import pytest
from icupy.icu import (
    Locale, LocaleDisplayNames, UDialectHandling, UScriptCode,
    U_ICU_VERSION_MAJOR_NUM, UnicodeString,
)


def test_api():
    ldn = LocaleDisplayNames.create_instance(
        Locale.get_us(),
        UDialectHandling.ULDN_DIALECT_NAMES)

    # UDialectHandling icu::LocaleDisplayNames::getDialectHandling()
    assert ldn.get_dialect_handling() == UDialectHandling.ULDN_DIALECT_NAMES

    # const Locale &icu::LocaleDisplayNames::getLocale()
    result = ldn.get_locale()
    assert isinstance(result, Locale)
    assert result == Locale("en-US")

    temp = UnicodeString()

    # UnicodeString &icu::LocaleDisplayNames::keyDisplayName(
    #       const char *key,
    #       UnicodeString &result
    # )
    result = ldn.key_display_name("efg", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "efg"

    # UnicodeString &icu::LocaleDisplayNames::keyValueDisplayName(
    #       const char *key,
    #       const char *value,
    #       UnicodeString &result
    # )
    result = ldn.key_value_display_name("ca", "ijk", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "ijk"

    # UnicodeString &icu::LocaleDisplayNames::languageDisplayName(
    #       const char *lang,
    #       UnicodeString &result
    # )
    result = ldn.language_display_name("xy", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy"

    # [1]
    # UnicodeString &icu::LocaleDisplayNames::localeDisplayName(
    #       const char *localeId,
    #       UnicodeString &result
    # )
    result = ldn.locale_display_name("xy", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy"

    # [2]
    # UnicodeString &icu::LocaleDisplayNames::localeDisplayName(
    #       const Locale &locale,
    #       UnicodeString &result
    # )
    result = ldn.locale_display_name(Locale("xy"), temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy"

    # UnicodeString &icu::LocaleDisplayNames::regionDisplayName(
    #       const char *region,
    #       UnicodeString &result
    # )
    result = ldn.region_display_name("xy-Latn-wx", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy-Latn-wx"

    # [1]
    # UnicodeString &icu::LocaleDisplayNames::scriptDisplayName(
    #       const char *script,
    #       UnicodeString &result
    # )
    result = ldn.script_display_name("wxyz", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "wxyz"

    # [2]
    # UnicodeString &icu::LocaleDisplayNames::scriptDisplayName(
    #       UScriptCode scriptCode,
    #       UnicodeString &result
    # )
    result = ldn.script_display_name(UScriptCode.USCRIPT_LATIN, temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "Latin"

    # UnicodeString &icu::LocaleDisplayNames::variantDisplayName(
    #       const char *variant,
    #       UnicodeString &result
    # )
    result = ldn.variant_display_name("abc", temp)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(temp)
    assert result == "abc"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51():
    from icupy.icu import UDisplayContext, UDisplayContextType

    ldn = LocaleDisplayNames.create_instance(Locale.get_germany())

    # UDisplayContext icu::LocaleDisplayNames::getContext(
    #       UDisplayContextType type
    # )
    result = ldn.get_context(
        UDisplayContextType.UDISPCTX_TYPE_SUBSTITUTE_HANDLING)
    assert result == UDisplayContext.UDISPCTX_SUBSTITUTE


def test_create_instance():
    temp = UnicodeString()

    # [1]
    # static LocaleDisplayNames *icu::LocaleDisplayNames::createInstance(
    #       const Locale &locale
    # )
    ldn = LocaleDisplayNames.create_instance(Locale.get_germany())
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    ldn = LocaleDisplayNames.create_instance("de-DE")
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    # [2]
    # static LocaleDisplayNames *icu::LocaleDisplayNames::createInstance(
    #       const Locale &locale,
    #       UDialectHandling dialectHandling
    # )
    ldn = LocaleDisplayNames.create_instance(
        Locale.get_us(),
        UDialectHandling.ULDN_DIALECT_NAMES)
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "English"

    ldn = LocaleDisplayNames.create_instance(
        "en-US",
        UDialectHandling.ULDN_DIALECT_NAMES)
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "English"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_create_instance_51():
    from icupy.icu import UDisplayContext

    temp = UnicodeString()

    # [3]
    # static LocaleDisplayNames *icu::LocaleDisplayNames::createInstance(
    #       const Locale &locale,
    #       UDisplayContext *contexts,
    #       int32_t length
    # )
    contexts = [
        UDisplayContext.UDISPCTX_NO_SUBSTITUTE,
        UDisplayContext.UDISPCTX_DIALECT_NAMES,
    ]
    ldn = LocaleDisplayNames.create_instance(
        Locale.get_germany(),
        contexts,
        len(contexts))
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    ldn = LocaleDisplayNames.create_instance(
        "de-DE",
        contexts,
        len(contexts))
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    # length is optional
    ldn = LocaleDisplayNames.create_instance(
        Locale.get_germany(),
        contexts)
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    ldn = LocaleDisplayNames.create_instance(
        "de-DE",
        contexts)
    assert isinstance(ldn, LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

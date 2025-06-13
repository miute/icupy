import pytest

from icupy import icu


def test_api():
    ldn = icu.LocaleDisplayNames.create_instance(
        icu.Locale.get_us(), icu.UDialectHandling.ULDN_DIALECT_NAMES
    )

    # UDialectHandling icu::LocaleDisplayNames::getDialectHandling()
    assert ldn.get_dialect_handling() == icu.UDialectHandling.ULDN_DIALECT_NAMES

    # const Locale &icu::LocaleDisplayNames::getLocale()
    result = ldn.get_locale()
    assert isinstance(result, icu.Locale)
    assert result == icu.Locale("en-US")

    temp = icu.UnicodeString()

    # UnicodeString &icu::LocaleDisplayNames::keyDisplayName(
    #       const char *key,
    #       UnicodeString &result
    # )
    result = ldn.key_display_name("efg", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "efg"

    # UnicodeString &icu::LocaleDisplayNames::keyValueDisplayName(
    #       const char *key,
    #       const char *value,
    #       UnicodeString &result
    # )
    result = ldn.key_value_display_name("ca", "ijk", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "ijk"

    # UnicodeString &icu::LocaleDisplayNames::languageDisplayName(
    #       const char *lang,
    #       UnicodeString &result
    # )
    result = ldn.language_display_name("xy", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy"

    # [1]
    # UnicodeString &icu::LocaleDisplayNames::localeDisplayName(
    #       const char *localeId,
    #       UnicodeString &result
    # )
    result = ldn.locale_display_name("xy", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy"

    # [2]
    # UnicodeString &icu::LocaleDisplayNames::localeDisplayName(
    #       const Locale &locale,
    #       UnicodeString &result
    # )
    result = ldn.locale_display_name(icu.Locale("xy"), temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy"

    # UnicodeString &icu::LocaleDisplayNames::regionDisplayName(
    #       const char *region,
    #       UnicodeString &result
    # )
    result = ldn.region_display_name("xy-Latn-wx", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "xy-Latn-wx"

    # [1]
    # UnicodeString &icu::LocaleDisplayNames::scriptDisplayName(
    #       const char *script,
    #       UnicodeString &result
    # )
    result = ldn.script_display_name("wxyz", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "wxyz"

    # [2]
    # UnicodeString &icu::LocaleDisplayNames::scriptDisplayName(
    #       UScriptCode scriptCode,
    #       UnicodeString &result
    # )
    result = ldn.script_display_name(icu.UScriptCode.USCRIPT_LATIN, temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "Latin"

    # UnicodeString &icu::LocaleDisplayNames::variantDisplayName(
    #       const char *variant,
    #       UnicodeString &result
    # )
    result = ldn.variant_display_name("abc", temp)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(temp)
    assert result == "abc"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51():
    ldn = icu.LocaleDisplayNames.create_instance(icu.Locale.get_germany())

    # UDisplayContext icu::LocaleDisplayNames::getContext(
    #       UDisplayContextType type
    # )
    result = ldn.get_context(icu.UDisplayContextType.UDISPCTX_TYPE_SUBSTITUTE_HANDLING)
    assert result == icu.UDisplayContext.UDISPCTX_SUBSTITUTE


def test_create_instance():
    temp = icu.UnicodeString()

    # [1]
    # static LocaleDisplayNames *icu::LocaleDisplayNames::createInstance(
    #       const Locale &locale
    # )
    ldn = icu.LocaleDisplayNames.create_instance(icu.Locale.get_germany())
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    ldn = icu.LocaleDisplayNames.create_instance("de-DE")
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    # [2]
    # static LocaleDisplayNames *icu::LocaleDisplayNames::createInstance(
    #       const Locale &locale,
    #       UDialectHandling dialectHandling
    # )
    ldn = icu.LocaleDisplayNames.create_instance(
        icu.Locale.get_us(), icu.UDialectHandling.ULDN_DIALECT_NAMES
    )
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "English"

    ldn = icu.LocaleDisplayNames.create_instance(
        "en-US", icu.UDialectHandling.ULDN_DIALECT_NAMES
    )
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp == "xy"
    ldn.locale_display_name("en", temp)
    assert temp == "English"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_create_instance_51():
    temp = icu.UnicodeString()

    # [3]
    # static LocaleDisplayNames *icu::LocaleDisplayNames::createInstance(
    #       const Locale &locale,
    #       UDisplayContext *contexts,
    #       int32_t length
    # )
    contexts = [
        icu.UDisplayContext.UDISPCTX_NO_SUBSTITUTE,
        icu.UDisplayContext.UDISPCTX_DIALECT_NAMES,
    ]
    ldn = icu.LocaleDisplayNames.create_instance(
        icu.Locale.get_germany(), contexts, len(contexts)
    )
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    ldn = icu.LocaleDisplayNames.create_instance("de-DE", contexts, len(contexts))
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    # length is optional
    ldn = icu.LocaleDisplayNames.create_instance(icu.Locale.get_germany(), contexts)
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

    ldn = icu.LocaleDisplayNames.create_instance("de-DE", contexts)
    assert isinstance(ldn, icu.LocaleDisplayNames)
    ldn.locale_display_name("xy", temp)
    assert temp.is_bogus()
    ldn.locale_display_name("en", temp)
    assert temp == "Englisch"

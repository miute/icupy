import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 64:
    pytest.skip("ICU4C<64", allow_module_level=True)


def test_api() -> None:
    # icu::LocaleBuilder::LocaleBuilder()
    bld = icu.LocaleBuilder()

    # LocaleBuilder &icu::LocaleBuilder::addUnicodeLocaleAttribute(
    #       StringPiece attribute
    # )
    result = bld.add_unicode_locale_attribute("abc")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::removeUnicodeLocaleAttribute(
    #       StringPiece attribute
    # )
    result = bld.remove_unicode_locale_attribute("abc")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::setExtension(
    #       char key,
    #       StringPiece value
    # )
    result = bld.set_extension("U", "co-pinyin")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::setLanguage(StringPiece language)
    result = bld.set_language("zh")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::setRegion(StringPiece region)
    result = bld.set_region("HK")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::setScript(StringPiece script)
    result = bld.set_script("Hans")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::setUnicodeLocaleKeyword(
    #       StringPiece key,
    #       StringPiece type
    # )
    result = bld.set_unicode_locale_keyword("nu", "latn")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # LocaleBuilder &icu::LocaleBuilder::setVariant(StringPiece variant)
    result = bld.set_variant("revised")
    assert isinstance(result, icu.LocaleBuilder)
    assert id(result) == id(bld)

    # Locale icu::LocaleBuilder::build(UErrorCode &status)
    loc = (
        bld.set_language("zh")
        .set_script("Hant")
        .set_region("SG")
        .set_region("HK")
        .set_script("Hans")
        .set_variant("revised")
        .set_unicode_locale_keyword("nu", "thai")
        .set_extension("U", "co-pinyin")
        .set_unicode_locale_keyword("nu", "latn")
        .build()
    )
    assert isinstance(loc, icu.Locale)
    assert loc.to_language_tag() == "zh-Hans-HK-revised-u-co-pinyin-nu-latn"

    loc = (
        bld.set_unicode_locale_keyword("nu", None)
        .set_unicode_locale_keyword("co", None)
        .set_script("")
        .set_variant("")
        .set_region("")
        .build()
    )
    assert loc.to_language_tag() == "zh"

    bld = icu.LocaleBuilder()
    loc = (
        bld.set_language("fr")
        .add_unicode_locale_attribute("abc")
        .add_unicode_locale_attribute("aBc")
        .add_unicode_locale_attribute("EFG")
        .add_unicode_locale_attribute("efghi")
        .add_unicode_locale_attribute("efgh")
        .add_unicode_locale_attribute("efGhi")
        .add_unicode_locale_attribute("EFg")
        .add_unicode_locale_attribute("hijk")
        .add_unicode_locale_attribute("EFG")
        .add_unicode_locale_attribute("HiJK")
        .add_unicode_locale_attribute("aBc")
        .build()
    )
    assert loc.to_language_tag() == "fr-u-abc-efg-efgh-efghi-hijk"

    # LocaleBuilder &icu::LocaleBuilder::setLanguageTag(StringPiece tag)
    loc = bld.set_language_tag("de-DE-u-co-phonebk").build()
    assert loc.to_language_tag() == "de-DE-u-co-phonebk"

    # LocaleBuilder &icu::LocaleBuilder::setLocale(const Locale &locale)
    loc2 = icu.LocaleBuilder().set_locale(loc).build()
    assert loc2 == loc

    loc2 = icu.LocaleBuilder().set_locale("de_DE@collation=phonebook").build()
    assert loc2 == loc

    # LocaleBuilder &icu::LocaleBuilder::clearExtensions()
    loc = bld.clear_extensions().build()
    assert loc.to_language_tag() == "de-DE"

    # LocaleBuilder &icu::LocaleBuilder::clear()
    loc = bld.clear().build()
    assert loc == icu.Locale("")


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 65, reason="ICU4C<65")
def test_copy_error_to() -> None:
    bld = icu.LocaleBuilder()
    bld.set_language("en").build()

    # UBool icu::LocaleBuilder::copyErrorTo(UErrorCode &outErrorCode)
    out_error_code = icu.ErrorCode()
    assert bld.copy_error_to(out_error_code) is False
    assert out_error_code.get() == icu.UErrorCode.U_ZERO_ERROR

    with pytest.raises(icu.ICUError) as exc_info:
        bld.set_unicode_locale_keyword("123", "abc").build()
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR
    out_error_code = icu.ErrorCode()
    assert bld.copy_error_to(out_error_code) is True
    assert out_error_code.get() == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

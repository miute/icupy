import pytest
from icupy import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 64:
    pytest.skip("ICU4C<64", allow_module_level=True)
from icupy import ICUException, Locale, LocaleBuilder, UErrorCode


def test_api():
    bld = LocaleBuilder()
    loc = (bld
           .set_language("zh")
           .set_script("Hant")
           .set_region("SG")
           .set_region("HK")
           .set_script("Hans")
           .set_variant("revised")
           .set_unicode_locale_keyword("nu", "thai")
           .set_extension("U", "co-pinyin")
           .set_unicode_locale_keyword("nu", "latn")
           .build())
    assert isinstance(loc, Locale)
    assert loc.to_language_tag() == "zh-Hans-HK-revised-u-co-pinyin-nu-latn"
    loc = (bld
           .set_unicode_locale_keyword("nu", None)
           .set_unicode_locale_keyword("co", None)
           .set_script("")
           .set_variant("")
           .set_region("")
           .build())
    assert loc.to_language_tag() == "zh"

    bld = LocaleBuilder()
    loc = (bld
           .set_language("fr")
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
           .build())
    assert loc.to_language_tag() == "fr-u-abc-efg-efgh-efghi-hijk"

    loc = bld.remove_unicode_locale_attribute("eFgH").build()
    assert loc.to_language_tag() == "fr-u-abc-efg-efghi-hijk"

    loc = bld.set_language_tag("de-DE-u-co-phonebk").build()
    assert loc.to_language_tag() == "de-DE-u-co-phonebk"

    loc2 = LocaleBuilder().set_locale(loc).build()
    assert loc2 == loc

    loc2 = LocaleBuilder().set_locale("de_DE@collation=phonebook").build()
    assert loc2 == loc

    loc = bld.clear_extensions().build()
    assert loc.to_language_tag() == "de-DE"

    loc = bld.clear().build()
    assert loc == Locale("")


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 65, reason="ICU4C<65")
def test_copy_error_to():
    bld = LocaleBuilder()
    bld.set_language("en").build()

    # UBool LocaleBuilder::copyErrorTo(UErrorCode &outErrorCode)
    result, out_error_code = bld.copy_error_to()
    assert ((result, out_error_code)
            == (False, UErrorCode.U_ZERO_ERROR))

    with pytest.raises(ICUException) as exc_info:
        bld.set_unicode_locale_keyword("123", "abc").build()
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR
    result, out_error_code = bld.copy_error_to()
    assert ((result, out_error_code)
            == (True, UErrorCode.U_ILLEGAL_ARGUMENT_ERROR))

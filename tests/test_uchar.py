import pytest
from icupy.icu import (
    ConstVoidPtr,
    ICUError, UBlockCode, UCharCategory, UCharDirection, UCharNameChoice,
    UErrorCode, UDecompositionType, UEastAsianWidth,
    UnicodeSet, UProperty, UPropertyNameChoice, U_FOLD_CASE_DEFAULT,
    U_FOLD_CASE_EXCLUDE_SPECIAL_I, U_ICU_VERSION_MAJOR_NUM, U_NO_NUMERIC_VALUE,
    u_char_age, u_char_digit_value, u_char_direction, u_char_from_name,
    u_char_mirror, u_char_name, u_char_type, u_digit, u_fold_case,
    u_for_digit, u_get_bidi_paired_bracket,
    u_get_combining_class, u_get_fc_nfkc_closure,
    u_get_int_property_max_value, u_get_int_property_min_value,
    u_get_int_property_value, u_get_numeric_value, u_get_property_enum,
    u_get_property_name, u_get_property_value_enum,
    u_get_property_value_name, u_get_unicode_version, u_has_binary_property,
    u_isalnum, u_isalpha, u_isbase, u_isblank, u_iscntrl, u_isdefined,
    u_isdigit, u_isgraph, u_is_id_ignorable, u_is_id_part, u_is_id_start,
    u_is_iso_control, u_is_java_id_part, u_is_java_id_start,
    u_is_java_space_char, u_islower, u_is_mirrored, u_isprint, u_ispunct,
    u_isspace, u_istitle, u_is_ualphabetic, u_is_ulowercase, u_isupper,
    u_is_uuppercase, u_is_uwhite_space, u_is_whitespace, u_isxdigit,
    u_tolower, u_totitle, u_toupper, ublock_get_code,
)


def test_api():
    # U+FF11: Fullwidth Digit One
    assert u_char_digit_value(ord("1")) == 1
    assert u_char_digit_value(0xff11) == 1

    # U+0631: Arabic Letter Reh
    assert u_char_direction(ord("A")) == UCharDirection.U_LEFT_TO_RIGHT
    assert u_char_direction(0x0631) == UCharDirection.U_RIGHT_TO_LEFT_ARABIC

    # U+1F338: Cherry Blossom
    assert u_char_from_name(UCharNameChoice.U_UNICODE_CHAR_NAME,
                            "Cherry Blossom") == 0x1f338
    assert u_char_from_name(UCharNameChoice.U_EXTENDED_CHAR_NAME,
                            "<Other Symbol-1f338>") == 0x1f338
    with pytest.raises(ICUError) as exc_info:
        _ = u_char_from_name(UCharNameChoice.U_EXTENDED_CHAR_NAME,
                             "<So-1f338>")
    assert exc_info.value.args[0] == UErrorCode.U_ILLEGAL_CHAR_FOUND

    # U+300C: Left Corner Bracket
    # U+300D: Right Corner Bracket
    assert u_char_mirror(ord("(")) == ord(")")
    assert u_char_mirror(0x300d) == 0x300c

    assert u_char_name(
        0x1f338,
        UCharNameChoice.U_UNICODE_CHAR_NAME) == "CHERRY BLOSSOM"

    assert u_char_type(ord("0")) == UCharCategory.U_DECIMAL_DIGIT_NUMBER
    assert u_char_type(ord("(")) == UCharCategory.U_START_PUNCTUATION

    assert u_digit(ord("A"), 16) == 10
    assert u_digit(0xff11, 10) == 1

    # U+0131: Latin Small Letter Dotless I
    assert u_fold_case(ord('I'), U_FOLD_CASE_DEFAULT) == ord("i")
    assert u_fold_case(ord('I'), U_FOLD_CASE_EXCLUDE_SPECIAL_I) == 0x0131

    assert u_for_digit(10, 16) == ord("a")
    assert u_for_digit(10, 10) == 0

    assert u_get_bidi_paired_bracket(ord("[")) == ord("]")
    assert u_get_bidi_paired_bracket(0x300c) == 0x300d

    # Combining Class: Kana Voicing
    # U+3099: Combining Katakana-Hiragana Voiced Sound Mark
    # U+309A: Combining Katakana-Hiragana Semi-Voiced Sound Mark
    assert u_get_combining_class(0x3099) == u_get_combining_class(0x309a) == 8

    # U+2121: Telephone Sign
    assert u_get_fc_nfkc_closure(0x2121) == "tel"

    # UCHAR_IDEOGRAPHIC: 0/1 (false/true)
    assert u_get_int_property_max_value(UProperty.UCHAR_IDEOGRAPHIC) == 1
    assert u_get_int_property_min_value(UProperty.UCHAR_IDEOGRAPHIC) == 0

    assert (u_get_int_property_value(ord("1"),
                                     UProperty.UCHAR_EAST_ASIAN_WIDTH)
            == UEastAsianWidth.U_EA_NARROW)
    assert (u_get_int_property_value(0xff11,
                                     UProperty.UCHAR_EAST_ASIAN_WIDTH)
            == UEastAsianWidth.U_EA_FULLWIDTH)

    # U+5146: CJK Unified Ideograph-5146 (= 10^12)
    # U+4EAC: CJK Unified Ideograph-4EAC (= 10^16)
    assert u_get_numeric_value(ord("1")) == 1.0
    assert u_get_numeric_value(0x5146) == 1000000000000.0
    assert u_get_numeric_value(0x4eac) == U_NO_NUMERIC_VALUE

    assert u_get_property_enum("gcm") == UProperty.UCHAR_GENERAL_CATEGORY_MASK

    assert u_get_property_name(
        UProperty.UCHAR_GENERAL_CATEGORY_MASK,
        UPropertyNameChoice.U_LONG_PROPERTY_NAME) == "General_Category_Mask"

    assert u_get_property_value_enum(
        UProperty.UCHAR_DECOMPOSITION_TYPE,
        "Can") == UDecompositionType.U_DT_CANONICAL

    assert u_get_property_value_name(
        UProperty.UCHAR_DECOMPOSITION_TYPE,
        UDecompositionType.U_DT_CANONICAL,
        UPropertyNameChoice.U_LONG_PROPERTY_NAME) == "Canonical"

    assert u_has_binary_property(0x300c, UProperty.UCHAR_BIDI_MIRRORED)

    assert u_isalnum(ord("a"))
    assert u_isalpha(ord("a"))
    assert u_isbase(ord("a"))
    assert u_isblank(0x3000)  # U+3000: Ideographic Space
    assert u_iscntrl(0x0a)  # U+000A: <End of Line> (EOL, LF, NL)
    assert not u_isdefined(0xfdd0)  # U+FDD0: Undefined Character
    assert u_isdigit(0xff11)  # U+FF11: Fullwidth Digit One
    assert u_isgraph(0x300c)  # U+300C: Left Corner Bracket
    assert u_is_id_ignorable(0xad)  # U+00AD: Soft Hyphen (SHY)
    assert u_is_id_part(0x16ee)  # U+16EE: Runic Arlaug Symbol
    assert u_is_id_start(0x16ee)  # U+16EE: Runic Arlaug Symbol
    assert u_is_iso_control(0x00)  # U+0000: <Null> (NUL)
    assert u_is_java_id_part(ord("$"))  # U+0024: Dollar Sign
    assert u_is_java_id_start(ord("_"))  # U+005F: Low Line
    assert u_is_java_space_char(0x20)  # U+0020: Space (SP)
    assert u_islower(ord("a"))
    assert u_is_mirrored(0x300c)  # U+300C: Left Corner Bracket
    assert u_isprint(ord("a"))
    assert u_ispunct(ord("_"))
    assert u_isspace(0x3000)  # U+3000: Ideographic Space
    # U+01C5: Latin Capital Letter D with Small Letter Z with Caron
    assert u_istitle(0x01c5)
    assert u_is_ualphabetic(ord("A"))
    assert u_is_ulowercase(ord("a"))
    assert u_isupper(ord("A"))
    assert u_is_uuppercase(ord("A"))
    assert u_is_uwhite_space(0x3000)  # U+3000: Ideographic Space
    assert u_is_whitespace(0x3000)  # U+3000: Ideographic Space
    assert u_isxdigit(0xff21)  # U+FF21: Fullwidth Latin Capital Letter A

    # U+FF21: Fullwidth Latin Capital Letter A
    # U+FF41: Fullwidth Latin Small Letter A
    assert u_tolower(0xff21) == 0xff41

    # U+01C5: Latin Capital Letter D with Small Letter Z with Caron
    # U+01C6: Latin Small Letter Dz with Caron
    assert u_totitle(0x01c6) == 0x01c5

    # U+FF21: Fullwidth Latin Capital Letter A
    # U+FF41: Fullwidth Latin Small Letter A
    assert u_toupper(0xff41) == 0xff21

    # U+01C5: Latin Capital Letter D with Small Letter Z with Caron
    assert ublock_get_code(0x01c5) == UBlockCode.UBLOCK_LATIN_EXTENDED_B


def test_u_char_age():
    # void u_charAge(UChar32 c,
    #                UVersionInfo versionArray
    # )
    # U+1F338: Cherry Blossom
    version_array = u_char_age(0x1f338)
    assert isinstance(version_array, list)
    assert len(version_array) == 4
    assert version_array[0] == 6
    assert version_array[1] == 0
    assert version_array[2] == 0
    assert version_array[3] == 0


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_u_get_binary_property_set():
    from icupy.icu import u_get_binary_property_set

    # const USet *u_getBinaryPropertySet(UProperty property,
    #                                    UErrorCode *pErrorCode
    # )
    uset = u_get_binary_property_set(UProperty.UCHAR_POSIX_BLANK)
    uniset = UnicodeSet.from_uset(uset)
    assert uniset.size() > 0
    assert uniset.contains(0x20)  # U+0020: Space (SP)
    assert uniset.contains(0xa0)  # U+00A0: No-Break Space (NBSP)
    assert uniset.contains(0x3000)  # U+3000: Ideographic Space


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_u_get_int_property_map():
    from icupy.icu import (
        UCPMapRangeOption, UCPMapValueFilterPtr,
        u_get_int_property_map, ucpmap_get, ucpmap_get_range,
    )

    # UCPMap *u_getIntPropertyMap(UProperty property,
    #                             UErrorCode *pErrorCode
    # )
    ucpmap = u_get_int_property_map(UProperty.UCHAR_EAST_ASIAN_WIDTH)

    # U+FF11: Fullwidth Digit One
    assert ucpmap_get(ucpmap, ord("1")) == UEastAsianWidth.U_EA_NARROW
    assert ucpmap_get(ucpmap, 0xff11) == UEastAsianWidth.U_EA_FULLWIDTH

    # uint32_t UCPMapValueFilter(const void *context,
    #                            uint32_t value)
    def _filter1(_context: object, _value: int) -> int:
        assert _context is None
        return _value | 0x1000

    def _filter2(_context: object, _value: int) -> int:
        assert isinstance(_context, dict)
        return _value | _context["key"]

    # UChar32 ucpmap_getRange(const UCPMap *map,
    #                         UChar32 start,
    #                         UCPMapRangeOption option,
    #                         uint32_t surrogateValue,
    #                         UCPMapValueFilter *filter,
    #                         const void *context,
    #                         uint32_t *pValue
    # )
    # Without filter
    result, value = ucpmap_get_range(ucpmap,
                                     0xff11,
                                     UCPMapRangeOption.UCPMAP_RANGE_NORMAL,
                                     0,
                                     UCPMapValueFilterPtr(None),
                                     ConstVoidPtr(None))
    assert result != -1
    assert value == UEastAsianWidth.U_EA_FULLWIDTH

    # With filter
    result, value = ucpmap_get_range(ucpmap,
                                     0xff11,
                                     UCPMapRangeOption.UCPMAP_RANGE_NORMAL,
                                     0,
                                     UCPMapValueFilterPtr(_filter1),
                                     ConstVoidPtr(None))
    assert result != -1
    assert value == UEastAsianWidth.U_EA_FULLWIDTH | 0x1000

    # With filter and context
    context = {"key": 0x2000}
    result, value = ucpmap_get_range(ucpmap,
                                     0xff11,
                                     UCPMapRangeOption.UCPMAP_RANGE_NORMAL,
                                     0,
                                     UCPMapValueFilterPtr(_filter2),
                                     ConstVoidPtr(context))
    assert result != -1
    assert value == UEastAsianWidth.U_EA_FULLWIDTH | 0x2000


def test_u_get_unicode_version():
    # void u_getUnicodeVersion(UVersionInfo versionArray)
    version_array = u_get_unicode_version()
    assert isinstance(version_array, list)
    assert len(version_array) == 4
    assert version_array[0] > 0
    assert version_array[1] >= 0
    assert version_array[2] >= 0
    assert version_array[3] >= 0

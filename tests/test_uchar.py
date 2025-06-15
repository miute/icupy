import pytest

from icupy import icu


def test_api() -> None:
    # U_GET_GC_MASK(c)
    # U_MASK(x)
    assert icu.u_get_gc_mask(0x41) == icu.U_GC_LU_MASK == icu.u_mask(icu.u_char_type(0x41))
    assert icu.u_get_gc_mask(0x662) == icu.U_GC_ND_MASK == icu.u_mask(icu.u_char_type(0x662))
    assert icu.u_get_gc_mask(0xA0) == icu.U_GC_ZS_MASK == icu.u_mask(icu.u_char_type(0xA0))
    assert icu.u_get_gc_mask(0x28) == icu.U_GC_PS_MASK == icu.u_mask(icu.u_char_type(0x28))
    assert icu.u_get_gc_mask(0x2044) == icu.U_GC_SM_MASK == icu.u_mask(icu.u_char_type(0x2044))
    assert (
        icu.u_get_gc_mask(0xE0063) == icu.U_GC_CF_MASK == icu.u_mask(icu.u_char_type(0xE0063))
    )

    # U+FF11: Fullwidth Digit One
    assert icu.u_char_digit_value(ord("1")) == 1
    assert icu.u_char_digit_value(0xFF11) == 1

    # U+0631: Arabic Letter Reh
    assert icu.u_char_direction(ord("A")) == icu.UCharDirection.U_LEFT_TO_RIGHT
    assert icu.u_char_direction(0x0631) == icu.UCharDirection.U_RIGHT_TO_LEFT_ARABIC

    # U+1F338: Cherry Blossom
    assert (
        icu.u_char_from_name(icu.UCharNameChoice.U_UNICODE_CHAR_NAME, "Cherry Blossom")
        == 0x1F338
    )
    assert (
        icu.u_char_from_name(icu.UCharNameChoice.U_EXTENDED_CHAR_NAME, "<Other Symbol-1f338>")
        == 0x1F338
    )
    with pytest.raises(icu.ICUError) as exc_info:
        _ = icu.u_char_from_name(icu.UCharNameChoice.U_EXTENDED_CHAR_NAME, "<So-1f338>")
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_CHAR_FOUND

    # U+300C: Left Corner Bracket
    # U+300D: Right Corner Bracket
    assert icu.u_char_mirror(ord("(")) == ord(")")
    assert icu.u_char_mirror(0x300D) == 0x300C

    assert icu.u_char_name(0x1F338, icu.UCharNameChoice.U_UNICODE_CHAR_NAME) == "CHERRY BLOSSOM"

    assert icu.u_char_type(ord("0")) == icu.UCharCategory.U_DECIMAL_DIGIT_NUMBER
    assert icu.u_char_type(ord("(")) == icu.UCharCategory.U_START_PUNCTUATION

    assert icu.u_digit(ord("A"), 16) == 10
    assert icu.u_digit(0xFF11, 10) == 1

    # U+0131: Latin Small Letter Dotless I
    assert icu.u_fold_case(ord("I"), icu.U_FOLD_CASE_DEFAULT) == ord("i")
    assert icu.u_fold_case(ord("I"), icu.U_FOLD_CASE_EXCLUDE_SPECIAL_I) == 0x0131

    assert icu.u_for_digit(10, 16) == ord("a")
    assert icu.u_for_digit(10, 10) == 0

    assert icu.u_get_bidi_paired_bracket(ord("[")) == ord("]")
    assert icu.u_get_bidi_paired_bracket(0x300C) == 0x300D

    # Combining Class: Kana Voicing
    # U+3099: Combining Katakana-Hiragana Voiced Sound Mark
    # U+309A: Combining Katakana-Hiragana Semi-Voiced Sound Mark
    assert icu.u_get_combining_class(0x3099) == icu.u_get_combining_class(0x309A) == 8

    # U+2121: Telephone Sign
    assert icu.u_get_fc_nfkc_closure(0x2121) == "tel"

    # UCHAR_IDEOGRAPHIC: 0/1 (false/true)
    assert icu.u_get_int_property_max_value(icu.UProperty.UCHAR_IDEOGRAPHIC) == 1
    assert icu.u_get_int_property_min_value(icu.UProperty.UCHAR_IDEOGRAPHIC) == 0

    assert (
        icu.u_get_int_property_value(ord("1"), icu.UProperty.UCHAR_EAST_ASIAN_WIDTH)
        == icu.UEastAsianWidth.U_EA_NARROW
    )
    assert (
        icu.u_get_int_property_value(0xFF11, icu.UProperty.UCHAR_EAST_ASIAN_WIDTH)
        == icu.UEastAsianWidth.U_EA_FULLWIDTH
    )

    # U+5146: CJK Unified Ideograph-5146 (= 10^12)
    # U+4EAC: CJK Unified Ideograph-4EAC (= 10^16)
    assert icu.u_get_numeric_value(ord("1")) == 1.0
    assert icu.u_get_numeric_value(0x5146) in (
        10**12,
        1000000.0,  # ICU4C==74rc
    )
    assert icu.u_get_numeric_value(0x4EAC) in (
        icu.U_NO_NUMERIC_VALUE,  # ICU4C<74
        10**16,  # ICU4C>=74
    )

    assert icu.u_get_property_enum("gcm") == icu.UProperty.UCHAR_GENERAL_CATEGORY_MASK

    assert (
        icu.u_get_property_name(
            icu.UProperty.UCHAR_GENERAL_CATEGORY_MASK,
            icu.UPropertyNameChoice.U_LONG_PROPERTY_NAME,
        )
        == "General_Category_Mask"
    )

    assert (
        icu.u_get_property_value_enum(icu.UProperty.UCHAR_DECOMPOSITION_TYPE, "Can")
        == icu.UDecompositionType.U_DT_CANONICAL
    )

    assert (
        icu.u_get_property_value_name(
            icu.UProperty.UCHAR_DECOMPOSITION_TYPE,
            icu.UDecompositionType.U_DT_CANONICAL,
            icu.UPropertyNameChoice.U_LONG_PROPERTY_NAME,
        )
        == "Canonical"
    )

    assert icu.u_has_binary_property(0x300C, icu.UProperty.UCHAR_BIDI_MIRRORED) is True

    assert icu.u_isalnum(ord("a")) is True
    assert icu.u_isalpha(ord("a")) is True
    assert icu.u_isbase(ord("a")) is True
    assert icu.u_isblank(0x3000) is True  # U+3000: Ideographic Space
    assert icu.u_iscntrl(0x0A) is True  # U+000A: <End of Line> (EOL, LF, NL)
    assert icu.u_isdefined(0xFDD0) is False  # U+FDD0: Undefined Character
    assert icu.u_isdigit(0xFF11) is True  # U+FF11: Fullwidth Digit One
    assert icu.u_isgraph(0x300C) is True  # U+300C: Left Corner Bracket
    assert icu.u_is_id_ignorable(0xAD) is True  # U+00AD: Soft Hyphen (SHY)
    assert icu.u_is_id_part(0x16EE) is True  # U+16EE: Runic Arlaug Symbol
    assert icu.u_is_id_start(0x16EE) is True  # U+16EE: Runic Arlaug Symbol
    assert icu.u_is_iso_control(0x00) is True  # U+0000: <Null> (NUL)
    assert icu.u_is_java_id_part(ord("$")) is True  # U+0024: Dollar Sign
    assert icu.u_is_java_id_start(ord("_")) is True  # U+005F: Low Line
    assert icu.u_is_java_space_char(0x20) is True  # U+0020: Space (SP)
    assert icu.u_islower(ord("a")) is True
    assert icu.u_is_mirrored(0x300C) is True  # U+300C: Left Corner Bracket
    assert icu.u_isprint(ord("a")) is True
    assert icu.u_ispunct(ord("_")) is True
    assert icu.u_isspace(0x3000) is True  # U+3000: Ideographic Space
    # U+01C5: Latin Capital Letter D with Small Letter Z with Caron
    assert icu.u_istitle(0x01C5) is True
    assert icu.u_is_ualphabetic(ord("A")) is True
    assert icu.u_is_ulowercase(ord("a")) is True
    assert icu.u_isupper(ord("A")) is True
    assert icu.u_is_uuppercase(ord("A")) is True
    assert icu.u_is_uwhite_space(0x3000) is True  # U+3000: Ideographic Space
    assert icu.u_is_whitespace(0x3000) is True  # U+3000: Ideographic Space
    assert icu.u_isxdigit(0xFF21) is True  # U+FF21: Fullwidth Latin Capital Letter A

    # U+FF21: Fullwidth Latin Capital Letter A
    # U+FF41: Fullwidth Latin Small Letter A
    assert icu.u_tolower(0xFF21) == 0xFF41

    # U+01C5: Latin Capital Letter D with Small Letter Z with Caron
    # U+01C6: Latin Small Letter Dz with Caron
    assert icu.u_totitle(0x01C6) == 0x01C5

    # U+FF21: Fullwidth Latin Capital Letter A
    # U+FF41: Fullwidth Latin Small Letter A
    assert icu.u_toupper(0xFF41) == 0xFF21

    # U+01C5: Latin Capital Letter D with Small Letter Z with Caron
    assert icu.ublock_get_code(0x01C5) == icu.UBlockCode.UBLOCK_LATIN_EXTENDED_B


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 70, reason="ICU4C<70")
def test_api_70() -> None:
    # UBool u_stringHasBinaryProperty(
    #       const UChar *s,
    #       int32_t length,
    #       UProperty which
    # )
    assert icu.u_string_has_binary_property("\U0001f338", icu.UCHAR_BASIC_EMOJI) is True
    assert icu.u_string_has_binary_property("\uff11", icu.UCHAR_BASIC_EMOJI) is False


def test_u_char_age() -> None:
    # void u_charAge(UChar32 c,
    #                UVersionInfo versionArray
    # )
    # U+1F338: Cherry Blossom
    version_array = icu.u_char_age(0x1F338)
    assert isinstance(version_array, tuple)
    assert len(version_array) == 4
    assert version_array[0] == 6
    assert version_array[1] == 0
    assert version_array[2] == 0
    assert version_array[3] == 0


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_u_get_binary_property_set() -> None:
    # const USet *u_getBinaryPropertySet(UProperty property,
    #                                    UErrorCode *pErrorCode
    # )
    uset = icu.u_get_binary_property_set(icu.UProperty.UCHAR_POSIX_BLANK)
    uniset = icu.UnicodeSet.from_uset(uset)
    assert uniset.size() > 0
    assert uniset.contains(0x20)  # U+0020: Space (SP)
    assert uniset.contains(0xA0)  # U+00A0: No-Break Space (NBSP)
    assert uniset.contains(0x3000)  # U+3000: Ideographic Space


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 75, reason="ICU4C<75")
def test_u_get_id_types() -> None:
    c = 0x1D1DE

    # int32_t u_getIDTypes (
    #       UChar32 c,
    #       UIdentifierType *types,
    #       int32_t capacity,
    #       UErrorCode *pErrorCode
    # )
    types = icu.u_get_id_types(c)
    assert isinstance(types, list)
    assert len(types) == 3
    types.sort()
    assert types == [
        icu.UIdentifierType.U_ID_TYPE_NOT_XID,
        icu.UIdentifierType.U_ID_TYPE_TECHNICAL,
        icu.UIdentifierType.U_ID_TYPE_UNCOMMON_USE,
    ]

    # bool u_hasIDType(
    #       UChar32 c,
    #       UIdentifierType type
    # )
    assert icu.u_has_id_type(c, icu.UIdentifierType.U_ID_TYPE_NOT_CHARACTER) is False
    assert icu.u_has_id_type(c, icu.UIdentifierType.U_ID_TYPE_NOT_XID) is True
    assert icu.u_has_id_type(c, icu.UIdentifierType.U_ID_TYPE_TECHNICAL) is True
    assert icu.u_has_id_type(c, icu.UIdentifierType.U_ID_TYPE_UNCOMMON_USE) is True


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 63, reason="ICU4C<63")
def test_u_get_int_property_map() -> None:
    # UCPMap *u_getIntPropertyMap(UProperty property,
    #                             UErrorCode *pErrorCode
    # )
    ucpmap = icu.u_get_int_property_map(icu.UProperty.UCHAR_EAST_ASIAN_WIDTH)

    # U+FF11: Fullwidth Digit One
    assert icu.ucpmap_get(ucpmap, ord("1")) == icu.UEastAsianWidth.U_EA_NARROW
    assert icu.ucpmap_get(ucpmap, 0xFF11) == icu.UEastAsianWidth.U_EA_FULLWIDTH

    # uint32_t UCPMapValueFilter(const void *context,
    #                            uint32_t value)
    def _filter1(_context: object, _value: int) -> int:
        assert _context is None
        return _value | 0x1000

    def _filter2(_context: object, _value: int) -> int:
        assert isinstance(_context, dict)
        assert isinstance(_context["key"], int)
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
    result, value = icu.ucpmap_get_range(
        ucpmap,
        0xFF11,
        icu.UCPMapRangeOption.UCPMAP_RANGE_NORMAL,
        0,
        icu.UCPMapValueFilterPtr(None),
        icu.ConstVoidPtr(None),
    )
    assert result != -1
    assert value == icu.UEastAsianWidth.U_EA_FULLWIDTH

    # With filter
    result, value = icu.ucpmap_get_range(
        ucpmap,
        0xFF11,
        icu.UCPMapRangeOption.UCPMAP_RANGE_NORMAL,
        0,
        icu.UCPMapValueFilterPtr(_filter1),
        icu.ConstVoidPtr(None),
    )
    assert result != -1
    assert value == icu.UEastAsianWidth.U_EA_FULLWIDTH | 0x1000

    # With filter and context
    context = {"key": 0x2000}
    result, value = icu.ucpmap_get_range(
        ucpmap,
        0xFF11,
        icu.UCPMapRangeOption.UCPMAP_RANGE_NORMAL,
        0,
        icu.UCPMapValueFilterPtr(_filter2),
        icu.ConstVoidPtr(context),
    )
    assert result != -1
    assert value == icu.UEastAsianWidth.U_EA_FULLWIDTH | 0x2000


def test_u_get_unicode_version() -> None:
    # void u_getUnicodeVersion(UVersionInfo versionArray)
    version_array = icu.u_get_unicode_version()
    assert isinstance(version_array, tuple)
    assert len(version_array) == 4
    assert version_array[0] > 0
    assert version_array[1] >= 0
    assert version_array[2] >= 0
    assert version_array[3] >= 0

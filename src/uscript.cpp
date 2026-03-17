#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/uscript.h>

using namespace icu;

void init_uscript(py::module &m) {
  //
  // enum UScriptCode
  //
  py::enum_<UScriptCode>(m, "UScriptCode", py::arithmetic(), R"doc(
Constants for ISO 15924 script codes.

The current set of script code constants supports at least all scripts that are
encoded in the version of Unicode which ICU currently supports. The names of
the constants are usually derived from the Unicode script property value
aliases. See UAX #24 Unicode Script Property
(http://www.unicode.org/reports/tr24/) and
http://www.unicode.org/Public/UCD/latest/ucd/PropertyValueAliases.txt .

In addition, constants for many ISO 15924 script codes are included, for use
with language tags, CLDR data, and similar. Some of those codes are not used
in the Unicode Character Database (UCD). For example, there are no characters
that have a UCD script property value of Hans or Hant. All Han ideographs have
the Hani script property value in Unicode.

Private-use codes Qaaa..Qabx are not included, except as used in the UCD or in
CLDR.

Starting with ICU 55, script codes are only added when their scripts have been
or will certainly be encoded in Unicode, and have been assigned Unicode script
property value aliases, to ensure that their script names are stable and match
the names of the constants. Script codes like Latf and Aran that are not
subject to separate encoding may be added at any time.
      )doc")
      .value("USCRIPT_INVALID_CODE", USCRIPT_INVALID_CODE)
      .value("USCRIPT_COMMON", USCRIPT_COMMON, R"doc(
             Zyyy
             )doc")
      .value("USCRIPT_INHERITED", USCRIPT_INHERITED, R"doc(
             Zinh
             )doc")
      .value("USCRIPT_ARABIC", USCRIPT_ARABIC, R"doc(
             Arab
             )doc")
      .value("USCRIPT_ARMENIAN", USCRIPT_ARMENIAN, R"doc(
             Armn
             )doc")
      .value("USCRIPT_BENGALI", USCRIPT_BENGALI, R"doc(
             Beng
             )doc")
      .value("USCRIPT_BOPOMOFO", USCRIPT_BOPOMOFO, R"doc(
             Bopo
             )doc")
      .value("USCRIPT_CHEROKEE", USCRIPT_CHEROKEE, R"doc(
             Cher
             )doc")
      .value("USCRIPT_COPTIC", USCRIPT_COPTIC, R"doc(
             Copt
             )doc")
      .value("USCRIPT_CYRILLIC", USCRIPT_CYRILLIC, R"doc(
             Cyrl
             )doc")
      .value("USCRIPT_DESERET", USCRIPT_DESERET, R"doc(
             Dsrt
             )doc")
      .value("USCRIPT_DEVANAGARI", USCRIPT_DEVANAGARI, R"doc(
             Deva
             )doc")
      .value("USCRIPT_ETHIOPIC", USCRIPT_ETHIOPIC, R"doc(
             Ethi
             )doc")
      .value("USCRIPT_GEORGIAN", USCRIPT_GEORGIAN, R"doc(
             Geor
             )doc")
      .value("USCRIPT_GOTHIC", USCRIPT_GOTHIC, R"doc(
             Goth
             )doc")
      .value("USCRIPT_GREEK", USCRIPT_GREEK, R"doc(
             Grek
             )doc")
      .value("USCRIPT_GUJARATI", USCRIPT_GUJARATI, R"doc(
             Gujr
             )doc")
      .value("USCRIPT_GURMUKHI", USCRIPT_GURMUKHI, R"doc(
             Guru
             )doc")
      .value("USCRIPT_HAN", USCRIPT_HAN, R"doc(
             Hani
             )doc")
      .value("USCRIPT_HANGUL", USCRIPT_HANGUL, R"doc(
             Hang
             )doc")
      .value("USCRIPT_HEBREW", USCRIPT_HEBREW, R"doc(
             Hebr
             )doc")
      .value("USCRIPT_HIRAGANA", USCRIPT_HIRAGANA, R"doc(
             Hira
             )doc")
      .value("USCRIPT_KANNADA", USCRIPT_KANNADA, R"doc(
             Knda
             )doc")
      .value("USCRIPT_KATAKANA", USCRIPT_KATAKANA, R"doc(
             Kana
             )doc")
      .value("USCRIPT_KHMER", USCRIPT_KHMER, R"doc(
             Khmr
             )doc")
      .value("USCRIPT_LAO", USCRIPT_LAO, R"doc(
             Laoo
             )doc")
      .value("USCRIPT_LATIN", USCRIPT_LATIN, R"doc(
             Latn
             )doc")
      .value("USCRIPT_MALAYALAM", USCRIPT_MALAYALAM, R"doc(
             Mlym
             )doc")
      .value("USCRIPT_MONGOLIAN", USCRIPT_MONGOLIAN, R"doc(
             Mong
             )doc")
      .value("USCRIPT_MYANMAR", USCRIPT_MYANMAR, R"doc(
             Mymr
             )doc")
      .value("USCRIPT_OGHAM", USCRIPT_OGHAM, R"doc(
             Ogam
             )doc")
      .value("USCRIPT_OLD_ITALIC", USCRIPT_OLD_ITALIC, R"doc(
             Ital
             )doc")
      .value("USCRIPT_ORIYA", USCRIPT_ORIYA, R"doc(
             Orya
             )doc")
      .value("USCRIPT_RUNIC", USCRIPT_RUNIC, R"doc(
             Runr
             )doc")
      .value("USCRIPT_SINHALA", USCRIPT_SINHALA, R"doc(
             Sinh
             )doc")
      .value("USCRIPT_SYRIAC", USCRIPT_SYRIAC, R"doc(
             Syrc
             )doc")
      .value("USCRIPT_TAMIL", USCRIPT_TAMIL, R"doc(
             Taml
             )doc")
      .value("USCRIPT_TELUGU", USCRIPT_TELUGU, R"doc(
             Telu
             )doc")
      .value("USCRIPT_THAANA", USCRIPT_THAANA, R"doc(
             Thaa
             )doc")
      .value("USCRIPT_THAI", USCRIPT_THAI, R"doc(
             Thai
             )doc")
      .value("USCRIPT_TIBETAN", USCRIPT_TIBETAN, R"doc(
             Tibt
             )doc")
      .value("USCRIPT_CANADIAN_ABORIGINAL", USCRIPT_CANADIAN_ABORIGINAL, R"doc(
             Cans; Canadian_Aboriginal script.
             )doc")
      .value("USCRIPT_UCAS", USCRIPT_UCAS, R"doc(
             Cans (alias)
             )doc")
      .value("USCRIPT_YI", USCRIPT_YI, R"doc(
             Yiii
             )doc")
      .value("USCRIPT_TAGALOG", USCRIPT_TAGALOG, R"doc(
             Tglg
             )doc")
      .value("USCRIPT_HANUNOO", USCRIPT_HANUNOO, R"doc(
             Hano
             )doc")
      .value("USCRIPT_BUHID", USCRIPT_BUHID, R"doc(
             Buhd
             )doc")
      .value("USCRIPT_TAGBANWA", USCRIPT_TAGBANWA, R"doc(
             Tagb
             )doc")
      .value("USCRIPT_BRAILLE", USCRIPT_BRAILLE, R"doc(
             Brai
             )doc")
      .value("USCRIPT_CYPRIOT", USCRIPT_CYPRIOT, R"doc(
             Cprt
             )doc")
      .value("USCRIPT_LIMBU", USCRIPT_LIMBU, R"doc(
             Limb
             )doc")
      .value("USCRIPT_LINEAR_B", USCRIPT_LINEAR_B, R"doc(
             Linb
             )doc")
      .value("USCRIPT_OSMANYA", USCRIPT_OSMANYA, R"doc(
             Osma
             )doc")
      .value("USCRIPT_SHAVIAN", USCRIPT_SHAVIAN, R"doc(
             Shaw
             )doc")
      .value("USCRIPT_TAI_LE", USCRIPT_TAI_LE, R"doc(
             Tale
             )doc")
      .value("USCRIPT_UGARITIC", USCRIPT_UGARITIC, R"doc(
             Ugar
             )doc")
      .value("USCRIPT_KATAKANA_OR_HIRAGANA", USCRIPT_KATAKANA_OR_HIRAGANA,
             R"doc(
             Hrkt
             )doc")
      .value("USCRIPT_BUGINESE", USCRIPT_BUGINESE, R"doc(
             Bugi
             )doc")
      .value("USCRIPT_GLAGOLITIC", USCRIPT_GLAGOLITIC, R"doc(
             Glag
             )doc")
      .value("USCRIPT_KHAROSHTHI", USCRIPT_KHAROSHTHI, R"doc(
             Khar
             )doc")
      .value("USCRIPT_SYLOTI_NAGRI", USCRIPT_SYLOTI_NAGRI, R"doc(
             Sylo
             )doc")
      .value("USCRIPT_NEW_TAI_LUE", USCRIPT_NEW_TAI_LUE, R"doc(
             Talu
             )doc")
      .value("USCRIPT_TIFINAGH", USCRIPT_TIFINAGH, R"doc(
             Tfng
             )doc")
      .value("USCRIPT_OLD_PERSIAN", USCRIPT_OLD_PERSIAN, R"doc(
             Xpeo
             )doc")
      .value("USCRIPT_BALINESE", USCRIPT_BALINESE, R"doc(
             Bali
             )doc")
      .value("USCRIPT_BATAK", USCRIPT_BATAK, R"doc(
             Batk
             )doc")
      .value("USCRIPT_BLISSYMBOLS", USCRIPT_BLISSYMBOLS, R"doc(
             Blis
             )doc")
      .value("USCRIPT_BRAHMI", USCRIPT_BRAHMI, R"doc(
             Brah
             )doc")
      .value("USCRIPT_CHAM", USCRIPT_CHAM, R"doc(
             Cham
             )doc")
      .value("USCRIPT_CIRTH", USCRIPT_CIRTH, R"doc(
             Cirt
             )doc")
      .value("USCRIPT_OLD_CHURCH_SLAVONIC_CYRILLIC",
             USCRIPT_OLD_CHURCH_SLAVONIC_CYRILLIC, R"doc(
             Cyrs
             )doc")
      .value("USCRIPT_DEMOTIC_EGYPTIAN", USCRIPT_DEMOTIC_EGYPTIAN, R"doc(
             Egyd
             )doc")
      .value("USCRIPT_HIERATIC_EGYPTIAN", USCRIPT_HIERATIC_EGYPTIAN, R"doc(
             Egyh
             )doc")
      .value("USCRIPT_EGYPTIAN_HIEROGLYPHS", USCRIPT_EGYPTIAN_HIEROGLYPHS,
             R"doc(
             Egyp
             )doc")
      .value("USCRIPT_KHUTSURI", USCRIPT_KHUTSURI, R"doc(
             Geok
             )doc")
      .value("USCRIPT_SIMPLIFIED_HAN", USCRIPT_SIMPLIFIED_HAN, R"doc(
             Hans
             )doc")
      .value("USCRIPT_TRADITIONAL_HAN", USCRIPT_TRADITIONAL_HAN, R"doc(
             Hant
             )doc")
      .value("USCRIPT_PAHAWH_HMONG", USCRIPT_PAHAWH_HMONG, R"doc(
             Hmng
             )doc")
      .value("USCRIPT_OLD_HUNGARIAN", USCRIPT_OLD_HUNGARIAN, R"doc(
             Hung
             )doc")
      .value("USCRIPT_HARAPPAN_INDUS", USCRIPT_HARAPPAN_INDUS, R"doc(
             Inds
             )doc")
      .value("USCRIPT_JAVANESE", USCRIPT_JAVANESE, R"doc(
             Java
             )doc")
      .value("USCRIPT_KAYAH_LI", USCRIPT_KAYAH_LI, R"doc(
             Kali
             )doc")
      .value("USCRIPT_LATIN_FRAKTUR", USCRIPT_LATIN_FRAKTUR, R"doc(
             Latf
             )doc")
      .value("USCRIPT_LATIN_GAELIC", USCRIPT_LATIN_GAELIC, R"doc(
             Latg
             )doc")
      .value("USCRIPT_LEPCHA", USCRIPT_LEPCHA, R"doc(
             Lepc
             )doc")
      .value("USCRIPT_LINEAR_A", USCRIPT_LINEAR_A, R"doc(
             Lina
             )doc")
      .value("USCRIPT_MANDAIC", USCRIPT_MANDAIC, R"doc(
             Mand
             )doc")
      .value("USCRIPT_MANDAEAN", USCRIPT_MANDAEAN, R"doc(
             Mand (alias)
             )doc")
      .value("USCRIPT_MAYAN_HIEROGLYPHS", USCRIPT_MAYAN_HIEROGLYPHS, R"doc(
             Maya
             )doc")
      .value("USCRIPT_MEROITIC_HIEROGLYPHS", USCRIPT_MEROITIC_HIEROGLYPHS,
             R"doc(
             Mero
             )doc")
      .value("USCRIPT_MEROITIC", USCRIPT_MEROITIC, R"doc(
             Mero (alias)
             )doc")
      .value("USCRIPT_NKO", USCRIPT_NKO, R"doc(
             Nkoo
             )doc")
      .value("USCRIPT_ORKHON", USCRIPT_ORKHON, R"doc(
             Orkh
             )doc")
      .value("USCRIPT_OLD_PERMIC", USCRIPT_OLD_PERMIC, R"doc(
             Perm
             )doc")
      .value("USCRIPT_PHAGS_PA", USCRIPT_PHAGS_PA, R"doc(
             Phag
             )doc")
      .value("USCRIPT_PHOENICIAN", USCRIPT_PHOENICIAN, R"doc(
             Phnx
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
      .value("USCRIPT_MIAO", USCRIPT_MIAO, R"doc(
             Plrd
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
      .value("USCRIPT_PHONETIC_POLLARD", USCRIPT_PHONETIC_POLLARD, R"doc(
             Plrd (alias)
             )doc")
      .value("USCRIPT_RONGORONGO", USCRIPT_RONGORONGO, R"doc(
             Roro
             )doc")
      .value("USCRIPT_SARATI", USCRIPT_SARATI, R"doc(
             Sara
             )doc")
      .value("USCRIPT_ESTRANGELO_SYRIAC", USCRIPT_ESTRANGELO_SYRIAC, R"doc(
             Syre
             )doc")
      .value("USCRIPT_WESTERN_SYRIAC", USCRIPT_WESTERN_SYRIAC, R"doc(
             Syrj
             )doc")
      .value("USCRIPT_EASTERN_SYRIAC", USCRIPT_EASTERN_SYRIAC, R"doc(
             Syrn
             )doc")
      .value("USCRIPT_TENGWAR", USCRIPT_TENGWAR, R"doc(
             Teng
             )doc")
      .value("USCRIPT_VAI", USCRIPT_VAI, R"doc(
             Vaii
             )doc")
      .value("USCRIPT_VISIBLE_SPEECH", USCRIPT_VISIBLE_SPEECH, R"doc(
             Visp
             )doc")
      .value("USCRIPT_CUNEIFORM", USCRIPT_CUNEIFORM, R"doc(
             Xsux
             )doc")
      .value("USCRIPT_UNWRITTEN_LANGUAGES", USCRIPT_UNWRITTEN_LANGUAGES, R"doc(
             Zxxx
             )doc")
      .value("USCRIPT_UNKNOWN", USCRIPT_UNKNOWN, R"doc(
             Zzzz
             )doc")
      .value("USCRIPT_CARIAN", USCRIPT_CARIAN, R"doc(
             Cari
             )doc")
      .value("USCRIPT_JAPANESE", USCRIPT_JAPANESE, R"doc(
             Jpan
             )doc")
      .value("USCRIPT_LANNA", USCRIPT_LANNA, R"doc(
             Lana
             )doc")
      .value("USCRIPT_LYCIAN", USCRIPT_LYCIAN, R"doc(
             Lyci
             )doc")
      .value("USCRIPT_LYDIAN", USCRIPT_LYDIAN, R"doc(
             Lydi
             )doc")
      .value("USCRIPT_OL_CHIKI", USCRIPT_OL_CHIKI, R"doc(
             Olck
             )doc")
      .value("USCRIPT_REJANG", USCRIPT_REJANG, R"doc(
             Rjng
             )doc")
      .value("USCRIPT_SAURASHTRA", USCRIPT_SAURASHTRA, R"doc(
             Saur
             )doc")
      .value("USCRIPT_SIGN_WRITING", USCRIPT_SIGN_WRITING, R"doc(
             Sgnw; Sutton SignWriting.
             )doc")
      .value("USCRIPT_SUNDANESE", USCRIPT_SUNDANESE, R"doc(
             Sund
             )doc")
      .value("USCRIPT_MOON", USCRIPT_MOON, R"doc(
             Moon
             )doc")
      .value("USCRIPT_MEITEI_MAYEK", USCRIPT_MEITEI_MAYEK, R"doc(
             Mtei
             )doc")
      .value("USCRIPT_IMPERIAL_ARAMAIC", USCRIPT_IMPERIAL_ARAMAIC, R"doc(
             Armi
             )doc")
      .value("USCRIPT_AVESTAN", USCRIPT_AVESTAN, R"doc(
             Avst
             )doc")
      .value("USCRIPT_CHAKMA", USCRIPT_CHAKMA, R"doc(
             Cakm
             )doc")
      .value("USCRIPT_KOREAN", USCRIPT_KOREAN, R"doc(
             Kore
             )doc")
      .value("USCRIPT_KAITHI", USCRIPT_KAITHI, R"doc(
             Kthi
             )doc")
      .value("USCRIPT_MANICHAEAN", USCRIPT_MANICHAEAN, R"doc(
             Mani
             )doc")
      .value("USCRIPT_INSCRIPTIONAL_PAHLAVI", USCRIPT_INSCRIPTIONAL_PAHLAVI,
             R"doc(
             Phli
             )doc")
      .value("USCRIPT_PSALTER_PAHLAVI", USCRIPT_PSALTER_PAHLAVI, R"doc(
             Phlp
             )doc")
      .value("USCRIPT_BOOK_PAHLAVI", USCRIPT_BOOK_PAHLAVI, R"doc(
             Phlv
             )doc")
      .value("USCRIPT_INSCRIPTIONAL_PARTHIAN", USCRIPT_INSCRIPTIONAL_PARTHIAN,
             R"doc(
             Prti
             )doc")
      .value("USCRIPT_SAMARITAN", USCRIPT_SAMARITAN, R"doc(
             Samr
             )doc")
      .value("USCRIPT_TAI_VIET", USCRIPT_TAI_VIET, R"doc(
             Tavt
             )doc")
      .value("USCRIPT_MATHEMATICAL_NOTATION", USCRIPT_MATHEMATICAL_NOTATION,
             R"doc(
             Zmth
             )doc")
      .value("USCRIPT_SYMBOLS", USCRIPT_SYMBOLS, R"doc(
             Zsym
             )doc")
      .value("USCRIPT_BAMUM", USCRIPT_BAMUM, R"doc(
             Bamu
             )doc")
      .value("USCRIPT_LISU", USCRIPT_LISU, R"doc(
             Lisu
             )doc")
      .value("USCRIPT_NAKHI_GEBA", USCRIPT_NAKHI_GEBA, R"doc(
             Nkgb
             )doc")
      .value("USCRIPT_OLD_SOUTH_ARABIAN", USCRIPT_OLD_SOUTH_ARABIAN, R"doc(
             Sarb
             )doc")
      .value("USCRIPT_BASSA_VAH", USCRIPT_BASSA_VAH, R"doc(
             Bass
             )doc")
      .value("USCRIPT_DUPLOYAN", USCRIPT_DUPLOYAN, R"doc(
             Dupl
             )doc")
      .value("USCRIPT_DUPLOYAN_SHORTAND", USCRIPT_DUPLOYAN_SHORTAND, R"doc(
             Deprecated: ICU 54 Typo, use USCRIPT_DUPLOYAN.
             )doc")
      .value("USCRIPT_ELBASAN", USCRIPT_ELBASAN, R"doc(
             Elba
             )doc")
      .value("USCRIPT_GRANTHA", USCRIPT_GRANTHA, R"doc(
             Gran
             )doc")
      .value("USCRIPT_KPELLE", USCRIPT_KPELLE, R"doc(
             Kpel
             )doc")
      .value("USCRIPT_LOMA", USCRIPT_LOMA, R"doc(
             Loma
             )doc")
      .value("USCRIPT_MENDE", USCRIPT_MENDE, R"doc(
             Mend; Mende Kikakui.
             )doc")
      .value("USCRIPT_MEROITIC_CURSIVE", USCRIPT_MEROITIC_CURSIVE, R"doc(
             Merc
             )doc")
      .value("USCRIPT_OLD_NORTH_ARABIAN", USCRIPT_OLD_NORTH_ARABIAN, R"doc(
             Narb
             )doc")
      .value("USCRIPT_NABATAEAN", USCRIPT_NABATAEAN, R"doc(
             Nbat
             )doc")
      .value("USCRIPT_PALMYRENE", USCRIPT_PALMYRENE, R"doc(
             Palm
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("USCRIPT_KHUDAWADI", USCRIPT_KHUDAWADI, R"doc(
             Sind
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("USCRIPT_SINDHI", USCRIPT_SINDHI, R"doc(
             Sind (alias)
             )doc")
      .value("USCRIPT_WARANG_CITI", USCRIPT_WARANG_CITI, R"doc(
             Wara
             )doc")
      .value("USCRIPT_AFAKA", USCRIPT_AFAKA, R"doc(
             Afak
             )doc")
      .value("USCRIPT_JURCHEN", USCRIPT_JURCHEN, R"doc(
             Jurc
             )doc")
      .value("USCRIPT_MRO", USCRIPT_MRO, R"doc(
             Mroo
             )doc")
      .value("USCRIPT_NUSHU", USCRIPT_NUSHU, R"doc(
             Nshu
             )doc")
      .value("USCRIPT_SHARADA", USCRIPT_SHARADA, R"doc(
             Shrd
             )doc")
      .value("USCRIPT_SORA_SOMPENG", USCRIPT_SORA_SOMPENG, R"doc(
             Sora
             )doc")
      .value("USCRIPT_TAKRI", USCRIPT_TAKRI, R"doc(
             Takr
             )doc")
      .value("USCRIPT_TANGUT", USCRIPT_TANGUT, R"doc(
             Tang
             )doc")
      .value("USCRIPT_WOLEAI", USCRIPT_WOLEAI, R"doc(
             Wole
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("USCRIPT_ANATOLIAN_HIEROGLYPHS", USCRIPT_ANATOLIAN_HIEROGLYPHS,
             R"doc(
             Hluw
             )doc")
      .value("USCRIPT_KHOJKI", USCRIPT_KHOJKI, R"doc(
             Khoj
             )doc")
      .value("USCRIPT_TIRHUTA", USCRIPT_TIRHUTA, R"doc(
             Tirh
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
      .value("USCRIPT_CAUCASIAN_ALBANIAN", USCRIPT_CAUCASIAN_ALBANIAN,
             R"doc(
             Aghb
             )doc")
      .value("USCRIPT_MAHAJANI", USCRIPT_MAHAJANI, R"doc(
             Mahj
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("USCRIPT_AHOM", USCRIPT_AHOM, R"doc(
             Ahom
             )doc")
      .value("USCRIPT_HATRAN", USCRIPT_HATRAN, R"doc(
             Hatr
             )doc")
      .value("USCRIPT_MODI", USCRIPT_MODI, R"doc(
             Modi
             )doc")
      .value("USCRIPT_MULTANI", USCRIPT_MULTANI, R"doc(
             Mult
             )doc")
      .value("USCRIPT_PAU_CIN_HAU", USCRIPT_PAU_CIN_HAU, R"doc(
             Pauc
             )doc")
      .value("USCRIPT_SIDDHAM", USCRIPT_SIDDHAM, R"doc(
             Sidd
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("USCRIPT_ADLAM", USCRIPT_ADLAM, R"doc(
             Adlm
             )doc")
      .value("USCRIPT_BHAIKSUKI", USCRIPT_BHAIKSUKI, R"doc(
             Bhks
             )doc")
      .value("USCRIPT_MARCHEN", USCRIPT_MARCHEN, R"doc(
             Marc
             )doc")
      .value("USCRIPT_NEWA", USCRIPT_NEWA, R"doc(
             Newa
             )doc")
      .value("USCRIPT_OSAGE", USCRIPT_OSAGE, R"doc(
             Osge
             )doc")
      .value("USCRIPT_HAN_WITH_BOPOMOFO", USCRIPT_HAN_WITH_BOPOMOFO,
             R"doc(
             Hanb
             )doc")
      .value("USCRIPT_JAMO", USCRIPT_JAMO, R"doc(
             Jamo
             )doc")
      .value("USCRIPT_SYMBOLS_EMOJI", USCRIPT_SYMBOLS_EMOJI, R"doc(
             Zsye
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
      .value("USCRIPT_MASARAM_GONDI", USCRIPT_MASARAM_GONDI, R"doc(
             Gonm
             )doc")
      .value("USCRIPT_SOYOMBO", USCRIPT_SOYOMBO, R"doc(
             Soyo
             )doc")
      .value("USCRIPT_ZANABAZAR_SQUARE", USCRIPT_ZANABAZAR_SQUARE, R"doc(
             Zanb
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("USCRIPT_DOGRA", USCRIPT_DOGRA, R"doc(
             Dogr
             )doc")
      .value("USCRIPT_GUNJALA_GONDI", USCRIPT_GUNJALA_GONDI, R"doc(
             Gong
             )doc")
      .value("USCRIPT_MAKASAR", USCRIPT_MAKASAR, R"doc(
             Maka
             )doc")
      .value("USCRIPT_MEDEFAIDRIN", USCRIPT_MEDEFAIDRIN, R"doc(
             Medf
             )doc")
      .value("USCRIPT_HANIFI_ROHINGYA", USCRIPT_HANIFI_ROHINGYA, R"doc(
             Rohg
             )doc")
      .value("USCRIPT_SOGDIAN", USCRIPT_SOGDIAN, R"doc(
             Sogd
             )doc")
      .value("USCRIPT_OLD_SOGDIAN", USCRIPT_OLD_SOGDIAN, R"doc(
             Sogo
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
      .value("USCRIPT_ELYMAIC", USCRIPT_ELYMAIC, R"doc(
             Elym
             )doc")
      .value("USCRIPT_NYIAKENG_PUACHUE_HMONG", USCRIPT_NYIAKENG_PUACHUE_HMONG,
             R"doc(
             Hmnp
             )doc")
      .value("USCRIPT_NANDINAGARI", USCRIPT_NANDINAGARI, R"doc(
             Nand
             )doc")
      .value("USCRIPT_WANCHO", USCRIPT_WANCHO, R"doc(
             Wcho
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
#if (U_ICU_VERSION_MAJOR_NUM >= 66)
      .value("USCRIPT_CHORASMIAN", USCRIPT_CHORASMIAN, R"doc(
             Chrs
             )doc")
      .value("USCRIPT_DIVES_AKURU", USCRIPT_DIVES_AKURU, R"doc(
             Diak
             )doc")
      .value("USCRIPT_KHITAN_SMALL_SCRIPT", USCRIPT_KHITAN_SMALL_SCRIPT,
             R"doc(
             Kits
             )doc")
      .value("USCRIPT_YEZIDI", USCRIPT_YEZIDI, R"doc(
             Yezi
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 66)
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
      .value("USCRIPT_CYPRO_MINOAN", USCRIPT_CYPRO_MINOAN, R"doc(
             Cpmn
             )doc")
      .value("USCRIPT_OLD_UYGHUR", USCRIPT_OLD_UYGHUR, R"doc(
             Ougr
             )doc")
      .value("USCRIPT_TANGSA", USCRIPT_TANGSA, R"doc(
             Tnsa
             )doc")
      .value("USCRIPT_TOTO", USCRIPT_TOTO, R"doc(
             Toto
             )doc")
      .value("USCRIPT_VITHKUQI", USCRIPT_VITHKUQI, R"doc(
             Vith
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
#if (U_ICU_VERSION_MAJOR_NUM >= 72)
      .value("USCRIPT_KAWI", USCRIPT_KAWI, R"doc(
             Kawi
             )doc")
      .value("USCRIPT_NAG_MUNDARI", USCRIPT_NAG_MUNDARI, R"doc(
             Nagm
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)
#if (U_ICU_VERSION_MAJOR_NUM >= 75)
      .value("USCRIPT_ARABIC_NASTALIQ", USCRIPT_ARABIC_NASTALIQ, R"doc(
             Aran
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 75)
#if (U_ICU_VERSION_MAJOR_NUM >= 76)
      .value("USCRIPT_GARAY", USCRIPT_GARAY, R"doc(
             Gara
             )doc")
      .value("USCRIPT_GURUNG_KHEMA", USCRIPT_GURUNG_KHEMA, R"doc(
            Gukh
            )doc")
      .value("USCRIPT_KIRAT_RAI", USCRIPT_KIRAT_RAI, R"doc(
            Krai
            )doc")
      .value("USCRIPT_OL_ONAL", USCRIPT_OL_ONAL, R"doc(
            Onao
            )doc")
      .value("USCRIPT_SUNUWAR", USCRIPT_SUNUWAR, R"doc(
            Sunu
            )doc")
      .value("USCRIPT_TODHRI", USCRIPT_TODHRI, R"doc(
            Todr
            )doc")
      .value("USCRIPT_TULU_TIGALARI", USCRIPT_TULU_TIGALARI, R"doc(
            Tutg
            )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)
#if (U_ICU_VERSION_MAJOR_NUM >= 78)
      .value("USCRIPT_BERIA_ERFE", USCRIPT_BERIA_ERFE, R"doc(
            Berf
            )doc")
      .value("USCRIPT_SIDETIC", USCRIPT_SIDETIC, R"doc(
            Sidt
            )doc")
      .value("USCRIPT_TAI_YO", USCRIPT_TAI_YO, R"doc(
            Tayo
            )doc")
      .value("USCRIPT_TOLONG_SIKI", USCRIPT_TOLONG_SIKI, R"doc(
            Tols
            )doc")
      .value("USCRIPT_TRADITIONAL_HAN_WITH_LATIN",
             USCRIPT_TRADITIONAL_HAN_WITH_LATIN, R"doc(
             Hntl
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)
      .value("USCRIPT_CODE_LIMIT", USCRIPT_CODE_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  //
  // enum UScriptUsage
  //
  py::enum_<UScriptUsage>(m, "UScriptUsage", py::arithmetic(), R"doc(
Script usage constants.

See UAX #31 Unicode Identifier and Pattern Syntax.
http://www.unicode.org/reports/tr31/#Table_Candidate_Characters_for_Exclusion_from_Identifiers
      )doc")
      .value("USCRIPT_USAGE_NOT_ENCODED", USCRIPT_USAGE_NOT_ENCODED, R"doc(
             Not encoded in Unicode.
             )doc")
      .value("USCRIPT_USAGE_UNKNOWN", USCRIPT_USAGE_UNKNOWN, R"doc(
             Unknown script usage.
             )doc")
      .value("USCRIPT_USAGE_EXCLUDED", USCRIPT_USAGE_EXCLUDED, R"doc(
             Candidate for Exclusion from Identifiers.
             )doc")
      .value("USCRIPT_USAGE_LIMITED_USE", USCRIPT_USAGE_LIMITED_USE, R"doc(
             Limited Use script.
             )doc")
      .value("USCRIPT_USAGE_ASPIRATIONAL", USCRIPT_USAGE_ASPIRATIONAL, R"doc(
             Aspirational Use script.
             )doc")
      .value("USCRIPT_USAGE_RECOMMENDED", USCRIPT_USAGE_RECOMMENDED, R"doc(
             Recommended script.
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  //
  // Functions
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uscript_breaks_between_letters",
      [](UScriptCode script) -> py::bool_ {
        return uscript_breaksBetweenLetters(script);
      },
      py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  m.def(
      "uscript_get_code",
      [](const std::string &name_or_abbr_or_locale) {
        auto name_or_abbr_or_locale_data = name_or_abbr_or_locale.data();
        ErrorCode error_code;
        const auto capacity = uscript_getCode(name_or_abbr_or_locale_data,
                                              nullptr, 0, error_code);
        std::vector<UScriptCode> result(capacity);
        error_code.reset();
        uscript_getCode(name_or_abbr_or_locale_data, result.data(), capacity,
                        error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("name_or_abbr_or_locale"));

  m.def("uscript_get_name", &uscript_getName, py::arg("script_code"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uscript_get_sample_string",
      [](UScriptCode script) {
        ErrorCode error_code;
        const auto capacity =
            uscript_getSampleString(script, nullptr, 0, error_code);
        std::u16string result(capacity, u'\0');
        error_code.reset();
        uscript_getSampleString(script, result.data(), capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("script"));

  m.def("uscript_get_sample_unicode_string", &uscript_getSampleUnicodeString,
        py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  m.def(
      "uscript_get_script",
      [](UChar32 codepoint) {
        ErrorCode error_code;
        auto result = uscript_getScript(codepoint, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("codepoint"));

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def(
      "uscript_get_script_extensions",
      [](UChar32 c) {
        ErrorCode error_code;
        const auto capacity =
            uscript_getScriptExtensions(c, nullptr, 0, error_code);
        std::vector<UScriptCode> result(capacity);
        error_code.reset();
        uscript_getScriptExtensions(c, result.data(), capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("c"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  m.def("uscript_get_short_name", &uscript_getShortName,
        py::arg("script_code"));

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def("uscript_get_usage", &uscript_getUsage, py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  m.def(
      "uscript_has_script",
      [](UChar32 c, UScriptCode sc) -> py::bool_ {
        return uscript_hasScript(c, sc);
      },
      py::arg("c"), py::arg("sc"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  m.def(
      "uscript_is_cased",
      [](UScriptCode script) -> py::bool_ { return uscript_isCased(script); },
      py::arg("script"));

  m.def(
      "uscript_is_right_to_left",
      [](UScriptCode script) -> py::bool_ {
        return uscript_isRightToLeft(script);
      },
      py::arg("script"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

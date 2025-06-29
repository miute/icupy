import pytest

from icupy import icu

# From icu/source/test/intltest/uts46test.cpp
_severe_errors = (
    icu.IDNA.ERROR_LEADING_COMBINING_MARK
    | icu.IDNA.ERROR_DISALLOWED
    | icu.IDNA.ERROR_PUNYCODE
    | icu.IDNA.ERROR_LABEL_HAS_DOT
    | icu.IDNA.ERROR_INVALID_ACE_LABEL
)

_common_options = (
    icu.IDNA.USE_STD3_RULES
    | icu.IDNA.CHECK_BIDI
    | icu.IDNA.CHECK_CONTEXTJ
    | icu.IDNA.CHECK_CONTEXTO
)

# static IDNA *icu::IDNA::createUTS46Instance(
#       uint32_t options,
#       UErrorCode &errorCode
# )
_trans = icu.IDNA.create_uts46_instance(_common_options)
_nontrans = icu.IDNA.create_uts46_instance(
    _common_options | icu.IDNA.NONTRANSITIONAL_TO_ASCII | icu.IDNA.NONTRANSITIONAL_TO_UNICODE
)

_test_cases = [
    ["www.eXample.cOm", "B", "www.example.com", 0],  # all ASCII
    ["B\\u00FCcher.de", "B", "b\\u00FCcher.de", 0],  # u-umlaut
    ["\\u00D6BB", "B", "\\u00F6bb", 0],  # O-umlaut
    ["fa\\u00DF.de", "N", "fa\\u00DF.de", 0],  # sharp s
    ["fa\\u00DF.de", "T", "fass.de", 0],  # sharp s
    ["XN--fA-hia.dE", "B", "fa\\u00DF.de", 0],  # sharp s in Punycode
    [
        "\\u03B2\\u03CC\\u03BB\\u03BF\\u03C2.com",
        "N",  # Greek with final sigma
        "\\u03B2\\u03CC\\u03BB\\u03BF\\u03C2.com",
        0,
    ],
    [
        "\\u03B2\\u03CC\\u03BB\\u03BF\\u03C2.com",
        "T",  # Greek with final sigma
        "\\u03B2\\u03CC\\u03BB\\u03BF\\u03C3.com",
        0,
    ],
    [
        "xn--nxasmm1c",
        "B",  # Greek with final sigma in Punycode
        "\\u03B2\\u03CC\\u03BB\\u03BF\\u03C2",
        0,
    ],
    [
        "www.\\u0DC1\\u0DCA\\u200D\\u0DBB\\u0DD3.com",
        "N",
        # "Sri" in "Sri Lanka" has a ZWJ
        "www.\\u0DC1\\u0DCA\\u200D\\u0DBB\\u0DD3.com",
        0,
    ],
    [
        "www.\\u0DC1\\u0DCA\\u200D\\u0DBB\\u0DD3.com",
        "T",
        # "Sri" in "Sri Lanka" has a ZWJ
        "www.\\u0DC1\\u0DCA\\u0DBB\\u0DD3.com",
        0,
    ],
    [
        "www.xn--10cl1a0b660p.com",
        "B",  # "Sri" in Punycode
        "www.\\u0DC1\\u0DCA\\u200D\\u0DBB\\u0DD3.com",
        0,
    ],
    [
        "\\u0646\\u0627\\u0645\\u0647\\u200C\\u0627\\u06CC",
        "N",  # ZWNJ
        "\\u0646\\u0627\\u0645\\u0647\\u200C\\u0627\\u06CC",
        0,
    ],
    [
        "\\u0646\\u0627\\u0645\\u0647\\u200C\\u0627\\u06CC",
        "T",  # ZWNJ
        "\\u0646\\u0627\\u0645\\u0647\\u0627\\u06CC",
        0,
    ],
    [
        "xn--mgba3gch31f060k.com",
        "B",  # ZWNJ in Punycode
        "\\u0646\\u0627\\u0645\\u0647\\u200C\\u0627\\u06CC.com",
        0,
    ],
    ["a.b\\uFF0Ec\\u3002d\\uFF61", "B", "a.b.c.d.", 0],
    ["U\\u0308.xn--tda", "B", "\\u00FC.\\u00FC", 0],  # U+umlaut.u-umlaut
    [
        "xn--u-ccb",
        "B",  # u+umlaut in Punycode
        "xn--u-ccb\\uFFFD",
        icu.IDNA.ERROR_INVALID_ACE_LABEL,
    ],
    [
        "a\\u2488com",
        "B",  # contains 1-dot
        "a\\uFFFDcom",
        icu.IDNA.ERROR_DISALLOWED,
    ],
    [
        "xn--a-ecp.ru",
        "B",  # contains 1-dot in Punycode
        "xn--a-ecp\\uFFFD.ru",
        icu.IDNA.ERROR_INVALID_ACE_LABEL,
    ],
    [
        "xn--0.pt",
        "B",  # invalid Punycode
        "xn--0\\uFFFD.pt",
        icu.IDNA.ERROR_PUNYCODE,
    ],
    [
        "xn--a.pt",
        "B",  # U+0080
        "xn--a\\uFFFD.pt",
        icu.IDNA.ERROR_INVALID_ACE_LABEL,
    ],
    [
        "xn--a-\\u00C4.pt",
        "B",  # invalid Punycode
        "xn--a-\\u00E4.pt",
        icu.IDNA.ERROR_PUNYCODE,
    ],
    [
        "\\u65E5\\u672C\\u8A9E\\u3002\\uFF2A\\uFF30",
        "B",
        # Japanese with fullwidth ".jp"
        "\\u65E5\\u672C\\u8A9E.jp",
        0,
    ],
    ["\\u2615", "B", "\\u2615", 0],  # Unicode 4.0 HOT BEVERAGE
    # some characters are disallowed because they are canonically equivalent
    # to sequences with non-LDH ASCII
    [
        "a\\u2260b\\u226Ec\\u226Fd",
        "B",
        (
            "a\\uFFFDb\\uFFFDc\\uFFFDd"
            if icu.U_ICU_VERSION_MAJOR_NUM < 74
            else "a\\u2260b\\u226Ec\\u226Fd"
        ),
        icu.IDNA.ERROR_DISALLOWED if icu.U_ICU_VERSION_MAJOR_NUM < 74 else 0,
    ],
    # many deviation characters, test the special mapping code
    [
        "1.a\\u00DF\\u200C\\u200Db\\u200C\\u200Dc\\u00DF\\u00DF\\u00DF\\u00DFd"
        "\\u03C2\\u03C3\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFe"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFx"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFy"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u0302\\u00DFz",
        "N",
        "1.a\\u00DF\\u200C\\u200Db\\u200C\\u200Dc\\u00DF\\u00DF\\u00DF\\u00DFd"
        "\\u03C2\\u03C3\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFe"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFx"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFy"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u0302\\u00DFz",
        icu.IDNA.ERROR_LABEL_TOO_LONG | icu.IDNA.ERROR_CONTEXTJ,
    ],
    [
        "1.a\\u00DF\\u200C\\u200Db\\u200C\\u200Dc\\u00DF\\u00DF\\u00DF\\u00DFd"
        "\\u03C2\\u03C3\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFe"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFx"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DFy"
        "\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u00DF\\u0302\\u00DFz",
        "T",
        "1.assbcssssssssd"
        "\\u03C3\\u03C3sssssssssssssssse"
        "ssssssssssssssssssssx"
        "ssssssssssssssssssssy"
        "sssssssssssssss\\u015Dssz",
        icu.IDNA.ERROR_LABEL_TOO_LONG,
    ],
    # "xn--bss" with deviation characters
    [
        "\\u200Cx\\u200Dn\\u200C-\\u200D-b\\u00DF",
        "N",
        "\\u200Cx\\u200Dn\\u200C-\\u200D-b\\u00DF",
        icu.IDNA.ERROR_CONTEXTJ,
    ],
    ["\\u200Cx\\u200Dn\\u200C-\\u200D-b\\u00DF", "T", "\\u5919", 0],
    # "xn--bssffl" written as:
    # 02E3 MODIFIER LETTER SMALL X
    # 034F COMBINING GRAPHEME JOINER (ignored)
    # 2115 DOUBLE-STRUCK CAPITAL N
    # 200B ZERO WIDTH SPACE (ignored)
    # FE63 SMALL HYPHEN-MINUS
    # 00AD SOFT HYPHEN (ignored)
    # FF0D FULLWIDTH HYPHEN-MINUS
    # 180C MONGOLIAN FREE VARIATION SELECTOR TWO (ignored)
    # 212C SCRIPT CAPITAL B
    # FE00 VARIATION SELECTOR-1 (ignored)
    # 017F LATIN SMALL LETTER LONG S
    # 2064 INVISIBLE PLUS (ignored)
    # 1D530 MATHEMATICAL FRAKTUR SMALL S
    # E01EF VARIATION SELECTOR-256 (ignored)
    # FB04 LATIN SMALL LIGATURE FFL
    [
        "\\u02E3\\u034F\\u2115\\u200B\\uFE63\\u00AD\\uFF0D\\u180C"
        "\\u212C\\uFE00\\u017F\\u2064\\U0001D530\\U000E01EF\\uFB04",
        "B",
        "\\u5921\\u591E\\u591C\\u5919",
        0,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        0,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901.",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901.",
        0,
    ],
    # Domain name >256 characters, forces slow path in UTF-8 processing.
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "12345678901234567890123456789012345678901234567890123456789012",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "12345678901234567890123456789012345678901234567890123456789012",
        icu.IDNA.ERROR_DOMAIN_NAME_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789\\u05D0",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789\\u05D0",
        icu.IDNA.ERROR_DOMAIN_NAME_TOO_LONG | icu.IDNA.ERROR_BIDI,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901234."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901234."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890",
        icu.IDNA.ERROR_LABEL_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901234."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890.",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901234."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890.",
        icu.IDNA.ERROR_LABEL_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901234."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901234."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        icu.IDNA.ERROR_LABEL_TOO_LONG | icu.IDNA.ERROR_DOMAIN_NAME_TOO_LONG,
    ],
    # label length 63: xn--1234567890123456789012345678901234567890123456789012345-9te
    [
        "\\u00E41234567890123456789012345678901234567890123456789012345",
        "B",
        "\\u00E41234567890123456789012345678901234567890123456789012345",
        0,
    ],
    [
        "1234567890\\u00E41234567890123456789012345678901234567890123456",
        "B",
        "1234567890\\u00E41234567890123456789012345678901234567890123456",
        icu.IDNA.ERROR_LABEL_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E4123456789012345678901234567890123456789012345."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E4123456789012345678901234567890123456789012345."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        0,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E4123456789012345678901234567890123456789012345."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901.",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E4123456789012345678901234567890123456789012345."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901.",
        0,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E4123456789012345678901234567890123456789012345."
        "123456789012345678901234567890123456789012345678901234567890123."
        "12345678901234567890123456789012345678901234567890123456789012",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E4123456789012345678901234567890123456789012345."
        "123456789012345678901234567890123456789012345678901234567890123."
        "12345678901234567890123456789012345678901234567890123456789012",
        icu.IDNA.ERROR_DOMAIN_NAME_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E41234567890123456789012345678901234567890123456."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E41234567890123456789012345678901234567890123456."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890",
        icu.IDNA.ERROR_LABEL_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E41234567890123456789012345678901234567890123456."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890.",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E41234567890123456789012345678901234567890123456."
        "123456789012345678901234567890123456789012345678901234567890123."
        "123456789012345678901234567890123456789012345678901234567890.",
        icu.IDNA.ERROR_LABEL_TOO_LONG,
    ],
    [
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E41234567890123456789012345678901234567890123456."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        "B",
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890\\u00E41234567890123456789012345678901234567890123456."
        "123456789012345678901234567890123456789012345678901234567890123."
        "1234567890123456789012345678901234567890123456789012345678901",
        icu.IDNA.ERROR_LABEL_TOO_LONG | icu.IDNA.ERROR_DOMAIN_NAME_TOO_LONG,
    ],
    # hyphen errors and empty-label errors
    # Ticket #10883: ToUnicode also checks for empty labels.
    [".", "B", ".", icu.IDNA.ERROR_EMPTY_LABEL],
    ["\\uFF0E", "B", ".", icu.IDNA.ERROR_EMPTY_LABEL],
    # "xn---q----jra"=="-q--a-umlaut-"
    [
        "a.b..-q--a-.e",
        "B",
        "a.b..-q--a-.e",
        icu.IDNA.ERROR_EMPTY_LABEL
        | icu.IDNA.ERROR_LEADING_HYPHEN
        | icu.IDNA.ERROR_TRAILING_HYPHEN
        | icu.IDNA.ERROR_HYPHEN_3_4,
    ],
    [
        "a.b..-q--\\u00E4-.e",
        "B",
        "a.b..-q--\\u00E4-.e",
        icu.IDNA.ERROR_EMPTY_LABEL
        | icu.IDNA.ERROR_LEADING_HYPHEN
        | icu.IDNA.ERROR_TRAILING_HYPHEN
        | icu.IDNA.ERROR_HYPHEN_3_4,
    ],
    [
        "a.b..xn---q----jra.e",
        "B",
        "a.b..-q--\\u00E4-.e",
        icu.IDNA.ERROR_EMPTY_LABEL
        | icu.IDNA.ERROR_LEADING_HYPHEN
        | icu.IDNA.ERROR_TRAILING_HYPHEN
        | icu.IDNA.ERROR_HYPHEN_3_4,
    ],
    ["a..c", "B", "a..c", icu.IDNA.ERROR_EMPTY_LABEL],
    ["a.xn--.c", "B", "a..c", icu.IDNA.ERROR_EMPTY_LABEL],
    ["a.-b.", "B", "a.-b.", icu.IDNA.ERROR_LEADING_HYPHEN],
    ["a.b-.c", "B", "a.b-.c", icu.IDNA.ERROR_TRAILING_HYPHEN],
    [
        "a.-.c",
        "B",
        "a.-.c",
        icu.IDNA.ERROR_LEADING_HYPHEN | icu.IDNA.ERROR_TRAILING_HYPHEN,
    ],
    ["a.bc--de.f", "B", "a.bc--de.f", icu.IDNA.ERROR_HYPHEN_3_4],
    ["\\u00E4.\\u00AD.c", "B", "\\u00E4..c", icu.IDNA.ERROR_EMPTY_LABEL],
    ["\\u00E4.xn--.c", "B", "\\u00E4..c", icu.IDNA.ERROR_EMPTY_LABEL],
    ["\\u00E4.-b.", "B", "\\u00E4.-b.", icu.IDNA.ERROR_LEADING_HYPHEN],
    ["\\u00E4.b-.c", "B", "\\u00E4.b-.c", icu.IDNA.ERROR_TRAILING_HYPHEN],
    [
        "\\u00E4.-.c",
        "B",
        "\\u00E4.-.c",
        icu.IDNA.ERROR_LEADING_HYPHEN | icu.IDNA.ERROR_TRAILING_HYPHEN,
    ],
    ["\\u00E4.bc--de.f", "B", "\\u00E4.bc--de.f", icu.IDNA.ERROR_HYPHEN_3_4],
    [
        "a.b.\\u0308c.d",
        "B",
        "a.b.\\uFFFDc.d",
        icu.IDNA.ERROR_LEADING_COMBINING_MARK,
    ],
    [
        "a.b.xn--c-bcb.d",
        "B",
        "a.b.xn--c-bcb\\uFFFD.d",
        icu.IDNA.ERROR_LEADING_COMBINING_MARK | icu.IDNA.ERROR_INVALID_ACE_LABEL,
    ],
    # BiDi
    ["A0", "B", "a0", 0],
    ["0A", "B", "0a", 0],  # all-LTR is ok to start with a digit (EN)
    [
        "0A.\\u05D0",
        "B",  # ASCII label does not start with L/R/AL
        "0a.\\u05D0",
        icu.IDNA.ERROR_BIDI,
    ],
    [
        "c.xn--0-eha.xn--4db",
        "B",  # 2nd label does not start with L/R/AL
        "c.0\\u00FC.\\u05D0",
        icu.IDNA.ERROR_BIDI,
    ],
    [
        "b-.\\u05D0",
        "B",  # label does not end with L/EN
        "b-.\\u05D0",
        icu.IDNA.ERROR_TRAILING_HYPHEN | icu.IDNA.ERROR_BIDI,
    ],
    [
        "d.xn----dha.xn--4db",
        "B",  # 2nd label does not end with L/EN
        "d.\\u00FC-.\\u05D0",
        icu.IDNA.ERROR_TRAILING_HYPHEN | icu.IDNA.ERROR_BIDI,
    ],
    ["a\\u05D0", "B", "a\\u05D0", icu.IDNA.ERROR_BIDI],
    # first dir != last dir
    ["\\u05D0\\u05C7", "B", "\\u05D0\\u05C7", 0],
    ["\\u05D09\\u05C7", "B", "\\u05D09\\u05C7", 0],
    ["\\u05D0a\\u05C7", "B", "\\u05D0a\\u05C7", icu.IDNA.ERROR_BIDI],
    # first dir != last dir
    ["\\u05D0\\u05EA", "B", "\\u05D0\\u05EA", 0],
    ["\\u05D0\\u05F3\\u05EA", "B", "\\u05D0\\u05F3\\u05EA", 0],
    ["a\\u05D0Tz", "B", "a\\u05D0tz", icu.IDNA.ERROR_BIDI],  # mixed dir
    ["\\u05D0T\\u05EA", "B", "\\u05D0t\\u05EA", icu.IDNA.ERROR_BIDI],
    # mixed dir
    ["\\u05D07\\u05EA", "B", "\\u05D07\\u05EA", 0],
    ["\\u05D0\\u0667\\u05EA", "B", "\\u05D0\\u0667\\u05EA", 0],
    # Arabic 7 in the middle
    ["a7\\u0667z", "B", "a7\\u0667z", icu.IDNA.ERROR_BIDI],  # AN digit in LTR
    ["a7\\u0667", "B", "a7\\u0667", icu.IDNA.ERROR_BIDI],  # AN digit in LTR
    [
        "\\u05D07\\u0667\\u05EA",
        "B",  # mixed EN/AN digits in RTL
        "\\u05D07\\u0667\\u05EA",
        icu.IDNA.ERROR_BIDI,
    ],
    [
        "\\u05D07\\u0667",
        "B",  # mixed EN/AN digits in RTL
        "\\u05D07\\u0667",
        icu.IDNA.ERROR_BIDI,
    ],
    # ZWJ
    ["\\u0BB9\\u0BCD\\u200D", "N", "\\u0BB9\\u0BCD\\u200D", 0],  # Virama+ZWJ
    ["\\u0BB9\\u200D", "N", "\\u0BB9\\u200D", icu.IDNA.ERROR_CONTEXTJ],
    # no Virama
    ["\\u200D", "N", "\\u200D", icu.IDNA.ERROR_CONTEXTJ],  # no Virama
    # ZWNJ
    ["\\u0BB9\\u0BCD\\u200C", "N", "\\u0BB9\\u0BCD\\u200C", 0],  # Virama+ZWNJ
    ["\\u0BB9\\u200C", "N", "\\u0BB9\\u200C", icu.IDNA.ERROR_CONTEXTJ],
    # no Virama
    ["\\u200C", "N", "\\u200C", icu.IDNA.ERROR_CONTEXTJ],  # no Virama
    [
        "\\u0644\\u0670\\u200C\\u06ED\\u06EF",
        "N",  # Joining types D T ZWNJ T R
        "\\u0644\\u0670\\u200C\\u06ED\\u06EF",
        0,
    ],
    [
        "\\u0644\\u0670\\u200C\\u06EF",
        "N",  # D T ZWNJ R
        "\\u0644\\u0670\\u200C\\u06EF",
        0,
    ],
    [
        "\\u0644\\u200C\\u06ED\\u06EF",
        "N",  # D ZWNJ T R
        "\\u0644\\u200C\\u06ED\\u06EF",
        0,
    ],
    ["\\u0644\\u200C\\u06EF", "N", "\\u0644\\u200C\\u06EF", 0],  # D ZWNJ R
    [
        "\\u0644\\u0670\\u200C\\u06ED",
        "N",  # D T ZWNJ T
        "\\u0644\\u0670\\u200C\\u06ED",
        icu.IDNA.ERROR_BIDI | icu.IDNA.ERROR_CONTEXTJ,
    ],
    [
        "\\u06EF\\u200C\\u06EF",
        "N",  # R ZWNJ R
        "\\u06EF\\u200C\\u06EF",
        icu.IDNA.ERROR_CONTEXTJ,
    ],
    [
        "\\u0644\\u200C",
        "N",  # D ZWNJ
        "\\u0644\\u200C",
        icu.IDNA.ERROR_BIDI | icu.IDNA.ERROR_CONTEXTJ,
    ],
    [
        "\\u0660\\u0661",
        "B",  # Arabic-Indic Digits alone
        "\\u0660\\u0661",
        icu.IDNA.ERROR_BIDI,
    ],
    [
        "\\u06F0\\u06F1",
        "B",  # Extended Arabic-Indic Digits alone
        "\\u06F0\\u06F1",
        0,
    ],
    [
        "\\u0660\\u06F1",
        "B",  # Mixed Arabic-Indic Digits
        "\\u0660\\u06F1",
        icu.IDNA.ERROR_CONTEXTO_DIGITS | icu.IDNA.ERROR_BIDI,
    ],
    # All of the CONTEXTO "Would otherwise have been DISALLOWED" characters
    # in their correct contexts,
    # then each in incorrect context.
    [
        "l\\u00B7l\\u4E00\\u0375\\u03B1\\u05D0\\u05F3\\u05F4\\u30FB",
        "B",
        "l\\u00B7l\\u4E00\\u0375\\u03B1\\u05D0\\u05F3\\u05F4\\u30FB",
        icu.IDNA.ERROR_BIDI,
    ],
    ["l\\u00B7", "B", "l\\u00B7", icu.IDNA.ERROR_CONTEXTO_PUNCTUATION],
    ["\\u00B7l", "B", "\\u00B7l", icu.IDNA.ERROR_CONTEXTO_PUNCTUATION],
    ["\\u0375", "B", "\\u0375", icu.IDNA.ERROR_CONTEXTO_PUNCTUATION],
    [
        "\\u03B1\\u05F3",
        "B",
        "\\u03B1\\u05F3",
        icu.IDNA.ERROR_CONTEXTO_PUNCTUATION | icu.IDNA.ERROR_BIDI,
    ],
    ["\\u05F4", "B", "\\u05F4", icu.IDNA.ERROR_CONTEXTO_PUNCTUATION],
    ["l\\u30FB", "B", "l\\u30FB", icu.IDNA.ERROR_CONTEXTO_PUNCTUATION],
    # Ticket #8137: UTS #46 toUnicode() fails with non-ASCII labels that turn
    # into 15 characters (UChars).
    # The bug was in u_strFromPunycode() which did not write the last character
    # if it just so fit into the end of the destination buffer.
    # The UTS #46 code gives a default-capacity UnicodeString as the destination buffer,
    # and the internal UnicodeString capacity is currently 15 UChars on 64-bit machines
    # but 13 on 32-bit machines.
    # Label with 15 UChars, for 64-bit-machine testing:
    ["aaaaaaaaaaaaa\\u00FCa.de", "B", "aaaaaaaaaaaaa\\u00FCa.de", 0],
    ["xn--aaaaaaaaaaaaaa-ssb.de", "B", "aaaaaaaaaaaaa\\u00FCa.de", 0],
    [
        "abschlu\\u00DFpr\\u00FCfung.de",
        "N",
        "abschlu\\u00DFpr\\u00FCfung.de",
        0,
    ],
    ["xn--abschluprfung-hdb15b.de", "B", "abschlu\\u00DFpr\\u00FCfung.de", 0],
    # Label with 13 UChars, for 32-bit-machine testing:
    ["xn--aaaaaaaaaaaa-nlb.de", "B", "aaaaaaaaaaa\\u00FCa.de", 0],
    ["xn--schluprfung-z6a39a.de", "B", "schlu\\u00DFpr\\u00FCfung.de", 0],
    # [ "", "B",
    #   "", 0 ],
]
if icu.U_ICU_VERSION_MAJOR_NUM >= 68:
    _row = [x[0] for x in _test_cases].index("a.xn--.c")
    _test_cases[_row][2] = "a.xn--\\uFFFD.c"
    _test_cases[_row][3] = icu.IDNA.ERROR_INVALID_ACE_LABEL

    _row = [x[0] for x in _test_cases].index("\\u00E4.xn--.c")
    _test_cases[_row][2] = "\\u00E4.xn--\\uFFFD.c"
    _test_cases[_row][3] = icu.IDNA.ERROR_INVALID_ACE_LABEL
else:  # U_ICU_VERSION_MAJOR_NUM <= 67
    _row = [x[0] for x in _test_cases].index("a.xn--.c")
    _test_cases[_row][2] = "a..c"
    _test_cases[_row][3] = icu.IDNA.ERROR_EMPTY_LABEL

    _row = [x[0] for x in _test_cases].index("\\u00E4.xn--.c")
    _test_cases[_row][2] = "\\u00E4..c"
    _test_cases[_row][3] = icu.IDNA.ERROR_EMPTY_LABEL


def _is_ascii(s: icu.UnicodeString) -> bool:
    p = s.get_buffer()
    return all(p[i] < 0x80 for i in range(s.length()))


def test_api() -> None:
    # UnicodeString &icu::IDNA::nameToASCII(
    #       const UnicodeString &name,
    #       UnicodeString &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    dest = icu.UnicodeString()
    info = icu.IDNAInfo()
    src = icu.UnicodeString("www.eXample.cOm")
    expected = icu.UnicodeString("www.example.com")
    result = _trans.name_to_ascii(src, dest, info)
    assert info.has_errors() is False
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert dest == expected

    dest.remove()
    result = _trans.name_to_ascii("www.eXample.cOm", dest, info)
    assert info.has_errors() is False
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert dest == expected

    # void icu::IDNA::nameToASCII_UTF8(
    #   	StringPiece name,
    #       ByteSink &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    result = _trans.name_to_ascii_utf8(b"www.eXample.cOm", info)
    assert info.has_errors() is False
    assert isinstance(result, bytes)
    assert result.decode() == expected

    # UnicodeString &icu::IDNA::labelToASCII(
    #       const UnicodeString &label,
    #       UnicodeString &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    src.set_to_bogus()
    dest = icu.UnicodeString("quatsch")
    with pytest.raises(icu.ICUError) as exc_info:
        _nontrans.label_to_ascii(src, dest, info)
    assert exc_info.value.args[0] == icu.UErrorCode.U_ILLEGAL_ARGUMENT_ERROR
    assert dest.is_bogus()

    dest.remove()
    src = icu.UnicodeString("xn--bcher.de-65a")
    expected = icu.UnicodeString("xn--bcher\\uFFFDde-65a").unescape()
    result = _nontrans.label_to_ascii(src, dest, info)
    assert info.get_errors() == (
        icu.IDNA.ERROR_LABEL_HAS_DOT | icu.IDNA.ERROR_INVALID_ACE_LABEL
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert dest == expected

    dest.remove()
    result = _nontrans.label_to_ascii("xn--bcher.de-65a", dest, info)
    assert info.get_errors() == (
        icu.IDNA.ERROR_LABEL_HAS_DOT | icu.IDNA.ERROR_INVALID_ACE_LABEL
    )
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert dest == expected

    # void icu::IDNA::labelToASCII_UTF8(
    # 	    StringPiece label,
    #       ByteSink &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    result = _nontrans.label_to_ascii_utf8(b"xn--bcher.de-65a", info)
    assert info.get_errors() == (
        icu.IDNA.ERROR_LABEL_HAS_DOT | icu.IDNA.ERROR_INVALID_ACE_LABEL
    )
    assert isinstance(result, bytes)
    assert result.decode() == expected

    # UnicodeString &icu::IDNA::labelToUnicode(
    # 	    const UnicodeString &label,
    #       UnicodeString &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    dest.remove()
    result = _trans.label_to_unicode("\x61\u00df", dest, info)
    assert info.get_errors() == 0
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert dest == "\x61\x73\x73"

    # void icu::IDNA::labelToUnicodeUTF8(
    #   	StringPiece label,
    #       ByteSink &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    result = _trans.label_to_unicode_utf8(b"\x61\xc3\x9f", info)
    assert info.get_errors() == 0
    assert isinstance(result, bytes)
    assert result == b"\x61\x73\x73"


def test_not_std3() -> None:
    not3 = icu.IDNA.create_uts46_instance(icu.IDNA.CHECK_BIDI)

    # '\x00A_2+2=4\n.eßen.net'
    src = icu.UnicodeString("\\u0000A_2+2=4\\u000A.e\\u00DFen.net").unescape()
    expected = icu.UnicodeString("\\u0000a_2+2=4\\u000A.essen.net").unescape()
    dest = icu.UnicodeString()
    info = icu.IDNAInfo()

    # UnicodeString &icu::IDNA::nameToUnicode(
    #       const UnicodeString &name,
    #       UnicodeString &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    # '\x00a_2+2=4\n.essen.net'
    result = not3.name_to_unicode(src, dest, info)
    assert info.has_errors() is False
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert result != src
    assert result == expected

    # void icu::IDNA::nameToUnicodeUTF8(
    #   	StringPiece name,
    #       ByteSink &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    result = not3.name_to_unicode_utf8(str(src).encode(), info)
    assert info.has_errors() is False
    assert isinstance(result, bytes)
    assert result.decode() == str(expected)

    # UnicodeString &icu::IDNA::nameToASCII(
    #       const UnicodeString &name,
    #       UnicodeString &dest,
    #       IDNAInfo &info,
    #       UErrorCode &errorCode
    # ) const
    dest.remove()
    src = icu.UnicodeString("a z.xn--4db.edu")
    result = not3.name_to_ascii(src, dest, info)
    assert info.get_errors() == icu.IDNA.ERROR_BIDI
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert result == src

    dest.remove()
    src = icu.UnicodeString("a\\u2260b\\u226Ec\\u226Fd").unescape()
    result = not3.name_to_unicode(src, dest, info)
    assert info.has_errors() is False
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert result == src

    dest.remove()
    result = not3.name_to_unicode("a\u2260b\u226ec\u226fd", dest, info)
    assert info.has_errors() is False
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(dest)
    assert result == src


@pytest.mark.parametrize(("s", "mode", "u", "errors"), _test_cases)
def test_some_cases(s: str, mode: str, u: str, errors: int) -> None:
    # ctou(chars) -> UnicodeString(chars, -1, US_INV).unescape()
    src = icu.UnicodeString(s, -1, icu.US_INV).unescape()
    expected = icu.UnicodeString(u, -1, icu.US_INV).unescape()
    at = icu.UnicodeString()
    ut = icu.UnicodeString()
    an = icu.UnicodeString()
    un = icu.UnicodeString()
    at_info = icu.IDNAInfo()
    ut_info = icu.IDNAInfo()
    an_info = icu.IDNAInfo()
    un_info = icu.IDNAInfo()
    _trans.name_to_ascii(src, at, at_info)
    _trans.name_to_unicode(src, ut, ut_info)
    _nontrans.name_to_ascii(src, an, an_info)
    _nontrans.name_to_unicode(src, un, un_info)

    # ToUnicode does not set length-overflow errors
    uni_errors = errors & ~(icu.IDNA.ERROR_LABEL_TOO_LONG | icu.IDNA.ERROR_DOMAIN_NAME_TOO_LONG)
    if mode in ("B", "N"):
        assert un_info.get_errors() == uni_errors
        assert un == expected
        assert an_info.get_errors() == errors
    if mode in ("B", "T"):
        assert ut_info.get_errors() == uni_errors
        assert ut == expected
        assert at_info.get_errors() == errors

    # ToASCII is all-ASCII if no severe errors
    assert not ((an_info.get_errors() & _severe_errors) == 0 and not _is_ascii(an))
    assert not ((at_info.get_errors() & _severe_errors) == 0 and not _is_ascii(at))

    # second-level processing
    atun = icu.UnicodeString()
    utan = icu.UnicodeString()
    anun = icu.UnicodeString()
    unan = icu.UnicodeString()
    atun_info = icu.IDNAInfo()
    utan_info = icu.IDNAInfo()
    anun_info = icu.IDNAInfo()
    unan_info = icu.IDNAInfo()
    _nontrans.name_to_unicode(at, atun, atun_info)
    _nontrans.name_to_ascii(ut, utan, utan_info)
    _nontrans.name_to_unicode(an, anun, anun_info)
    _nontrans.name_to_ascii(un, unan, unan_info)
    assert an == unan
    assert at == utan
    assert un == anun
    assert ut == atun

    # labelToUnicode
    atl = icu.UnicodeString()
    utl = icu.UnicodeString()
    anl = icu.UnicodeString()
    unl = icu.UnicodeString()
    atl_info = icu.IDNAInfo()
    utl_info = icu.IDNAInfo()
    anl_info = icu.IDNAInfo()
    unl_info = icu.IDNAInfo()
    _trans.label_to_ascii(src, atl, atl_info)
    _trans.label_to_unicode(src, utl, utl_info)
    _nontrans.label_to_ascii(src, anl, anl_info)
    _nontrans.label_to_unicode(src, unl, unl_info)
    if "\x2e" not in an:
        assert not (an != anl or an_info.get_errors() != anl_info.get_errors())
    else:
        assert anl_info.get_errors() & icu.IDNA.ERROR_LABEL_HAS_DOT

    if "\x2e" not in at:
        assert not (at != atl or at_info.get_errors() != atl_info.get_errors())
    else:
        assert atl_info.get_errors() & icu.IDNA.ERROR_LABEL_HAS_DOT

    if "\x2e" not in un:
        assert not (un != unl or un_info.get_errors() != unl_info.get_errors())
    else:
        assert unl_info.get_errors() & icu.IDNA.ERROR_LABEL_HAS_DOT

    if "\x2e" not in ut:
        assert not (ut != utl or ut_info.get_errors() != utl_info.get_errors())
    else:
        assert utl_info.get_errors() & icu.IDNA.ERROR_LABEL_HAS_DOT

    # Differences between transitional and nontransitional processing
    if mode == "B":
        assert not (
            an_info.is_transitional_different() is True
            or at_info.is_transitional_different() is True
            or un_info.is_transitional_different() is True
            or ut_info.is_transitional_different() is True
            or anl_info.is_transitional_different() is True
            or atl_info.is_transitional_different() is True
            or unl_info.is_transitional_different() is True
            or utl_info.is_transitional_different() is True
        )
        assert not (
            an != at
            or un != ut
            or anl != atl
            or unl != utl
            or an_info.get_errors() != at_info.get_errors()
            or un_info.get_errors() != ut_info.get_errors()
            or anl_info.get_errors() != atl_info.get_errors()
            or unl_info.get_errors() != utl_info.get_errors()
        )
    else:
        assert not (
            an_info.is_transitional_different() is False
            or at_info.is_transitional_different() is False
            or un_info.is_transitional_different() is False
            or ut_info.is_transitional_different() is False
            or anl_info.is_transitional_different() is False
            or atl_info.is_transitional_different() is False
            or unl_info.is_transitional_different() is False
            or utl_info.is_transitional_different() is False
        )
        assert not (an == at or un == ut or anl == atl or unl == utl)

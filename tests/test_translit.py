import copy

import pytest

from icupy import (
    ICUException, Locale, StringEnumeration, Transliterator, UErrorCode,
    UnicodeSet, UnicodeString, UParseError, UTransDirection, UTransPosition,
)


def test_adopt_filter():
    id1 = UnicodeString("Halfwidth-Fullwidth", -1)
    test1 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD)
    src = UnicodeString("123"
                        "abc"
                        "\uFF71\uFF72\uFF73\uFF74\uFF75",
                        -1)

    text = src.clone()
    test1.transliterate(text)
    assert (text == "\uFF11\uFF12\uFF13"
                    "\uFF41\uFF42\uFF43"
                    "\u30A2\u30A4\u30A6\u30A8\u30AA")

    uniset = UnicodeSet(UnicodeString("[^0-9]", -1))
    test1.adopt_filter(uniset)
    text = src.clone()
    test1.transliterate(text)
    assert (text == "123"
                    "\uFF41\uFF42\uFF43"
                    "\u30A2\u30A4\u30A6\u30A8\u30AA")

    test1.adopt_filter(None)
    text = src.clone()
    test1.transliterate(text)
    assert (text == "\uFF11\uFF12\uFF13"
                    "\uFF41\uFF42\uFF43"
                    "\u30A2\u30A4\u30A6\u30A8\u30AA")


def test_clone():
    id1 = UnicodeString("NFD;Jamo-Latin;Latin-Greek", -1)
    test1 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD)
    test2 = test1.clone()
    assert isinstance(test2, Transliterator)
    assert test2.get_id() == test1.get_id()

    test3 = copy.copy(test1)
    assert test3.get_id() == test1.get_id()

    test4 = copy.deepcopy(test1)
    assert test4.get_id() == test1.get_id()


def test_create_instance():
    # static Transliterator* createFromRules(
    #       const UnicodeString &ID,
    #       const UnicodeString &rules,
    #       UTransDirection dir,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    id3 = UnicodeString("<ID>", -1)
    rules3 = UnicodeString("pre {alpha} post > | @ ALPHA ;"
                           "eALPHA > beta ;"
                           "pre {beta} post > BETA @@ | ;"
                           "post > xyz",
                           -1)
    parse_error = UParseError()
    test3 = Transliterator.create_from_rules(
        id3,
        rules3,
        UTransDirection.UTRANS_FORWARD,
        parse_error)
    assert test3.count_elements() == 0
    assert test3.get_filter() is None
    assert test3.get_id() == id3
    assert test3.get_maximum_context_length() == 3

    test30 = test3.get_element(0)
    assert test30.get_id() == test3.get_id()

    with pytest.raises(ICUException) as exc_info:
        _ = test3.get_element(1)
    assert exc_info.value.args[0] == UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    source_set = UnicodeSet()
    result = test3.get_source_set(source_set)
    assert isinstance(result, UnicodeSet)
    assert result == source_set
    assert source_set.size() > 0

    target_set = UnicodeSet()
    result = test3.get_target_set(target_set)
    assert isinstance(result, UnicodeSet)
    assert result == target_set
    assert target_set.size() > 0

    assert test3.orphan_filter() is None

    rules = UnicodeString()
    result = test3.to_rules(rules, True)
    assert isinstance(result, UnicodeString)
    assert result == rules
    assert len(rules) > 0

    text = UnicodeString("prealphapost prebetapost", -1)
    test3.transliterate(text)
    assert text == "prbetaxyz preBETApost"

    # [1]
    # static Transliterator* createInstance(
    #       const UnicodeString &ID,
    #       UTransDirection dir,
    #       UErrorCode &status
    # )
    id1 = UnicodeString("NFD;Jamo-Latin;Latin-Greek", -1)
    test1 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD)
    assert test1.count_elements() == 3
    assert test1.get_filter() is None
    assert test1.get_id() == id1
    assert test1.get_maximum_context_length() == 3

    test10 = test1.get_element(0)
    assert isinstance(test10, Transliterator)
    assert test10.get_id() == "NFD"

    test11 = test1.get_element(1)
    assert isinstance(test11, Transliterator)
    assert test11.get_id() == "Jamo-Latin"

    test12 = test1.get_element(2)
    assert isinstance(test12, Transliterator)
    assert test12.get_id() == "Latin-Greek"

    # [2]
    # static Transliterator* createInstance(
    #       const UnicodeString &ID,
    #       UTransDirection dir,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = UParseError()
    test2 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD,
        parse_error)
    assert test2.get_id() == id1

    # Transliterator* createInverse(UErrorCode &status)
    id4 = UnicodeString("Hiragana-Katakana", -1)
    test4 = Transliterator.create_instance(
        id4,
        UTransDirection.UTRANS_FORWARD)

    test5 = test4.create_inverse()
    assert test5.get_id() == "Katakana-Hiragana"

    result = test5.get_filter()
    assert isinstance(result, UnicodeSet)
    assert result.size() > 0


def test_get_available_ids():
    ids = Transliterator.get_available_ids()
    assert isinstance(ids, StringEnumeration)
    assert len(ids) > 0
    assert "Hiragana-Katakana" in ids


def test_get_available_variant():
    source = UnicodeString()
    target = UnicodeString()
    variant = UnicodeString()
    count = 0
    for i in range(Transliterator.count_available_sources()):
        result = Transliterator.get_available_source(i, source)
        assert result == source
        assert len(source) > 0
        for j in range(Transliterator.count_available_targets(source)):
            result = Transliterator.get_available_target(j, source, target)
            assert result == target
            assert len(target) > 0
            for k in range(Transliterator.count_available_variants(
                    source,
                    target)):
                result = Transliterator.get_available_variant(
                    k,
                    source,
                    target,
                    variant)
                assert result == variant
                assert len(variant) >= 0
                count += 1
    assert count > 0


def test_get_display_name():
    # [1]
    # static UnicodeString& getDisplayName(
    #       const UnicodeString &ID,
    #       const Locale &inLocale,
    #       UnicodeString &result
    # )
    display_name = UnicodeString()
    result = Transliterator.get_display_name(
        UnicodeString("Hiragana-Katakana", -1),
        Locale.get_us(),
        display_name)
    assert result == display_name
    assert result == "Hiragana to Katakana"

    # [2]
    # static UnicodeString& getDisplayName(
    #       const UnicodeString &ID,
    #       UnicodeString &result
    # )
    result = Transliterator.get_display_name(
        UnicodeString("Hiragana-Katakana", -1),
        display_name)
    assert result == display_name
    assert len(result) > 0


def test_register_instance():
    id3 = UnicodeString("Any-MyTransliterator", -1)
    rules3 = UnicodeString("pre {alpha} post > | @ ALPHA ;"
                           "eALPHA > beta ;"
                           "pre {beta} post > BETA @@ | ;"
                           "post > xyz",
                           -1)
    parse_error = UParseError()
    test3 = Transliterator.create_from_rules(
        id3,
        rules3,
        UTransDirection.UTRANS_FORWARD,
        parse_error)

    ids = Transliterator.get_available_ids()
    assert str(id3) not in ids

    Transliterator.register_instance(test3)

    id3a = UnicodeString("Any-MyRule", -1)
    Transliterator.register_alias(id3a, id3)

    ids = Transliterator.get_available_ids()
    assert str(id3) in ids
    assert str(id3a) in ids

    test3a = Transliterator.create_instance(
        id3a,
        UTransDirection.UTRANS_FORWARD)
    assert test3a.get_id() == id3a

    text = UnicodeString("prealphapost prebetapost", -1)
    test3a.transliterate(text)
    assert text == "prbetaxyz preBETApost"

    Transliterator.unregister(id3)
    Transliterator.unregister(id3a)
    ids = Transliterator.get_available_ids()
    assert str(id3) not in ids
    assert str(id3a) not in ids


def test_transliterate():
    id4 = UnicodeString("Hiragana-Katakana", -1)
    test4 = Transliterator.create_instance(
        id4,
        UTransDirection.UTRANS_FORWARD)
    src = UnicodeString("\u3042\u3044\u3046\u3048\u304A", -1)

    # [1]
    # void transliterate(Replaceable &text)
    text = src.clone()
    test4.transliterate(text)
    assert text == "\u30A2\u30A4\u30A6\u30A8\u30AA"

    # [2]
    # int32_t transliterate(Replaceable &text,
    #                       int32_t start,
    #                       int32_t limit
    # )
    text = src.clone()
    result = test4.transliterate(text, 1, 3)
    assert text == "\u3042\u30A4\u30A6\u3048\u304A"
    assert result == 3

    # [3]
    # void transliterate(Replaceable &text,
    #                    UTransPosition &index,
    #                    const UnicodeString &insertion,
    #                    UErrorCode &status
    # )
    text = src.clone()
    index = UTransPosition()
    index.context_start = 0
    index.context_limit = 5
    index.start = 1
    index.limit = 4
    insertion = UnicodeString("_", -1)
    test4.transliterate(text, index, insertion)
    assert index.context_start == 0
    assert index.context_limit == 6
    assert index.start == 5
    assert index.limit == 5
    assert text == "\u3042\u30A4\u30A6\u30A8_\u304A"

    # [4]
    # void transliterate(Replaceable &text,
    #                    UTransPosition &index,
    #                    UChar32 insertion,
    #                    UErrorCode &status
    # )
    text = src.clone()
    index = UTransPosition(0, 5, 1, 4)
    assert index.context_start == 0
    assert index.context_limit == 5
    assert index.start == 1
    assert index.limit == 4
    insertion = ord("_")
    test4.transliterate(text, index, insertion)
    assert index.context_start == 0
    assert index.context_limit == 6
    assert index.start == 5
    assert index.limit == 5
    assert text == "\u3042\u30A4\u30A6\u30A8_\u304A"

    # [5]
    # void transliterate(Replaceable &text,
    #                    UTransPosition &index,
    #                    UErrorCode &status
    # )
    text = src.clone()
    # UTransPosition(context_start, start, limit, context_limit)
    index = UTransPosition(0, 5, 1, 4)
    assert index.context_start == 0
    assert index.context_limit == 5
    assert index.start == 1
    assert index.limit == 4
    test4.transliterate(text, index)
    assert index.context_start == 0
    assert index.context_limit == 5
    assert index.start == 4
    assert index.limit == 4
    assert text == "\u3042\u30A4\u30A6\u30A8\u304A"

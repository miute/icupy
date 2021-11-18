import copy

import pytest
from icupy.icu import (
    ICUError, Locale, StringEnumeration, Transliterator, UErrorCode,
    UnicodeSet, UnicodeString, UParseError, UTransDirection, UTransPosition,
)


def test_adopt_filter():
    id1 = UnicodeString("Halfwidth-Fullwidth", -1)
    test1 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD)
    src = UnicodeString(
        "123"
        "abc"
        "\uFF71\uFF72\uFF73\uFF74\uFF75",
        -1)

    text = src.clone()
    test1.transliterate(text)
    assert (text == "\uFF11\uFF12\uFF13"
                    "\uFF41\uFF42\uFF43"
                    "\u30A2\u30A4\u30A6\u30A8\u30AA")

    # void icu::Transliterator::adoptFilter(UnicodeFilter *adoptedFilter)
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


def test_api():
    id4 = UnicodeString("Katakana-Latin", -1)
    trans = Transliterator.create_instance(
        id4,
        UTransDirection.UTRANS_FORWARD)

    # int32_t icu::Transliterator::countElements()
    assert trans.count_elements() == 3

    # const Transliterator &icu::Transliterator::getElement(
    #       int32_t index,
    #       UErrorCode &ec
    # )
    t0 = trans.get_element(0)
    assert isinstance(t0, Transliterator)

    t1 = trans.get_element(1)
    assert isinstance(t1, Transliterator)

    t2 = trans.get_element(2)
    assert isinstance(t2, Transliterator)

    with pytest.raises(ICUError) as exc_info:
        _ = trans.get_element(3)
    assert exc_info.value.args[0] == UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # const UnicodeFilter *icu::Transliterator::getFilter(void)
    uniset = trans.get_filter()
    assert isinstance(uniset, UnicodeSet)
    assert len(uniset) > 0

    # const UnicodeString &icu::Transliterator::getID(void)
    result = trans.get_id()
    assert isinstance(result, UnicodeString)
    assert result == id4

    # int32_t icu::Transliterator::getMaximumContextLength(void)
    assert trans.get_maximum_context_length() == 3

    # UnicodeSet &icu::Transliterator::getSourceSet(UnicodeSet &result)
    source = UnicodeSet()
    result = trans.get_source_set(source)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(source)
    assert len(source) > 0

    # UnicodeSet &icu::Transliterator::getTargetSet(UnicodeSet &result)
    target = UnicodeSet()
    result = trans.get_target_set(target)
    assert isinstance(result, UnicodeSet)
    assert id(result) == id(target)
    assert len(target) > 0

    # UnicodeFilter *icu::Transliterator::orphanFilter(void)
    uniset2 = trans.orphan_filter()
    assert isinstance(uniset2, UnicodeSet)
    assert len(uniset2) > 0

    # UnicodeString &icu::Transliterator::toRules(
    #       UnicodeString &result,
    #       UBool escapeUnprintable
    # )
    rules = UnicodeString()
    result = trans.to_rules(rules, False)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(rules)
    assert len(rules) > 0


def test_clone():
    id1 = UnicodeString("NFD;Jamo-Latin;Latin-Greek", -1)
    test1 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD)

    # Transliterator *icu::Transliterator::clone()
    test2 = test1.clone()
    assert isinstance(test2, Transliterator)
    assert test2.get_id() == test1.get_id()

    test3 = copy.copy(test1)
    assert test3.get_id() == test1.get_id()

    test4 = copy.deepcopy(test1)
    assert test4.get_id() == test1.get_id()


def test_create_instance():
    # static Transliterator *icu::Transliterator::createFromRules(
    #       const UnicodeString &ID,
    #       const UnicodeString &rules,
    #       UTransDirection dir,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    id3 = UnicodeString("<ID>", -1)
    rules3 = UnicodeString(
        "pre {alpha} post > | @ ALPHA ;"
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
    assert isinstance(test3, Transliterator)
    assert test3.get_id() == id3
    parsed_rules3 = UnicodeString()
    test3.to_rules(parsed_rules3, True)

    test3a = Transliterator.create_from_rules(
        str(id3),
        rules3,
        UTransDirection.UTRANS_FORWARD,
        parse_error)
    assert isinstance(test3a, Transliterator)
    assert test3a.get_id() == id3
    rules = UnicodeString()
    assert test3a.to_rules(rules, True) == parsed_rules3

    test3b = Transliterator.create_from_rules(
        id3,
        str(rules3),
        UTransDirection.UTRANS_FORWARD,
        parse_error)
    assert isinstance(test3b, Transliterator)
    assert test3b.get_id() == id3
    rules = UnicodeString()
    assert test3b.to_rules(rules, True) == parsed_rules3

    test3c = Transliterator.create_from_rules(
        str(id3),
        str(rules3),
        UTransDirection.UTRANS_FORWARD,
        parse_error)
    assert isinstance(test3c, Transliterator)
    assert test3c.get_id() == id3
    rules = UnicodeString()
    assert test3c.to_rules(rules, True) == parsed_rules3

    # [1]
    # static Transliterator *icu::Transliterator::createInstance(
    #       const UnicodeString &ID,
    #       UTransDirection dir,
    #       UErrorCode &status
    # )
    id1 = UnicodeString("NFD;Jamo-Latin;Latin-Greek", -1)
    test1 = Transliterator.create_instance(
        id1,
        UTransDirection.UTRANS_FORWARD)
    assert isinstance(test1, Transliterator)
    assert test1.get_id() == id1
    parsed_rules1 = UnicodeString()
    test1.to_rules(parsed_rules1, True)

    test1a = Transliterator.create_instance(
        str(id1),
        UTransDirection.UTRANS_FORWARD)
    assert isinstance(test1a, Transliterator)
    assert test1a.get_id() == id1
    rules = UnicodeString()
    assert test1a.to_rules(rules, True) == parsed_rules1

    # [2]
    # static Transliterator *icu::Transliterator::createInstance(
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
    assert isinstance(test2, Transliterator)
    assert test2.get_id() == id1
    parsed_rules1 = UnicodeString()
    test2.to_rules(parsed_rules1, True)

    test2a = Transliterator.create_instance(
        str(id1),
        UTransDirection.UTRANS_FORWARD,
        parse_error)
    assert isinstance(test2a, Transliterator)
    assert test2a.get_id() == id1
    rules = UnicodeString()
    assert test2a.to_rules(rules, True) == parsed_rules1

    # Transliterator *icu::Transliterator::createInverse(UErrorCode &status)
    id4 = UnicodeString("Hiragana-Katakana", -1)
    test4 = Transliterator.create_instance(
        id4,
        UTransDirection.UTRANS_FORWARD)
    test5 = test4.create_inverse()
    assert isinstance(test5, Transliterator)
    assert test5.get_id() == "Katakana-Hiragana"


def test_get_available_ids():
    # static StringEnumeration *icu::Transliterator::getAvailableIDs(
    #       UErrorCode &ec
    # )
    ids = Transliterator.get_available_ids()
    assert isinstance(ids, StringEnumeration)
    assert len(ids) > 0
    assert "Hiragana-Katakana" in ids


def test_get_available_variant():
    source = UnicodeString()
    target = UnicodeString()
    variant = UnicodeString()
    count = 0

    # static int32_t icu::Transliterator::countAvailableSources(void)
    for i in range(Transliterator.count_available_sources()):
        # static UnicodeString &icu::Transliterator::getAvailableSource(
        #       int32_t index,
        #       UnicodeString &result
        # )
        result = Transliterator.get_available_source(i, source)
        assert isinstance(result, UnicodeString)
        assert id(result) == id(source)
        assert len(source) > 0

        # static int32_t icu::Transliterator::countAvailableTargets(
        #       const UnicodeString &source
        # )
        targets = Transliterator.count_available_targets(source)
        assert targets == Transliterator.count_available_targets(str(source))
        for j in range(targets):
            # static UnicodeString &icu::Transliterator::getAvailableTarget(
            #       int32_t index,
            #       const UnicodeString &source,
            #       UnicodeString &result
            # )
            result = Transliterator.get_available_target(j, source, target)
            assert isinstance(result, UnicodeString)
            assert id(result) == id(target)
            assert len(target) > 0

            assert result == Transliterator.get_available_target(
                j, str(source), target)

            # static int32_t icu::Transliterator::countAvailableVariants(
            #       const UnicodeString &source,
            #       const UnicodeString &target
            # )
            variants = Transliterator.count_available_variants(
                source,
                target)
            assert variants == Transliterator.count_available_variants(
                str(source),
                target)
            assert variants == Transliterator.count_available_variants(
                source,
                str(target))
            assert variants == Transliterator.count_available_variants(
                str(source),
                str(target))

            for k in range(variants):
                # static UnicodeString &
                # icu::Transliterator::getAvailableVariant(
                #       int32_t index,
                #       const UnicodeString &source,
                #       const UnicodeString &target,
                #       UnicodeString &result
                # )
                result = Transliterator.get_available_variant(
                    k,
                    source,
                    target,
                    variant)
                assert isinstance(result, UnicodeString)
                assert id(result) == id(variant)
                assert len(variant) >= 0

                assert result == Transliterator.get_available_variant(
                    k,
                    str(source),
                    target,
                    variant)
                assert result == Transliterator.get_available_variant(
                    k,
                    source,
                    str(target),
                    variant)
                assert result == Transliterator.get_available_variant(
                    k,
                    str(source),
                    str(target),
                    variant)
                count += 1
    assert count > 0


def test_get_display_name():
    # [1]
    # static UnicodeString &icu::Transliterator::getDisplayName(
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

    display_name.remove()
    result = Transliterator.get_display_name(
        "Hiragana-Katakana",
        Locale.get_us(),
        display_name)
    assert result == display_name
    assert result == "Hiragana to Katakana"

    display_name.remove()
    result = Transliterator.get_display_name(
        UnicodeString("Hiragana-Katakana", -1),
        "en_US",
        display_name)
    assert result == display_name
    assert result == "Hiragana to Katakana"

    display_name.remove()
    result = Transliterator.get_display_name(
        "Hiragana-Katakana",
        "en_US",
        display_name)
    assert result == display_name
    assert result == "Hiragana to Katakana"

    # [2]
    # static UnicodeString &icu::Transliterator::getDisplayName(
    #       const UnicodeString &ID,
    #       UnicodeString &result
    # )
    result = Transliterator.get_display_name(
        UnicodeString("Hiragana-Katakana", -1),
        display_name)
    assert result == display_name
    assert len(result) > 0

    display_name.remove()
    result = Transliterator.get_display_name(
        "Hiragana-Katakana",
        display_name)
    assert result == display_name
    assert len(result) > 0


def test_register_instance():
    id3 = UnicodeString("Any-MyTransliterator", -1)
    rules3 = UnicodeString(
        "pre {alpha} post > | @ ALPHA ;"
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

    # static void icu::Transliterator::registerInstance(
    #       Transliterator *adoptedObj
    # )
    Transliterator.register_instance(test3)

    # static void icu::Transliterator::registerAlias(
    #       const UnicodeString &aliasID,
    #       const UnicodeString &realID
    # )
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

    # static void icu::Transliterator::unregister(const UnicodeString &ID)
    Transliterator.unregister(id3)
    Transliterator.unregister(id3a)
    ids = Transliterator.get_available_ids()
    assert str(id3) not in ids
    assert str(id3a) not in ids

    Transliterator.register_instance(test3)
    Transliterator.register_alias("Any-MyRule1", id3)
    Transliterator.register_alias(UnicodeString("Any-MyRule2"), str(id3))
    Transliterator.register_alias("Any-MyRule3", str(id3))
    ids = Transliterator.get_available_ids()
    assert "Any-MyRule1" in ids
    assert "Any-MyRule2" in ids
    assert "Any-MyRule3" in ids


def test_transliterate():
    id4 = UnicodeString("Hiragana-Katakana", -1)
    test4 = Transliterator.create_instance(
        id4,
        UTransDirection.UTRANS_FORWARD)
    src = UnicodeString("\u3042\u3044\u3046\u3048\u304A", -1)

    # [1]
    # void icu::Transliterator::transliterate(Replaceable &text)
    text = src.clone()
    test4.transliterate(text)
    assert text == "\u30A2\u30A4\u30A6\u30A8\u30AA"

    # [2]
    # int32_t icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       int32_t start,
    #       int32_t limit
    # )
    text = src.clone()
    result = test4.transliterate(text, 1, 3)
    assert text == "\u3042\u30A4\u30A6\u3048\u304A"
    assert result == 3

    # [3]
    # void icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       UTransPosition &index,
    #       const UnicodeString &insertion,
    #       UErrorCode &status
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

    text = src.clone()
    index = UTransPosition(0, 5, 1, 4)
    test4.transliterate(text, index, "_")
    assert index.context_start == 0
    assert index.context_limit == 6
    assert index.start == 5
    assert index.limit == 5
    assert text == "\u3042\u30A4\u30A6\u30A8_\u304A"

    # [4]
    # void icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       UTransPosition &index,
    #       UChar32 insertion,
    #       UErrorCode &status
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
    # void icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       UTransPosition &index,
    #       UErrorCode &status
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

from __future__ import annotations

import copy

import pytest

from icupy import icu


class _TestTrans(icu.Transliterator):
    # icu::Transliterator::Transliterator(
    #       const UnicodeString &ID,
    #       UnicodeFilter *adoptedFilter
    # )
    def __init__(self, id_: icu.UnicodeString | str) -> None:
        super().__init__(id_, None)
        self.num_calls: int = 0

    # virtual void icu::Transliterator::handleTransliterate(
    #       Replaceable &text,
    #       UTransPosition &pos,
    #       UBool incremental
    # ) const
    def _handle_transliterate(
        self,
        text: icu.UnicodeString,
        pos: icu.UTransPosition,
        incremental: bool,
    ) -> None:
        pos.start = pos.limit
        self.num_calls += 1


# from CompoundTransliterator
class _TestTrans2(icu.Transliterator):
    # icu::Transliterator::Transliterator(
    #       const UnicodeString &ID,
    #       UnicodeFilter *adoptedFilter
    # )
    def __init__(
        self,
        id_: icu.UnicodeString | str,
        filter_set: icu.UnicodeSet = None,
    ) -> None:
        super().__init__("Null", filter_set)
        self.num_calls: int = 0
        self._trans: list[icu.Transliterator] = []
        for basic_id in str(id_).strip(";").split(";"):
            basic_id = basic_id.strip()  # noqa: PLW2901
            if len(basic_id) == 0:
                # static Transliterator *icu::Transliterator::createBasicInstance(
                #       const UnicodeString &id,
                #       const UnicodeString *canon
                # )
                t = self._create_basic_instance(icu.UnicodeString("Any-Null"), None)
            else:
                t = self._create_basic_instance(basic_id, None)
            if t:
                self._trans.append(t)

        names: list[str] = []
        x = 0
        for t in self._trans:
            names.append(str(t.get_id()))
            x = max(x, t.get_maximum_context_length())

        # void icu::Transliterator::setID(
        #       const UnicodeString &id
        # )
        self._set_id(";".join(names))

        # void icu::Transliterator::setMaximumContextLength(
        #       int32_t maxContextLength
        # )
        self._set_maximum_context_length(x)

    # virtual void icu::Transliterator::handleTransliterate(
    #       Replaceable &text,
    #       UTransPosition &pos,
    #       UBool incremental
    # ) const
    def _handle_transliterate(
        self,
        text: icu.UnicodeString,
        pos: icu.UTransPosition,
        incremental: bool,
    ) -> None:
        start = pos.start
        for t in self._trans:
            # virtual void icu::Transliterator::filteredTransliterate(
            #       Replaceable &text,
            #       UTransPosition &index,
            #       UBool incremental
            # )	const
            t.filtered_transliterate(text, pos, incremental)
            pos.start = start

        pos.start = pos.limit
        self.num_calls += 1

    # virtual UnicodeSet & icu::Transliterator::getTargetSet(
    #       UnicodeSet &result
    # ) const
    def get_target_set(self, result: icu.UnicodeSet) -> icu.UnicodeSet:
        result.clear()
        temp = icu.UnicodeSet()
        for t in self._trans:
            result.add_all(t.get_target_set(temp))
        return result

    # virtual void icu::Transliterator::handleGetSourceSet(
    #       UnicodeSet &result
    # ) const
    def handle_get_source_set(self, result: icu.UnicodeSet) -> None:
        result.clear()
        temp = icu.UnicodeSet()
        for t in self._trans:
            result.add_all(t.get_source_set(temp))


def test_adopt_filter() -> None:
    id1 = icu.UnicodeString("Halfwidth-Fullwidth", -1)
    test1 = icu.Transliterator.create_instance(id1, icu.UTransDirection.UTRANS_FORWARD)
    src = icu.UnicodeString("123abc\uff71\uff72\uff73\uff74\uff75", -1)

    text = src.clone()
    test1.transliterate(text)
    assert text == "\uff11\uff12\uff13\uff41\uff42\uff43\u30a2\u30a4\u30a6\u30a8\u30aa"

    # void icu::Transliterator::adoptFilter(UnicodeFilter *adoptedFilter)
    uniset = icu.UnicodeSet(icu.UnicodeString("[^0-9]", -1))
    test1.adopt_filter(uniset)
    text = src.clone()
    test1.transliterate(text)
    assert text == "123\uff41\uff42\uff43\u30a2\u30a4\u30a6\u30a8\u30aa"

    test1.adopt_filter(None)
    text = src.clone()
    test1.transliterate(text)
    assert text == "\uff11\uff12\uff13\uff41\uff42\uff43\u30a2\u30a4\u30a6\u30a8\u30aa"


def test_api() -> None:
    id4 = icu.UnicodeString("Katakana-Latin", -1)
    trans = icu.Transliterator.create_instance(id4, icu.UTransDirection.UTRANS_FORWARD)

    # int32_t icu::Transliterator::countElements()
    assert trans.count_elements() == 3

    # const Transliterator &icu::Transliterator::getElement(
    #       int32_t index,
    #       UErrorCode &ec
    # )
    t0 = trans.get_element(0)
    assert isinstance(t0, icu.Transliterator)

    t1 = trans.get_element(1)
    assert isinstance(t1, icu.Transliterator)

    t2 = trans.get_element(2)
    assert isinstance(t2, icu.Transliterator)

    with pytest.raises(icu.ICUError) as exc_info:
        _ = trans.get_element(3)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INDEX_OUTOFBOUNDS_ERROR

    # const UnicodeFilter *icu::Transliterator::getFilter(void)
    uniset = trans.get_filter()
    assert isinstance(uniset, icu.UnicodeSet)
    assert len(uniset) > 0

    # const UnicodeString &icu::Transliterator::getID(void)
    result = trans.get_id()
    assert isinstance(result, icu.UnicodeString)
    assert result == id4

    # int32_t icu::Transliterator::getMaximumContextLength(void)
    assert trans.get_maximum_context_length() == 3

    # UnicodeSet &icu::Transliterator::getSourceSet(UnicodeSet &result)
    source = icu.UnicodeSet()
    result = trans.get_source_set(source)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(source)
    assert len(source) > 0

    # UnicodeSet &icu::Transliterator::getTargetSet(UnicodeSet &result)
    target = icu.UnicodeSet()
    result = trans.get_target_set(target)
    assert isinstance(result, icu.UnicodeSet)
    assert id(result) == id(target)
    assert len(target) > 0

    # UnicodeFilter *icu::Transliterator::orphanFilter(void)
    uniset2 = trans.orphan_filter()
    assert isinstance(uniset2, icu.UnicodeSet)
    assert len(uniset2) > 0

    # UnicodeString &icu::Transliterator::toRules(
    #       UnicodeString &result,
    #       UBool escapeUnprintable
    # )
    rules = icu.UnicodeString()
    result = trans.to_rules(rules, False)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(rules)
    assert len(rules) > 0


def test_clone() -> None:
    id1 = icu.UnicodeString("NFD;Jamo-Latin;Latin-Greek", -1)
    test1 = icu.Transliterator.create_instance(id1, icu.UTransDirection.UTRANS_FORWARD)

    # Transliterator *icu::Transliterator::clone()
    test2 = test1.clone()
    assert isinstance(test2, icu.Transliterator)
    assert test2.get_id() == test1.get_id()

    test3 = copy.copy(test1)
    assert test3.get_id() == test1.get_id()

    test4 = copy.deepcopy(test1)
    assert test4.get_id() == test1.get_id()


def test_create_instance() -> None:
    # static Transliterator *icu::Transliterator::createFromRules(
    #       const UnicodeString &ID,
    #       const UnicodeString &rules,
    #       UTransDirection dir,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    id3 = icu.UnicodeString("<ID>", -1)
    rules3 = icu.UnicodeString(
        "pre {alpha} post > | @ ALPHA ;eALPHA > beta ;pre {beta} post > BETA @@ | ;post > xyz",
        -1,
    )
    parse_error = icu.UParseError()
    test3 = icu.Transliterator.create_from_rules(
        id3, rules3, icu.UTransDirection.UTRANS_FORWARD, parse_error
    )
    assert isinstance(test3, icu.Transliterator)
    assert test3.get_id() == id3
    parsed_rules3 = icu.UnicodeString()
    test3.to_rules(parsed_rules3, True)

    test3a = icu.Transliterator.create_from_rules(
        str(id3), rules3, icu.UTransDirection.UTRANS_FORWARD, parse_error
    )
    assert isinstance(test3a, icu.Transliterator)
    assert test3a.get_id() == id3
    rules = icu.UnicodeString()
    assert test3a.to_rules(rules, True) == parsed_rules3

    test3b = icu.Transliterator.create_from_rules(
        id3, str(rules3), icu.UTransDirection.UTRANS_FORWARD, parse_error
    )
    assert isinstance(test3b, icu.Transliterator)
    assert test3b.get_id() == id3
    rules = icu.UnicodeString()
    assert test3b.to_rules(rules, True) == parsed_rules3

    test3c = icu.Transliterator.create_from_rules(
        str(id3), str(rules3), icu.UTransDirection.UTRANS_FORWARD, parse_error
    )
    assert isinstance(test3c, icu.Transliterator)
    assert test3c.get_id() == id3
    rules = icu.UnicodeString()
    assert test3c.to_rules(rules, True) == parsed_rules3

    # [1]
    # static Transliterator *icu::Transliterator::createInstance(
    #       const UnicodeString &ID,
    #       UTransDirection dir,
    #       UErrorCode &status
    # )
    id1 = icu.UnicodeString("NFD;Jamo-Latin;Latin-Greek", -1)
    test1 = icu.Transliterator.create_instance(id1, icu.UTransDirection.UTRANS_FORWARD)
    assert isinstance(test1, icu.Transliterator)
    assert test1.get_id() == id1
    parsed_rules1 = icu.UnicodeString()
    test1.to_rules(parsed_rules1, True)

    test1a = icu.Transliterator.create_instance(str(id1), icu.UTransDirection.UTRANS_FORWARD)
    assert isinstance(test1a, icu.Transliterator)
    assert test1a.get_id() == id1
    rules = icu.UnicodeString()
    assert test1a.to_rules(rules, True) == parsed_rules1

    # [2]
    # static Transliterator *icu::Transliterator::createInstance(
    #       const UnicodeString &ID,
    #       UTransDirection dir,
    #       UParseError &parseError,
    #       UErrorCode &status
    # )
    parse_error = icu.UParseError()
    test2 = icu.Transliterator.create_instance(
        id1, icu.UTransDirection.UTRANS_FORWARD, parse_error
    )
    assert isinstance(test2, icu.Transliterator)
    assert test2.get_id() == id1
    parsed_rules1 = icu.UnicodeString()
    test2.to_rules(parsed_rules1, True)

    test2a = icu.Transliterator.create_instance(
        str(id1), icu.UTransDirection.UTRANS_FORWARD, parse_error
    )
    assert isinstance(test2a, icu.Transliterator)
    assert test2a.get_id() == id1
    rules = icu.UnicodeString()
    assert test2a.to_rules(rules, True) == parsed_rules1

    # Transliterator *icu::Transliterator::createInverse(UErrorCode &status)
    id4 = icu.UnicodeString("Hiragana-Katakana", -1)
    test4 = icu.Transliterator.create_instance(id4, icu.UTransDirection.UTRANS_FORWARD)
    test5 = test4.create_inverse()
    assert isinstance(test5, icu.Transliterator)
    assert test5.get_id() == "Katakana-Hiragana"


def test_get_available_ids() -> None:
    # static StringEnumeration *icu::Transliterator::getAvailableIDs(
    #       UErrorCode &ec
    # )
    ids = icu.Transliterator.get_available_ids()
    assert isinstance(ids, icu.StringEnumeration)
    assert len(ids) > 0
    assert "Hiragana-Katakana" in ids


def test_get_available_variant() -> None:
    source = icu.UnicodeString()
    target = icu.UnicodeString()
    variant = icu.UnicodeString()
    count = 0

    # static int32_t icu::Transliterator::countAvailableSources(void)
    for i in range(icu.Transliterator.count_available_sources()):
        # static UnicodeString &icu::Transliterator::getAvailableSource(
        #       int32_t index,
        #       UnicodeString &result
        # )
        result = icu.Transliterator.get_available_source(i, source)
        assert isinstance(result, icu.UnicodeString)
        assert id(result) == id(source)
        assert len(source) > 0

        # static int32_t icu::Transliterator::countAvailableTargets(
        #       const UnicodeString &source
        # )
        targets = icu.Transliterator.count_available_targets(source)
        assert targets == icu.Transliterator.count_available_targets(str(source))
        for j in range(targets):
            # static UnicodeString &icu::Transliterator::getAvailableTarget(
            #       int32_t index,
            #       const UnicodeString &source,
            #       UnicodeString &result
            # )
            result = icu.Transliterator.get_available_target(j, source, target)
            assert isinstance(result, icu.UnicodeString)
            assert id(result) == id(target)
            assert len(target) > 0

            assert result == icu.Transliterator.get_available_target(j, str(source), target)

            # static int32_t icu::Transliterator::countAvailableVariants(
            #       const UnicodeString &source,
            #       const UnicodeString &target
            # )
            variants = icu.Transliterator.count_available_variants(source, target)
            assert variants == icu.Transliterator.count_available_variants(str(source), target)
            assert variants == icu.Transliterator.count_available_variants(source, str(target))
            assert variants == icu.Transliterator.count_available_variants(
                str(source), str(target)
            )

            for k in range(variants):
                # static UnicodeString &
                # icu::Transliterator::getAvailableVariant(
                #       int32_t index,
                #       const UnicodeString &source,
                #       const UnicodeString &target,
                #       UnicodeString &result
                # )
                result = icu.Transliterator.get_available_variant(k, source, target, variant)
                assert isinstance(result, icu.UnicodeString)
                assert id(result) == id(variant)
                assert len(variant) >= 0

                assert result == icu.Transliterator.get_available_variant(
                    k, str(source), target, variant
                )
                assert result == icu.Transliterator.get_available_variant(
                    k, source, str(target), variant
                )
                assert result == icu.Transliterator.get_available_variant(
                    k, str(source), str(target), variant
                )
                count += 1
    assert count > 0


def test_get_display_name() -> None:
    # [1]
    # static UnicodeString &icu::Transliterator::getDisplayName(
    #       const UnicodeString &ID,
    #       const Locale &inLocale,
    #       UnicodeString &result
    # )
    display_name = icu.UnicodeString()
    result = icu.Transliterator.get_display_name(
        icu.UnicodeString("Hiragana-Katakana", -1),
        icu.Locale.get_us(),
        display_name,
    )
    assert result == display_name
    assert result == "Hiragana to Katakana"

    display_name.remove()
    result = icu.Transliterator.get_display_name(
        "Hiragana-Katakana", icu.Locale.get_us(), display_name
    )
    assert result == display_name
    assert result == "Hiragana to Katakana"

    display_name.remove()
    result = icu.Transliterator.get_display_name(
        icu.UnicodeString("Hiragana-Katakana", -1), "en_US", display_name
    )
    assert result == display_name
    assert result == "Hiragana to Katakana"

    display_name.remove()
    result = icu.Transliterator.get_display_name("Hiragana-Katakana", "en_US", display_name)
    assert result == display_name
    assert result == "Hiragana to Katakana"

    # [2]
    # static UnicodeString &icu::Transliterator::getDisplayName(
    #       const UnicodeString &ID,
    #       UnicodeString &result
    # )
    result = icu.Transliterator.get_display_name(
        icu.UnicodeString("Hiragana-Katakana", -1), display_name
    )
    assert result == display_name
    assert len(result) > 0

    display_name.remove()
    result = icu.Transliterator.get_display_name("Hiragana-Katakana", display_name)
    assert result == display_name
    assert len(result) > 0


def test_register_instance() -> None:
    id3 = icu.UnicodeString("Any-MyTransliterator", -1)
    rules3 = icu.UnicodeString(
        "pre {alpha} post > | @ ALPHA ;eALPHA > beta ;pre {beta} post > BETA @@ | ;post > xyz",
        -1,
    )
    parse_error = icu.UParseError()
    test3 = icu.Transliterator.create_from_rules(
        id3, rules3, icu.UTransDirection.UTRANS_FORWARD, parse_error
    )

    ids = icu.Transliterator.get_available_ids()
    assert str(id3) not in ids

    # static void icu::Transliterator::registerInstance(
    #       Transliterator *adoptedObj
    # )
    icu.Transliterator.register_instance(test3)

    # static void icu::Transliterator::registerAlias(
    #       const UnicodeString &aliasID,
    #       const UnicodeString &realID
    # )
    id3a = icu.UnicodeString("Any-MyRule", -1)
    icu.Transliterator.register_alias(id3a, id3)

    ids = icu.Transliterator.get_available_ids()
    assert str(id3) in ids
    assert str(id3a) in ids

    test3a = icu.Transliterator.create_instance(id3a, icu.UTransDirection.UTRANS_FORWARD)
    assert test3a.get_id() == id3a

    text = icu.UnicodeString("prealphapost prebetapost", -1)
    test3a.transliterate(text)
    assert text == "prbetaxyz preBETApost"

    # static void icu::Transliterator::unregister(const UnicodeString &ID)
    icu.Transliterator.unregister(id3)
    icu.Transliterator.unregister(id3a)
    ids = icu.Transliterator.get_available_ids()
    assert str(id3) not in ids
    assert str(id3a) not in ids

    icu.Transliterator.register_instance(test3)
    icu.Transliterator.register_alias("Any-MyRule1", id3)
    icu.Transliterator.register_alias(icu.UnicodeString("Any-MyRule2"), str(id3))
    icu.Transliterator.register_alias("Any-MyRule3", str(id3))
    ids = icu.Transliterator.get_available_ids()
    assert "Any-MyRule1" in ids
    assert "Any-MyRule2" in ids
    assert "Any-MyRule3" in ids


def test_subclass_filtered_transliterate() -> None:
    tid = "Halfwidth-Fullwidth; Lower; Hiragana-Katakana; Katakana-Latin"
    filter_set = icu.UnicodeSet("[^0-9]")
    t = _TestTrans2(tid, filter_set)
    assert t.get_id() == "Halfwidth-Fullwidth;Any-Lower;Hira-Kana;Kana-Latn"
    assert t.get_filter() == filter_set
    assert t.get_maximum_context_length() == 3
    assert t.count_elements() == 0

    text = icu.UnicodeString("ABC123\uff11\uff21\u3042\u30a2\uff71", -1)  # "ABC123１Ａあアｱ"

    index = icu.UTransPosition()
    index.context_limit = index.limit = len(text)
    t.transliterate(text, index)
    assert text == "\uff41\uff42\uff43123\uff11\uff41aaa"  # "ａｂｃ123１ａaaa"
    assert t.num_calls == 6


def test_subclass_handle_get_source_set() -> None:
    tid = "Seoridf-Sweorie"
    t1 = _TestTrans(tid)

    # virtual void icu::Transliterator::handleGetSourceSet(
    #       UnicodeSet &result
    # ) const
    result = icu.UnicodeSet("[A-z0-9]")
    t1.handle_get_source_set(result)
    assert result.size() == 0  # default implementation returns empty set.

    result = icu.UnicodeSet("[A-z0-9]")
    t1.get_source_set(result)
    assert result.size() == 0

    result = icu.UnicodeSet("[A-z0-9]")
    t1.get_target_set(result)
    assert result.size() == 0  # default implementation returns empty set.

    tid = "Halfwidth-Fullwidth"
    filter_set = icu.UnicodeSet("[^0-9]")
    t2 = _TestTrans2(tid, filter_set)

    result = icu.UnicodeSet("[A-z]")
    t2.handle_get_source_set(result)
    assert result.size() > 0
    assert result.contains(0x30, 0x39)
    assert not result.contains(0xFF10, 0xFF19)  # "０" - "９" (FULLWIDTH DIGIT)

    result = icu.UnicodeSet("[A-z0-9]")
    t2.get_source_set(result)
    assert result.size() > 0
    assert not result.contains(0x30, 0x39)
    assert not result.contains(0xFF10, 0xFF19)  # "０" - "９" (FULLWIDTH DIGIT)

    result = icu.UnicodeSet("[A-z0-9]")
    t2.get_target_set(result)
    assert result.size() > 0
    assert not result.contains(0x30, 0x39)
    assert result.contains(0xFF10, 0xFF19)  # "０" - "９" (FULLWIDTH DIGIT)


def test_subclass_register_instance() -> None:
    tid = "Seoridf-Sweorie"
    t = _TestTrans(tid)
    assert t.get_id() == tid

    icu.Transliterator.register_instance(t)

    t2 = icu.Transliterator.create_instance(tid, icu.UTransDirection.UTRANS_FORWARD)
    assert isinstance(t2, icu.Transliterator)
    assert t2.get_id() == tid

    icu.Transliterator.unregister(tid)

    with pytest.raises(icu.ICUError) as exc_info:
        _ = icu.Transliterator.create_instance(tid, icu.UTransDirection.UTRANS_FORWARD)
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_ID


def test_subclass_transliterate() -> None:
    tid = "Seoridf-Sweorie"
    t = _TestTrans(tid)
    assert t.get_id() == tid
    assert t.get_filter() is None

    text = icu.UnicodeString("abc123", -1)
    index = icu.UTransPosition()
    index.context_limit = index.limit = len(text)
    t.transliterate(text, index)
    assert index.start != 0

    t.finish_transliteration(text, index)
    assert index.start == index.limit
    assert t.num_calls == 6


def test_transliterate() -> None:
    id4 = icu.UnicodeString("Hiragana-Katakana", -1)
    test4 = icu.Transliterator.create_instance(id4, icu.UTransDirection.UTRANS_FORWARD)
    src = icu.UnicodeString("\u3042\u3044\u3046\u3048\u304a", -1)

    # [1]
    # void icu::Transliterator::transliterate(Replaceable &text)
    text = src.clone()
    test4.transliterate(text)
    assert text == "\u30a2\u30a4\u30a6\u30a8\u30aa"

    # [2]
    # int32_t icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       int32_t start,
    #       int32_t limit
    # )
    text = src.clone()
    result = test4.transliterate(text, 1, 3)
    assert text == "\u3042\u30a4\u30a6\u3048\u304a"
    assert result == 3

    # [3]
    # void icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       UTransPosition &index,
    #       const UnicodeString &insertion,
    #       UErrorCode &status
    # )
    text = src.clone()
    index = icu.UTransPosition()
    index.context_start = 0
    index.context_limit = 5
    index.start = 1
    index.limit = 4
    insertion = icu.UnicodeString("_", -1)
    test4.transliterate(text, index, insertion)
    assert index.context_start == 0
    assert index.context_limit == 6
    assert index.start == 5
    assert index.limit == 5
    assert text == "\u3042\u30a4\u30a6\u30a8_\u304a"

    text = src.clone()
    index = icu.UTransPosition(0, 5, 1, 4)
    test4.transliterate(text, index, "_")
    assert index.context_start == 0
    assert index.context_limit == 6
    assert index.start == 5
    assert index.limit == 5
    assert text == "\u3042\u30a4\u30a6\u30a8_\u304a"

    # [4]
    # void icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       UTransPosition &index,
    #       UChar32 insertion,
    #       UErrorCode &status
    # )
    text = src.clone()
    index = icu.UTransPosition(0, 5, 1, 4)
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
    assert text == "\u3042\u30a4\u30a6\u30a8_\u304a"

    # [5]
    # void icu::Transliterator::transliterate(
    #       Replaceable &text,
    #       UTransPosition &index,
    #       UErrorCode &status
    # )
    text = src.clone()
    # UTransPosition(context_start, start, limit, context_limit)
    index = icu.UTransPosition(0, 5, 1, 4)
    assert index.context_start == 0
    assert index.context_limit == 5
    assert index.start == 1
    assert index.limit == 4
    test4.transliterate(text, index)
    assert index.context_start == 0
    assert index.context_limit == 5
    assert index.start == 4
    assert index.limit == 4
    assert text == "\u3042\u30a4\u30a6\u30a8\u304a"

    # UTransPosition.__repr__() -> str
    assert repr(index) == ("UTransPosition(context_start=0, context_limit=5, start=4, limit=4)")

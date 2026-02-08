from icupy import icu
from icupy.utils import gc


def test_api() -> None:
    text = "bahrain \u0645\u0635\u0631 kuwait"

    # UBiDi *ubidi_open(void)
    # void ubidi_close(UBiDi *pBiDi)
    with gc(icu.ubidi_open(), icu.ubidi_close) as bidi:
        # const UChar *ubidi_getText(const UBiDi *pBiDi)
        result = icu.ubidi_get_text(bidi)
        assert result is None

        # void ubidi_setPara(UBiDi *pBiDi,
        #                    const UChar *text,
        #                    int32_t length,
        #                    UBiDiLevel paraLevel,
        #                    UBiDiLevel *embeddingLevels,
        #                    UErrorCode *pErrorCode
        # )
        icu.ubidi_set_para(bidi, text, -1, icu.UBIDI_DEFAULT_LTR)

        # int32_t ubidi_countParagraphs(UBiDi *pBiDi)
        assert icu.ubidi_count_paragraphs(bidi) == 1

        # int32_t ubidi_countRuns(UBiDi *pBiDi,
        #                         UErrorCode *pErrorCode
        # )
        assert icu.ubidi_count_runs(bidi) == 3

        # UBiDiDirection ubidi_getBaseDirection(const UChar *text,
        #                                       int32_t length
        # )
        assert icu.ubidi_get_base_direction(text, -1) == icu.UBiDiDirection.UBIDI_LTR

        # UBiDiDirection ubidi_getDirection(const UBiDi *pBiDi)
        assert icu.ubidi_get_direction(bidi) == icu.UBiDiDirection.UBIDI_MIXED

        # int32_t ubidi_getLength(const UBiDi *pBiDi)
        assert icu.ubidi_get_length(bidi) == 18

        # UBiDiLevel ubidi_getLevelAt(const UBiDi *pBiDi,
        #                             int32_t charIndex
        # )
        assert icu.ubidi_get_level_at(bidi, 0) == 0
        # ...
        assert icu.ubidi_get_level_at(bidi, 7) == 0
        assert icu.ubidi_get_level_at(bidi, 8) == 1
        assert icu.ubidi_get_level_at(bidi, 9) == 1
        assert icu.ubidi_get_level_at(bidi, 10) == 1
        assert icu.ubidi_get_level_at(bidi, 11) == 0
        # ...
        assert icu.ubidi_get_level_at(bidi, 17) == 0

        # const UBiDiLevel *ubidi_getLevels(UBiDi *pBiDi,
        #                                   UErrorCode *pErrorCode
        # )
        levels = icu.ubidi_get_levels(bidi)
        assert levels == [
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1,
            0, 0, 0, 0, 0, 0, 0,
        ]  # fmt: skip

        # int32_t ubidi_getLogicalIndex(UBiDi *pBiDi,
        #                               int32_t visualIndex,
        #                               UErrorCode *pErrorCode
        # )
        assert icu.ubidi_get_logical_index(bidi, 0) == 0
        # ...
        assert icu.ubidi_get_logical_index(bidi, 7) == 7
        assert icu.ubidi_get_logical_index(bidi, 8) == 10
        assert icu.ubidi_get_logical_index(bidi, 9) == 9
        assert icu.ubidi_get_logical_index(bidi, 10) == 8
        assert icu.ubidi_get_logical_index(bidi, 11) == 11
        # ...
        assert icu.ubidi_get_logical_index(bidi, 17) == 17

        # void ubidi_getLogicalMap(UBiDi *pBiDi,
        #                          int32_t *indexMap,
        #                          UErrorCode *pErrorCode
        # )
        logical_map = icu.ubidi_get_logical_map(bidi)
        assert logical_map == [
            0, 1, 2, 3, 4, 5, 6, 7,
            10, 9, 8,
            11, 12, 13, 14, 15, 16, 17,
        ]  # fmt: skip

        # void ubidi_getLogicalRun(const UBiDi *pBiDi,
        #                          int32_t logicalPosition,
        #                          int32_t *pLogicalLimit,
        #                          UBiDiLevel *pLevel
        # )
        limit, level = icu.ubidi_get_logical_run(bidi, 0)
        assert limit == 8
        assert level == 0
        limit, level = icu.ubidi_get_logical_run(bidi, 8)
        assert limit == 11
        assert level == 1
        limit, level = icu.ubidi_get_logical_run(bidi, 11)
        assert limit == 18
        assert level == 0

        # int32_t ubidi_getParagraph(const UBiDi *pBiDi,
        #                            int32_t charIndex,
        #                            int32_t *pParaStart,
        #                            int32_t *pParaLimit,
        #                            UBiDiLevel *pParaLevel,
        #                            UErrorCode *pErrorCode
        # )
        result, start, limit, level = icu.ubidi_get_paragraph(bidi, 0)
        assert (result, start, limit, level) == (0, 0, 18, 0)

        # void ubidi_getParagraphByIndex(const UBiDi *pBiDi,
        #                                int32_t paraIndex,
        #                                int32_t *pParaStart,
        #                                int32_t *pParaLimit,
        #                                UBiDiLevel *pParaLevel,
        #                                UErrorCode *pErrorCode
        # )
        start, limit, level = icu.ubidi_get_paragraph_by_index(bidi, 0)
        assert (start, limit, level) == (0, 18, 0)

        # UBiDiLevel ubidi_getParaLevel(const UBiDi *pBiDi)
        assert icu.ubidi_get_para_level(bidi) == 0

        # int32_t ubidi_getProcessedLength(const UBiDi *pBiDi)
        assert icu.ubidi_get_processed_length(bidi) == 18

        # UBiDiReorderingMode ubidi_getReorderingMode(UBiDi *pBiDi)
        assert (
            icu.ubidi_get_reordering_mode(bidi) == icu.UBiDiReorderingMode.UBIDI_REORDER_DEFAULT
        )

        # uint32_t ubidi_getReorderingOptions(UBiDi *pBiDi)
        assert (
            icu.ubidi_get_reordering_options(bidi)
            == icu.UBiDiReorderingOption.UBIDI_OPTION_DEFAULT
        )

        # int32_t ubidi_getResultLength(const UBiDi *pBiDi)
        assert icu.ubidi_get_result_length(bidi) == 18

        # const UChar *ubidi_getText(const UBiDi *pBiDi)
        result = icu.ubidi_get_text(bidi)
        assert result == text

        # int32_t ubidi_getVisualIndex(UBiDi *pBiDi,
        #                              int32_t logicalIndex,
        #                              UErrorCode *pErrorCode
        # )
        assert icu.ubidi_get_visual_index(bidi, 0) == 0
        # ...
        assert icu.ubidi_get_visual_index(bidi, 7) == 7
        assert icu.ubidi_get_visual_index(bidi, 8) == 10
        assert icu.ubidi_get_visual_index(bidi, 9) == 9
        assert icu.ubidi_get_visual_index(bidi, 10) == 8
        assert icu.ubidi_get_visual_index(bidi, 11) == 11
        # ...
        assert icu.ubidi_get_visual_index(bidi, 17) == 17

        # void ubidi_getVisualMap(UBiDi *pBiDi,
        #                         int32_t *indexMap,
        #                         UErrorCode *pErrorCode
        # )
        visual_map = icu.ubidi_get_visual_map(bidi)
        assert visual_map == [
            0, 1, 2, 3, 4, 5, 6, 7,
            10, 9, 8,
            11, 12, 13, 14, 15, 16, 17,
        ]  # fmt: skip

        # UBiDiDirection ubidi_getVisualRun(UBiDi *pBiDi,
        #                                   int32_t runIndex,
        #                                   int32_t *pLogicalStart,
        #                                   int32_t *pLength
        # )
        result, start, length = icu.ubidi_get_visual_run(bidi, 0)
        assert result == icu.UBiDiDirection.UBIDI_LTR
        assert start == 0
        assert length == 8
        result, start, length = icu.ubidi_get_visual_run(bidi, 1)
        assert result == icu.UBiDiDirection.UBIDI_RTL
        assert start == 8
        assert length == 3
        result, start, length = icu.ubidi_get_visual_run(bidi, 2)
        assert result == icu.UBiDiDirection.UBIDI_LTR
        assert start == 11
        assert length == 7

        # void ubidi_invertMap(const int32_t *srcMap,
        #                      int32_t *destMap,
        #                      int32_t length
        # )
        src_map = [0, 1, 2, 4]
        dest_map = icu.ubidi_invert_map(src_map, len(src_map))
        assert dest_map == [0, 1, 2, icu.UBIDI_MAP_NOWHERE]

        # UBool ubidi_isInverse(UBiDi *pBiDi)
        assert icu.ubidi_is_inverse(bidi) is False

        # UBool ubidi_isOrderParagraphsLTR(UBiDi *pBiDi)
        assert icu.ubidi_is_order_paragraphs_ltr(bidi) is False

        # void ubidi_orderParagraphsLTR(UBiDi *pBiDi,
        #                               UBool orderParagraphsLTR
        # )
        icu.ubidi_order_paragraphs_ltr(bidi, True)
        icu.ubidi_set_para(bidi, text, -1, icu.UBIDI_DEFAULT_LTR)
        assert icu.ubidi_is_order_paragraphs_ltr(bidi) is True

        # void ubidi_reorderLogical(const UBiDiLevel *levels,
        #                           int32_t length,
        #                           int32_t *indexMap
        # )
        logical_map2 = icu.ubidi_reorder_logical(levels, len(levels))
        assert logical_map2 == logical_map

        # void ubidi_reorderVisual(const UBiDiLevel *levels,
        #                          int32_t length,
        #                          int32_t *indexMap
        # )
        visual_map2 = icu.ubidi_reorder_visual(levels, len(levels))
        assert visual_map2 == visual_map

        # void ubidi_setInverse(UBiDi *pBiDi,
        #                       UBool isInverse
        # )
        icu.ubidi_set_inverse(bidi, True)
        icu.ubidi_set_para(bidi, text, -1, icu.UBIDI_DEFAULT_LTR)
        assert (
            icu.ubidi_get_reordering_mode(bidi)
            == icu.UBiDiReorderingMode.UBIDI_REORDER_INVERSE_NUMBERS_AS_L
        )

        # void ubidi_setReorderingMode(UBiDi *pBiDi,
        #                              UBiDiReorderingMode reorderingMode
        # )
        icu.ubidi_set_reordering_mode(bidi, icu.UBiDiReorderingMode.UBIDI_REORDER_DEFAULT)
        assert (
            icu.ubidi_get_reordering_mode(bidi) == icu.UBiDiReorderingMode.UBIDI_REORDER_DEFAULT
        )

        # void ubidi_setReorderingOptions(UBiDi *pBiDi,
        #                                 uint32_t reorderingOptions
        # )
        icu.ubidi_set_reordering_options(
            bidi, icu.UBiDiReorderingOption.UBIDI_OPTION_INSERT_MARKS
        )
        assert (
            icu.ubidi_get_reordering_options(bidi)
            == icu.UBiDiReorderingOption.UBIDI_OPTION_INSERT_MARKS
        )


def test_set_class_callback() -> None:
    # From icu/source/test/intltest/bidiconf.cpp
    char_from_bidi_class = [
        0x6C,  # U_LEFT_TO_RIGHT
        0x52,  # U_RIGHT_TO_LEFT
        0x33,  # U_EUROPEAN_NUMBER
        0x2D,  # U_EUROPEAN_NUMBER_SEPARATOR
        0x25,  # U_EUROPEAN_NUMBER_TERMINATOR
        0x39,  # U_ARABIC_NUMBER
        0x2C,  # U_COMMON_NUMBER_SEPARATOR
        0x2F,  # U_BLOCK_SEPARATOR
        0x5F,  # U_SEGMENT_SEPARATOR
        0x20,  # U_WHITE_SPACE_NEUTRAL
        0x3D,  # U_OTHER_NEUTRAL
    ]

    # UCharDirection UBiDiClassCallback(const void *context,
    #                                   UChar32 c)
    def _callback1(_: object, _c: int) -> icu.UCharDirection:
        nonlocal char_from_bidi_class
        if _c in char_from_bidi_class:
            _n = char_from_bidi_class.index(_c)
        else:
            _n = icu.u_get_int_property_max_value(icu.UProperty.UCHAR_BIDI_CLASS) + 1
        return icu.UCharDirection(_n)

    def _callback2(_context: list[int], _c: int) -> icu.UCharDirection:
        assert isinstance(_context, list)
        if _c in _context:
            _n = _context.index(_c)
        else:
            _n = icu.u_get_int_property_max_value(icu.UProperty.UCHAR_BIDI_CLASS) + 1
        return icu.UCharDirection(_n)

    with gc(icu.ubidi_open(), icu.ubidi_close) as bidi:
        # void ubidi_getClassCallback(UBiDi *pBiDi,
        #                             UBiDiClassCallback **fn,
        #                             const void **context
        # )
        old_fn1 = icu.ubidi_get_class_callback(bidi)
        assert old_fn1 is None

        assert icu.ubidi_get_customized_class(bidi, 0x52) == icu.UCharDirection.U_LEFT_TO_RIGHT
        assert (
            icu.ubidi_get_customized_class(bidi, 0x39) == icu.UCharDirection.U_EUROPEAN_NUMBER
        )

        # void ubidi_setClassCallback(UBiDi *pBiDi,
        #                             UBiDiClassCallback *newFn,
        #                             const void *newContext,
        #                             UBiDiClassCallback **oldFn,
        #                             const void **oldContext,
        #                             UErrorCode *pErrorCode
        # )
        fn2 = icu.UBiDiClassCallback(_callback1)
        assert bool(fn2)
        old_fn1 = icu.ubidi_set_class_callback(bidi, fn2)
        assert old_fn1 is None

        assert icu.ubidi_get_customized_class(bidi, 0x52) == icu.UCharDirection.U_RIGHT_TO_LEFT
        assert icu.ubidi_get_customized_class(bidi, 0x39) == icu.UCharDirection.U_ARABIC_NUMBER

        fn2a = icu.ubidi_get_class_callback(bidi)
        assert isinstance(fn2a, icu.UBiDiClassCallback)
        assert bool(fn2a)
        assert fn2a.context() is None

        # old_fn3, old_context3 = icu.ubidi_set_class_callback(bidi, old_fn1, old_context1)
        # result = icu.ubidi_get_customized_class(bidi, 0x52)
        # assert result == icu.UCharDirection.U_LEFT_TO_RIGHT
        # result = icu.ubidi_get_customized_class(bidi, 0x39)
        # assert result == icu.UCharDirection.U_EUROPEAN_NUMBER

        # icu.ubidi_set_class_callback(bidi, fn2a, context2a)
        # result = icu.ubidi_get_customized_class(bidi, 0x52)
        # assert result == icu.UCharDirection.U_RIGHT_TO_LEFT
        # result = icu.ubidi_get_customized_class(bidi, 0x39)
        # assert result == icu.UCharDirection.U_ARABIC_NUMBER

        context4 = icu.ConstVoidPtr(char_from_bidi_class)
        fn4 = icu.UBiDiClassCallback(_callback2, context4)
        old_fn2 = icu.ubidi_set_class_callback(bidi, fn4)
        assert isinstance(old_fn2, icu.UBiDiClassCallback)
        assert bool(old_fn2)
        assert old_fn2.context() is None

        assert icu.ubidi_get_customized_class(bidi, 0x52) == icu.UCharDirection.U_RIGHT_TO_LEFT
        assert icu.ubidi_get_customized_class(bidi, 0x39) == icu.UCharDirection.U_ARABIC_NUMBER

        # icu.ubidi_set_class_callback(bidi, old_fn2)

        # result = icu.ubidi_get_customized_class(bidi, 0x52)
        # assert result == icu.UCharDirection.U_LEFT_TO_RIGHT
        # result = icu.ubidi_get_customized_class(bidi, 0x39)
        # assert result == icu.UCharDirection.U_EUROPEAN_NUMBER

        old_fn4 = icu.ubidi_set_class_callback(bidi, old_fn2)
        assert isinstance(old_fn4, icu.UBiDiClassCallback)
        assert bool(old_fn4)
        old_context4 = old_fn4.context()
        assert isinstance(old_context4, icu.ConstVoidPtr)
        assert old_context4.value() == char_from_bidi_class

        assert icu.ubidi_get_customized_class(bidi, 0x52) == icu.UCharDirection.U_RIGHT_TO_LEFT
        assert icu.ubidi_get_customized_class(bidi, 0x39) == icu.UCharDirection.U_ARABIC_NUMBER

        fn5 = icu.UBiDiClassCallback()
        assert not bool(fn5)
        icu.ubidi_set_class_callback(bidi, fn5)

        assert icu.ubidi_get_customized_class(bidi, 0x52) == icu.UCharDirection.U_LEFT_TO_RIGHT
        assert (
            icu.ubidi_get_customized_class(bidi, 0x39) == icu.UCharDirection.U_EUROPEAN_NUMBER
        )

        _ = icu.ubidi_set_class_callback(bidi, fn2a)

        assert icu.ubidi_get_customized_class(bidi, 0x52) == icu.UCharDirection.U_RIGHT_TO_LEFT
        assert icu.ubidi_get_customized_class(bidi, 0x39) == icu.UCharDirection.U_ARABIC_NUMBER


def test_set_context() -> None:
    with gc(icu.ubidi_open(), icu.ubidi_close) as bidi:
        prologue = "<code>abc \u05d0\u05d1</code>"
        text = "<code>\u05d2\u05d3\u05d4 xyz</code>"
        para_level = 1

        icu.ubidi_set_para(bidi, text, -1, para_level)
        result1 = icu.ubidi_get_logical_map(bidi)

        # void ubidi_setContext(UBiDi *pBiDi,
        #                       const UChar *prologue,
        #                       int32_t proLength,
        #                       const UChar *epilogue,
        #                       int32_t epiLength,
        #                       UErrorCode *pErrorCode
        # )
        icu.ubidi_set_context(bidi, prologue, -1, None, 0)
        icu.ubidi_set_para(bidi, text, -1, para_level)
        result2 = icu.ubidi_get_logical_map(bidi)
        assert result1 != result2

        prologue = "a"
        epilogue = "b"
        text = ".-=\u05da\u05db\u05dc-+*"
        expected = ".-=\u05dc\u05db\u05da-+*"
        para_level = icu.UBiDiDirection.UBIDI_LTR
        icu.ubidi_set_context(bidi, prologue, -1, epilogue, -1)
        icu.ubidi_set_para(bidi, text, -1, para_level)
        dest = icu.ubidi_write_reordered(bidi, icu.UBIDI_DO_MIRRORING)
        assert dest == expected

        prologue = "\u0634"
        epilogue = "\u05d7"
        text = ".-=abc-+*"
        expected = "*+-abc=-."
        para_level = icu.UBiDiDirection.UBIDI_RTL
        icu.ubidi_set_context(bidi, prologue, -1, epilogue, -1)
        icu.ubidi_set_para(bidi, text, -1, para_level)
        dest = icu.ubidi_write_reordered(bidi, icu.UBIDI_DO_MIRRORING)
        assert dest == expected


def test_set_line() -> None:
    text = "bahrain \u0645\u0635\u0631 kuwait"
    length = icu.u_strlen(text)

    # UBiDi *ubidi_openSized(int32_t maxLength,
    #                        int32_t maxRunCount,
    #                        UErrorCode *pErrorCode
    # )
    # void ubidi_close(UBiDi *pBiDi)
    with (
        gc(icu.ubidi_open_sized(length, 0), icu.ubidi_close) as para,
        gc(icu.ubidi_open_sized(length, 0), icu.ubidi_close) as line,
    ):
        icu.ubidi_set_para(para, text, -1, icu.UBIDI_DEFAULT_LTR)
        assert icu.ubidi_get_direction(para) == icu.UBiDiDirection.UBIDI_MIXED

        start = 0
        limit, _ = icu.ubidi_get_logical_run(para, start)
        assert limit == 8

        # void ubidi_setLine(const UBiDi *pParaBiDi,
        #                    int32_t start,
        #                    int32_t limit,
        #                    UBiDi *pLineBiDi,
        #                    UErrorCode *pErrorCode
        # )
        icu.ubidi_set_line(para, start, limit, line)
        assert icu.ubidi_get_direction(line) == icu.UBiDiDirection.UBIDI_LTR

        start = limit
        limit, _ = icu.ubidi_get_logical_run(para, start)
        assert limit == 11

        icu.ubidi_set_line(para, start, limit, line)
        assert icu.ubidi_get_direction(line) == icu.UBiDiDirection.UBIDI_RTL

        start = limit
        limit, _ = icu.ubidi_get_logical_run(para, start)
        assert limit == 18

        icu.ubidi_set_line(para, start, limit, line)
        assert icu.ubidi_get_direction(line) == icu.UBiDiDirection.UBIDI_LTR


def test_set_para() -> None:
    text = "car means CAR."
    length = icu.u_strlen(text)

    with gc(icu.ubidi_open_sized(length, 0), icu.ubidi_close) as bidi:
        # void ubidi_setPara(UBiDi *pBiDi,
        #                    const UChar *text,
        #                    int32_t length,
        #                    UBiDiLevel paraLevel,
        #                    UBiDiLevel *embeddingLevels,
        #                    UErrorCode *pErrorCode
        # )
        # fmt: off
        embedding_levels = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1 | icu.UBIDI_LEVEL_OVERRIDE,
            1 | icu.UBIDI_LEVEL_OVERRIDE,
            1 | icu.UBIDI_LEVEL_OVERRIDE,
            0,
        ]
        # fmt: on
        icu.ubidi_set_para(bidi, text, length, icu.UBIDI_DEFAULT_LTR, embedding_levels)

        levels = icu.ubidi_get_levels(bidi)
        assert levels == [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1,
            0,
        ]  # fmt: skip

        result = icu.ubidi_get_logical_map(bidi)
        assert result == [
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            12, 11, 10,
            13,
        ]  # fmt: skip

        result = icu.ubidi_get_visual_map(bidi)
        assert result == [
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            12, 11, 10,
            13,
        ]  # fmt: skip


def test_write_reordered() -> None:
    text = "bahrain \u0645\u0635\u0631 kuwait"
    length = icu.u_strlen(text)

    with gc(icu.ubidi_open_sized(length, 0), icu.ubidi_close) as bidi:
        icu.ubidi_set_para(bidi, text, -1, icu.UBIDI_DEFAULT_LTR)

        # int32_t ubidi_writeReordered(UBiDi *pBiDi,
        #                              UChar *dest,
        #                              int32_t destSize,
        #                              uint16_t options,
        #                              UErrorCode *pErrorCode
        # )
        options = 0
        dest = icu.ubidi_write_reordered(bidi, options)
        assert dest == "bahrain \u0631\u0635\u0645 kuwait"

        # LRM: U+200E
        icu.ubidi_set_inverse(bidi, True)
        icu.ubidi_set_para(bidi, text, -1, icu.UBIDI_DEFAULT_LTR)
        options = icu.UBIDI_INSERT_LRM_FOR_NUMERIC | icu.UBIDI_DO_MIRRORING
        dest = icu.ubidi_write_reordered(bidi, options)
        assert dest == "bahrain \u200e\u0631\u0635\u0645\u200e kuwait"

        # int32_t ubidi_writeReverse(const UChar *src,
        #                            int32_t srcLength,
        #                            UChar *dest,
        #                            int32_t destSize,
        #                            uint16_t options,
        #                            UErrorCode *pErrorCode
        # )
        options = icu.UBIDI_DO_MIRRORING
        dest = icu.ubidi_write_reverse(text, length, options)
        assert dest == "tiawuk \u0631\u0635\u0645 niarhab"

from icupy.icu import (
    ConstVoidPtr,
    UBIDI_DEFAULT_LTR, UBIDI_DO_MIRRORING, UBIDI_INSERT_LRM_FOR_NUMERIC,
    UBIDI_LEVEL_OVERRIDE, UBIDI_MAP_NOWHERE,
    UBiDiClassCallbackPtr, UBiDiDirection, UBiDiReorderingMode,
    UBiDiReorderingOption, UCharDirection, UProperty,
    u_get_int_property_max_value, u_strlen,
    ubidi_close, ubidi_count_paragraphs, ubidi_count_runs,
    ubidi_get_base_direction, ubidi_get_class_callback,
    ubidi_get_customized_class, ubidi_get_direction, ubidi_get_length,
    ubidi_get_level_at, ubidi_get_levels,
    ubidi_get_logical_index, ubidi_get_logical_map, ubidi_get_logical_run,
    ubidi_get_paragraph, ubidi_get_paragraph_by_index, ubidi_get_para_level,
    ubidi_get_processed_length, ubidi_get_reordering_mode,
    ubidi_get_reordering_options, ubidi_get_result_length, ubidi_get_text,
    ubidi_get_visual_index, ubidi_get_visual_map, ubidi_get_visual_run,
    ubidi_invert_map, ubidi_is_inverse, ubidi_is_order_paragraphs_ltr,
    ubidi_open, ubidi_open_sized,
    ubidi_order_paragraphs_ltr, ubidi_reorder_logical, ubidi_reorder_visual,
    ubidi_set_class_callback, ubidi_set_context, ubidi_set_inverse,
    ubidi_set_line, ubidi_set_para, ubidi_set_reordering_mode,
    ubidi_set_reordering_options, ubidi_write_reordered, ubidi_write_reverse,
)
from icupy.utils import gc


def test_api():
    text = "bahrain \u0645\u0635\u0631 kuwait"

    # UBiDi *ubidi_open(void)
    # void ubidi_close(UBiDi *pBiDi)
    with gc(ubidi_open(), ubidi_close) as bidi:
        # const UChar *ubidi_getText(const UBiDi *pBiDi)
        result = ubidi_get_text(bidi)
        assert result is None

        # void ubidi_setPara(UBiDi *pBiDi,
        #                    const UChar *text,
        #                    int32_t length,
        #                    UBiDiLevel paraLevel,
        #                    UBiDiLevel *embeddingLevels,
        #                    UErrorCode *pErrorCode
        # )
        ubidi_set_para(bidi, text, -1, UBIDI_DEFAULT_LTR)

        # int32_t ubidi_countParagraphs(UBiDi *pBiDi)
        assert ubidi_count_paragraphs(bidi) == 1

        # int32_t ubidi_countRuns(UBiDi *pBiDi,
        #                         UErrorCode *pErrorCode
        # )
        assert ubidi_count_runs(bidi) == 3

        # UBiDiDirection ubidi_getBaseDirection(const UChar *text,
        #                                       int32_t length
        # )
        assert ubidi_get_base_direction(text, -1) == UBiDiDirection.UBIDI_LTR

        # UBiDiDirection ubidi_getDirection(const UBiDi *pBiDi)
        assert ubidi_get_direction(bidi) == UBiDiDirection.UBIDI_MIXED

        # int32_t ubidi_getLength(const UBiDi *pBiDi)
        assert ubidi_get_length(bidi) == 18

        # UBiDiLevel ubidi_getLevelAt(const UBiDi *pBiDi,
        #                             int32_t charIndex
        # )
        assert ubidi_get_level_at(bidi, 0) == 0
        # ...
        assert ubidi_get_level_at(bidi, 7) == 0
        assert ubidi_get_level_at(bidi, 8) == 1
        assert ubidi_get_level_at(bidi, 9) == 1
        assert ubidi_get_level_at(bidi, 10) == 1
        assert ubidi_get_level_at(bidi, 11) == 0
        # ...
        assert ubidi_get_level_at(bidi, 17) == 0

        # const UBiDiLevel *ubidi_getLevels(UBiDi *pBiDi,
        #                                   UErrorCode *pErrorCode
        # )
        levels = ubidi_get_levels(bidi)
        assert levels == [
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1,
            0, 0, 0, 0, 0, 0, 0,
        ]

        # int32_t ubidi_getLogicalIndex(UBiDi *pBiDi,
        #                               int32_t visualIndex,
        #                               UErrorCode *pErrorCode
        # )
        assert ubidi_get_logical_index(bidi, 0) == 0
        # ...
        assert ubidi_get_logical_index(bidi, 7) == 7
        assert ubidi_get_logical_index(bidi, 8) == 10
        assert ubidi_get_logical_index(bidi, 9) == 9
        assert ubidi_get_logical_index(bidi, 10) == 8
        assert ubidi_get_logical_index(bidi, 11) == 11
        # ...
        assert ubidi_get_logical_index(bidi, 17) == 17

        # void ubidi_getLogicalMap(UBiDi *pBiDi,
        #                          int32_t *indexMap,
        #                          UErrorCode *pErrorCode
        # )
        logical_map = ubidi_get_logical_map(bidi)
        assert logical_map == [
            0, 1, 2, 3, 4, 5, 6, 7,
            10, 9, 8,
            11, 12, 13, 14, 15, 16, 17,
        ]

        # void ubidi_getLogicalRun(const UBiDi *pBiDi,
        #                          int32_t logicalPosition,
        #                          int32_t *pLogicalLimit,
        #                          UBiDiLevel *pLevel
        # )
        limit, level = ubidi_get_logical_run(bidi, 0)
        assert limit == 8
        assert level == 0
        limit, level = ubidi_get_logical_run(bidi, 8)
        assert limit == 11
        assert level == 1
        limit, level = ubidi_get_logical_run(bidi, 11)
        assert limit == 18
        assert level == 0

        # int32_t ubidi_getParagraph(const UBiDi *pBiDi,
        #                            int32_t charIndex,
        #                            int32_t *pParaStart,
        #                            int32_t *pParaLimit,
        #                            UBiDiLevel *pParaLevel,
        #                            UErrorCode *pErrorCode
        # )
        result, start, limit, level = ubidi_get_paragraph(bidi, 0)
        assert (result, start, limit, level) == (0, 0, 18, 0)

        # void ubidi_getParagraphByIndex(const UBiDi *pBiDi,
        #                                int32_t paraIndex,
        #                                int32_t *pParaStart,
        #                                int32_t *pParaLimit,
        #                                UBiDiLevel *pParaLevel,
        #                                UErrorCode *pErrorCode
        # )
        start, limit, level = ubidi_get_paragraph_by_index(bidi, 0)
        assert (start, limit, level) == (0, 18, 0)

        # UBiDiLevel ubidi_getParaLevel(const UBiDi *pBiDi)
        assert ubidi_get_para_level(bidi) == 0

        # int32_t ubidi_getProcessedLength(const UBiDi *pBiDi)
        assert ubidi_get_processed_length(bidi) == 18

        # UBiDiReorderingMode ubidi_getReorderingMode(UBiDi *pBiDi)
        assert (ubidi_get_reordering_mode(bidi)
                == UBiDiReorderingMode.UBIDI_REORDER_DEFAULT)

        # uint32_t ubidi_getReorderingOptions(UBiDi *pBiDi)
        assert (ubidi_get_reordering_options(bidi)
                == UBiDiReorderingOption.UBIDI_OPTION_DEFAULT)

        # int32_t ubidi_getResultLength(const UBiDi *pBiDi)
        assert ubidi_get_result_length(bidi) == 18

        # const UChar *ubidi_getText(const UBiDi *pBiDi)
        result = ubidi_get_text(bidi)
        assert result == text

        # int32_t ubidi_getVisualIndex(UBiDi *pBiDi,
        #                              int32_t logicalIndex,
        #                              UErrorCode *pErrorCode
        # )
        assert ubidi_get_visual_index(bidi, 0) == 0
        # ...
        assert ubidi_get_visual_index(bidi, 7) == 7
        assert ubidi_get_visual_index(bidi, 8) == 10
        assert ubidi_get_visual_index(bidi, 9) == 9
        assert ubidi_get_visual_index(bidi, 10) == 8
        assert ubidi_get_visual_index(bidi, 11) == 11
        # ...
        assert ubidi_get_visual_index(bidi, 17) == 17

        # void ubidi_getVisualMap(UBiDi *pBiDi,
        #                         int32_t *indexMap,
        #                         UErrorCode *pErrorCode
        # )
        visual_map = ubidi_get_visual_map(bidi)
        assert visual_map == [
            0, 1, 2, 3, 4, 5, 6, 7,
            10, 9, 8,
            11, 12, 13, 14, 15, 16, 17,
        ]

        # UBiDiDirection ubidi_getVisualRun(UBiDi *pBiDi,
        #                                   int32_t runIndex,
        #                                   int32_t *pLogicalStart,
        #                                   int32_t *pLength
        # )
        result, start, length = ubidi_get_visual_run(bidi, 0)
        assert result == UBiDiDirection.UBIDI_LTR
        assert start == 0
        assert length == 8
        result, start, length = ubidi_get_visual_run(bidi, 1)
        assert result == UBiDiDirection.UBIDI_RTL
        assert start == 8
        assert length == 3
        result, start, length = ubidi_get_visual_run(bidi, 2)
        assert result == UBiDiDirection.UBIDI_LTR
        assert start == 11
        assert length == 7

        # void ubidi_invertMap(const int32_t *srcMap,
        #                      int32_t *destMap,
        #                      int32_t length
        # )
        src_map = [0, 1, 2, 4]
        dest_map = ubidi_invert_map(src_map, len(src_map))
        assert dest_map == [0, 1, 2, UBIDI_MAP_NOWHERE]

        # UBool ubidi_isInverse(UBiDi *pBiDi)
        assert not ubidi_is_inverse(bidi)

        # UBool ubidi_isOrderParagraphsLTR(UBiDi *pBiDi)
        assert not ubidi_is_order_paragraphs_ltr(bidi)

        # void ubidi_orderParagraphsLTR(UBiDi *pBiDi,
        #                               UBool orderParagraphsLTR
        # )
        ubidi_order_paragraphs_ltr(bidi, True)
        ubidi_set_para(bidi, text, -1, UBIDI_DEFAULT_LTR)
        assert ubidi_is_order_paragraphs_ltr(bidi)

        # void ubidi_reorderLogical(const UBiDiLevel *levels,
        #                           int32_t length,
        #                           int32_t *indexMap
        # )
        logical_map2 = ubidi_reorder_logical(levels, len(levels))
        assert logical_map2 == logical_map

        # void ubidi_reorderVisual(const UBiDiLevel *levels,
        #                          int32_t length,
        #                          int32_t *indexMap
        # )
        visual_map2 = ubidi_reorder_visual(levels, len(levels))
        assert visual_map2 == visual_map

        # void ubidi_setInverse(UBiDi *pBiDi,
        #                       UBool isInverse
        # )
        ubidi_set_inverse(bidi, True)
        ubidi_set_para(bidi, text, -1, UBIDI_DEFAULT_LTR)
        assert (ubidi_get_reordering_mode(bidi)
                == UBiDiReorderingMode.UBIDI_REORDER_INVERSE_NUMBERS_AS_L)

        # void ubidi_setReorderingMode(UBiDi *pBiDi,
        #                              UBiDiReorderingMode reorderingMode
        # )
        ubidi_set_reordering_mode(
            bidi,
            UBiDiReorderingMode.UBIDI_REORDER_DEFAULT)
        assert (ubidi_get_reordering_mode(bidi)
                == UBiDiReorderingMode.UBIDI_REORDER_DEFAULT)

        # void ubidi_setReorderingOptions(UBiDi *pBiDi,
        #                                 uint32_t reorderingOptions
        # )
        ubidi_set_reordering_options(
            bidi,
            UBiDiReorderingOption.UBIDI_OPTION_INSERT_MARKS)
        assert (ubidi_get_reordering_options(bidi)
                == UBiDiReorderingOption.UBIDI_OPTION_INSERT_MARKS)


def test_set_class_callback():
    # From icu/source/test/intltest/bidiconf.cpp
    char_from_bidi_class = [
        0x6c,  # U_LEFT_TO_RIGHT
        0x52,  # U_RIGHT_TO_LEFT
        0x33,  # U_EUROPEAN_NUMBER
        0x2d,  # U_EUROPEAN_NUMBER_SEPARATOR
        0x25,  # U_EUROPEAN_NUMBER_TERMINATOR
        0x39,  # U_ARABIC_NUMBER
        0x2c,  # U_COMMON_NUMBER_SEPARATOR
        0x2f,  # U_BLOCK_SEPARATOR
        0x5f,  # U_SEGMENT_SEPARATOR
        0x20,  # U_WHITE_SPACE_NEUTRAL
        0x3d,  # U_OTHER_NEUTRAL
    ]

    # UCharDirection UBiDiClassCallback(const void *context,
    #                                   UChar32 c)
    def _callback1(_context: object, _c: int) -> int:
        assert _context is None
        nonlocal char_from_bidi_class
        if _c in char_from_bidi_class:
            return char_from_bidi_class.index(_c)
        return u_get_int_property_max_value(UProperty.UCHAR_BIDI_CLASS) + 1

    def _callback2(_context: object, _c: int) -> int:
        assert isinstance(_context, list)
        if _c in _context:
            return _context.index(_c)
        return u_get_int_property_max_value(UProperty.UCHAR_BIDI_CLASS) + 1

    with gc(ubidi_open(), ubidi_close) as bidi:
        # void ubidi_getClassCallback(UBiDi *pBiDi,
        #                             UBiDiClassCallback **fn,
        #                             const void **context
        # )
        old_fn1, old_context1 = ubidi_get_class_callback(bidi)
        result = ubidi_get_customized_class(bidi, 0x52)
        assert result == UCharDirection.U_LEFT_TO_RIGHT
        result = ubidi_get_customized_class(bidi, 0x39)
        assert result == UCharDirection.U_EUROPEAN_NUMBER

        # void ubidi_setClassCallback(UBiDi *pBiDi,
        #                             UBiDiClassCallback *newFn,
        #                             const void *newContext,
        #                             UBiDiClassCallback **oldFn,
        #                             const void **oldContext,
        #                             UErrorCode *pErrorCode
        # )
        fn2 = UBiDiClassCallbackPtr(_callback1)
        context2 = ConstVoidPtr(None)
        old_fn2, old_context2 = ubidi_set_class_callback(bidi, fn2, context2)
        result = ubidi_get_customized_class(bidi, 0x52)
        assert result == UCharDirection.U_RIGHT_TO_LEFT
        result = ubidi_get_customized_class(bidi, 0x39)
        assert result == UCharDirection.U_ARABIC_NUMBER

        old_fn3, old_context3 = ubidi_set_class_callback(bidi,
                                                         old_fn1,
                                                         old_context1)
        result = ubidi_get_customized_class(bidi, 0x52)
        assert result == UCharDirection.U_LEFT_TO_RIGHT
        result = ubidi_get_customized_class(bidi, 0x39)
        assert result == UCharDirection.U_EUROPEAN_NUMBER

        fn4 = UBiDiClassCallbackPtr(_callback2)
        context4 = ConstVoidPtr(char_from_bidi_class)
        ubidi_set_class_callback(bidi, fn4, context4)
        result = ubidi_get_customized_class(bidi, 0x52)
        assert result == UCharDirection.U_RIGHT_TO_LEFT
        result = ubidi_get_customized_class(bidi, 0x39)
        assert result == UCharDirection.U_ARABIC_NUMBER

        ubidi_set_class_callback(bidi, old_fn2, old_context2)
        result = ubidi_get_customized_class(bidi, 0x52)
        assert result == UCharDirection.U_LEFT_TO_RIGHT
        result = ubidi_get_customized_class(bidi, 0x39)
        assert result == UCharDirection.U_EUROPEAN_NUMBER

        ubidi_set_class_callback(bidi, old_fn3, old_context3)
        result = ubidi_get_customized_class(bidi, 0x52)
        assert result == UCharDirection.U_RIGHT_TO_LEFT
        result = ubidi_get_customized_class(bidi, 0x39)
        assert result == UCharDirection.U_ARABIC_NUMBER


def test_set_context():
    with gc(ubidi_open(), ubidi_close) as bidi:
        prologue = "<code>abc \u05d0\u05d1</code>"
        text = "<code>\u05d2\u05d3\u05d4 xyz</code>"
        para_level = 1

        ubidi_set_para(bidi, text, -1, para_level)
        result1 = ubidi_get_logical_map(bidi)

        # void ubidi_setContext(UBiDi *pBiDi,
        #                       const UChar *prologue,
        #                       int32_t proLength,
        #                       const UChar *epilogue,
        #                       int32_t epiLength,
        #                       UErrorCode *pErrorCode
        # )
        ubidi_set_context(bidi, prologue, -1, None, 0)
        ubidi_set_para(bidi, text, -1, para_level)
        result2 = ubidi_get_logical_map(bidi)
        assert result1 != result2


def test_set_line():
    text = "bahrain \u0645\u0635\u0631 kuwait"
    length = u_strlen(text)

    # UBiDi *ubidi_openSized(int32_t maxLength,
    #                        int32_t maxRunCount,
    #                        UErrorCode *pErrorCode
    # )
    # void ubidi_close(UBiDi *pBiDi)
    with gc(ubidi_open_sized(length, 0), ubidi_close) as para, gc(
            ubidi_open_sized(length, 0), ubidi_close) as line:
        ubidi_set_para(para, text, -1, UBIDI_DEFAULT_LTR)
        assert ubidi_get_direction(para) == UBiDiDirection.UBIDI_MIXED

        start = 0
        limit, _ = ubidi_get_logical_run(para, start)
        assert limit == 8

        # void ubidi_setLine(const UBiDi *pParaBiDi,
        #                    int32_t start,
        #                    int32_t limit,
        #                    UBiDi *pLineBiDi,
        #                    UErrorCode *pErrorCode
        # )
        ubidi_set_line(para, start, limit, line)
        assert ubidi_get_direction(line) == UBiDiDirection.UBIDI_LTR

        start = limit
        limit, _ = ubidi_get_logical_run(para, start)
        assert limit == 11

        ubidi_set_line(para, start, limit, line)
        assert ubidi_get_direction(line) == UBiDiDirection.UBIDI_RTL

        start = limit
        limit, _ = ubidi_get_logical_run(para, start)
        assert limit == 18

        ubidi_set_line(para, start, limit, line)
        assert ubidi_get_direction(line) == UBiDiDirection.UBIDI_LTR


def test_set_para():
    text = "car means CAR."
    length = u_strlen(text)

    with gc(ubidi_open_sized(length, 0), ubidi_close) as bidi:
        # void ubidi_setPara(UBiDi *pBiDi,
        #                    const UChar *text,
        #                    int32_t length,
        #                    UBiDiLevel paraLevel,
        #                    UBiDiLevel *embeddingLevels,
        #                    UErrorCode *pErrorCode
        # )
        embedding_levels = [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1 | UBIDI_LEVEL_OVERRIDE,
            1 | UBIDI_LEVEL_OVERRIDE,
            1 | UBIDI_LEVEL_OVERRIDE,
            0,
        ]
        ubidi_set_para(bidi, text, length, UBIDI_DEFAULT_LTR, embedding_levels)

        levels = ubidi_get_levels(bidi)
        assert levels == [
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1,
            0,
        ]

        result = ubidi_get_logical_map(bidi)
        assert result == [
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            12, 11, 10,
            13,
        ]

        result = ubidi_get_visual_map(bidi)
        assert result == [
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            12, 11, 10,
            13,
        ]


def test_write_reordered():
    text = "bahrain \u0645\u0635\u0631 kuwait"
    length = u_strlen(text)

    with gc(ubidi_open_sized(length, 0), ubidi_close) as bidi:
        ubidi_set_para(bidi, text, -1, UBIDI_DEFAULT_LTR)

        # int32_t ubidi_writeReordered(UBiDi *pBiDi,
        #                              UChar *dest,
        #                              int32_t destSize,
        #                              uint16_t options,
        #                              UErrorCode *pErrorCode
        # )
        options = 0
        dest = ubidi_write_reordered(bidi, options)
        assert dest == "bahrain \u0631\u0635\u0645 kuwait"

        # LRM: U+200E
        ubidi_set_inverse(bidi, True)
        ubidi_set_para(bidi, text, -1, UBIDI_DEFAULT_LTR)
        options = UBIDI_INSERT_LRM_FOR_NUMERIC | UBIDI_DO_MIRRORING
        dest = ubidi_write_reordered(bidi, options)
        assert dest == "bahrain \u200e\u0631\u0635\u0645\u200e kuwait"

        # int32_t ubidi_writeReverse(const UChar *src,
        #                            int32_t srcLength,
        #                            UChar *dest,
        #                            int32_t destSize,
        #                            uint16_t options,
        #                            UErrorCode *pErrorCode
        # )
        options = UBIDI_DO_MIRRORING
        dest = ubidi_write_reverse(text, length, options)
        assert dest == "tiawuk \u0631\u0635\u0645 niarhab"

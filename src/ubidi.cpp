#include "main.hpp"
#include "ubidiptr.hpp"
#include "voidptr.hpp"
#include <pybind11/stl.h>
#include <unicode/ustring.h>

_UBiDiPtr::_UBiDiPtr(UBiDi *p) : p_(p) {}
_UBiDiPtr::~_UBiDiPtr() {}
UBiDi *_UBiDiPtr::get() const { return p_; }

void _UBiDiPtr::set_embedding_levels(const std::shared_ptr<UBiDiLevel[]> &embedding_levels) {
  embedding_levels_ = embedding_levels;
}

void _UBiDiPtr::set_epilogue(const std::shared_ptr<UChar[]> &epilogue) { epilogue_ = epilogue; }

void _UBiDiPtr::set_prologue(const std::shared_ptr<UChar[]> &prologue) { prologue_ = prologue; }

void _UBiDiPtr::set_text(const std::shared_ptr<UChar[]> &text) { text_ = text; }

_UBiDiClassCallbackPtr::_UBiDiClassCallbackPtr(std::nullptr_t action) : action_(action) {}
_UBiDiClassCallbackPtr::_UBiDiClassCallbackPtr(UBiDiClassCallback *action) : action_(action) {}
_UBiDiClassCallbackPtr::_UBiDiClassCallbackPtr(const py::function &action) : action_(action) {}
_UBiDiClassCallbackPtr::~_UBiDiClassCallbackPtr() {}

UCharDirection _UBiDiClassCallbackPtr::callback(const void *context, UChar32 c) {
  auto cp = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
  auto python_context = cp->to_object();
  auto &action = cp->get_action();
  return (UCharDirection)action(python_context, c).cast<int32_t>();
}

void init_ubidi(py::module &m) {
  py::enum_<UBiDiDirection>(m, "UBiDiDirection", py::arithmetic())
      .value("UBIDI_LTR", UBIDI_LTR)
      .value("UBIDI_RTL", UBIDI_RTL)
      .value("UBIDI_MIXED", UBIDI_MIXED)
      .value("UBIDI_NEUTRAL", UBIDI_NEUTRAL)
      .export_values();

  py::enum_<UBiDiReorderingMode>(m, "UBiDiReorderingMode", py::arithmetic())
      .value("UBIDI_REORDER_DEFAULT", UBIDI_REORDER_DEFAULT)
      .value("UBIDI_REORDER_NUMBERS_SPECIAL", UBIDI_REORDER_NUMBERS_SPECIAL)
      .value("UBIDI_REORDER_GROUP_NUMBERS_WITH_R", UBIDI_REORDER_GROUP_NUMBERS_WITH_R)
      .value("UBIDI_REORDER_RUNS_ONLY", UBIDI_REORDER_RUNS_ONLY)
      .value("UBIDI_REORDER_INVERSE_NUMBERS_AS_L", UBIDI_REORDER_INVERSE_NUMBERS_AS_L)
      .value("UBIDI_REORDER_INVERSE_LIKE_DIRECT", UBIDI_REORDER_INVERSE_LIKE_DIRECT)
      .value("UBIDI_REORDER_INVERSE_FOR_NUMBERS_SPECIAL", UBIDI_REORDER_INVERSE_FOR_NUMBERS_SPECIAL)
      .export_values();

  py::enum_<UBiDiReorderingOption>(m, "UBiDiReorderingOption", py::arithmetic())
      .value("UBIDI_OPTION_DEFAULT", UBIDI_OPTION_DEFAULT)
      .value("UBIDI_OPTION_INSERT_MARKS", UBIDI_OPTION_INSERT_MARKS)
      .value("UBIDI_OPTION_REMOVE_CONTROLS", UBIDI_OPTION_REMOVE_CONTROLS)
      .value("UBIDI_OPTION_STREAMING", UBIDI_OPTION_STREAMING)
      .export_values();

  py::class_<_UBiDiClassCallbackPtr>(m, "UBiDiClassCallbackPtr")
      .def(py::init<std::nullptr_t>(), py::arg("fn"))
      .def(py::init<py::function>(), py::arg("fn"));

  py::class_<_UBiDiPtr>(m, "_UBiDiPtr");

  m.def(
      "ubidi_close", [](_UBiDiPtr &bidi) { ubidi_close(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_count_paragraphs", [](_UBiDiPtr &bidi) { return ubidi_countParagraphs(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_count_runs",
      [](_UBiDiPtr &bidi) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ubidi_countRuns(bidi, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"));
  m.def("ubidi_get_base_direction", &ubidi_getBaseDirection, py::arg("text"), py::arg("length"));
  m.def(
      "ubidi_get_class_callback",
      [](_UBiDiPtr &bidi) {
        UBiDiClassCallback *fn;
        const void *context;
        ubidi_getClassCallback(bidi, &fn, &context);
        if (fn == _UBiDiClassCallbackPtr::callback) {
          // Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
          auto result1 = _UBiDiClassCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // C callback function and callback data
        return py::make_tuple(_UBiDiClassCallbackPtr(fn), _ConstVoidPtr(context));
      },
      py::arg("bidi"));
  m.def(
      "ubidi_get_customized_class", [](_UBiDiPtr &bidi, UChar32 c) { return ubidi_getCustomizedClass(bidi, c); },
      py::arg("bidi"), py::arg("c"));
  m.def(
      "ubidi_get_direction", [](_UBiDiPtr &bidi) { return ubidi_getDirection(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_get_length", [](_UBiDiPtr &bidi) { return ubidi_getLength(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_get_level_at", [](_UBiDiPtr &bidi, int32_t char_index) { return ubidi_getLevelAt(bidi, char_index); },
      py::arg("bidi"), py::arg("char_index"));
  m.def(
      "ubidi_get_levels",
      [](_UBiDiPtr &bidi) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ubidi_getLevels(bidi, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        const auto length = ubidi_getProcessedLength(bidi);
        std::vector<UBiDiLevel> result(p, p + length);
        return result;
      },
      py::arg("bidi"));
  m.def(
      "ubidi_get_logical_index",
      [](_UBiDiPtr &bidi, int32_t visual_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ubidi_getLogicalIndex(bidi, visual_index, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"), py::arg("visual_index"));
  m.def(
      "ubidi_get_logical_map",
      [](_UBiDiPtr &bidi) {
        UErrorCode error_code = U_ZERO_ERROR;
        const int32_t length = ubidi_getReorderingOptions(bidi) & UBIDI_OPTION_INSERT_MARKS
                                   ? ubidi_getResultLength(bidi)
                                   : ubidi_getProcessedLength(bidi);
        std::vector<int32_t> result(length);
        ubidi_getLogicalMap(bidi, result.data(), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"));
  m.def(
      "ubidi_get_logical_run",
      [](_UBiDiPtr &bidi, int32_t logical_position) {
        int32_t logical_limit;
        UBiDiLevel level;
        ubidi_getLogicalRun(bidi, logical_position, &logical_limit, &level);
        return py::make_tuple(logical_limit, level);
      },
      py::arg("bidi"), py::arg("logical_position"));
  m.def(
      "ubidi_get_paragraph",
      [](_UBiDiPtr &bidi, int32_t char_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        int32_t para_start;
        int32_t para_limit;
        UBiDiLevel para_level;
        auto result = ubidi_getParagraph(bidi, char_index, &para_start, &para_limit, &para_level, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(result, para_start, para_limit, para_level);
      },
      py::arg("bidi"), py::arg("char_index"));
  m.def(
      "ubidi_get_paragraph_by_index",
      [](_UBiDiPtr &bidi, int32_t para_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        int32_t para_start;
        int32_t para_limit;
        UBiDiLevel para_level;
        ubidi_getParagraphByIndex(bidi, para_index, &para_start, &para_limit, &para_level, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(para_start, para_limit, para_level);
      },
      py::arg("bidi"), py::arg("para_index"));
  m.def(
      "ubidi_get_para_level", [](_UBiDiPtr &bidi) { return ubidi_getParaLevel(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_get_processed_length", [](_UBiDiPtr &bidi) { return ubidi_getProcessedLength(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_get_reordering_mode", [](_UBiDiPtr &bidi) { return ubidi_getReorderingMode(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_get_reordering_options", [](_UBiDiPtr &bidi) { return ubidi_getReorderingOptions(bidi); },
      py::arg("bidi"));
  m.def(
      "ubidi_get_result_length", [](_UBiDiPtr &bidi) { return ubidi_getResultLength(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_get_text", [](_UBiDiPtr &bidi) { return ubidi_getText(bidi); }, py::return_value_policy::reference,
      py::arg("bidi"));
  m.def(
      "ubidi_get_visual_index",
      [](_UBiDiPtr &bidi, int32_t logical_index) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ubidi_getVisualIndex(bidi, logical_index, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"), py::arg("logical_index"));
  m.def(
      "ubidi_get_visual_map",
      [](_UBiDiPtr &bidi) {
        UErrorCode error_code = U_ZERO_ERROR;
        const int32_t length = ubidi_getReorderingOptions(bidi) & UBIDI_OPTION_REMOVE_CONTROLS
                                   ? ubidi_getProcessedLength(bidi)
                                   : ubidi_getResultLength(bidi);
        std::vector<int32_t> result(length);
        ubidi_getVisualMap(bidi, result.data(), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"));
  m.def(
      "ubidi_get_visual_run",
      [](_UBiDiPtr &bidi, int32_t run_index) {
        int32_t logical_start;
        int32_t length;
        auto result = ubidi_getVisualRun(bidi, run_index, &logical_start, &length);
        return py::make_tuple(result, logical_start, length);
      },
      py::arg("bidi"), py::arg("run_index"));
  m.def(
      "ubidi_invert_map",
      [](const std::vector<int32_t> &src_map, int32_t length) {
        std::vector<int32_t> result(length);
        ubidi_invertMap(src_map.data(), result.data(), length);
        return result;
      },
      py::arg("src_map"), py::arg("length"));
  m.def(
      "ubidi_is_inverse", [](_UBiDiPtr &bidi) { return ubidi_isInverse(bidi); }, py::arg("bidi"));
  m.def(
      "ubidi_is_order_paragraphs_ltr", [](_UBiDiPtr &bidi) { return ubidi_isOrderParagraphsLTR(bidi); },
      py::arg("bidi"));
  m.def("ubidi_open", []() {
    auto bidi = ubidi_open();
    return std::make_unique<_UBiDiPtr>(bidi);
  });
  m.def(
      "ubidi_open_sized",
      [](int32_t max_length, int32_t max_run_count) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto bidi = ubidi_openSized(max_length, max_run_count, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UBiDiPtr>(bidi);
      },
      py::arg("max_length"), py::arg("max_run_count"));
  m.def(
      "ubidi_order_paragraphs_ltr",
      [](_UBiDiPtr &bidi, UBool order_paragraphs_ltr) { ubidi_orderParagraphsLTR(bidi, order_paragraphs_ltr); },
      py::arg("bidi"), py::arg("order_paragraphs_ltr"));
  m.def(
      "ubidi_reorder_logical",
      [](const std::vector<UBiDiLevel> &levels, int32_t length) {
        std::vector<int32_t> result(length);
        ubidi_reorderLogical(levels.data(), length, result.data());
        return result;
      },
      py::arg("levels"), py::arg("length"));
  m.def(
      "ubidi_reorder_visual",
      [](const std::vector<UBiDiLevel> &levels, int32_t length) {
        std::vector<int32_t> result(length);
        ubidi_reorderVisual(levels.data(), length, result.data());
        return result;
      },
      py::arg("levels"), py::arg("length"));
  m.def(
      "ubidi_set_class_callback",
      [](_UBiDiPtr &bidi, _UBiDiClassCallbackPtr &new_fn, _ConstVoidPtr &new_context) {
        UBiDiClassCallback *fp = new_fn.get_if<UBiDiClassCallback *>();
        const void *cp = nullptr;
        if (fp == nullptr && new_fn.has_value()) {
          // New Python callback function and new callback data
          fp = new_fn.callback;
          new_context.set_action(new_fn.get<py::function>());
          cp = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data (not tested)
          cp = new_context.to_c_str();
        }

        UBiDiClassCallback *old_fn;
        const void *old_context;
        UErrorCode error_code = U_ZERO_ERROR;
        ubidi_setClassCallback(bidi, fp, cp, &old_fn, &old_context, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        if (old_fn == new_fn.callback) {
          // Old Python callback function and old callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(old_context));
          auto result1 = _UBiDiClassCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // Old C callback function and old callback data
        return py::make_tuple(_UBiDiClassCallbackPtr(old_fn), _ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(), py::keep_alive<3, 1>(), py::arg("bidi"),
      py::arg("new_fn"), py::arg("new_context"));
  m.def(
      "ubidi_set_context",
      [](_UBiDiPtr &bidi, const UChar *prologue, int32_t pro_length, const UChar *epilogue, int32_t epi_length) {
        UErrorCode error_code = U_ZERO_ERROR;
        std::shared_ptr<UChar[]> prologue_ptr;
        if (prologue) {
          int32_t n = pro_length == -1 ? u_strlen(prologue) : std::max(0, pro_length);
          prologue_ptr = std::shared_ptr<UChar[]>(new UChar[n + 1]);
          auto dest = prologue_ptr.get();
          u_memset(dest, 0, n + 1);
          u_strncpy(dest, prologue, n);
        }
        std::shared_ptr<UChar[]> epilogue_ptr;
        if (epilogue) {
          int32_t n = epi_length == -1 ? u_strlen(epilogue) : std::max(0, epi_length);
          epilogue_ptr = std::shared_ptr<UChar[]>(new UChar[n + 1]);
          auto dest = epilogue_ptr.get();
          u_memset(dest, 0, n + 1);
          u_strncpy(dest, epilogue, n);
        }
        ubidi_setContext(bidi, prologue_ptr.get(), pro_length, epilogue_ptr.get(), epi_length, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        bidi.set_prologue(prologue_ptr);
        bidi.set_epilogue(epilogue_ptr);
      },
      py::arg("bidi"), py::arg("prologue"), py::arg("pro_length"), py::arg("epilogue"), py::arg("epi_length"));
  m.def(
      "ubidi_set_inverse", [](_UBiDiPtr &bidi, UBool is_inverse) { ubidi_setInverse(bidi, is_inverse); },
      py::arg("bidi"), py::arg("is_inverse"));
  m.def(
      "ubidi_set_line",
      [](_UBiDiPtr &para_bidi, int32_t start, int32_t limit, _UBiDiPtr &line_bidi) {
        UErrorCode error_code = U_ZERO_ERROR;
        ubidi_setLine(para_bidi, start, limit, line_bidi, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("para_bidi"), py::arg("start"), py::arg("limit"), py::arg("line_bidi"));
  m.def(
      "ubidi_set_para",
      [](_UBiDiPtr &bidi, const UChar *text, int32_t length, UBiDiLevel para_level,
         std::optional<std::vector<UBiDiLevel>> &embedding_levels) {
        UErrorCode error_code = U_ZERO_ERROR;
        std::shared_ptr<UChar[]> text_ptr;
        if (text) {
          int32_t n = length == -1 ? u_strlen(text) : std::max(0, length);
          text_ptr = std::shared_ptr<UChar[]>(new UChar[n + 1]);
          auto dest = text_ptr.get();
          u_memset(dest, 0, n + 1);
          u_strncpy(dest, text, n);
        }
        std::shared_ptr<UBiDiLevel[]> embedding_levels_ptr;
        if (embedding_levels.has_value()) {
          embedding_levels_ptr = std::shared_ptr<UBiDiLevel[]>(new UBiDiLevel[embedding_levels->size()]);
          std::copy(embedding_levels->begin(), embedding_levels->end(), embedding_levels_ptr.get());
        }
        ubidi_setPara(bidi, text_ptr.get(), length, para_level, embedding_levels_ptr.get(), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        bidi.set_text(text_ptr);
        bidi.set_embedding_levels(embedding_levels_ptr);
      },
      py::arg("bidi"), py::arg("text").none(false), py::arg("length"), py::arg("para_level"),
      py::arg("embedding_levels") = std::nullopt);
  m.def(
      "ubidi_set_reordering_mode",
      [](_UBiDiPtr &bidi, UBiDiReorderingMode reordering_mode) { ubidi_setReorderingMode(bidi, reordering_mode); },
      py::arg("bidi"), py::arg("reordering_mode"));
  m.def(
      "ubidi_set_reordering_options",
      [](_UBiDiPtr &bidi, int32_t reordering_options) { ubidi_setReorderingOptions(bidi, reordering_options); },
      py::arg("bidi"), py::arg("reordering_options"));
  m.def(
      "ubidi_write_reordered",
      [](_UBiDiPtr &bidi, uint16_t options) {
        UErrorCode error_code = U_ZERO_ERROR;
        int32_t length = 0;
        if (options & UBIDI_INSERT_LRM_FOR_NUMERIC) {
          length = ubidi_getLength(bidi) + 2 * ubidi_countRuns(bidi, &error_code);
        } else if (options & UBIDI_REMOVE_BIDI_CONTROLS) {
          length = ubidi_getLength(bidi);
        } else {
          length = ubidi_getProcessedLength(bidi);
        }
        error_code = U_ZERO_ERROR;
        std::u16string result(length, u'\0');
        length = ubidi_writeReordered(bidi, result.data(), (int32_t)result.size(), options, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        result.resize(length);
        return result;
      },
      py::arg("bidi"), py::arg("options"));
  m.def(
      "ubidi_write_reverse",
      [](const UChar *src, int32_t src_length, uint16_t options) {
        UErrorCode error_code = U_ZERO_ERROR;
        std::u16string result(src_length, u'\0');
        const auto length =
            ubidi_writeReverse(src, src_length, result.data(), (int32_t)result.size(), options, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        result.resize(length);
        return result;
      },
      py::arg("src"), py::arg("src_length"), py::arg("options"));

  m.attr("UBIDI_DEFAULT_LTR") = UBIDI_DEFAULT_LTR;
  m.attr("UBIDI_DEFAULT_RTL") = UBIDI_DEFAULT_RTL;
  m.attr("UBIDI_DO_MIRRORING") = UBIDI_DO_MIRRORING;
  m.attr("UBIDI_INSERT_LRM_FOR_NUMERIC") = UBIDI_INSERT_LRM_FOR_NUMERIC;
  m.attr("UBIDI_KEEP_BASE_COMBINING") = UBIDI_KEEP_BASE_COMBINING;
  m.attr("UBIDI_LEVEL_OVERRIDE") = UBIDI_LEVEL_OVERRIDE;
  m.attr("UBIDI_MAP_NOWHERE") = UBIDI_MAP_NOWHERE;
  m.attr("UBIDI_MAX_EXPLICIT_LEVEL") = UBIDI_MAX_EXPLICIT_LEVEL;
  m.attr("UBIDI_OUTPUT_REVERSE") = UBIDI_OUTPUT_REVERSE;
  m.attr("UBIDI_REMOVE_BIDI_CONTROLS") = UBIDI_REMOVE_BIDI_CONTROLS;
}

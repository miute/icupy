#include "main.hpp"
#include "ubidiptr.hpp"
#include "voidptr.hpp"
#include <optional>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

using namespace icu;

namespace icupy {

//
// struct UBiDi
//
UBiDiPtr::UBiDiPtr(UBiDi *p) : p_(p) {}

UBiDiPtr::~UBiDiPtr() {}

UBiDi *UBiDiPtr::get() const { return p_; }

void UBiDiPtr::set_embedding_levels(
    const std::shared_ptr<UBiDiLevel[]> &embedding_levels) {
  embedding_levels_ = embedding_levels;
}

void UBiDiPtr::set_epilogue(const std::shared_ptr<std::u16string> &epilogue) {
  epilogue_ = epilogue;
}

void UBiDiPtr::set_prologue(const std::shared_ptr<std::u16string> &prologue) {
  prologue_ = prologue;
}

void UBiDiPtr::set_text(const std::shared_ptr<std::u16string> &text) {
  text_ = text;
}

//
// UBiDiClassCallback
//
UCharDirection UBiDiClassCallbackPtr::callback(const void *native_context,
                                               UChar32 c) {
  if (native_context == nullptr) {
    throw std::runtime_error("UBiDiClassCallback: context is not set");
  }
  auto pair = reinterpret_cast<ClassCallbackAndContextPair *>(
      const_cast<void *>(native_context));
  auto &action = pair->first;
  if (!action) {
    throw std::runtime_error(
        "UBiDiClassCallback: callback function is not set or invalid");
  }
  auto context = pair->second;
  auto object = context ? context->value() : py::none();
  return action(object, c);
}

} // namespace icupy

void init_ubidi(py::module &m) {
  //
  // enum UBiDiDirection
  //
  py::enum_<UBiDiDirection>(
      m, "UBiDiDirection", py::arithmetic(),
      "UBiDiDirection values indicate the text direction.")
      .value("UBIDI_LTR", UBIDI_LTR, R"doc(
             Left-to-right text.
             This is a 0 value.

             - As return value for :func:`ubidi_get_direction`, it means that the
               source string contains no right-to-left
               characters, or that the source string is empty and the paragraph
               level is even.
             - As return value for :func:`ubidi_get_base_direction`, it means
               that the first strong character of the
               source string has a left-to-right direction.
             )doc")
      .value("UBIDI_RTL", UBIDI_RTL, R"doc(
             Right-to-left text.
             This is a 1 value.

             - As return value for :func:`ubidi_get_direction`, it means that the
               source string contains no left-to-right
               characters, or that the source string is empty and the paragraph
               level is odd.
             - As return value for :func:`ubidi_get_base_direction`, it means
               that the first strong character of the
               source string has a right-to-left direction.
             )doc")
      .value("UBIDI_MIXED", UBIDI_MIXED, R"doc(
             Mixed-directional text.

             As return value for :func:`ubidi_get_direction`, it means that the
             source string contains both left-to-right
             and right-to-left characters.
             )doc")
      .value("UBIDI_NEUTRAL", UBIDI_NEUTRAL, R"doc(
             No strongly directional text.

             As return value for :func:`ubidi_get_base_direction`, it means that
             the source string is missing or empty,
             or contains neither left-to-right nor right-to-left characters.
             )doc")
      .export_values();

  //
  // enum UBiDiReorderingMode
  //
  py::enum_<UBiDiReorderingMode>(m, "UBiDiReorderingMode", py::arithmetic(),
                                 R"doc(
UBiDiReorderingMode values indicate which variant of the Bidi algorithm to use.

See Also:
    :func:`ubidi_set_inverse`
    :func:`ubidi_set_reordering_mode`
      )doc")
      .value("UBIDI_REORDER_DEFAULT", UBIDI_REORDER_DEFAULT, R"doc(
             Regular Logical to Visual Bidi algorithm according to Unicode.

             This is a 0 value.
             )doc")
      .value("UBIDI_REORDER_NUMBERS_SPECIAL", UBIDI_REORDER_NUMBERS_SPECIAL,
             R"doc(
             Logical to Visual algorithm which handles numbers in a way which
             mimics the behavior of Windows XP.
             )doc")
      .value("UBIDI_REORDER_GROUP_NUMBERS_WITH_R",
             UBIDI_REORDER_GROUP_NUMBERS_WITH_R, R"doc(
             Logical to Visual algorithm grouping numbers with adjacent R
             characters (reversible algorithm).
             )doc")
      .value("UBIDI_REORDER_RUNS_ONLY", UBIDI_REORDER_RUNS_ONLY, R"doc(
             Reorder runs only to transform a Logical LTR string to the
             Logical RTL string with the same display, or
             vice-versa.

             If this mode is set together with option
             :attr:`~UBiDiReorderingOption.UBIDI_OPTION_INSERT_MARKS`,
             some Bidi controls in the source
             text may be removed and other controls may be added to produce
             the minimum combination which has the
             required display.
             )doc")
      .value("UBIDI_REORDER_INVERSE_NUMBERS_AS_L",
             UBIDI_REORDER_INVERSE_NUMBERS_AS_L, R"doc(
             Visual to Logical algorithm which handles numbers like L (same
             algorithm as selected by
             ubidi_set_inverse(True).
             )doc")
      .value("UBIDI_REORDER_INVERSE_LIKE_DIRECT",
             UBIDI_REORDER_INVERSE_LIKE_DIRECT, R"doc(
             Visual to Logical algorithm equivalent to the regular Logical to
             Visual algorithm.
             )doc")
      .value("UBIDI_REORDER_INVERSE_FOR_NUMBERS_SPECIAL",
             UBIDI_REORDER_INVERSE_FOR_NUMBERS_SPECIAL, R"doc(
             Inverse Bidi (Visual to Logical) algorithm for the
             :attr:`~UBIDI_REORDER_NUMBERS_SPECIAL` Bidi algorithm.
             )doc")
      .export_values();

  //
  // enum UBiDiReorderingOption
  //
  py::enum_<UBiDiReorderingOption>(m, "UBiDiReorderingOption", py::arithmetic(),
                                   R"doc(
UBiDiReorderingOption values indicate which options are specified to affect
the Bidi algorithm.
      )doc")
      .value("UBIDI_OPTION_DEFAULT", UBIDI_OPTION_DEFAULT, R"doc(
             Option value for :func:`ubidi_set_reordering_options`: disable
             all the options which can be set with this function.
             )doc")
      .value("UBIDI_OPTION_INSERT_MARKS", UBIDI_OPTION_INSERT_MARKS, R"doc(
             Option bit for :func:`ubidi_set_reordering_options`: insert Bidi marks
             (LRM or RLM) when needed to ensure
             correct result of a reordering to a Logical order.

             This option must be set or reset before calling
             :func:`ubidi_set_para`.

             This option is significant only with reordering modes which
             generate a result with Logical order,
             specifically:

             - :attr:`~UBiDiReorderingMode.UBIDI_REORDER_RUNS_ONLY`
             - :attr:`~UBiDiReorderingMode.UBIDI_REORDER_INVERSE_NUMBERS_AS_L`
             - :attr:`~UBiDiReorderingMode.UBIDI_REORDER_INVERSE_LIKE_DIRECT`
             - :attr:`~UBiDiReorderingMode.UBIDI_REORDER_INVERSE_FOR_NUMBERS_SPECIAL`

             If this option is set in conjunction with reordering mode
             :attr:`~UBiDiReorderingMode.UBIDI_REORDER_INVERSE_NUMBERS_AS_L`
             or with calling ubidi_set_inverse(True), it implies option
             :attr:`~UBIDI_INSERT_LRM_FOR_NUMERIC` in calls to function
             :func:`ubidi_write_reordered`.

             For other reordering modes, a minimum number of LRM or RLM
             characters will be added to the source text
             after reordering it so as to ensure round trip, i.e. when
             applying the inverse reordering mode on the
             resulting logical text with removal of Bidi marks (option
             :attr:`~UBIDI_OPTION_REMOVE_CONTROLS` set before
             calling :func:`ubidi_set_para` or option
             :attr:`~UBIDI_REMOVE_BIDI_CONTROLS` in
             :func:`ubidi_write_reordered`, the
             result will be identical to the source text in the first
             transformation.

             This option will be ignored if specified together with option
             :attr:`~UBIDI_OPTION_REMOVE_CONTROLS`. It
             inhibits option :attr:`~UBIDI_REMOVE_BIDI_CONTROLS` in calls to
             function :func:`ubidi_write_reordered` and it
             implies option :attr:`~UBIDI_INSERT_LRM_FOR_NUMERIC` in calls to
             function :func:`ubidi_write_reordered` if the
             reordering mode is
             :attr:`~UBiDiReorderingMode.UBIDI_REORDER_INVERSE_NUMBERS_AS_L`.
             )doc")
      .value("UBIDI_OPTION_REMOVE_CONTROLS", UBIDI_OPTION_REMOVE_CONTROLS,
             R"doc(
             Option bit for :func:`ubidi_set_reordering_options`: remove Bidi
             control characters.

             This option must be set or reset before calling
             :func:`ubidi_set_para`.

             This option nullifies option :attr:`~UBIDI_OPTION_INSERT_MARKS`.
             It inhibits option
             :attr:`~UBIDI_INSERT_LRM_FOR_NUMERIC` in calls to function
             :func:`ubidi_write_reordered` and it implies option
             :attr:`~UBIDI_REMOVE_BIDI_CONTROLS` in calls to that function.
             )doc")
      .value("UBIDI_OPTION_STREAMING", UBIDI_OPTION_STREAMING, R"doc(
             Option bit for :func:`ubidi_set_reordering_options`: process the
             output as part of a stream to be continued.

             This option must be set or reset before calling
             :func:`ubidi_set_para`.

             This option specifies that the caller is interested in
             processing large text object in parts. The
             results of the successive calls are expected to be concatenated
             by the caller. Only the call for the
             last part will have this option bit off.

             When this option bit is on, :func:`ubidi_set_para` may process less
             than the full source text in order to
             truncate the text at a meaningful boundary. The caller should
             call :func:`ubidi_get_processed_length`
             immediately after calling :func:`ubidi_set_para` in order to
             determine how much of the source text has been
             processed. Source text beyond that length should be resubmitted
             in following calls to :func:`ubidi_set_para`.
             The processed length may be less than the length of the source
             text if a character preceding the last
             character of the source text constitutes a reasonable boundary
             (like a block separator) for text to be continued.

             If the last character of the source text constitutes a
             reasonable boundary, the whole text will be processed at once.

             If nowhere in the source text there exists such a reasonable
             boundary, the processed length will be zero.

             The caller should check for such an occurrence and do one of the
             following:

             - Submit a larger amount of text with a better chance to include
               a reasonable boundary.
             - Resubmit the same text after turning off option
               :attr:`UBIDI_OPTION_STREAMING`.

             In all cases, this option should be turned off before processing
             the last part of the text.

             When the :attr:`UBIDI_OPTION_STREAMING` option is used, it is
             recommended to call
             :func:`ubidi_order_paragraphs_ltr` with argument
             *order_paragraphs_ltr* set to ``True`` before calling
             :func:`ubidi_set_para` so that later paragraphs may be concatenated to
             previous paragraphs on the right.
             )doc")
      .export_values();

  //
  // UBiDiClassCallback
  //
  py::class_<icupy::UBiDiClassCallbackPtr> bccb(m, "UBiDiClassCallback", R"doc(
    Wrapper class for a callback function that override the default Bidi class
    values with custom values.

    See Also:
        :func:`ubidi_get_class_callback`
        :func:`ubidi_set_class_callback`
    )doc");

  bccb.def(py::init<>(), R"doc(
          Initialize the ``UBiDiClassCallback`` instance without a callback
          function.
          )doc")
      .def(py::init([](const icupy::ClassCallbackFunction &action,
                       std::optional<const icupy::ConstVoidPtr *> &context) {
             return std::make_unique<icupy::UBiDiClassCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
           Initialize the ``UBiDiClassCallback`` instance with the specified
           callback function and the user context.

           .. important::

               *action* and *context* must outlive the ``UBiDiClassCallback``
               object.
           )doc");

  bccb.def(
      "__bool__",
      [](const icupy::UBiDiClassCallbackPtr &self) { return !self.empty(); },
      R"doc(
      Return ``True`` if the ``UBiDiClassCallback`` has a callback function.
      )doc");

  bccb.def(
      "context",
      [](const icupy::UBiDiClassCallbackPtr &self)
          -> std::optional<const icupy::ConstVoidPtr *> {
        auto pair = self.context();
        if (pair == nullptr || pair->second == nullptr) {
          return std::nullopt;
        }
        return pair->second;
      },
      py::return_value_policy::reference,
      R"doc(
      Return the user context.
      )doc");

  //
  // struct UBiDi
  //
  py::class_<icupy::UBiDiPtr>(m, "UBiDi", R"doc(
  UBiDi structure.

  See Also:
      :func:`ubidi_close`
      :func:`ubidi_open_sized`
      :func:`ubidi_open`
  )doc");

  //
  // Functions
  //
  m.def(
      "ubidi_close", [](icupy::UBiDiPtr &bidi) { ubidi_close(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_count_paragraphs",
      [](icupy::UBiDiPtr &bidi) { return ubidi_countParagraphs(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_count_runs",
      [](icupy::UBiDiPtr &bidi) {
        ErrorCode error_code;
        auto result = ubidi_countRuns(bidi, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"));

  m.def("ubidi_get_base_direction", &ubidi_getBaseDirection, py::arg("text"),
        py::arg("length"));

  m.def(
      "ubidi_get_class_callback",
      [](icupy::UBiDiPtr &bidi) -> std::optional<icupy::UBiDiClassCallbackPtr> {
        UBiDiClassCallback *fn = nullptr;
        const void *context = nullptr;
        ubidi_getClassCallback(bidi, &fn, &context);
        if (context == nullptr) {
          return std::nullopt;
        }
        auto pair = reinterpret_cast<icupy::ClassCallbackAndContextPair *>(
            const_cast<void *>(context));
        return icupy::UBiDiClassCallbackPtr(pair);
      },
      py::arg("bidi"), R"doc(
      Return the callback function for this ``UBiDi``.

      See Also:
          :func:`ubidi_set_class_callback`
      )doc");

  m.def(
      "ubidi_get_customized_class",
      [](icupy::UBiDiPtr &bidi, UChar32 c) {
        return ubidi_getCustomizedClass(bidi, c);
      },
      py::arg("bidi"), py::arg("c"), R"doc(
      Return the Bidi class for the specified code point.

      See Also:
          :class:`UBiDiClassCallback`
          :func:`ubidi_set_class_callback`
      )doc");

  m.def(
      "ubidi_get_direction",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getDirection(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_length",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getLength(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_level_at",
      [](icupy::UBiDiPtr &bidi, int32_t char_index) {
        return ubidi_getLevelAt(bidi, char_index);
      },
      py::arg("bidi"), py::arg("char_index"));

  m.def(
      "ubidi_get_levels",
      [](icupy::UBiDiPtr &bidi) {
        ErrorCode error_code;
        auto p = ubidi_getLevels(bidi, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        const auto length = ubidi_getProcessedLength(bidi);
        std::vector<UBiDiLevel> result(p, p + length);
        return result;
      },
      py::arg("bidi"));

  m.def(
      "ubidi_get_logical_index",
      [](icupy::UBiDiPtr &bidi, int32_t visual_index) {
        ErrorCode error_code;
        auto result = ubidi_getLogicalIndex(bidi, visual_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"), py::arg("visual_index"));

  m.def(
      "ubidi_get_logical_map",
      [](icupy::UBiDiPtr &bidi) {
        const int32_t length =
            ubidi_getReorderingOptions(bidi) & UBIDI_OPTION_INSERT_MARKS
                ? ubidi_getResultLength(bidi)
                : ubidi_getProcessedLength(bidi);
        std::vector<int32_t> result(length, 0);
        ErrorCode error_code;
        ubidi_getLogicalMap(bidi, result.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"));

  m.def(
      "ubidi_get_logical_run",
      [](icupy::UBiDiPtr &bidi, int32_t logical_position) {
        int32_t logical_limit;
        UBiDiLevel level;
        ubidi_getLogicalRun(bidi, logical_position, &logical_limit, &level);
        return py::make_tuple(logical_limit, level);
      },
      py::arg("bidi"), py::arg("logical_position"));

  m.def(
      "ubidi_get_paragraph",
      [](icupy::UBiDiPtr &bidi, int32_t char_index) {
        int32_t para_start, para_limit;
        UBiDiLevel para_level;
        ErrorCode error_code;
        auto result = ubidi_getParagraph(bidi, char_index, &para_start,
                                         &para_limit, &para_level, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::make_tuple(result, para_start, para_limit, para_level);
      },
      py::arg("bidi"), py::arg("char_index"));

  m.def(
      "ubidi_get_paragraph_by_index",
      [](icupy::UBiDiPtr &bidi, int32_t para_index) {
        int32_t para_start, para_limit;
        UBiDiLevel para_level;
        ErrorCode error_code;
        ubidi_getParagraphByIndex(bidi, para_index, &para_start, &para_limit,
                                  &para_level, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::make_tuple(para_start, para_limit, para_level);
      },
      py::arg("bidi"), py::arg("para_index"));

  m.def(
      "ubidi_get_para_level",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getParaLevel(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_processed_length",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getProcessedLength(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_reordering_mode",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getReorderingMode(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_reordering_options",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getReorderingOptions(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_result_length",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getResultLength(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_get_text",
      [](icupy::UBiDiPtr &bidi) { return ubidi_getText(bidi); },
      py::return_value_policy::reference, py::arg("bidi"));

  m.def(
      "ubidi_get_visual_index",
      [](icupy::UBiDiPtr &bidi, int32_t logical_index) {
        ErrorCode error_code;
        auto result = ubidi_getVisualIndex(bidi, logical_index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"), py::arg("logical_index"));

  m.def(
      "ubidi_get_visual_map",
      [](icupy::UBiDiPtr &bidi) {
        const int32_t length =
            ubidi_getReorderingOptions(bidi) & UBIDI_OPTION_REMOVE_CONTROLS
                ? ubidi_getProcessedLength(bidi)
                : ubidi_getResultLength(bidi);
        std::vector<int32_t> result(length, 0);
        ErrorCode error_code;
        ubidi_getVisualMap(bidi, result.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("bidi"));

  m.def(
      "ubidi_get_visual_run",
      [](icupy::UBiDiPtr &bidi, int32_t run_index) {
        int32_t logical_start, length;
        auto result =
            ubidi_getVisualRun(bidi, run_index, &logical_start, &length);
        return py::make_tuple(result, logical_start, length);
      },
      py::arg("bidi"), py::arg("run_index"));

  m.def(
      "ubidi_invert_map",
      [](const std::vector<int32_t> &src_map, int32_t length) {
        std::vector<int32_t> result(length, 0);
        ubidi_invertMap(src_map.data(), result.data(), length);
        return result;
      },
      py::arg("src_map"), py::arg("length"));

  m.def(
      "ubidi_is_inverse",
      [](icupy::UBiDiPtr &bidi) -> py::bool_ { return ubidi_isInverse(bidi); },
      py::arg("bidi"));

  m.def(
      "ubidi_is_order_paragraphs_ltr",
      [](icupy::UBiDiPtr &bidi) -> py::bool_ {
        return ubidi_isOrderParagraphsLTR(bidi);
      },
      py::arg("bidi"));

  m.def("ubidi_open", []() {
    auto bidi = ubidi_open();
    return std::make_unique<icupy::UBiDiPtr>(bidi);
  });

  m.def(
      "ubidi_open_sized",
      [](int32_t max_length, int32_t max_run_count) {
        ErrorCode error_code;
        auto bidi = ubidi_openSized(max_length, max_run_count, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UBiDiPtr>(bidi);
      },
      py::arg("max_length"), py::arg("max_run_count"));

  m.def(
      "ubidi_order_paragraphs_ltr",
      [](icupy::UBiDiPtr &bidi, py::bool_ order_paragraphs_ltr) {
        ubidi_orderParagraphsLTR(bidi, order_paragraphs_ltr);
      },
      py::arg("bidi"), py::arg("order_paragraphs_ltr"));

  m.def(
      "ubidi_reorder_logical",
      [](const std::vector<UBiDiLevel> &levels, int32_t length) {
        std::vector<int32_t> result(length, 0);
        ubidi_reorderLogical(levels.data(), length, result.data());
        return result;
      },
      py::arg("levels"), py::arg("length"));

  m.def(
      "ubidi_reorder_visual",
      [](const std::vector<UBiDiLevel> &levels, int32_t length) {
        std::vector<int32_t> result(length, 0);
        ubidi_reorderVisual(levels.data(), length, result.data());
        return result;
      },
      py::arg("levels"), py::arg("length"));

  m.def(
      "ubidi_set_class_callback",
      [](icupy::UBiDiPtr &bidi, icupy::UBiDiClassCallbackPtr &new_fn)
          -> std::optional<icupy::UBiDiClassCallbackPtr> {
        auto fn = new_fn.get_native_callback();
        auto context = new_fn.context();
        UBiDiClassCallback *old_fn = nullptr;
        const void *old_context = nullptr;
        ErrorCode error_code;
        ubidi_setClassCallback(bidi, fn, context, &old_fn, &old_context,
                               error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        if (old_context == nullptr) {
          return std::nullopt;
        }
        auto pair = reinterpret_cast<icupy::ClassCallbackAndContextPair *>(
            const_cast<void *>(old_context));
        return icupy::UBiDiClassCallbackPtr(pair);
      },
      py::arg("bidi"), py::arg("new_fn"), R"doc(
      Set the new callback function to be used with this ``UBiDi``, and return
      the old callback function.

      .. important::

          *new_fn* must outlive the ``UBiDi`` object.

      See Also:
          :func:`ubidi_get_class_callback`
          :func:`ubidi_get_customized_class`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> def class_callback(options: dict[str, icu.UCharDirection], c: int) -> icu.UCharDirection:
          ...     if icu.u_isdigit(c):
          ...         return icu.U_LEFT_TO_RIGHT
          ...     return options["default"]
          ...
          >>> with gc(icu.ubidi_open(), icu.ubidi_close) as bidi:
          ...     icu.ubidi_get_customized_class(bidi, 0x31)  # U_EUROPEAN_NUMBER
          ...     icu.ubidi_get_customized_class(bidi, 0x661)  # U_ARABIC_NUMBER
          ...     icu.ubidi_get_customized_class(bidi, 0x2c)  # U_COMMON_NUMBER_SEPARATOR
          ...     value = icu.u_get_int_property_max_value(icu.UCHAR_BIDI_CLASS) + 1
          ...     custom_classes: dict[str, icu.UCharDirection] = {
          ...         "default": icu.UCharDirection(value),
          ...     }
          ...     context = icu.ConstVoidPtr(custom_classes)
          ...     new_fn = icu.UBiDiClassCallback(class_callback, context)
          ...     old_fn = icu.ubidi_set_class_callback(bidi, new_fn)
          ...     icu.ubidi_get_customized_class(bidi, 0x31)  # U_EUROPEAN_NUMBER
          ...     icu.ubidi_get_customized_class(bidi, 0x661)  # U_ARABIC_NUMBER
          ...     icu.ubidi_get_customized_class(bidi, 0x2c)  # U_COMMON_NUMBER_SEPARATOR
          ...
          <UCharDirection.U_EUROPEAN_NUMBER: 2>
          <UCharDirection.U_ARABIC_NUMBER: 5>
          <UCharDirection.U_COMMON_NUMBER_SEPARATOR: 6>
          <UCharDirection.U_LEFT_TO_RIGHT: 0>
          <UCharDirection.U_LEFT_TO_RIGHT: 0>
          <UCharDirection.U_COMMON_NUMBER_SEPARATOR: 6>
      )doc");

  m.def(
      "ubidi_set_context",
      [](icupy::UBiDiPtr &bidi, std::optional<const std::u16string> &prologue,
         int32_t pro_length, std::optional<const std::u16string> &epilogue,
         int32_t epi_length) {
        auto normalized_pro_length = pro_length;
        auto normalized_epi_length = epi_length;
        if (prologue && normalized_pro_length == -1) {
          normalized_pro_length = static_cast<int32_t>(prologue->size());
        }
        if (epilogue && normalized_epi_length == -1) {
          normalized_epi_length = static_cast<int32_t>(epilogue->size());
        }
        auto prologue_ptr = std::make_shared<std::u16string>(
            prologue.value_or(u""), 0, std::max(normalized_pro_length, 0));
        auto epilogue_ptr = std::make_shared<std::u16string>(
            epilogue.value_or(u""), 0, std::max(normalized_epi_length, 0));
        ErrorCode error_code;
        ubidi_setContext(bidi, prologue ? prologue_ptr->data() : nullptr,
                         pro_length, epilogue ? epilogue_ptr->data() : nullptr,
                         epi_length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        bidi.set_prologue(prologue_ptr);
        bidi.set_epilogue(epilogue_ptr);
      },
      py::arg("bidi"), py::arg("prologue"), py::arg("pro_length"),
      py::arg("epilogue"), py::arg("epi_length"));

  m.def(
      "ubidi_set_inverse",
      [](icupy::UBiDiPtr &bidi, py::bool_ is_inverse) {
        ubidi_setInverse(bidi, is_inverse);
      },
      py::arg("bidi"), py::arg("is_inverse"));

  m.def(
      "ubidi_set_line",
      [](icupy::UBiDiPtr &para_bidi, int32_t start, int32_t limit,
         icupy::UBiDiPtr &line_bidi) {
        ErrorCode error_code;
        ubidi_setLine(para_bidi, start, limit, line_bidi, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("para_bidi"), py::arg("start"), py::arg("limit"),
      py::arg("line_bidi"));

  m.def(
      "ubidi_set_para",
      [](icupy::UBiDiPtr &bidi, const std::u16string &text, int32_t length,
         UBiDiLevel para_level,
         std::optional<std::vector<UBiDiLevel>> &embedding_levels) {
        auto normalized_length = length;
        if (normalized_length == -1) {
          normalized_length = static_cast<int32_t>(text.size());
        }
        auto text_ptr = std::make_shared<std::u16string>(
            text, 0, std::max(normalized_length, 0));
        auto embedding_levels_ptr = std::shared_ptr<UBiDiLevel[]>();
        if (embedding_levels && embedding_levels->size() > 0) {
          embedding_levels_ptr = std::shared_ptr<UBiDiLevel[]>(
              new UBiDiLevel[embedding_levels->size()]);
          std::copy(embedding_levels->begin(), embedding_levels->end(),
                    embedding_levels_ptr.get());
        }
        ErrorCode error_code;
        ubidi_setPara(bidi, text_ptr->data(), length, para_level,
                      embedding_levels && embedding_levels->size() > 0
                          ? embedding_levels_ptr.get()
                          : nullptr,
                      error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        bidi.set_text(text_ptr);
        bidi.set_embedding_levels(embedding_levels_ptr);
      },
      py::arg("bidi"), py::arg("text"), py::arg("length"),
      py::arg("para_level"), py::arg("embedding_levels") = std::nullopt);

  m.def(
      "ubidi_set_reordering_mode",
      [](icupy::UBiDiPtr &bidi, UBiDiReorderingMode reordering_mode) {
        ubidi_setReorderingMode(bidi, reordering_mode);
      },
      py::arg("bidi"), py::arg("reordering_mode"));

  m.def(
      "ubidi_set_reordering_options",
      [](icupy::UBiDiPtr &bidi, int32_t reordering_options) {
        ubidi_setReorderingOptions(bidi, reordering_options);
      },
      py::arg("bidi"), py::arg("reordering_options"));

  m.def(
      "ubidi_write_reordered",
      [](icupy::UBiDiPtr &bidi, uint16_t options) {
        int32_t dest_size = 0;
        ErrorCode error_code;
        if (options & UBIDI_INSERT_LRM_FOR_NUMERIC) {
          dest_size =
              ubidi_getLength(bidi) + 2 * ubidi_countRuns(bidi, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
        } else if (options & UBIDI_REMOVE_BIDI_CONTROLS) {
          dest_size = ubidi_getLength(bidi);
        } else {
          dest_size = ubidi_getProcessedLength(bidi);
        }
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        dest_size = ubidi_writeReordered(bidi, result.data(), dest_size,
                                         options, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        result.resize(dest_size);
        return result;
      },
      py::arg("bidi"), py::arg("options"));

  m.def(
      "ubidi_write_reverse",
      [](const std::u16string &src, int32_t src_length, uint16_t options) {
        auto dest_size = src_length;
        if (dest_size < 0) {
          dest_size = static_cast<int32_t>(src.size());
        }
        std::u16string result(dest_size, u'\0');
        ErrorCode error_code;
        dest_size = ubidi_writeReverse(src.data(), src_length, result.data(),
                                       dest_size, options, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        result.resize(dest_size);
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

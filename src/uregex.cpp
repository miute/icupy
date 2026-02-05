#include "uregex.hpp"
#include "voidptr.hpp"
#include <pybind11/stl.h>

namespace icupy {

//
// URegexFindProgressCallback
//
UBool URegexFindProgressCallbackPtr::callback(const void *native_context,
                                              int64_t match_index) {
  if (native_context == nullptr) {
    throw std::runtime_error("URegexFindProgressCallback: context is not set");
  }
  auto pair = reinterpret_cast<FindProgressCallbackAndContextPair *>(
      const_cast<void *>(native_context));
  if (pair->second == nullptr) {
    throw std::runtime_error(
        "URegexFindProgressCallback: callback context is not set");
  }
  auto &action = pair->first;
  if (!action) {
    throw std::runtime_error(
        "URegexFindProgressCallback: callback function is not set or invalid");
  }
  auto context = pair->second;
  auto value = context->value();
  return action(value, match_index);
}

//
// URegexMatchCallback
//
UBool URegexMatchCallbackPtr::callback(const void *native_context,
                                       int32_t steps) {
  if (native_context == nullptr) {
    throw std::runtime_error("URegexMatchCallback: context is not set");
  }
  auto pair = reinterpret_cast<MatchCallbackAndContextPair *>(
      const_cast<void *>(native_context));
  if (pair->second == nullptr) {
    throw std::runtime_error(
        "URegexMatchCallback: callback context is not set");
  }
  auto &action = pair->first;
  if (!action) {
    throw std::runtime_error(
        "URegexMatchCallback: callback function is not set or invalid");
  }
  auto context = pair->second;
  auto value = context->value();
  return action(value, steps);
}

} // namespace icupy

void init_uregex(py::module &m) {
  //
  // enum URegexpFlag
  //
  py::enum_<URegexpFlag>(m, "URegexpFlag", py::arithmetic(),
                         "Constants for Regular Expression Match Modes.")
      .value("UREGEX_CANON_EQ", UREGEX_CANON_EQ,
             "Forces normalization of pattern and strings.\n\n  "
             "Not implemented yet, just a placeholder, hence draft.")
      .value("UREGEX_CASE_INSENSITIVE", UREGEX_CASE_INSENSITIVE,
             "Enable case insensitive matching.")
      .value("UREGEX_COMMENTS", UREGEX_COMMENTS,
             "Allow white space and comments within patterns.")
      .value("UREGEX_DOTALL", UREGEX_DOTALL,
             "If set, '.' matches line terminators, otherwise '.' matching "
             "stops at line end.")
      .value("UREGEX_LITERAL", UREGEX_LITERAL,
             "If set, treat the entire pattern as a literal string.\n\n  "
             "Metacharacters or escape sequences in the input sequence will be "
             "given no special meaning.\n\n  "
             "The flag *UREGEX_CASE_INSENSITIVE* retains its impact on "
             "matching when used in conjunction with this "
             "flag. The other flags become superfluous.")
      .value("UREGEX_MULTILINE", UREGEX_MULTILINE,
             "Control behavior of \"$\" and \"^\" If set, recognize line "
             "terminators within string, otherwise, match "
             "only at start and end of input string.")
      .value("UREGEX_UNIX_LINES", UREGEX_UNIX_LINES,
             "Unix-only line endings.\n\n  "
             "When this mode is enabled, only \\\\u000a is recognized as a "
             "line ending in the behavior of ., ^, and $.")
      .value(
          "UREGEX_UWORD", UREGEX_UWORD,
          "Unicode word boundaries.\n\n  "
          "If set, uses the Unicode TR 29 definition of word boundaries. "
          "Warning: Unicode word boundaries are quite "
          "different from traditional regular expression word boundaries. See "
          "http://unicode.org/reports/tr29/#Word_Boundaries")
      .value("UREGEX_ERROR_ON_UNKNOWN_ESCAPES", UREGEX_ERROR_ON_UNKNOWN_ESCAPES,
             "Error on Unrecognized backslash escapes.\n\n  "
             "If set, fail with an error on patterns that contain "
             "backslash-escaped ASCII letters without a known "
             "special meaning. If this flag is not set, these escaped letters "
             "represent themselves.")
      .export_values();

  //
  // URegexFindProgressCallback
  //
  py::class_<icupy::URegexFindProgressCallbackPtr> rfcb(
      m, "URegexFindProgressCallback", R"doc(
      Wrapper class for a regular expression find callback function.
      )doc");

  rfcb.def(py::init<>(), R"doc(
           Initialize the ``URegexFindProgressCallback`` instance without a callback
           function.
           )doc")
      .def(py::init<const std::function<icupy::FindProgressCallbackArgs> &,
                    const icupy::ConstVoidPtr *>(),
           py::arg("action"), py::arg("context").none(false), R"doc(
           Initialize the ``URegexFindProgressCallback`` instance with a callback function
           `action` and the user context `context`.

           `action` and `context` must outlive the ``URegexFindProgressCallback`` object.
           )doc");

  rfcb.def(
      "__bool__",
      [](const icupy::URegexFindProgressCallbackPtr &self) {
        return !self.empty();
      },
      R"doc(
      Return ``True`` if the ``URegexFindProgressCallback`` has a callback function.
      )doc");

  rfcb.def(
      "context",
      [](const icupy::URegexFindProgressCallbackPtr &self)
          -> std::optional<const icupy::ConstVoidPtr *> {
        auto pair = self.context();
        if (pair == nullptr) {
          return std::nullopt;
        }
        return pair->second;
      },
      py::return_value_policy::reference,
      R"doc(
      Get the user context.
      )doc");

  //
  // URegexMatchCallback
  //
  py::class_<icupy::URegexMatchCallbackPtr> rmcb(m, "URegexMatchCallback",
                                                 R"doc(
      Wrapper class for a regular expression matching callback function.
      )doc");

  rmcb.def(py::init<>(), R"doc(
           Initialize the ``URegexMatchCallback`` instance without a callback
           function.
           )doc")
      .def(py::init<const std::function<icupy::MatchCallbackArgs> &,
                    const icupy::ConstVoidPtr *>(),
           py::arg("action"), py::arg("context").none(false), R"doc(
           Initialize the ``URegexMatchCallback`` instance with a callback function
           `action` and the user context `context`.

           `action` and `context` must outlive the ``URegexMatchCallback`` object.
           )doc");

  rmcb.def(
      "__bool__",
      [](const icupy::URegexMatchCallbackPtr &self) { return !self.empty(); },
      R"doc(
      Return ``True`` if the ``URegexMatchCallback`` has a callback function.
      )doc");

  rmcb.def(
      "context",
      [](const icupy::URegexMatchCallbackPtr &self)
          -> std::optional<const icupy::ConstVoidPtr *> {
        auto pair = self.context();
        if (pair == nullptr) {
          return std::nullopt;
        }
        return pair->second;
      },
      py::return_value_policy::reference,
      R"doc(
      Get the user context.
      )doc");
}

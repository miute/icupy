#include "uregex.hpp"
#include "voidptr.hpp"
#include <optional>
#include <pybind11/functional.h>
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
  auto &action = pair->first;
  if (!action) {
    throw std::runtime_error(
        "URegexFindProgressCallback: callback function is not set or invalid");
  }
  auto context = pair->second;
  auto object = context ? context->value() : py::none();
  return action(object, match_index);
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
  auto &action = pair->first;
  if (!action) {
    throw std::runtime_error(
        "URegexMatchCallback: callback function is not set or invalid");
  }
  auto context = pair->second;
  auto object = context ? context->value() : py::none();
  return action(object, steps);
}

} // namespace icupy

void init_uregex(py::module &m) {
  //
  // enum URegexpFlag
  //
  py::enum_<URegexpFlag>(m, "URegexpFlag", py::arithmetic(), R"doc(
Constants for Regular Expression Match Modes.

See Also:
    :meth:`RegexMatcher.__init__`
      )doc")
      .value("UREGEX_CANON_EQ", UREGEX_CANON_EQ, R"doc(
             Forces normalization of pattern and strings.

             Not implemented yet, just a placeholder, hence draft.
             )doc")
      .value("UREGEX_CASE_INSENSITIVE", UREGEX_CASE_INSENSITIVE, R"doc(
             Enable case insensitive matching.
             )doc")
      .value("UREGEX_COMMENTS", UREGEX_COMMENTS, R"doc(
             Allow white space and comments within patterns.
             )doc")
      .value("UREGEX_DOTALL", UREGEX_DOTALL, R"doc(
             If set, '.' matches line terminators, otherwise '.' matching
             stops at line end.
             )doc")
      .value("UREGEX_LITERAL", UREGEX_LITERAL, R"doc(
             If set, treat the entire pattern as a literal string.

             Metacharacters or escape sequences in the input sequence will be
             given no special meaning.

             The flag :attr:`UREGEX_CASE_INSENSITIVE` retains its impact on
             matching when used in conjunction with this
             flag. The other flags become superfluous.
             )doc")
      .value("UREGEX_MULTILINE", UREGEX_MULTILINE, R"doc(
             Control behavior of '$' and '^' If set, recognize line
             terminators within string, otherwise, match
             only at start and end of input string.
             )doc")
      .value("UREGEX_UNIX_LINES", UREGEX_UNIX_LINES, R"doc(
             Unix-only line endings.

             When this mode is enabled, only '\\u000a' is recognized as a
             line ending in the behavior of '.', '^', and '$'.
             )doc")
      .value("UREGEX_UWORD", UREGEX_UWORD, R"doc(
             Unicode word boundaries.

             If set, uses the Unicode TR 29 definition of word boundaries.

             Warning: Unicode word boundaries are quite
             different from traditional regular expression word boundaries. See
             http://unicode.org/reports/tr29/#Word_Boundaries
             )doc")
      .value("UREGEX_ERROR_ON_UNKNOWN_ESCAPES", UREGEX_ERROR_ON_UNKNOWN_ESCAPES,
             R"doc(
             Error on Unrecognized backslash escapes.

             If set, fail with an error on patterns that contain
             backslash-escaped ASCII letters without a known
             special meaning. If this flag is not set, these escaped letters
             represent themselves.
             )doc")
      .export_values();

  //
  // URegexFindProgressCallback
  //
  py::class_<icupy::URegexFindProgressCallbackPtr> rfcb(
      m, "URegexFindProgressCallback", R"doc(
      Wrapper class for a regular expression find callback function.

      See Also:
          :meth:`RegexMatcher.get_find_progress_callback`
          :meth:`RegexMatcher.set_find_progress_callback`
      )doc");

  rfcb.def(py::init<>(), R"doc(
           Initialize the ``URegexFindProgressCallback`` instance without a
           callback function.
           )doc")
      .def(py::init([](const icupy::FindProgressCallbackFunction &action,
                       std::optional<const icupy::ConstVoidPtr *> &context) {
             return std::make_unique<icupy::URegexFindProgressCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
           Initialize the ``URegexFindProgressCallback`` instance with the
           specified callback function and the user context.

           .. important::

               *action* and *context* must outlive the
               ``URegexFindProgressCallback`` object.
           )doc");

  rfcb.def(
      "__bool__",
      [](const icupy::URegexFindProgressCallbackPtr &self) {
        return !self.empty();
      },
      R"doc(
      Return ``True`` if the ``URegexFindProgressCallback`` has a callback
      function.
      )doc");

  rfcb.def(
      "context",
      [](const icupy::URegexFindProgressCallbackPtr &self)
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
  // URegexMatchCallback
  //
  py::class_<icupy::URegexMatchCallbackPtr> rmcb(m, "URegexMatchCallback",
                                                 R"doc(
      Wrapper class for a regular expression matching callback function.

      See Also:
          :meth:`RegexMatcher.get_match_callback`
          :meth:`RegexMatcher.set_match_callback`
      )doc");

  rmcb.def(py::init<>(), R"doc(
           Initialize the ``URegexMatchCallback`` instance without a callback
           function.
           )doc")
      .def(py::init([](const icupy::MatchCallbackFunction &action,
                       std::optional<const icupy::ConstVoidPtr *> &context) {
             return std::make_unique<icupy::URegexMatchCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
           Initialize the ``URegexMatchCallback`` instance with the specified
           callback function and the user context.

           .. important::

               *action* and *context* must outlive the ``URegexMatchCallback``
               object.
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
        if (pair == nullptr || pair->second == nullptr) {
          return std::nullopt;
        }
        return pair->second;
      },
      py::return_value_policy::reference,
      R"doc(
      Return the user context.
      )doc");
}

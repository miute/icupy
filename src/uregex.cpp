#include "uregex.hpp"
#include "voidptr.hpp"

_URegexFindProgressCallbackPtr::_URegexFindProgressCallbackPtr(URegexFindProgressCallback *action) : action_(action) {}
_URegexFindProgressCallbackPtr::_URegexFindProgressCallbackPtr(const py::function &action) : action_(action) {}
_URegexFindProgressCallbackPtr::~_URegexFindProgressCallbackPtr() {}

UBool _URegexFindProgressCallbackPtr::callback(const void *context, int64_t match_index) {
  if (context == nullptr) {
    return false;
  }
  auto cp = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
  auto python_context = cp->to_object();
  auto &action = cp->get_action();
  return action(python_context, match_index).cast<UBool>();
}

_URegexMatchCallbackPtr::_URegexMatchCallbackPtr(URegexMatchCallback *action) : action_(action) {}
_URegexMatchCallbackPtr::_URegexMatchCallbackPtr(const py::function &action) : action_(action) {}
_URegexMatchCallbackPtr::~_URegexMatchCallbackPtr() {}

UBool _URegexMatchCallbackPtr::callback(const void *context, int32_t steps) {
  if (context == nullptr) {
    return false;
  }
  auto cp = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
  auto python_context = cp->to_object();
  auto &action = cp->get_action();
  return action(python_context, steps).cast<UBool>();
}

void init_uregex(py::module &m) {
  py::enum_<URegexpFlag>(m, "URegexpFlag", py::arithmetic(), "Constants for Regular Expression Match Modes.")
      .value("UREGEX_CANON_EQ", UREGEX_CANON_EQ,
             "Forces normalization of pattern and strings.\n\n  "
             "Not implemented yet, just a placeholder, hence draft.")
      .value("UREGEX_CASE_INSENSITIVE", UREGEX_CASE_INSENSITIVE, "Enable case insensitive matching.")
      .value("UREGEX_COMMENTS", UREGEX_COMMENTS, "Allow white space and comments within patterns.")
      .value("UREGEX_DOTALL", UREGEX_DOTALL,
             "If set, '.' matches line terminators, otherwise '.' matching stops at line end.")
      .value("UREGEX_LITERAL", UREGEX_LITERAL,
             "If set, treat the entire pattern as a literal string.\n\n  "
             "Metacharacters or escape sequences in the input sequence will be given no special meaning.\n\n  "
             "The flag *UREGEX_CASE_INSENSITIVE* retains its impact on matching when used in conjunction with this "
             "flag. The other flags become superfluous.")
      .value("UREGEX_MULTILINE", UREGEX_MULTILINE,
             "Control behavior of \"$\" and \"^\" If set, recognize line terminators within string, otherwise, match "
             "only at start and end of input string.")
      .value("UREGEX_UNIX_LINES", UREGEX_UNIX_LINES,
             "Unix-only line endings.\n\n  "
             "When this mode is enabled, only \\\\u000a is recognized as a line ending in the behavior of ., ^, and $.")
      .value("UREGEX_UWORD", UREGEX_UWORD,
             "Unicode word boundaries.\n\n  "
             "If set, uses the Unicode TR 29 definition of word boundaries. Warning: Unicode word boundaries are quite "
             "different from traditional regular expression word boundaries. See "
             "http://unicode.org/reports/tr29/#Word_Boundaries")
      .value("UREGEX_ERROR_ON_UNKNOWN_ESCAPES", UREGEX_ERROR_ON_UNKNOWN_ESCAPES,
             "Error on Unrecognized backslash escapes.\n\n  "
             "If set, fail with an error on patterns that contain backslash-escaped ASCII letters without a known "
             "special meaning. If this flag is not set, these escaped letters represent themselves.")
      .export_values();

  py::class_<_URegexFindProgressCallbackPtr>(m, "URegexFindProgressCallbackPtr")
      .def(py::init<py::function>(), py::arg("action"));

  py::class_<_URegexMatchCallbackPtr>(m, "URegexMatchCallbackPtr").def(py::init<py::function>(), py::arg("action"));
}

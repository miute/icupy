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
  py::enum_<URegexpFlag>(m, "URegexpFlag", py::arithmetic())
      .value("UREGEX_CANON_EQ", UREGEX_CANON_EQ)
      .value("UREGEX_CASE_INSENSITIVE", UREGEX_CASE_INSENSITIVE)
      .value("UREGEX_COMMENTS", UREGEX_COMMENTS)
      .value("UREGEX_DOTALL", UREGEX_DOTALL)
      .value("UREGEX_LITERAL", UREGEX_LITERAL)
      .value("UREGEX_MULTILINE", UREGEX_MULTILINE)
      .value("UREGEX_UNIX_LINES", UREGEX_UNIX_LINES)
      .value("UREGEX_UWORD", UREGEX_UWORD)
      .value("UREGEX_ERROR_ON_UNKNOWN_ESCAPES", UREGEX_ERROR_ON_UNKNOWN_ESCAPES)
      .export_values();

  py::class_<_URegexFindProgressCallbackPtr>(m, "URegexFindProgressCallbackPtr")
      .def(py::init<py::function>(), py::arg("action"));

  py::class_<_URegexMatchCallbackPtr>(m, "URegexMatchCallbackPtr").def(py::init<py::function>(), py::arg("action"));
}

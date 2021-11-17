#include "ucnv_err.hpp"
#include "voidptr.hpp"

_UConverterPtr::_UConverterPtr(UConverter *p) : p_(p) {}
_UConverterPtr::~_UConverterPtr() {}
UConverter *_UConverterPtr::get() const { return p_; }

_UConverterFromUCallbackPtr::_UConverterFromUCallbackPtr(UConverterFromUCallback action) : action_(action) {}
_UConverterFromUCallbackPtr::_UConverterFromUCallbackPtr(const py::function &action) : action_(action) {}
_UConverterFromUCallbackPtr::~_UConverterFromUCallbackPtr() {}

void _UConverterFromUCallbackPtr::callback(const void *context, UConverterFromUnicodeArgs *args,
                                           const UChar *code_units, int32_t length, UChar32 code_point,
                                           UConverterCallbackReason reason, UErrorCode *error_code) {
  if (context == nullptr) {
    return;
  }
  auto cp = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
  auto python_context = cp->to_object();
  auto &action = cp->get_action();
  *error_code = action(python_context, args, code_units, length, code_point, reason, *error_code).cast<UErrorCode>();
}

_UConverterToUCallbackPtr::_UConverterToUCallbackPtr(UConverterToUCallback action) : action_(action) {}
_UConverterToUCallbackPtr::_UConverterToUCallbackPtr(const py::function &action) : action_(action) {}
_UConverterToUCallbackPtr::~_UConverterToUCallbackPtr() {}

void _UConverterToUCallbackPtr::callback(const void *context, UConverterToUnicodeArgs *args, const char *code_units,
                                         int32_t length, UConverterCallbackReason reason, UErrorCode *error_code) {
  if (context == nullptr) {
    return;
  }
  auto cp = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
  auto python_context = cp->to_object();
  auto &action = cp->get_action();
  *error_code =
      action(python_context, args, py::bytes(code_units, length), length, reason, *error_code).cast<UErrorCode>();
}

void init_ucnv_err(py::module &m) {
  py::enum_<UConverterCallbackReason>(
      m, "UConverterCallbackReason", py::arithmetic(),
      "The process condition code to be used with the callbacks.\n\n"
      "Codes which are greater than *UCNV_IRREGULAR* should be passed on to any chained callbacks.")
      .value("UCNV_UNASSIGNED", UCNV_UNASSIGNED,
             "The code point is unassigned.\n\n  "
             "The error code *U_INVALID_CHAR_FOUND* will be set.")
      .value("UCNV_ILLEGAL", UCNV_ILLEGAL,
             "The code point is illegal.\n\n  "
             "For example, \\\\x81\\\\x2E is illegal in SJIS because \\\\x2E is not a valid trail byte for the \\\\x81 "
             "lead byte. Also, starting with Unicode 3.0.1, non-shortest byte sequences in UTF-8 (like \\\\xC1\\\\xA1 "
             "instead of \\\\x61 for U+0061) are also illegal, not just irregular. The error code "
             "*U_ILLEGAL_CHAR_FOUND* will be set.")
      .value("UCNV_IRREGULAR", UCNV_IRREGULAR,
             "The codepoint is not a regular sequence in the encoding.\n\n  "
             "For example, \\\\xED\\\\xA0\\\\x80..\\\\xED\\\\xBF\\\\xBF are irregular UTF-8 byte sequences for single "
             "surrogate code points. The error code *U_INVALID_CHAR_FOUND* will be set.")
      .value("UCNV_RESET", UCNV_RESET,
             "The callback is called with this reason when a 'reset' has occurred.\n\n  "
             "Callback should reset all state.")
      .value("UCNV_CLOSE", UCNV_CLOSE,
             "Called when the converter is closed.\n\n  "
             "The callback should release any allocated memory.")
      .value("UCNV_CLONE", UCNV_CLONE,
             "Called when *ucnv_safe_clone()* is called on the converter.\n\n  "
             "the pointer available as the 'context' is an alias to the original converters' context pointer. If the "
             "context must be owned by the new converter, the callback must clone the data and call "
             "*ucnv_set_from_ucall_back* (or *set_to_ucall_back*) with the correct pointer.")
      .export_values();

  py::class_<_UConverterFromUCallbackPtr>(m, "UConverterFromUCallbackPtr")
      .def(py::init<py::function>(), py::arg("action"));

  py::class_<_UConverterPtr>(m, "_UConverterPtr");

  py::class_<_UConverterToUCallbackPtr>(m, "UConverterToUCallbackPtr").def(py::init<py::function>(), py::arg("action"));

  py::class_<UConverterFromUnicodeArgs>(m, "UConverterFromUnicodeArgs")
      .def_property_readonly("converter",
                             [](const UConverterFromUnicodeArgs &self) { return _UConverterPtr(self.converter); });

  py::class_<UConverterToUnicodeArgs>(m, "UConverterToUnicodeArgs")
      .def_property_readonly("converter",
                             [](const UConverterToUnicodeArgs &self) { return _UConverterPtr(self.converter); });

  m.attr("UCNV_FROM_U_CALLBACK_ESCAPE") = _UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_ESCAPE);
  m.attr("UCNV_FROM_U_CALLBACK_SKIP") = _UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_SKIP);
  m.attr("UCNV_FROM_U_CALLBACK_STOP") = _UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_STOP);
  m.attr("UCNV_FROM_U_CALLBACK_SUBSTITUTE") = _UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_SUBSTITUTE);

  m.attr("UCNV_TO_U_CALLBACK_ESCAPE") = _UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_ESCAPE);
  m.attr("UCNV_TO_U_CALLBACK_SKIP") = _UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_SKIP);
  m.attr("UCNV_TO_U_CALLBACK_STOP") = _UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_STOP);
  m.attr("UCNV_TO_U_CALLBACK_SUBSTITUTE") = _UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_SUBSTITUTE);

  m.attr("UCNV_ESCAPE_C") = UCNV_ESCAPE_C;
  m.attr("UCNV_ESCAPE_CSS2") = UCNV_ESCAPE_CSS2;
  m.attr("UCNV_ESCAPE_ICU") = UCNV_ESCAPE_ICU;
  m.attr("UCNV_ESCAPE_JAVA") = UCNV_ESCAPE_JAVA;
  m.attr("UCNV_ESCAPE_UNICODE") = UCNV_ESCAPE_UNICODE;
  m.attr("UCNV_ESCAPE_XML_DEC") = UCNV_ESCAPE_XML_DEC;
  m.attr("UCNV_ESCAPE_XML_HEX") = UCNV_ESCAPE_XML_HEX;

  m.attr("UCNV_SKIP_STOP_ON_ILLEGAL") = UCNV_SKIP_STOP_ON_ILLEGAL;

  m.attr("UCNV_SUB_STOP_ON_ILLEGAL") = UCNV_SUB_STOP_ON_ILLEGAL;
}

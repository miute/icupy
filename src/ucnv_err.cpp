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
  py::enum_<UConverterCallbackReason>(m, "UConverterCallbackReason", py::arithmetic())
      .value("UCNV_UNASSIGNED", UCNV_UNASSIGNED)
      .value("UCNV_ILLEGAL", UCNV_ILLEGAL)
      .value("UCNV_IRREGULAR", UCNV_IRREGULAR)
      .value("UCNV_RESET", UCNV_RESET)
      .value("UCNV_CLOSE", UCNV_CLOSE)
      .value("UCNV_CLONE", UCNV_CLONE)
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

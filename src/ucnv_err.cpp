#include "ucnv_err.hpp"
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace icupy {

std::u16string pystr_to_u16string(const py::str &str) {
  auto b = PyUnicode_AsEncodedString(str.ptr(), "utf-16le", "surrogatepass");
  if (!b) {
    throw py::error_already_set();
  }

  char *buffer;
  Py_ssize_t length;
  if (PyBytes_AsStringAndSize(b, &buffer, &length) == -1) {
    Py_DECREF(b);
    throw py::error_already_set();
  }

  std::u16string result(reinterpret_cast<const char16_t *>(buffer),
                        static_cast<size_t>(length / sizeof(char16_t)));
  Py_DECREF(b);
  return result;
}

py::str u16string_to_pystr(const std::u16string &str) {
  if (str.empty()) {
    return py::str("");
  }

  auto p = PyUnicode_DecodeUTF16(
      reinterpret_cast<const char *>(str.data()),
      static_cast<Py_ssize_t>(str.size() * sizeof(char16_t)), "surrogatepass",
      nullptr);
  if (!p) {
    throw py::error_already_set();
  }
  return py::reinterpret_steal<py::str>(p);
}

//
// struct UConverter
//
UConverterPtr::UConverterPtr(UConverter *p) : p_(p) {}

UConverterPtr::~UConverterPtr() {}

UConverter *UConverterPtr::get() const { return p_; }

//
// UConverterFromUCallback
//
void UConverterFromUCallbackPtr::callback(const void *native_context,
                                          UConverterFromUnicodeArgs *args,
                                          const UChar *code_units,
                                          int32_t length, UChar32 code_point,
                                          UConverterCallbackReason reason,
                                          UErrorCode *error_code) {
  if (native_context == nullptr) {
    throw std::runtime_error("UConverterFromUCallback: context is not set");
  }
  auto pair = reinterpret_cast<FromUCallbackAndContextPair *>(
      const_cast<void *>(native_context));
  auto &variant = pair->first;
  if (!std::holds_alternative<FromUCallbackFunction>(variant)) {
    throw std::runtime_error(
        "UConverterFromUCallback: callback function is not set or invalid");
  }
  auto &action = std::get<FromUCallbackFunction>(variant);
  const ConstVoidPtr *context = nullptr;
  if (pair->second) {
    context = reinterpret_cast<const ConstVoidPtr *>(pair->second);
  }
  auto object = context ? context->value() : py::none();
  auto py_code_units = u16string_to_pystr(std::u16string(code_units, length));
  auto status = icu::ErrorCode();
  status.set(*error_code);
  action(object, args, py_code_units, length, code_point, reason, &status);
  *error_code = status.get();
}

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_ESCAPE
//
class UConverterFromUCallbackEscapePtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackEscapePtr(const ConstVoidPtr *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_ESCAPE, context) {}
};

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SKIP
//
class UConverterFromUCallbackSkipPtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackSkipPtr(const ConstVoidPtr *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_SKIP, context) {}
};

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_STOP
//
class UConverterFromUCallbackStopPtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackStopPtr(const ConstVoidPtr *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_STOP, context) {}
};

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SUBSTITUTE
//
class UConverterFromUCallbackSubstitutePtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackSubstitutePtr(const ConstVoidPtr *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_SUBSTITUTE, context) {}
};

//
// UConverterToUCallback
//
void UConverterToUCallbackPtr::callback(const void *native_context,
                                        UConverterToUnicodeArgs *args,
                                        const char *code_units, int32_t length,
                                        UConverterCallbackReason reason,
                                        UErrorCode *error_code) {
  if (native_context == nullptr) {
    throw std::runtime_error("UConverterToUCallback: context is not set");
  }
  auto pair = reinterpret_cast<ToUCallbackAndContextPair *>(
      const_cast<void *>(native_context));
  auto &variant = pair->first;
  if (!std::holds_alternative<ToUCallbackFunction>(variant)) {
    throw std::runtime_error(
        "UConverterToUCallback: callback function is not set or invalid");
  }
  auto &action = std::get<ToUCallbackFunction>(variant);
  const ConstVoidPtr *context = nullptr;
  if (pair->second) {
    context = reinterpret_cast<const ConstVoidPtr *>(pair->second);
  }
  auto object = context ? context->value() : py::none();
  auto status = icu::ErrorCode();
  status.set(*error_code);
  action(object, args, py::bytes(code_units, length), length, reason, &status);
  *error_code = status.get();
}

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_ESCAPE
//
class UConverterToUCallbackEscapePtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackEscapePtr(const ConstVoidPtr *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_ESCAPE, context) {}
};

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_SKIP
//
class UConverterToUCallbackSkipPtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackSkipPtr(const ConstVoidPtr *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_SKIP, context) {}
};

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_STOP
//
class UConverterToUCallbackStopPtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackStopPtr(const ConstVoidPtr *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_STOP, context) {}
};

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_SUBSTITUTE
//
class UConverterToUCallbackSubstitutePtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackSubstitutePtr(const ConstVoidPtr *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_SUBSTITUTE, context) {}
};

} // namespace icupy

void init_ucnv_err(py::module &m) {
  //
  // enum UConverterCallbackReason
  //
  py::enum_<UConverterCallbackReason>(
      m, "UConverterCallbackReason", py::arithmetic(),
      "The process condition code to be used with the callbacks.\n\n"
      "Codes which are greater than *UCNV_IRREGULAR* should be passed on to "
      "any chained callbacks.")
      .value("UCNV_UNASSIGNED", UCNV_UNASSIGNED,
             "The code point is unassigned.\n\n  "
             "The error code *U_INVALID_CHAR_FOUND* will be set.")
      .value("UCNV_ILLEGAL", UCNV_ILLEGAL,
             "The code point is illegal.\n\n  "
             "For example, \\\\x81\\\\x2E is illegal in SJIS because \\\\x2E "
             "is not a valid trail byte for the \\\\x81 "
             "lead byte. Also, starting with Unicode 3.0.1, non-shortest byte "
             "sequences in UTF-8 (like \\\\xC1\\\\xA1 "
             "instead of \\\\x61 for U+0061) are also illegal, not just "
             "irregular. The error code "
             "*U_ILLEGAL_CHAR_FOUND* will be set.")
      .value("UCNV_IRREGULAR", UCNV_IRREGULAR,
             "The codepoint is not a regular sequence in the encoding.\n\n  "
             "For example, \\\\xED\\\\xA0\\\\x80..\\\\xED\\\\xBF\\\\xBF are "
             "irregular UTF-8 byte sequences for single "
             "surrogate code points. The error code *U_INVALID_CHAR_FOUND* "
             "will be set.")
      .value("UCNV_RESET", UCNV_RESET,
             "The callback is called with this reason when a 'reset' has "
             "occurred.\n\n  "
             "Callback should reset all state.")
      .value("UCNV_CLOSE", UCNV_CLOSE,
             "Called when the converter is closed.\n\n  "
             "The callback should release any allocated memory.")
      .value("UCNV_CLONE", UCNV_CLONE,
             "Called when *ucnv_safe_clone()* is called on the converter.\n\n  "
             "the pointer available as the 'context' is an alias to the "
             "original converters' context pointer. If the "
             "context must be owned by the new converter, the callback must "
             "clone the data and call "
             "*ucnv_set_from_ucall_back* (or *set_to_ucall_back*) with the "
             "correct pointer.")
      .export_values();

  //
  // struct UConverter
  //
  py::class_<icupy::UConverterPtr>(m, "UConverter", R"doc(
      UConverter structure.

      See Also:
          :func:`ucnv_close`
          :func:`ucnv_open_ccsid`
          :func:`ucnv_open_package`
          :func:`ucnv_open`
      )doc");

  //
  // struct UConverterFromUnicodeArgs
  //
  py::class_<UConverterFromUnicodeArgs>(m, "UConverterFromUnicodeArgs")
      .def_property_readonly("converter",
                             [](const UConverterFromUnicodeArgs &self) {
                               return icupy::UConverterPtr(self.converter);
                             });

  //
  // struct UConverterToUnicodeArgs
  //
  py::class_<UConverterToUnicodeArgs>(m, "UConverterToUnicodeArgs")
      .def_property_readonly("converter",
                             [](const UConverterToUnicodeArgs &self) {
                               return icupy::UConverterPtr(self.converter);
                             });

  //
  // UConverterFromUCallback
  //
  py::class_<icupy::UConverterFromUCallbackPtr> fucb(
      m, "UConverterFromUCallback", R"doc(
      Wrapper class for error callback function in the unicode to codepage
      direction.

      See Also:
          :class:`UConverterFromUCallbackEscape`
          :class:`UConverterFromUCallbackSkip`
          :class:`UConverterFromUCallbackStop`
          :class:`UConverterFromUCallbackSubstitute`
          :func:`ucnv_get_from_ucall_back`
          :func:`ucnv_set_from_ucall_back`
      )doc");

  fucb.def(py::init([](const icupy::FromUCallbackFunction &action,
                       std::optional<const icupy::ConstVoidPtr *> &context) {
             return std::make_unique<icupy::UConverterFromUCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterFromUCallback`` instance with a callback
      function `action` and the user context `context`.

      `action` and `context` must outlive the ``UConverterFromUCallback``
      object.
      )doc");

  fucb.def(
      "context",
      [](const icupy::UConverterFromUCallbackPtr &self)
          -> std::optional<const icupy::ConstVoidPtr *> {
        auto pair = self.context();
        if (pair == nullptr || pair->second == nullptr ||
            !self.is_cpp_function()) {
          return std::nullopt;
        }
        return reinterpret_cast<const icupy::ConstVoidPtr *>(pair->second);
      },
      py::return_value_policy::reference,
      R"doc(
      Get the user context.
      )doc");

  fucb.def(
      "option",
      [](const icupy::UConverterFromUCallbackPtr &self)
          -> std::optional<const char *> { return self.option(); },
      py::return_value_policy::reference,
      R"doc(
      Get the predefined error callback option.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_ESCAPE
  //
  py::class_<icupy::UConverterFromUCallbackEscapePtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_esc(m, "UConverterFromUCallbackEscape", R"doc(
      Wrapper class for UCNV_FROM_U_CALLBACK_ESCAPE callback function.

      Substitute the ILLEGAL SEQUENCE with the hexadecimal representation of
      the illegal codepoints.

      See Also:
          :class:`UConverterFromUCallbackSkip`
          :class:`UConverterFromUCallbackStop`
          :class:`UConverterFromUCallbackSubstitute`
          :func:`ucnv_set_from_ucall_back`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
          ...     context = icu.ConstVoidPtr(icu.UCNV_ESCAPE_XML_HEX)
          ...     action = icu.UConverterFromUCallbackEscape(context)
          ...     old_action = icu.ucnv_set_from_ucall_back(cnv, action)
          ...     s = icu.UnicodeString("A€B")
          ...     s.extract(cnv)
          ...
          b'A&#x20AC;B'
      )doc");

  fucb_esc.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackEscapePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterFromUCallbackEscape`` instance with
      :func:`UCNV_FROM_U_CALLBACK_ESCAPE` callback function and the user
      context `context`.

      `context` can be
      :attr:`UCNV_ESCAPE_ICU`,
      :attr:`UCNV_ESCAPE_JAVA`,
      :attr:`UCNV_ESCAPE_C`,
      :attr:`UCNV_ESCAPE_XML_DEC`,
      :attr:`UCNV_ESCAPE_XML_HEX`,
      or ``None``.

      `context` must outlive the ``UConverterFromUCallbackEscape`` object.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SKIP
  //
  py::class_<icupy::UConverterFromUCallbackSkipPtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_skip(m, "UConverterFromUCallbackSkip", R"doc(
      Wrapper class for UCNV_FROM_U_CALLBACK_SKIP callback function.

      Skip any ILLEGAL SEQUENCE, or skip only UNASSIGNED SEQUENCE.

      See Also:
          :class:`UConverterFromUCallbackEscape`
          :class:`UConverterFromUCallbackStop`
          :class:`UConverterFromUCallbackSubstitute`
          :func:`ucnv_set_from_ucall_back`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
          ...     action = icu.UConverterFromUCallbackSkip()
          ...     old_action = icu.ucnv_set_from_ucall_back(cnv, action)
          ...     s = icu.UnicodeString("A€B")
          ...     s.extract(cnv)
          ...
          b'AB'
      )doc");

  fucb_skip.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackSkipPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterFromUCallbackSkip`` instance with
      :func:`UCNV_FROM_U_CALLBACK_SKIP` callback function and the user
      context `context`.

      `context` can be
      :attr:`UCNV_SKIP_STOP_ON_ILLEGAL`
      or ``None``.

      `context` must outlive the ``UConverterFromUCallbackSkip``
      object.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_STOP
  //
  py::class_<icupy::UConverterFromUCallbackStopPtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_stop(m, "UConverterFromUCallbackStop", R"doc(
      Wrapper class for UCNV_FROM_U_CALLBACK_STOP callback function.

      Stop at the ILLEGAL SEQUENCE.

      See Also:
          :class:`UConverterFromUCallbackEscape`
          :class:`UConverterFromUCallbackSkip`
          :class:`UConverterFromUCallbackSubstitute`
          :func:`ucnv_set_from_ucall_back`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
          ...     action = icu.UConverterFromUCallbackStop()
          ...     old_action = icu.ucnv_set_from_ucall_back(cnv, action)
          ...     s = icu.UnicodeString("A€B")
          ...     s.extract(cnv)
          ...
          icupy.icu.ICUError: U_INVALID_CHAR_FOUND
      )doc");

  fucb_stop.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackStopPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterFromUCallbackStop`` instance with
      :func:`UCNV_FROM_U_CALLBACK_STOP` callback function and the user
      context `context`.

      `context` must outlive the ``UConverterFromUCallbackStop``
      object.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SUBSTITUTE
  //
  py::class_<icupy::UConverterFromUCallbackSubstitutePtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_sub(m, "UConverterFromUCallbackSubstitute", R"doc(
      Wrapper class for UCNV_FROM_U_CALLBACK_SUBSTITUTE callback function.

      Substitute the ILLEGAL SEQUENCE, or UNASSIGNED SEQUENCE with the current
      substitution string for the converter.

      See Also:
          :class:`UConverterFromUCallbackEscape`
          :class:`UConverterFromUCallbackSkip`
          :class:`UConverterFromUCallbackStop`
          :func:`ucnv_get_subst_chars`
          :func:`ucnv_set_from_ucall_back`
          :func:`ucnv_set_subst_chars`
          :func:`ucnv_set_subst_string`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
          ...     icu.ucnv_set_subst_string(cnv, "?")
          ...     action = icu.UConverterFromUCallbackSubstitute()
          ...     old_action = icu.ucnv_set_from_ucall_back(cnv, action)
          ...     s = icu.UnicodeString("A€B")
          ...     s.extract(cnv)
          ...
          b'A?B'
      )doc");

  fucb_sub.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackSubstitutePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterFromUCallbackSubstitute`` instance
      with :func:`UCNV_FROM_U_CALLBACK_SUBSTITUTE` callback function and
      the user context `context`.

      `context` can be
      :attr:`UCNV_SUB_STOP_ON_ILLEGAL`
      or ``None``.

      `context` must outlive the ``UConverterFromUCallbackSubstitute``
      object.
      )doc");

  //
  // UConverterToUCallback
  //
  py::class_<icupy::UConverterToUCallbackPtr> tucb(m, "UConverterToUCallback",
                                                   R"doc(
      Wrapper class for error callback function in the codepage to unicode
      direction.

      See Also:
          :class:`UConverterToUCallbackEscape`
          :class:`UConverterToUCallbackSkip`
          :class:`UConverterToUCallbackStop`
          :class:`UConverterToUCallbackSubstitute`
          :func:`ucnv_get_to_ucall_back`
          :func:`ucnv_set_to_ucall_back`
      )doc");

  tucb.def(py::init([](const icupy::ToUCallbackFunction &action,
                       std::optional<const icupy::ConstVoidPtr *> &context) {
             return std::make_unique<icupy::UConverterToUCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterToUCallback`` instance with a callback
      function `action` and the user context `context`.

      `action` and `context` must outlive the ``UConverterToUCallback``
      object.
      )doc");

  tucb.def(
      "context",
      [](const icupy::UConverterToUCallbackPtr &self)
          -> std::optional<const icupy::ConstVoidPtr *> {
        auto pair = self.context();
        if (pair == nullptr || pair->second == nullptr ||
            !self.is_cpp_function()) {
          return std::nullopt;
        }
        return reinterpret_cast<const icupy::ConstVoidPtr *>(pair->second);
      },
      py::return_value_policy::reference,
      R"doc(
      Get the user context.
      )doc");

  tucb.def(
      "option",
      [](const icupy::UConverterToUCallbackPtr &self)
          -> std::optional<const char *> { return self.option(); },
      py::return_value_policy::reference,
      R"doc(
      Get the predefined error callback option.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_ESCAPE
  //
  py::class_<icupy::UConverterToUCallbackEscapePtr,
             icupy::UConverterToUCallbackPtr>
      tucb_esc(m, "UConverterToUCallbackEscape",
               R"doc(
      Wrapper class for UCNV_TO_U_CALLBACK_ESCAPE callback function.

      Substitute the ILLEGAL SEQUENCE with the hexadecimal representation of
      the illegal bytes.

      See Also:
          :class:`UConverterToUCallbackSkip`
          :class:`UConverterToUCallbackStop`
          :class:`UConverterToUCallbackSubstitute`
          :func:`ucnv_set_to_ucall_back`
      )doc");

  tucb_esc.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackEscapePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterToUCallbackEscape`` instance with
      :func:`UCNV_TO_U_CALLBACK_ESCAPE` callback function and the user
      context `context`.

      `context` can be
      :attr:`UCNV_ESCAPE_ICU`,
      :attr:`UCNV_ESCAPE_JAVA`,
      :attr:`UCNV_ESCAPE_C`,
      :attr:`UCNV_ESCAPE_XML_DEC`,
      :attr:`UCNV_ESCAPE_XML_HEX`,
      :attr:`UCNV_ESCAPE_UNICODE`,
      or ``None``.

      `context` must outlive the ``UConverterToUCallbackEscape``
      object.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_SKIP
  //
  py::class_<icupy::UConverterToUCallbackSkipPtr,
             icupy::UConverterToUCallbackPtr>
      tucb_skip(m, "UConverterToUCallbackSkip", R"doc(
      Wrapper class for UCNV_TO_U_CALLBACK_SKIP callback function.

      Skip any ILLEGAL SEQUENCE, or skip only UNASSIGNED SEQUENCE.

      See Also:
          :class:`UConverterToUCallbackEscape`
          :class:`UConverterToUCallbackStop`
          :class:`UConverterToUCallbackSubstitute`
          :func:`ucnv_set_to_ucall_back`
      )doc");

  tucb_skip.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackSkipPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterToUCallbackSkip`` instance with
      :func:`UCNV_TO_U_CALLBACK_SKIP` callback function and the user
      context `context`.

      `context` can be
      :attr:`UCNV_SKIP_STOP_ON_ILLEGAL`
      or ``None``.

      `context` must outlive the ``UConverterToUCallbackSkip``
      object.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_STOP
  //
  py::class_<icupy::UConverterToUCallbackStopPtr,
             icupy::UConverterToUCallbackPtr>
      tucb_stop(m, "UConverterToUCallbackStop", R"doc(
      Wrapper class for UCNV_TO_U_CALLBACK_STOP callback function.

      Stop at the ILLEGAL SEQUENCE.

      See Also:
          :class:`UConverterToUCallbackEscape`
          :class:`UConverterToUCallbackSkip`
          :class:`UConverterToUCallbackSubstitute`
          :func:`ucnv_set_to_ucall_back`
      )doc");

  tucb_stop.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackStopPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterToUCallbackStop`` instance with
      :func:`UCNV_TO_U_CALLBACK_STOP` callback function and the user
      context `context`.

      `context` must outlive the ``UConverterToUCallbackStop``
      object.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_SUBSTITUTE
  //
  py::class_<icupy::UConverterToUCallbackSubstitutePtr,
             icupy::UConverterToUCallbackPtr>
      tucb_sub(m, "UConverterToUCallbackSubstitute", R"doc(
      Wrapper class for UCNV_TO_U_CALLBACK_SUBSTITUTE callback function.

      Substitute the ILLEGAL SEQUENCE, or UNASSIGNED SEQUENCE with the Unicode
      substitution character, U+FFFD.

      See Also:
          :class:`UConverterToUCallbackEscape`
          :class:`UConverterToUCallbackSkip`
          :class:`UConverterToUCallbackStop`
          :func:`ucnv_set_to_ucall_back`
      )doc");

  tucb_sub.def(
      py::init([](std::optional<const icupy::ConstVoidPtr *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackSubstitutePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize the ``UConverterToUCallbackSubstitute`` instance
      with :func:`UCNV_TO_U_CALLBACK_SUBSTITUTE` callback function and
      the user context `context`.

      `context` can be
      :attr:`UCNV_SUB_STOP_ON_ILLEGAL`
      or ``None``.

      `context` must outlive the ``UConverterToUCallbackSubstitute``
      object.
      )doc");

  //
  // Functions
  //
  m.def(
      "UCNV_FROM_U_CALLBACK_ESCAPE",
      [](py::object &context, UConverterFromUnicodeArgs *args,
         const py::str &code_units, int32_t length, UChar32 code_point,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = icupy::pystr_to_u16string(code_units);
        UCNV_FROM_U_CALLBACK_ESCAPE(options.size() ? options.data() : nullptr,
                                    args, native_code_units.data(),
                                    native_code_units.size(), code_point,
                                    reason, error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("code_point"), py::arg("reason"),
      py::arg("error_code"), R"doc(
      Substitute the ILLEGAL SEQUENCE with the hexadecimal representation of
      the illegal codepoints.

      This function has overhead;
      use :class:`UConverterFromUCallbackEscape` instead.

      See Also:
          :class:`UConverterFromUCallback`
      )doc");

  m.def(
      "UCNV_FROM_U_CALLBACK_SKIP",
      [](py::object &context, UConverterFromUnicodeArgs *args,
         const py::str &code_units, int32_t length, UChar32 code_point,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = icupy::pystr_to_u16string(code_units);
        UCNV_FROM_U_CALLBACK_SKIP(options.size() ? options.data() : nullptr,
                                  args, native_code_units.data(),
                                  native_code_units.size(), code_point, reason,
                                  error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("code_point"), py::arg("reason"),
      py::arg("error_code"), R"doc(
      Skip any ILLEGAL SEQUENCE, or skip only UNASSIGNED SEQUENCE.

      This function has overhead;
      use :class:`UConverterFromUCallbackSkip` instead.

      See Also:
          :class:`UConverterFromUCallback`
      )doc");

  m.def(
      "UCNV_FROM_U_CALLBACK_STOP",
      [](py::object &context, UConverterFromUnicodeArgs *args,
         const py::str &code_units, int32_t length, UChar32 code_point,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = icupy::pystr_to_u16string(code_units);
        UCNV_FROM_U_CALLBACK_STOP(options.size() ? options.data() : nullptr,
                                  args, native_code_units.data(),
                                  native_code_units.size(), code_point, reason,
                                  error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("code_point"), py::arg("reason"),
      py::arg("error_code"), R"doc(
      Stop at the ILLEGAL SEQUENCE.

      This function has overhead;
      use :class:`UConverterFromUCallbackStop` instead.

      See Also:
          :class:`UConverterFromUCallback`
      )doc");

  m.def(
      "UCNV_FROM_U_CALLBACK_SUBSTITUTE",
      [](py::object &context, UConverterFromUnicodeArgs *args,
         const py::str &code_units, int32_t length, UChar32 code_point,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = icupy::pystr_to_u16string(code_units);
        UCNV_FROM_U_CALLBACK_SUBSTITUTE(
            options.size() ? options.data() : nullptr, args,
            native_code_units.data(), native_code_units.size(), code_point,
            reason, error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("code_point"), py::arg("reason"),
      py::arg("error_code"), R"doc(
      Substitute the ILLEGAL SEQUENCE, or UNASSIGNED SEQUENCE with the current
      substitution string for the converter.

      This function has overhead;
      use :class:`UConverterFromUCallbackSubstitute` instead.

      See Also:
          :class:`UConverterFromUCallback`
          :func:`ucnv_get_subst_chars`
          :func:`ucnv_set_subst_chars`
          :func:`ucnv_set_subst_string`
      )doc");

  m.def(
      "UCNV_TO_U_CALLBACK_ESCAPE",
      [](py::object &context, UConverterToUnicodeArgs *args,
         const py::bytes &code_units, int32_t length,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = code_units.cast<std::string>();
        UCNV_TO_U_CALLBACK_ESCAPE(options.size() ? options.data() : nullptr,
                                  args, native_code_units.data(),
                                  native_code_units.size(), reason, error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("reason"), py::arg("error_code"), R"doc(
      Substitute the ILLEGAL SEQUENCE with the hexadecimal representation of
      the illegal bytes.

      This function has overhead;
      use :class:`UConverterToUCallbackEscape` instead.

      See Also:
          :class:`UConverterToUCallback`
      )doc");

  m.def(
      "UCNV_TO_U_CALLBACK_SKIP",
      [](py::object &context, UConverterToUnicodeArgs *args,
         const py::bytes &code_units, int32_t length,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = code_units.cast<std::string>();
        UCNV_TO_U_CALLBACK_SKIP(options.size() ? options.data() : nullptr, args,
                                native_code_units.data(),
                                native_code_units.size(), reason, error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("reason"), py::arg("error_code"), R"doc(
      Skip any ILLEGAL SEQUENCE, or skip only UNASSIGNED SEQUENCE.

      This function has overhead;
      use :class:`UConverterToUCallbackSkip` instead.

      See Also:
          :class:`UConverterToUCallback`
      )doc");

  m.def(
      "UCNV_TO_U_CALLBACK_STOP",
      [](py::object &context, UConverterToUnicodeArgs *args,
         const py::bytes &code_units, int32_t length,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = code_units.cast<std::string>();
        UCNV_TO_U_CALLBACK_STOP(options.size() ? options.data() : nullptr, args,
                                native_code_units.data(),
                                native_code_units.size(), reason, error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("reason"), py::arg("error_code"), R"doc(
      Stop at the ILLEGAL SEQUENCE.

      This function has overhead;
      use :class:`UConverterToUCallbackStop` instead.

      See Also:
          :class:`UConverterToUCallback`
      )doc");

  m.def(
      "UCNV_TO_U_CALLBACK_SUBSTITUTE",
      [](py::object &context, UConverterToUnicodeArgs *args,
         const py::bytes &code_units, int32_t length,
         UConverterCallbackReason reason, icu::ErrorCode &error_code) {
        std::string options;
        if (py::isinstance<py::str>(context)) {
          options = context.cast<std::string>();
        }
        auto native_code_units = code_units.cast<std::string>();
        UCNV_TO_U_CALLBACK_SUBSTITUTE(options.size() ? options.data() : nullptr,
                                      args, native_code_units.data(),
                                      native_code_units.size(), reason,
                                      error_code);
      },
      py::arg("context"), py::arg("args").none(false), py::arg("code_units"),
      py::arg("length"), py::arg("reason"), py::arg("error_code"), R"doc(
      Substitute the ILLEGAL SEQUENCE, or UNASSIGNED SEQUENCE with the Unicode
      substitution character, U+FFFD.

      This function has overhead;
      use :class:`UConverterToUCallbackSubstitute` instead.

      See Also:
          :class:`UConverterToUCallback`
      )doc");

  //
  // Attributes
  //
  m.attr("UCNV_ESCAPE_C") = UCNV_ESCAPE_C;
  m.attr("UCNV_ESCAPE_CSS2") = UCNV_ESCAPE_CSS2;
  m.attr("UCNV_ESCAPE_ICU") = py::none(); // UCNV_ESCAPE_ICU;
  m.attr("UCNV_ESCAPE_JAVA") = UCNV_ESCAPE_JAVA;
  m.attr("UCNV_ESCAPE_UNICODE") = UCNV_ESCAPE_UNICODE;
  m.attr("UCNV_ESCAPE_XML_DEC") = UCNV_ESCAPE_XML_DEC;
  m.attr("UCNV_ESCAPE_XML_HEX") = UCNV_ESCAPE_XML_HEX;
  m.attr("UCNV_SKIP_STOP_ON_ILLEGAL") = UCNV_SKIP_STOP_ON_ILLEGAL;
  m.attr("UCNV_SUB_STOP_ON_ILLEGAL") = UCNV_SUB_STOP_ON_ILLEGAL;
}

#include "ucnv_err.hpp"
#include <pybind11/functional.h>
#include <pybind11/native_enum.h>
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
  const UserContext *context = nullptr;
  if (pair->second) {
    context = reinterpret_cast<const UserContext *>(pair->second);
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
  UConverterFromUCallbackEscapePtr(const UserContext *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_ESCAPE, context) {}
};

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SKIP
//
class UConverterFromUCallbackSkipPtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackSkipPtr(const UserContext *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_SKIP, context) {}
};

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_STOP
//
class UConverterFromUCallbackStopPtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackStopPtr(const UserContext *context)
      : UConverterFromUCallbackPtr(UCNV_FROM_U_CALLBACK_STOP, context) {}
};

//
// UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SUBSTITUTE
//
class UConverterFromUCallbackSubstitutePtr : public UConverterFromUCallbackPtr {
public:
  UConverterFromUCallbackSubstitutePtr(const UserContext *context)
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
  const UserContext *context = nullptr;
  if (pair->second) {
    context = reinterpret_cast<const UserContext *>(pair->second);
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
  UConverterToUCallbackEscapePtr(const UserContext *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_ESCAPE, context) {}
};

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_SKIP
//
class UConverterToUCallbackSkipPtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackSkipPtr(const UserContext *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_SKIP, context) {}
};

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_STOP
//
class UConverterToUCallbackStopPtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackStopPtr(const UserContext *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_STOP, context) {}
};

//
// UConverterToUCallback: UCNV_TO_U_CALLBACK_SUBSTITUTE
//
class UConverterToUCallbackSubstitutePtr : public UConverterToUCallbackPtr {
public:
  UConverterToUCallbackSubstitutePtr(const UserContext *context)
      : UConverterToUCallbackPtr(UCNV_TO_U_CALLBACK_SUBSTITUTE, context) {}
};

} // namespace icupy

void init_ucnv_err(py::module &m) {
  //
  // enum UConverterCallbackReason
  //
  py::native_enum<UConverterCallbackReason>(m, "UConverterCallbackReason",
                                            "enum.IntEnum", R"doc(
Process condition code to be used with the callbacks.

Codes which are greater than :attr:`UCNV_IRREGULAR` should be passed on to
any chained callbacks.
      )doc")
      .value("UCNV_UNASSIGNED", UCNV_UNASSIGNED, R"doc(
             The code point is unassigned.

             The error code :attr:`~UErrorCode.U_INVALID_CHAR_FOUND`
             will be set.
             )doc")
      .value("UCNV_ILLEGAL", UCNV_ILLEGAL, R"doc(
             The code point is illegal.

             For example, \\x81\\x2E is illegal in SJIS because \\x2E
             is not a valid trail byte for the \\x81
             lead byte. Also, starting with Unicode 3.0.1, non-shortest byte
             sequences in UTF-8 (like \\xC1\\xA1 instead of \\x61 for U+0061)
             are also illegal, not just irregular.
             The error code :attr:`~UErrorCode.U_ILLEGAL_CHAR_FOUND`
             will be set.
             )doc")
      .value("UCNV_IRREGULAR", UCNV_IRREGULAR, R"doc(
             The codepoint is not a regular sequence in the encoding.

             For example, \\xED\\xA0\\x80..\\xED\\xBF\\xBF are irregular UTF-8
             byte sequences for single surrogate code points.
             The error code :attr:`~UErrorCode.U_INVALID_CHAR_FOUND`
             will be set.
             )doc")
      .value("UCNV_RESET", UCNV_RESET, R"doc(
             The callback is called with this reason when a 'reset' has
             occurred.

             Callback should reset all state.
             )doc")
      .value("UCNV_CLOSE", UCNV_CLOSE, R"doc(
             Called when the converter is closed.

             The callback should release any allocated memory.
             )doc")
      .value("UCNV_CLONE", UCNV_CLONE, R"doc(
             Called when :func:`ucnv_safe_clone` is called on the converter.
             )doc")
      .export_values()
      .finalize();

  //
  // struct UConverter
  //
  py::class_<icupy::UConverterPtr>(m, "UConverter", R"doc(
      UConverter structure.

      .. seealso::

         :func:`ucnv_close`
         :func:`ucnv_open`
         :func:`ucnv_open_ccsid`
         :func:`ucnv_open_package`
      )doc");

  //
  // struct UConverterFromUnicodeArgs
  //
  py::class_<UConverterFromUnicodeArgs> fua(m, "UConverterFromUnicodeArgs",
                                            R"doc(
Structure for the from-Unicode callback function parameter.

.. seealso::

   :class:`UConverterFromUCallback`
      )doc");

  fua.def_property_readonly(
      "converter",
      [](const UConverterFromUnicodeArgs &self) {
        return icupy::UConverterPtr(self.converter);
      },
      R"doc(
      :class:`UConverter` that is opened and to which this struct is passed as
      an argument.
      )doc");

  //
  // struct UConverterToUnicodeArgs
  //
  py::class_<UConverterToUnicodeArgs> fta(m, "UConverterToUnicodeArgs",
                                          R"doc(
Structure for the to-Unicode callback function parameter.

.. seealso::

   :class:`UConverterToUCallback`
      )doc");

  fta.def_property_readonly(
      "converter",
      [](const UConverterToUnicodeArgs &self) {
        return icupy::UConverterPtr(self.converter);
      },
      R"doc(
      :class:`UConverter` that is opened and to which this struct is passed as
      an argument.
      )doc");

  //
  // UConverterFromUCallback
  //
  py::class_<icupy::UConverterFromUCallbackPtr> fucb(
      m, "UConverterFromUCallback", R"doc(
      Wrapper class for the error callback function used to convert from
      Unicode to a codepage.

      The specified callback function is called if an error occurs when
      converting from Unicode to a codepage, or when opening/closing the
      callback.

      .. seealso::

         :class:`UConverterFromUCallbackEscape`
         :class:`UConverterFromUCallbackSkip`
         :class:`UConverterFromUCallbackStop`
         :class:`UConverterFromUCallbackSubstitute`
         :func:`ucnv_get_from_u_call_back`
         :func:`ucnv_set_from_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> def from_unicode_cb(
         ...     options: object,
         ...     args: icu.UConverterFromUnicodeArgs,
         ...     code_units: str,
         ...     length: int,
         ...     code_point: int,
         ...     reason: icu.UConverterCallbackReason,
         ...     error_code: icu.ErrorCode,
         ... ) -> None:
         ...     _ = options, length, code_point  # unused
         ...     if reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
         ...         error_code.reset()
         ...         source = "".join(f"\\u{ord(c):04x}" for c in code_units)
         ...         icu.ucnv_cb_from_u_write_bytes(args, source, len(source), 0)

         >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterFromUCallback(from_unicode_cb)
         ...     old_action = icu.ucnv_set_from_u_call_back(cnv, action)
         ...     s = icu.UnicodeString("A€B")
         ...     s.extract(cnv)
         ...
         b'A\\u20acB'
      )doc");

  fucb.def(py::init([](const icupy::FromUCallbackFunction &action,
                       std::optional<const icupy::UserContext *> &context) {
             return std::make_unique<icupy::UConverterFromUCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterFromUCallback`` instance with the specified
      callback function and the user context.

      *action* is the from-Unicode callback function that is called when the
      converter detects an illegal or invalid Unicode sequence.

      *context* is the callback's context data.

      .. important::

         *action* and *context* must outlive this callback container object.

      .. note::

         The callback function must have the following signature:

         .. code-block:: python

            def action(
                options: object,
                args: icu.UConverterFromUnicodeArgs,
                code_units: str,
                length: int,
                code_point: int,
                reason: icu.UConverterCallbackReason,
                error_code: icu.ErrorCode,
            ) -> None:
                ...

         *options* is the callback's context data.

         *args* is the Information about the conversion in progress.

         *code_units* is the concerned Unicode sequence.

         *length* is the length of *code_units* in bytes.

         *code_point* is the concerned Unicode codepoint.

         *reason* is the reason that the callback was invoked.

         *error_code* is the ICU error code.
      )doc");

  fucb.def(
      "context",
      [](const icupy::UConverterFromUCallbackPtr &self)
          -> std::optional<const icupy::UserContext *> {
        auto pair = self.context();
        if (pair == nullptr || pair->second == nullptr ||
            !self.is_cpp_function()) {
          return std::nullopt;
        }
        return reinterpret_cast<const icupy::UserContext *>(pair->second);
      },
      py::return_value_policy::reference,
      R"doc(
      Return the user context.
      )doc");

  fucb.def(
      "option",
      [](const icupy::UConverterFromUCallbackPtr &self)
          -> std::optional<const char *> { return self.option(); },
      py::return_value_policy::reference,
      R"doc(
      Return the predefined error callback option.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_ESCAPE
  //
  py::class_<icupy::UConverterFromUCallbackEscapePtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_esc(m, "UConverterFromUCallbackEscape", R"doc(
      Wrapper class for :func:`UCNV_FROM_U_CALLBACK_ESCAPE` callback function.

      This from-Unicode callback will substitute the ILLEGAL SEQUENCE with the
      hexadecimal representation of the illegal codepoints.

      .. seealso::

         :class:`UConverterFromUCallbackSkip`
         :class:`UConverterFromUCallbackStop`
         :class:`UConverterFromUCallbackSubstitute`
         :func:`ucnv_set_from_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
         ...     context = icu.UserContext(icu.UCNV_ESCAPE_XML_HEX)
         ...     action = icu.UConverterFromUCallbackEscape(context)
         ...     old_action = icu.ucnv_set_from_u_call_back(cnv, action)
         ...     s = icu.UnicodeString("A€B")
         ...     s.extract(cnv)
         ...
         b'A&#x20AC;B'
      )doc");

  fucb_esc.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackEscapePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterFromUCallbackEscape`` instance with
      :func:`UCNV_FROM_U_CALLBACK_ESCAPE` callback function and the specified
      user context.

      *context* is a callback option with the following values:

      - :attr:`UCNV_ESCAPE_ICU` or ``None``: substitutes the ILLEGAL SEQUENCE
        with the hexadecimal representation in the format %UXXXX,
        e.g. "%UFFFE\\xac%UC8FE".
      - :attr:`UCNV_ESCAPE_JAVA`: substitutes the ILLEGAL SEQUENCE with the
        hexadecimal representation in the format \\uXXXX,
        e.g. "\\\\uFFFE\\xac\\\\uC8FE".
      - :attr:`UCNV_ESCAPE_C`: substitutes the ILLEGAL SEQUENCE with the
        hexadecimal representation in the format \\uXXXX,
        e.g. "\\\\uFFFE\\xac\\\\uC8FE".
      - :attr:`UCNV_ESCAPE_XML_DEC`: substitutes the ILLEGAL SEQUENCE with the
        decimal representation in the format &#DDDDD;,
        e.g. "&#65534;\\xac;&#51454;".
      - :attr:`UCNV_ESCAPE_XML_HEX`: substitutes the ILLEGAL SEQUENCE with the
        decimal representation in the format &#xXXXX;
        e.g. "&#xFFFE;\\xac;&#xC8FE;".

      .. important::

         *context* must outlive this callback container object.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SKIP
  //
  py::class_<icupy::UConverterFromUCallbackSkipPtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_skip(m, "UConverterFromUCallbackSkip", R"doc(
      Wrapper class for :func:`UCNV_FROM_U_CALLBACK_SKIP` callback function.

      This from-Unicode callback will skip any ILLEGAL SEQUENCE, or skip only
      UNASSIGNED SEQUENCE.

      .. seealso::

         :class:`UConverterFromUCallbackEscape`
         :class:`UConverterFromUCallbackStop`
         :class:`UConverterFromUCallbackSubstitute`
         :func:`ucnv_set_from_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterFromUCallbackSkip()
         ...     old_action = icu.ucnv_set_from_u_call_back(cnv, action)
         ...     s = icu.UnicodeString("A€B")
         ...     s.extract(cnv)
         ...
         b'AB'
      )doc");

  fucb_skip.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackSkipPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterFromUCallbackSkip`` instance with
      :func:`UCNV_FROM_U_CALLBACK_SKIP` callback function and the specified
      user context.

      *context* is a callback option with the following values:

      - :attr:`UCNV_SKIP_STOP_ON_ILLEGAL`: stops at ILLEGAL SEQUENCE and throws
        an :class:`ICUError` exception.
      - ``None``: skips any ILLEGAL SEQUENCE.

      .. important::

         *context* must outlive this callback container object.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_STOP
  //
  py::class_<icupy::UConverterFromUCallbackStopPtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_stop(m, "UConverterFromUCallbackStop", R"doc(
      Wrapper class for :func:`UCNV_FROM_U_CALLBACK_STOP` callback function.

      This from-Unicode callback will stop at the ILLEGAL SEQUENCE.

      .. seealso::

         :class:`UConverterFromUCallbackEscape`
         :class:`UConverterFromUCallbackSkip`
         :class:`UConverterFromUCallbackSubstitute`
         :func:`ucnv_set_from_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterFromUCallbackStop()
         ...     old_action = icu.ucnv_set_from_u_call_back(cnv, action)
         ...     s = icu.UnicodeString("A€B")
         ...     s.extract(cnv)
         ...
         icupy.icu.ICUError: U_INVALID_CHAR_FOUND
      )doc");

  fucb_stop.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackStopPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterFromUCallbackStop`` instance with
      :func:`UCNV_FROM_U_CALLBACK_STOP` callback function and the specified
      user context.

      This from-Unicode callback will stop at the ILLEGAL SEQUENCE and throw an
      :class:`ICUError` exception.

      *context* is reserved; specify ``None``.
      )doc");

  //
  // UConverterFromUCallback: UCNV_FROM_U_CALLBACK_SUBSTITUTE
  //
  py::class_<icupy::UConverterFromUCallbackSubstitutePtr,
             icupy::UConverterFromUCallbackPtr>
      fucb_sub(m, "UConverterFromUCallbackSubstitute", R"doc(
      Wrapper class for :func:`UCNV_FROM_U_CALLBACK_SUBSTITUTE` callback
      function.

      This from-Unicode callback will substitute the ILLEGAL SEQUENCE, or
      UNASSIGNED SEQUENCE with the current substitution string for the
      converter.

      .. seealso::

         :class:`UConverterFromUCallbackEscape`
         :class:`UConverterFromUCallbackSkip`
         :class:`UConverterFromUCallbackStop`
         :func:`ucnv_get_subst_chars`
         :func:`ucnv_set_from_u_call_back`
         :func:`ucnv_set_subst_chars`
         :func:`ucnv_set_subst_string`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
         ...     icu.ucnv_set_subst_string(cnv, "?")
         ...     action = icu.UConverterFromUCallbackSubstitute()
         ...     old_action = icu.ucnv_set_from_u_call_back(cnv, action)
         ...     s = icu.UnicodeString("A€B")
         ...     s.extract(cnv)
         ...
         b'A?B'
      )doc");

  fucb_sub.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterFromUCallbackSubstitutePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterFromUCallbackSubstitute`` instance
      with :func:`UCNV_FROM_U_CALLBACK_SUBSTITUTE` callback function and
      the specified user context.

      *context* is a callback option with the following values:

      - :attr:`UCNV_SUB_STOP_ON_ILLEGAL`: stops at ILLEGAL SEQUENCE and throws
        an :class:`ICUError` exception.
      - ``None``: substitutes any ILLEGAL SEQUENCE.

      .. important::

         *context* must outlive this callback container object.
      )doc");

  //
  // UConverterToUCallback
  //
  py::class_<icupy::UConverterToUCallbackPtr> tucb(m, "UConverterToUCallback",
                                                   R"doc(
      Wrapper class for the error callback function used to convert from a
      codepage to Unicode.

      The specified callback function is called if an error occurs when
      converting from a codepage to Unicode, or when opening/closing the
      callback.

      .. seealso::

         :class:`UConverterToUCallbackEscape`
         :class:`UConverterToUCallbackSkip`
         :class:`UConverterToUCallbackStop`
         :class:`UConverterToUCallbackSubstitute`
         :func:`ucnv_get_to_u_call_back`
         :func:`ucnv_set_to_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> def to_unicode_cb(
         ...     options: object,
         ...     args: icu.UConverterToUnicodeArgs,
         ...     code_units: bytes,
         ...     length: int,
         ...     reason: icu.UConverterCallbackReason,
         ...     error_code: icu.ErrorCode,
         ... ) -> None:
         ...     _ = options, length  # unused
         ...     if reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
         ...         error_code.reset()
         ...         source = "".join(f"%{b:02X}" for b in code_units)
         ...         icu.ucnv_cb_to_u_write_uchars(args, source, len(source), 0)

         >>> with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterToUCallback(to_unicode_cb)
         ...     old_action = icu.ucnv_set_to_u_call_back(cnv, action)
         ...     src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UCNV_UNASSIGNED
         ...     s = icu.UnicodeString(src, -1, cnv)
         ...     str(s)
         ...
         'a%EB%40b'
      )doc");

  tucb.def(py::init([](const icupy::ToUCallbackFunction &action,
                       std::optional<const icupy::UserContext *> &context) {
             return std::make_unique<icupy::UConverterToUCallbackPtr>(
                 action, context.value_or(nullptr));
           }),
           py::arg("action"), py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterToUCallback`` instance with the specified
      callback function and the user context.

      *action* is the to-Unicode callback function that is called when the
      converter detects an illegal or invalid sequence.

      *context* is the callback's context data.

      .. important::

         *action* and *context* must outlive this callback container object.

      .. note::

         The callback function must have the following signature:

         .. code-block:: python

            def action(
                options: object,
                args: icu.UConverterToUnicodeArgs,
                code_units: bytes,
                length: int,
                reason: icu.UConverterCallbackReason,
                error_code: icu.ErrorCode,
            ) -> None:
                ...

         *options* is the callback's context data.

         *args* is the Information about the conversion in progress.

         *code_units* is the concerned codepage sequence.

         *length* is the length of *code_units* in bytes.

         *reason* is the reason that the callback was invoked.

         *error_code* is the ICU error code.
      )doc");

  tucb.def(
      "context",
      [](const icupy::UConverterToUCallbackPtr &self)
          -> std::optional<const icupy::UserContext *> {
        auto pair = self.context();
        if (pair == nullptr || pair->second == nullptr ||
            !self.is_cpp_function()) {
          return std::nullopt;
        }
        return reinterpret_cast<const icupy::UserContext *>(pair->second);
      },
      py::return_value_policy::reference,
      R"doc(
      Return the user context.
      )doc");

  tucb.def(
      "option",
      [](const icupy::UConverterToUCallbackPtr &self)
          -> std::optional<const char *> { return self.option(); },
      py::return_value_policy::reference,
      R"doc(
      Return the predefined error callback option.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_ESCAPE
  //
  py::class_<icupy::UConverterToUCallbackEscapePtr,
             icupy::UConverterToUCallbackPtr>
      tucb_esc(m, "UConverterToUCallbackEscape",
               R"doc(
      Wrapper class for :func:`UCNV_TO_U_CALLBACK_ESCAPE` callback function.

      This to-Unicode callback will substitute the ILLEGAL SEQUENCE with the
      hexadecimal representation of the illegal bytes.

      .. seealso::

         :class:`UConverterToUCallbackSkip`
         :class:`UConverterToUCallbackStop`
         :class:`UConverterToUCallbackSubstitute`
         :func:`ucnv_set_to_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
         ...     context = icu.UserContext(icu.UCNV_ESCAPE_XML_HEX)
         ...     action = icu.UConverterToUCallbackEscape(context)
         ...     old_action = icu.ucnv_set_to_u_call_back(cnv, action)
         ...     src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UNASSIGNED SEQUENCE
         ...     s = icu.UnicodeString(src, -1, cnv)
         ...     str(s)
         ...
         'a&#xEB;&#x40;b'
      )doc");

  tucb_esc.def(py::init([](std::optional<const icupy::UserContext *> &context) {
                 return std::make_unique<icupy::UConverterToUCallbackEscapePtr>(
                     context.value_or(nullptr));
               }),
               py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterToUCallbackEscape`` instance with
      :func:`UCNV_TO_U_CALLBACK_ESCAPE` callback function and the specified
      user context.

      *context* is a callback option with the following values:
      :attr:`UCNV_ESCAPE_ICU` | ``None``,
      :attr:`UCNV_ESCAPE_JAVA`,
      :attr:`UCNV_ESCAPE_C`,
      :attr:`UCNV_ESCAPE_XML_DEC`,
      :attr:`UCNV_ESCAPE_XML_HEX`, and
      :attr:`UCNV_ESCAPE_UNICODE`.

      .. important::

         *context* must outlive this callback container object.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_SKIP
  //
  py::class_<icupy::UConverterToUCallbackSkipPtr,
             icupy::UConverterToUCallbackPtr>
      tucb_skip(m, "UConverterToUCallbackSkip", R"doc(
      Wrapper class for :func:`UCNV_TO_U_CALLBACK_SKIP` callback function.

      This to-Unicode callback will skip any ILLEGAL SEQUENCE, or skip only
      UNASSIGNED SEQUENCE.

      .. seealso::

         :class:`UConverterToUCallbackEscape`
         :class:`UConverterToUCallbackStop`
         :class:`UConverterToUCallbackSubstitute`
         :func:`ucnv_set_to_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterToUCallbackSkip()
         ...     old_action = icu.ucnv_set_to_u_call_back(cnv, action)
         ...     src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UNASSIGNED SEQUENCE
         ...     s = icu.UnicodeString(src, -1, cnv)
         ...     str(s)
         ...
         'ab'
      )doc");

  tucb_skip.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackSkipPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterToUCallbackSkip`` instance with
      :func:`UCNV_TO_U_CALLBACK_SKIP` callback function and the specified user
      context.

      *context* is a callback option with the following values:

      - :attr:`UCNV_SKIP_STOP_ON_ILLEGAL`: stops at ILLEGAL SEQUENCE and throws
        an :class:`ICUError` exception.
      - ``None``: skips any ILLEGAL SEQUENCE.

      .. important::

         *context* must outlive this callback container object.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_STOP
  //
  py::class_<icupy::UConverterToUCallbackStopPtr,
             icupy::UConverterToUCallbackPtr>
      tucb_stop(m, "UConverterToUCallbackStop", R"doc(
      Wrapper class for :func:`UCNV_TO_U_CALLBACK_STOP` callback function.

      This to-Unicode callback will stop at the ILLEGAL SEQUENCE.

      .. seealso::

         :class:`UConverterToUCallbackEscape`
         :class:`UConverterToUCallbackSkip`
         :class:`UConverterToUCallbackSubstitute`
         :func:`ucnv_set_to_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterToUCallbackStop()
         ...     old_action = icu.ucnv_set_to_u_call_back(cnv, action)
         ...     src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UNASSIGNED SEQUENCE
         ...     s = icu.UnicodeString(src, -1, cnv)
         ...
         icupy.icu.ICUError: U_INVALID_CHAR_FOUND
      )doc");

  tucb_stop.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackStopPtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterToUCallbackStop`` instance with
      :func:`UCNV_TO_U_CALLBACK_STOP` callback function and the specified user
      context.

      This to-Unicode callback will stop at the ILLEGAL SEQUENCE and throw an
      :class:`ICUError` exception.

      *context* is reserved; specify ``None``.
      )doc");

  //
  // UConverterToUCallback: UCNV_TO_U_CALLBACK_SUBSTITUTE
  //
  py::class_<icupy::UConverterToUCallbackSubstitutePtr,
             icupy::UConverterToUCallbackPtr>
      tucb_sub(m, "UConverterToUCallbackSubstitute", R"doc(
      Wrapper class for :func:`UCNV_TO_U_CALLBACK_SUBSTITUTE` callback
      function.

      This to-Unicode callback will substitute the ILLEGAL SEQUENCE, or
      UNASSIGNED SEQUENCE with the Unicode substitution character, U+FFFD.

      .. seealso::

         :class:`UConverterToUCallbackEscape`
         :class:`UConverterToUCallbackSkip`
         :class:`UConverterToUCallbackStop`
         :func:`ucnv_set_to_u_call_back`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
         ...     action = icu.UConverterToUCallbackSubstitute()
         ...     old_action = icu.ucnv_set_to_u_call_back(cnv, action)
         ...     src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UNASSIGNED SEQUENCE
         ...     s = icu.UnicodeString(src, -1, cnv)
         ...     str(s)
         ...
         'a�b'  # "a\uFFFDb"
      )doc");

  tucb_sub.def(
      py::init([](std::optional<const icupy::UserContext *> &context) {
        return std::make_unique<icupy::UConverterToUCallbackSubstitutePtr>(
            context.value_or(nullptr));
      }),
      py::arg("context") = std::nullopt, R"doc(
      Initialize a ``UConverterToUCallbackSubstitute`` instance
      with :func:`UCNV_TO_U_CALLBACK_SUBSTITUTE` callback function and
      the specified user context.

      *context* is a callback option with the following values:

      - :attr:`UCNV_SUB_STOP_ON_ILLEGAL`: stops at ILLEGAL SEQUENCE and throws
        an :class:`ICUError` exception.
      - ``None``: substitutes any ILLEGAL SEQUENCE.

      .. important::

         *context* must outlive this callback container object.
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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterFromUCallbackEscape` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterFromUCallbackSkip` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterFromUCallbackStop` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterFromUCallbackSubstitute` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterToUCallbackEscape` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterToUCallbackSkip` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterToUCallbackStop` instead.

      .. seealso::

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

      .. note::

         Do not call this function directly.

      .. note::

         The Python implementation of this function incurs overhead.
         For improved performance, consider using
         :class:`UConverterToUCallbackSubstitute` instead.

      .. seealso::

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

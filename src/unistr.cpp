#include "main.hpp"
#include "ucnv_err.hpp"
#include <iomanip>
#include <memory>
#include <optional>
#include <pybind11/native_enum.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/brkiter.h>
#include <unicode/uchriter.h>

using namespace icu;

void init_unistr(py::module &m, py::class_<Replaceable, UObject> &rep,
                 py::class_<UnicodeString, Replaceable> &us) {
  //
  // class icu::Replaceable
  //
  rep.def("__copy__", &Replaceable::clone, R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  rep.def(
      "__deepcopy__",
      [](const Replaceable &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"), R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  rep.def("__len__", &Replaceable::length, R"doc(
      Return the number of 16-bit code units in this instance.

      This is equivalent to calling :meth:`.length`.
      )doc");

  rep.def("char32_at", &Replaceable::char32At, py::arg("offset"), R"doc(
      Return the 32-bit code point at the specified *offset*.

      If *offset* points to the leading or trailing code unit of a surrogate
      pair, return the code point of the surrogate pair.
      )doc");

  rep.def(
      "char_at",
      [](const Replaceable &self, int32_t offset) -> uint16_t {
        return self.charAt(offset);
      },
      py::arg("offset"), R"doc(
      Return the 16-bit code unit at the specified *offset*.
      )doc");

  rep.def("clone", &Replaceable::clone, R"doc(
      Return a copy of this instance.

      .. seealso::

         :meth:`.__copy__`
         :meth:`.__deepcopy__`
      )doc");

  rep.def("copy", &Replaceable::copy, py::arg("start"), py::arg("limit"),
          py::arg("dest"), R"doc(
      Copy the substring within the range [*start*, *limit*) of this
      ``Replaceable`` to *dest*.

      *dest* must satisfy either *dest* <= *start* or *dest* >= *limit*.
      )doc");

  rep.def("extract_between", &Replaceable::extractBetween, py::arg("start"),
          py::arg("limit"), py::arg("target"), R"doc(
      Copy the substring within the range [*start*, *limit*) of this
      ``Replaceable`` to *target*.
      )doc");

  rep.def(
      "handle_replace_between",
      [](Replaceable &self, int32_t start, int32_t limit,
         const icupy::UnicodeStringVariant &text) {
        self.handleReplaceBetween(start, limit, icupy::to_unistr(text));
      },
      py::arg("start"), py::arg("limit"), py::arg("text"), R"doc(
      Replace the substring within the range [*start*, *limit*) of this
      ``Replaceable`` with *text*.
      )doc");

  rep.def("length", &Replaceable::length, R"doc(
      Return the number of 16-bit code units in this ``Replaceable``.

      .. seealso::

         :meth:`.__len__`
      )doc");

  //
  // enum icu::UnicodeString::EInvariant
  //
  py::native_enum<UnicodeString::EInvariant>(us, "EInvariant", "enum.IntEnum",
                                             R"doc(
Constant to be used in the ``UnicodeString(src, text_length, inv)``
constructor which constructs a Unicode string from an invariant-character
string.
      )doc")
      .value("INVARIANT", UnicodeString::kInvariant, R"doc(
             All characters in a string are invariant characters.

             .. note::

                Use the :attr:`US_INV` instead of the full qualification for
                this value.
      )doc")
      .export_values()
      .finalize();

  //
  // class icu::UnicodeString
  //
  us.def(
        // [1] UnicodeString()
        py::init<>(), R"doc(
      Initialize a ``UnicodeString`` instance as an empty string.
      )doc")
      .def(
          // [2] UnicodeString(int32_t capacity, UChar32 c, int32_t count)
          py::init<int32_t, UChar32, int32_t>(), py::arg("capacity"),
          py::arg("c"), py::arg("count"), R"doc(
      Initialize a ``UnicodeString`` instance with a string consisting of the
      code unit *c* repeated *count* times.

      *capacity* is the number of char16_ts that this ``UnicodeString`` can
      hold before resizing is required. If *count* is greater than 0 and
      *count* code points *c* occupy more space than *capacity*, *capacity* is
      adjusted accordingly.
      )doc")
      .def(
          // [4] UnicodeString(UChar32 ch)
          py::init<UChar32>(), py::arg("ch"), R"doc(
      Initialize a ``UnicodeString`` instance with the single code point *ch*.
      )doc")
      .def(
          // [10] UnicodeString(const S &text)
          //      UnicodeString(const char16_t *text)
          py::init([](const std::u16string &text) {
#if (U_ICU_VERSION_MAJOR_NUM >= 76)
            return std::make_unique<UnicodeString>(text);
#else
            return std::make_unique<UnicodeString>(text.data());
#endif
          }),
          py::arg("text"), R"doc(
      Initialize a ``UnicodeString`` instance with the string literal *text*.
      )doc")
      .def(
          // [6] UnicodeString(const char16_t *text, int32_t textLength)
          py::init([](const std::u16string &text, int32_t text_length) {
            return std::make_unique<UnicodeString>(text.data(), text_length);
          }),
          py::arg("text"), py::arg("text_length"), R"doc(
      Initialize a ``UnicodeString`` instance with the substring
      *text[:text_length]* of *text*.
      )doc")
      .def(
          // [16] UnicodeString(const char *codepageData)
          py::init([](const py::bytes &codepage_data) {
            const auto codepage_data_text =
                PyBytes_AS_STRING(codepage_data.ptr());
            return std::make_unique<UnicodeString>(codepage_data_text);
          }),
          py::arg("codepage_data"), R"doc(
      Initialize a ``UnicodeString`` instance from NUL-terminated
      codepage data *codepage_data* using the default converter.
      )doc")
      .def(
          // [18] UnicodeString(const char *codepageData, const char *codepage)
          py::init([](const py::bytes &codepage_data,
                      const std::optional<std::string> &codepage) {
            const auto codepage_data_text =
                PyBytes_AS_STRING(codepage_data.ptr());
            return std::make_unique<UnicodeString>(
                codepage_data_text, codepage ? codepage->data() : nullptr);
          }),
          py::arg("codepage_data"), py::arg("codepage"), R"doc(
      Initialize a ``UnicodeString`` instance from NUL-terminated
      codepage data *codepage_data* using the converter specified by
      *codepage*.

      If *codepage* is ``None``, the default converter is used. If *codepage*
      is an empty string (""), *codepage_data* must be an
      "invariant characters".

      .. note::

         For invariant-character strings use
         ``UnicodeString(src, text_length, inv)`` instead.
      )doc")
      .def(
          // [17] UnicodeString(const char *codepageData, int32_t dataLength)
          py::init([](const py::bytes &codepage_data, int32_t data_length) {
            const auto codepage_data_text = codepage_data.cast<std::string>();
            if (data_length == -1) {
              data_length = static_cast<int32_t>(codepage_data_text.size());
            }
            return std::make_unique<UnicodeString>(codepage_data_text.data(),
                                                   data_length);
          }),
          py::arg("codepage_data"), py::arg("data_length"), R"doc(
      Initialize a ``UnicodeString`` instance from the codepage data
      *codepage_data* within the range [0, *data_length*) using the default
      converter.
      )doc")
      .def(
          // [19] UnicodeString(const char *codepageData,
          //                    int32_t data_length,
          //                    const char *codepage)
          py::init([](const py::bytes &codepage_data, int32_t data_length,
                      const std::optional<std::string> &codepage) {
            const auto codepage_data_text = codepage_data.cast<std::string>();
            if (data_length == -1) {
              data_length = static_cast<int32_t>(codepage_data_text.size());
            }
            return std::make_unique<UnicodeString>(
                codepage_data_text.data(), data_length,
                codepage ? codepage->data() : nullptr);
          }),
          py::arg("codepage_data"), py::arg("data_length"), py::arg("codepage"),
          R"doc(
      Initialize a ``UnicodeString`` instance from the codepage data
      *codepage_data* within the range [0, *data_length*) using the converter
      specified by *codepage*.

      If *codepage* is ``None``, the default converter is used. If *codepage*
      is an empty string (""), *codepage_data* must be an
      "invariant characters".

      .. note::

         For invariant-character strings use
         ``UnicodeString(src, text_length, inv)`` instead.
      )doc")
      .def(
          // [20] UnicodeString(const char *src,
          //                    int32_t srcLength,
          //                    UConverter *cnv,
          //                    UErrorCode &errorCode)
          py::init([](const py::bytes &src, int32_t src_length,
                      icupy::UConverterPtr &cnv) {
            const auto src_text = src.cast<std::string>();
            if (src_length == -1) {
              src_length = static_cast<int32_t>(src_text.size());
            }
            ErrorCode error_code;
            auto result = std::make_unique<UnicodeString>(
                src_text.data(), src_length, cnv, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("src"), py::arg("src_length"), py::arg("cnv"), R"doc(
      Initialize a ``UnicodeString`` instance from the codepage data *src*
      within the range [0, *src_length*) using the existing converter *cnv*.

      .. seealso::

         :func:`ucnv_set_to_u_call_back`
      )doc")
      .def(
          // [21] UnicodeString(const char *src,
          //                    int32_t textLength,
          //                    enum EInvariant inv)
          py::init([](const std::string &src, int32_t text_length,
                      UnicodeString::EInvariant inv) {
            return std::make_unique<UnicodeString>(src.data(), text_length,
                                                   inv);
          }),
          py::arg("src"), py::arg("text_length"), py::arg("inv"), R"doc(
      Initialize a ``UnicodeString`` instance with the substring
      *src[:text_length]* of *src* as an invariant-character string.

      .. note::

         For *inv*, use :attr:`US_INV` instead of the fully qualified name.
      )doc")
      .def(
          // [22] UnicodeString(const UnicodeString &that)
          py::init<const UnicodeString &>(), py::arg("other"), R"doc(
      Initialize a ``UnicodeString`` instance from another ``UnicodeString``.
      )doc")
      .def(
          // [24] UnicodeString(const UnicodeString &src,
          //                    int32_t srcStart)
          py::init<const UnicodeString &, int32_t>(), py::arg("src"),
          py::arg("src_start"), R"doc(
      Initialize a ``UnicodeString`` instance from the substring
      *src[src_start:]* of an existing ``UnicodeString`` object.
      )doc")
      .def(
          // [25] UnicodeString(const UnicodeString &src,
          //                    int32_t srcStart,
          //                    int32_t srcLength)
          py::init<const UnicodeString &, int32_t, int32_t>(), py::arg("src"),
          py::arg("src_start"), py::arg("src_length"), R"doc(
      Initialize a ``UnicodeString`` instance from the substring
      *src[src_start:src_start + src_length]* of an existing
      ``UnicodeString`` object.
      )doc");

  us.def(
        "__add__",
        [](UnicodeString &self, const icupy::UnicodeStringVariant &other) {
          return self + icupy::to_unistr(other);
        },
        py::is_operator(), py::arg("other"), R"doc(
      Concatenate this ``UnicodeString`` with the string *other* and return the
      result as a new ``UnicodeString`` instance.
      )doc")
      .def(
          "__add__",
          [](UnicodeString &self, UChar32 other) { return self + other; },
          py::is_operator(), py::arg("other"), R"doc(
      Concatenate this ``UnicodeString`` with the code point *other* and return
      the result as a new ``UnicodeString`` instance.
      )doc");

  us.def(
      "__contains__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &item) {
        return self.indexOf(icupy::to_unistr(item)) >= 0;
      },
      py::arg("item"), R"doc(
      Return ``True`` if *item* is a substring of this instance, ``False``
      otherwise.

      This is equivalent to :meth:`index_of` >= 0.
      )doc");

  us.def("__copy__", &UnicodeString::clone, R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  us.def(
      "__deepcopy__",
      [](const UnicodeString &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"), R"doc(
      Return a deep copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  us.def(
      "__eq__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) {
        return self == icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.

      .. note::

         Only bitwise comparisons are performed.
      )doc");

  us.def(
      "__ge__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other)
          -> bool { return self >= icupy::to_unistr(other); },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* >= *other*.

      .. note::

         Only bitwise comparisons are performed.
      )doc");

  us.def(
        "__getitem__",
        [](const UnicodeString &self, int32_t index) {
          auto actual_index = index;
          const auto length = self.length();
          if (actual_index < 0) {
            actual_index += length;
          }
          if (actual_index < 0 || actual_index >= length) {
            throw py::index_error("string index out of range: " +
                                  std::to_string(index));
          }
          return py::str(PyUnicode_FromOrdinal(self[actual_index]));
        },
        py::arg("index"), R"doc(
      Return the code unit at offset *index*.
      )doc")
      .def(
          "__getitem__",
          [](const UnicodeString &self, const py::slice &slice) {
            py::ssize_t start, stop, step, slice_length;
            if (!slice.compute(self.length(), &start, &stop, &step,
                               &slice_length)) {
              throw py::error_already_set();
            }
            PyObject *result = nullptr;
            for (py::ssize_t n = 0; n < slice_length; ++n) {
              auto p = PyUnicode_FromOrdinal(self[static_cast<int32_t>(start)]);
              if (result) {
                PyUnicode_Append(&result, p);
              } else {
                result = p;
              }
              start += step;
            }
            return result ? py::str(result) : py::str();
          },
          py::arg("slice"), R"doc(
      Return a set of code units in the *range(start, stop, step)*.
      )doc");

  us.def(
      "__gt__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other)
          -> bool { return self > icupy::to_unistr(other); },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* > *other*.

      .. note::

         Only bitwise comparisons are performed.
      )doc");

  us.def("__hash__", &UnicodeString::hashCode, R"doc(
      Return a hash value of this instance.

      This is equivalent to calling :meth:`.hash_code`.
      )doc");

  us.def(
        "__iadd__",
        [](UnicodeString &self, const icupy::UnicodeStringVariant &other)
            -> UnicodeString & { return self += icupy::to_unistr(other); },
        py::is_operator(), py::arg("other"), R"doc(
      Append the string *other* to this ``UnicodeString`` and return the
      ``UnicodeString`` itself.
      )doc")
      .def(
          "__iadd__",
          [](UnicodeString &self, UChar32 other) -> UnicodeString & {
            return self += other;
          },
          py::is_operator(), py::arg("other"), R"doc(
      Append the code point *other* to this ``UnicodeString`` and return the
      ``UnicodeString`` itself.
      )doc");

  us.def(
      "__le__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other)
          -> bool { return self <= icupy::to_unistr(other); },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* <= *other*.

      .. note::

         Only bitwise comparisons are performed.
      )doc");

  us.def(
      "__lt__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other)
          -> bool { return self < icupy::to_unistr(other); },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* < *other*.

      .. note::

         Only bitwise comparisons are performed.
      )doc");

  us.def(
      "__ne__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) {
        return self != icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.

      .. note::

         Only bitwise comparisons are performed.
      )doc");

  us.def("__repr__", [](const UnicodeString &self) {
    std::stringstream ss;
    ss << "UnicodeString('";
    const auto text_length = self.length();
    auto it = UCharCharacterIterator(self.getBuffer(), text_length);
    const char *prefix;
    int w;
    for (auto c = it.first32(); it.hasNext(); c = it.next32()) {
      if (c >= 0x20 && c <= 0x7e) {
        if (c == '\'') {
          ss << "\\'";
        } else {
          ss << static_cast<char>(c);
        }
      } else {
        if (c <= 0xff) {
          prefix = "\\x";
          w = 2;
        } else if (c <= 0xffff) {
          prefix = "\\u";
          w = 4;
        } else {
          prefix = "\\U";
          w = 8;
        }
        ss << prefix << std::nouppercase << std::hex << std::setw(w)
           << std::setfill('0') << c;
      }
    }
    ss << "', text_length=" << std::dec << text_length;
    ss << ")";
    return ss.str();
  });

  us.def(
      "__setitem__",
      [](UnicodeString &self, int32_t index,
         const icupy::Char16Variant &value) {
        auto actual_index = index;
        const auto length = self.length();
        if (actual_index < 0) {
          actual_index += length;
        }
        if (actual_index < 0 || actual_index >= length) {
          throw py::index_error("string index out of range: " +
                                std::to_string(index));
        }
        self.setCharAt(actual_index, icupy::to_char16(value));
      },
      py::arg("index"), py::arg("value"), R"doc(
      Set the code unit at the specified *index* to *value*.

      This is similar to calling :meth:`.set_char_at`.
      )doc");

  us.def(
      "__str__",
      [](const UnicodeString &self) {
        std::string result;
        self.toUTF8String(result);
        return result;
      },
      R"doc(
      Return a string representation of this instance.

      This is equivalent to calling :meth:`.to_utf8_string`.
      )doc");

  us.def(
        // [2] append(const char16_t *srcChars, int32_t srcStart,
        //            int32_t srcLength)
        "append",
        [](UnicodeString &self, const std::u16string &src_chars,
           int32_t src_start, int32_t src_length) -> UnicodeString & {
          return self.append(src_chars.data(), src_start, src_length);
        },
        py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
        R"doc(
      Append the substring *src_chars[src_start:src_start + src_length]*
      of *src_chars* to this ``UnicodeString`` and return the ``UnicodeString``
      itself.
      )doc")
      .def(
          // [4] append(const UnicodeString &srcText)
          "append",
          [](UnicodeString &self,
             const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.append(icupy::to_unistr(src_text));
          },
          py::arg("src_text"), R"doc(
      Append the string *src_text* to this ``UnicodeString`` and return the
      ``UnicodeString`` itself.
      )doc")
      .def(
          // [5] append(const UnicodeString &srcText, int32_t srcStart,
          //            int32_t srcLength)
          "append",
          [](UnicodeString &self, const UnicodeString &src_text,
             int32_t src_start, int32_t src_length) -> UnicodeString & {
            return self.append(src_text, src_start, src_length);
          },
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          R"doc(
      Append the substring *src_text[src_start:src_start + src_length]* of
      *src_text* to this ``UnicodeString`` and return the ``UnicodeString``
      itself.
      )doc")
      .def(
          // [6] append(ConstChar16Ptr srcChars, int32_t srcLength)
          "append",
          [](UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length) -> UnicodeString & {
            return self.append(src_chars.data(), src_length);
          },
          py::arg("src_chars"), py::arg("src_length"), R"doc(
      Append the substring *src_chars[:src_length]* of *src_chars* to this
      ``UnicodeString`` and return the ``UnicodeString`` itself.
      )doc")
      .def(
          // [7] append(UChar32 srcChar)
          "append",
          [](UnicodeString &self, UChar32 src_char) -> UnicodeString & {
            return self.append(src_char);
          },
          py::arg("src_char"), R"doc(
      Append the code point *src_char* to this ``UnicodeString`` and return the
      ``UnicodeString`` itself.
      )doc");

  us.def(
        // [1] caseCompare(const UnicodeString &text, uint32_t options)
        "case_compare",
        [](const UnicodeString &self, const icupy::UnicodeStringVariant &text,
           uint32_t options) {
          return self.caseCompare(icupy::to_unistr(text), options);
        },
        py::arg("text"), py::arg("options"), R"doc(
      Compare this ``UnicodeString`` with the string *text* case-insensitively
      using full case folding; return a negative integer if this
      ``UnicodeString`` is less than *text*, a positive integer if it is
      greater than *text*, and 0 if they are equal.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc")
      .def(
          // [2] caseCompare(ConstChar16Ptr srcChars, int32_t srcLength,
          //                 uint32_t options)
          "case_compare",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length, uint32_t options) {
            return self.caseCompare(src_chars.data(), src_length, options);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("options"),
          R"doc(
      Compare this ``UnicodeString`` with the substring
      *src_chars[:src_length]* of *src_chars* case-insensitively using full
      case folding.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc")
      .def(
          // [3] caseCompare(int32_t start, int32_t length,
          //                 const char16_t *srcChars, int32_t srcStart,
          //                 int32_t srcLength, uint32_t options)
          "case_compare",
          [](const UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars, int32_t src_start,
             int32_t src_length, uint32_t options) {
            return self.caseCompare(start, length, src_chars.data(), src_start,
                                    src_length, options);
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"),
          py::arg("src_start"), py::arg("src_length"), py::arg("options"),
          R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars*
      case-insensitively using full case folding.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc")
      .def(
          // [4] caseCompare(int32_t start, int32_t length,
          //                 const char16_t *srcChars, uint32_t options)
          "case_compare",
          [](const UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars, uint32_t options) {
            return self.caseCompare(start, length, src_chars.data(), options);
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"),
          py::arg("options"),
          R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the string *src_chars* case-insensitively using
      full case folding.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc")
      .def(
          // [5] caseCompare(int32_t start, int32_t length,
          //                 const UnicodeString &srcText,
          //                 int32_t srcStart, int32_t srcLength,
          //                 uint32_t options)
          "case_compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t,
                            int32_t, uint32_t>(&UnicodeString::caseCompare,
                                               py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"),
          py::arg("src_start"), py::arg("src_length"), py::arg("options"),
          R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the substring
      *src_text[src_start:src_start + src_length]* of *src_text*
      case-insensitively using full case folding.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc")
      .def(
          // [6] caseCompare(int32_t start, int32_t length,
          //                 const UnicodeString &srcText, uint32_t options)
          "case_compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, uint32_t>(
              &UnicodeString::caseCompare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"),
          py::arg("options"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the string *src_text* case-insensitively using
      full case folding.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc");

  us.def(
      "case_compare_between",
      [](const UnicodeString &self, int32_t start, int32_t limit,
         const icupy::UnicodeStringVariant &src_text, int32_t src_start,
         int32_t src_limit, uint32_t options) {
        return self.caseCompareBetween(start, limit, icupy::to_unistr(src_text),
                                       src_start, src_limit, options);
      },
      py::arg("start"), py::arg("limit"), py::arg("src_text"),
      py::arg("src_start"), py::arg("src_limit"), py::arg("options"), R"doc(
      Compare the substring *self[start:limit]* of this ``UnicodeString`` with
      the substring *src_text[src_start:src_limit]* of *src_text*
      case-insensitively using full case folding; return a negative integer if
      the substring of this ``UnicodeString`` is less than the substring of
      *src_text*, a positive integer if it is greater than the substring of
      *src_text*, and 0 if they are equal.

      *options* is a bit set of the following values:

      - :attr:`U_FOLD_CASE_DEFAULT` or 0:
        Compare strings in code unit order with the default mappings defined in
        CaseFolding.txt.
      - :attr:`U_COMPARE_CODE_POINT_ORDER`:
        Compare strings in code point order instead of code unit order.
      - :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`:
        Compare strings in code unit order with the modified set of mappings
        provided in CaseFolding.txt to handle dotted I and dotless i
        appropriately for Turkic languages (tr, az).
      )doc");

  us.def("clone", &UnicodeString::clone, R"doc(
      Return a copy of this instance.

      .. seealso::

         :meth:`.__copy__`
         :meth:`.__deepcopy__`
      )doc");

  us.def(
        // [1] compare(const UnicodeString &text)
        "compare",
        [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) {
          return self.compare(icupy::to_unistr(text));
        },
        py::arg("text"), R"doc(
      Compare this ``UnicodeString`` with the string *text* bitwise; return a
      negative integer if this ``UnicodeString`` is less than *text*, a
      positive integer if it is greater than *text*, and 0 if they are equal.
      )doc")
      .def(
          // [2] compare(ConstChar16Ptr srcChars, int32_t srcLength)
          "compare",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length) {
            return self.compare(src_chars.data(), src_length);
          },
          py::arg("src_chars"), py::arg("src_length"), R"doc(
      Compare this ``UnicodeString`` with the substring
      *src_chars[:src_length]* of *src_chars* bitwise.
      )doc")
      .def(
          // [3] compare(int32_t start, int32_t length,
          //             const char16_t *srcChars)
          "compare",
          [](const UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars) {
            return self.compare(start, length, src_chars.data());
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the string *src_chars* bitwise.
      )doc")
      .def(
          // [4] compare(int32_t start, int32_t length,
          //             const char16_t *srcChars, int32_t srcStart,
          //             int32_t srcLength)
          "compare",
          [](const UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars, int32_t src_start,
             int32_t src_length) {
            return self.compare(start, length, src_chars.data(), src_start,
                                src_length);
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"),
          py::arg("src_start"), py::arg("src_length"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars* bitwise.
      )doc")
      .def(
          // [5] compare(int32_t start, int32_t length,
          //             const UnicodeString &srcText, int32_t srcStart,
          //             int32_t srcLength)
          "compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t,
                            int32_t>(&UnicodeString::compare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"),
          py::arg("src_start"), py::arg("src_length"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the substring
      *src_text[src_start:src_start + src_length]* of *src_text* bitwise.
      )doc")
      .def(
          // [6] compare(int32_t start, int32_t length,
          //             const UnicodeString &text)
          "compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &>(
              &UnicodeString::compare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("text"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the string *text* bitwise.
      )doc");

  us.def(
      "compare_between",
      [](const UnicodeString &self, int32_t start, int32_t limit,
         const icupy::UnicodeStringVariant &src_text, int32_t src_start,
         int32_t src_limit) {
        return self.compareBetween(start, limit, icupy::to_unistr(src_text),
                                   src_start, src_limit);
      },
      py::arg("start"), py::arg("limit"), py::arg("src_text"),
      py::arg("src_start"), py::arg("src_limit"), R"doc(
      Compare the substring *self[start:limit]* of this
      ``UnicodeString`` with the substring *src_text[src_start:src_limit]* of
      *src_text* bitwise; return a negative integer if the substring of this
      ``UnicodeString`` is less than the substring of *src_text*, a positive
      integer if it is greater than the substring of *src_text*, and 0 if they
      are equal.
      )doc");

  us.def(
        // [1] compareCodePointOrder(const UnicodeString &text)
        "compare_code_point_order",
        [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) {
          return self.compareCodePointOrder(icupy::to_unistr(text));
        },
        py::arg("text"), R"doc(
      Compare this ``UnicodeString`` with the string *text* in code point
      order; return a negative integer if this ``UnicodeString`` is less than
      *text*, a positive integer if it is greater than *text*, and 0 if they
      are equal.
      )doc")
      .def(
          // [2] compareCodePointOrder(ConstChar16Ptr srcChars,
          //                           int32_t srcLength)
          "compare_code_point_order",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length) {
            return self.compareCodePointOrder(src_chars.data(), src_length);
          },
          py::arg("src_chars"), py::arg("src_length"), R"doc(
      Compare this ``UnicodeString`` with the substring
      *src_chars[:src_length]* of *src_chars* in code point order.
      )doc")
      .def(
          // [3] compareCodePointOrder(int32_t start, int32_t length,
          //                           const char16_t *srcChars)
          "compare_code_point_order",
          [](const UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars) {
            return self.compareCodePointOrder(start, length, src_chars.data());
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the string *src_chars* in code point order.
      )doc")
      .def(
          // [4] compareCodePointOrder(int32_t start, int32_t length,
          //                           const char16_t *srcChars,
          //                           int32_t srcStart, int32_t srcLength)
          "compare_code_point_order",
          [](const UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars, int32_t src_start,
             int32_t src_length) {
            return self.compareCodePointOrder(start, length, src_chars.data(),
                                              src_start, src_length);
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"),
          py::arg("src_start"), py::arg("src_length"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars* in code
      point order.
      )doc")
      .def(
          // [5] compareCodePointOrder(int32_t start, int32_t length,
          //                           const UnicodeString &srcText)
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const UnicodeString &>(
              &UnicodeString::compareCodePointOrder, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the string *src_text* in code point order.
      )doc")
      .def(
          // [6] compareCodePointOrder(int32_t start, int32_t length,
          //                           const UnicodeString &srcText,
          //                           int32_t srcStart, int32_t srcLength)
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t,
                            int32_t>(&UnicodeString::compareCodePointOrder,
                                     py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"),
          py::arg("src_start"), py::arg("src_length"), R"doc(
      Compare the substring *self[start:start + length]* of this
      ``UnicodeString`` with the substring
      *src_text[src_start:src_start + src_length]* of *src_text* in code point
      order.
      )doc");

  us.def(
      "compare_code_point_order_between",
      [](const UnicodeString &self, int32_t start, int32_t limit,
         const icupy::UnicodeStringVariant &src_text, int32_t src_start,
         int32_t src_limit) {
        return self.compareCodePointOrderBetween(
            start, limit, icupy::to_unistr(src_text), src_start, src_limit);
      },
      py::arg("start"), py::arg("limit"), py::arg("src_text"),
      py::arg("src_start"), py::arg("src_limit"), R"doc(
      Compare the substring *self[start:limit]* of this
      ``UnicodeString`` with the substring *src_text[src_start:src_limit]* of
      *src_text* in code point order; return a negative integer if the
      substring of this ``UnicodeString`` is less than the substring of
      *src_text*, a positive integer if it is greater than the substring of
      *src_text*, and 0 if they are equal.
      )doc");

  us.def("count_char32", &UnicodeString::countChar32, py::arg("start ") = 0,
         py::arg("length") = INT32_MAX, R"doc(
      Return the number of code points within the substring
      *self[start:start + length]* of this ``UnicodeString``.
      )doc");

  us.def(
        // [1] endsWith(const char16_t *srcChars, int32_t srcStart,
        //              int32_t srcLength)
        "ends_with",
        [](const UnicodeString &self, const std::u16string &src_chars,
           int32_t src_start, int32_t src_length) -> py::bool_ {
          return self.endsWith(src_chars.data(), src_start, src_length);
        },
        py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
        R"doc(
      Return ``True`` if this ``UnicodeString`` ends with the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars*, ``False``
      otherwise.
      )doc")
      .def(
          // [2] endsWith(const UnicodeString &srcText, int32_t srcStart,
          //              int32_t srcLength)
          "ends_with",
          [](const UnicodeString &self, const UnicodeString &src_text,
             int32_t src_start, int32_t src_length) -> py::bool_ {
            return self.endsWith(src_text, src_start, src_length);
          },
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          R"doc(
      Return ``True`` if this ``UnicodeString`` ends with the substring
      *src_text[src_start:src_start + src_length]* of *src_text*, ``False``
      otherwise.
      )doc")
      .def(
          // [3] endsWith(const UnicodeString &text)
          "ends_with",
          [](const UnicodeString &self,
             const icupy::UnicodeStringVariant &src_text) -> py::bool_ {
            return self.endsWith(icupy::to_unistr(src_text));
          },
          py::arg("src_text"), R"doc(
      Return ``True`` if this ``UnicodeString`` ends with the string
      *src_text*, ``False`` otherwise.
      )doc")
      .def(
          // [4] endsWith(ConstChar16Ptr srcChars, int32_t srcLength)
          "ends_with",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length) -> py::bool_ {
            return self.endsWith(src_chars.data(), src_length);
          },
          py::arg("src_chars"), py::arg("src_length"), R"doc(
      Return ``True`` if this ``UnicodeString`` ends with the substring
      *src_chars[:src_length]* of *src_chars*, ``False`` otherwise.
      )doc");

  us.def(
        // [1] extract(char *dest, int32_t destCapacity, UConverter *cnv,
        //             UErrorCode &errorCode)
        "extract",
        [](const UnicodeString &self, icupy::UConverterPtr &cnv) {
          ErrorCode error_code;
          const auto length = self.extract(nullptr, 0, cnv, error_code);
          std::string dest(length, '\0');
          error_code.reset();
          self.extract(dest.data(), static_cast<int32_t>(dest.size()), cnv,
                       error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return py::bytes(dest.data(), length);
        },
        py::arg("cnv"), R"doc(
      Convert this ``UnicodeString`` to the codepage string using the existing
      converter *cnv* and return the result.

      .. seealso::

         :func:`ucnv_set_from_u_call_back`
      )doc")
      .def(
          // [2] extract(Char16Ptr dest, int32_t destCapacity,
          //             UErrorCode &errorCode)
          "extract",
          [](const UnicodeString &self) {
            std::u16string result(self.length(), u'\0');
            ErrorCode error_code;
            self.extract(result.data(), static_cast<int32_t>(result.size()),
                         error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          R"doc(
      Convert this ``UnicodeString`` to a UTF-8 string and return the result.
      )doc")
      .def(
          // [4] extract(int32_t start, int32_t length, UnicodeString &target)
          "extract",
          py::overload_cast<int32_t, int32_t, UnicodeString &>(
              &UnicodeString::extract, py::const_),
          py::arg("start"), py::arg("length"), py::arg("target"), R"doc(
      Copy the substring *self[start:start + length]* of this
      ``UnicodeString`` into *target*.
      )doc")
      .def(
          // [5] extract(int32_t start, int32_t startLength, char *target,
          //             const char *codepage = 0)
          "extract",
          [](const UnicodeString &self, int32_t start, int32_t start_length,
             const std::optional<std::string> &codepage) {
            auto codepage_data = codepage ? codepage->data() : nullptr;
            const auto length =
                self.extract(start, start_length, nullptr, codepage_data);
            if (length <= 0) {
              return py::bytes();
            }
            std::string target(length, '\0');
            self.extract(start, start_length, target.data(), codepage_data);
            return py::bytes(target.data(), length);
          },
          py::arg("start"), py::arg("start_length"), py::arg("codepage"),
          R"doc(
      Convert the substring *self[start:start + start_length]* of this
      ``UnicodeString`` to the codepage string specified by *codepage* and
      return the result.

      If *codepage* is ``None``, the default converter is used. If *codepage*
      is an empty string (""), all characters must be an
      "invariant characters".

      .. note::

         For invariant-character strings use
         ``extract(start, start_length, inv)`` instead.
      )doc")
      .def(
          // [6] extract(int32_t start, int32_t startLength, char *target,
          //             int32_t targetCapacity,
          //             enum EInvariant inv)
          "extract",
          [](const UnicodeString &self, int32_t start, int32_t start_length,
             UnicodeString::EInvariant inv) {
            const auto length =
                self.extract(start, start_length, nullptr, 0, inv);
            if (length <= 0) {
              return py::bytes();
            }
            std::string target(length, '\0');
            self.extract(start, start_length, target.data(),
                         static_cast<int32_t>(target.size()), inv);
            return py::bytes(target.data(), length);
          },
          py::arg("start"), py::arg("start_length"), py::arg("inv"), R"doc(
      Convert the substring *self[start:start + start_length]* of this
      ``UnicodeString`` to an invariant-character string using a simple
      converter and return the result.

      .. note::

         All characters must be an "invariant characters".

      .. note::

         For *inv*, use :attr:`US_INV` instead of the fully qualified name.
      )doc")
      .def(
          // [7] extract(int32_t start, int32_t startLength, char *target,
          //             uint32_t targetLength)
          "extract",
          [](const UnicodeString &self, int32_t start, int32_t start_length) {
            const auto length =
                self.extract(start, start_length, nullptr, uint32_t{0});
            if (length <= 0) {
              return py::bytes();
            }
            std::string target(length, '\0');
            self.extract(start, start_length, target.data(),
                         static_cast<uint32_t>(target.size()));
            return py::bytes(target.data(), length);
          },
          py::arg("start"), py::arg("start_length"), R"doc(
      Convert the substring *self[start:start + start_length]* of this
      ``UnicodeString`` to the platform's default codepage and return
      the result.
      )doc");

  us.def(
        // [1] extractBetween(int32_t start, int32_t limit, char16_t *dst,
        //                    int32_t dstStart = 0)
        "extract_between",
        [](const UnicodeString &self, int32_t start, int32_t limit) {
          std::u16string result(
              std::max(0, std::min(self.length(), std::max(0, limit)) -
                              std::max(0, start)),
              u'\0');
          self.extractBetween(start, limit, result.data());
          return result;
        },
        py::arg("start"), py::arg("limit"), R"doc(
      Convert the substring *self[start:limit]* of this ``UnicodeString`` to a
      UTF-8 string and return it.
      )doc")
      .def(
          // [2] extractBetween(int32_t start, int32_t limit,
          //                    UnicodeString &target)
          "extract_between",
          py::overload_cast<int32_t, int32_t, UnicodeString &>(
              &UnicodeString::extractBetween, py::const_),
          py::arg("start"), py::arg("limit"), py::arg("target"), R"doc(
      Copy the substring *self[start:limit]* of this ``UnicodeString`` into
      *target*.
      )doc");

  us.def("fast_copy_from", &UnicodeString::fastCopyFrom, py::arg("src"), R"doc(
      Replace the contents of this ``UnicodeString`` with the contents of
      *src* and return the ``UnicodeString`` itself.
      )doc");

  us.def(
        // [1] findAndReplace(const UnicodeString &oldText,
        //                    const UnicodeString &newText)
        "find_and_replace",
        [](UnicodeString &self, const icupy::UnicodeStringVariant &old_text,
           const icupy::UnicodeStringVariant &new_text) -> UnicodeString & {
          return self.findAndReplace(icupy::to_unistr(old_text),
                                     icupy::to_unistr(new_text));
        },
        py::arg("old_text"), py::arg("new_text"), R"doc(
      Replace all occurrences of the string *old_text* within this
      ``UnicodeString`` with *new_text*, and return the ``UnicodeString``
      itself.
      )doc")
      .def(
          // [2] findAndReplace(int32_t start, int32_t length,
          //                    const UnicodeString &oldText,
          //                    const UnicodeString &newText)
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length,
             const icupy::UnicodeStringVariant &old_text,
             const icupy::UnicodeStringVariant &new_text) -> UnicodeString & {
            return self.findAndReplace(start, length,
                                       icupy::to_unistr(old_text),
                                       icupy::to_unistr(new_text));
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"),
          py::arg("new_text"), R"doc(
      Replace all occurrences of the string *old_text* within the substring
      *self[start:start + length]* of this ``UnicodeString`` with *new_text*,
      and return the ``UnicodeString`` itself.
      )doc")
      .def(
          // [3] findAndReplace(int32_t start, int32_t length,
          //                    const UnicodeString &oldText, int32_t oldStart,
          //                    int32_t oldLength, const UnicodeString &newText,
          //                    int32_t newStart, int32_t newLength)
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length,
             const icupy::UnicodeStringVariant &old_text, int32_t old_start,
             int32_t old_length, const icupy::UnicodeStringVariant &new_text,
             int32_t new_start, int32_t new_length) -> UnicodeString & {
            return self.findAndReplace(
                start, length, icupy::to_unistr(old_text), old_start,
                old_length, icupy::to_unistr(new_text), new_start, new_length);
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"),
          py::arg("old_start"), py::arg("old_length"), py::arg("new_text"),
          py::arg("new_start"), py::arg("new_length"), R"doc(
      Replace all occurrences of the substring
      *old_text[old_start:old_start + old_length]* of *old_text* within the
      substring *self[start:start + length]* of this ``UnicodeString`` with the
      substring *new_text[new_start:new_start + new_length]* of *new_text*, and
      return the ``UnicodeString`` itself.
      )doc");

  us.def("fold_case", &UnicodeString::foldCase, py::arg("options") = 0, R"doc(
      Case-fold this ``UnicodeString`` and return the ``UnicodeString`` itself.

      *options* is either :attr:`U_FOLD_CASE_DEFAULT` or
      :attr:`U_FOLD_CASE_EXCLUDE_SPECIAL_I`.
      )doc");

  us.def_static(
      "from_utf32",
      [](const py::bytes &utf32) {
        auto str = utf32.cast<std::string>();
        auto p = reinterpret_cast<const UChar32 *>(str.data());
        int32_t length = static_cast<int32_t>(str.size()) / sizeof(UChar32);
        return UnicodeString::fromUTF32(p, length);
      },
      py::arg("utf32"), R"doc(
      Create a new ``UnicodeString`` instance from a UTF-32 byte sequence.

      Illegal input is replaced with U+FFFD.

      .. seealso::

         :meth:`to_utf32`
      )doc");

  us.def_static(
      "from_utf8",
      [](const py::bytes &utf8) {
        return UnicodeString::fromUTF8(StringPiece(utf8));
      },
      py::arg("utf8"), R"doc(
      Create a new ``UnicodeString`` instance from a UTF-8 byte sequence.

      Illegal input is replaced with U+FFFD.

      .. seealso::

         :meth:`to_utf8`
         :meth:`to_utf8_string`
      )doc");

  us.def(
      "get_buffer",
      [](const UnicodeString &self) -> std::optional<py::memoryview> {
        auto p = self.getBuffer();
        if (p == nullptr) {
          return std::nullopt;
        }
        return py::memoryview::from_buffer(
            reinterpret_cast<uint16_t *>(const_cast<char16_t *>(p)),
            sizeof(uint16_t), py::format_descriptor<uint16_t>::value,
            {self.getCapacity()}, {sizeof(uint16_t)}, true);
      },
      py::keep_alive<0, 1>(), R"doc(
      Return a read-only internal buffer.

      .. note::

         The ``memoryview`` object returned by this method is valid until
         the ``UnicodeString`` object is modified. Once modified, the
         ``memoryview`` object becomes semantically invalid and should not be
         used anymore.

      .. seealso::

         :meth:`.get_terminated_buffer`
      )doc");

  us.def("get_capacity", &UnicodeString::getCapacity, R"doc(
      Return the capacity of the internal buffer.

      .. seealso::

         :meth:`.get_buffer`
      )doc");

  us.def("get_char32_limit", &UnicodeString::getChar32Limit, py::arg("offset"),
         R"doc(
      Adjust *offset* to the code point boundary following the code point.

      If *offset* is behind the lead surrogate of a surrogate pair, *offset*
      is incremented by 1. Otherwise, nothing changes.
      )doc");

  us.def("get_char32_start", &UnicodeString::getChar32Start, py::arg("offset"),
         R"doc(
      Adjust *offset* to the code point boundary preceding the code point.

      If *offset* points to the trail surrogate of a surrogate pair,
      *offset* is decremented by 1. Otherwise, nothing changes.
      )doc");

  us.def(
      "get_terminated_buffer",
      [](UnicodeString &self) -> std::optional<py::memoryview> {
        auto p = self.getTerminatedBuffer();
        if (p == nullptr) {
          return std::nullopt;
        }
        return py::memoryview::from_buffer(
            reinterpret_cast<uint16_t *>(const_cast<char16_t *>(p)),
            sizeof(uint16_t), py::format_descriptor<uint16_t>::value,
            {self.getCapacity()}, {sizeof(uint16_t)}, true);
      },
      py::keep_alive<0, 1>(), R"doc(
      Return a read-only internal buffer.

      The buffer contents is guaranteed to be NUL-terminated.

      .. note::

         The ``memoryview`` object returned by this method is valid until
         the ``UnicodeString`` object is modified. Once modified, the
         ``memoryview`` object becomes semantically invalid and should not be
         used anymore.

      .. seealso::

         :meth:`.get_buffer`
      )doc");

  us.def("hash_code", &UnicodeString::hashCode, R"doc(
      Return a hash code for this instance.

      .. seealso::

         :meth:`.__hash__`
      )doc");

  us.def(
      "has_more_char32_than",
      [](const UnicodeString &self, int32_t start, int32_t length,
         int32_t number) -> py::bool_ {
        return self.hasMoreChar32Than(start, length, number);
      },
      py::arg("start"), py::arg("length"), py::arg("number"), R"doc(
      Return ``True`` if there are more than the specified *number* of code
      points within the substring *self[start:start + length]* of this
      ``UnicodeString``, ``False`` otherwise.
      )doc");

  us.def(
        // [4] indexOf(const char16_t *srcChars, int32_t srcLength,
        //             int32_t start)
        "index_of",
        [](const UnicodeString &self, const std::u16string &src_chars,
           int32_t src_length, int32_t start) {
          return self.indexOf(src_chars.data(), src_length, start);
        },
        py::arg("src_chars"), py::arg("src_length"), py::arg("start"), R"doc(
      Return the index of the first occurrence of the substring
      *src_chars[:src_length]* of *src_chars* within the substring
      *self[start:]* of this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [5] indexOf(const char16_t *srcChars, int32_t srcStart,
          //             int32_t srcLength, int32_t start, int32_t length)
          "index_of",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_start, int32_t src_length, int32_t start,
             int32_t length) {
            return self.indexOf(src_chars.data(), src_start, src_length, start,
                                length);
          },
          py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
          py::arg("start"), py::arg("length"), R"doc(
      Return the index of the first occurrence of the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [6] indexOf(const UnicodeString &srcText, int32_t srcStart,
          //             int32_t srcLength, int32_t start,
          //             int32_t length)
          "index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t, int32_t,
                            int32_t>(&UnicodeString::indexOf, py::const_),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          py::arg("start"), py::arg("length"), R"doc(
      Return the index of the first occurrence of the substring
      *src_text[src_start:src_start + src_length]* of *src_text* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [7] indexOf(const UnicodeString &text)
          "index_of",
          [](const UnicodeString &self,
             const icupy::UnicodeStringVariant &text) {
            return self.indexOf(icupy::to_unistr(text));
          },
          py::arg("text"), R"doc(
      Return the index of the first occurrence of the string *text* within this
      ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [8] indexOf(const UnicodeString &text, int32_t start)
          "index_of",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text,
             int32_t start) {
            return self.indexOf(icupy::to_unistr(text), start);
          },
          py::arg("text"), py::arg("start"), R"doc(
      Return the index of the first occurrence of the string *text* within the
      substring *self[start:]* of this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [9] indexOf(const UnicodeString &text, int32_t start,
          //             int32_t length)
          "index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t>(
              &UnicodeString::indexOf, py::const_),
          py::arg("text"), py::arg("start"), py::arg("length"), R"doc(
      Return the index of the first occurrence of the string *text* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [10] indexOf(ConstChar16Ptr srcChars, int32_t srcLength,
          //              int32_t start, int32_t length)
          "index_of",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length, int32_t start, int32_t length) {
            return self.indexOf(src_chars.data(), src_length, start, length);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("start"),
          py::arg("length"), R"doc(
      Return the index of the first occurrence of the substring
      *src_chars[:src_length]* of *src_chars* within the substring
      *self[start:start + length]* of this ``UnicodeString``, or -1 if not
      found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [11] indexOf(UChar32 c)
          "index_of",
          py::overload_cast<UChar32>(&UnicodeString::indexOf, py::const_),
          py::arg("c"), R"doc(
      Return the index of the first occurrence of the code point *c* within
      this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [12] indexOf(UChar32 c, int32_t start)
          "index_of",
          py::overload_cast<UChar32, int32_t>(&UnicodeString::indexOf,
                                              py::const_),
          py::arg("c"), py::arg("start"), R"doc(
      Return the index of the first occurrence of the code point *c* within the
      substring *self[start:]* of this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [13] indexOf(UChar32 c, int32_t start, int32_t length)
          "index_of",
          py::overload_cast<UChar32, int32_t, int32_t>(&UnicodeString::indexOf,
                                                       py::const_),
          py::arg("c"), py::arg("start"), py::arg("length"), R"doc(
      Return the index of the first occurrence of the code point *c* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc");

  us.def(
        // [2] insert(int32_t start, const char16_t *srcChars, int32_t srcStart,
        //            int32_t srcLength)
        "insert",
        [](UnicodeString &self, int32_t start, const std::u16string &src_chars,
           int32_t src_start, int32_t src_length) -> UnicodeString & {
          return self.insert(start, src_chars.data(), src_start, src_length);
        },
        py::arg("start"), py::arg("src_chars"), py::arg("src_start"),
        py::arg("src_length"), R"doc(
      Insert the substring *src_chars[src_start:src_start + src_length]* of
      *src_chars* into this ``UnicodeString`` at the offset *start*, and return
      the ``UnicodeString`` itself.
      )doc")
      .def(
          // [3] insert(int32_t start, const UnicodeString &srcText)
          "insert",
          [](UnicodeString &self, int32_t start,
             const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.insert(start, icupy::to_unistr(src_text));
          },
          py::arg("start"), py::arg("src_text"), R"doc(
      Insert the string *src_text* into this ``UnicodeString`` at the offset
      *start*, and return the ``UnicodeString`` itself.
      )doc")
      .def(
          // [4] insert(int32_t start, const UnicodeString &srcText,
          //            int32_t srcStart, int32_t srcLength)
          "insert",
          py::overload_cast<int32_t, const UnicodeString &, int32_t, int32_t>(
              &UnicodeString::insert),
          py::arg("start"), py::arg("src_text"), py::arg("src_start"),
          py::arg("src_length"), R"doc(
      Insert the substring *src_text[src_start:src_start + src_length]* of
      *src_text* into this ``UnicodeString`` at the offset *start*, and return
      the ``UnicodeString`` itself.
      )doc")
      .def(
          // [5] insert(int32_t start, ConstChar16Ptr srcChars,
          //            int32_t srcLength)
          "insert",
          [](UnicodeString &self, int32_t start,
             const std::u16string &src_chars,
             int32_t src_length) -> UnicodeString & {
            return self.insert(start, src_chars.data(), src_length);
          },
          py::arg("start"), py::arg("src_chars"), py::arg("src_length"), R"doc(
      Insert the substring *src_chars[:src_length]* of *src_chars* into this
      ``UnicodeString`` at the offset *start*, and return the ``UnicodeString``
      itself.
      )doc")
      .def(
          // [6] insert(int32_t start, UChar32 srcChar)
          "insert", py::overload_cast<int32_t, UChar32>(&UnicodeString::insert),
          py::arg("start"), py::arg("src_char"), R"doc(
      Insert the code point *src_char* into this ``UnicodeString`` at the offset
      *start*, and return the ``UnicodeString`` itself.
      )doc");

  us.def(
      "is_bogus",
      [](const UnicodeString &self) -> py::bool_ { return self.isBogus(); },
      R"doc(
      Return ``True`` if this ``UnicodeString`` is not valid, ``False``
      otherwise.

      .. seealso::

         :meth:`.set_to_bogus`
      )doc");

  us.def(
      "is_empty",
      [](const UnicodeString &self) -> py::bool_ { return self.isEmpty(); },
      R"doc(
      Return ``True`` if this ``UnicodeString`` is empty, ``False`` otherwise.
      )doc");

  us.def(
        // [4] lastIndexOf(const char16_t *srcChars, int32_t srcLength,
        //                 int32_t start)
        "last_index_of",
        [](const UnicodeString &self, const std::u16string &src_chars,
           int32_t src_length, int32_t start) {
          return self.lastIndexOf(src_chars.data(), src_length, start);
        },
        py::arg("src_chars"), py::arg("src_length"), py::arg("start"), R"doc(
      Return the index of the last occurrence of the substring
      *src_chars[:src_length]* of *src_chars* within the substring
      *self[start:]* of this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [5] lastIndexOf(const char16_t *srcChars, int32_t srcStart,
          //                 int32_t srcLength, int32_t start,
          //                 int32_t length)
          "last_index_of",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_start, int32_t src_length, int32_t start,
             int32_t length) {
            return self.lastIndexOf(src_chars.data(), src_start, src_length,
                                    start, length);
          },
          py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
          py::arg("start"), py::arg("length"), R"doc(
      Return the index of the last occurrence of the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [6] lastIndexOf(const UnicodeString &srcText, int32_t srcStart,
          //                 int32_t srcLength, int32_t start,
          //                 int32_t length)
          "last_index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t, int32_t,
                            int32_t>(&UnicodeString::lastIndexOf, py::const_),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          py::arg("start"), py::arg("length"), R"doc(
      Return the index of the last occurrence of the substring
      *src_text[src_start:src_start + src_length]* of *src_text* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [7] lastIndexOf(const UnicodeString &text)
          "last_index_of",
          [](const UnicodeString &self,
             const icupy::UnicodeStringVariant &text) {
            return self.lastIndexOf(icupy::to_unistr(text));
          },
          py::arg("text"), R"doc(
      Return the index of the last occurrence of the string *text* within this
      ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [8] lastIndexOf(const UnicodeString &text, int32_t start)
          "last_index_of",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text,
             int32_t start) {
            return self.lastIndexOf(icupy::to_unistr(text), start);
          },
          py::arg("text"), py::arg("start"), R"doc(
      Return the index of the last occurrence of the string *text* within the
      substring *self[start:]* of this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [9] lastIndexOf(const UnicodeString &text, int32_t start,
          //                 int32_t length)
          "last_index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t>(
              &UnicodeString::lastIndexOf, py::const_),
          py::arg("text"), py::arg("start"), py::arg("length"), R"doc(
      Return the index of the last occurrence of the string *text* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [10] lastIndexOf(ConstChar16Ptr srcChars, int32_t srcLength,
          //                  int32_t start, int32_t length)
          "last_index_of",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length, int32_t start, int32_t length) {
            return self.lastIndexOf(src_chars.data(), src_length, start,
                                    length);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("start"),
          py::arg("length"), R"doc(
      Return the index of the last occurrence of the substring
      *src_chars[:src_length]* of *src_chars* within the substring
      *self[start:start + length]* of this ``UnicodeString``, or -1 if not
      found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [11] lastIndexOf(UChar32 c)
          "last_index_of",
          py::overload_cast<UChar32>(&UnicodeString::lastIndexOf, py::const_),
          py::arg("c"), R"doc(
      Return the index of the last occurrence of the code point *c* within
      this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [12] lastIndexOf(UChar32 c, int32_t start)
          "last_index_of",
          py::overload_cast<UChar32, int32_t>(&UnicodeString::lastIndexOf,
                                              py::const_),
          py::arg("c"), py::arg("start"), R"doc(
      Return the index of the last occurrence of the code point *c* within the
      substring *self[start:]* of this ``UnicodeString``, or -1 if not found.

      The search is performed using a bitwise comparison.
      )doc")
      .def(
          // [13] lastIndexOf(UChar32 c, int32_t start, int32_t length)
          "last_index_of",
          py::overload_cast<UChar32, int32_t, int32_t>(
              &UnicodeString::lastIndexOf, py::const_),
          py::arg("c"), py::arg("start"), py::arg("length"), R"doc(
      Return the index of the last occurrence of the code point *c* within the
      substring *self[start:start + length]* of this ``UnicodeString``, or -1
      if not found.

      The search is performed using a bitwise comparison.
      )doc");

  us.def("move_index32", &UnicodeString::moveIndex32, py::arg("index"),
         py::arg("delta"), R"doc(
      Move *index* by *delta* code points and return the new index.

      If there are more than one code points, the input *index* must point to
      the first code unit of the code points.

      If *delta* < 0 then the index moves backward; if *delta* > 0 then it
      moves forward.

      .. seealso::

         :func:`u16_back_n`
         :func:`u16_fwd_n`
      )doc");

  us.def(
      "pad_leading",
      [](UnicodeString &self, int32_t target_length, uint16_t pad_char)
          -> py::bool_ { return self.padLeading(target_length, pad_char); },
      py::arg("target_length"), py::arg("pad_char") = 0x20, R"doc(
      Pad this ``UnicodeString`` on the left with *pad_char* until it reaches
      *target_length* in length, and return ``True`` if padding is performed,
      ``False`` otherwise.

      *pad_char* is the character used for padding. The default value is Space
      (U+0020).
      )doc");

  us.def(
      "pad_trailing",
      [](UnicodeString &self, int32_t target_length, uint16_t pad_char)
          -> py::bool_ { return self.padTrailing(target_length, pad_char); },
      py::arg("target_length"), py::arg("pad_char") = 0x20, R"doc(
      Pad this ``UnicodeString`` on the right with *pad_char* until it reaches
      *target_length* in length, and return ``True`` if padding is performed,
      ``False`` otherwise.

      *pad_char* is the character used for padding. The default value is Space
      (U+0020).
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 78)
  us.def("push_back", &UnicodeString::push_back, py::arg("c"), R"doc(
      Append the code unit *c* to this ``UnicodeString``.

      .. seealso::

         :meth:`.append`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)

  us.def("remove", py::overload_cast<>(&UnicodeString::remove), R"doc(
      Remove all characters from this ``UnicodeString``, clear the bogus flag,
      and return the ``UnicodeString`` itself.

      .. seealso::

         :meth:`.set_to_bogus`
      )doc")
      .def("remove",
           py::overload_cast<int32_t, int32_t>(&UnicodeString::remove),
           py::arg("start"), py::arg("length") = INT32_MAX, R"doc(
      Remove the characters within the range [*start*, *start* + *length*) from
      this ``UnicodeString`` and return the ``UnicodeString`` itself.
      )doc");

  us.def("remove_between", &UnicodeString::removeBetween, py::arg("start"),
         py::arg("limit") = INT32_MAX, R"doc(
      Remove the characters within the range [*start*, *limit*) from this
      ``UnicodeString`` and return the ``UnicodeString`` itself.
      )doc");

  us.def(
        // [2] replace(int32_t start, int32_t length, const char16_t *srcChars,
        //             int32_t srcStart, int32_t srcLength)
        "replace",
        [](UnicodeString &self, int32_t start, int32_t length,
           const std::u16string &src_chars, int32_t src_start,
           int32_t src_length) -> UnicodeString & {
          return self.replace(start, length, src_chars.data(), src_start,
                              src_length);
        },
        py::arg("start"), py::arg("length"), py::arg("src_chars"),
        py::arg("src_start"), py::arg("src_length"), R"doc(
      Replace the characters within the range [*start*, *start* + *length*) of
      this ``UnicodeString`` with the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars*, and return
      the ``UnicodeString`` itself.
      )doc")
      .def(
          // [3] replace(int32_t start, int32_t length,
          //             const UnicodeString &srcText)
          "replace",
          [](UnicodeString &self, int32_t start, int32_t length,
             const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.replace(start, length, icupy::to_unistr(src_text));
          },
          py::arg("start"), py::arg("length"), py::arg("src_text"), R"doc(
      Replace the characters within the range [*start*, *start* + *length*) of
      this ``UnicodeString`` with the string *src_text*, and return the
      ``UnicodeString`` itself.
      )doc")
      .def(
          // [4] replace(int32_t start, int32_t length,
          //             const UnicodeString &srcText, int32_t srcStart,
          //             int32_t srcLength)
          "replace",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t,
                            int32_t>(&UnicodeString::replace),
          py::arg("start"), py::arg("length"), py::arg("src_text"),
          py::arg("src_start"), py::arg("src_length"), R"doc(
      Replace the characters within the range [*start*, *start* + *length*) of
      this ``UnicodeString`` with the substring
      *src_text[src_start:src_start + src_length]* of *src_text*, and return
      the ``UnicodeString`` itself.
      )doc")
      .def(
          // [5] replace(int32_t start, int32_t length, ConstChar16Ptr srcChars,
          //             int32_t srcLength)
          "replace",
          [](UnicodeString &self, int32_t start, int32_t length,
             const std::u16string &src_chars,
             int32_t src_length) -> UnicodeString & {
            return self.replace(start, length, src_chars.data(), src_length);
          },
          py::arg("start"), py::arg("length"), py::arg("src_chars"),
          py::arg("src_length"), R"doc(
      Replace the characters within the range [*start*, *start* + *length*) of
      this ``UnicodeString`` with the substring *src_chars[:src_length]* of
      *src_chars*, and return the ``UnicodeString`` itself.
      )doc")
      .def(
          // [6] replace(int32_t start, int32_t length, UChar32 srcChar)
          "replace",
          py::overload_cast<int32_t, int32_t, UChar32>(&UnicodeString::replace),
          py::arg("start"), py::arg("length"), py::arg("src_char"), R"doc(
      Replace the characters within the range [*start*, *start* + *length*) of
      this ``UnicodeString`` with the code point *src_char*, and return the
      ``UnicodeString`` itself.
      )doc");

  us.def(
        // [1] replaceBetween(int32_t start, int32_t limit,
        //                    const UnicodeString &srcText)
        "replace_between",
        [](UnicodeString &self, int32_t start, int32_t limit,
           const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
          return self.replaceBetween(start, limit, icupy::to_unistr(src_text));
        },
        py::arg("start"), py::arg("limit"), py::arg("src_text"), R"doc(
      Replace the characters within the range [*start*, *limit*) of this
      ``UnicodeString`` with the string *src_text*, and return the
      ``UnicodeString`` itself.
      )doc")
      .def(
          // [2] replaceBetween(int32_t start, int32_t limit,
          //                    const UnicodeString &srcText, int32_t srcStart,
          //                    int32_t srcLimit)
          "replace_between",
          [](UnicodeString &self, int32_t start, int32_t limit,
             const icupy::UnicodeStringVariant &src_text, int32_t src_start,
             int32_t src_limit) -> UnicodeString & {
            return self.replaceBetween(start, limit, icupy::to_unistr(src_text),
                                       src_start, src_limit);
          },
          py::arg("start"), py::arg("limit"), py::arg("src_text"),
          py::arg("src_start"), py::arg("src_limit"), R"doc(
      Replace the characters within the range [*start*, *limit*) of this
      ``UnicodeString`` with the substring *src_text[src_start:src_limit]* of
      *src_text*, and return the ``UnicodeString`` itself.
      )doc");

  us.def("retain_between", &UnicodeString::retainBetween, py::arg("start"),
         py::arg("limit") = INT32_MAX, R"doc(
      Retain only the characters within the range [*start*, *limit*) from this
      ``UnicodeString`` and return the ``UnicodeString`` itself.

      The characters before *start* and those after *limit* including *limit*
      will be deleted.
      )doc");

  us.def("reverse",
         py::overload_cast<int32_t, int32_t>(&UnicodeString::reverse),
         py::arg("start"), py::arg("length"), R"doc(
      Reverse the characters within the range [*start*, *start* + *length*) of
      this ``UnicodeString`` and return the ``UnicodeString`` itself.
      )doc")
      .def("reverse", py::overload_cast<>(&UnicodeString::reverse), R"doc(
      Reverse the characters in this ``UnicodeString`` and return the
      ``UnicodeString`` itself.
      )doc");

  us.def(
      "set_char_at",
      [](UnicodeString &self, int32_t offset, uint16_t ch) -> UnicodeString & {
        return self.setCharAt(offset, ch);
      },
      py::arg("offset"), py::arg("ch"), R"doc(
      Set the code unit at the specified *offset* to *ch* and return this
      ``UnicodeString`` itself.

      .. seealso::

         :meth:`.__setitem__`
      )doc");

  us.def(
        // [3] setTo(const char16_t *srcChars, int32_t srcLength)
        "set_to",
        [](UnicodeString &self, const std::u16string &src_chars,
           int32_t src_length) -> UnicodeString & {
          return self.setTo(src_chars.data(), src_length);
        },
        py::arg("src_chars"), py::arg("src_length"), R"doc(
      Set this ``UnicodeString`` to the substring *src_chars[:src_length]* of
      *src_chars* and return the ``UnicodeString`` itself.
      )doc")
      .def(
          // [4] setTo(const UnicodeString &srcText)
          "set_to",
          [](UnicodeString &self,
             const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.setTo(icupy::to_unistr(src_text));
          },
          py::arg("src_text"), R"doc(
      Set this ``UnicodeString`` to the string *src_text* and return the
      ``UnicodeString`` itself.
      )doc")
      .def(
          // [5] setTo(const UnicodeString &srcText, int32_t srcStart)
          "set_to",
          py::overload_cast<const UnicodeString &, int32_t>(
              &UnicodeString::setTo),
          py::arg("src_text"), py::arg("src_start"), R"doc(
      Set this ``UnicodeString`` to the substring *src_text[src_start:]* of
      *src_text* and return the ``UnicodeString`` itself.
      )doc")
      .def(
          // [6] setTo(const UnicodeString &srcText, int32_t srcStart, int32_t
          // srcLength)
          "set_to",
          py::overload_cast<const UnicodeString &, int32_t, int32_t>(
              &UnicodeString::setTo),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          R"doc(
      Set this ``UnicodeString`` to the substring
      *src_text[src_start:src_length]* of *src_text* and return the
      ``UnicodeString`` itself.
      )doc")
      .def(
          // [8] setTo(UChar32 srcChar)
          "set_to", py::overload_cast<UChar32>(&UnicodeString::setTo),
          py::arg("src_char"), R"doc(
      Set this ``UnicodeString`` to the code point *src_char* and return the
      ``UnicodeString`` itself.
      )doc");

  us.def("set_to_bogus", &UnicodeString::setToBogus, R"doc(
      Set this ``UnicodeString`` to be invalid.

      .. seealso::

         :meth:`.is_bogus`
      )doc");

  us.def(
        // [1] startsWith(const char16_t *srcChars, int32_t srcStart,
        //                int32_t srcLength)
        "starts_with",
        [](const UnicodeString &self, const std::u16string &src_chars,
           int32_t src_start, int32_t src_length) -> py::bool_ {
          return self.startsWith(src_chars.data(), src_start, src_length);
        },
        py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
        R"doc(
      Return ``True`` if this ``UnicodeString`` starts with the substring
      *src_chars[src_start:src_start + src_length]* of *src_chars*, ``False``
      otherwise.
      )doc")
      .def(
          // [2] startsWith(const UnicodeString &srcText, int32_t srcStart,
          //                int32_t srcLength)
          "starts_with",
          [](const UnicodeString &self, const UnicodeString &src_text,
             int32_t src_start, int32_t src_length) -> py::bool_ {
            return self.startsWith(src_text, src_start, src_length);
          },
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          R"doc(
      Return ``True`` if this ``UnicodeString`` starts with the substring
      *src_text[src_start:src_start + src_length]* of *src_text*, ``False``
      otherwise.
      )doc")
      .def(
          // [3] startsWith(const UnicodeString &text)
          "starts_with",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text)
              -> py::bool_ { return self.startsWith(icupy::to_unistr(text)); },
          py::arg("text"), R"doc(
      Return ``True`` if this ``UnicodeString`` starts with the string *text*,
      ``False`` otherwise.
      )doc")
      .def(
          // [4] startsWith(ConstChar16Ptr srcChars, int32_t srcLength)
          "starts_with",
          [](const UnicodeString &self, const std::u16string &src_chars,
             int32_t src_length) -> py::bool_ {
            return self.startsWith(src_chars.data(), src_length);
          },
          py::arg("src_chars"), py::arg("src_length"), R"doc(
      Return ``True`` if this ``UnicodeString`` starts with the substring
      *src_chars[:src_length]* of *src_chars*, ``False`` otherwise.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  us.def("swap", &UnicodeString::swap, py::arg("other"), R"doc(
      Swap the contents of this ``UnicodeString`` with those of *other*.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

  us.def("temp_sub_string", &UnicodeString::tempSubString, py::arg("start") = 0,
         py::arg("length") = INT32_MAX, py::keep_alive<0, 1>(), R"doc(
      Return a temporary ``UnicodeString`` that is a substring
      *self[start:start + length]* of this ``UnicodeString``.

      .. version-deprecated:: 0.24
         Do not use this method. It may be removed in a future release.
      )doc");

  us.def("temp_sub_string_between", &UnicodeString::tempSubStringBetween,
         py::arg("start") = 0, py::arg("limit") = INT32_MAX,
         py::keep_alive<0, 1>(), R"doc(
      Return a temporary ``UnicodeString`` that is a substring
      *self[start:limit]* of this ``UnicodeString``.

      .. version-deprecated:: 0.24
         Do not use this method. It may be removed in a future release.
      )doc");

  us.def(
        "to_lower",
        [](UnicodeString &self,
           const icupy::LocaleVariant &locale) -> UnicodeString & {
          return self.toLower(icupy::to_locale(locale));
        },
        py::arg("locale"), R"doc(
      Convert this ``UnicodeString`` to lowercase according to the case mapping
      rules of the specified *locale* and return the ``UnicodeString`` itself.
      )doc")
      .def("to_lower", py::overload_cast<>(&UnicodeString::toLower), R"doc(
      Convert this ``UnicodeString`` to lowercase according to the case mapping
      rules of the default locale and return the ``UnicodeString`` itself.
      )doc");

  us.def(
        "to_title",
        [](UnicodeString &self,
           std::optional<BreakIterator *> &title_iter) -> UnicodeString & {
          return self.toTitle(title_iter.value_or(nullptr));
        },
        py::arg("title_iter") = std::nullopt, R"doc(
      Convert this ``UnicodeString`` to titlecase using the default locale
      and return the ``UnicodeString`` itself.

      *title_iter* is a break iterator used to find the first characters of
      titlecase words. If *title_iter* is ``None``, the standard titlecase
      break iterator is used.
      )doc")
      .def(
          "to_title",
          [](UnicodeString &self, std::optional<BreakIterator *> &title_iter,
             const icupy::LocaleVariant &locale) -> UnicodeString & {
            return self.toTitle(title_iter.value_or(nullptr),
                                icupy::to_locale(locale));
          },
          py::arg("title_iter"), py::arg("locale"), R"doc(
      Convert this ``UnicodeString`` to titlecase using the specified *locale*
      and return the ``UnicodeString`` itself.

      *title_iter* is a break iterator used to find the first characters of
      titlecase words. If *title_iter* is ``None``, the standard titlecase
      break iterator is used.
      )doc")
      .def(
          "to_title",
          [](UnicodeString &self, std::optional<BreakIterator *> &title_iter,
             const icupy::LocaleVariant &locale,
             uint32_t options) -> UnicodeString & {
            return self.toTitle(title_iter.value_or(nullptr),
                                icupy::to_locale(locale), options);
          },
          py::arg("title_iter"), py::arg("locale"), py::arg("options"), R"doc(
      Convert this ``UnicodeString`` to titlecase using the specified *locale*
      and *options*, and return the ``UnicodeString`` itself.

      *title_iter* is a break iterator used to find the first characters of
      titlecase words. If *title_iter* is ``None``, the standard titlecase
      break iterator is used.

      *options* is a bit set of following values, usually 0:
      :attr:`U_TITLECASE_NO_LOWERCASE`,
      :attr:`U_TITLECASE_NO_BREAK_ADJUSTMENT`,
      :attr:`U_TITLECASE_ADJUST_TO_CASED`,
      :attr:`U_TITLECASE_WHOLE_STRING`, and
      :attr:`U_TITLECASE_SENTENCES`.
      )doc");

  us.def(
        "to_upper",
        [](UnicodeString &self,
           const icupy::LocaleVariant &locale) -> UnicodeString & {
          return self.toUpper(icupy::to_locale(locale));
        },
        py::arg("locale"), R"doc(
      Convert this ``UnicodeString`` to uppercase according to the case mapping
      rules of the specified *locale* and return the ``UnicodeString`` itself.
      )doc")
      .def("to_upper", py::overload_cast<>(&UnicodeString::toUpper), R"doc(
      Convert this ``UnicodeString`` to uppercase according to the case mapping
      rules of the default locale and return the ``UnicodeString`` itself.
      )doc");

  us.def(
      "to_utf32",
      [](const UnicodeString &self) {
        ErrorCode error_code;
        const auto length = self.toUTF32(nullptr, 0, error_code);
        std::vector<UChar32> dest(length, 0);
        error_code.reset();
        self.toUTF32(dest.data(), length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(reinterpret_cast<const char *>(dest.data()),
                         length * sizeof(UChar32));
      },
      R"doc(
      Convert this ``UnicodeString`` to UTF-32 and return the result.

      Unpaired surrogates are replaced with U+FFFD.

      .. seealso::

         :meth:`.extract`
         :meth:`.from_utf32`
      )doc");

  us.def(
      "to_utf8",
      [](const UnicodeString &self) {
        std::string result;
        auto sink = StringByteSink(&result);
        self.toUTF8(sink);
        return py::bytes(result.data(), result.size());
      },
      R"doc(
      Convert this ``UnicodeString`` to UTF-8 and return the result.

      Unpaired surrogates are replaced with U+FFFD.

      .. seealso::

         :meth:`.extract`
         :meth:`.from_utf8`
         :meth:`.to_utf8_string`
      )doc");

  us.def(
      "to_utf8_string",
      [](const UnicodeString &self) {
        std::string result;
        self.toUTF8String(result);
        return result;
      },
      R"doc(
      Convert this ``UnicodeString`` to a UTF-8 string and return the result.

      Unpaired surrogates are replaced with U+FFFD.

      .. seealso::

         :meth:`.__str__`
         :meth:`.extract`
         :meth:`.from_utf8`
         :meth:`.to_utf8`
      )doc");

  us.def("trim", &UnicodeString::trim, R"doc(
      Trim leading and trailing whitespace characters from this
      ``UnicodeString`` and return the ``UnicodeString`` itself.
      )doc");

  us.def(
      "truncate",
      [](UnicodeString &self, int32_t target_length) -> py::bool_ {
        return self.truncate(target_length);
      },
      py::arg("target_length"), R"doc(
      Truncate this ``UnicodeString`` to the specified *target_length* and return
      a boolean indicating whether the string was modified.
      )doc");

  us.def("unescape", &UnicodeString::unescape, R"doc(
      Unescape this ``UnicodeString`` and return the result.
      )doc");

  us.def("unescape_at", &UnicodeString::unescapeAt, py::arg("offset"), R"doc(
      Unescape a single escape sequence at the specified *offset* in this
      ``UnicodeString`` and return the represented character, or
      :attr:`U_SENTINEL` if the *offset* is invalid.
      )doc");

  us.def_property_readonly_static(
      "SENTINEL", [](const py::object &) { return U_SENTINEL; });

  us.def_property_readonly_static(
      "COMPARE_CODE_POINT_ORDER",
      [](const py::object &) { return U_COMPARE_CODE_POINT_ORDER; });

  us.def_property_readonly_static("FOLD_CASE_DEFAULT", [](const py::object &) {
    return U_FOLD_CASE_DEFAULT;
  });

  us.def_property_readonly_static(
      "FOLD_CASE_EXCLUDE_SPECIAL_I",
      [](const py::object &) { return U_FOLD_CASE_EXCLUDE_SPECIAL_I; });

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static(
      "TITLECASE_ADJUST_TO_CASED",
      [](const py::object &) { return U_TITLECASE_ADJUST_TO_CASED; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  us.def_property_readonly_static(
      "TITLECASE_NO_BREAK_ADJUSTMENT",
      [](const py::object &) { return U_TITLECASE_NO_BREAK_ADJUSTMENT; });

  us.def_property_readonly_static(
      "TITLECASE_NO_LOWERCASE",
      [](const py::object &) { return U_TITLECASE_NO_LOWERCASE; });

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static(
      "TITLECASE_SENTENCES",
      [](const py::object &) { return U_TITLECASE_SENTENCES; });

  us.def_property_readonly_static(
      "TITLECASE_WHOLE_STRING",
      [](const py::object &) { return U_TITLECASE_WHOLE_STRING; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  m.attr("US_INV") = US_INV;
}

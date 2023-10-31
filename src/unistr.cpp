#include "char16ptr.hpp"
#include "main.hpp"
#include "ucnv_err.hpp"
#include <iomanip>
#include <memory>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/brkiter.h>
#include <unicode/locid.h>
#include <unicode/uchar.h>
#include <unicode/uchriter.h>

using namespace icu;

void init_unistr(py::module &m, py::class_<Replaceable, UObject> &rep, py::class_<UnicodeString, Replaceable> &us) {
  //
  // icu::Replaceable
  //
  rep.def("__copy__", &Replaceable::clone);

  rep.def(
      "__deepcopy__", [](const Replaceable &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  rep.def("__len__", &Replaceable::length);

  rep.def("char32_at", &Replaceable::char32At, py::arg("offset"));

  rep.def("clone", &Replaceable::clone);

  rep.def(
      "char_at", [](const Replaceable &self, int32_t offset) -> uint16_t { return self.charAt(offset); },
      py::arg("offset"));

  rep.def("copy", &Replaceable::copy, py::arg("start"), py::arg("limit"), py::arg("dest"));

  rep.def("extract_between", &Replaceable::extractBetween, py::arg("start"), py::arg("limit"), py::arg("target"));

  rep.def(
      "handle_replace_between",
      [](Replaceable &self, int32_t start, int32_t limit, const icupy::UnicodeStringVariant &text) {
        self.handleReplaceBetween(start, limit, icupy::to_unistr(text));
      },
      py::arg("start"), py::arg("limit"), py::arg("text"));

  rep.def("length", &Replaceable::length);

  //
  // icu::UnicodeString::EInvariant
  //
  py::enum_<UnicodeString::EInvariant>(
      us, "EInvariant", py::arithmetic(),
      "Constant to be used in the *UnicodeString(char *, int32_t, EInvariant)* constructor which constructs a Unicode "
      "string from an invariant-character char \\* string.\n\n"
      "Use the macro *US_INV* instead of the full qualification for this value.")
      .value("INVARIANT", UnicodeString::kInvariant)
      .export_values();

  //
  // icu::UnicodeString
  //
  us.def(py::init<>())
      .def(py::init<int32_t, UChar32, int32_t>(), py::arg("capacity"), py::arg("c"), py::arg("count"))
      .def(py::init<UChar32>(), py::arg("ch"))
#if (U_ICU_VERSION_MAJOR_NUM >= 59)
      .def(py::init<const char16_t *>(), py::arg("text"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
      .def(py::init<const char16_t *, int32_t>(), py::arg("text"), py::arg("text_length"))
      /*
      .def(py::init<UBool, ConstChar16Ptr, int32_t>(), py::arg("is_terminated"), py::arg("text"),
           py::arg("text_length"), py::keep_alive<1, 3>())
      .def(
          // UnicodeString(UBool isTerminated, ConstChar16Ptr text, int32_t textLength)
          py::init([](UBool is_terminated, const char16_t *text, int32_t text_length) {
            return new UnicodeString(is_terminated, text, text_length);
          }),
          py::arg("is_terminated"), py::arg("text"), py::arg("text_length"),
          py::keep_alive<1, 3>())
      */
      .def(py::init<const char *, const char *>(), py::arg("codepage_data"), py::arg("codepage"))
      .def(py::init<const char *, int32_t, const char *>(), py::arg("codepage_data"), py::arg("data_length"),
           py::arg("codepage"))
      .def(py::init([](const char *src, int32_t src_length, _UConverterPtr &cnv) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeString>(src, src_length, cnv, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("src"), py::arg("src_length"), py::arg("cnv"))
      .def(py::init<const char *, int32_t, UnicodeString::EInvariant>(), py::arg("src"), py::arg("text_length"),
           py::arg("inv"))
      .def(py::init<const UnicodeString &>(), py::arg("other"))
      .def(py::init<const UnicodeString &, int32_t>(), py::arg("src"), py::arg("src_start"))
      .def(py::init<const UnicodeString &, int32_t, int32_t>(), py::arg("src"), py::arg("src_start"),
           py::arg("src_length"));

  us.def(
      "__add__",
      [](UnicodeString &self, const icupy::UnicodeStringVariant &other) { return self + icupy::to_unistr(other); },
      py::is_operator(), py::arg("other"));

  us.def(
      "__contains__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &item) {
        return self.indexOf(icupy::to_unistr(item)) >= 0;
      },
      py::arg("item"));

  us.def("__copy__", &UnicodeString::clone);

  us.def(
      "__deepcopy__", [](const UnicodeString &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  us.def(
      "__eq__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) {
        return self == icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"));

  us.def(
      "__ge__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) -> bool {
        return self >= icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"));

  us.def(
        "__getitem__",
        [](const UnicodeString &self, int32_t index) {
          const auto length = self.length();
          if (index < 0) {
            index += length;
          }
          if (index < 0 || index >= length) {
            throw py::index_error("string index out of range: " + std::to_string(index));
          }
          return self[index];
        },
        py::arg("index"))
      .def(
          "__getitem__",
          [](const UnicodeString &self, const py::slice &index) {
            size_t start, stop, step, slice_length;
            if (!index.compute(self.length(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }
            UnicodeString result;
            for (size_t n = 0; n < slice_length; ++n) {
              result.append(self[static_cast<int32_t>(start)]);
              start += step;
            }
            return result;
          },
          py::arg("index"));

  us.def(
      "__gt__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) -> bool {
        return self > icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"));

  us.def("__hash__", &UnicodeString::hashCode);

  us.def(
        "__iadd__",
        [](UnicodeString &self, const icupy::UnicodeStringVariant &other) -> UnicodeString & {
          return self += icupy::to_unistr(other);
        },
        py::is_operator(), py::arg("other"))
      .def(
          "__iadd__", [](UnicodeString &self, UChar32 other) -> UnicodeString & { return self += other; },
          py::is_operator(), py::arg("other"));

  us.def(
      "__le__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) -> bool {
        return self <= icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"));

  us.def(
      "__lt__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) -> bool {
        return self < icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"));

  us.def(
      "__ne__",
      [](const UnicodeString &self, const icupy::UnicodeStringVariant &other) {
        return self != icupy::to_unistr(other);
      },
      py::is_operator(), py::arg("other"));

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
        ss << prefix << std::nouppercase << std::hex << std::setw(w) << std::setfill('0') << c;
      }
    }
    ss << "', text_length=" << std::dec << text_length;
    ss << ")";
    return ss.str();
  });

  us.def(
      "__setitem__",
      [](UnicodeString &self, int32_t index, const icupy::Char16Variant &value) {
        const auto length = self.length();
        if (index < 0) {
          index += length;
        }
        if (index < 0 || index >= length) {
          throw py::index_error("string index out of range: " + std::to_string(index));
        }
        self.setCharAt(index, icupy::to_char16(value));
      },
      py::arg("index"), py::arg("value"));

  us.def("__str__", [](const UnicodeString &self) {
    std::string result;
    self.toUTF8String(result);
    return result;
  });

  // us.def(
  //       // [1] append(char16_t srcChar)
  //       "append", py::overload_cast<char16_t>(&UnicodeString::append), py::arg("src_char"))
  us.def(
        // [2] append(const char16_t *srcChars, int32_t srcStart, int32_t srcLength)
        "append", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::append), py::arg("src_chars"),
        py::arg("src_start"), py::arg("src_length"))
      .def(
          // [3] append(const UnicodeString &srcText)
          "append",
          [](UnicodeString &self, const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.append(icupy::to_unistr(src_text));
          },
          py::arg("src_text"))
      .def(
          // [4] append(const UnicodeString &srcText, int32_t srcStart, int32_t srcLength)
          "append", py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::append),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [5] append(ConstChar16Ptr srcChars, int32_t srcLength)
          "append",
          [](UnicodeString &self, const char16_t *src_chars, int32_t src_length) -> UnicodeString & {
            return self.append(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"))
      .def(
          // [6] append(UChar32 srcChar)
          "append", py::overload_cast<UChar32>(&UnicodeString::append), py::arg("src_char"));

  us.def(
        // [1] caseCompare(const UnicodeString &text, uint32_t options)
        "case_compare",
        [](const UnicodeString &self, const icupy::UnicodeStringVariant &text, uint32_t options) {
          return self.caseCompare(icupy::to_unistr(text), options);
        },
        py::arg("text"), py::arg("options"))
      .def(
          // [2] caseCompare(ConstChar16Ptr srcChars, int32_t srcLength, uint32_t options)
          "case_compare",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length, uint32_t options) {
            return self.caseCompare(src_chars, src_length, options);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("options"))
      .def(
          // [3] caseCompare(int32_t start, int32_t length, const char16_t *srcChars, int32_t srcStart,
          //                 int32_t srcLength, uint32_t options)
          "case_compare",
          py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t, uint32_t>(&UnicodeString::caseCompare,
                                                                                            py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
          py::arg("options"))
      .def(
          // [4] caseCompare(int32_t start, int32_t length, const char16_t *srcChars, uint32_t options)
          "case_compare",
          py::overload_cast<int32_t, int32_t, const char16_t *, uint32_t>(&UnicodeString::caseCompare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("options"))
      .def(
          // [5] caseCompare(int32_t start, int32_t length, const UnicodeString &srcText,
          //                 int32_t srcStart, int32_t srcLength, uint32_t options)
          "case_compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t, uint32_t>(
              &UnicodeString::caseCompare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
          py::arg("options"))
      .def(
          // [6] caseCompare(int32_t start, int32_t length, const UnicodeString &srcText, uint32_t options)
          "case_compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, uint32_t>(&UnicodeString::caseCompare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("options"));

  us.def(
      "case_compare_between",
      [](const UnicodeString &self, int32_t start, int32_t limit, const icupy::UnicodeStringVariant &src_text,
         int32_t src_start, int32_t src_limit, uint32_t options) {
        return self.caseCompareBetween(start, limit, icupy::to_unistr(src_text), src_start, src_limit, options);
      },
      py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"),
      py::arg("options"));

  us.def("clone", &UnicodeString::clone);

  us.def(
        // [1] compare(const UnicodeString &text)
        "compare",
        [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) {
          return self.compare(icupy::to_unistr(text));
        },
        py::arg("text"))
      .def(
          // [2] compare(ConstChar16Ptr srcChars, int32_t srcLength)
          "compare",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) {
            return self.compare(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"))
      .def(
          // [3] compare(int32_t start, int32_t length, const char16_t *srcChars)
          "compare", py::overload_cast<int32_t, int32_t, const char16_t *>(&UnicodeString::compare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_chars"))
      .def(
          // [4] compare(int32_t start, int32_t length, const char16_t *srcChars, int32_t srcStart, int32_t srcLength)
          "compare",
          py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::compare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [5] compare(int32_t start, int32_t length, const UnicodeString &srcText, int32_t srcStart,
          //             int32_t srcLength)
          "compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::compare,
                                                                                       py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [6] compare(int32_t start, int32_t length, const UnicodeString &text)
          "compare", py::overload_cast<int32_t, int32_t, const UnicodeString &>(&UnicodeString::compare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("text"));

  us.def(
      "compare_between",
      [](const UnicodeString &self, int32_t start, int32_t limit, const icupy::UnicodeStringVariant &src_text,
         int32_t src_start, int32_t src_limit) {
        return self.compareBetween(start, limit, icupy::to_unistr(src_text), src_start, src_limit);
      },
      py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"));

  us.def(
        // [1] compareCodePointOrder(const UnicodeString &text)
        "compare_code_point_order",
        [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) {
          return self.compareCodePointOrder(icupy::to_unistr(text));
        },
        py::arg("text"))
      .def(
          // [2] compareCodePointOrder(ConstChar16Ptr srcChars, int32_t srcLength)
          "compare_code_point_order",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) {
            return self.compareCodePointOrder(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"))
      .def(
          // [3] compareCodePointOrder(int32_t start, int32_t length, const char16_t *srcChars)
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const char16_t *>(&UnicodeString::compareCodePointOrder, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_chars"))
      .def(
          // [4] compareCodePointOrder(int32_t start, int32_t length, const char16_t *srcChars, int32_t srcStart,
          //                           int32_t srcLength)
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::compareCodePointOrder,
                                                                                  py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [5] compareCodePointOrder(int32_t start, int32_t length, const UnicodeString &srcText)
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const UnicodeString &>(&UnicodeString::compareCodePointOrder, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"))
      .def(
          // [6] compareCodePointOrder(int32_t start, int32_t length, const UnicodeString &srcText, int32_t srcStart,
          //                           int32_t srcLength)
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(
              &UnicodeString::compareCodePointOrder, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"));

  us.def(
      "compare_code_point_order_between",
      [](const UnicodeString &self, int32_t start, int32_t limit, const icupy::UnicodeStringVariant &src_text,
         int32_t src_start, int32_t src_limit) {
        return self.compareCodePointOrderBetween(start, limit, icupy::to_unistr(src_text), src_start, src_limit);
      },
      py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"));

  us.def("count_char32", &UnicodeString::countChar32, py::arg("start ") = 0, py::arg("length") = INT32_MAX);

  us.def(
        // [1] endsWith(const char16_t *srcChars, int32_t srcStart, int32_t srcLength)
        "ends_with",
        [](const UnicodeString &self, const char16_t *src_chars, int32_t src_start, int32_t src_length) -> py::bool_ {
          return self.endsWith(src_chars, src_start, src_length);
        },
        py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [2] endsWith(const UnicodeString &srcText, int32_t srcStart, int32_t srcLength)
          "ends_with",
          [](const UnicodeString &self, const UnicodeString &src_text, int32_t src_start,
             int32_t src_length) -> py::bool_ { return self.endsWith(src_text, src_start, src_length); },
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [3] endsWith(const UnicodeString &text)
          "ends_with",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &src_text) -> py::bool_ {
            return self.endsWith(icupy::to_unistr(src_text));
          },
          py::arg("src_text"))
      .def(
          // [4] endsWith(ConstChar16Ptr srcChars, int32_t srcLength)
          "ends_with",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) -> py::bool_ {
            return self.endsWith(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"));

  us.def(
        // [1] extract(char *dest, int32_t destCapacity, UConverter *cnv, UErrorCode &errorCode)
        "extract",
        [](const UnicodeString &self, _UConverterPtr &cnv) {
          ErrorCode error_code;
          const auto length = self.extract(nullptr, 0, cnv, error_code);
          std::string dest(length, '\0');
          error_code.reset();
          self.extract(dest.data(), static_cast<int32_t>(dest.size()), cnv, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return py::bytes(dest.data(), length);
        },
        py::arg("cnv"))
      .def(
          // [2] extract(Char16Ptr dest, int32_t destCapacity, UErrorCode &errorCode)
          "extract",
          [](const UnicodeString &self) {
            std::u16string result(self.length(), u'\0');
            ErrorCode error_code;
            self.extract(result.data(), static_cast<int32_t>(result.size()), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          })
      .def(
          // [4] extract(int32_t start, int32_t length, UnicodeString &target)
          "extract", py::overload_cast<int32_t, int32_t, UnicodeString &>(&UnicodeString::extract, py::const_),
          py::arg("start"), py::arg("length"), py::arg("target"))
      .def(
          // [5] extract(int32_t start, int32_t startLength, char *target, const char *codepage = 0)
          "extract",
          [](const UnicodeString &self, int32_t start, int32_t start_length, const char *codepage) {
            const auto length = self.extract(start, start_length, nullptr, codepage);
            if (length <= 0) {
              return py::bytes();
            }
            std::string target(length, '\0');
            self.extract(start, start_length, target.data(), codepage);
            return py::bytes(target.data(), length);
          },
          py::arg("start"), py::arg("start_length"), py::arg("codepage"))
      .def(
          // [6] extract(int32_t start, int32_t startLength, char *target, int32_t targetCapacity,
          //             enum EInvariant inv)
          "extract",
          [](const UnicodeString &self, int32_t start, int32_t start_length, UnicodeString::EInvariant inv) {
            const auto length = self.extract(start, start_length, nullptr, 0, inv);
            if (length <= 0) {
              return py::bytes();
            }
            std::string target(length, '\0');
            self.extract(start, start_length, target.data(), static_cast<int32_t>(target.size()), inv);
            return py::bytes(target.data(), length);
          },
          py::arg("start"), py::arg("start_length"), py::arg("inv"))
      .def(
          // [7] extract(int32_t start, int32_t startLength, char *target, uint32_t targetLength)
          "extract",
          [](const UnicodeString &self, int32_t start, int32_t start_length) {
            const auto length = self.extract(start, start_length, nullptr, uint32_t{0});
            if (length <= 0) {
              return py::bytes();
            }
            std::string target(length, '\0');
            self.extract(start, start_length, target.data(), static_cast<uint32_t>(target.size()));
            return py::bytes(target.data(), length);
          },
          py::arg("start"), py::arg("start_length"));

  us.def(
        // [1] extractBetween(int32_t start, int32_t limit, char16_t *dst, int32_t dstStart = 0)
        "extract_between",
        [](const UnicodeString &self, int32_t start, int32_t limit) {
          std::u16string result(std::max(0, std::min(self.length(), std::max(0, limit)) - std::max(0, start)), u'\0');
          self.extractBetween(start, limit, result.data());
          return result;
        },
        py::arg("start"), py::arg("limit"))
      .def(
          // [2] extractBetween(int32_t start, int32_t limit, UnicodeString &target)
          "extract_between",
          py::overload_cast<int32_t, int32_t, UnicodeString &>(&UnicodeString::extractBetween, py::const_),
          py::arg("start"), py::arg("limit"), py::arg("target"));

  us.def("fast_copy_from", &UnicodeString::fastCopyFrom, py::arg("src"));

  us.def(
        // [1] findAndReplace(const UnicodeString &oldText, const UnicodeString &newText)
        "find_and_replace",
        [](UnicodeString &self, const icupy::UnicodeStringVariant &old_text,
           const icupy::UnicodeStringVariant &new_text) -> UnicodeString & {
          return self.findAndReplace(icupy::to_unistr(old_text), icupy::to_unistr(new_text));
        },
        py::arg("old_text"), py::arg("new_text"))
      .def(
          // [2] findAndReplace(int32_t start, int32_t length, const UnicodeString &oldText,
          //                    const UnicodeString &newText)
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const icupy::UnicodeStringVariant &old_text,
             const icupy::UnicodeStringVariant &new_text) -> UnicodeString & {
            return self.findAndReplace(start, length, icupy::to_unistr(old_text), icupy::to_unistr(new_text));
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("new_text"))
      .def(
          // [3] findAndReplace(int32_t start, int32_t length, const UnicodeString &oldText, int32_t oldStart,
          //                    int32_t oldLength, const UnicodeString &newText, int32_t newStart, int32_t newLength)
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const icupy::UnicodeStringVariant &old_text,
             int32_t old_start, int32_t old_length, const icupy::UnicodeStringVariant &new_text, int32_t new_start,
             int32_t new_length) -> UnicodeString & {
            return self.findAndReplace(start, length, icupy::to_unistr(old_text), old_start, old_length,
                                       icupy::to_unistr(new_text), new_start, new_length);
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("old_start"), py::arg("old_length"),
          py::arg("new_text"), py::arg("new_start"), py::arg("new_length"));

  us.def("fold_case", &UnicodeString::foldCase, py::arg("options") = 0);

  us.def_static(
      "from_utf32",
      [](const std::u32string utf32, int32_t length) {
        auto p = reinterpret_cast<const UChar32 *>(utf32.c_str());
        return UnicodeString::fromUTF32(p, length);
      },
      py::arg("utf32"), py::arg("length"));

  us.def_static(
      "from_utf8", [](const py::bytes &utf8) { return UnicodeString::fromUTF8(StringPiece(utf8)); }, py::arg("utf8"));

  us.def(
      "get_buffer",
      [](const UnicodeString &self) -> std::optional<std::unique_ptr<_ConstChar16Ptr>> {
        auto p = self.getBuffer();
        if (p == 0) {
          return std::nullopt;
        }
        return std::make_unique<_ConstChar16Ptr>(p, self.length(), self.getCapacity());
      },
      py::keep_alive<0, 1>());

  us.def("get_capacity", &UnicodeString::getCapacity);

  us.def("get_char32_limit", &UnicodeString::getChar32Limit, py::arg("offset"));

  us.def("get_char32_start", &UnicodeString::getChar32Start, py::arg("offset"));

  us.def(
      "get_terminated_buffer",
      [](UnicodeString &self) -> std::optional<std::unique_ptr<_ConstChar16Ptr>> {
        auto p = self.getTerminatedBuffer();
        if (p == 0) {
          return std::nullopt;
        }
        return std::make_unique<_ConstChar16Ptr>(p, self.length(), self.getCapacity());
      },
      py::keep_alive<0, 1>());

  us.def("hash_code", &UnicodeString::hashCode);

  us.def(
      "has_more_char32_than",
      [](const UnicodeString &self, int32_t start, int32_t length, int32_t number) -> py::bool_ {
        return self.hasMoreChar32Than(start, length, number);
      },
      py::arg("start"), py::arg("length"), py::arg("number"));

  us.def(
        // [4] indexOf(const char16_t *srcChars, int32_t srcLength, int32_t start)
        "index_of", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
        py::arg("src_chars"), py::arg("src_length"), py::arg("start"))
      .def(
          // [5] indexOf(const char16_t *srcChars, int32_t srcStart, int32_t srcLength, int32_t start, int32_t length)
          "index_of",
          py::overload_cast<const char16_t *, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
          py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def(
          // [6] indexOf(const UnicodeString &srcText, int32_t srcStart, int32_t srcLength, int32_t start,
          //             int32_t length)
          "index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::indexOf,
                                                                                       py::const_),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def(
          // [7] indexOf(const UnicodeString &text)
          "index_of",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) {
            return self.indexOf(icupy::to_unistr(text));
          },
          py::arg("text"))
      .def(
          // [8] indexOf(const UnicodeString &text, int32_t start)
          "index_of",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text, int32_t start) {
            return self.indexOf(icupy::to_unistr(text), start);
          },
          py::arg("text"), py::arg("start"))
      .def(
          // [9] indexOf(const UnicodeString &text, int32_t start, int32_t length)
          "index_of", py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
          py::arg("text"), py::arg("start"), py::arg("length"))
      .def(
          // [10] indexOf(ConstChar16Ptr srcChars, int32_t srcLength, int32_t start, int32_t length)
          "index_of",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length, int32_t start, int32_t length) {
            return self.indexOf(src_chars, src_length, start, length);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def(
          // [11] indexOf(UChar32 c)
          "index_of", py::overload_cast<UChar32>(&UnicodeString::indexOf, py::const_), py::arg("c"))
      .def(
          // [12] indexOf(UChar32 c, int32_t start)
          "index_of", py::overload_cast<UChar32, int32_t>(&UnicodeString::indexOf, py::const_), py::arg("c"),
          py::arg("start"))
      .def(
          // [13] indexOf(UChar32 c, int32_t start, int32_t length)
          "index_of", py::overload_cast<UChar32, int32_t, int32_t>(&UnicodeString::indexOf, py::const_), py::arg("c"),
          py::arg("start"), py::arg("length"));

  us.def(
        // [2] insert(int32_t start, const char16_t *srcChars, int32_t srcStart, int32_t srcLength)
        "insert", py::overload_cast<int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::insert),
        py::arg("start"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [3] insert(int32_t start, const UnicodeString &srcText)
          "insert",
          [](UnicodeString &self, int32_t start, const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.insert(start, icupy::to_unistr(src_text));
          },
          py::arg("start"), py::arg("src_text"))
      .def(
          // [4] insert(int32_t start, const UnicodeString &srcText, int32_t srcStart, int32_t srcLength)
          "insert", py::overload_cast<int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::insert),
          py::arg("start"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [5] insert(int32_t start, ConstChar16Ptr srcChars, int32_t srcLength)
          "insert",
          [](UnicodeString &self, int32_t start, const char16_t *src_chars, int32_t src_length) -> UnicodeString & {
            return self.insert(start, src_chars, src_length);
          },
          py::arg("start"), py::arg("src_chars"), py::arg("src_length"))
      .def(
          // [6] insert(int32_t start, UChar32 srcChar)
          "insert", py::overload_cast<int32_t, UChar32>(&UnicodeString::insert), py::arg("start"), py::arg("src_char"));

  us.def("is_bogus", [](const UnicodeString &self) -> py::bool_ { return self.isBogus(); });

  us.def("is_empty", [](const UnicodeString &self) -> py::bool_ { return self.isEmpty(); });

  us.def(
        // [4] lastIndexOf(const char16_t *srcChars, int32_t srcLength, int32_t start)
        "last_index_of", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
        py::arg("src_chars"), py::arg("src_length"), py::arg("start"))
      .def(
          // [5] lastIndexOf(const char16_t *srcChars, int32_t srcStart, int32_t srcLength, int32_t start,
          //                 int32_t length)
          "last_index_of",
          py::overload_cast<const char16_t *, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::lastIndexOf,
                                                                                  py::const_),
          py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def(
          // [6] lastIndexOf(const UnicodeString &srcText, int32_t srcStart, int32_t srcLength, int32_t start,
          //                 int32_t length)
          "last_index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::lastIndexOf,
                                                                                       py::const_),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def(
          // [7] lastIndexOf(const UnicodeString &text)
          "last_index_of",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) {
            return self.lastIndexOf(icupy::to_unistr(text));
          },
          py::arg("text"))
      .def(
          // [8] lastIndexOf(const UnicodeString &text, int32_t start)
          "last_index_of",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text, int32_t start) {
            return self.lastIndexOf(icupy::to_unistr(text), start);
          },
          py::arg("text"), py::arg("start"))
      .def(
          // [9] lastIndexOf(const UnicodeString &text, int32_t start, int32_t length)
          "last_index_of",
          py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
          py::arg("text"), py::arg("start"), py::arg("length"))
      .def(
          // [10] lastIndexOf(ConstChar16Ptr srcChars, int32_t srcLength, int32_t start, int32_t length)
          "last_index_of",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length, int32_t start, int32_t length) {
            return self.lastIndexOf(src_chars, src_length, start, length);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def(
          // [11] lastIndexOf(UChar32 c)
          "last_index_of", py::overload_cast<UChar32>(&UnicodeString::lastIndexOf, py::const_), py::arg("c"))
      .def(
          // [12] lastIndexOf(UChar32 c, int32_t start)
          "last_index_of", py::overload_cast<UChar32, int32_t>(&UnicodeString::lastIndexOf, py::const_), py::arg("c"),
          py::arg("start"))
      .def(
          // [13] lastIndexOf(UChar32 c, int32_t start, int32_t length)
          "last_index_of", py::overload_cast<UChar32, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
          py::arg("c"), py::arg("start"), py::arg("length"));

  us.def("move_index32", &UnicodeString::moveIndex32, py::arg("index"), py::arg("delta"));

  us.def(
      "pad_leading",
      [](UnicodeString &self, int32_t target_length, uint16_t pad_char) -> py::bool_ {
        return self.padLeading(target_length, pad_char);
      },
      py::arg("target_length"), py::arg("pad_char") = 0x20);

  us.def(
      "pad_trailing",
      [](UnicodeString &self, int32_t target_length, uint16_t pad_char) -> py::bool_ {
        return self.padTrailing(target_length, pad_char);
      },
      py::arg("target_length"), py::arg("pad_char") = 0x20);

  us.def("remove", py::overload_cast<>(&UnicodeString::remove))
      .def("remove", py::overload_cast<int32_t, int32_t>(&UnicodeString::remove), py::arg("start"),
           py::arg("length") = INT32_MAX);

  us.def("remove_between", &UnicodeString::removeBetween, py::arg("start"), py::arg("limit") = INT32_MAX);

  us.def(
        // [2] replace(int32_t start, int32_t length, const char16_t *srcChars, int32_t srcStart, int32_t srcLength)
        "replace", py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::replace),
        py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [3] replace(int32_t start, int32_t length, const UnicodeString &srcText)
          "replace",
          [](UnicodeString &self, int32_t start, int32_t length, const icupy::UnicodeStringVariant &src_text)
              -> UnicodeString & { return self.replace(start, length, icupy::to_unistr(src_text)); },
          py::arg("start"), py::arg("length"), py::arg("src_text"))
      .def(
          // [4] replace(int32_t start, int32_t length, const UnicodeString &srcText, int32_t srcStart,
          //             int32_t srcLength)
          "replace",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::replace),
          py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [5] replace(int32_t start, int32_t length, ConstChar16Ptr srcChars, int32_t srcLength)
          "replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *src_chars,
             int32_t src_length) -> UnicodeString & { return self.replace(start, length, src_chars, src_length); },
          py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_length"))
      .def(
          // [6] replace(int32_t start, int32_t length, UChar32 srcChar)
          "replace", py::overload_cast<int32_t, int32_t, UChar32>(&UnicodeString::replace), py::arg("start"),
          py::arg("length"), py::arg("src_char"));

  us.def(
        // [1] replaceBetween(int32_t start, int32_t limit, const UnicodeString &srcText)
        "replace_between",
        [](UnicodeString &self, int32_t start, int32_t limit, const icupy::UnicodeStringVariant &src_text)
            -> UnicodeString & { return self.replaceBetween(start, limit, icupy::to_unistr(src_text)); },
        py::arg("start"), py::arg("limit"), py::arg("src_text"))
      .def(
          // [2] replaceBetween(int32_t start, int32_t limit, const UnicodeString &srcText, int32_t srcStart,
          //                    int32_t srcLimit)
          "replace_between",
          [](UnicodeString &self, int32_t start, int32_t limit, const icupy::UnicodeStringVariant &src_text,
             int32_t src_start, int32_t src_limit) -> UnicodeString & {
            return self.replaceBetween(start, limit, icupy::to_unistr(src_text), src_start, src_limit);
          },
          py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"));

  us.def("retain_between", &UnicodeString::retainBetween, py::arg("start"), py::arg("limit") = INT32_MAX);

  us.def("reverse", py::overload_cast<int32_t, int32_t>(&UnicodeString::reverse), py::arg("start"), py::arg("length"))
      .def("reverse", py::overload_cast<>(&UnicodeString::reverse));

  us.def(
      "set_char_at",
      [](UnicodeString &self, int32_t offset, uint16_t ch) -> UnicodeString & { return self.setCharAt(offset, ch); },
      py::arg("offset"), py::arg("ch"));

  us.def(
        // [3] setTo(const char16_t *srcChars, int32_t srcLength)
        "set_to", py::overload_cast<const char16_t *, int32_t>(&UnicodeString::setTo), py::arg("src_chars"),
        py::arg("src_length"))
      .def(
          // [4] setTo(const UnicodeString &srcText)
          "set_to",
          [](UnicodeString &self, const icupy::UnicodeStringVariant &src_text) -> UnicodeString & {
            return self.setTo(icupy::to_unistr(src_text));
          },
          py::arg("src_text"))
      .def(
          // [5] setTo(const UnicodeString &srcText, int32_t srcStart)
          "set_to", py::overload_cast<const UnicodeString &, int32_t>(&UnicodeString::setTo), py::arg("src_text"),
          py::arg("src_start"))
      .def(
          // [6] setTo(const UnicodeString &srcText, int32_t srcStart, int32_t srcLength)
          "set_to", py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::setTo),
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      // .def(
      //     // [7] setTo(UBool isTerminated, ConstChar16Ptr text, int32_t textLength)
      //     "set_to",
      //     [](UnicodeString &self, UBool is_terminated, const char16_t *text, int32_t text_length) -> UnicodeString &
      //     {
      //       return self.setTo(is_terminated, text, text_length);
      //     },
      //     py::arg("is_terminated"), py::arg("text"), py::arg("text_length"))
      .def(
          // [8] setTo(UChar32 srcChar)
          "set_to", py::overload_cast<UChar32>(&UnicodeString::setTo), py::arg("src_char"));

  us.def("set_to_bogus", &UnicodeString::setToBogus);

  us.def(
        // [1] startsWith(const char16_t *srcChars, int32_t srcStart, int32_t srcLength)
        "starts_with",
        [](const UnicodeString &self, const char16_t *src_chars, int32_t src_start, int32_t src_length) -> py::bool_ {
          return self.startsWith(src_chars, src_start, src_length);
        },
        py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [2] startsWith(const UnicodeString &srcText, int32_t srcStart, int32_t srcLength)
          "starts_with",
          [](const UnicodeString &self, const UnicodeString &src_text, int32_t src_start,
             int32_t src_length) -> py::bool_ { return self.startsWith(src_text, src_start, src_length); },
          py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def(
          // [3] startsWith(const UnicodeString &text)
          "starts_with",
          [](const UnicodeString &self, const icupy::UnicodeStringVariant &text) -> py::bool_ {
            return self.startsWith(icupy::to_unistr(text));
          },
          py::arg("text"))
      .def(
          // [4] startsWith(ConstChar16Ptr srcChars, int32_t srcLength)
          "starts_with",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) -> py::bool_ {
            return self.startsWith(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"));

#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  us.def("swap", &UnicodeString::swap, py::arg("other"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)

  us.def("temp_sub_string", &UnicodeString::tempSubString, py::arg("start") = 0, py::arg("length") = INT32_MAX);

  us.def("temp_sub_string_between", &UnicodeString::tempSubStringBetween, py::arg("start") = 0,
         py::arg("limit") = INT32_MAX);

  us.def(
        "to_lower",
        [](UnicodeString &self, const icupy::LocaleVariant &locale) -> UnicodeString & {
          return self.toLower(icupy::to_locale(locale));
        },
        py::arg("locale"))
      .def("to_lower", py::overload_cast<>(&UnicodeString::toLower));

  us.def("to_title", py::overload_cast<BreakIterator *>(&UnicodeString::toTitle), py::arg("title_iter"))
      .def(
          "to_title",
          [](UnicodeString &self, BreakIterator *title_iter, const icupy::LocaleVariant &locale) -> UnicodeString & {
            return self.toTitle(title_iter, icupy::to_locale(locale));
          },
          py::arg("title_iter"), py::arg("locale"))
      .def(
          "to_title",
          [](UnicodeString &self, BreakIterator *title_iter, const icupy::LocaleVariant &locale, uint32_t options)
              -> UnicodeString & { return self.toTitle(title_iter, icupy::to_locale(locale), options); },
          py::arg("title_iter"), py::arg("locale"), py::arg("options"));

  us.def(
        "to_upper",
        [](UnicodeString &self, const icupy::LocaleVariant &locale) -> UnicodeString & {
          return self.toUpper(icupy::to_locale(locale));
        },
        py::arg("locale"))
      .def("to_upper", py::overload_cast<>(&UnicodeString::toUpper));

  us.def("to_utf32", [](const UnicodeString &self) {
    ErrorCode error_code;
    const auto length = self.toUTF32(nullptr, 0, error_code);
    std::vector<UChar32> dest(length);
    error_code.reset();
    self.toUTF32(dest.data(), length, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::u32string(dest.begin(), dest.end());
  });

  us.def("to_utf8", [](const UnicodeString &self) {
    std::string result;
    auto sink = StringByteSink(&result);
    self.toUTF8(sink);
    return py::bytes(result.c_str(), result.size());
  });

  us.def("to_utf8_string", [](const UnicodeString &self) {
    std::string result;
    self.toUTF8String(result);
    return result;
  });

  us.def("trim", &UnicodeString::trim);

  us.def(
      "truncate", [](UnicodeString &self, int32_t target_length) -> py::bool_ { return self.truncate(target_length); },
      py::arg("target_length"));

  us.def("unescape", &UnicodeString::unescape);

  us.def("unescape_at", &UnicodeString::unescapeAt, py::arg("offset"));

  us.def_property_readonly_static("SENTINEL", [](const py::object &) { return U_SENTINEL; });

  us.def_property_readonly_static("COMPARE_CODE_POINT_ORDER",
                                  [](const py::object &) { return U_COMPARE_CODE_POINT_ORDER; });

  us.def_property_readonly_static("FOLD_CASE_DEFAULT", [](const py::object &) { return U_FOLD_CASE_DEFAULT; });

  us.def_property_readonly_static("FOLD_CASE_EXCLUDE_SPECIAL_I",
                                  [](const py::object &) { return U_FOLD_CASE_EXCLUDE_SPECIAL_I; });

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static("TITLECASE_ADJUST_TO_CASED",
                                  [](const py::object &) { return U_TITLECASE_ADJUST_TO_CASED; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  us.def_property_readonly_static("TITLECASE_NO_BREAK_ADJUSTMENT",
                                  [](const py::object &) { return U_TITLECASE_NO_BREAK_ADJUSTMENT; });

  us.def_property_readonly_static("TITLECASE_NO_LOWERCASE",
                                  [](const py::object &) { return U_TITLECASE_NO_LOWERCASE; });

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static("TITLECASE_SENTENCES", [](const py::object &) { return U_TITLECASE_SENTENCES; });

  us.def_property_readonly_static("TITLECASE_WHOLE_STRING",
                                  [](const py::object &) { return U_TITLECASE_WHOLE_STRING; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  m.attr("US_INV") = US_INV;
}

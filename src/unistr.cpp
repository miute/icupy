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
  // icu::Replaceable
  rep.def("char32_at", &Replaceable::char32At, py::arg("offset"));
  rep.def(
      "char_at", [](const Replaceable &self, int32_t offset) -> uint16_t { return self.charAt(offset); },
      py::arg("offset"));
  rep.def("length", &Replaceable::length);

  // icu::UnicodeString
  py::enum_<UnicodeString::EInvariant>(
      us, "EInvariant", py::arithmetic(),
      "Constant to be used in the *UnicodeString(char *, int32_t, EInvariant)* constructor which constructs a Unicode "
      "string from an invariant-character char \\* string.\n\n"
      "Use the macro *US_INV* instead of the full qualification for this value.")
      .value("INVARIANT", UnicodeString::kInvariant)
      .export_values();

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
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<UnicodeString>(src, src_length, cnv, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("src"), py::arg("src_length"), py::arg("cnv"))
      .def(py::init<const char *, int32_t, UnicodeString::EInvariant>(), py::arg("src"), py::arg("text_length"),
           py::arg("inv"))
      .def(py::init<const UnicodeString &>(), py::arg("that"))
      .def(py::init<const UnicodeString &, int32_t>(), py::arg("src"), py::arg("src_start"))
      .def(py::init<const UnicodeString &, int32_t, int32_t>(), py::arg("src"), py::arg("src_start"),
           py::arg("src_length"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self < py::self, py::arg("other"))
      .def(py::self <= py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"))
      .def(py::self > py::self, py::arg("other"))
      .def(py::self >= py::self, py::arg("other"))
      .def(py::self += py::self, py::arg("other"))
      .def(py::self + py::self, py::arg("other"));
  us.def(
        "__add__", [](UnicodeString &self, const char16_t *other) { return self + other; }, py::is_operator(),
        py::arg("other"))
      .def(
          "__contains__",
          [](const UnicodeString &self, const char16_t *item) { return self.indexOf(item, u_strlen(item), 0) >= 0; },
          py::arg("item"))
      .def(
          "__contains__", [](const UnicodeString &self, const UnicodeString &item) { return self.indexOf(item) >= 0; },
          py::arg("item"))
      .def("__copy__", &UnicodeString::clone)
      .def(
          "__deepcopy__", [](const UnicodeString &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const UnicodeString &self, const char16_t *other) { return self == other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__ge__", [](const UnicodeString &self, const char16_t *other) { return self >= other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__getitem__",
          [](const UnicodeString &self, int32_t index) {
            if (index < 0) {
              index += self.length();
            }
            if (index < 0 || index >= self.length()) {
              throw py::index_error(std::to_string(index));
            }
            return self[index];
          },
          py::arg("index"))
      .def(
          "__getitem__",
          [](const UnicodeString &self, py::slice slice) {
            size_t start, stop, step, slice_length;
            if (!slice.compute(self.length(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }
            std::u16string result(slice_length, u'\0');
            for (size_t n = 0; n < slice_length; ++n) {
              result[n] = self[static_cast<int32_t>(start)];
              start += step;
            }
            return result;
          },
          py::arg("slice"))
      .def(
          "__gt__", [](const UnicodeString &self, const char16_t *other) { return self > other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__iadd__", [](UnicodeString &self, const char16_t *other) { return self += other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__iadd__", [](UnicodeString &self, UChar32 other) { return self += other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__le__", [](const UnicodeString &self, const char16_t *other) { return self <= other; }, py::is_operator(),
          py::arg("other"))
      .def("__len__", [](const UnicodeString &self) { return self.length(); })
      .def(
          "__lt__", [](const UnicodeString &self, const char16_t *other) { return self < other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__ne__", [](const UnicodeString &self, const char16_t *other) { return self != other; }, py::is_operator(),
          py::arg("other"))
      .def("__repr__",
           [](const UnicodeString &self) {
             std::stringstream ss;
             ss << "UnicodeString('";
             auto it = UCharCharacterIterator(self.getBuffer(), self.length());
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
             ss << "')";
             return ss.str();
           })
      .def("__str__", [](const UnicodeString &self) {
        std::string result;
        self.toUTF8String(result);
        return result;
      });
  us.def(
        // const char16_t *src_text -> const UnicodeString &src_text
        "append",
        [](UnicodeString &self, const char16_t *src_text) -> UnicodeString & { return self.append(src_text); },
        py::arg("src_text"))
      .def("append", py::overload_cast<char16_t>(&UnicodeString::append), py::arg("src_char"))
      .def("append", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::append),
           py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def("append", py::overload_cast<const UnicodeString &>(&UnicodeString::append), py::arg("src_text"))
      .def("append", py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::append),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      /*
      .def("append", py::overload_cast<ConstChar16Ptr, int32_t>(&UnicodeString::append), py::arg("src_chars"),
           py::arg("src_length"))
      */
      .def(
          // UnicodeString & append(ConstChar16Ptr srcChars, int32_t srcLength)
          "append",
          [](UnicodeString &self, const char16_t *src_chars, int32_t src_length) -> UnicodeString & {
            return self.append(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"))
      .def("append", py::overload_cast<UChar32>(&UnicodeString::append), py::arg("src_char"));
  us.def("case_compare", py::overload_cast<const UnicodeString &, uint32_t>(&UnicodeString::caseCompare, py::const_),
         py::arg("text"), py::arg("options"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "case_compare",
          [](const UnicodeString &self, const char16_t *text, uint32_t options) {
            return self.caseCompare(text, options);
          },
          py::arg("text"), py::arg("options"))
      /*
      .def("case_compare",
           py::overload_cast<ConstChar16Ptr, int32_t, uint32_t>(&UnicodeString::caseCompare, py::const_),
           py::arg("src_chars"), py::arg("src_length"), py::arg("options"))
      */
      .def(
          // int8_t caseCompare(ConstChar16Ptr srcChars, int32_t srcLength, uint32_t options)
          "case_compare",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length, uint32_t options) {
            return self.caseCompare(src_chars, src_length, options);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("options"))
      .def("case_compare",
           py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t, uint32_t>(
               &UnicodeString::caseCompare, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"),
           py::arg("options"))
      .def("case_compare",
           py::overload_cast<int32_t, int32_t, const char16_t *, uint32_t>(&UnicodeString::caseCompare, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("options"))
      .def("case_compare",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t, uint32_t>(
               &UnicodeString::caseCompare, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"),
           py::arg("options"))
      .def(
          "case_compare",
          py::overload_cast<int32_t, int32_t, const UnicodeString &, uint32_t>(&UnicodeString::caseCompare, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("options"))
      .def("case_compare_between", &UnicodeString::caseCompareBetween, py::arg("start"), py::arg("limit"),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"), py::arg("options"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "case_compare_between",
          [](const UnicodeString &self, int32_t start, int32_t limit, const char16_t *src_text, int32_t src_start,
             int32_t src_limit, uint32_t options) {
            return self.caseCompareBetween(start, limit, src_text, src_start, src_limit, options);
          },
          py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"),
          py::arg("options"));
  us.def("clone", &UnicodeString::clone);
  us.def("compare", py::overload_cast<const UnicodeString &>(&UnicodeString::compare, py::const_), py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "compare", [](const UnicodeString &self, const char16_t *text) { return self.compare(text); },
          py::arg("text"))
      /*
      .def("compare", py::overload_cast<ConstChar16Ptr, int32_t>(&UnicodeString::compare, py::const_),
           py::arg("src_chars"), py::arg("src_length"))
      */
      .def(
          // int8_t compare(ConstChar16Ptr srcChars, int32_t srcLength)
          "compare",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) {
            return self.compare(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"))
      .def("compare", py::overload_cast<int32_t, int32_t, const char16_t *>(&UnicodeString::compare, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_chars"))
      .def("compare",
           py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::compare, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def("compare",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::compare,
                                                                                        py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def("compare", py::overload_cast<int32_t, int32_t, const UnicodeString &>(&UnicodeString::compare, py::const_),
           py::arg("start"), py::arg("length"), py::arg("text"))
      .def("compare_between", &UnicodeString::compareBetween, py::arg("start"), py::arg("limit"), py::arg("src_text"),
           py::arg("src_start"), py::arg("src_limit"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "compare_between",
          [](const UnicodeString &self, int32_t start, int32_t limit, const char16_t *src_text, int32_t src_start,
             int32_t src_limit) { return self.compareBetween(start, limit, src_text, src_start, src_limit); },
          py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"));
  us.def("compare_code_point_order",
         py::overload_cast<const UnicodeString &>(&UnicodeString::compareCodePointOrder, py::const_), py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "compare_code_point_order",
          [](const UnicodeString &self, const char16_t *text) { return self.compareCodePointOrder(text); },
          py::arg("text"))
      /*
      .def("compare_code_point_order",
           py::overload_cast<ConstChar16Ptr, int32_t>(&UnicodeString::compareCodePointOrder, py::const_),
           py::arg("src_chars"), py::arg("src_length"))
      */
      .def(
          // int8_t compareCodePointOrder(ConstChar16Ptr srcChars, int32_t srcLength)
          "compare_code_point_order",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) {
            return self.compareCodePointOrder(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"))
      .def("compare_code_point_order",
           py::overload_cast<int32_t, int32_t, const char16_t *>(&UnicodeString::compareCodePointOrder, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_chars"))
      .def("compare_code_point_order",
           py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t>(
               &UnicodeString::compareCodePointOrder, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def(
          "compare_code_point_order",
          py::overload_cast<int32_t, int32_t, const UnicodeString &>(&UnicodeString::compareCodePointOrder, py::const_),
          py::arg("start"), py::arg("length"), py::arg("src_text"))
      .def("compare_code_point_order",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(
               &UnicodeString::compareCodePointOrder, py::const_),
           py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def("compare_code_point_order_between", &UnicodeString::compareCodePointOrderBetween, py::arg("start"),
           py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "compare_code_point_order_between",
          [](const UnicodeString &self, int32_t start, int32_t limit, const char16_t *src_text, int32_t src_start,
             int32_t src_limit) {
            return self.compareCodePointOrderBetween(start, limit, src_text, src_start, src_limit);
          },
          py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"));
  us.def("copy", &UnicodeString::copy, py::arg("start"), py::arg("limit"), py::arg("dest"));
  us.def("count_char32", &UnicodeString::countChar32, py::arg("start ") = 0, py::arg("length") = INT32_MAX);
  us.def("ends_with", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::endsWith, py::const_),
         py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def("ends_with",
           py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::endsWith, py::const_),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def("ends_with", py::overload_cast<const UnicodeString &>(&UnicodeString::endsWith, py::const_),
           py::arg("src_text"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "ends_with", [](const UnicodeString &self, const char16_t *src_text) { return self.endsWith(src_text); },
          py::arg("src_text"))
      /*
      .def("ends_with", py::overload_cast<ConstChar16Ptr, int32_t>(&UnicodeString::endsWith, py::const_),
           py::arg("src_chars"), py::arg("src_length"))
      */
      .def(
          // UBool endsWith(ConstChar16Ptr srcChars, int32_t srcLength)
          "ends_with",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) {
            return self.endsWith(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"));
  us.def(
        // int32_t extract(char *dest, int32_t destCapacity, UConverter *cnv, UErrorCode &errorCode)
        "extract",
        [](const UnicodeString &self, _UConverterPtr &cnv) {
          UErrorCode error_code = U_ZERO_ERROR;
          const auto length = self.extract(nullptr, 0, cnv, error_code);
          std::string dest(length, '\0');
          error_code = U_ZERO_ERROR;
          self.extract(dest.data(), static_cast<int32_t>(dest.size()), cnv, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return py::bytes(dest.data(), length);
        },
        py::arg("cnv"))
      .def(
          // int32_t extract(Char16Ptr dest, int32_t destCapacity, UErrorCode &errorCode)
          "extract",
          [](const UnicodeString &self) {
            std::u16string result(self.length(), u'\0');
            UErrorCode error_code = U_ZERO_ERROR;
            self.extract(result.data(), static_cast<int32_t>(result.size()), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          })
      .def("extract", py::overload_cast<int32_t, int32_t, UnicodeString &>(&UnicodeString::extract, py::const_),
           py::arg("start"), py::arg("length"), py::arg("target"))
      .def(
          // int32_t extract(int32_t start, int32_t startLength, char *target, const char *codepage = 0)
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
          // int32_t extract(int32_t start, int32_t startLength, char *target, int32_t targetCapacity,
          //                 enum EInvariant inv)
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
          // int32_t extract(int32_t start, int32_t startLength, char *target, uint32_t targetLength)
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
        // void extractBetween(int32_t start, int32_t limit, char16_t *dst, int32_t dstStart = 0)
        "extract_between",
        [](const UnicodeString &self, int32_t start, int32_t limit) {
          std::u16string result(self.length(), u'\0');
          self.extractBetween(start, limit, result.data());
          auto pos = result.find_last_not_of(u'\0');
          result.erase(pos == std::u16string::npos ? 0 : pos + 1);
          return result;
        },
        py::arg("start"), py::arg("limit"))
      .def("extract_between",
           py::overload_cast<int32_t, int32_t, UnicodeString &>(&UnicodeString::extractBetween, py::const_),
           py::arg("start"), py::arg("limit"), py::arg("target"));
  us.def("fast_copy_from", &UnicodeString::fastCopyFrom, py::arg("src"));
  us.def("find_and_replace",
         py::overload_cast<const UnicodeString &, const UnicodeString &>(&UnicodeString::findAndReplace),
         py::arg("old_text"), py::arg("new_text"))
      .def(
          // const char16_t *old_text -> const UnicodeString &old_text
          "find_and_replace",
          [](UnicodeString &self, const char16_t *old_text, const UnicodeString &new_text) -> UnicodeString & {
            return self.findAndReplace(old_text, new_text);
          },
          py::arg("old_text"), py::arg("new_text"))
      .def(
          // const char16_t *new_text -> const UnicodeString &new_text
          "find_and_replace",
          [](UnicodeString &self, const UnicodeString &old_text, const char16_t *new_text) -> UnicodeString & {
            return self.findAndReplace(old_text, new_text);
          },
          py::arg("old_text"), py::arg("new_text"))
      .def(
          // const char16_t *old_text -> const UnicodeString &old_text
          // const char16_t *new_text -> const UnicodeString &new_text
          "find_and_replace",
          [](UnicodeString &self, const char16_t *old_text, const char16_t *new_text) -> UnicodeString & {
            return self.findAndReplace(old_text, new_text);
          },
          py::arg("old_text"), py::arg("new_text"))
      .def("find_and_replace",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, const UnicodeString &>(
               &UnicodeString::findAndReplace),
           py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("new_text"))
      .def(
          // const char16_t *old_text -> const UnicodeString &old_text
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *old_text,
             const UnicodeString &new_text) -> UnicodeString & {
            return self.findAndReplace(start, length, old_text, new_text);
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("new_text"))
      .def(
          // const char16_t *new_text -> const UnicodeString &new_text
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const UnicodeString &old_text,
             const char16_t *new_text) -> UnicodeString & {
            return self.findAndReplace(start, length, old_text, new_text);
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("new_text"))
      .def(
          // const char16_t *old_text -> const UnicodeString &old_text
          // const char16_t *new_text -> const UnicodeString &new_text
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *old_text, const char16_t *new_text)
              -> UnicodeString & { return self.findAndReplace(start, length, old_text, new_text); },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("new_text"))
      .def("find_and_replace",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t, const UnicodeString &, int32_t,
                             int32_t>(&UnicodeString::findAndReplace),
           py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("old_start"), py::arg("old_length"),
           py::arg("new_text"), py::arg("new_start"), py::arg("new_length"))
      .def(
          // const char16_t *old_text -> const UnicodeString &old_text
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *old_text, int32_t old_start,
             int32_t old_length, const UnicodeString &new_text, int32_t new_start,
             int32_t new_length) -> UnicodeString & {
            return self.findAndReplace(start, length, old_text, old_start, old_length, new_text, new_start, new_length);
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("old_start"), py::arg("old_length"),
          py::arg("new_text"), py::arg("new_start"), py::arg("new_length"))
      .def(
          // const char16_t *new_text -> const UnicodeString &new_text
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const UnicodeString &old_text, int32_t old_start,
             int32_t old_length, const char16_t *new_text, int32_t new_start, int32_t new_length) -> UnicodeString & {
            return self.findAndReplace(start, length, old_text, old_start, old_length, new_text, new_start, new_length);
          },
          py::arg("start"), py::arg("length"), py::arg("old_text"), py::arg("old_start"), py::arg("old_length"),
          py::arg("new_text"), py::arg("new_start"), py::arg("new_length"))
      .def(
          // const char16_t *old_text -> const UnicodeString &old_text
          // const char16_t *new_text -> const UnicodeString &new_text
          "find_and_replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *old_text, int32_t old_start,
             int32_t old_length, const char16_t *new_text, int32_t new_start, int32_t new_length) -> UnicodeString & {
            return self.findAndReplace(start, length, old_text, old_start, old_length, new_text, new_start, new_length);
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
      "from_utf8", [](const char *utf8) { return UnicodeString::fromUTF8(StringPiece(utf8)); }, py::arg("utf8"));
  us.def(
      "get_buffer",
      [](const UnicodeString &self) -> std::optional<std::unique_ptr<_ConstChar16Ptr>> {
        auto p = self.getBuffer();
        if (p == 0) {
          return std::nullopt;
        }
        return std::make_unique<_ConstChar16Ptr>(p);
      },
      py::return_value_policy::reference);
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
        return std::make_unique<_ConstChar16Ptr>(p);
      },
      py::return_value_policy::reference);
  us.def("hash_code", &UnicodeString::hashCode);
  us.def("has_more_char32_than", &UnicodeString::hasMoreChar32Than, py::arg("start"), py::arg("length"),
         py::arg("number"));
  us.def("index_of", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
         py::arg("src_chars"), py::arg("src_length"), py::arg("start"))
      .def("index_of",
           py::overload_cast<const char16_t *, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
           py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def("index_of",
           py::overload_cast<const UnicodeString &, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::indexOf,
                                                                                        py::const_),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def("index_of", py::overload_cast<const UnicodeString &>(&UnicodeString::indexOf, py::const_), py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "index_of", [](const UnicodeString &self, const char16_t *text) { return self.indexOf(text); },
          py::arg("text"))
      .def("index_of", py::overload_cast<const UnicodeString &, int32_t>(&UnicodeString::indexOf, py::const_),
           py::arg("text"), py::arg("start"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "index_of",
          [](const UnicodeString &self, const char16_t *text, int32_t start) { return self.indexOf(text, start); },
          py::arg("text"), py::arg("start"))
      .def("index_of", py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
           py::arg("text"), py::arg("start"), py::arg("length"))
      /*
      .def("index_of",
           py::overload_cast<ConstChar16Ptr, int32_t, int32_t, int32_t>(&UnicodeString::indexOf, py::const_),
           py::arg("src_chars"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      */
      .def(
          // int32_t indexOf(ConstChar16Ptr srcChars, int32_t srcLength, int32_t start, int32_t length)
          "index_of",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length, int32_t start, int32_t length) {
            return self.indexOf(src_chars, src_length, start, length);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def("index_of", py::overload_cast<UChar32>(&UnicodeString::indexOf, py::const_), py::arg("c"))
      .def("index_of", py::overload_cast<UChar32, int32_t>(&UnicodeString::indexOf, py::const_), py::arg("c"),
           py::arg("start"))
      .def("index_of", py::overload_cast<UChar32, int32_t, int32_t>(&UnicodeString::indexOf, py::const_), py::arg("c"),
           py::arg("start"), py::arg("length"));
  us.def(
        // const char16_t *src_text -> const UnicodeString &src_text
        "insert",
        [](UnicodeString &self, int32_t start, const char16_t *src_text) -> UnicodeString & {
          return self.insert(start, src_text);
        },
        py::arg("start"), py::arg("src_text"))
      .def("insert", py::overload_cast<int32_t, char16_t>(&UnicodeString::insert), py::arg("start"),
           py::arg("src_char"))
      .def("insert", py::overload_cast<int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::insert),
           py::arg("start"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def("insert", py::overload_cast<int32_t, const UnicodeString &>(&UnicodeString::insert), py::arg("start"),
           py::arg("src_text"))
      .def("insert", py::overload_cast<int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::insert),
           py::arg("start"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      /*
      .def("insert", py::overload_cast<int32_t, ConstChar16Ptr, int32_t>(&UnicodeString::insert), py::arg("start"),
           py::arg("src_chars"), py::arg("src_length"))
      */
      .def(
          // UnicodeString & insert(int32_t start, ConstChar16Ptr srcChars, int32_t srcLength)
          "insert",
          [](UnicodeString &self, int32_t start, const char16_t *src_chars, int32_t src_length) -> UnicodeString & {
            return self.insert(start, src_chars, src_length);
          },
          py::arg("start"), py::arg("src_chars"), py::arg("src_length"))
      .def("insert", py::overload_cast<int32_t, UChar32>(&UnicodeString::insert), py::arg("start"),
           py::arg("src_char"));
  us.def("is_bogus", &UnicodeString::isBogus);
  us.def("is_empty", &UnicodeString::isEmpty);
  us.def("last_index_of",
         py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
         py::arg("src_chars"), py::arg("src_length"), py::arg("start"))
      .def("last_index_of",
           py::overload_cast<const char16_t *, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::lastIndexOf,
                                                                                   py::const_),
           py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def("last_index_of",
           py::overload_cast<const UnicodeString &, int32_t, int32_t, int32_t, int32_t>(&UnicodeString::lastIndexOf,
                                                                                        py::const_),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def("last_index_of", py::overload_cast<const UnicodeString &>(&UnicodeString::lastIndexOf, py::const_),
           py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "last_index_of", [](const UnicodeString &self, const char16_t *text) { return self.lastIndexOf(text); },
          py::arg("text"))
      .def("last_index_of", py::overload_cast<const UnicodeString &, int32_t>(&UnicodeString::lastIndexOf, py::const_),
           py::arg("text"), py::arg("start"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "last_index_of",
          [](const UnicodeString &self, const char16_t *text, int32_t start) { return self.lastIndexOf(text, start); },
          py::arg("text"), py::arg("start"))
      .def("last_index_of",
           py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
           py::arg("text"), py::arg("start"), py::arg("length"))
      /*
      .def("last_index_of",
           py::overload_cast<ConstChar16Ptr, int32_t, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
           py::arg("src_chars"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      */
      .def(
          // int32_t lastIndexOf(ConstChar16Ptr srcChars, int32_t srcLength, int32_t start, int32_t length)
          "last_index_of",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length, int32_t start, int32_t length) {
            return self.lastIndexOf(src_chars, src_length, start, length);
          },
          py::arg("src_chars"), py::arg("src_length"), py::arg("start"), py::arg("length"))
      .def("last_index_of", py::overload_cast<UChar32>(&UnicodeString::lastIndexOf, py::const_), py::arg("c"))
      .def("last_index_of", py::overload_cast<UChar32, int32_t>(&UnicodeString::lastIndexOf, py::const_), py::arg("c"),
           py::arg("start"))
      .def("last_index_of", py::overload_cast<UChar32, int32_t, int32_t>(&UnicodeString::lastIndexOf, py::const_),
           py::arg("c"), py::arg("start"), py::arg("length"));
  us.def("move_index32", &UnicodeString::moveIndex32, py::arg("index"), py::arg("delta"));
  us.def(
      "pad_leading",
      [](UnicodeString &self, int32_t target_length, uint16_t pad_char) {
        return self.padLeading(target_length, pad_char);
      },
      py::arg("target_length"), py::arg("pad_char") = 0x20);
  us.def(
      "pad_trailing",
      [](UnicodeString &self, int32_t target_length, uint16_t pad_char) {
        return self.padTrailing(target_length, pad_char);
      },
      py::arg("target_length"), py::arg("pad_char") = 0x20);
  us.def("remove", py::overload_cast<>(&UnicodeString::remove))
      .def("remove", py::overload_cast<int32_t, int32_t>(&UnicodeString::remove), py::arg("start"),
           py::arg("length") = INT32_MAX)
      .def("remove_between", &UnicodeString::removeBetween, py::arg("start"), py::arg("limit") = INT32_MAX);
  us.def("replace", py::overload_cast<int32_t, int32_t, const char16_t *, int32_t, int32_t>(&UnicodeString::replace),
         py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def("replace", py::overload_cast<int32_t, int32_t, const UnicodeString &>(&UnicodeString::replace),
           py::arg("start"), py::arg("length"), py::arg("src_text"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *src_text) -> UnicodeString & {
            return self.replace(start, length, src_text);
          },
          py::arg("start"), py::arg("length"), py::arg("src_text"))
      .def("replace",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::replace),
           py::arg("start"), py::arg("length"), py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      /*
      .def("replace", py::overload_cast<int32_t, int32_t, ConstChar16Ptr, int32_t>(&UnicodeString::replace),
           py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_length"))
      */
      .def(
          // UnicodeString & replace(int32_t start, int32_t length, ConstChar16Ptr srcChars, int32_t srcLength)
          "replace",
          [](UnicodeString &self, int32_t start, int32_t length, const char16_t *src_chars,
             int32_t src_length) -> UnicodeString & { return self.replace(start, length, src_chars, src_length); },
          py::arg("start"), py::arg("length"), py::arg("src_chars"), py::arg("src_length"))
      .def("replace", py::overload_cast<int32_t, int32_t, UChar32>(&UnicodeString::replace), py::arg("start"),
           py::arg("length"), py::arg("src_char"))
      .def("replace_between",
           py::overload_cast<int32_t, int32_t, const UnicodeString &>(&UnicodeString::replaceBetween), py::arg("start"),
           py::arg("limit"), py::arg("src_text"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "replace_between",
          [](UnicodeString &self, int32_t start, int32_t limit, const char16_t *src_text) -> UnicodeString & {
            return self.replaceBetween(start, limit, src_text);
          },
          py::arg("start"), py::arg("limit"), py::arg("src_text"))
      .def("replace_between",
           py::overload_cast<int32_t, int32_t, const UnicodeString &, int32_t, int32_t>(&UnicodeString::replaceBetween),
           py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "replace_between",
          [](UnicodeString &self, int32_t start, int32_t limit, const char16_t *src_text, int32_t src_start,
             int32_t src_limit) -> UnicodeString & {
            return self.replaceBetween(start, limit, src_text, src_start, src_limit);
          },
          py::arg("start"), py::arg("limit"), py::arg("src_text"), py::arg("src_start"), py::arg("src_limit"));
  us.def("retain_between", &UnicodeString::retainBetween, py::arg("start"), py::arg("limit") = INT32_MAX);
  us.def("reverse", py::overload_cast<int32_t, int32_t>(&UnicodeString::reverse), py::arg("start"), py::arg("length"))
      .def("reverse", py::overload_cast<>(&UnicodeString::reverse));
  us.def(
      "set_char_at",
      [](UnicodeString &self, int32_t offset, uint16_t ch) -> UnicodeString & { return self.setCharAt(offset, ch); },
      py::arg("offset"), py::arg("ch"));
  us.def("set_to", py::overload_cast<const char16_t *, int32_t>(&UnicodeString::setTo), py::arg("src_chars"),
         py::arg("src_length"))
      .def("set_to", py::overload_cast<const UnicodeString &>(&UnicodeString::setTo), py::arg("src_text"))
      .def(
          // const char16_t *src_text -> const UnicodeString &src_text
          "set_to",
          [](UnicodeString &self, const char16_t *src_text) -> UnicodeString & { return self.setTo(src_text); },
          py::arg("src_text"))
      .def("set_to", py::overload_cast<const UnicodeString &, int32_t>(&UnicodeString::setTo), py::arg("src_text"),
           py::arg("src_start"))
      .def("set_to", py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::setTo),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      /*
      .def("set_to", py::overload_cast<UBool, ConstChar16Ptr, int32_t>(&UnicodeString::setTo), py::arg("is_terminated"),
           py::arg("text"), py::arg("text_length"))
      */
      .def(
          // UnicodeString& setTo(UBool isTerminated, ConstChar16Ptr text, int32_t textLength)
          "set_to",
          [](UnicodeString &self, UBool is_terminated, const char16_t *text, int32_t text_length) -> UnicodeString & {
            return self.setTo(is_terminated, text, text_length);
          },
          py::arg("is_terminated"), py::arg("text"), py::arg("text_length"))
      .def("set_to", py::overload_cast<UChar32>(&UnicodeString::setTo), py::arg("src_char"));
  us.def("set_to_bogus", &UnicodeString::setToBogus);
  us.def("starts_with", py::overload_cast<const char16_t *, int32_t, int32_t>(&UnicodeString::startsWith, py::const_),
         py::arg("src_chars"), py::arg("src_start"), py::arg("src_length"))
      .def("starts_with",
           py::overload_cast<const UnicodeString &, int32_t, int32_t>(&UnicodeString::startsWith, py::const_),
           py::arg("src_text"), py::arg("src_start"), py::arg("src_length"))
      .def("starts_with", py::overload_cast<const UnicodeString &>(&UnicodeString::startsWith, py::const_),
           py::arg("text"))
      .def(
          // const char16_t *text -> const UnicodeString &text
          "starts_with", [](const UnicodeString &self, const char16_t *text) { return self.startsWith(text); },
          py::arg("text"))
      /*
      .def("starts_with", py::overload_cast<ConstChar16Ptr, int32_t>(&UnicodeString::startsWith, py::const_),
           py::arg("src_chars"), py::arg("src_length"))
      */
      .def(
          // UBool startsWith(ConstChar16Ptr srcChars, int32_t srcLength)
          "starts_with",
          [](const UnicodeString &self, const char16_t *src_chars, int32_t src_length) {
            return self.startsWith(src_chars, src_length);
          },
          py::arg("src_chars"), py::arg("src_length"));
#if (U_ICU_VERSION_MAJOR_NUM >= 56)
  us.def("swap", &UnicodeString::swap, py::arg("other"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 56)
  us.def("temp_sub_string", &UnicodeString::tempSubString, py::arg("start") = 0, py::arg("length") = INT32_MAX)
      .def("temp_sub_string_between", &UnicodeString::tempSubStringBetween, py::arg("start") = 0,
           py::arg("limit") = INT32_MAX);
  us.def("to_lower", py::overload_cast<const Locale &>(&UnicodeString::toLower), py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "to_lower", [](UnicodeString &self, const char *locale) -> UnicodeString & { return self.toLower(locale); },
          py::arg("locale"))
      .def("to_lower", py::overload_cast<>(&UnicodeString::toLower));
  us.def("to_title", py::overload_cast<BreakIterator *>(&UnicodeString::toTitle), py::arg("title_iter"))
      .def("to_title", py::overload_cast<BreakIterator *, const Locale &>(&UnicodeString::toTitle),
           py::arg("title_iter"), py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "to_title",
          [](UnicodeString &self, BreakIterator *title_iter, const char *locale) -> UnicodeString & {
            return self.toTitle(title_iter, locale);
          },
          py::arg("title_iter"), py::arg("locale"))
      .def("to_title", py::overload_cast<BreakIterator *, const Locale &, uint32_t>(&UnicodeString::toTitle),
           py::arg("title_iter"), py::arg("locale"), py::arg("options"))
      .def(
          // const char *locale -> const Locale &locale
          "to_title",
          [](UnicodeString &self, BreakIterator *title_iter, const char *locale, uint32_t options) -> UnicodeString & {
            return self.toTitle(title_iter, locale, options);
          },
          py::arg("title_iter"), py::arg("locale"), py::arg("options"));
  us.def("to_upper", py::overload_cast<const Locale &>(&UnicodeString::toUpper), py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "to_upper", [](UnicodeString &self, const char *locale) -> UnicodeString & { return self.toUpper(locale); },
          py::arg("locale"))
      .def("to_upper", py::overload_cast<>(&UnicodeString::toUpper));
  us.def("to_utf32", [](const UnicodeString &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    const auto length = self.toUTF32(nullptr, 0, error_code);
    std::vector<UChar32> dest(length);
    error_code = U_ZERO_ERROR;
    self.toUTF32(dest.data(), length, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
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
  us.def("truncate", &UnicodeString::truncate, py::arg("target_length"));
  us.def("unescape", &UnicodeString::unescape).def("unescape_at", &UnicodeString::unescapeAt, py::arg("offset"));

  us.def_property_readonly_static("SENTINEL", [](py::object) { return U_SENTINEL; });

  us.def_property_readonly_static("COMPARE_CODE_POINT_ORDER", [](py::object) { return U_COMPARE_CODE_POINT_ORDER; });
  us.def_property_readonly_static("FOLD_CASE_DEFAULT", [](py::object) { return U_FOLD_CASE_DEFAULT; });
  us.def_property_readonly_static("FOLD_CASE_EXCLUDE_SPECIAL_I",
                                  [](py::object) { return U_FOLD_CASE_EXCLUDE_SPECIAL_I; });
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static("TITLECASE_ADJUST_TO_CASED", [](py::object) { return U_TITLECASE_ADJUST_TO_CASED; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static("TITLECASE_NO_BREAK_ADJUSTMENT",
                                  [](py::object) { return U_TITLECASE_NO_BREAK_ADJUSTMENT; });
  us.def_property_readonly_static("TITLECASE_NO_LOWERCASE", [](py::object) { return U_TITLECASE_NO_LOWERCASE; });
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  us.def_property_readonly_static("TITLECASE_SENTENCES", [](py::object) { return U_TITLECASE_SENTENCES; });
  us.def_property_readonly_static("TITLECASE_WHOLE_STRING", [](py::object) { return U_TITLECASE_WHOLE_STRING; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  m.attr("US_INV") = US_INV;
}

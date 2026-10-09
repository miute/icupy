#include "main.hpp"
#include "usetptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/native_enum.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/parsepos.h>
#include <unicode/symtable.h>
#include <unicode/uniset.h>
#include <unicode/ustream.h>

using namespace icu;

void init_unifunctor(py::module &m) {
  //
  // class icu::UnicodeFunctor
  //
  py::class_<UnicodeFunctor, UObject>(m, "UnicodeFunctor", R"doc(
      Abstract base class for :class:`UnicodeFilter` that performs match and/or
      replace operations on Unicode strings.

      .. seealso::

         :class:`UnicodeFilter`
      )doc");
}

void init_uniset(py::module &m, py::module &h) {
  //
  // enum icu::UMatchDegree
  //
  py::native_enum<UMatchDegree>(m, "UMatchDegree", "enum.IntEnum", R"doc(
Constants returned by :meth:`UnicodeMatcher.matches` indicating the degree of
match.
)doc")
      .value("U_MISMATCH", U_MISMATCH, R"doc(
             Constant returned by :meth:`UnicodeMatcher.matches` indicating a
             mismatch between the text and this matcher.

             The text contains a character which does not match, or the text
             does not contain all desired characters for a non-incremental
             match.
             )doc")
      .value("U_PARTIAL_MATCH", U_PARTIAL_MATCH, R"doc(
             Constant returned by :meth:`UnicodeMatcher.matches` indicating
             a partial match between the text and this matcher.

             This value is only returned for incremental match operations.
             All characters of the text match, but more characters are required
             for a complete match. Alternatively, for variable-length matchers,
             all characters of the text match, and if more characters were
             supplied at limit, they might also match.
             )doc")
      .value("U_MATCH", U_MATCH, R"doc(
             Constant returned by :meth:`UnicodeMatcher.matches` indicating
             a complete match between the text and this matcher.

             For an incremental variable-length match, this value is returned
             if the given text matches, and it is known that additional
             characters would not alter the extent of the match.
             )doc")
      .export_values()
      .finalize();

  //
  // class icu::UnicodeMatcher
  //
  py::class_<UnicodeMatcher> umat(m, "UnicodeMatcher", R"doc(
      Abstract base class for :class:`UnicodeFilter` defines a protocol for
      objects that can match a range of characters in a :class:`Replaceable`
      string.

      .. seealso::

         :class:`UnicodeFilter`
      )doc");

  //
  // class icu::UnicodeFilter
  //
  py::class_<UnicodeFilter, UnicodeFunctor, UnicodeMatcher>(m, "UnicodeFilter",
                                                            R"doc(
      ``UnicodeFilter`` defines a protocol for selecting a subset of the full
      range (U+0000 to U+10FFFF) of Unicode characters.

      .. seealso::

         :class:`UnicodeSet`
      )doc");

  //
  // class icu::UnicodeSet
  //
  py::class_<UnicodeSet, UnicodeFilter> us(m, "UnicodeSet", R"doc(
      Mutable set of Unicode characters and multicharacter strings.

      For more information, see the
      `ICU User Guide: UnicodeSet
      <https://unicode-org.github.io/icu/userguide/strings/unicodeset.html#unicodeset>`__ and
      the C++ API reference: `icu::UnicodeSet
      <https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeSet.html#details>`__.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  //
  // class U_HEADER_ONLY_NAMESPACE::CodePointRange
  //
  py::class_<U_HEADER_ONLY_NAMESPACE::CodePointRange> cpr(h, "CodePointRange",
                                                          R"doc(
      Range of contiguous code points in a :class:`~icupy.icu.UnicodeSet`.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> us = icu.UnicodeSet("[0-9a-z{345}]")
         >>> [tuple(x) for x in us.ranges()]
         [(48, 49, 50, 51, 52, 53, 54, 55, 56, 57), (97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122)]
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  //
  // class icu::UnicodeMatcher
  //
  umat.def("add_match_set_to", &UnicodeMatcher::addMatchSetTo,
           py::arg("to_union_to"), R"doc(
      Union the set of all characters that may match this object into the
      specified set.
      )doc");

  umat.def(
      "matches",
      [](UnicodeMatcher &self, const Replaceable &text, int32_t offset,
         int32_t limit, py::bool_ incremental) {
        auto result = self.matches(text, offset, limit, incremental);
        return std::make_tuple(result, offset);
      },
      py::arg("text"), py::arg("offset"), py::arg("limit"),
      py::arg("incremental"), R"doc(
      Return a tuple with the :class:`UMatchDegree` value indicating the match
      degree of the specified text at the specified offset, and the end offset
      of the matched text.

      *text* is the text to be matched.

      *offset* is the start index for a string match. *offset* should always
      point to the HIGH SURROGATE (leading code unit) of a surrogate pair.

      *limit* is the end index of the text to be matched. For forward
      matches, it is greater than *offset*; for backward matches, it is less
      than *offset*. The last character to be matched is
      *text.char_at(limit-1)* for forward matches and
      *text.char_at(limit+1)* for backward matches.

      If *incremental* is ``True``, partial matches are checked on the
      assumption that additional characters may be inserted at the *limit*
      position. Otherwise, the specified text assumes the text is complete.

      - Forward matching is indicated by *limit* > *offset*. The characters
        from *offset* to *limit* - 1 will be matched.
      - Reverse matching is indicated by *limit* < *offset*. The characters
        from *offset* to *limit* + 1 will be matched.
      )doc");

  umat.def(
      "to_pattern",
      [](const UnicodeMatcher &self, UnicodeString &result,
         py::bool_ escape_unprintable) -> UnicodeString & {
        return self.toPattern(result, escape_unprintable);
      },
      py::arg("result"), py::arg("escape_unprintable") = false, R"doc(
      Retrieve the string representation of this matcher and store it in
      *result*; return *result* itself.

      *result* is a string object to receive the string representation.

      If *escape_unprintable* is ``True``, unprintable characters are escaped
      in the output. Unprintable characters are those other than U+000A and
      U+0020 to U+007E.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  //
  // class U_HEADER_ONLY_NAMESPACE::CodePointRange
  //
  cpr.def_property_readonly(
      "range_end",
      [](const U_HEADER_ONLY_NAMESPACE::CodePointRange &self) {
        return self.rangeEnd;
      },
      R"doc(
      Return the last code point in the range.
      )doc");

  cpr.def_property_readonly(
      "range_start",
      [](const U_HEADER_ONLY_NAMESPACE::CodePointRange &self) {
        return self.rangeStart;
      },
      R"doc(
      Return the first code point in the range.
      )doc");

  cpr.def(
      "__iter__",
      [](U_HEADER_ONLY_NAMESPACE::CodePointRange &self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>(), R"doc(
      Return an iterator for iterating over the code points in this range.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  //
  // class icu::UnicodeSet
  //
  us.def_property_readonly_static(
      "MAX_VALUE",
      [](const py::object & /* self */) -> int32_t {
        return UnicodeSet::MAX_VALUE;
      },
      R"doc(
      Maximum value that can be stored in ``UnicodeSet``.
      )doc");

  us.def_property_readonly_static(
      "MIN_VALUE",
      [](const py::object & /* self */) -> int32_t {
        return UnicodeSet::MIN_VALUE;
      },
      R"doc(
      Minimum value that can be stored in ``UnicodeSet``.
      )doc");

  us.def(py::init<>(), R"doc(
      Initialize a ``UnicodeSet`` instance as an empty set.
      )doc")
      .def(py::init<UChar32, UChar32>(), py::arg("start"), py::arg("end"),
           R"doc(
      Initialize a ``UnicodeSet`` instance with the characters in the specified
      range.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.

      If *end* is greater than *start*, an empty set is created.
      )doc")
      .def(py::init([](const icupy::UnicodeStringVariant &pattern) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeSet>(
                 icupy::to_unistr(pattern), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"), R"doc(
      Initialize a ``UnicodeSet`` instance with the specified pattern.

      *pattern* is a string that specify the characters within the set.
      For more information about the syntax of the pattern language, see the
      `ICU User Guide: UnicodeSet
      <https://unicode-org.github.io/icu/userguide/strings/unicodeset.html#unicodeset>`__ and
      the C++ API reference: `icu::UnicodeSet
      <https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeSet.html#details>`__.
      )doc")
      .def(py::init([](const icupy::UnicodeStringVariant &pattern,
                       ParsePosition &pos, uint32_t options,
                       std::optional<const SymbolTable *> symbols) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeSet>(
                 icupy::to_unistr(pattern), pos, options,
                 symbols.value_or(nullptr), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"), py::arg("pos"), py::arg("options"),
           py::arg("symbols") = std::nullopt, R"doc(
      Initialize a ``UnicodeSet`` instance with the specified pattern.

      *pattern* is a string that specify the characters within the set.
      For more information about the syntax of the pattern language, see the
      `ICU User Guide: UnicodeSet
      <https://unicode-org.github.io/icu/userguide/strings/unicodeset.html#unicodeset>`__ and
      the C++ API reference: `icu::UnicodeSet
      <https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeSet.html#details>`__.

      *pos* is the position within the pattern to start parsing. After this
      call, it is updated to the position after the last parsed character.

      *options* is a bitmask to apply to the pattern. Valid options are
      :attr:`USET_IGNORE_SPACE` and one of the following:
      :attr:`USET_CASE_INSENSITIVE`, :attr:`USET_ADD_CASE_MAPPINGS`, or
      :attr:`USET_SIMPLE_CASE_INSENSITIVE`.

      *symbols* is a symbol table that maps variable names to values and maps
      substitution characters to ``UnicodeSet``.
      )doc")
      .def(py::init<const UnicodeSet &>(), py::arg("other"), R"doc(
      Initialize a ``UnicodeSet`` instance from a copy of *other*.
      )doc");

  us.def(
        "__contains__",
        [](const UnicodeSet &self, const icupy::UnicodeStringVariant &item)
            -> py::bool_ { return self.contains(icupy::to_unistr(item)); },
        py::arg("item"), R"doc(
      Return ``True`` if this set contains the specified multi-character
      string, ``False`` otherwise.

      This is equivalent to calling :meth:`.contains`.
      )doc")
      .def(
          "__contains__",
          [](const UnicodeSet &self, UChar32 item) -> py::bool_ {
            return self.contains(item);
          },
          py::arg("item"), R"doc(
      Return ``True`` if this set contains the specified character, ``False``
      otherwise.

      This is equivalent to calling :meth:`.contains`.
      )doc");

  us.def("__copy__", &UnicodeSet::clone, R"doc(
      Return a copy of this set.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  us.def(
      "__deepcopy__",
      [](const UnicodeSet &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"), R"doc(
      Return a copy of this set.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  us.def(
        "__eq__",
        [](const UnicodeSet &self, const UnicodeSet &other) {
          return self == other;
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc")
      .def(
          "__eq__",
          [](const UnicodeSet &self, icupy::ConstUSetPtr &other) -> py::bool_ {
            return uset_equals(self.toUSet(), other);
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc")
      .def(
          "__eq__",
          [](const UnicodeSet &self, icupy::USetPtr &other) -> py::bool_ {
            return uset_equals(self.toUSet(), other);
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  us.def(
      "__getitem__",
      [](const UnicodeSet &self, int32_t index) {
        auto normalized_index = index;
        const auto size = self.size();
        if (normalized_index < 0) {
          normalized_index += size;
        }
        if (normalized_index < 0 || normalized_index >= size) {
          throw py::index_error("elements index out of range: " +
                                std::to_string(index));
        }

        const auto range_count = self.getRangeCount();
        int32_t characters = 0;
        for (int32_t i = 0; i < range_count; ++i) {
          const auto start = self.getRangeStart(i);
          const auto end = self.getRangeEnd(i);
          const auto base = characters;
          characters += end - start + 1;
          if (normalized_index < characters) {
            return py::str(
                PyUnicode_FromOrdinal(start + normalized_index - base));
          }
        }
#if (U_ICU_VERSION_MAJOR_NUM < 76)
        return py::str();
#else  // (U_ICU_VERSION_MAJOR_NUM >= 76)
        const auto uset = self.toUSet();
        int32_t length;
        const auto p =
            uset_getString(uset, normalized_index - characters, &length);
        const auto text = UnicodeString(p, length);
        std::string result;
        text.toUTF8String(result);
        return py::str(result);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)
      },
      py::arg("index"), R"doc(
      Return the character or multi-character string at the specified index.

      .. version-changed:: ICU76
         Added support for multi-character strings.
      )doc");

  us.def("__hash__", &UnicodeSet::hashCode, R"doc(
      Return a hash value of this set.

      This is equivalent to calling :meth:`.hash_code`.
      )doc");

  /*
#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  us.def(
      "__iter__",
      [](const UnicodeSet &self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>());
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)
  */

  us.def("__len__", &UnicodeSet::size, R"doc(
      Return the number of elements in this set.

      This is equivalent to calling :meth:`.size`.
      )doc");

  us.def(
        "__ne__",
        [](const UnicodeSet &self, const UnicodeSet &other) {
          return self != other;
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc")
      .def(
          "__ne__",
          [](const UnicodeSet &self, icupy::ConstUSetPtr &other) {
            return !uset_equals(self.toUSet(), other);
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc")
      .def(
          "__ne__",
          [](const UnicodeSet &self, icupy::USetPtr &other) {
            return !uset_equals(self.toUSet(), other);
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  us.def("__repr__", [](const UnicodeSet &self) {
    std::stringstream ss;
    ss << "<UnicodeSet('";
    UnicodeString pattern;
    self.toPattern(pattern, true).findAndReplace("'", "\\'");
    ss << pattern;
    ss << "')>";
    return ss.str();
  });

  us.def(
        "add",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
            -> UnicodeSet & { return self.add(icupy::to_unistr(s)); },
        py::arg("s"), R"doc(
      Add the specified multiple-character string to this set and return the
      set itself.
      )doc")
      .def("add", py::overload_cast<UChar32>(&UnicodeSet::add), py::arg("c"),
           R"doc(
      Add the specified character to this set and return the set itself.
      )doc")
      .def("add", py::overload_cast<UChar32, UChar32>(&UnicodeSet::add),
           py::arg("start"), py::arg("end"), R"doc(
      Add the characters in the specified range to this set and return the set
      itself.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.

      If *end* is greater than *start*, an empty range is added.
      )doc");

  us.def("add_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::addAll),
         py::arg("c"), R"doc(
      Add all elements of the specified set to this set and return the set
      itself.
      )doc")
      .def(
          "add_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
              -> UnicodeSet & { return self.addAll(icupy::to_unistr(s)); },
          py::arg("s"), R"doc(
      Add each character in the specified string to this set and return the set
      itself.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> us = icu.UnicodeSet()
         >>> us.add("345")
         <UnicodeSet('[{345}]')>
         >>> us.add_all("345")
         <UnicodeSet('[3-5{345}]')>
         )doc");

  us.def(
      "apply_int_property_value",
      [](UnicodeSet &self, UProperty prop, int32_t value) -> UnicodeSet & {
        ErrorCode error_code;
        auto &result = self.applyIntPropertyValue(prop, value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("prop"), py::arg("value"), R"doc(
      Modify this set to contain only code points with the specified value for
      the specified binary or enumeration property, using the value returned
      by :func:`u_get_int_property_value`, and return the set itself.

      The previous contents of this set are lost.

      *prop* is a property and must be one of the following:
      [:attr:`~UProperty.UCHAR_BINARY_START`,
      :attr:`~UProperty.UCHAR_BINARY_LIMIT`),
      [:attr:`~UProperty.UCHAR_INT_START`,
      :attr:`~UProperty.UCHAR_INT_LIMIT`), or
      [:attr:`~UProperty.UCHAR_MASK_START`,
      :attr:`~UProperty.UCHAR_MASK_LIMIT`).

      *value* must be
      [*u_get_int_property_min_value(prop)*,
      *u_get_int_property_max_value(prop)*].
      However, if *prop* is :attr:`~UProperty.UCHAR_GENERAL_CATEGORY_MASK`,
      *value* must be a mask value produced by :func:`u_get_gc_mask`.

      .. seealso::

         :func:`u_get_gc_mask`
         :func:`u_get_int_property_min_value`
         :func:`u_get_int_property_max_value`
         :func:`u_get_int_property_value`
      )doc");

  us.def(
        "apply_pattern",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &pattern,
           ParsePosition &pos, uint32_t options,
           std::optional<const SymbolTable *> symbols) -> UnicodeSet & {
          ErrorCode error_code;
          auto &result =
              self.applyPattern(icupy::to_unistr(pattern), pos, options,
                                symbols.value_or(nullptr), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("pos"), py::arg("options"),
        py::arg("symbols") = std::nullopt, R"doc(
      Modify this set to contain the set represented by the specified pattern
      and return the set itself.

      The previous contents of this set are lost.

      *pattern* is a string that specify the characters within the set.
      For more information about the syntax of the pattern language, see the
      `ICU User Guide: UnicodeSet
      <https://unicode-org.github.io/icu/userguide/strings/unicodeset.html#unicodeset>`__ and
      the C++ API reference: `icu::UnicodeSet
      <https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeSet.html#details>`__.

      *pos* is the position within the pattern to start parsing. After this
      call, it is updated to the position after the last parsed character.

      *options* is a bitmask to apply to the pattern. Valid options are
      :attr:`USET_IGNORE_SPACE` and one of the following:
      :attr:`USET_CASE_INSENSITIVE`, :attr:`USET_ADD_CASE_MAPPINGS`, or
      :attr:`USET_SIMPLE_CASE_INSENSITIVE`.

      *symbols* is a symbol table that maps variable names to values and maps
      substitution characters to ``UnicodeSet``.
      )doc")
      .def(
          "apply_pattern",
          [](UnicodeSet &self,
             const icupy::UnicodeStringVariant &pattern) -> UnicodeSet & {
            ErrorCode error_code;
            auto &result =
                self.applyPattern(icupy::to_unistr(pattern), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), R"doc(
      Modify this set to contain the set represented by the specified pattern,
      excluding Unicode Pattern_White_Space characters, and return the set
      itself.

      The previous contents of this set are lost.

      *pattern* is a string that specify the characters within the set.
      For more information about the syntax of the pattern language, see the
      `ICU User Guide: UnicodeSet
      <https://unicode-org.github.io/icu/userguide/strings/unicodeset.html#unicodeset>`__ and
      the C++ API reference: `icu::UnicodeSet
      <https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeSet.html#details>`__.
      )doc");

  us.def(
      "apply_property_alias",
      [](UnicodeSet &self, const icupy::UnicodeStringVariant &prop,
         const icupy::UnicodeStringVariant &value) -> UnicodeSet & {
        ErrorCode error_code;
        auto &result = self.applyPropertyAlias(
            icupy::to_unistr(prop), icupy::to_unistr(value), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("prop"), py::arg("value"), R"doc(
      Modify this set to contain only code points that have the specified value
      for the specified property and return the set itself.

      The previous contents of this set are lost.

      *prop* is an alias for a property, either short or long. The name is
      matched loosely. For a description of names and loose matching, see
      PropertyAliases.txt. If the value string is empty, it is interpreted as
      an alias for a General_Category value, a script value, a binary property,
      or a special ID. Special IDs are loosely matched and correspond to the
      following sets:

      - "ANY" = [\\u0000-\\U0010FFFF]
      - "ASCII" = [\\u0000-\\u007F]
      - "Assigned" = [:^Cn:]

      *value* is an alias for a short or long value. The name is matched
      loosely. For a description of names and loose matching, see
      PropertyValueAliases.txt. In addition to the listed aliases, numeric
      values and canonical combining classes may also be expressed numerically,
      e.g., ("nv", "0.5") or ("ccc", "220"). The value string may be empty.
      )doc");

  us.def("char_at", &UnicodeSet::charAt, py::arg("index"), R"doc(
      Return the character at the specified index, or -1 if *index* is out of
      range.

      .. seealso::

         :meth:`.index_of`
         :meth:`.size`
      )doc");

  us.def("clear", &UnicodeSet::clear, R"doc(
      Remove all elements from this set and return the set itself.
      )doc");

  us.def("clone", &UnicodeSet::clone, R"doc(
      Return a copy of this set.

      .. seealso::

         :meth:`.__copy__`
         :meth:`.__deepcopy__`
         :meth:`.clone_as_thawed`
      )doc");

  us.def("clone_as_thawed", &UnicodeSet::cloneAsThawed, R"doc(
      Return a mutable copy of this set.

      .. seealso::

         :meth:`.freeze`
         :meth:`.is_frozen`
      )doc");

  us.def("close_over", &UnicodeSet::closeOver, py::arg("attribute"), R"doc(
      Close this set over with the specified attribute and return the set
      itself.

      *attribute* is a bitmask for the attribute to close this set over.
      One of the following options is valid:
      :attr:`USET_CASE_INSENSITIVE`,
      :attr:`USET_ADD_CASE_MAPPINGS`, or
      :attr:`USET_SIMPLE_CASE_INSENSITIVE`.
      Irrelevant option bits are ignored.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> us = icu.UnicodeSet("[aq\u00DF{Bc}{bC}{Fi}]")
         >>> us
         <UnicodeSet('[aq\u00DF{Bc}{Fi}{bC}]')>
         >>> us.close_over(icu.USET_CASE_INSENSITIVE)
         <UnicodeSet('[AQaq\u00DF\u1E9E\uFB01{bc}{fi}{ss}]')>
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  us.def(
      "code_points",
      [](const UnicodeSet &self) {
        auto it = self.codePoints();
        return py::make_iterator(it.begin(), it.end());
      },
      py::keep_alive<0, 1>(), R"doc(
      Return an iterator for iterating over the code points in this set.

      .. seealso::

         :meth:`.ranges`
         :meth:`.strings`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  us.def("compact", &UnicodeSet::compact, R"doc(
      Reallocate the internal structures to occupy minimal memory without
      changing the values of this object and return the set itself.
      )doc");

  us.def(
        "complement",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
            -> UnicodeSet & { return self.complement(icupy::to_unistr(s)); },
        py::arg("s"), R"doc(
      Complement this set with the specified multi-character string
      and return the set itself.

      If *s* is already in this set, it will be removed, otherwise it will be
      added.
      )doc")
      .def("complement", py::overload_cast<UChar32>(&UnicodeSet::complement),
           py::arg("c"), R"doc(
      Complement this set with the specified character and return the set
      itself.

      If *c* is already in this set, it will be removed, otherwise it will be
      added.
      )doc")
      .def("complement",
           py::overload_cast<UChar32, UChar32>(&UnicodeSet::complement),
           py::arg("start"), py::arg("end"), R"doc(
      Complement this set with the characters in the specified range and return
      the set itself.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.

      If any character in the range is already in this set, it will be
      removed, otherwise it will be added.
      )doc")
      .def("complement", py::overload_cast<>(&UnicodeSet::complement), R"doc(
      Complement this set and return the set itself.

      This is equivalent to calling :meth:`.complement` with
      :attr:`~UnicodeSet.MIN_VALUE` and :attr:`~UnicodeSet.MAX_VALUE`.

      All characters that are not in this set will be added, and all
      characters that are in this set will be removed.
      )doc");

  us.def("complement_all",
         py::overload_cast<const UnicodeSet &>(&UnicodeSet::complementAll),
         py::arg("c"), R"doc(
      Complement this set with all elements in the specified set and return the
      set itself.

      Characters that are in the other set will be removed if they are already
      in this set, and added if they are not.
      )doc")
      .def(
          "complement_all",
          [](UnicodeSet &self,
             const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
            return self.complementAll(icupy::to_unistr(s));
          },
          py::arg("s"), R"doc(
      Complement this set with each character in the specified string and
      return the set itself.

      Characters that are in the other set will be removed if they are already
      in this set, and added if they are not.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> us = icu.UnicodeSet(0x30, 0x39)
         >>> us
         <UnicodeSet('[0-9]')>
         >>> us.complement("345")
         <UnicodeSet('[0-9{345}]')>
         >>> us.complement_all("345")
         <UnicodeSet('[0-26-9{345}]')>
      )doc");

  us.def(
        "contains",
        [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s)
            -> py::bool_ { return self.contains(icupy::to_unistr(s)); },
        py::arg("s"), R"doc(
      Return ``True`` if this set contains the specified multi-character
      string, ``False`` otherwise.

      .. seealso::

         :meth:`.__contains__`.
      )doc")
      .def(
          "contains",
          [](const UnicodeSet &self, UChar32 c) -> py::bool_ {
            return self.contains(c);
          },
          py::arg("c"), R"doc(
      Return ``True`` if this set contains the specified character, ``False``
      otherwise.

      .. seealso::

         :meth:`.__contains__`.
      )doc")
      .def(
          "contains",
          [](const UnicodeSet &self, UChar32 start, UChar32 end) -> py::bool_ {
            return self.contains(start, end);
          },
          py::arg("start"), py::arg("end"), R"doc(
      Return ``True`` if this set contains the characters in the specified
      range, ``False`` otherwise.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.
      )doc");

  us.def(
        "contains_all",
        [](const UnicodeSet &self, const UnicodeSet &c) -> py::bool_ {
          return self.containsAll(c);
        },
        py::arg("c"), R"doc(
      Return ``True`` if this set contains all characters and multi-character
      strings in the specified set, ``False`` otherwise.
      )doc")
      .def(
          "contains_all",
          [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s)
              -> py::bool_ { return self.containsAll(icupy::to_unistr(s)); },
          py::arg("s"), R"doc(
      Return ``True`` if this set contains all characters of the specified
      string, ``False`` otherwise.
      )doc");

  us.def(
        "contains_none",
        [](const UnicodeSet &self, const UnicodeSet &c) -> py::bool_ {
          return self.containsNone(c);
        },
        py::arg("c"), R"doc(
      Return ``True`` if this set does not contain any characters or
      multi-character strings in the specified set, ``False``
      otherwise.
      )doc")
      .def(
          "contains_none",
          [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s)
              -> py::bool_ { return self.containsNone(icupy::to_unistr(s)); },
          py::arg("s"), R"doc(
      Return ``True`` if this set does not contain any characters in the
      specified string, ``False`` otherwise.
      )doc")
      .def(
          "contains_none",
          [](const UnicodeSet &self, UChar32 start, UChar32 end) -> py::bool_ {
            return self.containsNone(start, end);
          },
          py::arg("start"), py::arg("end"), R"doc(
      Return ``True`` if this set does not contain any characters in the
      specified range, ``False`` otherwise.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.
      )doc");

  us.def(
        "contains_some",
        [](const UnicodeSet &self, const UnicodeSet &c) -> py::bool_ {
          return self.containsSome(c);
        },
        py::arg("c"), R"doc(
      Return ``True`` if this set contains one or more characters or
      multi-character strings in the specified set, ``False``
      otherwise.
      )doc")
      .def(
          "contains_some",
          [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s)
              -> py::bool_ { return self.containsSome(icupy::to_unistr(s)); },
          py::arg("s"), R"doc(
      Return ``True`` if this set contains one or more characters in the
      specified string, ``False`` otherwise.
      )doc")
      .def(
          "contains_some",
          [](const UnicodeSet &self, UChar32 start, UChar32 end) -> py::bool_ {
            return self.containsSome(start, end);
          },
          py::arg("start"), py::arg("end"), R"doc(
      Return ``True`` if this set contains one or more characters in the
      specified range, ``False`` otherwise.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.
      )doc");

  us.def_static(
      "create_from",
      [](const icupy::UnicodeStringVariant &s) {
        return UnicodeSet::createFrom(icupy::to_unistr(s));
      },
      py::arg("s"), R"doc(
      Create a new ``UnicodeSet`` instance from a multi-character string.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> icu.UnicodeSet.create_from("ch")
         <UnicodeSet('[{ch}]')>
      )doc");

  us.def_static(
      "create_from_all",
      [](const icupy::UnicodeStringVariant &s) {
        return UnicodeSet::createFromAll(icupy::to_unistr(s));
      },
      py::arg("s"), R"doc(
      Create a new ``UnicodeSet`` instance from each character in the string.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> icu.UnicodeSet.create_from_all("ch")
         <UnicodeSet('[ch]')>
      )doc");

  us.def("freeze", &UnicodeSet::freeze, R"doc(
      Freeze the set (make it immutable) and return the set itself.

      A frozen set cannot be modified.

      .. seealso::

         :meth:`.clone_as_thawed`
         :meth:`.is_frozen`
      )doc");

  us.def_static(
        "from_uset",
        [](icupy::ConstUSetPtr &uset) { return UnicodeSet::fromUSet(uset); },
        py::return_value_policy::reference, py::arg("uset"), R"doc(
      Create a ``UnicodeSet`` instance from an immutable ``USet`` object.

      .. important::

         *uset* must outlive the returned set object.
      )doc")
      .def_static(
          "from_uset",
          [](icupy::USetPtr &uset) { return UnicodeSet::fromUSet(uset); },
          py::return_value_policy::reference, py::arg("uset"), R"doc(
      Create a ``UnicodeSet`` instance from a mutable ``USet`` object.

      .. important::

         *uset* must outlive the returned set object.
      )doc");

  us.def("get_range_count", &UnicodeSet::getRangeCount, R"doc(
      Return the number of ranges in this set.

      .. seealso::

         :meth:`.get_range_start`
         :meth:`.get_range_end`
      )doc");

  us.def("get_range_end", &UnicodeSet::getRangeEnd, py::arg("index"), R"doc(
      Return the last character in the specified range within this set.

      The result is undefined if *index* is out of range.

      .. seealso::

         :meth:`.get_range_count`
         :meth:`.get_range_start`
      )doc");

  us.def("get_range_start", &UnicodeSet::getRangeStart, py::arg("index"), R"doc(
      Return the first character in the specified range within this set.

      The result is undefined if *index* is out of range.

      .. seealso::

         :meth:`.get_range_count`
         :meth:`.get_range_end`
      )doc");

  us.def("hash_code", &UnicodeSet::hashCode, R"doc(
      Return a hash code for this set.

      .. seealso::

         :meth:`.__hash__`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  us.def(
      "has_strings",
      [](const UnicodeSet &self) -> py::bool_ { return self.hasStrings(); },
      R"doc(
      Return ``True`` if this set has multi-character strings or the empty
      string, ``False`` otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

  us.def("index_of", &UnicodeSet::indexOf, py::arg("c"), R"doc(
      Return the index of the specified character in this set, or -1 if
      it does not exist.

      .. seealso::

         :meth:`.char_at`
         :meth:`.size`
      )doc");

  us.def(
      "is_bogus",
      [](const UnicodeSet &self) -> py::bool_ { return self.isBogus(); }, R"doc(
      Return ``True`` if this set is not valid, ``False`` otherwise.

      .. seealso::

         :meth:`.set_to_bogus`
      )doc");

  us.def(
      "is_empty",
      [](const UnicodeSet &self) -> py::bool_ { return self.isEmpty(); }, R"doc(
      Return ``True`` if this set is empty, ``False`` otherwise.
      )doc");

  us.def(
      "is_frozen",
      [](const UnicodeSet &self) -> py::bool_ { return self.isFrozen(); },
      R"doc(
      Return ``True`` if this set is frozen, ``False`` otherwise.

      .. seealso::

         :meth:`.clone_as_thawed`
         :meth:`.freeze`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  us.def(
      "ranges",
      [](const UnicodeSet &self) {
        auto it = self.ranges();
        return py::make_iterator(it.begin(), it.end());
      },
      py::keep_alive<0, 1>(), R"doc(
      Return an iterator for iterating over the code point ranges in this set.

      .. seealso::

         :meth:`.code_points`
         :meth:`.strings`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  us.def(
        "remove",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
            -> UnicodeSet & { return self.remove(icupy::to_unistr(s)); },
        py::arg("s"), R"doc(
      Remove the specified multi-character string from this set, if it
      exists, and return the set itself.
      )doc")
      .def("remove", py::overload_cast<UChar32>(&UnicodeSet::remove),
           py::arg("c"), R"doc(
      Remove the specified character from this set, if it exists, and
      return the set itself.
      )doc")
      .def("remove", py::overload_cast<UChar32, UChar32>(&UnicodeSet::remove),
           py::arg("start"), py::arg("end"), R"doc(
      Remove the characters in the specified range from this set, if they
      exist, and return the set itself.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.
      )doc");

  us.def("remove_all",
         py::overload_cast<const UnicodeSet &>(&UnicodeSet::removeAll),
         py::arg("c"), R"doc(
      Remove all elements in the specified set from this set and return the set
      itself.
      )doc")
      .def(
          "remove_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
              -> UnicodeSet & { return self.removeAll(icupy::to_unistr(s)); },
          py::arg("s"), R"doc(
      Remove all characters in the specified string from this set and return
      the set itself.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> us = icu.UnicodeSet("[0-9{345}]")
         >>> us
         <UnicodeSet('[0-9{345}]')>
         >>> us.remove("345")
         <UnicodeSet('[0-9]')>
         >>> us.remove_all("345")
         <UnicodeSet('[0-26-9]')>
      )doc");

  us.def("remove_all_strings", &UnicodeSet::removeAllStrings, R"doc(
      Remove all multi-character strings from this set and return the set
      itself.
      )doc");

  us.def_static(
      "resembles_pattern",
      [](const icupy::UnicodeStringVariant &pattern, int32_t pos) -> py::bool_ {
        return UnicodeSet::resemblesPattern(icupy::to_unistr(pattern), pos);
      },
      py::arg("pattern"), py::arg("pos"), R"doc(
      Return ``True`` if the position specified by *pos* within the pattern
      specified by *pattern* appears to be the starting position of a
      ``UnicodeSet`` pattern, ``False`` otherwise.

      *pattern* is a string to be checked.

      *pos* is the start index within the *pattern* to be checked.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  us.def(
      "retain",
      [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
          -> UnicodeSet & { return self.retain(icupy::to_unistr(s)); },
      py::arg("s"), R"doc(
      Retain only the specified multi-character string from this set and
      return the set itself.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  us.def("retain", py::overload_cast<UChar32>(&UnicodeSet::retain),
         py::arg("c"), R"doc(
      Retain only the specified character from this set and return the
      set itself.
      )doc")
      .def("retain", py::overload_cast<UChar32, UChar32>(&UnicodeSet::retain),
           py::arg("start"), py::arg("end"), R"doc(
      Retain only the characters in the specified range from this set and
      return the set itself.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.
      )doc");

  us.def("retain_all",
         py::overload_cast<const UnicodeSet &>(&UnicodeSet::retainAll),
         py::arg("c"), R"doc(
      Retain only the elements of the specified set from this set and return
      the set itself.
      )doc")
      .def(
          "retain_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s)
              -> UnicodeSet & { return self.retainAll(icupy::to_unistr(s)); },
          py::arg("s"), R"doc(
      Retain only the characters in the specified string from this set and
      return the set itself.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> us = icu.UnicodeSet("[0-9{345}]")
         >>> us.retain("345")
         <UnicodeSet('[{345}]')>
         >>> us = icu.UnicodeSet("[0-9{345}]")
         >>> us.retain_all("345")
         <UnicodeSet('[3-5]')>
      )doc");

  us.def(
      "serialize",
      [](const UnicodeSet &self) {
        ErrorCode error_code;
        const auto capacity = self.serialize(nullptr, 0, error_code);
        std::vector<uint16_t> result(capacity, 0);
        error_code.reset();
        self.serialize(result.data(), capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Serialize this set into a list of 16-bit integers and return it.

      .. note::

         Currently, serialization records only the characters within a set;
         multi-character strings are ignored.
      )doc");

  us.def("set", &UnicodeSet::set, py::arg("start"), py::arg("end"), R"doc(
      Replace this set with the characters in the specified range and return
      the set itself.

      *start* is the first character of the range, inclusive.

      *end* is the last character of the range, inclusive.

      If *end* is greater than *start*, the set is replaced with an empty
      range.
      )doc");

  us.def("set_to_bogus", &UnicodeSet::setToBogus, R"doc(
      Make this set invalid.

      .. seealso::

         :meth:`.is_bogus`
      )doc");

  us.def("size", &UnicodeSet::size, R"doc(
      Return the number of elements in this set.

      .. note::

         ``size()`` is slower than :meth:`.get_range_count` because it counts
         all code points in the range.

      .. seealso::

         :meth:`.__len__`
         :meth:`.get_range_count`
      )doc");

  us.def(
        "span",
        [](const UnicodeSet &self, const std::u16string &s, int32_t length,
           USetSpanCondition span_condition) {
          return self.span(s.data(), length, span_condition);
        },
        py::arg("s"), py::arg("length"), py::arg("span_condition"), R"doc(
      Return the end index of a substring of the specified string that contains
      (or does not contain) only the characters in this set, or 0 if such a
      substring is not found.

      *s* is a string that contains (or does not contain) only the characters
      to be searched for.

      *length* is the length of *s*; can be set to -1 for a NUL-terminated
      string.

      *span_condition* specifies the containment condition.
      )doc")
      .def("span",
           py::overload_cast<const UnicodeString &, int32_t, USetSpanCondition>(
               &UnicodeSet::span, py::const_),
           py::arg("s"), py::arg("start"), py::arg("span_condition"), R"doc(
      Return the end index of a substring of the specified string that contains
      (or does not contain) only the characters in this set, or 0 if such a
      substring is not found.

      *s* is a string that contains (or does not contain) only the characters
      to be searched for.

      *start* is the start index for search within a string.

      *span_condition* specifies the containment condition.
      )doc");

  us.def(
        "span_back",
        [](const UnicodeSet &self, const std::u16string &s, int32_t length,
           USetSpanCondition span_condition) {
          return self.spanBack(s.data(), length, span_condition);
        },
        py::arg("s"), py::arg("length"), py::arg("span_condition"), R"doc(
      Return the start index of the trailing substring of the specified string
      that contains (or does not contain) only the characters in this set, or
      the length of the specified string if such a substring is not found.

      *s* is a string that contains (or does not contain) only the characters
      to be searched for.

      *length* is the length of *s*; can be set to -1 for a NUL-terminated
      string.

      *span_condition* specifies the containment condition.
      )doc")
      .def("span_back",
           py::overload_cast<const UnicodeString &, int32_t, USetSpanCondition>(
               &UnicodeSet::spanBack, py::const_),
           py::arg("s"), py::arg("limit"), py::arg("span_condition"), R"doc(
      Return the start index of the trailing substring of the specified string
      that contains (or does not contain) only the characters in this set, or
      the length of the specified string if such a substring is not found.

      *s* is a string that contains (or does not contain) only the characters
      to be searched for.

      *limit* is the start index for the backward search within the string.
      Use *s.length()* or :attr:`INT32_MAX` to search from the end of the
      string.

      *span_condition* specifies the containment condition.
      )doc");

  us.def(
      "span_utf8",
      [](const UnicodeSet &self, const py::bytes &b, int32_t length,
         USetSpanCondition span_condition) {
        const auto b_text = b.cast<std::string>();
        if (length == -1) {
          length = static_cast<int32_t>(b_text.size());
        }
        return self.spanUTF8(b_text.data(), length, span_condition);
      },
      py::arg("b"), py::arg("length"), py::arg("span_condition"), R"doc(
      Return the end index of a substring of the specified string that contains
      (or does not contain) only the characters in this set, or 0 if such a
      substring is not found.

      *b* is a UTF-8 string that contains (or does not contain) only the
      characters to be searched for.

      *length* is the length of *b*; can be set to -1 for a NUL-terminated
      string.

      *span_condition* specifies the containment condition.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  us.def(
      "strings",
      [](const UnicodeSet &self) {
        auto it = self.strings();
        return py::make_iterator(it.begin(), it.end());
      },
      py::keep_alive<0, 1>(), R"doc(
      Return an iterator for iterating over the multi-character strings in this
      set.

      .. seealso::

         :meth:`.code_points`
         :meth:`.ranges`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  us.def(
      "to_uset",
      [](UnicodeSet &self) {
        auto uset = self.toUSet();
        return std::make_unique<icupy::USetPtr>(uset);
      },
      R"doc(
      Convert this set to a mutable ``USet`` object and return it.

      .. important::

         The set object must outlive the returned ``USet`` object.
      )doc");

  us.def_property_readonly_static("IGNORE_SPACE",
                                  [](const py::object & /* self */) -> int32_t {
                                    return USET_IGNORE_SPACE;
                                  });

  us.def_property_readonly_static("CASE_INSENSITIVE",
                                  [](const py::object & /* self */) -> int32_t {
                                    return USET_CASE_INSENSITIVE;
                                  });

  us.def_property_readonly_static("ADD_CASE_MAPPINGS",
                                  [](const py::object & /* self */) -> int32_t {
                                    return USET_ADD_CASE_MAPPINGS;
                                  });

  us.def_property_readonly_static(
      "SERIALIZED_STATIC_ARRAY_CAPACITY",
      [](const py::object & /* self */) -> int32_t {
        return USET_SERIALIZED_STATIC_ARRAY_CAPACITY;
      });
}

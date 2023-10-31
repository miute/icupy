#include "main.hpp"
#include "usetptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/parsepos.h>
#include <unicode/symtable.h>
#include <unicode/uniset.h>
#include <unicode/ustream.h>

using namespace icu;

void init_uniset(py::module &m) {
  //
  // icu::UMatchDegree
  //
  py::enum_<UMatchDegree>(m, "UMatchDegree", py::arithmetic(),
                          "Constants returned by *UnicodeMatcher::matches()* indicating the degree of match.")
      .value("U_MISMATCH", U_MISMATCH,
             "Constant returned by *matches()* indicating a mismatch between the text and this matcher.\n\n  "
             "The text contains a character which does not match, or the text does not contain all desired characters "
             "for a non-incremental match.")
      .value("U_PARTIAL_MATCH", U_PARTIAL_MATCH,
             "Constant returned by *matches()* indicating a partial match between the text and this matcher.\n\n  "
             "This value is only returned for incremental match operations. All characters of the text match, but more "
             "characters are required for a complete match. Alternatively, for variable-length matchers, all "
             "characters of the text match, and if more characters were supplied at limit, they might also match.")
      .value("U_MATCH", U_MATCH,
             "Constant returned by *matches()* indicating a complete match between the text and this matcher.\n\n  "
             "For an incremental variable-length match, this value is returned if the given text matches, and it is "
             "known that additional characters would not alter the extent of the match.")
      .export_values();

  //
  // icu::UnicodeFunctor
  //
  py::class_<UnicodeFunctor, UObject>(m, "UnicodeFunctor");

  //
  // icu::UnicodeMatcher
  //
  py::class_<UnicodeMatcher> umat(m, "UnicodeMatcher");

  //
  // icu::UnicodeFilter
  //
  py::class_<UnicodeFilter, UnicodeFunctor, UnicodeMatcher>(m, "UnicodeFilter");

  //
  // icu::UnicodeSet
  //
  py::class_<UnicodeSet, UnicodeFilter> us(m, "UnicodeSet");

  //
  // icu::UnicodeMatcher
  //
  umat.def("add_match_set_to", &UnicodeMatcher::addMatchSetTo, py::arg("to_union_to"));

  umat.def(
      "matches",
      [](UnicodeMatcher &self, const Replaceable &text, int32_t offset, int32_t limit, py::bool_ incremental) {
        auto result = self.matches(text, offset, limit, incremental);
        return py::make_tuple(result, offset);
      },
      py::arg("text"), py::arg("offset"), py::arg("limit"), py::arg("incremental"));

  umat.def(
      "to_pattern",
      [](const UnicodeMatcher &self, UnicodeString &result, py::bool_ escape_unprintable) -> UnicodeString & {
        return self.toPattern(result, escape_unprintable);
      },
      py::arg("result"), py::arg("escape_unprintable") = false);

  //
  // icu::UnicodeSet
  //
  py::enum_<decltype(UnicodeSet::MIN_VALUE)>(us, "UnicodeSet", py::arithmetic())
      .value("MIN_VALUE", UnicodeSet::MIN_VALUE, "Minimum value that can be stored in a *UnicodeSet*.")
      .value("MAX_VALUE", UnicodeSet::MAX_VALUE, "Maximum value that can be stored in a *UnicodeSet*.")
      .export_values();

  us.def(py::init<>())
      .def(py::init<UChar32, UChar32>(), py::arg("start"), py::arg("end"))
      .def(py::init([](const icupy::UnicodeStringVariant &pattern) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeSet>(icupy::to_unistr(pattern), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"))
      .def(py::init([](const icupy::UnicodeStringVariant &pattern, ParsePosition &pos, uint32_t options,
                       std::optional<const SymbolTable *> symbols) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeSet>(icupy::to_unistr(pattern), pos, options,
                                                        symbols.value_or(nullptr), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(py::init<const UnicodeSet &>(), py::arg("other"));

  us.def(
        "__contains__",
        [](const UnicodeSet &self, const icupy::UnicodeStringVariant &item) -> py::bool_ {
          return self.contains(icupy::to_unistr(item));
        },
        py::arg("item"))
      .def(
          "__contains__", [](const UnicodeSet &self, UChar32 item) -> py::bool_ { return self.contains(item); },
          py::arg("item"));

  us.def("__copy__", &UnicodeSet::clone);

  us.def(
      "__deepcopy__", [](const UnicodeSet &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  us.def(
        "__eq__", [](const UnicodeSet &self, const UnicodeSet &other) { return self == other; }, py::is_operator(),
        py::arg("other"))
      .def(
          "__eq__", [](const UnicodeSet &self, _ConstUSetPtr &other) { return uset_equals(self.toUSet(), other); },
          py::is_operator(), py::arg("other"))
      .def(
          "__eq__", [](const UnicodeSet &self, _USetPtr &other) { return uset_equals(self.toUSet(), other); },
          py::is_operator(), py::arg("other"));

  us.def(
        "__getitem__",
        [](const UnicodeSet &self, int32_t index) {
          const auto size = self.size();
          if (index < 0) {
            index += size;
          }
          if (index < 0 || index >= size) {
            throw py::index_error("characters index out of range: " + std::to_string(index));
          }
          return self.charAt(index);
        },
        py::arg("index"))
      .def(
          "__getitem__",
          [](const UnicodeSet &self, const py::slice &index) {
            size_t start, stop, step, slice_length;
            if (!index.compute(self.size(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }
            UnicodeSet result;
            for (size_t n = 0; n < slice_length; ++n) {
              result.add(self.charAt(static_cast<int32_t>(start)));
              start += step;
            }
            return result;
          },
          py::arg("index"));

  us.def("__hash__", &UnicodeSet::hashCode);

  us.def("__len__", &UnicodeSet::size);

  us.def(
        "__ne__", [](const UnicodeSet &self, const UnicodeSet &other) { return self != other; }, py::is_operator(),
        py::arg("other"))
      .def(
          "__ne__", [](const UnicodeSet &self, _ConstUSetPtr &other) { return !uset_equals(self.toUSet(), other); },
          py::is_operator(), py::arg("other"))
      .def(
          "__ne__", [](const UnicodeSet &self, _USetPtr &other) { return !uset_equals(self.toUSet(), other); },
          py::is_operator(), py::arg("other"));

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
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
          return self.add(icupy::to_unistr(s));
        },
        py::arg("s"))
      .def("add", py::overload_cast<UChar32>(&UnicodeSet::add), py::arg("c"))
      .def("add", py::overload_cast<UChar32, UChar32>(&UnicodeSet::add), py::arg("start"), py::arg("end"));

  us.def("add_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::addAll), py::arg("c"))
      .def(
          "add_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
            return self.addAll(icupy::to_unistr(s));
          },
          py::arg("s"));

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
      py::arg("prop"), py::arg("value"));

  us.def(
        "apply_pattern",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &pattern, ParsePosition &pos, uint32_t options,
           std::optional<const SymbolTable *> symbols) -> UnicodeSet & {
          ErrorCode error_code;
          auto &result =
              self.applyPattern(icupy::to_unistr(pattern), pos, options, symbols.value_or(nullptr), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(
          "apply_pattern",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &pattern) -> UnicodeSet & {
            ErrorCode error_code;
            auto &result = self.applyPattern(icupy::to_unistr(pattern), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));

  us.def(
      "apply_property_alias",
      [](UnicodeSet &self, const icupy::UnicodeStringVariant &prop,
         const icupy::UnicodeStringVariant &value) -> UnicodeSet & {
        ErrorCode error_code;
        auto &result = self.applyPropertyAlias(icupy::to_unistr(prop), icupy::to_unistr(value), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("prop"), py::arg("value"));

  us.def("char_at", &UnicodeSet::charAt, py::arg("index"));

  us.def("clear", &UnicodeSet::clear);

  us.def("clone", &UnicodeSet::clone);

  us.def("clone_as_thawed", &UnicodeSet::cloneAsThawed);

  us.def("close_over", &UnicodeSet::closeOver, py::arg("attribute"));

  us.def("compact", &UnicodeSet::compact);

  us.def(
        "complement",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
          return self.complement(icupy::to_unistr(s));
        },
        py::arg("s"))
      .def("complement", py::overload_cast<UChar32>(&UnicodeSet::complement), py::arg("c"))
      .def("complement", py::overload_cast<UChar32, UChar32>(&UnicodeSet::complement), py::arg("start"), py::arg("end"))
      .def("complement", py::overload_cast<>(&UnicodeSet::complement));

  us.def("complement_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::complementAll), py::arg("c"))
      .def(
          "complement_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
            return self.complementAll(icupy::to_unistr(s));
          },
          py::arg("s"));

  us.def(
        "contains",
        [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> py::bool_ {
          return self.contains(icupy::to_unistr(s));
        },
        py::arg("s"))
      .def(
          "contains", [](const UnicodeSet &self, UChar32 c) -> py::bool_ { return self.contains(c); }, py::arg("c"))
      .def(
          "contains",
          [](const UnicodeSet &self, UChar32 start, UChar32 end) -> py::bool_ { return self.contains(start, end); },
          py::arg("start"), py::arg("end"));

  us.def(
        "contains_all", [](const UnicodeSet &self, const UnicodeSet &c) -> py::bool_ { return self.containsAll(c); },
        py::arg("c"))
      .def(
          "contains_all",
          [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> py::bool_ {
            return self.containsAll(icupy::to_unistr(s));
          },
          py::arg("s"));

  us.def(
        "contains_none", [](const UnicodeSet &self, const UnicodeSet &c) -> py::bool_ { return self.containsNone(c); },
        py::arg("c"))
      .def(
          "contains_none",
          [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> py::bool_ {
            return self.containsNone(icupy::to_unistr(s));
          },
          py::arg("s"))
      .def(
          "contains_none",
          [](const UnicodeSet &self, UChar32 start, UChar32 end) -> py::bool_ { return self.containsNone(start, end); },
          py::arg("start"), py::arg("end"));

  us.def(
        "contains_some", [](const UnicodeSet &self, const UnicodeSet &c) -> py::bool_ { return self.containsSome(c); },
        py::arg("c"))
      .def(
          "contains_some",
          [](const UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> py::bool_ {
            return self.containsSome(icupy::to_unistr(s));
          },
          py::arg("s"))
      .def(
          "contains_some",
          [](const UnicodeSet &self, UChar32 start, UChar32 end) -> py::bool_ { return self.containsSome(start, end); },
          py::arg("start"), py::arg("end"));

  us.def_static(
      "create_from", [](const icupy::UnicodeStringVariant &s) { return UnicodeSet::createFrom(icupy::to_unistr(s)); },
      py::arg("s"));

  us.def_static(
      "create_from_all",
      [](const icupy::UnicodeStringVariant &s) { return UnicodeSet::createFromAll(icupy::to_unistr(s)); },
      py::arg("s"));

  us.def("freeze", &UnicodeSet::freeze);

  us.def_static(
        "from_uset", [](_ConstUSetPtr &uset) { return UnicodeSet::fromUSet(uset); }, py::return_value_policy::reference,
        py::arg("uset"))
      .def_static(
          "from_uset", [](_USetPtr &uset) { return UnicodeSet::fromUSet(uset); }, py::return_value_policy::reference,
          py::arg("uset"));

  us.def("get_range_count", &UnicodeSet::getRangeCount);

  us.def("get_range_end", &UnicodeSet::getRangeEnd, py::arg("index"));

  us.def("get_range_start", &UnicodeSet::getRangeStart, py::arg("index"));

  us.def("hash_code", &UnicodeSet::hashCode);

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  us.def("has_strings", [](const UnicodeSet &self) -> py::bool_ { return self.hasStrings(); });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

  us.def("index_of", &UnicodeSet::indexOf, py::arg("c"));

  us.def("is_bogus", [](const UnicodeSet &self) -> py::bool_ { return self.isBogus(); });

  us.def("is_empty", [](const UnicodeSet &self) -> py::bool_ { return self.isEmpty(); });

  us.def("is_frozen", [](const UnicodeSet &self) -> py::bool_ { return self.isFrozen(); });

  us.def(
        "remove",
        [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
          return self.remove(icupy::to_unistr(s));
        },
        py::arg("s"))
      .def("remove", py::overload_cast<UChar32>(&UnicodeSet::remove), py::arg("c"))
      .def("remove", py::overload_cast<UChar32, UChar32>(&UnicodeSet::remove), py::arg("start"), py::arg("end"));

  us.def("remove_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::removeAll), py::arg("c"))
      .def(
          "remove_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
            return self.removeAll(icupy::to_unistr(s));
          },
          py::arg("s"));

  us.def("remove_all_strings", &UnicodeSet::removeAllStrings);

  us.def_static(
      "resembles_pattern",
      [](const icupy::UnicodeStringVariant &pattern, int32_t pos) -> py::bool_ {
        return UnicodeSet::resemblesPattern(icupy::to_unistr(pattern), pos);
      },
      py::arg("pattern"), py::arg("pos"));

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  us.def(
      "retain",
      [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
        return self.retain(icupy::to_unistr(s));
      },
      py::arg("s"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  us.def("retain", py::overload_cast<UChar32>(&UnicodeSet::retain), py::arg("c"))
      .def("retain", py::overload_cast<UChar32, UChar32>(&UnicodeSet::retain), py::arg("start"), py::arg("end"));

  us.def("retain_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::retainAll), py::arg("c"))
      .def(
          "retain_all",
          [](UnicodeSet &self, const icupy::UnicodeStringVariant &s) -> UnicodeSet & {
            return self.retainAll(icupy::to_unistr(s));
          },
          py::arg("s"));

  us.def("serialize", [](const UnicodeSet &self) {
    ErrorCode error_code;
    const auto length = self.serialize(nullptr, 0, error_code);
    std::vector<uint16_t> result(length);
    error_code.reset();
    self.serialize(result.data(), length, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  us.def("set", &UnicodeSet::set, py::arg("start"), py::arg("end"));

  us.def("set_to_bogus", &UnicodeSet::setToBogus);

  us.def("size", &UnicodeSet::size);

  us.def("span", py::overload_cast<const char16_t *, int32_t, USetSpanCondition>(&UnicodeSet::span, py::const_),
         py::arg("s"), py::arg("length"), py::arg("span_condition"))
      .def("span", py::overload_cast<const UnicodeString &, int32_t, USetSpanCondition>(&UnicodeSet::span, py::const_),
           py::arg("s"), py::arg("start"), py::arg("span_condition"));

  us.def(
      "span_utf8",
      [](const UnicodeSet &self, const py::bytes &b, int32_t length, USetSpanCondition span_condition) {
        auto s = static_cast<std::string>(b);
        return self.spanUTF8(s.data(), length, span_condition);
      },
      py::arg("b"), py::arg("length"), py::arg("span_condition"));

  us.def("span_back",
         py::overload_cast<const char16_t *, int32_t, USetSpanCondition>(&UnicodeSet::spanBack, py::const_),
         py::arg("s"), py::arg("length"), py::arg("span_condition"))
      .def("span_back",
           py::overload_cast<const UnicodeString &, int32_t, USetSpanCondition>(&UnicodeSet::spanBack, py::const_),
           py::arg("s"), py::arg("limit"), py::arg("span_condition"));

  us.def("to_uset", [](UnicodeSet &self) {
    auto uset = self.toUSet();
    return std::make_unique<_USetPtr>(uset);
  });

  us.def_property_readonly_static("IGNORE_SPACE", [](const py::object &) -> int32_t { return USET_IGNORE_SPACE; });

  us.def_property_readonly_static("CASE_INSENSITIVE",
                                  [](const py::object &) -> int32_t { return USET_CASE_INSENSITIVE; });

  us.def_property_readonly_static("ADD_CASE_MAPPINGS",
                                  [](const py::object &) -> int32_t { return USET_ADD_CASE_MAPPINGS; });

  us.def_property_readonly_static("SERIALIZED_STATIC_ARRAY_CAPACITY",
                                  [](const py::object &) -> int32_t { return USET_SERIALIZED_STATIC_ARRAY_CAPACITY; });
}

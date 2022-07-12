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
  py::class_<UnicodeMatcher>(m, "UnicodeMatcher");

  //
  // icu::UnicodeFilter
  //
  py::class_<UnicodeFilter, UnicodeFunctor, UnicodeMatcher>(m, "UnicodeFilter");

  //
  // icu::UnicodeSet
  //
  py::class_<UnicodeSet, UnicodeFilter> us(m, "UnicodeSet");

  py::enum_<decltype(UnicodeSet::MIN_VALUE)>(us, "UnicodeSet", py::arithmetic())
      .value("MIN_VALUE", UnicodeSet::MIN_VALUE, "Minimum value that can be stored in a *UnicodeSet*.")
      .value("MAX_VALUE", UnicodeSet::MAX_VALUE, "Maximum value that can be stored in a *UnicodeSet*.")
      .export_values();

  us.def(py::init<>())
      .def(py::init<UChar32, UChar32>(), py::arg("start"), py::arg("end"))
      .def(py::init([](const _UnicodeStringVariant &pattern) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeSet>(VARIANT_TO_UNISTR(pattern), error_code);
             if (error_code.isFailure()) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"))
      .def(py::init([](const _UnicodeStringVariant &pattern, ParsePosition &pos, uint32_t options,
                       std::optional<const SymbolTable *> symbols) {
             ErrorCode error_code;
             auto result = std::make_unique<UnicodeSet>(VARIANT_TO_UNISTR(pattern), pos, options,
                                                        symbols.value_or(nullptr), error_code);
             if (error_code.isFailure()) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(py::init<const UnicodeSet &>(), py::arg("other"));

  us.def(
        "__contains__",
        [](const UnicodeSet &self, const _UnicodeStringVariant &item) {
          return self.contains(VARIANT_TO_UNISTR(item));
        },
        py::arg("item"))
      .def(
          "__contains__", [](const UnicodeSet &self, UChar32 item) { return self.contains(item); }, py::arg("item"));

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
          if (index < 0) {
            index += self.size();
          }
          if (index < 0 || index >= self.size()) {
            throw py::index_error(std::to_string(index));
          }
          return self.charAt(index);
        },
        py::arg("index"))
      .def(
          "__getitem__",
          [](const UnicodeSet &self, py::slice slice) {
            size_t start, stop, step, slice_length;
            if (!slice.compute(self.size(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }
            std::vector<UChar32> result(slice_length);
            for (size_t n = 0; n < slice_length; ++n) {
              result[n] = self.charAt(static_cast<int32_t>(start));
              start += step;
            }
            return result;
          },
          py::arg("slice"));

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
        [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & { return self.add(VARIANT_TO_UNISTR(s)); },
        py::arg("s"))
      .def("add", py::overload_cast<UChar32>(&UnicodeSet::add), py::arg("c"))
      .def("add", py::overload_cast<UChar32, UChar32>(&UnicodeSet::add), py::arg("start"), py::arg("end"));

  us.def("add_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::addAll), py::arg("c"))
      .def(
          "add_all",
          [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
            return self.addAll(VARIANT_TO_UNISTR(s));
          },
          py::arg("s"));

  us.def("add_match_set_to", &UnicodeSet::addMatchSetTo, py::arg("to_union_to"));

  us.def(
      "apply_int_property_value",
      [](UnicodeSet &self, UProperty prop, int32_t value) -> UnicodeSet & {
        ErrorCode error_code;
        auto &result = self.applyIntPropertyValue(prop, value, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("prop"), py::arg("value"));

  us.def(
        "apply_pattern",
        [](UnicodeSet &self, const _UnicodeStringVariant &pattern, ParsePosition &pos, uint32_t options,
           std::optional<const SymbolTable *> symbols) -> UnicodeSet & {
          ErrorCode error_code;
          auto &result =
              self.applyPattern(VARIANT_TO_UNISTR(pattern), pos, options, symbols.value_or(nullptr), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(
          "apply_pattern",
          [](UnicodeSet &self, const _UnicodeStringVariant &pattern) -> UnicodeSet & {
            ErrorCode error_code;
            auto &result = self.applyPattern(VARIANT_TO_UNISTR(pattern), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));

  us.def(
      "apply_property_alias",
      [](UnicodeSet &self, const _UnicodeStringVariant &prop, const _UnicodeStringVariant &value) -> UnicodeSet & {
        ErrorCode error_code;
        auto &result = self.applyPropertyAlias(VARIANT_TO_UNISTR(prop), VARIANT_TO_UNISTR(value), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
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
        [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
          return self.complement(VARIANT_TO_UNISTR(s));
        },
        py::arg("s"))
      .def("complement", py::overload_cast<UChar32>(&UnicodeSet::complement), py::arg("c"))
      .def("complement", py::overload_cast<UChar32, UChar32>(&UnicodeSet::complement), py::arg("start"), py::arg("end"))
      .def("complement", py::overload_cast<>(&UnicodeSet::complement));

  us.def("complement_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::complementAll), py::arg("c"))
      .def(
          "complement_all",
          [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
            return self.complementAll(VARIANT_TO_UNISTR(s));
          },
          py::arg("s"));

  us.def(
        "contains",
        [](const UnicodeSet &self, const _UnicodeStringVariant &s) { return self.contains(VARIANT_TO_UNISTR(s)); },
        py::arg("s"))
      .def("contains", py::overload_cast<UChar32>(&UnicodeSet::contains, py::const_), py::arg("c"))
      .def("contains", py::overload_cast<UChar32, UChar32>(&UnicodeSet::contains, py::const_), py::arg("start"),
           py::arg("end"));

  us.def("contains_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::containsAll, py::const_), py::arg("c"))
      .def(
          "contains_all",
          [](const UnicodeSet &self, const _UnicodeStringVariant &s) { return self.containsAll(VARIANT_TO_UNISTR(s)); },
          py::arg("s"));

  us.def("contains_none", py::overload_cast<const UnicodeSet &>(&UnicodeSet::containsNone, py::const_), py::arg("c"))
      .def(
          "contains_none",
          [](const UnicodeSet &self, const _UnicodeStringVariant &s) {
            return self.containsNone(VARIANT_TO_UNISTR(s));
          },
          py::arg("s"))
      .def("contains_none", py::overload_cast<UChar32, UChar32>(&UnicodeSet::containsNone, py::const_),
           py::arg("start"), py::arg("end"));

  us.def("contains_some", py::overload_cast<const UnicodeSet &>(&UnicodeSet::containsSome, py::const_), py::arg("c"))
      .def(
          "contains_some",
          [](const UnicodeSet &self, const _UnicodeStringVariant &s) {
            return self.containsSome(VARIANT_TO_UNISTR(s));
          },
          py::arg("s"))
      .def("contains_some", py::overload_cast<UChar32, UChar32>(&UnicodeSet::containsSome, py::const_),
           py::arg("start"), py::arg("end"));

  us.def_static(
      "create_from", [](const _UnicodeStringVariant &s) { return UnicodeSet::createFrom(VARIANT_TO_UNISTR(s)); },
      py::arg("s"));

  us.def_static(
      "create_from_all", [](const _UnicodeStringVariant &s) { return UnicodeSet::createFromAll(VARIANT_TO_UNISTR(s)); },
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
  us.def("has_strings", &UnicodeSet::hasStrings);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)

  us.def("index_of", &UnicodeSet::indexOf, py::arg("c"));

  us.def("is_bogus", &UnicodeSet::isBogus);

  us.def("is_empty", &UnicodeSet::isEmpty);

  us.def("is_frozen", &UnicodeSet::isFrozen);

  us.def(
      "matches",
      [](UnicodeSet &self, const Replaceable &text, int32_t offset, int32_t limit, UBool incremental) {
        auto result = self.matches(text, offset, limit, incremental);
        return py::make_tuple(result, offset);
      },
      py::arg("text"), py::arg("offset"), py::arg("limit"), py::arg("incremental"));

  us.def(
        "remove",
        [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
          return self.remove(VARIANT_TO_UNISTR(s));
        },
        py::arg("s"))
      .def("remove", py::overload_cast<UChar32>(&UnicodeSet::remove), py::arg("c"))
      .def("remove", py::overload_cast<UChar32, UChar32>(&UnicodeSet::remove), py::arg("start"), py::arg("end"));

  us.def("remove_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::removeAll), py::arg("c"))
      .def(
          "remove_all",
          [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
            return self.removeAll(VARIANT_TO_UNISTR(s));
          },
          py::arg("s"));

  us.def("remove_all_strings", &UnicodeSet::removeAllStrings);

  us.def_static(
      "resembles_pattern",
      [](const _UnicodeStringVariant &pattern, int32_t pos) {
        return UnicodeSet::resemblesPattern(VARIANT_TO_UNISTR(pattern), pos);
      },
      py::arg("pattern"), py::arg("pos"));

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  us.def(
      "retain",
      [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
        return self.retain(VARIANT_TO_UNISTR(s));
      },
      py::arg("s"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  us.def("retain", py::overload_cast<UChar32>(&UnicodeSet::retain), py::arg("c"))
      .def("retain", py::overload_cast<UChar32, UChar32>(&UnicodeSet::retain), py::arg("start"), py::arg("end"));

  us.def("retain_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::retainAll), py::arg("c"))
      .def(
          "retain_all",
          [](UnicodeSet &self, const _UnicodeStringVariant &s) -> UnicodeSet & {
            return self.retainAll(VARIANT_TO_UNISTR(s));
          },
          py::arg("s"));

  us.def("serialize", [](const UnicodeSet &self) {
    ErrorCode error_code;
    const auto length = self.serialize(nullptr, 0, error_code);
    std::vector<uint16_t> result(length);
    error_code.reset();
    self.serialize(result.data(), length, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
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

  us.def("span_back",
         py::overload_cast<const char16_t *, int32_t, USetSpanCondition>(&UnicodeSet::spanBack, py::const_),
         py::arg("s"), py::arg("length"), py::arg("span_condition"))
      .def("span_back",
           py::overload_cast<const UnicodeString &, int32_t, USetSpanCondition>(&UnicodeSet::spanBack, py::const_),
           py::arg("s"), py::arg("limit"), py::arg("span_condition"));

  us.def("to_pattern", &UnicodeSet::toPattern, py::arg("result"), py::arg("escape_unprintable") = false);

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

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
  // icu::UMatchDegree
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

  // icu::UnicodeFunctor
  py::class_<UnicodeFunctor, UObject>(m, "UnicodeFunctor");

  // icu::UnicodeMatcher
  py::class_<UnicodeMatcher>(m, "UnicodeMatcher");

  // icu::UnicodeFilter
  py::class_<UnicodeFilter, UnicodeFunctor, UnicodeMatcher>(m, "UnicodeFilter");

  // icu::UnicodeSet
  py::class_<UnicodeSet, UnicodeFilter> us(m, "UnicodeSet");

  py::enum_<decltype(UnicodeSet::MIN_VALUE)>(us, "UnicodeSet", py::arithmetic())
      .value("MIN_VALUE", UnicodeSet::MIN_VALUE, "Minimum value that can be stored in a *UnicodeSet*.")
      .value("MAX_VALUE", UnicodeSet::MAX_VALUE, "Maximum value that can be stored in a *UnicodeSet*.")
      .export_values();

  us.def(py::init<>())
      .def(py::init<UChar32, UChar32>(), py::arg("start"), py::arg("end"))
      .def(py::init([](const UnicodeString &pattern) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<UnicodeSet>(pattern, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<UnicodeSet>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(py::init([](const UnicodeString &pattern, ParsePosition &pos, uint32_t options,
                       std::optional<const SymbolTable *> symbols) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<UnicodeSet>(pattern, pos, options, symbols.value_or(nullptr), error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, ParsePosition &pos, uint32_t options,
                      std::optional<const SymbolTable *> symbols) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<UnicodeSet>(pattern, pos, options, symbols.value_or(nullptr), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(py::init<const UnicodeSet &>(), py::arg("o"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  us.def(
        "__contains__", [](const UnicodeSet &self, const UnicodeString &item) { return self.contains(item); },
        py::arg("item"))
      .def(
          "__contains__", [](const UnicodeSet &self, const char16_t *item) { return self.contains(item); },
          py::arg("item"))
      .def(
          "__contains__", [](const UnicodeSet &self, UChar32 item) { return self.contains(item); }, py::arg("item"))
      .def("__copy__", &UnicodeSet::clone)
      .def(
          "__deepcopy__", [](const UnicodeSet &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const UnicodeSet &self, _ConstUSetPtr &other) { return self.toUSet() == other; },
          py::is_operator(), py::arg("other"))
      .def(
          "__eq__", [](const UnicodeSet &self, _USetPtr &other) { return self.toUSet() == other; }, py::is_operator(),
          py::arg("other"))
      .def(
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
          py::arg("slice"))
      .def("__len__", &UnicodeSet::size)
      .def("__repr__", [](const UnicodeSet &self) {
        std::stringstream ss;
        ss << "UnicodeSet(";
        UnicodeString pattern;
        self.toPattern(pattern, true);
        ss << pattern;
        ss << ")";
        return ss.str();
      });
  us.def("add", py::overload_cast<const UnicodeString &>(&UnicodeSet::add), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "add", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.add(s); }, py::arg("s"))
      .def("add", py::overload_cast<UChar32>(&UnicodeSet::add), py::arg("c"))
      .def("add", py::overload_cast<UChar32, UChar32>(&UnicodeSet::add), py::arg("start"), py::arg("end"));
  us.def("add_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::addAll), py::arg("c"))
      .def("add_all", py::overload_cast<const UnicodeString &>(&UnicodeSet::addAll), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "add_all", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.addAll(s); }, py::arg("s"));
  us.def("add_match_set_to", &UnicodeSet::addMatchSetTo, py::arg("to_union_to"));
  us.def(
      "apply_int_property_value",
      [](UnicodeSet &self, UProperty prop, int32_t value) -> UnicodeSet & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.applyIntPropertyValue(prop, value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("prop"), py::arg("value"));
  us.def(
        "apply_pattern",
        [](UnicodeSet &self, const UnicodeString &pattern, ParsePosition &pos, uint32_t options,
           std::optional<const SymbolTable *> symbols) -> UnicodeSet & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.applyPattern(pattern, pos, options, symbols.value_or(nullptr), error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_pattern",
          [](UnicodeSet &self, const char16_t *pattern, ParsePosition &pos, uint32_t options,
             std::optional<const SymbolTable *> symbols) -> UnicodeSet & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.applyPattern(pattern, pos, options, symbols.value_or(nullptr), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("pos"), py::arg("options"), py::arg("symbols"))
      .def(
          "apply_pattern",
          [](UnicodeSet &self, const UnicodeString &pattern) -> UnicodeSet & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.applyPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_pattern",
          [](UnicodeSet &self, const char16_t *pattern) -> UnicodeSet & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.applyPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"));
  us.def(
        "apply_property_alias",
        [](UnicodeSet &self, const UnicodeString &prop, const UnicodeString &value) -> UnicodeSet & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.applyPropertyAlias(prop, value, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("prop"), py::arg("value"))
      .def(
          // const char16_t *prop -> const UnicodeString &prop
          "apply_property_alias",
          [](UnicodeSet &self, const char16_t *prop, const UnicodeString &value) -> UnicodeSet & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.applyPropertyAlias(prop, value, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("prop"), py::arg("value"))
      .def(
          // const char16_t *value -> const UnicodeString &value
          "apply_property_alias",
          [](UnicodeSet &self, const UnicodeString &prop, const char16_t *value) -> UnicodeSet & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.applyPropertyAlias(prop, value, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("prop"), py::arg("value"))
      .def(
          // const char16_t *prop -> const UnicodeString &prop
          // const char16_t *value -> const UnicodeString &value
          "apply_property_alias",
          [](UnicodeSet &self, const char16_t *prop, const char16_t *value) -> UnicodeSet & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.applyPropertyAlias(prop, value, error_code);
            if (U_FAILURE(error_code)) {
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
  us.def("complement", py::overload_cast<const UnicodeString &>(&UnicodeSet::complement), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "complement", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.complement(s); },
          py::arg("s"))
      .def("complement", py::overload_cast<UChar32>(&UnicodeSet::complement), py::arg("c"))
      .def("complement", py::overload_cast<UChar32, UChar32>(&UnicodeSet::complement), py::arg("start"), py::arg("end"))
      .def("complement", py::overload_cast<>(&UnicodeSet::complement));
  us.def("complement_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::complementAll), py::arg("c"))
      .def("complement_all", py::overload_cast<const UnicodeString &>(&UnicodeSet::complementAll), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "complement_all", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.complementAll(s); },
          py::arg("s"));
  us.def("contains", py::overload_cast<const UnicodeString &>(&UnicodeSet::contains, py::const_), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "contains", [](const UnicodeSet &self, const char16_t *s) { return self.contains(s); }, py::arg("s"))
      .def("contains", py::overload_cast<UChar32>(&UnicodeSet::contains, py::const_), py::arg("c"))
      .def("contains", py::overload_cast<UChar32, UChar32>(&UnicodeSet::contains, py::const_), py::arg("start"),
           py::arg("end"));
  us.def("contains_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::containsAll, py::const_), py::arg("c"))
      .def("contains_all", py::overload_cast<const UnicodeString &>(&UnicodeSet::containsAll, py::const_), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "contains_all", [](const UnicodeSet &self, const char16_t *s) { return self.containsAll(s); }, py::arg("s"));
  us.def("contains_none", py::overload_cast<const UnicodeSet &>(&UnicodeSet::containsNone, py::const_), py::arg("c"))
      .def("contains_none", py::overload_cast<const UnicodeString &>(&UnicodeSet::containsNone, py::const_),
           py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "contains_none", [](const UnicodeSet &self, const char16_t *s) { return self.containsNone(s); }, py::arg("s"))
      .def("contains_none", py::overload_cast<UChar32, UChar32>(&UnicodeSet::containsNone, py::const_),
           py::arg("start"), py::arg("end"));
  us.def("contains_some", py::overload_cast<const UnicodeSet &>(&UnicodeSet::containsSome, py::const_), py::arg("c"))
      .def("contains_some", py::overload_cast<const UnicodeString &>(&UnicodeSet::containsSome, py::const_),
           py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "contains_some", [](const UnicodeSet &self, const char16_t *s) { return self.containsSome(s); }, py::arg("s"))
      .def("contains_some", py::overload_cast<UChar32, UChar32>(&UnicodeSet::containsSome, py::const_),
           py::arg("start"), py::arg("end"));
  us.def_static("create_from", &UnicodeSet::createFrom, py::arg("s"))
      .def_static(
          // const char16_t *s -> const UnicodeString &s
          "create_from", [](const char16_t *s) { return UnicodeSet::createFrom(s); }, py::arg("s"));
  us.def_static("create_from_all", &UnicodeSet::createFromAll, py::arg("s"))
      .def_static(
          // const char16_t *s -> const UnicodeString &s
          "create_from_all", [](const char16_t *s) { return UnicodeSet::createFromAll(s); }, py::arg("s"));
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
  us.def("remove", py::overload_cast<const UnicodeString &>(&UnicodeSet::remove), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "remove", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.remove(s); }, py::arg("s"))
      .def("remove", py::overload_cast<UChar32>(&UnicodeSet::remove), py::arg("c"))
      .def("remove", py::overload_cast<UChar32, UChar32>(&UnicodeSet::remove), py::arg("start"), py::arg("end"));
  us.def("remove_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::removeAll), py::arg("c"))
      .def("remove_all", py::overload_cast<const UnicodeString &>(&UnicodeSet::removeAll), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "remove_all", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.removeAll(s); },
          py::arg("s"));
  us.def("remove_all_strings", &UnicodeSet::removeAllStrings);
  us.def_static("resembles_pattern", &UnicodeSet::resemblesPattern, py::arg("pattern"), py::arg("pos"))
      .def_static(
          // const char16_t *pattern -> const UnicodeString &pattern
          "resembles_pattern",
          [](const char16_t *pattern, int32_t pos) { return UnicodeSet::resemblesPattern(pattern, pos); },
          py::arg("pattern"), py::arg("pos"));
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  us.def("retain", py::overload_cast<const UnicodeString &>(&UnicodeSet::retain), py::arg("s"))
      .def(
          "retain", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.retain(s); }, py::arg("s"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
  us.def("retain", py::overload_cast<UChar32>(&UnicodeSet::retain), py::arg("c"))
      .def("retain", py::overload_cast<UChar32, UChar32>(&UnicodeSet::retain), py::arg("start"), py::arg("end"));
  us.def("retain_all", py::overload_cast<const UnicodeSet &>(&UnicodeSet::retainAll), py::arg("c"))
      .def("retain_all", py::overload_cast<const UnicodeString &>(&UnicodeSet::retainAll), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "retain_all", [](UnicodeSet &self, const char16_t *s) -> UnicodeSet & { return self.retainAll(s); },
          py::arg("s"));
  us.def("serialize", [](const UnicodeSet &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    const auto length = self.serialize(nullptr, 0, error_code);
    std::vector<uint16_t> result(length);
    error_code = U_ZERO_ERROR;
    self.serialize(result.data(), length, error_code);
    if (U_FAILURE(error_code)) {
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
    }).def("to_uset", [](const UnicodeSet &self) {
    auto uset = self.toUSet();
    return std::make_unique<_ConstUSetPtr>(uset);
  });

  us.def_property_readonly_static("IGNORE_SPACE", [](py::object) -> int32_t { return USET_IGNORE_SPACE; });
  us.def_property_readonly_static("CASE_INSENSITIVE", [](py::object) -> int32_t { return USET_CASE_INSENSITIVE; });
  us.def_property_readonly_static("ADD_CASE_MAPPINGS", [](py::object) -> int32_t { return USET_ADD_CASE_MAPPINGS; });

  us.def_property_readonly_static("SERIALIZED_STATIC_ARRAY_CAPACITY",
                                  [](py::object) -> int32_t { return USET_SERIALIZED_STATIC_ARRAY_CAPACITY; });
}

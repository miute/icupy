#include "main.hpp"
#include <algorithm>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/chariter.h>
#include <unicode/coleitr.h>
#include <unicode/sortkey.h>
#include <unicode/strenum.h>
#include <unicode/tblcoll.h>

using namespace icu;

void init_tblcoll(py::module &m) {
  //
  // icu::Collator
  //
  py::class_<Collator, UObject> coll(m, "Collator");

  //
  // icu::Collator::ECollationStrength
  //
  py::enum_<Collator::ECollationStrength>(
      coll, "ECollationStrength", py::arithmetic(),
      "Base letter represents a primary difference.\n\n"
      "Set comparison level to PRIMARY to ignore secondary and tertiary differences. Use this to set the strength of "
      "a Collator object.\n"
      "Example of primary difference, \"abc\" < \"abd\"\n\n"
      "Diacritical differences on the same base letter represent a secondary difference. Set comparison level to "
      "SECONDARY to ignore tertiary differences. Use this to set the strength of a Collator object.\n"
      "Example of secondary difference, \"&auml;\" >> \"a\".\n\n"
      "Uppercase and lowercase versions of the same character represents a tertiary difference. Set comparison level "
      "to TERTIARY to include all comparison differences. Use this to set the strength of a Collator object.\n"
      "Example of tertiary difference, \"abc\" <<< \"ABC\".\n\n"
      "Two characters are considered \"identical\" when they have the same unicode spellings.\n"
      "For example, \"&auml;\" == \"&auml;\".\n\n"
      "*UCollationStrength* is also used to determine the strength of sort keys generated from Collator objects.")
      .value("PRIMARY", Collator::PRIMARY)
      .value("SECONDARY", Collator::SECONDARY)
      .value("TERTIARY", Collator::TERTIARY)
      .value("QUATERNARY", Collator::QUATERNARY)
      .value("IDENTICAL", Collator::IDENTICAL)
      .export_values();

#ifndef U_FORCE_HIDE_DEPRECATED_API
  //
  // icu::Collator::EComparisonResult
  //
  py::enum_<Collator::EComparisonResult>(coll, "EComparisonResult", py::arithmetic(),
                                         "**Deprecated:** ICU 2.6. Use C enum UCollationResult defined in ucol.h")
      .value("LESS", Collator::LESS)
      .value("EQUAL", Collator::EQUAL)
      .value("GREATER", Collator::GREATER)
      .export_values();
#endif // U_FORCE_HIDE_DEPRECATED_API

  //
  // icu::Collator
  //
  coll.def("__copy__", &Collator::clone);

  coll.def(
      "__deepcopy__", [](const Collator &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  coll.def(
      "__eq__", [](const Collator &self, const Collator &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  coll.def("__hash__", &Collator::hashCode);

  coll.def(
      "__ne__", [](const Collator &self, const Collator &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  coll.def("clone", &Collator::clone);

  coll.def(
          // [2] icu::Collator::compare
          "compare",
          [](const Collator &self, const char16_t *source, int32_t source_length, const char16_t *target,
             int32_t target_length) {
            ErrorCode error_code;
            auto result = self.compare(source, source_length, target, target_length, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("source_length"), py::arg("target"), py::arg("target_length"))
      .def(
          // [5] icu::Collator::compare
          "compare",
          [](const Collator &self, const icupy::UnicodeStringVariant &source, const icupy::UnicodeStringVariant &target,
             int32_t length) {
            ErrorCode error_code;
            auto result = self.compare(icupy::to_unistr(source), icupy::to_unistr(target), length, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), py::arg("length"))
      .def(
          // [6] icu::Collator::compare
          "compare",
          [](const Collator &self, const icupy::UnicodeStringVariant &source,
             const icupy::UnicodeStringVariant &target) {
            ErrorCode error_code;
            auto result = self.compare(icupy::to_unistr(source), icupy::to_unistr(target), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"))
      // FIXME: Implement uiter C API.
      /*
      .def(
          // [7] icu::Collator::compare
          "compare",
          [](const Collator &self, UCharIterator &s_iter, UCharIterator &t_iter) {
            ErrorCode error_code;
            auto result = self.compare(s_iter, t_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("s_iter"), py::arg("t_iter"))
      */
      ;

  coll.def(
      "compare_utf8",
      [](Collator &self, const py::bytes &source, const py::bytes &target) {
        ErrorCode error_code;
        auto result = self.compareUTF8(StringPiece(source), StringPiece(target), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("source"), py::arg("target"));

  coll.def_static(
          "create_instance",
          [](const icupy::LocaleVariant &loc) {
            ErrorCode error_code;
            auto result = Collator::createInstance(icupy::to_locale(loc), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("loc"))
      .def_static("create_instance", []() {
        ErrorCode error_code;
        auto result = Collator::createInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  coll.def(
      "equals",
      [](const Collator &self, const icupy::UnicodeStringVariant &source, const icupy::UnicodeStringVariant &target)
          -> py::bool_ { return self.equals(icupy::to_unistr(source), icupy::to_unistr(target)); },
      py::arg("source"), py::arg("target"));

  coll.def(
      "get_attribute",
      [](const Collator &self, UColAttribute attr) {
        ErrorCode error_code;
        auto result = self.getAttribute(attr, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"));

  coll.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = Collator::getAvailableLocales(count);
        std::vector<const Locale *> result(count, nullptr);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);

  coll.def_static(
      "get_bound",
      [](const py::buffer &source, int32_t source_length, UColBoundMode bound_type, uint32_t no_of_levels) {
        auto info = source.request();
        if (source_length == -1) {
          source_length = static_cast<int32_t>(info.size);
        }
        auto _source = reinterpret_cast<const uint8_t *>(info.ptr);
        ErrorCode error_code;
        const auto result_length =
            Collator::getBound(_source, source_length, bound_type, no_of_levels, nullptr, 0, error_code);
        std::vector<uint8_t> result(result_length);
        error_code.reset();
        Collator::getBound(_source, source_length, bound_type, no_of_levels, result.data(), result_length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(reinterpret_cast<char *>(result.data()), result_length);
      },
      py::arg("source"), py::arg("source_length"), py::arg("bound_type"), py::arg("no_of_levels"));

  coll.def(
          "get_collation_key",
          [](const Collator &self, const char16_t *source, int32_t source_length, CollationKey &key) -> CollationKey & {
            ErrorCode error_code;
            auto &result = self.getCollationKey(source, source_length, key, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("source_length"), py::arg("key"))
      .def(
          "get_collation_key",
          [](const Collator &self, const icupy::UnicodeStringVariant &source, CollationKey &key) -> CollationKey & {
            ErrorCode error_code;
            auto &result = self.getCollationKey(icupy::to_unistr(source), key, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("key"));

  coll.def_static(
          "get_display_name",
          [](const icupy::LocaleVariant &object_locale, const icupy::LocaleVariant &display_locale,
             UnicodeString &name) -> UnicodeString & {
            return Collator::getDisplayName(icupy::to_locale(object_locale), icupy::to_locale(display_locale), name);
          },
          py::arg("object_locale"), py::arg("display_locale"), py::arg("name"))
      .def_static(
          "get_display_name",
          [](const icupy::LocaleVariant &object_locale, UnicodeString &name) -> UnicodeString & {
            return Collator::getDisplayName(icupy::to_locale(object_locale), name);
          },
          py::arg("object_locale"), py::arg("name"));

  coll.def_static(
      "get_equivalent_reorder_codes",
      [](int32_t reorder_code) {
        ErrorCode error_code;
        const auto dest_capacity = Collator::getEquivalentReorderCodes(reorder_code, nullptr, 0, error_code);
        std::vector<int32_t> result(dest_capacity);
        error_code.reset();
        Collator::getEquivalentReorderCodes(reorder_code, result.data(), dest_capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("reorder_code"));

  coll.def_static(
      "get_functional_equivalent",
      [](const char *keyword, const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        UBool is_available = true;
        auto result = Collator::getFunctionalEquivalent(keyword, icupy::to_locale(locale), is_available, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::make_tuple(result, py::bool_(is_available));
      },
      py::arg("keyword"), py::arg("locale"));

  coll.def_static("get_keywords", []() {
    ErrorCode error_code;
    auto result = Collator::getKeywords(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  coll.def_static(
      "get_keyword_values",
      [](const char *keyword) {
        ErrorCode error_code;
        auto result = Collator::getKeywordValues(keyword, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword"));

  coll.def_static(
      "get_keyword_values_for_locale",
      [](const char *keyword, const icupy::LocaleVariant &locale, py::bool_ commonly_used) {
        ErrorCode error_code;
        auto result = Collator::getKeywordValuesForLocale(keyword, icupy::to_locale(locale), commonly_used, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword"), py::arg("locale"), py::arg("commonly_used"));

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  coll.def("get_max_variable", &Collator::getMaxVariable);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  coll.def("get_reorder_codes", [](const Collator &self) {
    ErrorCode error_code;
    const auto dest_capacity = self.getReorderCodes(nullptr, 0, error_code);
    error_code.reset();
    std::vector<int32_t> result(dest_capacity);
    self.getReorderCodes(result.data(), dest_capacity, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  coll.def(
          "get_sort_key",
          [](const Collator &self, const char16_t *source, int32_t source_length) {
            const auto result_length = self.getSortKey(source, source_length, nullptr, 0);
            std::vector<uint8_t> result(result_length);
            self.getSortKey(source, source_length, result.data(), result_length);
            return py::bytes(reinterpret_cast<char *>(result.data()), result_length);
          },
          py::arg("source"), py::arg("source_length"))
      .def(
          "get_sort_key",
          [](const Collator &self, const icupy::UnicodeStringVariant &source) {
            const auto result_length = self.getSortKey(icupy::to_unistr(source), nullptr, 0);
            std::vector<uint8_t> result(result_length);
            self.getSortKey(icupy::to_unistr(source), result.data(), result_length);
            return py::bytes(reinterpret_cast<char *>(result.data()), result_length);
          },
          py::arg("source"));

  coll.def("get_tailored_set", [](const Collator &self) {
    ErrorCode error_code;
    auto result = self.getTailoredSet(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  coll.def("get_variable_top", [](const Collator &self) {
    ErrorCode error_code;
    auto result = self.getVariableTop(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  coll.def("get_version", [](const Collator &self) {
    UVersionInfo info;
    self.getVersion(info);
    py::tuple result(U_MAX_VERSION_LENGTH);
    int n = 0;
    std::for_each(std::begin(info), std::end(info), [&](auto v) { result[n++] = v; });
    return result;
  });

  coll.def(
      "greater",
      [](const Collator &self, const icupy::UnicodeStringVariant &source, const icupy::UnicodeStringVariant &target)
          -> py::bool_ { return self.greater(icupy::to_unistr(source), icupy::to_unistr(target)); },
      py::arg("source"), py::arg("target"));

  coll.def(
      "greater_or_equal",
      [](const Collator &self, const icupy::UnicodeStringVariant &source, const icupy::UnicodeStringVariant &target)
          -> py::bool_ { return self.greaterOrEqual(icupy::to_unistr(source), icupy::to_unistr(target)); },
      py::arg("source"), py::arg("target"));

  coll.def("hash_code", &Collator::hashCode);

  // TODO: Implement "static URegistryKey icu::Collator::registerFactory(CollatorFactory *toAdopt, UErrorCode &status)".
  // TODO: Implement "static URegistryKey icu::Collator::registerInstance(Collator *toAdopt, const Locale &locale,
  //  UErrorCode &status)".

  coll.def(
      "set_attribute",
      [](Collator &self, UColAttribute attr, UColAttributeValue value) {
        ErrorCode error_code;
        self.setAttribute(attr, value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("attr"), py::arg("value"));

  coll.def(
      "set_max_variable",
      [](Collator &self, UColReorderCode group) -> Collator & {
        ErrorCode error_code;
        auto &result = self.setMaxVariable(group, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("group"));

  coll.def(
      "set_reorder_codes",
      [](Collator &self, const std::vector<int32_t> &reorder_codes, int32_t reorder_codes_length) {
        ErrorCode error_code;
        self.setReorderCodes(reorder_codes.data(), reorder_codes_length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("reorder_codes"), py::arg("reorder_codes_length"));

  // TODO: Implement "static UBool icu::Collator::unregister(URegistryKey key, UErrorCode &status)".

  //
  // icu::RuleBasedCollator
  //
  py::class_<RuleBasedCollator, Collator> rbc(m, "RuleBasedCollator");

  rbc.def(py::init([](const icupy::UnicodeStringVariant &rules) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedCollator>(icupy::to_unistr(rules), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"))
      .def(py::init([](const icupy::UnicodeStringVariant &rules, Collator::ECollationStrength collation_strength) {
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(icupy::to_unistr(rules), collation_strength, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("collation_strength"))
      .def(py::init([](const icupy::UnicodeStringVariant &rules, UColAttributeValue decomposition_mode) {
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(icupy::to_unistr(rules), decomposition_mode, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("decomposition_mode"))
      .def(py::init([](const icupy::UnicodeStringVariant &rules, Collator::ECollationStrength collation_strength,
                       UColAttributeValue decomposition_mode) {
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(icupy::to_unistr(rules), collation_strength,
                                                               decomposition_mode, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("collation_strength"), py::arg("decomposition_mode"))
      .def(py::init<const RuleBasedCollator &>(), py::arg("other"))
      .def(py::init([](const py::buffer &bin, int32_t length, const RuleBasedCollator *base) {
             auto info = bin.request();
             if (length == -1) {
               length = static_cast<int32_t>(info.size);
             }
             ErrorCode error_code;
             auto result =
                 std::make_unique<RuleBasedCollator>(reinterpret_cast<uint8_t *>(info.ptr), length, base, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::keep_alive<1, 2>(), py::keep_alive<1, 4>(), py::arg("bin"), py::arg("length"), py::arg("base"));

  rbc.def("__copy__", &RuleBasedCollator::clone);

  rbc.def(
      "__deepcopy__", [](const RuleBasedCollator &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  rbc.def("clone", &RuleBasedCollator::clone);

  rbc.def("clone_binary", [](const RuleBasedCollator &self) {
    ErrorCode error_code;
    const auto capacity = self.cloneBinary(nullptr, 0, error_code);
    std::vector<uint8_t> buffer(capacity);
    error_code.reset();
    self.cloneBinary(buffer.data(), capacity, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return py::bytes(reinterpret_cast<char *>(buffer.data()), capacity);
  });

  rbc.def("create_collation_element_iterator",
          py::overload_cast<const CharacterIterator &>(&RuleBasedCollator::createCollationElementIterator, py::const_),
          py::arg("source"))
      .def(
          "create_collation_element_iterator",
          [](const RuleBasedCollator &self, const icupy::UnicodeStringVariant &source) {
            return self.createCollationElementIterator(icupy::to_unistr(source));
          },
          py::arg("source"));

  rbc.def("get_rules", py::overload_cast<>(&RuleBasedCollator::getRules, py::const_))
      .def("get_rules", py::overload_cast<UColRuleOption, UnicodeString &>(&RuleBasedCollator::getRules, py::const_),
           py::arg("delta"), py::arg("buffer"));
}

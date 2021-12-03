#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/chariter.h>
#include <unicode/coleitr.h>
#include <unicode/sortkey.h>
#include <unicode/strenum.h>
#include <unicode/tblcoll.h>

using namespace icu;

void init_tblcoll(py::module &m) {
  // icu::Collator
  py::class_<Collator, UObject> coll(m, "Collator");

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
  py::enum_<Collator::EComparisonResult>(coll, "EComparisonResult", py::arithmetic(),
                                         "**Deprecated:** ICU 2.6. Use C enum UCollationResult defined in ucol.h")
      .value("LESS", Collator::LESS)
      .value("EQUAL", Collator::EQUAL)
      .value("GREATER", Collator::GREATER)
      .export_values();
#endif // U_FORCE_HIDE_DEPRECATED_API

  coll.def("__copy__", &Collator::clone)
      .def(
          "__deepcopy__", [](const Collator &self, py::dict) { return self.clone(); }, py::arg("memo"));
  coll.def_static(
          "create_instance",
          [](const Locale &loc) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = Collator::createInstance(loc, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("loc"))
      .def_static("create_instance", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = Collator::createInstance(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  coll.def("equals", &Collator::equals, py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "equals",
          [](const Collator &self, const char16_t *source, const UnicodeString &target) {
            return self.equals(source, target);
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *target -> const UnicodeString &target
          "equals",
          [](const Collator &self, const UnicodeString &source, const char16_t *target) {
            return self.equals(source, target);
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          // const char16_t *target -> const UnicodeString &target
          "equals",
          [](const Collator &self, const char16_t *source, const char16_t *target) {
            return self.equals(source, target);
          },
          py::arg("source"), py::arg("target"));
  coll.def_static(
      "get_available_locales",
      []() {
        int32_t count;
        auto p = Collator::getAvailableLocales(count);
        std::vector<const Locale *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p + i;
        }
        return result;
      },
      py::return_value_policy::reference);
  coll.def_static(
      "get_bound",
      [](const std::vector<uint8_t> &source, int32_t source_length, UColBoundMode bound_type, uint32_t no_of_levels) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto result_length =
            Collator::getBound(source.data(), source_length, bound_type, no_of_levels, nullptr, 0, error_code);
        std::vector<uint8_t> result(result_length);
        error_code = U_ZERO_ERROR;
        Collator::getBound(source.data(), source_length, bound_type, no_of_levels, result.data(), result_length,
                           error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("source"), py::arg("source_length"), py::arg("bound_type"), py::arg("no_of_levels"));
  coll.def_static("get_display_name",
                  py::overload_cast<const Locale &, const Locale &, UnicodeString &>(&Collator::getDisplayName),
                  py::arg("object_locale"), py::arg("display_locale"), py::arg("name"))
      .def_static("get_display_name", py::overload_cast<const Locale &, UnicodeString &>(&Collator::getDisplayName),
                  py::arg("object_locale"), py::arg("name"));
  coll.def_static(
      "get_equivalent_reorder_codes",
      [](int32_t reorder_code) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto dest_capacity = Collator::getEquivalentReorderCodes(reorder_code, nullptr, 0, error_code);
        std::vector<int32_t> result(dest_capacity);
        error_code = U_ZERO_ERROR;
        Collator::getEquivalentReorderCodes(reorder_code, result.data(), dest_capacity, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("reorder_code"));
  coll.def_static(
      "get_functional_equivalent",
      [](const char *keyword, const Locale &locale) {
        UErrorCode error_code = U_ZERO_ERROR;
        UBool is_available = true;
        auto result = Collator::getFunctionalEquivalent(keyword, locale, is_available, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(result, is_available);
      },
      py::arg("keyword"), py::arg("locale"));
  coll.def_static("get_keywords", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = Collator::getKeywords(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  coll.def_static(
      "get_keyword_values",
      [](const char *keyword) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = Collator::getKeywordValues(keyword, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword"));
  coll.def_static(
      "get_keyword_values_for_locale",
      [](const char *keyword, const Locale &locale, UBool commonly_used) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = Collator::getKeywordValuesForLocale(keyword, locale, commonly_used, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword"), py::arg("locale"), py::arg("commonly_used"));
  coll.def("greater", &Collator::greater, py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "greater",
          [](const Collator &self, const char16_t *source, const UnicodeString &target) {
            return self.greater(source, target);
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *target -> const UnicodeString &target
          "greater",
          [](const Collator &self, const UnicodeString &source, const char16_t *target) {
            return self.greater(source, target);
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          // const char16_t *target -> const UnicodeString &target
          "greater",
          [](const Collator &self, const char16_t *source, const char16_t *target) {
            return self.greater(source, target);
          },
          py::arg("source"), py::arg("target"));
  coll.def("greater_or_equal", &Collator::greaterOrEqual, py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "greater_or_equal",
          [](const Collator &self, const char16_t *source, const UnicodeString &target) {
            return self.greaterOrEqual(source, target);
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *target -> const UnicodeString &target
          "greater_or_equal",
          [](const Collator &self, const UnicodeString &source, const char16_t *target) {
            return self.greaterOrEqual(source, target);
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          // const char16_t *target -> const UnicodeString &target
          "greater_or_equal",
          [](const Collator &self, const char16_t *source, const char16_t *target) {
            return self.greaterOrEqual(source, target);
          },
          py::arg("source"), py::arg("target"));
  // TODO: Implement "static URegistryKey icu::Collator::registerFactory(CollatorFactory *toAdopt, UErrorCode &status)".
  // TODO: Implement "static URegistryKey icu::Collator::registerInstance(Collator *toAdopt, const Locale &locale,
  // UErrorCode &status)".
  // TODO: Implement "static UBool icu::Collator::unregister(URegistryKey key, UErrorCode &status)".

  // icu::RuleBasedCollator
  py::class_<RuleBasedCollator, Collator> rbc(m, "RuleBasedCollator");
  rbc.def(py::init([](const UnicodeString &rules) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RuleBasedCollator>(rules, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"))
      .def(
          // const char16_t *rules -> const UnicodeString &rules
          py::init([](const char16_t *rules) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RuleBasedCollator>(rules, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"))
      .def(py::init([](const UnicodeString &rules, Collator::ECollationStrength collation_strength) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<RuleBasedCollator>(rules, collation_strength, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("collation_strength"))
      .def(
          // const char16_t *rules -> const UnicodeString &rules
          py::init([](const char16_t *rules, Collator::ECollationStrength collation_strength) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RuleBasedCollator>(rules, collation_strength, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("collation_strength"))
      .def(py::init([](const UnicodeString &rules, UColAttributeValue decomposition_mode) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<RuleBasedCollator>(rules, decomposition_mode, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("decomposition_mode"))
      .def(
          // const char16_t *rules -> const UnicodeString &rules
          py::init([](const char16_t *rules, UColAttributeValue decomposition_mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RuleBasedCollator>(rules, decomposition_mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("decomposition_mode"))
      .def(py::init([](const UnicodeString &rules, Collator::ECollationStrength collation_strength,
                       UColAttributeValue decomposition_mode) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result =
                 std::make_unique<RuleBasedCollator>(rules, collation_strength, decomposition_mode, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("collation_strength"), py::arg("decomposition_mode"))
      .def(
          // const char16_t *rules -> const UnicodeString &rules
          py::init([](const char16_t *rules, Collator::ECollationStrength collation_strength,
                      UColAttributeValue decomposition_mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result =
                std::make_unique<RuleBasedCollator>(rules, collation_strength, decomposition_mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("collation_strength"), py::arg("decomposition_mode"))
      .def(py::init<const RuleBasedCollator &>(), py::arg("other"))
      // FIXME: Implement "icu::RuleBasedCollator::RuleBasedCollator(const uint8_t *bin,
      // int32_t length, const RuleBasedCollator *base, UErrorCode &status)".
      /*
      .def(py::init([](const std::vector<uint8_t> &bin, int32_t length, const RuleBasedCollator *base) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<RuleBasedCollator>(bin.data(), length, base, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("bin"), py::arg("length"), py::arg("base"));
      */
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  rbc.def("__copy__", &RuleBasedCollator::clone)
      .def(
          "__deepcopy__", [](const RuleBasedCollator &self, py::dict) { return self.clone(); }, py::arg("memo"));
  rbc.def("clone", &RuleBasedCollator::clone);
  rbc.def("clone_binary", [](const RuleBasedCollator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    const auto capacity = self.cloneBinary(nullptr, 0, error_code);
    std::vector<uint8_t> result(capacity);
    error_code = U_ZERO_ERROR;
    self.cloneBinary(result.data(), capacity, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  rbc.def(
         "compare",
         [](const RuleBasedCollator &self, const char16_t *source, int32_t source_length, const char16_t *target,
            int32_t target_length) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = self.compare(source, source_length, target, target_length, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("source"), py::arg("source_length"), py::arg("target"), py::arg("target_length"))
      .def(
          "compare",
          [](const RuleBasedCollator &self, const UnicodeString &source, const UnicodeString &target, int32_t length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), py::arg("length"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "compare",
          [](const RuleBasedCollator &self, const char16_t *source, const UnicodeString &target, int32_t length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), py::arg("length"))
      .def(
          // const char16_t *target -> const UnicodeString &target
          "compare",
          [](const RuleBasedCollator &self, const UnicodeString &source, const char16_t *target, int32_t length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), py::arg("length"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          // const char16_t *target -> const UnicodeString &target
          "compare",
          [](const RuleBasedCollator &self, const char16_t *source, const char16_t *target, int32_t length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), py::arg("length"))
      .def(
          "compare",
          [](const RuleBasedCollator &self, const UnicodeString &source, const UnicodeString &target) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "compare",
          [](const RuleBasedCollator &self, const char16_t *source, const UnicodeString &target) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *target -> const UnicodeString &target
          "compare",
          [](const RuleBasedCollator &self, const UnicodeString &source, const char16_t *target) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          // const char16_t *target -> const UnicodeString &target
          "compare",
          [](const RuleBasedCollator &self, const char16_t *source, const char16_t *target) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(source, target, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"))
      // FIXME: Implement uiter C API.
      /*
      .def(
          "compare",
          [](const RuleBasedCollator &self, UCharIterator &s_iter, UCharIterator &t_iter) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.compare(s_iter, t_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("s_iter"), py::arg("t_iter"))
      */
      ;
  rbc.def("create_collation_element_iterator",
          py::overload_cast<const CharacterIterator &>(&RuleBasedCollator::createCollationElementIterator, py::const_),
          py::arg("source"))
      .def("create_collation_element_iterator",
           py::overload_cast<const UnicodeString &>(&RuleBasedCollator::createCollationElementIterator, py::const_),
           py::arg("source"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "create_collation_element_iterator",
          [](const RuleBasedCollator &self, const char16_t *source) {
            return self.createCollationElementIterator(source);
          },
          py::arg("source"));
  rbc.def(
      "get_attribute",
      [](const RuleBasedCollator &self, UColAttribute attr) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getAttribute(attr, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("attr"));
  rbc.def(
         "get_collation_key",
         [](const RuleBasedCollator &self, const char16_t *source, int32_t source_length,
            CollationKey &key) -> CollationKey & {
           UErrorCode error_code = U_ZERO_ERROR;
           auto &result = self.getCollationKey(source, source_length, key, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("source"), py::arg("source_length"), py::arg("key"))
      .def(
          "get_collation_key",
          [](const RuleBasedCollator &self, const UnicodeString &source, CollationKey &key) -> CollationKey & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.getCollationKey(source, key, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("key"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "get_collation_key",
          [](const RuleBasedCollator &self, const char16_t *source, CollationKey &key) -> CollationKey & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.getCollationKey(source, key, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("key"));
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  rbc.def("get_max_variable", &RuleBasedCollator::getMaxVariable);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  rbc.def("get_reorder_codes", [](const RuleBasedCollator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    const auto dest_capacity = self.getReorderCodes(nullptr, 0, error_code);
    error_code = U_ZERO_ERROR;
    std::vector<int32_t> result(dest_capacity);
    self.getReorderCodes(result.data(), dest_capacity, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  rbc.def("get_rules", py::overload_cast<>(&RuleBasedCollator::getRules, py::const_))
      .def("get_rules", py::overload_cast<UColRuleOption, UnicodeString &>(&RuleBasedCollator::getRules, py::const_),
           py::arg("delta"), py::arg("buffer"));
  rbc.def(
         "get_sort_key",
         [](const RuleBasedCollator &self, const char16_t *source, int32_t source_length) {
           const auto result_length = self.getSortKey(source, source_length, nullptr, 0);
           std::vector<uint8_t> result(result_length);
           self.getSortKey(source, source_length, result.data(), result_length);
           return result;
         },
         py::arg("source"), py::arg("source_length"))
      .def(
          "get_sort_key",
          [](const RuleBasedCollator &self, const UnicodeString &source) {
            const auto result_length = self.getSortKey(source, nullptr, 0);
            std::vector<uint8_t> result(result_length);
            self.getSortKey(source, result.data(), result_length);
            return result;
          },
          py::arg("source"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "get_sort_key",
          [](const RuleBasedCollator &self, const char16_t *source) {
            const auto result_length = self.getSortKey(source, nullptr, 0);
            std::vector<uint8_t> result(result_length);
            self.getSortKey(source, result.data(), result_length);
            return result;
          },
          py::arg("source"));
  rbc.def("get_tailored_set", [](const RuleBasedCollator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getTailoredSet(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  rbc.def("get_variable_top", [](const RuleBasedCollator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getVariableTop(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  rbc.def("get_version", [](const RuleBasedCollator &self) {
    UVersionInfo info;
    self.getVersion(info);
    std::vector<uint8_t> result(info, info + sizeof(info));
    return result;
  });
  rbc.def("hash_code", &RuleBasedCollator::hashCode);
  rbc.def(
      "set_attribute",
      [](RuleBasedCollator &self, UColAttribute attr, UColAttributeValue value) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setAttribute(attr, value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("attr"), py::arg("value"));
  rbc.def(
      "set_max_variable",
      [](RuleBasedCollator &self, UColReorderCode group) -> Collator & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.setMaxVariable(group, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("group"));
  rbc.def(
      "set_reorder_codes",
      [](RuleBasedCollator &self, const std::vector<int32_t> &reorder_codes, int32_t reorder_codes_length) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setReorderCodes(reorder_codes.data(), reorder_codes_length, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("reorder_codes"), py::arg("reorder_codes_length"));
}

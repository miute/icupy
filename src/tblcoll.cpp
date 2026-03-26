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
  // class icu::Collator
  //
  py::class_<Collator, UObject> coll(m, "Collator", R"doc(
      Perform locale-sensitive string comparison.

      See Also:
          :class:`RuleBasedCollator`

      Example:
          Natural sort (human-friendly sorting):

          >>> from icupy import icu
          >>> coll = icu.Collator.create_instance(icu.ULOC_US)
          >>> coll.set_attribute(icu.UCOL_NUMERIC_COLLATION, icu.UCOL_ON)
          >>> data = ["file1.txt", "file10.txt", "file2.txt", "file20.txt", "file3.txt"]
          >>> sorted(data, key=coll.get_sort_key)
          ['file1.txt', 'file2.txt', 'file3.txt', 'file10.txt', 'file20.txt']
      )doc");

  //
  // enum icu::Collator::ECollationStrength
  //
  py::enum_<Collator::ECollationStrength>(coll, "ECollationStrength",
                                          py::arithmetic(), R"doc(
Base letter represent a primary difference.

Set comparison level to :attr:`PRIMARY` to ignore secondary and tertiary
differences. Use this to set the strength of a :class:`Collator` object.

Example of primary difference, "abc" < "abd"

Diacritical differences on the same base letter represent a secondary
difference. Set comparison level to :attr:`SECONDARY` to ignore tertiary
differences. Use this to set the strength of a :class:`Collator` object.

Example of secondary difference, "ä" >> "a".

Uppercase and lowercase versions of the same character represent a tertiary
difference. Set comparison level to :attr:`TERTIARY` to include all comparison
differences. Use this to set the strength of a :class:`Collator` object.

Example of tertiary difference, "abc" <<< "ABC".

Two characters are considered "identical" when they have the same unicode
spellings.

For example, "ä" == "ä".

:class:`UCollationStrength` is also used to determine the strength of sort keys
generated from :class:`Collator` objects.

See Also:
    :class:`UColAttributeValue`
      )doc")
      .value("PRIMARY", Collator::PRIMARY)
      .value("SECONDARY", Collator::SECONDARY)
      .value("TERTIARY", Collator::TERTIARY)
      .value("QUATERNARY", Collator::QUATERNARY)
      .value("IDENTICAL", Collator::IDENTICAL)
      .export_values();

  //
  // enum icu::Collator::EComparisonResult
  //
  py::enum_<Collator::EComparisonResult>(coll, "EComparisonResult",
                                         py::arithmetic(), R"doc(
Deprecated: ICU 2.6. Use :class:`UCollationResult` instead.
      )doc")
      .value("LESS", Collator::LESS)
      .value("EQUAL", Collator::EQUAL)
      .value("GREATER", Collator::GREATER)
      .export_values();

  //
  // class icu::Collator
  //
  coll.def("__copy__", &Collator::clone, R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  coll.def(
      "__deepcopy__",
      [](const Collator &self, py::dict & /* memo */) { return self.clone(); },
      py::arg("memo"), R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  coll.def(
      "__eq__",
      [](const Collator &self, const Collator &other) { return self == other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  coll.def("__hash__", &Collator::hashCode, R"doc(
      Return the hash value of this object.

      This is equivalent to :meth:`hash_code`.
      )doc");

  coll.def(
      "__ne__",
      [](const Collator &self, const Collator &other) { return self != other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  coll.def("clone", &Collator::clone, R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");

  coll.def(
          // [2] icu::Collator::compare
          "compare",
          [](const Collator &self, const std::u16string &source,
             int32_t source_length, const std::u16string &target,
             int32_t target_length) {
            ErrorCode error_code;
            auto result =
                self.compare(source.data(), source_length, target.data(),
                             target_length, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("source_length"), py::arg("target"),
          py::arg("target_length"), R"doc(
      Return :attr:`~UCollationResult.UCOL_LESS` if *source* < *target*
      (up to the specified length),
      :attr:`~UCollationResult.UCOL_GREATER` if *source* > *target*
      (up to the specified length), and
      :attr:`~UCollationResult.UCOL_EQUAL` otherwise.
      )doc")
      .def(
          // [5] icu::Collator::compare
          "compare",
          [](const Collator &self, const icupy::UnicodeStringVariant &source,
             const icupy::UnicodeStringVariant &target, int32_t length) {
            ErrorCode error_code;
            auto result =
                self.compare(icupy::to_unistr(source), icupy::to_unistr(target),
                             length, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), py::arg("length"), R"doc(
      Return :attr:`~UCollationResult.UCOL_LESS` if *source* < *target*
      (up to the specified length),
      :attr:`~UCollationResult.UCOL_GREATER` if *source* > *target*
      (up to the specified length), and
      :attr:`~UCollationResult.UCOL_EQUAL` otherwise.
      )doc")
      .def(
          // [6] icu::Collator::compare
          "compare",
          [](const Collator &self, const icupy::UnicodeStringVariant &source,
             const icupy::UnicodeStringVariant &target) {
            ErrorCode error_code;
            auto result = self.compare(icupy::to_unistr(source),
                                       icupy::to_unistr(target), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("target"), R"doc(
      Return :attr:`~UCollationResult.UCOL_LESS` if *source* < *target*,
      :attr:`~UCollationResult.UCOL_GREATER` if *source* > *target*, and
      :attr:`~UCollationResult.UCOL_EQUAL` otherwise.
      )doc")
      // FIXME: Implement uiter C API.
      /*
      .def(
          // [7] icu::Collator::compare
          "compare",
          [](const Collator &self, UCharIterator &s_iter,
             UCharIterator &t_iter) {
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
        auto result = self.compareUTF8(StringPiece(source), StringPiece(target),
                                       error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return :attr:`~UCollationResult.UCOL_LESS` if *source* < *target*,
      :attr:`~UCollationResult.UCOL_GREATER` if *source* > *target*, and
      :attr:`~UCollationResult.UCOL_EQUAL` otherwise.
      )doc");

  coll.def_static(
          "create_instance",
          [](const icupy::LocaleVariant &loc) {
            ErrorCode error_code;
            auto result =
                Collator::createInstance(icupy::to_locale(loc), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("loc"), R"doc(
      Create the ``Collator`` object for the desired locale.
      )doc")
      .def_static(
          "create_instance",
          []() {
            ErrorCode error_code;
            auto result = Collator::createInstance(error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          R"doc(
      Create the ``Collator`` object for the current default locale.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  coll.def(
      "equal_to",
      [](const Collator &self, icupy::UnicodeStringVariant &source,
         icupy::UnicodeStringVariant &target) {
        return self.equal_to()(icupy::to_unistr(source),
                               icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the strings are equal according to the collation
      rules. ``False`` otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  coll.def(
      "equals",
      [](const Collator &self, const icupy::UnicodeStringVariant &source,
         const icupy::UnicodeStringVariant &target) -> py::bool_ {
        return self.equals(icupy::to_unistr(source), icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the strings are equal according to the collation
      rules. ``False`` otherwise.
      )doc");

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
      py::arg("attr"), R"doc(
      Return the value of the specified attribute.

      See Also:
          :meth:`.set_attribute`
      )doc");

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
      py::return_value_policy::reference, R"doc(
      Return the set of locales for which collations are installed.

      .. note::

          This does not include locales supported by registered collators.
      )doc");

  coll.def_static(
      "get_bound",
      [](const py::buffer &source, int32_t source_length,
         UColBoundMode bound_type, uint32_t no_of_levels) {
        auto info = source.request();
        if (source_length == -1) {
          source_length = static_cast<int32_t>(info.size);
        }
        auto source_ptr = reinterpret_cast<const uint8_t *>(info.ptr);
        ErrorCode error_code;
        const auto result_length =
            Collator::getBound(source_ptr, source_length, bound_type,
                               no_of_levels, nullptr, 0, error_code);
        std::vector<uint8_t> result(result_length, 0);
        error_code.reset();
        Collator::getBound(source_ptr, source_length, bound_type, no_of_levels,
                           result.data(), result_length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(reinterpret_cast<char *>(result.data()),
                         result_length);
      },
      py::arg("source"), py::arg("source_length"), py::arg("bound_type"),
      py::arg("no_of_levels"), R"doc(
      Produce a bound for the specified sortkey and number of levels.
      )doc");

  coll.def(
          "get_collation_key",
          [](const Collator &self, const std::u16string &source,
             int32_t source_length, CollationKey &key) -> CollationKey & {
            ErrorCode error_code;
            auto &result = self.getCollationKey(source.data(), source_length,
                                                key, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("source_length"), py::arg("key"), R"doc(
      Transform the string (up to the specified length) into a series of
      characters that can be compared with :meth:`CollationKey.compare_to`.
      )doc")
      .def(
          "get_collation_key",
          [](const Collator &self, const icupy::UnicodeStringVariant &source,
             CollationKey &key) -> CollationKey & {
            ErrorCode error_code;
            auto &result =
                self.getCollationKey(icupy::to_unistr(source), key, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("key"), R"doc(
      Transform the string into a series of characters that can be compared
      with :meth:`CollationKey.compare_to`.
      )doc");

  coll.def_static(
          "get_display_name",
          [](const icupy::LocaleVariant &object_locale,
             const icupy::LocaleVariant &display_locale,
             UnicodeString &name) -> UnicodeString & {
            return Collator::getDisplayName(icupy::to_locale(object_locale),
                                            icupy::to_locale(display_locale),
                                            name);
          },
          py::arg("object_locale"), py::arg("display_locale"), py::arg("name"),
          R"doc(
      Return the object name for the desired locale, in the desired
      language.
      )doc")
      .def_static(
          "get_display_name",
          [](const icupy::LocaleVariant &object_locale,
             UnicodeString &name) -> UnicodeString & {
            return Collator::getDisplayName(icupy::to_locale(object_locale),
                                            name);
          },
          py::arg("object_locale"), py::arg("name"), R"doc(
      Return the object name for the desired locale, in the language of the
      default locale.
      )doc");

  coll.def_static(
      "get_equivalent_reorder_codes",
      [](int32_t reorder_code) {
        ErrorCode error_code;
        const auto dest_capacity = Collator::getEquivalentReorderCodes(
            reorder_code, nullptr, 0, error_code);
        std::vector<int32_t> result(dest_capacity, 0);
        error_code.reset();
        Collator::getEquivalentReorderCodes(reorder_code, result.data(),
                                            dest_capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("reorder_code"), R"doc(
      Return the reorder codes that are grouped with the specified reorder
      code.

      See Also:
          :class:`UColReorderCode`
          :class:`UScriptCode`
          :meth:`.get_reorder_codes`
          :meth:`.set_reorder_codes`
      )doc");

  coll.def_static(
      "get_functional_equivalent",
      [](const std::string &keyword, const icupy::LocaleVariant &locale) {
        UBool is_available = true;
        ErrorCode error_code;
        auto result = Collator::getFunctionalEquivalent(
            keyword.data(), icupy::to_locale(locale), is_available, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_tuple(result, py::bool_(is_available));
      },
      py::arg("keyword"), py::arg("locale"), R"doc(
      Return a tuple of the functionally equivalent locale and the
      "available" flag for the specified requested locale and keyword.

      A locale is defined as "available" if it physically exists within the
      collation locale data.
      )doc");

  coll.def_static(
      "get_keywords",
      []() {
        ErrorCode error_code;
        auto result = Collator::getKeywords(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return a string enumeration of all possible keywords that are relevant to
      collation.
      )doc");

  coll.def_static(
      "get_keyword_values",
      [](const std::string &keyword) {
        ErrorCode error_code;
        auto result = Collator::getKeywordValues(keyword.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword"), R"doc(
      Create a string enumeration of all currently used values for the
      specified keyword.
      )doc");

  coll.def_static(
      "get_keyword_values_for_locale",
      [](const std::string &keyword, const icupy::LocaleVariant &locale,
         py::bool_ commonly_used) {
        ErrorCode error_code;
        auto result = Collator::getKeywordValuesForLocale(
            keyword.data(), icupy::to_locale(locale), commonly_used,
            error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("keyword"), py::arg("locale"), py::arg("commonly_used"), R"doc(
      Create a string enumeration of all currently used values for the
      specified keyword and locale.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  coll.def("get_max_variable", &Collator::getMaxVariable, R"doc(
      Return the maximum reordering group whose characters are affected by
      :attr:`~UColAttribute.UCOL_ALTERNATE_HANDLING`.

      See Also:
          :meth:`.set_max_variable`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  coll.def(
      "get_reorder_codes",
      [](const Collator &self) {
        ErrorCode error_code;
        const auto dest_capacity = self.getReorderCodes(nullptr, 0, error_code);
        error_code.reset();
        std::vector<int32_t> result(dest_capacity, 0);
        self.getReorderCodes(result.data(), dest_capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the reordering codes for this collator.

      See Also:
          :class:`UColReorderCode`
          :class:`UScriptCode`
          :meth:`.get_equivalent_reorder_codes`
          :meth:`.set_reorder_codes`
      )doc");

  coll.def(
          "get_sort_key",
          [](const Collator &self, const std::u16string &source,
             int32_t source_length) {
            auto source_data = source.data();
            const auto result_length =
                self.getSortKey(source_data, source_length, nullptr, 0);
            std::vector<uint8_t> result(result_length, 0);
            self.getSortKey(source_data, source_length, result.data(),
                            result_length);
            return py::bytes(reinterpret_cast<char *>(result.data()),
                             result_length);
          },
          py::arg("source"), py::arg("source_length"), R"doc(
      Return the sort key as an array of bytes from a ``str``.
      )doc")
      .def(
          "get_sort_key",
          [](const Collator &self, const icupy::UnicodeStringVariant &source) {
            const auto source_value = icupy::to_unistr(source);
            const auto result_length =
                self.getSortKey(source_value, nullptr, 0);
            std::vector<uint8_t> result(result_length, 0);
            self.getSortKey(source_value, result.data(), result_length);
            return py::bytes(reinterpret_cast<char *>(result.data()),
                             result_length);
          },
          py::arg("source"), R"doc(
      Return the sort key as an array of bytes from a :class:`UnicodeString`.
      )doc");

  coll.def(
      "get_tailored_set",
      [](const Collator &self) {
        ErrorCode error_code;
        auto result = self.getTailoredSet(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return a :class:`UnicodeSet` containing all code points and sequences
      that may be sorted in an order different from that of the root collator.
      )doc");

  coll.def(
      "get_variable_top",
      [](const Collator &self) {
        ErrorCode error_code;
        auto result = self.getVariableTop(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the variable top primary weight.
      )doc");

  coll.def(
      "get_version",
      [](const Collator &self) {
        UVersionInfo info;
        self.getVersion(info);
        py::tuple result(U_MAX_VERSION_LENGTH);
        int n = 0;
        std::for_each(std::begin(info), std::end(info),
                      [&](auto v) { result[n++] = v; });
        return result;
      },
      R"doc(
      Return the version information for a ``Collator``.
      )doc");

  coll.def(
      "greater",
      [](const Collator &self, const icupy::UnicodeStringVariant &source,
         const icupy::UnicodeStringVariant &target) -> py::bool_ {
        return self.greater(icupy::to_unistr(source), icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the first string is greater than the second one,
      according to the collation rules. ``False`` otherwise.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  coll.def(
      "greater_equal",
      [](const Collator &self, icupy::UnicodeStringVariant &source,
         icupy::UnicodeStringVariant &target) {
        return self.greater_equal()(icupy::to_unistr(source),
                                    icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the first string is greater than or equal to the
      second one, according to the collation rules. ``False`` otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  coll.def(
      "greater_or_equal",
      [](const Collator &self, const icupy::UnicodeStringVariant &source,
         const icupy::UnicodeStringVariant &target) -> py::bool_ {
        return self.greaterOrEqual(icupy::to_unistr(source),
                                   icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the first string is greater than or equal to the
      second one, according to the collation rules. ``False`` otherwise.
      )doc");

  coll.def("hash_code", &Collator::hashCode, R"doc(
      Return the hash code for the collation object.

      See Also:
          :meth:`.__hash__`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  coll.def(
      "less",
      [](const Collator &self, icupy::UnicodeStringVariant &source,
         icupy::UnicodeStringVariant &target) {
        return self.less()(icupy::to_unistr(source), icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the first string is less than the second one,
      according to the collation rules. ``False`` otherwise.
      )doc");

  coll.def(
      "less_equal",
      [](const Collator &self, icupy::UnicodeStringVariant &source,
         icupy::UnicodeStringVariant &target) {
        return self.less_equal()(icupy::to_unistr(source),
                                 icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the first string is less than or equal to the
      second one, according to the collation rules. ``False`` otherwise.
      )doc");

  coll.def(
      "not_equal_to",
      [](const Collator &self, icupy::UnicodeStringVariant &source,
         icupy::UnicodeStringVariant &target) {
        return self.not_equal_to()(icupy::to_unistr(source),
                                   icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return ``True`` if the strings are not equal according to the collation
      rules. ``False`` otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 76)

  // TODO: Implement "static URegistryKey icu::Collator::registerFactory(
  //  CollatorFactory *toAdopt, UErrorCode &status)".

  // TODO: Implement "static URegistryKey icu::Collator::registerInstance(
  //  Collator *toAdopt, const Locale &locale, UErrorCode &status)".

  coll.def(
      "set_attribute",
      [](Collator &self, UColAttribute attr, UColAttributeValue value) {
        ErrorCode error_code;
        self.setAttribute(attr, value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("attr"), py::arg("value"), R"doc(
      Set the *value* of the specified attribute.

      See Also:
          :meth:`.get_attribute`
      )doc");

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
      py::arg("group"), R"doc(
      Set the variable top to the top of the specified reordering group.

      The variable top determines the highest-sorting character which is
      affected by :attr:`~UColAttribute.UCOL_ALTERNATE_HANDLING`. If that
      attribute is set to :attr:`~UColAttributeValue.UCOL_NON_IGNORABLE`, then
      the variable top has no effect.

      See Also:
          :meth:`.get_max_variable`
      )doc");

  coll.def(
      "set_reorder_codes",
      [](Collator &self, const std::vector<int32_t> &reorder_codes,
         int32_t reorder_codes_length) {
        ErrorCode error_code;
        self.setReorderCodes(reorder_codes.data(), reorder_codes_length,
                             error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("reorder_codes"), py::arg("reorder_codes_length"), R"doc(
      Set the reordering codes for this collator.

      The reordering codes are a combination of script codes and reorder codes.

      See Also:
          :class:`UColReorderCode`
          :class:`UScriptCode`
          :meth:`.get_equivalent_reorder_codes`
          :meth:`.get_reorder_codes`
      )doc");

  // TODO: Implement "static UBool icu::Collator::unregister(URegistryKey key,
  //  UErrorCode &status)".

  //
  // class icu::RuleBasedCollator
  //
  py::class_<RuleBasedCollator, Collator> rbc(m, "RuleBasedCollator", R"doc(
      :class:`Collator` using data-driven tables.

      For more information, see the ICU User Guide:
      `Collation <https://unicode-org.github.io/icu/userguide/collation>`__.
      )doc");

  rbc.def(py::init([](const icupy::UnicodeStringVariant &rules) {
            ErrorCode error_code;
            auto result = std::make_unique<RuleBasedCollator>(
                icupy::to_unistr(rules), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"))
      .def(py::init([](const icupy::UnicodeStringVariant &rules,
                       Collator::ECollationStrength collation_strength) {
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(
                 icupy::to_unistr(rules), collation_strength, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("collation_strength"))
      .def(py::init([](const icupy::UnicodeStringVariant &rules,
                       UColAttributeValue decomposition_mode) {
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(
                 icupy::to_unistr(rules), decomposition_mode, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("decomposition_mode"))
      .def(py::init([](const icupy::UnicodeStringVariant &rules,
                       Collator::ECollationStrength collation_strength,
                       UColAttributeValue decomposition_mode) {
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(
                 icupy::to_unistr(rules), collation_strength,
                 decomposition_mode, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("rules"), py::arg("collation_strength"),
           py::arg("decomposition_mode"))
      .def(py::init<const RuleBasedCollator &>(), py::arg("other"))
      .def(py::init([](const py::buffer &bin, int32_t length,
                       const RuleBasedCollator *base) {
             auto info = bin.request();
             if (length == -1) {
               length = static_cast<int32_t>(info.size);
             }
             ErrorCode error_code;
             auto result = std::make_unique<RuleBasedCollator>(
                 reinterpret_cast<uint8_t *>(info.ptr), length, base,
                 error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::keep_alive<1, 2>(), py::keep_alive<1, 4>(), py::arg("bin"),
           py::arg("length"), py::arg("base").none(false));

  rbc.def("__copy__", &RuleBasedCollator::clone);

  rbc.def(
      "__deepcopy__",
      [](const RuleBasedCollator &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"));

  rbc.def("clone", &RuleBasedCollator::clone);

  rbc.def("clone_binary", [](const RuleBasedCollator &self) {
    ErrorCode error_code;
    const auto capacity = self.cloneBinary(nullptr, 0, error_code);
    std::vector<uint8_t> buffer(capacity, 0);
    error_code.reset();
    self.cloneBinary(buffer.data(), capacity, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return py::bytes(reinterpret_cast<char *>(buffer.data()), capacity);
  });

  rbc.def("create_collation_element_iterator",
          py::overload_cast<const CharacterIterator &>(
              &RuleBasedCollator::createCollationElementIterator, py::const_),
          py::arg("source"))
      .def(
          "create_collation_element_iterator",
          [](const RuleBasedCollator &self,
             const icupy::UnicodeStringVariant &source) {
            return self.createCollationElementIterator(
                icupy::to_unistr(source));
          },
          py::arg("source"));

  rbc.def("get_rules",
          py::overload_cast<>(&RuleBasedCollator::getRules, py::const_))
      .def("get_rules",
           py::overload_cast<UColRuleOption, UnicodeString &>(
               &RuleBasedCollator::getRules, py::const_),
           py::arg("delta"), py::arg("buffer"));
}

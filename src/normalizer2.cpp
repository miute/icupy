#include "main.hpp"
#include <optional>
#include <pybind11/stl.h>
#include <unicode/normalizer2.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
#include <unicode/edits.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

using namespace icu;

void init_normalizer2(py::module &m) {
  //
  // class icu::Normalizer2
  //
  py::class_<Normalizer2, UObject> n2(m, "Normalizer2", R"doc(
      Unicode normalization functionality for using standard Unicode
      normalization or custom mapping tables.

      .. seealso::

         :class:`FilteredNormalizer2`
      )doc");

  n2.def(
      "append",
      [](const Normalizer2 &self, UnicodeString &first,
         const icupy::UnicodeStringVariant &second) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.append(first, icupy::to_unistr(second), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("first"), py::arg("second"), R"doc(
      Append the string specified by *second* to the string specified by
      *first* (merging them at the boundary); return *first* itself.

      *first* and *second* should be normalized.

      .. note::

         The first and second strings must be different objects.

      .. seealso::

         :meth:`.normalize_second_and_append`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  n2.def("compose_pair", &Normalizer2::composePair, py::arg("a"), py::arg("b"),
         R"doc(
      Return a composite code point from the two specified code points, or a
      negative value if no such composite code point exists.

      In standard Unicode normalization, this means that c has the canonical
      decomposition a+b and does not have the Full_Composition_Exclusion
      property.
      )doc");

  n2.def("get_combining_class", &Normalizer2::getCombiningClass, py::arg("c"),
         R"doc(
      Return the combining class of the specified code point.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  n2.def("get_decomposition", &Normalizer2::getDecomposition, py::arg("c"),
         py::arg("decomposition"), R"doc(
      Set the decomposition mapping for the code point specified by *c* to
      *decomposition*; return ``True`` if *c* has a decomposition, ``False``
      otherwise.

      *c* is a code point.

      *decomposition* is a string object to receive the decomposition mapping
      for *c*.

      .. seealso::

         :meth:`.get_raw_decomposition`
      )doc");

  n2.def_static(
      "get_instance",
      [](std::optional<const std::string> &package_name,
         const std::string &name, UNormalization2Mode mode) {
        ErrorCode error_code;
        auto result = Normalizer2::getInstance(
            package_name ? package_name->data() : nullptr, name.data(), mode,
            error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("package_name"),
      py::arg("name"), py::arg("mode"), R"doc(
      Return a ``Normalizer2`` instance which uses the specified data file and
      composes or decomposes text according to the specified mode.

      *package_name* is the name of the application data package.
      If *package_name* is ``None``, the ICU built-in data is used.

      *name* is one of the following: "nfc", "nfkc", "nfkc_cf", "nfkc_scf", or
      the name of a custom data file.

      *mode* is the normalization mode:

      - name="nfc" and
        :attr:`~UNormalization2Mode.UNORM2_COMPOSE`/:attr:`~UNormalization2Mode.UNORM2_DECOMPOSE`
        for Unicode standard NFC/NFD.
      - name="nfkc" and
        :attr:`~UNormalization2Mode.UNORM2_COMPOSE`/:attr:`~UNormalization2Mode.UNORM2_DECOMPOSE`
        for Unicode standard NFKC/NFKD.
      - name="nfkc_cf" and
        :attr:`~UNormalization2Mode.UNORM2_COMPOSE`
        for Unicode standard NFKC_CF=NFKC_Casefold.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  n2.def_static(
      "get_nfc_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFCInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a ``Normalizer2`` instance for Unicode NFC normalization.

      This is equivalent to calling
      ``get_instance(None, "nfc", UNORM2_COMPOSE)``.
      )doc");

  n2.def_static(
      "get_nfd_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFDInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a ``Normalizer2`` instance for Unicode NFD normalization.

      This is equivalent to calling
      ``get_instance(None, "nfc", UNORM2_DECOMPOSE)``.
      )doc");

  n2.def_static(
      "get_nfkc_casefold_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKCCasefoldInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a ``Normalizer2`` instance for Unicode toNFKC_Casefold()
      normalization which is equivalent to applying the NFKC_Casefold mappings
      and then NFC.

      See https://www.unicode.org/reports/tr44/#NFKC_Casefold.

      This is equivalent to calling
      ``get_instance(None, "nfkc_cf", UNORM2_COMPOSE)``.
      )doc");

  n2.def_static(
      "get_nfkc_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKCInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a ``Normalizer2`` instance for Unicode NFKC normalization.

      This is equivalent to calling
      ``get_instance(None, "nfkc", UNORM2_COMPOSE)``.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 74)
  n2.def_static(
      "get_nfkc_simple_casefold_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKCSimpleCasefoldInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a ``Normalizer2`` instance for a variant of Unicode
      toNFKC_Casefold() normalization which is equivalent to applying the
      NFKC_Simple_Casefold mappings and then NFC.

      See https://www.unicode.org/reports/tr44/#NFKC_Simple_Casefold.

      This is equivalent to calling
      ``get_instance(None, "nfkc_scf", UNORM2_COMPOSE)``.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 74)

  n2.def_static(
      "get_nfkd_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKDInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, R"doc(
      Return a ``Normalizer2`` instance for Unicode NFKD normalization.

      This is equivalent to calling
      ``get_instance(None, "nfkc", UNORM2_DECOMPOSE)``.
      )doc");

  n2.def("get_raw_decomposition", &Normalizer2::getRawDecomposition,
         py::arg("c"), py::arg("decomposition"), R"doc(
      Set the raw decomposition mapping for the code point specified by *c* to
      *decomposition*; return ``True`` if *c* has a decomposition, ``False``
      otherwise.

      *c* is a code point.

      *decomposition* is a string object to receive the raw decomposition
      mapping for *c*.

      - For standard NFKC Normalizer2 instance, ``get_raw_decomposition()``
        returns the Unicode Decomposition_Mapping (dm) property.
      - For standard NFC Normalizer2 instance, it returns the
        Decomposition_Mapping only if the Decomposition_Type (dt) is
        Canonical (Can); in this case, the result contains either one or two
        code points (=1..4 char16_ts).

      .. seealso::

         :meth:`.get_decomposition`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

  n2.def("has_boundary_after", &Normalizer2::hasBoundaryAfter, py::arg("c"),
         R"doc(
      Return ``True`` if the specified character is always followed by
      a normalizing boundary, regardless of context; ``False`` otherwise.

      .. note::

         ``has_boundary_after()`` may be significantly slower than
         :meth:`has_boundary_before`.
      )doc");

  n2.def("has_boundary_before", &Normalizer2::hasBoundaryBefore, py::arg("c"),
         R"doc(
      Return ``True`` if the specified character is always preceded by
      a normalizing boundary, regardless of context; ``False`` otherwise.
      )doc");

  n2.def("is_inert", &Normalizer2::isInert, py::arg("c"), R"doc(
      Return ``True`` if the specified character is normalization-inert,
      ``False`` otherwise.

      .. note::

         ``is_inert()`` may be significantly slower than
         :meth:`has_boundary_before`.
      )doc");

  n2.def(
      "is_normalized",
      [](const Normalizer2 &self, const icupy::UnicodeStringVariant &s) {
        ErrorCode error_code;
        auto result = self.isNormalized(icupy::to_unistr(s), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("s"), R"doc(
      Return ``True`` if the specified string is normalized, ``False``
      otherwise.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  n2.def(
      "is_normalized_utf8",
      [](const Normalizer2 &self, const py::bytes &b) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.isNormalizedUTF8(StringPiece(b), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("b"), R"doc(
      Return ``True`` if the specified UTF-8 encoded string is normalized,
      ``False`` otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  n2.def(
        "normalize",
        [](const Normalizer2 &self, const icupy::UnicodeStringVariant &src) {
          ErrorCode error_code;
          auto result = self.normalize(icupy::to_unistr(src), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("src"), R"doc(
      Normalize the specified string and return the result.
      )doc")
      .def(
          "normalize",
          [](const Normalizer2 &self, const icupy::UnicodeStringVariant &src,
             UnicodeString &dest) -> UnicodeString & {
            ErrorCode error_code;
            auto &result =
                self.normalize(icupy::to_unistr(src), dest, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("src"), py::arg("dest"), R"doc(
      Normalize the string specified by *src* and set the result to *dest*;
      return *dest* itself.
      )doc");

  n2.def(
      "normalize_second_and_append",
      [](const Normalizer2 &self, UnicodeString &first,
         const icupy::UnicodeStringVariant &second) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.normalizeSecondAndAppend(
            first, icupy::to_unistr(second), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("first"), py::arg("second"), R"doc(
      Normalize the string specified by *second* and append the result to
      *first*; return *first* itself.

      .. note::

         The first and second strings must be different objects.

      .. seealso::

         :meth:`.append`
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  n2.def(
      "normalize_utf8",
      [](const Normalizer2 &self, uint32_t options, const py::bytes &src,
         std::optional<Edits *> &edits) {
        std::string dest;
        auto sink = StringByteSink<std::string>(&dest);
        ErrorCode error_code;
        self.normalizeUTF8(options, StringPiece(src), sink,
                           edits.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(dest);
      },
      py::arg("options"), py::arg("src"), py::arg("edits") = std::nullopt,
      R"doc(
      Normalize the UTF-8 encoded string specified by *src* and return the
      result.

      *options* is a bit set of :attr:`U_OMIT_UNCHANGED_TEXT` and
      :attr:`U_EDITS_NO_RESET`, and is usually 0.

      If *edits* is specified, record edits to the index mapping, operations on
      styled text, and only the changes (if any).
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  n2.def(
      "quick_check",
      [](const Normalizer2 &self, const icupy::UnicodeStringVariant &s) {
        ErrorCode error_code;
        auto result = self.quickCheck(icupy::to_unistr(s), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("s"), R"doc(
      Determine if the specified string is normalized.
      )doc");

  n2.def(
      "span_quick_check_yes",
      [](const Normalizer2 &self, const icupy::UnicodeStringVariant &s) {
        ErrorCode error_code;
        auto result = self.spanQuickCheckYes(icupy::to_unistr(s), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("s"), R"doc(
      Return the end index of the normalized substring in the specified
      input string.
      )doc");

  //
  // class icu::FilteredNormalizer2
  //
  py::class_<FilteredNormalizer2, Normalizer2> fn2(m, "FilteredNormalizer2",
                                                   R"doc(
      A :class:`Normalizer2` that filters the input text using
      :class:`UnicodeSet`.
      )doc");

  fn2.def(py::init<const Normalizer2 &, const UnicodeSet &>(), py::arg("n2"),
          py::arg("filter_set"), R"doc(
      Initialize a ``FilteredNormalizer2`` instance using
      a :class:`Normalizer2` instance and a filter set.

      *n2* is a :class:`Normalizer2` instance to be wrapped.

      *filter_set* is an :class:`UnicodeSet` which determines the characters to
      be normalized.

      .. important::

         *n2* and *filter_set* must outlive this normalizer object.
      )doc");
}

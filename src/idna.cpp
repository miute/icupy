#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/idna.h>

using namespace icu;

void init_idna(py::module &m) {
  //
  // class icu::IDNAInfo
  //
  py::class_<IDNAInfo, UMemory> info(m, "IDNAInfo", R"doc(
      Output container for :class:`IDNA` processing errors.
      )doc");

  info.def(py::init<>(), R"doc(
      Initialize the ``IDNAInfo`` instance.
      )doc");

  info.def("get_errors", &IDNAInfo::getErrors, R"doc(
      Return a bit set indicating :class:`IDNA` processing errors.

      The bit set is a combination of the following values:
      :attr:`UIDNA_ERROR_EMPTY_LABEL`,
      :attr:`UIDNA_ERROR_LABEL_TOO_LONG`,
      :attr:`UIDNA_ERROR_DOMAIN_NAME_TOO_LONG`,
      :attr:`UIDNA_ERROR_LEADING_HYPHEN`,
      :attr:`UIDNA_ERROR_TRAILING_HYPHEN`,
      :attr:`UIDNA_ERROR_HYPHEN_3_4`,
      :attr:`UIDNA_ERROR_LEADING_COMBINING_MARK`,
      :attr:`UIDNA_ERROR_DISALLOWED`,
      :attr:`UIDNA_ERROR_PUNYCODE`,
      :attr:`UIDNA_ERROR_LABEL_HAS_DOT`,
      :attr:`UIDNA_ERROR_INVALID_ACE_LABEL`,
      :attr:`UIDNA_ERROR_BIDI`,
      :attr:`UIDNA_ERROR_CONTEXTJ`,
      :attr:`UIDNA_ERROR_CONTEXTO`,
      :attr:`UIDNA_ERROR_CONTEXTO_PUNCTUATION`, and
      :attr:`UIDNA_ERROR_CONTEXTO_DIGIT`.
      )doc");

  info.def(
      "has_errors",
      [](const IDNAInfo &self) -> py::bool_ { return self.hasErrors(); }, R"doc(
      Return ``True`` if any :class:`IDNA` processing errors are present.
      )doc");

  info.def(
      "is_transitional_different",
      [](const IDNAInfo &self) -> py::bool_ {
        return self.isTransitionalDifferent();
      },
      R"doc(
      Return ``True`` if the result of processing with nontransitional
      processing would be different from the result of processing with
      transitional processing.

      This is the case when the input label or domain name contains one or more
      deviation characters outside a Punycode label (see UTS #46).

      - With nontransitional processing, such characters are copied to the
        destination string.
      - With transitional processing, such characters are mapped (sharp
        s/sigma) or removed (joiner/nonjoiner).
      )doc");

  //
  // class icu::IDNA
  //
  py::class_<IDNA, UObject> idna(m, "IDNA", R"doc(
      Abstract base class for IDNA processing.

      See http://www.unicode.org/reports/tr46/ and
      http://www.ietf.org/rfc/rfc3490.txt

      The IDNA class is not intended for public subclassing.

      Example:
          >>> from icupy import icu
          >>> uts46 = icu.IDNA.create_uts46_instance(icu.UIDNA_DEFAULT | icu.UIDNA_CHECK_BIDI | icu.UIDNA_CHECK_CONTEXTJ)
          >>> dest = icu.UnicodeString()
          >>> info = icu.IDNAInfo()
          >>> uts46.name_to_ascii("a\u200cb.com", dest, info)  # a + ZERO WIDTH NON-JOINER + b.com
          UnicodeString('xn--ab-j1t.com', text_length=14)
          >>> bool(info.get_errors() & icu.UIDNA_ERROR_BIDI)
          False
          >>> bool(info.get_errors() & icu.UIDNA_ERROR_CONTEXTJ)
          True
      )doc");

  //
  // enum
  //
  py::enum_<decltype(UIDNA_DEFAULT)>(idna, "IDNA", py::arithmetic(), R"doc(
Bit set to modify the :class:`IDNA` processing and error checking.
      )doc")
      .value("DEFAULT", UIDNA_DEFAULT, R"doc(
             Default options value: None of the other options are set.

             For use in static worker and factory methods.
             )doc")
      .value("ALLOW_UNASSIGNED", UIDNA_ALLOW_UNASSIGNED, R"doc(
             Deprecated: ICU 55 Use UTS #46 instead via class :class:`IDNA`.
             )doc")
      .value("USE_STD3_RULES", UIDNA_USE_STD3_RULES, R"doc(
             Option to check whether the input conforms to the STD3 ASCII
             rules, for example the restriction of labels to LDH characters
             (ASCII Letters, Digits and Hyphen-Minus).

             For use in static worker and factory methods.
             )doc")
      .value("CHECK_BIDI", UIDNA_CHECK_BIDI, R"doc(
             IDNA option to check for whether the input conforms to the BiDi
             rules.

             For use in static worker and factory methods.

             This option is ignored by the IDNA2003 implementation. (IDNA2003
             always performs a BiDi check.)
             )doc")
      .value("CHECK_CONTEXTJ", UIDNA_CHECK_CONTEXTJ, R"doc(
             IDNA option to check for whether the input conforms to the
             CONTEXTJ rules.

             For use in static worker and factory methods.

             This option is ignored by the IDNA2003 implementation. (The
             CONTEXTJ check is new in IDNA2008.)
             )doc")
      .value("NONTRANSITIONAL_TO_ASCII", UIDNA_NONTRANSITIONAL_TO_ASCII, R"doc(
             IDNA option for nontransitional processing in to_ascii().

             For use in static worker and factory methods.

             By default, to_ascii() uses transitional processing.

             This option is ignored by the IDNA2003 implementation. (This is
             only relevant for compatibility of newer IDNA implementations with
             IDNA2003.)
             )doc")
      .value("NONTRANSITIONAL_TO_UNICODE", UIDNA_NONTRANSITIONAL_TO_UNICODE,
             R"doc(
             IDNA option for nontransitional processing in to_unicode().

             For use in static worker and factory methods.

             By default, to_unicode() uses transitional processing.

             This option is ignored by the IDNA2003 implementation. (This is
             only relevant for compatibility of newer IDNA implementations with
             IDNA2003.)
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("CHECK_CONTEXTO", UIDNA_CHECK_CONTEXTO, R"doc(
             IDNA option to check for whether the input conforms to the
             CONTEXTO rules.

             For use in static worker and factory methods.

             This option is ignored by the IDNA2003 implementation. (The
             CONTEXTO check is new in IDNA2008.)

             This is for use by registries for IDNA2008 conformance. UTS #46
             does not require the CONTEXTO check.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();

  //
  // class icu::IDNA
  //
  idna.def_static(
      "create_uts46_instance",
      [](int options) {
        ErrorCode error_code;
        auto result = IDNA::createUTS46Instance(options, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("options"), R"doc(
      Create an ``IDNA`` object which implements UTS #46.

      *options* is a bit set to modify the processing and error checking.
      These should include :attr:`UIDNA_NONTRANSITIONAL_TO_ASCII` |
      :attr:`UIDNA_NONTRANSITIONAL_TO_UNICODE`.
      )doc");

  idna.def(
      "label_to_ascii",
      [](const IDNA &self, const icupy::UnicodeStringVariant &label,
         UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result =
            self.labelToASCII(icupy::to_unistr(label), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("label"), py::arg("dest"), py::arg("info"), R"doc(
      Convert a single domain name label into its ASCII form for DNS lookup.

      If any processing step fails, then :meth:`IDNAInfo.has_errors` will be
      ``True`` and the result might not be an ASCII string. The label might be
      modified according to the types of errors. Labels with severe errors will
      be left in (or turned into) their Unicode form.
      )doc");

  idna.def(
      "label_to_ascii_utf8",
      [](const IDNA &self, const py::bytes &label, IDNAInfo &info) {
        std::string dest;
        auto sink = StringByteSink<std::string>(&dest);
        ErrorCode error_code;
        self.labelToASCII_UTF8(StringPiece(label), sink, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(dest);
      },
      py::arg("label"), py::arg("info"), R"doc(
      Convert a single domain name label into its ASCII form for DNS lookup.

      ``bytes`` version of :meth:`.label_to_ascii`, same behavior.
      )doc");

  idna.def(
      "label_to_unicode",
      [](const IDNA &self, const icupy::UnicodeStringVariant &label,
         UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.labelToUnicode(icupy::to_unistr(label), dest, info,
                                           error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("label"), py::arg("dest"), py::arg("info"), R"doc(
      Convert a single domain name label into its Unicode form for
      human-readable display.

      If any processing step fails, then :meth:`IDNAInfo.has_errors` will be
      ``True``. The label might be modified according to the types of errors.
      )doc");

  idna.def(
      "label_to_unicode_utf8",
      [](const IDNA &self, const py::bytes &label, IDNAInfo &info) {
        std::string dest;
        auto sink = StringByteSink<std::string>(&dest);
        ErrorCode error_code;
        self.labelToUnicodeUTF8(StringPiece(label), sink, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(dest);
      },
      py::arg("label"), py::arg("info"), R"doc(
      Convert a single domain name label into its Unicode form for
      human-readable display.

      ``bytes`` version of :meth:`.label_to_unicode`, same behavior.
      )doc");

  idna.def(
      "name_to_ascii",
      [](const IDNA &self, const icupy::UnicodeStringVariant &name,
         UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result =
            self.nameToASCII(icupy::to_unistr(name), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("name"), py::arg("dest"), py::arg("info"), R"doc(
      Convert a whole domain name into its ASCII form for DNS lookup.

      If any processing step fails, then :meth:`IDNAInfo.has_errors` will be
      ``True`` and the result might not be an ASCII string. The domain name
      might be modified according to the types of errors. Labels with severe
      errors will be left in (or turned into) their Unicode form.
      )doc");

  idna.def(
      "name_to_ascii_utf8",
      [](const IDNA &self, const py::bytes &name, IDNAInfo &info) {
        std::string dest;
        auto sink = StringByteSink(&dest);
        ErrorCode error_code;
        self.nameToASCII_UTF8(StringPiece(name), sink, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(dest);
      },
      py::arg("name"), py::arg("info"), R"doc(
      Convert a whole domain name into its ASCII form for DNS lookup.

      ``bytes`` version of :meth:`.name_to_ascii`, same behavior.
      )doc");

  idna.def(
      "name_to_unicode",
      [](const IDNA &self, const icupy::UnicodeStringVariant &name,
         UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result =
            self.nameToUnicode(icupy::to_unistr(name), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("name"), py::arg("dest"), py::arg("info"), R"doc(
      Convert a whole domain name into its Unicode form for human-readable
      display.

      If any processing step fails, then :meth:`IDNAInfo.has_errors` will be
      ``True``. The domain name might be modified according to the types of
      errors.
      )doc");

  idna.def(
      "name_to_unicode_utf8",
      [](const IDNA &self, const py::bytes &name, IDNAInfo &info) {
        std::string dest;
        auto sink = StringByteSink(&dest);
        ErrorCode error_code;
        self.nameToUnicodeUTF8(StringPiece(name), sink, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(dest);
      },
      py::arg("name"), py::arg("info"), R"doc(
      Convert a whole domain name into its Unicode form for human-readable
      display.

      ``bytes`` version of :meth:`.name_to_unicode`, same behavior.
      )doc");

  idna.def_property_readonly_static(
      "ERROR_EMPTY_LABEL",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_EMPTY_LABEL; });

  idna.def_property_readonly_static(
      "ERROR_LABEL_TOO_LONG",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_LABEL_TOO_LONG; });

  idna.def_property_readonly_static("ERROR_DOMAIN_NAME_TOO_LONG",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_ERROR_DOMAIN_NAME_TOO_LONG;
                                    });

  idna.def_property_readonly_static(
      "ERROR_LEADING_HYPHEN",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_LEADING_HYPHEN; });

  idna.def_property_readonly_static("ERROR_TRAILING_HYPHEN",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_ERROR_TRAILING_HYPHEN;
                                    });

  idna.def_property_readonly_static(
      "ERROR_HYPHEN_3_4",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_HYPHEN_3_4; });

  idna.def_property_readonly_static("ERROR_LEADING_COMBINING_MARK",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_ERROR_LEADING_COMBINING_MARK;
                                    });

  idna.def_property_readonly_static(
      "ERROR_DISALLOWED",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_DISALLOWED; });

  idna.def_property_readonly_static(
      "ERROR_PUNYCODE",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_PUNYCODE; });

  idna.def_property_readonly_static(
      "ERROR_LABEL_HAS_DOT",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_LABEL_HAS_DOT; });

  idna.def_property_readonly_static("ERROR_INVALID_ACE_LABEL",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_ERROR_INVALID_ACE_LABEL;
                                    });

  idna.def_property_readonly_static(
      "ERROR_BIDI",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_BIDI; });

  idna.def_property_readonly_static(
      "ERROR_CONTEXTJ",
      [](const py::object &) -> int32_t { return UIDNA_ERROR_CONTEXTJ; });

  idna.def_property_readonly_static("ERROR_CONTEXTO_PUNCTUATION",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_ERROR_CONTEXTO_PUNCTUATION;
                                    });

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  idna.def_property_readonly_static("ERROR_CONTEXTO_DIGITS",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_ERROR_CONTEXTO_DIGITS;
                                    });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
}

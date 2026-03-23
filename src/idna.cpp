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

      0 if no errors. See UIDNA_ERROR\_... constants.

      See Also:
          :class:`UIDNAError`
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
      Create a ``IDNA`` object which implements UTS #46.

      *options* is a bit set to modify the processing and error checking.
      These should include :attr:`UIDNA_NONTRANSITIONAL_TO_ASCII` |
      :attr:`UIDNA_NONTRANSITIONAL_TO_UNICODE`.

      See Also:
          :class:`UIDNAOption`
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
      "DEFAULT", [](const py::object &) -> int32_t { return UIDNA_DEFAULT; });

  idna.def_property_readonly_static(
      "ALLOW_UNASSIGNED",
      [](const py::object &) -> int32_t { return UIDNA_ALLOW_UNASSIGNED; });

  idna.def_property_readonly_static(
      "USE_STD3_RULES",
      [](const py::object &) -> int32_t { return UIDNA_USE_STD3_RULES; });

  idna.def_property_readonly_static(
      "CHECK_BIDI",
      [](const py::object &) -> int32_t { return UIDNA_CHECK_BIDI; });

  idna.def_property_readonly_static(
      "CHECK_CONTEXTJ",
      [](const py::object &) -> int32_t { return UIDNA_CHECK_CONTEXTJ; });

  idna.def_property_readonly_static("NONTRANSITIONAL_TO_ASCII",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_NONTRANSITIONAL_TO_ASCII;
                                    });

  idna.def_property_readonly_static("NONTRANSITIONAL_TO_UNICODE",
                                    [](const py::object &) -> int32_t {
                                      return UIDNA_NONTRANSITIONAL_TO_UNICODE;
                                    });

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  idna.def_property_readonly_static(
      "CHECK_CONTEXTO",
      [](const py::object &) -> int32_t { return UIDNA_CHECK_CONTEXTO; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)

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

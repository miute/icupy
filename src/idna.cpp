#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/idna.h>

using namespace icu;

void init_idna(py::module &m) {
  //
  // icu::IDNAInfo
  //
  py::class_<IDNAInfo, UMemory> info(m, "IDNAInfo");

  info.def(py::init<>());

  info.def("get_errors", &IDNAInfo::getErrors);

  info.def("has_errors", [](const IDNAInfo &self) -> py::bool_ { return self.hasErrors(); });

  info.def("is_transitional_different",
           [](const IDNAInfo &self) -> py::bool_ { return self.isTransitionalDifferent(); });

  //
  // icu::IDNA
  //
  py::class_<IDNA, UObject> idna(m, "IDNA");

  py::enum_<decltype(UIDNA_DEFAULT)>(idna, "IDNA", py::arithmetic())
      .value("DEFAULT", UIDNA_DEFAULT,
             "Default options value: None of the other options are set.\n\n  "
             "For use in static worker and factory methods.")
#ifndef U_HIDE_DEPRECATED_API
      .value("ALLOW_UNASSIGNED", UIDNA_ALLOW_UNASSIGNED,
             "**Deprecated:** ICU 55 Use UTS #46 instead via *uidna_open_uts46()* or class *IDNA*.")
#endif // U_HIDE_DEPRECATED_API
      .value("USE_STD3_RULES", UIDNA_USE_STD3_RULES,
             "Option to check whether the input conforms to the STD3 ASCII rules, for example the restriction of "
             "labels to LDH characters (ASCII Letters, Digits and Hyphen-Minus).\n\n  "
             "For use in static worker and factory methods.")
      .value("CHECK_BIDI", UIDNA_CHECK_BIDI,
             "IDNA option to check for whether the input conforms to the BiDi rules.\n\n  "
             "For use in static worker and factory methods.\n\n  "
             "This option is ignored by the IDNA2003 implementation. (IDNA2003 always performs a BiDi check.)")
      .value("CHECK_CONTEXTJ", UIDNA_CHECK_CONTEXTJ,
             "IDNA option to check for whether the input conforms to the CONTEXTJ rules.\n\n  "
             "For use in static worker and factory methods.\n\n  "
             "This option is ignored by the IDNA2003 implementation. (The CONTEXTJ check is new in IDNA2008.)")
      .value("NONTRANSITIONAL_TO_ASCII", UIDNA_NONTRANSITIONAL_TO_ASCII,
             "IDNA option for nontransitional processing in *to_ascii()*.\n\n  "
             "For use in static worker and factory methods.\n\n  "
             "By default, *to_ascii()* uses transitional processing.\n\n  "
             "This option is ignored by the IDNA2003 implementation. (This is only relevant for compatibility of newer "
             "IDNA implementations with IDNA2003.)")
      .value("NONTRANSITIONAL_TO_UNICODE", UIDNA_NONTRANSITIONAL_TO_UNICODE,
             "IDNA option for nontransitional processing in *to_unicode()*.\n\n  "
             "For use in static worker and factory methods.\n\n  "
             "By default, *to_unicode()* uses transitional processing.\n\n  "
             "This option is ignored by the IDNA2003 implementation. (This is only relevant for compatibility of newer "
             "IDNA implementations with IDNA2003.)")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("CHECK_CONTEXTO", UIDNA_CHECK_CONTEXTO,
             "IDNA option to check for whether the input conforms to the CONTEXTO rules.\n\n  "
             "For use in static worker and factory methods.\n\n  "
             "This option is ignored by the IDNA2003 implementation. (The CONTEXTO check is new in "
             "IDNA2008.)\n\n  "
             "This is for use by registries for IDNA2008 conformance. UTS #46 does not require the CONTEXTO check.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();

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
      py::arg("options"));

  idna.def(
      "label_to_ascii",
      [](const IDNA &self, const icupy::UnicodeStringVariant &label, UnicodeString &dest,
         IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.labelToASCII(icupy::to_unistr(label), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("label"), py::arg("dest"), py::arg("info"));

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
      py::arg("label"), py::arg("info"));

  idna.def(
      "label_to_unicode",
      [](const IDNA &self, const icupy::UnicodeStringVariant &label, UnicodeString &dest,
         IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.labelToUnicode(icupy::to_unistr(label), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("label"), py::arg("dest"), py::arg("info"));

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
      py::arg("label"), py::arg("info"));

  idna.def(
      "name_to_ascii",
      [](const IDNA &self, const icupy::UnicodeStringVariant &name, UnicodeString &dest,
         IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.nameToASCII(icupy::to_unistr(name), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("name"), py::arg("dest"), py::arg("info"));

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
      py::arg("name"), py::arg("info"));

  idna.def(
      "name_to_unicode",
      [](const IDNA &self, const icupy::UnicodeStringVariant &name, UnicodeString &dest,
         IDNAInfo &info) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.nameToUnicode(icupy::to_unistr(name), dest, info, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("name"), py::arg("dest"), py::arg("info"));

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
      py::arg("name"), py::arg("info"));

  idna.def_property_readonly_static("ERROR_EMPTY_LABEL",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_EMPTY_LABEL; });

  idna.def_property_readonly_static("ERROR_LABEL_TOO_LONG",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_LABEL_TOO_LONG; });

  idna.def_property_readonly_static("ERROR_DOMAIN_NAME_TOO_LONG",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_DOMAIN_NAME_TOO_LONG; });

  idna.def_property_readonly_static("ERROR_LEADING_HYPHEN",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_LEADING_HYPHEN; });

  idna.def_property_readonly_static("ERROR_TRAILING_HYPHEN",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_TRAILING_HYPHEN; });

  idna.def_property_readonly_static("ERROR_HYPHEN_3_4",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_HYPHEN_3_4; });

  idna.def_property_readonly_static("ERROR_LEADING_COMBINING_MARK",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_LEADING_COMBINING_MARK; });

  idna.def_property_readonly_static("ERROR_DISALLOWED",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_DISALLOWED; });

  idna.def_property_readonly_static("ERROR_PUNYCODE",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_PUNYCODE; });

  idna.def_property_readonly_static("ERROR_LABEL_HAS_DOT",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_LABEL_HAS_DOT; });

  idna.def_property_readonly_static("ERROR_INVALID_ACE_LABEL",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_INVALID_ACE_LABEL; });

  idna.def_property_readonly_static("ERROR_BIDI", [](const py::object &) -> int32_t { return UIDNA_ERROR_BIDI; });

  idna.def_property_readonly_static("ERROR_CONTEXTJ",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_CONTEXTJ; });

  idna.def_property_readonly_static("ERROR_CONTEXTO_PUNCTUATION",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_CONTEXTO_PUNCTUATION; });

#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  idna.def_property_readonly_static("ERROR_CONTEXTO_DIGITS",
                                    [](const py::object &) -> int32_t { return UIDNA_ERROR_CONTEXTO_DIGITS; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
}

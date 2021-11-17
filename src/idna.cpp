#include "main.hpp"
#include <unicode/idna.h>

using namespace icu;

void init_idna(py::module &m) {
  py::class_<IDNAInfo, UMemory>(m, "IDNAInfo")
      .def(py::init<>())
      .def("get_errors", &IDNAInfo::getErrors)
      .def("has_errors", &IDNAInfo::hasErrors)
      .def("is_transitional_different", &IDNAInfo::isTransitionalDifferent);

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
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = IDNA::createUTS46Instance(options, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("options"));
  idna.def(
          "label_to_ascii",
          [](const IDNA &self, const UnicodeString &label, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.labelToASCII(label, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("label"), py::arg("dest"), py::arg("info"))
      .def(
          // const char16_t *label -> const UnicodeString &label
          "label_to_ascii",
          [](const IDNA &self, const char16_t *label, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.labelToASCII(label, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("label"), py::arg("dest"), py::arg("info"));
  idna.def(
          "label_to_unicode",
          [](const IDNA &self, const UnicodeString &label, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.labelToUnicode(label, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("label"), py::arg("dest"), py::arg("info"))
      .def(
          // const char16_t *label -> const UnicodeString &label
          "label_to_unicode",
          [](const IDNA &self, const char16_t *label, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.labelToUnicode(label, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("label"), py::arg("dest"), py::arg("info"));
  idna.def(
          "name_to_ascii",
          [](const IDNA &self, const UnicodeString &name, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.nameToASCII(name, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("name"), py::arg("dest"), py::arg("info"))
      .def(
          // const char16_t *name -> const UnicodeString &name
          "name_to_ascii",
          [](const IDNA &self, const char16_t *name, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.nameToASCII(name, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("name"), py::arg("dest"), py::arg("info"));
  idna.def(
          "name_to_unicode",
          [](const IDNA &self, const UnicodeString &name, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.nameToUnicode(name, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("name"), py::arg("dest"), py::arg("info"))
      .def(
          // const char16_t *name -> const UnicodeString &name
          "name_to_unicode",
          [](const IDNA &self, const char16_t *name, UnicodeString &dest, IDNAInfo &info) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.nameToUnicode(name, dest, info, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("name"), py::arg("dest"), py::arg("info"));

  idna.def_property_readonly_static("ERROR_EMPTY_LABEL", [](py::object) -> int32_t { return UIDNA_ERROR_EMPTY_LABEL; });
  idna.def_property_readonly_static("ERROR_LABEL_TOO_LONG",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_LABEL_TOO_LONG; });
  idna.def_property_readonly_static("ERROR_DOMAIN_NAME_TOO_LONG",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_DOMAIN_NAME_TOO_LONG; });
  idna.def_property_readonly_static("ERROR_LEADING_HYPHEN",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_LEADING_HYPHEN; });
  idna.def_property_readonly_static("ERROR_TRAILING_HYPHEN",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_TRAILING_HYPHEN; });
  idna.def_property_readonly_static("ERROR_HYPHEN_3_4", [](py::object) -> int32_t { return UIDNA_ERROR_HYPHEN_3_4; });
  idna.def_property_readonly_static("ERROR_LEADING_COMBINING_MARK",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_LEADING_COMBINING_MARK; });
  idna.def_property_readonly_static("ERROR_DISALLOWED", [](py::object) -> int32_t { return UIDNA_ERROR_DISALLOWED; });
  idna.def_property_readonly_static("ERROR_PUNYCODE", [](py::object) -> int32_t { return UIDNA_ERROR_PUNYCODE; });
  idna.def_property_readonly_static("ERROR_LABEL_HAS_DOT",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_LABEL_HAS_DOT; });
  idna.def_property_readonly_static("ERROR_INVALID_ACE_LABEL",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_INVALID_ACE_LABEL; });
  idna.def_property_readonly_static("ERROR_BIDI", [](py::object) -> int32_t { return UIDNA_ERROR_BIDI; });
  idna.def_property_readonly_static("ERROR_CONTEXTJ", [](py::object) -> int32_t { return UIDNA_ERROR_CONTEXTJ; });
  idna.def_property_readonly_static("ERROR_CONTEXTO_PUNCTUATION",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_CONTEXTO_PUNCTUATION; });
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  idna.def_property_readonly_static("ERROR_CONTEXTO_DIGITS",
                                    [](py::object) -> int32_t { return UIDNA_ERROR_CONTEXTO_DIGITS; });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
}

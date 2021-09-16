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
      .value("DEFAULT", UIDNA_DEFAULT)
#ifndef U_HIDE_DEPRECATED_API
      .value("ALLOW_UNASSIGNED", UIDNA_ALLOW_UNASSIGNED)
#endif // U_HIDE_DEPRECATED_API
      .value("USE_STD3_RULES", UIDNA_USE_STD3_RULES)
      .value("CHECK_BIDI", UIDNA_CHECK_BIDI)
      .value("CHECK_CONTEXTJ", UIDNA_CHECK_CONTEXTJ)
      .value("NONTRANSITIONAL_TO_ASCII", UIDNA_NONTRANSITIONAL_TO_ASCII)
      .value("NONTRANSITIONAL_TO_UNICODE", UIDNA_NONTRANSITIONAL_TO_UNICODE)
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("CHECK_CONTEXTO", UIDNA_CHECK_CONTEXTO)
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

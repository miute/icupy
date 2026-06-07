#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
#include <pybind11/native_enum.h>
#include <unicode/udisplayoptions.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

void init_udisplayoptions(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  //
  // enum UDisplayOptionsCapitalization
  //
  py::native_enum<UDisplayOptionsCapitalization>(
      m, "UDisplayOptionsCapitalization", "enum.IntEnum", R"doc(
Represent all the capitalization options.
      )doc")
      .value("UDISPOPT_CAPITALIZATION_UNDEFINED",
             UDISPOPT_CAPITALIZATION_UNDEFINED, R"doc(
             A possible setting for Capitalization.

             The capitalization context to be used is unknown (this is the
             default value).
             )doc")
      .value("UDISPOPT_CAPITALIZATION_BEGINNING_OF_SENTENCE",
             UDISPOPT_CAPITALIZATION_BEGINNING_OF_SENTENCE, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for the
             beginning of a sentence.
             )doc")
      .value("UDISPOPT_CAPITALIZATION_MIDDLE_OF_SENTENCE",
             UDISPOPT_CAPITALIZATION_MIDDLE_OF_SENTENCE, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for the middle
             of a sentence.
             )doc")
      .value("UDISPOPT_CAPITALIZATION_STANDALONE",
             UDISPOPT_CAPITALIZATION_STANDALONE, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for stand-alone
             usage such as an isolated name on a calendar page.
             )doc")
      .value("UDISPOPT_CAPITALIZATION_UI_LIST_OR_MENU",
             UDISPOPT_CAPITALIZATION_UI_LIST_OR_MENU, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for a
             user-interface list or menu item.
             )doc")
      .export_values()
      .finalize();

  //
  // enum UDisplayOptionsDisplayLength
  //
  py::native_enum<UDisplayOptionsDisplayLength>(
      m, "UDisplayOptionsDisplayLength", "enum.IntEnum", R"doc(
Represent all the display lengths.
      )doc")
      .value("UDISPOPT_DISPLAY_LENGTH_UNDEFINED",
             UDISPOPT_DISPLAY_LENGTH_UNDEFINED, R"doc(
             A possible setting for DisplayLength.

             The DisplayLength context to be used is unknown (this is the
             default value).
             )doc")
      .value("UDISPOPT_DISPLAY_LENGTH_FULL", UDISPOPT_DISPLAY_LENGTH_FULL,
             R"doc(
             Uses full names when generating a locale name, e.g.
             "United States" for US.
             )doc")
      .value("UDISPOPT_DISPLAY_LENGTH_SHORT", UDISPOPT_DISPLAY_LENGTH_SHORT,
             R"doc(
             Use short names when generating a locale name, e.g.
             "U.S." for US.
             )doc")
      .export_values()
      .finalize();

  //
  // enum UDisplayOptionsGrammaticalCase
  //
  py::native_enum<UDisplayOptionsGrammaticalCase>(
      m, "UDisplayOptionsGrammaticalCase", "enum.IntEnum", R"doc(
Represent all the grammatical cases that are supported by CLDR.
      )doc")
      .value("UDISPOPT_GRAMMATICAL_CASE_UNDEFINED",
             UDISPOPT_GRAMMATICAL_CASE_UNDEFINED, R"doc(
             A possible setting for GrammaticalCase.

             The grammatical case context to be used is unknown (this is the
             default value).
             )doc")
      .value("UDISPOPT_GRAMMATICAL_CASE_ABLATIVE",
             UDISPOPT_GRAMMATICAL_CASE_ABLATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_ACCUSATIVE",
             UDISPOPT_GRAMMATICAL_CASE_ACCUSATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_COMITATIVE",
             UDISPOPT_GRAMMATICAL_CASE_COMITATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_DATIVE",
             UDISPOPT_GRAMMATICAL_CASE_DATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_ERGATIVE",
             UDISPOPT_GRAMMATICAL_CASE_ERGATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_GENITIVE",
             UDISPOPT_GRAMMATICAL_CASE_GENITIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_INSTRUMENTAL",
             UDISPOPT_GRAMMATICAL_CASE_INSTRUMENTAL, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_LOCATIVE",
             UDISPOPT_GRAMMATICAL_CASE_LOCATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_LOCATIVE_COPULATIVE",
             UDISPOPT_GRAMMATICAL_CASE_LOCATIVE_COPULATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_NOMINATIVE",
             UDISPOPT_GRAMMATICAL_CASE_NOMINATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_OBLIQUE",
             UDISPOPT_GRAMMATICAL_CASE_OBLIQUE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_PREPOSITIONAL",
             UDISPOPT_GRAMMATICAL_CASE_PREPOSITIONAL, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_SOCIATIVE",
             UDISPOPT_GRAMMATICAL_CASE_SOCIATIVE, "")
      .value("UDISPOPT_GRAMMATICAL_CASE_VOCATIVE",
             UDISPOPT_GRAMMATICAL_CASE_VOCATIVE, "")
      .export_values()
      .finalize();

  //
  // enum UDisplayOptionsNameStyle
  //
  py::native_enum<UDisplayOptionsNameStyle>(m, "UDisplayOptionsNameStyle",
                                            "enum.IntEnum", R"doc(
Represent all the dialect handlings.
      )doc")
      .value("UDISPOPT_NAME_STYLE_UNDEFINED", UDISPOPT_NAME_STYLE_UNDEFINED,
             R"doc(
             A possible setting for NameStyle.

             The NameStyle context to be used is unknown (this is the default
             value).
             )doc")
      .value("UDISPOPT_NAME_STYLE_STANDARD_NAMES",
             UDISPOPT_NAME_STYLE_STANDARD_NAMES, R"doc(
             Use standard names when generating a locale name, e.g. en_GB
             displays as "English (United Kingdom)".
             )doc")
      .value("UDISPOPT_NAME_STYLE_DIALECT_NAMES",
             UDISPOPT_NAME_STYLE_DIALECT_NAMES, R"doc(
             Use dialect names, when generating a locale name, e.g. en_GB
             displays as "British English".
             )doc")
      .export_values()
      .finalize();

  //
  // enum UDisplayOptionsNounClass
  //
  py::native_enum<UDisplayOptionsNounClass>(m, "UDisplayOptionsNounClass",
                                            "enum.IntEnum", R"doc(
Represent all the grammatical noun classes that are supported by CLDR.
      )doc")
      .value("UDISPOPT_NOUN_CLASS_UNDEFINED", UDISPOPT_NOUN_CLASS_UNDEFINED,
             R"doc(
             A possible setting for NounClass.

             The noun class case context to be used is unknown (this is the
             default value).
             )doc")
      .value("UDISPOPT_NOUN_CLASS_OTHER", UDISPOPT_NOUN_CLASS_OTHER, "")
      .value("UDISPOPT_NOUN_CLASS_NEUTER", UDISPOPT_NOUN_CLASS_NEUTER, "")
      .value("UDISPOPT_NOUN_CLASS_FEMININE", UDISPOPT_NOUN_CLASS_FEMININE, "")
      .value("UDISPOPT_NOUN_CLASS_MASCULINE", UDISPOPT_NOUN_CLASS_MASCULINE, "")
      .value("UDISPOPT_NOUN_CLASS_ANIMATE", UDISPOPT_NOUN_CLASS_ANIMATE, "")
      .value("UDISPOPT_NOUN_CLASS_INANIMATE", UDISPOPT_NOUN_CLASS_INANIMATE, "")
      .value("UDISPOPT_NOUN_CLASS_PERSONAL", UDISPOPT_NOUN_CLASS_PERSONAL, "")
      .value("UDISPOPT_NOUN_CLASS_COMMON", UDISPOPT_NOUN_CLASS_COMMON, "")
      .export_values()
      .finalize();

  //
  // enum UDisplayOptionsPluralCategory
  //
  py::native_enum<UDisplayOptionsPluralCategory>(
      m, "UDisplayOptionsPluralCategory", "enum.IntEnum", R"doc(
Standard CLDR plural form/category constants.

See https://www.unicode.org/reports/tr35/tr35-numbers.html#Language_Plural_Rules
      )doc")
      .value("UDISPOPT_PLURAL_CATEGORY_UNDEFINED",
             UDISPOPT_PLURAL_CATEGORY_UNDEFINED, R"doc(
             A possible setting for PluralCategory.

             The plural category case context to be used is unknown (this is
             the default value).
             )doc")
      .value("UDISPOPT_PLURAL_CATEGORY_ZERO", UDISPOPT_PLURAL_CATEGORY_ZERO, "")
      .value("UDISPOPT_PLURAL_CATEGORY_ONE", UDISPOPT_PLURAL_CATEGORY_ONE, "")
      .value("UDISPOPT_PLURAL_CATEGORY_TWO", UDISPOPT_PLURAL_CATEGORY_TWO, "")
      .value("UDISPOPT_PLURAL_CATEGORY_FEW", UDISPOPT_PLURAL_CATEGORY_FEW, "")
      .value("UDISPOPT_PLURAL_CATEGORY_MANY", UDISPOPT_PLURAL_CATEGORY_MANY, "")
      .value("UDISPOPT_PLURAL_CATEGORY_OTHER", UDISPOPT_PLURAL_CATEGORY_OTHER,
             "")
      .export_values()
      .finalize();

  //
  // enum UDisplayOptionsSubstituteHandling
  //
  py::native_enum<UDisplayOptionsSubstituteHandling>(
      m, "UDisplayOptionsSubstituteHandling", "enum.IntEnum", R"doc(
Represent all the substitute handling.
      )doc")
      .value("UDISPOPT_SUBSTITUTE_HANDLING_UNDEFINED",
             UDISPOPT_SUBSTITUTE_HANDLING_UNDEFINED, R"doc(
             A possible setting for SubstituteHandling.

             The SubstituteHandling context to be used is unknown (this is the
             default value).
             )doc")
      .value("UDISPOPT_SUBSTITUTE_HANDLING_SUBSTITUTE",
             UDISPOPT_SUBSTITUTE_HANDLING_SUBSTITUTE, R"doc(
             Returns a fallback value (e.g., the input code) when no data is
             available.

             This is the default behaviour.
             )doc")
      .value("UDISPOPT_SUBSTITUTE_HANDLING_NO_SUBSTITUTE",
             UDISPOPT_SUBSTITUTE_HANDLING_NO_SUBSTITUTE, R"doc(
             Returns a null value when no data is available.
             )doc")
      .export_values()
      .finalize();

  //
  // C APIs
  //
  m.def(
      "udispopt_from_grammatical_case_identifier",
      [](const std::string &identifier) {
        return udispopt_fromGrammaticalCaseIdentifier(identifier.data());
      },
      py::arg("identifier"));

  m.def(
      "udispopt_from_noun_class_identifier",
      [](const std::string &identifier) {
        return udispopt_fromNounClassIdentifier(identifier.data());
      },
      py::arg("identifier"));

  m.def(
      "udispopt_from_plural_category_identifier",
      [](const std::string &identifier) {
        return udispopt_fromPluralCategoryIdentifier(identifier.data());
      },
      py::arg("identifier"));

  m.def("udispopt_get_grammatical_case_identifier",
        &udispopt_getGrammaticalCaseIdentifier, py::arg("grammatical_case"));

  m.def("udispopt_get_noun_class_identifier", &udispopt_getNounClassIdentifier,
        py::arg("noun_class"));

  m.def("udispopt_get_plural_category_identifier",
        &udispopt_getPluralCategoryIdentifier, py::arg("plural_category"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)
}

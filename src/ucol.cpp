#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/ucol.h>

void init_ucol(py::module &m) {
  // UCollationStrength

  py::enum_<UColAttribute>(
      m, "UColAttribute", py::arithmetic(),
      "Attributes that collation service understands.\n\n"
      "All the attributes can take *UCOL_DEFAULT* value, as well as the values specific to each one.")
      .value("UCOL_FRENCH_COLLATION", UCOL_FRENCH_COLLATION,
             "Attribute for direction of secondary weights - used in Canadian French.\n\n  "
             "Acceptable values are *UCOL_ON*, which results in secondary weights being considered backwards and "
             "*UCOL_OFF* which treats secondary weights in the order they appear.")
      .value("UCOL_ALTERNATE_HANDLING", UCOL_ALTERNATE_HANDLING,
             "Attribute for handling variable elements.\n\n  "
             "Acceptable values are *UCOL_NON_IGNORABLE* (default) which treats all the codepoints with non-ignorable "
             "primary weights in the same way, and *UCOL_SHIFTED* which causes codepoints with primary weights that "
             "are equal or below the variable top value to be ignored on primary level and moved to the quaternary "
             "level.")
      .value("UCOL_CASE_FIRST", UCOL_CASE_FIRST,
             "Controls the ordering of upper and lower case letters.\n\n  "
             "Acceptable values are *UCOL_OFF* (default), which orders upper and lower case letters in accordance to "
             "their tertiary weights, *UCOL_UPPER_FIRST* which forces upper case letters to sort before lower case "
             "letters, and *UCOL_LOWER_FIRST* which does the opposite.")
      .value("UCOL_CASE_LEVEL", UCOL_CASE_LEVEL,
             "Controls whether an extra case level (positioned before the third level) is generated or not.\n\n  "
             "Acceptable values are *UCOL_OFF* (default), when case level is not generated, and *UCOL_ON* which causes "
             "the case level to be generated. Contents of the case level are affected by the value of "
             "*UCOL_CASE_FIRST* attribute. A simple way to ignore accent differences in a string is to set the "
             "strength to *UCOL_PRIMARY* and enable case level.")
      .value("UCOL_NORMALIZATION_MODE", UCOL_NORMALIZATION_MODE,
             "Controls whether the normalization check and necessary normalizations are performed.\n\n  "
             "When set to *UCOL_OFF* (default) no normalization check is performed. The correctness of the result is "
             "guaranteed only if the input data is in so-called FCD form (see users manual for more info). When set to "
             "*UCOL_ON*, an incremental check is performed to see whether the input data is in the FCD form. If the "
             "data is not in the FCD form, incremental NFD normalization is performed.")
      .value("UCOL_DECOMPOSITION_MODE", UCOL_DECOMPOSITION_MODE, "An alias for *UCOL_NORMALIZATION_MODE* attribute.")
      .value("UCOL_STRENGTH", UCOL_STRENGTH,
             "The strength attribute.\n\n  "
             "Can be either *UCOL_PRIMARY*, *UCOL_SECONDARY*, *UCOL_TERTIARY*, *UCOL_QUATERNARY* or *UCOL_IDENTICAL*. "
             "The usual strength for most locales (except Japanese) is tertiary.\n\n  "
             "Quaternary strength is useful when combined with shifted setting for alternate handling attribute and "
             "for JIS X 4061 collation, when it is used to distinguish between Katakana and Hiragana. Otherwise, "
             "quaternary level is affected only by the number of non-ignorable code points in the string.\n\n  "
             "Identical strength is rarely useful, as it amounts to codepoints of the NFD form of the string.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_HIRAGANA_QUATERNARY_MODE", UCOL_HIRAGANA_QUATERNARY_MODE,
             "**Deprecated:** ICU 50 Implementation detail, cannot be set via API, was removed from implementation.")
#endif // U_HIDE_DEPRECATED_API
      .value("UCOL_NUMERIC_COLLATION", UCOL_NUMERIC_COLLATION,
             "When turned on, this attribute makes substrings of digits sort according to their numeric values.\n\n  "
             "This is a way to get '100' to sort AFTER '2'. Note that the longest digit substring that can be treated "
             "as a single unit is 254 digits (not counting leading zeros). If a digit substring is longer than that, "
             "the digits beyond the limit will be treated as a separate digit substring.\n\n  "
             "A \"digit\" in this sense is a code point with General_Category=Nd, which does not include circled "
             "numbers, roman numerals, etc. Only a contiguous digit substring is considered, that is, non-negative "
             "integers without separators. There is no support for plus/minus signs, decimals, exponents, etc.")
#ifndef U_FORCE_HIDE_DEPRECATED_API
      .value("UCOL_ATTRIBUTE_COUNT", UCOL_ATTRIBUTE_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_FORCE_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UColAttributeValue>(
      m, "UColAttributeValue", py::arithmetic(),
      "Enum containing attribute values for controlling collation behavior.\n\n"
      "Here are all the allowable values. Not every attribute can take every value. The only universal value is "
      "*UCOL_DEFAULT*, which resets the attribute value to the predefined value for that locale.")
      .value("UCOL_DEFAULT", UCOL_DEFAULT, "Accepted by most attributes.")
      .value("UCOL_PRIMARY", UCOL_PRIMARY, "Primary collation strength.")
      .value("UCOL_SECONDARY", UCOL_SECONDARY, "Secondary collation strength.")
      .value("UCOL_TERTIARY", UCOL_TERTIARY, "Tertiary collation strength.")
      .value("UCOL_DEFAULT_STRENGTH", UCOL_DEFAULT_STRENGTH, "Default collation strength.")
      .value("UCOL_CE_STRENGTH_LIMIT", UCOL_CE_STRENGTH_LIMIT)
      .value("UCOL_QUATERNARY", UCOL_QUATERNARY, "Quaternary collation strength.")
      .value("UCOL_IDENTICAL", UCOL_IDENTICAL, "Identical collation strength.")
      .value("UCOL_STRENGTH_LIMIT", UCOL_STRENGTH_LIMIT)
      .value("UCOL_OFF", UCOL_OFF,
             "Turn the feature off - works for *UCOL_FRENCH_COLLATION*, *UCOL_CASE_LEVEL*, "
             "*UCOL_HIRAGANA_QUATERNARY_MODE* & *UCOL_DECOMPOSITION_MODE*.")
      .value("UCOL_ON", UCOL_ON,
             "Turn the feature off - works for *UCOL_FRENCH_COLLATION*, *UCOL_CASE_LEVEL*, "
             "*UCOL_HIRAGANA_QUATERNARY_MODE* & *UCOL_DECOMPOSITION_MODE*.")
      .value("UCOL_SHIFTED", UCOL_SHIFTED,
             "Valid for *UCOL_ALTERNATE_HANDLING*.\n\n  "
             "Alternate handling will be shifted.")
      .value("UCOL_NON_IGNORABLE", UCOL_NON_IGNORABLE,
             "Valid for *UCOL_ALTERNATE_HANDLING*.\n\n  "
             "Alternate handling will be non ignorable.")
      .value("UCOL_LOWER_FIRST", UCOL_LOWER_FIRST, "Valid for *UCOL_CASE_FIRST* - lower case sorts before upper case.")
      .value("UCOL_UPPER_FIRST", UCOL_UPPER_FIRST, "Upper case sorts before lower case.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_ATTRIBUTE_VALUE_COUNT", UCOL_ATTRIBUTE_VALUE_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UColBoundMode>(m, "UColBoundMode", py::arithmetic())
      .value("UCOL_BOUND_LOWER", UCOL_BOUND_LOWER, "Lower bound.")
      .value("UCOL_BOUND_UPPER", UCOL_BOUND_UPPER, "Upper bound that will match strings of exact size.")
      .value("UCOL_BOUND_UPPER_LONG", UCOL_BOUND_UPPER_LONG,
             "Upper bound that will match all the strings that have the same initial substring as the given string.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_BOUND_VALUE_COUNT", UCOL_BOUND_VALUE_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UCollationResult>(m, "UCollationResult", py::arithmetic(),
                              "*UCOL_LESS* is returned if source string is compared to be less than target string in "
                              "the *ucol_strcoll()* method.\n\n"
                              "*UCOL_EQUAL* is returned if source string is compared to be equal to target string in "
                              "the *ucol_strcoll()* method. *UCOL_GREATER* is returned if source string is compared to "
                              "be greater than target string in the *ucol_strcoll()* method.")
      .value("UCOL_EQUAL", UCOL_EQUAL, "string a == string b")
      .value("UCOL_GREATER", UCOL_GREATER, "string a > string b")
      .value("UCOL_LESS", UCOL_LESS, "string a < string b")
      .export_values();

  py::enum_<UColReorderCode>(
      m, "UColReorderCode", py::arithmetic(),
      "Enum containing the codes for reordering segments of the collation table that are not script codes.\n\n"
      "These reordering codes are to be used in conjunction with the script codes.")
      .value("UCOL_REORDER_CODE_DEFAULT", UCOL_REORDER_CODE_DEFAULT,
             "A special reordering code that is used to specify the default reordering codes for a locale.")
      .value("UCOL_REORDER_CODE_NONE", UCOL_REORDER_CODE_NONE,
             "A special reordering code that is used to specify no reordering codes.")
      .value("UCOL_REORDER_CODE_OTHERS", UCOL_REORDER_CODE_OTHERS,
             "A special reordering code that is used to specify all other codes used for reordering except for the "
             "codes lised as *UColReorderCode* values and those listed explicitly in a reordering.")
      .value("UCOL_REORDER_CODE_SPACE", UCOL_REORDER_CODE_SPACE,
             "Characters with the space property.\n\n  "
             "This is equivalent to the rule value \"space\".")
      .value("UCOL_REORDER_CODE_FIRST", UCOL_REORDER_CODE_FIRST,
             "The first entry in the enumeration of reordering groups.\n\n  "
             "This is intended for use in range checking and enumeration of the reorder codes.")
      .value("UCOL_REORDER_CODE_PUNCTUATION", UCOL_REORDER_CODE_PUNCTUATION,
             "Characters with the punctuation property.\n\n  "
             "This is equivalent to the rule value \"punct\".")
      .value("UCOL_REORDER_CODE_SYMBOL", UCOL_REORDER_CODE_SYMBOL,
             "Characters with the symbol property.\n\n  "
             "This is equivalent to the rule value \"symbol\".")
      .value("UCOL_REORDER_CODE_CURRENCY", UCOL_REORDER_CODE_CURRENCY,
             "Characters with the currency property.\n\n  "
             "This is equivalent to the rule value \"currency\".")
      .value("UCOL_REORDER_CODE_DIGIT", UCOL_REORDER_CODE_DIGIT,
             "Characters with the digit property.\n\n  "
             "This is equivalent to the rule value \"digit\".")
#ifndef U_HIDE_DEPRECATED_API
      .value("UCOL_REORDER_CODE_LIMIT", UCOL_REORDER_CODE_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UColRuleOption>(m, "UColRuleOption", py::arithmetic(), "Options for retrieving the rule string.")
      .value("UCOL_TAILORING_ONLY", UCOL_TAILORING_ONLY,
             "Retrieves the tailoring rules only.\n\n  "
             "Same as calling the version of *get_rules()* without *UColRuleOption*.")
      .value("UCOL_FULL_RULES", UCOL_FULL_RULES,
             "Retrieves the \"UCA rules\" concatenated with the tailoring rules.\n\n  "
             "The \"UCA rules\" are an approximation of the root collator's sort order. They are almost never used or "
             "useful at runtime and can be removed from the data. See "
             "https://unicode-org.github.io/icu/userguide/collation/customization#building-on-existing-locales")
      .export_values();
}

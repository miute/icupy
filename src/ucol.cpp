#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/ucol.h>

void init_ucol(py::module &m) {
  //
  // enum UColAttribute
  //
  py::enum_<UColAttribute>(m, "UColAttribute", py::arithmetic(), R"doc(
Attributes that collation service understands.

All the attributes can take :attr:`~UColAttributeValue.UCOL_DEFAULT` value,
as well as the values specific to each one.
      )doc")
      .value("UCOL_FRENCH_COLLATION", UCOL_FRENCH_COLLATION, R"doc(
             Attribute for direction of secondary weights - used in Canadian
             French.

             Acceptable values are :attr:`~UColAttributeValue.UCOL_ON`, which
             results in secondary weights being considered backwards and
             :attr:`~UColAttributeValue.UCOL_OFF` which treats secondary
             weights in the order they appear.
             )doc")
      .value("UCOL_ALTERNATE_HANDLING", UCOL_ALTERNATE_HANDLING, R"doc(
             Attribute for handling variable elements.

             Acceptable values are
             :attr:`~UColAttributeValue.UCOL_NON_IGNORABLE` which treats all
             the codepoints with non-ignorable primary weights in the same way,
             and :attr:`~UColAttributeValue.UCOL_SHIFTED` which causes
             codepoints with primary weights that are equal or below the
             variable top value to be ignored on primary level and moved to the
             quaternary level. The default setting in a Collator object depends
             on the locale data loaded from the resources. For most locales,
             the default is :attr:`~UColAttributeValue.UCOL_NON_IGNORABLE`, but
             for others, such as "th", the default could be
             :attr:`~UColAttributeValue.UCOL_SHIFTED`.
             )doc")
      .value("UCOL_CASE_FIRST", UCOL_CASE_FIRST, R"doc(
             Controls the ordering of upper and lower case letters.

             Acceptable values are :attr:`~UColAttributeValue.UCOL_OFF`, which
             orders upper and lower case letters in accordance to their
             tertiary weights, :attr:`~UColAttributeValue.UCOL_UPPER_FIRST`
             which forces upper case letters to sort before lower case letters,
             and :attr:`~UColAttributeValue.UCOL_LOWER_FIRST` which does the
             opposite. The default setting in a Collator object depends on the
             locale data loaded from the resources. For most locales, the
             default is :attr:`~UColAttributeValue.UCOL_OFF`, but for others,
             such as "da" or "mt", the default could be
             :attr:`~UColAttributeValue.UCOL_UPPER_FIRST`.
             )doc")
      .value("UCOL_CASE_LEVEL", UCOL_CASE_LEVEL, R"doc(
             Controls whether an extra case level (positioned before the third
             level) is generated or not.

             Acceptable values are :attr:`~UColAttributeValue.UCOL_OFF`, when
             case level is not generated, and
             :attr:`~UColAttributeValue.UCOL_ON` which causes the case level to
             be generated. Contents of the case level are affected by the value
             of UCOL_CASE_FIRST attribute. A simple way to ignore accent
             differences in a string is to set the strength to
             :attr:`~UColAttributeValue.UCOL_PRIMARY` and enable case level.
             The default setting in a Collator object depends on the locale
             data loaded from the resources.
             )doc")
      .value("UCOL_NORMALIZATION_MODE", UCOL_NORMALIZATION_MODE, R"doc(
             Controls whether the normalization check and necessary
             normalizations are performed.

             When set to :attr:`~UColAttributeValue.UCOL_OFF` no normalization
             check is performed. The correctness of the result is guaranteed
             only if the input data is in so-called FCD form (see users manual
             for more info). When set to :attr:`~UColAttributeValue.UCOL_ON`,
             an incremental check is performed to see whether the input data is
             in the FCD form. If the data is not in the FCD form, incremental
             NFD normalization is performed. The default setting in a Collator
             object depends on the locale data loaded from the resources. For
             many locales, the default is :attr:`~UColAttributeValue.UCOL_OFF`,
             but for others, such as "hi" "vi', or "bn", \* the default could be
             :attr:`~UColAttributeValue.UCOL_ON`.
             )doc")
      .value("UCOL_DECOMPOSITION_MODE", UCOL_DECOMPOSITION_MODE, R"doc(
             An alias for UCOL_NORMALIZATION_MODE attribute.
             )doc")
      .value("UCOL_STRENGTH", UCOL_STRENGTH, R"doc(
             The strength attribute.

             Can be either :attr:`~UColAttributeValue.UCOL_PRIMARY`,
             :attr:`~UColAttributeValue.UCOL_SECONDARY`,
             :attr:`~UColAttributeValue.UCOL_TERTIARY`,
             :attr:`~UColAttributeValue.UCOL_QUATERNARY`, or
             :attr:`~UColAttributeValue.UCOL_IDENTICAL`. The usual strength for
             most locales (except Japanese) is tertiary.

             Quaternary strength is useful when combined with shifted setting
             for alternate handling attribute and for JIS X 4061 collation,
             when it is used to distinguish between Katakana and Hiragana.
             Otherwise, quaternary level is affected only by the number of
             non-ignorable code points in the string.

             Identical strength is rarely useful, as it amounts to codepoints
             of the NFD form of the string.
             )doc")
      .value("UCOL_HIRAGANA_QUATERNARY_MODE", UCOL_HIRAGANA_QUATERNARY_MODE,
             R"doc(
             Deprecated: ICU 50 Implementation detail, cannot be set via API,
             was removed from implementation.
             )doc")
      .value("UCOL_NUMERIC_COLLATION", UCOL_NUMERIC_COLLATION, R"doc(
             When turned on, this attribute makes substrings of digits sort
             according to their numeric values.

             This is a way to get '100' to sort AFTER '2'. Note that the
             longest digit substring that can be treated as a single unit is
             254 digits (not counting leading zeros). If a digit substring is
             longer than that, the digits beyond the limit will be treated as a
             separate digit substring.

             A "digit" in this sense is a code point with General_Category=Nd,
             which does not include circled numbers, roman numerals, etc. Only
             a contiguous digit substring is considered, that is, non-negative
             integers without separators. There is no support for plus/minus
             signs, decimals, exponents, etc.
             )doc")
      .value("UCOL_ATTRIBUTE_COUNT", UCOL_ATTRIBUTE_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UColAttributeValue
  //
  py::enum_<UColAttributeValue>(m, "UColAttributeValue", py::arithmetic(),
                                R"doc(
Enum containing attribute values for controlling collation behavior.

Here are all the allowable values. Not every attribute can take every value.
The only universal value is :attr:`UCOL_DEFAULT`, which resets the attribute
value to the predefined value for that locale.
      )doc")
      .value("UCOL_DEFAULT", UCOL_DEFAULT, R"doc(
             Accepted by most attributes.
             )doc")
      .value("UCOL_PRIMARY", UCOL_PRIMARY, R"doc(
             Primary collation strength.
             )doc")
      .value("UCOL_SECONDARY", UCOL_SECONDARY, R"doc(
             Secondary collation strength.
             )doc")
      .value("UCOL_TERTIARY", UCOL_TERTIARY, R"doc(
             Tertiary collation strength.
             )doc")
      .value("UCOL_DEFAULT_STRENGTH", UCOL_DEFAULT_STRENGTH, R"doc(
             Default collation strength.
             )doc")
      .value("UCOL_CE_STRENGTH_LIMIT", UCOL_CE_STRENGTH_LIMIT)
      .value("UCOL_QUATERNARY", UCOL_QUATERNARY, R"doc(
             Quaternary collation strength.
             )doc")
      .value("UCOL_IDENTICAL", UCOL_IDENTICAL, R"doc(
             Identical collation strength.
             )doc")
      .value("UCOL_STRENGTH_LIMIT", UCOL_STRENGTH_LIMIT)
      .value("UCOL_OFF", UCOL_OFF, R"doc(
             Turn the feature off - works for
             :attr:`~UColAttribute.UCOL_FRENCH_COLLATION`,
             :attr:`~UColAttribute.UCOL_CASE_LEVEL`,
             :attr:`~UColAttribute.UCOL_HIRAGANA_QUATERNARY_MODE`, and
             :attr:`~UColAttribute.UCOL_DECOMPOSITION_MODE`.
             )doc")
      .value("UCOL_ON", UCOL_ON, R"doc(
             Turn the feature on - works for
             :attr:`~UColAttribute.UCOL_FRENCH_COLLATION`,
             :attr:`~UColAttribute.UCOL_CASE_LEVEL`,
             :attr:`~UColAttribute.UCOL_HIRAGANA_QUATERNARY_MODE`, and
             :attr:`~UColAttribute.UCOL_DECOMPOSITION_MODE`.
             )doc")
      .value("UCOL_SHIFTED", UCOL_SHIFTED, R"doc(
             Valid for :attr:`~UColAttribute.UCOL_ALTERNATE_HANDLING`.

             Alternate handling will be shifted.
             )doc")
      .value("UCOL_NON_IGNORABLE", UCOL_NON_IGNORABLE, R"doc(
             Valid for :attr:`~UColAttribute.UCOL_ALTERNATE_HANDLING`.

             Alternate handling will be non ignorable.
             )doc")
      .value("UCOL_LOWER_FIRST", UCOL_LOWER_FIRST, R"doc(
             Valid for :attr:`~UColAttribute.UCOL_CASE_FIRST` - lower case
             sorts before upper case.
             )doc")
      .value("UCOL_UPPER_FIRST", UCOL_UPPER_FIRST, R"doc(
             Upper case sorts before lower case.
             )doc")
      .value("UCOL_ATTRIBUTE_VALUE_COUNT", UCOL_ATTRIBUTE_VALUE_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UColBoundMode
  //
  py::enum_<UColBoundMode>(m, "UColBoundMode", py::arithmetic(), R"doc(
Enum that is taken by :meth:`Collator.get_bound`. See below for explanation do
not change the values assigned to the members of this enum.

Underlying code depends on them having these numbers.
      )doc")
      .value("UCOL_BOUND_LOWER", UCOL_BOUND_LOWER, R"doc(
             Lower bound.
             )doc")
      .value("UCOL_BOUND_UPPER", UCOL_BOUND_UPPER, R"doc(
             Upper bound that will match strings of exact size.
             )doc")
      .value("UCOL_BOUND_UPPER_LONG", UCOL_BOUND_UPPER_LONG, R"doc(
             Upper bound that will match all the strings that have the same
             initial substring as the given string.
             )doc")
      .value("UCOL_BOUND_VALUE_COUNT", UCOL_BOUND_VALUE_COUNT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UCollationResult
  //
  py::enum_<UCollationResult>(m, "UCollationResult", py::arithmetic(), R"doc(
:attr:`UCOL_LESS` is returned if source string is compared to be less than
target string in the :meth:`Collator.compare` method.

:attr:`UCOL_EQUAL` is returned if source string is compared to be equal to
target string in the :meth:`Collator.compare` method.

:attr:`UCOL_GREATER` is returned if source string is compared to be greater
than target string in the :meth:`Collator.compare` method.
      )doc")
      .value("UCOL_EQUAL", UCOL_EQUAL, R"doc(
             string a == string b
             )doc")
      .value("UCOL_GREATER", UCOL_GREATER, R"doc(
             string a > string b
             )doc")
      .value("UCOL_LESS", UCOL_LESS, R"doc(
             string a < string b
             )doc")
      .export_values();

  //
  // enum UColReorderCode
  //
  py::enum_<UColReorderCode>(m, "UColReorderCode", py::arithmetic(), R"doc(
Enum containing the codes for reordering segments of the collation table that
are not script codes.

These reordering codes are to be used in conjunction with the script codes.
      )doc")
      .value("UCOL_REORDER_CODE_DEFAULT", UCOL_REORDER_CODE_DEFAULT, R"doc(
             A special reordering code that is used to specify the default
             reordering codes for a locale.
             )doc")
      .value("UCOL_REORDER_CODE_NONE", UCOL_REORDER_CODE_NONE, R"doc(
             A special reordering code that is used to specify no reordering
             codes.
             )doc")
      .value("UCOL_REORDER_CODE_OTHERS", UCOL_REORDER_CODE_OTHERS, R"doc(
             A special reordering code that is used to specify all other codes
             used for reordering except for the codes listed as
             UColReorderCode values and those listed explicitly in a
             reordering.
             )doc")
      .value("UCOL_REORDER_CODE_SPACE", UCOL_REORDER_CODE_SPACE, R"doc(
             Characters with the space property.

             This is equivalent to the rule value "space".
             )doc")
      .value("UCOL_REORDER_CODE_FIRST", UCOL_REORDER_CODE_FIRST, R"doc(
             The first entry in the enumeration of reordering groups.

             This is intended for use in range checking and enumeration of the
             reorder codes.
             )doc")
      .value("UCOL_REORDER_CODE_PUNCTUATION", UCOL_REORDER_CODE_PUNCTUATION,
             R"doc(
             Characters with the punctuation property.

             This is equivalent to the rule value "punct".
             )doc")
      .value("UCOL_REORDER_CODE_SYMBOL", UCOL_REORDER_CODE_SYMBOL, R"doc(
             Characters with the symbol property.

             This is equivalent to the rule value "symbol".
             )doc")
      .value("UCOL_REORDER_CODE_CURRENCY", UCOL_REORDER_CODE_CURRENCY, R"doc(
             Characters with the currency property.

             This is equivalent to the rule value "currency".
             )doc")
      .value("UCOL_REORDER_CODE_DIGIT", UCOL_REORDER_CODE_DIGIT, R"doc(
             Characters with the digit property.

             This is equivalent to the rule value "digit".
             )doc")
      .value("UCOL_REORDER_CODE_LIMIT", UCOL_REORDER_CODE_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UColRuleOption
  //
  py::enum_<UColRuleOption>(m, "UColRuleOption", py::arithmetic(), R"doc(
Options for retrieving the rule string.
      )doc")
      .value("UCOL_TAILORING_ONLY", UCOL_TAILORING_ONLY, R"doc(
             Retrieves the tailoring rules only.

             Same as calling the version of ``.get_rules()`` without
             UColRuleOption.
             )doc")
      .value("UCOL_FULL_RULES", UCOL_FULL_RULES, R"doc(
             Retrieves the "UCA rules" concatenated with the tailoring rules.

             The "UCA rules" are an *approximation* of the root collator's sort
             order. They are almost never used or useful at runtime and can be
             removed from the data. See
             https://unicode-org.github.io/icu/userguide/collation/customization#building-on-existing-locales
             )doc")
      .export_values();
}

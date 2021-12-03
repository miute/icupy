#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
#include <unicode/unumberformatter.h>
#else
#include <unicode/numberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

void init_unumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  py::enum_<UNumberDecimalSeparatorDisplay>(m, "UNumberDecimalSeparatorDisplay", py::arithmetic(),
                                            "An enum declaring how to render the decimal separator.\n\n"
                                            "* UNUM_DECIMAL_SEPARATOR_AUTO: \"1\", \"1.1\"\n"
                                            "* UNUM_DECIMAL_SEPARATOR_ALWAYS: \"1.\", \"1.1\"")
      .value("UNUM_DECIMAL_SEPARATOR_AUTO", UNUM_DECIMAL_SEPARATOR_AUTO,
             "Show the decimal separator when there are one or more digits to display after the separator, and do not "
             "show it otherwise.\n\n  "
             "This is the default behavior.")
      .value("UNUM_DECIMAL_SEPARATOR_ALWAYS", UNUM_DECIMAL_SEPARATOR_ALWAYS,
             "Always show the decimal separator, even if there are no digits to display after the separator.")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UNumberGroupingStrategy>(
      m, "UNumberGroupingStrategy", py::arithmetic(),
      "An enum declaring the strategy for when and how to display grouping separators (i.e., the separator, often a "
      "comma or period, after every 2-3 powers of ten).\n\n"
      "The choices are several pre-built strategies for different use cases that employ locale data whenever possible. "
      "Example outputs for 1234 and 1234567 in *en-IN*:\n\n"
      "* OFF: 1234 and 12345\n"
      "* MIN2: 1234 and 12,34,567\n"
      "* AUTO: 1,234 and 12,34,567\n"
      "* ON_ALIGNED: 1,234 and 12,34,567\n"
      "* THOUSANDS: 1,234 and 1,234,567\n\n"
      "The default is AUTO, which displays grouping separators unless the locale data says that grouping is not "
      "customary. To force grouping for all numbers greater than 1000 consistently across locales, use ON_ALIGNED. "
      "On the other hand, to display grouping less frequently than the default, use MIN2 or OFF. See the docs of "
      "each option for details.\n\n"
      "Note: This enum specifies the strategy for grouping sizes. To set which character to use as the grouping "
      "separator, use the \"symbols\" setter.")
      .value("UNUM_GROUPING_OFF", UNUM_GROUPING_OFF, "Do not display grouping separators in any locale.")
      .value("UNUM_GROUPING_MIN2", UNUM_GROUPING_MIN2,
             "Display grouping using locale defaults, except do not show grouping on values smaller than 10000 (such "
             "that there is a *minimum of two digits* before the first separator).\n\n  "
             "Note that locales may restrict grouping separators to be displayed only on 1 million or greater (for "
             "example, ee and hu) or disable grouping altogether (for example, bg currency).\n\n  "
             "Locale data is used to determine whether to separate larger numbers into groups of 2 (customary in South "
             "Asia) or groups of 3 (customary in Europe and the Americas).")
      .value("UNUM_GROUPING_AUTO", UNUM_GROUPING_AUTO,
             "Display grouping using the default strategy for all locales.\n\n  "
             "This is the default behavior.\n\n  "
             "Note that locales may restrict grouping separators to be displayed only on 1 million or greater (for "
             "example, ee and hu) or disable grouping altogether (for example, bg currency).\n\n  "
             "Locale data is used to determine whether to separate larger numbers into groups of 2 (customary in South "
             "Asia) or groups of 3 (customary in Europe and the Americas).")
      .value("UNUM_GROUPING_ON_ALIGNED", UNUM_GROUPING_ON_ALIGNED,
             "Always display the grouping separator on values of at least 1000.\n\n  "
             "This option ignores the locale data that restricts or disables grouping, described in MIN2 and AUTO. "
             "This option may be useful to normalize the alignment of numbers, such as in a spreadsheet.\n\n  "
             "Locale data is used to determine whether to separate larger numbers into groups of 2 (customary in South "
             "Asia) or groups of 3 (customary in Europe and the Americas).")
      .value("UNUM_GROUPING_THOUSANDS", UNUM_GROUPING_THOUSANDS,
             "Use the Western defaults: groups of 3 and enabled for all numbers 1000 or greater.\n\n  "
             "Do not use locale data for determining the grouping strategy.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  py::enum_<UNumberRoundingPriority>(
      m, "UNumberRoundingPriority", py::arithmetic(),
      "An enum declaring how to resolve conflicts between maximum fraction digits and maximum significant digits.\n\n"
      "There are two modes, RELAXED and STRICT:\n\n"
      "* RELAXED: Relax one of the two constraints (fraction digits or significant digits) in order to round the "
      "number to a higher level of precision.\n"
      "* STRICT: Enforce both constraints, resulting in the number being rounded to a lower level of precision.\n\n"
      "The default settings for compact notation rounding are Max-Fraction = 0 (round to the nearest integer), "
      "Max-Significant = 2 (round to 2 significant digits), and priority RELAXED (choose the constraint that results "
      "in more digits being displayed).\n\n"
      "Conflicting minimum fraction and significant digits are always resolved in the direction that results in more "
      "trailing zeros.\n\n"
      "Example 1: Consider the number 3.141, with various different settings:\n\n"
      "* Max-Fraction = 1: \"3.1\"\n"
      "* Max-Significant = 3: \"3.14\"\n\n"
      "The rounding priority determines how to resolve the conflict when both Max-Fraction and Max-Significant are "
      "set. With RELAXED, the less-strict setting (the one that causes more digits to be displayed) will be used; "
      "Max-Significant wins. With STRICT, the more-strict setting (the one that causes fewer digits to be displayed) "
      "will be used; Max-Fraction wins.\n\n"
      "Example 2: Consider the number 8317, with various different settings:\n\n"
      "* Max-Fraction = 1: \"8317\"\n"
      "* Max-Significant = 3: \"8320\"\n\n"
      "Here, RELAXED favors Max-Fraction and STRICT favors Max-Significant. Note that this larger number caused the "
      "two modes to favor the opposite result.")
      .value("UNUM_ROUNDING_PRIORITY_RELAXED", UNUM_ROUNDING_PRIORITY_RELAXED,
             "Favor greater precision by relaxing one of the rounding constraints.")
      .value("UNUM_ROUNDING_PRIORITY_STRICT", UNUM_ROUNDING_PRIORITY_STRICT,
             "Favor adherence to all rounding constraints by producing lower precision.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  py::enum_<UNumberSignDisplay>(
      m, "UNumberSignDisplay", py::arithmetic(),
      "An enum declaring how to denote positive and negative numbers.\n\n"
      "Example outputs when formatting 123, 0, and -123 in *en-US*:\n\n"
      "* AUTO: \"123\", \"0\", and \"-123\"\n"
      "* ALWAYS: \"+123\", \"+0\", and \"-123\"\n"
      "* NEVER: \"123\", \"0\", and \"123\"\n"
      "* ACCOUNTING: \"$123\", \"$0\", and \"($123)\"\n"
      "* ACCOUNTING_ALWAYS: \"+$123\", \"+$0\", and \"($123)\"\n"
      "* EXCEPT_ZERO: \"+123\", \"0\", and \"-123\"\n"
      "* ACCOUNTING_EXCEPT_ZERO: \"+$123\", \"$0\", and \"($123)\"\n\n"
      "The exact format, including the position and the code point of the sign, differ by locale.")
      .value("UNUM_SIGN_AUTO", UNUM_SIGN_AUTO,
             "Show the minus sign on negative numbers, and do not show the sign on positive numbers.\n\n  "
             "This is the default behavior.\n\n  "
             "If using this option, a sign will be displayed on negative zero, including negative numbers that round "
             "to zero. To hide the sign on negative zero, use the NEGATIVE option.")
      .value("UNUM_SIGN_ALWAYS", UNUM_SIGN_ALWAYS,
             "Show the minus sign on negative numbers and the plus sign on positive numbers, including zero.\n\n  "
             "To hide the sign on zero, see *UNUM_SIGN_EXCEPT_ZERO*.")
      .value("UNUM_SIGN_NEVER", UNUM_SIGN_NEVER, "Do not show the sign on positive or negative numbers.")
      .value("UNUM_SIGN_ACCOUNTING", UNUM_SIGN_ACCOUNTING,
             "Use the locale-dependent accounting format on negative numbers, and do not show the sign on positive "
             "numbers.\n\n  "
             "The accounting format is defined in CLDR and varies by locale; in many Western locales, the format is a "
             "pair of parentheses around the number.\n\n  "
             "Note: Since CLDR defines the accounting format in the monetary context only, this option falls back to "
             "the AUTO sign display strategy when formatting without a currency unit. This limitation may be lifted "
             "in the future.")
      .value("UNUM_SIGN_ACCOUNTING_ALWAYS", UNUM_SIGN_ACCOUNTING_ALWAYS,
             "Use the locale-dependent accounting format on negative numbers, and show the plus sign on positive "
             "numbers, including zero.\n\n  "
             "For more information on the accounting format, see the ACCOUNTING sign display strategy. To hide the "
             "sign on zero, see *UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO*.")
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("UNUM_SIGN_EXCEPT_ZERO", UNUM_SIGN_EXCEPT_ZERO,
             "Show the minus sign on negative numbers and the plus sign on positive numbers.\n\n  "
             "Do not show a sign on zero, numbers that round to zero, or NaN.")
      .value("UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO", UNUM_SIGN_ACCOUNTING_EXCEPT_ZERO,
             "Use the locale-dependent accounting format on negative numbers, and show the plus sign on positive "
             "numbers.\n\n  "
             "Do not show a sign on zero, numbers that round to zero, or NaN. For more information on the accounting "
             "format, see the ACCOUNTING sign display strategy.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
      .value("UNUM_SIGN_NEGATIVE", UNUM_SIGN_NEGATIVE, "Same as AUTO, but do not show the sign on negative zero.")
      .value("UNUM_SIGN_ACCOUNTING_NEGATIVE", UNUM_SIGN_ACCOUNTING_NEGATIVE,
             "Same as ACCOUNTING, but do not show the sign on negative zero.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  py::enum_<UNumberTrailingZeroDisplay>(m, "UNumberTrailingZeroDisplay", py::arithmetic(),
                                        "An enum declaring how to render trailing zeros.\n\n"
                                        "* UNUM_TRAILING_ZERO_AUTO: 0.90, 1.00, 1.10\n"
                                        "* UNUM_TRAILING_ZERO_HIDE_IF_WHOLE: 0.90, 1, 1.10")
      .value("UNUM_TRAILING_ZERO_AUTO", UNUM_TRAILING_ZERO_AUTO,
             "Display trailing zeros according to the settings for minimum fraction and significant digits.")
      .value("UNUM_TRAILING_ZERO_HIDE_IF_WHOLE", UNUM_TRAILING_ZERO_HIDE_IF_WHOLE,
             "Same as AUTO, but hide trailing zeros after the decimal separator if they are all zero.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  py::enum_<UNumberUnitWidth>(m, "UNumberUnitWidth", py::arithmetic(),
                              "An enum declaring how to render units, including currencies.\n\n"
                              "Example outputs when formatting 123 USD and 123 meters in *en-CA*:\n\n"
                              "* NARROW: \"$123.00\" and \"123 m\"\n"
                              "* SHORT: \"US$ 123.00\" and \"123 m\"\n"
                              "* FULL_NAME: \"123.00 US dollars\" and \"123 meters\"\n"
                              "* ISO_CODE: \"USD 123.00\" and undefined behavior\n"
                              "* HIDDEN: \"123.00\" and \"123\"\n\n"
                              "This enum is similar to *UMeasureFormatWidth*.")
      .value("UNUM_UNIT_WIDTH_NARROW", UNUM_UNIT_WIDTH_NARROW,
             "Print an abbreviated version of the unit name.\n\n  "
             "Similar to SHORT, but always use the shortest available abbreviation or symbol. This option can be "
             "used when the context hints at the identity of the unit. For more information on the difference between "
             "NARROW and SHORT, see SHORT.\n\n  "
             "In CLDR, this option corresponds to the \"Narrow\" format for measure units and the \"¤¤¤¤¤\" "
             "placeholder for currencies.")
      .value("UNUM_UNIT_WIDTH_SHORT", UNUM_UNIT_WIDTH_SHORT,
             "Print an abbreviated version of the unit name.\n\n  "
             "Similar to NARROW, but use a slightly wider abbreviation or symbol when there may be ambiguity. This "
             "is the default behavior.\n\n  "
             "For example, in *es-US*, the SHORT form for Fahrenheit is \"{0} °F\", but the NARROW form is "
             "\"{0}°\", since Fahrenheit is the customary unit for temperature in that locale.\n\n  "
             "In CLDR, this option corresponds to the \"Short\" format for measure units and the \"¤\" placeholder for "
             "currencies.")
      .value("UNUM_UNIT_WIDTH_FULL_NAME", UNUM_UNIT_WIDTH_FULL_NAME,
             "Print the full name of the unit, without any abbreviations.\n\n  "
             "In CLDR, this option corresponds to the default format for measure units and the \"¤¤¤\" placeholder for "
             "currencies.")
      .value("UNUM_UNIT_WIDTH_ISO_CODE", UNUM_UNIT_WIDTH_ISO_CODE,
             "Use the three-digit ISO XXX code in place of the symbol for displaying currencies.\n\n  "
             "The behavior of this option is currently undefined for use with measure units.\n\n  "
             "In CLDR, this option corresponds to the \"¤¤\" placeholder for currencies.")
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UNUM_UNIT_WIDTH_FORMAL", UNUM_UNIT_WIDTH_FORMAL,
             "Use the formal variant of the currency symbol; for example, \"NT$\" for the New Taiwan dollar in "
             "zh-TW.\n\n  "
             "Behavior of this option with non-currency units is not defined at this time.")
      .value("UNUM_UNIT_WIDTH_VARIANT", UNUM_UNIT_WIDTH_VARIANT,
             "Use the alternate variant of the currency symbol; for example, \"TL\" for the Turkish lira (TRY).\n\n  "
             "Behavior of this option with non-currency units is not defined at this time.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UNUM_UNIT_WIDTH_HIDDEN", UNUM_UNIT_WIDTH_HIDDEN,
             "Format the number according to the specified unit, but do not display the unit.\n\n  "
             "For currencies, apply monetary symbols and formats as with SHORT, but omit the currency symbol. For "
             "measure units, the behavior is equivalent to not specifying the unit at all.")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
}

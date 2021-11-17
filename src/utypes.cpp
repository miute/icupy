#include "main.hpp"

void init_utypes(py::module &m) {
  py::enum_<UErrorCode>(m, "UErrorCode", py::arithmetic(),
                        "Standard ICU4C error code type, a substitute for exceptions.")
      .value("U_USING_FALLBACK_WARNING", U_USING_FALLBACK_WARNING,
             "A resource bundle lookup returned a fallback result (not an error).")
      .value("U_ERROR_WARNING_START", U_ERROR_WARNING_START, "Start of information results (semantically successful).")
      .value("U_USING_DEFAULT_WARNING", U_USING_DEFAULT_WARNING,
             "A resource bundle lookup returned a result from the root locale (not an error).")
      .value("U_SAFECLONE_ALLOCATED_WARNING", U_SAFECLONE_ALLOCATED_WARNING,
             "A SafeClone operation required allocating memory (informational only).")
      .value("U_STATE_OLD_WARNING", U_STATE_OLD_WARNING,
             "ICU has to use compatibility layer to construct the service. Expect performance/memory usage "
             "degradation. Consider upgrading.")
      .value("U_STRING_NOT_TERMINATED_WARNING", U_STRING_NOT_TERMINATED_WARNING,
             "An output string could not be NUL-terminated because output length==destCapacity.")
      .value("U_SORT_KEY_TOO_SHORT_WARNING", U_SORT_KEY_TOO_SHORT_WARNING,
             "Number of levels requested in *get_bound* is higher than the number of levels in the sort key.")
      .value("U_AMBIGUOUS_ALIAS_WARNING", U_AMBIGUOUS_ALIAS_WARNING,
             "This converter alias can go to different converter implementations.")
      .value("U_DIFFERENT_UCA_VERSION", U_DIFFERENT_UCA_VERSION,
             "*ucol_open* encountered a mismatch between UCA version and collator image version, so the collator was "
             "constructed from rules. No impact to further function.")
      .value("U_PLUGIN_CHANGED_LEVEL_WARNING", U_PLUGIN_CHANGED_LEVEL_WARNING,
             "A plugin caused a level change. May not be an error, but later plugins may not load.")
#ifndef U_HIDE_DEPRECATED_API
      .value("U_ERROR_WARNING_LIMIT", U_ERROR_WARNING_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_ZERO_ERROR", U_ZERO_ERROR, "No error, no warning.")
      .value("U_ILLEGAL_ARGUMENT_ERROR", U_ILLEGAL_ARGUMENT_ERROR, "Start of codes indicating failure.")
      .value("U_MISSING_RESOURCE_ERROR", U_MISSING_RESOURCE_ERROR, "The requested resource cannot be found.")
      .value("U_INVALID_FORMAT_ERROR", U_INVALID_FORMAT_ERROR, "Data format is not what is expected.")
      .value("U_FILE_ACCESS_ERROR", U_FILE_ACCESS_ERROR, "The requested file cannot be found.")
      .value("U_INTERNAL_PROGRAM_ERROR", U_INTERNAL_PROGRAM_ERROR, "Indicates a bug in the library code.")
      .value("U_MESSAGE_PARSE_ERROR", U_MESSAGE_PARSE_ERROR, "Unable to parse a message (message format).")
      .value("U_MEMORY_ALLOCATION_ERROR", U_MEMORY_ALLOCATION_ERROR, "Memory allocation error.")
      .value("U_INDEX_OUTOFBOUNDS_ERROR", U_INDEX_OUTOFBOUNDS_ERROR,
             "Trying to access the index that is out of bounds.")
      .value("U_PARSE_ERROR", U_PARSE_ERROR, "Equivalent to Java ParseException.")
      .value("U_INVALID_CHAR_FOUND", U_INVALID_CHAR_FOUND,
             "Character conversion: Unmappable input sequence. In other APIs: Invalid character.")
      .value("U_TRUNCATED_CHAR_FOUND", U_TRUNCATED_CHAR_FOUND, "Character conversion: Incomplete input sequence.")
      .value("U_ILLEGAL_CHAR_FOUND", U_ILLEGAL_CHAR_FOUND,
             "Character conversion: Illegal input sequence/combination of input units.")
      .value("U_INVALID_TABLE_FORMAT", U_INVALID_TABLE_FORMAT, "Conversion table file found, but corrupted.")
      .value("U_INVALID_TABLE_FILE", U_INVALID_TABLE_FILE, "Conversion table file not found.")
      .value("U_BUFFER_OVERFLOW_ERROR", U_BUFFER_OVERFLOW_ERROR, "A result would not fit in the supplied buffer.")
      .value("U_UNSUPPORTED_ERROR", U_UNSUPPORTED_ERROR, "Requested operation not supported in current context.")
      .value("U_RESOURCE_TYPE_MISMATCH", U_RESOURCE_TYPE_MISMATCH,
             "an operation is requested over a resource that does not support it.")
      .value("U_ILLEGAL_ESCAPE_SEQUENCE", U_ILLEGAL_ESCAPE_SEQUENCE, "ISO-2022 illegal escape sequence.")
      .value("U_UNSUPPORTED_ESCAPE_SEQUENCE", U_UNSUPPORTED_ESCAPE_SEQUENCE, "ISO-2022 unsupported escape sequence.")
      .value("U_NO_SPACE_AVAILABLE", U_NO_SPACE_AVAILABLE,
             "No space available for in-buffer expansion for Arabic shaping.")
      .value("U_CE_NOT_FOUND_ERROR", U_CE_NOT_FOUND_ERROR,
             "Currently used only while setting variable top, but can be used generally.")
      .value("U_PRIMARY_TOO_LONG_ERROR", U_PRIMARY_TOO_LONG_ERROR,
             "User tried to set variable top to a primary that is longer than two bytes.")
      .value("U_STATE_TOO_OLD_ERROR", U_STATE_TOO_OLD_ERROR,
             "ICU cannot construct a service from this state, as it is no longer supported.")
      .value("U_TOO_MANY_ALIASES_ERROR", U_TOO_MANY_ALIASES_ERROR,
             "There are too many aliases in the path to the requested resource. It is very possible that a circular "
             "alias definition has occurred.")
      .value("U_ENUM_OUT_OF_SYNC_ERROR", U_ENUM_OUT_OF_SYNC_ERROR,
             "UEnumeration out of sync with underlying collection.")
      .value("U_INVARIANT_CONVERSION_ERROR", U_INVARIANT_CONVERSION_ERROR,
             "Unable to convert a UChar* string to char* with the invariant converter.")
      .value("U_INVALID_STATE_ERROR", U_INVALID_STATE_ERROR,
             "Requested operation can not be completed with ICU in its current state.")
      .value("U_COLLATOR_VERSION_MISMATCH", U_COLLATOR_VERSION_MISMATCH,
             "Collator version is not compatible with the base version.")
      .value("U_USELESS_COLLATOR_ERROR", U_USELESS_COLLATOR_ERROR, "Collator is options only and no base is specified.")
      .value("U_NO_WRITE_PERMISSION", U_NO_WRITE_PERMISSION, "Attempt to modify read-only or constant data.")
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("U_INPUT_TOO_LONG_ERROR", U_INPUT_TOO_LONG_ERROR,
             "The input is impractically long for an operation. It is rejected because it may lead to problems such as "
             "excessive processing time, stack depth, or heap memory requirements.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
#ifndef U_HIDE_DEPRECATED_API
      .value("U_STANDARD_ERROR_LIMIT", U_STANDARD_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_BAD_VARIABLE_DEFINITION", U_BAD_VARIABLE_DEFINITION, "Missing '$' or duplicate variable name.")
      .value("U_PARSE_ERROR_START", U_PARSE_ERROR_START, "Start of Transliterator errors.")
      .value("U_MALFORMED_RULE", U_MALFORMED_RULE, "Elements of a rule are misplaced.")
      .value("U_MALFORMED_SET", U_MALFORMED_SET, "A UnicodeSet pattern is invalid.")
      .value("U_MALFORMED_SYMBOL_REFERENCE", U_MALFORMED_SYMBOL_REFERENCE, "UNUSED as of ICU 2.4.")
      .value("U_MALFORMED_UNICODE_ESCAPE", U_MALFORMED_UNICODE_ESCAPE, "A Unicode escape pattern is invalid.")
      .value("U_MALFORMED_VARIABLE_DEFINITION", U_MALFORMED_VARIABLE_DEFINITION, "A variable definition is invalid.")
      .value("U_MALFORMED_VARIABLE_REFERENCE", U_MALFORMED_VARIABLE_REFERENCE, "A variable reference is invalid.")
      .value("U_MISMATCHED_SEGMENT_DELIMITERS", U_MISMATCHED_SEGMENT_DELIMITERS, "UNUSED as of ICU 2.4.")
      .value("U_MISPLACED_ANCHOR_START", U_MISPLACED_ANCHOR_START, "A start anchor appears at an illegal position.")
      .value("U_MISPLACED_CURSOR_OFFSET", U_MISPLACED_CURSOR_OFFSET, "A cursor offset occurs at an illegal position.")
      .value("U_MISPLACED_QUANTIFIER", U_MISPLACED_QUANTIFIER, "A quantifier appears after a segment close delimiter.")
      .value("U_MISSING_OPERATOR", U_MISSING_OPERATOR, "A rule contains no operator.")
      .value("U_MISSING_SEGMENT_CLOSE", U_MISSING_SEGMENT_CLOSE, "UNUSED as of ICU 2.4.")
      .value("U_MULTIPLE_ANTE_CONTEXTS", U_MULTIPLE_ANTE_CONTEXTS, "More than one ante context.")
      .value("U_MULTIPLE_CURSORS", U_MULTIPLE_CURSORS, "More than one cursor.")
      .value("U_MULTIPLE_POST_CONTEXTS", U_MULTIPLE_POST_CONTEXTS, "More than one post context.")
      .value("U_TRAILING_BACKSLASH", U_TRAILING_BACKSLASH, "A dangling backslash.")
      .value("U_UNDEFINED_SEGMENT_REFERENCE", U_UNDEFINED_SEGMENT_REFERENCE,
             "A segment reference does not correspond to a defined segment.")
      .value("U_UNDEFINED_VARIABLE", U_UNDEFINED_VARIABLE,
             "A variable reference does not correspond to a defined variable.")
      .value("U_UNQUOTED_SPECIAL", U_UNQUOTED_SPECIAL, "A special character was not quoted or escaped.")
      .value("U_UNTERMINATED_QUOTE", U_UNTERMINATED_QUOTE, "A closing single quote is missing.")
      .value("U_RULE_MASK_ERROR", U_RULE_MASK_ERROR, "A rule is hidden by an earlier more general rule.")
      .value("U_MISPLACED_COMPOUND_FILTER", U_MISPLACED_COMPOUND_FILTER, "A compound filter is in an invalid location.")
      .value("U_MULTIPLE_COMPOUND_FILTERS", U_MULTIPLE_COMPOUND_FILTERS, "More than one compound filter.")
      .value("U_INVALID_RBT_SYNTAX", U_INVALID_RBT_SYNTAX,
             "A \"::id\" rule was passed to the RuleBasedTransliterator parser.")
      .value("U_INVALID_PROPERTY_PATTERN", U_INVALID_PROPERTY_PATTERN, "UNUSED as of ICU 2.4.")
      .value("U_MALFORMED_PRAGMA", U_MALFORMED_PRAGMA, "A 'use' pragma is invalid.")
      .value("U_UNCLOSED_SEGMENT", U_UNCLOSED_SEGMENT, "A closing ')' is missing.")
      .value("U_ILLEGAL_CHAR_IN_SEGMENT", U_ILLEGAL_CHAR_IN_SEGMENT, "UNUSED as of ICU 2.4.")
      .value("U_VARIABLE_RANGE_EXHAUSTED", U_VARIABLE_RANGE_EXHAUSTED,
             "Too many stand-ins generated for the given variable range.")
      .value("U_VARIABLE_RANGE_OVERLAP", U_VARIABLE_RANGE_OVERLAP,
             "The variable range overlaps characters used in rules.")
      .value("U_ILLEGAL_CHARACTER", U_ILLEGAL_CHARACTER, "A special character is outside its allowed context.")
      .value("U_INTERNAL_TRANSLITERATOR_ERROR", U_INTERNAL_TRANSLITERATOR_ERROR,
             "Internal transliterator system error.")
      .value("U_INVALID_ID", U_INVALID_ID, "A \"::id\" rule specifies an unknown transliterator.")
      .value("U_INVALID_FUNCTION", U_INVALID_FUNCTION, "A \"&fn()\" rule specifies an unknown transliterator.")
#ifndef U_HIDE_DEPRECATED_API
      .value("U_PARSE_ERROR_LIMIT", U_PARSE_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_UNEXPECTED_TOKEN", U_UNEXPECTED_TOKEN, "Syntax error in format pattern.")
      .value("U_FMT_PARSE_ERROR_START", U_FMT_PARSE_ERROR_START, "Start of format library errors.")
      .value("U_MULTIPLE_DECIMAL_SEPARATORS", U_MULTIPLE_DECIMAL_SEPARATORS,
             "More than one decimal separator in number pattern.")
      .value("U_MULTIPLE_DECIMAL_SEPERATORS", U_MULTIPLE_DECIMAL_SEPERATORS,
             "Typo: kept for backward compatibility. Use *U_MULTIPLE_DECIMAL_SEPARATORS*.")
      .value("U_MULTIPLE_EXPONENTIAL_SYMBOLS", U_MULTIPLE_EXPONENTIAL_SYMBOLS,
             "More than one exponent symbol in number pattern.")
      .value("U_MALFORMED_EXPONENTIAL_PATTERN", U_MALFORMED_EXPONENTIAL_PATTERN, "Grouping symbol in exponent pattern.")
      .value("U_MULTIPLE_PERCENT_SYMBOLS", U_MULTIPLE_PERCENT_SYMBOLS,
             "More than one percent symbol in number pattern.")
      .value("U_MULTIPLE_PERMILL_SYMBOLS", U_MULTIPLE_PERMILL_SYMBOLS,
             "More than one permill symbol in number pattern.")
      .value("U_MULTIPLE_PAD_SPECIFIERS", U_MULTIPLE_PAD_SPECIFIERS, "More than one pad symbol in number pattern.")
      .value("U_PATTERN_SYNTAX_ERROR", U_PATTERN_SYNTAX_ERROR, "Syntax error in format pattern.")
      .value("U_ILLEGAL_PAD_POSITION", U_ILLEGAL_PAD_POSITION, "Pad symbol misplaced in number pattern.")
      .value("U_UNMATCHED_BRACES", U_UNMATCHED_BRACES, "Braces do not match in message pattern.")
      .value("U_UNSUPPORTED_PROPERTY", U_UNSUPPORTED_PROPERTY, "UNUSED as of ICU 2.4.")
      .value("U_UNSUPPORTED_ATTRIBUTE", U_UNSUPPORTED_ATTRIBUTE, "UNUSED as of ICU 2.4.")
      .value("U_ARGUMENT_TYPE_MISMATCH", U_ARGUMENT_TYPE_MISMATCH,
             "Argument name and argument index mismatch in MessageFormat functions.")
      .value("U_DUPLICATE_KEYWORD", U_DUPLICATE_KEYWORD, "Duplicate keyword in *PluralFormat*.")
      .value("U_UNDEFINED_KEYWORD", U_UNDEFINED_KEYWORD, "Undefined Plural keyword.")
      .value("U_DEFAULT_KEYWORD_MISSING", U_DEFAULT_KEYWORD_MISSING, "Missing DEFAULT rule in plural rules.")
      .value("U_DECIMAL_NUMBER_SYNTAX_ERROR", U_DECIMAL_NUMBER_SYNTAX_ERROR, "Decimal number syntax error.")
      .value("U_FORMAT_INEXACT_ERROR", U_FORMAT_INEXACT_ERROR,
             "Cannot format a number exactly and rounding mode is ROUND_UNNECESSARY @stable ICU 4.8.")
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("U_NUMBER_ARG_OUTOFBOUNDS_ERROR", U_NUMBER_ARG_OUTOFBOUNDS_ERROR,
             "The argument to a NumberFormatter helper method was out of bounds; the bounds are usually 0 to 999.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("U_NUMBER_SKELETON_SYNTAX_ERROR", U_NUMBER_SKELETON_SYNTAX_ERROR,
             "The number skeleton passed to C++ NumberFormatter or C UNumberFormatter was invalid or contained a "
             "syntax error.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
#ifndef U_HIDE_DEPRECATED_API
      .value("U_FMT_PARSE_ERROR_LIMIT", U_FMT_PARSE_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_BRK_INTERNAL_ERROR", U_BRK_INTERNAL_ERROR, "An internal error (bug) was detected.")
      .value("U_BRK_ERROR_START", U_BRK_ERROR_START, "Start of codes indicating Break Iterator failures.")
      .value("U_BRK_HEX_DIGITS_EXPECTED", U_BRK_HEX_DIGITS_EXPECTED,
             "Hex digits expected as part of a escaped char in a rule.")
      .value("U_BRK_SEMICOLON_EXPECTED", U_BRK_SEMICOLON_EXPECTED, "Missing ';' at the end of a RBBI rule.")
      .value("U_BRK_RULE_SYNTAX", U_BRK_RULE_SYNTAX, "Syntax error in RBBI rule.")
      .value("U_BRK_UNCLOSED_SET", U_BRK_UNCLOSED_SET, "UnicodeSet writing an RBBI rule missing a closing ']'.")
      .value("U_BRK_ASSIGN_ERROR", U_BRK_ASSIGN_ERROR, "Syntax error in RBBI rule assignment statement.")
      .value("U_BRK_VARIABLE_REDFINITION", U_BRK_VARIABLE_REDFINITION, "RBBI rule $Variable redefined.")
      .value("U_BRK_MISMATCHED_PAREN", U_BRK_MISMATCHED_PAREN, "Mis-matched parentheses in an RBBI rule.")
      .value("U_BRK_NEW_LINE_IN_QUOTED_STRING", U_BRK_NEW_LINE_IN_QUOTED_STRING,
             "Missing closing quote in an RBBI rule.")
      .value("U_BRK_UNDEFINED_VARIABLE", U_BRK_UNDEFINED_VARIABLE, "Use of an undefined $Variable in an RBBI rule.")
      .value("U_BRK_INIT_ERROR", U_BRK_INIT_ERROR, "Initialization failure.  Probable missing ICU Data.")
      .value("U_BRK_RULE_EMPTY_SET", U_BRK_RULE_EMPTY_SET, "Rule contains an empty Unicode Set.")
      .value("U_BRK_UNRECOGNIZED_OPTION", U_BRK_UNRECOGNIZED_OPTION, "!!option in RBBI rules not recognized.")
      .value("U_BRK_MALFORMED_RULE_TAG", U_BRK_MALFORMED_RULE_TAG, "The {nnn} tag on a rule is malformed.")
#ifndef U_HIDE_DEPRECATED_API
      .value("U_BRK_ERROR_LIMIT", U_BRK_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_REGEX_INTERNAL_ERROR", U_REGEX_INTERNAL_ERROR, "An internal error (bug) was detected.")
      .value("U_REGEX_ERROR_START", U_REGEX_ERROR_START, "Start of codes indicating Regexp failures.")
      .value("U_REGEX_RULE_SYNTAX", U_REGEX_RULE_SYNTAX, "Syntax error in regexp pattern.")
      .value("U_REGEX_INVALID_STATE", U_REGEX_INVALID_STATE, "RegexMatcher in invalid state for requested operation.")
      .value("U_REGEX_BAD_ESCAPE_SEQUENCE", U_REGEX_BAD_ESCAPE_SEQUENCE,
             "Unrecognized backslash escape sequence in pattern.")
      .value("U_REGEX_PROPERTY_SYNTAX", U_REGEX_PROPERTY_SYNTAX, "Incorrect Unicode property.")
      .value("U_REGEX_UNIMPLEMENTED", U_REGEX_UNIMPLEMENTED, "Use of regexp feature that is not yet implemented.")
      .value("U_REGEX_MISMATCHED_PAREN", U_REGEX_MISMATCHED_PAREN, "Incorrectly nested parentheses in regexp pattern.")
      .value("U_REGEX_NUMBER_TOO_BIG", U_REGEX_NUMBER_TOO_BIG, "Decimal number is too large.")
      .value("U_REGEX_BAD_INTERVAL", U_REGEX_BAD_INTERVAL, "Error in {min,max} interval.")
      .value("U_REGEX_MAX_LT_MIN", U_REGEX_MAX_LT_MIN, "In {min,max}, max is less than min.")
      .value("U_REGEX_INVALID_BACK_REF", U_REGEX_INVALID_BACK_REF, "Back-reference to a non-existent capture group.")
      .value("U_REGEX_INVALID_FLAG", U_REGEX_INVALID_FLAG, "Invalid value for match mode flags.")
      .value("U_REGEX_LOOK_BEHIND_LIMIT", U_REGEX_LOOK_BEHIND_LIMIT,
             "Look-Behind pattern matches must have a bounded maximum length.")
      .value("U_REGEX_SET_CONTAINS_STRING", U_REGEX_SET_CONTAINS_STRING,
             "Regexps cannot have UnicodeSets containing strings.")
#ifndef U_HIDE_DEPRECATED_API
      .value("U_REGEX_OCTAL_TOO_BIG", U_REGEX_OCTAL_TOO_BIG, "**Deprecated:** ICU 54. This error cannot occur.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_REGEX_MISSING_CLOSE_BRACKET", U_REGEX_MISSING_CLOSE_BRACKET,
             "Missing closing bracket on a bracket expression.")
      .value("U_REGEX_INVALID_RANGE", U_REGEX_INVALID_RANGE, "In a character range [x-y], x is greater than y.")
      .value("U_REGEX_STACK_OVERFLOW", U_REGEX_STACK_OVERFLOW, "Regular expression backtrack stack overflow.")
      .value("U_REGEX_TIME_OUT", U_REGEX_TIME_OUT, "Maximum allowed match time exceeded.")
      .value("U_REGEX_STOPPED_BY_CALLER", U_REGEX_STOPPED_BY_CALLER, "Matching operation aborted by user callback fn.")
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
      .value("U_REGEX_PATTERN_TOO_BIG", U_REGEX_PATTERN_TOO_BIG, "Pattern exceeds limits on size or complexity.")
      .value("U_REGEX_INVALID_CAPTURE_GROUP_NAME", U_REGEX_INVALID_CAPTURE_GROUP_NAME, "Invalid capture group name.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
#ifndef U_HIDE_DEPRECATED_API
      .value("U_REGEX_ERROR_LIMIT", U_REGEX_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_IDNA_PROHIBITED_ERROR", U_IDNA_PROHIBITED_ERROR)
      .value("U_IDNA_ERROR_START", U_IDNA_ERROR_START)
      .value("U_IDNA_UNASSIGNED_ERROR", U_IDNA_UNASSIGNED_ERROR)
      .value("U_IDNA_CHECK_BIDI_ERROR", U_IDNA_CHECK_BIDI_ERROR)
      .value("U_IDNA_STD3_ASCII_RULES_ERROR", U_IDNA_STD3_ASCII_RULES_ERROR)
      .value("U_IDNA_ACE_PREFIX_ERROR", U_IDNA_ACE_PREFIX_ERROR)
      .value("U_IDNA_VERIFICATION_ERROR", U_IDNA_VERIFICATION_ERROR)
      .value("U_IDNA_LABEL_TOO_LONG_ERROR", U_IDNA_LABEL_TOO_LONG_ERROR)
      .value("U_IDNA_ZERO_LENGTH_LABEL_ERROR", U_IDNA_ZERO_LENGTH_LABEL_ERROR)
      .value("U_IDNA_DOMAIN_NAME_TOO_LONG_ERROR", U_IDNA_DOMAIN_NAME_TOO_LONG_ERROR)
#ifndef U_HIDE_DEPRECATED_API
      .value("U_IDNA_ERROR_LIMIT", U_IDNA_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .value("U_STRINGPREP_PROHIBITED_ERROR", U_STRINGPREP_PROHIBITED_ERROR)
      .value("U_STRINGPREP_UNASSIGNED_ERROR", U_STRINGPREP_UNASSIGNED_ERROR)
      .value("U_STRINGPREP_CHECK_BIDI_ERROR", U_STRINGPREP_CHECK_BIDI_ERROR)
      .value("U_PLUGIN_ERROR_START", U_PLUGIN_ERROR_START, "Start of codes indicating plugin failures.")
      .value("U_PLUGIN_TOO_HIGH", U_PLUGIN_TOO_HIGH, "The plugin's level is too high to be loaded right now.")
      .value("U_PLUGIN_DIDNT_SET_LEVEL", U_PLUGIN_DIDNT_SET_LEVEL,
             "The plugin didn't call uplug_setPlugLevel in response to a QUERY.")
#ifndef U_HIDE_DEPRECATED_API
      .value("U_PLUGIN_ERROR_LIMIT", U_PLUGIN_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
#ifndef U_HIDE_DEPRECATED_API
      .value("U_ERROR_LIMIT", U_ERROR_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  m.def("u_error_name", &u_errorName, py::arg("code"));
  m.def("u_failure", &U_FAILURE, py::arg("code"));
  m.def("u_success", &U_SUCCESS, py::arg("code"));

  m.attr("U_MILLIS_PER_DAY") = U_MILLIS_PER_DAY;
  m.attr("U_MILLIS_PER_HOUR") = U_MILLIS_PER_HOUR;
  m.attr("U_MILLIS_PER_MINUTE") = U_MILLIS_PER_MINUTE;
  m.attr("U_MILLIS_PER_SECOND") = U_MILLIS_PER_SECOND;
}

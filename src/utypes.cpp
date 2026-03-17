#include "main.hpp"

void init_utypes(py::module &m) {
  //
  // enum UErrorCode
  //
  py::enum_<UErrorCode>(m, "UErrorCode", py::arithmetic(), R"doc(
Standard ICU4C error code type.

The error code is used by ICU4C functions to report errors.

See Also:
    :class:`ErrorCode`
    :class:`ICUError`
      )doc")
      .value("U_USING_FALLBACK_WARNING", U_USING_FALLBACK_WARNING, R"doc(
             A resource bundle lookup returned a fallback result (not an
             error).
             )doc")
      .value("U_ERROR_WARNING_START", U_ERROR_WARNING_START, R"doc(
             Start of information results (semantically successful).
             )doc")
      .value("U_USING_DEFAULT_WARNING", U_USING_DEFAULT_WARNING, R"doc(
             A resource bundle lookup returned a result from the root locale
             (not an error).
             )doc")
      .value("U_SAFECLONE_ALLOCATED_WARNING", U_SAFECLONE_ALLOCATED_WARNING,
             R"doc(
             A SafeClone operation required allocating memory (informational
             only).
             )doc")
      .value("U_STATE_OLD_WARNING", U_STATE_OLD_WARNING, R"doc(
             ICU has to use compatibility layer to construct the service.
             Expect performance/memory usage degradation. Consider upgrading.
             )doc")
      .value("U_STRING_NOT_TERMINATED_WARNING", U_STRING_NOT_TERMINATED_WARNING,
             R"doc(
             An output string could not be NUL-terminated because output
             length==dest_capacity.
             )doc")
      .value("U_SORT_KEY_TOO_SHORT_WARNING", U_SORT_KEY_TOO_SHORT_WARNING,
             R"doc(
             Number of levels requested in getBound is higher than the
             number of levels in the sort key.
             )doc")
      .value("U_AMBIGUOUS_ALIAS_WARNING", U_AMBIGUOUS_ALIAS_WARNING,
             R"doc(
             This converter alias can go to different converter
             implementations.
             )doc")
      .value("U_DIFFERENT_UCA_VERSION", U_DIFFERENT_UCA_VERSION, R"doc(
             ucol_open encountered a mismatch between UCA version and
             collator image version, so the collator was constructed from
             rules. No impact to further function.
             )doc")
      .value("U_PLUGIN_CHANGED_LEVEL_WARNING", U_PLUGIN_CHANGED_LEVEL_WARNING,
             R"doc(
             A plugin caused a level change. May not be an error, but later
             plugins may not load.
             )doc")
      .value("U_ERROR_WARNING_LIMIT", U_ERROR_WARNING_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_ZERO_ERROR", U_ZERO_ERROR, R"doc(
             No error, no warning.
             )doc")
      .value("U_ILLEGAL_ARGUMENT_ERROR", U_ILLEGAL_ARGUMENT_ERROR, R"doc(
             Start of codes indicating failure.
             )doc")
      .value("U_MISSING_RESOURCE_ERROR", U_MISSING_RESOURCE_ERROR, R"doc(
             The requested resource cannot be found.
             )doc")
      .value("U_INVALID_FORMAT_ERROR", U_INVALID_FORMAT_ERROR, R"doc(
             Data format is not what is expected.
             )doc")
      .value("U_FILE_ACCESS_ERROR", U_FILE_ACCESS_ERROR, R"doc(
             The requested file cannot be found.
             )doc")
      .value("U_INTERNAL_PROGRAM_ERROR", U_INTERNAL_PROGRAM_ERROR, R"doc(
             Indicates a bug in the library code.
             )doc")
      .value("U_MESSAGE_PARSE_ERROR", U_MESSAGE_PARSE_ERROR, R"doc(
             Unable to parse a message (message format).
             )doc")
      .value("U_MEMORY_ALLOCATION_ERROR", U_MEMORY_ALLOCATION_ERROR, R"doc(
             Memory allocation error.
             )doc")
      .value("U_INDEX_OUTOFBOUNDS_ERROR", U_INDEX_OUTOFBOUNDS_ERROR, R"doc(
             Trying to access the index that is out of bounds.
             )doc")
      .value("U_PARSE_ERROR", U_PARSE_ERROR, R"doc(
             Equivalent to Java ParseException.
             )doc")
      .value("U_INVALID_CHAR_FOUND", U_INVALID_CHAR_FOUND, R"doc(
             Character conversion: Unmappable input sequence. In other APIs:
             Invalid character.
             )doc")
      .value("U_TRUNCATED_CHAR_FOUND", U_TRUNCATED_CHAR_FOUND, R"doc(
             Character conversion: Incomplete input sequence.
             )doc")
      .value("U_ILLEGAL_CHAR_FOUND", U_ILLEGAL_CHAR_FOUND, R"doc(
             Character conversion: Illegal input sequence/combination of
             input units.
             )doc")
      .value("U_INVALID_TABLE_FORMAT", U_INVALID_TABLE_FORMAT, R"doc(
             Conversion table file found, but corrupted.
             )doc")
      .value("U_INVALID_TABLE_FILE", U_INVALID_TABLE_FILE, R"doc(
             Conversion table file not found.
             )doc")
      .value("U_BUFFER_OVERFLOW_ERROR", U_BUFFER_OVERFLOW_ERROR, R"doc(
             A result would not fit in the supplied buffer.
             )doc")
      .value("U_UNSUPPORTED_ERROR", U_UNSUPPORTED_ERROR, R"doc(
             Requested operation not supported in current context.
             )doc")
      .value("U_RESOURCE_TYPE_MISMATCH", U_RESOURCE_TYPE_MISMATCH, R"doc(
             an operation is requested over a resource that does not support
             it.
             )doc")
      .value("U_ILLEGAL_ESCAPE_SEQUENCE", U_ILLEGAL_ESCAPE_SEQUENCE, R"doc(
             ISO-2022 illegal escape sequence.
             )doc")
      .value("U_UNSUPPORTED_ESCAPE_SEQUENCE", U_UNSUPPORTED_ESCAPE_SEQUENCE,
             R"doc(
             ISO-2022 unsupported escape sequence.
             )doc")
      .value("U_NO_SPACE_AVAILABLE", U_NO_SPACE_AVAILABLE, R"doc(
             No space available for in-buffer expansion for Arabic shaping.
             )doc")
      .value("U_CE_NOT_FOUND_ERROR", U_CE_NOT_FOUND_ERROR, R"doc(
             Currently used only while setting variable top, but can be used
             generally.
             )doc")
      .value("U_PRIMARY_TOO_LONG_ERROR", U_PRIMARY_TOO_LONG_ERROR, R"doc(
             User tried to set variable top to a primary that is longer than
             two bytes.
             )doc")
      .value("U_STATE_TOO_OLD_ERROR", U_STATE_TOO_OLD_ERROR, R"doc(
             ICU cannot construct a service from this state, as it is no
             longer supported.
             )doc")
      .value("U_TOO_MANY_ALIASES_ERROR", U_TOO_MANY_ALIASES_ERROR, R"doc(
             There are too many aliases in the path to the requested
             resource. It is very possible that a circular alias definition has
             occurred.
             )doc")
      .value("U_ENUM_OUT_OF_SYNC_ERROR", U_ENUM_OUT_OF_SYNC_ERROR, R"doc(
             UEnumeration out of sync with underlying collection.
             )doc")
      .value("U_INVARIANT_CONVERSION_ERROR", U_INVARIANT_CONVERSION_ERROR,
             R"doc(
             Unable to convert a ``UChar*`` string to ``char*`` with the
             invariant converter.
             )doc")
      .value("U_INVALID_STATE_ERROR", U_INVALID_STATE_ERROR, R"doc(
             Requested operation can not be completed with ICU in its current
             state.
             )doc")
      .value("U_COLLATOR_VERSION_MISMATCH", U_COLLATOR_VERSION_MISMATCH, R"doc(
             Collator version is not compatible with the base version.
             )doc")
      .value("U_USELESS_COLLATOR_ERROR", U_USELESS_COLLATOR_ERROR, R"doc(
             Collator is options only and no base is specified.
             )doc")
      .value("U_NO_WRITE_PERMISSION", U_NO_WRITE_PERMISSION, R"doc(
             Attempt to modify read-only or constant data.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("U_INPUT_TOO_LONG_ERROR", U_INPUT_TOO_LONG_ERROR, R"doc(
             The input is impractically long for an operation. It is rejected
             because it may lead to problems such as excessive processing time,
             stack depth, or heap memory requirements.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("U_STANDARD_ERROR_LIMIT", U_STANDARD_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_BAD_VARIABLE_DEFINITION", U_BAD_VARIABLE_DEFINITION, R"doc(
             Missing '$' or duplicate variable name.
             )doc")
      .value("U_PARSE_ERROR_START", U_PARSE_ERROR_START, R"doc(
             Start of Transliterator errors.
             )doc")
      .value("U_MALFORMED_RULE", U_MALFORMED_RULE, R"doc(
             Elements of a rule are misplaced.
             )doc")
      .value("U_MALFORMED_SET", U_MALFORMED_SET, R"doc(
             A UnicodeSet pattern is invalid.
             )doc")
      .value("U_MALFORMED_SYMBOL_REFERENCE", U_MALFORMED_SYMBOL_REFERENCE,
             R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_MALFORMED_UNICODE_ESCAPE", U_MALFORMED_UNICODE_ESCAPE, R"doc(
             A Unicode escape pattern is invalid.
             )doc")
      .value("U_MALFORMED_VARIABLE_DEFINITION", U_MALFORMED_VARIABLE_DEFINITION,
             R"doc(
             A variable definition is invalid.
             )doc")
      .value("U_MALFORMED_VARIABLE_REFERENCE", U_MALFORMED_VARIABLE_REFERENCE,
             R"doc(
             A variable reference is invalid.
             )doc")
      .value("U_MISMATCHED_SEGMENT_DELIMITERS", U_MISMATCHED_SEGMENT_DELIMITERS,
             R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_MISPLACED_ANCHOR_START", U_MISPLACED_ANCHOR_START, R"doc(
             A start anchor appears at an illegal position.
             )doc")
      .value("U_MISPLACED_CURSOR_OFFSET", U_MISPLACED_CURSOR_OFFSET, R"doc(
             A cursor offset occurs at an illegal position.
             )doc")
      .value("U_MISPLACED_QUANTIFIER", U_MISPLACED_QUANTIFIER, R"doc(
             A quantifier appears after a segment close delimiter.
             )doc")
      .value("U_MISSING_OPERATOR", U_MISSING_OPERATOR, R"doc(
             A rule contains no operator.
             )doc")
      .value("U_MISSING_SEGMENT_CLOSE", U_MISSING_SEGMENT_CLOSE, R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_MULTIPLE_ANTE_CONTEXTS", U_MULTIPLE_ANTE_CONTEXTS, R"doc(
             More than one ante context.
             )doc")
      .value("U_MULTIPLE_CURSORS", U_MULTIPLE_CURSORS, R"doc(
             More than one cursor.
             )doc")
      .value("U_MULTIPLE_POST_CONTEXTS", U_MULTIPLE_POST_CONTEXTS, R"doc(
             More than one post context.
             )doc")
      .value("U_TRAILING_BACKSLASH", U_TRAILING_BACKSLASH, R"doc(
             A dangling backslash.
             )doc")
      .value("U_UNDEFINED_SEGMENT_REFERENCE", U_UNDEFINED_SEGMENT_REFERENCE,
             R"doc(
             A segment reference does not correspond to a defined segment.
             )doc")
      .value("U_UNDEFINED_VARIABLE", U_UNDEFINED_VARIABLE, R"doc(
             A variable reference does not correspond to a defined variable.
             )doc")
      .value("U_UNQUOTED_SPECIAL", U_UNQUOTED_SPECIAL, R"doc(
             A special character was not quoted or escaped.
             )doc")
      .value("U_UNTERMINATED_QUOTE", U_UNTERMINATED_QUOTE, R"doc(
             A closing single quote is missing.
             )doc")
      .value("U_RULE_MASK_ERROR", U_RULE_MASK_ERROR, R"doc(
             A rule is hidden by an earlier more general rule.
             )doc")
      .value("U_MISPLACED_COMPOUND_FILTER", U_MISPLACED_COMPOUND_FILTER,
             R"doc(
             A compound filter is in an invalid location.
             )doc")
      .value("U_MULTIPLE_COMPOUND_FILTERS", U_MULTIPLE_COMPOUND_FILTERS,
             R"doc(
             More than one compound filter.
             )doc")
      .value("U_INVALID_RBT_SYNTAX", U_INVALID_RBT_SYNTAX, R"doc(
             A "::id" rule was passed to the RuleBasedTransliterator parser.
             )doc")
      .value("U_INVALID_PROPERTY_PATTERN", U_INVALID_PROPERTY_PATTERN, R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_MALFORMED_PRAGMA", U_MALFORMED_PRAGMA, R"doc(
             A 'use' pragma is invalid.
             )doc")
      .value("U_UNCLOSED_SEGMENT", U_UNCLOSED_SEGMENT, R"doc(
             A closing ')' is missing.
             )doc")
      .value("U_ILLEGAL_CHAR_IN_SEGMENT", U_ILLEGAL_CHAR_IN_SEGMENT, R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_VARIABLE_RANGE_EXHAUSTED", U_VARIABLE_RANGE_EXHAUSTED, R"doc(
             Too many stand-ins generated for the given variable range.
             )doc")
      .value("U_VARIABLE_RANGE_OVERLAP", U_VARIABLE_RANGE_OVERLAP, R"doc(
             The variable range overlaps characters used in rules.
             )doc")
      .value("U_ILLEGAL_CHARACTER", U_ILLEGAL_CHARACTER, R"doc(
             A special character is outside its allowed context.
             )doc")
      .value("U_INTERNAL_TRANSLITERATOR_ERROR", U_INTERNAL_TRANSLITERATOR_ERROR,
             R"doc(
             Internal transliterator system error.
             )doc")
      .value("U_INVALID_ID", U_INVALID_ID, R"doc(
             A "::id" rule specifies an unknown transliterator.
             )doc")
      .value("U_INVALID_FUNCTION", U_INVALID_FUNCTION, R"doc(
             A "&fn()" rule specifies an unknown transliterator.
             )doc")
      .value("U_PARSE_ERROR_LIMIT", U_PARSE_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_UNEXPECTED_TOKEN", U_UNEXPECTED_TOKEN, R"doc(
             Syntax error in format pattern.
             )doc")
      .value("U_FMT_PARSE_ERROR_START", U_FMT_PARSE_ERROR_START, R"doc(
             Start of format library errors.
             )doc")
      .value("U_MULTIPLE_DECIMAL_SEPARATORS", U_MULTIPLE_DECIMAL_SEPARATORS,
             R"doc(
             More than one decimal separator in number pattern.
             )doc")
      .value("U_MULTIPLE_DECIMAL_SEPERATORS", U_MULTIPLE_DECIMAL_SEPERATORS,
             R"doc(
             Typo: kept for backward compatibility. Use
             U_MULTIPLE_DECIMAL_SEPARATORS.
             )doc")
      .value("U_MULTIPLE_EXPONENTIAL_SYMBOLS", U_MULTIPLE_EXPONENTIAL_SYMBOLS,
             R"doc(
             More than one exponent symbol in number pattern.
             )doc")
      .value("U_MALFORMED_EXPONENTIAL_PATTERN", U_MALFORMED_EXPONENTIAL_PATTERN,
             R"doc(
             Grouping symbol in exponent pattern.
             )doc")
      .value("U_MULTIPLE_PERCENT_SYMBOLS", U_MULTIPLE_PERCENT_SYMBOLS, R"doc(
             More than one percent symbol in number pattern.
             )doc")
      .value("U_MULTIPLE_PERMILL_SYMBOLS", U_MULTIPLE_PERMILL_SYMBOLS, R"doc(
             More than one permill symbol in number pattern.
             )doc")
      .value("U_MULTIPLE_PAD_SPECIFIERS", U_MULTIPLE_PAD_SPECIFIERS, R"doc(
             More than one pad symbol in number pattern.
             )doc")
      .value("U_PATTERN_SYNTAX_ERROR", U_PATTERN_SYNTAX_ERROR, R"doc(
             Syntax error in format pattern.
             )doc")
      .value("U_ILLEGAL_PAD_POSITION", U_ILLEGAL_PAD_POSITION, R"doc(
             Pad symbol misplaced in number pattern.
             )doc")
      .value("U_UNMATCHED_BRACES", U_UNMATCHED_BRACES, R"doc(
             Braces do not match in message pattern.
             )doc")
      .value("U_UNSUPPORTED_PROPERTY", U_UNSUPPORTED_PROPERTY, R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_UNSUPPORTED_ATTRIBUTE", U_UNSUPPORTED_ATTRIBUTE, R"doc(
             UNUSED as of ICU 2.4.
             )doc")
      .value("U_ARGUMENT_TYPE_MISMATCH", U_ARGUMENT_TYPE_MISMATCH, R"doc(
             Argument name and argument index mismatch in MessageFormat
             functions.
             )doc")
      .value("U_DUPLICATE_KEYWORD", U_DUPLICATE_KEYWORD, R"doc(
             Duplicate keyword in PluralFormat.
             )doc")
      .value("U_UNDEFINED_KEYWORD", U_UNDEFINED_KEYWORD, R"doc(
             Undefined Plural keyword.
             )doc")
      .value("U_DEFAULT_KEYWORD_MISSING", U_DEFAULT_KEYWORD_MISSING, R"doc(
             Missing DEFAULT rule in plural rules.
             )doc")
      .value("U_DECIMAL_NUMBER_SYNTAX_ERROR", U_DECIMAL_NUMBER_SYNTAX_ERROR,
             R"doc(
             Decimal number syntax error.
             )doc")
      .value("U_FORMAT_INEXACT_ERROR", U_FORMAT_INEXACT_ERROR, R"doc(
             Cannot format a number exactly and rounding mode is
             ROUND_UNNECESSARY.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("U_NUMBER_ARG_OUTOFBOUNDS_ERROR", U_NUMBER_ARG_OUTOFBOUNDS_ERROR,
             R"doc(
             The argument to a NumberFormatter helper method was out of
             bounds; the bounds are usually 0 to 999.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("U_NUMBER_SKELETON_SYNTAX_ERROR", U_NUMBER_SKELETON_SYNTAX_ERROR,
             R"doc(
             The number skeleton passed to C++ NumberFormatter or C
             UNumberFormatter was invalid or contained a syntax error.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
      .value("U_FMT_PARSE_ERROR_LIMIT", U_FMT_PARSE_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_BRK_INTERNAL_ERROR", U_BRK_INTERNAL_ERROR, R"doc(
             An internal error (bug) was detected.
             )doc")
      .value("U_BRK_ERROR_START", U_BRK_ERROR_START, R"doc(
             Start of codes indicating Break Iterator failures.
             )doc")
      .value("U_BRK_HEX_DIGITS_EXPECTED", U_BRK_HEX_DIGITS_EXPECTED, R"doc(
             Hex digits expected as part of a escaped char in a rule.
             )doc")
      .value("U_BRK_SEMICOLON_EXPECTED", U_BRK_SEMICOLON_EXPECTED, R"doc(
             Missing ';' at the end of a RBBI rule.
             )doc")
      .value("U_BRK_RULE_SYNTAX", U_BRK_RULE_SYNTAX, R"doc(
             Syntax error in RBBI rule.
             )doc")
      .value("U_BRK_UNCLOSED_SET", U_BRK_UNCLOSED_SET, R"doc(
             UnicodeSet writing an RBBI rule missing a closing ']'.
             )doc")
      .value("U_BRK_ASSIGN_ERROR", U_BRK_ASSIGN_ERROR, R"doc(
             Syntax error in RBBI rule assignment statement.
             )doc")
      .value("U_BRK_VARIABLE_REDFINITION", U_BRK_VARIABLE_REDFINITION, R"doc(
             RBBI rule $Variable redefined.
             )doc")
      .value("U_BRK_MISMATCHED_PAREN", U_BRK_MISMATCHED_PAREN, R"doc(
             Mis-matched parentheses in an RBBI rule.
             )doc")
      .value("U_BRK_NEW_LINE_IN_QUOTED_STRING", U_BRK_NEW_LINE_IN_QUOTED_STRING,
             R"doc(
             Missing closing quote in an RBBI rule.
             )doc")
      .value("U_BRK_UNDEFINED_VARIABLE", U_BRK_UNDEFINED_VARIABLE, R"doc(
             Use of an undefined $Variable in an RBBI rule.
             )doc")
      .value("U_BRK_INIT_ERROR", U_BRK_INIT_ERROR, R"doc(
             Initialization failure. Probable missing ICU Data.
             )doc")
      .value("U_BRK_RULE_EMPTY_SET", U_BRK_RULE_EMPTY_SET, R"doc(
             Rule contains an empty Unicode Set.
             )doc")
      .value("U_BRK_UNRECOGNIZED_OPTION", U_BRK_UNRECOGNIZED_OPTION, R"doc(
             Option in RBBI rules not recognized.
             )doc")
      .value("U_BRK_MALFORMED_RULE_TAG", U_BRK_MALFORMED_RULE_TAG, R"doc(
             The {nnn} tag on a rule is malformed.
             )doc")
      .value("U_BRK_ERROR_LIMIT", U_BRK_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_REGEX_INTERNAL_ERROR", U_REGEX_INTERNAL_ERROR, R"doc(
             An internal error (bug) was detected.
             )doc")
      .value("U_REGEX_ERROR_START", U_REGEX_ERROR_START, R"doc(
             Start of codes indicating Regexp failures.
             )doc")
      .value("U_REGEX_RULE_SYNTAX", U_REGEX_RULE_SYNTAX, R"doc(
             Syntax error in regexp pattern.
             )doc")
      .value("U_REGEX_INVALID_STATE", U_REGEX_INVALID_STATE, R"doc(
             RegexMatcher in invalid state for requested operation.
             )doc")
      .value("U_REGEX_BAD_ESCAPE_SEQUENCE", U_REGEX_BAD_ESCAPE_SEQUENCE, R"doc(
             Unrecognized backslash escape sequence in pattern.
             )doc")
      .value("U_REGEX_PROPERTY_SYNTAX", U_REGEX_PROPERTY_SYNTAX, R"doc(
             Incorrect Unicode property.
             )doc")
      .value("U_REGEX_UNIMPLEMENTED", U_REGEX_UNIMPLEMENTED, R"doc(
             Use of regexp feature that is not yet implemented.
             )doc")
      .value("U_REGEX_MISMATCHED_PAREN", U_REGEX_MISMATCHED_PAREN, R"doc(
             Incorrectly nested parentheses in regexp pattern.
             )doc")
      .value("U_REGEX_NUMBER_TOO_BIG", U_REGEX_NUMBER_TOO_BIG, R"doc(
             Decimal number is too large.
             )doc")
      .value("U_REGEX_BAD_INTERVAL", U_REGEX_BAD_INTERVAL, R"doc(
             Error in {min,max} interval.
             )doc")
      .value("U_REGEX_MAX_LT_MIN", U_REGEX_MAX_LT_MIN, R"doc(
             In {min,max}, max is less than min.
             )doc")
      .value("U_REGEX_INVALID_BACK_REF", U_REGEX_INVALID_BACK_REF, R"doc(
             Back-reference to a non-existent capture group.
             )doc")
      .value("U_REGEX_INVALID_FLAG", U_REGEX_INVALID_FLAG, R"doc(
             Invalid value for match mode flags.
             )doc")
      .value("U_REGEX_LOOK_BEHIND_LIMIT", U_REGEX_LOOK_BEHIND_LIMIT, R"doc(
             Look-Behind pattern matches must have a bounded maximum length.
             )doc")
      .value("U_REGEX_SET_CONTAINS_STRING", U_REGEX_SET_CONTAINS_STRING, R"doc(
             Regexps cannot have UnicodeSets containing strings.
             )doc")
      .value("U_REGEX_OCTAL_TOO_BIG", U_REGEX_OCTAL_TOO_BIG, R"doc(
             Deprecated: ICU 54. This error cannot occur.
             )doc")
      .value("U_REGEX_MISSING_CLOSE_BRACKET", U_REGEX_MISSING_CLOSE_BRACKET,
             R"doc(
             Missing closing bracket on a bracket expression.
             )doc")
      .value("U_REGEX_INVALID_RANGE", U_REGEX_INVALID_RANGE, R"doc(
             In a character range [x-y], x is greater than y.
             )doc")
      .value("U_REGEX_STACK_OVERFLOW", U_REGEX_STACK_OVERFLOW, R"doc(
             Regular expression backtrack stack overflow.
             )doc")
      .value("U_REGEX_TIME_OUT", U_REGEX_TIME_OUT, R"doc(
             Maximum allowed match time exceeded.
             )doc")
      .value("U_REGEX_STOPPED_BY_CALLER", U_REGEX_STOPPED_BY_CALLER, R"doc(
             Matching operation aborted by user callback fn.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
      .value("U_REGEX_PATTERN_TOO_BIG", U_REGEX_PATTERN_TOO_BIG, R"doc(
             Pattern exceeds limits on size or complexity.
             )doc")
      .value("U_REGEX_INVALID_CAPTURE_GROUP_NAME",
             U_REGEX_INVALID_CAPTURE_GROUP_NAME, R"doc(
             Invalid capture group name.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
      .value("U_REGEX_ERROR_LIMIT", U_REGEX_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_IDNA_PROHIBITED_ERROR", U_IDNA_PROHIBITED_ERROR)
      .value("U_IDNA_ERROR_START", U_IDNA_ERROR_START)
      .value("U_IDNA_UNASSIGNED_ERROR", U_IDNA_UNASSIGNED_ERROR)
      .value("U_IDNA_CHECK_BIDI_ERROR", U_IDNA_CHECK_BIDI_ERROR)
      .value("U_IDNA_STD3_ASCII_RULES_ERROR", U_IDNA_STD3_ASCII_RULES_ERROR)
      .value("U_IDNA_ACE_PREFIX_ERROR", U_IDNA_ACE_PREFIX_ERROR)
      .value("U_IDNA_VERIFICATION_ERROR", U_IDNA_VERIFICATION_ERROR)
      .value("U_IDNA_LABEL_TOO_LONG_ERROR", U_IDNA_LABEL_TOO_LONG_ERROR)
      .value("U_IDNA_ZERO_LENGTH_LABEL_ERROR", U_IDNA_ZERO_LENGTH_LABEL_ERROR)
      .value("U_IDNA_DOMAIN_NAME_TOO_LONG_ERROR",
             U_IDNA_DOMAIN_NAME_TOO_LONG_ERROR)
      .value("U_IDNA_ERROR_LIMIT", U_IDNA_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_STRINGPREP_PROHIBITED_ERROR", U_STRINGPREP_PROHIBITED_ERROR)
      .value("U_STRINGPREP_UNASSIGNED_ERROR", U_STRINGPREP_UNASSIGNED_ERROR)
      .value("U_STRINGPREP_CHECK_BIDI_ERROR", U_STRINGPREP_CHECK_BIDI_ERROR)
      .value("U_PLUGIN_ERROR_START", U_PLUGIN_ERROR_START, R"doc(
             Start of codes indicating plugin failures.
             )doc")
      .value("U_PLUGIN_TOO_HIGH", U_PLUGIN_TOO_HIGH, R"doc(
             The plugin's level is too high to be loaded right now.
             )doc")
      .value("U_PLUGIN_DIDNT_SET_LEVEL", U_PLUGIN_DIDNT_SET_LEVEL, R"doc(
             The plugin didn't call uplug_setPlugLevel in response to a QUERY.
             )doc")
      .value("U_PLUGIN_ERROR_LIMIT", U_PLUGIN_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .value("U_ERROR_LIMIT", U_ERROR_LIMIT, R"doc(
             Deprecated: ICU 58 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // Functions
  //
  m.def("u_error_name", &u_errorName, py::arg("code"));

  m.def(
      "u_failure", [](UErrorCode code) -> py::bool_ { return U_FAILURE(code); },
      py::arg("code"));

  m.def(
      "u_success", [](UErrorCode code) -> py::bool_ { return U_SUCCESS(code); },
      py::arg("code"));

  m.attr("U_MILLIS_PER_DAY") = U_MILLIS_PER_DAY;
  m.attr("U_MILLIS_PER_HOUR") = U_MILLIS_PER_HOUR;
  m.attr("U_MILLIS_PER_MINUTE") = U_MILLIS_PER_MINUTE;
  m.attr("U_MILLIS_PER_SECOND") = U_MILLIS_PER_SECOND;
}

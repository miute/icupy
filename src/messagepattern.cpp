#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/messagepattern.h>

using namespace icu;
using Part = MessagePattern::Part;

void init_messagepattern(py::module &m) {
  py::enum_<UMessagePatternApostropheMode>(
      m, "UMessagePatternApostropheMode", py::arithmetic(),
      "Mode for when an apostrophe starts quoted literal text for *MessageFormat* output.\n\n"
      "The default is DOUBLE_OPTIONAL unless overridden via uconfig.h (*UCONFIG_MSGPAT_DEFAULT_APOSTROPHE_MODE*).\n\n"
      "A pair of adjacent apostrophes always results in a single apostrophe in the output, even when the pair is "
      "between two single, text-quoting apostrophes.")
      .value("UMSGPAT_APOS_DOUBLE_OPTIONAL", UMSGPAT_APOS_DOUBLE_OPTIONAL,
             "A literal apostrophe is represented by either a single or a double apostrophe pattern character.\n\n  "
             "Within a MessageFormat pattern, a single apostrophe only starts quoted literal text if it immediately "
             "precedes a curly brace {}, or a pipe symbol | if inside a choice format, or a pound symbol # if inside a "
             "plural format.\n\n  "
             "This is the default behavior starting with ICU 4.8.")
      .value("UMSGPAT_APOS_DOUBLE_REQUIRED", UMSGPAT_APOS_DOUBLE_REQUIRED,
             "A literal apostrophe must be represented by a double apostrophe pattern character.\n\n  "
             "A single apostrophe always starts quoted literal text.\n\n  "
             "This is the behavior of ICU 4.6 and earlier, and of the JDK.")
      .export_values();

  py::enum_<UMessagePatternArgType>(m, "UMessagePatternArgType", py::arithmetic(),
                                    "Argument type constants.\n\n"
                                    "Returned by *Part.get_arg_type()* for ARG_START and ARG_LIMIT parts.\n\n"
                                    "Messages nested inside an argument are each delimited by MSG_START and MSG_LIMIT, "
                                    "with a nesting level one greater than the surrounding message.")
      .value("UMSGPAT_ARG_TYPE_NONE", UMSGPAT_ARG_TYPE_NONE, "The argument has no specified type.")
      .value("UMSGPAT_ARG_TYPE_SIMPLE", UMSGPAT_ARG_TYPE_SIMPLE,
             "The argument has a \"simple\" type which is provided by the ARG_TYPE part. An ARG_STYLE part might "
             "follow that.")
      .value("UMSGPAT_ARG_TYPE_CHOICE", UMSGPAT_ARG_TYPE_CHOICE,
             "The argument is a *ChoiceFormat* with one or more ((ARG_INT | ARG_DOUBLE), ARG_SELECTOR, message) "
             "tuples.")
      .value("UMSGPAT_ARG_TYPE_PLURAL", UMSGPAT_ARG_TYPE_PLURAL,
             "The argument is a cardinal-number *PluralFormat* with an optional ARG_INT or ARG_DOUBLE offset "
             "(e.g., offset:1) and one or more (ARG_SELECTOR [explicit-value] message) tuples.\n\n  "
             "If the selector has an explicit value (e.g., =2), then that value is provided by the ARG_INT or "
             "ARG_DOUBLE part preceding the message. Otherwise the message immediately follows the ARG_SELECTOR.")
      .value("UMSGPAT_ARG_TYPE_SELECT", UMSGPAT_ARG_TYPE_SELECT,
             "The argument is a *SelectFormat* with one or more (ARG_SELECTOR, message) pairs.")
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UMSGPAT_ARG_TYPE_SELECTORDINAL", UMSGPAT_ARG_TYPE_SELECTORDINAL,
             "The argument is an ordinal-number *PluralFormat* with the same style parts sequence and semantics as "
             "*UMSGPAT_ARG_TYPE_PLURAL*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      .export_values();

  py::enum_<UMessagePatternPartType>(m, "UMessagePatternPartType", py::arithmetic(),
                                     "*MessagePattern::Part* type constants.")
      .value("UMSGPAT_PART_TYPE_MSG_START", UMSGPAT_PART_TYPE_MSG_START,
             "Start of a message pattern (main or nested).\n\n  "
             "The length is 0 for the top-level message and for a choice argument sub-message, otherwise 1 for the "
             "'{'. The value indicates the nesting level, starting with 0 for the main message.\n\n  "
             "There is always a later MSG_LIMIT part.")
      .value("UMSGPAT_PART_TYPE_MSG_LIMIT", UMSGPAT_PART_TYPE_MSG_LIMIT,
             "End of a message pattern (main or nested).\n\n  "
             "The length is 0 for the top-level message and the last sub-message of a choice argument, otherwise 1 for "
             "the '}' or (in a choice argument style) the '|'. The value indicates the nesting level, starting with 0 "
             "for the main message.")
      .value("UMSGPAT_PART_TYPE_SKIP_SYNTAX", UMSGPAT_PART_TYPE_SKIP_SYNTAX,
             "Indicates a substring of the pattern string which is to be skipped when formatting.\n\n  "
             "For example, an apostrophe that begins or ends quoted text would be indicated with such a part. The "
             "value is undefined and currently always 0.")
      .value("UMSGPAT_PART_TYPE_INSERT_CHAR", UMSGPAT_PART_TYPE_INSERT_CHAR,
             "Indicates that a syntax character needs to be inserted for auto-quoting.\n\n  "
             "The length is 0. The value is the character code of the insertion character. (U+0027=APOSTROPHE)")
      .value("UMSGPAT_PART_TYPE_REPLACE_NUMBER", UMSGPAT_PART_TYPE_REPLACE_NUMBER,
             "Indicates a syntactic (non-escaped) # symbol in a plural variant.\n\n  "
             "When formatting, replace this part's substring with the (value-offset) for the plural argument value. "
             "The value is undefined and currently always 0.")
      .value("UMSGPAT_PART_TYPE_ARG_START", UMSGPAT_PART_TYPE_ARG_START,
             "Start of an argument.\n\n  "
             "The length is 1 for the '{'. The value is the ordinal value of the ArgType. Use *get_arg_type()*.\n\n  "
             "This part is followed by either an ARG_NUMBER or ARG_NAME, followed by optional argument sub-parts "
             "(see *UMessagePatternArgType* constants) and finally an ARG_LIMIT part.")
      .value("UMSGPAT_PART_TYPE_ARG_LIMIT", UMSGPAT_PART_TYPE_ARG_LIMIT,
             "End of an argument.\n\n  "
             "The length is 1 for the '}'. The value is the ordinal value of the ArgType. Use *get_arg_type()*.")
      .value("UMSGPAT_PART_TYPE_ARG_NUMBER", UMSGPAT_PART_TYPE_ARG_NUMBER,
             "The argument number, provided by the value.")
      .value("UMSGPAT_PART_TYPE_ARG_NAME", UMSGPAT_PART_TYPE_ARG_NAME,
             "The argument name.\n\n  "
             "The value is undefined and currently always 0.")
      .value("UMSGPAT_PART_TYPE_ARG_TYPE", UMSGPAT_PART_TYPE_ARG_TYPE,
             "The argument type.\n\n  "
             "The value is undefined and currently always 0.")
      .value("UMSGPAT_PART_TYPE_ARG_STYLE", UMSGPAT_PART_TYPE_ARG_STYLE,
             "The argument style text.\n\n  "
             "The value is undefined and currently always 0.")
      .value("UMSGPAT_PART_TYPE_ARG_SELECTOR", UMSGPAT_PART_TYPE_ARG_SELECTOR,
             "A selector substring in a \"complex\" argument style.\n\n  "
             "The value is undefined and currently always 0.")
      .value("UMSGPAT_PART_TYPE_ARG_INT", UMSGPAT_PART_TYPE_ARG_INT,
             "An integer value, for example the offset or an explicit selector value in a *PluralFormat* style.\n\n  "
             "The part value is the integer value.")
      .value("UMSGPAT_PART_TYPE_ARG_DOUBLE", UMSGPAT_PART_TYPE_ARG_DOUBLE,
             "A numeric value, for example the offset or an explicit selector value in a *PluralFormat* style.\n\n  "
             "The part value is an index into an internal array of numeric values; use *get_numeric_value()*.")
      .export_values();

  // icu::MessagePattern
  py::class_<MessagePattern, UObject> mp(m, "MessagePattern");

  // icu::MessagePattern::Part
  py::class_<MessagePattern::Part, UMemory> part(mp, "Part");
  // FIXME: Unresolved external symbol error with "icu::MessagePattern::Part::operator==(const Part &other)".
  // part.def(py::self != py::self, py::arg("other")).def(py::self == py::self, py::arg("other"));
  part.def("get_arg_type", &MessagePattern::Part::getArgType);
  part.def("get_index", &MessagePattern::Part::getIndex);
  part.def("get_length", &MessagePattern::Part::getLength);
  part.def("get_limit", &MessagePattern::Part::getLimit);
  part.def("get_type", &MessagePattern::Part::getType);
  part.def("get_value", &MessagePattern::Part::getValue);
  part.def("hash_code", &MessagePattern::Part::hashCode);
  part.def_static("has_numeric_value", &MessagePattern::Part::hasNumericValue, py::arg("type_"));

  // icu::MessagePattern
  mp.def(
        // [1] MessagePattern::MessagePattern
        py::init([]() {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = std::make_unique<MessagePattern>(error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        }))
      .def(
          // [2] MessagePattern::MessagePattern
          py::init([](UMessagePatternApostropheMode mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<MessagePattern>(mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("mode"))
      .def(
          // [3] MessagePattern::MessagePattern
          py::init([](const UnicodeString &pattern, UParseError *parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<MessagePattern>(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern, UParseError *parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<MessagePattern>(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("parse_error"))
      .def(
          // [4] MessagePattern::MessagePattern
          py::init<const MessagePattern &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  mp.def("auto_quote_apostrophe_deep", &MessagePattern::autoQuoteApostropheDeep);
  mp.def("clear", &MessagePattern::clear);
  mp.def("clear_pattern_and_set_apostrophe_mode", &MessagePattern::clearPatternAndSetApostropheMode, py::arg("mode"));
  mp.def("count_parts", &MessagePattern::countParts);
  mp.def("get_apostrophe_mode", &MessagePattern::getApostropheMode);
  mp.def("get_limit_part_index", &MessagePattern::getLimitPartIndex, py::arg("start"));
  mp.def("get_numeric_value", &MessagePattern::getNumericValue, py::arg("part"));
  mp.def("get_part", &MessagePattern::getPart, py::arg("i"));
  mp.def("get_part_type", &MessagePattern::getPartType, py::arg("i"));
  mp.def("get_pattern_index", &MessagePattern::getPatternIndex, py::arg("part_index"));
  mp.def("get_pattern_string", &MessagePattern::getPatternString);
  mp.def("get_plural_offset", &MessagePattern::getPluralOffset, py::arg("plural_start"));
  mp.def("get_substring", &MessagePattern::getSubstring, py::arg("part"));
  mp.def("hash_code", &MessagePattern::hashCode);
  mp.def("has_named_arguments", &MessagePattern::hasNamedArguments);
  mp.def("has_numbered_arguments", &MessagePattern::hasNumberedArguments);
  mp.def(
        "parse",
        [](MessagePattern &self, const UnicodeString &pattern, UParseError *parse_error) -> MessagePattern & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.parse(pattern, parse_error, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "parse",
          [](MessagePattern &self, const char16_t *pattern, UParseError *parse_error) -> MessagePattern & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.parse(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("parse_error"));
  mp.def(
        "parse_choice_style",
        [](MessagePattern &self, const UnicodeString &pattern, UParseError *parse_error) -> MessagePattern & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.parseChoiceStyle(pattern, parse_error, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "parse_choice_style",
          [](MessagePattern &self, const char16_t *pattern, UParseError *parse_error) -> MessagePattern & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.parseChoiceStyle(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("parse_error"));
  mp.def(
        "parse_plural_style",
        [](MessagePattern &self, const UnicodeString &pattern, UParseError *parse_error) -> MessagePattern & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.parsePluralStyle(pattern, parse_error, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "parse_plural_style",
          [](MessagePattern &self, const char16_t *pattern, UParseError *parse_error) -> MessagePattern & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.parsePluralStyle(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("parse_error"));
  mp.def(
        "parse_select_style",
        [](MessagePattern &self, const UnicodeString &pattern, UParseError *parse_error) -> MessagePattern & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.parseSelectStyle(pattern, parse_error, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("pattern"), py::arg("parse_error"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "parse_select_style",
          [](MessagePattern &self, const char16_t *pattern, UParseError *parse_error) -> MessagePattern & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.parseSelectStyle(pattern, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("parse_error"));
  mp.def("part_substring_matches", &MessagePattern::partSubstringMatches, py::arg("part"), py::arg("s"))
      .def(
          // const char16_t *s -> const UnicodeString &s
          "part_substring_matches",
          [](const MessagePattern &self, const Part &part, const char16_t *s) {
            return self.partSubstringMatches(part, s);
          },
          py::arg("part"), py::arg("s"));
  mp.def_static("validate_argument_name", &MessagePattern::validateArgumentName, py::arg("name"))
      .def_static(
          // const char16_t *name -> const UnicodeString &name
          "validate_argument_name", [](const char16_t *name) { return MessagePattern::validateArgumentName(name); },
          py::arg("name"));

  m.attr("UMSGPAT_ARG_NAME_NOT_NUMBER") = int32_t{UMSGPAT_ARG_NAME_NOT_NUMBER};
  m.attr("UMSGPAT_ARG_NAME_NOT_VALID") = int32_t{UMSGPAT_ARG_NAME_NOT_VALID};
  m.attr("UMSGPAT_NO_NUMERIC_VALUE") = UMSGPAT_NO_NUMERIC_VALUE;
}

#include "main.hpp"
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/messagepattern.h>

using namespace icu;
using Part = MessagePattern::Part;

void init_messagepattern(py::module &m) {
  //
  // enum UMessagePatternApostropheMode
  //
  py::enum_<UMessagePatternApostropheMode>(m, "UMessagePatternApostropheMode",
                                           py::arithmetic(), R"doc(
Mode for when an apostrophe starts quoted literal text for
:class:`MessageFormat` output.

The default is DOUBLE_OPTIONAL unless overridden via uconfig.h
(UCONFIG_MSGPAT_DEFAULT_APOSTROPHE_MODE).

A pair of adjacent apostrophes always results in a single apostrophe in the
output, even when the pair is between two single, text-quoting apostrophes.
      )doc")
      .value("UMSGPAT_APOS_DOUBLE_OPTIONAL", UMSGPAT_APOS_DOUBLE_OPTIONAL,
             R"doc(
             A literal apostrophe is represented by either a single or a
             double apostrophe pattern character.

             Within a :class:`MessageFormat` pattern, a single apostrophe only
             starts quoted literal text if it immediately precedes a curly
             brace {}, or a pipe symbol | if inside a choice format, or a pound
             symbol # if inside a plural format.

             This is the default behavior starting with ICU 4.8.
             )doc")
      .value("UMSGPAT_APOS_DOUBLE_REQUIRED", UMSGPAT_APOS_DOUBLE_REQUIRED,
             R"doc(
             A literal apostrophe must be represented by a double apostrophe
             pattern character.

             A single apostrophe always starts quoted literal text.

             This is the behavior of ICU 4.6 and earlier, and of the JDK.
             )doc")
      .export_values();

  //
  // enum UMessagePatternArgName
  //
  py::enum_<decltype(UMSGPAT_ARG_NAME_NOT_NUMBER)>(m, "UMessagePatternArgName",
                                                   py::arithmetic(), R"doc(
Enum constants for :meth:`MessagePattern.validate_argument_name`.

Warning:
    ``UMessagePatternArgName`` is not standard. Do not use the fully qualified
    name as this name may change in the future.
  )doc")
      .value("UMSGPAT_ARG_NAME_NOT_NUMBER", UMSGPAT_ARG_NAME_NOT_NUMBER, R"doc(
             Return value from :meth:`MessagePattern.validate_argument_name`
             for when the string is a valid "pattern identifier" but not a
             number.
             )doc")
      .value("UMSGPAT_ARG_NAME_NOT_VALID", UMSGPAT_ARG_NAME_NOT_VALID, R"doc(
             Return value from :meth:`MessagePattern.validate_argument_name`
             for when the string is invalid.

             It might not be a valid "pattern identifier", or it have only
             ASCII digits but there is a leading zero or the number is too
             large.
             )doc")
      .export_values();

  //
  // enum UMessagePatternArgType
  //
  py::enum_<UMessagePatternArgType>(m, "UMessagePatternArgType",
                                    py::arithmetic(), R"doc(
Argument type constants.

Returned by :meth:`MessagePattern.Part.get_arg_type` for ARG_START and
ARG_LIMIT parts.

Messages nested inside an argument are each delimited by MSG_START and
MSG_LIMIT, with a nesting level one greater than the surrounding message.
      )doc")
      .value("UMSGPAT_ARG_TYPE_NONE", UMSGPAT_ARG_TYPE_NONE, R"doc(
             The argument has no specified type.
             )doc")
      .value("UMSGPAT_ARG_TYPE_SIMPLE", UMSGPAT_ARG_TYPE_SIMPLE, R"doc(
             The argument has a "simple" type which is provided by the
             ARG_TYPE part.

             An ARG_STYLE part might follow that.
             )doc")
      .value("UMSGPAT_ARG_TYPE_CHOICE", UMSGPAT_ARG_TYPE_CHOICE, R"doc(
             The argument is a :class:`ChoiceFormat` with one or more
             ((ARG_INT | ARG_DOUBLE), ARG_SELECTOR, message) tuples.
             )doc")
      .value("UMSGPAT_ARG_TYPE_PLURAL", UMSGPAT_ARG_TYPE_PLURAL, R"doc(
             The argument is a cardinal-number :class:`PluralFormat` with an
             optional ARG_INT or ARG_DOUBLE offset (e.g., offset:1) and one or
             more (ARG_SELECTOR [explicit-value] message) tuples.

             If the selector has an explicit value (e.g., =2), then that
             value is provided by the ARG_INT or ARG_DOUBLE part preceding the
             message. Otherwise the message immediately follows the
             ARG_SELECTOR.
             )doc")
      .value("UMSGPAT_ARG_TYPE_SELECT", UMSGPAT_ARG_TYPE_SELECT, R"doc(
             The argument is a :class:`SelectFormat` with one or more
             (ARG_SELECTOR, message) pairs.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UMSGPAT_ARG_TYPE_SELECTORDINAL", UMSGPAT_ARG_TYPE_SELECTORDINAL,
             R"doc(
             The argument is an ordinal-number :class:`PluralFormat` with the same
             style parts sequence and semantics as :attr:`UMSGPAT_ARG_TYPE_PLURAL`.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      .export_values();

  //
  // enum UMessagePatternPartType
  //
  py::enum_<UMessagePatternPartType>(m, "UMessagePatternPartType",
                                     py::arithmetic(), R"doc(
:class:`MessagePattern.Part` type constants.
      )doc")
      .value("UMSGPAT_PART_TYPE_MSG_START", UMSGPAT_PART_TYPE_MSG_START, R"doc(
             Start of a message pattern (main or nested).

             The length is 0 for the top-level message and for a choice
             argument sub-message, otherwise 1 for the '{'. The value indicates
             the nesting level, starting with 0 for the main message.

             There is always a later MSG_LIMIT part.
             )doc")
      .value("UMSGPAT_PART_TYPE_MSG_LIMIT", UMSGPAT_PART_TYPE_MSG_LIMIT, R"doc(
             End of a message pattern (main or nested).

             The length is 0 for the top-level message and the last
             sub-message of a choice argument, otherwise 1 for the '}' or (in
             a choice argument style) the '|'. The value indicates the nesting
             level, starting with 0 for the main message.
             )doc")
      .value("UMSGPAT_PART_TYPE_SKIP_SYNTAX", UMSGPAT_PART_TYPE_SKIP_SYNTAX,
             R"doc(
             Indicates a substring of the pattern string which is to be
             skipped when formatting.

             For example, an apostrophe that begins or ends quoted text would
             be indicated with such a part. The value is undefined and
             currently always 0.
             )doc")
      .value("UMSGPAT_PART_TYPE_INSERT_CHAR", UMSGPAT_PART_TYPE_INSERT_CHAR,
             R"doc(
             Indicates that a syntax character needs to be inserted for
             auto-quoting.

             The length is 0. The value is the character code of the insertion
             character. (U+0027=APOSTROPHE)
             )doc")
      .value("UMSGPAT_PART_TYPE_REPLACE_NUMBER",
             UMSGPAT_PART_TYPE_REPLACE_NUMBER, R"doc(
             Indicates a syntactic (non-escaped) # symbol in a plural
             variant.

             When formatting, replace this part's substring with the
             (value-offset) for the plural argument value.
             The value is undefined and currently always 0.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_START", UMSGPAT_PART_TYPE_ARG_START, R"doc(
             Start of an argument.

             The length is 1 for the '{'. The value is the ordinal value of
             the ArgType. Use :meth:`MessagePattern.Part.get_arg_type`.
             This part is followed by either an ARG_NUMBER or ARG_NAME,
             followed by optional argument sub-parts (see
             :class:`UMessagePatternArgType` constants) and finally an
             ARG_LIMIT part.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_LIMIT", UMSGPAT_PART_TYPE_ARG_LIMIT, R"doc(
             End of an argument.

             The length is 1 for the '}'. The value is the ordinal value of
             the ArgType. Use :meth:`MessagePattern.Part.get_arg_type`.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_NUMBER", UMSGPAT_PART_TYPE_ARG_NUMBER,
             R"doc(
             The argument number, provided by the value.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_NAME", UMSGPAT_PART_TYPE_ARG_NAME, R"doc(
             The argument name.

             The value is undefined and currently always 0.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_TYPE", UMSGPAT_PART_TYPE_ARG_TYPE, R"doc(
             The argument type.

             The value is undefined and currently always 0.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_STYLE", UMSGPAT_PART_TYPE_ARG_STYLE, R"doc(
             The argument style text.

             The value is undefined and currently always 0.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_SELECTOR", UMSGPAT_PART_TYPE_ARG_SELECTOR,
             R"doc(
             A selector substring in a "complex" argument style.

             The value is undefined and currently always 0.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_INT", UMSGPAT_PART_TYPE_ARG_INT, R"doc(
             An integer value, for example the offset or an explicit selector
             value in a :class:`PluralFormat` style.

             The part value is the integer value.
             )doc")
      .value("UMSGPAT_PART_TYPE_ARG_DOUBLE", UMSGPAT_PART_TYPE_ARG_DOUBLE,
             R"doc(
             A numeric value, for example the offset or an explicit selector
             value in a :class:`PluralFormat` style.

             The part value is an index into an internal array of numeric
             values; use :meth:`MessagePattern.get_numeric_value`.
             )doc")
      .export_values();

  //
  // class icu::MessagePattern
  //
  py::class_<MessagePattern, UObject> mp(m, "MessagePattern", R"doc(
      Parse and represent ICU :class:`MessageFormat`,
      :class:`PluralFormat` and
      :class:`SelectFormat` patterns.
      )doc");

  //
  // class icu::MessagePattern::Part
  //
  py::class_<MessagePattern::Part, UMemory> part(mp, "Part", R"doc(
      A message pattern "part", representing a pattern parsing event.

      There is a part for the start and end of a message or argument, for
      quoting and escaping of and with ASCII apostrophes, and for syntax
      elements of "complex" arguments.
      )doc");

  // FIXME: Unresolved external symbol error with
  //  "icu::MessagePattern::Part::operator==(const Part &other)".
  /*
  part.def(
      "__eq__",
      [](const MessagePattern::Part &self, const Part &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"));

  part.def(
      "__ne__",
      [](const MessagePattern::Part &self, const Part &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"));
   */

  part.def("__hash__", &MessagePattern::Part::hashCode);

  part.def("get_arg_type", &MessagePattern::Part::getArgType);

  part.def("get_index", &MessagePattern::Part::getIndex);

  part.def("get_length", &MessagePattern::Part::getLength);

  part.def("get_limit", &MessagePattern::Part::getLimit);

  part.def("get_type", &MessagePattern::Part::getType);

  part.def("get_value", &MessagePattern::Part::getValue);

  part.def("hash_code", &MessagePattern::Part::hashCode);

  part.def_static(
      "has_numeric_value",
      [](UMessagePatternPartType type) -> py::bool_ {
        return MessagePattern::Part::hasNumericValue(type);
      },
      py::arg("type"));

  //
  // class icu::MessagePattern
  //
  mp.def(
        // [1] MessagePattern::MessagePattern
        py::init([]() {
          ErrorCode error_code;
          auto result = std::make_unique<MessagePattern>(error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        }))
      .def(
          // [2] MessagePattern::MessagePattern
          py::init([](UMessagePatternApostropheMode mode) {
            ErrorCode error_code;
            auto result = std::make_unique<MessagePattern>(mode, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("mode"))
      .def(
          // [3] MessagePattern::MessagePattern
          py::init([](const icupy::UnicodeStringVariant &pattern,
                      std::optional<UParseError *> &parse_error) {
            ErrorCode error_code;
            auto result = std::make_unique<MessagePattern>(
                icupy::to_unistr(pattern), parse_error.value_or(nullptr),
                error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("parse_error") = std::nullopt)
      .def(
          // [4] MessagePattern::MessagePattern
          py::init<const MessagePattern &>(), py::arg("other"));

  mp.def(
      "__eq__",
      [](const MessagePattern &self, const MessagePattern &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"));

  mp.def("__hash__", &MessagePattern::hashCode);

  mp.def(
      "__ne__",
      [](const MessagePattern &self, const MessagePattern &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"));

  mp.def("auto_quote_apostrophe_deep",
         &MessagePattern::autoQuoteApostropheDeep);

  mp.def("clear", &MessagePattern::clear);

  mp.def("clear_pattern_and_set_apostrophe_mode",
         &MessagePattern::clearPatternAndSetApostropheMode, py::arg("mode"));

  mp.def("count_parts", &MessagePattern::countParts);

  mp.def("get_apostrophe_mode", &MessagePattern::getApostropheMode);

  mp.def("get_limit_part_index", &MessagePattern::getLimitPartIndex,
         py::arg("start"));

  mp.def("get_numeric_value", &MessagePattern::getNumericValue,
         py::arg("part"));

  mp.def("get_part", &MessagePattern::getPart, py::arg("i"));

  mp.def("get_part_type", &MessagePattern::getPartType, py::arg("i"));

  mp.def("get_pattern_index", &MessagePattern::getPatternIndex,
         py::arg("part_index"));

  mp.def("get_pattern_string", &MessagePattern::getPatternString);

  mp.def("get_plural_offset", &MessagePattern::getPluralOffset,
         py::arg("plural_start"));

  mp.def("get_substring", &MessagePattern::getSubstring, py::arg("part"));

  mp.def("hash_code", &MessagePattern::hashCode);

  mp.def("has_named_arguments", [](const MessagePattern &self) -> py::bool_ {
    return self.hasNamedArguments();
  });

  mp.def("has_numbered_arguments", [](const MessagePattern &self) -> py::bool_ {
    return self.hasNumberedArguments();
  });

  mp.def(
      "parse",
      [](MessagePattern &self, const icupy::UnicodeStringVariant &pattern,
         std::optional<UParseError *> &parse_error) -> MessagePattern & {
        ErrorCode error_code;
        auto &result = self.parse(icupy::to_unistr(pattern),
                                  parse_error.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"), py::arg("parse_error") = std::nullopt);

  mp.def(
      "parse_choice_style",
      [](MessagePattern &self, const icupy::UnicodeStringVariant &pattern,
         std::optional<UParseError *> &parse_error) -> MessagePattern & {
        ErrorCode error_code;
        auto &result =
            self.parseChoiceStyle(icupy::to_unistr(pattern),
                                  parse_error.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"), py::arg("parse_error") = std::nullopt);

  mp.def(
      "parse_plural_style",
      [](MessagePattern &self, const icupy::UnicodeStringVariant &pattern,
         std::optional<UParseError *> &parse_error) -> MessagePattern & {
        ErrorCode error_code;
        auto &result =
            self.parsePluralStyle(icupy::to_unistr(pattern),
                                  parse_error.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"), py::arg("parse_error") = std::nullopt);

  mp.def(
      "parse_select_style",
      [](MessagePattern &self, const icupy::UnicodeStringVariant &pattern,
         std::optional<UParseError *> &parse_error) -> MessagePattern & {
        ErrorCode error_code;
        auto &result =
            self.parseSelectStyle(icupy::to_unistr(pattern),
                                  parse_error.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"), py::arg("parse_error") = std::nullopt);

  mp.def(
      "part_substring_matches",
      [](const MessagePattern &self, const Part &part,
         const icupy::UnicodeStringVariant &s) -> py::bool_ {
        return self.partSubstringMatches(part, icupy::to_unistr(s));
      },
      py::arg("part"), py::arg("s"));

  mp.def_static(
      "validate_argument_name",
      [](const icupy::UnicodeStringVariant &name) {
        return MessagePattern::validateArgumentName(icupy::to_unistr(name));
      },
      py::arg("name"));

  m.attr("UMSGPAT_NO_NUMERIC_VALUE") = UMSGPAT_NO_NUMERIC_VALUE;
}

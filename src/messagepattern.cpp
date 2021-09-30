#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/messagepattern.h>

using namespace icu;
using Part = MessagePattern::Part;

void init_messagepattern(py::module &m) {
  py::enum_<UMessagePatternApostropheMode>(m, "UMessagePatternApostropheMode", py::arithmetic())
      .value("UMSGPAT_APOS_DOUBLE_OPTIONAL", UMSGPAT_APOS_DOUBLE_OPTIONAL)
      .value("UMSGPAT_APOS_DOUBLE_REQUIRED", UMSGPAT_APOS_DOUBLE_REQUIRED)
      .export_values();

  py::enum_<UMessagePatternArgType>(m, "UMessagePatternArgType", py::arithmetic())
      .value("UMSGPAT_ARG_TYPE_NONE", UMSGPAT_ARG_TYPE_NONE)
      .value("UMSGPAT_ARG_TYPE_SIMPLE", UMSGPAT_ARG_TYPE_SIMPLE)
      .value("UMSGPAT_ARG_TYPE_CHOICE", UMSGPAT_ARG_TYPE_CHOICE)
      .value("UMSGPAT_ARG_TYPE_PLURAL", UMSGPAT_ARG_TYPE_PLURAL)
      .value("UMSGPAT_ARG_TYPE_SELECT", UMSGPAT_ARG_TYPE_SELECT)
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
      .value("UMSGPAT_ARG_TYPE_SELECTORDINAL", UMSGPAT_ARG_TYPE_SELECTORDINAL)
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
      .export_values();

  py::enum_<UMessagePatternPartType>(m, "UMessagePatternPartType", py::arithmetic())
      .value("UMSGPAT_PART_TYPE_MSG_START", UMSGPAT_PART_TYPE_MSG_START)
      .value("UMSGPAT_PART_TYPE_MSG_LIMIT", UMSGPAT_PART_TYPE_MSG_LIMIT)
      .value("UMSGPAT_PART_TYPE_SKIP_SYNTAX", UMSGPAT_PART_TYPE_SKIP_SYNTAX)
      .value("UMSGPAT_PART_TYPE_INSERT_CHAR", UMSGPAT_PART_TYPE_INSERT_CHAR)
      .value("UMSGPAT_PART_TYPE_REPLACE_NUMBER", UMSGPAT_PART_TYPE_REPLACE_NUMBER)
      .value("UMSGPAT_PART_TYPE_ARG_START", UMSGPAT_PART_TYPE_ARG_START)
      .value("UMSGPAT_PART_TYPE_ARG_LIMIT", UMSGPAT_PART_TYPE_ARG_LIMIT)
      .value("UMSGPAT_PART_TYPE_ARG_NUMBER", UMSGPAT_PART_TYPE_ARG_NUMBER)
      .value("UMSGPAT_PART_TYPE_ARG_NAME", UMSGPAT_PART_TYPE_ARG_NAME)
      .value("UMSGPAT_PART_TYPE_ARG_TYPE", UMSGPAT_PART_TYPE_ARG_TYPE)
      .value("UMSGPAT_PART_TYPE_ARG_STYLE", UMSGPAT_PART_TYPE_ARG_STYLE)
      .value("UMSGPAT_PART_TYPE_ARG_SELECTOR", UMSGPAT_PART_TYPE_ARG_SELECTOR)
      .value("UMSGPAT_PART_TYPE_ARG_INT", UMSGPAT_PART_TYPE_ARG_INT)
      .value("UMSGPAT_PART_TYPE_ARG_DOUBLE", UMSGPAT_PART_TYPE_ARG_DOUBLE)
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

  m.attr("UMSGPAT_ARG_NAME_NOT_NUMBER") = (int32_t)UMSGPAT_ARG_NAME_NOT_NUMBER;
  m.attr("UMSGPAT_ARG_NAME_NOT_VALID") = (int32_t)UMSGPAT_ARG_NAME_NOT_VALID;
  m.attr("UMSGPAT_NO_NUMERIC_VALUE") = UMSGPAT_NO_NUMERIC_VALUE;
}

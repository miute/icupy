#include "main.hpp"
#include <unicode/selfmt.h>

using namespace icu;

void init_selfmt(py::module &m) {
  // icu::SelectFormat
  py::class_<SelectFormat, Format> sf(m, "SelectFormat");
  sf.def(py::init([](const UnicodeString &pattern) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = std::make_unique<SelectFormat>(pattern, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         }),
         py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SelectFormat>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(py::init<const SelectFormat &>(), py::arg("other"));
  sf.def("__copy__", &SelectFormat::clone)
      .def(
          "___deepcopy__", [](const SelectFormat &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const SelectFormat &self, const Format &other) { return self == other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__ne__", [](const SelectFormat &self, const Format &other) { return self != other; }, py::is_operator(),
          py::arg("other"));
  sf.def(
        "apply_pattern",
        [](SelectFormat &self, const UnicodeString &pattern) {
          UErrorCode error_code = U_ZERO_ERROR;
          self.applyPattern(pattern, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          "apply_pattern",
          [](SelectFormat &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"));
  sf.def("clone", &SelectFormat::clone);
  sf.def(
        // [1] SelectFormat::format
        // [2] Format::format
        "format",
        [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to,
           FieldPosition &pos) -> UnicodeString & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.format(obj, append_to, pos, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [3] Format::format
          "format",
          [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [4] Format::format
          "format",
          [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] SelectFormat::format
          "format",
          [](const SelectFormat &self, const UnicodeString &keyword, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(keyword, append_to, pos, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("keyword"), py::arg("append_to"), py::arg("pos"))
      .def(
          // const char16_t *keyword -> const UnicodeString &keyword
          "format",
          [](const SelectFormat &self, const char16_t *keyword, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(keyword, append_to, pos, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("keyword"), py::arg("append_to"), py::arg("pos"));
  sf.def("parse_object", &SelectFormat::parseObject, py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const SelectFormat &self, const char16_t *source, Formattable &result, ParsePosition &parse_pos) {
            self.parseObject(source, result, parse_pos);
          },
          py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          "parse_object",
          [](const Format &self, const UnicodeString &source, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parseObject(source, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const Format &self, const char16_t *source, Formattable &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.parseObject(source, result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
  sf.def("to_pattern", &SelectFormat::toPattern, py::arg("append_to"));
}

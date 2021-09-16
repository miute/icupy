#include "main.hpp"
#include <unicode/format.h>

using namespace icu;

void init_format(py::module &m) {
  // icu::Format
  py::class_<Format, UObject> fmt(m, "Format");
  fmt.def("__copy__", &Format::clone)
      .def(
          "__deepcopy__", [](const Format &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const Format &self, const Format &other) { return self == other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__ne__", [](const Format &self, const Format &other) { return self != other; }, py::is_operator(),
          py::arg("other"));
  fmt.def("clone", &Format::clone);
  fmt.def(
         "format",
         [](const Format &self, const Formattable &obj, UnicodeString &append_to,
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
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to,
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
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));
  fmt.def(
      "get_locale",
      [](const Format &self, ULocDataLocaleType type) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getLocale(type, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
  fmt.def("parse_object",
          py::overload_cast<const UnicodeString &, Formattable &, ParsePosition &>(&Format::parseObject, py::const_),
          py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const Format &self, const char16_t *source, Formattable &result, ParsePosition &parse_pos) {
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
}

#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/format.h>

using namespace icu;

void init_format(py::module &m) {
  //
  // icu::Format
  //
  py::class_<Format, UObject> fmt(m, "Format");

  fmt.def("__copy__", &Format::clone);

  fmt.def(
      "__deepcopy__", [](const Format &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  fmt.def(
      "__eq__", [](const Format &self, const Format &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  fmt.def(
      "__ne__", [](const Format &self, const Format &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  fmt.def("clone", &Format::clone);

  fmt.def(
         "format",
         [](const Format &self, const Formattable &obj, UnicodeString &append_to,
            FieldPosition &pos) -> UnicodeString & {
           ErrorCode error_code;
           auto &result = self.format(obj, append_to, pos, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          "format",
          [](const Format &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));

  fmt.def(
      "get_locale",
      [](const Format &self, ULocDataLocaleType type) {
        ErrorCode error_code;
        auto result = self.getLocale(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));

  fmt.def(
         "parse_object",
         [](const Format &self, const icupy::UnicodeStringVariant &source, Formattable &result,
            ParsePosition &parse_pos) { self.parseObject(icupy::to_unistr(source), result, parse_pos); },
         py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          "parse_object",
          [](const Format &self, const icupy::UnicodeStringVariant &source, Formattable &result) {
            ErrorCode error_code;
            self.parseObject(icupy::to_unistr(source), result, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
}

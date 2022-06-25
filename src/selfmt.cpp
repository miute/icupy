#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/selfmt.h>

using namespace icu;

void init_selfmt(py::module &m) {
  //
  // icu::SelectFormat
  //
  py::class_<SelectFormat, Format> sf(m, "SelectFormat");

  sf.def(py::init([](const _UnicodeStringVariant &pattern) {
           ErrorCode error_code;
           auto result = std::make_unique<SelectFormat>(VARIANT_TO_UNISTR(pattern), error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
           }
           return result;
         }),
         py::arg("pattern"))
      .def(py::init<const SelectFormat &>(), py::arg("other"));

  sf.def("__copy__", &SelectFormat::clone);

  sf.def(
      "___deepcopy__", [](const SelectFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  sf.def(
      "__eq__", [](const SelectFormat &self, const Format &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  sf.def(
      "__ne__", [](const SelectFormat &self, const Format &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  sf.def(
      "apply_pattern",
      [](SelectFormat &self, const _UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.applyPattern(VARIANT_TO_UNISTR(pattern), error_code);
        if (error_code.isFailure()) {
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
          ErrorCode error_code;
          auto &result = self.format(obj, append_to, pos, error_code);
          if (error_code.isFailure()) {
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
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [4] Format::format
          "format",
          [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] SelectFormat::format
          "format",
          [](const SelectFormat &self, const _UnicodeStringVariant &keyword, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(VARIANT_TO_UNISTR(keyword), append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("keyword"), py::arg("append_to"), py::arg("pos"));

  sf.def(
        "parse_object",
        [](const SelectFormat &self, const _UnicodeStringVariant &source, Formattable &result,
           ParsePosition &parse_pos) { self.parseObject(VARIANT_TO_UNISTR(source), result, parse_pos); },
        py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          "parse_object",
          [](const Format &self, const _UnicodeStringVariant &source, Formattable &result) {
            ErrorCode error_code;
            self.parseObject(VARIANT_TO_UNISTR(source), result, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));

  sf.def("to_pattern", &SelectFormat::toPattern, py::arg("append_to"));
}

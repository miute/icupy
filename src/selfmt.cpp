#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/selfmt.h>

using namespace icu;

void init_selfmt(py::module &m) {
  //
  // icu::SelectFormat
  //
  py::class_<SelectFormat, Format> sf(m, "SelectFormat");

  sf.def(py::init([](const icupy::UnicodeStringVariant &pattern) {
           ErrorCode error_code;
           auto result = std::make_unique<SelectFormat>(icupy::to_unistr(pattern), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }),
         py::arg("pattern"))
      .def(py::init<const SelectFormat &>(), py::arg("other"));

  sf.def("__copy__", &SelectFormat::clone);

  sf.def(
      "___deepcopy__", [](const SelectFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  sf.def(
      "apply_pattern",
      [](SelectFormat &self, const icupy::UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.applyPattern(icupy::to_unistr(pattern), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("pattern"));

  sf.def("clone", &SelectFormat::clone);

  sf.def(
        // [1] icu::SelectFormat::format
        // [2] icu::Format::format
        "format",
        [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          // [3] icu::Format::format
          "format",
          [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          // [4] icu::Format::format
          "format",
          [](const SelectFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] icu::SelectFormat::format
          "format",
          [](const SelectFormat &self, const icupy::UnicodeStringVariant &keyword, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(icupy::to_unistr(keyword), append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("keyword"), py::arg("append_to"), py::arg("pos"));

  sf.def("to_pattern", &SelectFormat::toPattern, py::arg("append_to"));
}

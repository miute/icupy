#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/plurfmt.h>

using namespace icu;

void init_plurfmt(py::module &m) {
  //
  // icu::PluralFormat
  //
  py::class_<PluralFormat, Format> pf(m, "PluralFormat");

  pf.def(
        // [1] PluralFormat::PluralFormat
        py::init([]() {
          ErrorCode error_code;
          auto result = std::make_unique<PluralFormat>(error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        }))
      .def(
          // [2] PluralFormat::PluralFormat
          py::init([](const Locale &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(locale, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      .def(
          // [3] icu::PluralFormat
          py::init([](const PluralRules &rules) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(rules, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"))
      .def(
          // [4] icu::PluralFormat
          py::init([](const _LocaleVariant &locale, const PluralRules &rules) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(VARIANT_TO_LOCALE(locale), rules, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"))
      .def(
          // [5] icu::PluralFormat
          py::init([](const _LocaleVariant &locale, UPluralType type) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(VARIANT_TO_LOCALE(locale), type, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"))
      .def(
          // [6] icu::PluralFormat
          py::init([](const _UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(VARIANT_TO_UNISTR(pattern), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // [7] icu::PluralFormat
          py::init([](const _LocaleVariant &locale, const _UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result =
                std::make_unique<PluralFormat>(VARIANT_TO_LOCALE(locale), VARIANT_TO_UNISTR(pattern), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("pattern"))
      .def(
          // [8] icu::PluralFormat
          py::init([](const PluralRules &rules, const _UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(rules, VARIANT_TO_UNISTR(pattern), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("pattern"))
      .def(
          // [9] icu::PluralFormat
          py::init([](const _LocaleVariant &locale, const PluralRules &rules, const _UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result = std::make_unique<PluralFormat>(VARIANT_TO_LOCALE(locale), rules, VARIANT_TO_UNISTR(pattern),
                                                         error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"), py::arg("pattern"))
      .def(
          // [10] icu::PluralFormat
          py::init([](const _LocaleVariant &locale, UPluralType type, const _UnicodeStringVariant &pattern) {
            ErrorCode error_code;
            auto result =
                std::make_unique<PluralFormat>(VARIANT_TO_LOCALE(locale), type, VARIANT_TO_UNISTR(pattern), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"), py::arg("pattern"))
      .def(
          // [11] icu::PluralFormat
          py::init<const PluralFormat &>(), py::arg("other"));

  pf.def("__copy__", &PluralFormat::clone);

  pf.def(
      "___deepcopy__", [](const PluralFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  pf.def(
      "__eq__", [](const PluralFormat &self, const Format &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  pf.def(
      "__ne__", [](const PluralFormat &self, const Format &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  pf.def(
      "apply_pattern",
      [](PluralFormat &self, const _UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        self.applyPattern(VARIANT_TO_UNISTR(pattern), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("pattern"));

  pf.def("clone", &PluralFormat::clone);

  pf.def(
        // [1] PluralFormat::format
        // [2] Format::format
        "format",
        [](const PluralFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          [](const PluralFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          [](const PluralFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] PluralFormat::format
          "format",
          [](const PluralFormat &self, double number) {
            ErrorCode error_code;
            auto result = self.format(number, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert())
      .def(
          // [6] PluralFormat::format
          "format",
          [](const PluralFormat &self, double number, UnicodeString &append_to, FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos"))
      .def(
          // [7] PluralFormat::format
          "format",
          [](const PluralFormat &self, int32_t number) {
            ErrorCode error_code;
            auto result = self.format(number, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"))
      .def(
          // [8] PluralFormat::format
          "format",
          [](const PluralFormat &self, int32_t number, UnicodeString &append_to,
             FieldPosition &pos) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(number, append_to, pos, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos"));

  pf.def(
        "parse_object",
        [](const PluralFormat &self, const _UnicodeStringVariant &source, Formattable &result,
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

  pf.def(
      "set_number_format",
      [](PluralFormat &self, const NumberFormat *format) {
        ErrorCode error_code;
        self.setNumberFormat(format, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
      },
      py::arg("format_"));

  pf.def("to_pattern", &PluralFormat::toPattern, py::arg("append_to"));
}

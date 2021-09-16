#include "main.hpp"
#include <unicode/plurfmt.h>

using namespace icu;

void init_plurfmt(py::module &m) {
  // icu::PluralFormat
  py::class_<PluralFormat, Format> pf(m, "PluralFormat");
  pf.def(
        // [1] PluralFormat::PluralFormat
        py::init([]() {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = std::make_unique<PluralFormat>(error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        }))
      .def(
          // [2] PluralFormat::PluralFormat
          py::init([](const Locale &locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      /*
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
      */
      .def(
          // [3] icu::PluralFormat
          py::init([](const PluralRules &rules) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(rules, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"))
      .def(
          // [4] icu::PluralFormat
          py::init([](const Locale &locale, const PluralRules &rules) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, rules, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, const PluralRules &rules) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, rules, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"))
      .def(
          // [5] icu::PluralFormat
          py::init([](const Locale &locale, UPluralType type) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, type, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, UPluralType type) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, type, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"))
      .def(
          // [6] icu::PluralFormat
          py::init([](const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(pattern, error_code);
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
            auto result = std::make_unique<PluralFormat>(pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"))
      .def(
          // [7] icu::PluralFormat
          py::init([](const Locale &locale, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("pattern"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const Locale &locale, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("pattern"))
      .def(
          // const char *locale -> const Locale &locale
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char *locale, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("pattern"))
      .def(
          // [8] icu::PluralFormat
          py::init([](const PluralRules &rules, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(rules, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const PluralRules &rules, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(rules, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("rules"), py::arg("pattern"))
      .def(
          // [9] icu::PluralFormat
          py::init([](const Locale &locale, const PluralRules &rules, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, rules, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"), py::arg("pattern"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, const PluralRules &rules, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, rules, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const Locale &locale, const PluralRules &rules, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, rules, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"), py::arg("pattern"))
      .def(
          // const char *locale -> const Locale &locale
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char *locale, const PluralRules &rules, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, rules, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("rules"), py::arg("pattern"))
      .def(
          // [10] icu::PluralFormat
          py::init([](const Locale &locale, UPluralType type, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, type, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"), py::arg("pattern"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale, UPluralType type, const UnicodeString &pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, type, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"), py::arg("pattern"))
      .def(
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const Locale &locale, UPluralType type, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, type, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"), py::arg("pattern"))
      .def(
          // const char *locale -> const Locale &locale
          // const char16_t *pattern -> const UnicodeString &pattern
          py::init([](const char *locale, UPluralType type, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<PluralFormat>(locale, type, pattern, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("type_"), py::arg("pattern"))
      .def(
          // [11] icu::PluralFormat
          py::init<const PluralFormat &>(), py::arg("other"));
  pf.def("__copy__", &PluralFormat::clone)
      .def(
          "___deepcopy__", [](const PluralFormat &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const PluralFormat &self, const Format &other) { return self == other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__ne__", [](const PluralFormat &self, const Format &other) { return self != other; }, py::is_operator(),
          py::arg("other"));
  pf.def(
        "apply_pattern",
        [](PluralFormat &self, const UnicodeString &pattern) {
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
          [](PluralFormat &self, const char16_t *pattern) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.applyPattern(pattern, error_code);
            if (U_FAILURE(error_code)) {
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
          [](const PluralFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          [](const PluralFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(obj, append_to, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] PluralFormat::format
          "format",
          [](const PluralFormat &self, double number) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.format(number, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert())
      .def(
          // [6] PluralFormat::format
          "format",
          [](const PluralFormat &self, double number, UnicodeString &append_to, FieldPosition &pos) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(number, append_to, pos, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number").noconvert(), py::arg("append_to"), py::arg("pos"))
      .def(
          // [7] PluralFormat::format
          "format",
          [](const PluralFormat &self, int32_t number) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.format(number, error_code);
            if (U_FAILURE(error_code)) {
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
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.format(number, append_to, pos, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("number"), py::arg("append_to"), py::arg("pos"));
  pf.def("parse_object", &PluralFormat::parseObject, py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          "parse_object",
          [](const PluralFormat &self, const char16_t *source, Formattable &result, ParsePosition &parse_pos) {
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
  pf.def(
      "set_number_format",
      [](PluralFormat &self, const NumberFormat *format) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setNumberFormat(format, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("format_"));
  pf.def("to_pattern", &PluralFormat::toPattern, py::arg("append_to"));
}

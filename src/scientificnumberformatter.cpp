#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 55)
#include <unicode/decimfmt.h>
#include <unicode/errorcode.h>
#include <unicode/locid.h>
#include <unicode/scientificnumberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)

using namespace icu;

void init_scientificnumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  // icu::ScientificNumberFormatter
  py::class_<ScientificNumberFormatter, UObject> snf(m, "ScientificNumberFormatter");
  snf.def("__copy__", &ScientificNumberFormatter::clone)
      .def(
          "__deepcopy__", [](const ScientificNumberFormatter &self, py::dict) { return self.clone(); },
          py::arg("memo"));
  snf.def("clone", &ScientificNumberFormatter::clone);
  snf.def_static(
         // [1] ScientificNumberFormatter::createMarkupInstance
         "create_markup_instance",
         [](const Locale &locale, const UnicodeString &begin_markup, const UnicodeString &end_markup) {
           ErrorCode error_code;
           auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_markup_instance",
          [](const char *locale, const UnicodeString &begin_markup, const UnicodeString &end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char16_t *begin_markup -> const UnicodeString &begin_markup
          "create_markup_instance",
          [](const Locale &locale, const char16_t *begin_markup, const UnicodeString &end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char16_t *end_markup -> const UnicodeString &end_markup
          "create_markup_instance",
          [](const Locale &locale, const UnicodeString &begin_markup, const char16_t *end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char *locale -> const Locale &locale
          // const char16_t *begin_markup -> const UnicodeString &begin_markup
          "create_markup_instance",
          [](const char *locale, const char16_t *begin_markup, const UnicodeString &end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char *locale -> const Locale &locale
          // const char16_t *end_markup -> const UnicodeString &end_markup
          "create_markup_instance",
          [](const char *locale, const UnicodeString &begin_markup, const char16_t *end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char16_t *begin_markup -> const UnicodeString &begin_markup
          // const char16_t *end_markup -> const UnicodeString &end_markup
          "create_markup_instance",
          [](const Locale &locale, const char16_t *begin_markup, const char16_t *end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char *locale -> const Locale &locale
          // const char16_t *begin_markup -> const UnicodeString &begin_markup
          // const char16_t *end_markup -> const UnicodeString &end_markup
          "create_markup_instance",
          [](const char *locale, const char16_t *begin_markup, const char16_t *end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(locale, begin_markup, end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // [2] ScientificNumberFormatter::createMarkupInstance
          "create_markup_instance",
          [](DecimalFormat *fmt_to_adopt, const UnicodeString &begin_markup, const UnicodeString &end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(
                reinterpret_cast<DecimalFormat *>(fmt_to_adopt ? fmt_to_adopt->clone() : nullptr), begin_markup,
                end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char16_t *begin_markup -> const UnicodeString &begin_markup
          "create_markup_instance",
          [](DecimalFormat *fmt_to_adopt, const char16_t *begin_markup, const UnicodeString &end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(
                reinterpret_cast<DecimalFormat *>(fmt_to_adopt ? fmt_to_adopt->clone() : nullptr), begin_markup,
                end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char16_t *end_markup -> const UnicodeString &end_markup
          "create_markup_instance",
          [](DecimalFormat *fmt_to_adopt, const UnicodeString &begin_markup, const char16_t *end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(
                reinterpret_cast<DecimalFormat *>(fmt_to_adopt ? fmt_to_adopt->clone() : nullptr), begin_markup,
                end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // const char16_t *begin_markup -> const UnicodeString &begin_markup
          // const char16_t *end_markup -> const UnicodeString &end_markup
          "create_markup_instance",
          [](DecimalFormat *fmt_to_adopt, const char16_t *begin_markup, const char16_t *end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(
                reinterpret_cast<DecimalFormat *>(fmt_to_adopt ? fmt_to_adopt->clone() : nullptr), begin_markup,
                end_markup, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt"), py::arg("begin_markup"), py::arg("end_markup"));
  snf.def_static(
         // [1] ScientificNumberFormatter::createSuperscriptInstance
         "create_superscript_instance",
         [](const Locale &locale) {
           ErrorCode error_code;
           auto result = ScientificNumberFormatter::createSuperscriptInstance(locale, error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_superscript_instance",
          [](const char *locale) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createSuperscriptInstance(locale, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def_static(
          // [2] ScientificNumberFormatter::createSuperscriptInstance
          "create_superscript_instance",
          [](DecimalFormat *fmt_to_adopt) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createSuperscriptInstance(
                reinterpret_cast<DecimalFormat *>(fmt_to_adopt ? fmt_to_adopt->clone() : nullptr), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt"));
  snf.def(
      "format",
      [](const ScientificNumberFormatter &self, const Formattable &number,
         UnicodeString &append_to) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.format(number, append_to, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("number"), py::arg("append_to"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
}

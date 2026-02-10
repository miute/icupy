#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 55)
#include <pybind11/stl.h>
#include <unicode/decimfmt.h>
#include <unicode/scientificnumberformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)

using namespace icu;

void init_scientificnumberformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  //
  // class icu::ScientificNumberFormatter
  //
  py::class_<ScientificNumberFormatter, UObject> snf(
      m, "ScientificNumberFormatter");

  snf.def("__copy__", &ScientificNumberFormatter::clone);

  snf.def(
      "__deepcopy__",
      [](const ScientificNumberFormatter &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"));

  snf.def("clone", &ScientificNumberFormatter::clone);

  snf.def_static(
         // [1] ScientificNumberFormatter::createMarkupInstance
         "create_markup_instance",
         [](const icupy::LocaleVariant &locale,
            const icupy::UnicodeStringVariant &begin_markup,
            const icupy::UnicodeStringVariant &end_markup) {
           ErrorCode error_code;
           auto result = ScientificNumberFormatter::createMarkupInstance(
               icupy::to_locale(locale), icupy::to_unistr(begin_markup),
               icupy::to_unistr(end_markup), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"), py::arg("begin_markup"), py::arg("end_markup"))
      .def_static(
          // [2] ScientificNumberFormatter::createMarkupInstance
          "create_markup_instance",
          [](DecimalFormat *fmt_to_adopt,
             const icupy::UnicodeStringVariant &begin_markup,
             const icupy::UnicodeStringVariant &end_markup) {
            ErrorCode error_code;
            auto result = ScientificNumberFormatter::createMarkupInstance(
                fmt_to_adopt->clone(), icupy::to_unistr(begin_markup),
                icupy::to_unistr(end_markup), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt").none(false), py::arg("begin_markup"),
          py::arg("end_markup"));

  snf.def_static(
         // [1] ScientificNumberFormatter::createSuperscriptInstance
         "create_superscript_instance",
         [](const icupy::LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = ScientificNumberFormatter::createSuperscriptInstance(
               icupy::to_locale(locale), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
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
                fmt_to_adopt->clone(), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("fmt_to_adopt").none(false));

  snf.def(
      "format",
      [](const ScientificNumberFormatter &self, const Formattable &number,
         UnicodeString &append_to) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.format(number, append_to, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("number"), py::arg("append_to"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
}

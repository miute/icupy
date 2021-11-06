#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include <pybind11/stl.h>
#include <unicode/errorcode.h>
#include <unicode/numberrangeformatter.h>

using namespace icu;
using namespace icu::number;

using _LocalizedNumberRangeFormatterSettings = NumberRangeFormatterSettings<LocalizedNumberRangeFormatter>;
using _UnlocalizedNumberRangeFormatterSettings = NumberRangeFormatterSettings<UnlocalizedNumberRangeFormatter>;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_numberrangeformatter(py::module &, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  // icu::number::FormattedNumberRange
  py::class_<FormattedNumberRange, UMemory, FormattedValue> fnr(m2, "FormattedNumberRange");

  // icu::number::NumberRangeFormatterSettings<LocalizedNumberRangeFormatter>
  py::class_<_LocalizedNumberRangeFormatterSettings> nrfs_lnrf(m2, "_LocalizedNumberRangeFormatterSettings");

  // icu::number::LocalizedNumberRangeFormatter
  py::class_<LocalizedNumberRangeFormatter, _LocalizedNumberRangeFormatterSettings, UMemory> lnrf(
      m2, "LocalizedNumberRangeFormatter");

  // icu::number::NumberRangeFormatterSettings<UnlocalizedNumberRangeFormatter>
  py::class_<_UnlocalizedNumberRangeFormatterSettings> nrfs_unrf(m2, "_UnlocalizedNumberRangeFormatterSettings");

  // icu::number::UnlocalizedNumberRangeFormatter
  py::class_<UnlocalizedNumberRangeFormatter, _UnlocalizedNumberRangeFormatterSettings, UMemory> unrf(
      m2, "UnlocalizedNumberRangeFormatter");

  // icu::number::NumberRangeFormatter
  py::class_<NumberRangeFormatter> nrf(m2, "NumberRangeFormatter");

  // icu::number::FormattedNumberRange
  // Omit `icu::number::FormattedNumberRange::FormattedNumberRange()`.
  fnr.def(
      "append_to",
      [](const FormattedNumberRange &self, Appendable &appendable) -> Appendable & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.appendTo(appendable, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"));
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fnr.def("get_decimal_numbers", [](const FormattedNumberRange &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getDecimalNumbers<std::string>(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
  fnr.def("get_identity_result", [](const FormattedNumberRange &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getIdentityResult(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  fnr.def(
      "next_position",
      [](const FormattedNumberRange &self, ConstrainedFieldPosition &cfpos) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.nextPosition(cfpos, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("cfpos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  fnr.def("to_string", [](const FormattedNumberRange &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  fnr.def("to_temp_string", [](const FormattedNumberRange &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toTempString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  // icu::number::LocalizedNumberRangeFormatter
  lnrf.def(py::init<>()).def(py::init<const LocalizedNumberRangeFormatter &>(), py::arg("other"));
  lnrf.def(
      "format_formattable_range",
      [](const LocalizedNumberRangeFormatter &self, const Formattable &first, const Formattable &second) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.formatFormattableRange(first, second, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("first"), py::arg("second"));

  // icu::number::NumberRangeFormatter
  nrf.def_static("with_", &NumberRangeFormatter::with);
  nrf.def_static("with_locale", &NumberRangeFormatter::withLocale, py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "with_locale", [](const char *locale) { return NumberRangeFormatter::withLocale(locale); },
          py::arg("locale"));

  // icu::number::NumberRangeFormatterSettings<LocalizedNumberRangeFormatter>
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nrfs_lnrf
      .def("__copy__",
           [](const _LocalizedNumberRangeFormatterSettings &self) {
             return std::unique_ptr<LocalizedNumberRangeFormatter>(self.clone());
           })
      .def(
          "__deepcopy__",
          [](const _LocalizedNumberRangeFormatterSettings &self, py::dict) {
            return std::unique_ptr<LocalizedNumberRangeFormatter>(self.clone());
          },
          py::arg("memo"));
  nrfs_lnrf.def("clone", [](const _LocalizedNumberRangeFormatterSettings &self) {
    return std::unique_ptr<LocalizedNumberRangeFormatter>(self.clone());
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  nrfs_lnrf.def(
      "collapse",
      [](const _LocalizedNumberRangeFormatterSettings &self, UNumberRangeCollapse collapse) {
        return self.collapse(collapse);
      },
      py::arg("collapse"));
  nrfs_lnrf.def(
      "copy_error_to",
      [](const _LocalizedNumberRangeFormatterSettings &self, ErrorCode &out_error_code) {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"));
  nrfs_lnrf.def(
      "identity_fallback",
      [](const _LocalizedNumberRangeFormatterSettings &self, UNumberRangeIdentityFallback identity_fallback) {
        return self.identityFallback(identity_fallback);
      },
      py::arg("identity_fallback"));
  nrfs_lnrf.def(
      "number_formatter_both",
      [](const _LocalizedNumberRangeFormatterSettings &self, const UnlocalizedNumberFormatter &formatter) {
        return self.numberFormatterBoth(formatter);
      },
      py::arg("formatter"));
  nrfs_lnrf.def(
      "number_formatter_first",
      [](const _LocalizedNumberRangeFormatterSettings &self, const UnlocalizedNumberFormatter &formatter_first) {
        return self.numberFormatterFirst(formatter_first);
      },
      py::arg("formatter_first"));
  nrfs_lnrf.def(
      "number_formatter_second",
      [](const _LocalizedNumberRangeFormatterSettings &self, const UnlocalizedNumberFormatter &formatter_second) {
        return self.numberFormatterSecond(formatter_second);
      },
      py::arg("formatter_second"));

  // icu::number::NumberRangeFormatterSettings<UnlocalizedNumberRangeFormatter>
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nrfs_unrf
      .def("__copy__",
           [](const _UnlocalizedNumberRangeFormatterSettings &self) {
             return std::unique_ptr<UnlocalizedNumberRangeFormatter>(self.clone());
           })
      .def(
          "__deepcopy__",
          [](const _UnlocalizedNumberRangeFormatterSettings &self, py::dict) {
            return std::unique_ptr<UnlocalizedNumberRangeFormatter>(self.clone());
          },
          py::arg("memo"));
  nrfs_unrf.def("clone", [](const _UnlocalizedNumberRangeFormatterSettings &self) {
    return std::unique_ptr<UnlocalizedNumberRangeFormatter>(self.clone());
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  nrfs_unrf.def(
      "collapse",
      [](const _UnlocalizedNumberRangeFormatterSettings &self, UNumberRangeCollapse collapse) {
        return self.collapse(collapse);
      },
      py::arg("collapse"));
  nrfs_unrf.def(
      "copy_error_to",
      [](const _UnlocalizedNumberRangeFormatterSettings &self, ErrorCode &out_error_code) {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"));
  nrfs_unrf.def(
      "identity_fallback",
      [](const _UnlocalizedNumberRangeFormatterSettings &self, UNumberRangeIdentityFallback identity_fallback) {
        return self.identityFallback(identity_fallback);
      },
      py::arg("identity_fallback"));
  nrfs_unrf.def(
      "number_formatter_both",
      [](const _UnlocalizedNumberRangeFormatterSettings &self, const UnlocalizedNumberFormatter &formatter) {
        return self.numberFormatterBoth(formatter);
      },
      py::arg("formatter"));
  nrfs_unrf.def(
      "number_formatter_first",
      [](const _UnlocalizedNumberRangeFormatterSettings &self, const UnlocalizedNumberFormatter &formatter_first) {
        return self.numberFormatterFirst(formatter_first);
      },
      py::arg("formatter_first"));
  nrfs_unrf.def(
      "number_formatter_second",
      [](const _UnlocalizedNumberRangeFormatterSettings &self, const UnlocalizedNumberFormatter &formatter_second) {
        return self.numberFormatterSecond(formatter_second);
      },
      py::arg("formatter_second"));

  // icu::number::UnlocalizedNumberRangeFormatter
  unrf.def(py::init<>()).def(py::init<const UnlocalizedNumberRangeFormatter &>(), py::arg("other"));
  unrf.def(
          "locale",
          [](const UnlocalizedNumberRangeFormatter &self, const Locale &locale) { return self.locale(locale); },
          py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "locale", [](const UnlocalizedNumberRangeFormatter &self, const char *locale) { return self.locale(locale); },
          py::arg("locale"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

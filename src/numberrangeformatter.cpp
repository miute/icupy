#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include <pybind11/stl.h>
#include <unicode/numberrangeformatter.h>

using namespace icu;
using namespace icu::number;

using _LocalizedNumberRangeFormatterSettings =
    NumberRangeFormatterSettings<LocalizedNumberRangeFormatter>;
using _UnlocalizedNumberRangeFormatterSettings =
    NumberRangeFormatterSettings<UnlocalizedNumberRangeFormatter>;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_numberrangeformatter(py::module &, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  //
  // class icu::number::FormattedNumberRange
  //
  py::class_<FormattedNumberRange, UMemory, FormattedValue> fnr(
      m2, "FormattedNumberRange", R"doc(
      Result of a number range formatting operation.

      See Also:
          :class:`LocalizedNumberRangeFormatter`
      )doc");

  //);

  //
  // class
  // icu::number::NumberRangeFormatterSettings<LocalizedNumberRangeFormatter>
  //
  py::class_<_LocalizedNumberRangeFormatterSettings> nrfs_lnrf(
      m2, "_LocalizedNumberRangeFormatterSettings", R"doc(
      Abstract base class for specifying settings related to number range
      formatting.

      See Also:
          :class:`LocalizedNumberRangeFormatter`
      )doc");

  //
  // class icu::number::LocalizedNumberRangeFormatter
  //
  py::class_<LocalizedNumberRangeFormatter,
             _LocalizedNumberRangeFormatterSettings, UMemory>
      lnrf(m2, "LocalizedNumberRangeFormatter",
           R"doc(
      :class:`NumberRangeFormatter` that has a locale associated with it.

      See Also:
          :class:`NumberRangeFormatter`
          :class:`UnlocalizedNumberRangeFormatter`
      )doc");

  //
  // class
  // icu::number::NumberRangeFormatterSettings<UnlocalizedNumberRangeFormatter>
  //
  py::class_<_UnlocalizedNumberRangeFormatterSettings> nrfs_unrf(
      m2, "_UnlocalizedNumberRangeFormatterSettings", R"doc(
      Abstract base class for specifying settings related to number range
      formatting.

      See Also:
          :class:`UnlocalizedNumberRangeFormatter`
      )doc");

  //
  // class icu::number::UnlocalizedNumberRangeFormatter
  //
  py::class_<UnlocalizedNumberRangeFormatter,
             _UnlocalizedNumberRangeFormatterSettings, UMemory>
      unrf(m2, "UnlocalizedNumberRangeFormatter",
           R"doc(
      :class:`NumberRangeFormatter` that does not yet have a locale.

      In order to format, a locale must be specified.

      See Also:
          :class:`NumberRangeFormatter`
          :class:`LocalizedNumberRangeFormatter`
      )doc");

  //
  // class icu::number::NumberRangeFormatter
  //
  py::class_<NumberRangeFormatter> nrf(m2, "NumberRangeFormatter", R"doc(
      Formatter for localized number, currency, and unit ranges.

      See Also:
          :class:`NumberFormatter`

      Examples:
          .. code-block:: python

              from icupy import icu
              from icupy.icu import number
              first = number.NumberFormatter.with_().unit(icu.MeasureUnit.create_meter())
              second = number.NumberFormatter.with_().unit(icu.MeasureUnit.create_kilometer())
              fmt = (
                  number.NumberRangeFormatter.with_()
                  .identity_fallback(icu.UNUM_IDENTITY_FALLBACK_APPROXIMATELY_OR_SINGLE_VALUE)
                  .number_formatter_first(first)
                  .number_formatter_second(second)
                  .locale("en-GB")
              )
              fmtval = fmt.format_formattable_range(icu.Formattable(750), icu.Formattable(1.2))
              fmtval.to_string()  # "750 m - 1.2 km"
      )doc");

  //
  // class icu::number::FormattedNumberRange
  //
  fnr.def(py::init<>(), R"doc(
      Initialize a ``FormattedNumberRange`` instance as an empty
      FormattedNumberRange.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM < 64)
  fnr.def(
      "append_to",
      [](const FormattedNumberRange &self,
         Appendable &appendable) -> Appendable & {
        ErrorCode error_code;
        auto &result = self.appendTo(appendable, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"), R"doc(
      Append the formatted string to *appendable*.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM < 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fnr.def(
      "get_decimal_numbers",
      [](const FormattedNumberRange &self) {
        ErrorCode error_code;
        auto result = self.getDecimalNumbers<std::string>(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the formatted range as a pair of numeric strings
      ``(first, second)``.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  fnr.def(
      "get_identity_result",
      [](const FormattedNumberRange &self) {
        ErrorCode error_code;
        auto result = self.getIdentityResult(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return whether the pair of numbers was successfully formatted as a range
      or whether an identity fallback was used.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM < 64)
  fnr.def(
      "to_string",
      [](const FormattedNumberRange &self) {
        ErrorCode error_code;
        auto result = self.toString(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the formatted string.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM < 64)

  //
  // class icu::number::LocalizedNumberRangeFormatter
  //
  lnrf.def(py::init<>(), R"doc(
      Initialize a ``LocalizedNumberRangeFormatter`` instance in a valid but
      undefined state.
      )doc")
      .def(py::init<const LocalizedNumberRangeFormatter &>(), py::arg("other"),
           R"doc(
      Initialize a ``LocalizedNumberRangeFormatter`` instance from another
      ``LocalizedNumberRangeFormatter``.
      )doc");

  lnrf.def(
      "format_formattable_range",
      [](const LocalizedNumberRangeFormatter &self, const Formattable &first,
         const Formattable &second) {
        ErrorCode error_code;
        auto result = self.formatFormattableRange(first, second, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("first"), py::arg("second"), R"doc(
      Format the given Formattables to a string using the settings specified in
      the :class:`NumberRangeFormatter` fluent setting chain.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 75)
  lnrf.def(
      "without_locale",
      [](const LocalizedNumberRangeFormatter &self) {
        return self.withoutLocale();
      },
      R"doc(
      Disassociate the locale from this formatter.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 75)

  //
  // class icu::number::NumberRangeFormatter
  //
  nrf.def_static("with_", &NumberRangeFormatter::with, R"doc(
      Create a new :class:`UnlocalizedNumberRangeFormatter` object without
      locale.
      )doc");

  nrf.def_static(
      "with_locale",
      [](const icupy::LocaleVariant &locale) {
        return NumberRangeFormatter::withLocale(icupy::to_locale(locale));
      },
      py::arg("locale"), R"doc(
      Create a new :class:`LocalizedNumberRangeFormatter` object associated
      with the specified *locale*.
      )doc");

  //
  // class
  // icu::number::NumberRangeFormatterSettings<LocalizedNumberRangeFormatter>
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nrfs_lnrf.def(
      "__copy__",
      [](const _LocalizedNumberRangeFormatterSettings &self) {
        return std::unique_ptr<LocalizedNumberRangeFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nrfs_lnrf.def(
      "__deepcopy__",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         py::dict & /* memo */) {
        return std::unique_ptr<LocalizedNumberRangeFormatter>(self.clone());
      },
      py::arg("memo"), R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nrfs_lnrf.def(
      "clone",
      [](const _LocalizedNumberRangeFormatterSettings &self) {
        return std::unique_ptr<LocalizedNumberRangeFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  nrfs_lnrf.def(
      "collapse",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         UNumberRangeCollapse collapse) { return self.collapse(collapse); },
      py::arg("collapse"), R"doc(
      Set the aggressiveness of "collapsing" fields across the range separator.

      Possible values:

      - ALL: "3-5K miles"
      - UNIT: "3K - 5K miles"
      - NONE: "3K miles - 5K miles"
      - AUTO: usually UNIT or NONE, depending on the locale and formatter
        settings

      The default is AUTO.
      )doc");

  nrfs_lnrf.def(
      "copy_error_to",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Set the :class:`~icupy.icu.UErrorCode` if an error occurred in the
      fluent chain.

      Preserves older error codes in the *out_error_code*.
      )doc");

  nrfs_lnrf.def(
      "identity_fallback",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         UNumberRangeIdentityFallback identity_fallback) {
        return self.identityFallback(identity_fallback);
      },
      py::arg("identity_fallback"), R"doc(
      Set the behavior when the two sides of the range are the same.

      This could happen if the same two numbers are passed to the
      :meth:`LocalizedNumberRangeFormatter.format_formattable_range` function,
      or if different numbers are passed to the function but they become the
      same after rounding rules are applied.

      Possible values:

      - SINGLE_VALUE: "5 miles"
      - APPROXIMATELY_OR_SINGLE_VALUE: "~5 miles" or "5 miles", depending on
        whether the number was the same before rounding was applied
      - APPROXIMATELY: "~5 miles"
      - RANGE: "5-5 miles" (with collapse=UNIT)

      The default is APPROXIMATELY.
      )doc");

  nrfs_lnrf.def(
      "number_formatter_both",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         const UnlocalizedNumberFormatter &formatter) {
        return self.numberFormatterBoth(formatter);
      },
      py::arg("formatter"), R"doc(
      Set the :class:`NumberFormatter` instance to use for the numbers in the
      range.

      The same formatter is applied to both sides of the range.

      The :class:`NumberFormatter` instances must not have a locale applied
      yet; the locale specified on the :class:`NumberRangeFormatter` will be
      used.
      )doc");

  nrfs_lnrf.def(
      "number_formatter_first",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         const UnlocalizedNumberFormatter &formatter_first) {
        return self.numberFormatterFirst(formatter_first);
      },
      py::arg("formatter_first"), R"doc(
      Set the :class:`NumberFormatter` instance to use for the first number in
      the range.

      The :class:`NumberFormatter` instances must not have a locale applied
      yet; the locale specified on the :class:`NumberRangeFormatter` will be
      used.
      )doc");

  nrfs_lnrf.def(
      "number_formatter_second",
      [](const _LocalizedNumberRangeFormatterSettings &self,
         const UnlocalizedNumberFormatter &formatter_second) {
        return self.numberFormatterSecond(formatter_second);
      },
      py::arg("formatter_second"), R"doc(
      Set the :class:`NumberFormatter` instance to use for the second number
      in the range.

      The :class:`NumberFormatter` instances must not have a locale applied
      yet; the locale specified on the :class:`NumberRangeFormatter` will be
      used.
      )doc");

  //
  // class
  // icu::number::NumberRangeFormatterSettings<UnlocalizedNumberRangeFormatter>
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nrfs_unrf.def(
      "__copy__",
      [](const _UnlocalizedNumberRangeFormatterSettings &self) {
        return std::unique_ptr<UnlocalizedNumberRangeFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nrfs_unrf.def(
      "__deepcopy__",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         py::dict & /* memo */) {
        return std::unique_ptr<UnlocalizedNumberRangeFormatter>(self.clone());
      },
      py::arg("memo"),
      R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  nrfs_unrf.def(
      "clone",
      [](const _UnlocalizedNumberRangeFormatterSettings &self) {
        return std::unique_ptr<UnlocalizedNumberRangeFormatter>(self.clone());
      },
      R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  nrfs_unrf.def(
      "collapse",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         UNumberRangeCollapse collapse) { return self.collapse(collapse); },
      py::arg("collapse"), R"doc(
      Set the aggressiveness of "collapsing" fields across the range separator.

      Possible values:

      - ALL: "3-5K miles"
      - UNIT: "3K - 5K miles"
      - NONE: "3K miles - 5K miles"
      - AUTO: usually UNIT or NONE, depending on the locale and formatter
        settings

      The default is AUTO.
      )doc");

  nrfs_unrf.def(
      "copy_error_to",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Set the :class:`~icupy.icu.UErrorCode` if an error occurred in the
      fluent chain.

      Preserves older error codes in the *out_error_code*.
      )doc");

  nrfs_unrf.def(
      "identity_fallback",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         UNumberRangeIdentityFallback identity_fallback) {
        return self.identityFallback(identity_fallback);
      },
      py::arg("identity_fallback"), R"doc(
      Set the behavior when the two sides of the range are the same.

      This could happen if the same two numbers are passed to the
      :meth:`LocalizedNumberRangeFormatter.format_formattable_range` function,
      or if different numbers are passed to the function but they become the
      same after rounding rules are applied.

      Possible values:

      - SINGLE_VALUE: "5 miles"
      - APPROXIMATELY_OR_SINGLE_VALUE: "~5 miles" or "5 miles", depending on
        whether the number was the same before rounding was applied
      - APPROXIMATELY: "~5 miles"
      - RANGE: "5-5 miles" (with collapse=UNIT)

      The default is APPROXIMATELY.
      )doc");

  nrfs_unrf.def(
      "number_formatter_both",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         const UnlocalizedNumberFormatter &formatter) {
        return self.numberFormatterBoth(formatter);
      },
      py::arg("formatter"), R"doc(
      Set the :class:`NumberFormatter` instance to use for the numbers in the
      range.

      The same formatter is applied to both sides of the range.

      The :class:`NumberFormatter` instances must not have a locale applied
      yet; the locale specified on the :class:`NumberRangeFormatter` will be
      used.
      )doc");

  nrfs_unrf.def(
      "number_formatter_first",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         const UnlocalizedNumberFormatter &formatter_first) {
        return self.numberFormatterFirst(formatter_first);
      },
      py::arg("formatter_first"), R"doc(
      Set the :class:`NumberFormatter` instance to use for the first number in
      the range.

      The :class:`NumberFormatter` instances must not have a locale applied
      yet; the locale specified on the :class:`NumberRangeFormatter` will be
      used.
      )doc");

  nrfs_unrf.def(
      "number_formatter_second",
      [](const _UnlocalizedNumberRangeFormatterSettings &self,
         const UnlocalizedNumberFormatter &formatter_second) {
        return self.numberFormatterSecond(formatter_second);
      },
      py::arg("formatter_second"), R"doc(
      Set the :class:`NumberFormatter` instance to use for the second number
      in the range.

      The :class:`NumberFormatter` instances must not have a locale applied
      yet; the locale specified on the :class:`NumberRangeFormatter` will be
      used.
      )doc");

  //
  // class icu::number::UnlocalizedNumberRangeFormatter
  //
  unrf.def(py::init<>(), R"doc(
      Initialize a ``UnlocalizedNumberRangeFormatter`` instance in a valid but
      undefined state.
      )doc")
      .def(py::init<const UnlocalizedNumberRangeFormatter &>(),
           py::arg("other"),
           R"doc(
      Initialize a ``UnlocalizedNumberRangeFormatter`` instance from another
      ``UnlocalizedNumberRangeFormatter``.
      )doc");

  unrf.def(
      "locale",
      [](const UnlocalizedNumberRangeFormatter &self,
         const icupy::LocaleVariant &locale) {
        return self.locale(icupy::to_locale(locale));
      },
      py::arg("locale"), R"doc(
      Associate the given locale with the number range formatter.

      The locale is used for picking the appropriate symbols, formats, and
      other data for number display.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

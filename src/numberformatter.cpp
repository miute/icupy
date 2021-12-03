#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
#include <unicode/errorcode.h>
#include <unicode/numberformatter.h>

using namespace icu;
using namespace icu::number;

using _LocalizedNumberFormatterSettings = NumberFormatterSettings<LocalizedNumberFormatter>;
using _UnlocalizedNumberFormatterSettings = NumberFormatterSettings<UnlocalizedNumberFormatter>;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

void init_numberformatter(py::module &, py::module &m2) {
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  // icu::number::NumberFormatter
  py::class_<NumberFormatter> nf(m2, "NumberFormatter");

  // icu::number::FormattedNumber
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  py::class_<FormattedNumber, UMemory, FormattedValue> fn(m2, "FormattedNumber");
#else
  py::class_<FormattedNumber, UMemory> fn(m2, "FormattedNumber");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

  // icu::number::IntegerWidth
  py::class_<IntegerWidth, UMemory> iw(m2, "IntegerWidth");

  // icu::number::Notation
  py::class_<Notation, UMemory> no(m2, "Notation");

  // icu::number::ScientificNotation
  py::class_<ScientificNotation, Notation> sn(m2, "ScientificNotation");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::Precision
  py::class_<Precision, UMemory> pre(m2, "Precision");

  // icu::number::CurrencyPrecision
  py::class_<CurrencyPrecision, Precision> cp(m2, "CurrencyPrecision");

  // icu::number::FractionPrecision
  py::class_<FractionPrecision, Precision> fp(m2, "FractionPrecision");

  // icu::number::IncrementPrecision
  py::class_<IncrementPrecision, Precision> ip(m2, "IncrementPrecision");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::Scale
  py::class_<Scale, UMemory> sc(m2, "Scale");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  // icu::number::NumberFormatterSettings<LocalizedNumberFormatter>
  py::class_<_LocalizedNumberFormatterSettings> nfs_lnf(m2, "_LocalizedNumberFormatterSettings");

  // icu::number::NumberFormatterSettings<UnlocalizedNumberFormatter>
  py::class_<_UnlocalizedNumberFormatterSettings> nfs_unf(m2, "_UnlocalizedNumberFormatterSettings");

  // icu::number::LocalizedNumberFormatter
  py::class_<LocalizedNumberFormatter, _LocalizedNumberFormatterSettings, UMemory> lnf(m2, "LocalizedNumberFormatter");

  // icu::number::UnlocalizedNumberFormatter
  py::class_<UnlocalizedNumberFormatter, _UnlocalizedNumberFormatterSettings, UMemory> unf(
      m2, "UnlocalizedNumberFormatter");

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::CurrencyPrecision
  cp.def("with_currency", &CurrencyPrecision::withCurrency, py::arg("currency"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  // icu::number::FormattedNumber
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  fn.def(
      "append_to",
      [](const FormattedNumber &self, Appendable &appendable) -> Appendable & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.appendTo(appendable, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  fn.def("get_output_unit", [](const FormattedNumber &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getOutputUnit(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  fn.def(
      "next_position",
      [](const FormattedNumber &self, ConstrainedFieldPosition &cfpos) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.nextPosition(cfpos, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("cfpos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  fn.def("to_decimal_number", [](const FormattedNumber &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toDecimalNumber<std::string>(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  fn.def("to_string", [](const FormattedNumber &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  fn.def("to_temp_string", [](const FormattedNumber &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toTempString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::FractionPrecision
  fp.def("with_max_digits", &FractionPrecision::withMaxDigits, py::arg("max_significant_digits"));
  fp.def("with_min_digits", &FractionPrecision::withMinDigits, py::arg("min_significant_digits"));
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  fp.def("with_significant_digits", &FractionPrecision::withSignificantDigits, py::arg("min_significant_digits"),
         py::arg("max_significant_digits"), py::arg("priority"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::IncrementPrecision
  ip.def("with_min_fraction", &IncrementPrecision::withMinFraction, py::arg("min_frac"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  // icu::number::IntegerWidth
  iw.def("truncate_at", &IntegerWidth::truncateAt, py::arg("max_int"));
  iw.def_static("zero_fill_to", &IntegerWidth::zeroFillTo, py::arg("min_int"));

  // icu::number::LocalizedNumberFormatter
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  lnf.def(py::init<>());
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  lnf.def(py::init<const LocalizedNumberFormatter &>(), py::arg("other"));
  lnf.def(
      "format_decimal",
      [](const LocalizedNumberFormatter &self, const char *value) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.formatDecimal(value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("value"));
  lnf.def(
      "format_double",
      [](const LocalizedNumberFormatter &self, double value) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.formatDouble(value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("value"));
  lnf.def(
      "format_int",
      [](const LocalizedNumberFormatter &self, int64_t value) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.formatInt(value, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("value"));
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  lnf.def("to_format", [](const LocalizedNumberFormatter &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toFormat(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  // icu::number::NumberFormatter
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nf.def_static(
        "for_skeleton",
        [](const UnicodeString &skeleton) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberFormatter::forSkeleton(skeleton, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("skeleton"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "for_skeleton",
          [](const char16_t *skeleton) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormatter::forSkeleton(skeleton, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nf.def_static(
        "for_skeleton",
        [](const UnicodeString &skeleton, UParseError &perror) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = NumberFormatter::forSkeleton(skeleton, perror, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("skeleton"), py::arg("perror"))
      .def_static(
          // const char16_t *skeleton -> const UnicodeString &skeleton
          "for_skeleton",
          [](const char16_t *skeleton, UParseError &perror) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = NumberFormatter::forSkeleton(skeleton, perror, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("skeleton"), py::arg("perror"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  nf.def_static("with_", &NumberFormatter::with);
  nf.def_static("with_locale", &NumberFormatter::withLocale, py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "with_locale", [](const char *locale) { return NumberFormatter::withLocale(locale); }, py::arg("locale"));

  // icu::number::NumberFormatterSettings<LocalizedNumberFormatter>
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nfs_lnf
      .def("__copy__",
           [](const _LocalizedNumberFormatterSettings &self) {
             return std::unique_ptr<LocalizedNumberFormatter>(self.clone());
           })
      .def(
          "__deepcopy__",
          [](const _LocalizedNumberFormatterSettings &self, py::dict) {
            return std::unique_ptr<LocalizedNumberFormatter>(self.clone());
          },
          py::arg("memo"));
  nfs_lnf.def("clone", [](const _LocalizedNumberFormatterSettings &self) {
    return std::unique_ptr<LocalizedNumberFormatter>(self.clone());
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  nfs_lnf.def(
      "copy_error_to",
      [](const _LocalizedNumberFormatterSettings &self, ErrorCode &out_error_code) {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"));
  nfs_lnf.def(
      "decimal",
      [](const _LocalizedNumberFormatterSettings &self, UNumberDecimalSeparatorDisplay style) {
        return self.decimal(style);
      },
      py::arg("style"));
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_lnf.def(
      "grouping",
      [](const _LocalizedNumberFormatterSettings &self, UNumberGroupingStrategy strategy) {
        return self.grouping(strategy);
      },
      py::arg("strategy"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_lnf.def(
      "integer_width",
      [](const _LocalizedNumberFormatterSettings &self, const IntegerWidth &style) { return self.integerWidth(style); },
      py::arg("style"));
  nfs_lnf.def(
      "notation",
      [](const _LocalizedNumberFormatterSettings &self, const Notation &notation) { return self.notation(notation); },
      py::arg("notation"));
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_lnf.def(
      "per_unit",
      [](const _LocalizedNumberFormatterSettings &self, const MeasureUnit &per_unit) { return self.perUnit(per_unit); },
      py::arg("per_unit"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_lnf.def(
      "precision",
      [](const _LocalizedNumberFormatterSettings &self, const Precision &precision) {
        return self.precision(precision);
      },
      py::arg("precision"));
  nfs_lnf.def(
      "rounding_mode",
      [](const _LocalizedNumberFormatterSettings &self, UNumberFormatRoundingMode rounding_mode) {
        return self.roundingMode(rounding_mode);
      },
      py::arg("rounding_mode"));
  nfs_lnf.def(
      "scale", [](const _LocalizedNumberFormatterSettings &self, const Scale &scale) { return self.scale(scale); },
      py::arg("scale"));
  nfs_lnf.def(
      "sign", [](const _LocalizedNumberFormatterSettings &self, UNumberSignDisplay style) { return self.sign(style); },
      py::arg("style"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_lnf.def(
      "symbols",
      [](const _LocalizedNumberFormatterSettings &self, const DecimalFormatSymbols &symbols) {
        return self.symbols(symbols);
      },
      py::arg("symbols"));
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_lnf.def("to_skeleton", [](const _LocalizedNumberFormatterSettings &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toSkeleton(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_lnf.def(
      "unit", [](const _LocalizedNumberFormatterSettings &self, const MeasureUnit &unit) { return self.unit(unit); },
      py::arg("unit"));
  nfs_lnf.def(
      "unit_width",
      [](const _LocalizedNumberFormatterSettings &self, UNumberUnitWidth width) { return self.unitWidth(width); },
      py::arg("width"));
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  nfs_lnf.def(
      "usage", [](const _LocalizedNumberFormatterSettings &self, const char *usage) { return self.usage(usage); },
      py::arg("usage"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  // icu::number::NumberFormatterSettings<UnlocalizedNumberFormatter>
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  nfs_unf
      .def("__copy__",
           [](const _UnlocalizedNumberFormatterSettings &self) {
             return std::unique_ptr<UnlocalizedNumberFormatter>(self.clone());
           })
      .def(
          "__deepcopy__",
          [](const _UnlocalizedNumberFormatterSettings &self, py::dict) {
            return std::unique_ptr<UnlocalizedNumberFormatter>(self.clone());
          },
          py::arg("memo"));
  nfs_unf.def("clone", [](const _UnlocalizedNumberFormatterSettings &self) {
    return std::unique_ptr<UnlocalizedNumberFormatter>(self.clone());
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
  nfs_unf.def(
      "copy_error_to",
      [](const _UnlocalizedNumberFormatterSettings &self, ErrorCode &out_error_code) {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"));
  nfs_unf.def(
      "decimal",
      [](const _UnlocalizedNumberFormatterSettings &self, UNumberDecimalSeparatorDisplay style) {
        return self.decimal(style);
      },
      py::arg("style"));
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_unf.def(
      "grouping",
      [](const _UnlocalizedNumberFormatterSettings &self, UNumberGroupingStrategy strategy) {
        return self.grouping(strategy);
      },
      py::arg("strategy"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_unf.def(
      "integer_width",
      [](const _UnlocalizedNumberFormatterSettings &self, const IntegerWidth &style) {
        return self.integerWidth(style);
      },
      py::arg("style"));
  nfs_unf.def(
      "notation",
      [](const _UnlocalizedNumberFormatterSettings &self, const Notation &notation) { return self.notation(notation); },
      py::arg("notation"));
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
  nfs_unf.def(
      "per_unit",
      [](const _UnlocalizedNumberFormatterSettings &self, const MeasureUnit &per_unit) {
        return self.perUnit(per_unit);
      },
      py::arg("per_unit"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_unf.def(
      "precision",
      [](const _UnlocalizedNumberFormatterSettings &self, const Precision &precision) {
        return self.precision(precision);
      },
      py::arg("precision"));
  nfs_unf.def(
      "rounding_mode",
      [](const _UnlocalizedNumberFormatterSettings &self, UNumberFormatRoundingMode rounding_mode) {
        return self.roundingMode(rounding_mode);
      },
      py::arg("rounding_mode"));
  nfs_unf.def(
      "scale", [](const _UnlocalizedNumberFormatterSettings &self, const Scale &scale) { return self.scale(scale); },
      py::arg("scale"));
  nfs_unf.def(
      "sign",
      [](const _UnlocalizedNumberFormatterSettings &self, UNumberSignDisplay style) { return self.sign(style); },
      py::arg("style"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_unf.def(
      "symbols",
      [](const _UnlocalizedNumberFormatterSettings &self, const DecimalFormatSymbols &symbols) {
        return self.symbols(symbols);
      },
      py::arg("symbols"));
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_unf.def("to_skeleton", [](const _UnlocalizedNumberFormatterSettings &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toSkeleton(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  nfs_unf.def(
      "unit", [](const _UnlocalizedNumberFormatterSettings &self, const MeasureUnit &unit) { return self.unit(unit); },
      py::arg("unit"));
  nfs_unf.def(
      "unit_width",
      [](const _UnlocalizedNumberFormatterSettings &self, UNumberUnitWidth width) { return self.unitWidth(width); },
      py::arg("width"));
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
  nfs_unf.def(
      "usage", [](const _UnlocalizedNumberFormatterSettings &self, const char *usage) { return self.usage(usage); },
      py::arg("usage"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)

  // icu::number::Notation
  no.def_static("compact_long", &Notation::compactLong);
  no.def_static("compact_short", &Notation::compactShort);
  no.def_static("engineering", &Notation::engineering);
  no.def_static("scientific", &Notation::scientific);
  no.def_static("simple", &Notation::simple);

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::Precision
  pre.def_static("currency", &Precision::currency, py::arg("currency_usage"));
  pre.def_static("fixed_fraction", &Precision::fixedFraction, py::arg("min_max_fraction_places"));
  pre.def_static("fixed_significant_digits", &Precision::fixedSignificantDigits, py::arg("min_max_significant_digits"));
  pre.def_static("increment", &Precision::increment, py::arg("rounding_increment"));
  pre.def_static("integer", &Precision::integer);
  pre.def_static("max_fraction", &Precision::maxFraction, py::arg("max_fraction_places"));
  pre.def_static("max_significant_digits", &Precision::maxSignificantDigits, py::arg("max_significant_digits"));
  pre.def_static("min_fraction", &Precision::minFraction, py::arg("min_fraction_places"));
  pre.def_static("min_max_fraction", &Precision::minMaxFraction, py::arg("min_fraction_places"),
                 py::arg("max_fraction_places"));
  pre.def_static("min_max_significant_digits", &Precision::minMaxSignificantDigits, py::arg("min_significant_digits"),
                 py::arg("max_significant_digits"));
  pre.def_static("min_significant_digits", &Precision::minSignificantDigits, py::arg("min_significant_digits"));

#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  pre.def("trailing_zero_display", &Precision::trailingZeroDisplay, py::arg("trailing_zero_display"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)

  pre.def_static("unlimited", &Precision::unlimited);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  // icu::number::Scale
  sc.def(py::init<const Scale &>(), py::arg("other"));
  sc.def_static(
      "by_decimal", [](const char *multiplicand) { return Scale::byDecimal(multiplicand); }, py::arg("multiplicand"));
  sc.def_static("by_double", &Scale::byDouble, py::arg("multiplicand"));
  sc.def_static("by_double_and_power_of_ten", &Scale::byDoubleAndPowerOfTen, py::arg("multiplicand"), py::arg("power"));
  sc.def_static("none", &Scale::none);
  sc.def_static("power_of_ten", &Scale::powerOfTen, py::arg("power"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)

  // icu::number::ScientificNotation
  sn.def("with_exponent_sign_display", &ScientificNotation::withExponentSignDisplay, py::arg("exponent_sign_display"));
  sn.def("with_min_exponent_digits", &ScientificNotation::withMinExponentDigits, py::arg("min_exponent_digits"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  // icu::number::UnlocalizedNumberFormatter
#if (U_ICU_VERSION_MAJOR_NUM >= 62)
  unf.def(py::init<>());
#endif // (U_ICU_VERSION_MAJOR_NUM >= 62)
  unf.def(py::init<const UnlocalizedNumberFormatter &>(), py::arg("other"));
  unf.def(
         "locale", [](const UnlocalizedNumberFormatter &self, const Locale &locale) { return self.locale(locale); },
         py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "locale", [](const UnlocalizedNumberFormatter &self, const char *locale) { return self.locale(locale); },
          py::arg("locale"));
}

#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/measfmt.h>
#include <unicode/measunit.h>
#include <unicode/measure.h>

using namespace icu;

void init_measfmt(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  //
  // UMeasureFormatWidth
  //
  py::enum_<UMeasureFormatWidth>(
      m, "UMeasureFormatWidth", py::arithmetic(),
      "Constants for various widths.\n\n"
      "There are 4 widths: Wide, Short, Narrow, Numeric. For example, for English, when formatting \"3 hours\" Wide is "
      "\"3 hours\"; short is \"3 hrs\"; narrow is \"3h\"; formatting \"3 hours 17 minutes\" as numeric give \"3:17\"")
      .value("UMEASFMT_WIDTH_WIDE", UMEASFMT_WIDTH_WIDE, "Spell out measure units.")
      .value("UMEASFMT_WIDTH_SHORT", UMEASFMT_WIDTH_SHORT, "Abbreviate measure units.")
      .value("UMEASFMT_WIDTH_NARROW", UMEASFMT_WIDTH_NARROW, "Use symbols for measure units when possible.")
      .value("UMEASFMT_WIDTH_NUMERIC", UMEASFMT_WIDTH_NUMERIC,
             "Completely omit measure units when possible.\n\n  "
             "For example, format '5 hours, 37 minutes' as '5:37'")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  //
  // icu::MeasureFormat
  //
  py::class_<MeasureFormat, Format> fmt(m, "MeasureFormat");

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  fmt.def(py::init([](const _LocaleVariant &locale, UMeasureFormatWidth width) {
            ErrorCode error_code;
            auto result = std::make_unique<MeasureFormat>(VARIANT_TO_LOCALE(locale), width, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"), py::arg("width_"));

  // FIXME: Implement "icu::MeasureFormat::MeasureFormat(const Locale &locale, UMeasureFormatWidth width,
  //  NumberFormat *nfToAdopt, UErrorCode &status)".
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)
  fmt.def(py::init<const MeasureFormat &>(), py::arg("other"));

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  fmt.def("__copy__", &MeasureFormat::clone);

  fmt.def(
      "__deepcopy__", [](const MeasureFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  fmt.def(
      "__eq__", [](const MeasureFormat &self, const Format &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  fmt.def("clone", &MeasureFormat::clone);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

  fmt.def_static(
         "create_currency_format",
         [](const _LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = MeasureFormat::createCurrencyFormat(VARIANT_TO_LOCALE(locale), error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"))
      .def_static("create_currency_format", []() {
        ErrorCode error_code;
        auto result = MeasureFormat::createCurrencyFormat(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      });

  fmt.def(
         // [2] icu::MeasureFormat::format
         "format",
         [](const MeasureFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          // [3] icu::Format::format
          "format",
          [](const MeasureFormat &self, const Formattable &obj, UnicodeString &append_to,
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
          // [4] icu::Format::format
          "format",
          [](const MeasureFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"));

#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  fmt.def(
      "format_measure_per_unit",
      [](const MeasureFormat &self, const Measure &measure, const MeasureUnit &per_unit, UnicodeString &append_to,
         FieldPosition &pos) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.formatMeasurePerUnit(measure, per_unit, append_to, pos, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("measure"), py::arg("per_unit"), py::arg("append_to"), py::arg("pos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)

#if (U_ICU_VERSION_MAJOR_NUM >= 53)
  fmt.def(
      "format_measures",
      [](const MeasureFormat &self, const std::vector<Measure> &measures, int32_t measure_count,
         UnicodeString &append_to, FieldPosition &pos) -> UnicodeString & {
        ErrorCode error_code;
        if (measure_count == -1) {
          measure_count = static_cast<int32_t>(measures.size());
        }
        auto &result = self.formatMeasures(measures.data(), measure_count, append_to, pos, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("measures"), py::arg("measure_count"), py::arg("append_to"), py::arg("pos"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 53)

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
  fmt.def(
      "get_unit_display_name",
      [](const MeasureFormat &self, const MeasureUnit &unit) {
        ErrorCode error_code;
        auto result = self.getUnitDisplayName(unit, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("unit"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

  fmt.def(
         // [2] icu::Format::parseObject
         "parse_object",
         [](const MeasureFormat &self, const _UnicodeStringVariant &source, Formattable &result,
            ParsePosition &parse_pos) { self.parseObject(VARIANT_TO_UNISTR(source), result, parse_pos); },
         py::arg("source"), py::arg("result"), py::arg("parse_pos"))
      .def(
          // [3] icu::Format::parseObject
          "parse_object",
          [](const MeasureFormat &self, const _UnicodeStringVariant &source, Formattable &result) {
            ErrorCode error_code;
            self.parseObject(VARIANT_TO_UNISTR(source), result, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
}

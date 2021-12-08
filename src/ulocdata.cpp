#include "main.hpp"
#include "ulocdataptr.hpp"
#include "usetptr.hpp"
#include <pybind11/stl.h>
#include <unicode/errorcode.h>

using namespace icu;

_ULocaleDataPtr::_ULocaleDataPtr(ULocaleData *p) : p_(p) {}
_ULocaleDataPtr::~_ULocaleDataPtr() {}
ULocaleData *_ULocaleDataPtr::get() const { return p_; }

void init_ulocdata(py::module &m) {
  py::enum_<ULocaleDataDelimiterType>(m, "ULocaleDataDelimiterType", py::arithmetic(),
                                      "The possible types of delimiters.")
      .value("ULOCDATA_QUOTATION_START", ULOCDATA_QUOTATION_START, "Quotation start.")
      .value("ULOCDATA_QUOTATION_END", ULOCDATA_QUOTATION_END, "Quotation end.")
      .value("ULOCDATA_ALT_QUOTATION_START", ULOCDATA_ALT_QUOTATION_START, "Alternate quotation start.")
      .value("ULOCDATA_ALT_QUOTATION_END", ULOCDATA_ALT_QUOTATION_END, "Alternate quotation end.")
#ifndef U_HIDE_DEPRECATED_API
      .value("ULOCDATA_DELIMITER_COUNT", ULOCDATA_DELIMITER_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<ULocaleDataExemplarSetType>(m, "ULocaleDataExemplarSetType", py::arithmetic(),
                                        "The possible types of exemplar character sets.")
      .value("ULOCDATA_ES_STANDARD", ULOCDATA_ES_STANDARD, "Basic set.")
      .value("ULOCDATA_ES_AUXILIARY", ULOCDATA_ES_AUXILIARY, "Auxiliary set.")
      .value("ULOCDATA_ES_INDEX", ULOCDATA_ES_INDEX, "Index Character set.")
      .value("ULOCDATA_ES_PUNCTUATION", ULOCDATA_ES_PUNCTUATION, "Punctuation set.")
#ifndef U_HIDE_DEPRECATED_API
      .value("ULOCDATA_ES_COUNT", ULOCDATA_ES_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UMeasurementSystem>(m, "UMeasurementSystem", py::arithmetic(),
                                "Enumeration for representing the measurement systems.")
      .value("UMS_SI", UMS_SI, "Measurement system specified by SI otherwise known as Metric system.")
      .value("UMS_US", UMS_US, "Measurement system followed in the United States of America.")
      .value("UMS_UK", UMS_UK, "Mix of metric and imperial units used in Great Britain.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UMS_LIMIT", UMS_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::class_<_ULocaleDataPtr>(m, "_ULocaleDataPtr");

  m.def(
      "ulocdata_close", [](_ULocaleDataPtr &uld) { ulocdata_close(uld); }, py::arg("uld"));
  m.def("ulocdata_get_cldr_version", []() {
    UVersionInfo version_array;
    ErrorCode error_code;
    ulocdata_getCLDRVersion(version_array, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    std::vector<uint8_t> result(version_array, version_array + sizeof(version_array));
    return result;
  });
  m.def(
      "ulocdata_get_delimiter",
      [](_ULocaleDataPtr &uld, ULocaleDataDelimiterType type) {
        ErrorCode error_code;
        auto dest_size = ulocdata_getDelimiter(uld, type, nullptr, 0, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        ulocdata_getDelimiter(uld, type, result.data(), dest_size, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("uld"), py::arg("type_"));
  m.def(
      "ulocdata_get_exemplar_set",
      [](_ULocaleDataPtr &uld, std::optional<_USetPtr> &fill_in, uint32_t options, ULocaleDataExemplarSetType extype) {
        ErrorCode error_code;
        auto p = ulocdata_getExemplarSet(uld, fill_in.value_or(nullptr), options, extype, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return std::make_unique<_USetPtr>(p);
      },
      py::arg("uld"), py::arg("fill_in"), py::arg("options"), py::arg("extype"));
  m.def(
      "ulocdata_get_locale_display_pattern",
      [](_ULocaleDataPtr &uld) {
        ErrorCode error_code;
        auto dest_size = ulocdata_getLocaleDisplayPattern(uld, nullptr, 0, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        ulocdata_getLocaleDisplayPattern(uld, result.data(), dest_size, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("uld"));
  m.def(
      "ulocdata_get_locale_separator",
      [](_ULocaleDataPtr &uld) {
        ErrorCode error_code;
        auto dest_size = ulocdata_getLocaleSeparator(uld, nullptr, 0, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        ulocdata_getLocaleSeparator(uld, result.data(), dest_size, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("uld"));
  m.def(
      "ulocdata_get_measurement_system",
      [](const char *locale_id) {
        ErrorCode error_code;
        auto result = ulocdata_getMeasurementSystem(locale_id, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("locale_id"));
  m.def(
      "ulocdata_get_no_substitute", [](_ULocaleDataPtr &uld) { return ulocdata_getNoSubstitute(uld); }, py::arg("uld"));
  m.def(
      "ulocdata_get_paper_size",
      [](const char *locale_id) {
        int32_t height, width;
        ErrorCode error_code;
        ulocdata_getPaperSize(locale_id, &height, &width, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return py::make_tuple(height, width);
      },
      py::arg("locale_id"));
  m.def(
      "ulocdata_open",
      [](const char *locale_id) {
        ErrorCode error_code;
        auto p = ulocdata_open(locale_id, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return std::make_unique<_ULocaleDataPtr>(p);
      },
      py::arg("locale_id"));
  m.def(
      "ulocdata_set_no_substitute", [](_ULocaleDataPtr &uld, UBool setting) { ulocdata_setNoSubstitute(uld, setting); },
      py::arg("uld"), py::arg("setting"));
}

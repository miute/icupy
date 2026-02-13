#include "main.hpp"
#include "ulocdataptr.hpp"
#include "usetptr.hpp"
#include <algorithm>
#include <pybind11/stl.h>

using namespace icu;

namespace icupy {

//
// struct ULocaleData
//
ULocaleDataPtr::ULocaleDataPtr(ULocaleData *p) : p_(p) {}

ULocaleDataPtr::~ULocaleDataPtr() {}

ULocaleData *ULocaleDataPtr::get() const { return p_; }

} // namespace icupy

void init_ulocdata(py::module &m) {
  //
  // enum ULocaleDataDelimiterType
  //
  py::enum_<ULocaleDataDelimiterType>(m, "ULocaleDataDelimiterType",
                                      py::arithmetic(),
                                      "The possible types of delimiters.")
      .value("ULOCDATA_QUOTATION_START", ULOCDATA_QUOTATION_START,
             "Quotation start.")
      .value("ULOCDATA_QUOTATION_END", ULOCDATA_QUOTATION_END, "Quotation end.")
      .value("ULOCDATA_ALT_QUOTATION_START", ULOCDATA_ALT_QUOTATION_START,
             "Alternate quotation start.")
      .value("ULOCDATA_ALT_QUOTATION_END", ULOCDATA_ALT_QUOTATION_END,
             "Alternate quotation end.")
      .value("ULOCDATA_DELIMITER_COUNT", ULOCDATA_DELIMITER_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, "
             "see ICU ticket #12420.")
      .export_values();

  //
  // enum ULocaleDataExemplarSetType
  //
  py::enum_<ULocaleDataExemplarSetType>(
      m, "ULocaleDataExemplarSetType", py::arithmetic(),
      "The possible types of exemplar character sets.")
      .value("ULOCDATA_ES_STANDARD", ULOCDATA_ES_STANDARD, "Basic set.")
      .value("ULOCDATA_ES_AUXILIARY", ULOCDATA_ES_AUXILIARY, "Auxiliary set.")
      .value("ULOCDATA_ES_INDEX", ULOCDATA_ES_INDEX, "Index Character set.")
      .value("ULOCDATA_ES_PUNCTUATION", ULOCDATA_ES_PUNCTUATION,
             "Punctuation set.")
      .value("ULOCDATA_ES_COUNT", ULOCDATA_ES_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, "
             "see ICU ticket #12420.")
      .export_values();

  //
  // enum UMeasurementSystem
  //
  py::enum_<UMeasurementSystem>(
      m, "UMeasurementSystem", py::arithmetic(),
      "Enumeration for representing the measurement systems.")
      .value("UMS_SI", UMS_SI,
             "Measurement system specified by SI otherwise known as Metric "
             "system.")
      .value("UMS_US", UMS_US,
             "Measurement system followed in the United States of America.")
      .value("UMS_UK", UMS_UK,
             "Mix of metric and imperial units used in Great Britain.")
      .value("UMS_LIMIT", UMS_LIMIT,
             "**Deprecated:** ICU 58 The numeric value may change over time, "
             "see ICU ticket #12420.")
      .export_values();

  //
  // struct ULocaleData
  //
  py::class_<icupy::ULocaleDataPtr>(m, "ULocaleData", R"doc(
    ULocaleData structure.

    See Also:
        :func:`ulocdata_close`
        :func:`ulocdata_open`
    )doc");

  //
  // Functions
  //
  m.def(
      "ulocdata_close", [](icupy::ULocaleDataPtr &uld) { ulocdata_close(uld); },
      py::arg("uld"));

  m.def("ulocdata_get_cldr_version", []() {
    UVersionInfo version_array;
    ErrorCode error_code;
    ulocdata_getCLDRVersion(version_array, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    py::tuple result(U_MAX_VERSION_LENGTH);
    int n = 0;
    std::for_each(std::begin(version_array), std::end(version_array),
                  [&](auto v) { result[n++] = v; });
    return result;
  });

  m.def(
      "ulocdata_get_delimiter",
      [](icupy::ULocaleDataPtr &uld, ULocaleDataDelimiterType type) {
        ErrorCode error_code;
        auto dest_size =
            ulocdata_getDelimiter(uld, type, nullptr, 0, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        ulocdata_getDelimiter(uld, type, result.data(), dest_size, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("uld"), py::arg("type"));

  m.def(
      "ulocdata_get_exemplar_set",
      [](icupy::ULocaleDataPtr &uld, std::optional<icupy::USetPtr> &fill_in,
         uint32_t options, ULocaleDataExemplarSetType extype) {
        ErrorCode error_code;
        auto p = ulocdata_getExemplarSet(uld, fill_in.value_or(nullptr),
                                         options, extype, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::USetPtr>(p);
      },
      py::arg("uld"), py::arg("fill_in"), py::arg("options"),
      py::arg("extype"));

  m.def(
      "ulocdata_get_locale_display_pattern",
      [](icupy::ULocaleDataPtr &uld) {
        ErrorCode error_code;
        auto dest_size =
            ulocdata_getLocaleDisplayPattern(uld, nullptr, 0, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        ulocdata_getLocaleDisplayPattern(uld, result.data(), dest_size,
                                         error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("uld"));

  m.def(
      "ulocdata_get_locale_separator",
      [](icupy::ULocaleDataPtr &uld) {
        ErrorCode error_code;
        auto dest_size =
            ulocdata_getLocaleSeparator(uld, nullptr, 0, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        ulocdata_getLocaleSeparator(uld, result.data(), dest_size, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("uld"));

  m.def(
      "ulocdata_get_measurement_system",
      [](const std::string &locale_id) {
        ErrorCode error_code;
        auto result =
            ulocdata_getMeasurementSystem(locale_id.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale_id"));

  m.def(
      "ulocdata_get_no_substitute",
      [](icupy::ULocaleDataPtr &uld) -> py::bool_ {
        return ulocdata_getNoSubstitute(uld);
      },
      py::arg("uld"));

  m.def(
      "ulocdata_get_paper_size",
      [](const std::string &locale_id) {
        int32_t height, width;
        ErrorCode error_code;
        ulocdata_getPaperSize(locale_id.data(), &height, &width, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::make_tuple(height, width);
      },
      py::arg("locale_id"));

  m.def(
      "ulocdata_open",
      [](const std::string &locale_id) {
        ErrorCode error_code;
        auto p = ulocdata_open(locale_id.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::ULocaleDataPtr>(p);
      },
      py::arg("locale_id"));

  m.def(
      "ulocdata_set_no_substitute",
      [](icupy::ULocaleDataPtr &uld, py::bool_ setting) {
        ulocdata_setNoSubstitute(uld, setting);
      },
      py::arg("uld"), py::arg("setting"));
}

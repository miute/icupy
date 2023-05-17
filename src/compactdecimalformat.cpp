#include "main.hpp"
#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
#include <unicode/compactdecimalformat.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

using namespace icu;

void init_compactdecimalformat(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  //
  // icu::CompactDecimalFormat
  //
  py::class_<CompactDecimalFormat, DecimalFormat> cdf(m, "CompactDecimalFormat");

  cdf.def(py::init<const CompactDecimalFormat &>(), py::arg("other"));

  cdf.def("__copy__", &CompactDecimalFormat::clone);

  cdf.def(
      "__deepcopy__", [](const CompactDecimalFormat &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  cdf.def("clone", &CompactDecimalFormat::clone);

  cdf.def_static(
      "create_instance",
      [](const icupy::LocaleVariant &in_locale, UNumberCompactStyle style) {
        ErrorCode error_code;
        auto result = CompactDecimalFormat::createInstance(icupy::to_locale(in_locale), style, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("in_locale"), py::arg("style"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

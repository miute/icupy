#include "main.hpp"
#include <unicode/currunit.h>

using namespace icu;

void init_currunit(py::module &m) {
  // icu::CurrencyUnit
  py::class_<CurrencyUnit, MeasureUnit> cu(m, "CurrencyUnit");
  cu.def(py::init([](const char16_t *iso_code) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = std::make_unique<CurrencyUnit>(iso_code, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         }),
         py::arg("iso_code"))
      .def(py::init<const CurrencyUnit &>(), py::arg("other"));
  cu.def("__copy__", &CurrencyUnit::clone)
      .def(
          "__deepcopy__", [](const CurrencyUnit &self, py::dict) { return self.clone(); }, py::arg("memo"));
  cu.def("clone", &CurrencyUnit::clone);
  cu.def("get_iso_currency", &CurrencyUnit::getISOCurrency);
}

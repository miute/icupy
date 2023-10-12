#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/curramt.h>
#include <unicode/measunit.h>
#include <unicode/measure.h>
#include <unicode/tmutamt.h>

using namespace icu;

void init_measure(py::module &m) {
  //
  // icu::Measure
  //
  py::class_<Measure, UObject> meas(m, "Measure");

  meas.def(py::init([](const Formattable &number, const MeasureUnit *adopted_unit) {
             ErrorCode error_code;
             auto result = std::make_unique<Measure>(
                 number, reinterpret_cast<MeasureUnit *>(adopted_unit ? adopted_unit->clone() : nullptr), error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("number"), py::arg("adopted_unit"))
      .def(py::init<const Measure &>(), py::arg("other"));

  meas.def("__copy__", &Measure::clone);

  meas.def(
      "__deepcopy__", [](const Measure &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  meas.def(
      "__eq__", [](const Measure &self, const UObject &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

#if (U_ICU_VERSION_MAJOR_NUM >= 74)
  meas.def(
      "__ne__", [](const Measure &self, const UObject &other) { return self != other; }, py::is_operator(),
      py::arg("other"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 74)

  meas.def("clone", &Measure::clone);

  meas.def("get_number", &Measure::getNumber);

  meas.def("get_unit", &Measure::getUnit);

  //
  // icu::CurrencyAmount
  //
  py::class_<CurrencyAmount, Measure> ca(m, "CurrencyAmount");

  ca.def(py::init([](const Formattable &amount, const char16_t *iso_code) {
           ErrorCode error_code;
           auto result = std::make_unique<CurrencyAmount>(amount, iso_code, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }),
         py::arg("amount"), py::arg("iso_code").none(false))
      .def(py::init([](double amount, const char16_t *iso_code) {
             ErrorCode error_code;
             auto result = std::make_unique<CurrencyAmount>(amount, iso_code, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("amount"), py::arg("iso_code").none(false))
      .def(py::init<const CurrencyAmount &>(), py::arg("other"));

  ca.def("__copy__", &CurrencyAmount::clone);

  ca.def(
      "__deepcopy__", [](const CurrencyAmount &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  ca.def("clone", &CurrencyAmount::clone);

  ca.def("get_currency", &CurrencyAmount::getCurrency);

  ca.def("get_iso_currency", &CurrencyAmount::getISOCurrency);

  //
  // icu::TimeUnitAmount
  //
  py::class_<TimeUnitAmount, Measure> tua(m, "TimeUnitAmount");

  tua.def(py::init([](const Formattable &number, TimeUnit::UTimeUnitFields time_unit_field) {
            ErrorCode error_code;
            auto result = std::make_unique<TimeUnitAmount>(number, time_unit_field, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("number"), py::arg("time_unit_field"))
      .def(py::init([](double amount, TimeUnit::UTimeUnitFields time_unit_field) {
             ErrorCode error_code;
             auto result = std::make_unique<TimeUnitAmount>(amount, time_unit_field, error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           }),
           py::arg("amount"), py::arg("time_unit_field"))
      .def(py::init<const TimeUnitAmount &>(), py::arg("other"));

  tua.def("__copy__", &TimeUnitAmount::clone);

  tua.def(
      "__deepcopy__", [](const TimeUnitAmount &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  tua.def("clone", &TimeUnitAmount::clone);

  tua.def("get_time_unit", &TimeUnitAmount::getTimeUnit);

  tua.def("get_time_unit_field", &TimeUnitAmount::getTimeUnitField);
}

#include "main.hpp"
#include <unicode/gregocal.h>

using namespace icu;

void init_gregocal(py::module &m) {
  // icu::Calendar
  py::class_<Calendar, UObject> cal(m, "Calendar");

  // icu::GregorianCalendar
  py::class_<GregorianCalendar, Calendar> gc(m, "GregorianCalendar");

  py::enum_<GregorianCalendar::EEras>(gc, "EEras", py::arithmetic())
      .value("BC", GregorianCalendar::EEras::BC)
      .value("AD", GregorianCalendar::EEras::AD)
      .export_values();
}

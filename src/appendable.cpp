#include "main.hpp"
#include <unicode/appendable.h>

using namespace icu;

void init_appendable(py::module &m) {
  // icu::Appendable
  py::class_<Appendable, UObject> app(m, "Appendable");
  app.def("append_code_point", &Appendable::appendCodePoint, py::arg("c"));
  app.def(
      "append_code_unit", [](Appendable &self, int16_t c) { return self.appendCodeUnit(c); }, py::arg("c"));
  app.def("append_string", &Appendable::appendString, py::arg("s"), py::arg("length"));
  // FIXME: Implement "char16_t *icu::Appendable::getAppendBuffer(int32_t minCapacity, int32_t desiredCapacityHint,
  // char16_t *scratch, int32_t scratchCapacity, int32_t *resultCapacity)".
  app.def("reserve_append_capacity", &Appendable::reserveAppendCapacity, py::arg("append_capacity"));

  // icu::UnicodeStringAppendable
  py::class_<UnicodeStringAppendable, Appendable> usa(m, "UnicodeStringAppendable");
  usa.def(py::init<UnicodeString &>(), py::arg("s"));
  usa.def("append_code_point", &UnicodeStringAppendable::appendCodePoint, py::arg("c"));
  usa.def(
      "append_code_unit", [](UnicodeStringAppendable &self, int16_t c) { return self.appendCodeUnit(c); },
      py::arg("c"));
  usa.def("append_string", &UnicodeStringAppendable::appendString, py::arg("s"), py::arg("length"));
  // FIXME: Implement "char16_t *icu::UnicodeStringAppendable::getAppendBuffer(int32_t minCapacity,
  // int32_t desiredCapacityHint, char16_t *scratch, int32_t scratchCapacity, int32_t *resultCapacity)".
  usa.def("reserve_append_capacity", &UnicodeStringAppendable::reserveAppendCapacity, py::arg("append_capacity"));
}

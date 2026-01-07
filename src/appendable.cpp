#include "main.hpp"
#include <unicode/appendable.h>

using namespace icu;

void init_appendable(py::module &m) {
  //
  // class icu::Appendable
  //
  py::class_<Appendable, UObject> app(m, "Appendable");

  //
  // class icu::UnicodeStringAppendable
  //
  py::class_<UnicodeStringAppendable, Appendable> usa(
      m, "UnicodeStringAppendable");

  //
  // class icu::Appendable
  //
  app.def(
      "append_code_point",
      [](Appendable &self, UChar32 c) -> py::bool_ {
        return self.appendCodePoint(c);
      },
      py::arg("c"));

  app.def(
      "append_code_unit",
      [](Appendable &self, int16_t c) -> py::bool_ {
        return self.appendCodeUnit(c);
      },
      py::arg("c"));

  app.def(
      "append_string",
      [](Appendable &self, const std::u16string &s, int32_t length)
          -> py::bool_ { return self.appendString(s.data(), length); },
      py::arg("s"), py::arg("length") = -1);

  app.def(
      "reserve_append_capacity",
      [](Appendable &self, int32_t append_capacity) -> py::bool_ {
        return self.reserveAppendCapacity(append_capacity);
      },
      py::arg("append_capacity"));

  //
  // class icu::UnicodeStringAppendable
  //
  usa.def(py::init<UnicodeString &>(), py::arg("s"));
}

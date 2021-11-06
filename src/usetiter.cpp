#include "main.hpp"
#include <unicode/uniset.h>
#include <unicode/usetiter.h>

using namespace icu;

void init_usetiter(py::module &m) {
  // icu::UnicodeSetIterator
  py::class_<UnicodeSetIterator, UObject> usi(m, "UnicodeSetIterator");
  usi.def(py::init<const UnicodeSet &>(), py::arg("set_")).def(py::init<>());
  usi.def("get_codepoint", &UnicodeSetIterator::getCodepoint);
  usi.def("get_codepoint_end", &UnicodeSetIterator::getCodepointEnd);
  usi.def("get_string", &UnicodeSetIterator::getString);
  usi.def("is_string", &UnicodeSetIterator::isString);
  usi.def("next", &UnicodeSetIterator::next);
  usi.def("next_range", &UnicodeSetIterator::nextRange);
  usi.def("reset", py::overload_cast<>(&UnicodeSetIterator::reset))
      .def("reset", py::overload_cast<const UnicodeSet &>(&UnicodeSetIterator::reset), py::arg("set_"));
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  usi.def("skip_to_strings", &UnicodeSetIterator::skipToStrings);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
}

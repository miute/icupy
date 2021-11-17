#include "main.hpp"
#include <sstream>
#include <unicode/utrans.h>

void init_utrans(py::module &m) {
  py::enum_<UTransDirection>(
      m, "UTransDirection", py::arithmetic(),
      "Direction constant indicating the direction in a transliterator, e.g., the forward or reverse rules of a "
      "*RuleBasedTransliterator*.\n\n"
      "Specified when a transliterator is opened. An \"A-B\" transliterator transliterates A to B when operating in "
      "the forward direction, and B to A when operating in the reverse direction.")
      .value("UTRANS_FORWARD", UTRANS_FORWARD,
             "*UTRANS_FORWARD* means from <source> to <target> for a transliterator with ID <source>-<target>.\n\n  "
             "For a transliterator opened using a rule, it means forward direction rules, e.g., \"A > B\".")
      .value("UTRANS_REVERSE", UTRANS_REVERSE,
             "*UTRANS_REVERSE* means from <target> to <source> for a transliterator with ID <source>-<target>.\n\n  "
             "For a transliterator opened using a rule, it means reverse direction rules, e.g., \"A < B\".")
      .export_values();

  py::class_<UTransPosition> tp(m, "UTransPosition");
  tp.def(py::init<>())
      .def(py::init<int32_t, int32_t, int32_t, int32_t>(), py::arg("context_start"), py::arg("context_limit"),
           py::arg("start"), py::arg("limit"));
  tp.def("__repr__", [](const UTransPosition &self) {
    std::stringstream ss;
    ss << "UTransPosition(";
    ss << "context_start=" << self.contextStart;
    ss << ", start=" << self.start;
    ss << ", limit=" << self.limit;
    ss << ", context_limit=" << self.contextLimit;
    ss << ")";
    return ss.str();
  });
  tp.def_readwrite("context_limit", &UTransPosition::contextLimit);
  tp.def_readwrite("context_start", &UTransPosition::contextStart);
  tp.def_readwrite("limit", &UTransPosition::limit);
  tp.def_readwrite("start", &UTransPosition::start);
}

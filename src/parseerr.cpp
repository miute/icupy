#include "main.hpp"
#include <iomanip>
#include <sstream>
#include <unicode/parseerr.h>
#include <unicode/unistr.h>

using namespace icu;

void init_parseerr(py::module &m) {
  py::class_<UParseError> pe(m, "UParseError");
  pe.def(py::init<>());
  pe.def("__repr__", [](const UParseError &self) {
    std::stringstream ss;
    std::string pre_context, post_context;
    UnicodeString(self.preContext).toUTF8String(pre_context);
    UnicodeString(self.postContext).toUTF8String(post_context);
    ss << "UParseError(";
    ss << "line=" << self.line;
    ss << ", offset=" << self.offset;
    ss << ", pre_context=" << std::quoted(pre_context, '\'');
    ss << ", post_context=" << std::quoted(post_context, '\'');
    ss << ")";
    return ss.str();
  });
  pe.def_readonly("line", &UParseError::line);
  pe.def_readonly("offset", &UParseError::offset);
  pe.def_readonly("post_context", &UParseError::postContext, py::return_value_policy::reference);
  pe.def_readonly("pre_context", &UParseError::preContext, py::return_value_policy::reference);
}

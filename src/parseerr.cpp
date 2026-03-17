#include "main.hpp"
#include <iomanip>
#include <sstream>
#include <unicode/parseerr.h>
#include <unicode/unistr.h>

using namespace icu;

void init_parseerr(py::module &m) {
  //
  // struct UParseError
  //
  py::class_<UParseError> pe(m, "UParseError", R"doc(
      Structure that stores detailed information about parsing errors.
      )doc");

  pe.def(py::init<>(), R"doc(
      Initialize a ``UParseError`` instance.
      )doc");

  pe.def("__repr__", [](const UParseError &self) {
    std::stringstream ss;
    std::string pre_context, post_context;
    UnicodeString(self.preContext).toUTF8String(pre_context);
    UnicodeString(self.postContext).toUTF8String(post_context);
    ss << "<UParseError(";
    ss << "line=" << self.line;
    ss << ", offset=" << self.offset;
    ss << ", pre_context=" << std::quoted(pre_context, '\'');
    ss << ", post_context=" << std::quoted(post_context, '\'');
    ss << ")>";
    return ss.str();
  });

  pe.def_readonly("line", &UParseError::line, R"doc(
      int: The line on which the error occurred.

      If the parser uses this field, it sets it to the line number of the
      source text line on which the error appears, which will be a value >= 1.
      If the parse does not support line numbers, the value will be <= 0.
      )doc");

  pe.def_readonly("offset", &UParseError::offset, R"doc(
      int: The character offset to the error.

      If the line field is >= 1, then this is the offset from the start of the
      line. Otherwise, this is the offset from the start of the text. If the
      parser does not support this field, it will have a value < 0.
      )doc");

  pe.def_readonly("post_context", &UParseError::postContext,
                  py::return_value_policy::reference, R"doc(
      str: The error itself and/or textual context after the error.

      The empty string if not supported by parser.
      )doc");

  pe.def_readonly("pre_context", &UParseError::preContext,
                  py::return_value_policy::reference, R"doc(
      str: Textual context before the error.

      The empty string if not supported by parser.
      )doc");
}

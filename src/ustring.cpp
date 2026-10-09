#include "main.hpp"
#include <unicode/ustring.h>

void init_ustring(py::module &m) {
  m.def(
      "u_count_char32",
      [](const std::u16string &s, int32_t length) {
        return u_countChar32(s.data(), length);
      },
      py::arg("s"), py::arg("length") = -1, R"doc(
      Return the number of code points in the specified string.

      *s* is a string to be counted.

      *length* is the length of *s*; can be set to -1 for a NUL-terminated
      string.
      )doc");

  m.def(
      "u_strlen", [](const std::u16string &s) { return u_strlen(s.data()); },
      py::arg("s"), R"doc(
      Return the length of the specified NUL-terminated string in code units.
      )doc");

  m.def(
      "u_unescape",
      [](const std::string &src) {
        auto src_data = src.data();
        const auto dest_capacity = u_unescape(src_data, nullptr, 0);
        std::u16string result(dest_capacity, u'\0');
        u_unescape(src_data, result.data(), dest_capacity);
        return result;
      },
      py::arg("src"), R"doc(
      Unescape the specified NUL-terminated string and return the result.
      If the escape sequence is ill-formed, the result will be an empty string.

      The following escape sequences are recognized:

      * ``\\uXXXX`` - 4-digit hexadecimal number
      * ``\\UXXXXXXXX`` - 8-digit hexadecimal number
      * ``\\xXX`` - 1-2 digit hexadecimal number
      * ``\\ooo`` - 1-3-digit octal number
      * ``\\cX`` - control character; X is masked with 0x1F

      As well as the standard ANSI C escape sequences:

      ``\\a`` => U+0007, ``\\b`` => U+0008, ``\\t`` => U+0009, ``\\n`` => U+000A,
      ``\\v`` => U+000B, ``\\f`` => U+000C, ``\\r`` => U+000D, ``\\e`` => U+001B,
      ``\\"`` => U+0022, ``\\'`` => U+0027, ``\\?`` => U+003F, ``\\\\`` => U+005C

      Anything else following a backslash is generically escaped.

      .. seealso::

         :meth:`UnicodeString.unescape`
         :meth:`UnicodeString.unescape_at`
      )doc");
}

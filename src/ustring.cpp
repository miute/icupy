#include "main.hpp"
#include <unicode/ustring.h>

void init_ustring(py::module &m) {
  m.def(
      "u_count_char32", [](const std::u16string &s, int32_t length) { return u_countChar32(s.data(), length); },
      py::arg("s"), py::arg("length"));

  m.def("u_strlen", [](const std::u16string &s) { return u_strlen(s.data()); }, py::arg("s"));

  m.def(
      "u_unescape",
      [](const std::string &src) {
        auto p = src.data();
        const auto dest_capacity = u_unescape(p, nullptr, 0);
        std::u16string result(dest_capacity, u'\0');
        u_unescape(p, result.data(), dest_capacity);
        return result;
      },
      py::arg("src"));
}

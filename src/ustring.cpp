#include "main.hpp"
#include <unicode/ustring.h>

void init_ustring(py::module &m) {
  m.def("u_count_char32", &u_countChar32, py::arg("s"), py::arg("length"));
  m.def("u_strlen", &u_strlen, py::arg("s"));
  m.def(
      "u_unescape",
      [](const char *src) {
        const auto length = u_unescape(src, nullptr, 0);
        std::u16string result(length, u'\0');
        u_unescape(src, result.data(), static_cast<int32_t>(result.size()));
        return result;
      },
      py::arg("src"));
}

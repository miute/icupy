#include "char16ptr.hpp"
#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <unicode/char16ptr.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

using namespace icu;

_ConstChar16Ptr::_ConstChar16Ptr(const char16_t *p) : p_(p) {}
_ConstChar16Ptr::~_ConstChar16Ptr() {}
const char16_t *_ConstChar16Ptr::get() const { return p_; }

void init_char16ptr(py::module &m) {
  py::class_<_ConstChar16Ptr>(m, "_ConstChar16Ptr")
      .def(
          "__getitem__", [](const _ConstChar16Ptr &self, int32_t index) -> uint16_t { return *(self + index); },
          py::arg("index"));

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  py::class_<Char16Ptr>(m, "Char16Ptr")
      .def(py::init<char16_t *>(), py::arg("p"))
      .def(
          "__getitem__", [](const Char16Ptr &self, int32_t index) -> uint16_t { return *(self + index); },
          py::arg("index"))
      .def("get", &Char16Ptr::get);

  py::class_<ConstChar16Ptr>(m, "ConstChar16Ptr")
      .def(py::init<const char16_t *>(), py::arg("p"))
      .def(
          "__getitem__", [](const ConstChar16Ptr &self, int32_t index) -> uint16_t { return *(self + index); },
          py::arg("index"))
      .def("get", &ConstChar16Ptr::get);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
}

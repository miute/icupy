#include "char16ptr.hpp"
#include "main.hpp"

#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <unicode/char16ptr.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

using namespace icu;

_ConstChar16Ptr::_ConstChar16Ptr(const char16_t *p, int32_t length, int32_t capacity)
    : p_(p), length_(length), capacity_(capacity) {}

_ConstChar16Ptr::~_ConstChar16Ptr() {}

const char16_t *_ConstChar16Ptr::get() const { return p_; }

void init_char16ptr(py::module &m) {
  //
  // _ConstChar16Ptr
  //
  py::class_<_ConstChar16Ptr> pccp(m, "_ConstChar16Ptr");

  pccp.def(
          "__getitem__",
          [](const _ConstChar16Ptr &self, int32_t index) -> uint16_t {
            const auto capacity = self.capacity();
            if (index < 0) {
              index += capacity;
            }
            if (index < 0 || index >= capacity) {
              throw py::index_error("string index out of range: " + std::to_string(index));
            }
            return self[index];
          },
          py::arg("index"))
      .def(
          "__getitem__",
          [](const _ConstChar16Ptr &self, const py::slice &slice) {
            size_t start = 0, stop = 0, step = 0, slice_length = 0;
            if (!slice.compute(self.capacity(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }

            std::vector<uint16_t> result(slice_length);
            for (size_t n = 0; n < slice_length; ++n) {
              result[n] = self[static_cast<int32_t>(start)];
              start += step;
            }
            return result;
          },
          py::arg("slice"));

  pccp.def("__len__", &_ConstChar16Ptr::capacity);

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  //
  // icu::Char16Ptr
  //
  py::class_<Char16Ptr>(m, "Char16Ptr")
      .def(py::init<char16_t *>(), py::arg("p"))
      .def(
          "__getitem__", [](const Char16Ptr &self, int32_t index) -> uint16_t { return *(self + index); },
          py::arg("index"))
      .def("get", &Char16Ptr::get);

  //
  // icu::ConstChar16Ptr
  //
  py::class_<ConstChar16Ptr>(m, "ConstChar16Ptr")
      .def(py::init<const char16_t *>(), py::arg("p"))
      .def(
          "__getitem__", [](const ConstChar16Ptr &self, int32_t index) -> uint16_t { return *(self + index); },
          py::arg("index"))
      .def("get", &ConstChar16Ptr::get);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
}

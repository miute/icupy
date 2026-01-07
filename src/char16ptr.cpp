#include "char16ptr.hpp"
#include "main.hpp"
#include <memory>
#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <unicode/char16ptr.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

using namespace icu;

_ConstChar16Ptr::_ConstChar16Ptr(const char16_t *p, int32_t length,
                                 int32_t capacity)
    : p_(p), length_(length), capacity_(capacity) {}

_ConstChar16Ptr::~_ConstChar16Ptr() {}

const char16_t *_ConstChar16Ptr::get() const { return p_; }

void init_char16ptr(py::module &m) {
  //
  // class _ConstChar16Ptr
  //
  py::class_<_ConstChar16Ptr> pccp(m, "_ConstChar16Ptr");

  pccp.def(
      "__getitem__",
      [](const _ConstChar16Ptr &self, int32_t index) -> uint16_t {
        auto n = index;
        const auto capacity = self.capacity();
        if (n < 0) {
          n += capacity;
        }
        if (n < 0 || n >= capacity) {
          throw py::index_error("string index out of range: " +
                                std::to_string(index));
        }
        return self[n];
      },
      py::arg("index"));

  pccp.def("__len__", &_ConstChar16Ptr::capacity);
}

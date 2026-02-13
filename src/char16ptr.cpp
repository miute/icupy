#include "char16ptr.hpp"
#include "main.hpp"

namespace icupy {

ConstChar16Ptr::ConstChar16Ptr(const char16_t *p, int32_t length,
                               int32_t capacity)
    : p_(p), length_(length), capacity_(capacity) {}

ConstChar16Ptr::~ConstChar16Ptr() {}

const char16_t *ConstChar16Ptr::get() const { return p_; }

} // namespace icupy

void init_char16ptr(py::module &m) {
  //
  // class ConstChar16Ptr
  //
  py::class_<icupy::ConstChar16Ptr> pccp(m, "ConstChar16Ptr");

  pccp.def(
      "__getitem__",
      [](const icupy::ConstChar16Ptr &self, int32_t index) -> uint16_t {
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

  pccp.def("__len__", &icupy::ConstChar16Ptr::capacity);
}

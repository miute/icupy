#include "main.hpp"
#include <memory>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/sortkey.h>

using namespace icu;

void init_sortkey(py::module &m) {
  //
  // icu::CollationKey
  //
  py::class_<CollationKey, UObject> ck(m, "CollationKey");

  ck.def(py::init<>())
      .def(py::init([](const py::buffer &values, int32_t count) {
             auto info = values.request();
             if (count == -1) {
               count = static_cast<int32_t>(info.size);
             }
             return std::make_unique<CollationKey>(reinterpret_cast<uint8_t *>(info.ptr), count);
           }),
           py::arg("values"), py::arg("count") = -1)
      .def(py::init<CollationKey &>(), py::arg("other"));

  ck.def(
      "__eq__", [](const CollationKey &self, const CollationKey &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  ck.def("__hash__", &CollationKey::hashCode);

  ck.def(
      "__ne__", [](const CollationKey &self, const CollationKey &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  ck.def(
      "compare_to",
      [](const CollationKey &self, const CollationKey &target) {
        ErrorCode error_code;
        auto result = self.compareTo(target, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("target"));

  ck.def("get_byte_array", [](const CollationKey &self) {
    int32_t count = 0;
    auto p = self.getByteArray(count);
    return py::bytes(reinterpret_cast<const char *>(p), count);
  });

  ck.def("hash_code", &CollationKey::hashCode);

  ck.def("is_bogus", [](const CollationKey &self) -> py::bool_ { return self.isBogus(); });
}

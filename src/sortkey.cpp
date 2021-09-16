#include "main.hpp"
#include <memory>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/sortkey.h>

using namespace icu;

void init_sortkey(py::module &m) {
  py::class_<CollationKey, UObject> ck(m, "CollationKey");
  ck.def(py::init<>())
      .def(py::init([](const std::vector<uint8_t> &values, int32_t count) {
             return std::make_unique<CollationKey>(values.data(), count);
           }),
           py::arg("values"), py::arg("count"))
      .def(py::init<CollationKey &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  ck.def(
      "compare_to",
      [](const CollationKey &self, const CollationKey &target) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.compareTo(target, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("target"));
  ck.def(
      "get_byte_array",
      [](const CollationKey &self) {
        int32_t count = 0;
        auto p = self.getByteArray(count);
        std::vector<uint8_t> result(p, p + count);
        return result;
      },
      py::return_value_policy::reference);
  ck.def("hash_code", &CollationKey::hashCode);
  ck.def("is_bogus", &CollationKey::isBogus);
}

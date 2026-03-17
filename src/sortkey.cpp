#include "main.hpp"
#include <memory>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/sortkey.h>

using namespace icu;

void init_sortkey(py::module &m) {
  //
  // class icu::CollationKey
  //
  py::class_<CollationKey, UObject> ck(m, "CollationKey", R"doc(
      Optimize the comparison operations of the :class:`Collator` class.

      Use the ``CollationKey`` objects instead of :class:`Collator` to compare
      strings multiple times. A ``CollationKey`` preprocesses the comparison
      information from the :class:`Collator` object to make the comparison
      faster. If you are not going to comparing strings multiple times, then
      using the :class:`Collator` object is generally faster, since it only
      processes as much of the string as needed to make a comparison.
      )doc");

  ck.def(py::init<>(), R"doc(
      Initialize a ``CollationKey`` instance as an empty collation key based on
      the null string.

      An empty collation key contains no sorting information. When comparing
      two empty collation keys, the result is
      :attr:`~UCollationResult.UCOL_EQUAL`. Comparing empty collation key with
      non-empty collation key is always :attr:`~UCollationResult.UCOL_LESS`.
      )doc")
      .def(py::init([](const py::buffer &values, int32_t count) {
             auto info = values.request();
             if (count == -1) {
               count = static_cast<int32_t>(info.size);
             }
             return std::make_unique<CollationKey>(
                 reinterpret_cast<uint8_t *>(info.ptr), count);
           }),
           py::arg("values"), py::arg("count") = -1, R"doc(
      Initialize a ``CollationKey`` instance with the specified collation key
      values.
      )doc")
      .def(py::init<CollationKey &>(), py::arg("other"), R"doc(
      Initialize a ``CollationKey`` instance from another ``CollationKey``.
      )doc");

  ck.def(
      "__eq__",
      [](const CollationKey &self, const CollationKey &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  ck.def("__hash__", &CollationKey::hashCode, R"doc(
      Return the hash value of this object.

      This is equivalent to :meth:`hash_code`.
      )doc");

  ck.def(
      "__ne__",
      [](const CollationKey &self, const CollationKey &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

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
      py::arg("target"), R"doc(
      Return :attr:`~UCollationResult.UCOL_LESS` if *self* < *target*,
      :attr:`~UCollationResult.UCOL_GREATER` if *self* > *target*, and
      :attr:`~UCollationResult.UCOL_EQUAL` otherwise.
      )doc");

  ck.def(
      "get_byte_array",
      [](const CollationKey &self) {
        int32_t count;
        auto p = self.getByteArray(count);
        return py::bytes(reinterpret_cast<const char *>(p), count);
      },
      R"doc(
      Return the collation key values.
      )doc");

  ck.def("hash_code", &CollationKey::hashCode, R"doc(
      Return an integer that is unique to the collation key.

      See Also:
          :meth:`.__hash__`
      )doc");

  ck.def(
      "is_bogus",
      [](const CollationKey &self) -> py::bool_ { return self.isBogus(); },
      R"doc(
      Return ``True`` if the key is in an invalid, ``False`` otherwise.
      )doc");
}

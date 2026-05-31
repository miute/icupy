#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/chariter.h>
#include <unicode/coleitr.h>

using namespace icu;

void init_coleitr(py::module &m) {
  //
  // class icu::CollationElementIterator
  //
  py::class_<CollationElementIterator, UObject> cei(
      m, "CollationElementIterator", R"doc(
      Iterator to walk through each character of an international string.

      .. seealso::

         :meth:`RuleBasedCollator.create_collation_element_iterator`

      .. rubric:: Attributes

      .. autoattribute:: CollationElementIterator.NULLORDER

         ``NULLORDER`` indicates that the iteration over the source string
         has been finished.

         .. seealso::

            :meth:`.next`
            :meth:`.previous`
      )doc");

  //
  // enum icu::CollationElementIterator::NULLORDER
  //
  cei.def_property_readonly_static(
      "NULLORDER", [](const py::object & /* self */) -> int32_t {
        return CollationElementIterator::NULLORDER;
      });

  //
  // class icu::CollationElementIterator
  //
  cei.def(py::init<CollationElementIterator &>(), py::arg("other"), R"doc(
      Initialize a ``CollationElementIterator`` instance from another
      ``CollationElementIterator``.
      )doc");

  cei.def(
      "__eq__",
      [](const CollationElementIterator &self,
         const CollationElementIterator &other) { return self == other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  cei.def(
      "__iter__",
      [](CollationElementIterator &self) -> CollationElementIterator & {
        self.reset();
        return self;
      },
      R"doc(
      Return the iterator object.

      The iterator is reset to the beginning.

      .. seealso::

         :meth:`.__next__`
      )doc");

  cei.def(
      "__ne__",
      [](const CollationElementIterator &self,
         const CollationElementIterator &other) { return self != other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  cei.def(
      "__next__",
      [](CollationElementIterator &self) {
        ErrorCode error_code;
        auto result = self.next(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        } else if (result == CollationElementIterator::NULLORDER) {
          throw py::stop_iteration();
        }
        return result;
      },
      R"doc(
      Return the collation order of the next character from the iterator.

      .. seealso::

         :meth:`.next`
      )doc");

  cei.def("get_max_expansion",
          py::overload_cast<int32_t>(&CollationElementIterator::getMaxExpansion,
                                     py::const_),
          py::arg("order"), R"doc(
      Return the maximum expansion for the specified collation order.
      )doc");

  cei.def("get_offset", &CollationElementIterator::getOffset, R"doc(
      Return the current offset in the source string.
      )doc");

  cei.def_static(
      "is_ignorable",
      [](int32_t order) -> py::bool_ {
        return CollationElementIterator::isIgnorable(order);
      },
      py::arg("order"), R"doc(
      Return whether the specified collation order is ignorable.
      )doc");

  cei.def(
      "next",
      [](CollationElementIterator &self) {
        ErrorCode error_code;
        auto result = self.next(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the collation order of the next character in the string, or
      :attr:`NULLORDER` if the end of the string is reached.
      )doc");

  cei.def(
      "previous",
      [](CollationElementIterator &self) {
        ErrorCode error_code;
        auto result = self.previous(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the collation order of the previous character in the string, or
      :attr:`NULLORDER` if the start of the string is reached.
      )doc");

  cei.def_static("primary_order", &CollationElementIterator::primaryOrder,
                 py::arg("order"), R"doc(
      Return the primary order for the specified collation order.
      )doc");

  cei.def("reset", &CollationElementIterator::reset, R"doc(
      Set the offset to the beginning of the source string.
      )doc");

  cei.def_static("secondary_order", &CollationElementIterator::secondaryOrder,
                 py::arg("order"), R"doc(
      Return the secondary order for the specified collation order.
      )doc");

  cei.def(
      "set_offset",
      [](CollationElementIterator &self, int32_t new_offset) {
        ErrorCode error_code;
        self.setOffset(new_offset, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("new_offset"),
      R"doc(
      Sets the offset to the specified new offset in the source string.
      )doc");

  cei.def(
         "set_text",
         [](CollationElementIterator &self, CharacterIterator &str) {
           ErrorCode error_code;
           self.setText(str, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
         },
         py::arg("text"), R"doc(
      Set the source string.
      )doc")
      .def(
          "set_text",
          [](CollationElementIterator &self,
             const icupy::UnicodeStringVariant &str) {
            ErrorCode error_code;
            self.setText(icupy::to_unistr(str), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), R"doc(
      Set the source string.
      )doc");

  cei.def("strength_order", &CollationElementIterator::strengthOrder,
          py::arg("order"), R"doc(
      Return the comparison order for the specified collation order.
      )doc");

  cei.def_static("tertiary_order", &CollationElementIterator::tertiaryOrder,
                 py::arg("order"), R"doc(
      Return the tertiary order for the specified collation order.
      )doc");
}

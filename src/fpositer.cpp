#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/fpositer.h>

using namespace icu;

void init_fpositer(py::module &m) {
  //
  // class icu::FieldPositionIterator
  //
  py::class_<FieldPositionIterator, UObject> fpi(m, "FieldPositionIterator",
                                                 R"doc(
      Iterator for iterating over fields in formatted output.

      .. seealso::

         :class:`FieldPosition`
         :meth:`Format.format`

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         fmt = icu.DateFormat.create_date_instance(icu.DateFormat.LONG, icu.Locale.get_france())
         dest = icu.UnicodeString()
         pos_iter = icu.FieldPositionIterator()
         fmt.format(icu.Calendar.get_now(), dest, pos_iter)
         pos = icu.FieldPosition()
         while pos_iter.next(pos):
             print(dest[pos.get_begin_index():pos.get_end_index()], icu.UDateFormatField(pos.get_field()))
      )doc");

  fpi.def(py::init<>(), R"doc(
      Initialize a ``FieldPositionIterator`` instance as an empty iterator.
      )doc")
      .def(py::init<const FieldPositionIterator &>(), py::arg("other"), R"doc(
      Initialize a ``FieldPositionIterator`` instance from another
      ``FieldPositionIterator``.
      )doc");

  fpi.def(
      "__eq__",
      [](const FieldPositionIterator &self,
         const FieldPositionIterator &other) { return self == other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  fpi.def(
      "__ne__",
      [](const FieldPositionIterator &self,
         const FieldPositionIterator &other) { return self != other; },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  fpi.def(
      "next",
      [](FieldPositionIterator &self, FieldPosition &fp) -> py::bool_ {
        return self.next(fp);
      },
      py::arg("fp"), R"doc(
      Iterate to the next field and return ``True`` if there is a next field,
      ``False`` otherwise.
      )doc");
}

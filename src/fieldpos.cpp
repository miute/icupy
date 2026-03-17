#include "main.hpp"
#include <pybind11/operators.h>
#include <sstream>
#include <unicode/fieldpos.h>

using namespace icu;

void init_fieldpos(py::module &m) {
  //
  // class icu::FieldPosition
  //
  py::class_<FieldPosition, UObject> fp(m, "FieldPosition", R"doc(
      Simple class used by :class:`Format` and its subclasses to identify
      fields in formatted output.

      Fields are identified by constants, whose names are typically defined in
      various enumeration types ending with Field.

      See Also:
          :class:`UDateFormatField`
          :class:`UListFormatterField`
          :class:`UNumberFormatFields`
          :class:`URelativeDateTimeFormatterField`

      Example:
          Retrieve a specific field of a date:

          >>> from icupy import icu
          >>> fmt = icu.DateFormat.create_date_instance(icu.DateFormat.LONG, icu.Locale.get_france())
          >>> dest = icu.UnicodeString()
          >>> pos = icu.FieldPosition(icu.UDAT_YEAR_FIELD)
          >>> dest = fmt.format(icu.Calendar.get_now(), dest, pos)
          >>> print(dest[pos.get_begin_index():pos.get_end_index()])
      )doc");

  //
  // enum icu::FieldPosition::DONT_CARE
  //
  py::enum_<decltype(FieldPosition::DONT_CARE)>(fp, "FieldPosition",
                                                py::arithmetic())
      .value("DONT_CARE", FieldPosition::DONT_CARE, R"doc(
             DONT_CARE may be specified as the field to indicate that the
             caller doesn't need to specify a field.
             )doc")
      .export_values();

  //
  // class icu::FieldPosition
  //
  fp.def(py::init<>(), R"doc(
      Initialize a ``FieldPosition`` instance with a non-specified field.
      )doc")
      .def(py::init<int32_t>(), py::arg("field"), R"doc(
      Initialize a ``FieldPosition`` instance with the specified field.
      )doc")
      .def(py::init<const FieldPosition &>(), py::arg("other"), R"doc(
      Initialize a ``FieldPosition`` instance from another ``FieldPosition``.
      )doc");

  fp.def("__copy__", &FieldPosition::clone, R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  fp.def(
      "__deepcopy__",
      [](const FieldPosition &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"), R"doc(
      Return a copy of this object.

      This is equivalent to :meth:`.clone`.
      )doc");

  fp.def(
      "__eq__",
      [](const FieldPosition &self, const FieldPosition &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  fp.def(
      "__ne__",
      [](const FieldPosition &self, const FieldPosition &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  fp.def("__repr__", [](const FieldPosition &self) {
    std::stringstream ss;
    ss << "<FieldPosition(";
    ss << "field=" << self.getField();
    ss << ", begin_index=" << self.getBeginIndex();
    ss << ", end_index=" << self.getEndIndex();
    ss << ")>";
    return ss.str();
  });

  fp.def("clone", &FieldPosition::clone, R"doc(
      Return a copy of this object.

      See Also:
          :meth:`.__copy__`
          :meth:`.__deepcopy__`
      )doc");

  fp.def("get_begin_index", &FieldPosition::getBeginIndex, R"doc(
      Return the index of the first character in the requested field.
      )doc");

  fp.def("get_end_index", &FieldPosition::getEndIndex, R"doc(
      Return the index of the character following the last character in the
      requested field.
      )doc");

  fp.def("get_field", &FieldPosition::getField, R"doc(
      Return the field identifier.
      )doc");

  fp.def("set_begin_index", &FieldPosition::setBeginIndex, py::arg("bi"), R"doc(
      Set the begin index.

      For use by subclasses of :class:`Format`.
      )doc");

  fp.def("set_end_index", &FieldPosition::setEndIndex, py::arg("ei"), R"doc(
      Set the end index.

      For use by subclasses of :class:`Format`.
      )doc");

  fp.def("set_field", &FieldPosition::setField, py::arg("f"), R"doc(
      Set the field.
      )doc");
}

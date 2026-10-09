#include "main.hpp"
#include <pybind11/operators.h>
#include <sstream>
#include <unicode/parsepos.h>

using namespace icu;

void init_parsepos(py::module &m) {
  //
  // class icu::ParsePosition
  //
  py::class_<ParsePosition, UObject> pp(m, "ParsePosition", R"doc(
      ``ParsePosition`` is a simple class used by :class:`Format` and its
      subclasses to keep track of the current position during parsing.
      )doc");

  pp.def(py::init<>(), R"doc(
      Initialize a ``ParsePosition`` instance with an index of 0.
      )doc")
      .def(py::init<int32_t>(), py::arg("new_index"), R"doc(
      Initialize a ``ParsePosition`` instance with the specified index.
      )doc")
      .def(py::init<const ParsePosition &>(), py::arg("other"), R"doc(
      Initialize a ``ParsePosition`` instance from a copy of *other*.
      )doc");

  pp.def("__copy__", &ParsePosition::clone, R"doc(
      Return a copy of this object.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  pp.def(
      "__deepcopy__",
      [](const ParsePosition &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"), R"doc(
      Return a copy of this object.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  pp.def(
      "__eq__",
      [](const ParsePosition &self, const ParsePosition &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* == *other*.
      )doc");

  pp.def(
      "__ne__",
      [](const ParsePosition &self, const ParsePosition &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"), R"doc(
      Return *self* != *other*.
      )doc");

  pp.def("__repr__", [](const ParsePosition &self) {
    std::stringstream ss;
    ss << "<ParsePosition(";
    ss << "index=" << self.getIndex();
    ss << ", error_index=" << self.getErrorIndex();
    ss << ")>";
    return ss.str();
  });

  pp.def("clone", &ParsePosition::clone, R"doc(
      Return a copy of this object.

      .. seealso::

         :meth:`.__copy__`
         :meth:`.__deepcopy__`
      )doc");

  pp.def("get_error_index", &ParsePosition::getErrorIndex, R"doc(
      Return the index where the error occurred, or -1 if the error index is
      not set.
      )doc");

  pp.def("get_index", &ParsePosition::getIndex, R"doc(
      Return the current parsing position.

      - As input to a parsing method, this is the index of the character at
        which parsing will begin.
      - As output, this is the index of the character following the last
        character parsed.
      )doc");

  pp.def("set_error_index", &ParsePosition::setErrorIndex, py::arg("ei"), R"doc(
      Set the index where the parsing error occurred.

      The default value is -1 if this value is not set.
      )doc");

  pp.def("set_index", &ParsePosition::setIndex, py::arg("index"), R"doc(
      Set the current parsing position.
      )doc");
}

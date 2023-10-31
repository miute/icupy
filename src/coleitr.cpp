#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/chariter.h>
#include <unicode/coleitr.h>

using namespace icu;

void init_coleitr(py::module &m) {
  //
  // icu::CollationElementIterator
  //
  py::class_<CollationElementIterator, UObject> cei(m, "CollationElementIterator");

  py::enum_<decltype(CollationElementIterator::NULLORDER)>(cei, "CollationElementIterator", py::arithmetic())
      .value("NULLORDER", CollationElementIterator::NULLORDER,
             "*NULLORDER* indicates that an error has occurred while processing.")
      .export_values();

  cei.def(py::init<CollationElementIterator &>(), py::arg("other"));

  cei.def(
      "__eq__",
      [](const CollationElementIterator &self, const CollationElementIterator &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  cei.def("__iter__", [](CollationElementIterator &self) -> CollationElementIterator & {
    self.reset();
    return self;
  });

  cei.def(
      "__ne__",
      [](const CollationElementIterator &self, const CollationElementIterator &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  cei.def("__next__", [](CollationElementIterator &self) {
    ErrorCode error_code;
    auto result = self.next(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    } else if (result == CollationElementIterator::NULLORDER) {
      throw py::stop_iteration();
    }
    return result;
  });

  cei.def("get_max_expansion", py::overload_cast<int32_t>(&CollationElementIterator::getMaxExpansion, py::const_),
          py::arg("order"));

  cei.def("get_offset", &CollationElementIterator::getOffset);

  cei.def_static(
      "is_ignorable", [](int32_t order) -> py::bool_ { return CollationElementIterator::isIgnorable(order); },
      py::arg("order"));

  cei.def("next", [](CollationElementIterator &self) {
    ErrorCode error_code;
    auto result = self.next(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  cei.def("previous", [](CollationElementIterator &self) {
    ErrorCode error_code;
    auto result = self.previous(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  cei.def_static("primary_order", &CollationElementIterator::primaryOrder, py::arg("order"));

  cei.def("reset", &CollationElementIterator::reset);

  cei.def_static("secondary_order", &CollationElementIterator::secondaryOrder, py::arg("order"));

  cei.def(
      "set_offset",
      [](CollationElementIterator &self, int32_t new_offset) {
        ErrorCode error_code;
        self.setOffset(new_offset, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("new_offset"));

  cei.def(
         "set_text",
         [](CollationElementIterator &self, CharacterIterator &str) {
           ErrorCode error_code;
           self.setText(str, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
         },
         py::arg("str_"))
      .def(
          "set_text",
          [](CollationElementIterator &self, const icupy::UnicodeStringVariant &str) {
            ErrorCode error_code;
            self.setText(icupy::to_unistr(str), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("str_"));

  cei.def("strength_order", &CollationElementIterator::strengthOrder, py::arg("order"));

  cei.def_static("tertiary_order", &CollationElementIterator::tertiaryOrder, py::arg("order"));
}

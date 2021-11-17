#include "main.hpp"
#include <pybind11/operators.h>
#include <unicode/chariter.h>
#include <unicode/coleitr.h>

using namespace icu;

void init_coleitr(py::module &m) {
  py::class_<CollationElementIterator, UObject> cei(m, "CollationElementIterator");

  py::enum_<decltype(CollationElementIterator::NULLORDER)>(cei, "CollationElementIterator", py::arithmetic())
      .value("NULLORDER", CollationElementIterator::NULLORDER,
             "*NULLORDER* indicates that an error has occurred while processing.")
      .export_values();

  cei.def(py::init<CollationElementIterator &>(), py::arg("other"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  cei.def("__iter__",
          [](CollationElementIterator &self) -> CollationElementIterator & {
            self.reset();
            return self;
          })
      .def("__next__", [](CollationElementIterator &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.next(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        } else if (result == CollationElementIterator::NULLORDER) {
          throw py::stop_iteration();
        }
        return result;
      });
  cei.def("get_max_expansion", py::overload_cast<int32_t>(&CollationElementIterator::getMaxExpansion, py::const_),
          py::arg("order"));
  cei.def("get_offset", &CollationElementIterator::getOffset);
  cei.def_static("is_ignorable", &CollationElementIterator::isIgnorable, py::arg("order"));
  cei.def("next", [](CollationElementIterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.next(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  cei.def("previous", [](CollationElementIterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.previous(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  cei.def_static("primary_order", &CollationElementIterator::primaryOrder, py::arg("order"));
  cei.def("reset", &CollationElementIterator::reset);
  cei.def_static("secondary_order", &CollationElementIterator::secondaryOrder, py::arg("order"));
  cei.def(
      "set_offset",
      [](CollationElementIterator &self, int32_t new_offset) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setOffset(new_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("new_offset"));
  cei.def(
         "set_text",
         [](CollationElementIterator &self, CharacterIterator &str) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setText(str, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("str_"))
      .def(
          "set_text",
          [](CollationElementIterator &self, const UnicodeString &str) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setText(str, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("str_"))
      .def(
          // const char16_t *str -> const UnicodeString &str
          "set_text",
          [](CollationElementIterator &self, const char16_t *str) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setText(str, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("str_"));
  cei.def("strength_order", &CollationElementIterator::strengthOrder, py::arg("order"));
  cei.def_static("tertiary_order", &CollationElementIterator::tertiaryOrder, py::arg("order"));
}
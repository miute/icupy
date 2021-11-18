#include "main.hpp"
#include <unicode/caniter.h>
#include <unicode/errorcode.h>

using namespace icu;

void init_caniter(py::module &m) {
  // icu::CanonicalIterator
  py::class_<CanonicalIterator, UObject> ci(m, "CanonicalIterator");
  ci.def(py::init([](const UnicodeString &source) {
           ErrorCode error_code;
           auto result = std::make_unique<CanonicalIterator>(source, error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
           }
           return result;
         }),
         py::arg("source"))
      .def(
          // const char16_t *source -> const UnicodeString &source
          py::init([](const char16_t *source) {
            ErrorCode error_code;
            auto result = std::make_unique<CanonicalIterator>(source, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("source"));
  ci.def("__iter__", [](CanonicalIterator &self) -> CanonicalIterator & {
      self.reset();
      return self;
    }).def("__next__", [](CanonicalIterator &self) {
    auto result = self.next();
    if (result.isBogus()) {
      throw py::stop_iteration();
    }
    return result;
  });
  ci.def("get_source", &CanonicalIterator::getSource);
  ci.def("next", &CanonicalIterator::next);
  ci.def("reset", &CanonicalIterator::reset);
  ci.def(
        "set_source",
        [](CanonicalIterator &self, const UnicodeString &new_source) {
          ErrorCode error_code;
          self.setSource(new_source, error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
        },
        py::arg("new_source"))
      .def(
          // const char16_t *new_source -> const UnicodeString &new_source
          "set_source",
          [](CanonicalIterator &self, const char16_t *new_source) {
            ErrorCode error_code;
            self.setSource(new_source, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("new_source"));
}

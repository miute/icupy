#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/caniter.h>

using namespace icu;

void init_caniter(py::module &m) {
  //
  // icu::CanonicalIterator
  //
  py::class_<CanonicalIterator, UObject> ci(m, "CanonicalIterator");

  ci.def(py::init([](const icupy::UnicodeStringVariant &source) {
           ErrorCode error_code;
           auto result = std::make_unique<CanonicalIterator>(icupy::to_unistr(source), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }),
         py::arg("source"));

  ci.def("__iter__", [](CanonicalIterator &self) -> CanonicalIterator & {
    self.reset();
    return self;
  });

  ci.def("__next__", [](CanonicalIterator &self) {
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
      [](CanonicalIterator &self, const icupy::UnicodeStringVariant &new_source) {
        ErrorCode error_code;
        self.setSource(icupy::to_unistr(new_source), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("new_source"));
}

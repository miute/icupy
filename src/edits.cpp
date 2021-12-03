#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <unicode/edits.h>
#include <unicode/errorcode.h>

using namespace icu;
using Iterator = Edits::Iterator;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

void init_edits(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  // icu::Edits
  py::class_<Edits, UMemory> ed(m, "Edits");

  // icu::Edits::Iterator
  py::class_<Edits::Iterator, UMemory> it(ed, "Iterator");

  // icu::Edits
  ed.def(py::init<>());
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  ed.def(py::init<const Edits &>(), py::arg("other"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
  ed.def("add_replace", &Edits::addReplace, py::arg("old_length"), py::arg("new_length"));
  ed.def("add_unchanged", &Edits::addUnchanged, py::arg("unchanged_length"));
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  ed.def(
      "copy_error_to", [](const Edits &self, ErrorCode &out_error_code) { return self.copyErrorTo(out_error_code); },
      py::arg("out_error_code"));
#else
  ed.def(
      "copy_error_to", [](Edits &self, ErrorCode &out_error_code) { return self.copyErrorTo(out_error_code); },
      py::arg("out_error_code"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)
  ed.def("get_coarse_changes_iterator", &Edits::getCoarseChangesIterator);
  ed.def("get_coarse_iterator", &Edits::getCoarseIterator);
  ed.def("get_fine_changes_iterator", &Edits::getFineChangesIterator);
  ed.def("get_fine_iterator", &Edits::getFineIterator);
  ed.def("has_changes", &Edits::hasChanges);
  ed.def("length_delta", &Edits::lengthDelta);
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  ed.def(
      "merge_and_append",
      [](Edits &self, const Edits &ab, const Edits &bc) -> Edits & {
        ErrorCode error_code;
        auto &result = self.mergeAndAppend(ab, bc, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("ab"), py::arg("bc"));
  ed.def("number_of_changes", &Edits::numberOfChanges);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
  ed.def("reset", &Edits::reset);

  // icu::Edits::Iterator
  // Omit "icu::Edits::Iterator::Iterator()".
  it.def(py::init<const Iterator &>(), py::arg("other"));
  it.def("destination_index", &Iterator::destinationIndex);
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  it.def(
      "destination_index_from_source_index",
      [](Iterator &self, int32_t i) {
        ErrorCode error_code;
        auto result = self.destinationIndexFromSourceIndex(i, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("i"));
  it.def(
      "find_destination_index",
      [](Iterator &self, int32_t i) {
        ErrorCode error_code;
        auto result = self.findDestinationIndex(i, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("i"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)
  it.def(
      "find_source_index",
      [](Iterator &self, int32_t i) {
        ErrorCode error_code;
        auto result = self.findSourceIndex(i, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("i"));
  it.def("has_change", &Iterator::hasChange);
  it.def("new_length", &Iterator::newLength);
  it.def("next", [](Iterator &self) {
    ErrorCode error_code;
    auto result = self.next(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  it.def("old_length", &Iterator::oldLength);
  it.def("replacement_index", &Iterator::replacementIndex);
  it.def("source_index", &Iterator::sourceIndex);
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  it.def(
      "source_index_from_destination_index",
      [](Iterator &self, int32_t i) {
        ErrorCode error_code;
        auto result = self.sourceIndexFromDestinationIndex(i, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("i"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
}

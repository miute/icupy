#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <unicode/edits.h>

using namespace icu;
using Iterator = Edits::Iterator;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

void init_edits(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  //
  // class icu::Edits
  //
  py::class_<Edits, UMemory> ed(m, "Edits", R"doc(
      ``Edit`` records the edit history of a string, but does not record
      replacement text.

      It supports replacements, insertions, and deletions in linear order.
      It does not support moving or reordering text.
      )doc");

  //
  // class icu::Edits::Iterator
  //
  py::class_<Edits::Iterator, UMemory> it(ed, "Iterator", R"doc(
      Access to the list of edits.
      )doc");

  //
  // class icu::Edits
  //
  ed.def(py::init<>(), R"doc(
      Initialize a ``Edits`` instance.
      )doc");
#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  ed.def(py::init<const Edits &>(), py::arg("other"), R"doc(
      Initialize a ``Edits`` instance from a copy of *other*.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  ed.def("add_replace", &Edits::addReplace, py::arg("old_length"),
         py::arg("new_length"), R"doc(
      Add a change edit: a record related to text replacement, insertion, and
      deletion.

      .. note::

         Normally, this method is not called from user code, but from within
         the ICU string transformation functions.
      )doc");

  ed.def("add_unchanged", &Edits::addUnchanged, py::arg("unchanged_length"),
         R"doc(
      Add an unchanged edit: a record of text that has not been changed.

      .. note::

         Normally, this method is not called from user code, but from within
         the ICU string transformation functions.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  ed.def(
      "copy_error_to",
      [](const Edits &self, ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Copy the internal :class:`UErrorCode` to *out_error_code*; return
      ``True`` if :class:`UErrorCode` indicates a failure, ``False``
      otherwise.

      If *out_error_code* already contains an error, its value will not be
      changed.
      )doc");
#else
  ed.def(
      "copy_error_to",
      [](Edits &self, ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Copy the internal :class:`UErrorCode` to *out_error_code*; return
      ``True`` if :class:`UErrorCode` indicates a failure, ``False``
      otherwise.

      If *out_error_code* already contains an error, its value will not be
      changed.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

  ed.def("get_coarse_changes_iterator", &Edits::getCoarseChangesIterator, R"doc(
      Return an iterator for coarse-grained edit changes
      (adjacent change edits are treated as a single edit).
      )doc");

  ed.def("get_coarse_iterator", &Edits::getCoarseIterator, R"doc(
      Return an iterator for edits with and without coarse-grained changes
      (adjacent change edits are treated as a single edit).
      )doc");

  ed.def("get_fine_changes_iterator", &Edits::getFineChangesIterator, R"doc(
      Return an iterator for fine-grained edit changes
      (full granularity of change edits are retained).
      )doc");

  ed.def("get_fine_iterator", &Edits::getFineIterator, R"doc(
      Return an iterator for edits with and without fine-grained changes
      (full granularity of change edits are retained).
      )doc");

  ed.def(
      "has_changes",
      [](const Edits &self) -> py::bool_ { return self.hasChanges(); }, R"doc(
      Return ``True`` if there are changes, ``False`` otherwise.
      )doc");

  ed.def("length_delta", &Edits::lengthDelta, R"doc(
      Return the result of subtracting the old length from the new length.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  ed.def(
      "merge_and_append",
      [](Edits &self, const Edits &ab, const Edits &bc) -> Edits & {
        ErrorCode error_code;
        auto &result = self.mergeAndAppend(ab, bc, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("ab"), py::arg("bc"), R"doc(
      Merge the two input ``Edits`` specified by *ab* and *bc*, append the
      result to this ``Edits``, and return ``Edits`` itself.

      *ab* represents which substring of the input string a corresponds to
      which substring of the intermediate string b.

      *bc* represents which substring of the intermediate string b corresponds
      to which substring of the output string c.
      )doc");

  ed.def("number_of_changes", &Edits::numberOfChanges, R"doc(
      Return the number of changes.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  ed.def("reset", &Edits::reset, R"doc(
      Reset the data.
      )doc");

  //
  // class icu::Edits::Iterator
  //
  // Omit "icu::Edits::Iterator::Iterator()".
  it.def(py::init<const Iterator &>(), py::arg("other"), R"doc(
      Initialize an ``Iterator`` instance from a copy of *other*.
      )doc");

  it.def("destination_index", &Iterator::destinationIndex, R"doc(
      Return the start index of the current span in the destination string.

      The length of this span is :meth:`.new_length`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  it.def(
      "destination_index_from_source_index",
      [](Iterator &self, int32_t i) {
        ErrorCode error_code;
        auto result = self.destinationIndexFromSourceIndex(i, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("i"), R"doc(
      Return the destination index corresponding to the source index
      specified by *i*; if *i* is outside the range from 0 to the string
      length, the result is undefined.

      *i* is the source index.

      .. note::

         This operation will usually, but not always, modify this object.
         The state of the iterator after this search is undefined.
      )doc");

  it.def(
      "find_destination_index",
      [](Iterator &self, int32_t i) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.findDestinationIndex(i, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("i"), R"doc(
      Move the iterator to the edit position containing the destination index
      specified by *i*; return ``True`` if an edit for the destination index is
      found, ``False`` otherwise.

      *i* is the destination index.

      .. note::

         The iterator state after this search is undefined if the destination
         index is outside the range of the destination string.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

  it.def(
      "find_source_index",
      [](Iterator &self, int32_t i) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.findSourceIndex(i, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("i"), R"doc(
      Move the iterator to the edit position containing the source index
      specified by *i*; return ``True`` if an edit for the source index is
      found, ``False`` otherwise.

      *i* is the source index.

      .. note::

         The iterator state after this search is undefined if the source
         index is outside the range of the source string.
      )doc");

  it.def(
      "has_change",
      [](const Iterator &self) -> py::bool_ { return self.hasChange(); }, R"doc(
      Return ``True`` if the current span has changes, ``False`` otherwise.
      )doc");

  it.def("new_length", &Iterator::newLength, R"doc(
      Return the length of the current span in the destination string.
      )doc");

  it.def(
      "next",
      [](Iterator &self) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.next(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Move the iterator to the next span; return ``True`` if there are more
      edits, ``False`` otherwise.
      )doc");

  it.def("old_length", &Iterator::oldLength, R"doc(
      Return the length of the current span in the source string.
      )doc");

  it.def("replacement_index", &Iterator::replacementIndex, R"doc(
      Return the start index of the current span within the replacement string.

      This is Well-defined only if the current edit is a change edit.

      The length of this span is :meth:`.new_length`.
      )doc");

  it.def("source_index", &Iterator::sourceIndex, R"doc(
      Return the start index of the current span within the source string.

      The length of this span is :meth:`.old_length`.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 60)
  it.def(
      "source_index_from_destination_index",
      [](Iterator &self, int32_t i) {
        ErrorCode error_code;
        auto result = self.sourceIndexFromDestinationIndex(i, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("i"), R"doc(
      Return the source index corresponding to the destination index
      specified by *i*; if *i* is outside the range from 0 to the string
      length, the result is undefined.

      *i* is the destination index.

      .. note::

         This operation will usually, but not always, modify this object.
         The state of the iterator after this search is undefined.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 60)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
}

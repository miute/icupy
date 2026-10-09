#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <sstream>
#include <unicode/formattedvalue.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_formattedvalue(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // class icu::ConstrainedFieldPosition
  //
  py::class_<ConstrainedFieldPosition, UMemory> cfp(
      m, "ConstrainedFieldPosition", R"doc(
      Represent a span of a string containing the specified field.
      )doc");

  cfp.def(py::init<>(), R"doc(
      Initialize a ``ConstrainedFieldPosition`` instance.
      )doc");

  cfp.def(
      "__repr__",
      [](const ConstrainedFieldPosition &self) {
        std::stringstream ss;
        ss << "<ConstrainedFieldPosition(";
        ss << "category=0x" << std::hex << self.getCategory();
        ss << ", field=" << std::dec << self.getField();
        ss << ", start=" << self.getStart();
        ss << ", limit=" << self.getLimit();
        ss << ")>";
        return ss.str();
      },
      R"doc(
      Return a string representation of this object.
      )doc");

  cfp.def("constrain_category", &ConstrainedFieldPosition::constrainCategory,
          py::arg("category"), R"doc(
      Set a constraint on the field category.

      *category* is the field category to modify when iterating.

      When this instance of ``ConstrainedFieldPosition`` is passed to
      :meth:`FormattedValue.next_position`, positions are skipped unless they
      have the specified category.

      Any previously set constraints are cleared.

      Changing the constraint while in the middle of iterating over a
      :class:`FormattedValue` does not generally have well-defined behavior.

      .. rubric:: Example

      To loop over only the number-related fields:

      .. code-block:: python

         from icupy import icu
         fmt = (
             icu.number.NumberFormatter.with_()
             .notation(icu.number.Notation.compact_short())
             .unit(icu.MeasureUnit.get_kelvin())
             .locale(icu.ULOC_US)
         )
         fmtval = fmt.format_double(65000)
         cfpos = icu.ConstrainedFieldPosition()
         cfpos.constrain_category(icu.UFIELD_CATEGORY_NUMBER)
         while fmtval.next_position(cfpos):
             pass  # do something with cfpos
      )doc");

  cfp.def("constrain_field", &ConstrainedFieldPosition::constrainField,
          py::arg("category"), py::arg("field"), R"doc(
      Set a constraint on the category and field.

      *category* is the field category to modify when iterating.

      *field* is the field to modify when iterating.

      When this instance of ``ConstrainedFieldPosition`` is passed to
      :meth:`FormattedValue.next_position`, positions are skipped unless they
      have the specified category and field.

      Any previously set constraints are cleared.

      Changing the constraint while in the middle of iterating over a
      :class:`FormattedValue` does not generally have well-defined behavior.

      .. rubric:: Example

      To loop over only the number-related fields:

      .. code-block:: python

         from icupy import icu
         fmt = (
             icu.number.NumberFormatter.with_()
             .notation(icu.number.Notation.compact_short())
             .unit(icu.MeasureUnit.get_kelvin())
             .locale(icu.ULOC_US)
         )
         fmtval = fmt.format_double(65000)
         cfpos = icu.ConstrainedFieldPosition()
         cfpos.constrain_field(icu.UFIELD_CATEGORY_NUMBER, icu.UNUM_INTEGER_FIELD)
         while fmtval.next_position(cfpos):
             pass  # do something with cfpos
      )doc");

  cfp.def("get_category", &ConstrainedFieldPosition::getCategory, R"doc(
      Return the field category for the current position.

      The return value is well-defined only after
      :meth:`FormattedValue.next_position` returns ``True``.
      )doc");

  cfp.def("get_field", &ConstrainedFieldPosition::getField, R"doc(
      Return the field for the current position.

      The return value is well-defined only after
      :meth:`FormattedValue.next_position` returns ``True``.
      )doc");

  cfp.def("get_int64_iteration_context",
          &ConstrainedFieldPosition::getInt64IterationContext, R"doc(
      Return an ``int64`` that :class:`FormattedValue` implementations may use
      for storage.

      The initial value is zero.

      .. note::

         Users should not need to call this method.
      )doc");

  cfp.def("get_limit", &ConstrainedFieldPosition::getLimit, R"doc(
      Return the end index (exclusive) stored for the current position.

      The return value is well-defined only after
      :meth:`FormattedValue.next_position` returns ``True``.
      )doc");

  cfp.def("get_start", &ConstrainedFieldPosition::getStart, R"doc(
      Return the start index (inclusive) for the current position.

      The return value is well-defined only after
      :meth:`FormattedValue.next_position` returns ``True``.
      )doc");

  cfp.def(
      "matches_field",
      [](const ConstrainedFieldPosition &self, int32_t category, int32_t field)
          -> py::bool_ { return self.matchesField(category, field); },
      py::arg("category"), py::arg("field"), R"doc(
      Return ``True`` if the constraint includes the specified category and
      field, ``False`` otherwise.

      *category* is the field category to test.

      *field* is the field to test.
      )doc");

  cfp.def("reset", &ConstrainedFieldPosition::reset, R"doc(
      Reset the constraint to its initial state, as if it were newly created:

      - Remove any constraints that may have been set on the instance.
      - Reset the iteration position.
      )doc");

  cfp.def("set_int64_iteration_context",
          &ConstrainedFieldPosition::setInt64IterationContext,
          py::arg("context"), R"doc(
      Set an ``int64`` that :class:`FormattedValue` implementations may use for
      storage.

      *context* is a new iteration context.

      .. note::

         Users should not need to call this method.
      )doc");

  cfp.def("set_state", &ConstrainedFieldPosition::setState, py::arg("category"),
          py::arg("field"), py::arg("start"), py::arg("limit"), R"doc(
      Set new values for the primary public getters.

      *category* is a new field category.

      *field* is a new field.

      *start* is a new start index (inclusive).

      *limit* is a new end index (exclusive).

      .. note::

         Users should not need to call this method.
      )doc");

  //
  // class icu::FormattedValue
  //
  py::class_<FormattedValue> fv(m, "FormattedValue", R"doc(
      Abstract formatted value: a string with associated field attributes.

      .. seealso::

         :class:`FormattedDateInterval`
         :class:`FormattedList`
         :class:`FormattedRelativeDateTime`
         :class:`number.FormattedNumber`
         :class:`number.FormattedNumberRange`
      )doc");

  fv.def(
      "__str__",
      [](const FormattedValue &self) {
        ErrorCode error_code;
        auto str = self.toString(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        std::string result;
        str.toUTF8String(result);
        return result;
      },
      R"doc(
      Return a string representation of this object.

      This is equivalent to calling ``to_string().to_utf8_string()``.

      .. seealso::

         :meth:`.to_string`
      )doc");

  fv.def(
      "append_to",
      [](FormattedValue &self, Appendable &appendable) -> Appendable & {
        ErrorCode error_code;
        auto &result = self.appendTo(appendable, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"), R"doc(
      Append the formatted string to :class:`Appendable` specified by
      *appendable*.
      )doc");

  fv.def(
      "next_position",
      [](const FormattedValue &self,
         ConstrainedFieldPosition &cfpos) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.nextPosition(cfpos, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("cfpos"), R"doc(
      Iterate through the field positions within ``FormattedValue``;
      return ``True`` if a new occurrence of that field is found, ``False``
      otherwise or if an error occurs.

      *cfpos* is used to represent the iteration state, and can be used to set
      constraints that limit the iteration to specific category or field.

      .. seealso::

         :meth:`ConstrainedFieldPosition.constrain_category`
         :meth:`ConstrainedFieldPosition.constrain_field`

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         fmt = (
             icu.number.NumberFormatter.with_()
             .notation(icu.number.Notation.compact_short())
             .unit(icu.MeasureUnit.get_kelvin())
             .locale(icu.ULOC_US)
         )
         fmtval = fmt.format_double(65000)
         cfpos = icu.ConstrainedFieldPosition()
         while fmtval.next_position(cfpos):
             pass  # do something with cfpos
      )doc");

  fv.def(
      "to_string",
      [](const FormattedValue &self) {
        ErrorCode error_code;
        auto result = self.toString(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the formatted string as a self-contained :class:`UnicodeString`.

      .. seealso::

         :meth:`.__str__`
      )doc");

  // TODO: Deprecate FormattedValue.to_temp_string().
  fv.def(
      "to_temp_string",
      [](const FormattedValue &self) {
        ErrorCode error_code;
        auto result = self.toTempString(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the formatted string as a temporary :class:`UnicodeString`.

      .. version-deprecated:: 0.23
         Use :meth:`.to_string` instead.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

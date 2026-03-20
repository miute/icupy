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
      Represent a span of a string containing a given field.
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
      Return a string representation of the object.
      )doc");

  cfp.def("constrain_category", &ConstrainedFieldPosition::constrainCategory,
          py::arg("category"), R"doc(
      Set a constraint on the field category.

      When this instance of ``ConstrainedFieldPosition`` is passed to
      :meth:`FormattedValue.next_position`, positions are skipped unless they
      have the given category.

      Any previously set constraints are cleared.

      Changing the constraint while in the middle of iterating over a
      :class:`FormattedValue` does not generally have well-defined behavior.

      Example:
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

      When this instance of ``ConstrainedFieldPosition`` is passed to
      :meth:`FormattedValue.next_position`, positions are skipped unless they
      have the given category and field.

      Any previously set constraints are cleared.

      Changing the constraint while in the middle of iterating over a
      :class:`FormattedValue` does not generally have well-defined behavior.

      Example:
          To loop over only the number-related fields:

          .. code-block:: python

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

      Users of :class:`FormattedValue` should not need to call this method.
      )doc");

  cfp.def("get_limit", &ConstrainedFieldPosition::getLimit, R"doc(
      Return the EXCLUSIVE end index stored for the current position.

      The return value is well-defined only after
      :meth:`FormattedValue.next_position` returns ``True``.
      )doc");

  cfp.def("get_start", &ConstrainedFieldPosition::getStart, R"doc(
      Return the INCLUSIVE start index for the current position.

      The return value is well-defined only after
      :meth:`FormattedValue.next_position` returns ``True``.
      )doc");

  cfp.def(
      "matches_field",
      [](const ConstrainedFieldPosition &self, int32_t category, int32_t field)
          -> py::bool_ { return self.matchesField(category, field); },
      py::arg("category"), py::arg("field"), R"doc(
      Determine whether a given field should be included given the constraints.
      )doc");

  cfp.def("reset", &ConstrainedFieldPosition::reset, R"doc(
      Reset this ``ConstrainedFieldPosition`` to its initial state, as if it
      were newly created:

      - Remove any constraints that may have been set on the instance.
      - Reset the iteration position.
      )doc");

  cfp.def("set_int64_iteration_context",
          &ConstrainedFieldPosition::setInt64IterationContext,
          py::arg("context"), R"doc(
      Set an ``int64`` that :class:`FormattedValue` implementations may use for
      storage.
      )doc");

  cfp.def("set_state", &ConstrainedFieldPosition::setState, py::arg("category"),
          py::arg("field"), py::arg("start"), py::arg("limit"), R"doc(
      Set new values for the primary public getters.

      Intended to be used by :class:`FormattedValue` implementations.
      )doc");

  //
  // class icu::FormattedValue
  //
  py::class_<FormattedValue> fv(m, "FormattedValue", R"doc(
      Abstract formatted value: a string with associated field attributes.

      See Also:
          :class:`FormattedDateInterval`
          :class:`FormattedList`
          :class:`FormattedRelativeDateTime`
          :class:`number.FormattedNumber`
          :class:`number.FormattedNumberRange`
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
      Append the formatted string to *appendable*.
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
      Iterate through the field positions within ``FormattedValue``.

      This allows you to identify the positions of specific types of
      substrings, such as the month or decimal separator.

      Example:
          >>> from icupy import icu
          >>> fmt = (
          ...     icu.number.NumberFormatter.with_()
          ...     .notation(icu.number.Notation.compact_short())
          ...     .unit(icu.MeasureUnit.get_kelvin())
          ...     .locale(icu.ULOC_US)
          ... )
          >>> fmtval = fmt.format_double(65000)
          >>> cfpos = icu.ConstrainedFieldPosition()
          >>> while fmtval.next_position(cfpos):
          ...     pass  # do something with cfpos
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
      .. deprecated:: 0.23
          Use :meth:`.to_string` instead.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

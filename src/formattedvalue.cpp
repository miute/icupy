#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <sstream>
#include <unicode/formattedvalue.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_formattedvalue(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  // icu::ConstrainedFieldPosition
  py::class_<ConstrainedFieldPosition, UMemory> cfp(m, "ConstrainedFieldPosition");
  cfp.def(py::init<>());
  cfp.def("__repr__", [](const ConstrainedFieldPosition &self) {
    std::stringstream ss;
    ss << "ConstrainedFieldPosition(";
    ss << "category=0x" << std::hex << self.getCategory();
    ss << ", field=" << std::dec << self.getField();
    ss << ", start=" << self.getStart();
    ss << ", limit=" << self.getLimit();
    ss << ")";
    return ss.str();
  });
  cfp.def("constrain_category", &ConstrainedFieldPosition::constrainCategory, py::arg("category"));
  cfp.def("constrain_field", &ConstrainedFieldPosition::constrainField, py::arg("category"), py::arg("field"));
  cfp.def("get_category", &ConstrainedFieldPosition::getCategory);
  cfp.def("get_field", &ConstrainedFieldPosition::getField);
  cfp.def("get_int64_iteration_context", &ConstrainedFieldPosition::getInt64IterationContext);
  cfp.def("get_limit", &ConstrainedFieldPosition::getLimit);
  cfp.def("get_start", &ConstrainedFieldPosition::getStart);
  cfp.def("matches_field", &ConstrainedFieldPosition::matchesField, py::arg("category"), py::arg("field"));
  cfp.def("reset", &ConstrainedFieldPosition::reset);
  cfp.def("set_int64_iteration_context", &ConstrainedFieldPosition::setInt64IterationContext, py::arg("context"));
  cfp.def("set_state", &ConstrainedFieldPosition::setState, py::arg("category"), py::arg("field"), py::arg("start"),
          py::arg("limit"));

  // icu::FormattedValue
  py::class_<FormattedValue> fv(m, "FormattedValue");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

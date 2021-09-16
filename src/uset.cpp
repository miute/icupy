#include "main.hpp"
#include "usetptr.hpp"
#include <memory>

_ConstUSetPtr::_ConstUSetPtr(const USet *p) : p_(p) {}
_ConstUSetPtr::~_ConstUSetPtr() {}
const USet *_ConstUSetPtr::get() const { return p_; }

_USetPtr::_USetPtr(USet *p) : p_(p) {}
_USetPtr::~_USetPtr() {}
USet *_USetPtr::get() const { return p_; }

void init_uset(py::module &m) {
  py::enum_<USetSpanCondition>(m, "USetSpanCondition", py::arithmetic())
      .value("USET_SPAN_NOT_CONTAINED", USET_SPAN_NOT_CONTAINED)
      .value("USET_SPAN_CONTAINED", USET_SPAN_CONTAINED)
      .value("USET_SPAN_SIMPLE", USET_SPAN_SIMPLE)
#ifndef U_HIDE_DEPRECATED_API
      .value("USET_SPAN_CONDITION_COUNT", USET_SPAN_CONDITION_COUNT)
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::class_<_ConstUSetPtr>(m, "_ConstUSetPtr");

  py::class_<_USetPtr>(m, "_USetPtr");

  py::class_<USerializedSet>(m, "USerializedSet").def(py::init<>());

  m.attr("USET_IGNORE_SPACE") = (int32_t)USET_IGNORE_SPACE;
  m.attr("USET_CASE_INSENSITIVE") = (int32_t)USET_CASE_INSENSITIVE;
  m.attr("USET_ADD_CASE_MAPPINGS") = (int32_t)USET_ADD_CASE_MAPPINGS;

  m.attr("USET_SERIALIZED_STATIC_ARRAY_CAPACITY") = (int32_t)USET_SERIALIZED_STATIC_ARRAY_CAPACITY;
}

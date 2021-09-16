#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include "ucpmapptr.hpp"
#include "voidptr.hpp"
#include <pybind11/stl.h>

using namespace icu;

_ConstUCPMapPtr::_ConstUCPMapPtr(const UCPMap *p) : p_(p) {}
_ConstUCPMapPtr::~_ConstUCPMapPtr() {}
const UCPMap *_ConstUCPMapPtr::get() const { return p_; }

_UCPMapValueFilterPtr::_UCPMapValueFilterPtr(std::nullptr_t filter) : action_(filter) {}
_UCPMapValueFilterPtr::_UCPMapValueFilterPtr(const py::function &filter) : action_(filter) {}
_UCPMapValueFilterPtr::~_UCPMapValueFilterPtr() {}

uint32_t _UCPMapValueFilterPtr::filter(const void *context, uint32_t value) {
  auto cp = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
  auto python_context = cp->to_object();
  auto &action = cp->get_action();
  return action(python_context, value).cast<uint32_t>();
}
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_ucpmap(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  py::enum_<UCPMapRangeOption>(m, "UCPMapRangeOption", py::arithmetic())
      .value("UCPMAP_RANGE_NORMAL", UCPMAP_RANGE_NORMAL)
      .value("UCPMAP_RANGE_FIXED_LEAD_SURROGATES", UCPMAP_RANGE_FIXED_LEAD_SURROGATES)
      .value("UCPMAP_RANGE_FIXED_ALL_SURROGATES", UCPMAP_RANGE_FIXED_ALL_SURROGATES)
      .export_values();

  py::class_<_ConstUCPMapPtr>(m, "_ConstUCPMapPtr");

  py::class_<_UCPMapValueFilterPtr>(m, "UCPMapValueFilterPtr")
      .def(py::init<py::function &>(), py::arg("filter_"))
      .def(py::init<std::nullptr_t>(), py::arg("filter_"));

  m.def(
      "ucpmap_get", [](_ConstUCPMapPtr &map, UChar32 c) { return ucpmap_get(map, c); }, py::arg("map_"), py::arg("c"));
  m.def(
      "ucpmap_get_range",
      [](_ConstUCPMapPtr &map, UChar32 start, UCPMapRangeOption option, uint32_t surrogate_value,
         _UCPMapValueFilterPtr &filter, _ConstVoidPtr &context) {
        UCPMapValueFilter *fp = nullptr;
        void *cp = nullptr;
        if (filter.has_value()) {
          fp = filter.filter;
          context.set_action(filter.get<py::function>());
          cp = &context;
        }

        uint32_t value;
        auto result = ucpmap_getRange(map, start, option, surrogate_value, fp, cp, &value);
        return py::make_tuple(result, value);
      },
      py::arg("map_"), py::arg("start"), py::arg("option"), py::arg("surrogate_value"), py::arg("filter_"),
      py::arg("context"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

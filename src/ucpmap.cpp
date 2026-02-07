#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
#include "ucpmapptr.hpp"
#include <pybind11/stl.h>

using namespace icu;

namespace icupy {

//
// struct UCPMap
//
UCPMapPtr::UCPMapPtr(const UCPMap *p) : p_(p) {}

UCPMapPtr::~UCPMapPtr() {}

const UCPMap *UCPMapPtr::get() const { return p_; }

//
// UCPMapValueFilter
//
uint32_t UCPMapValueFilterPtr::filter(const void *native_context,
                                      uint32_t value) {
  if (native_context == nullptr) {
    throw std::runtime_error("UCPMapValueFilter: context is not set");
  }
  auto pair = reinterpret_cast<ValueFilterAndContextPair *>(
      const_cast<void *>(native_context));
  auto &action = pair->first;
  if (!action) {
    throw std::runtime_error(
        "UCPMapValueFilter: callback function is not set or invalid");
  }
  auto context = pair->second;
  auto object = context ? context->value() : py::none();
  return action(object, value);
}

} // namespace icupy

#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)

void init_ucpmap(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 63)
  //
  // enum UCPMapRangeOption
  //
  py::enum_<UCPMapRangeOption>(
      m, "UCPMapRangeOption", py::arithmetic(),
      "Selectors for how *ucpmap_get_range()* etc.\n\n"
      "Should report value ranges overlapping with surrogates. Most users "
      "should use *UCPMAP_RANGE_NORMAL*.")
      .value("UCPMAP_RANGE_NORMAL", UCPMAP_RANGE_NORMAL,
             "*ucpmap_get_range()* enumerates all same-value ranges as stored "
             "in the map.\n\n  "
             "Most users should use this option.")
      .value("UCPMAP_RANGE_FIXED_LEAD_SURROGATES",
             UCPMAP_RANGE_FIXED_LEAD_SURROGATES,
             "*ucpmap_get_range()* enumerates all same-value ranges as stored "
             "in the map, except that lead surrogates "
             "(U+D800..U+DBFF) are treated as having the *surrogate_value*, "
             "which is passed to *get_range()* as a "
             "separate parameter.\n\n  "
             "The *surrogate_value* is not transformed via *filter()*. See "
             "*u_is_lead(c)*.\n\n  "
             "Most users should use *UCPMAP_RANGE_NORMAL* instead.\n\n  "
             "This option is useful for maps that map surrogate code units to "
             "special values optimized for UTF-16 "
             "string processing or for special error behavior for unpaired "
             "surrogates, but those values are not to be "
             "associated with the lead surrogate code points.")
      .value("UCPMAP_RANGE_FIXED_ALL_SURROGATES",
             UCPMAP_RANGE_FIXED_ALL_SURROGATES,
             "*ucpmap_get_range()* enumerates all same-value ranges as stored "
             "in the map, except that all surrogates "
             "(U+D800..U+DFFF) are treated as having the *surrogate_value*, "
             "which is passed to *get_range()* as a "
             "separate parameter.\n\n  "
             "The *surrogate_value* is not transformed via *filter()*. See "
             "*u_is_surrogate(c)*.\n\n  "
             "Most users should use *UCPMAP_RANGE_NORMAL* instead.\n\n  "
             "This option is useful for maps that map surrogate code units to "
             "special values optimized for UTF-16 "
             "string processing or for special error behavior for unpaired "
             "surrogates, but those values are not to be "
             "associated with the lead surrogate code points.")
      .export_values();

  //
  // struct UCPMap
  //
  py::class_<icupy::UCPMapPtr>(m, "UCPMap", R"doc(
    Abstract map from Unicode code points [U+0000, U+10FFFF] to integer values.

    See Also:
        :func:`u_get_int_property_map`
    )doc");

  //
  // UCPMapValueFilter
  //
  py::class_<icupy::UCPMapValueFilterPtr> vf(m, "UCPMapValueFilter", R"doc(
    Wrapper class for a callback function that modify map data value.

    See Also:
        :func:`ucpmap_get_range`
    )doc");

  vf.def(py::init([](const icupy::ValueFilterFunction &action,
                     std::optional<const icupy::ConstVoidPtr *> context) {
           return std::make_unique<icupy::UCPMapValueFilterPtr>(
               action, context.value_or(nullptr));
         }),
         py::arg("action"), py::arg("context") = std::nullopt, R"doc(
         Initialize the ``UCPMapValueFilter`` instance with a callback
         function `action` and the user context `context`.

         `action` and `context` must outlive the ``UCPMapValueFilter``
         object.
         )doc");

  vf.def(
      "context",
      [](const icupy::UCPMapValueFilterPtr &self)
          -> std::optional<const icupy::ConstVoidPtr *> {
        auto pair = self.context();
        if (pair == nullptr || pair->second == nullptr) {
          return std::nullopt;
        }
        return reinterpret_cast<const icupy::ConstVoidPtr *>(pair->second);
      },
      py::return_value_policy::reference,
      R"doc(
      Get the user context.
      )doc");

  //
  // Functions
  //
  m.def(
      "ucpmap_get",
      [](const icupy::UCPMapPtr &map, UChar32 c) { return ucpmap_get(map, c); },
      py::arg("ucpmap"), py::arg("c"), R"doc(
      Get the property value for a code point in a map.

      `c` must be between 0 and 0x10FFFF.

      See Also:
          :func:`u_get_int_property_map`
      )doc");

  m.def(
      "ucpmap_get_range",
      [](const icupy::UCPMapPtr &map, UChar32 start, UCPMapRangeOption option,
         uint32_t surrogate_value,
         std::optional<icupy::UCPMapValueFilterPtr> &filter) {
        UCPMapValueFilter *new_filter = nullptr;
        const void *new_context = nullptr;
        if (filter.has_value()) {
          new_filter = filter->get_native_callback();
          new_context = filter->context();
        }
        uint32_t value = 0;
        auto result = ucpmap_getRange(map, start, option, surrogate_value,
                                      new_filter, new_context, &value);
        return std::make_tuple(result, value);
      },
      py::arg("ucpmap"), py::arg("start"), py::arg("option"),
      py::arg("surrogate_value"), py::arg("filter") = std::nullopt, R"doc(
      Get the last code point and property value in the range that has
      the same property value as the code point starting at `start`
      as a tuple ``(end, value)``.

      See Also:
          :func:`u_get_int_property_map`

      Examples:
          >>> from icupy import icu
          >>> ucpmap = icu.u_get_int_property_map(icu.UCHAR_EAST_ASIAN_WIDTH)
          >>> result: list[tuple[int, int, icu.UEastAsianWidth]] = []
          >>> start = 0
          >>> while start <= 0x10fff:
          ...     end, value = icu.ucpmap_get_range(ucpmap, start, icu.UCPMAP_RANGE_NORMAL, 0)
          ...     if end < 0:
          ...         break
          ...     result.append((start, end, icu.UEastAsianWidth(value)))
          ...     start = end + 1
          ...
          >>> [x for x in result if x[2] == icu.U_EA_FULLWIDTH]
          [(12288, 12288, <UEastAsianWidth.U_EA_FULLWIDTH: 3>), (65281, 65376, <UEastAsianWidth.U_EA_FULLWIDTH: 3>), (65504, 65510, <UEastAsianWidth.U_EA_FULLWIDTH: 3>)]

          >>> from icupy import icu
          >>> def my_filter(new_map: dict[int, int], value: int) -> int:
          ...     return new_map.get(value, value)
          ...
          >>> ucpmap = icu.u_get_int_property_map(icu.UCHAR_EAST_ASIAN_WIDTH)
          >>> eaw_map: dict[int, int] = {int(icu.U_EA_AMBIGUOUS): int(icu.U_EA_FULLWIDTH)}
          >>> context = icu.ConstVoidPtr(eaw_map)
          >>> action = icu.UCPMapValueFilter(my_filter, context)
          >>> result: list[tuple[int, int, icu.UEastAsianWidth]] = []
          >>> start = 0
          >>> while start <= 0x10fff:
          ...     end, value = icu.ucpmap_get_range(ucpmap, start, icu.UCPMAP_RANGE_NORMAL, 0, action)
          ...     if end < 0:
          ...         break
          ...     result.append((start, end, icu.UEastAsianWidth(value)))
          ...     start = end + 1
          ...
          >>> len([x for x in result if x[2] == icu.U_EA_FULLWIDTH])
          173
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 63)
}

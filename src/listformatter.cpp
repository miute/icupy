#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/stl.h>
#include <unicode/listformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_listformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // icu::FormattedList
  //
  py::class_<FormattedList, UMemory, FormattedValue> fl(m, "FormattedList");

  fl.def(py::init<>());
  // FIXME: Implement "icu::FormattedList::FormattedList(FormattedList &&src)".
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // icu::ListFormatter
  //
  py::class_<ListFormatter, UObject> lf(m, "ListFormatter");

#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  lf.def(py::init<const ListFormatter &>(), py::arg("other"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)

  lf.def_static(
        "create_instance",
        [](const icupy::LocaleVariant &locale) {
          ErrorCode error_code;
          auto result = ListFormatter::createInstance(icupy::to_locale(locale), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
      .def_static("create_instance", []() {
        ErrorCode error_code;
        auto result = ListFormatter::createInstance(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  lf.def_static(
      "create_instance",
      [](const icupy::LocaleVariant &locale, UListFormatterType type, UListFormatterWidth width) {
        ErrorCode error_code;
        auto result = ListFormatter::createInstance(icupy::to_locale(locale), type, width, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("type_"), py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)

  lf.def(
      "format",
      [](const ListFormatter &self, const std::list<UnicodeString> &items, int32_t count,
         UnicodeString &append_to) -> UnicodeString & {
        std::vector<UnicodeString> _items(items.begin(), items.end());
        if (count == -1) {
          count = static_cast<int32_t>(items.size());
        }
        ErrorCode error_code;
        auto &result = self.format(_items.data(), count, append_to, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("items"), py::arg("count"), py::arg("append_to"));

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  lf.def(
      "format_strings_to_value",
      [](const ListFormatter &self, const std::list<UnicodeString> &items, int32_t count) {
        std::vector<UnicodeString> _items(items.begin(), items.end());
        if (count == -1) {
          count = static_cast<int32_t>(items.size());
        }
        ErrorCode error_code;
        auto result = self.formatStringsToValue(_items.data(), count, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("items"), py::arg("count") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

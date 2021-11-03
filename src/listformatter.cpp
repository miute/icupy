#include "main.hpp"
#include <pybind11/stl.h>

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <unicode/listformatter.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_listformatter(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  // icu::FormattedList
  py::class_<FormattedList, UMemory, FormattedValue> fl(m, "FormattedList");
  fl.def(py::init<>());
  // FIXME: Implement "icu::FormattedList::FormattedList(FormattedList &&src)".
  fl.def(
      "append_to",
      [](const FormattedList &self, Appendable &appendable) -> Appendable & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.appendTo(appendable, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("appendable"));
  fl.def(
      "next_position",
      [](const FormattedList &self, ConstrainedFieldPosition &cfpos) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.nextPosition(cfpos, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("cfpos"));
  fl.def("to_string", [](const FormattedList &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  fl.def("to_temp_string", [](const FormattedList &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.toTempString(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  // icu::ListFormatter
  py::class_<ListFormatter, UObject> lf(m, "ListFormatter");
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  lf.def(py::init<const ListFormatter &>(), py::arg("source"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
  lf.def_static(
        "create_instance",
        [](const Locale &locale) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = ListFormatter::createInstance(locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = ListFormatter::createInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def_static("create_instance", []() {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ListFormatter::createInstance(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
#if (U_ICU_VERSION_MAJOR_NUM >= 67)
  lf.def_static(
        "create_instance",
        [](const Locale &locale, UListFormatterType type, UListFormatterWidth width) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = ListFormatter::createInstance(locale, type, width, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"), py::arg("type_"), py::arg("width"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale, UListFormatterType type, UListFormatterWidth width) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = ListFormatter::createInstance(locale, type, width, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"), py::arg("type_"), py::arg("width"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 67)
  lf.def(
      "format",
      [](const ListFormatter &self, const std::list<UnicodeString> &items, int32_t count,
         UnicodeString &append_to) -> UnicodeString & {
        std::vector<UnicodeString> _items(std::begin(items), std::end(items));
        if (count == -1) {
          count = static_cast<int32_t>(items.size());
        }
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.format(_items.data(), count, append_to, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("items"), py::arg("count"), py::arg("append_to"));
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  lf.def(
      "format_strings_to_value",
      [](const ListFormatter &self, const std::list<UnicodeString> &items, int32_t count) {
        std::vector<UnicodeString> _items(std::begin(items), std::end(items));
        if (count == -1) {
          count = static_cast<int32_t>(items.size());
        }
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.formatStringsToValue(_items.data(), count, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("items"), py::arg("count") = -1);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

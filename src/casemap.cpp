#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <optional>
#include <pybind11/stl.h>
#include <unicode/brkiter.h>
#include <unicode/casemap.h>
#include <unicode/edits.h>

using namespace icu;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

void init_casemap(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  //
  // class icu::CaseMap
  //
  py::class_<CaseMap, UMemory> cm(m, "CaseMap");

  cm.def_static(
      "fold",
      [](uint32_t options, const std::u16string &src, int32_t src_length,
         std::optional<Edits *> &edits) {
        auto src_data = src.data();
        ErrorCode error_code;
        const auto dest_capacity = CaseMap::fold(
            options, src_data, src_length, nullptr, 0, nullptr, error_code);
        std::u16string result(dest_capacity, u'\0');
        error_code.reset();
        CaseMap::fold(options, src_data, src_length, result.data(),
                      dest_capacity, edits.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("options"), py::arg("src"), py::arg("src_length") = -1,
      py::arg("edits") = std::nullopt);

  cm.def_static(
      "to_lower",
      [](std::optional<const std::string> &locale, uint32_t options,
         const std::u16string &src, int32_t src_length,
         std::optional<Edits *> &edits) {
        auto locale_data = locale ? locale->data() : nullptr;
        auto src_data = src.data();
        ErrorCode error_code;
        const auto dest_capacity =
            CaseMap::toLower(locale_data, options, src_data, src_length,
                             nullptr, 0, nullptr, error_code);
        std::u16string result(dest_capacity, u'\0');
        error_code.reset();
        CaseMap::toLower(locale_data, options, src_data, src_length,
                         result.data(), dest_capacity, edits.value_or(nullptr),
                         error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("options"), py::arg("src"),
      py::arg("src_length") = -1, py::arg("edits") = std::nullopt);

  cm.def_static(
      "to_title",
      [](std::optional<const std::string> &locale, uint32_t options,
         std::optional<BreakIterator *> &iter, const std::u16string &src,
         int32_t src_length, std::optional<Edits *> &edits) {
        auto locale_data = locale ? locale->data() : nullptr;
        auto src_data = src.data();
        auto iter_value = iter.value_or(nullptr);
        ErrorCode error_code;
        const auto dest_capacity =
            CaseMap::toTitle(locale_data, options, iter_value, src_data,
                             src_length, nullptr, 0, nullptr, error_code);
        std::u16string result(dest_capacity, u'\0');
        error_code.reset();
        CaseMap::toTitle(locale_data, options, iter_value, src_data, src_length,
                         result.data(), dest_capacity, edits.value_or(nullptr),
                         error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("options"), py::arg("iter"), py::arg("src"),
      py::arg("src_length") = -1, py::arg("edits") = std::nullopt);

  cm.def_static(
      "to_upper",
      [](std::optional<const std::string> &locale, uint32_t options,
         const std::u16string &src, int32_t src_length,
         std::optional<Edits *> &edits) {
        auto locale_data = locale ? locale->data() : nullptr;
        auto src_data = src.data();
        ErrorCode error_code;
        const auto dest_capacity =
            CaseMap::toUpper(locale_data, options, src_data, src_length,
                             nullptr, 0, nullptr, error_code);
        std::u16string result(dest_capacity, u'\0');
        error_code.reset();
        CaseMap::toUpper(locale_data, options, src_data, src_length,
                         result.data(), dest_capacity, edits.value_or(nullptr),
                         error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("options"), py::arg("src"),
      py::arg("src_length") = -1, py::arg("edits") = std::nullopt);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 59)
#include <unicode/brkiter.h>
#include <unicode/casemap.h>
#include <unicode/edits.h>
#include <unicode/errorcode.h>

using namespace icu;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)

void init_casemap(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 59)
  // icu::CaseMap
  py::class_<CaseMap, UMemory> cm(m, "CaseMap");
  cm.def_static(
      "fold",
      [](uint32_t options, const char16_t *src, int32_t src_length, Edits *edits) {
        ErrorCode error_code;
        auto dest_size = CaseMap::fold(options, src, src_length, nullptr, 0, nullptr, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        CaseMap::fold(options, src, src_length, result.data(), dest_size, edits, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("options"), py::arg("src"), py::arg("src_length") = -1, py::arg("edits") = nullptr);
  cm.def_static(
      "to_lower",
      [](const char *locale, uint32_t options, const char16_t *src, int32_t src_length, Edits *edits) {
        ErrorCode error_code;
        auto dest_size = CaseMap::toLower(locale, options, src, src_length, nullptr, 0, nullptr, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        CaseMap::toLower(locale, options, src, src_length, result.data(), dest_size, edits, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("options"), py::arg("src"), py::arg("src_length") = -1, py::arg("edits") = nullptr);
  cm.def_static(
      "to_title",
      [](const char *locale, uint32_t options, BreakIterator *iter, const char16_t *src, int32_t src_length,
         Edits *edits) {
        ErrorCode error_code;
        auto dest_size = CaseMap::toTitle(locale, options, iter, src, src_length, nullptr, 0, nullptr, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        CaseMap::toTitle(locale, options, iter, src, src_length, result.data(), dest_size, edits, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("options"), py::arg("iter"), py::arg("src"), py::arg("src_length") = -1,
      py::arg("edits") = nullptr);
  cm.def_static(
      "to_upper",
      [](const char *locale, uint32_t options, const char16_t *src, int32_t src_length, Edits *edits) {
        ErrorCode error_code;
        auto dest_size = CaseMap::toUpper(locale, options, src, src_length, nullptr, 0, nullptr, error_code);
        std::u16string result(dest_size, u'\0');
        error_code.reset();
        CaseMap::toUpper(locale, options, src, src_length, result.data(), dest_size, edits, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"), py::arg("options"), py::arg("src"), py::arg("src_length") = -1, py::arg("edits") = nullptr);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 59)
}

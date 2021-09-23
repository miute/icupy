#include "main.hpp"
#include "ucsdetptr.hpp"
#include "uenumptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/stl.h>

_ConstUCharsetMatchPtr::_ConstUCharsetMatchPtr(const UCharsetMatch *p) : p_(p) {}
_ConstUCharsetMatchPtr::~_ConstUCharsetMatchPtr() {}
const UCharsetMatch *_ConstUCharsetMatchPtr::get() const { return p_; }

_UCharsetDetectorPtr::_UCharsetDetectorPtr(UCharsetDetector *p) : p_(p) {}
_UCharsetDetectorPtr::~_UCharsetDetectorPtr() {}
UCharsetDetector *_UCharsetDetectorPtr::get() const { return p_; }

void _UCharsetDetectorPtr::set_source(const std::shared_ptr<void> &source) { source_ = source; }

void init_ucsdet(py::module &m) {
  py::class_<_ConstUCharsetMatchPtr>(m, "_ConstUCharsetMatchPtr");

  py::class_<_UCharsetDetectorPtr>(m, "_UCharsetDetectorPtr");

  m.def(
      "ucsdet_close", [](_UCharsetDetectorPtr &ucsd) { ucsdet_close(ucsd); }, py::arg("ucsd"));
  m.def(
      "ucsdet_detect",
      [](_UCharsetDetectorPtr &ucsd) -> std::optional<std::unique_ptr<_ConstUCharsetMatchPtr>> {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ucsdet_detect(ucsd, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        } else if (p == nullptr) {
          return std::nullopt;
        }
        return std::make_unique<_ConstUCharsetMatchPtr>(p);
      },
      py::arg("ucsd"));
  m.def(
      "ucsdet_detect_all",
      [](_UCharsetDetectorPtr &ucsd) {
        UErrorCode error_code = U_ZERO_ERROR;
        int32_t matches_found;
        auto p = ucsdet_detectAll(ucsd, &matches_found, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        std::vector<std::unique_ptr<_ConstUCharsetMatchPtr>> result(matches_found);
        for (int32_t n = 0; n < matches_found; ++n, ++p) {
          result[n] = std::move(std::make_unique<_ConstUCharsetMatchPtr>(*p));
        }
        return result;
      },
      py::arg("ucsd"));
  m.def(
      "ucsdet_enable_input_filter",
      [](_UCharsetDetectorPtr &ucsd, bool filter) { return ucsdet_enableInputFilter(ucsd, filter); }, py::arg("ucsd"),
      py::arg("filter_"));
  m.def(
      "ucsdet_get_all_detectable_charsets",
      [](const _UCharsetDetectorPtr &ucsd) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ucsdet_getAllDetectableCharsets(ucsd, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p);
      },
      py::arg("ucsd"));
  m.def(
      "ucsdet_get_confidence",
      [](_ConstUCharsetMatchPtr &ucsm) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucsdet_getConfidence(ucsm, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("ucsm"));
  m.def(
      "ucsdet_get_language",
      [](_ConstUCharsetMatchPtr &ucsm) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucsdet_getLanguage(ucsm, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("ucsm"));
  m.def(
      "ucsdet_get_name",
      [](_ConstUCharsetMatchPtr &ucsm) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucsdet_getName(ucsm, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("ucsm"));
  m.def(
      "ucsdet_get_uchars",
      [](_ConstUCharsetMatchPtr &ucsm) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto cap = ucsdet_getUChars(ucsm, nullptr, 0, &error_code);
        std::u16string result(cap, '\0');
        error_code = U_ZERO_ERROR;
        ucsdet_getUChars(ucsm, result.data(), cap, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("ucsm"));
  m.def(
      "ucsdet_is_input_filter_enabled",
      [](const _UCharsetDetectorPtr &ucsd) { return ucsdet_isInputFilterEnabled(ucsd); }, py::arg("ucsd"));
  m.def("ucsdet_open", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto p = ucsdet_open(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_UCharsetDetectorPtr>(p);
  });
  m.def(
      "ucsdet_set_declared_encoding",
      [](_UCharsetDetectorPtr &ucsd, const char *encoding, int32_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucsdet_setDeclaredEncoding(ucsd, encoding, length, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("ucsd"), py::arg("encoding"), py::arg("length") = -1);
  m.def(
      "ucsdet_set_text",
      [](_UCharsetDetectorPtr &ucsd, const char *text_in, int32_t len) {
        UErrorCode error_code = U_ZERO_ERROR;
        size_t size = text_in && len == -1 ? std::strlen(text_in) : std::max(0, len);
        auto source = std::shared_ptr<char[]>(new char[size + 1]);
        auto s = source.get();
        std::memset(s, 0, size + 1);
        std::memcpy(s, text_in, size);
        ucsdet_setText(ucsd, s, len, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        ucsd.set_source(source);
      },
      py::arg("ucsd"), py::arg("text_in"), py::arg("len_") = -1);
}

#include "main.hpp"
#include "ucsdetptr.hpp"
#include "uenumptr.hpp"
#include <optional>
#include <pybind11/stl.h>

using namespace icu;

_ConstUCharsetMatchPtr::_ConstUCharsetMatchPtr(const UCharsetMatch *p)
    : p_(p) {}

_ConstUCharsetMatchPtr::~_ConstUCharsetMatchPtr() {}

const UCharsetMatch *_ConstUCharsetMatchPtr::get() const { return p_; }

_UCharsetDetectorPtr::_UCharsetDetectorPtr(UCharsetDetector *p) : p_(p) {}

_UCharsetDetectorPtr::~_UCharsetDetectorPtr() {}

UCharsetDetector *_UCharsetDetectorPtr::get() const { return p_; }

void _UCharsetDetectorPtr::set_source(std::unique_ptr<std::string> &source) {
  source_ = std::move(source);
}

void init_ucsdet(py::module &m) {
  //
  // struct UCharsetMatch
  //
  py::class_<_ConstUCharsetMatchPtr>(m, "_ConstUCharsetMatchPtr");

  //
  // struct UCharsetDetector
  //
  py::class_<_UCharsetDetectorPtr>(m, "_UCharsetDetectorPtr");

  //
  // Functions
  //
  m.def(
      "ucsdet_close", [](_UCharsetDetectorPtr &ucsd) { ucsdet_close(ucsd); },
      py::arg("ucsd"));

  m.def(
      "ucsdet_detect",
      [](_UCharsetDetectorPtr &ucsd)
          -> std::optional<std::unique_ptr<_ConstUCharsetMatchPtr>> {
        ErrorCode error_code;
        auto p = ucsdet_detect(ucsd, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        } else if (p == nullptr) {
          return std::nullopt;
        }
        return std::make_unique<_ConstUCharsetMatchPtr>(p);
      },
      py::arg("ucsd"));

  m.def(
      "ucsdet_detect_all",
      [](_UCharsetDetectorPtr &ucsd) {
        int32_t matches_found;
        ErrorCode error_code;
        auto p = ucsdet_detectAll(ucsd, &matches_found, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        std::vector<std::unique_ptr<_ConstUCharsetMatchPtr>> result(
            matches_found);
        for (int32_t n = 0; n < matches_found; ++n, ++p) {
          result[n] = std::make_unique<_ConstUCharsetMatchPtr>(*p);
        }
        return result;
      },
      py::arg("ucsd"));

  m.def(
      "ucsdet_enable_input_filter",
      [](_UCharsetDetectorPtr &ucsd, py::bool_ filter) -> py::bool_ {
        return ucsdet_enableInputFilter(ucsd, filter);
      },
      py::arg("ucsd"), py::arg("filter_"));

  m.def(
      "ucsdet_get_all_detectable_charsets",
      [](const _UCharsetDetectorPtr &ucsd) {
        ErrorCode error_code;
        auto p = ucsdet_getAllDetectableCharsets(ucsd, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p);
      },
      py::arg("ucsd"));

  m.def(
      "ucsdet_get_confidence",
      [](_ConstUCharsetMatchPtr &ucsm) {
        ErrorCode error_code;
        auto result = ucsdet_getConfidence(ucsm, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("ucsm"));

  m.def(
      "ucsdet_get_language",
      [](_ConstUCharsetMatchPtr &ucsm) {
        ErrorCode error_code;
        auto result = ucsdet_getLanguage(ucsm, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("ucsm"));

  m.def(
      "ucsdet_get_name",
      [](_ConstUCharsetMatchPtr &ucsm) {
        ErrorCode error_code;
        auto result = ucsdet_getName(ucsm, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("ucsm"));

  m.def(
      "ucsdet_get_uchars",
      [](_ConstUCharsetMatchPtr &ucsm) {
        ErrorCode error_code;
        const auto capacity = ucsdet_getUChars(ucsm, nullptr, 0, error_code);
        std::u16string result(capacity, '\0');
        error_code.reset();
        ucsdet_getUChars(ucsm, result.data(), capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("ucsm"));

  m.def(
      "ucsdet_is_input_filter_enabled",
      [](const _UCharsetDetectorPtr &ucsd) -> py::bool_ {
        return ucsdet_isInputFilterEnabled(ucsd);
      },
      py::arg("ucsd"));

  m.def("ucsdet_open", []() {
    ErrorCode error_code;
    auto p = ucsdet_open(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::make_unique<_UCharsetDetectorPtr>(p);
  });

  m.def(
      "ucsdet_set_declared_encoding",
      [](_UCharsetDetectorPtr &ucsd, const std::string &encoding,
         int32_t length) {
        ErrorCode error_code;
        ucsdet_setDeclaredEncoding(ucsd, encoding.data(), length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("ucsd"), py::arg("encoding"), py::arg("length") = -1);

  m.def(
      "ucsdet_set_text",
      [](_UCharsetDetectorPtr &ucsd, const icupy::CharPtrVariant &text_in,
         int32_t length) {
        const auto text_in_value = icupy::CharPtr(text_in);
        auto normalized_length = length;
        if (normalized_length == -1) {
          normalized_length = static_cast<int32_t>(text_in_value.size());
        }
        auto text_in_ptr = std::make_unique<std::string>(
            text_in_value, std::max(normalized_length, 0));
        ErrorCode error_code;
        ucsdet_setText(ucsd, text_in_ptr->data(), length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        ucsd.set_source(text_in_ptr);
      },
      py::arg("ucsd"), py::arg("text_in"), py::arg("len_") = -1);
}

#include "main.hpp"
#include "ucsdetptr.hpp"
#include "uenumptr.hpp"
#include <optional>
#include <pybind11/stl.h>

using namespace icu;

namespace icupy {

//
// struct UCharsetMatch
//
UCharsetMatchPtr::UCharsetMatchPtr(const UCharsetMatch *p) : p_(p) {}

UCharsetMatchPtr::~UCharsetMatchPtr() {}

const UCharsetMatch *UCharsetMatchPtr::get() const { return p_; }

//
// struct UCharsetDetector
//
UCharsetDetectorPtr::UCharsetDetectorPtr(UCharsetDetector *p) : p_(p) {}

UCharsetDetectorPtr::~UCharsetDetectorPtr() {}

UCharsetDetector *UCharsetDetectorPtr::get() const { return p_; }

void UCharsetDetectorPtr::set_source(std::unique_ptr<std::string> &source) {
  source_ = std::move(source);
}

} // namespace icupy

void init_ucsdet(py::module &m) {
  //
  // struct UCharsetMatch
  //
  py::class_<icupy::UCharsetMatchPtr>(m, "UCharsetMatch", R"doc(
    Opaque structure representing a match that was identified from a
    charset detection operation.

    See Also:
        :func:`ucsdet_detect`
        :func:`ucsdet_detect_all`
    )doc");

  //
  // struct UCharsetDetector
  //
  py::class_<icupy::UCharsetDetectorPtr>(m, "UCharsetDetector", R"doc(
    UCharsetDetector structure representing a charset detector.

    See Also:
        :func:`ucsdet_close`
        :func:`ucsdet_open`
    )doc");

  //
  // Functions
  //
  m.def(
      "ucsdet_close",
      [](icupy::UCharsetDetectorPtr &ucsd) { ucsdet_close(ucsd); },
      py::arg("ucsd"));

  m.def(
      "ucsdet_detect",
      [](icupy::UCharsetDetectorPtr &ucsd)
          -> std::optional<std::unique_ptr<icupy::UCharsetMatchPtr>> {
        ErrorCode error_code;
        auto p = ucsdet_detect(ucsd, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        } else if (p == nullptr) {
          return std::nullopt;
        }
        return std::make_unique<icupy::UCharsetMatchPtr>(p);
      },
      py::arg("ucsd"));

  m.def(
      "ucsdet_detect_all",
      [](icupy::UCharsetDetectorPtr &ucsd) {
        int32_t matches_found;
        ErrorCode error_code;
        auto p = ucsdet_detectAll(ucsd, &matches_found, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        std::vector<std::unique_ptr<icupy::UCharsetMatchPtr>> result(
            matches_found);
        for (int32_t n = 0; n < matches_found; ++n, ++p) {
          result[n] = std::make_unique<icupy::UCharsetMatchPtr>(*p);
        }
        return result;
      },
      py::arg("ucsd"));

  m.def(
      "ucsdet_enable_input_filter",
      [](icupy::UCharsetDetectorPtr &ucsd, py::bool_ filter) -> py::bool_ {
        return ucsdet_enableInputFilter(ucsd, filter);
      },
      py::arg("ucsd"), py::arg("filter"));

  m.def(
      "ucsdet_get_all_detectable_charsets",
      [](const icupy::UCharsetDetectorPtr &ucsd) {
        ErrorCode error_code;
        auto p = ucsdet_getAllDetectableCharsets(ucsd, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UEnumerationPtr>(p);
      },
      py::arg("ucsd"));

  m.def(
      "ucsdet_get_confidence",
      [](icupy::UCharsetMatchPtr &ucsm) {
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
      [](icupy::UCharsetMatchPtr &ucsm) {
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
      [](icupy::UCharsetMatchPtr &ucsm) {
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
      [](icupy::UCharsetMatchPtr &ucsm) {
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
      [](const icupy::UCharsetDetectorPtr &ucsd) -> py::bool_ {
        return ucsdet_isInputFilterEnabled(ucsd);
      },
      py::arg("ucsd"));

  m.def("ucsdet_open", []() {
    ErrorCode error_code;
    auto p = ucsdet_open(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::make_unique<icupy::UCharsetDetectorPtr>(p);
  });

  m.def(
      "ucsdet_set_declared_encoding",
      [](icupy::UCharsetDetectorPtr &ucsd, const std::string &encoding,
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
      [](icupy::UCharsetDetectorPtr &ucsd, const icupy::CharPtrVariant &text_in,
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
      py::arg("ucsd"), py::arg("text_in"), py::arg("length") = -1);
}

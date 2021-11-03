#include "main.hpp"
#include "utextvec.hpp"
#include <cassert>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/ustring.h>

using namespace icu;

_UTextPtr::_UTextPtr() : p_(nullptr) {}
_UTextPtr::_UTextPtr(UText *p) : p_(p) {}
_UTextPtr::_UTextPtr(UText *p, const std::shared_ptr<void> &source) : p_(p), source_(source) {}
_UTextPtr::~_UTextPtr() {}
UText *_UTextPtr::get() const { return p_; }

const std::shared_ptr<void> &_UTextPtr::get_source() const { return source_; }

_UTextVector::_UTextVector(size_t n) {
  values_ = std::vector<_UTextPtr>(n);
  sources_ = std::vector<UnicodeString>(n);
  UErrorCode error_code;
  for (size_t i = 0; i < n; ++i) {
    error_code = U_ZERO_ERROR;
    values_[i] = utext_openUnicodeString(nullptr, &sources_[i], &error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
  }
}

_UTextVector::_UTextVector(const _UnicodeStringList &iterable) {
  const auto size = iterable.size();
  values_ = std::vector<_UTextPtr>(size);
  sources_ = std::vector<UnicodeString>(iterable.begin(), iterable.end());
  UErrorCode error_code;
  for (size_t i = 0; i < size; ++i) {
    error_code = U_ZERO_ERROR;
    values_[i] = utext_openUnicodeString(nullptr, &sources_[i], &error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
  }
}

_UTextVector::~_UTextVector() { clear(); }

void _UTextVector::clear() {
  for (size_t i = 0; i < values_.size(); ++i) {
    utext_close(values_[i]);
  }
  values_.clear();
  sources_.clear();
}

void init_utext(py::module &m) {
  // _UTextPtr
  py::class_<_UTextPtr> utp(m, "_UTextPtr");
  utp.def(py::self != py::self, py::arg("other")).def(py::self == py::self, py::arg("other"));
  utp.def("__repr__", [](const _UTextPtr &self) {
    std::stringstream ss;
    ss << "_UTextPtr(UText(";
    ss << "magic=0x" << std::hex << self->magic;
    ss << ", flags=0x" << std::hex << self->flags;
    ss << ", providerProperties=0x" << std::hex << self->providerProperties;
    ss << ", pFuncs=0x" << std::hex << self->pFuncs;
    ss << "))";
    return ss.str();
  });
  utp.def_property_readonly("magic", [](const _UTextPtr &self) { return self->magic; });
  utp.def_property_readonly("flags", [](const _UTextPtr &self) { return self->flags; });
  utp.def_property_readonly("provider_properties", [](const _UTextPtr &self) { return self->providerProperties; });
  utp.def_property_readonly("p_funcs",
                            [](const _UTextPtr &self) { return reinterpret_cast<std::intptr_t>(self->pFuncs); });

  // _UTextVector
  py::class_<_UTextVector> utv(m, "UTextVector");
  utv.def(py::init<size_t>(), py::arg("n")).def(py::init<std::list<UnicodeString>>(), py::arg("iterable"));
  utv.def("__del__", &_UTextVector::clear)
      .def(
          "__getitem__",
          [](const _UTextVector &self, size_t index) {
            if (index < 0) {
              index += self.size();
            }
            if (index < 0 || index >= self.size()) {
              throw py::index_error(std::to_string(index));
            }
            return self[index];
          },
          py::arg("index"))
      .def(
          "__iter__", [](const _UTextVector &self) { return py::make_iterator(self.begin(), self.end()); },
          py::keep_alive<0, 1>())
      .def("__len__", &_UTextVector::size);

  m.def(
      "utext_char32_at", [](_UTextPtr &ut, int64_t native_index) { return utext_char32At(ut, native_index); },
      py::arg("ut"), py::arg("native_index"));
  m.def(
      "utext_clone",
      [](std::optional<_UTextPtr> &dest, const _UTextPtr &src, UBool deep, UBool read_only) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = utext_clone(dest.value_or(nullptr), src, deep, read_only, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p, src.get_source());
      },
      py::arg("dest"), py::arg("src"), py::arg("deep"), py::arg("read_only"));
  m.def(
      "utext_close",
      [](_UTextPtr &ut) -> std::optional<_UTextPtr> {
        auto p = utext_close(ut);
        if (p == nullptr) {
          return std::nullopt;
        }
        assert(p == (UText *)ut);
        return ut;
      },
      py::arg("ut"));
  m.def(
      "utext_copy",
      [](_UTextPtr &ut, int64_t native_start, int64_t native_limit, int64_t dest_index, UBool move) {
        UErrorCode error_code = U_ZERO_ERROR;
        utext_copy(ut, native_start, native_limit, dest_index, move, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("ut"), py::arg("native_start"), py::arg("native_limit"), py::arg("dest_index"), py::arg("move"));
  m.def(
      "utext_current32", [](_UTextPtr &ut) { return utext_current32(ut); }, py::arg("ut"));
  m.def(
      "utext_equals", [](_UTextPtr &a, _UTextPtr &b) { return utext_equals(a, b); }, py::arg("a"), py::arg("b"));
  m.def(
      "utext_extract",
      [](_UTextPtr &ut, int64_t native_start, int64_t native_limit) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto dest_capacity = utext_extract(ut, native_start, native_limit, nullptr, 0, &error_code);
        std::u16string result(dest_capacity, u'\0');
        error_code = U_ZERO_ERROR;
        utext_extract(ut, native_start, native_limit, result.data(), dest_capacity, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("ut"), py::arg("native_start"), py::arg("native_limit"));
  m.def(
      "utext_freeze", [](_UTextPtr &ut) { utext_freeze(ut); }, py::arg("ut"));
  m.def(
      "utext_get_native_index", [](_UTextPtr &ut) { return utext_getNativeIndex(ut); }, py::arg("ut"));
  m.def(
      "utext_get_previous_native_index", [](_UTextPtr &ut) { return utext_getPreviousNativeIndex(ut); }, py::arg("ut"));
  m.def(
      "utext_has_meta_data", [](_UTextPtr &ut) { return utext_hasMetaData(ut); }, py::arg("ut"));
  m.def(
      "utext_is_length_expensive", [](_UTextPtr &ut) { return utext_isLengthExpensive(ut); }, py::arg("ut"));
  m.def(
      "utext_is_writable", [](_UTextPtr &ut) { return utext_isWritable(ut); }, py::arg("ut"));
  m.def(
      "utext_move_index32", [](_UTextPtr &ut, int32_t delta) { return utext_moveIndex32(ut, delta); }, py::arg("ut"),
      py::arg("delta"));
  m.def(
      "utext_native_length", [](_UTextPtr &ut) { return utext_nativeLength(ut); }, py::arg("ut"));
  m.def(
      "utext_next32", [](_UTextPtr &ut) { return utext_next32(ut); }, py::arg("ut"));
  m.def(
      "utext_next32_from", [](_UTextPtr &ut, int64_t native_index) { return utext_next32From(ut, native_index); },
      py::arg("ut"), py::arg("native_index"));
  m.def(
      "utext_open_character_iterator",
      [](std::optional<_UTextPtr> &ut, CharacterIterator *ci) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = utext_openCharacterIterator(ut.value_or(nullptr), ci, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("ut"), py::arg("ci"));
  m.def(
      "utext_open_const_unicode_string",
      [](std::optional<_UTextPtr> &ut, const UnicodeString *s) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = utext_openConstUnicodeString(ut.value_or(nullptr), s, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("ut"), py::arg("s"));
  m.def(
      "utext_open_replaceable",
      [](std::optional<_UTextPtr> &ut, Replaceable *rep) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = utext_openReplaceable(ut.value_or(nullptr), rep, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("ut"), py::arg("rep"));
  m.def(
      "utext_open_uchars",
      [](std::optional<_UTextPtr> &ut, const UChar *s, int64_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto text = std::make_shared<std::u16string>(s, s && length == -1 ? u_strlen(s) : std::max(int64_t{0}, length));
        auto p = utext_openUChars(ut.value_or(nullptr), text->data(), length, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p, text);
      },
      py::keep_alive<1, 0>(), py::arg("ut"), py::arg("s"), py::arg("length"));
  m.def(
      "utext_open_unicode_string",
      [](std::optional<_UTextPtr> &ut, UnicodeString *s) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = utext_openUnicodeString(ut.value_or(nullptr), s, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("ut"), py::arg("s"));
  m.def(
      "utext_open_utf8",
      [](std::optional<_UTextPtr> &ut, const char *s, int64_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto text = std::make_shared<std::string>(s, s && length == -1 ? std::strlen(s) : std::max(int64_t{0}, length));
        auto p = utext_openUTF8(ut.value_or(nullptr), text->data(), length, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p, text);
      },
      py::keep_alive<1, 0>(), py::arg("ut"), py::arg("s"), py::arg("length"));
  m.def(
      "utext_previous32", [](_UTextPtr &ut) { return utext_previous32(ut); }, py::arg("ut"));
  m.def(
      "utext_previous32_from",
      [](_UTextPtr &ut, int64_t native_index) { return utext_previous32From(ut, native_index); }, py::arg("ut"),
      py::arg("native_index"));
  m.def(
      "utext_replace",
      [](_UTextPtr &ut, int64_t native_start, int64_t native_limit, const UChar *replacement_text,
         int32_t replacement_length) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = utext_replace(ut, native_start, native_limit, replacement_text, replacement_length, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("ut"), py::arg("native_start"), py::arg("native_limit"), py::arg("replacement_text"),
      py::arg("replacement_length"));
  m.def(
      "utext_set_native_index", [](_UTextPtr &ut, int64_t native_index) { utext_setNativeIndex(ut, native_index); },
      py::arg("ut"), py::arg("native_index"));
  // FIXME: Add support for UText text provider.
  /*
  m.def(
      "utext_setup",
      [](std::optional<_UTextPtr> &ut, int32_t extra_space) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = utext_setup(ut.value_or(nullptr), extra_space, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("ut"), py::arg("extra_space"));
  */

  m.attr("UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE") = int32_t{UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE};
  m.attr("UTEXT_PROVIDER_STABLE_CHUNKS") = int32_t{UTEXT_PROVIDER_STABLE_CHUNKS};
  m.attr("UTEXT_PROVIDER_WRITABLE") = int32_t{UTEXT_PROVIDER_WRITABLE};
  m.attr("UTEXT_PROVIDER_HAS_META_DATA") = int32_t{UTEXT_PROVIDER_HAS_META_DATA};
  m.attr("UTEXT_PROVIDER_OWNS_TEXT") = int32_t{UTEXT_PROVIDER_OWNS_TEXT};
}

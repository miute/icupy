#include "main.hpp"
#include "utextvec.hpp"
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>

using namespace icu;

namespace icupy {

//
// struct UText
//
UTextPtr::UTextPtr() : p_(nullptr) {}

UTextPtr::UTextPtr(UText *p) : p_(p) {}

UTextPtr::UTextPtr(UText *p, const SharedVoidPtr &source)
    : p_(p), source_(source) {}

UTextPtr::~UTextPtr() {}

UText *UTextPtr::get() const { return p_; }

const SharedVoidPtr &UTextPtr::get_source() const { return source_; }

//
// class UTextVector
//
UTextVector::UTextVector() {}

UTextVector::UTextVector(
    const std::list<std::reference_wrapper<UnicodeString>> &iterable) {
  const auto size = iterable.size();
  values_.reserve(size);
  sources_.assign(iterable.begin(), iterable.end());
  ErrorCode error_code;
  for (size_t i = 0; i < size; ++i) {
    error_code.reset();
    auto ut = utext_openUnicodeString(nullptr, &sources_[i].get(), error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    values_.push_back(ut);
  }
}

UTextVector::~UTextVector() { clear(); }

void UTextVector::append(UnicodeString &src) {
  ErrorCode error_code;
  auto ut = utext_openUnicodeString(nullptr, &src, error_code);
  if (error_code.isFailure()) {
    throw icupy::ICUError(error_code);
  }
  append(ut, src);
}

void UTextVector::append(UText *ut, UnicodeString &src) {
  values_.push_back(ut);
  sources_.push_back(src);
}

void UTextVector::clear() {
  for (size_t i = 0; i < values_.size(); ++i) {
    utext_close(values_[i]);
  }
  values_.clear();
  sources_.clear();
}

void UTextVector::extend(
    const std::list<std::reference_wrapper<UnicodeString>> &iterable) {
  for (auto &src : iterable) {
    append(src);
  }
}

void UTextVector::insert(int32_t index, UnicodeString &src) {
  auto normalized_index = index;
  const auto size = static_cast<int32_t>(values_.size());
  if (normalized_index < 0) {
    normalized_index += size;
  }
  if (normalized_index < 0 || normalized_index >= size) {
    throw py::index_error("list index out of range: " + std::to_string(index));
  }
  ErrorCode error_code;
  auto ut = utext_openUnicodeString(nullptr, &src, error_code);
  if (error_code.isFailure()) {
    throw icupy::ICUError(error_code);
  }
  values_.insert(values_.cbegin() + normalized_index, ut);
  sources_.insert(sources_.cbegin() + normalized_index, src);
}

void UTextVector::remove(int32_t index) {
  auto normalized_index = index;
  const auto size = static_cast<int32_t>(values_.size());
  if (normalized_index < 0) {
    normalized_index += size;
  }
  if (normalized_index < 0 || normalized_index >= size) {
    throw py::index_error("list index out of range: " + std::to_string(index));
  }
  utext_close(values_[normalized_index]);
  values_.erase(values_.cbegin() + normalized_index);
  sources_.erase(sources_.cbegin() + normalized_index);
}

} // namespace icupy

void init_utext(py::module &m) {
  //
  // struct UText
  //
  py::class_<icupy::UTextPtr> utp(m, "UText", R"doc(
    UText structure.

    See Also:
        :class:`UTextVector`
        :func:`utext_clone`
        :func:`utext_close`
        :func:`utext_open_character_iterator`
        :func:`utext_open_const_unicode_string`
        :func:`utext_open_replaceable`
        :func:`utext_open_uchars`
        :func:`utext_open_unicode_string`
        :func:`utext_open_utf8`
    )doc");

  utp.def(
      "__eq__",
      [](const icupy::UTextPtr &self, const icupy::UTextPtr &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"));

  utp.def(
      "__ne__",
      [](const icupy::UTextPtr &self, const icupy::UTextPtr &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"));

  utp.def("__repr__", [](const icupy::UTextPtr &self) {
    std::stringstream ss;
    ss << "<UText(";
    ss << "magic=0x" << std::hex << std::nouppercase << self->magic;
    ss << ", flags=0x" << std::hex << self->flags;
    ss << ", providerProperties=0x" << std::hex << self->providerProperties;
    ss << ", pFuncs=0x" << std::hex
       << reinterpret_cast<uintptr_t>(self->pFuncs);
    ss << ")>";
    return ss.str();
  });

  utp.def_property_readonly(
      "magic", [](const icupy::UTextPtr &self) { return self->magic; });

  utp.def_property_readonly(
      "flags", [](const icupy::UTextPtr &self) { return self->flags; });

  utp.def_property_readonly(
      "provider_properties",
      [](const icupy::UTextPtr &self) { return self->providerProperties; });

  utp.def_property_readonly("p_funcs", [](const icupy::UTextPtr &self) {
    return reinterpret_cast<std::intptr_t>(self->pFuncs);
  });

  //
  // class UTextVector
  //
  py::class_<icupy::UTextVector> utv(m, "UTextVector");

  utv.def(py::init<>())
      .def(py::init<std::list<std::reference_wrapper<UnicodeString>>>(),
           py::keep_alive<1, 2>(), py::arg("iterable"));

  utv.def("__del__", &icupy::UTextVector::clear);

  utv.def("__delitem__", &icupy::UTextVector::remove, py::arg("index"))
      .def(
          "__delitem__",
          [](icupy::UTextVector &self, const py::slice &index) {
            size_t start, stop, step, slice_length;
            if (!index.compute(self.size(), &start, &stop, &step,
                               &slice_length)) {
              throw py::error_already_set();
            }
            for (size_t n = 0; n < slice_length; ++n) {
              self.remove(
                  static_cast<int32_t>(start + (slice_length - n - 1) * step));
            }
          },
          py::arg("index"));

  utv.def(
      "__getitem__",
      [](icupy::UTextVector &self, int32_t index) -> icupy::UTextPtr & {
        auto actual_index = index;
        const auto size = static_cast<int32_t>(self.size());
        if (actual_index < 0) {
          actual_index += size;
        }
        if (actual_index < 0 || actual_index >= size) {
          throw py::index_error("list index out of range: " +
                                std::to_string(index));
        }
        return self[actual_index];
      },
      py::return_value_policy::reference_internal, py::arg("index"));

  utv.def(
      "__iadd__",
      [](icupy::UTextVector &self,
         const std::list<std::reference_wrapper<icu::UnicodeString>> &iterable)
          -> icupy::UTextVector & {
        self.extend(iterable);
        return self;
      },
      py::keep_alive<1, 2>(), py::arg("iterable"));

  utv.def(
      "__iter__",
      [](const icupy::UTextVector &self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>());

  utv.def(
      "__reversed__",
      [](const icupy::UTextVector & /* self */) { return py::none(); }, R"doc(
      ``UTextVector`` does not support reverse iterator and always returns
      ``None``.
      )doc");

  utv.def("__len__", &icupy::UTextVector::size);

  utv.def("append",
          py::overload_cast<UnicodeString &>(&icupy::UTextVector::append),
          py::keep_alive<1, 2>(), py::arg("src"));

  utv.def("clear", &icupy::UTextVector::clear);

  utv.def("extend", &icupy::UTextVector::extend, py::keep_alive<1, 2>(),
          py::arg("iterable"));

  utv.def("insert", &icupy::UTextVector::insert, py::keep_alive<1, 3>(),
          py::arg("index"), py::arg("src"));

  //
  // Functions
  //
  m.def(
      "utext_char32_at",
      [](icupy::UTextPtr &ut, int64_t native_index) {
        return utext_char32At(ut, native_index);
      },
      py::arg("ut"), py::arg("native_index"));

  m.def(
      "utext_clone",
      [](std::optional<icupy::UTextPtr> &dest, const icupy::UTextPtr &src,
         py::bool_ deep, py::bool_ read_only) {
        ErrorCode error_code;
        auto p = utext_clone(dest.value_or(nullptr), src, deep, read_only,
                             error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p, src.get_source());
      },
      py::arg("dest"), py::arg("src"), py::arg("deep"), py::arg("read_only"));

  m.def(
      "utext_close",
      [](icupy::UTextPtr &ut) -> std::optional<icupy::UTextPtr> {
        auto p = utext_close(ut);
        if (p == nullptr) {
          return std::nullopt;
        }
        return ut;
      },
      py::arg("ut"));

  m.def(
      "utext_copy",
      [](icupy::UTextPtr &ut, int64_t native_start, int64_t native_limit,
         int64_t dest_index, py::bool_ move) {
        ErrorCode error_code;
        utext_copy(ut, native_start, native_limit, dest_index, move,
                   error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("ut"), py::arg("native_start"), py::arg("native_limit"),
      py::arg("dest_index"), py::arg("move"));

  m.def(
      "utext_current32",
      [](icupy::UTextPtr &ut) { return utext_current32(ut); }, py::arg("ut"));

  m.def(
      "utext_equals",
      [](icupy::UTextPtr &a, icupy::UTextPtr &b) -> py::bool_ {
        return utext_equals(a, b);
      },
      py::arg("a"), py::arg("b"));

  m.def(
      "utext_extract",
      [](icupy::UTextPtr &ut, int64_t native_start, int64_t native_limit) {
        ErrorCode error_code;
        const auto dest_capacity = utext_extract(ut, native_start, native_limit,
                                                 nullptr, 0, error_code);
        std::u16string result(dest_capacity, u'\0');
        error_code.reset();
        utext_extract(ut, native_start, native_limit, result.data(),
                      dest_capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("ut"), py::arg("native_start"), py::arg("native_limit"));

  m.def(
      "utext_freeze", [](icupy::UTextPtr &ut) { utext_freeze(ut); },
      py::arg("ut"));

  m.def(
      "utext_get_native_index",
      [](icupy::UTextPtr &ut) { return utext_getNativeIndex(ut); },
      py::arg("ut"));

  m.def(
      "utext_get_previous_native_index",
      [](icupy::UTextPtr &ut) { return utext_getPreviousNativeIndex(ut); },
      py::arg("ut"));

  m.def(
      "utext_has_meta_data",
      [](icupy::UTextPtr &ut) -> py::bool_ { return utext_hasMetaData(ut); },
      py::arg("ut"));

  m.def(
      "utext_is_length_expensive",
      [](icupy::UTextPtr &ut) -> py::bool_ {
        return utext_isLengthExpensive(ut);
      },
      py::arg("ut"));

  m.def(
      "utext_is_writable",
      [](icupy::UTextPtr &ut) -> py::bool_ { return utext_isWritable(ut); },
      py::arg("ut"));

  m.def(
      "utext_move_index32",
      [](icupy::UTextPtr &ut, int32_t delta) -> py::bool_ {
        return utext_moveIndex32(ut, delta);
      },
      py::arg("ut"), py::arg("delta"));

  m.def(
      "utext_native_length",
      [](icupy::UTextPtr &ut) { return utext_nativeLength(ut); },
      py::arg("ut"));

  m.def(
      "utext_next32", [](icupy::UTextPtr &ut) { return utext_next32(ut); },
      py::arg("ut"));

  m.def(
      "utext_next32_from",
      [](icupy::UTextPtr &ut, int64_t native_index) {
        return utext_next32From(ut, native_index);
      },
      py::arg("ut"), py::arg("native_index"));

  m.def(
      "utext_open_character_iterator",
      [](std::optional<icupy::UTextPtr> &ut, CharacterIterator *ci) {
        ErrorCode error_code;
        auto p =
            utext_openCharacterIterator(ut.value_or(nullptr), ci, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p);
      },
      py::arg("ut"), py::arg("ci").none(false));

  m.def(
      "utext_open_const_unicode_string",
      [](std::optional<icupy::UTextPtr> &ut, const UnicodeString *s) {
        ErrorCode error_code;
        auto p =
            utext_openConstUnicodeString(ut.value_or(nullptr), s, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p);
      },
      py::arg("ut"), py::arg("s").none(false));

  m.def(
      "utext_open_replaceable",
      [](std::optional<icupy::UTextPtr> &ut, Replaceable *rep) {
        ErrorCode error_code;
        auto p = utext_openReplaceable(ut.value_or(nullptr), rep, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p);
      },
      py::arg("ut"), py::arg("rep").none(false));

  m.def(
      "utext_open_uchars",
      [](std::optional<icupy::UTextPtr> &ut, const std::u16string &s,
         int64_t length) {
        auto normalized_length = length;
        if (normalized_length == -1) {
          normalized_length = static_cast<int64_t>(s.size());
        }
        auto s_ptr = std::make_shared<std::u16string>(
            s, 0, std::max(normalized_length, int64_t{0}));
        ErrorCode error_code;
        auto p = utext_openUChars(ut.value_or(nullptr), s_ptr->data(), length,
                                  error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p, s_ptr);
      },
      py::keep_alive<1, 0>(), py::arg("ut"), py::arg("s"),
      py::arg("length") = -1);

  m.def(
      "utext_open_unicode_string",
      [](std::optional<icupy::UTextPtr> &ut, UnicodeString *s) {
        ErrorCode error_code;
        auto p = utext_openUnicodeString(ut.value_or(nullptr), s, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p);
      },
      py::arg("ut"), py::arg("s").none(false));

  m.def(
      "utext_open_utf8",
      [](std::optional<icupy::UTextPtr> &ut, const py::bytes &s,
         int64_t length) {
        const auto s_text = s.cast<std::string>();
        if (length == -1) {
          length = static_cast<int64_t>(s_text.size());
        }
        auto s_ptr = std::make_shared<std::string>(
            s_text, 0, std::max(length, int64_t{0}));
        ErrorCode error_code;
        auto p = utext_openUTF8(ut.value_or(nullptr), s_ptr->data(), length,
                                error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p, s_ptr);
      },
      py::keep_alive<1, 0>(), py::arg("ut"), py::arg("s"),
      py::arg("length") = -1);

  m.def(
      "utext_previous32",
      [](icupy::UTextPtr &ut) { return utext_previous32(ut); }, py::arg("ut"));

  m.def(
      "utext_previous32_from",
      [](icupy::UTextPtr &ut, int64_t native_index) {
        return utext_previous32From(ut, native_index);
      },
      py::arg("ut"), py::arg("native_index"));

  m.def(
      "utext_replace",
      [](icupy::UTextPtr &ut, int64_t native_start, int64_t native_limit,
         const std::u16string &replacement_text, int32_t replacement_length) {
        ErrorCode error_code;
        auto result = utext_replace(ut, native_start, native_limit,
                                    replacement_text.data(), replacement_length,
                                    error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("ut"), py::arg("native_start"), py::arg("native_limit"),
      py::arg("replacement_text"), py::arg("replacement_length"));

  m.def(
      "utext_set_native_index",
      [](icupy::UTextPtr &ut, int64_t native_index) {
        utext_setNativeIndex(ut, native_index);
      },
      py::arg("ut"), py::arg("native_index"));

  // FIXME: Add support for UText text provider.
  /*
  m.def(
      "utext_setup",
      [](std::optional<icupy::UTextPtr> &ut, int32_t extra_space) {
        ErrorCode error_code;
        auto p = utext_setup(ut.value_or(nullptr), extra_space, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UTextPtr>(p);
      },
      py::arg("ut"), py::arg("extra_space"));
  */

  m.attr("UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE") =
      int32_t{UTEXT_PROVIDER_LENGTH_IS_EXPENSIVE};
  m.attr("UTEXT_PROVIDER_STABLE_CHUNKS") =
      int32_t{UTEXT_PROVIDER_STABLE_CHUNKS};
  m.attr("UTEXT_PROVIDER_WRITABLE") = int32_t{UTEXT_PROVIDER_WRITABLE};
  m.attr("UTEXT_PROVIDER_HAS_META_DATA") =
      int32_t{UTEXT_PROVIDER_HAS_META_DATA};
  m.attr("UTEXT_PROVIDER_OWNS_TEXT") = int32_t{UTEXT_PROVIDER_OWNS_TEXT};
}

#ifndef ICUPY_UBIDIPTR_HPP
#define ICUPY_UBIDIPTR_HPP

#include <optional>
#include <pybind11/functional.h>
#include <unicode/ubidi.h>
#include <variant>

class _UBiDiPtr {
public:
  _UBiDiPtr(UBiDi *p);
  ~_UBiDiPtr();

  UBiDi *get() const;
  operator UBiDi *() const { return get(); }

  UBiDiLevel *get_embedding_levels() const;

  const UChar *get_epilogue() const { return epilogue_.size() == 0 ? nullptr : epilogue_.c_str(); }

  const UChar *get_prologue() const { return prologue_.size() == 0 ? nullptr : prologue_.c_str(); }

  const UChar *get_text() const { return text_.c_str(); }

  void set_embedding_levels(std::optional<std::vector<UBiDiLevel>> &embedding_levels);

  void set_epilogue(const UChar *epilogue, int32_t length);

  void set_prologue(const UChar *prologue, int32_t length);

  void set_text(const UChar *text, int32_t length);

private:
  _UBiDiPtr() = delete;
  UBiDi *p_;
  std::u16string text_;
  std::u16string prologue_;
  std::u16string epilogue_;
  std::optional<std::vector<UBiDiLevel>> embedding_levels_;
};

class _UBiDiClassCallbackPtr {
public:
  _UBiDiClassCallbackPtr(std::nullptr_t action);
  _UBiDiClassCallbackPtr(UBiDiClassCallback *action);
  _UBiDiClassCallbackPtr(const py::function &action);
  ~_UBiDiClassCallbackPtr();

  static UCharDirection callback(const void *context, UChar32 c);

  template <typename T> T get() const { return std::get<T>(action_); };

  template <typename T> auto get_if() const {
    return std::holds_alternative<T>(action_) ? std::get<T>(action_) : nullptr;
  };

  bool has_value() const {
    return !action_.valueless_by_exception() && (get_if<UBiDiClassCallback *>() || action_.index() != 0);
  };

private:
  _UBiDiClassCallbackPtr() = delete;
  std::variant<UBiDiClassCallback *, py::function> action_;
};

#endif // ICUPY_UBIDIPTR_HPP

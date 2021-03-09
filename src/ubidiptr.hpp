#ifndef ICUPY_UBIDIPTR_HPP
#define ICUPY_UBIDIPTR_HPP

#include <memory>
#include <pybind11/functional.h>
#include <unicode/ubidi.h>
#include <variant>

class _UBiDiPtr {
public:
  _UBiDiPtr(UBiDi *p);
  ~_UBiDiPtr();

  UBiDi *get() const;
  operator UBiDi *() const { return get(); }

  void set_embedding_levels(const std::shared_ptr<UBiDiLevel[]> &embedding_levels);

  void set_epilogue(const std::shared_ptr<UChar[]> &epilogue);

  void set_prologue(const std::shared_ptr<UChar[]> &prologue);

  void set_text(const std::shared_ptr<UChar[]> &text);

private:
  _UBiDiPtr() = delete;
  UBiDi *p_;
  std::shared_ptr<UChar[]> text_;
  std::shared_ptr<UChar[]> prologue_;
  std::shared_ptr<UChar[]> epilogue_;
  std::shared_ptr<UBiDiLevel[]> embedding_levels_;
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

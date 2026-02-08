#ifndef ICUPY_UBIDIPTR_HPP
#define ICUPY_UBIDIPTR_HPP

#include <functional>
#include <memory>
#include <unicode/ubidi.h>

namespace icupy {

class ConstVoidPtr;
class UBiDiClassCallbackPtr;

using ClassCallbackArgs = UCharDirection(py::object &, UChar32);

using ClassCallbackFunction = std::function<ClassCallbackArgs>;

using ClassCallbackAndContextPair =
    std::pair<const ClassCallbackFunction, const ConstVoidPtr *>;

using SharedClassCallbackAndContextPair =
    std::shared_ptr<ClassCallbackAndContextPair>;

//
// struct UBiDi
//
class UBiDiPtr {
public:
  UBiDiPtr(UBiDi *p);

  ~UBiDiPtr();

  UBiDi *get() const;

  operator UBiDi *() const { return get(); }

  void
  set_embedding_levels(const std::shared_ptr<UBiDiLevel[]> &embedding_levels);

  void set_epilogue(const std::shared_ptr<std::u16string> &epilogue);

  void set_prologue(const std::shared_ptr<std::u16string> &prologue);

  void set_text(const std::shared_ptr<std::u16string> &text);

private:
  UBiDiPtr() = delete;
  UBiDi *p_;
  std::shared_ptr<std::u16string> text_;
  std::shared_ptr<std::u16string> prologue_;
  std::shared_ptr<std::u16string> epilogue_;
  std::shared_ptr<UBiDiLevel[]> embedding_levels_;
};

//
// UBiDiClassCallback
//
class UBiDiClassCallbackPtr {
public:
  UBiDiClassCallbackPtr() {}

  UBiDiClassCallbackPtr(const std::function<ClassCallbackArgs> &action,
                        const ConstVoidPtr *context) {
    action_and_context_ =
        std::make_shared<ClassCallbackAndContextPair>(action, context);
  }

  UBiDiClassCallbackPtr(const ClassCallbackAndContextPair *pair) {
    action_and_context_ = std::make_shared<ClassCallbackAndContextPair>(
        pair->first, pair->second);
  }

  ~UBiDiClassCallbackPtr() {}

  ClassCallbackAndContextPair *context() const {
    return action_and_context_.get();
  }

  bool empty() const noexcept { return action_and_context_.get() == nullptr; };

  UBiDiClassCallback *get_native_callback() {
    return action_and_context_ ? callback : nullptr;
  }

private:
  static UCharDirection callback(const void *context, UChar32 c);

  SharedClassCallbackAndContextPair action_and_context_;
};

} // namespace icupy

#endif // ICUPY_UBIDIPTR_HPP

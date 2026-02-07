#ifndef ICUPY_UCPMAPPTR_HPP
#define ICUPY_UCPMAPPTR_HPP

#include "main.hpp"
#include "voidptr.hpp"
#include <functional>
#include <unicode/ucpmap.h>

namespace icupy {

using ValueFilterArgs = uint32_t(py::object &, uint32_t);

using ValueFilterFunction = std::function<ValueFilterArgs>;

using ValueFilterAndContextPair =
    std::pair<const ValueFilterFunction, const ConstVoidPtr *>;

using SharedValueFilterAndContextPair =
    std::shared_ptr<ValueFilterAndContextPair>;

//
// struct UCPMap
//
class UCPMapPtr {
public:
  UCPMapPtr(const UCPMap *p);

  ~UCPMapPtr();

  const UCPMap *get() const;

  operator const UCPMap *() const { return get(); }

private:
  UCPMapPtr() = delete;
  const UCPMap *p_;
};

//
// UCPMapValueFilter
//
class UCPMapValueFilterPtr {
public:
  UCPMapValueFilterPtr(const ValueFilterFunction &filter,
                       const ConstVoidPtr *context) {
    action_and_context_ =
        std::make_shared<ValueFilterAndContextPair>(filter, context);
  }

  ~UCPMapValueFilterPtr() {}

  ValueFilterAndContextPair *context() const {
    return action_and_context_.get();
  }

  UCPMapValueFilter *get_native_callback() {
    return action_and_context_ ? filter : nullptr;
  }

private:
  UCPMapValueFilterPtr() = delete;

  static uint32_t filter(const void *context, uint32_t value);

  SharedValueFilterAndContextPair action_and_context_;
};

} // namespace icupy

#endif // ICUPY_UCPMAPPTR_HPP

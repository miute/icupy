#ifndef ICUPY_UENUMPTR_HPP
#define ICUPY_UENUMPTR_HPP

#include <memory>
#include <unicode/uenum.h>

namespace icupy {

//
// struct UEnumeration
//
class UEnumerationPtr {
public:
  UEnumerationPtr(UEnumeration *p);

  UEnumerationPtr(UEnumeration *p, const std::shared_ptr<void> &source);

  ~UEnumerationPtr();

  UEnumeration *get() const;

  operator UEnumeration *() const { return get(); }

private:
  UEnumerationPtr() = delete;
  UEnumeration *p_;
  std::shared_ptr<void> source_;
};

} // namespace icupy

#endif // ICUPY_UENUMPTR_HPP

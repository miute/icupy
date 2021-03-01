#ifndef ICUPY_UENUMPTR_HPP
#define ICUPY_UENUMPTR_HPP

#include <memory>
#include <unicode/uenum.h>

class _UEnumerationPtr {
public:
  _UEnumerationPtr(UEnumeration *p);
  _UEnumerationPtr(UEnumeration *p, const std::shared_ptr<void> &source);
  ~_UEnumerationPtr();

  UEnumeration *get() const;
  operator UEnumeration *() const { return get(); }

private:
  _UEnumerationPtr() = delete;
  UEnumeration *p_;
  std::shared_ptr<void> source_;
};

#endif // ICUPY_UENUMPTR_HPP

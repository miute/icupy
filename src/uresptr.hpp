#ifndef ICUPY_URESPTR_HPP
#define ICUPY_URESPTR_HPP

#include <unicode/ures.h>

class _UResourceBundlePtr {
public:
  _UResourceBundlePtr(UResourceBundle *p);
  ~_UResourceBundlePtr();

  UResourceBundle *get() const;
  operator UResourceBundle *() const { return get(); }

private:
  _UResourceBundlePtr() = delete;
  UResourceBundle *p_;
};

#endif // ICUPY_URESPTR_HPP

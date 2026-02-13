#ifndef ICUPY_URESPTR_HPP
#define ICUPY_URESPTR_HPP

#include <unicode/ures.h>

namespace icupy {

//
// struct UResourceBundle
//
class UResourceBundlePtr {
public:
  UResourceBundlePtr(UResourceBundle *p);

  ~UResourceBundlePtr();

  UResourceBundle *get() const;

  operator UResourceBundle *() const { return get(); }

private:
  UResourceBundlePtr() = delete;
  UResourceBundle *p_;
};

} // namespace icupy

#endif // ICUPY_URESPTR_HPP

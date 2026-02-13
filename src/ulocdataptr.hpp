#ifndef ICUPY_ULOCDATAPTR_HPP
#define ICUPY_ULOCDATAPTR_HPP

#include <unicode/ulocdata.h>

namespace icupy {

//
// struct ULocaleData
//
class ULocaleDataPtr {
public:
  ULocaleDataPtr(ULocaleData *p);

  ~ULocaleDataPtr();

  ULocaleData *get() const;

  operator ULocaleData *() const { return get(); }

private:
  ULocaleDataPtr() = delete;
  ULocaleData *p_;
};

} // namespace icupy

#endif // ICUPY_ULOCDATAPTR_HPP

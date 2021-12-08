#ifndef ICUPY_ULOCDATAPTR_HPP
#define ICUPY_ULOCDATAPTR_HPP

#include <unicode/ulocdata.h>

class _ULocaleDataPtr {
public:
  _ULocaleDataPtr(ULocaleData *p);
  ~_ULocaleDataPtr();

  ULocaleData *get() const;
  operator ULocaleData *() const { return get(); }

private:
  _ULocaleDataPtr() = delete;
  ULocaleData *p_;
};

#endif // ICUPY_ULOCDATAPTR_HPP

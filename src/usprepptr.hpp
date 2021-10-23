#ifndef ICUPY_USPREPPTR_HPP
#define ICUPY_USPREPPTR_HPP

#include <unicode/usprep.h>

class _UStringPrepProfilePtr {
public:
  _UStringPrepProfilePtr(UStringPrepProfile *p);
  ~_UStringPrepProfilePtr();

  UStringPrepProfile *get() const;
  operator UStringPrepProfile *() const { return get(); }

private:
  _UStringPrepProfilePtr() = delete;
  UStringPrepProfile *p_;
};

#endif // ICUPY_USPREPPTR_HPP

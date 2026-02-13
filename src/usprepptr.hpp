#ifndef ICUPY_USPREPPTR_HPP
#define ICUPY_USPREPPTR_HPP

#include <unicode/usprep.h>

namespace icupy {

//
// struct UStringPrepProfile
//
class UStringPrepProfilePtr {
public:
  UStringPrepProfilePtr(UStringPrepProfile *p);

  ~UStringPrepProfilePtr();

  UStringPrepProfile *get() const;

  operator UStringPrepProfile *() const { return get(); }

private:
  UStringPrepProfilePtr() = delete;
  UStringPrepProfile *p_;
};

} // namespace icupy

#endif // ICUPY_USPREPPTR_HPP

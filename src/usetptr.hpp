#ifndef ICUPY_USETPTR_HPP
#define ICUPY_USETPTR_HPP

#include <unicode/uset.h>

namespace icupy {

//
// const USet structure
//
class ConstUSetPtr {
public:
  ConstUSetPtr(const USet *p);

  ~ConstUSetPtr();

  const USet *get() const;

  operator const USet *() const { return get(); }

private:
  ConstUSetPtr() = delete;
  const USet *p_;
};

//
// USet structure
//
class USetPtr {
public:
  USetPtr(USet *p);

  ~USetPtr();

  USet *get() const;

  operator USet *() const { return get(); }

private:
  USetPtr() = delete;
  USet *p_;
};

} // namespace icupy

#endif // ICUPY_USETPTR_HPP

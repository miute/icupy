#ifndef ICUPY_USETPTR_HPP
#define ICUPY_USETPTR_HPP

#include <unicode/uset.h>

class _ConstUSetPtr {
public:
  _ConstUSetPtr(const USet *p);
  ~_ConstUSetPtr();

  const USet *get() const;
  operator const USet *() const { return get(); }

private:
  _ConstUSetPtr() = delete;
  const USet *p_;
};

class _USetPtr {
public:
  _USetPtr(USet *p);
  ~_USetPtr();

  USet *get() const;
  operator USet *() const { return get(); }

private:
  _USetPtr() = delete;
  USet *p_;
};

#endif // ICUPY_USETPTR_HPP

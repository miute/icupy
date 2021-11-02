#ifndef ICUPY_USPOOFPTR_HPP
#define ICUPY_USPOOFPTR_HPP

#include <unicode/uspoof.h>

class _USpoofCheckerPtr {
public:
  _USpoofCheckerPtr(USpoofChecker *p);
  ~_USpoofCheckerPtr();

  USpoofChecker *get() const;
  operator USpoofChecker *() const { return get(); }

private:
  _USpoofCheckerPtr() = delete;
  USpoofChecker *p_;
};

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
class _USpoofCheckResultPtr {
public:
  _USpoofCheckResultPtr(USpoofCheckResult *p);
  ~_USpoofCheckResultPtr();

  USpoofCheckResult *get() const;
  operator USpoofCheckResult *() const { return get(); }

private:
  _USpoofCheckResultPtr() = delete;
  USpoofCheckResult *p_;
};
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

#endif // ICUPY_USPOOFPTR_HPP

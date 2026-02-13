#ifndef ICUPY_USPOOFPTR_HPP
#define ICUPY_USPOOFPTR_HPP

#include <unicode/uspoof.h>

namespace icupy {

//
// struct USpoofChecker
//
class USpoofCheckerPtr {
public:
  USpoofCheckerPtr(USpoofChecker *p);

  ~USpoofCheckerPtr();

  USpoofChecker *get() const;

  operator USpoofChecker *() const { return get(); }

private:
  USpoofCheckerPtr() = delete;
  USpoofChecker *p_;
};

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
//
// struct USpoofCheckResult
//
class USpoofCheckResultPtr {
public:
  USpoofCheckResultPtr(USpoofCheckResult *p);

  ~USpoofCheckResultPtr();

  USpoofCheckResult *get() const;

  operator USpoofCheckResult *() const { return get(); }

private:
  USpoofCheckResultPtr() = delete;
  USpoofCheckResult *p_;
};
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

} // namespace icupy

#endif // ICUPY_USPOOFPTR_HPP

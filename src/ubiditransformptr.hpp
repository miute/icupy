#ifndef ICUPY_UBIDITRANSFORMPTR_HPP
#define ICUPY_UBIDITRANSFORMPTR_HPP

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
#include <unicode/ubiditransform.h>

class _UBiDiTransformPtr {
public:
  _UBiDiTransformPtr(UBiDiTransform *p);
  ~_UBiDiTransformPtr();

  UBiDiTransform *get() const;
  operator UBiDiTransform *() const { return get(); }

private:
  _UBiDiTransformPtr() = delete;
  UBiDiTransform *p_;
};

#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

#endif // ICUPY_UBIDITRANSFORMPTR_HPP

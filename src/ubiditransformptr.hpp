#ifndef ICUPY_UBIDITRANSFORMPTR_HPP
#define ICUPY_UBIDITRANSFORMPTR_HPP

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
#include <unicode/ubiditransform.h>

namespace icupy {

//
// struct UBiDiTransform
//
class UBiDiTransformPtr {
public:
  UBiDiTransformPtr(UBiDiTransform *p);

  ~UBiDiTransformPtr();

  UBiDiTransform *get() const;

  operator UBiDiTransform *() const { return get(); }

private:
  UBiDiTransformPtr() = delete;
  UBiDiTransform *p_;
};

} // namespace icupy

#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)

#endif // ICUPY_UBIDITRANSFORMPTR_HPP

#ifndef ICUPY_CHAR16PTR_HPP
#define ICUPY_CHAR16PTR_HPP

#include <cstdint>

class _ConstChar16Ptr {
public:
  _ConstChar16Ptr(const char16_t *p, int32_t length, int32_t capacity);
  ~_ConstChar16Ptr();

  const char16_t *get() const;

  operator const char16_t *() const { return get(); }

  const char16_t operator[](int32_t index) const { return *(get() + index); }

  int32_t capacity() const { return capacity_; }

  int32_t length() const { return length_; }

private:
  _ConstChar16Ptr() = delete;
  const char16_t *p_;
  int32_t length_;
  int32_t capacity_;
};

#endif // ICUPY_CHAR16PTR_HPP

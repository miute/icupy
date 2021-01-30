#ifndef ICUPY_CHAR16PTR_HPP
#define ICUPY_CHAR16PTR_HPP

class _ConstChar16Ptr {
public:
  _ConstChar16Ptr(const char16_t *p);
  ~_ConstChar16Ptr();

  const char16_t *get() const;
  operator const char16_t *() const { return get(); }

private:
  _ConstChar16Ptr() = delete;
  const char16_t *p_;
};

#endif // ICUPY_CHAR16PTR_HPP

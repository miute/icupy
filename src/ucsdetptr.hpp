#ifndef ICUPY_UCSDETPTR_HPP
#define ICUPY_UCSDETPTR_HPP

#include <memory>
#include <unicode/ucsdet.h>

class _ConstUCharsetMatchPtr {
public:
  _ConstUCharsetMatchPtr(const UCharsetMatch *p);
  ~_ConstUCharsetMatchPtr();

  const UCharsetMatch *get() const;
  operator const UCharsetMatch *() const { return get(); }

private:
  _ConstUCharsetMatchPtr() = delete;
  const UCharsetMatch *p_;
};

class _UCharsetDetectorPtr {
public:
  _UCharsetDetectorPtr(UCharsetDetector *p);
  ~_UCharsetDetectorPtr();

  UCharsetDetector *get() const;
  operator UCharsetDetector *() const { return get(); }

  void set_source(const std::shared_ptr<void> &source);

private:
  _UCharsetDetectorPtr() = delete;
  UCharsetDetector *p_;
  std::shared_ptr<void> source_;
};

#endif // ICUPY_UCSDETPTR_HPP

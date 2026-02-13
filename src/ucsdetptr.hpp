#ifndef ICUPY_UCSDETPTR_HPP
#define ICUPY_UCSDETPTR_HPP

#include <memory>
#include <unicode/ucsdet.h>

namespace icupy {

//
// struct UCharsetMatch
//
class UCharsetMatchPtr {
public:
  UCharsetMatchPtr(const UCharsetMatch *p);

  ~UCharsetMatchPtr();

  const UCharsetMatch *get() const;

  operator const UCharsetMatch *() const { return get(); }

private:
  UCharsetMatchPtr() = delete;
  const UCharsetMatch *p_;
};

//
// struct UCharsetDetector
//
class UCharsetDetectorPtr {
public:
  UCharsetDetectorPtr(UCharsetDetector *p);

  ~UCharsetDetectorPtr();

  UCharsetDetector *get() const;

  operator UCharsetDetector *() const { return get(); }

  void set_source(std::unique_ptr<std::string> &source);

private:
  UCharsetDetectorPtr() = delete;
  UCharsetDetector *p_;
  std::unique_ptr<std::string> source_;
};

} // namespace icupy

#endif // ICUPY_UCSDETPTR_HPP

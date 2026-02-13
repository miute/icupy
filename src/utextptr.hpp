#ifndef ICUPY_UTEXTPTR_HPP
#define ICUPY_UTEXTPTR_HPP

#include <memory>
#include <unicode/utext.h>

namespace icupy {

using SharedVoidPtr = std::shared_ptr<void>;

//
// struct UText
//
class UTextPtr {
public:
  UTextPtr();

  UTextPtr(UText *p);

  UTextPtr(UText *p, const SharedVoidPtr &source);

  ~UTextPtr();

  UText *get() const;

  operator UText *() const { return get(); }

  bool operator!=(const UTextPtr &other) { return !utext_equals(get(), other); }

  bool operator==(const UTextPtr &other) { return utext_equals(get(), other); }

  const UText *operator->() const { return get(); }

  const SharedVoidPtr &get_source() const;

private:
  UText *p_;
  SharedVoidPtr source_;
};

} // namespace icupy

#endif // ICUPY_UTEXTPTR_HPP

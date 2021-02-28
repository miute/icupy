#ifndef ICUPY_UTEXTPTR_HPP
#define ICUPY_UTEXTPTR_HPP

#include <memory>
#include <unicode/utext.h>

class _UTextPtr {
public:
  _UTextPtr();
  _UTextPtr(UText *p);
  _UTextPtr(UText *p, const std::shared_ptr<void> &source);
  ~_UTextPtr();

  UText *get() const;
  operator UText *() const { return get(); }

  bool operator!=(const _UTextPtr &other) { return get() != other.get(); }
  bool operator==(const _UTextPtr &other) { return get() == other.get(); }
  const UText *operator->() const { return get(); }

  const std::shared_ptr<void> &get_source() const;

private:
  UText *p_;
  std::shared_ptr<void> source_;
};

#endif // ICUPY_UTEXTPTR_HPP

#ifndef ICUPY_UTEXTVEC_HPP
#define ICUPY_UTEXTVEC_HPP

#include "main.hpp"
#include "utextptr.hpp"

class _UTextVector {
public:
  _UTextVector(size_t n);
  _UTextVector(const _UnicodeStringList &iterable);
  ~_UTextVector();

  _UTextPtr operator[](size_t index) const { return values_[index]; }
  _UTextPtr &operator[](size_t index) { return values_[index]; }

  std::vector<_UTextPtr>::iterator begin() noexcept { return values_.begin(); }
  std::vector<_UTextPtr>::const_iterator begin() const noexcept { return values_.begin(); }

  void clear();

  _UTextPtr *data() noexcept { return values_.data(); };
  const _UTextPtr *data() const noexcept { return values_.data(); };

  std::vector<_UTextPtr>::iterator end() noexcept { return values_.end(); }
  std::vector<_UTextPtr>::const_iterator end() const noexcept { return values_.end(); }

  size_t size() const noexcept { return values_.size(); }

private:
  std::vector<_UTextPtr> values_;
  std::vector<icu::UnicodeString> sources_;
};

#endif // ICUPY_UTEXTVEC_HPP

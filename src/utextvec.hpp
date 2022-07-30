#ifndef ICUPY_UTEXTVEC_HPP
#define ICUPY_UTEXTVEC_HPP

#include "main.hpp"
#include "utextptr.hpp"
#include <functional>

class _UTextVector {
public:
  _UTextVector();
  _UTextVector(const std::list<std::reference_wrapper<icu::UnicodeString>> &iterable);
  ~_UTextVector();

  _UTextPtr operator[](size_t index) const { return values_[index]; }
  _UTextPtr &operator[](size_t index) { return values_[index]; }

  void append(icu::UnicodeString &src);
  void append(UText *ut, icu::UnicodeString &src);

  std::vector<_UTextPtr>::iterator begin() noexcept { return values_.begin(); }
  std::vector<_UTextPtr>::const_iterator begin() const noexcept { return values_.begin(); }

  void clear();

  _UTextPtr *data() noexcept { return values_.data(); };
  const _UTextPtr *data() const noexcept { return values_.data(); };

  std::vector<_UTextPtr>::iterator end() noexcept { return values_.end(); }
  std::vector<_UTextPtr>::const_iterator end() const noexcept { return values_.end(); }

  void extend(const std::list<std::reference_wrapper<icu::UnicodeString>> &iterable);

  void insert(int32_t index, icu::UnicodeString &src);

  void remove(int32_t index);

  size_t size() const noexcept { return values_.size(); }

  // icu::UnicodeString &source(size_t index) const { return sources_[index].get(); }

  // UText *value(size_t index) const { return values_[index]; }

private:
  std::vector<_UTextPtr> values_;
  std::vector<std::reference_wrapper<icu::UnicodeString>> sources_;
};

#endif // ICUPY_UTEXTVEC_HPP

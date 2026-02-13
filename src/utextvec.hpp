#ifndef ICUPY_UTEXTVEC_HPP
#define ICUPY_UTEXTVEC_HPP

#include "utextptr.hpp"
#include <list>
#include <vector>

namespace icupy {

//
// class UTextVector
//
class UTextVector {
public:
  UTextVector();

  UTextVector(
      const std::list<std::reference_wrapper<icu::UnicodeString>> &iterable);

  ~UTextVector();

  UTextPtr operator[](size_t index) const { return values_[index]; }

  UTextPtr &operator[](size_t index) { return values_[index]; }

  void append(icu::UnicodeString &src);

  void append(UText *ut, icu::UnicodeString &src);

  std::vector<UTextPtr>::iterator begin() noexcept { return values_.begin(); }

  std::vector<UTextPtr>::const_iterator begin() const noexcept {
    return values_.begin();
  }

  void clear();

  UTextPtr *data() noexcept { return values_.data(); };

  const UTextPtr *data() const noexcept { return values_.data(); };

  std::vector<UTextPtr>::iterator end() noexcept { return values_.end(); }

  std::vector<UTextPtr>::const_iterator end() const noexcept {
    return values_.end();
  }

  void
  extend(const std::list<std::reference_wrapper<icu::UnicodeString>> &iterable);

  void insert(int32_t index, icu::UnicodeString &src);

  void remove(int32_t index);

  size_t size() const noexcept { return values_.size(); }

  // icu::UnicodeString &source(size_t index) const { return
  // sources_[index].get(); }

  // UText *value(size_t index) const { return values_[index]; }

private:
  std::vector<UTextPtr> values_;
  std::vector<std::reference_wrapper<icu::UnicodeString>> sources_;
};

} // namespace icupy

#endif // ICUPY_UTEXTVEC_HPP

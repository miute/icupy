#ifndef ICUPY_UNISTRLIST_HPP
#define ICUPY_UNISTRLIST_HPP

#include "main.hpp"
#include <algorithm>
#include <list>
#include <unicode/coll.h>

namespace icupy {

//
// icupy::UnicodeStringList
//
class UnicodeStringList {
private:
  std::vector<icu::UnicodeString> strings_;

public:
  UnicodeStringList() = default;

  UnicodeStringList(std::size_t size) : strings_(size) {}

  UnicodeStringList(const std::list<icu::UnicodeString> &iterable)
      : strings_(iterable.cbegin(), iterable.cend()) {}

#if (U_ICU_VERSION_MAJOR_NUM >= 76)
  UnicodeStringList(const std::list<std::u16string> &iterable)
      : strings_(iterable.cbegin(), iterable.cend()) {}
#else
  UnicodeStringList(const std::list<std::u16string> &iterable) {
    strings_.reserve(iterable.size());
    for (const auto &value : iterable) {
      strings_.emplace_back(value.data(), static_cast<int32_t>(value.size()));
    }
  }
#endif

  UnicodeStringList(const UnicodeStringList &other)
      : strings_(other.strings_) {}

  ~UnicodeStringList() = default;

  void append(const UnicodeStringVariant &value) { append(to_unistr(value)); }

  void append(const icu::UnicodeString &value) { strings_.push_back(value); }

  void append(const std::u16string &value) {
#if (U_ICU_VERSION_MAJOR_NUM >= 76)
    strings_.emplace_back(value);
#else
    strings_.emplace_back(value.data(), static_cast<int32_t>(value.size()));
#endif
  }

  icu::UnicodeString &at(std::size_t index) { return strings_.at(index); }

  const icu::UnicodeString &at(std::size_t index) const {
    return strings_.at(index);
  }

  std::vector<icu::UnicodeString>::iterator begin() noexcept {
    return strings_.begin();
  }

  std::vector<icu::UnicodeString>::const_iterator begin() const noexcept {
    return strings_.begin();
  }

  void clear() { strings_.clear(); }

  std::size_t count(const UnicodeStringVariant &value) const;

  icu::UnicodeString *data() noexcept { return strings_.data(); }

  const icu::UnicodeString *data() const noexcept { return strings_.data(); }

  std::vector<icu::UnicodeString>::iterator end() noexcept {
    return strings_.end();
  }

  std::vector<icu::UnicodeString>::const_iterator end() const noexcept {
    return strings_.end();
  }

  std::vector<icu::UnicodeString>::iterator
  erase(std::vector<icu::UnicodeString>::iterator pos) {
    return strings_.erase(pos);
  }

  std::vector<icu::UnicodeString>::iterator
  erase(std::vector<icu::UnicodeString>::iterator first,
        std::vector<icu::UnicodeString>::iterator last) {
    return strings_.erase(first, last);
  }

  UnicodeStringList &extend(const UnicodeStringList &other);

  UnicodeStringList &extend(const std::list<icu::UnicodeString> &iterable);

  UnicodeStringList &extend(const std::list<std::u16string> &iterable);

  std::size_t index(const UnicodeStringVariant &value, int32_t start = 0,
                    int32_t stop = -1) const;

  void insert(int32_t index, const UnicodeStringVariant &value) {
    insert(index, to_unistr(value));
  }

  void insert(int32_t index, const icu::UnicodeString &value);

  void insert(std::size_t index,
              std::vector<icu::UnicodeString>::const_iterator first,
              std::vector<icu::UnicodeString>::const_iterator last) {
    strings_.insert(strings_.begin() + index, first, last);
  }

  icu::UnicodeString &operator[](std::size_t index) { return strings_[index]; }

  const icu::UnicodeString &operator[](std::size_t index) const {
    return strings_[index];
  }

  icu::UnicodeString pop(int32_t index = -1);

  void remove(const UnicodeStringVariant &value);

  void reserve(std::size_t size) { strings_.reserve(size); }

  void reverse() { std::reverse(strings_.begin(), strings_.end()); }

  std::size_t size() const { return strings_.size(); }

  void sort(icu::Collator *coll = nullptr, bool reverse = false);
};

} // namespace icupy

#endif // ICUPY_UNISTRLIST_HPP

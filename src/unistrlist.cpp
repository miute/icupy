#include "unistrlist.hpp"
#include "pybind11/pytypes.h"
#include <iomanip>
#include <pybind11/stl.h>
#include <sstream>

using namespace icu;

namespace icupy {

//
// icupy::UnicodeStringList
//
std::size_t UnicodeStringList::count(const UnicodeStringVariant &value) const {
  const auto b = to_unistr(value);
  return std::count_if(
      strings_.begin(), strings_.end(),
      [&b](const UnicodeString &a) { return a.compare(b) == 0; });
}

UnicodeStringList &UnicodeStringList::extend(const UnicodeStringList &other) {
  strings_.reserve(strings_.size() + other.size());
  for (const auto &value : other) {
    strings_.push_back(value);
  }
  return *this;
}

UnicodeStringList &
UnicodeStringList::extend(const std::list<UnicodeString> &iterable) {
  strings_.reserve(strings_.size() + iterable.size());
  for (const auto &value : iterable) {
    strings_.push_back(value);
  }
  return *this;
}

UnicodeStringList &
UnicodeStringList::extend(const std::list<std::u16string> &iterable) {
  strings_.reserve(strings_.size() + iterable.size());
  for (const auto &value : iterable) {
#if (U_ICU_VERSION_MAJOR_NUM >= 76)
    strings_.emplace_back(value);
#else
    strings_.emplace_back(value.data(), static_cast<int32_t>(value.size()));
#endif
  }
  return *this;
}

std::size_t UnicodeStringList::index(const UnicodeStringVariant &value,
                                     int32_t start, int32_t stop) const {
  const auto size = static_cast<int32_t>(strings_.size());
  if (start < 0) {
    start += size;
  }
  if (stop < 0) {
    stop += size;
  }
  start = std::max(0, std::min(start, size - 1));
  stop = std::max(0, std::min(stop, size - 1));
  const auto b = to_unistr(value);
  for (std::size_t i = start; i <= stop; ++i) {
    if (strings_[i].compare(b) == 0) {
      return i;
    }
  }
  std::string s;
  b.toUTF8String<std::string>(s);
  std::stringstream ss;
  ss << std::quoted(s, '\'');
  ss << " not in list";
  throw py::value_error(ss.str());
}

void UnicodeStringList::insert(int32_t index, const UnicodeString &value) {
  const auto size = static_cast<int32_t>(strings_.size());
  if (index < 0) {
    index += size;
  }
  index = std::max(0, std::min(index, size));
  strings_.insert(strings_.begin() + index, value);
}

UnicodeString UnicodeStringList::pop(int32_t index) {
  auto actual_index = index;
  const auto size = static_cast<int32_t>(strings_.size());
  if (actual_index < 0) {
    actual_index += size;
  }
  if (actual_index < 0 || actual_index >= size) {
    throw py::index_error("list index out of range: " + std::to_string(index));
  }
  UnicodeString value = strings_.at(actual_index);
  strings_.erase(strings_.begin() + actual_index);
  return value;
}

void UnicodeStringList::remove(const UnicodeStringVariant &value) {
  const auto b = to_unistr(value);
  for (auto it = strings_.begin(); it != strings_.end(); ++it) {
    if (it->compare(b) == 0) {
      strings_.erase(it);
      return;
    }
  }
  std::string s;
  b.toUTF8String<std::string>(s);
  std::stringstream ss;
  ss << std::quoted(s, '\'');
  ss << " not in list";
  throw py::value_error(ss.str());
}

void UnicodeStringList::sort(icu::Collator *coll, bool reverse) {
  if (coll) {
    std::stable_sort(
        strings_.begin(), strings_.end(),
        [&](const icu::UnicodeString &a, const icu::UnicodeString &b) {
          ErrorCode error_code;
          auto comp = coll->compare(a, b, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return reverse ? (comp > 0) : (comp < 0);
        });
  } else {
    std::stable_sort(
        strings_.begin(), strings_.end(),
        [&](const icu::UnicodeString &a, const icu::UnicodeString &b) {
          auto comp = a.compare(b);
          return reverse ? (comp > 0) : (comp < 0);
        });
  }
}

} // namespace icupy

void init_unistrlist(py::module &m) {
  //
  // class icupy::UnicodeStringList
  //
  py::class_<icupy::UnicodeStringList> usl(m, "UnicodeStringList", R"doc(
      Mutable sequence of :class:`UnicodeString` objects.
      )doc");

  usl.def(py::init<>(), R"doc(
      Initialize a ``UnicodeStringList`` instance as an empty list.
      )doc")
      .def(py::init<std::size_t>(), py::arg("size"), R"doc(
      Initialize a ``UnicodeStringList`` instance with the specified *size*.

      The list will be filled with the empty string.
      )doc")
      .def(py::init<const icupy::UnicodeStringList &>(), py::arg("other"),
           R"doc(
      Initialize a ``UnicodeStringList`` instance from another
      ``UnicodeStringList``.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``UnicodeStringList``
         will not be reflected.
      )doc")
      .def(py::init<const std::list<UnicodeString> &>(), py::arg("iterable"),
           R"doc(
      Initialize a ``UnicodeStringList`` instance with the specified
      *iterable*.

      .. note::

         Each :class:`UnicodeString` in *iterable* will be copied: changes
         made to the string object after passing it to ``UnicodeStringList``
         will not be reflected.
      )doc")
      .def(py::init<const std::list<std::u16string> &>(), py::arg("iterable"),
           R"doc(
      Initialize a ``UnicodeStringList`` instance with the specified
      *iterable*.
      )doc");

  usl.def(
         "__add__",
         [](const icupy::UnicodeStringList &self,
            const icupy::UnicodeStringList &other) {
           const auto size = self.size();
           auto result = std::make_unique<icupy::UnicodeStringList>(self);
           result->reserve(size + other.size());
           for (const auto &other : other) {
             result->append(other);
           }
           return result;
         },
         py::arg("other"), R"doc(
      Concatenate this ``UnicodeStringList`` with the contents of *other* and
      return the result as a new ``UnicodeStringList``.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``__add__()``
         will not be reflected.
      )doc")
      .def(
          "__add__",
          [](const icupy::UnicodeStringList &self,
             const std::list<UnicodeString> &other) {
            const auto size = self.size();
            auto result = std::make_unique<icupy::UnicodeStringList>(self);
            result->reserve(size + other.size());
            for (const auto &other : other) {
              result->append(other);
            }
            return result;
          },
          py::arg("other"), R"doc(
      Concatenate this ``UnicodeStringList`` with the contents of *other* and
      return the result as a new ``UnicodeStringList``.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``__add__()``
         will not be reflected.
      )doc")
      .def(
          "__add__",
          [](const icupy::UnicodeStringList &self,
             const std::list<std::u16string> &other) {
            const auto size = self.size();
            auto result = std::make_unique<icupy::UnicodeStringList>(self);
            result->reserve(size + other.size());
            for (const auto &other : other) {
              result->append(other);
            }
            return result;
          },
          py::arg("other"), R"doc(
      Concatenate this ``UnicodeStringList`` with the contents of *other* and
      return the result as a new ``UnicodeStringList``.
      )doc");

  usl.def(
      "__contains__",
      [](const icupy::UnicodeStringList &self,
         const icupy::UnicodeStringVariant &value) {
        return self.count(value) > 0;
      },
      py::arg("value"), R"doc(
      Return ``True`` if *value* is in the list, ``False`` otherwise.
      )doc");

  usl.def(
         "__delitem__",
         [](icupy::UnicodeStringList &self, int32_t index) {
           auto actual_index = index;
           const auto size = static_cast<int32_t>(self.size());
           if (actual_index < 0) {
             actual_index += size;
           }
           if (actual_index < 0 || actual_index >= size) {
             throw py::index_error("list index out of range: " +
                                   std::to_string(index));
           }
           self.erase(self.begin() + actual_index);
         },
         py::arg("index"), R"doc(
      Remove the item at the specified *index* from the list.
      )doc")
      .def(
          "__delitem__",
          [](icupy::UnicodeStringList &self, const py::slice &slice) {
            py::ssize_t start = 0, stop = 0, step = 1, slice_length = 0;
            if (!slice.compute(self.size(), &start, &stop, &step,
                               &slice_length)) {
              throw py::error_already_set();
            }
            std::list<std::size_t> indices(slice_length);
            auto it = indices.begin();
            for (py::ssize_t n = 0; n < slice_length; ++n, start += step) {
              *it++ = start;
            }
            if (step > 0) {
              indices.reverse();
            }
            for (auto index : indices) {
              self.erase(self.begin() + index);
            }
          },
          py::arg("slice"), R"doc(
      Remove the items at the specified *range(start, stop, step)* from the
      list.
      )doc");

  usl.def(
         "__getitem__",
         [](icupy::UnicodeStringList &self, int32_t index) -> UnicodeString & {
           auto actual_index = index;
           const auto size = static_cast<int32_t>(self.size());
           if (actual_index < 0) {
             actual_index += size;
           }
           if (actual_index < 0 || actual_index >= size) {
             throw py::index_error("list index out of range: " +
                                   std::to_string(index));
           }
           return self[actual_index];
         },
         py::return_value_policy::reference_internal, py::arg("index"), R"doc(
      Return the item at the specified *index*.
      )doc")
      .def(
          "__getitem__",
          [](const icupy::UnicodeStringList &self, const py::slice &slice) {
            py::ssize_t start, stop, step, slice_length;
            if (!slice.compute(self.size(), &start, &stop, &step,
                               &slice_length)) {
              throw py::error_already_set();
            }
            auto result =
                std::make_unique<icupy::UnicodeStringList>(slice_length);
            for (py::ssize_t n = 0; n < slice_length; ++n, start += step) {
              (*result)[n] = self[start];
            }
            return result;
          },
          py::arg("slice"), R"doc(
      Return a new ``UnicodeStringList`` containing the items at the
      specified *range(start, stop, step)*.
      )doc");

  usl.def("__iadd__",
          py::overload_cast<const icupy::UnicodeStringList &>(
              &icupy::UnicodeStringList::extend),
          py::arg("other"), R"doc(
      Extend this ``UnicodeStringList`` with the contents of *other* and
      return this ``UnicodeStringList`` itself.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``__iadd__()``
         will not be reflected.
      )doc")
      .def("__iadd__",
           py::overload_cast<const std::list<UnicodeString> &>(
               &icupy::UnicodeStringList::extend),
           py::arg("other"), R"doc(
      Extend this ``UnicodeStringList`` with the contents of *other* and
      return this ``UnicodeStringList`` itself.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``__iadd__()``
         will not be reflected.
      )doc")
      .def("__iadd__",
           py::overload_cast<const std::list<std::u16string> &>(
               &icupy::UnicodeStringList::extend),
           py::arg("other"), R"doc(
      Extend this ``UnicodeStringList`` with the contents of *other* and
      return this ``UnicodeStringList`` itself.
      )doc");

  usl.def("__len__", &icupy::UnicodeStringList::size, R"doc(
      Return the number of items in the list.
      )doc");

  usl.def("__repr__", [](const icupy::UnicodeStringList &self) {
    std::stringstream ss;
    ss << "UnicodeStringList([";
    for (std::size_t n = 0; n < self.size(); ++n) {
      std::string s;
      self[n].toUTF8String<std::string>(s);
      if (n > 0) {
        ss << ", ";
      }
      ss << std::quoted(s, '\'');
    }
    ss << "])";
    return ss.str();
  });

  usl.def(
         "__setitem__",
         [](icupy::UnicodeStringList &self, int32_t index,
            const icupy::UnicodeStringVariant &value) {
           auto actual_index = index;
           const auto size = static_cast<int32_t>(self.size());
           if (actual_index < 0) {
             actual_index += size;
           }
           if (actual_index < 0 || actual_index >= size) {
             throw py::index_error("list index out of range: " +
                                   std::to_string(index));
           }
           self[actual_index] = icupy::to_unistr(value);
         },
         py::arg("index"), py::arg("value").none(false), R"doc(
      Set the item at the specified *index* to *value*.

      .. note::

         An instance of :class:`UnicodeString` will be copied: changes made to
         the string object after passing it to ``__setitem__()`` will not be
         reflected.
      )doc")
      .def(
          "__setitem__",
          [](icupy::UnicodeStringList &self, const py::slice &slice,
             const icupy::UnicodeStringList &values) {
            py::ssize_t start, stop, step, slice_length;
            if (!slice.compute(self.size(), &start, &stop, &step,
                               &slice_length)) {
              throw py::error_already_set();
            } else if (step != 1 && slice_length != values.size()) {
              throw py::value_error("attempt to assign sequence of size " +
                                    std::to_string(values.size()) +
                                    " to extended slice of size " +
                                    std::to_string(slice_length));
            }
            py::ssize_t n = 0;
            for (; n < slice_length && n < values.size(); ++n, start += step) {
              self[start] = values[n];
            }
            if (n < slice_length) {
              self.erase(self.begin() + start, self.begin() + stop);
            } else if (n < values.size()) {
              self.reserve(self.size() + values.size() - n);
              self.insert(start, values.begin() + n, values.end());
            }
          },
          py::arg("slice"), py::arg("values"), R"doc(
      Set the items at the specified *range(start, stop, step)* to *values*.

      .. note::

         Each :class:`UnicodeString` in *values* will be copied: changes
         made to the string object after passing it to ``__setitem__()``
         will not be reflected.
      )doc");

  usl.def("append",
          py::overload_cast<const icupy::UnicodeStringVariant &>(
              &icupy::UnicodeStringList::append),
          py::arg("value"), R"doc(
      Append a new item to the end of the list.

      .. note::

         An instance of :class:`UnicodeString` will be copied: changes made to
         the string object after passing it to ``append()`` will not be
         reflected.
      )doc");

  usl.def("clear", &icupy::UnicodeStringList::clear, R"doc(
      Remove all items from the list.
      )doc");

  usl.def("count", &icupy::UnicodeStringList::count, py::arg("value"), R"doc(
      Return the number of occurrences of *value* in the list.
      )doc");

  usl.def("extend",
          py::overload_cast<const icupy::UnicodeStringList &>(
              &icupy::UnicodeStringList::extend),
          py::arg("other"), R"doc(
      Extend this ``UnicodeStringList`` with the contents of *other* and
      return this ``UnicodeStringList`` itself.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``extend()``
         will not be reflected.
      )doc")
      .def("extend",
           py::overload_cast<const std::list<UnicodeString> &>(
               &icupy::UnicodeStringList::extend),
           py::arg("other"), R"doc(
      Extend this ``UnicodeStringList`` with the contents of *other* and
      return this ``UnicodeStringList`` itself.

      .. note::

         Each :class:`UnicodeString` in *other* will be copied: changes
         made to the string object after passing it to ``extend()``
         will not be reflected.
      )doc")
      .def("extend",
           py::overload_cast<const std::list<std::u16string> &>(
               &icupy::UnicodeStringList::extend),
           py::arg("other"), R"doc(
      Extend this ``UnicodeStringList`` with the contents of *other* and
      return this ``UnicodeStringList`` itself.
      )doc");

  usl.def("index", &icupy::UnicodeStringList::index, py::arg("value"),
          py::arg("start") = 0, py::arg("stop") = -1, R"doc(
      Return the index of the first occurrence of *value* in the list.

      Raise :class:`ValueError` if *value* is not found in the list.
      )doc");

  usl.def("insert",
          py::overload_cast<int32_t, const icupy::UnicodeStringVariant &>(
              &icupy::UnicodeStringList::insert),
          py::arg("index"), py::arg("value"), R"doc(
      Insert a new item at the specified *index*.

      .. note::

         An instance of :class:`UnicodeString` will be copied: changes made to
         the string object after passing it to ``insert()`` will not be
         reflected.
      )doc");

  usl.def("pop", py::overload_cast<int32_t>(&icupy::UnicodeStringList::pop),
          py::arg("index") = -1, R"doc(
      Remove the item at the pecified *index* and return it.

      If *index* is not specified, the last item is removed.
      )doc");

  usl.def("remove", &icupy::UnicodeStringList::remove, py::arg("value"), R"doc(
      Remove the first occurrence of *value* in the list.

      Raise :class:`ValueError` if *value* is not found in the list.
      )doc");

  usl.def("reverse", &icupy::UnicodeStringList::reverse, R"doc(
      Reverse the order of the elements in the list.
      )doc");

  usl.def(
      "sort",
      [](icupy::UnicodeStringList &self, std::optional<icu::Collator *> &coll,
         bool reverse) { self.sort(coll.value_or(nullptr), reverse); },
      py::kw_only(), py::arg("coll") = std::nullopt, py::arg("reverse") = false,
      R"doc(
      Sort the elements of the list in place.
      )doc");
}

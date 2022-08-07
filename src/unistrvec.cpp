#include "main.hpp"
#include <iomanip>
#include <pybind11/stl.h>
#include <sstream>

using namespace icu;

void init_unistrvec(py::module & /*m*/, py::class_<_UnicodeStringVector> &usv) {
  //
  // _UnicodeStringVector (std::vector<icu::UnicodeString>)
  //
  usv.def(py::init<>())
      .def(py::init<size_t>(), py::arg("n"))
      .def(py::init<const _UnicodeStringVector &>(), py::arg("other"))
      .def(py::init([](const std::list<UnicodeString> &iterable) {
             return std::make_unique<_UnicodeStringVector>(iterable.begin(), iterable.end());
           }),
           py::arg("iterable"))
      .def(py::init([](const std::list<std::u16string> &iterable) {
             auto result = std::make_unique<_UnicodeStringVector>();
             result->reserve(iterable.size());
             for (auto &item : iterable) {
               result->push_back(item.c_str());
             }
             return result;
           }),
           py::arg("iterable"));

  usv.def("__bool__", [](const _UnicodeStringVector &self) -> bool { return !self.empty(); });

  usv.def(
      "__contains__",
      [](const _UnicodeStringVector &self, const _UnicodeStringVariant &item) {
        return std::find(self.begin(), self.end(), VARIANT_TO_UNISTR(item)) != self.end();
      },
      py::arg("item"));

  usv.def(
         "__delitem__",
         [](_UnicodeStringVector &self, int32_t index) {
           const auto size = static_cast<int32_t>(self.size());
           if (index < 0) {
             index += size;
           }
           if (index < 0 || index >= size) {
             throw py::index_error("list index out of range: " + std::to_string(index));
           }
           self.erase(self.begin() + index);
         },
         py::arg("index"))
      .def(
          "__delitem__",
          [](_UnicodeStringVector &self, const py::slice &index) {
            size_t start = 0, stop = 0, step = 0, slice_length = 0;
            if (!index.compute(self.size(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }
            for (size_t n = 0; n < slice_length; ++n) {
              self.erase(self.begin() + start + (slice_length - n - 1) * step);
            }
          },
          py::arg("index"));

  usv.def(
      "__eq__", [](const _UnicodeStringVector &self, const _UnicodeStringVector &other) { return self == other; },
      py::arg("other"));

  usv.def(
         "__getitem__",
         [](_UnicodeStringVector &self, int32_t index) -> UnicodeString & {
           const auto size = static_cast<int32_t>(self.size());
           if (index < 0) {
             index += size;
           }
           if (index < 0 || index >= size) {
             throw py::index_error("list index out of range: " + std::to_string(index));
           }
           return self[index];
         },
         py::return_value_policy::reference_internal, py::arg("index"))
      .def(
          "__getitem__",
          [](const _UnicodeStringVector &self, const py::slice &index) {
            size_t start = 0, stop = 0, step = 0, slice_length = 0;
            if (!index.compute(self.size(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            }
            auto result = std::make_unique<_UnicodeStringVector>();
            result->reserve(slice_length);
            for (size_t n = 0; n < slice_length; ++n, start += step) {
              result->push_back(self[start]);
            }
            return result;
          },
          py::return_value_policy::reference_internal, py::arg("index"));

  usv.def(
         "__iadd__",
         [](_UnicodeStringVector &self, const _UnicodeStringVariant &item) -> _UnicodeStringVector & {
           self.push_back(VARIANT_TO_UNISTR(item));
           return self;
         },
         py::arg("item"))
      .def(
          "__iadd__",
          [](_UnicodeStringVector &self, const std::list<UnicodeString> &iterable) -> _UnicodeStringVector & {
            self.insert(self.end(), iterable.begin(), iterable.end());
            return self;
          },
          py::arg("iterable"))
      .def(
          "__iadd__",
          [](_UnicodeStringVector &self, const std::list<std::u16string> &iterable) -> _UnicodeStringVector & {
            self.reserve(self.size() + iterable.size());
            for (auto &item : iterable) {
              self.push_back(item.c_str());
            }
            return self;
          },
          py::arg("iterable"));

  usv.def(
      "__iter__", [](const _UnicodeStringVector &self) { return py::make_iterator(self.begin(), self.end()); },
      py::keep_alive<0, 1>());

  usv.def("__len__", &_UnicodeStringVector::size);

  usv.def(
      "__ne__", [](const _UnicodeStringVector &self, const _UnicodeStringVector &other) { return self != other; },
      py::arg("other"));

  usv.def("__repr__", [](const _UnicodeStringVector &self) {
    std::stringstream ss;
    ss << "UnicodeStringVector([";
    for (size_t n = 0; n < self.size(); ++n) {
      std::string str;
      self[n].toUTF8String<std::string>(str);
      if (n > 0) {
        ss << ", ";
      }
      ss << std::quoted(str, '\'');
    }
    ss << "])";
    return ss.str();
  });

  usv.def("__reversed__", [](const _UnicodeStringVector &self) {
    return std::make_unique<_UnicodeStringVector>(self.rbegin(), self.rend());
  });

  usv.def(
         "__setitem__",
         [](_UnicodeStringVector &self, int32_t index, const _UnicodeStringVariant &item) {
           const auto size = static_cast<int32_t>(self.size());
           if (index < 0) {
             index += size;
           }
           if (index < 0 || index >= size) {
             throw py::index_error("list index out of range: " + std::to_string(index));
           }
           self[index] = VARIANT_TO_UNISTR(item);
         },
         py::arg("index"), py::arg("item"))
      .def(
          "__setitem__",
          [](_UnicodeStringVector &self, const py::slice &index, const std::list<UnicodeString> &iterable) {
            size_t start = 0, stop = 0, step = 0, slice_length = 0;
            if (!index.compute(self.size(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            } else if (slice_length != iterable.size()) {
              // TODO: Emulate `t[:] = []`.
              throw py::value_error("attempt to assign sequence of size " + std::to_string(iterable.size()) +
                                    " to extended slice of size " + std::to_string(slice_length));
            }
            for (auto &item : iterable) {
              self[start] = item;
              start += step;
            }
          },
          py::arg("index"), py::arg("iterable"))
      .def(
          "__setitem__",
          [](_UnicodeStringVector &self, const py::slice &index, const std::list<std::u16string> &iterable) {
            size_t start = 0, stop = 0, step = 0, slice_length = 0;
            if (!index.compute(self.size(), &start, &stop, &step, &slice_length)) {
              throw py::error_already_set();
            } else if (slice_length != iterable.size()) {
              // TODO: Emulate `t[:] = []`.
              throw py::value_error("attempt to assign sequence of size " + std::to_string(iterable.size()) +
                                    " to extended slice of size " + std::to_string(slice_length));
            }
            for (auto &item : iterable) {
              self[start] = item.c_str();
              start += step;
            }
          },
          py::arg("index"), py::arg("iterable"));

  usv.def(
      "append",
      [](_UnicodeStringVector &self, const _UnicodeStringVariant &item) { self.push_back(VARIANT_TO_UNISTR(item)); },
      py::arg("item"));

  usv.def("clear", &_UnicodeStringVector::clear);

  usv.def(
      "count",
      [](const _UnicodeStringVector &self, const _UnicodeStringVariant &item) {
        return std::count(self.begin(), self.end(), VARIANT_TO_UNISTR(item));
      },
      py::arg("item"));

  usv.def(
         "extend",
         [](_UnicodeStringVector &self, const std::list<UnicodeString> &iterable) {
           self.insert(self.end(), iterable.begin(), iterable.end());
         },
         py::arg("iterable"))
      .def(
          "extend",
          [](_UnicodeStringVector &self, const std::list<std::u16string> &iterable) {
            self.reserve(self.size() + iterable.size());
            for (auto &item : iterable) {
              self.push_back(item.c_str());
            }
          },
          py::arg("iterable"));

  usv.def(
      "insert",
      [](_UnicodeStringVector &self, int32_t index, const _UnicodeStringVariant &item) {
        const auto size = static_cast<int32_t>(self.size());
        if (index < 0) {
          index += size;
        }
        if (index < 0 || index >= size) {
          throw py::index_error("list index out of range: " + std::to_string(index));
        }
        self.insert(self.begin() + index, VARIANT_TO_UNISTR(item));
      },
      py::arg("index"), py::arg("item"));

  usv.def(
      "pop",
      [](_UnicodeStringVector &self, int32_t index) {
        const auto size = static_cast<int32_t>(self.size());
        if (index < 0) {
          index += size;
        }
        if (index < 0 || index >= size) {
          throw py::index_error("list index out of range: " + std::to_string(index));
        }
        auto result = std::move(self[index]);
        self.erase(self.begin() + index);
        return result;
      },
      py::arg("index") = -1);

  usv.def(
      "remove",
      [](_UnicodeStringVector &self, const _UnicodeStringVariant &item) {
        auto result = std::find(self.begin(), self.end(), VARIANT_TO_UNISTR(item));
        if (result == self.end()) {
          throw py::value_error("item not in list");
        }
        self.erase(result);
      },
      py::arg("item"));

  usv.def("reverse", [](_UnicodeStringVector &self) { std::reverse(self.begin(), self.end()); });
}

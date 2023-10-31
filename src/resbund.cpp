#include "main.hpp"
#include "uresptr.hpp"
#include <algorithm>
#include <pybind11/stl.h>
#include <unicode/resbund.h>

using namespace icu;

void init_resbund(py::module &m) {
  //
  // icu::ResourceBundle
  //
  py::class_<ResourceBundle, UObject> res(m, "ResourceBundle");

  res.def(
         // [1] ResourceBundle::ResourceBundle
         py::init([](const UnicodeString &package_name, const icupy::LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = std::make_unique<ResourceBundle>(package_name, icupy::to_locale(locale), error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         }),
         py::arg("package_name"), py::arg("locale"))
      .def(
          // [2] ResourceBundle::ResourceBundle
          py::init([](const icupy::UnicodeStringVariant &package_name) {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(icupy::to_unistr(package_name), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("package_name"))
      .def(
          // [3] ResourceBundle::ResourceBundle
          py::init([]() {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }))
      .def(
          // [4] ResourceBundle::ResourceBundle
          py::init([](const char *package_name, const icupy::LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(package_name, icupy::to_locale(locale), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("package_name"), py::arg("locale"))
      .def(
          // [5] ResourceBundle::ResourceBundle
          py::init<const ResourceBundle &>(), py::arg("other"))
      .def(
          // [6] ResourceBundle::ResourceBundle
          py::init([](_UResourceBundlePtr &res) {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(res, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("res"));

  res.def("__copy__", &ResourceBundle::clone);

  res.def(
      "__deepcopy__", [](const ResourceBundle &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  res.def("__iter__", [](ResourceBundle &self) -> ResourceBundle & {
    self.resetIterator();
    return self;
  });

  res.def("__len__", &ResourceBundle::getSize);

  res.def("__next__", [](ResourceBundle &self) {
    if (!self.hasNext()) {
      throw py::stop_iteration();
    }
    ErrorCode error_code;
    auto result = self.getNext(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  res.def("clone", &ResourceBundle::clone);

  res.def(
         "get",
         [](const ResourceBundle &self, const char *key) {
           ErrorCode error_code;
           auto result = self.get(key, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("key"))
      .def(
          "get",
          [](const ResourceBundle &self, int32_t index) {
            ErrorCode error_code;
            auto result = self.get(index, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("index"));

  res.def("get_binary", [](const ResourceBundle &self) {
    ErrorCode error_code;
    int32_t length = 0;
    auto p = self.getBinary(length, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return py::bytes(reinterpret_cast<const char *>(p), length);
  });

  res.def("get_int", [](const ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getInt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  res.def("get_int_vector", [](const ResourceBundle &self) {
    ErrorCode error_code;
    int32_t length;
    auto p = self.getIntVector(length, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    std::vector<int32_t> result(p, p + length);
    return result;
  });

  res.def("get_key", &ResourceBundle::getKey, py::return_value_policy::reference);

  res.def(
      "get_locale",
      [](const ResourceBundle &self, ULocDataLocaleType type) {
        ErrorCode error_code;
        auto result = self.getLocale(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));

  res.def("get_name", &ResourceBundle::getName, py::return_value_policy::reference);

  res.def("get_next", [](ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getNext(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  res.def("get_next_string", [](ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getNextString(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  res.def("get_size", &ResourceBundle::getSize);

  res.def("get_string", [](const ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getString(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  res.def(
         "get_string_ex",
         [](const ResourceBundle &self, const char *key) {
           ErrorCode error_code;
           auto result = self.getStringEx(key, error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("key"))
      .def(
          "get_string_ex",
          [](const ResourceBundle &self, int32_t index) {
            ErrorCode error_code;
            auto result = self.getStringEx(index, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("index"));

  res.def("get_type", &ResourceBundle::getType);

  res.def("get_uint", [](const ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getUInt(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  res.def("get_version", [](const ResourceBundle &self) {
    UVersionInfo info;
    self.getVersion(info);
    py::tuple result(U_MAX_VERSION_LENGTH);
    int n = 0;
    std::for_each(std::begin(info), std::end(info), [&](auto v) { result[n++] = v; });
    return result;
  });

  res.def("has_next", [](const ResourceBundle &self) -> py::bool_ { return self.hasNext(); });

  res.def("reset_iterator", &ResourceBundle::resetIterator);
}

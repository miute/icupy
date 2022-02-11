#include "main.hpp"
#include "uresptr.hpp"
#include <pybind11/stl.h>
#include <unicode/resbund.h>

using namespace icu;

void init_resbund(py::module &m) {
  py::class_<ResourceBundle, UObject> res(m, "ResourceBundle");
  res.def(
         // [1] ResourceBundle::ResourceBundle
         py::init([](const UnicodeString &package_name, const _LocaleVariant &locale) {
           ErrorCode error_code;
           auto result = std::make_unique<ResourceBundle>(package_name, VARIANT_TO_LOCALE(locale), error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
           }
           return result;
         }),
         py::arg("package_name"), py::arg("locale"))
      .def(
          // [2] ResourceBundle::ResourceBundle
          py::init([](const _UnicodeStringVariant &package_name) {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(VARIANT_TO_UNISTR(package_name), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
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
              throw ICUError(error_code);
            }
            return result;
          }))
      .def(
          // [4] ResourceBundle::ResourceBundle
          py::init([](const char *package_name, const _LocaleVariant &locale) {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(package_name, VARIANT_TO_LOCALE(locale), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("package_name"), py::arg("locale"))
      .def(
          // [5] ResourceBundle::ResourceBundle
          py::init<const ResourceBundle &>(), py::arg("original"))
      .def(
          // [6] ResourceBundle::ResourceBundle
          py::init([](_UResourceBundlePtr &res) {
            ErrorCode error_code;
            auto result = std::make_unique<ResourceBundle>(res, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("res"));
  res.def("__copy__", &ResourceBundle::clone)
      .def(
          "__deepcopy__", [](const ResourceBundle &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def("__iter__",
           [](ResourceBundle &self) -> ResourceBundle & {
             self.resetIterator();
             return self;
           })
      .def("__len__", &ResourceBundle::getSize)
      .def("__next__", [](ResourceBundle &self) {
        if (!self.hasNext()) {
          throw py::stop_iteration();
        }
        ErrorCode error_code;
        auto result = self.getNext(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
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
             throw ICUError(error_code);
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
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("index"));
  res.def("get_binary", [](const ResourceBundle &self) {
    ErrorCode error_code;
    int32_t length;
    auto p = self.getBinary(length, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    std::vector<uint8_t> result(p, p + length);
    return result;
  });
  res.def("get_int", [](const ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getInt(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  res.def("get_int_vector", [](const ResourceBundle &self) {
    ErrorCode error_code;
    int32_t length;
    auto p = self.getIntVector(length, error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
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
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("type_"));
  res.def("get_name", &ResourceBundle::getName, py::return_value_policy::reference);
  res.def("get_next", [](ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getNext(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  res.def("get_next_string", [](ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getNextString(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  res.def("get_size", &ResourceBundle::getSize);
  res.def("get_string", [](const ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getString(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  res.def(
         "get_string_ex",
         [](const ResourceBundle &self, const char *key) {
           ErrorCode error_code;
           auto result = self.getStringEx(key, error_code);
           if (error_code.isFailure()) {
             throw ICUError(error_code);
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
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("index"));
  res.def("get_type", &ResourceBundle::getType);
  res.def("get_uint", [](const ResourceBundle &self) {
    ErrorCode error_code;
    auto result = self.getUInt(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  res.def("get_version", [](const ResourceBundle &self) {
    UVersionInfo info;
    self.getVersion(info);
    std::vector<uint8_t> result(info, info + sizeof(info));
    return result;
  });
  res.def("has_next", &ResourceBundle::hasNext);
  res.def("reset_iterator", &ResourceBundle::resetIterator);
}

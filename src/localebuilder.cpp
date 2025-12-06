#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <pybind11/stl.h>
#include <unicode/localebuilder.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_localebuilder(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // icu::LocaleBuilder
  //
  py::class_<LocaleBuilder, UObject> lb(m, "LocaleBuilder");

  lb.def(py::init<>());

  lb.def(
      "add_unicode_locale_attribute",
      [](LocaleBuilder &self, const std::string &attribute) -> LocaleBuilder & {
        return self.addUnicodeLocaleAttribute(attribute);
      },
      py::arg("attribute"));

  lb.def("build", [](LocaleBuilder &self) {
    ErrorCode error_code;
    auto result = self.build(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  lb.def("clear", &LocaleBuilder::clear);

  lb.def("clear_extensions", &LocaleBuilder::clearExtensions);

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  lb.def(
      "copy_error_to",
      [](const LocaleBuilder &self, ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

  lb.def(
      "remove_unicode_locale_attribute",
      [](LocaleBuilder &self, const std::string &attribute) -> LocaleBuilder & {
        return self.removeUnicodeLocaleAttribute(attribute);
      },
      py::arg("attribute"));

  lb.def(
      "set_extension",
      [](LocaleBuilder &self, char key, const std::string &value) -> LocaleBuilder & {
        return self.setExtension(key, value);
      },
      py::arg("key"), py::arg("value"));

  lb.def(
      "set_language",
      [](LocaleBuilder &self, const std::string &language) -> LocaleBuilder & { return self.setLanguage(language); },
      py::arg("language"));

  lb.def(
      "set_language_tag",
      [](LocaleBuilder &self, const std::string &tag) -> LocaleBuilder & { return self.setLanguageTag(tag); },
      py::arg("tag"));

  lb.def(
      "set_locale",
      [](LocaleBuilder &self, const icupy::LocaleVariant &locale) -> LocaleBuilder & {
        return self.setLocale(icupy::to_locale(locale));
      },
      py::arg("locale"));

  lb.def(
      "set_region",
      [](LocaleBuilder &self, const std::string &region) -> LocaleBuilder & { return self.setRegion(region); },
      py::arg("region"));

  lb.def(
      "set_script",
      [](LocaleBuilder &self, const std::string &script) -> LocaleBuilder & { return self.setScript(script); },
      py::arg("script"));

  lb.def(
      "set_unicode_locale_keyword",
      [](LocaleBuilder &self, const std::string &key, const std::optional<std::string> &type) -> LocaleBuilder & {
        return self.setUnicodeLocaleKeyword(key, type ? type->data() : nullptr);
      },
      py::arg("key"), py::arg("type_"));

  lb.def(
      "set_variant",
      [](LocaleBuilder &self, const std::string &variant) -> LocaleBuilder & { return self.setVariant(variant); },
      py::arg("variant"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

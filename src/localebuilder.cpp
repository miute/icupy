#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <unicode/errorcode.h>
#include <unicode/localebuilder.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_localebuilder(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  // icu::LocaleBuilder
  py::class_<LocaleBuilder, UObject> lb(m, "LocaleBuilder");
  lb.def(py::init<>());
  lb.def(
      "add_unicode_locale_attribute",
      [](LocaleBuilder &self, const char *attribute) -> LocaleBuilder & {
        return self.addUnicodeLocaleAttribute(attribute);
      },
      py::arg("attribute"));
  lb.def("build", [](LocaleBuilder &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.build(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  lb.def("clear", &LocaleBuilder::clear);
  lb.def("clear_extensions", &LocaleBuilder::clearExtensions);
#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  lb.def(
      "copy_error_to",
      [](const LocaleBuilder &self, ErrorCode &out_error_code) { return self.copyErrorTo(out_error_code); },
      py::arg("out_error_code"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)
  lb.def(
      "remove_unicode_locale_attribute",
      [](LocaleBuilder &self, const char *attribute) -> LocaleBuilder & {
        return self.removeUnicodeLocaleAttribute(attribute);
      },
      py::arg("attribute"));
  lb.def(
      "set_extension",
      [](LocaleBuilder &self, char key, const char *value) -> LocaleBuilder & { return self.setExtension(key, value); },
      py::arg("key"), py::arg("value"));
  lb.def(
      "set_language",
      [](LocaleBuilder &self, const char *language) -> LocaleBuilder & { return self.setLanguage(language); },
      py::arg("language"));
  lb.def(
      "set_language_tag",
      [](LocaleBuilder &self, const char *tag) -> LocaleBuilder & { return self.setLanguageTag(tag); }, py::arg("tag"));
  lb.def(
        "set_locale",
        [](LocaleBuilder &self, const Locale &locale) -> LocaleBuilder & { return self.setLocale(locale); },
        py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "set_locale",
          [](LocaleBuilder &self, const char *locale) -> LocaleBuilder & { return self.setLocale(locale); },
          py::arg("locale"));
  lb.def(
      "set_region", [](LocaleBuilder &self, const char *region) -> LocaleBuilder & { return self.setRegion(region); },
      py::arg("region"));
  lb.def(
      "set_script", [](LocaleBuilder &self, const char *script) -> LocaleBuilder & { return self.setScript(script); },
      py::arg("script"));
  lb.def(
      "set_unicode_locale_keyword",
      [](LocaleBuilder &self, const char *key, const char *type) -> LocaleBuilder & {
        return self.setUnicodeLocaleKeyword(key, type);
      },
      py::arg("key"), py::arg("type_"));
  lb.def(
      "set_variant",
      [](LocaleBuilder &self, const char *variant) -> LocaleBuilder & { return self.setVariant(variant); },
      py::arg("variant"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

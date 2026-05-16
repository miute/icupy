#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 64)
#include <pybind11/stl.h>
#include <unicode/localebuilder.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)

using namespace icu;

void init_localebuilder(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 64)
  //
  // class icu::LocaleBuilder
  //
  py::class_<LocaleBuilder, UObject> lb(m, "LocaleBuilder", R"doc(
      Build a :class:`Locale` instance from the values configured by the
      setters.

      Unlike the :class:`Locale` constructor, ``LocaleBuilder`` checks if the
      values configured via setters satisfy the syntax requirements defined in
      the :class:`Locale` class. :class:`Locale` objects created by
      ``LocaleBuilder`` are well-formed and can be transformed into well-formed
      IETF BCP 47 language tags without losing any information.

      .. seealso::

         :class:`Locale`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> builder = (
         ...     icu.LocaleBuilder().set_language("ja")
         ...     .set_script("Jpan")
         ...     .set_region("JP")
         ...     .set_unicode_locale_keyword("ca", "japanese")
         ...     .set_unicode_locale_keyword("cu", "jpy")
         ... )
         >>> locale = builder.build()  # Locale('ja_Jpan_JP@calendar=japanese;currency=jpy')
         >>> locale.to_language_tag()
         'ja-Jpan-JP-u-ca-japanese-cu-jpy'
      )doc");

  lb.def(py::init<>(), R"doc(
      Initialize a ``LocaleBuilder`` instance as an empty builder.
      )doc");

  lb.def(
      "add_unicode_locale_attribute",
      [](LocaleBuilder &self, const std::string &attribute) -> LocaleBuilder & {
        return self.addUnicodeLocaleAttribute(attribute);
      },
      py::arg("attribute"), R"doc(
      Add a Unicode locale attribute to the builder and return the builder
      itself. if *attribute* is already present in the builder, this method
      does nothing.

      *attribute* must be a non-empty string and must be well-formed.
      )doc");

  lb.def(
      "build",
      [](LocaleBuilder &self) {
        ErrorCode error_code;
        auto result = self.build(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Create a new :class:`Locale` instance from the values configured by the
      setters.
      )doc");

  lb.def("clear", &LocaleBuilder::clear, R"doc(
      Clear the builder and return the builder itself.

      The internal :class:`UErrorCode` is also reset to
      :attr:`~UErrorCode.U_ZERO_ERROR`.
      )doc");

  lb.def("clear_extensions", &LocaleBuilder::clearExtensions, R"doc(
      Clear the extensions from the builder and return the builder itself.

      The language, script, region, and variant fields are not affected by
      this method.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 65)
  lb.def(
      "copy_error_to",
      [](const LocaleBuilder &self, ErrorCode &out_error_code) -> py::bool_ {
        return self.copyErrorTo(out_error_code);
      },
      py::arg("out_error_code"), R"doc(
      Copy the internal :class:`UErrorCode` to *out_error_code* and return
      ``True`` if :class:`UErrorCode` indicates a failure, or ``False``
      otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 65)

  lb.def(
      "remove_unicode_locale_attribute",
      [](LocaleBuilder &self, const std::string &attribute) -> LocaleBuilder & {
        return self.removeUnicodeLocaleAttribute(attribute);
      },
      py::arg("attribute"), R"doc(
      Remove a Unicode locale attribute from the builder and return the builder
      itself. If *attribute* is not present in the builder, this method does
      nothing.

      *attribute* must be a non-empty string and must be well-formed.
      )doc");

  lb.def(
      "set_extension",
      [](LocaleBuilder &self, char key, const std::string &value)
          -> LocaleBuilder & { return self.setExtension(key, value); },
      py::arg("key"), py::arg("value"), R"doc(
      Set the extension with the specified *key* to *value* and return the
      builder itself.

      If *value* is the empty string, the extension with the *key* is removed
      from the builder. Otherwise, *key* and *value* must be well-formed.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> builder = (
         ...     icu.LocaleBuilder().set_language("ja")
         ...     .set_extension("u", "ca-japanese-cu-jpy")
         ... )
         >>> builder.build()
         Locale('ja@calendar=japanese;currency=jpy')
      )doc");

  lb.def(
      "set_language",
      [](LocaleBuilder &self, const std::string &language) -> LocaleBuilder & {
        return self.setLanguage(language);
      },
      py::arg("language"), R"doc(
      Set the language of the builder and return the builder itself.

      If *language* is the empty string, the language in the builder is
      removed. Otherwise, *language* must be well-formed.
      )doc");

  lb.def(
      "set_language_tag",
      [](LocaleBuilder &self, const std::string &tag) -> LocaleBuilder & {
        return self.setLanguageTag(tag);
      },
      py::arg("tag"), R"doc(
      Reset the builder to match the specified IETF BCP 47 language tag and
      return the builder itself.

      If *tag* is the empty string, the builder is cleared, like :meth:`clear`.
      Legacy language tags (marked as "Type: grandfathered" in BCP 47) are
      converted to their canonical form before being processed.
      Otherwise, *tag* must be well-formed.

      The internal :class:`UErrorCode` is also reset to
      :attr:`~UErrorCode.U_ZERO_ERROR`.
      )doc");

  lb.def(
      "set_locale",
      [](LocaleBuilder &self,
         const icupy::LocaleVariant &locale) -> LocaleBuilder & {
        return self.setLocale(icupy::to_locale(locale));
      },
      py::arg("locale"), R"doc(
      Reset the builder to match the specified locale and return the builder
      itself.

      All fields of *locale* must be well-formed.

      The internal :class:`UErrorCode` is also reset to
      :attr:`~UErrorCode.U_ZERO_ERROR`.
      )doc");

  lb.def(
      "set_region",
      [](LocaleBuilder &self, const std::string &region) -> LocaleBuilder & {
        return self.setRegion(region);
      },
      py::arg("region"), R"doc(
      Set the region of the builder and return the builder itself.

      If *region* is the empty string, the region in the builder is removed.
      Otherwise, *region* must be well-formed.
      )doc");

  lb.def(
      "set_script",
      [](LocaleBuilder &self, const std::string &script) -> LocaleBuilder & {
        return self.setScript(script);
      },
      py::arg("script"), R"doc(
      Set the script of the builder and return the builder itself.

      If *script* is the empty string, the script in the builder is removed.
      Otherwise, *script* must be well-formed.
      )doc");

  lb.def(
      "set_unicode_locale_keyword",
      [](LocaleBuilder &self, const std::string &key,
         const std::optional<std::string> &type) -> LocaleBuilder & {
        return self.setUnicodeLocaleKeyword(key, type ? type->data() : nullptr);
      },
      py::arg("key"), py::arg("type"), R"doc(
      Set the Unicode locale keyword with the specified *key* to *type* and
      return the builder itself.

      If *type* is ``None`` or the empty string, the Unicode locale keyword
      with the *key* is removed from the builder. Otherwise, *key* and *type*
      must be well-formed.

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> builder = (
         ...     icu.LocaleBuilder().set_language("ja")
         ...     .set_unicode_locale_keyword("ca", "japanese")
         ...     .set_unicode_locale_keyword("cu", "jpy")
         ... )
         >>> builder.build()
         Locale('ja@calendar=japanese;currency=jpy')
      )doc");

  lb.def(
      "set_variant",
      [](LocaleBuilder &self, const std::string &variant) -> LocaleBuilder & {
        return self.setVariant(variant);
      },
      py::arg("variant"), R"doc(
      Set the variant of the builder and return the builder itself.

      If *variant* is the empty string, the variant in the builder is removed.
      Otherwise, *variant* must be well-formed.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 64)
}

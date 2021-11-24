#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/locdspnm.h>

using namespace icu;

void init_locdspnm(py::module &m) {
  // icu::LocaleDisplayNames
  py::class_<LocaleDisplayNames, UObject> ldn(m, "LocaleDisplayNames");
  ldn.def_static(
         // [1] LocaleDisplayNames::createInstance
         "create_instance", py::overload_cast<const Locale &>(&LocaleDisplayNames::createInstance), py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance", [](const char *locale) { return LocaleDisplayNames::createInstance(locale); },
          py::arg("locale"))
      .def_static(
          // [2] LocaleDisplayNames::createInstance
          "create_instance", py::overload_cast<const Locale &, UDialectHandling>(&LocaleDisplayNames::createInstance),
          py::arg("locale"), py::arg("dialect_handling"))
      .def_static(
          "create_instance",
          [](const char *locale, UDialectHandling dialect_handling) {
            return LocaleDisplayNames::createInstance(locale, dialect_handling);
          },
          py::arg("locale"), py::arg("dialect_handling"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  ldn.def_static(
         // [3] LocaleDisplayNames::createInstance
         "create_instance",
         [](const Locale &locale, std::vector<UDisplayContext> &contexts, int32_t length) {
           if (length == -1) {
             length = static_cast<int32_t>(contexts.size());
           }
           return LocaleDisplayNames::createInstance(locale, contexts.data(), length);
         },
         py::arg("locale"), py::arg("contexts"), py::arg("length") = -1)
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale, std::vector<UDisplayContext> &contexts, int32_t length) {
            if (length == -1) {
              length = static_cast<int32_t>(contexts.size());
            }
            return LocaleDisplayNames::createInstance(locale, contexts.data(), length);
          },
          py::arg("locale"), py::arg("contexts"), py::arg("length") = -1);
  ldn.def("get_context", &LocaleDisplayNames::getContext, py::arg("type_"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  ldn.def("get_dialect_handling", &LocaleDisplayNames::getDialectHandling);
  ldn.def("get_locale", &LocaleDisplayNames::getLocale);
  ldn.def("key_display_name", &LocaleDisplayNames::keyDisplayName, py::arg("key"), py::arg("result"));
  ldn.def("key_value_display_name", &LocaleDisplayNames::keyValueDisplayName, py::arg("key"), py::arg("value"),
          py::arg("result"));
  ldn.def("language_display_name", &LocaleDisplayNames::languageDisplayName, py::arg("lang"), py::arg("result"));
  ldn.def("locale_display_name",
          py::overload_cast<const char *, UnicodeString &>(&LocaleDisplayNames::localeDisplayName, py::const_),
          py::arg("locale_id"), py::arg("result"))
      .def("locale_display_name",
           py::overload_cast<const Locale &, UnicodeString &>(&LocaleDisplayNames::localeDisplayName, py::const_),
           py::arg("locale"), py::arg("result"));
  ldn.def("region_display_name", &LocaleDisplayNames::regionDisplayName, py::arg("region"), py::arg("result"));
  ldn.def("script_display_name",
          py::overload_cast<const char *, UnicodeString &>(&LocaleDisplayNames::scriptDisplayName, py::const_),
          py::arg("script"), py::arg("result"))
      .def("script_display_name",
           py::overload_cast<UScriptCode, UnicodeString &>(&LocaleDisplayNames::scriptDisplayName, py::const_),
           py::arg("script_code"), py::arg("result"));
  ldn.def("variant_display_name", &LocaleDisplayNames::variantDisplayName, py::arg("variant"), py::arg("result"));
}

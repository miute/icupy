#include "main.hpp"
#include <unicode/locid.h>
#include <unicode/translit.h>
#include <unicode/uniset.h>

using namespace icu;

void init_translit(py::module &m) {
  py::class_<Transliterator> tl(m, "Transliterator");

  py::class_<Transliterator::Token>(tl, "Token");

  tl.def("__copy__", &Transliterator::clone).def("__deepcopy__", [](const Transliterator &self, py::dict) {
    return self.clone();
  });
  tl.def(
      "adopt_filter",
      [](Transliterator &self, UnicodeFilter *adopted_filter) {
        self.adoptFilter(reinterpret_cast<UnicodeFilter *>(adopted_filter ? adopted_filter->clone() : nullptr));
      },
      py::arg("adopted_filter"));
  tl.def("clone", &Transliterator::clone);
  tl.def_static("count_available_sources", &Transliterator::countAvailableSources);
  tl.def_static("count_available_targets", &Transliterator::countAvailableTargets, py::arg("source"));
  tl.def_static("count_available_variants", &Transliterator::countAvailableVariants, py::arg("source"),
                py::arg("target"));
  tl.def("count_elements", &Transliterator::countElements);
  tl.def_static(
      "create_from_rules",
      [](const UnicodeString &id, const UnicodeString &rules, UTransDirection dir, UParseError &parse_error) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = Transliterator::createFromRules(id, rules, dir, parse_error, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return result;
      },
      py::arg("id_"), py::arg("rules"), py::arg("dir_"), py::arg("parse_error"));
  tl.def_static(
        "create_instance",
        [](const UnicodeString &id, UTransDirection dir) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = Transliterator::createInstance(id, dir, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUException(error_code);
          }
          return result;
        },
        py::arg("id_"), py::arg("dir_"))
      .def_static(
          "create_instance",
          [](const UnicodeString &id, UTransDirection dir, UParseError &parse_error) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = Transliterator::createInstance(id, dir, parse_error, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUException(error_code);
            }
            return result;
          },
          py::arg("id_"), py::arg("dir_"), py::arg("parse_error"));
  tl.def("create_inverse", [](const Transliterator &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.createInverse(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
    return result;
  });
  /*
  tl.def("filtered_transliterate",
         py::overload_cast<Replaceable &, UTransPosition &, UBool>(&Transliterator::filteredTransliterate, py::const_),
         py::arg("text"), py::arg("index"), py::arg("incremental"));
  tl.def("finish_transliteration", &Transliterator::finishTransliteration, py::arg("text"), py::arg("index"));
  */
  tl.def_static("get_available_ids", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = Transliterator::getAvailableIDs(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUException(error_code);
    }
    return result;
  });
  tl.def_static("get_available_source", &Transliterator::getAvailableSource, py::arg("index"), py::arg("result"));
  tl.def_static("get_available_target", &Transliterator::getAvailableTarget, py::arg("index"), py::arg("source"),
                py::arg("result"));
  tl.def_static("get_available_variant", &Transliterator::getAvailableVariant, py::arg("index"), py::arg("source"),
                py::arg("target"), py::arg("result"));
  tl.def_static(
        "get_display_name",
        py::overload_cast<const UnicodeString &, const Locale &, UnicodeString &>(&Transliterator::getDisplayName),
        py::arg("id_"), py::arg("in_locale"), py::arg("result"))
      .def_static("get_display_name",
                  py::overload_cast<const UnicodeString &, UnicodeString &>(&Transliterator::getDisplayName),
                  py::arg("id_"), py::arg("result"));
  tl.def(
      "get_element",
      [](const Transliterator &self, int32_t index) -> const Transliterator & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.getElement(index, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUException(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("index"));
  tl.def("get_filter", &Transliterator::getFilter, py::return_value_policy::reference);
  tl.def("get_id", &Transliterator::getID);
  tl.def("get_maximum_context_length", &Transliterator::getMaximumContextLength);
  tl.def("get_source_set", &Transliterator::getSourceSet, py::arg("result"));
  tl.def("get_target_set", &Transliterator::getTargetSet, py::arg("result"));
  /*
  tl.def("handle_get_source_set", &Transliterator::handleGetSourceSet, py::arg("result"));
  // FIXME: Implement "void handleTransliterate(Replaceable &text, UTransPosition &pos, UBool incremental)".
  */
  tl.def("orphan_filter", &Transliterator::orphanFilter, py::return_value_policy::reference);
  tl.def_static("register_alias", &Transliterator::registerAlias, py::arg("alias_id"), py::arg("real_id"));
  // FIXME: Implement "static void registerFactory(const UnicodeString &id, Factory factory, Token context)".
  tl.def_static(
      "register_instance", [](Transliterator *adopted_obj) { Transliterator::registerInstance(adopted_obj->clone()); },
      py::arg("adopted_obj").none(false));
  tl.def("to_rules", &Transliterator::toRules, py::arg("result"), py::arg("escape_unprintable"));
  tl.def("transliterate", py::overload_cast<Replaceable &>(&Transliterator::transliterate, py::const_), py::arg("text"))
      .def("transliterate",
           py::overload_cast<Replaceable &, int32_t, int32_t>(&Transliterator::transliterate, py::const_),
           py::arg("text"), py::arg("start"), py::arg("limit"))
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text, UTransPosition &index, const UnicodeString &insertion) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.transliterate(text, index, insertion, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUException(error_code);
            }
          },
          py::arg("text"), py::arg("index"), py::arg("insertion"))
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text, UTransPosition &index, UChar32 insertion) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.transliterate(text, index, insertion, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUException(error_code);
            }
          },
          py::arg("text"), py::arg("index"), py::arg("insertion"))
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text, UTransPosition &index) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.transliterate(text, index, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUException(error_code);
            }
          },
          py::arg("text"), py::arg("index"));
  tl.def_static("unregister", &Transliterator::unregister, py::arg("id_"));
}

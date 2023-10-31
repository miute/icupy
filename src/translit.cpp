#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/locid.h>
#include <unicode/translit.h>
#include <unicode/uniset.h>

using namespace icu;

class PyTransliterator : public Transliterator {
public:
  PyTransliterator(const UnicodeString &id, UnicodeFilter *adopted_filter) : Transliterator(id, adopted_filter) {}
  PyTransliterator(const Transliterator &other) : Transliterator(other) {}

  virtual Transliterator *clone() const override { return new PyTransliterator(*this); }

  virtual UnicodeSet &getTargetSet(UnicodeSet &result) const override {
    PYBIND11_OVERRIDE_NAME(UnicodeSet &, Transliterator, "get_target_set", getTargetSet, result);
  }

  virtual void handleGetSourceSet(UnicodeSet &result) const override {
    PYBIND11_OVERRIDE_NAME(void, Transliterator, "handle_get_source_set", handleGetSourceSet, result);
  }

  virtual void handleTransliterate(Replaceable &text, UTransPosition &pos, UBool incremental) const override {
    PYBIND11_OVERRIDE_PURE_NAME(void, Transliterator, "_handle_transliterate", handleTransliterate, text, pos,
                                py::bool_(incremental));
  }

  virtual UClassID getDynamicClassID() const override;
  static UClassID U_EXPORT2 getStaticClassID();

  using Transliterator::createBasicInstance;
  using Transliterator::setID;
  using Transliterator::setMaximumContextLength;
};

UOBJECT_DEFINE_RTTI_IMPLEMENTATION(PyTransliterator);

void init_translit(py::module &m) {
  //
  // icu::Transliterator
  //
  py::class_<Transliterator, UObject, PyTransliterator> tl(m, "Transliterator");

  //
  // icu::Transliterator::Token
  //
  py::class_<Transliterator::Token>(tl, "Token");

  //
  // icu::Transliterator
  //
  tl.def(py::init([](const icupy::UnicodeStringVariant &id, UnicodeFilter *adopted_filter) {
           return std::make_unique<PyTransliterator>(icupy::to_unistr(id),
                                                     adopted_filter ? adopted_filter->clone() : nullptr);
         }),
         py::arg("id_"), py::arg("adopted_filter"))
      .def(py::init([](const PyTransliterator &other) { return std::make_unique<PyTransliterator>(other); }),
           py::arg("other"));

  tl.def("__copy__", &Transliterator::clone);

  tl.def(
      "__deepcopy__", [](const Transliterator &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  tl.def(
      "adopt_filter",
      [](Transliterator &self, UnicodeFilter *adopted_filter) {
        self.adoptFilter(reinterpret_cast<UnicodeFilter *>(adopted_filter ? adopted_filter->clone() : nullptr));
      },
      py::arg("adopted_filter"));

  tl.def("clone", &Transliterator::clone);

  tl.def_static("count_available_sources", &Transliterator::countAvailableSources);

  tl.def_static(
      "count_available_targets",
      [](const icupy::UnicodeStringVariant &source) {
        return Transliterator::countAvailableTargets(icupy::to_unistr(source));
      },
      py::arg("source"));

  tl.def_static(
      "count_available_variants",
      [](const icupy::UnicodeStringVariant &source, const icupy::UnicodeStringVariant &target) {
        return Transliterator::countAvailableVariants(icupy::to_unistr(source), icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"));

  tl.def("count_elements", &Transliterator::countElements);

  tl.def_static(
      "_create_basic_instance",
      [](const icupy::UnicodeStringVariant &id, const UnicodeString *canon) {
        return PyTransliterator::createBasicInstance(icupy::to_unistr(id), canon);
      },
      py::arg("id_"), py::arg("canon"));

  tl.def_static(
      "create_from_rules",
      [](const icupy::UnicodeStringVariant &id, const icupy::UnicodeStringVariant &rules, UTransDirection dir,
         UParseError &parse_error) {
        ErrorCode error_code;
        auto result = Transliterator::createFromRules(icupy::to_unistr(id), icupy::to_unistr(rules), dir, parse_error,
                                                      error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("id_"), py::arg("rules"), py::arg("dir_"), py::arg("parse_error"));

  tl.def_static(
        "create_instance",
        [](const icupy::UnicodeStringVariant &id, UTransDirection dir) {
          ErrorCode error_code;
          auto result = Transliterator::createInstance(icupy::to_unistr(id), dir, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("id_"), py::arg("dir_"))
      .def_static(
          "create_instance",
          [](const icupy::UnicodeStringVariant &id, UTransDirection dir, UParseError &parse_error) {
            ErrorCode error_code;
            auto result = Transliterator::createInstance(icupy::to_unistr(id), dir, parse_error, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("id_"), py::arg("dir_"), py::arg("parse_error"));

  tl.def("create_inverse", [](const Transliterator &self) {
    ErrorCode error_code;
    auto result = self.createInverse(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  tl.def(
      "filtered_transliterate",
      [](const Transliterator &self, Replaceable &text, UTransPosition &index, py::bool_ incremental) {
        self.filteredTransliterate(text, index, incremental);
      },
      py::arg("text"), py::arg("index"), py::arg("incremental"));

  tl.def("finish_transliteration", &Transliterator::finishTransliteration, py::arg("text"), py::arg("index"));

  tl.def_static("get_available_ids", []() {
    ErrorCode error_code;
    auto result = Transliterator::getAvailableIDs(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return result;
  });

  tl.def_static("get_available_source", &Transliterator::getAvailableSource, py::arg("index"), py::arg("result"));

  tl.def_static(
      "get_available_target",
      [](int32_t index, const icupy::UnicodeStringVariant &source, UnicodeString &result) -> UnicodeString & {
        return Transliterator::getAvailableTarget(index, icupy::to_unistr(source), result);
      },
      py::arg("index"), py::arg("source"), py::arg("result"));

  tl.def_static(
      "get_available_variant",
      [](int32_t index, const icupy::UnicodeStringVariant &source, const icupy::UnicodeStringVariant &target,
         UnicodeString &result) -> UnicodeString & {
        return Transliterator::getAvailableVariant(index, icupy::to_unistr(source), icupy::to_unistr(target), result);
      },
      py::arg("index"), py::arg("source"), py::arg("target"), py::arg("result"));

  tl.def_static(
        "get_display_name",
        [](const icupy::UnicodeStringVariant &id, const icupy::LocaleVariant &in_locale,
           UnicodeString &result) -> UnicodeString & {
          return Transliterator::getDisplayName(icupy::to_unistr(id), icupy::to_locale(in_locale), result);
        },
        py::arg("id_"), py::arg("in_locale"), py::arg("result"))
      .def_static(
          "get_display_name",
          [](const icupy::UnicodeStringVariant &id, UnicodeString &result) -> UnicodeString & {
            return Transliterator::getDisplayName(icupy::to_unistr(id), result);
          },
          py::arg("id_"), py::arg("result"));

  tl.def(
      "get_element",
      [](const Transliterator &self, int32_t index) -> const Transliterator & {
        ErrorCode error_code;
        auto &result = self.getElement(index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("index"));

  tl.def("get_filter", &Transliterator::getFilter, py::return_value_policy::reference);

  tl.def("get_id", &Transliterator::getID);

  tl.def("get_maximum_context_length", &Transliterator::getMaximumContextLength);

  tl.def("get_source_set", &Transliterator::getSourceSet, py::arg("result"));

  tl.def("get_target_set", &Transliterator::getTargetSet, py::arg("result"));

  tl.def("handle_get_source_set", &Transliterator::handleGetSourceSet, py::arg("result"));

  tl.def("orphan_filter", &Transliterator::orphanFilter, py::return_value_policy::reference);

  tl.def_static(
      "register_alias",
      [](const icupy::UnicodeStringVariant &alias_id, const icupy::UnicodeStringVariant &real_id) {
        Transliterator::registerAlias(icupy::to_unistr(alias_id), icupy::to_unistr(real_id));
      },
      py::arg("alias_id"), py::arg("real_id"));

  // FIXME: Implement "static void registerFactory(const UnicodeString &id, Factory factory, Token context)".

  tl.def_static(
      "register_instance",
      [](Transliterator *adopted_obj) {
        Transliterator::registerInstance(adopted_obj ? adopted_obj->clone() : nullptr);
      },
      py::arg("adopted_obj").none(false));

  tl.def(
      "_set_id",
      [](PyTransliterator &self, const icupy::UnicodeStringVariant &id) { self.setID(icupy::to_unistr(id)); },
      py::arg("id_"));

  tl.def("_set_maximum_context_length", &PyTransliterator::setMaximumContextLength, py::arg("max_context_length"));

  tl.def(
      "to_rules",
      [](const Transliterator &self, UnicodeString &result, py::bool_ escape_unprintable) -> UnicodeString & {
        return self.toRules(result, escape_unprintable);
      },
      py::arg("result"), py::arg("escape_unprintable"));

  tl.def("transliterate", py::overload_cast<Replaceable &>(&Transliterator::transliterate, py::const_), py::arg("text"))
      .def("transliterate",
           py::overload_cast<Replaceable &, int32_t, int32_t>(&Transliterator::transliterate, py::const_),
           py::arg("text"), py::arg("start"), py::arg("limit"))
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text, UTransPosition &index,
             const icupy::UnicodeStringVariant &insertion) {
            ErrorCode error_code;
            self.transliterate(text, index, icupy::to_unistr(insertion), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("index"), py::arg("insertion"))
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text, UTransPosition &index, UChar32 insertion) {
            ErrorCode error_code;
            self.transliterate(text, index, insertion, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("index"), py::arg("insertion"))
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text, UTransPosition &index) {
            ErrorCode error_code;
            self.transliterate(text, index, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("index"));

  tl.def_static(
      "unregister", [](const icupy::UnicodeStringVariant &id) { Transliterator::unregister(icupy::to_unistr(id)); },
      py::arg("id_"));
}

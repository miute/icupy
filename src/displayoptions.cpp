#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 72)
#include <unicode/displayoptions.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)

using namespace icu;

void init_displayoptions(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 72)
  //
  // icu::DisplayOptions
  //
  py::class_<DisplayOptions> ops(m, "DisplayOptions");

  //
  // icu::DisplayOptions::Builder
  //
  py::class_<DisplayOptions::Builder> bld(ops, "Builder");

  bld.def("build", &DisplayOptions::Builder::build);

  bld.def("set_capitalization", &DisplayOptions::Builder::setCapitalization, py::arg("capitalization"));

  bld.def("set_display_length", &DisplayOptions::Builder::setDisplayLength, py::arg("display_length"));

  bld.def("set_grammatical_case", &DisplayOptions::Builder::setGrammaticalCase, py::arg("grammatical_case"));

  bld.def("set_name_style", &DisplayOptions::Builder::setNameStyle, py::arg("name_style"));

  bld.def("set_noun_class", &DisplayOptions::Builder::setNounClass, py::arg("noun_class"));

  bld.def("set_plural_category", &DisplayOptions::Builder::setPluralCategory, py::arg("plural_category"));

  bld.def("set_substitute_handling", &DisplayOptions::Builder::setSubstituteHandling, py::arg("substitute_handling"));

  //
  // icu::DisplayOptions
  //
  ops.def(py::init<const DisplayOptions &>(), py::arg("other"));

  ops.def_static("builder", &DisplayOptions::builder);

  ops.def("copy_to_builder", &DisplayOptions::copyToBuilder);

  ops.def("get_capitalization", &DisplayOptions::getCapitalization);

  ops.def("get_display_length", &DisplayOptions::getDisplayLength);

  ops.def("get_grammatical_case", &DisplayOptions::getGrammaticalCase);

  ops.def("get_name_style", &DisplayOptions::getNameStyle);

  ops.def("get_noun_class", &DisplayOptions::getNounClass);

  ops.def("get_plural_category", &DisplayOptions::getPluralCategory);

  ops.def("get_substitute_handling", &DisplayOptions::getSubstituteHandling);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 72)
}

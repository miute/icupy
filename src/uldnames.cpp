#include "main.hpp"
#include <unicode/uldnames.h>

void init_uldnames(py::module &m) {
  py::enum_<UDialectHandling>(m, "UDialectHandling", py::arithmetic(),
                              "Enum used in *LocaleDisplayNames::create_instance*.")
      .value("ULDN_STANDARD_NAMES", ULDN_STANDARD_NAMES,
             "Use standard names when generating a locale name, e.g., en_GB displays as 'English (United Kingdom)'.")
      .value("ULDN_DIALECT_NAMES", ULDN_DIALECT_NAMES,
             "Use dialect names, when generating a locale name, e.g., en_GB displays as 'British English'.")
      .export_values();
}

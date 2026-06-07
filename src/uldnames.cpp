#include "main.hpp"
#include <pybind11/native_enum.h>
#include <unicode/uldnames.h>

void init_uldnames(py::module &m) {
  //
  // enum UDialectHandling
  //
  py::native_enum<UDialectHandling>(m, "UDialectHandling", "enum.IntEnum",
                                    R"doc(
Enum used in :meth:`LocaleDisplayNames.create_instance`.
      )doc")
      .value("ULDN_STANDARD_NAMES", ULDN_STANDARD_NAMES, R"doc(
             Use standard names when generating a locale name, e.g., en_GB
             displays as "English (United Kingdom)".
             )doc")
      .value("ULDN_DIALECT_NAMES", ULDN_DIALECT_NAMES, R"doc(
             Use dialect names, when generating a locale name, e.g., en_GB
             displays as "British English".
             )doc")
      .export_values()
      .finalize();
}

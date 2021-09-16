#include "main.hpp"
#include <unicode/unorm2.h>

void init_unorm2(py::module &m) {
  py::enum_<UNormalization2Mode>(m, "UNormalization2Mode", py::arithmetic())
      .value("UNORM2_COMPOSE", UNORM2_COMPOSE)
      .value("UNORM2_DECOMPOSE", UNORM2_DECOMPOSE)
      .value("UNORM2_COMPOSE_CONTIGUOUS", UNORM2_COMPOSE_CONTIGUOUS)
      .export_values();

  py::enum_<UNormalizationCheckResult>(m, "UNormalizationCheckResult", py::arithmetic())
      .value("UNORM_NO", UNORM_NO)
      .value("UNORM_YES", UNORM_YES)
      .value("UNORM_MAYBE", UNORM_MAYBE)
      .export_values();
}

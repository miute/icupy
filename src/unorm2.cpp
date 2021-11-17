#include "main.hpp"
#include <unicode/unorm2.h>

void init_unorm2(py::module &m) {
  py::enum_<UNormalization2Mode>(
      m, "UNormalization2Mode", py::arithmetic(),
      "Constants for normalization modes.\n\n"
      "For details about standard Unicode normalization forms and about the algorithms which are also used with custom "
      "mapping tables see http://www.unicode.org/unicode/reports/tr15/")
      .value("UNORM2_COMPOSE", UNORM2_COMPOSE,
             "Decomposition followed by composition.\n\n  "
             "Same as standard NFC when using an \"nfc\" instance. Same as standard NFKC when using an \"nfkc\" "
             "instance. For details about standard Unicode normalization forms see "
             "http://www.unicode.org/unicode/reports/tr15/")
      .value("UNORM2_DECOMPOSE", UNORM2_DECOMPOSE,
             "Map, and reorder canonically.\n\n  "
             "Same as standard NFD when using an \"nfc\" instance. Same as standard NFKD when using an \"nfkc\" "
             "instance. For details about standard Unicode normalization forms see "
             "http://www.unicode.org/unicode/reports/tr15/")
      .value("UNORM2_FCD", UNORM2_FCD,
             "\"Fast C or D\" form.\n\n  "
             "If a string is in this form, then further decomposition without reordering would yield the same form as "
             "DECOMPOSE. Text in \"Fast C or D\" form can be processed efficiently with data tables that are "
             "\"canonically closed\", that is, that provide equivalent data for equivalent text, without having to be "
             "fully normalized. Not a standard Unicode normalization form. Not a unique form: Different FCD strings "
             "can be canonically equivalent. For details see http://www.unicode.org/notes/tn5/#FCD")
      .value("UNORM2_COMPOSE_CONTIGUOUS", UNORM2_COMPOSE_CONTIGUOUS,
             "Compose only contiguously.\n\n  "
             "Also known as \"FCC\" or \"Fast C Contiguous\". The result will often but not always be in NFC. The "
             "result will conform to FCD which is useful for processing. Not a standard Unicode normalization form. "
             "For details see http://www.unicode.org/notes/tn5/#FCC")
      .export_values();

  py::enum_<UNormalizationCheckResult>(
      m, "UNormalizationCheckResult", py::arithmetic(),
      "Result values for normalization quick check functions.\n\n"
      "For details see http://www.unicode.org/reports/tr15/#Detecting_Normalization_Forms")
      .value("UNORM_NO", UNORM_NO, "The input string is not in the normalization form.")
      .value("UNORM_YES", UNORM_YES, "The input string is in the normalization form.")
      .value("UNORM_MAYBE", UNORM_MAYBE,
             "The input string may or may not be in the normalization form.\n\n  "
             "This value is only returned for composition forms like NFC and FCC, when a backward-combining character "
             "is found for which the surrounding text would have to be analyzed further.")
      .export_values();
}

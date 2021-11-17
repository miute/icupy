#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
#include <unicode/udisplaycontext.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

void init_udisplaycontext(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  py::enum_<UDisplayContext>(m, "UDisplayContext", py::arithmetic(),
                             "Display context settings.\n\n"
                             "Note, the specific numeric values are internal and may change.")
      .value("UDISPCTX_STANDARD_NAMES", UDISPCTX_STANDARD_NAMES,
             "A possible setting for DIALECT_HANDLING: use standard names when generating a locale name, e.g. "
             "DIALECT_HANDLING can be set to one of *UDISPCTX_STANDARD_NAMES* or *UDISPCTX_DIALECT_NAMES*.\n\n  "
             "Use *UDisplayContextType* *UDISPCTX_TYPE_DIALECT_HANDLING* to get the value. en_GB displays as 'English "
             "(United Kingdom)'.")
      .value("UDISPCTX_DIALECT_NAMES", UDISPCTX_DIALECT_NAMES,
             "A possible setting for DIALECT_HANDLING: use dialect names, when generating a locale name, e.g. "
             "en_GB displays as 'British English'.")
      .value("UDISPCTX_CAPITALIZATION_NONE", UDISPCTX_CAPITALIZATION_NONE,
             "The capitalization context to be used is unknown (this is the default value).\n\n  "
             "CAPITALIZATION can be set to one of *UDISPCTX_CAPITALIZATION_NONE*, "
             "*UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE*, *UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE*, "
             "*UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU*, or *UDISPCTX_CAPITALIZATION_FOR_STANDALONE*. Use "
             "*UDisplayContextType* *UDISPCTX_TYPE_CAPITALIZATION* to get the value.")
      .value("UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE", UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE,
             "The capitalization context if a date, date symbol or display name is to be formatted with capitalization "
             "appropriate for the middle of a sentence.")
      .value("UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE", UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE,
             "The capitalization context if a date, date symbol or display name is to be formatted with capitalization "
             "appropriate for the beginning of a sentence.")
      .value("UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU", UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU,
             "The capitalization context if a date, date symbol or display name is to be formatted with capitalization "
             "appropriate for a user-interface list or menu item.")
      .value("UDISPCTX_CAPITALIZATION_FOR_STANDALONE", UDISPCTX_CAPITALIZATION_FOR_STANDALONE,
             "The capitalization context if a date, date symbol or display name is to be formatted with capitalization "
             "appropriate for stand-alone usage such as an isolated name on a calendar page.")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UDISPCTX_LENGTH_FULL", UDISPCTX_LENGTH_FULL,
             "A possible setting for DISPLAY_LENGTH: use full names when generating a locale name, e.g. "
             "DISPLAY_LENGTH can be set to one of *UDISPCTX_LENGTH_FULL* or *UDISPCTX_LENGTH_SHORT*.\n\n  "
             "Use *UDisplayContextType* *UDISPCTX_TYPE_DISPLAY_LENGTH* to get the value. \"United States\" for US.")
      .value("UDISPCTX_LENGTH_SHORT", UDISPCTX_LENGTH_SHORT,
             "A possible setting for DISPLAY_LENGTH: use short names when generating a locale name, e.g. "
             "\"U.S.\" for US.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("UDISPCTX_SUBSTITUTE", UDISPCTX_SUBSTITUTE,
             "A possible setting for SUBSTITUTE_HANDLING: Returns a fallback value (e.g., the input code) when no "
             "data is available.\n\n  "
             "SUBSTITUTE_HANDLING can be set to one of *UDISPCTX_SUBSTITUTE* or *UDISPCTX_NO_SUBSTITUTE*. Use "
             "*UDisplayContextType* *UDISPCTX_TYPE_SUBSTITUTE_HANDLING* to get the value. This is the default value.")
      .value("UDISPCTX_NO_SUBSTITUTE", UDISPCTX_NO_SUBSTITUTE,
             "A possible setting for SUBSTITUTE_HANDLING: Returns a null value with error code set to "
             "*U_ILLEGAL_ARGUMENT_ERROR* when no data is available.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
      .export_values();

  py::enum_<UDisplayContextType>(m, "UDisplayContextType", py::arithmetic(),
                                 "Display context types, for getting values of a particular setting.\n\n"
                                 "Note, the specific numeric values are internal and may change.")
      .value("UDISPCTX_TYPE_DIALECT_HANDLING", UDISPCTX_TYPE_DIALECT_HANDLING,
             "Type to retrieve the dialect handling setting, e.g. "
             "*UDISPCTX_STANDARD_NAMES* or *UDISPCTX_DIALECT_NAMES*.")
      .value("UDISPCTX_TYPE_CAPITALIZATION", UDISPCTX_TYPE_CAPITALIZATION,
             "Type to retrieve the capitalization context setting, e.g. "
             "*UDISPCTX_CAPITALIZATION_NONE*, *UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE*, "
             "*UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE*, etc.")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UDISPCTX_TYPE_DISPLAY_LENGTH", UDISPCTX_TYPE_DISPLAY_LENGTH,
             "Type to retrieve the display length setting, e.g. "
             "*UDISPCTX_LENGTH_FULL*, *UDISPCTX_LENGTH_SHORT*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("UDISPCTX_TYPE_SUBSTITUTE_HANDLING", UDISPCTX_TYPE_SUBSTITUTE_HANDLING,
             "Type to retrieve the substitute handling setting, e.g. "
             "*UDISPCTX_SUBSTITUTE*, *UDISPCTX_NO_SUBSTITUTE*.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
      .export_values();

#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

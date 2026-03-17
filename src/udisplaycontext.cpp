#include "main.hpp"
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
#include <unicode/udisplaycontext.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

void init_udisplaycontext(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  //
  // enum UDisplayContext
  //
  py::enum_<UDisplayContext>(m, "UDisplayContext", py::arithmetic(), R"doc(
Display context settings.

Note, the specific numeric values are internal and may change.
      )doc")
      .value("UDISPCTX_STANDARD_NAMES", UDISPCTX_STANDARD_NAMES, R"doc(
             A possible setting for
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_DIALECT_HANDLING`:
             use standard names when generating a locale name, e.g.
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_DIALECT_HANDLING` can be
             set to one of UDISPCTX_STANDARD_NAMES or UDISPCTX_DIALECT_NAMES.

             Use :attr:`~UDisplayContextType.UDISPCTX_TYPE_DIALECT_HANDLING` to
             get the value. en_GB displays as "English (United Kingdom)".
             )doc")
      .value("UDISPCTX_DIALECT_NAMES", UDISPCTX_DIALECT_NAMES, R"doc(
             A possible setting for
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_DIALECT_HANDLING`: use
             dialect names, when generating a locale name, e.g. en_GB displays
             as "British English".
             )doc")
      .value("UDISPCTX_CAPITALIZATION_NONE", UDISPCTX_CAPITALIZATION_NONE,
             R"doc(
             The capitalization context to be used is unknown (this is the
             default value).

             :attr:`~UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION` can be
             set to one of
             UDISPCTX_CAPITALIZATION_NONE,
             UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE,
             UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE,
             UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU, or
             UDISPCTX_CAPITALIZATION_FOR_STANDALONE.

             Use
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_CAPITALIZATION` to get
             the value.
             )doc")
      .value("UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE",
             UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE, R"doc(
             The capitalization context if a date, date symbol or display
             name is to be formatted with capitalization appropriate for the
             middle of a sentence.
             )doc")
      .value("UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE",
             UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for the
             beginning of a sentence.
             )doc")
      .value("UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU",
             UDISPCTX_CAPITALIZATION_FOR_UI_LIST_OR_MENU, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for a
             user-interface list or menu item.
             )doc")
      .value("UDISPCTX_CAPITALIZATION_FOR_STANDALONE",
             UDISPCTX_CAPITALIZATION_FOR_STANDALONE, R"doc(
             The capitalization context if a date, date symbol or display name
             is to be formatted with capitalization appropriate for stand-alone
             usage such as an isolated name on a calendar page.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UDISPCTX_LENGTH_FULL", UDISPCTX_LENGTH_FULL, R"doc(
             A possible setting for
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_DISPLAY_LENGTH`:
             use full names when generating a locale name, e.g.
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_DISPLAY_LENGTH` can be
             set to one of UDISPCTX_LENGTH_FULL or UDISPCTX_LENGTH_SHORT.

             Use :attr:`~UDisplayContextType.UDISPCTX_TYPE_DISPLAY_LENGTH` to
             get the value. "United States" for US.
             )doc")
      .value("UDISPCTX_LENGTH_SHORT", UDISPCTX_LENGTH_SHORT, R"doc(
             A possible setting for
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_DISPLAY_LENGTH`:
             use short names when generating a locale name, e.g. "U.S." for US.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("UDISPCTX_SUBSTITUTE", UDISPCTX_SUBSTITUTE, R"doc(
             A possible setting for
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_SUBSTITUTE_HANDLING`:
             Returns a fallback value (e.g., the input code) when no data is
             available.

             :attr:`~UDisplayContextType.UDISPCTX_TYPE_SUBSTITUTE_HANDLING` can
             be set to one of UDISPCTX_SUBSTITUTE or UDISPCTX_NO_SUBSTITUTE.

             Use :attr:`~UDisplayContextType.UDISPCTX_TYPE_SUBSTITUTE_HANDLING`
             to get the value. This is the default value.
             )doc")
      .value("UDISPCTX_NO_SUBSTITUTE", UDISPCTX_NO_SUBSTITUTE, R"doc(
             A possible setting for
             :attr:`~UDisplayContextType.UDISPCTX_TYPE_SUBSTITUTE_HANDLING`:
             Returns a null value with error code set to
             :attr:`~UErrorCode.U_ILLEGAL_ARGUMENT_ERROR` when no data is
             available.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
      .export_values();

  //
  // enum UDisplayContextType
  //
  py::enum_<UDisplayContextType>(m, "UDisplayContextType", py::arithmetic(),
                                 R"doc(
Display context types, for getting values of a particular setting.

Note, the specific numeric values are internal and may change.
      )doc")
      .value("UDISPCTX_TYPE_DIALECT_HANDLING", UDISPCTX_TYPE_DIALECT_HANDLING,
             R"doc(
             Type to retrieve the dialect handling setting, e.g.
             :attr:`~UDisplayContext.UDISPCTX_STANDARD_NAMES` or
             :attr:`~UDisplayContext.UDISPCTX_DIALECT_NAMES`.
             )doc")
      .value("UDISPCTX_TYPE_CAPITALIZATION", UDISPCTX_TYPE_CAPITALIZATION,
             R"doc(
             Type to retrieve the capitalization context setting, e.g.
             :attr:`~UDisplayContext.UDISPCTX_CAPITALIZATION_NONE`,
             :attr:`~UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_MIDDLE_OF_SENTENCE`,
             :attr:`~UDisplayContext.UDISPCTX_CAPITALIZATION_FOR_BEGINNING_OF_SENTENCE`,
             etc.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
      .value("UDISPCTX_TYPE_DISPLAY_LENGTH", UDISPCTX_TYPE_DISPLAY_LENGTH,
             R"doc(
             Type to retrieve the display length setting, e.g.
             :attr:`~UDisplayContext.UDISPCTX_LENGTH_FULL`,
             :attr:`~UDisplayContext.UDISPCTX_LENGTH_SHORT`.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
#if (U_ICU_VERSION_MAJOR_NUM >= 58)
      .value("UDISPCTX_TYPE_SUBSTITUTE_HANDLING",
             UDISPCTX_TYPE_SUBSTITUTE_HANDLING, R"doc(
             Type to retrieve the substitute handling setting, e.g.
             :attr:`~UDisplayContext.UDISPCTX_SUBSTITUTE`,
             :attr:`~UDisplayContext.UDISPCTX_NO_SUBSTITUTE`.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 58)
      .export_values();

#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
}

#include "main.hpp"
#include <unicode/uidna.h>

void init_uidna(py::module &m) {
  //
  // enum UIDNAOption
  //
  py::enum_<decltype(UIDNA_DEFAULT)>(m, "UIDNAOption", py::arithmetic(), R"doc(
Bit set to modify the :class:`IDNA` processing and error checking.

Warning:
    ``UIDNAOption`` is not standard. Do not use the fully qualified name as
    this name may change in the future.
      )doc")
      .value("UIDNA_DEFAULT", UIDNA_DEFAULT, R"doc(
             Default options value: UTS #46 nontransitional processing.

             For use in static worker and factory methods.

             Since ICU 76, this is the same as
             :attr:`UIDNA_NONTRANSITIONAL_TO_ASCII` |
             :attr:`UIDNA_NONTRANSITIONAL_TO_UNICODE`, corresponding to Unicode
             15.1 UTS #46 deprecating transitional processing. (These options
             are ignored by the IDNA2003 implementation.)

             Before ICU 76, this constant did not set any of the options.
             )doc")
      .value("UIDNA_ALLOW_UNASSIGNED", UIDNA_ALLOW_UNASSIGNED, R"doc(
             Deprecated: ICU 55 Use UTS #46 instead via class :class:`IDNA`.
             )doc")
      .value("UIDNA_USE_STD3_RULES", UIDNA_USE_STD3_RULES, R"doc(
             Option to check whether the input conforms to the STD3 ASCII
             rules, for example the restriction of labels to LDH characters
             (ASCII Letters, Digits and Hyphen-Minus).

             For use in static worker and factory methods.
             )doc")
      .value("UIDNA_CHECK_BIDI", UIDNA_CHECK_BIDI, R"doc(
             IDNA option to check for whether the input conforms to the BiDi
             rules.

             For use in static worker and factory methods.

             This option is ignored by the IDNA2003 implementation. (IDNA2003
             always performs a BiDi check.)
             )doc")
      .value("UIDNA_CHECK_CONTEXTJ", UIDNA_CHECK_CONTEXTJ, R"doc(
             IDNA option to check for whether the input conforms to the
             CONTEXTJ rules.

             For use in static worker and factory methods.

             This option is ignored by the IDNA2003 implementation. (The
             CONTEXTJ check is new in IDNA2008.)
             )doc")
      .value("UIDNA_NONTRANSITIONAL_TO_ASCII", UIDNA_NONTRANSITIONAL_TO_ASCII,
             R"doc(
             IDNA option for nontransitional processing in ``to_ascii()``.

             For use in static worker and factory methods.

             By default, ``to_ascii()`` uses transitional processing.
             Unicode 15.1 UTS #46 deprecated transitional processing.

             This option is ignored by the IDNA2003 implementation. (This is
             only relevant for compatibility of newer IDNA implementations with
             IDNA2003.)
             )doc")
      .value("UIDNA_NONTRANSITIONAL_TO_UNICODE",
             UIDNA_NONTRANSITIONAL_TO_UNICODE,
             R"doc(
             IDNA option for nontransitional processing in ``to_unicode()``.

             For use in static worker and factory methods.

             By default, ``to_unicode()`` uses transitional processing.
             Unicode 15.1 UTS #46 deprecated transitional processing.

             This option is ignored by the IDNA2003 implementation. (This is
             only relevant for compatibility of newer IDNA implementations with
             IDNA2003.)
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UIDNA_CHECK_CONTEXTO", UIDNA_CHECK_CONTEXTO, R"doc(
             IDNA option to check for whether the input conforms to the
             CONTEXTO rules.

             For use in static worker and factory methods.

             This option is ignored by the IDNA2003 implementation. (The
             CONTEXTO check is new in IDNA2008.)

             This is for use by registries for IDNA2008 conformance. UTS #46
             does not require the CONTEXTO check.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();

  //
  // enum UIDNAError
  //
  py::enum_<decltype(UIDNA_ERROR_EMPTY_LABEL)>(m, "UIDNAError",
                                               py::arithmetic(),
                                               R"doc(
Bit set indicating :class:`IDNA` processing errors.

Warning:
    ``UIDNAError`` is not standard. Do not use the fully qualified name as
    this name may change in the future.
      )doc")
      .value("UIDNA_ERROR_EMPTY_LABEL", UIDNA_ERROR_EMPTY_LABEL, R"doc(
             A non-final domain name label (or the whole domain name) is empty.
             )doc")
      .value("UIDNA_ERROR_LABEL_TOO_LONG", UIDNA_ERROR_LABEL_TOO_LONG, R"doc(
             A domain name label is longer than 63 bytes.

             (See STD13/RFC1034 3.1. Name space specifications and
             terminology.) This is only checked in ToASCII operations, and only
             if the output label is all-ASCII.
             )doc")
      .value("UIDNA_ERROR_DOMAIN_NAME_TOO_LONG",
             UIDNA_ERROR_DOMAIN_NAME_TOO_LONG,
             R"doc(
             A domain name is longer than 255 bytes in its storage form.

             (See STD13/RFC1034 3.1. Name space specifications and
             terminology.) This is only checked in ToASCII operations, and only
             if the output domain name is all-ASCII.
             )doc")
      .value("UIDNA_ERROR_LEADING_HYPHEN", UIDNA_ERROR_LEADING_HYPHEN,
             R"doc(
             A label starts with a hyphen-minus ('-').
             )doc")
      .value("UIDNA_ERROR_TRAILING_HYPHEN", UIDNA_ERROR_TRAILING_HYPHEN,
             R"doc(
             A label ends with a hyphen-minus ('-').
             )doc")
      .value("UIDNA_ERROR_HYPHEN_3_4", UIDNA_ERROR_HYPHEN_3_4, R"doc(
             A label contains hyphen-minus ('-') in the third and fourth
             positions.
             )doc")
      .value("UIDNA_ERROR_LEADING_COMBINING_MARK",
             UIDNA_ERROR_LEADING_COMBINING_MARK, R"doc(
             A label starts with a combining mark.
             )doc")
      .value("UIDNA_ERROR_DISALLOWED", UIDNA_ERROR_DISALLOWED, R"doc(
             A label or domain name contains disallowed characters.
             )doc")
      .value("UIDNA_ERROR_PUNYCODE", UIDNA_ERROR_PUNYCODE, R"doc(
             A label starts with "xn--" but does not contain valid Punycode.

             That is, an xn– label failed Punycode decoding.
             )doc")
      .value("UIDNA_ERROR_LABEL_HAS_DOT", UIDNA_ERROR_LABEL_HAS_DOT, R"doc(
             A label contains a dot=full stop.

             This can occur in an input string for a single-label function.
             )doc")
      .value("UIDNA_ERROR_INVALID_ACE_LABEL", UIDNA_ERROR_INVALID_ACE_LABEL,
             R"doc(
             An ACE label does not contain a valid label string.

             The label was successfully ACE (Punycode) decoded but the
             resulting string had severe validation errors. For example, it
             might contain characters that are not allowed in ACE labels, or it
             might not be normalized.
             )doc")
      .value("UIDNA_ERROR_BIDI", UIDNA_ERROR_BIDI, R"doc(
             A label does not meet the IDNA BiDi requirements (for
             right-to-left characters).
             )doc")
      .value("UIDNA_ERROR_CONTEXTJ", UIDNA_ERROR_CONTEXTJ, R"doc(
             A label does not meet the IDNA CONTEXTJ requirements.
             )doc")
      .value("UIDNA_ERROR_CONTEXTO_PUNCTUATION",
             UIDNA_ERROR_CONTEXTO_PUNCTUATION, R"doc(
             A label does not meet the IDNA CONTEXTO requirements for
             punctuation characters.

             Some punctuation characters "Would otherwise have been DISALLOWED"
             but are allowed in certain contexts. (RFC 5892)
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
      .value("UIDNA_ERROR_CONTEXTO_DIGITS", UIDNA_ERROR_CONTEXTO_DIGITS, R"doc(
             A label does not meet the IDNA CONTEXTO requirements for digits.

             Arabic-Indic Digits (U+066x) must not be mixed with Extended
             Arabic-Indic Digits (U+06Fx).
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
      .export_values();
}

#include "main.hpp"
#include <unicode/decimfmt.h>
#include <unicode/errorcode.h>
#include <unicode/fmtable.h>
#include <unicode/locid.h>
#include <unicode/plurrule.h>

#ifndef MODULE_NAME
#define MODULE_NAME icu
#endif // MODULE_NAME

using namespace icu;

void init_alphaindex(py::module &m);
void init_appendable(py::module &m);
void init_caniter(py::module &m);
void init_casemap(py::module &m);
void init_char16ptr(py::module &m);
void init_coleitr(py::module &m);
void init_compactdecimalformat(py::module &m);
void init_currpinf(py::module &m);
void init_currunit(py::module &m);
void init_datefmt(py::module &m);
void init_dcfmtsym(py::module &m);
void init_decimfmt(py::module &m, py::class_<DecimalFormat, NumberFormat> &df);
void init_dtfmtsym(py::module &m);
void init_dtintrv(py::module &m);
void init_dtitvfmt(py::module &m);
void init_dtitvinf(py::module &m);
void init_dtptngen(py::module &m);
void init_dtrule(py::module &m);
void init_edits(py::module &m);
void init_errorcode(py::module &m);
void init_fieldpos(py::module &m);
void init_filteredbrk(py::module &m);
void init_fmtable(py::module &m, py::class_<Formattable, UObject> &fmt);
void init_format(py::module &m);
void init_formattedvalue(py::module &m);
void init_fpositer(py::module &m);
void init_gender(py::module &m);
void init_gregocal(py::module &m);
void init_icudataver(py::module &m);
void init_idna(py::module &m);
void init_listformatter(py::module &m);
void init_localebuilder(py::module &m);
void init_localematcher(py::module &m);
void init_locdspnm(py::module &m);
void init_locid(py::module &m, py::class_<Locale, UObject> &loc);
void init_measfmt(py::module &m);
void init_measunit(py::module &m);
void init_measure(py::module &m);
void init_messagepattern(py::module &m);
void init_msgfmt(py::module &m);
void init_normalizer2(py::module &m);
void init_nounit(py::module &m);
void init_numberformatter(py::module &m, py::module &m2);
void init_numberrangeformatter(py::module &m, py::module &m2);
void init_numfmt(py::module &m, py::class_<NumberFormat, Format> &nf);
void init_numsys(py::module &m);
void init_parseerr(py::module &m);
void init_parsepos(py::module &m);
void init_plurfmt(py::module &m);
void init_plurrule(py::module &m, py::class_<PluralRules, UObject> &pr);
void init_rbbi(py::module &m);
void init_rbnf(py::module &m);
void init_regex(py::module &m);
void init_region(py::module &m);
void init_reldatefmt(py::module &m);
void init_resbund(py::module &m);
void init_schriter(py::module &m);
void init_scientificnumberformatter(py::module &m);
void init_selfmt(py::module &m);
void init_smpdtfmt(py::module &m);
void init_sortkey(py::module &m);
void init_strenum(py::module &m);
void init_stringoptions(py::module &m);
void init_stsearch(py::module &m);
void init_tblcoll(py::module &m);
void init_timezone(py::module &m);
void init_tmunit(py::module &m);
void init_translit(py::module &m);
void init_tzfmt(py::module &m);
void init_tznames(py::module &m);
void init_tzrule(py::module &m);
void init_tztrans(py::module &m);
void init_ubidi(py::module &m);
void init_ubiditransform(py::module &m);
void init_ubrk(py::module &m);
void init_ucal(py::module &m);
void init_uchar(py::module &m);
void init_ucnv(py::module &m);
void init_ucnv_cb(py::module &m);
void init_ucnv_err(py::module &m);
void init_ucol(py::module &m);
void init_ucpmap(py::module &m);
void init_ucsdet(py::module &m);
void init_ucurr(py::module &m);
void init_udat(py::module &m);
void init_udatpg(py::module &m);
void init_udisplaycontext(py::module &m);
void init_uenum(py::module &m);
void init_uformattedvalue(py::module &m);
void init_ugender(py::module &m);
void init_uidna(py::module &m);
void init_uldnames(py::module &m);
void init_ulistformatter(py::module &m);
void init_uloc(py::module &m);
void init_ulocdata(py::module &m);
void init_uniset(py::module &m);
void init_unistr(py::module &m, py::class_<Replaceable, UObject> &rep, py::class_<UnicodeString, Replaceable> &us);
void init_unorm2(py::module &m);
void init_unum(py::module &m);
void init_unumberformatter(py::module &m);
void init_unumberrangeformatter(py::module &m);
void init_upluralrules(py::module &m);
void init_uregex(py::module &m);
void init_uregion(py::module &m);
void init_ureldatefmt(py::module &m);
void init_ures(py::module &m);
void init_uscript(py::module &m);
void init_usearch(py::module &m);
void init_uset(py::module &m);
void init_usetiter(py::module &m);
void init_ushape(py::module &m);
void init_uspoof(py::module &m);
void init_usprep(py::module &m);
void init_ustring(py::module &m);
void init_utext(py::module &m);
void init_utmscale(py::module &m);
void init_utrans(py::module &m);
void init_utypes(py::module &m);
void init_uversion(py::module &m);
void init_voidptr(py::module &m);

ICUError::ICUError(UErrorCode error_code, const char *message) {
  error_code_ = error_code;
  what_.append(u_errorName(error_code));
  if (message != nullptr && *message != 0) {
    message_.append(message);
    what_.append(": ");
    what_.append(message);
  }
}

PYBIND11_MODULE(MODULE_NAME, m) {
  static py::exception<ICUError> ex(m, "ICUError");
  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p) {
        std::rethrow_exception(p);
      }
    } catch (const ICUError &e) {
      ErrorCode error_code;
      error_code.set(e.get_error_code());
      auto message = e.get_message();
      if (message == nullptr || strlen(message) == 0) {
        PyErr_SetObject(ex.ptr(), py::cast(error_code).ptr());
      } else {
        py::tuple args(2);
        args[0] = error_code;
        args[1] = message;
        PyErr_SetObject(ex.ptr(), args.ptr());
      }
    }
  });

  // icu::number
  auto number = m.def_submodule("number");

  py::class_<UMemory>(m, "UMemory");

  py::class_<UObject, UMemory>(m, "UObject");

  py::class_<Locale, UObject> loc(m, "Locale");

  py::class_<Replaceable, UObject> rep(m, "Replaceable");

  py::class_<UnicodeString, Replaceable> us(m, "UnicodeString");

  py::class_<Formattable, UObject> fmt(m, "Formattable");

  py::class_<PluralRules, UObject> pr(m, "PluralRules");

  py::bind_vector<_UnicodeStringVector>(m, "UnicodeStringVector", py::module_local(false))
      .def(py::init<size_t>(), py::arg("n"));

  init_appendable(m);     // icu::Appendable
  init_char16ptr(m);      // icu::Char16Ptr, icu::ConstChar16Ptr
  init_errorcode(m);      // icu::ErrorCode
  init_formattedvalue(m); // icu::FormattedValue
  init_parseerr(m);       // UParseError
  init_parsepos(m);       // icu::ParsePosition
  init_strenum(m);        // icu::StringEnumeration
  init_voidptr(m);        // _ConstVoidPtr

  init_dtrule(m);   // icu::DateTimeRule
  init_tzrule(m);   // icu::TimeZoneRule
  init_tztrans(m);  // icu::TimeZoneTransition
  init_timezone(m); // icu::TimeZone

  init_gregocal(m); // icu::Calendar, icu::GregorianCalendar

  init_measunit(m); // icu::MeasureUnit
  init_currunit(m); // icu::CurrencyUnit
  init_tmunit(m);   // icu::TimeUnit
  init_nounit(m);   // icu::NoUnit
  init_measure(m);  // icu::Measure, icu::CurrencyAmount, icu::TimeUnitAmount

  init_currpinf(m);     // icu::CurrencyPluralInfo
  init_dtintrv(m);      // icu::DateInterval
  init_dtitvinf(m);     // icu::DateIntervalInfo
  init_fieldpos(m);     // icu::FieldPosition
  init_fpositer(m);     // icu::FieldPositionIterator
  init_fmtable(m, fmt); // icu::Formattable
  init_numsys(m);       // icu::NumberingSystem
  init_dcfmtsym(m);     // icu::DecimalFormatSymbols

  init_format(m); // icu::Format

  py::class_<NumberFormat, Format> numfmt(m, "NumberFormat");

  py::class_<DecimalFormat, NumberFormat> decfmt(m, "DecimalFormat");

  init_numfmt(m, numfmt); // icu::NumberFormat
  init_rbnf(m);           // icu::RuleBasedNumberFormat

  init_datefmt(m);  // icu::DateFormat
  init_dtfmtsym(m); // icu::DateFormatSymbols
  init_dtitvfmt(m); // icu::DateIntervalFormat
  init_tznames(m);  // icu::TimeZoneNames
  init_tzfmt(m);    // icu::TimeZoneFormat
  init_smpdtfmt(m); // icu::SimpleDateFormat

  init_measfmt(m); // icu::MeasureFormat

  init_messagepattern(m); // icu::MessagePattern
  init_msgfmt(m);         // icu::MessageFormat

  init_selfmt(m); // icu::SelectFormat

  init_numberformatter(m, number);      // icu::number::NumberFormatter
  init_numberrangeformatter(m, number); // icu::number::NumberRangeFormatter
  init_plurrule(m, pr);                 // icu::PluralRules
  init_decimfmt(m, decfmt);             // icu::DecimalFormat
  init_compactdecimalformat(m);         // icu::CompactDecimalFormat
  init_plurfmt(m);                      // icu::PluralFormat
  init_scientificnumberformatter(m);    // icu::ScientificNumberFormatter

  init_schriter(m); // icu::StringCharacterIterator
  init_coleitr(m);  // icu::CollationElementIterator

  init_caniter(m);       // icu::CanonicalIterator
  init_dtptngen(m);      // icu::DateTimePatternGenerator
  init_gender(m);        // icu::GenderInfo
  init_idna(m);          // icu::IDNA
  init_listformatter(m); // icu::ListFormatter
  init_localebuilder(m); // icu::LocaleBuilder
  init_localematcher(m); // icu::LocaleMatcher
  init_locdspnm(m);      // icu::LocaleDisplayNames
  init_locid(m, loc);    // icu::Locale
  init_rbbi(m);          // icu::RuleBasedBreakIterator
  init_regex(m);         // icu::RegexPattern, icu::RegexMatcher
  init_region(m);        // icu::Region
  init_reldatefmt(m);    // icu::RelativeDateTimeFormatter
  init_resbund(m);       // icu::ResourceBundle
  init_sortkey(m);       // icu::CollationKey

  init_filteredbrk(m); // icu::FilteredBreakIteratorBuilder

  init_edits(m);   // icu::Edits
  init_casemap(m); // icu::CaseMap

  init_uniset(m);      // icu::UnicodeSet
  init_usetiter(m);    // icu::UnicodeSetIterator
  init_normalizer2(m); // icu::Normalizer2, icu::FilteredNormalizer2
  init_tblcoll(m);     // icu::RuleBasedCollator
  init_stsearch(m);    // icu::StringSearch
  init_translit(m);    // icu::Transliterator

  init_alphaindex(m); // icu::AlphabeticIndex

  init_unistr(m, rep, us); // icu::UnicodeString

  init_icudataver(m);
  init_stringoptions(m);
  init_ubidi(m);
  init_ubiditransform(m);
  init_ubrk(m);
  init_ucal(m);
  init_uchar(m);
  init_ucnv(m);
  init_ucnv_cb(m);
  init_ucnv_err(m);
  init_ucol(m);
  init_ucpmap(m);
  init_ucsdet(m);
  init_ucurr(m);
  init_udat(m);
  init_udatpg(m);
  init_udisplaycontext(m);
  init_uenum(m);
  init_uformattedvalue(m);
  init_ugender(m);
  init_uidna(m);
  init_uldnames(m);
  init_ulistformatter(m);
  init_uloc(m);
  init_ulocdata(m);
  init_unorm2(m);
  init_unum(m);
  init_unumberformatter(m);
  init_unumberrangeformatter(m);
  init_upluralrules(m);
  init_uregex(m);
  init_uregion(m);
  init_ureldatefmt(m);
  init_ures(m);
  init_uscript(m);
  init_usearch(m);
  init_uset(m);
  init_ushape(m);
  init_uspoof(m);
  init_usprep(m);
  init_ustring(m);
  init_utext(m);
  init_utmscale(m);
  init_utrans(m);
  init_utypes(m);
  init_uversion(m);

  m.attr("VERSION_INFO") = VERSION_INFO; // VERSION_INFO is defined by _build.py or CMakeLists.txt

  // <unicode/umachine.h>
  m.attr("INT16_MAX") = INT16_MAX;
  m.attr("INT16_MIN") = INT16_MIN;
  m.attr("INT32_MAX") = INT32_MAX;
  m.attr("INT32_MIN") = INT32_MIN;
  m.attr("INT64_MAX") = INT64_MAX;
  m.attr("INT64_MIN") = INT64_MIN;
  m.attr("U_SENTINEL") = U_SENTINEL;
}

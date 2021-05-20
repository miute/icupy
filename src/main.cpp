#include "main.hpp"
#include <unicode/simpletz.h>
#include <unicode/smpdtfmt.h>

#ifndef MODULE_NAME
#define MODULE_NAME icu
#endif // MODULE_NAME

using namespace icu;

void init_appendable(py::module &m);
void init_char16ptr(py::module &m);
void init_coleitr(py::module &m);
void init_datefmt(py::module &m);
void init_dtfmtsym(py::module &m);
void init_dtintrv(py::module &m);
void init_dtitvfmt(py::module &m);
void init_dtitvinf(py::module &m);
void init_dtptngen(py::module &m);
void init_dtrule(py::module &m);
void init_fieldpos(py::module &m);
void init_fmtable(py::module &m);
void init_format(py::module &m);
void init_formattedvalue(py::module &m);
void init_fpositer(py::module &m);
void init_gregocal(py::module &m);
void init_idna(py::module &m);
void init_localebuilder(py::module &m);
void init_localematcher(py::module &m);
void init_locid(py::module &m, py::class_<Locale, UObject> &loc);
void init_normalizer2(py::module &m);
void init_parseerr(py::module &m);
void init_parsepos(py::module &m);
void init_rbbi(py::module &m);
void init_regex(py::module &m);
void init_resbund(py::module &m);
void init_schriter(py::module &m);
void init_smpdtfmt(py::module &m);
void init_sortkey(py::module &m);
void init_strenum(py::module &m);
void init_stsearch(py::module &m);
void init_tblcoll(py::module &m);
void init_timezone(py::module &m);
void init_translit(py::module &m);
void init_tzfmt(py::module &m);
void init_tznames(py::module &m);
void init_tzrule(py::module &m);
void init_tztrans(py::module &m);
void init_ubidi(py::module &m);
void init_ubrk(py::module &m);
void init_ucal(py::module &m);
void init_uchar(py::module &m);
void init_ucnv(py::module &m);
void init_ucnv_cb(py::module &m);
void init_ucnv_err(py::module &m);
void init_ucol(py::module &m);
void init_ucpmap(py::module &m);
void init_ucsdet(py::module &m);
void init_udat(py::module &m);
void init_udatpg(py::module &m);
void init_udisplaycontext(py::module &m);
void init_uenum(py::module &m);
void init_uformattedvalue(py::module &m);
void init_uidna(py::module &m);
void init_uloc(py::module &m);
void init_uniset(py::module &m);
void init_unistr(py::module &m, py::class_<Replaceable, UObject> &rep, py::class_<UnicodeString, Replaceable> &us);
void init_unorm2(py::module &m);
void init_uregex(py::module &m);
void init_ures(py::module &m);
void init_uscript(py::module &m);
void init_usearch(py::module &m);
void init_uset(py::module &m);
void init_ustring(py::module &m);
void init_utext(py::module &m);
void init_utrans(py::module &m);
void init_utypes(py::module &m);
void init_uversion(py::module &m);
void init_voidptr(py::module &m);

ICUException::ICUException(UErrorCode error_code, const char *message) {
  error_code_ = error_code;
  what_.append(u_errorName(error_code));
  if (message != nullptr && *message != 0) {
    message_.append(message);
    what_.append(": ");
    what_.append(message);
  }
}

PYBIND11_MODULE(MODULE_NAME, m) {
  static py::exception<ICUException> ex(m, "ICUException");
  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p) {
        std::rethrow_exception(p);
      }
    } catch (const ICUException &e) {
      auto error_code = e.get_error_code();
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

  py::class_<UMemory>(m, "UMemory");

  py::class_<UObject, UMemory>(m, "UObject");

  py::class_<Locale, UObject> loc(m, "Locale");

  py::class_<Replaceable, UObject> rep(m, "Replaceable");

  py::class_<UnicodeString, Replaceable> us(m, "UnicodeString");

  py::bind_vector<_UnicodeStringVector>(m, "UnicodeStringVector", py::module_local(false))
      .def(py::init<size_t>(), py::arg("n"));

  init_appendable(m);
  init_char16ptr(m);
  init_formattedvalue(m);
  init_parseerr(m);
  init_parsepos(m);
  init_strenum(m);
  init_voidptr(m);

  init_dtrule(m);
  init_tzrule(m);
  init_tztrans(m);
  init_timezone(m);

  init_gregocal(m);

  init_dtintrv(m);
  init_dtitvinf(m);
  init_fieldpos(m);
  init_fpositer(m);
  init_fmtable(m);
  init_format(m);
  init_datefmt(m);
  init_dtfmtsym(m);
  init_dtitvfmt(m);
  init_tznames(m);
  init_tzfmt(m);
  init_smpdtfmt(m);

  init_schriter(m);
  init_coleitr(m);

  init_dtptngen(m);
  init_idna(m);
  init_localebuilder(m);
  init_localematcher(m);
  init_locid(m, loc);
  init_rbbi(m);
  init_regex(m);
  init_resbund(m);
  init_sortkey(m);

  init_uniset(m);
  init_normalizer2(m);
  init_tblcoll(m);
  init_stsearch(m);
  init_translit(m);

  init_unistr(m, rep, us);

  init_ubidi(m);
  init_ubrk(m);
  init_ucal(m);
  init_uchar(m);
  init_ucnv(m);
  init_ucnv_cb(m);
  init_ucnv_err(m);
  init_ucol(m);
  init_ucpmap(m);
  init_ucsdet(m);
  init_udat(m);
  init_udatpg(m);
  init_udisplaycontext(m);
  init_uenum(m);
  init_uformattedvalue(m);
  init_uidna(m);
  init_uloc(m);
  init_unorm2(m);
  init_uregex(m);
  init_ures(m);
  init_uscript(m);
  init_usearch(m);
  init_uset(m);
  init_ustring(m);
  init_utext(m);
  init_utrans(m);
  init_utypes(m);
  init_uversion(m);

#ifdef VERSION_INFO
  m.attr("LIB_VERSION") = VERSION_INFO;
#else
  UErrorCode error_code = U_ZERO_ERROR;
  LocalPointer<DateFormat> ifmt(
      DateFormat::createInstanceForSkeleton("MMM dd yyyy HH:mm:ss", Locale::getUS(), error_code));
  auto pos = ParsePosition();
  auto date = ifmt->parse(__DATE__ " " __TIME__, pos); // "MMM dd yyyy" "HH:mm:ss"
  error_code = U_ZERO_ERROR;
  auto ofmt = SimpleDateFormat(u"yyyy.M.d.'dev'Hmm", error_code);
  ofmt.setTimeZone(SimpleTimeZone(0, "UTC"));
  UnicodeString dest;
  ofmt.format(date, dest);
  std::string result;
  dest.toUTF8String(result);
  m.attr("LIB_VERSION") = result;
#endif // VERSION_INFO

  // <unicode/umachine.h>
  m.attr("INT16_MAX") = INT16_MAX;
  m.attr("INT16_MIN") = INT16_MIN;
  m.attr("INT32_MAX") = INT32_MAX;
  m.attr("INT32_MIN") = INT32_MIN;
  m.attr("U_SENTINEL") = U_SENTINEL;
}

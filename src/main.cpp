#include "main.hpp"
#include <unicode/simpletz.h>
#include <unicode/smpdtfmt.h>

#ifndef MODULE_NAME
#define MODULE_NAME icu
#endif // MODULE_NAME

using namespace icu;

void init_char16ptr(py::module &m);
void init_coleitr(py::module &m);
void init_idna(py::module &);
void init_locid(py::module &);
void init_normalizer2(py::module &m);
void init_parseerr(py::module &m);
void init_parsepos(py::module &);
void init_rbbi(py::module &);
void init_regex(py::module &);
void init_resbund(py::module &);
void init_schriter(py::module &);
void init_sortkey(py::module &);
void init_strenum(py::module &);
void init_tblcoll(py::module &m);
void init_translit(py::module &m);
void init_ubidi(py::module &);
void init_ubrk(py::module &);
void init_uchar(py::module &);
void init_ucnv(py::module &);
void init_ucnv_cb(py::module &);
void init_ucnv_err(py::module &);
void init_ucol(py::module &);
void init_ucpmap(py::module &);
void init_uidna(py::module &);
void init_uloc(py::module &);
void init_uniset(py::module &m);
void init_unistr(py::module &);
void init_unorm2(py::module &m);
void init_uregex(py::module &m);
void init_ures(py::module &m);
void init_uscript(py::module &m);
void init_uset(py::module &m);
void init_ustring(py::module &);
void init_utext(py::module &);
void init_utrans(py::module &m);
void init_utypes(py::module &);
void init_uversion(py::module &);
void init_voidptr(py::module &);

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

  init_char16ptr(m);
  init_coleitr(m);
  init_idna(m);
  init_locid(m);
  init_normalizer2(m);
  init_parseerr(m);
  init_parsepos(m);
  init_rbbi(m);
  init_regex(m);
  init_resbund(m);
  init_schriter(m);
  init_sortkey(m);
  init_strenum(m);
  init_tblcoll(m);
  init_translit(m);
  init_ubidi(m);
  init_ubrk(m);
  init_uchar(m);
  init_ucnv(m);
  init_ucnv_cb(m);
  init_ucnv_err(m);
  init_ucol(m);
  init_ucpmap(m);
  init_uidna(m);
  init_uloc(m);
  init_uniset(m);
  init_unistr(m);
  init_unorm2(m);
  init_uregex(m);
  init_ures(m);
  init_uscript(m);
  init_uset(m);
  init_ustring(m);
  init_utext(m);
  init_utrans(m);
  init_utypes(m);
  init_uversion(m);
  init_voidptr(m);

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

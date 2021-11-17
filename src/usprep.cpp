#include "main.hpp"
#include "usprepptr.hpp"

_UStringPrepProfilePtr::_UStringPrepProfilePtr(UStringPrepProfile *p) : p_(p) {}
_UStringPrepProfilePtr::~_UStringPrepProfilePtr() {}
UStringPrepProfile *_UStringPrepProfilePtr::get() const { return p_; }

void init_usprep(py::module &m) {
  py::enum_<UStringPrepProfileType>(
      m, "UStringPrepProfileType", py::arithmetic(),
      "enums for the standard stringprep profile types supported by *usprep_open_by_type*.")
      .value("USPREP_RFC3491_NAMEPREP", USPREP_RFC3491_NAMEPREP, "RFC3491 Nameprep.")
      .value("USPREP_RFC3530_NFS4_CS_PREP", USPREP_RFC3530_NFS4_CS_PREP, "RFC3530 nfs4_cs_prep.")
      .value("USPREP_RFC3530_NFS4_CS_PREP_CI", USPREP_RFC3530_NFS4_CS_PREP_CI,
             "RFC3530 nfs4_cs_prep with case insensitive option.")
      .value("USPREP_RFC3530_NFS4_CIS_PREP", USPREP_RFC3530_NFS4_CIS_PREP, "RFC3530 nfs4_cis_prep.")
      .value("USPREP_RFC3530_NFS4_MIXED_PREP_PREFIX", USPREP_RFC3530_NFS4_MIXED_PREP_PREFIX,
             "RFC3530 nfs4_mixed_prep for prefix.")
      .value("USPREP_RFC3530_NFS4_MIXED_PREP_SUFFIX", USPREP_RFC3530_NFS4_MIXED_PREP_SUFFIX,
             "RFC3530 nfs4_mixed_prep for suffix.")
      .value("USPREP_RFC3722_ISCSI", USPREP_RFC3722_ISCSI, "RFC3722 iSCSI.")
      .value("USPREP_RFC3920_NODEPREP", USPREP_RFC3920_NODEPREP, "RFC3920 XMPP Nodeprep.")
      .value("USPREP_RFC3920_RESOURCEPREP", USPREP_RFC3920_RESOURCEPREP, "RFC3920 XMPP Resourceprep.")
      .value("USPREP_RFC4011_MIB", USPREP_RFC4011_MIB, "RFC4011 Policy MIB Stringprep.")
      .value("USPREP_RFC4013_SASLPREP", USPREP_RFC4013_SASLPREP, "RFC4013 SASLprep.")
      .value("USPREP_RFC4505_TRACE", USPREP_RFC4505_TRACE, "RFC4505 trace.")
      .value("USPREP_RFC4518_LDAP", USPREP_RFC4518_LDAP, "RFC4518 LDAP.")
      .value("USPREP_RFC4518_LDAP_CI", USPREP_RFC4518_LDAP_CI,
             "RFC4518 LDAP for case ignore, numeric and stored prefix matching rules.")
      .export_values();

  py::class_<_UStringPrepProfilePtr>(m, "_UStringPrepProfilePtr");

  m.def(
      "usprep_close", [](_UStringPrepProfilePtr &profile) { usprep_close(profile); }, py::arg("profile"));
  m.def(
      "usprep_open",
      [](const char *path, const char *file_name) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto profile = usprep_open(path, file_name, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UStringPrepProfilePtr>(profile);
      },
      py::arg("path"), py::arg("file_name"));
  m.def(
      "usprep_open_by_type",
      [](UStringPrepProfileType type) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto profile = usprep_openByType(type, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UStringPrepProfilePtr>(profile);
      },
      py::arg("type_"));
  m.def(
      "usprep_prepare",
      [](_UStringPrepProfilePtr &prep, const char16_t *src, int32_t src_length, int32_t options,
         UParseError *parse_error) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto dest_size = usprep_prepare(prep, src, src_length, nullptr, 0, options, parse_error, &error_code);
        std::u16string result(dest_size, '\0');
        error_code = U_ZERO_ERROR;
        usprep_prepare(prep, src, src_length, result.data(), dest_size, options, parse_error, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("prep"), py::arg("src"), py::arg("src_length"), py::arg("options"), py::arg("parse_error"));

  m.attr("USPREP_ALLOW_UNASSIGNED") = USPREP_ALLOW_UNASSIGNED;
  m.attr("USPREP_DEFAULT") = USPREP_DEFAULT;
}

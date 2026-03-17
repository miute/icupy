#include "main.hpp"
#include "usprepptr.hpp"
#include <optional>
#include <pybind11/stl.h>

using namespace icu;

namespace icupy {

//
// struct UStringPrepProfile
//
UStringPrepProfilePtr::UStringPrepProfilePtr(UStringPrepProfile *p) : p_(p) {}

UStringPrepProfilePtr::~UStringPrepProfilePtr() {}

UStringPrepProfile *UStringPrepProfilePtr::get() const { return p_; }

} // namespace icupy

void init_usprep(py::module &m) {
  //
  // enum UStringPrepProfileType
  //
  py::enum_<UStringPrepProfileType>(m, "UStringPrepProfileType",
                                    py::arithmetic(), R"doc(
Enums for the standard StringPrep profile types supported by
:func:`usprep_open_by_type`.
      )doc")
      .value("USPREP_RFC3491_NAMEPREP", USPREP_RFC3491_NAMEPREP, R"doc(
             RFC3491 Nameprep.
             )doc")
      .value("USPREP_RFC3530_NFS4_CS_PREP", USPREP_RFC3530_NFS4_CS_PREP, R"doc(
             RFC3530 nfs4_cs_prep.
             )doc")
      .value("USPREP_RFC3530_NFS4_CS_PREP_CI", USPREP_RFC3530_NFS4_CS_PREP_CI,
             R"doc(
             RFC3530 nfs4_cs_prep with case insensitive option.
             )doc")
      .value("USPREP_RFC3530_NFS4_CIS_PREP", USPREP_RFC3530_NFS4_CIS_PREP,
             R"doc(
             RFC3530 nfs4_cis_prep.
             )doc")
      .value("USPREP_RFC3530_NFS4_MIXED_PREP_PREFIX",
             USPREP_RFC3530_NFS4_MIXED_PREP_PREFIX, R"doc(
             RFC3530 nfs4_mixed_prep for prefix.
             )doc")
      .value("USPREP_RFC3530_NFS4_MIXED_PREP_SUFFIX",
             USPREP_RFC3530_NFS4_MIXED_PREP_SUFFIX, R"doc(
             RFC3530 nfs4_mixed_prep for suffix.
             )doc")
      .value("USPREP_RFC3722_ISCSI", USPREP_RFC3722_ISCSI, R"doc(
             RFC3722 iSCSI.
             )doc")
      .value("USPREP_RFC3920_NODEPREP", USPREP_RFC3920_NODEPREP, R"doc(
             RFC3920 XMPP Nodeprep.
             )doc")
      .value("USPREP_RFC3920_RESOURCEPREP", USPREP_RFC3920_RESOURCEPREP, R"doc(
             RFC3920 XMPP Resourceprep.
             )doc")
      .value("USPREP_RFC4011_MIB", USPREP_RFC4011_MIB, R"doc(
             RFC4011 Policy MIB Stringprep.
             )doc")
      .value("USPREP_RFC4013_SASLPREP", USPREP_RFC4013_SASLPREP, R"doc(
             RFC4013 SASLprep.
             )doc")
      .value("USPREP_RFC4505_TRACE", USPREP_RFC4505_TRACE, R"doc(
             RFC4505 trace.
             )doc")
      .value("USPREP_RFC4518_LDAP", USPREP_RFC4518_LDAP, R"doc(
             RFC4518 LDAP.
             )doc")
      .value("USPREP_RFC4518_LDAP_CI", USPREP_RFC4518_LDAP_CI, R"doc(
             RFC4518 LDAP for case ignore, numeric and stored prefix matching
             rules.
             )doc")
      .export_values();

  //
  // struct UStringPrepProfile
  //
  py::class_<icupy::UStringPrepProfilePtr>(m, "UStringPrepProfile", R"doc(
       UStringPrepProfile structure for the StingPrep framework as defined
       in RFC 3454.

       See Also:
           :func:`usprep_close`
           :func:`usprep_open`
           :func:`usprep_open_by_type`
       )doc");

  //
  // Functions
  //
  m.def(
      "usprep_close",
      [](icupy::UStringPrepProfilePtr &profile) { usprep_close(profile); },
      py::arg("profile"));

  m.def(
      "usprep_open",
      [](const std::optional<std::string> &path, const std::string &file_name) {
        ErrorCode error_code;
        auto profile = usprep_open(path ? path->data() : nullptr,
                                   file_name.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UStringPrepProfilePtr>(profile);
      },
      py::arg("path"), py::arg("file_name"));

  m.def(
      "usprep_open_by_type",
      [](UStringPrepProfileType type) {
        ErrorCode error_code;
        auto profile = usprep_openByType(type, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UStringPrepProfilePtr>(profile);
      },
      py::arg("type"));

  m.def(
      "usprep_prepare",
      [](icupy::UStringPrepProfilePtr &prep, const std::u16string &src,
         int32_t src_length, int32_t options,
         std::optional<UParseError *> &parse_error) {
        auto src_data = src.data();
        ErrorCode error_code;
        const auto dest_capacity =
            usprep_prepare(prep, src_data, src_length, nullptr, 0, options,
                           nullptr, error_code);
        std::u16string result(dest_capacity, '\0');
        error_code.reset();
        usprep_prepare(prep, src_data, src_length, result.data(), dest_capacity,
                       options, parse_error.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("prep"), py::arg("src"), py::arg("src_length"),
      py::arg("options"), py::arg("parse_error") = std::nullopt);

  m.attr("USPREP_ALLOW_UNASSIGNED") = USPREP_ALLOW_UNASSIGNED;
  m.attr("USPREP_DEFAULT") = USPREP_DEFAULT;
}

#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/strenum.h>
#include <unicode/tznames.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_tznames(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // enum UTimeZoneNameType
  //
  py::enum_<UTimeZoneNameType>(m, "UTimeZoneNameType", py::arithmetic(),
                               "Constants for time zone display name types.")
      .value("UTZNM_UNKNOWN", UTZNM_UNKNOWN, R"doc(
             Unknown display name type.
             )doc")
      .value("UTZNM_LONG_GENERIC", UTZNM_LONG_GENERIC, R"doc(
             Long display name, such as "Eastern Time".
             )doc")
      .value("UTZNM_LONG_STANDARD", UTZNM_LONG_STANDARD, R"doc(
             Long display name for standard time, such as "Eastern Standard
             Time".
             )doc")
      .value("UTZNM_LONG_DAYLIGHT", UTZNM_LONG_DAYLIGHT, R"doc(
             Long display name for daylight saving time, such as "Eastern
             Daylight Time".
             )doc")
      .value("UTZNM_SHORT_GENERIC", UTZNM_SHORT_GENERIC, R"doc(
             Short display name, such as "ET".
             )doc")
      .value("UTZNM_SHORT_STANDARD", UTZNM_SHORT_STANDARD, R"doc(
             Short display name for standard time, such as "EST".
             )doc")
      .value("UTZNM_SHORT_DAYLIGHT", UTZNM_SHORT_DAYLIGHT, R"doc(
             Short display name for daylight saving time, such as "EDT".
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UTZNM_EXEMPLAR_LOCATION", UTZNM_EXEMPLAR_LOCATION, R"doc(
             Exemplar location name, such as "Los Angeles".
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
      .export_values();

  //
  // class icu::TimeZoneNames
  //
  py::class_<TimeZoneNames, UObject> tzn(m, "TimeZoneNames", R"doc(
      Abstract class representing the time zone display name data model defined
      in `Unicode's LDML (UTS#35) <http://www.unicode.org/reports/tr35/>`__.

      .. note::

          The methods of this class assume that the time zone ID is already
          canonicalized. For example, calling a method with the time zone ID
          "America/Indiana/Indianapolis" may not return the correct result
          because it is not canonicalized (the canonicalized ID for this time
          zone is "America/Indianapolis"). For ICU's canonicalized time zone
          IDs, refer to :meth:`TimeZone.get_canonical_id`.

      Example:
          >>> from icupy import icu
          >>> tzn = icu.TimeZoneNames.create_instance(icu.ULOC_US)
          >>> tzid = "America/Los_Angeles"
          >>> result = icu.UnicodeString()
          >>> tzn.get_display_name(tzid, icu.UTZNM_LONG_STANDARD, 0, result)
          UnicodeString('Pacific Standard Time', text_length=21)
          >>> tzn.get_display_name(tzid, icu.UTZNM_SHORT_STANDARD, 0, result)
          UnicodeString('PST', text_length=3)
      )doc");

  tzn.def("__copy__", &TimeZoneNames::clone);

  tzn.def(
      "__deepcopy__",
      [](const TimeZoneNames &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"));

  tzn.def(
      "__eq__",
      [](const TimeZoneNames &self, const TimeZoneNames &other) {
        return self == other;
      },
      py::is_operator(), py::arg("other"));

  tzn.def(
      "__ne__",
      [](const TimeZoneNames &self, const TimeZoneNames &other) {
        return self != other;
      },
      py::is_operator(), py::arg("other"));

  tzn.def("clone", &TimeZoneNames::clone);

  tzn.def_static(
      "create_instance",
      [](const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        auto result =
            TimeZoneNames::createInstance(icupy::to_locale(locale), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"));

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  tzn.def_static(
      "create_tzdb_instance",
      [](const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        auto result = TimeZoneNames::createTZDBInstance(
            icupy::to_locale(locale), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  tzn.def(
         "get_available_meta_zone_ids",
         [](const TimeZoneNames &self,
            const icupy::UnicodeStringVariant &tz_id) {
           ErrorCode error_code;
           auto result = self.getAvailableMetaZoneIDs(icupy::to_unistr(tz_id),
                                                      error_code);
           if (error_code.isFailure()) {
             throw icupy::ICUError(error_code);
           }
           return result;
         },
         py::arg("tz_id"))
      .def("get_available_meta_zone_ids", [](const TimeZoneNames &self) {
        ErrorCode error_code;
        auto result = self.getAvailableMetaZoneIDs(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  tzn.def(
      "get_display_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id,
         UTimeZoneNameType type, UDate date,
         UnicodeString &name) -> UnicodeString & {
        return self.getDisplayName(icupy::to_unistr(tz_id), type, date, name);
      },
      py::arg("tz_id"), py::arg("type"), py::arg("date"), py::arg("name"));

  tzn.def(
      "get_exemplar_location_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id,
         UnicodeString &name) -> UnicodeString & {
        return self.getExemplarLocationName(icupy::to_unistr(tz_id), name);
      },
      py::arg("tz_id"), py::arg("name"));

  tzn.def(
      "get_meta_zone_display_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &mz_id,
         UTimeZoneNameType type, UnicodeString &name) -> UnicodeString & {
        return self.getMetaZoneDisplayName(icupy::to_unistr(mz_id), type, name);
      },
      py::arg("mz_id"), py::arg("type"), py::arg("name"));

  tzn.def(
      "get_meta_zone_id",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id,
         UDate date, UnicodeString &mz_id) -> UnicodeString & {
        return self.getMetaZoneID(icupy::to_unistr(tz_id), date, mz_id);
      },
      py::arg("tz_id"), py::arg("date"), py::arg("mz_id"));

  tzn.def(
      "get_reference_zone_id",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &mz_id,
         const std::string &region, UnicodeString &tz_id) -> UnicodeString & {
        return self.getReferenceZoneID(icupy::to_unistr(mz_id), region.data(),
                                       tz_id);
      },
      py::arg("mz_id"), py::arg("region"), py::arg("tz_id"));

  tzn.def(
      "get_time_zone_display_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id,
         UTimeZoneNameType type, UnicodeString &name) -> UnicodeString & {
        return self.getTimeZoneDisplayName(icupy::to_unistr(tz_id), type, name);
      },
      py::arg("tz_id"), py::arg("type"), py::arg("name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

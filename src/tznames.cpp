#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/locid.h>
#include <unicode/strenum.h>
#include <unicode/tznames.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_tznames(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
  //
  // UTimeZoneNameType
  //
  py::enum_<UTimeZoneNameType>(m, "UTimeZoneNameType", py::arithmetic(), "Constants for time zone display name types.")
      .value("UTZNM_UNKNOWN", UTZNM_UNKNOWN, "Unknown display name type.")
      .value("UTZNM_LONG_GENERIC", UTZNM_LONG_GENERIC, "Long display name, such as \"Eastern Time\".")
      .value("UTZNM_LONG_STANDARD", UTZNM_LONG_STANDARD,
             "Long display name for standard time, such as \"Eastern Standard Time\".")
      .value("UTZNM_LONG_DAYLIGHT", UTZNM_LONG_DAYLIGHT,
             "Long display name for daylight saving time, such as \"Eastern Daylight Time\".")
      .value("UTZNM_SHORT_GENERIC", UTZNM_SHORT_GENERIC, "Short display name, such as \"ET\".")
      .value("UTZNM_SHORT_STANDARD", UTZNM_SHORT_STANDARD, "Short display name for standard time, such as \"EST\".")
      .value("UTZNM_SHORT_DAYLIGHT", UTZNM_SHORT_DAYLIGHT,
             "Short display name for daylight saving time, such as \"EDT\".")
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .value("UTZNM_EXEMPLAR_LOCATION", UTZNM_EXEMPLAR_LOCATION, "Exemplar location name, such as \"Los Angeles\".")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
      .export_values();

  //
  // icu::TimeZoneNames
  //
  py::class_<TimeZoneNames, UObject> tzn(m, "TimeZoneNames");

  tzn.def("__copy__", &TimeZoneNames::clone);

  tzn.def(
      "__deepcopy__", [](const TimeZoneNames &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  tzn.def(
      "__eq__", [](const TimeZoneNames &self, const TimeZoneNames &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  tzn.def(
      "__ne__", [](const TimeZoneNames &self, const TimeZoneNames &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  tzn.def("clone", &TimeZoneNames::clone);

  tzn.def_static(
      "create_instance",
      [](const icupy::LocaleVariant &locale) {
        ErrorCode error_code;
        auto result = TimeZoneNames::createInstance(icupy::to_locale(locale), error_code);
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
        auto result = TimeZoneNames::createTZDBInstance(icupy::to_locale(locale), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("locale"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  tzn.def(
         "get_available_meta_zone_ids",
         [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id) {
           ErrorCode error_code;
           auto result = self.getAvailableMetaZoneIDs(icupy::to_unistr(tz_id), error_code);
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
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id, UTimeZoneNameType type, UDate date,
         UnicodeString &name) -> UnicodeString & {
        return self.getDisplayName(icupy::to_unistr(tz_id), type, date, name);
      },
      py::arg("tz_id"), py::arg("type_"), py::arg("date"), py::arg("name"));

  tzn.def(
      "get_exemplar_location_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id, UnicodeString &name) -> UnicodeString & {
        return self.getExemplarLocationName(icupy::to_unistr(tz_id), name);
      },
      py::arg("tz_id"), py::arg("name"));

  tzn.def(
      "get_meta_zone_display_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &mz_id, UTimeZoneNameType type,
         UnicodeString &name) -> UnicodeString & {
        return self.getMetaZoneDisplayName(icupy::to_unistr(mz_id), type, name);
      },
      py::arg("mz_id"), py::arg("type_"), py::arg("name"));

  tzn.def(
      "get_meta_zone_id",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id, UDate date,
         UnicodeString &mz_id) -> UnicodeString & { return self.getMetaZoneID(icupy::to_unistr(tz_id), date, mz_id); },
      py::arg("tz_id"), py::arg("date"), py::arg("mz_id"));

  tzn.def(
      "get_reference_zone_id",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &mz_id, const char *region, UnicodeString &tz_id)
          -> UnicodeString & { return self.getReferenceZoneID(icupy::to_unistr(mz_id), region, tz_id); },
      py::arg("mz_id"), py::arg("region"), py::arg("tz_id"));

  tzn.def(
      "get_time_zone_display_name",
      [](const TimeZoneNames &self, const icupy::UnicodeStringVariant &tz_id, UTimeZoneNameType type,
         UnicodeString &name) -> UnicodeString & {
        return self.getTimeZoneDisplayName(icupy::to_unistr(tz_id), type, name);
      },
      py::arg("tz_id"), py::arg("type_"), py::arg("name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

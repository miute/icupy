#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 50)
#include <pybind11/operators.h>
#include <unicode/locid.h>
#include <unicode/strenum.h>
#include <unicode/tznames.h>
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)

using namespace icu;

void init_tznames(py::module &m) {
#if (U_ICU_VERSION_MAJOR_NUM >= 50)
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

  // icu::TimeZoneNames
  py::class_<TimeZoneNames, UObject> tzn(m, "TimeZoneNames");
  tzn.def("__copy__", &TimeZoneNames::clone)
      .def(
          "__deepcopy__", [](const TimeZoneNames &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const TimeZoneNames &self, const TimeZoneNames &other) { return self == other; },
          py::is_operator(), py::arg("other"))
      .def(
          "__ne__", [](const TimeZoneNames &self, const TimeZoneNames &other) { return self != other; },
          py::is_operator(), py::arg("other"));
  tzn.def("clone", &TimeZoneNames::clone);
  tzn.def_static(
         "create_instance",
         [](const Locale &locale) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = TimeZoneNames::createInstance(locale, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_instance",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = TimeZoneNames::createInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"));
#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  tzn.def_static(
         "create_tzdb_instance",
         [](const Locale &locale) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = TimeZoneNames::createTZDBInstance(locale, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("locale"))
      .def_static(
          // const char *locale -> const Locale &locale
          "create_tzdb_instance",
          [](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = TimeZoneNames::createTZDBInstance(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)
  tzn.def(
         "get_available_meta_zone_ids",
         [](const TimeZoneNames &self, const UnicodeString &tz_id) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = self.getAvailableMetaZoneIDs(tz_id, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("tz_id"))
      .def(
          // const char16_t *tz_id -> const UnicodeString &tz_id
          "get_available_meta_zone_ids",
          [](const TimeZoneNames &self, const char16_t *tz_id) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getAvailableMetaZoneIDs(tz_id, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("tz_id"))
      .def("get_available_meta_zone_ids", [](const TimeZoneNames &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.getAvailableMetaZoneIDs(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  tzn.def("get_display_name", &TimeZoneNames::getDisplayName, py::arg("tz_id"), py::arg("type_"), py::arg("date"),
          py::arg("name"))
      .def(
          // const char16_t *tz_id -> const UnicodeString &tz_id
          "get_display_name",
          [](const TimeZoneNames &self, const char16_t *tz_id, UTimeZoneNameType type, UDate date,
             UnicodeString &name) -> UnicodeString & { return self.getDisplayName(tz_id, type, date, name); },
          py::arg("tz_id"), py::arg("type_"), py::arg("date"), py::arg("name"));
  tzn.def("get_exemplar_location_name", &TimeZoneNames::getExemplarLocationName, py::arg("tz_id"), py::arg("name"))
      .def(
          // const char16_t *tz_id -> const UnicodeString &tz_id
          "get_exemplar_location_name",
          [](const TimeZoneNames &self, const char16_t *tz_id, UnicodeString &name) -> UnicodeString & {
            return self.getExemplarLocationName(tz_id, name);
          },
          py::arg("tz_id"), py::arg("name"));
  tzn.def("get_meta_zone_display_name", &TimeZoneNames::getMetaZoneDisplayName, py::arg("mz_id"), py::arg("type_"),
          py::arg("name"))
      .def(
          // const char16_t *mz_id -> const UnicodeString &mz_id
          "get_meta_zone_display_name",
          [](const TimeZoneNames &self, const char16_t *mz_id, UTimeZoneNameType type,
             UnicodeString &name) -> UnicodeString & { return self.getMetaZoneDisplayName(mz_id, type, name); },
          py::arg("mz_id"), py::arg("type_"), py::arg("name"));
  tzn.def("get_meta_zone_id", &TimeZoneNames::getMetaZoneID, py::arg("tz_id"), py::arg("date"), py::arg("mz_id"))
      .def(
          // const char16_t *tz_id -> const UnicodeString &tz_id
          "get_meta_zone_id",
          [](const TimeZoneNames &self, const char16_t *tz_id, UDate date, UnicodeString &mz_id) -> UnicodeString & {
            return self.getMetaZoneID(tz_id, date, mz_id);
          },
          py::arg("tz_id"), py::arg("date"), py::arg("mz_id"));
  tzn.def("get_reference_zone_id", &TimeZoneNames::getReferenceZoneID, py::arg("mz_id"), py::arg("region"),
          py::arg("tz_id"))
      .def(
          // const char16_t *mz_id -> const UnicodeString &mz_id
          "get_reference_zone_id",
          [](const TimeZoneNames &self, const char16_t *mz_id, const char *region,
             UnicodeString &tz_id) -> UnicodeString & { return self.getReferenceZoneID(mz_id, region, tz_id); },
          py::arg("mz_id"), py::arg("region"), py::arg("tz_id"));
  tzn.def("get_time_zone_display_name", &TimeZoneNames::getTimeZoneDisplayName, py::arg("tz_id"), py::arg("type_"),
          py::arg("name"))
      .def(
          // const char16_t *tz_id -> const UnicodeString &tz_id
          "get_time_zone_display_name",
          [](const TimeZoneNames &self, const char16_t *tz_id, UTimeZoneNameType type,
             UnicodeString &name) -> UnicodeString & { return self.getTimeZoneDisplayName(tz_id, type, name); },
          py::arg("tz_id"), py::arg("type_"), py::arg("name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 50)
}

#include "main.hpp"
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/locid.h>
#include <unicode/rbtz.h>
#include <unicode/simpletz.h>
#include <unicode/strenum.h>
#include <unicode/vtzone.h>

using namespace icu;

void init_timezone(py::module &m) {
  // icu::TimeZone
  py::class_<TimeZone, UObject> tz(m, "TimeZone");

  // icu::BasicTimeZone
  py::class_<BasicTimeZone, TimeZone> btz(m, "BasicTimeZone");

  // icu::RuleBasedTimeZone
  py::class_<RuleBasedTimeZone, BasicTimeZone> rbtz(m, "RuleBasedTimeZone");

  // icu::SimpleTimeZone
  py::class_<SimpleTimeZone, BasicTimeZone> stz(m, "SimpleTimeZone");

  // icu::VTimeZone
  py::class_<VTimeZone, BasicTimeZone> vtz(m, "VTimeZone");

  // icu::TimeZone
  py::enum_<TimeZone::EDisplayType>(tz, "EDisplayType", py::arithmetic(), "Enum for use with *get_display_name*.")
      .value("SHORT", TimeZone::EDisplayType::SHORT, "Selector for short display name.")
      .value("LONG", TimeZone::EDisplayType::LONG, "Selector for long display name.")
      .value("SHORT_GENERIC", TimeZone::EDisplayType::SHORT_GENERIC, "Selector for short generic display name.")
      .value("LONG_GENERIC", TimeZone::EDisplayType::LONG_GENERIC, "Selector for long generic display name.")
      .value("SHORT_GMT", TimeZone::EDisplayType::SHORT_GMT,
             "Selector for short display name derived from time zone offset.")
      .value("LONG_GMT", TimeZone::EDisplayType::LONG_GMT,
             "Selector for long display name derived from time zone offset.")
      .value("SHORT_COMMONLY_USED", TimeZone::EDisplayType::SHORT_COMMONLY_USED,
             "Selector for short display name derived from the time zone's fallback name.")
      .value("GENERIC_LOCATION", TimeZone::EDisplayType::GENERIC_LOCATION,
             "Selector for long display name derived from the time zone's fallback name.")
      .export_values();

  tz.def("__copy__", &TimeZone::clone)
      .def(
          "__deepcopy__", [](const TimeZone &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const TimeZone &self, const TimeZone &other) { return self == other; }, py::is_operator(),
          py::arg("other"))
      .def(
          "__ne__", [](const TimeZone &self, const TimeZone &other) { return self != other; }, py::is_operator(),
          py::arg("other"));
  // FIXME: Implement "static void icu::TimeZone::adoptDefault(TimeZone *zone)".
  /*
  tz.def_static(
      "adopt_default", [](TimeZone *zone) { TimeZone::adoptDefault(zone ? zone->clone() : nullptr); }, py::arg("zone"));
  */
  tz.def("clone", &TimeZone::clone);
  tz.def_static("count_equivalent_ids", &TimeZone::countEquivalentIDs, py::arg("id_"))
      .def_static(
          // const char16_t *id -> const UnicodeString &id
          "count_equivalent_ids", [](const char16_t *id) { return TimeZone::countEquivalentIDs(id); }, py::arg("id_"));
  tz.def_static("create_default", []() -> std::variant<BasicTimeZone *, TimeZone *> {
    auto tz = TimeZone::createDefault();
    auto btz = dynamic_cast<BasicTimeZone *>(tz);
    if (btz) {
      return btz;
    }
    return tz;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  tz.def_static("create_enumeration", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = TimeZone::createEnumeration(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#ifndef U_HIDE_DEPRECATED_API
  tz.def_static("create_enumeration", py::overload_cast<const char *>(&TimeZone::createEnumeration), py::arg("country"))
      .def_static("create_enumeration", py::overload_cast<int32_t>(&TimeZone::createEnumeration),
                  py::arg("raw_offset"));
#endif // U_HIDE_DEPRECATED_API
#else  // !(U_ICU_VERSION_MAJOR_NUM >= 70)
  tz.def_static("create_enumeration", py::overload_cast<>(&TimeZone::createEnumeration))
      .def_static("create_enumeration", py::overload_cast<const char *>(&TimeZone::createEnumeration),
                  py::arg("country"))
      .def_static("create_enumeration", py::overload_cast<int32_t>(&TimeZone::createEnumeration),
                  py::arg("raw_offset"));
#endif // !(U_ICU_VERSION_MAJOR_NUM >= 70)

#if (U_ICU_VERSION_MAJOR_NUM >= 70)
  tz.def_static(
      "create_enumeration_for_raw_offset",
      [](int32_t raw_offset) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = TimeZone::createEnumerationForRawOffset(raw_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("raw_offset"));
  tz.def_static(
      "create_enumeration_for_region",
      [](const char *region) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = TimeZone::createEnumerationForRegion(region, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("region"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 70)
  tz.def_static(
        "create_time_zone",
        [](const UnicodeString &id) -> std::variant<BasicTimeZone *, TimeZone *> {
          auto tz = TimeZone::createTimeZone(id);
          auto btz = dynamic_cast<BasicTimeZone *>(tz);
          if (btz) {
            return btz;
          }
          return tz;
        },
        py::arg("id_"))
      .def_static(
          // const char16_t *id -> const UnicodeString &id
          "create_time_zone",
          [](const char16_t *id) -> std::variant<BasicTimeZone *, TimeZone *> {
            auto tz = TimeZone::createTimeZone(id);
            auto btz = dynamic_cast<BasicTimeZone *>(tz);
            if (btz) {
              return btz;
            }
            return tz;
          },
          py::arg("id_"));
  tz.def_static(
      "create_time_zone_id_enumeration",
      [](USystemTimeZoneType zone_type, const char *region, const std::optional<int32_t> &raw_offset) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = TimeZone::createTimeZoneIDEnumeration(
            zone_type, region, raw_offset.has_value() ? &raw_offset.value() : nullptr, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("zone_type"), py::arg("region"), py::arg("raw_offset"));
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  tz.def_static("detect_host_time_zone", []() -> std::variant<BasicTimeZone *, TimeZone *> {
    auto tz = TimeZone::detectHostTimeZone();
    auto btz = dynamic_cast<BasicTimeZone *>(tz);
    if (btz) {
      return btz;
    }
    return tz;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
  tz.def_static(
        "get_canonical_id",
        [](const UnicodeString &id, UnicodeString &canonical_id) {
          UErrorCode error_code = U_ZERO_ERROR;
          UBool is_system_id;
          auto &result = TimeZone::getCanonicalID(id, canonical_id, is_system_id, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return py::make_tuple(result, is_system_id);
        },
        py::arg("id_"), py::arg("canonical_id"))
      .def_static(
          // const char16_t *id -> const UnicodeString &id
          "get_canonical_id",
          [](const char16_t *id, UnicodeString &canonical_id) {
            UErrorCode error_code = U_ZERO_ERROR;
            UBool is_system_id;
            auto &result = TimeZone::getCanonicalID(id, canonical_id, is_system_id, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return py::make_tuple(result, is_system_id);
          },
          py::arg("id_"), py::arg("canonical_id"));
  tz.def("get_display_name", py::overload_cast<const Locale &, UnicodeString &>(&TimeZone::getDisplayName, py::const_),
         py::arg("locale"), py::arg("result"))
      .def(
          // const char *locale -> const Locale &locale
          "get_display_name",
          [](const TimeZone &self, const char *locale, UnicodeString &result) {
            return self.getDisplayName(locale, result);
          },
          py::arg("locale"), py::arg("result"))
      .def("get_display_name",
           py::overload_cast<UBool, TimeZone::EDisplayType, const Locale &, UnicodeString &>(&TimeZone::getDisplayName,
                                                                                             py::const_),
           py::arg("in_daylight"), py::arg("style"), py::arg("locale"), py::arg("result"))
      .def(
          // const char *locale -> const Locale &locale
          "get_display_name",
          [](const TimeZone &self, UBool in_daylight, TimeZone::EDisplayType style, const char *locale,
             UnicodeString &result) { return self.getDisplayName(in_daylight, style, locale, result); },
          py::arg("in_daylight"), py::arg("style"), py::arg("locale"), py::arg("result"))
      .def("get_display_name",
           py::overload_cast<UBool, TimeZone::EDisplayType, UnicodeString &>(&TimeZone::getDisplayName, py::const_),
           py::arg("in_daylight"), py::arg("style"), py::arg("result"))
      .def("get_display_name", py::overload_cast<UnicodeString &>(&TimeZone::getDisplayName, py::const_),
           py::arg("result"));
  tz.def("get_dst_savings", &TimeZone::getDSTSavings);
  tz.def_static("get_equivalent_id", &TimeZone::getEquivalentID, py::arg("id_"), py::arg("index"))
      .def_static(
          // const char16_t *id -> const UnicodeString &id
          "get_equivalent_id", [](const char16_t *id, int32_t index) { return TimeZone::getEquivalentID(id, index); },
          py::arg("id_"), py::arg("index"));
  tz.def_static("get_gmt", &TimeZone::getGMT, py::return_value_policy::reference);
  tz.def("get_id", &TimeZone::getID, py::arg("id_"));
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  tz.def_static(
        "get_id_for_windows_id",
        [](const UnicodeString &winid, const char *region, UnicodeString &id) -> UnicodeString & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = TimeZone::getIDForWindowsID(winid, region, id, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("winid"), py::arg("region"), py::arg("id_"))
      .def_static(
          // const char16_t *winid -> const UnicodeString &winid
          "get_id_for_windows_id",
          [](const char16_t *winid, const char *region, UnicodeString &id) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = TimeZone::getIDForWindowsID(winid, region, id, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("winid"), py::arg("region"), py::arg("id_"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
  tz.def(
      "get_offset",
      [](const TimeZone &self, UDate date, UBool local) {
        UErrorCode error_code = U_ZERO_ERROR;
        int32_t raw_offset, dst_offset;
        self.getOffset(date, local, raw_offset, dst_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(raw_offset, dst_offset);
      },
      py::arg("date"), py::arg("local"));
  tz.def("get_raw_offset", &TimeZone::getRawOffset);
  tz.def_static(
        "get_region",
        [](const UnicodeString &id) {
          UErrorCode error_code = U_ZERO_ERROR;
          std::string result(8, '\0');
          auto length = TimeZone::getRegion(id, result.data(), static_cast<int32_t>(result.size()), error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          result.resize(length);
          return result;
        },
        py::arg("id_"))
      .def_static(
          // const char16_t *id -> const UnicodeString &id
          "get_region",
          [](const char16_t *id) {
            UErrorCode error_code = U_ZERO_ERROR;
            std::string result(8, '\0');
            auto length = TimeZone::getRegion(id, result.data(), static_cast<int32_t>(result.size()), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            result.resize(length);
            return result;
          },
          py::arg("id_"));
  tz.def_static("get_tz_data_version", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = TimeZone::getTZDataVersion(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  tz.def_static("get_unknown", &TimeZone::getUnknown, py::return_value_policy::reference);
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
#if (U_ICU_VERSION_MAJOR_NUM >= 52)
  tz.def_static(
        "get_windows_id",
        [](const UnicodeString &id, UnicodeString &winid) -> UnicodeString & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = TimeZone::getWindowsID(id, winid, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("id_"), py::arg("winid"))
      .def_static(
          // const char16_t *id -> const UnicodeString &id
          "get_windows_id",
          [](const char16_t *id, UnicodeString &winid) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = TimeZone::getWindowsID(id, winid, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("id_"), py::arg("winid"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 52)
  tz.def("has_same_rules", &TimeZone::hasSameRules, py::arg("other"));
  tz.def_static("set_default", &TimeZone::setDefault, py::arg("zone"));
  tz.def("set_id", &TimeZone::setID, py::arg("id_"))
      .def(
          // const char16_t *id -> const UnicodeString &id
          "set_id", [](TimeZone &self, const char16_t *id) { self.setID(id); }, py::arg("id_"));
  tz.def("set_raw_offset", &TimeZone::setRawOffset, py::arg("offset_millis"));
  tz.def("use_daylight_time", &TimeZone::useDaylightTime);

  // icu::BasicTimeZone
  btz.def("__copy__", &BasicTimeZone::clone)
      .def(
          "__deepcopy__", [](const BasicTimeZone &self, py::dict) { return self.clone(); }, py::arg("memo"));
  btz.def("clone", &BasicTimeZone::clone);
  btz.def("count_transition_rules", [](const BasicTimeZone &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.countTransitionRules(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  btz.def("get_next_transition", &BasicTimeZone::getNextTransition, py::arg("base"), py::arg("inclusive"),
          py::arg("result"));
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  btz.def(
      "get_offset_from_local",
      [](const BasicTimeZone &self, UDate date, UTimeZoneLocalOption non_existing_time_opt,
         UTimeZoneLocalOption duplicated_time_opt) {
        int32_t raw_offset = 0, dst_offset = 0;
        UErrorCode error_code = U_ZERO_ERROR;
        self.getOffsetFromLocal(date, non_existing_time_opt, duplicated_time_opt, raw_offset, dst_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(raw_offset, dst_offset);
      },
      py::arg("date"), py::arg("non_existing_time_opt"), py::arg("duplicated_time_opt"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
  btz.def("get_previous_transition", &BasicTimeZone::getPreviousTransition, py::arg("base"), py::arg("inclusive"),
          py::arg("result"));
  btz.def(
      "get_simple_rules_near",
      [](const BasicTimeZone &self, UDate date) {
        UErrorCode error_code = U_ZERO_ERROR;
        InitialTimeZoneRule *initial;
        AnnualTimeZoneRule *trsrules[2]{0};
        self.getSimpleRulesNear(date, initial, trsrules[0], trsrules[1], error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(initial, trsrules[0], trsrules[1]);
      },
      py::return_value_policy::reference, py::arg("date"));
  btz.def(
      "get_time_zone_rules",
      [](const BasicTimeZone &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        const InitialTimeZoneRule *initial;
        std::vector<const TimeZoneRule *> trsrules(self.countTransitionRules(error_code));
        int32_t trscount = static_cast<int32_t>(trsrules.size());
        self.getTimeZoneRules(initial, trsrules.data(), trscount, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(initial, trsrules);
      },
      py::return_value_policy::reference);
  btz.def(
      "has_equivalent_transitions",
      [](const BasicTimeZone &self, const BasicTimeZone &tz, UDate start, UDate end, UBool ignore_dst_amount) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.hasEquivalentTransitions(tz, start, end, ignore_dst_amount, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("tz"), py::arg("start"), py::arg("end"), py::arg("ignore_dst_amount"));

  // icu::RuleBasedTimeZone
  rbtz.def(py::init([](const UnicodeString &id, InitialTimeZoneRule *initial_rule) {
             return std::make_unique<RuleBasedTimeZone>(id, initial_rule ? initial_rule->clone() : nullptr);
           }),
           py::arg("id_"), py::arg("initial_rule"))
      .def(py::init([](const char *id, InitialTimeZoneRule *initial_rule) {
             return std::make_unique<RuleBasedTimeZone>(id, initial_rule ? initial_rule->clone() : nullptr);
           }),
           py::arg("id_"), py::arg("initial_rule"))
      .def(py::init<const RuleBasedTimeZone &>(), py::arg("source"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  rbtz.def("__copy__", &RuleBasedTimeZone::clone)
      .def(
          "__deepcopy__", [](const RuleBasedTimeZone &self, py::dict) { return self.clone(); }, py::arg("memo"));
  rbtz.def(
      "add_transition_rule",
      [](RuleBasedTimeZone &self, TimeZoneRule *rule) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.addTransitionRule(rule ? rule->clone() : nullptr, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("rule"));
  rbtz.def("clone", &RuleBasedTimeZone::clone);
  rbtz.def("complete", [](RuleBasedTimeZone &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    self.complete(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
  });
  rbtz.def("count_transition_rules", [](const RuleBasedTimeZone &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.countTransitionRules(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  rbtz.def("get_next_transition", &RuleBasedTimeZone::getNextTransition, py::arg("base"), py::arg("inclusive"),
           py::arg("result"));
  rbtz.def(
          "get_offset",
          [](const RuleBasedTimeZone &self, UDate date, UBool local) {
            UErrorCode error_code = U_ZERO_ERROR;
            int32_t raw_offset, dst_offset;
            self.getOffset(date, local, raw_offset, dst_offset, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return py::make_tuple(raw_offset, dst_offset);
          },
          py::arg("date"), py::arg("local"))
      .def(
          "get_offset",
          [](const RuleBasedTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t millis, int32_t month_length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, millis, month_length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"), py::arg("millis"),
          py::arg("month_length"))
      .def(
          "get_offset",
          [](const RuleBasedTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t millis) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, millis, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"), py::arg("millis"));
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  rbtz.def(
      "get_offset_from_local",
      [](const RuleBasedTimeZone &self, UDate date, UTimeZoneLocalOption non_existing_time_opt,
         UTimeZoneLocalOption duplicated_time_opt) {
        int32_t raw_offset = 0, dst_offset = 0;
        UErrorCode error_code = U_ZERO_ERROR;
        self.getOffsetFromLocal(date, non_existing_time_opt, duplicated_time_opt, raw_offset, dst_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(raw_offset, dst_offset);
      },
      py::arg("date"), py::arg("non_existing_time_opt"), py::arg("duplicated_time_opt"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
  rbtz.def("get_previous_transition", &RuleBasedTimeZone::getPreviousTransition, py::arg("base"), py::arg("inclusive"),
           py::arg("result"));
  rbtz.def("get_raw_offset", &RuleBasedTimeZone::getRawOffset);
  rbtz.def(
      "get_time_zone_rules",
      [](const RuleBasedTimeZone &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        const InitialTimeZoneRule *initial;
        std::vector<const TimeZoneRule *> trsrules(self.countTransitionRules(error_code));
        int32_t trscount = static_cast<int32_t>(trsrules.size());
        self.getTimeZoneRules(initial, trsrules.data(), trscount, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(initial, trsrules);
      },
      py::return_value_policy::reference);
  rbtz.def("has_same_rules", &RuleBasedTimeZone::hasSameRules, py::arg("other"));
  rbtz.def("set_raw_offset", &RuleBasedTimeZone::setRawOffset, py::arg("offset_millis"));
  rbtz.def("use_daylight_time", &RuleBasedTimeZone::useDaylightTime);

  // icu::SimpleTimeZone
  py::enum_<SimpleTimeZone::TimeMode>(
      stz, "TimeMode", py::arithmetic(),
      "*TimeMode* is used, together with a millisecond offset after midnight, to specify a rule transition time.\n\n"
      "Most rules transition at a local wall time, that is, according to the current time in effect, either standard, "
      "or DST. However, some rules transition at local standard time, and some at a specific UTC time. Although it "
      "might seem that all times could be converted to wall time, thus eliminating the need for this parameter, this "
      "is not the case.")
      .value("WALL_TIME", SimpleTimeZone::TimeMode::WALL_TIME)
      .value("STANDARD_TIME", SimpleTimeZone::TimeMode::STANDARD_TIME)
      .value("UTC_TIME", SimpleTimeZone::TimeMode::UTC_TIME)
      .export_values();

  stz.def(py::init<const SimpleTimeZone &>(), py::arg("source"))
      .def(py::init<int32_t, const UnicodeString &>(), py::arg("raw_offset_gmt"), py::arg("id_"))
      .def(py::init<int32_t, const char *>(), py::arg("raw_offset_gmt"), py::arg("id_"))
      .def(py::init([](int32_t raw_offset_gmt, const UnicodeString &id, int8_t savings_start_month,
                       int8_t savings_start_day_of_week_in_month, int8_t savings_start_day_of_week,
                       int32_t savings_start_time, int8_t savings_end_month, int8_t savings_end_day_of_week_in_month,
                       int8_t savings_end_day_of_week, int32_t savings_end_time) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<SimpleTimeZone>(
                 raw_offset_gmt, id, savings_start_month, savings_start_day_of_week_in_month, savings_start_day_of_week,
                 savings_start_time, savings_end_month, savings_end_day_of_week_in_month, savings_end_day_of_week,
                 savings_end_time, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("raw_offset_gmt"), py::arg("id_"), py::arg("savings_start_month"),
           py::arg("savings_start_day_of_week_in_month"), py::arg("savings_start_day_of_week"),
           py::arg("savings_start_time"), py::arg("savings_end_month"), py::arg("savings_end_day_of_week_in_month"),
           py::arg("savings_end_day_of_week"), py::arg("savings_end_time"))
      .def(py::init([](int32_t raw_offset_gmt, const char *id, int8_t savings_start_month,
                       int8_t savings_start_day_of_week_in_month, int8_t savings_start_day_of_week,
                       int32_t savings_start_time, int8_t savings_end_month, int8_t savings_end_day_of_week_in_month,
                       int8_t savings_end_day_of_week, int32_t savings_end_time) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<SimpleTimeZone>(
                 raw_offset_gmt, id, savings_start_month, savings_start_day_of_week_in_month, savings_start_day_of_week,
                 savings_start_time, savings_end_month, savings_end_day_of_week_in_month, savings_end_day_of_week,
                 savings_end_time, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("raw_offset_gmt"), py::arg("id_"), py::arg("savings_start_month"),
           py::arg("savings_start_day_of_week_in_month"), py::arg("savings_start_day_of_week"),
           py::arg("savings_start_time"), py::arg("savings_end_month"), py::arg("savings_end_day_of_week_in_month"),
           py::arg("savings_end_day_of_week"), py::arg("savings_end_time"))
      .def(py::init([](int32_t raw_offset_gmt, const UnicodeString &id, int8_t savings_start_month,
                       int8_t savings_start_day_of_week_in_month, int8_t savings_start_day_of_week,
                       int32_t savings_start_time, int8_t savings_end_month, int8_t savings_end_day_of_week_in_month,
                       int8_t savings_end_day_of_week, int32_t savings_end_time, int32_t savings_dst) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<SimpleTimeZone>(
                 raw_offset_gmt, id, savings_start_month, savings_start_day_of_week_in_month, savings_start_day_of_week,
                 savings_start_time, savings_end_month, savings_end_day_of_week_in_month, savings_end_day_of_week,
                 savings_end_time, savings_dst, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("raw_offset_gmt"), py::arg("id_"), py::arg("savings_start_month"),
           py::arg("savings_start_day_of_week_in_month"), py::arg("savings_start_day_of_week"),
           py::arg("savings_start_time"), py::arg("savings_end_month"), py::arg("savings_end_day_of_week_in_month"),
           py::arg("savings_end_day_of_week"), py::arg("savings_end_time"), py::arg("savings_dst"))
      .def(py::init([](int32_t raw_offset_gmt, const char *id, int8_t savings_start_month,
                       int8_t savings_start_day_of_week_in_month, int8_t savings_start_day_of_week,
                       int32_t savings_start_time, int8_t savings_end_month, int8_t savings_end_day_of_week_in_month,
                       int8_t savings_end_day_of_week, int32_t savings_end_time, int32_t savings_dst) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<SimpleTimeZone>(
                 raw_offset_gmt, id, savings_start_month, savings_start_day_of_week_in_month, savings_start_day_of_week,
                 savings_start_time, savings_end_month, savings_end_day_of_week_in_month, savings_end_day_of_week,
                 savings_end_time, savings_dst, error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("raw_offset_gmt"), py::arg("id_"), py::arg("savings_start_month"),
           py::arg("savings_start_day_of_week_in_month"), py::arg("savings_start_day_of_week"),
           py::arg("savings_start_time"), py::arg("savings_end_month"), py::arg("savings_end_day_of_week_in_month"),
           py::arg("savings_end_day_of_week"), py::arg("savings_end_time"), py::arg("savings_dst"))
      .def(
          py::init([](int32_t raw_offset_gmt, const UnicodeString &id, int8_t savings_start_month,
                      int8_t savings_start_day_of_week_in_month, int8_t savings_start_day_of_week,
                      int32_t savings_start_time, SimpleTimeZone::TimeMode savings_start_time_mode,
                      int8_t savings_end_month, int8_t savings_end_day_of_week_in_month, int8_t savings_end_day_of_week,
                      int32_t savings_end_time, SimpleTimeZone::TimeMode savings_end_time_mode, int32_t savings_dst) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleTimeZone>(
                raw_offset_gmt, id, savings_start_month, savings_start_day_of_week_in_month, savings_start_day_of_week,
                savings_start_time, savings_start_time_mode, savings_end_month, savings_end_day_of_week_in_month,
                savings_end_day_of_week, savings_end_time, savings_end_time_mode, savings_dst, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("raw_offset_gmt"), py::arg("id_"), py::arg("savings_start_month"),
          py::arg("savings_start_day_of_week_in_month"), py::arg("savings_start_day_of_week"),
          py::arg("savings_start_time"), py::arg("savings_start_time_mode"), py::arg("savings_end_month"),
          py::arg("savings_end_day_of_week_in_month"), py::arg("savings_end_day_of_week"), py::arg("savings_end_time"),
          py::arg("savings_end_time_mode"), py::arg("savings_dst"))
      .def(
          py::init([](int32_t raw_offset_gmt, const char *id, int8_t savings_start_month,
                      int8_t savings_start_day_of_week_in_month, int8_t savings_start_day_of_week,
                      int32_t savings_start_time, SimpleTimeZone::TimeMode savings_start_time_mode,
                      int8_t savings_end_month, int8_t savings_end_day_of_week_in_month, int8_t savings_end_day_of_week,
                      int32_t savings_end_time, SimpleTimeZone::TimeMode savings_end_time_mode, int32_t savings_dst) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<SimpleTimeZone>(
                raw_offset_gmt, id, savings_start_month, savings_start_day_of_week_in_month, savings_start_day_of_week,
                savings_start_time, savings_start_time_mode, savings_end_month, savings_end_day_of_week_in_month,
                savings_end_day_of_week, savings_end_time, savings_end_time_mode, savings_dst, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("raw_offset_gmt"), py::arg("id_"), py::arg("savings_start_month"),
          py::arg("savings_start_day_of_week_in_month"), py::arg("savings_start_day_of_week"),
          py::arg("savings_start_time"), py::arg("savings_start_time_mode"), py::arg("savings_end_month"),
          py::arg("savings_end_day_of_week_in_month"), py::arg("savings_end_day_of_week"), py::arg("savings_end_time"),
          py::arg("savings_end_time_mode"), py::arg("savings_dst"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  stz.def("__copy__", &SimpleTimeZone::clone)
      .def(
          "__deepcopy__", [](const SimpleTimeZone &self, py::dict) { return self.clone(); }, py::arg("memo"));
  stz.def("clone", &SimpleTimeZone::clone);
  stz.def("count_transition_rules", [](const SimpleTimeZone &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.countTransitionRules(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  stz.def("get_dst_savings", &SimpleTimeZone::getDSTSavings);
  stz.def("get_next_transition", &SimpleTimeZone::getNextTransition, py::arg("base"), py::arg("inclusive"),
          py::arg("result"));
  stz.def(
         "get_offset",
         [](const SimpleTimeZone &self, UDate date, UBool local) {
           UErrorCode error_code = U_ZERO_ERROR;
           int32_t raw_offset, dst_offset;
           self.getOffset(date, local, raw_offset, dst_offset, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return py::make_tuple(raw_offset, dst_offset);
         },
         py::arg("date"), py::arg("local"))
      .def(
          "get_offset",
          [](const SimpleTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t millis) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, millis, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"), py::arg("millis"))
      .def(
          "get_offset",
          [](const SimpleTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t milliseconds, int32_t month_length, int32_t prev_month_length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, milliseconds, month_length,
                                         prev_month_length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"),
          py::arg("milliseconds"), py::arg("month_length"), py::arg("prev_month_length"))
      .def(
          "get_offset",
          [](const SimpleTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t milliseconds, int32_t month_length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, milliseconds, month_length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"),
          py::arg("milliseconds"), py::arg("month_length"));
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  stz.def(
      "get_offset_from_local",
      [](const SimpleTimeZone &self, UDate date, UTimeZoneLocalOption non_existing_time_opt,
         UTimeZoneLocalOption duplicated_time_opt) {
        int32_t raw_offset = 0, dst_offset = 0;
        UErrorCode error_code = U_ZERO_ERROR;
        self.getOffsetFromLocal(date, non_existing_time_opt, duplicated_time_opt, raw_offset, dst_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(raw_offset, dst_offset);
      },
      py::arg("date"), py::arg("non_existing_time_opt"), py::arg("duplicated_time_opt"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
  stz.def("get_previous_transition", &SimpleTimeZone::getPreviousTransition, py::arg("base"), py::arg("inclusive"),
          py::arg("result"));
  stz.def("get_raw_offset", &SimpleTimeZone::getRawOffset);
  stz.def(
      "get_time_zone_rules",
      [](const SimpleTimeZone &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        const InitialTimeZoneRule *initial;
        std::vector<const TimeZoneRule *> trsrules(self.countTransitionRules(error_code));
        int32_t trscount = static_cast<int32_t>(trsrules.size());
        self.getTimeZoneRules(initial, trsrules.data(), trscount, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(initial, trsrules);
      },
      py::return_value_policy::reference);
  stz.def("has_same_rules", &SimpleTimeZone::hasSameRules, py::arg("other"));
  stz.def(
      "set_dst_savings",
      [](SimpleTimeZone &self, int32_t millis_saved_during_dst) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setDSTSavings(millis_saved_during_dst, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("millis_saved_during_dst"));
  stz.def(
         // [1] setEndRule(int32_t month, int32_t dayOfMonth, int32_t dayOfWeek, int32_t time, TimeMode mode,
         //                UBool after, UErrorCode &status)
         "set_end_rule",
         [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t day_of_week, int32_t time,
            SimpleTimeZone::TimeMode mode, UBool after) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setEndRule(month, day_of_month, day_of_week, time, mode, after, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("month"), py::arg("day_of_month"), py::arg("day_of_week"), py::arg("time"), py::arg("mode"),
         py::arg("after"))
      .def(
          // [5] setEndRule(int32_t month, int32_t dayOfWeekInMonth, int32_t dayOfWeek, int32_t time, TimeMode mode,
          //                UErrorCode &status)
          "set_end_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_week_in_month, int32_t day_of_week, int32_t time,
             SimpleTimeZone::TimeMode mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setEndRule(month, day_of_week_in_month, day_of_week, time, mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_week_in_month"), py::arg("day_of_week"), py::arg("time"), py::arg("mode"))
      .def(
          // [2] setEndRule(int32_t month, int32_t dayOfMonth, int32_t dayOfWeek, int32_t time, UBool after,
          //                UErrorCode &status)
          "set_end_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t day_of_week, int32_t time,
             UBool after) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setEndRule(month, day_of_month, day_of_week, time, after, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_month"), py::arg("day_of_week"), py::arg("time"), py::arg("after"))
      .def(
          // [3] setEndRule(int32_t month, int32_t dayOfMonth, int32_t time, TimeMode mode, UErrorCode &status)
          "set_end_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t time, SimpleTimeZone::TimeMode mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setEndRule(month, day_of_month, time, mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_month"), py::arg("time"), py::arg("mode"))
      .def(
          // [4] setEndRule(int32_t month, int32_t dayOfMonth, int32_t time, UErrorCode &status)
          "set_end_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t time) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setEndRule(month, day_of_month, time, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_month"), py::arg("time"))
      .def(
          // [6] setEndRule(int32_t month, int32_t dayOfWeekInMonth, int32_t dayOfWeek, int32_t time,
          //                UErrorCode &status)
          "set_end_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_week_in_month, int32_t day_of_week, int32_t time) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setEndRule(month, day_of_week_in_month, day_of_week, time, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_week_in_month"), py::arg("day_of_week"), py::arg("time"));
  stz.def("set_raw_offset", &SimpleTimeZone::setRawOffset, py::arg("offset_millis"));
  stz.def(
         // [1] setStartRule(int32_t month, int32_t dayOfMonth, int32_t dayOfWeek, int32_t time, TimeMode mode,
         //                  UBool after, UErrorCode &status)
         "set_start_rule",
         [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t day_of_week, int32_t time,
            SimpleTimeZone::TimeMode mode, UBool after) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.setStartRule(month, day_of_month, day_of_week, time, mode, after, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("month"), py::arg("day_of_month"), py::arg("day_of_week"), py::arg("time"), py::arg("mode"),
         py::arg("after"))
      .def(
          // [5] setStartRule(int32_t month, int32_t dayOfWeekInMonth, int32_t dayOfWeek, int32_t time, TimeMode mode,
          //                  UErrorCode &status)
          "set_start_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_week_in_month, int32_t day_of_week, int32_t time,
             SimpleTimeZone::TimeMode mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setStartRule(month, day_of_week_in_month, day_of_week, time, mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_week_in_month"), py::arg("day_of_week"), py::arg("time"), py::arg("mode"))
      .def(
          // [2] setStartRule(int32_t month, int32_t dayOfMonth, int32_t dayOfWeek, int32_t time, UBool after,
          //                  UErrorCode &status)
          "set_start_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t day_of_week, int32_t time,
             UBool after) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setStartRule(month, day_of_month, day_of_week, time, after, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_month"), py::arg("day_of_week"), py::arg("time"), py::arg("after"))
      .def(
          // [3] setStartRule(int32_t month, int32_t dayOfMonth, int32_t time, TimeMode mode, UErrorCode &status)
          "set_start_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t time, SimpleTimeZone::TimeMode mode) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setStartRule(month, day_of_month, time, mode, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_month"), py::arg("time"), py::arg("mode"))
      .def(
          // [4] setStartRule(int32_t month, int32_t dayOfMonth, int32_t time, UErrorCode &status)
          "set_start_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_month, int32_t time) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setStartRule(month, day_of_month, time, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_month"), py::arg("time"))
      .def(
          // [6] setStartRule(int32_t month, int32_t dayOfWeekInMonth, int32_t dayOfWeek, int32_t time,
          //                  UErrorCode &status)
          "set_start_rule",
          [](SimpleTimeZone &self, int32_t month, int32_t day_of_week_in_month, int32_t day_of_week, int32_t time) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.setStartRule(month, day_of_week_in_month, day_of_week, time, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("month"), py::arg("day_of_week_in_month"), py::arg("day_of_week"), py::arg("time"));
  stz.def("set_start_year", &SimpleTimeZone::setStartYear, py::arg("year"));
  stz.def("use_daylight_time", &SimpleTimeZone::useDaylightTime);

  // icu::VTimeZone
  vtz.def(py::init<const VTimeZone &>(), py::arg("source"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  vtz.def("__copy__", &VTimeZone::clone)
      .def(
          "__deepcopy__", [](const VTimeZone &self, py::dict) { return self.clone(); }, py::arg("memo"));
  vtz.def("clone", &VTimeZone::clone);
  vtz.def("count_transition_rules", [](const VTimeZone &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.countTransitionRules(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  vtz.def_static(
         "create_vtime_zone",
         [](const UnicodeString &vtzdata) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = VTimeZone::createVTimeZone(vtzdata, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         },
         py::arg("vtzdata"))
      .def_static(
          "create_vtime_zone",
          [](const char *vtzdata) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = VTimeZone::createVTimeZone(vtzdata, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("vtzdata"));
  vtz.def_static("create_vtime_zone_by_id", &VTimeZone::createVTimeZoneByID, py::arg("id_"))
      .def_static(
          "create_vtime_zone_by_id", [](const char *id) { return VTimeZone::createVTimeZoneByID(id); }, py::arg("id_"));
  vtz.def_static(
      "create_vtime_zone_from_basic_time_zone",
      [](const BasicTimeZone &basic_tz) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = VTimeZone::createVTimeZoneFromBasicTimeZone(basic_tz, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("basic_tz"));
  vtz.def("get_last_modified", [](const VTimeZone &self) {
    UDate last_modified = 0;
    auto result = self.getLastModified(last_modified);
    return py::make_tuple(result, last_modified);
  });
  vtz.def("get_next_transition", &VTimeZone::getNextTransition, py::arg("base"), py::arg("inclusive"),
          py::arg("result"));
  vtz.def(
         "get_offset",
         [](const VTimeZone &self, UDate date, UBool local) {
           UErrorCode error_code = U_ZERO_ERROR;
           int32_t raw_offset, dst_offset;
           self.getOffset(date, local, raw_offset, dst_offset, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return py::make_tuple(raw_offset, dst_offset);
         },
         py::arg("date"), py::arg("local"))
      .def(
          "get_offset",
          [](const VTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t millis, int32_t month_length) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, millis, month_length, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"), py::arg("millis"),
          py::arg("month_length"))
      .def(
          "get_offset",
          [](const VTimeZone &self, uint8_t era, int32_t year, int32_t month, int32_t day, uint8_t day_of_week,
             int32_t millis) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getOffset(era, year, month, day, day_of_week, millis, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("era"), py::arg("year"), py::arg("month"), py::arg("day"), py::arg("day_of_week"), py::arg("millis"));
#if (U_ICU_VERSION_MAJOR_NUM >= 69)
  vtz.def(
      "get_offset_from_local",
      [](const VTimeZone &self, UDate date, UTimeZoneLocalOption non_existing_time_opt,
         UTimeZoneLocalOption duplicated_time_opt) {
        int32_t raw_offset = 0, dst_offset = 0;
        UErrorCode error_code = U_ZERO_ERROR;
        self.getOffsetFromLocal(date, non_existing_time_opt, duplicated_time_opt, raw_offset, dst_offset, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(raw_offset, dst_offset);
      },
      py::arg("date"), py::arg("non_existing_time_opt"), py::arg("duplicated_time_opt"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 69)
  vtz.def("get_previous_transition", &VTimeZone::getPreviousTransition, py::arg("base"), py::arg("inclusive"),
          py::arg("result"));
  vtz.def("get_raw_offset", &VTimeZone::getRawOffset);
  vtz.def(
      "get_time_zone_rules",
      [](const VTimeZone &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        const InitialTimeZoneRule *initial;
        std::vector<const TimeZoneRule *> trsrules(self.countTransitionRules(error_code));
        int32_t trscount = static_cast<int32_t>(trsrules.size());
        self.getTimeZoneRules(initial, trsrules.data(), trscount, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::make_tuple(initial, trsrules);
      },
      py::return_value_policy::reference);
  vtz.def("get_tzurl", &VTimeZone::getTZURL, py::arg("url"));
  vtz.def("has_same_rules", &VTimeZone::hasSameRules, py::arg("other"));
  vtz.def("set_last_modified", &VTimeZone::setLastModified, py::arg("last_modified"));
  vtz.def("set_raw_offset", &VTimeZone::setRawOffset, py::arg("offset_millis"));
  vtz.def("set_tzurl", &VTimeZone::setTZURL, py::arg("url"))
      .def(
          "set_tzurl", [](VTimeZone &self, const char *url) { self.setTZURL(url); }, py::arg("url"));
  vtz.def("use_daylight_time", &VTimeZone::useDaylightTime);
  vtz.def(
         "write",
         [](const VTimeZone &self, UDate start, UnicodeString &result) {
           UErrorCode error_code = U_ZERO_ERROR;
           self.write(start, result, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
         },
         py::arg("start"), py::arg("result"))
      .def(
          "write",
          [](const VTimeZone &self, UnicodeString &result) {
            UErrorCode error_code = U_ZERO_ERROR;
            self.write(result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
          },
          py::arg("result"));
  vtz.def(
      "write_simple",
      [](const VTimeZone &self, UDate time, UnicodeString &result) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.writeSimple(time, result, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("time"), py::arg("result"));
}

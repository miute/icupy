#include "main.hpp"
#include <unicode/utmscale.h>

void init_utmscale(py::module &m) {
  py::enum_<UDateTimeScale>(
      m, "UDateTimeScale", py::arithmetic(),
      "*UDateTimeScale* values are used to specify the time scale used for conversion into or out "
      "if the universal time scale.")
      .value("UDTS_JAVA_TIME", UDTS_JAVA_TIME,
             "Used in the JDK. Data is a Java *long* (*int64_t*). Value is milliseconds since January 1, 1970.")
      .value("UDTS_UNIX_TIME", UDTS_UNIX_TIME,
             "Used on Unix systems. Data is *int32_t* or *int64_t*. Value is seconds since January 1, 1970.")
      .value("UDTS_ICU4C_TIME", UDTS_ICU4C_TIME,
             "Used in IUC4C. Data is a *double*. Value is milliseconds since January 1, 1970.")
      .value("UDTS_WINDOWS_FILE_TIME", UDTS_WINDOWS_FILE_TIME,
             "Used in Windows for file times. Data is an *int64_t*. Value is ticks (1 tick == 100 nanoseconds) since "
             "January 1, 1601.")
      .value("UDTS_DOTNET_DATE_TIME", UDTS_DOTNET_DATE_TIME,
             "Used in the .NET framework's *System.DateTime* structure. Data is an *int64_t*. Value is ticks "
             "(1 tick == 100 nanoseconds) since January 1, 0001.")
      .value("UDTS_MAC_OLD_TIME", UDTS_MAC_OLD_TIME,
             "Used in older Macintosh systems. Data is *int32_t* or *int64_t*. Value is seconds since January 1, 1904.")
      .value("UDTS_MAC_TIME", UDTS_MAC_TIME,
             "Used in newer Macintosh systems. Data is a *double*. Value is seconds since January 1, 2001.")
      .value("UDTS_EXCEL_TIME", UDTS_EXCEL_TIME,
             "Used in Excel. Data is an *?unknown?*. Value is days since December 31, 1899.")
      .value("UDTS_DB2_TIME", UDTS_DB2_TIME,
             "Used in DB2. Data is an *?unknown?*. Value is days since December 31, 1899.")
      .value("UDTS_UNIX_MICROSECONDS_TIME", UDTS_UNIX_MICROSECONDS_TIME,
             "Data is a *long*. Value is microseconds since January 1, 1970. Similar to Unix time (linear value from "
             "1970) and struct timeval (microseconds resolution).")
#ifndef U_HIDE_DEPRECATED_API
      .value("UDTS_MAX_SCALE", UDTS_MAX_SCALE,
             "**Deprecated:** ICU 59 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  py::enum_<UTimeScaleValue>(
      m, "UTimeScaleValue", py::arithmetic(),
      "*UTimeScaleValue* values are used to specify the time scale values to *utmscale_get_time_scale_value*.")
      .value("UTSV_UNITS_VALUE", UTSV_UNITS_VALUE, "The constant used to select the units vale for a time scale.")
      .value("UTSV_EPOCH_OFFSET_VALUE", UTSV_EPOCH_OFFSET_VALUE,
             "The constant used to select the epoch offset value for a time scale.")
      .value("UTSV_FROM_MIN_VALUE", UTSV_FROM_MIN_VALUE,
             "The constant used to select the minimum from value for a time scale.")
      .value("UTSV_FROM_MAX_VALUE", UTSV_FROM_MAX_VALUE,
             "The constant used to select the maximum from value for a time scale.")
      .value("UTSV_TO_MIN_VALUE", UTSV_TO_MIN_VALUE,
             "The constant used to select the minimum to value for a time scale.")
      .value("UTSV_TO_MAX_VALUE", UTSV_TO_MAX_VALUE,
             "The constant used to select the maximum to value for a time scale.")
  // .value("UTSV_EPOCH_OFFSET_PLUS_1_VALUE", UTSV_EPOCH_OFFSET_PLUS_1_VALUE)
  // .value("UTSV_EPOCH_OFFSET_MINUS_1_VALUE", UTSV_EPOCH_OFFSET_MINUS_1_VALUE)
  // .value("UTSV_UNITS_ROUND_VALUE", UTSV_UNITS_ROUND_VALUE)
  // .value("UTSV_MIN_ROUND_VALUE", UTSV_MIN_ROUND_VALUE)
  // .value("UTSV_MAX_ROUND_VALUE", UTSV_MAX_ROUND_VALUE)
#ifndef U_HIDE_DEPRECATED_API
      .value("UTSV_MAX_SCALE_VALUE", UTSV_MAX_SCALE_VALUE,
             "**Deprecated:** ICU 59 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  m.def(
      "utmscale_from_int64",
      [](int64_t other_time, UDateTimeScale time_scale) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = utmscale_fromInt64(other_time, time_scale, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("other_time"), py::arg("time_scale"));
  m.def(
      "utmscale_get_time_scale_value",
      [](UDateTimeScale time_scale, UTimeScaleValue value) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = utmscale_getTimeScaleValue(time_scale, value, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("time_scale"), py::arg("value"));
  m.def(
      "utmscale_to_int64",
      [](int64_t universal_time, UDateTimeScale time_scale) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = utmscale_toInt64(universal_time, time_scale, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("universal_time"), py::arg("time_scale"));
}

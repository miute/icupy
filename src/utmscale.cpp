#include "main.hpp"
#include <unicode/utmscale.h>

using namespace icu;

void init_utmscale(py::module &m) {
  //
  // enum UDateTimeScale
  //
  py::enum_<UDateTimeScale>(m, "UDateTimeScale", py::arithmetic(), R"doc(
UDateTimeScale values are used to specify the time scale used for conversion
into or out if the universal time scale.
)doc")
      .value("UDTS_JAVA_TIME", UDTS_JAVA_TIME, R"doc(
             Used in the JDK. Data is a Java ``long`` (``int64_t``).
             Value is milliseconds since January 1, 1970.
             )doc")
      .value("UDTS_UNIX_TIME", UDTS_UNIX_TIME, R"doc(
             Used on Unix systems. Data is ``int32_t`` or ``int64_t``.
             Value is seconds since January 1, 1970.
             )doc")
      .value("UDTS_ICU4C_TIME", UDTS_ICU4C_TIME, R"doc(
             Used in IUC4C. Data is a ``double``.
             Value is milliseconds since January 1, 1970.
             )doc")
      .value("UDTS_WINDOWS_FILE_TIME", UDTS_WINDOWS_FILE_TIME, R"doc(
             Used in Windows for file times. Data is an ``int64_t``.
             Value is ticks (1 tick == 100 nanoseconds) since January 1, 1601.
             )doc")
      .value("UDTS_DOTNET_DATE_TIME", UDTS_DOTNET_DATE_TIME, R"doc(
             Used in the .NET framework's ``System.DateTime`` structure.
             Data is an ``int64_t``.
             Value is ticks (1 tick == 100 nanoseconds) since January 1, 0001.
             )doc")
      .value("UDTS_MAC_OLD_TIME", UDTS_MAC_OLD_TIME, R"doc(
             Used in older Macintosh systems.
             Data is ``int32_t`` or ``int64_t``.
             Value is seconds since January 1, 1904.
             )doc")
      .value("UDTS_MAC_TIME", UDTS_MAC_TIME, R"doc(
             Used in newer Macintosh systems. Data is a ``double``.
             Value is seconds since January 1, 2001.
             )doc")
      .value("UDTS_EXCEL_TIME", UDTS_EXCEL_TIME, R"doc(
             Used in Excel. Data is an ``?unknown?``.
             Value is days since December 31, 1899.
             )doc")
      .value("UDTS_DB2_TIME", UDTS_DB2_TIME, R"doc(
             Used in DB2. Data is an ``?unknown?``.
             Value is days since December 31, 1899.
             )doc")
      .value("UDTS_UNIX_MICROSECONDS_TIME", UDTS_UNIX_MICROSECONDS_TIME, R"doc(
             Data is a ``long``. Value is microseconds since January 1, 1970.
             Similar to Unix time (linear value from 1970) and struct timeval
             (microseconds resolution).
             )doc")
      .value("UDTS_MAX_SCALE", UDTS_MAX_SCALE, R"doc(
             Deprecated: ICU 59 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // enum UTimeScaleValue
  //
  py::enum_<UTimeScaleValue>(m, "UTimeScaleValue", py::arithmetic(), R"doc(
UTimeScaleValue values are used to specify the time scale values to
:func:`utmscale_get_time_scale_value`.
)doc")
      .value("UTSV_UNITS_VALUE", UTSV_UNITS_VALUE, R"doc(
             The constant used to select the units vale for a time scale.
             )doc")
      .value("UTSV_EPOCH_OFFSET_VALUE", UTSV_EPOCH_OFFSET_VALUE, R"doc(
             The constant used to select the epoch offset value for a time
             scale.
             )doc")
      .value("UTSV_FROM_MIN_VALUE", UTSV_FROM_MIN_VALUE, R"doc(
             The constant used to select the minimum from value for a time
             scale.
             )doc")
      .value("UTSV_FROM_MAX_VALUE", UTSV_FROM_MAX_VALUE, R"doc(
             The constant used to select the maximum from value for a time
             scale.
             )doc")
      .value("UTSV_TO_MIN_VALUE", UTSV_TO_MIN_VALUE, R"doc(
             The constant used to select the minimum to value for a time scale.
             )doc")
      .value("UTSV_TO_MAX_VALUE", UTSV_TO_MAX_VALUE, R"doc(
             The constant used to select the maximum to value for a time scale.
             )doc")
      .value("UTSV_MAX_SCALE_VALUE", UTSV_MAX_SCALE_VALUE, R"doc(
             Deprecated: ICU 59 The numeric value may change over time,
             see ICU ticket #12420.
             )doc")
      .export_values();

  //
  // Functions
  //
  m.def(
      "utmscale_from_int64",
      [](int64_t other_time, UDateTimeScale time_scale) {
        ErrorCode error_code;
        auto result = utmscale_fromInt64(other_time, time_scale, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("other_time"), py::arg("time_scale"));

  m.def(
      "utmscale_get_time_scale_value",
      [](UDateTimeScale time_scale, UTimeScaleValue value) {
        ErrorCode error_code;
        auto result = utmscale_getTimeScaleValue(time_scale, value, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("time_scale"), py::arg("value"));

  m.def(
      "utmscale_to_int64",
      [](int64_t universal_time, UDateTimeScale time_scale) {
        ErrorCode error_code;
        auto result = utmscale_toInt64(universal_time, time_scale, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("universal_time"), py::arg("time_scale"));
}

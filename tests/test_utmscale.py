from icupy.icu import (
    UDateTimeScale, UTimeScaleValue,
    utmscale_from_int64, utmscale_get_time_scale_value, utmscale_to_int64,
)


def test_api():
    # int64_t utmscale_getTimeScaleValue(
    #       UDateTimeScale timeScale,
    #       UTimeScaleValue value,
    #       UErrorCode *status
    # )
    from_min = utmscale_get_time_scale_value(
        UDateTimeScale.UDTS_JAVA_TIME, UTimeScaleValue.UTSV_FROM_MIN_VALUE)
    assert isinstance(from_min, int)

    # int64_t utmscale_fromInt64(
    #       int64_t otherTime,
    #       UDateTimeScale timeScale,
    #       UErrorCode *status
    # )
    universal_time = utmscale_from_int64(
        from_min, UDateTimeScale.UDTS_JAVA_TIME)
    assert isinstance(universal_time, int)

    # int64_t utmscale_toInt64(
    #       int64_t universalTime,
    #       UDateTimeScale timeScale,
    #       UErrorCode *status
    # )
    dt = utmscale_to_int64(universal_time, UDateTimeScale.UDTS_JAVA_TIME)
    assert isinstance(dt, int)
    assert dt == from_min

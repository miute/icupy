from icupy import icu


def test_api() -> None:
    # int64_t utmscale_getTimeScaleValue(
    #       UDateTimeScale timeScale,
    #       UTimeScaleValue value,
    #       UErrorCode *status
    # )
    from_min = icu.utmscale_get_time_scale_value(
        icu.UDateTimeScale.UDTS_JAVA_TIME,
        icu.UTimeScaleValue.UTSV_FROM_MIN_VALUE,
    )
    assert isinstance(from_min, int)

    # int64_t utmscale_fromInt64(
    #       int64_t otherTime,
    #       UDateTimeScale timeScale,
    #       UErrorCode *status
    # )
    universal_time = icu.utmscale_from_int64(from_min, icu.UDateTimeScale.UDTS_JAVA_TIME)
    assert isinstance(universal_time, int)

    # int64_t utmscale_toInt64(
    #       int64_t universalTime,
    #       UDateTimeScale timeScale,
    #       UErrorCode *status
    # )
    dt = icu.utmscale_to_int64(universal_time, icu.UDateTimeScale.UDTS_JAVA_TIME)
    assert isinstance(dt, int)
    assert dt == from_min

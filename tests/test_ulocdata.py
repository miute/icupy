from icupy import icu
from icupy.utils import gc


def test_api():
    # void ulocdata_getCLDRVersion(
    #       UVersionInfo versionArray,
    #       UErrorCode *status
    # )
    version_array = icu.ulocdata_get_cldr_version()
    assert isinstance(version_array, tuple)
    assert len(version_array) > 0
    assert all(isinstance(x, int) for x in version_array)

    # UMeasurementSystem ulocdata_getMeasurementSystem(
    #       const char *localeID,
    #       UErrorCode *status
    # )
    assert icu.ulocdata_get_measurement_system("en") == icu.UMeasurementSystem.UMS_US

    # void ulocdata_getPaperSize(
    #       const char *localeID,
    #       int32_t *height,
    #       int32_t *width,
    #       UErrorCode *status
    # )
    height, width = icu.ulocdata_get_paper_size("en")
    assert (height, width) == (279, 216)

    # ULocaleData *ulocdata_open(
    #       const char *localeID,
    #       UErrorCode *status
    # )
    # void ulocdata_close(LocaleData *uld)
    with gc(icu.ulocdata_open("en"), icu.ulocdata_close) as uld:
        # int32_t ulocdata_getDelimiter(
        #       ULocaleData *uld,
        #       ULocaleDataDelimiterType type,
        #       UChar *result,
        #       int32_t resultLength,
        #       UErrorCode *status
        # )
        result = icu.ulocdata_get_delimiter(
            uld, icu.ULocaleDataDelimiterType.ULOCDATA_QUOTATION_START
        )
        assert isinstance(result, str)
        assert result == "\u201c"  # '“'

        # USet *ulocdata_getExemplarSet(
        #       ULocaleData *uld,
        #       USet *fillIn,
        #       uint32_t options,
        #       ULocaleDataExemplarSetType extype,
        #       UErrorCode *status
        # )
        uset1 = icu.ulocdata_get_exemplar_set(
            uld,
            None,
            icu.USET_ADD_CASE_MAPPINGS,
            icu.ULocaleDataExemplarSetType.ULOCDATA_ES_STANDARD,
        )
        uniset1 = icu.UnicodeSet.from_uset(uset1)
        assert uniset1.size() > 0

        uniset2 = icu.UnicodeSet()
        fill_in = uniset2.to_uset()
        uset3 = icu.ulocdata_get_exemplar_set(
            uld,
            fill_in,
            icu.USET_ADD_CASE_MAPPINGS,
            icu.ULocaleDataExemplarSetType.ULOCDATA_ES_STANDARD,
        )
        uniset3 = icu.UnicodeSet.from_uset(uset3)
        assert uniset1 == uniset2 == uniset3

        # int32_t ulocdata_getLocaleDisplayPattern(
        #       ULocaleData *uld,
        #       UChar *pattern,
        #       int32_t patternCapacity,
        #       UErrorCode *status
        # )
        pattern = icu.ulocdata_get_locale_display_pattern(uld)
        assert isinstance(pattern, str)
        assert pattern == "{0} ({1})"

        # int32_t ulocdata_getLocaleSeparator(
        #       ULocaleData *uld,
        #       UChar *separator,
        #       int32_t separatorCapacity,
        #       UErrorCode *status
        # )
        separator = icu.ulocdata_get_locale_separator(uld)
        assert isinstance(separator, str)
        assert separator == ", "

        # UBool ulocdata_getNoSubstitute(ULocaleData *uld)
        assert icu.ulocdata_get_no_substitute(uld) is False

        # void ulocdata_setNoSubstitute(
        #       ULocaleData *uld,
        #       UBool setting
        # )
        icu.ulocdata_set_no_substitute(uld, True)
        assert icu.ulocdata_get_no_substitute(uld) is True

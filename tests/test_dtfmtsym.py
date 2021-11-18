import pytest
from icupy.icu import (
    DateFormatSymbols, Locale, ULocDataLocaleType, U_ICU_VERSION_MAJOR_NUM,
    UnicodeString,
)


def test_api():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))

    # Locale icu::DateFormatSymbols::getLocale(
    #       ULocDataLocaleType type,
    #       UErrorCode &status
    # )
    locale = sym.get_locale(ULocDataLocaleType.ULOC_VALID_LOCALE)
    assert isinstance(locale, Locale)
    assert locale == Locale("ja_JP")

    # UnicodeString &icu::DateFormatSymbols::getLocalPatternChars(
    #       UnicodeString &result
    # )
    result = UnicodeString()
    chars = sym.get_local_pattern_chars(result)
    assert isinstance(chars, UnicodeString)
    assert id(chars) == id(result)
    assert len(chars) > 0

    # void icu::DateFormatSymbols::setLocalPatternChars(
    #       const UnicodeString &newLocalPatternChars
    # )
    sym2.set_local_pattern_chars(UnicodeString("abcdefghijklmonpqr"))
    chars = sym2.get_local_pattern_chars(result)
    assert chars == "abcdefghijklmonpqr"

    sym2.set_local_pattern_chars("GyMdkHmsSEDFwWahKzYeugAZvcLQqVUOXxrbB")
    chars = sym2.get_local_pattern_chars(result)
    assert chars == "GyMdkHmsSEDFwWahKzYeugAZvcLQqVUOXxrbB"

    # static const char16_t *icu::DateFormatSymbols::getPatternUChars(void)
    uchars = DateFormatSymbols.get_pattern_uchars()
    assert isinstance(uchars, str)
    assert len(uchars) > 0


def test_date_format_symbols():
    # [1]
    # icu::DateFormatSymbols::DateFormatSymbols(UErrorCode &status)
    sym1 = DateFormatSymbols()

    # [2]
    # icu::DateFormatSymbols::DateFormatSymbols(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    sym2 = DateFormatSymbols(Locale.get_us())

    # [5]
    # icu::DateFormatSymbols::DateFormatSymbols(const DateFormatSymbols &)
    sym5 = DateFormatSymbols(sym2)

    # icu::DateFormatSymbols::operator!=(const DateFormatSymbols &other)
    assert sym1 != sym2
    assert sym1 != sym5
    assert not (sym2 != sym5)

    # icu::DateFormatSymbols::operator==(const DateFormatSymbols &other)
    assert not (sym1 == sym2)
    assert not (sym1 == sym5)
    assert sym2 == sym5


def test_get_am_pm_strings():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getAmPmStrings(
    #       int32_t &count
    # )
    ampms = sym.get_am_pm_strings()
    assert isinstance(ampms, list)
    assert len(ampms) == 2
    assert all(isinstance(x, UnicodeString) for x in ampms)
    assert ampms[0] == "\u5348\u524d"  # 午前
    assert ampms[1] == "\u5348\u5f8c"  # 午後

    # void icu::DateFormatSymbols::setAmPmStrings(
    #       const UnicodeString *ampms,
    #       int32_t count
    # )
    assert sym2.get_am_pm_strings() != ampms
    sym2.set_am_pm_strings(ampms, len(ampms))
    assert sym2.get_am_pm_strings() == ampms

    assert sym3.get_am_pm_strings() != ampms
    sym3.set_am_pm_strings(ampms)  # count is optional
    assert sym3.get_am_pm_strings() == ampms


def test_get_era_names():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getEraNames(int32_t &count)
    era_names = sym.get_era_names()
    assert isinstance(era_names, list)
    assert len(era_names) == 2
    assert all(isinstance(x, UnicodeString) for x in era_names)
    assert era_names[0] == "\u7D00\u5143\u524D"  # 紀元前
    assert era_names[1] == "\u897F\u66A6"  # 西暦

    # void icu::DateFormatSymbols::setEraNames(
    #       const UnicodeString *eraNames,
    #       int32_t count
    # )
    assert sym2.get_era_names() != era_names
    sym2.set_era_names(era_names, len(era_names))
    assert sym2.get_era_names() == era_names

    assert sym3.get_era_names() != era_names
    sym3.set_era_names(era_names)  # count is optional
    assert sym3.get_era_names() == era_names


def test_get_eras():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getEras(int32_t &count)
    eras = sym.get_eras()
    assert isinstance(eras, list)
    assert len(eras) == 2
    assert all(isinstance(x, UnicodeString) for x in eras)
    assert eras[0] == "\u7D00\u5143\u524D"  # 紀元前
    assert eras[1] == "\u897F\u66A6"  # 西暦

    # void icu::DateFormatSymbols::setEras(
    #       const UnicodeString *eras,
    #       int32_t count
    # )
    assert sym2.get_eras() != eras
    sym2.set_eras(eras, len(eras))
    assert sym2.get_eras() == eras

    assert sym3.get_eras() != eras
    sym3.set_eras(eras)  # count is optional
    assert sym3.get_eras() == eras


def test_get_months():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # [1]
    # const UnicodeString *icu::DateFormatSymbols::getMonths(int32_t &count)
    months1 = sym.get_months()
    assert isinstance(months1, list)
    assert len(months1) == 12
    assert all(isinstance(x, UnicodeString) for x in months1)
    assert months1[0] == "1\u6708"  # 1月
    assert months1[11] == "12\u6708"  # 12月

    # [1]
    # void icu::DateFormatSymbols::setMonths(
    #       const UnicodeString *months,
    #       int32_t count
    # )
    assert sym2.get_months() != months1
    sym2.set_months(months1, len(months1))
    assert sym2.get_months() == months1

    assert sym3.get_months() != months1
    sym3.set_months(months1)  # count is optional
    assert sym3.get_months() == months1

    # [2]
    # const UnicodeString *icu::DateFormatSymbols::getMonths(
    #       int32_t &count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    months2 = sym.get_months(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert isinstance(months2, list)
    assert len(months2) == 12
    assert all(isinstance(x, UnicodeString) for x in months2)
    assert months2[0] == "1\u6708"  # 1月
    assert months2[11] == "12\u6708"  # 12月

    # [2]
    # void icu::DateFormatSymbols::setMonths(
    #       const UnicodeString *months,
    #       int32_t count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    assert sym2.get_months(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != months2
    sym2.set_months(
        months2,
        len(months2),
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym2.get_months(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == months2

    assert sym3.get_months(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != months2
    sym3.set_months(
        months2,
        -1,
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym3.get_months(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == months2


def test_get_narrow_eras():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getNarrowEras(
    #       int32_t &count
    # )
    narrow_eras = sym.get_narrow_eras()
    assert isinstance(narrow_eras, list)
    assert len(narrow_eras) == 2
    assert all(isinstance(x, UnicodeString) for x in narrow_eras)
    assert narrow_eras[0] == "BC"
    assert narrow_eras[1] == "AD"

    # void icu::DateFormatSymbols::setNarrowEras(
    #       const UnicodeString *narrowEras,
    #       int32_t count
    # )
    assert sym2.get_narrow_eras() != narrow_eras
    sym2.set_narrow_eras(narrow_eras, len(narrow_eras))
    assert sym2.get_narrow_eras() == narrow_eras

    assert sym3.get_narrow_eras() != narrow_eras
    sym3.set_narrow_eras(narrow_eras)  # count is optional
    assert sym3.get_narrow_eras() == narrow_eras


def test_get_quarters():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getQuarters(
    #       int32_t &count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    quarters = sym.get_quarters(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE)
    assert isinstance(quarters, list)
    assert len(quarters) == 4
    assert all(isinstance(x, UnicodeString) for x in quarters)
    assert quarters[0] == "\u7B2C1\u56DB\u534A\u671F"  # 第1四半期
    assert quarters[3] == "\u7B2C4\u56DB\u534A\u671F"  # 第4四半期

    # void icu::DateFormatSymbols::setQuarters(
    #       const UnicodeString *quarters,
    #       int32_t count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    assert sym2.get_quarters(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE) != quarters
    sym2.set_quarters(
        quarters,
        len(quarters),
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE)
    assert sym2.get_quarters(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE) == quarters

    assert sym3.get_quarters(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE) != quarters
    sym3.set_quarters(
        quarters,
        -1,
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE)
    assert sym3.get_quarters(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.WIDE) == quarters


def test_get_short_months():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getShortMonths(
    #       int32_t &count
    # )
    short_months = sym.get_short_months()
    assert isinstance(short_months, list)
    assert len(short_months) == 12
    assert all(isinstance(x, UnicodeString) for x in short_months)
    assert short_months[0] == "1\u6708"  # 1月
    assert short_months[11] == "12\u6708"  # 12月

    # void icu::DateFormatSymbols::setShortMonths(
    #       const UnicodeString *shortMonths,
    #       int32_t count
    # )
    assert sym2.get_short_months() != short_months
    sym2.set_short_months(short_months, len(short_months))
    assert sym2.get_short_months() == short_months

    assert sym3.get_short_months() != short_months
    sym3.set_short_months(short_months)  # count is optional
    assert sym3.get_short_months() == short_months


def test_get_short_weekdays():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getShortWeekdays(
    #       int32_t &count
    # )
    abbrev_weekdays = sym.get_short_weekdays()
    assert isinstance(abbrev_weekdays, list)
    assert len(abbrev_weekdays) == 8
    assert all(isinstance(x, UnicodeString) for x in abbrev_weekdays)
    assert len(abbrev_weekdays[0]) == 0
    assert abbrev_weekdays[1] == "\u65E5"  # 日
    assert abbrev_weekdays[7] == "\u571F"  # 土

    # void icu::DateFormatSymbols::setShortWeekdays(
    #       const UnicodeString *abbrevWeekdays,
    #       int32_t count
    # )
    assert sym2.get_short_weekdays() != abbrev_weekdays
    sym2.set_short_weekdays(abbrev_weekdays, len(abbrev_weekdays))
    assert sym2.get_short_weekdays() == abbrev_weekdays

    assert sym3.get_short_weekdays() != abbrev_weekdays
    sym3.set_short_weekdays(abbrev_weekdays)  # count is optional
    assert sym3.get_short_weekdays() == abbrev_weekdays


def test_get_weekdays():
    sym = DateFormatSymbols(Locale.get_japan())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # [1]
    # const UnicodeString *icu::DateFormatSymbols::getWeekdays(int32_t &count)
    weekdays1 = sym.get_weekdays()
    assert isinstance(weekdays1, list)
    assert len(weekdays1) == 8
    assert all(isinstance(x, UnicodeString) for x in weekdays1)
    assert len(weekdays1[0]) == 0
    assert weekdays1[1] == "\u65E5\u66DC\u65E5"  # 日曜日
    assert weekdays1[7] == "\u571F\u66DC\u65E5"  # 土曜日

    # [1]
    # void icu::DateFormatSymbols::setWeekdays(
    #       const UnicodeString *weekdays,
    #       int32_t count
    # )
    assert sym2.get_weekdays() != weekdays1
    sym2.set_weekdays(weekdays1, len(weekdays1))
    assert sym2.get_weekdays() == weekdays1

    assert sym3.get_weekdays() != weekdays1
    sym3.set_weekdays(weekdays1)  # count is optional
    assert sym3.get_weekdays() == weekdays1

    # [2]
    # const UnicodeString *icu::DateFormatSymbols::getWeekdays(
    #       int32_t &count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    weekdays2 = sym.get_weekdays(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert isinstance(weekdays2, list)
    assert len(weekdays2) == 8
    assert all(isinstance(x, UnicodeString) for x in weekdays2)
    assert len(weekdays2[0]) == 0
    assert weekdays2[1] == "\u65E5"  # 日
    assert weekdays2[7] == "\u571F"  # 土

    # [2]
    # void icu::DateFormatSymbols::setWeekdays(
    #       const UnicodeString *weekdays,
    #       int32_t count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    assert sym2.get_weekdays(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != weekdays2
    sym2.set_weekdays(
        weekdays2,
        len(weekdays2),
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym2.get_weekdays(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == weekdays2

    assert sym3.get_weekdays(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != weekdays2
    sym3.set_weekdays(
        weekdays2,
        -1,
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym3.get_weekdays(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == weekdays2


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_get_year_names():
    sym = DateFormatSymbols(Locale.get_china())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getYearNames(
    #       int32_t &count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    year_names = sym.get_year_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert isinstance(year_names, list)
    assert len(year_names) == 0

    # https://en.wikipedia.org/wiki/Sexagenary_cycle#Sexagenary_years
    src = [
        # "甲子", "乙丑", "丙寅",
        "\u7532\u5b50", "\u4e59\u4e11", "\u4e19\u5bc5",
        # "丁卯", "戊辰", "己巳",
        "\u4e01\u536f", "\u620a\u8fb0", "\u5df1\u5df3",
        # "庚午", "辛未", "壬申",
        "\u5e9a\u5348", "\u8f9b\u672a", "\u58ec\u7533",
        # "癸酉", "甲戌", "乙亥",
        "\u7678\u9149", "\u7532\u620c", "\u4e59\u4ea5",
        # "丙子", "丁丑", "戊寅",
        "\u4e19\u5b50", "\u4e01\u4e11", "\u620a\u5bc5",
        # "己卯", "庚辰", "辛巳",
        "\u5df1\u536f", "\u5e9a\u8fb0", "\u8f9b\u5df3",
        # "壬午", "癸未", "甲申",
        "\u58ec\u5348", "\u7678\u672a", "\u7532\u7533",
        # "乙酉", "丙戌", "丁亥",
        "\u4e59\u9149", "\u4e19\u620c", "\u4e01\u4ea5",
        # "戊子", "己丑", "庚寅",
        "\u620a\u5b50", "\u5df1\u4e11", "\u5e9a\u5bc5",
        # "辛卯", "壬辰", "癸巳",
        "\u8f9b\u536f", "\u58ec\u8fb0", "\u7678\u5df3",
        # "甲午", "乙未", "丙申",
        "\u7532\u5348", "\u4e59\u672a", "\u4e19\u7533",
        # "丁酉", "戊戌", "己亥",
        "\u4e01\u9149", "\u620a\u620c", "\u5df1\u4ea5",
        # "庚子", "辛丑", "壬寅",
        "\u5e9a\u5b50", "\u8f9b\u4e11", "\u58ec\u5bc5",
        # "癸卯", "甲辰", "乙巳",
        "\u7678\u536f", "\u7532\u8fb0", "\u4e59\u5df3",
        # "丙午", "丁未", "戊申",
        "\u4e19\u5348", "\u4e01\u672a", "\u620a\u7533",
        # "己酉", "庚戌", "辛亥",
        "\u5df1\u9149", "\u5e9a\u620c", "\u8f9b\u4ea5",
        # "壬子", "癸丑", "甲寅",
        "\u58ec\u5b50", "\u7678\u4e11", "\u7532\u5bc5",
        # "乙卯", "丙辰", "丁巳",
        "\u4e59\u536f", "\u4e19\u8fb0", "\u4e01\u5df3",
        # "戊午", "己未", "庚申",
        "\u620a\u5348", "\u5df1\u672a", "\u5e9a\u7533",
        # "辛酉", "壬戌", "癸亥",
        "\u8f9b\u9149", "\u58ec\u620c", "\u7678\u4ea5",
    ]
    year_names = [UnicodeString(s) for s in src]

    # void icu::DateFormatSymbols::setYearNames(
    #       const UnicodeString *yearNames,
    #       int32_t count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    assert sym2.get_year_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != year_names
    sym2.set_year_names(
        year_names,
        len(year_names),
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym2.get_year_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == year_names

    assert sym3.get_year_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != year_names
    sym3.set_year_names(
        year_names,
        -1,
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym3.get_year_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == year_names


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 54, reason="ICU4C<54")
def test_get_zodiac_names():
    sym = DateFormatSymbols(Locale.get_china())
    sym2 = DateFormatSymbols(Locale("und"))
    sym3 = DateFormatSymbols(Locale("und"))

    # const UnicodeString *icu::DateFormatSymbols::getZodiacNames(
    #       int32_t &count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    zodiac_names = sym.get_zodiac_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert isinstance(zodiac_names, list)
    assert len(zodiac_names) == 0

    # https://en.wikipedia.org/wiki/Chinese_zodiac
    src = [
        "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake",
        "Horse", "Goat", "Monkey", "Rooster", "Dog", "Pig",
    ]
    zodiac_names = [UnicodeString(s) for s in src]

    # void icu::DateFormatSymbols::setZodiacNames(
    #       const UnicodeString *zodiacNames,
    #       int32_t count,
    #       DtContextType context,
    #       DtWidthType width
    # )
    assert sym2.get_zodiac_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != zodiac_names
    sym2.set_zodiac_names(
        zodiac_names,
        len(zodiac_names),
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym2.get_zodiac_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == zodiac_names

    assert sym3.get_zodiac_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) != zodiac_names
    sym3.set_zodiac_names(
        zodiac_names,
        -1,
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED)
    assert sym3.get_zodiac_names(
        DateFormatSymbols.FORMAT,
        DateFormatSymbols.ABBREVIATED) == zodiac_names

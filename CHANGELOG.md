# CHANGELOG

## v0.13.0 / 2022-07-12

- [CHANGED] Update pybind11 to v2.9.2
- [CHANGED] Change args of RuleBasedBreakIterator.\_\_init__(compiled_rules: list[int], rule_length: int) → \_\_init__(compiled_rules: bytes, rule_length: int)
- [CHANGED] Change return type of RuleBasedBreakIterator.get_binary_rules() -> list[int] → bytes
- [IMPROVED] Allow to create subclass of SearchIterator
  - Add SearchIterator.\_\_init__(other: SearchIterator)
  - Add SearchIterator.\_\_init__()
  - Add SearchIterator.\_\_init__(text: UnicodeString|str, breakiter: BreakIterator = None)
  - Add SearchIterator.\_\_init__(text: CharacterIterator, breakiter: BreakIterator = None)
  - Add SearchIterator.\_\_eq__(other: SearchIterator)
  - Add SearchIterator.\_\_ne__(other: SearchIterator)
  - Add SearchIterator.\_handle_next(position: int)
  - Add SearchIterator.\_handle_prev(position: int)
  - Add SearchIterator.\_set_match_length(length: int)
  - Add SearchIterator.\_set_match_not_found()
  - Add SearchIterator.\_set_match_start(position: int)
  - Add SearchIterator.reset()
  - Add SearchIterator.set_text(text: CharacterIterator)
  - Add SearchIterator.set_text(text: UnicodeString|str)
- [IMPROVED] Allow to create subclass of Transliterator
  - Add Transliterator.\_\_init__(id_: UnicodeString|str, adopted_filter: UnicodeFilter)
  - Add Transliterator.\_\_init__(other: Transliterator)
  - Add Transliterator.\_create_basic_instance(id_: UnicodeString|str, canon: UnicodeString)
  - Add Transliterator.\_handle_transliterate(text: Replaceable, pos: UTransPosition, incremental: bool)
  - Add Transliterator.\_set_id(id_: UnicodeString|str)
  - Add Transliterator.\_set_maximum_context_length(max_context_length: int)
  - Add Transliterator.filtered_transliterate(text: Replaceable, index: UTransPosition, incremental: bool)
  - Add Transliterator.finish_transliteration(text: Replaceable, index: UTransPosition)
  - Add Transliterator.handle_get_source_set(result: UnicodeSet)
- [IMPROVED] Add CollationKey.\_\_hash__()
- [IMPROVED] Add Locale.\_\_hash__()
- [IMPROVED] Add MessagePattern.\_\_hash__()
- [IMPROVED] Add MessagePattern.Part.\_\_hash__()
- [IMPROVED] Add RuleBasedBreakIterator.\_\_hash__()
- [IMPROVED] Add RuleBasedCollator.\_\_hash__()
- [IMPROVED] Add UnicodeSet.\_\_hash__()
- [IMPROVED] Add UnicodeString.\_\_hash__()
- [IMPROVED] Add UnicodeString.handle_replace_between(start: int, limit: int, text: UnicodeString|str)
- [IMPROVED] Change args of Transliterator.unregister(id_: UnicodeString) → unregister(id_: UnicodeString|str)
- [FIXED] Fix comparison method of \_UTextPtr.\_\_eq__(other: \_UTextPtr)
- [FIXED] Fix comparison method of \_UTextPtr.\_\_ne__(other: \_UTextPtr)
- [FIXED] Fix keep alive policy of RuleBasedBreakIterator.\_\_init__(compiled_rules: bytes, rule_length: int)
- [FIXED] Fix keep alive policy of RuleBasedCollator.\_\_init__(bin: bytes, length: int, base: RuleBasedCollator)
- [FIXED] Fix repr() format of \_UTextPtr
- [FIXED] Fix repr() format of AnnualTimeZoneRule
- [FIXED] Fix repr() format of ConstrainedFieldPosition
- [FIXED] Fix repr() format of DateTimeRule
- [FIXED] Fix repr() format of ErrorCode
- [FIXED] Fix repr() format of FieldPosition
- [FIXED] Fix repr() format of GregorianCalendar
- [FIXED] Fix repr() format of ParsePosition
- [FIXED] Fix repr() format of TimeArrayTimeZoneRule
- [FIXED] Fix repr() format of UnicodeSet
- [FIXED] Fix repr() format of UnicodeString
- [FIXED] Fix repr() format of UParseError
- [FIXED] Fix repr() format of UTransPosition

## v0.12.0 / 2022-04-09

- [NEW] Add support for ICU 71.1
- [ADDED] Add DateTimePatternGenerator.get_date_time_format(style: UDateFormatStyle)
- [ADDED] Add DateTimePatternGenerator.set_date_time_format(style: UDateFormatStyle, date_time_format: UnicodeString|str)
- [ADDED] Add RuleBasedCollator.\_\_init__(bin: bytes, length: int, base: RuleBasedCollator)
- [ADDED] Add number.FormattedNumber.get_noun_class()
- [ADDED] Add number.Precision.increment_exact(mantissa: int, magnitude: int)
- [ADDED] Add u_string_has_binary_property(s: str, which: UProperty)
- [ADDED] Add ucnv_clone(cnv: \_UConverterPtr)
- [ADDED] Add NounClass enum
- [ADDED] Add UNumberFormatFields.UNUM_APPROXIMATELY_SIGN_FIELD
- [CHANGED] Change return type of RuleBasedCollator.clone_binary() -> list[int] → bytes
- [IMPROVED] Change args of DateFormatSymbols.\_\_init__(locale: Locale) → \_\_init__(locale: Locale|str)
- [IMPROVED] Change args of Collator.create_instance(loc: Locale) → create_instance(loc: Locale|str)
- [IMPROVED] Change args of Collator.get_display_name(object_locale: Locale, display_locale: Locale, name: UnicodeString) → get_display_name(object_locale: Locale|str, display_locale: Locale|str, name: UnicodeString)
- [IMPROVED] Change args of Collator.get_display_name(object_locale: Locale, name: UnicodeString) → get_display_name(object_locale: Locale|str, name: UnicodeString)
- [IMPROVED] Change args of Collator.get_functional_equivalent(keyword: str, locale: Locale) → get_functional_equivalent(keyword: str, locale: Locale|str)
- [REMOVED] Remove ICUException. Use ICUError instead
- [REMOVED] Remove icupy.\_\_version__. Use importlib.metadata or importlib-metadata package instead

## v0.11.2 / 2022-03-02

- [NEW] Add U_GC_*_MASK constants
- [ADDED] Add UnicodeSet.\_\_ne__(other: \_ConstUSetPtr)
- [ADDED] Add UnicodeSet.\_\_ne__(other: \_USetPtr)
- [ADDED] Add ucnv_cb_from_uwrite_bytes(args: UConverterFromUnicodeArgs, source: str, length: int, offset_index: int)
- [ADDED] Add ucnv_cb_from_uwrite_sub(args: UConverterFromUnicodeArgs, offset_index: int)
- [ADDED] Add ucnv_cb_to_uwrite_sub(args: UConverterToUnicodeArgs, offset_index: int)
- [ADDED] Add ucnv_cb_to_uwrite_uchars(args: UConverterToUnicodeArgs, source: str, length: int, offset_index: int)
- [CHANGED] Update pybind11 to v2.9.1
- [CHANGED] Change args of PluralRules.get_samples(keyword: UnicodeString|str) → get_samples(keyword: UnicodeString|str, dest_capacity: int)
- [CHANGED] Change return type of Formattable.get_object() -> BasicTimeZone|Calendar|CurrencyAmount|DateInterval|TimeUnitAmount|TimeZone|UObject → BasicTimeZone|Calendar|CurrencyAmount|DateInterval|TimeUnitAmount|UObject
- [IMPROVED] Add test cases to increase coverage
- [FIXED] Fix return value policy of BreakIterator.get_text()
- [FIXED] Fix comparison method of UnicodeSet.\_\_eq__(other: \_ConstUSetPtr)
- [FIXED] Fix comparison method of UnicodeSet.\_\_eq__(other: \_USetPtr)
- [DEPRECATED] ICUException is deprecated and will be removed in icupy 0.12
- [DEPRECATED] ucnv_cb_from_u_write_bytes() is deprecated. Use ucnv_cb_from_uwrite_bytes() instead
- [DEPRECATED] ucnv_cb_from_u_write_sub() is deprecated. Use ucnv_cb_from_uwrite_sub() instead
- [DEPRECATED] ucnv_cb_to_u_write_sub() is deprecated. Use ucnv_cb_to_uwrite_sub() instead
- [DEPRECATED] ucnv_cb_to_u_write_uchars() is deprecated. Use ucnv_cb_to_uwrite_uchars() instead

## v0.11.1 / 2021-12-29

Maintenance release.

## v0.11.0 / 2021-12-09

- [NEW] Add support for Python 3.7
- [NEW] Implement icu::CanonicalIterator class
- [NEW] Implement icu::CaseMap class
- [NEW] Implement icu::FilteredBreakIteratorBuilder class
- [NEW] Implement icu::GenderInfo class
- [NEW] Implement icu::LocaleDisplayNames class
- [NEW] Implement icu::Region class
- [NEW] Implement icu::ScientificNumberFormatter class
- [NEW] Implement Locale data API (ulocdata)
- [NEW] Implement icu::NoUnit
- [NEW] Add u_get_data_version()
- [NEW] Add icupy.utils module
- [ADDED] Add ucnv_get_from_ucall_back(converter: \_UConverterPtr)
- [ADDED] Add ucnv_get_to_ucall_back(converter: \_UConverterPtr)
- [ADDED] Add ucnv_set_from_ucall_back(converter: \_UConverterPtr, new_action: \_UConverterFromUCallbackPtr, new_context: ConstVoidPtr)
- [ADDED] Add ucnv_set_to_ucall_back(converter: \_UConverterPtr, new_action: \_UConverterToUCallbackPtr, new_context: ConstVoidPtr)
- [CHANGED] Change module name from icupy.number to icupy.icu.number
- [CHANGED] Move tests.gc() to icupy.utils.gc()
- [IMPROVED] On Windows, add ICU4C DLL search path at initialization
- [IMPROVED] Improve repr() format of ErrorCode
- [IMPROVED] Improve repr() format of Calendar
- [IMPROVED] Improve repr() format of Locale
- [IMPROVED] Improve repr() format of UParseError
- [IMPROVED] Improve repr() format of AnnualTimeZoneRule
- [IMPROVED] Improve repr() format of InitialTimeZoneRule
- [IMPROVED] Improve repr() format of TimeArrayTimeZoneRule
- [IMPROVED] Improve repr() format of UnicodeString
- [DEPRECATED] icupy.number module is deprecated. Use icupy.icu.number instead
- [DEPRECATED] ucnv_get_from_u_call_back() is deprecated. Use ucnv_get_from_ucall_back() instead
- [DEPRECATED] ucnv_get_to_u_call_back() is deprecated. Use ucnv_get_to_ucall_back() instead
- [DEPRECATED] ucnv_set_from_u_call_back() is deprecated. Use ucnv_set_from_ucall_back() instead
- [DEPRECATED] ucnv_set_to_u_call_back() is deprecated. Use ucnv_set_to_ucall_back() instead

## v0.10.0 / 2021-11-06

- [NEW] Add support for ICU 70
- [NEW] Add support for Python 3.10
- [NEW] Implement Arabic shaping API (ushape)
- [NEW] Implement Bidi Transformations API (ubiditransform)
- [NEW] Implement StringPrep API (usprep)
- [NEW] Implement Unicode Security and Spoofing Detection API (uspoof)
- [NEW] Implement Universal Time Scale API (utmscale)
- [NEW] Implement icu::UnicodeSetIterator class
- [ADDED] Add MeasureUnit.create_item()
- [ADDED] Add MeasureUnit.create_kilowatt_hour_per_100_kilometer()
- [ADDED] Add MeasureUnit.get_item()
- [ADDED] Add MeasureUnit.get_kilowatt_hour_per_100_kilometer()
- [ADDED] Add TimeZone.create_enumeration()
- [ADDED] Add TimeZone.create_enumeration_for_raw_offset(raw_offset: int)
- [ADDED] Add TimeZone.create_enumeration_for_region(region: str)
- [ADDED] Add UnicodeSet.has_strings()
- [CHANGED] Update pybind11 to v2.8.0
- [CHANGED] Update UBlockCode enum
- [CHANGED] Update UJoiningGroup enum
- [CHANGED] Update UProperty enum
- [CHANGED] Update UDateFormatSymbolType enum
- [CHANGED] Update UScriptCode enum
- [CHANGED] Update UErrorCode enum

## v0.9.0 / 2021-10-04

- [NEW] Implement icu::AlphabeticIndex class
- [NEW] Implement icu::ListFormatter class
- [NEW] Implement icu::MessagePattern class
- [NEW] Implement icu::RelativeDateTimeFormatter class
- [CHANGED] Update pybind11 to v2.7.1
- [CHANGED] Change default value of DateFormatSymbols.set_am_pm_strings(ampms: list[UnicodeString], count: int = None) → set_am_pm_strings(ampms: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_era_names(era_names: list[UnicodeString], count: int = None) → set_era_names(era_names: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_eras(eras: list[UnicodeString], count: int = None) → set_eras(eras: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_months(months: list[UnicodeString], count: int = None) → set_months(months: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_narrow_eras(narrow_eras: list[UnicodeString], count: int = None) → set_narrow_eras(narrow_eras: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_short_months(short_months: list[UnicodeString], count: int = None) → set_short_months(short_months: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_short_weekdays(abbrev_weekdays: list[UnicodeString], count: int = None) → set_short_weekdays(abbrev_weekdays: list[UnicodeString], count: int = -1)
- [CHANGED] Change default value of DateFormatSymbols.set_weekdays(weekdays: list[UnicodeString], count: int = None) → set_weekdays(weekdays: list[UnicodeString], count: int = -1)
- [IMPROVED] Use CMake environment vars in \_build.py

## v0.8.0 / 2021-09-16

- [NEW] Implement icu::CompactDecimalFormat class
- [NEW] Implement icu::CurrencyPluralInfo class
- [NEW] Implement icu::DecimalFormat class
- [NEW] Implement icu::MessageFormat class
- [NEW] Implement icu::PluralFormat class
- [NEW] Implement icu::PluralRules class
- [NEW] Implement icu::RuleBasedNumberFormat class
- [NEW] Implement icu::SelectFormat class
- [ADDED] Add DateFormat.get_number_format()
- [ADDED] Add DateFormat.set_number_format(new_number_format: NumberFormat)
- [CHANGED] Rename ICUException to ICUError
- [CHANGED] Change args of DateFormatSymbols.set_am_pm_strings(ampms: UnicodeStringVector, count: int = None) → set_am_pm_strings(ampms: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_era_names(era_names: UnicodeStringVector, count: int = None) → set_era_names(era_names: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_eras(eras: UnicodeStringVector, count: int = None) → set_eras(eras: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_months(months: UnicodeStringVector, count: int = None) → set_months(months: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_months(months: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_months(months: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
- [CHANGED] Change args of DateFormatSymbols.set_narrow_eras(narrow_eras: UnicodeStringVector, count: int = None) → set_narrow_eras(narrow_eras: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_quarters(quarters: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_quarters(quarters: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
- [CHANGED] Change args of DateFormatSymbols.set_short_months(short_months: UnicodeStringVector, count: int = None) → set_short_months(short_months: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_short_weekdays(abbrev_weekdays: UnicodeStringVector, count: int = None) → set_short_weekdays(abbrev_weekdays: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_weekdays(weekdays: UnicodeStringVector, count: int = None) → set_weekdays(weekdays: list[UnicodeString], count: int = None)
- [CHANGED] Change args of DateFormatSymbols.set_weekdays(weekdays: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_weekdays(weekdays: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
- [CHANGED] Change args of DateFormatSymbols.set_year_names(year_names: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_year_names(year_names: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
- [CHANGED] Change args of DateFormatSymbols.set_zodiac_names(zodiac_names: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_zodiac_names(zodiac_names: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
- [CHANGED] Change return type of Collator.get_available_locales() -> StringEnumeration → list[Locale]
- [CHANGED] Change return type of BreakIterator.get_available_locales() -> StringEnumeration → list[Locale]
- [IMPROVED] Export enum members into parent scope
- [FIXED] Add missing inherited methods
  - Add CompactDecimalFormat.format()
  - Add DecimalFormat.format()
  - Add MessageFormat.parse_object()
- [DEPRECATED] ICUException is deprecated and will be removed in a future release. Use ICUError instead

## v0.7.0 / 2021-08-11

## v0.6.0 / 2021-03-21

## v0.5.0 / 2021-03-12

## v0.4.0 / 2021-03-02

## v0.3.0 / 2021-02-27

## v0.2.0 / 2021-02-07

## v0.1.0 / 2021-01-30

Initial release.

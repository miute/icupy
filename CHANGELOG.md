# CHANGELOG

## v0.17.0 / 2023-07-10

- CHANGED
  - Drop support for Python 3.7
  - Refactoring
- FIXED
  - Add **icupy.icu.FormattedRelativeDateTime.\_\_init__()**
  - Add **icupy.icu.MeasureUnit.create_milligram_ofglucose_per_deciliter()**
  - Add **icupy.icu.MeasureUnit.get_milligram_ofglucose_per_deciliter()**
  - Add **icupy.icu.MessageFormat.get_locale(type_: ULocDataLocaleType)**
  - Add **icupy.icu.number.FormattedNumber.\_\_init__()**
  - Add **icupy.icu.number.FormattedNumberRange.\_\_init__()**

## v0.16.0 / 2023-04-18

- NEW
  - Add support for [ICU 73](https://github.com/unicode-org/icu/releases/tag/release-73-1) (partial)
- ADDED
  - Add **icupy.icu.Calendar.get_temporal_month_code()**
  - Add **icupy.icu.Calendar.in_daylight_time()**
  - Add **icupy.icu.Calendar.in_temporal_leap_year()**
  - Add **icupy.icu.Calendar.set_temporal_month_code(temporal_month: str)**
  - Add **icupy.icu.MeasureUnit.create_beaufort()**
  - Add **icupy.icu.MeasureUnit.get_beaufort()**
  - Add **icupy.icu.NumberFormat.ERoundingMode.ROUND_HALF_CEILING**
  - Add **icupy.icu.NumberFormat.ERoundingMode.ROUND_HALF_FLOOR**
  - Add **icupy.icu.NumberFormat.ERoundingMode.ROUND_HALF_ODD**
  - Add **icupy.icu.UCalendarDateFields.UCAL_ORDINAL_MONTH**
  - Add **icupy.icu.USET_SIMPLE_CASE_INSENSITIVE**
- CHANGED
  - Update [pybind11](https://github.com/pybind/pybind11) to v2.10.4
- FIXED
  - Move **get_actual_maximum(field: UCalendarDateFields)** of **icupy.icu.GregorianCalendar** class to **icupy.icu.Calendar** class
  - Move **get_actual_minimum(field: UCalendarDateFields)** of **icupy.icu.GregorianCalendar** class to **icupy.icu.Calendar** class
  - Move **roll(field: UCalendarDateFields, amount: int)** of **icupy.icu.GregorianCalendar** class to **icupy.icu.Calendar** class
- REMOVED
  - Remove **icupy.icu.GregorianCalendar.in_daylight_time()**
  - Remove **icupy.number** module

## v0.15.0 / 2022-10-21

- NEW
  - Add support for ICU 72
  - Add support for Python 3.11
  - Implement icu::DisplayOptions class
    - Add icupy.icu.DisplayOptions class
  - Implement Display options API (udisplayoptions)
    - Add icupy.icu.UDisplayOptionsCapitalization enum
    - Add icupy.icu.UDisplayOptionsDisplayLength enum
    - Add icupy.icu.UDisplayOptionsGrammaticalCase enum
    - Add icupy.icu.UDisplayOptionsNameStyle enum
    - Add icupy.icu.UDisplayOptionsNounClass enum
    - Add icupy.icu.UDisplayOptionsPluralCategory enum
    - Add icupy.icu.UDisplayOptionsSubstituteHandling enum
    - Add icupy.icu.udispopt_from_grammatical_case_identifier(identifier: str)
    - Add icupy.icu.udispopt_from_noun_class_identifier(identifier: str)
    - Add icupy.icu.udispopt_from_plural_category_identifier(identifier: str)
    - Add icupy.icu.udispopt_get_grammatical_case_identifier(grammatical_case: UDisplayOptionsGrammaticalCase)
    - Add icupy.icu.udispopt_get_noun_class_identifier(noun_class: UDisplayOptionsNounClass)
    - Add icupy.icu.udispopt_get_plural_category_identifier(plural_category: UDisplayOptionsPluralCategory)
- ADDED
  - Add icupy.icu.UBlockCode.UBLOCK_ARABIC_EXTENDED_C
  - Add icupy.icu.UBlockCode.UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H
  - Add icupy.icu.UBlockCode.UBLOCK_CYRILLIC_EXTENDED_D
  - Add icupy.icu.UBlockCode.UBLOCK_DEVANAGARI_EXTENDED_A
  - Add icupy.icu.UBlockCode.UBLOCK_KAKTOVIK_NUMERALS
  - Add icupy.icu.UBlockCode.UBLOCK_KAWI
  - Add icupy.icu.UBlockCode.UBLOCK_NAG_MUNDARI
  - Add icupy.icu.UScriptCode.USCRIPT_KAWI
  - Add icupy.icu.UScriptCode.USCRIPT_NAG_MUNDARI
  - Add icupy.icu.MeasureUnit.create_quarter()
  - Add icupy.icu.MeasureUnit.create_tonne()
  - Add icupy.icu.MeasureUnit.get_quarter()
  - Add icupy.icu.MeasureUnit.get_tonne()
  - Add icupy.icu.number.LocalizedNumberFormatter.display_options(display_options: DisplayOptions)
  - Add icupy.icu.number.UnlocalizedNumberFormatter.display_options(display_options: DisplayOptions)
- FIXED
  - Fix return type of icupy.icu.number.FormattedNumber.get_noun_class()

## v0.14.0 / 2022-08-29

- CHANGED
  - Update pybind11 to v2.10.0
  - Change args and return type of icupy.icu.Collator.get_bound(source: list[int], source_length: int, bound_type: UColBoundMode, no_of_levels: int) -> list[int] → get_bound(source: bytes, source_length: int, bound_type: UColBoundMode, no_of_levels: int) -> bytes
  - Change args of icupy.icu.CollationKey.\_\_init__(values: list[int], count: int) → \_\_init__(values: bytes, count: int = -1)
  - Change return type of icupy.icu.CollationKey.get_byte_array() -> list[int] → bytes
  - Change return type of icupy.icu.ResourceBundle.get_binary() -> list[int] → bytes
  - Change return type of icupy.icu.ResourceBundle.get_version() -> list → tuple
  - Change return type of icupy.icu.RuleBasedCollator.get_sort_key(...) -> list[int] → bytes
  - Change return type of icupy.icu.RuleBasedCollator.get_version() -> list → tuple
  - Change return type of icupy.icu.u_char_age(c: int) -> list → tuple
  - Change return type of icupy.icu.u_get_data_version() -> list → tuple
  - Change return type of icupy.icu.u_get_unicode_version() -> list → tuple
  - Change return type of icupy.icu.u_get_version() -> list → tuple
  - Change return type of icupy.icu.u_version_from_string() -> list → tuple
  - Change return type of icupy.icu.ulocdata_get_cldr_version() -> list → tuple
- FIXED
  - Raise IndexError when icupy.icu.\_ConstChar16Ptr.\_\_getitem__(...) accessing beyond capacity
  - Change return type of icupy.icu.UnicodeSet.\_\_getitem__(index: slice) -> list[int] → UnicodeSet
  - Change return type of icupy.icu.UnicodeString.\_\_getitem__(index: slice) -> str → UnicodeString
  - Change return type of icupy.icu.UnicodeString comparison operators from int to bool
  - Fix icupy.icu.UnicodeString.\_\_iadd__(other: UnicodeString|str|int) to return itself
  - Fix return value of icupy.icu.UnicodeString.extract_between(start: int, limit: int)
  - Change icupy.icu.UTextVector to keep references
- IMPROVED
  - Add icupy.icu.\_ConstChar16Ptr.\_\_len__()
  - Add icupy.icu.UnicodeString.\_\_setitem__(index: int, value: str|int)
  - Reimplement icupy.icu.UnicodeStringVector
    - Add UnicodeStringVector.\_\_iadd__(item: UnicodeString|str)
    - Add UnicodeStringVector.\_\_iadd__(item: list[UnicodeString])
    - Add UnicodeStringVector.\_\_iadd__(item: list[str])
    - Add UnicodeStringVector.\_\_init__(iterable: list[str])
    - Add UnicodeStringVector.\_\_reversed__()
    - Add UnicodeStringVector.\_\_setitem__(index: int, iterable: list[str])
    - Add UnicodeStringVector.extend(iterable: list[str])
    - Add UnicodeStringVector.reverse()
    - Change args of UnicodeStringVector.\_\_contains__(x: UnicodeString) → \_\_contains__(item: UnicodeString|str)
    - Change args of UnicodeStringVector.\_\_setitem__(arg0: int, arg1: UnicodeString) → \_\_setitem__(index: int, item: UnicodeString|str)
    - Change args of UnicodeStringVector.append(x: UnicodeString) → append(item: UnicodeString|str)
    - Change args of UnicodeStringVector.insert(i: int, x: UnicodeString) → insert(index: int, item: UnicodeString|str)
    - Change args of UnicodeStringVector.remove(x: UnicodeString) → remove(item: UnicodeString|str)
  - Improve icupy.icu.UTextVector
    - Disable reverse iterator of UTextVector
    - Add UTextVector.\_\_init__()
    - Add UTextVector.\_\_delitem__(index: int|slice)
    - Add UTextVector.\_\_iadd__(src: list[UnicodeString])
    - Add UTextVector.append(src: UnicodeString)
    - Add UTextVector.clear()
    - Add UTextVector.extend(src: list[UnicodeString])
    - Add UTextVector.insert(index: int, src: UnicodeString)
  - Change args of icupy.icu.u_version_to_string(version_array: list[int]) → u_version_to_string(version_array: list[int[4]])
- REMOVED
  - Remove icupy.icu.UTextVector.\_\_init__(n: int)

## v0.13.0 / 2022-07-12

- CHANGED
  - Update pybind11 to v2.9.2
  - Change args of icupy.icu.RuleBasedBreakIterator.\_\_init__(compiled_rules: list[int], rule_length: int) → \_\_init__(compiled_rules: bytes, rule_length: int)
  - Change return type of icupy.icu.RuleBasedBreakIterator.get_binary_rules() -> list[int] → bytes
- FIXED
  - Fix comparison method of icupy.icu.\_UTextPtr.\_\_eq__(other: \_UTextPtr)
  - Fix comparison method of icupy.icu.\_UTextPtr.\_\_ne__(other: \_UTextPtr)
  - Fix keep alive policy of icupy.icu.RuleBasedBreakIterator.\_\_init__(compiled_rules: bytes, rule_length: int)
  - Fix keep alive policy of icupy.icu.RuleBasedCollator.\_\_init__(bin: bytes, length: int, base: RuleBasedCollator)
  - Fix repr() format of icupy.icu.\_UTextPtr
  - Fix repr() format of icupy.icu.AnnualTimeZoneRule
  - Fix repr() format of icupy.icu.ConstrainedFieldPosition
  - Fix repr() format of icupy.icu.DateTimeRule
  - Fix repr() format of icupy.icu.ErrorCode
  - Fix repr() format of icupy.icu.FieldPosition
  - Fix repr() format of icupy.icu.GregorianCalendar
  - Fix repr() format of icupy.icu.ParsePosition
  - Fix repr() format of icupy.icu.TimeArrayTimeZoneRule
  - Fix repr() format of icupy.icu.UnicodeSet
  - Fix repr() format of icupy.icu.UnicodeString
  - Fix repr() format of icupy.icu.UParseError
  - Fix repr() format of icupy.icu.UTransPosition
- IMPROVED
  - Allow to create subclass of icupy.icu.SearchIterator
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
  - Allow to create subclass of icupy.icu.Transliterator
    - Add Transliterator.\_\_init__(id_: UnicodeString|str, adopted_filter: UnicodeFilter)
    - Add Transliterator.\_\_init__(other: Transliterator)
    - Add Transliterator.\_create_basic_instance(id_: UnicodeString|str, canon: UnicodeString)
    - Add Transliterator.\_handle_transliterate(text: Replaceable, pos: UTransPosition, incremental: bool)
    - Add Transliterator.\_set_id(id_: UnicodeString|str)
    - Add Transliterator.\_set_maximum_context_length(max_context_length: int)
    - Add Transliterator.filtered_transliterate(text: Replaceable, index: UTransPosition, incremental: bool)
    - Add Transliterator.finish_transliteration(text: Replaceable, index: UTransPosition)
    - Add Transliterator.handle_get_source_set(result: UnicodeSet)
  - Add icupy.icu.CollationKey.\_\_hash__()
  - Add icupy.icu.Locale.\_\_hash__()
  - Add icupy.icu.MessagePattern.\_\_hash__()
  - Add icupy.icu.MessagePattern.Part.\_\_hash__()
  - Add icupy.icu.RuleBasedBreakIterator.\_\_hash__()
  - Add icupy.icu.RuleBasedCollator.\_\_hash__()
  - Add icupy.icu.UnicodeSet.\_\_hash__()
  - Add icupy.icu.UnicodeString.\_\_hash__()
  - Add icupy.icu.UnicodeString.handle_replace_between(start: int, limit: int, text: UnicodeString|str)
  - Change args of icupy.icu.Transliterator.unregister(id_: UnicodeString) → unregister(id_: UnicodeString|str)

## v0.12.0 / 2022-04-09

- NEW
  - Add support for ICU 71
- ADDED
  - Add icupy.icu.DateTimePatternGenerator.get_date_time_format(style: UDateFormatStyle)
  - Add icupy.icu.DateTimePatternGenerator.set_date_time_format(style: UDateFormatStyle, date_time_format: UnicodeString|str)
  - Add icupy.icu.RuleBasedCollator.\_\_init__(bin: bytes, length: int, base: RuleBasedCollator)
  - Add icupy.icu.number.FormattedNumber.get_noun_class()
  - Add icupy.icu.number.Precision.increment_exact(mantissa: int, magnitude: int)
  - Add icupy.icu.u_string_has_binary_property(s: str, which: UProperty)
  - Add icupy.icu.ucnv_clone(cnv: \_UConverterPtr)
  - Add icupy.icu.NounClass enum
  - Add icupy.icu.UNumberFormatFields.UNUM_APPROXIMATELY_SIGN_FIELD
- CHANGED
  - Change return type of icupy.icu.RuleBasedCollator.clone_binary() -> list[int] → bytes
- IMPROVED
  - Change args of icupy.icu.DateFormatSymbols.\_\_init__(locale: Locale) → \_\_init__(locale: Locale|str)
  - Change args of icupy.icu.Collator.create_instance(loc: Locale) → create_instance(loc: Locale|str)
  - Change args of icupy.icu.Collator.get_display_name(object_locale: Locale, display_locale: Locale, name: UnicodeString) → get_display_name(object_locale: Locale|str, display_locale: Locale|str, name: UnicodeString)
  - Change args of icupy.icu.Collator.get_display_name(object_locale: Locale, name: UnicodeString) → get_display_name(object_locale: Locale|str, name: UnicodeString)
  - Change args of icupy.icu.Collator.get_functional_equivalent(keyword: str, locale: Locale) → get_functional_equivalent(keyword: str, locale: Locale|str)
- REMOVED
  - Remove icupy.icu.ICUException. Use icupy.icu.ICUError instead
  - Remove icupy.\_\_version__. Use importlib.metadata or importlib-metadata package instead

## v0.11.2 / 2022-03-02

- NEW
  - Add icupy.icu.U_GC_*_MASK constants
- ADDED
  - Add icupy.icu.UnicodeSet.\_\_ne__(other: \_ConstUSetPtr)
  - Add icupy.icu.UnicodeSet.\_\_ne__(other: \_USetPtr)
  - Add icupy.icu.ucnv_cb_from_uwrite_bytes(args: UConverterFromUnicodeArgs, source: str, length: int, offset_index: int)
  - Add icupy.icu.ucnv_cb_from_uwrite_sub(args: UConverterFromUnicodeArgs, offset_index: int)
  - Add icupy.icu.ucnv_cb_to_uwrite_sub(args: UConverterToUnicodeArgs, offset_index: int)
  - Add icupy.icu.ucnv_cb_to_uwrite_uchars(args: UConverterToUnicodeArgs, source: str, length: int, offset_index: int)
- CHANGED
  - Update pybind11 to v2.9.1
  - Change args of icupy.icu.PluralRules.get_samples(keyword: UnicodeString|str) → get_samples(keyword: UnicodeString|str, dest_capacity: int)
  - Change return type of icupy.icu.Formattable.get_object() -> BasicTimeZone|Calendar|CurrencyAmount|DateInterval|TimeUnitAmount|TimeZone|UObject → BasicTimeZone|Calendar|CurrencyAmount|DateInterval|TimeUnitAmount|UObject
- FIXED
  - Fix return value policy of icupy.icu.BreakIterator.get_text()
  - Fix comparison method of icupy.icu.UnicodeSet.\_\_eq__(other: \_ConstUSetPtr)
  - Fix comparison method of icupy.icu.UnicodeSet.\_\_eq__(other: \_USetPtr)
- DEPRECATED
  - icupy.icu.ICUException is deprecated and will be removed in icupy 0.12
  - icupy.icu.ucnv_cb_from_u_write_bytes() is deprecated. Use icupy.icu.ucnv_cb_from_uwrite_bytes() instead
  - icupy.icu.ucnv_cb_from_u_write_sub() is deprecated. Use icupy.icu.ucnv_cb_from_uwrite_sub() instead
  - icupy.icu.ucnv_cb_to_u_write_sub() is deprecated. Use icupy.icu.ucnv_cb_to_uwrite_sub() instead
  - icupy.icu.ucnv_cb_to_u_write_uchars() is deprecated. Use icupy.icu.ucnv_cb_to_uwrite_uchars() instead

## v0.11.1 / 2021-12-29

Maintenance release.

## v0.11.0 / 2021-12-09

- NEW
  - Add support for Python 3.7
  - Implement icu::CanonicalIterator class
  - Implement icu::CaseMap class
  - Implement icu::FilteredBreakIteratorBuilder class
  - Implement icu::GenderInfo class
  - Implement icu::LocaleDisplayNames class
  - Implement icu::Region class
  - Implement icu::ScientificNumberFormatter class
  - Implement Locale data API (ulocdata)
  - Implement icu::NoUnit
  - Add icupy.icu.u_get_data_version()
  - Add icupy.utils.gc()
- ADDED
  - Add icupy.icu.ucnv_get_from_ucall_back(converter: \_UConverterPtr)
  - Add icupy.icu.ucnv_get_to_ucall_back(converter: \_UConverterPtr)
  - Add icupy.icu.ucnv_set_from_ucall_back(converter: \_UConverterPtr, new_action: \_UConverterFromUCallbackPtr, new_context: ConstVoidPtr)
  - Add icupy.icu.ucnv_set_to_ucall_back(converter: \_UConverterPtr, new_action: \_UConverterToUCallbackPtr, new_context: ConstVoidPtr)
- CHANGED
  - Change module name from icupy.number to icupy.icu.number
- IMPROVED
  - On Windows, add ICU4C DLL search path at initialization
  - Improve repr() format of icupy.icu.ErrorCode
  - Improve repr() format of icupy.icu.Calendar
  - Improve repr() format of icupy.icu.Locale
  - Improve repr() format of icupy.icu.UParseError
  - Improve repr() format of icupy.icu.AnnualTimeZoneRule
  - Improve repr() format of icupy.icu.InitialTimeZoneRule
  - Improve repr() format of icupy.icu.TimeArrayTimeZoneRule
  - Improve repr() format of icupy.icu.UnicodeString
- DEPRECATED
  - icupy.number submodule is deprecated. Use icupy.icu.number instead
  - icupy.icu.ucnv_get_from_u_call_back() is deprecated. Use icupy.icu.ucnv_get_from_ucall_back() instead
  - icupy.icu.ucnv_get_to_u_call_back() is deprecated. Use icupy.icu.ucnv_get_to_ucall_back() instead
  - icupy.icu.ucnv_set_from_u_call_back() is deprecated. Use icupy.icu.ucnv_set_from_ucall_back() instead
  - icupy.icu.ucnv_set_to_u_call_back() is deprecated. Use icupy.icu.ucnv_set_to_ucall_back() instead

## v0.10.0 / 2021-11-06

- NEW
  - Add support for ICU 70
  - Add support for Python 3.10
  - Implement Arabic shaping API (ushape)
  - Implement Bidi Transformations API (ubiditransform)
  - Implement StringPrep API (usprep)
  - Implement Unicode Security and Spoofing Detection API (uspoof)
  - Implement Universal Time Scale API (utmscale)
  - Implement icu::UnicodeSetIterator class
- ADDED
  - Add icupy.icu.MeasureUnit.create_item()
  - Add icupy.icu.MeasureUnit.create_kilowatt_hour_per_100_kilometer()
  - Add icupy.icu.MeasureUnit.get_item()
  - Add icupy.icu.MeasureUnit.get_kilowatt_hour_per_100_kilometer()
  - Add icupy.icu.TimeZone.create_enumeration()
  - Add icupy.icu.TimeZone.create_enumeration_for_raw_offset(raw_offset: int)
  - Add icupy.icu.TimeZone.create_enumeration_for_region(region: str)
  - Add icupy.icu.UnicodeSet.has_strings()
- CHANGED
  - Update pybind11 to v2.8.0
  - Update icupy.icu.UBlockCode enum
  - Update icupy.icu.UJoiningGroup enum
  - Update icupy.icu.UProperty enum
  - Update icupy.icu.UDateFormatSymbolType enum
  - Update icupy.icu.UScriptCode enum
  - Update icupy.icu.UErrorCode enum

## v0.9.0 / 2021-10-04

- NEW
  - Implement icu::AlphabeticIndex class
  - Implement icu::ListFormatter class
  - Implement icu::MessagePattern class
  - Implement icu::RelativeDateTimeFormatter class
- CHANGED
  - Update pybind11 to v2.7.1
  - Change default value of icupy.icu.DateFormatSymbols.set_am_pm_strings(ampms: list[UnicodeString], count: int = None) → set_am_pm_strings(ampms: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_era_names(era_names: list[UnicodeString], count: int = None) → set_era_names(era_names: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_eras(eras: list[UnicodeString], count: int = None) → set_eras(eras: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_months(months: list[UnicodeString], count: int = None) → set_months(months: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_narrow_eras(narrow_eras: list[UnicodeString], count: int = None) → set_narrow_eras(narrow_eras: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_short_months(short_months: list[UnicodeString], count: int = None) → set_short_months(short_months: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_short_weekdays(abbrev_weekdays: list[UnicodeString], count: int = None) → set_short_weekdays(abbrev_weekdays: list[UnicodeString], count: int = -1)
  - Change default value of icupy.icu.DateFormatSymbols.set_weekdays(weekdays: list[UnicodeString], count: int = None) → set_weekdays(weekdays: list[UnicodeString], count: int = -1)
- IMPROVED
  - Use CMake environment vars in \_build.py

## v0.8.0 / 2021-09-16

- NEW
  - Implement icu::CompactDecimalFormat class
  - Implement icu::CurrencyPluralInfo class
  - Implement icu::DecimalFormat class
  - Implement icu::MessageFormat class
  - Implement icu::PluralFormat class
  - Implement icu::PluralRules class
  - Implement icu::RuleBasedNumberFormat class
  - Implement icu::SelectFormat class
- ADDED
  - Add icupy.icu.DateFormat.get_number_format()
  - Add icupy.icu.DateFormat.set_number_format(new_number_format: NumberFormat)
- CHANGED
  - Rename icupy.icu.ICUException to icupy.icu.ICUError
  - Change args of icupy.icu.DateFormatSymbols.set_am_pm_strings(ampms: UnicodeStringVector, count: int = None) → set_am_pm_strings(ampms: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_era_names(era_names: UnicodeStringVector, count: int = None) → set_era_names(era_names: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_eras(eras: UnicodeStringVector, count: int = None) → set_eras(eras: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_months(months: UnicodeStringVector, count: int = None) → set_months(months: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_months(months: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_months(months: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
  - Change args of icupy.icu.DateFormatSymbols.set_narrow_eras(narrow_eras: UnicodeStringVector, count: int = None) → set_narrow_eras(narrow_eras: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_quarters(quarters: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_quarters(quarters: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
  - Change args of icupy.icu.DateFormatSymbols.set_short_months(short_months: UnicodeStringVector, count: int = None) → set_short_months(short_months: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_short_weekdays(abbrev_weekdays: UnicodeStringVector, count: int = None) → set_short_weekdays(abbrev_weekdays: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_weekdays(weekdays: UnicodeStringVector, count: int = None) → set_weekdays(weekdays: list[UnicodeString], count: int = None)
  - Change args of icupy.icu.DateFormatSymbols.set_weekdays(weekdays: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_weekdays(weekdays: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
  - Change args of icupy.icu.DateFormatSymbols.set_year_names(year_names: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_year_names(year_names: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
  - Change args of icupy.icu.DateFormatSymbols.set_zodiac_names(zodiac_names: UnicodeStringVector, count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType) → set_zodiac_names(zodiac_names: list[UnicodeString], count: int, context: DateFormatSymbols.DtContextType, width: DateFormatSymbols.DtWidthType)
  - Change return type of icupy.icu.Collator.get_available_locales() -> StringEnumeration → list[Locale]
  - Change return type of icupy.icu.BreakIterator.get_available_locales() -> StringEnumeration → list[Locale]
- FIXED
  - Add missing inherited methods
    - Add icupy.icu.CompactDecimalFormat.format(...)
    - Add icupy.icu.DecimalFormat.format(...)
    - Add icupy.icu.MessageFormat.parse_object(...)
- IMPROVED
  - Export enum members into parent scope
- DEPRECATED
  - icupy.icu.ICUException is deprecated and will be removed in a future release. Use icupy.icu.ICUError instead

## v0.7.0 / 2021-08-11

## v0.6.0 / 2021-03-21

## v0.5.0 / 2021-03-12

## v0.4.0 / 2021-03-02

## v0.3.0 / 2021-02-27

## v0.2.0 / 2021-02-07

## v0.1.0 / 2021-01-30

Initial release.

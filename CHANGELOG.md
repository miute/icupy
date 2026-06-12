# Changelog

<!-- 2026-05-31/ -->
## Unreleased

### Changed

- **Breaking:** rename the module `icupy.icu.NoUnit` to `icupy.icu.nounit` ([#242])
- Replace anonymous enum members within a class with class attributes ([#243])
- Change the base class of the enums from `pybind11_object` to `enum.IntEnum` ([#245])
- Change the default value of the `locale` parameter in the factory methods of `icupy.icu.DateFormat` and in `icupy.icu.RuleBasedNumberFormat.get_rule_set_display_name()` from `Locale.get_default()` to `None` ([#247])

### Added

- Add `icupy.icu.SymbolTable` class; `UnicodeSet.__init__()` and `UnicodeSet.apply_pattern()` now support `SymbolTable` ([#246])

<!-- 2026-03-27/2026-05-28 -->
## [0.24.0] - 2026-05-28

### Changed

- **Breaking:** update version information APIs to return `UVersionInfo` instead of 4-tuple ([#211])
- **Breaking:** update `icupy.icu.SearchIterator.__next__()` to return tuple of start and end indices instead of start index of match ([#224])
- **Breaking:** update `icupy.icu.SearchIterator.__reversed__()` to return list of `(start, end)` tuples instead of list of start indices of matches ([#224])
- Update `icupy.icu.RuleBasedBreakIterator.get_binary_rules()` to return `memoryview` instead of `bytes` ([#212])
- Update `icupy.icu.ResourceBundle.get_binary()` to return `memoryview` instead of `bytes` ([#213])
- Update `icupy.icu.UnicodeString.get_buffer()` and `.get_terminated_buffer()` to return `memoryview` instead of `ConstChar16Ptr` ([#215])
- Update docstring ([#231])
- Add rich comparison operators for `icupy.icu.CollationKey` ([#235])
- Add `icupy.icu.UnicodeString.__add__()` overload for code point ([#236])
- Add `icupy.icu.FormattedValue.__str__()` ([#239])
- Deprecate `icupy.icu.Transliterator.register_instance()` and `.unregister()` ([#221])
- Deprecate `icupy.icu.ConstVoidPtr`; use `icupy.icu.UserContext` instead ([#227])
- Deprecate `icupy.icu.BreakIterator.BreakIterator.DONE` and `icupy.icu.BreakIterator.DONE`; use `UBRK_DONE` instead ([#229])
- Deprecate `icupy.icu.UnicodeString.temp_sub_string()` and `.temp_sub_string_between()` ([#238])
- Bump pybind11 from 3.0.2 to 3.0.4 ([#217], [#234])

### Added

- Add `icupy.icu.UVersionInfo` class and conversion function ([#210])
- Add helper functions for 16-bit code unit ([#218])
- Add `icupy.icu.UserContext` to replace `icupy.icu.ConstVoidPtr` ([#227])

### Removed

- Delete `icupy.icu.ConstChar16Ptr` class ([#215])
- Delete `icupy.icu.Transliterator.Token` class ([#221])

### Fixed

- Fix `UTransPosition` changes in `icupy.icu.Transliterator._handle_transliterate()` to propagate to the caller ([#221])
- Fix return value ownership of `icupy.icu.AlphabeticIndex.get_record_data()` ([#226])
- Fix return value ownership of `icupy.icu.SearchIterator.get_break_iterator()` ([#232])
- Fix a build issue with pybind11-stubgen ([#237])

<!-- 2025-11-11/2026-03-26 -->
## [0.23.0] - 2026-03-26

### Changed

- **Breaking:** update parameters of `icupy.icu.UnicodeString.__init__()` ([#148])
- **Breaking:** update parameters of `icupy.icu.UnicodeString.from_utf32()` ([#199])
- **Breaking:** update `icupy.icu.UnicodeString.to_utf32()` to return `bytes` instead of `str` ([#199])
- **Breaking:** update `icupy.icu.UnicodeSet.__getitem__(index: int)` to return `str` instead of `UnicodeString` ([#164])
- **Breaking:** update parameters and return type of `icupy.icu.TimeZoneFormat.format()` and `.parse()` to return tuple of result and time type, instead of just returning result ([#167])
- **Breaking:** improve RegexMatcher callback API ([#174], [#175], [#176], [#183])
  - Rename `icupy.icu.URegexFindProgressCallbackPtr` to `icupy.icu.URegexFindProgressCallback`
  - Update parameters of `icupy.icu.RegexMatcher.set_find_progress_callback()`
  - Update parameters of `icupy.icu.URegexFindProgressCallback.__init__()`
  - Update `icupy.icu.RegexMatcher.get_find_progress_callback()` to return `URegexFindProgressCallback` instead of `(URegexFindProgressCallbackPtr, ConstVoidPtr)`
  - Rename `icupy.icu.URegexMatchCallbackPtr` to `icupy.icu.URegexMatchCallback`
  - Update parameters of `icupy.icu.RegexMatcher.set_match_callback()`
  - Update parameters of `icupy.icu.URegexMatchCallback.__init__()`
  - Update `icupy.icu.RegexMatcher.get_match_callback()` to return `URegexMatchCallback` instead of `(URegexMatchCallbackPtr, ConstVoidPtr)`
- **Breaking:** improve UBiDi callback API ([#178], [#183])
  - Rename `icupy.icu.UBiDiClassCallbackPtr` to `icupy.icu.UBiDiClassCallback`
  - Update parameters and return type of `icupy.icu.ubidi_set_class_callback()` to return `UBiDiClassCallback` instead of `(UBiDiClassCallbackPtr, ConstVoidPtr)`
  - Update parameters of `icupy.icu.UBiDiClassCallback.__init__()`
  - Update `icupy.icu.ubidi_get_class_callback()` to return `UBiDiClassCallback` instead of `(UBiDiClassCallbackPtr, ConstVoidPtr)`
- **Breaking:** improve UConverter callback API ([#180], [#183])
  - Rename `icupy.icu.UConverterFromUCallbackPtr` to `icupy.icu.UConverterFromUCallback`
  - Update parameters and return type of `icupy.icu.ucnv_set_from_u_call_back()` to return `UConverterFromUCallback` instead of `(UConverterFromUCallbackPtr, ConstVoidPtr)`
  - Update parameters of `icupy.icu.UConverterFromUCallback.__init__()`
  - Update `icupy.icu.ucnv_get_from_u_call_back()` to return `UConverterFromUCallback` instead of `(UConverterFromUCallbackPtr, ConstVoidPtr)`
  - Rename `icupy.icu.UConverterToUCallbackPtr` to `icupy.icu.UConverterToUCallback`
  - Update parameters and return type of `icupy.icu.ucnv_set_to_u_call_back()` to return `UConverterToUCallback` instead of `(UConverterToUCallbackPtr, ConstVoidPtr)`
  - Update parameters of `icupy.icu.UConverterToUCallback.__init__()`
  - Update `icupy.icu.ucnv_get_to_u_call_back()` to return `UConverterToUCallback` instead of `(UConverterToUCallbackPtr, ConstVoidPtr)`
- **Breaking:** improve `ucpmap_get_range()` ([#182])
  - Rename `icupy.icu.UCPMapValueFilterPtr` to `icupy.icu.UCPMapValueFilter`
  - Update parameters of `icupy.icu.ucpmap_get_range()`
  - Update parameters of `icupy.icu.UCPMapValueFilter.__init__()`
- **Breaking:** remove trailing underscores from parameters in several APIs ([#187])
- Mark previously deprecated APIs as stable again ([#198])
- Add `error_code` attribute to `icupy.icu.ICUError` ([#162])
- Deprecate `icupy.icu.ConstVoidPtr.to_object()`; use `.value()` instead ([#159])
- Deprecate `icupy.icu.FormattedValue.to_temp_string()`; use `.to_string()` instead ([#200])
- Deprecate `.get_buffer()` and `.get_terminated_buffer()` in `icupy.icu.UnicodeString`; use `.__getitem__(index: int)` instead ([#200])
- Improve type checking ([#154])
- Update docstring ([#196])
- Bump pybind11 from 3.0.1 to 3.0.2 ([#197])

### Added

- Add support for [ICU 78.3] ([#170], [#205])

### Fixed

- Fix UnicodeDecodeError in UConverter From Unicode callback function ([#180])
- Add missing functions ([#148], [#202])
- Add missing enums ([#203])
- Fix comparison operators for `icupy.icu.UnicodeSet` to return `bool` instead of `int` ([#165])

<!-- 2025-04-06/2025-11-02 -->
## [0.22.0] - 2025-11-02

### Changed

- **Breaking:** update `icupy.icu.UnicodeString.__getitem__(slice: slice)` to return `str` instead of `UnicodeString` ([#135])
- Bump pybind11 from 2.13.6 to 3.0.1 ([#122], [#131])

### Added

- Add support for Python 3.14 ([#132])
- Add support for [ICU 78.1] ([#134])

### Removed

- **Breaking:** drop support for CMake old than 3.15 ([#115])
- **Breaking:** drop support for Python 3.9 ([#130])
- Remove `icupy.icu.MeasureUnit.create_milligram_of_glucose_per_deciliter()`; use `.create_milligram_ofglucose_per_deciliter()` instead ([#134])
- Remove `icupy.icu.MeasureUnit.get_milligram_of_glucose_per_deciliter()`; use `.get_milligram_ofglucose_per_deciliter()` instead ([#134])

### Fixed

- Fix UnicodeDecodeError in `icupy.icu.UnicodeString.__getitem__(index: int)` ([#135])
- Fix IndexError message when index is negative ([#136])

<!-- /2025-03-15 -->
## [0.21.0] - 2025-03-15

### Added

- Add support for [ICU 77.1] ([#107])

<!-- 2024-10-18/2024-10-26 -->
## [0.20.0] - 2024-10-26

### Changed

- **Breaking:** update `icupy.icu.UnicodeSet.__getitem__(index: int)` to return `str` instead of `int` ([#93])
- Bump pybind11 from 2.12.0 to 2.13.6

### Added

- Add support for Python 3.13
- Add support for [ICU 76.1] ([#93])

### Removed

- **Breaking:** drop support for Python 3.8
- **Breaking:** remove `icupy.icu.UnicodeSet.__getitem__(slice: slice)` ([#93])

<!-- /2024-05-02 -->
## [0.19.0] - 2024-05-02

### Changed

- Bump pybind11 from 2.11.1 to 2.12.0

### Added

- Add support for [ICU 74.2]
- Add support for [ICU 75.1] ([#73])

<!-- 2023-10-12/2023-11-01 -->
## [0.18.0] - 2023-11-01

### Changed

- Change UTF-8 string parameter from `str` to `bytes` in several APIs (9bf09e3)
- Change Unicode string parameter from `UnicodeString` to `UnicodeString | str` in several APIs (25298f7)
- Bump pybind11 from 2.10.4 to 2.11.1

### Added

- Add support for Python 3.12
- Add support for [ICU 74.1] (partial) (#53)
- Add `icupy.icu.number.SimpleNumberFormatter` class (#55)

### Fixed

- Add missing functions for UTF-8 strings (#56)
- Change binding for C/C++ `UBool` type from `int` to `bool` (#59)

<!-- 2023-05-16/2023-07-10 -->
## [0.17.0] - 2023-07-10

### Added

- Add support for [ICU 73.2]

### Removed

- **Breaking:** drop support for Python 3.7

### Fixed

- Add missing functions (52a117e, 24f6714, c20d5ed)

<!-- 2023-03-25/2023-04-18 -->
## [0.16.0] - 2023-04-18

### Changed

- Bump pybind11 from 2.10.0 to 2.10.4

### Added

- Add support for [ICU 73.1] (partial) (86a1778, 982141e, 33c7c37, 83189ae, 6014709, c0b48d0)

### Removed

- Remove `icupy.number` package (4e25ced)

<!-- 2022-10-05/2022-10-21 -->
## [0.15.0] - 2022-10-21

### Added

- Add support for Python 3.11
- Add support for [ICU 72.1] (9223a00, 8872898, 4da30bb, 65866ab, 3faffa3, 254f291, 4a00a15)

<!-- 2022-07-16/2022-08-29 -->
## 0.14.0 - 2022-08-29

### Changed

- **Breaking:** update `icupy.icu.UnicodeSet.__getitem__(slice: slice)` to return `UnicodeSet` instead of `list[int]` (c0ad903)
- **Breaking:** update parameters of `icupy.icu.CollationKey.__init__()` (f585245)
- **Breaking:** update `icupy.icu.CollationKey.get_byte_array()` to return `bytes` instead of `list[int]` (f585245)
- **Breaking:** update `icupy.icu.ResourceBundle.get_binary()` to return `bytes` instead of `list[int]` (532667d)
- **Breaking:** update `icupy.icu.RuleBasedCollator.get_sort_key()` to return `bytes` instead of `list[int]` (6b13e2a)
- **Breaking:** update parameters and return type of `icupy.icu.Collator.get_bound()`; `list[int]` → `bytes` (056a18d)
- **Breaking:** update `icupy.icu.UnicodeString.__getitem__(slice: slice)` to return `UnicodeString` instead of `str` (eea7135)
- Update return type of functions that takes `UVersionInfo` as output parameter; `list` → `tuple` (b6b5d28)
- Improve `icupy.icu.UTextVector` (aeb1d4c, 01a19e4)
- Refactor `icupy.icu.UnicodeStringVector` (5e3bd6e)
- Bump pybind11 from 2.9.2 to 2.10.0

### Added

- Add `icupy.icu.UnicodeString.__setitem__(index: int, value: str | int)` (dfa70ff)

### Removed

- **Breaking:** remove `icupy.icu._ConstChar16Ptr.__getitem__(slice: slice)` (8a9c789)
- **Breaking:** remove `icupy.icu.UTextVector.__init__(n: int)` (aeb1d4c)

### Fixed

- Make IndexError messages more human-readable (fb5b96c)
- Raise IndexError when `icupy.icu._ConstChar16Ptr.__getitem__()` accessing beyond capacity (dda896b)
- Fix return type of comparison operator for `icupy.icu.UnicodeString` (2e4c58d)
- Fix return value of `icupy.icu.UnicodeString.__iadd__()` to return itself (305dde4)

<!-- 2022-04-09/2022-07-12 -->
## 0.13.0 - 2022-07-12

### Changed

- **Breaking:** update parameters of `icupy.icu.RuleBasedBreakIterator.__init__()` (acb6eda)
- Bump pybind11 from 2.9.1 to 2.9.2

### Added

- Enable creation of subclasses of `icupy.icu.SearchIterator` (83da181, 34e91eb)
- Enable creation of subclasses of `icupy.icu.Transliterator` (3499f54)
- Add `.__hash__()` to several APIs (97b0e6c)
- Add `icupy.icu.UnicodeString.handle_replace_between()` (24a4935)

### Fixed

- Fix `repr` format in several APIs (aed7385)

<!-- 2022-03-20/2022-04-08 -->
## 0.12.0 - 2022-04-09

### Changed

- Change locale parameter from `Locale` to `Locale | str` in several APIs (d686d3e, dafa93e)
- Update `icupy.icu.RuleBasedCollator.clone_binary()` to return `bytes` instead of `list[int]` (d95f4ee)

### Added

- Add support for [ICU 71.1] (e968747)
- Add `icupy.icu.u_string_has_binary_property()` (fe37f49)
- Add `icupy.icu.RuleBasedCollator.__init__(bin: collections.abc.Buffer, length: int, base: RuleBasedCollator)` (f3decea)

### Removed

- Remove `icupy.icu.ICUException`; use `icupy.icu.ICUError` instead (6da1f49)
- Remove `icupy.__version__`; use `importlib.metadata` or `importlib-metadata` instead (3867b0d)

## 0.11.2 - 2022-03-02

## 0.11.1 - 2021-12-29

## 0.11.0 - 2021-12-09

## 0.10.0 - 2021-11-06

## 0.9.0 - 2021-10-04

## 0.8.0 - 2021-09-16

## 0.7.0 - 2021-08-11

## 0.6.0 - 2021-03-21

## 0.5.0 - 2021-03-12

## 0.4.0 - 2021-03-02

## 0.3.0 - 2021-02-27

## 0.2.0 - 2021-02-07

## 0.1.0 - 2021-01-30

Initial release.

[ICU 71.1]: https://github.com/unicode-org/icu/releases/tag/release-71-1
[ICU 72.1]: https://github.com/unicode-org/icu/releases/tag/release-72-1
[ICU 73.1]: https://github.com/unicode-org/icu/releases/tag/release-73-1
[ICU 73.2]: https://github.com/unicode-org/icu/releases/tag/release-73-2
[ICU 74.1]: https://github.com/unicode-org/icu/releases/tag/release-74-1
[ICU 74.2]: https://github.com/unicode-org/icu/releases/tag/release-74-2
[ICU 75.1]: https://github.com/unicode-org/icu/releases/tag/release-75-1
[ICU 76.1]: https://github.com/unicode-org/icu/releases/tag/release-76-1
[ICU 77.1]: https://github.com/unicode-org/icu/releases/tag/release-77-1
[ICU 78.1]: https://github.com/unicode-org/icu/releases/tag/release-78.1
[ICU 78.3]: https://github.com/unicode-org/icu/releases/tag/release-78.3

[0.15.0]: https://github.com/miute/icupy/releases/tag/v0.15.0
[0.16.0]: https://github.com/miute/icupy/releases/tag/v0.16.0
[0.17.0]: https://github.com/miute/icupy/releases/tag/v0.17.0
[0.18.0]: https://github.com/miute/icupy/releases/tag/v0.18.0
[0.19.0]: https://github.com/miute/icupy/releases/tag/v0.19.0
[0.20.0]: https://github.com/miute/icupy/releases/tag/v0.20.0
[0.21.0]: https://github.com/miute/icupy/releases/tag/v0.21.0
[0.22.0]: https://github.com/miute/icupy/releases/tag/v0.22.0
[0.23.0]: https://github.com/miute/icupy/releases/tag/v0.23.0
[0.24.0]: https://github.com/miute/icupy/releases/tag/v0.24.0

[#73]: https://github.com/miute/icupy/pull/73
[#93]: https://github.com/miute/icupy/pull/93
[#107]: https://github.com/miute/icupy/pull/107
[#115]: https://github.com/miute/icupy/pull/115
[#122]: https://github.com/miute/icupy/pull/122
[#130]: https://github.com/miute/icupy/pull/130
[#131]: https://github.com/miute/icupy/pull/131
[#132]: https://github.com/miute/icupy/pull/132
[#134]: https://github.com/miute/icupy/pull/134
[#135]: https://github.com/miute/icupy/pull/135
[#136]: https://github.com/miute/icupy/pull/136
[#148]: https://github.com/miute/icupy/pull/148
[#154]: https://github.com/miute/icupy/pull/154
[#159]: https://github.com/miute/icupy/pull/159
[#162]: https://github.com/miute/icupy/pull/162
[#164]: https://github.com/miute/icupy/pull/164
[#165]: https://github.com/miute/icupy/pull/165
[#167]: https://github.com/miute/icupy/pull/167
[#170]: https://github.com/miute/icupy/pull/170
[#174]: https://github.com/miute/icupy/pull/174
[#175]: https://github.com/miute/icupy/pull/175
[#176]: https://github.com/miute/icupy/pull/176
[#178]: https://github.com/miute/icupy/pull/178
[#180]: https://github.com/miute/icupy/pull/180
[#182]: https://github.com/miute/icupy/pull/182
[#183]: https://github.com/miute/icupy/pull/183
[#187]: https://github.com/miute/icupy/pull/187
[#196]: https://github.com/miute/icupy/pull/196
[#197]: https://github.com/miute/icupy/pull/197
[#198]: https://github.com/miute/icupy/pull/198
[#199]: https://github.com/miute/icupy/pull/199
[#200]: https://github.com/miute/icupy/pull/200
[#202]: https://github.com/miute/icupy/pull/202
[#203]: https://github.com/miute/icupy/pull/203
[#205]: https://github.com/miute/icupy/pull/205
[#210]: https://github.com/miute/icupy/pull/210
[#211]: https://github.com/miute/icupy/pull/211
[#212]: https://github.com/miute/icupy/pull/212
[#213]: https://github.com/miute/icupy/pull/213
[#215]: https://github.com/miute/icupy/pull/215
[#217]: https://github.com/miute/icupy/pull/217
[#218]: https://github.com/miute/icupy/pull/218
[#221]: https://github.com/miute/icupy/pull/221
[#224]: https://github.com/miute/icupy/pull/224
[#226]: https://github.com/miute/icupy/pull/226
[#227]: https://github.com/miute/icupy/pull/227
[#229]: https://github.com/miute/icupy/pull/229
[#231]: https://github.com/miute/icupy/pull/231
[#232]: https://github.com/miute/icupy/pull/232
[#234]: https://github.com/miute/icupy/pull/234
[#235]: https://github.com/miute/icupy/pull/235
[#236]: https://github.com/miute/icupy/pull/236
[#237]: https://github.com/miute/icupy/pull/237
[#238]: https://github.com/miute/icupy/pull/238
[#239]: https://github.com/miute/icupy/pull/239
[#242]: https://github.com/miute/icupy/pull/242
[#243]: https://github.com/miute/icupy/pull/243
[#245]: https://github.com/miute/icupy/pull/245
[#246]: https://github.com/miute/icupy/pull/246
[#247]: https://github.com/miute/icupy/pull/247

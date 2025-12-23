# Changelog

<!-- 2025-11-11/ -->
## Unreleased

### Changed

- **Breaking:** change the `codepage_data` argument type from `str` to `bytes` in `icupy.icu.UnicodeString` (#148)
- **Breaking:** change the return type of `icupy.icu.UnicodeSet.__getitem__(index: int)` from `UnicodeString` to `str`
- **Breaking:** remove the `time_type` argument from `icupy.icu.TimeZoneFormat.format()` and `icupy.icu.TimeZoneFormat.parse()`, and return the output `time_type` together with the original return value as a tuple
- Deprecate `to_object()` in `icupy.icu.ConstVoidPtr`; use `value()` instead
- Improve type checking (#154)

### Added

- Add `error_code` attribute to `icupy.icu.ICUError` exception to store ICU4C error codes

### Removed

- Remove `icupy.icu.ucnv_cb_from_u_write_bytes()`; use `icupy.icu.ucnv_cb_from_uwrite_bytes()` instead
- Remove `icupy.icu.ucnv_cb_from_u_write_sub()`; use `icupy.icu.ucnv_cb_from_uwrite_sub()` instead
- Remove `icupy.icu.ucnv_cb_to_u_write_sub()`; use `icupy.icu.ucnv_cb_to_uwrite_sub()` instead
- Remove `icupy.icu.ucnv_cb_to_u_write_uchars()`; use `icupy.icu.ucnv_cb_to_uwrite_uchars()` instead
- Remove `icupy.icu.ucnv_get_from_u_call_back()`; use `icupy.icu.ucnv_get_from_ucall_back()` instead
- Remove `icupy.icu.ucnv_get_to_u_call_back()`; use `icupy.icu.ucnv_get_to_ucall_back()` instead
- Remove `icupy.icu.ucnv_set_from_u_call_back()`; use `icupy.icu.ucnv_set_from_ucall_back()` instead
- Remove `icupy.icu.ucnv_set_to_u_call_back()`; use `icupy.icu.ucnv_set_to_ucall_back()` instead

### Fixed

- Add missing functions (#148)
- Change the return type of the comparison operator for `icupy.icu.UnicodeSet` from `int` to `bool`

<!-- 2025-04-06/2025-11-02 -->
## [0.22.0] - 2025-11-02

### Changed

- **Breaking:** change the return type of `icupy.icu.UnicodeString.__getitem__(slice: slice)` from `UnicodeString` to `str` (#135)
- Bump `pybind11` from 2.13.6 to 3.0.1 (#122, #131)

### Added

- Add support for Python 3.14 (#132)
- Add support for [ICU 78.1] (#134)

### Removed

- **Breaking:** drop support for CMake old than 3.15 (#115)
- **Breaking:** drop support for Python 3.9 (#130)
- Remove `icupy.icu.MeasureUnit.create_milligram_of_glucose_per_deciliter()`. Use `icupy.icu.MeasureUnit.create_milligram_ofglucose_per_deciliter()` instead (#134)
- Remove `icupy.icu.MeasureUnit.get_milligram_of_glucose_per_deciliter()`. Use `icupy.icu.MeasureUnit.get_milligram_ofglucose_per_deciliter()` instead (#134)

### Fixed

- Fix UnicodeDecodeError in `icupy.icu.UnicodeString.__getitem__(index: int)` (#135)
- Fix IndexError message when index is negative (#136)

<!-- /2025-03-15 -->
## [0.21.0] - 2025-03-15

### Added

- Add support for [ICU 77.1] (#107)

<!-- 2024-10-18/2024-10-26 -->
## [0.20.0] - 2024-10-26

### Changed

- **Breaking:** change the return type of `icupy.icu.UnicodeSet.__getitem__(index: int)` from `int` to `UnicodeString` (#93)
- Bump `pybind11` from 2.12.0 to 2.13.6

### Added

- Add support for Python 3.13
- Add support for [ICU 76.1] (#93)

### Removed

- **Breaking:** drop support for Python 3.8
- **Breaking:** remove `icupy.icu.UnicodeSet.__getitem__(slice: slice)` (#93)

<!-- /2024-05-02 -->
## [0.19.0] - 2024-05-02

### Changed

- Bump `pybind11` from 2.11.1 to 2.12.0

### Added

- Add support for [ICU 74.2]
- Add support for [ICU 75.1] (#73)

<!-- 2023-10-12/2023-11-01 -->
## [0.18.0] - 2023-11-01

### Changed

- Change the argument type of the UTF-8 string from `str` to `bytes` (9bf09e3)
- Change the argument type from `UnicodeString` to `UnicodeString | str` (25298f7)
- Bump `pybind11` from 2.10.4 to 2.11.1

### Added

- Add support for Python 3.12
- Add support for [ICU 74.1] (partial) (#53)
- Add `icupy.icu.number.SimpleNumberFormatter` class (#55)

### Fixed

- Add missing functions for UTF-8 strings (#56)
- Change the C/C++ `UBool` type binding from `int` to `bool` (#59)

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

- Bump `pybind11` from 2.10.0 to 2.10.4

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

- **Breaking:** change the return type of `icupy.icu.UnicodeSet.__getitem__(slice: slice)` from `list[int]` to `UnicodeSet` (c0ad903)
- **Breaking:** change the `values` argument type in `icupy.icu.CollationKey.__init__(...)` from `list[int]` to `bytes` (f585245)
- **Breaking:** change the return type of `icupy.icu.CollationKey.get_byte_array()` from `list[int]` to `bytes` (f585245)
- **Breaking:** change the return type of `icupy.icu.ResourceBundle.get_binary()` from `list[int]` to `bytes` (532667d)
- **Breaking:** change the return type of `icupy.icu.RuleBasedCollator.get_sort_key(...)` from `list[int]` to `bytes` (6b13e2a)
- **Breaking:** change the `source` argument and return type of `icupy.icu.Collator.get_bound(...)` from `list[int]` to `bytes` (056a18d)
- **Breaking:** change the return type of the functions that takes `UVersionInfo` as an output parameter from `list` to `tuple` (b6b5d28)
- **Breaking:** change the return type of `icupy.icu.UnicodeString.__getitem__(slice: slice)` from `str` to `UnicodeString` (eea7135)
- Improve `icupy.icu.UTextVector` (aeb1d4c, 01a19e4)
- Refactor `icupy.icu.UnicodeStringVector` (5e3bd6e)
- Bump `pybind11` from 2.9.2 to 2.10.0

### Added

- Add `icupy.icu.UnicodeString.__setitem__(index: int, value: str | int)` (dfa70ff)

### Removed

- **Breaking:** remove `icupy.icu._ConstChar16Ptr.__getitem__(slice: slice)` (8a9c789)
- **Breaking:** remove `icupy.icu.UTextVector.__init__(n: int)` (aeb1d4c)

### Fixed

- Make IndexError messages more human-readable (fb5b96c)
- Raise IndexError when `icupy.icu._ConstChar16Ptr.__getitem__(...)` accessing beyond capacity (dda896b)
- Change the return type of the comparison operator for `icupy.icu.UnicodeString` from `int` to `bool` (2e4c58d)
- Fix the return value of `icupy.icu.UnicodeString.__iadd__(...)` to return itself (305dde4)

<!-- 2022-04-09/2022-07-12 -->
## 0.13.0 - 2022-07-12

### Changed

- **Breaking:** change the `compiled_rules` argument type in `icupy.icu.RuleBasedBreakIterator` from `list[int]` to `buffer` (acb6eda)
- Bump `pybind11` from 2.9.1 to 2.9.2

### Added

- Enable the creation of subclasses of `icupy.icu.SearchIterator` (83da181, 34e91eb)
- Enable the creation of subclasses of `icupy.icu.Transliterator` (3499f54)
- Add `__hash__()` (97b0e6c)
- Add `icupy.icu.UnicodeString.handle_replace_between(...)` (24a4935)

### Fixed

- Fix `repr()` format (aed7385)

<!-- 2022-03-20/2022-04-08 -->
## 0.12.0 - 2022-04-09

### Changed

- Change the argument type from `Locale` to `Locale | str` (d686d3e, dafa93e)
- Change return type of `icupy.icu.RuleBasedCollator.clone_binary()` from `list[int]` to `bytes` (d95f4ee)

### Added

- Add support for [ICU 71.1] (e968747)
- Add `icupy.icu.u_string_has_binary_property(...)` (fe37f49)
- Add `icupy.icu.RuleBasedCollator.__init__(bin: buffer, length: int, base: RuleBasedCollator)` (f3decea)

### Removed

- Remove `icupy.icu.ICUException`. Use `icupy.icu.ICUError` instead (6da1f49)
- Remove `icupy.__version__`. Use `importlib.metadata` or `importlib-metadata` instead (3867b0d)

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

[0.15.0]: https://github.com/miute/icupy/releases/tag/v0.15.0
[0.16.0]: https://github.com/miute/icupy/releases/tag/v0.16.0
[0.17.0]: https://github.com/miute/icupy/releases/tag/v0.17.0
[0.18.0]: https://github.com/miute/icupy/releases/tag/v0.18.0
[0.19.0]: https://github.com/miute/icupy/releases/tag/v0.19.0
[0.20.0]: https://github.com/miute/icupy/releases/tag/v0.20.0
[0.21.0]: https://github.com/miute/icupy/releases/tag/v0.21.0
[0.22.0]: https://github.com/miute/icupy/releases/tag/v0.22.0

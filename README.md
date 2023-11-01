# icupy

[![PyPI](https://img.shields.io/pypi/v/icupy)](https://pypi.org/project/icupy/)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/icupy)](https://pypi.org/project/icupy/)
[![icu](https://img.shields.io/badge/icu-70.1%20%7C%2071.1%20%7C%2072.1%20%7C%2073.2%20%7C%2074.1-red)](https://icu.unicode.org/)
[![PyPI - License](https://img.shields.io/pypi/l/icupy)](https://pypi.org/project/icupy/)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/miute/icupy/main.svg)](https://results.pre-commit.ci/latest/github/miute/icupy/main)
[![tests](https://github.com/miute/icupy/actions/workflows/tests.yml/badge.svg)](https://github.com/miute/icupy/actions/workflows/tests.yml)
[![build wheels](https://github.com/miute/icupy/actions/workflows/build.yml/badge.svg)](https://github.com/miute/icupy/actions/workflows/build.yml)
[![codecov](https://codecov.io/gh/miute/icupy/branch/main/graph/badge.svg?token=QCW3K19ARA)](https://codecov.io/gh/miute/icupy)

Python bindings for [ICU4C](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/index.html) using [pybind11](https://github.com/pybind/pybind11).

## Changes from ICU4C

- **Naming Conventions**

  Renamed functions, methods, and C++ enumerators to conform to [PEP 8](https://peps.python.org/pep-0008/#naming-conventions).

  - Function Names:
    Use `lower_case_with_underscores` style.
  - Method Names:
    Use `lower_case_with_underscores` style.
    Also, use one leading underscore only for protected methods.
  - C++ Enumerators: Use `UPPER_CASE_WITH_UNDERSCORES` style without a leading "k". (e.g., `kDateOffset` → `DATE_OFFSET`)
  - APIs that match Python reserved words: e.g.,
    - `with()` → `with_()`

- **Error Handling**
  - Unlike the C/C++ APIs, `icupy` raises the `icupy.icu.ICUError` exception if an error code indicates a failure instead of receiving an error code `UErrorCode`.

    You can access the [icu::ErrorCode](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1ErrorCode.html) object from `ICUError.args[0]`.
    For example:

    ```python
    from icupy import icu
    try:
        ...
    except icu.ICUError as e:
        print(e.args[0])  # → icupy.icu.ErrorCode
        print(e.args[0].get())  # → icupy.icu.UErrorCode
    ```

## Examples

- [icu::UnicodeString](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeString.html) with
  [error callback](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/ucnv__err_8h.html)

  ```python
  from icupy import icu
  cnv = icu.ucnv_open('utf-8')
  action = icu.UCNV_TO_U_CALLBACK_ESCAPE
  context = icu.ConstVoidPtr(icu.UCNV_ESCAPE_C)
  icu.ucnv_set_to_ucall_back(cnv, action, context)
  utf8 = b'\x61\xfe\x62'  # Impossible bytes
  s = icu.UnicodeString(utf8, -1, cnv)
  str(s)  # → 'a\\xFEb'

  action = icu.UCNV_TO_U_CALLBACK_ESCAPE
  context = icu.ConstVoidPtr(icu.UCNV_ESCAPE_XML_DEC)
  icu.ucnv_set_to_ucall_back(cnv, action, context)
  s = icu.UnicodeString(utf8, -1, cnv)
  str(s)  # → 'a&#254;b'
  ```

- [icu::UnicodeString](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeString.html) with
  [user callback](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/ucnv__cb_8h.html)

  ```python
  from icupy import icu
  def _to_callback(
      _context: object,
      _args: icu.UConverterToUnicodeArgs,
      _code_units: bytes,
      _length: int,
      _reason: icu.UConverterCallbackReason,
      _error_code: icu.UErrorCode,
  ) -> icu.UErrorCode:
      if _reason == icu.UCNV_ILLEGAL:
          _source = ''.join(['%{:02X}'.format(x) for x in _code_units])
          icu.ucnv_cb_to_uwrite_uchars(_args, _source, len(_source), 0)
          _error_code = icu.U_ZERO_ERROR
      return _error_code

  cnv = icu.ucnv_open('utf-8')
  action = icu.UConverterToUCallbackPtr(_to_callback)
  context = icu.ConstVoidPtr(None)
  icu.ucnv_set_to_ucall_back(cnv, action, context)
  utf8 = b'\x61\xfe\x62'  # Impossible bytes
  s = icu.UnicodeString(utf8, -1, cnv)
  str(s)  # → 'a%FEb'
  ```

- [icu::DateFormat](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1DateFormat.html)

  ```python
  from icupy import icu
  tz = icu.TimeZone.create_time_zone('America/Los_Angeles')
  fmt = icu.DateFormat.create_instance_for_skeleton('yMMMMd', icu.Locale.get_english())
  fmt.set_time_zone(tz)
  dest = icu.UnicodeString()
  s = fmt.format(0, dest)
  str(s)  # → 'December 31, 1969'
  ```

- [icu::MessageFormat](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1MessageFormat.html)

  ```python
  from icupy import icu
  fmt = icu.MessageFormat(
      "At {1,time,::jmm} on {1,date,::dMMMM}, "
      "there was {2} on planet {0,number}.",
      icu.Locale.get_us(),
  )
  tz = icu.TimeZone.get_gmt()
  subfmts = fmt.get_formats()
  subfmts[0].set_time_zone(tz)
  subfmts[1].set_time_zone(tz)
  date = 1637685775000.0  # 2021-11-23T16:42:55Z
  obj = icu.Formattable(
      [
          icu.Formattable(7),
          icu.Formattable(date, icu.Formattable.IS_DATE),
          icu.Formattable(icu.UnicodeString('a disturbance in the Force')),
      ]
  )
  dest = icu.UnicodeString()
  s = fmt.format(obj, dest)
  str(s)  # → 'At 4:42 PM on November 23, there was a disturbance in the Force on planet 7.'
  ```

- [icu::number::NumberFormatter](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1number_1_1NumberFormatter.html)

  ```python
  from icupy import icu
  fmt = icu.number.NumberFormatter.with_().unit(icu.MeasureUnit.get_meter()).per_unit(icu.MeasureUnit.get_second())
  print(fmt.locale(icu.Locale.get_us()).format_double(3000).to_string())  # → '3,000 m/s'
  print(fmt.locale(icu.Locale.get_france()).format_double(3000).to_string())  # → '3 000 m/s'
  print(fmt.locale('ar').format_double(3000).to_string())  # → '٣٬٠٠٠ م/ث'
  ```

- [icu::BreakIterator](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1BreakIterator.html)

  ```python
  from icupy import icu
  text = icu.UnicodeString('In the meantime Mr. Weston arrived with his small ship.')
  bi = icu.BreakIterator.create_sentence_instance(icu.Locale('en'))
  bi.set_text(text)
  list(bi)  # → [20, 55]
  # filter based on common English language abbreviations
  bi = icu.BreakIterator.create_sentence_instance(icu.Locale('en@ss=standard'))
  bi.set_text(text)
  list(bi)  # → [55]
  ```

- [icu::IDNA](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1IDNA.html) (UTS #46)

  ```python
  from icupy import icu
  uts46 = icu.IDNA.create_uts46_instance(icu.UIDNA_NONTRANSITIONAL_TO_ASCII)
  dest = icu.UnicodeString()
  info = icu.IDNAInfo()
  uts46.name_to_ascii(icu.UnicodeString('faß.ExAmPlE'), dest, info)
  info.get_errors()  # → 0
  str(dest)  # → 'xn--fa-hia.example'
  ```

- For more examples, see [tests](https://github.com/miute/icupy/tree/main/tests).

## Installation

### Prerequisites

- [Python](https://www.python.org/) >=3.8
- [ICU4C](https://github.com/unicode-org/icu/releases) [(ICU - The International Components for Unicode)](https://icu.unicode.org/) (>=64 recommended)
- C++17 compatible compiler (see [supported compilers](https://github.com/pybind/pybind11#supported-compilers))
- [CMake](https://cmake.org/) >=3.7

### Installing prerequisites

- Windows

  Install the following dependencies.

  - [Python](https://www.python.org/downloads/) >=3.8
  - [Pre-built ICU4C binary package](https://github.com/unicode-org/icu/releases) (>=64 recommended)
  - Visual Studio 2015 Update 3 or newer. Visual Studio 2019 or newer recommended
  - [CMake](https://cmake.org/download/) >=3.7
    - *Note: Add CMake to the system PATH.*

- Linux

  To install dependencies, run the following command:

  - Ubuntu/Debian:

    ```bash
    sudo apt install g++ cmake libicu-dev python3-dev python3-pip
    ```

  - Fedora:

    ```bash
    sudo dnf install gcc-c++ cmake icu libicu-devel python3-devel
    ```

  If your system's ICU is out of date, consider
  [building ICU4C from source](https://unicode-org.github.io/icu/userguide/icu4c/build.html) or installing [pre-built
  ICU4C binary package](https://github.com/unicode-org/icu/releases).

### Building icupy from source

1. Configuring environment variables:

   - Windows:

     - Set the `ICU_ROOT` environment variable to the root of the ICU installation (default is `C:\icu`).
       For example, if the ICU is located in `C:\icu4c`:

       ```bat
       set ICU_ROOT=C:\icu4c
       ```

       or in PowerShell:

       ```powershell
       $env:ICU_ROOT = "C:\icu4c"
       ```

     - To verify settings using `icuinfo` (64-bit):

       ```bat
       %ICU_ROOT%\bin64\icuinfo
       ```

       or in PowerShell:

       ```powershell
       & $env:ICU_ROOT\bin64\icuinfo
       ```

   - Linux:

     - If the ICU is located in a non-regular place, set the `PKG_CONFIG_PATH` and `LD_LIBRARY_PATH` environment variables.
       For example, if the ICU is located in `/usr/local`:

       ```bash
       export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
       export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
       ```

     - To verify settings using `pkg-config`:

       ```bash
       $ pkg-config --cflags --libs icu-uc
       -I/usr/local/include -L/usr/local/lib -licuuc -licudata
       ```

2. Installing from PyPI:

   ```bash
   pip install icupy
   ```

   Optionally, CMake environment variables are available.
   For example, using the Ninja build system and Clang:

   ```bash
   CMAKE_GENERATOR=Ninja CXX=clang++ pip install icupy
   ```

   Alternatively, installing development version from the git repository:

   ```bash
   pip install git+https://github.com/miute/icupy.git
   ```

## Usage

1. Configuring environment variables:

   - Windows:

     - Set the `ICU_ROOT` environment variable to the root of the ICU installation (default is `C:\icu`).
       For example, if the ICU is located in `C:\icu4c`:

       ```bat
       set ICU_ROOT=C:\icu4c
       ```

       or in PowerShell:

       ```powershell
       $env:ICU_ROOT = "C:\icu4c"
       ```

   - Linux:

     - If the ICU is located in a non-regular place, set the `LD_LIBRARY_PATH` environment variables.
       For example, if the ICU is located in `/usr/local`:

       ```bash
       export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
       ```

2. Using `icupy`:

   ```python
   import icupy.icu as icu
   # or
   from icupy import icu
   ```

## License

This project is licensed under the [MIT License](https://github.com/miute/icupy/blob/main/LICENSE).

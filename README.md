# icupy

[![PyPI](https://img.shields.io/pypi/v/icupy)](https://pypi.org/project/icupy/)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/icupy)](https://pypi.org/project/icupy/)
[![icu](https://img.shields.io/badge/icu-70%20%7C%2071%20%7C%2072%20%7C%2073%20%7C%2074%20%7C%2075%20%7C%2076%20%7C%2077%20%7C%2078%20%7C%2079rc-red)](https://icu.unicode.org/)
[![PyPI - License](https://img.shields.io/pypi/l/icupy)](https://pypi.org/project/icupy/)
[![pre-commit.ci status](https://results.pre-commit.ci/badge/github/miute/icupy/main.svg)](https://results.pre-commit.ci/latest/github/miute/icupy/main)
[![tests](https://github.com/miute/icupy/actions/workflows/tests.yml/badge.svg)](https://github.com/miute/icupy/actions/workflows/tests.yml)
[![build wheels](https://github.com/miute/icupy/actions/workflows/build.yml/badge.svg)](https://github.com/miute/icupy/actions/workflows/build.yml)

Python bindings for [ICU4C](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/index.html) using [pybind11](https://github.com/pybind/pybind11).

## Changes from ICU4C

- **Naming Conventions**

  Renamed functions, methods, and enums to conform to [PEP 8](https://peps.python.org/pep-0008/#naming-conventions).

  - **Function Names:**
    use `lower_case_with_underscores` style.
  - **Method Names:**
    use `lower_case_with_underscores` style.
    Also, use one leading underscore only for protected methods.
  - **C++ Enum Member Names:**
    use `UPPER_CASE_WITH_UNDERSCORES` style without a leading "k". (e.g., `kDateOffset` → `DATE_OFFSET`)
  - **APIs that match Python reserved words:** e.g.,
    - `with()` → `with_()`

- **Error Handling**
  - ICU C/C++ API errors are raised as `icupy.icu.ICUError` exceptions.
    The underlying `UErrorCode` can be retrieved from the `error_code` attribute of the exception.

    For example:

    ```python
    from icupy import icu

    try:
        pass  # Call ICU API here...
    except icu.ICUError as e:
        print(e.error_code)  # → icu.ErrorCode
        print(e.error_code.get())  # → icu.UErrorCode
    ```

## Examples

- [icu::UnicodeString](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1UnicodeString.html) with
  [custom error callback function](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/ucnv__cb_8h.html)

  ```python
  # from Unicode to codepage
  from icupy import icu
  from icupy.utils import gc


  def from_unicode_cb(
      options: object,
      args: icu.UConverterFromUnicodeArgs,
      code_units: str,
      length: int,
      code_point: int,
      reason: icu.UConverterCallbackReason,
      error_code: icu.ErrorCode,
  ) -> None:
      _ = options, length, code_point  # unused
      if reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
          error_code.reset()
          source = "".join(f"\\u{ord(c):04x}" for c in code_units)
          icu.ucnv_cb_from_u_write_bytes(args, source, len(source), 0)


  with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
      action = icu.UConverterFromUCallback(from_unicode_cb)
      old_action = icu.ucnv_set_from_u_call_back(cnv, action)
      s = icu.UnicodeString("A€B")
      s.extract(cnv)  # → b'A\\u20acB'
  ```

  ```python
  # from codepage to Unicode
  from icupy import icu
  from icupy.utils import gc


  def to_unicode_cb(
      options: object,
      args: icu.UConverterToUnicodeArgs,
      code_units: bytes,
      length: int,
      reason: icu.UConverterCallbackReason,
      error_code: icu.ErrorCode,
  ) -> None:
      _ = options, length  # unused
      if reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
          error_code.reset()
          source = "".join(f"%{b:02X}" for b in code_units)
          icu.ucnv_cb_to_u_write_uchars(args, source, len(source), 0)


  with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
      action = icu.UConverterToUCallback(to_unicode_cb)
      old_action = icu.ucnv_set_to_u_call_back(cnv, action)
      src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UNASSIGNED SEQUENCE
      s = icu.UnicodeString(src, -1, cnv)
      str(s)  # → 'a%EB%40b'
  ```

- [icu::number::NumberFormatter](https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1number_1_1NumberFormatter.html) (ICU 60+)

  ```python
  from icupy import icu
  from icupy.icu import number

  template = (
      number.NumberFormatter.with_()
      .notation(number.Notation.compact_short())
      .unit(icu.CurrencyUnit("EUR"))
      .precision(number.Precision.max_significant_digits(2))
  )
  template.locale("en_US").format_int(1234).to_string()  # "€1.2K" in en-US
  ```

## Installation

### Prerequisites

- [Python](https://www.python.org/) >=3.11
- [ICU4C](https://github.com/unicode-org/icu/releases) [(ICU - The International Components for Unicode)](https://icu.unicode.org/) (>=70 recommended)
- C++17 compatible compiler (see [Supported Compilers](https://github.com/pybind/pybind11#supported-platforms--compilers))
- [CMake](https://cmake.org/) >=3.15

### Installing prerequisites

- **Windows:**

  Install the following dependencies:

  - [Python](https://www.python.org/downloads/) >=3.11
  - [Pre-built ICU4C binary package](https://github.com/unicode-org/icu/releases) (>=70 recommended)
  - C++17 compatible compiler. Visual Studio 2022 or newer recommended
  - [CMake](https://cmake.org/download/) >=3.15
    - *Note: Add CMake to the system PATH.*

- **Linux:**

  To install dependencies, run the following command:

  - Ubuntu/Debian:

    ```bash
    sudo apt install g++ cmake libicu-dev python3-dev python3-pip
    ```

  - Fedora:

    ```bash
    sudo dnf install gcc-c++ cmake icu libicu-devel python3-devel
    ```

  *Note: If your system's ICU is out of date, consider
  [building ICU4C from source](https://unicode-org.github.io/icu/userguide/icu4c/build.html) or installing [pre-built
  ICU4C binary package](https://github.com/unicode-org/icu/releases).*

### Installing icupy

1. **Configuring environment variables**

   - **Windows:**

     - Set the `ICU_ROOT` environment variable to the root of the ICU installation.

       For example, if the ICU is located in `C:\icu4c`:

       in PowerShell:

       ```powershell
       $env:ICU_ROOT = "C:\icu4c"
       ```

       or in Command Prompt:

       ```batchfile
       set ICU_ROOT=C:\icu4c
       ```

     - To verify settings using `icuinfo` (64-bit):

       in PowerShell:

       ```powershell
       & $env:ICU_ROOT\bin64\icuinfo
       ```

       or in Command Prompt:

       ```batchfile
       %ICU_ROOT%\bin64\icuinfo
       ```

   - **Linux:**

     - If the ICU is located in a non-regular place, set the `PKG_CONFIG_PATH` and `LD_LIBRARY_PATH` environment variables.

       For example, if the ICU is located in `/usr/local`:

       ```bash
       export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
       export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
       ```

     - To verify settings using `pkg-config`:

       ```bash
       pkg-config --cflags --libs icu-uc
       # -I/usr/local/include -L/usr/local/lib -licuuc
       ```

2. **Installing from PyPI**

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

1. **Configuring environment variables**

   - **Windows:**

     - Set the `ICU_ROOT` environment variable to the root of the ICU installation (default is `C:\icu`).

       For example, if the ICU is located in `C:\icu4c`:

       in PowerShell:

       ```powershell
       $env:ICU_ROOT = "C:\icu4c"
       ```

       or in Command Prompt:

       ```batchfile
       set ICU_ROOT=C:\icu4c
       ```

   - **Linux:**

     - If the ICU is located in a non-regular place, set the `LD_LIBRARY_PATH` environment variables.

       For example, if the ICU is located in `/usr/local`:

       ```bash
       export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
       ```

2. **Using icupy**

   ```python
   from icupy import icu  # or 'import icupy.icu as icu'
   ```

## License

This project is licensed under the [MIT License](https://github.com/miute/icupy/blob/main/LICENSE).

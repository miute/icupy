# icupy

Python bindings for [ICU4C](https://unicode-org.github.io/icu/userguide/icu4c/) using [pybind11](https://github.com/pybind/pybind11).

## Installation

### Prerequisites

- [Python](https://www.python.org/downloads/) >=3.7
- [ICU4C](https://icu.unicode.org/download) (>=64 recommended)
- C++17 compatible compiler (See [supported compilers](https://github.com/pybind/pybind11#supported-compilers))
  - Windows: Visual Studio 2015 Update 3 or newer. Visual Studio 2019 recommended.
- [CMake](https://cmake.org/download/) >=3.7

### Installing prerequisites

To install dependencies, run the following command:

- Ubuntu/Debian:

    ```bash
    sudo apt install g++ cmake libicu-dev python3-dev python3-pip
    ```

- Fedora:

    ```bash
    sudo dnf install gcc-c++ cmake icu libicu-devel python3-devel
    ```

- openSUSE:

    If default Python version is 3.8:

    ```bash
    sudo zypper install gcc-c++ cmake icu libicu-devel python38-devel python38-pip
    ```

### Building from source

1. Configuring environment variables:

    - Windows:

        - Set the `ICU_ROOT` environment variable to the root of the ICU installation (default is `C:\icu`).
        For example, if the ICU is located in `C:\icu4c`:

            ```bat
            set ICU_ROOT=C:\icu4c
            ```

            or in PowerShell:

            ```bat
            $env:ICU_ROOT = "C:\icu4c"
            ```

    - Linux/POSIX:

        - If the ICU is located in a non-regular place, set the `PKG_CONFIG_PATH` and `LD_LIBRARY_PATH` environment variables.
        For example, if the ICU is located in `/usr/local`:

            ```bash
            export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
            export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
            ```

2. Installing from PyPI:

    ```bash
    pip install icupy
    ```

    Alternatively, installing development version from the git repository:

    ```bash
    pip install git+https://github.com/miute/icupy.git@develop
    ```

    Optionally, CMake environment variables are available.
    For example, using the Ninja build system and Clang:

    ```bash
    CMAKE_GENERATOR=Ninja CXX=clang pip install icupy
    ```

## Changes from ICU4C

- **Naming Rules**
  - Renamed C functions and C++ class methods from mixed case to snake case. (e.g., `findAndReplace()` → `find_and_replace()`)
  - Renamed C++ enumerators to upper snake case without "k" prefix. (e.g., `kDateOffset` → `DATE_OFFSET`)

## Examples

- icu::UnicodeString with error callback

    ```python
    >>> import icupy.icu as icu
    >>> cnv = icu.ucnv_open("utf-8")
    >>> action = icu.UCNV_TO_U_CALLBACK_ESCAPE
    >>> context = icu.ConstVoidPtr(icu.UCNV_ESCAPE_C)
    >>> icu.ucnv_set_to_ucall_back(cnv, action, context)
    >>> utf8 = b"\x61\xfe\x62"  # Impossible bytes
    >>> s = icu.UnicodeString(utf8, -1, cnv)
    >>> str(s)
    'a\\xFEb'
    ```

- icu::IDNA (UTS #46)

    ```python
    >>> import icupy.icu as icu
    >>> uts46 = icu.IDNA.create_uts46_instance(icu.UIDNA_NONTRANSITIONAL_TO_ASCII)
    >>> dest = icu.UnicodeString()
    >>> info = icu.IDNAInfo()
    >>> uts46.name_to_ascii(icu.UnicodeString("faß.ExAmPlE"), dest, info)
    UnicodeString('xn--fa-hia.example')
    >>> str(dest)
    'xn--fa-hia.example'
    ```

- See also [tests](https://github.com/miute/icupy/tree/main/tests) directory.

## License

This project is licensed under the [MIT License](https://github.com/miute/icupy/blob/main/LICENSE).

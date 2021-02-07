# icupy

Python bindings for ICU4C.

## Installation

### Prerequisites

- [Python](https://www.python.org/) >=3.8, <4
- [ICU4C](http://site.icu-project.org/) (>=64 recommended)
- C++17 compatible compiler (depends on [pybind11](https://github.com/pybind/pybind11))
- [CMake](https://cmake.org/) >=3.7

### Building from source

1. If on Windows or ICU is located in a non-regular place, set the `ICU_ROOT` environment variable to the root of the ICU installation.

1. Installing from PyPI:

    ```bash
    pip install icupy
    ```

    Alternatively, installing from the git repository:

    ```bash
    git clone --recursive https://github.com/miute/icupy.git
    pip install ./icupy
    ```

## Notes

- Changed C function name and class method name from camel case to snake case.

## Examples

- icu::UnicodeString with error callback

    ```python
    >>> import icupy as icu
    >>> cnv = icu.ucnv_open("utf-8")
    >>> action = icu.UCNV_TO_U_CALLBACK_ESCAPE
    >>> context = icu.ConstVoidPtr(icu.UCNV_ESCAPE_C)
    >>> icu.ucnv_set_to_u_call_back(cnv, action, context)
    >>> utf8 = b"\x61\xfe\x62"  # Impossible bytes
    >>> s = icu.UnicodeString(utf8, -1, cnv)
    >>> str(s)
    'a\\xFEb'
    ```

- icu::IDNA (UTS #46)

    ```python
    >>> import icupy as icu
    >>> uts46 = icu.IDNA.create_uts46_instance(icu.IDNA.NONTRANSITIONAL_TO_ASCII)
    >>> dest = icu.UnicodeString()
    >>> info = icu.IDNAInfo()
    >>> uts46.name_to_ascii(icu.UnicodeString("faß.ExAmPlE"), dest, info)
    >>> str(dest)
    'xn--fa-hia.example'
    ```

- See also [tests](https://github.com/miute/icupy/tree/main/tests) directory.

## License

This project is licensed under the [MIT License](https://github.com/miute/icupy/blob/main/LICENSE).

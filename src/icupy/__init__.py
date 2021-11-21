"""
Python bindings for ICU4C
"""

import sys

if sys.platform.startswith("win"):
    import os
    from pathlib import Path

    path = (Path(os.getenv("ICU_ROOT", "C:/icu"))
            / "bin64" if sys.maxsize > 2 ** 32 else "bin").resolve()
    assert path.is_dir(), path  # ICU4C not found
    os.add_dll_directory(path)
    del path

from .icu import *  # noqa

__version__ = icu.VERSION_INFO  # noqa

# DEPRECATED: ICUException is deprecated. Use ICUError instead.
# TODO: Remove ICUException in a future release.
ICUException = ICUError  # noqa

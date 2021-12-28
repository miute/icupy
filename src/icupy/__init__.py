"""
Python bindings for ICU4C
"""

import sys

if sys.platform.startswith("win"):
    import os
    from pathlib import Path

    path = (  # type: ignore
        Path(os.getenv("ICU_ROOT", "C:/icu")) / "bin64"
        if sys.maxsize > 2 ** 32
        else "bin"
    ).resolve()
    if not path.is_dir():
        raise FileNotFoundError(
            "%s is not a valid directory. "
            "Check the ICU_ROOT environment variable setting (ICU_ROOT=%s)"
            % (path, os.getenv("ICU_ROOT", ""))
        )
    if sys.version_info[:2] >= (3, 8):
        os.add_dll_directory(path)
    else:
        os.environ["PATH"] = "{}{}{}".format(
            path, os.pathsep, os.getenv("PATH", "")
        )
    del path

from . import icu  # type: ignore

__version__ = icu.VERSION_INFO

# DEPRECATED: ICUException is deprecated. Use ICUError instead.
# TODO: Remove ICUException in a future release.
ICUException = icu.ICUError

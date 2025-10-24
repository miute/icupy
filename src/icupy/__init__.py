"""
Python bindings for ICU4C
"""

import sys

if sys.platform.startswith("win"):
    import os
    from pathlib import Path

    path = (
        Path(os.getenv("ICU_ROOT", "C:/icu")) / "bin64" if sys.maxsize > 2**32 else "bin"
    ).resolve()  # type: ignore
    if not path.is_dir():
        msg = (
            f"{path!s} is not a valid directory or does not exist. "
            f"Check the ICU_ROOT environment variable setting "
            f"[ICU_ROOT={os.getenv('ICU_ROOT', '')!s}]"
        )
        raise FileNotFoundError(msg)
    os.add_dll_directory(str(path))
    del path

from . import icu  # noqa: F401

import os
import sys
from contextlib import contextmanager
from pathlib import Path


def _startup():
    path = str((Path(__file__).parent / ".." / "src").resolve())
    if path not in sys.path:
        sys.path.append(path)

    if sys.platform.startswith("win"):
        path = os.path.join(os.getenv("ICU_ROOT", "C:\\icu"),
                            "bin" + "64" if sys.maxsize > 2 ** 32 else "")
        assert os.path.isdir(path)  # ICU4C not found
        os.add_dll_directory(path)


@contextmanager
def gc(obj, closer):
    assert callable(closer)
    try:
        yield obj
    finally:
        closer(obj)


_startup()

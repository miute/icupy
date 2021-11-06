import sys
from contextlib import contextmanager
from pathlib import Path


def _startup():
    path = str((Path(__file__).parent / ".." / "src").resolve())
    if path not in sys.path:
        sys.path.append(path)

    if sys.platform.startswith("win"):
        import os

        path = (Path(os.getenv("ICU_ROOT", "C:\\icu"))
                / "bin64" if sys.maxsize > 2 ** 32 else "bin").resolve()
        assert path.is_dir()  # ICU4C not found
        os.add_dll_directory(path)


@contextmanager
def gc(obj, closer):
    assert callable(closer)
    try:
        yield obj
    finally:
        closer(obj)


_startup()

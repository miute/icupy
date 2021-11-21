import sys
from contextlib import contextmanager
from pathlib import Path

path = str((Path(__file__).parent / ".." / "src").resolve())
if path not in sys.path:
    sys.path.append(path)
del path


@contextmanager
def gc(obj, closer):
    assert callable(closer)
    try:
        yield obj
    finally:
        closer(obj)

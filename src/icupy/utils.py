from __future__ import annotations

from collections.abc import Callable, Generator
from contextlib import contextmanager
from typing import Any, TypeVar

__all__ = ["gc"]

T = TypeVar("T")


@contextmanager
def gc(thing: T, closer: Callable[[T], Any]) -> Generator[T, None, None]:
    """Return a context manager that calls `closer(thing)` when the block
    completes.

    Example:
        >>> from icupy import icu
        >>> from icupy.utils import gc
        >>> with gc(icu.ubidi_open(), icu.ubidi_close) as bidi:
        ...     pass

        is equivalent to this:

        >>> from icupy import icu
        >>> bidi = icu.ubidi_open()
        >>> try:
        ...     pass
        ... finally:
        ...     icu.ubidi_close(bidi)
    """
    if not callable(closer):
        msg = f"{type(closer).__name__!r} object is not callable"
        raise TypeError(msg)
    try:
        yield thing
    finally:
        closer(thing)

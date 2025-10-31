from __future__ import annotations

from collections.abc import Callable, Generator
from contextlib import contextmanager
from typing import Any

__all__ = ["gc"]


@contextmanager
def gc(obj: Any, closer: Callable[[Any], Any]) -> Generator[Any]:  # noqa: ANN401
    """Context to automatically release something at the end of a block.

    Args:
        obj (Any): The object that needs to be released.
        closer (Callable[[Any], Any]): A function to release `obj`.

    Yields:
        Any: The object that needs to be released.

    Example:
        >>> with icupy.utils.gc(icupy.icu.ubidi_open(), icupy.icu.ubidi_close) as bidi:
        ...     pass

        is equivalent to this:

        >>> bidi = icupy.icu.ubidi_open()
        >>> try:
        ...     pass
        ... finally:
        ...     icupy.icu.ubidi_close(bidi)
    """
    if not callable(closer):
        msg = f"{type(closer).__name__!r} object is not callable"
        raise TypeError(msg)
    try:
        yield obj
    finally:
        closer(obj)

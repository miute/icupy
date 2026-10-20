from __future__ import annotations

from collections.abc import Callable, Generator, Iterator, Sized
from contextlib import contextmanager
from typing import Any, TypeVar, cast

from icupy import icu

__all__ = ["gc"]

T = TypeVar("T")


class Enumeration(Iterator, Sized):
    """Wrapper class for :class:`~icupy.icu.UEnumeration` that provides
    a Pythonic interface.
    """

    def __init__(self, en: icu.UEnumeration) -> None:
        """Initialize the enumeration with a :class:`~icupy.icu.UEnumeration`
        object.

        .. note::

           :func:`~icupy.icu.uenum_close()` is called when this instance is
           destroyed. Do not reuse *en*.
        """
        self._en = en
        self._count = cast(int, icu.uenum_count(en))

    def __del__(self) -> None:
        """Close the enumeration."""
        icu.uenum_close(self._en)

    def __iter__(self) -> Enumeration:
        """Return the enumeration itself."""
        icu.uenum_reset(self._en)
        return self

    def __len__(self) -> int:
        """Return the number of elements in the enumeration."""
        return self._count

    def __next__(self) -> str:
        """Return the next element in the enumeration."""
        result = icu.uenum_next(self._en)
        if result is None:
            raise StopIteration
        return cast(str, result)


@contextmanager
def gc(thing: T, closer: Callable[[T], Any]) -> Generator[T, None, None]:
    """Return a context manager that calls `closer(thing)` when the block
    completes.

    .. rubric:: Example

    .. code-block:: python

        >>> from icupy import icu
        >>> from icupy.utils import gc
        >>> with gc(icu.ubidi_open(), icu.ubidi_close) as bidi:
        ...     pass

    is equivalent to this:

    .. code-block:: python

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

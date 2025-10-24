from __future__ import annotations

from collections.abc import Callable, Iterator
from contextlib import contextmanager
from typing import Any

__all__ = ["gc"]


@contextmanager
def gc(obj: Any, closer: Callable[[Any], Any]) -> Iterator[Any]:  # noqa: ANN401
    """Context to automatically close something at the end of a block.

    Code like this:

    .. code-block:: python

        with gc(<module>.open(<arguments>), <module>.close) as f:
            <block>

    is equivalent to this:

    .. code-block:: python

        f = <module>.open(<arguments>)
        try:
            <block>
        finally:
            <module>.close(f)
    """
    if not callable(closer):
        msg = f"{type(closer).__name__!r} object is not callable"
        raise TypeError(msg)
    try:
        yield obj
    finally:
        closer(obj)

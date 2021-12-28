import sys
from contextlib import contextmanager
from typing import Any, Iterator

if sys.version_info[:2] >= (3, 9):
    from collections.abc import Callable
else:
    from typing import Callable

__all__ = ["gc"]


@contextmanager
def gc(obj: Any, closer: Callable[[Any], Any]) -> Iterator[Any]:
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
    assert callable(closer)
    try:
        yield obj
    finally:
        closer(obj)

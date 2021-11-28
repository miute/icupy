import sys
from contextlib import contextmanager
from typing import Any

if sys.version_info[:2] >= (3, 9):
    from collections.abc import Callable
    from contextlib import AbstractContextManager
else:
    from typing import Callable, ContextManager as AbstractContextManager

__all__ = ["gc"]


@contextmanager
def gc(obj: Any, closer: Callable[[Any], Any]) -> AbstractContextManager[Any]:
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
            <module>.close(<f>)
    """
    assert callable(closer)
    try:
        yield obj
    finally:
        closer(obj)

"""
Module for icu::number namespace
"""

import warnings

from .icu.number import *  # noqa

# TODO: Remove src/icupy/number.py in a future release.
warnings.warn(
    "icupy.number module is deprecated. Use icupy.icu.number instead.",
    stacklevel=2)

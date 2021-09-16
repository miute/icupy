"""
Python bindings for ICU4C
"""

from .icu import *  # noqa

__version__ = VERSION_INFO  # noqa

# DEPRECATED: ICUException is deprecated. Use ICUError instead.
# TODO: Remove ICUException in a future release.
ICUException = ICUError  # noqa

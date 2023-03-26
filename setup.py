from __future__ import annotations

import sys
from typing import Any

from setuptools import setup

sys.path.append("")
from _build import build  # noqa

setup_kwargs: dict[str, Any] = {}
build(setup_kwargs)
setup(**setup_kwargs)

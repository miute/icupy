import sys
from pathlib import Path

from setuptools import setup
from setuptools.config import read_configuration

sys.path.append("")
from _build import build  # noqa

here = Path(__file__).parent.resolve()
conf_dict = read_configuration(here / "setup.cfg")
setup_kwargs = conf_dict["metadata"]
setup_kwargs.update(conf_dict.get("options", {}))
if "python_requires" in setup_kwargs:
    # Convert SpecifierSet to string
    setup_kwargs["python_requires"] = str(setup_kwargs["python_requires"])

build(setup_kwargs)
setup(**setup_kwargs)

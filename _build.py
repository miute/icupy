import os
import subprocess
import sys
from distutils import log

from setuptools import Extension
from setuptools.command.build_ext import build_ext

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        extdir = os.path.join(extdir, ext.name)

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cfg = "Debug" if self.debug else "Release"

        env = os.environ.copy()
        env["CXXFLAGS"] = " ".join([
            env.get("CXXFLAGS", "").strip(),
            "-DVERSION_INFO=\\\"{}\\\"".format(
                self.distribution.get_version()),
        ]).strip()

        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + extdir,
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(
                cfg.upper(), extdir),
            "-DPYTHON_EXECUTABLE=" + sys.executable,
            "-DCMAKE_BUILD_TYPE=" + cfg,
        ]

        build_args = []

        if sys.platform.startswith("win"):
            if "ICU_ROOT" not in env:
                self.warn("ICU_ROOT variable is not defined.")
                env["ICU_ROOT"] = "C:\\icu"
            cmake_args += [
                "-Wno-dev",
            ]

        if self.compiler.compiler_type == "msvc":
            cmake_args += [
                "-A", PLAT_TO_CMAKE[self.plat_name],
            ]
            build_args += [
                "--config", cfg,
            ]
        else:
            cmake_generator = env.get("CMAKE_GENERATOR")
            if (cmake_generator is None
                    and "CMAKE_BUILD_PARALLEL_LEVEL" not in env):
                build_args += [
                    "--parallel", "4",
                ]

        self.announce("-- CXXFLAGS variable: {}"
                      "".format(repr(env.get("CXXFLAGS"))),
                      level=log.INFO)
        self.announce("-- CMAKE* variables: {}"
                      "".format(["{}={}".format(k, v) for k, v in env.items()
                                 if k.upper().startswith("CMAKE")]),
                      level=log.INFO)
        self.announce("-- CMake options: {}".format(cmake_args),
                      level=log.INFO)
        self.announce("-- Build options: {}".format(build_args),
                      level=log.INFO)

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args,
            cwd=self.build_temp,
            env=env)
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args,
            cwd=self.build_temp,
            env=env)


def build(setup_kwargs):
    src = (setup_kwargs["package_dir"][""] if "package_dir" in setup_kwargs
           else "")
    ext_modules = [CMakeExtension(setup_kwargs["name"], src)]
    cmdclass = dict(build_ext=CMakeBuild)
    setup_kwargs.update({
        "ext_modules": ext_modules,
        "cmdclass": cmdclass,
    })

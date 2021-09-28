import os
import subprocess
import sys

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

        env = os.environ.copy()
        cmake_build_type = env.get("CMAKE_BUILD_TYPE", "Release")
        cfg = "Debug" if self.debug else cmake_build_type

        env["CXXFLAGS"] = " ".join([
            env.get("CXXFLAGS", "").strip(),
            "-DVERSION_INFO=\\\"{}\\\"".format(
                self.distribution.get_version()),
        ]).strip()

        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + extdir,
            "-DPYTHON_EXECUTABLE=" + sys.executable,
            "-DCMAKE_BUILD_TYPE=" + cfg,
        ]

        build_args = []

        if sys.platform.startswith("win"):
            if "ICU_ROOT" not in env:
                env["ICU_ROOT"] = "C:\\icu"
                self.warn(
                    "ICU_ROOT environment variable was not set. "
                    "Using default '{}'.".format(env["ICU_ROOT"]))
            cmake_args += [
                "-Wno-dev",
            ]

        cmake_generator = env.get("CMAKE_GENERATOR")
        is_msbuild = (
                self.compiler.compiler_type == "msvc"
                and (cmake_generator is None
                     or cmake_generator.startswith("Visual Studio"))
        )
        is_multi_config = (
                is_msbuild
                or cmake_generator in ["Ninja Multi-Config", "Xcode"]
        )

        if is_multi_config:
            cmake_args += [
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(
                    cfg.upper(), extdir),
            ]
            build_args += [
                "--config", cfg,
            ]

        if is_msbuild:
            if "CMAKE_GENERATOR_PLATFORM" not in env:
                cmake_args += [
                    "-A", PLAT_TO_CMAKE[self.plat_name],
                ]

        if ("CMAKE_BUILD_PARALLEL_LEVEL" not in env
                and not is_msbuild
                and (cmake_generator is None
                     or not cmake_generator.startswith("Ninja"))):
            jobs = ((hasattr(self, "parallel") and self.parallel)
                    or os.cpu_count())
            if jobs:
                build_args += [
                    "--parallel", str(jobs),
                ]

        self.announce(
            "-- CXXFLAGS environment variable: " + repr(env.get("CXXFLAGS")),
            level=2)
        self.announce(
            "-- CMake environment variables: " + repr(
                ["{}={}".format(k, v) for k, v in env.items()
                 if k.upper().startswith("CMAKE")]),
            level=2)
        self.announce(
            "-- CMake build system options: " + repr(cmake_args),
            level=2)
        self.announce(
            "-- CMake build options: " + repr(build_args),
            level=2)

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

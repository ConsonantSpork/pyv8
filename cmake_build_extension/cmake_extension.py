"""MIT License

Copyright (c) 2020 Diego Ferigo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

from typing import List
from pathlib import Path
from setuptools import Extension


class CMakeExtension(Extension):
    """
    Custom setuptools extension that configures a CMake project.

    Args:
        name: The name of the extension.
        install_prefix: The path relative to the site-package directory where the CMake
            project is installed (typically the name of the Python package).
        disable_editable: Skip this extension in editable mode.
        source_dir: The location of the main CMakeLists.txt.
        cmake_build_type: The default build type of the CMake project.
        cmake_component: The name of component to install. Defaults to all
            components.
        cmake_depends_on: List of dependency packages containing required CMake projects.
    """

    def __init__(self,
                 name: str,
                 install_prefix: str,
                 disable_editable: bool = False,
                 cmake_configure_options: List[str] = (),
                 source_dir: str = str(Path(".").absolute()),
                 cmake_build_type: str = "Release",
                 cmake_component: str = None,
                 cmake_depends_on: List[str] = ()):

        super().__init__(name=name, sources=[])

        if not Path(source_dir).is_absolute():
            source_dir = str(Path(".").absolute() / source_dir)

        if not Path(source_dir).absolute().is_dir():
            raise ValueError(f"Directory '{source_dir}' does not exist")

        self.install_prefix = install_prefix
        self.cmake_build_type = cmake_build_type
        self.disable_editable = disable_editable
        self.cmake_depends_on = cmake_depends_on
        self.source_dir = str(Path(source_dir).absolute())
        self.cmake_configure_options = cmake_configure_options
        self.cmake_component = cmake_component

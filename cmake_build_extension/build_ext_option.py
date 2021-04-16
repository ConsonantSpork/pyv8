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

from typing import NamedTuple
from setuptools.command.build_ext import build_ext


class BuildExtOption(NamedTuple):
    """
    NamedTuple that stores the metadata of a custom build_ext option.

    Example:

        The following option:

        BuildExtOption(variable="define=", short="D", help="New compiler define")

        is displayed as follows:

        $ python setup.py build_ext --help
        ...
        Options for 'BuildExtension' command:
          ...
          --define (-D)     New compiler define
          ...
    """

    variable: str
    short: str
    help: str = ""


def add_new_build_ext_option(option: BuildExtOption, override: bool = True):
    """
    Workaround to add an existing option shown in python setup.py build_ext -h.

    Args:
        option: The new option to add.
        override: Delete the option if it already exists.
    """

    if override:
        build_ext.user_options = \
            [o for o in build_ext.user_options if o[1] is not option.short]
    else:
        for o in build_ext.user_options:
            if o[1] == option.short:
                raise ValueError(f"Short option '{o[1]}' already exists")

    build_ext.user_options.append(tuple(option))

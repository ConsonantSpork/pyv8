# pyv8
V8 engine wrapper for Python 3

# Installation
## Dependencies
* g++
* make
* cmake 3.15.1+
* Python 3.7+
* Boost libraries 1.67+
  ```shell
  sudo apt install libboost1.67-dev  # install boost libs for Ubuntu/Debian
  ```
* Boost.Python 1.67+
  ```shell
  sudo apt install libboost-python-dev  # install Boost.Python for Ubuntu/Debian
  ```
## Installation process
To install first checkout branch ```prebuilt-ubuntu-x64```. It contains binaries for debian/ubuntu. In case you would like to build these binaries yourself, you can obtain required source files from [v8-cmake repository](https://github.com/ConsonantSpork/v8-cmake)
```shell
git checkout prebuilt-ubuntu-x64
```
Then run the installation command:
```shell
python setup.py install
```
## Troubleshooting
In case of errors like the following
```
CMake Error at /usr/share/cmake-3.16/Modules/FindPackageHandleStandardArgs.cmake:146 (message):
  Could NOT find Boost (missing: python3) (found version "1.67.0")
```
* Make sure that all dependencies are installed correctly;
* Apply path `patches/0001-Change-component-requirement-to-python.patch` and rerun installation command.
  ```shell
  git apply patches/0001-Change-component-requirement-to-python.patch  # apply path to fix cmake Boost component not found error
  ```
# Examples
```python
from pyv8 import V8Initializer, V8Instance

V8Initializer.get_instance()  # Initial engine setup, should only be run once

instance = V8Instance()
print(instance.run("'Hello, ' + 'World!'"))  # 'Hello, World'
print(instance.run("1 + 2"))  # 3
print(instance.run("var obj = {key1: 'value1', key2: 'value2'}; obj;"))  # {'key1': 'value1', 'key2': 'value2'}
```


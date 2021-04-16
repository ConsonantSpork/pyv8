# List of things to do before 0.1 release

- [X] Optimize performance to at least pyduk speeds
- [X] TypedArray/ArrayBuffer conversions to python's bytearray
- [ ] Fork https://github.com/bnoordhuis/v8-cmake and set it up as a git submodule

# Before 0.2 release

- [ ] Refactor API to closer resemble PyV8
- [ ] Fix circular references (probably keep a set of Object.GetIdentityHash'es of objects already converted)
- [ ] Add tests to check for librarires interacting with global object
- [ ] Fix install error (Run "python setup.py install; pip install .;" to reproduce)
- [ ] ...

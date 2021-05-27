from setuptools import find_packages, setup
from cmake_build_extension import BuildExtension, CMakeExtension

setup(
    name='pyv8',
    version='0.1.2',
    description="Google's V8 engine wrapper for Python 3",
    author="Dmitry Belov",
    author_email="dimabelov.tu99@gmail.com",
    packages=find_packages('python'),
    package_dir={'': 'python'},
    ext_modules=[
        CMakeExtension(name='_pyv8',
                       install_prefix='pyv8',
                       cmake_configure_options=[
                           '-DMODULE_ONLY=ON',
                       ]),
    ],
    cmdclass={'build_ext' : BuildExtension},
    zip_safe=False
)

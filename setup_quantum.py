from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import pybind11

ext_modules = [
    Extension(
        'amsin_quantum_core',
        ['amsin_quantum_core.cpp'],
        include_dirs=[
            pybind11.get_include()
        ],
        language='c++',
    ),
]

class BuildExtOpt(build_ext):
    def build_extensions(self):
        for ext in self.extensions:
            ext.extra_compile_args = [
                '/O2',
                '/Ox',
                '/Ot',
                '/Oi',
                '/openmp',
                '/std:c++17',
                '/arch:AVX2',
                '/GL',
                '/EHsc'
            ]
        super().build_extensions()

setup(
    name='amsin_quantum_core',
    version='4.01',
    author='Łukasz Włodarski (System Labs)',
    ext_modules=ext_modules,
    cmdclass={'build_ext': BuildExtOpt},
    zip_safe=False,
)

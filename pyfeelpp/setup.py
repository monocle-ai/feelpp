import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = [
            '-DCMAKE_CXX_COMPILER=@CMAKE_CXX_COMPILER@',
            '-DCMAKE_C_COMPILER=@CMAKE_C_COMPILER@',
            '-DCMAKE_INSTALL_PREFIX=@CMAKE_INSTALL_PREFIX@',
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        self.build_temp=self.build_temp+"-"+ext.name
        print self.build_temp
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

setup(
    name='pyfeelpp',
    version='0.0.1',
    author='Christophe Prud\'homme',
    author_email='christophe.prudhomme@feelpp.org',
    description='Python bindings for Feel++',
    long_description='',
    package_dir={ 'pyfeelpp': '@CMAKE_CURRENT_SOURCE_DIR@/pyfeelpp' },
    packages=['pyfeelpp','pyfeelpp.core','pyfeelpp.crb'],
    #packages=['pyfeelpp','pyfeelpp.core' ],
#    ext_modules=[Extension('pyfeelpp',['pyfeelpp/python.cpp'],include_dirs=@FEELPP_INCLUDE_DIRS@,libraries=@FEELPP_LIBRARIES@)
    ext_modules=[CMakeExtension('pyfeelpp._pyfeelpp','@CMAKE_CURRENT_SOURCE_DIR@/pyfeelpp'),
                 CMakeExtension('pyfeelpp.core._core','@CMAKE_CURRENT_SOURCE_DIR@/pyfeelpp/core'),
                 CMakeExtension('pyfeelpp.crb._crb','@CMAKE_CURRENT_SOURCE_DIR@/pyfeelpp/crb'),
    ],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)
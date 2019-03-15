import os

current_path = os.path.realpath('.')

common_inc = os.path.realpath(os.path.join(
    current_path, 'ReluplexCav2017', 'common'))
glpk_inc = os.path.realpath(
    os.path.join(
        current_path, 'ReluplexCav2017', 'glpk-4.60', 'installed', 'include'))
reluplex_inc = os.path.realpath(
    os.path.join(
        current_path, 'ReluplexCav2017', 'reluplex'))
src_inc = glpk_lib = os.path.realpath(
    os.path.join(
        current_path, 'reluplex'))


glpk_lib = os.path.realpath(
    os.path.join(
        current_path, 'ReluplexCav2017', 'glpk-4.60', 'installed', 'lib'))


def configuration(parent_package='', top_path=None):
    from numpy.distutils.misc_util import Configuration
    from numpy.distutils.misc_util import get_info

    info = get_info('npymath')

    config = Configuration('',
                           parent_package,
                           top_path)

    config.add_extension('cNeuralReluplex',
                         sources=['reluplex/Reluplex2py.cpp'],
                         include_dirs=[common_inc, glpk_inc,
                                       reluplex_inc, src_inc],
                         library_dirs=[glpk_lib],
                         runtime_library_dirs=[glpk_lib],
                         libraries=["glpk"],
                         extra_compile_args=['--std=c++14'],
                         language='c++',
                         extra_info=info)

    return config


if __name__ == "__main__":
    from numpy.distutils.core import setup
    setup(configuration=configuration,
          name='cNeuralReluplex',
          description='cReluplex Python module',
          version='1.0')

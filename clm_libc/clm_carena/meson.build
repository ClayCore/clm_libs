# --------------------------------------------------------------------------- #
# Project declaration ------------------------------------------------------- #
# --------------------------------------------------------------------------- #

project(
    'libclm_carena',
    ['c'],
    version: '0.1.0-dev',
    license: 'BSD-3-Clause',
    default_options: [
        'c_std=c17',
        'warning_level=3',
    ]
)

# --------------------------------------------------------------------------- #
# Variables ----------------------------------------------------------------- #
# --------------------------------------------------------------------------- #

cc = meson.get_compiler('c')
host = host_machine.system()

include_dirs = [
    include_directories('src'),
    include_directories('include')
]

sources = [
    'src/carena.c'
]

# --------------------------------------------------------------------------- #
# Dependencies -------------------------------------------------------------- #
# --------------------------------------------------------------------------- #

deps = []

# Shared library for all C projects
shared_lib_inc = ['../clm_cshared']
shared_lib = declare_dependency(include_directories: shared_lib_inc)

deps += shared_lib

# --------------------------------------------------------------------------- #
# Library build ------------------------------------------------------------- #
# --------------------------------------------------------------------------- #

library(
    'libclm_carena',
    sources,
    include_directories: include_dirs,
    dependencies: deps,
    c_pch: 'include/libc.h'
)

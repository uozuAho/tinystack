import os
import _globals
import _utilities
from doit.tools import create_folder
from doit_helpers import file_utils
from doit_helpers import gcc_utils

#-----------------------------------------------------------
# Constants

#------------------------------------
# Paths

# Name of this build configuration
NAME = 'unit_tests'

DESCRIPTION = "Build & run unit tests"

# Directory under which all built files/directories will be placed
BUILD_DIR = _globals.from_build_root(NAME)

# Directory to place all c object files
OBJ_DIR = os.path.join(BUILD_DIR, 'obj')


#------------------------------------
# Unit test runner code generator

# Path of the test runner generator script
UNIT_TESTS_RUNNER_GENERATOR = _globals.from_proj_root(
    'test', 'unit_test_harness', 'Unity', 'scripts', 'makeTestRunner.py')

# Root directory of the unit tests
UNIT_TESTS_LOCATION = _globals.from_proj_root(
    'test', 'unit_tests')

# Path of the generated test runner source
UNIT_TEST_RUNNER_SOURCE = _globals.from_build_root(
    'generated_src', '_all_tests.c')


#------------------------------------
# Compiler & linker

COMPILER = 'gcc'
COMPILER_DEFINITIONS = []
COMPILER_FLAGS = [
    '-O0',      # no optimisation
    '-g3',      # full debugging info
    '-Wall',    # all warnings
    '-MMD'      # generate dependency files
]

COMPILER_INCLUDE_DIRS = [
    _globals.from_proj_root('include'),
    _globals.from_proj_root('test', 'unit_test_harness', 'Unity')
]

LINKER = 'gcc'

SOURCE_DIRS = [
    _globals.from_proj_root('source'),
    _globals.from_proj_root('test', 'unit_test_harness', 'Unity'),
    _globals.from_proj_root('test', 'unit_tests')
]

SOURCES = []
for sdir in SOURCE_DIRS:
    SOURCES += file_utils.find(sdir, '*.c')

# Add generated source manually
SOURCES += [UNIT_TEST_RUNNER_SOURCE]

OBJECTS = [_utilities.source_to_obj(source, OBJ_DIR) for source in SOURCES]

DEPS = gcc_utils.get_dependency_dict(OBJ_DIR)

EXE_TARGET_NAME = _globals.get_exe_target_name(NAME, 'exe')

EXE_TARGET = os.path.join(BUILD_DIR, EXE_TARGET_NAME)


#-----------------------------------------------------------
# Doit task generators


def get_all_tasks():
    tasks = get_code_gen_tasks() + get_compile_tasks() + get_link_tasks()
    tasks += get_run_test_tasks()
    return tasks


def get_code_gen_tasks():
    "Generate test runner"

    generate_runner_args = ['python',
                            UNIT_TESTS_RUNNER_GENERATOR,
                            UNIT_TESTS_LOCATION,
                            '-o',
                            UNIT_TEST_RUNNER_SOURCE]

    generate_runner_cmd = ' '.join(generate_runner_args)

    return [{
        'name': 'generate test runners',
        'actions': [(create_folder, [os.path.dirname(UNIT_TEST_RUNNER_SOURCE)]),
                    generate_runner_cmd],
        'targets': [UNIT_TEST_RUNNER_SOURCE],
        'clean': True
    }]


def get_compile_tasks():
    tasks = []

    for source in SOURCES:
        obj = _utilities.source_to_obj(source, OBJ_DIR)
        dep = _utilities.source_to_dep(source, OBJ_DIR)
        if obj in DEPS:
            dependencies = DEPS[obj]
        else:
            dependencies = [source]
        tasks.append({
            'name': source.replace('.c', '.o'),
            'actions': [(create_folder, [os.path.dirname(obj)]),
                        gcc_utils.get_compile_cmd_str(source, obj,
                                                      compiler=COMPILER,
                                                      defs=COMPILER_DEFINITIONS,
                                                      includes=COMPILER_INCLUDE_DIRS,
                                                      flags=COMPILER_FLAGS)],
            'targets': [obj, dep],
            'file_dep': dependencies,
            'clean': True
        })
    return tasks


def get_link_tasks():
    return [{
        'name': EXE_TARGET_NAME,
        'actions': [gcc_utils.get_link_cmd_str(EXE_TARGET, OBJECTS,
                                               linker=LINKER)],
        'file_dep': OBJECTS,
        'targets': [EXE_TARGET],
        'clean': True
    }]


def get_run_test_tasks():
    return [{
        'name': 'run tests',
        'actions': [EXE_TARGET],
        'file_dep': [EXE_TARGET],
        # dummy target means this task always runs
        'targets': ['unit_tests'],
        'verbosity': 2
    }]

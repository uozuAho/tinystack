import os
import _globals
import _utilities

#-----------------------------------------------------------
# Constants

#------------------------------------
# Paths

# Name of this build configuration
NAME = 'unit_tests'

DESCRIPTION = "Build & run unit tests"

# Directory under which all built files/directories will be placed
BUILD_DIR = _globals.from_build_root(NAME)

# Dummy file that signifies the existence of required build directories
BUILD_DIR_DUMMY = os.path.join(BUILD_DIR, '.dummy')

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
COMPILER_FLAGS = ['-O0', '-g3', '-Wall', '-MMD']
COMPILER_INCLUDE_DIRS = [
    _globals.from_proj_root('source', 'include'),
    _globals.from_proj_root('test', 'unit_test_harness', 'Unity')
]

LINKER = 'gcc'

SOURCE_DIRS = [
    _globals.from_proj_root('source'),
    _globals.from_proj_root('test', 'unit_test_harness', 'Unity'),
    _globals.from_proj_root('test', 'unit_tests')
]

SOURCES = _utilities.find_files(SOURCE_DIRS, extensions=['.c'])

# Add generated source manually
SOURCES += [UNIT_TEST_RUNNER_SOURCE]

OBJECTS = [_utilities.source_to_obj(source, OBJ_DIR) for source in SOURCES]

EXE_TARGET_NAME = _globals.get_exe_target_name(NAME, 'exe')

EXE_TARGET = os.path.join(BUILD_DIR, EXE_TARGET_NAME)


#-----------------------------------------------------------
# Utility functions


def arg_list_to_command_string(arg_list):
    return ''.join([arg+' ' for arg in arg_list])


def get_compile_command(source_path):
    """ Return a compiler command string """
    cmd_args = [COMPILER]
    cmd_args += ['-D'+d for d in COMPILER_DEFINITIONS]
    cmd_args += ['-I'+i for i in COMPILER_INCLUDE_DIRS]
    cmd_args += COMPILER_FLAGS + ['-c']
    cmd_args += ['-o', _utilities.source_to_obj(source_path, OBJ_DIR)]
    cmd_args += [source_path]
    return arg_list_to_command_string(cmd_args)


def get_link_command():
    cmd_args = [LINKER]
    cmd_args += OBJECTS
    cmd_args += ['-o', EXE_TARGET]
    return arg_list_to_command_string(cmd_args)


def create_build_dirs():
    _utilities.create_dirs(OBJECTS)
    # Create the dummy file
    with open(BUILD_DIR_DUMMY, 'w') as ofile:
        ofile.write('')


#-----------------------------------------------------------
# Doit task generators


def get_build_dir_task():
    return {
        'name': 'create build dirs',
        'actions': [create_build_dirs],
        'targets': [BUILD_DIR_DUMMY]
    }


def get_code_gen_tasks():
    "Generate test runner"

    generate_runner_args = ['python',
                            UNIT_TESTS_RUNNER_GENERATOR,
                            UNIT_TESTS_LOCATION,
                            '-o',
                            UNIT_TEST_RUNNER_SOURCE]

    generate_runner_cmd = arg_list_to_command_string(generate_runner_args)

    return [{
        'name': 'generate test runners',
        'actions': [(_utilities.create_dirs, [UNIT_TEST_RUNNER_SOURCE]),
                    generate_runner_cmd],
        'targets': [UNIT_TEST_RUNNER_SOURCE],
        'clean': True
    }]


def get_compile_tasks():
    tasks = [get_build_dir_task()]

    for source in SOURCES:
        obj = _utilities.source_to_obj(source, OBJ_DIR)
        dep = _utilities.source_to_dep(source, OBJ_DIR)
        dependencies = [BUILD_DIR_DUMMY]
        depfile_deps = _utilities.get_obj_dependencies(obj)
        if depfile_deps is None:
            dependencies += [source]
        else:
            dependencies += depfile_deps
        tasks.append({
            'name': source.replace('.c', '.o'),
            'actions': [get_compile_command(source)],
            'targets': [obj, dep],
            'file_dep': dependencies,
            'clean': True
        })
    return tasks


def get_link_tasks():
    return [{
        'name': EXE_TARGET_NAME,
        'actions': [get_link_command()],
        'file_dep': OBJECTS,
        'targets': [EXE_TARGET],
        'clean': True
    }]


def get_run_test_tasks():
    return [{
        'name': 'run tests',
        'actions': [EXE_TARGET],
        'file_dep': [EXE_TARGET],
    }]

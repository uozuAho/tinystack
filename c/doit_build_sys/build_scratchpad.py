import os
import _globals
from doit.tools import create_folder
from doit_helpers import file_utils
from doit_helpers import gcc_utils

#-----------------------------------------------------------
# Constants

#------------------------------------
# Paths

# Name of this build configuration
NAME = 'scratchpad'

DESCRIPTION = "Run whatever's in the test scratchpad"

# Directory under which all built files/directories will be placed
BUILD_DIR = _globals.from_build_root(NAME)

# Directory to place all c object files
OBJ_DIR = os.path.join(BUILD_DIR, 'obj')


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
]

LINKER = 'gcc'

SOURCE_DIRS = [
    _globals.from_proj_root('source'),
    _globals.from_proj_root('test', 'scratchpad')
]

SOURCES = []
for sdir in SOURCE_DIRS:
    SOURCES += file_utils.find(sdir, '*.c')

OBJECTS = [_globals.source_to_obj_path(source, OBJ_DIR) for source in SOURCES]

DEPS = gcc_utils.get_dependency_dict(OBJ_DIR)

EXE_TARGET_NAME = _globals.get_exe_target_name(NAME, 'exe')

EXE_TARGET = os.path.join(BUILD_DIR, EXE_TARGET_NAME)


#-----------------------------------------------------------
# Doit task generators


def get_all_tasks():
    return get_compile_tasks() + get_link_tasks() + get_run_tasks()


def get_compile_tasks():
    tasks = []

    for source in SOURCES:
        obj = _globals.source_to_obj_path(source, OBJ_DIR)
        dep = _globals.source_to_dep_path(source, OBJ_DIR)
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


def get_run_tasks():
    return [{
        'name': 'run',
        'actions': [EXE_TARGET],
        'file_dep': [EXE_TARGET],
        # dummy target means this task always runs
        'targets': ['scratchpad'],
        'verbosity': 2
    }]

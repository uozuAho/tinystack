import os
import _globals
import _utilities

#-----------------------------------------------------------
# Constants

#------------------------------------
# Paths

# Name of this build configuration
NAME = 'scratchpad'

DESCRIPTION = "Run whatever's in the test scratchpad"

# Directory under which all built files/directories will be placed
BUILD_DIR = _globals.from_build_root(NAME)

# Dummy file that signifies the existence of required build directories
BUILD_DIR_DUMMY = os.path.join(BUILD_DIR, '.dummy')

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

SOURCES = _utilities.find_files(SOURCE_DIRS, extensions=['.c'])

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


def get_run_tasks():
    return [{
        'name': 'run',
        'actions': [EXE_TARGET],
        'file_dep': [EXE_TARGET],
        # dummy target means this task always runs
        'targets': ['scratchpad'],
        'verbosity': 2
    }]

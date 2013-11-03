import os

#-----------------------------------------------------------
# Constants

PROJECT_NAME = 'tinystack'

VERSION_STRING = '0.0.1'

# Root directory of the c project
# NOTE: This assumes the build script is run from the project root
PROJECT_ROOT = ''

# All builds go under this directory
BUILD_ROOT = os.path.join(PROJECT_ROOT, 'build')


#-----------------------------------------------------------
# Functions


def from_proj_root(*args):
    """ Returns os.path.join(PROJECT_ROOT, *args) """
    return os.path.join(PROJECT_ROOT, *args)


def from_build_root(*args):
    """ Returns os.path.join(BUILD_ROOT, *args) """
    return os.path.join(BUILD_ROOT, *args)


def get_exe_target_name(name, extension):
    return PROJECT_NAME+'-'+VERSION_STRING+'-'+name+'.'+extension


def source_to_obj_path(source_path, dest_dir):
    """ Return the path of the object file to be built from the
        given source file
    """
    source_no_ext = os.path.splitext(source_path)[0]
    return os.path.join(dest_dir, source_no_ext + '.o')


def source_to_dep_path(source_path, dest_dir):
    """ Return the path of the dependency file that will be generated
        when compiling the source_path
    """
    source_no_ext = os.path.splitext(source_path)[0]
    return os.path.join(dest_dir, source_no_ext + '.d')

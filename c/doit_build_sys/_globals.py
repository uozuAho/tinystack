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

# This dummy file is used to detect the presence of build_root
BUILD_ROOT_DUMMY_FILE = os.path.join(BUILD_ROOT, '.dummy')


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

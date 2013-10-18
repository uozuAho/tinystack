C_SOURCE = 'cobs_codec.c'
C_INCLUDE_DIR = "../../c/include"
TARGET_EXE = 'cobs_codec.exe'


def task_build_c_codec():
    return {
        'actions': ['gcc -I'+C_INCLUDE_DIR+' -O0 -g3 -Wall -o '+TARGET_EXE+' '+C_SOURCE],
        'file_dep': [C_SOURCE],
        'targets': [TARGET_EXE],
        'clean': True
    }

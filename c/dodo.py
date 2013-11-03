DOIT_CONFIG = {'default_tasks': ['unit_tests']}


def task_unit_tests():
    "Build and run unit tests"
    from doit_build_sys import build_unit_tests

    for task in build_unit_tests.get_all_tasks():
        yield task


def task_scratchpad():
    "Build & run whatever's in the test scratchpad"
    from doit_build_sys import build_scratchpad

    for task in build_scratchpad.get_all_tasks():
        yield task

from doit_build_sys import build_unit_tests
from doit_build_sys import build_scratchpad


DOIT_CONFIG = {'default_tasks': ['unit_tests']}


def task_unit_tests():
    "Build and run unit tests"
    tasks = build_unit_tests.get_code_gen_tasks()
    tasks += build_unit_tests.get_compile_tasks()
    tasks += build_unit_tests.get_link_tasks()
    tasks += build_unit_tests.get_run_test_tasks()

    for task in tasks:
        yield task


def task_scratchpad():
    "Build & run whatever's in the test scratchpad"
    tasks = build_scratchpad.get_compile_tasks()
    tasks += build_scratchpad.get_link_tasks()
    tasks += build_scratchpad.get_run_tasks()

    for task in tasks:
        yield task

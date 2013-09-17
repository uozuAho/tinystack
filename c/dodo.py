from doit_build_sys import build_unit_tests


def task_unit_tests():
    "Build and run unit tests"
    tasks = build_unit_tests.get_compile_tasks()
    tasks += [build_unit_tests.get_link_task()]

    for task in tasks:
        yield task

from doit_build_sys import _globals


def test_some_stuff():
    print _globals.TEST_RUNNER_GENERATOR


def task_thing():
    return {
        'actions': [test_some_stuff],
        'targets': []
    }

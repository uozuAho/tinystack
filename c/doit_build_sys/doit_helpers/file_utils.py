import fnmatch
import os


def find(path, pattern, exclude_pattern=None, recursive=True):
    """ Return a list of files under the given path that match the
        pattern. Searches all subdirectories by default.
    """
    matches = []
    if recursive:
        for root, dirnames, filenames in os.walk(path):
            for filename in filenames:
                if fnmatch.fnmatch(filename, pattern):
                    matches.append(os.path.join(root, filename))
    else:
        for path in os.listdir(path):
            if os.path.isfile(path) and fnmatch.fnmatch(path, pattern):
                matches.append(os.path.join(path))

    if exclude_pattern is None:
        return matches
    else:
        return [x for x in matches if not fnmatch.fnmatch(x, exclude_pattern)]

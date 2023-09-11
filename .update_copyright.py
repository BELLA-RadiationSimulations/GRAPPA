# Copyright 2021-2023
#
# Authors:
# Stanimir Kisyov, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

import argparse
from datetime import date
from pathlib import Path
from numpy import any

_FILE_CHANGED_CODE = -1
authors = ["Stanimir Kisyov", "Davide Terzani"]

copyright_initial_year: int = 2021
copyright_end_year = date.today().year

creator = "Davide Terzani"

comment = {
    "cpp": "//",
    "py": "#",
    "mac": "#",
}


def generate_copyright(language="cpp"):
    c = comment[language] + " "
    cnospace = comment[language]
    copy_str = list()
    copy_str.append(
        c + "Copyright {}-{}".format(copyright_initial_year, copyright_end_year) + "\n"
    )
    copy_str.append(cnospace + "\n")
    copy_str.append(c + "Authors:" + "\n")

    # Collect authors in groups of 3
    auth_strings = list()
    auth_string = ""
    for i, auth in enumerate(authors):
        if ((i + 1) % 3) == 0 or i == (len(authors) - 1):
            auth_string += auth
            auth_strings.append(auth_string)
            auth_string = ""
        else:
            auth_string += auth + ", "

    for string in auth_strings:
        copy_str.append(c + string + "\n")

    copy_str.append(cnospace + "\n")
    copy_str.append(c + "Creator: {}".format(creator) + "\n")
    copy_str.append(cnospace + "\n")
    copy_str.append(c + "This file is part of GRAPPA." + "\n")
    copy_str.append(cnospace + "\n")
    copy_str.append(c + "License: BSD-3-Clause" + "\n")

    return copy_str


def find_written_copyright(file: Path, language="cpp"):
    c = comment[language]
    with open(file, "r") as fp:
        lines = fp.readlines()

    previous_copyright = list()
    # Stripping all the initial newlines
    nonewlinesindex = lines.index(next(elem for elem in lines if elem != "\n"))
    lines = lines[nonewlinesindex:]
    for i, line in enumerate(lines):
        if len(line) < len(c) or line[: len(c)] != c:
            break
        previous_copyright.append(line)

    newlines = lines[i:]
    # Stripping all the newlines between header copyright and file
    newindex = newlines.index(next(elem for elem in newlines if elem != "\n"))
    newlines = newlines[newindex:]

    return (previous_copyright, newlines)


def populate_files(source_dir="."):
    exclude_dirs = ["bin", ".vscode", "build", ".git"]
    cpp_ext = [".cpp", ".hpp", ".C.in", ".h.in"]
    py_ext = [".py"]
    mac_ext = [".mac"]
    file_lists = dict()
    file_lists["cpp"] = list()
    file_lists["py"] = list()
    file_lists["mac"] = list()
    basepath = Path(source_dir)
    exclude_paths = [basepath.joinpath(directory) for directory in exclude_dirs]
    for path in basepath.rglob("*"):
        is_excluded = any([pp in path.parents for pp in exclude_paths])
        if not is_excluded and path.is_file():
            path_ext = "".join(path.suffixes)
            if path_ext in cpp_ext:
                file_lists["cpp"].append(path)
            if path_ext in py_ext:
                file_lists["py"].append(path)
            if path_ext in mac_ext:
                file_lists["mac"].append(path)

    return file_lists


def update_copyright(file: Path, language="cpp", only_diff=False):
    previous, nocopyright = find_written_copyright(file, language)
    newcopyright = generate_copyright(language)

    updated = False
    if previous != newcopyright:
        updated = True
        if not only_diff:
            with open(file, "w") as fp:
                fp.writelines(newcopyright)
                fp.writelines("\n")
                fp.writelines(nocopyright)
    return updated


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog=".update_copyright.py",
        description="Update the copyright notice throughout the codebase.",
    )
    parser.add_argument(
        "-d",
        "--diff",
        action="store_true",
        help="If specified, only returns 0 if no file would be modified, otherwise a -1.",
        required=False,
        dest="diff",
    )
    args = parser.parse_args()
    only_diff = args.diff
    file_list = populate_files()
    Nfileschanged = 0

    for language, files in file_list.items():
        for ff in files:
            if update_copyright(ff, language, only_diff):
                Nfileschanged += 1

    if Nfileschanged > 0 and only_diff:
        print(
            "     Program run with the --diff option. {} files would be changed.".format(
                Nfileschanged
            )
        )
        exit(_FILE_CHANGED_CODE)
    if Nfileschanged > 0 and not only_diff:
        print("     {} files changed.".format(Nfileschanged))
        exit(_FILE_CHANGED_CODE)
    if Nfileschanged == 0:
        print("     All the files were already up-to-date.")
        exit(0)

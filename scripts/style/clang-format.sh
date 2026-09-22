#!/usr/bin/env bash

set -euo pipefail

required_version=18

if ! command -v clang-format >/dev/null 2>&1; then
    echo "Error: clang-format was not found in PATH." >&2
    exit 1
fi

version_output="$(clang-format --version)"

if [[ "${version_output}" =~ ([0-9]+)\. ]]; then
    major_version="${BASH_REMATCH[1]}"
else
    echo "Error: unable to determine clang-format version from:" >&2
    echo "  ${version_output}" >&2
    exit 1
fi

if [[ "${major_version}" -ne "${required_version}" ]]; then
    echo "Error: clang-format ${required_version} is required; found ${major_version}." >&2
    exit 1
fi

find src scripts -type f \
    \( -name '*.h' -o \
       -name '*.hpp' -o \
       -name '*.c' -o \
       -name '*.cc' -o \
       -name '*.cpp' -o \
       -name '*.C.in' -o \
       -name '*.h.in' \) \
    -print0 |
    xargs -0 --no-run-if-empty \
        clang-format \
        --style=file \
        -i

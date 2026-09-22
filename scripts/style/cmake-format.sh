#!/usr/bin/env bash

set -euo pipefail

if ! command -v cmake-format >/dev/null 2>&1; then
    echo "Error: cmake-format was not found in PATH." >&2
    echo "Install it with: python -m pip install cmakelang" >&2
    exit 1
fi

find . \
    \( -type d \( \
        -name build -o \
        -name dependency_install -o \
        -name .git \
    \) -prune \) \
    -o \
    \( -type f \( \
        -name 'CMakeLists.txt' -o \
        -name '*.cmake' \
    \) -print0 \) |
    xargs -0 --no-run-if-empty \
        cmake-format \
        -i

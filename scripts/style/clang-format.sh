#!/bin/bash

find src/ include/ scripts/ \
    -type f -name "*.hpp" -o \
    -type f -name "*.cpp" -o \
    -type f -name "*.C.in" -o \
    -type f -name "*.h.in" \
    | xargs clang-format --style=file -i

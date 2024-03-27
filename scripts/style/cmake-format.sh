#!/bin/bash

# Since it relies on a configuration file,
# the package pyyaml needs to be installed

find . -name build -prune -o -type f \
    -name "*CMakeLists.txt" -print \
    | xargs -n1 cmake-format -i

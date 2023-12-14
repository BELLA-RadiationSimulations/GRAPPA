#!/bin/bash

# This script requires cmake-format to be installed in python

find . -name build -prune -o -type f \
    -name "*CMakeLists.txt" -print \
    | xargs -n1 cmake-format --tab-size 4 \
    --enable-markup False -i

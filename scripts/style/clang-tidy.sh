#!/bin/bash

cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$HOME/anaconda3/envs/geant4/lib/" \
  -DCMAKE_CXX_CLANG_TIDY="clang-tidy;--system-headers=0;--config-file=$PWD/.clang-tidy"

cmake --build build -j

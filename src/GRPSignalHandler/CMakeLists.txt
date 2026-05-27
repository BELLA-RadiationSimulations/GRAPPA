# Copyright 2021-2026
#
# Authors:
# Stanimir Kisyov, Sarah Schröder, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause-LBNL

add_library(GRPSignalHandler STATIC)
target_sources(
    GRPSignalHandler PRIVATE # cmake-format: sortable
                             GRPSignalHandler.cpp
)

target_include_directories(GRPSignalHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPSignalHandler PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPSignalHandler PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPSignalHandler PRIVATE GRPMessages)

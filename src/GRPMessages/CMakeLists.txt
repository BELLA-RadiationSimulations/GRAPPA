# Copyright 2021-2026
#
# Authors:
# Stanimir Kisyov, Sarah Schröder, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

add_library(GRPMessages STATIC)
target_sources(
    GRPMessages PRIVATE # cmake-format: sortable
                        GRPMessages.cpp
)

target_include_directories(GRPMessages PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPMessages PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPMessages PRIVATE ${Geant4_LIBRARIES})

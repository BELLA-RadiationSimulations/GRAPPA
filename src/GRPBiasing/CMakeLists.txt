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

add_library(GRPBiasing STATIC)
target_sources(
    GRPBiasing
    PRIVATE # cmake-format: sortable
            GRPMuonPairSplittingOperation.cpp
            GRPPionDecaySplittingOperation.cpp
            GRPSplittingOperator.cpp
            GRPSplittingOperatorMessenger.cpp
)

target_include_directories(GRPBiasing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPBiasing PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPBiasing PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPBiasing PRIVATE GRPPhysicsList)

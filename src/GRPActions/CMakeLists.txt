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

add_library(GRPActions STATIC)

target_sources(
    GRPActions
    PRIVATE # cmake-format: sortable
            GRPActionInitialization.cpp
            GRPPrimaryGeneratorAction.cpp
            GRPRun.cpp
            GRPRunAction.cpp
            GRPTrackingAction.cpp
)

target_include_directories(GRPActions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPActions PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPActions PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPActions PRIVATE GRPHistandNTupleManager)
target_link_libraries(GRPActions PRIVATE GRPMessages)
target_link_libraries(GRPActions PRIVATE GRPParticleSource)

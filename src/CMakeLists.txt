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

target_sources(
    GRAPPA
    PRIVATE # cmake-format: sortable
            grappa.cpp
            GRPAbsorberHit.cpp
            GRPActionInitialization.cpp
            GRPDetectorConstruction.cpp
            GRPHistandNTupleManager.cpp
            GRPPrimaryGeneratorAction.cpp
            GRPRun.cpp
            GRPRunAction.cpp
            GRPSDAbsorber.cpp
            GRPTrackingAction.cpp
)

add_subdirectory(GRPBiasing)
target_link_libraries(GRAPPA PRIVATE GRPBiasing)

add_subdirectory(GRPMessages)
target_link_libraries(GRAPPA PRIVATE GRPMessages)

add_subdirectory(GRPParticleSource)
target_link_libraries(GRAPPA PRIVATE GRPParticleSource)

add_subdirectory(GRPSignalHandler)
target_link_libraries(GRAPPA PRIVATE GRPSignalHandler)

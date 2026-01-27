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
    GRAPPA PRIVATE # cmake-format: sortable
                   grappa.cpp
)

target_include_directories(GRAPPA PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(GRPAbsorber)
target_link_libraries(GRAPPA PRIVATE GRPAbsorber)

add_subdirectory(GRPActions)
target_link_libraries(GRAPPA PRIVATE GRPActions)

add_subdirectory(GRPBiasing)
target_link_libraries(GRAPPA PRIVATE GRPBiasing)

add_subdirectory(GRPDetectorConstruction)
target_link_libraries(GRAPPA PRIVATE GRPDetectorConstruction)

add_subdirectory(GRPHistandNTupleManager)
target_link_libraries(GRAPPA PRIVATE GRPHistandNTupleManager)

add_subdirectory(GRPMessages)
target_link_libraries(GRAPPA PRIVATE GRPMessages)

add_subdirectory(GRPParticleSource)
target_link_libraries(GRAPPA PRIVATE GRPParticleSource)

add_subdirectory(GRPPhysicsList)
target_link_libraries(GRAPPA PRIVATE GRPPhysicsList)

add_subdirectory(GRPSignalHandler)
target_link_libraries(GRAPPA PRIVATE GRPSignalHandler)

add_subdirectory(GRPUtils)
target_link_libraries(GRAPPA PRIVATE GRPUtils)

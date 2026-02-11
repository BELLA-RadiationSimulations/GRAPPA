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

add_library(GRPDetectorConstruction STATIC)
target_sources(
    GRPDetectorConstruction PRIVATE # cmake-format: sortable
                                    GRPDetectorConstruction.cpp
)

target_include_directories(
    GRPDetectorConstruction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(
    GRPDetectorConstruction PRIVATE ${Geant4_INCLUDE_DIR}
)
target_link_libraries(GRPDetectorConstruction PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPDetectorConstruction PRIVATE GRPAbsorber)
target_link_libraries(GRPDetectorConstruction PRIVATE GRPBiasing)
target_link_libraries(GRPDetectorConstruction PRIVATE GRPHistandNTupleManager)
target_link_libraries(GRPDetectorConstruction PRIVATE GRPStoppingDetector)

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

add_library(GRPAbsorber STATIC)
target_sources(
    GRPAbsorber PRIVATE # cmake-format: sortable
                        GRPAbsorberHit.cpp GRPSDAbsorber.cpp
)

target_include_directories(GRPAbsorber PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPAbsorber PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPAbsorber PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPAbsorber PRIVATE GRPUtils)
target_link_libraries(GRPAbsorber PRIVATE GRPHistandNTupleManager)

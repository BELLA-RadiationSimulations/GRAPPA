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

add_library(GRPParticleSource STATIC)
target_sources(
    GRPParticleSource
    PRIVATE # cmake-format: sortable
            GRPASCIIReader.cpp GRPParticleContainer.cpp GRPParticleSource.cpp
)

target_include_directories(GRPParticleSource PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPParticleSource PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPParticleSource PRIVATE ${Geant4_LIBRARIES})

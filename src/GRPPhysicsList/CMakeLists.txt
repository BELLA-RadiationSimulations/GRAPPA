# Copyright 2021-2025
#
# Authors:
# Stanimir Kisyov, Sarah Schröder, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

add_library(GRPPhysicsList STATIC)
target_sources(
    GRPPhysicsList
    PRIVATE # cmake-format: sortable
            GRPEmPhysics.cpp
            GRPPhysicsList.cpp
            GRPPhysicsListMessenger.cpp
)

target_include_directories(GRPPhysicsList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPPhysicsList PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPPhysicsList PRIVATE ${Geant4_LIBRARIES})

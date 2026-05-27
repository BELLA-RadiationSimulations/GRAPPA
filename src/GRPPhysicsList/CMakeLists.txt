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

add_library(GRPPhysicsList STATIC)
target_sources(
    GRPPhysicsList
    PRIVATE # cmake-format: sortable
            GRPEmExtraPhysics.cpp
            GRPEmPhysics.cpp
            GRPPhysicsList.cpp
            GRPPhysicsListMessenger.cpp
)

# Checking if the Geant4 version is greater than 11.3.2 to roll back the
# Bertini-like intra-nuclear cascade model (BERT) as it was in Geant4 version
# 11.2. This is an option introduced in Geant4 as it looks like the new model
# may not perform as well as the previous one.
option(GRAPPA_ROLLBACK_BERT
       "Roll back Bertini cascade model to previous version." ON
)
mark_as_advanced(GRAPPA_ROLLBACK_BERT)
if(Geant4_VERSION VERSION_GREATER_EQUAL 11.3.2 AND GRAPPA_ROLLBACK_BERT)
    message(
        STATUS
            "Rollback requested for the Bertini cascade. The code will use the model as in Geant4 11.2."
    )
    target_compile_definitions(
        GRPPhysicsList PRIVATE GRAPPA_ENABLED_BERTINI_ROLLBACK
    )
endif()

target_include_directories(GRPPhysicsList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GRPPhysicsList PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPPhysicsList PRIVATE ${Geant4_LIBRARIES})

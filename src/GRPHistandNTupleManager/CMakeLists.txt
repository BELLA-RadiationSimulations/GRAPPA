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

add_library(GRPHistandNTupleManager STATIC)
target_sources(
    GRPHistandNTupleManager PRIVATE # cmake-format: sortable
                                    GRPHistandNTupleManager.cpp
)

target_include_directories(
    GRPHistandNTupleManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(
    GRPHistandNTupleManager PRIVATE ${Geant4_INCLUDE_DIR}
)
target_link_libraries(GRPHistandNTupleManager PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPHistandNTupleManager PRIVATE GRPUtils)

# Checking if the Geant4 version is less than 11 to enable backward
# compatibility with the analysis manager of version 10
if(Geant4_VERSION VERSION_LESS 11)
    message(
        STATUS
            "The major version of Geant 4 is less than 11. Backward compatibility with the analysis manager is enabled."
    )
    target_compile_definitions(
        GRPHistandNTupleManager PUBLIC GRAPPA_USE_BACKWARDS_ANALYSIS
    )
endif()

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

add_library(GRPStoppingDetector STATIC)
target_sources(
    GRPStoppingDetector
    PRIVATE # cmake-format: sortable
            GRPStoppingDetector.cpp GRPStoppingDetectorMessenger.cpp
            GRPStoppingHit.cpp
)

target_include_directories(
    GRPStoppingDetector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(GRPStoppingDetector PRIVATE ${Geant4_INCLUDE_DIR})
target_link_libraries(GRPStoppingDetector PRIVATE ${Geant4_LIBRARIES})
target_link_libraries(GRPStoppingDetector PRIVATE GRPUtils)
target_link_libraries(GRPStoppingDetector PRIVATE GRPHistandNTupleManager)

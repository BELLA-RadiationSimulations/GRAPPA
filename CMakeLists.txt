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

# Setting CMAKE options
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
    GRAPPA
    VERSION 0.5.0
    DESCRIPTION "Geant4 RApid Pair Production Application"
    HOMEPAGE_URL "https://github.com/BELLA-RadiationSimulations/GRAPPA"
    LANGUAGES CXX
)

# Defining the target
add_executable(GRAPPA)

# ~~~
# Instructions needed to link with Geant 4 if not installed in a standard
# location This is cmake default behavior
# ~~~
set(CMAKE_SKIP_BUILD_RPATH FALSE)
# This is cmake default behavior
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# This is cmake default behavior
set(CMAKE_INSTALL_RPATH "")
# ~~~
# This is NOT cmake default behavior and it is needed with Geant4 in order to
# link to the libraries it uses
# ~~~
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(APPLE)
    set(MACOSX_RPATH TRUE)
endif(APPLE)

# =============#

# This standard is required for compatibility with GEANT4
set_property(TARGET GRAPPA PROPERTY CXX_STANDARD_REQUIRED 17)

# CMAKE_INSTALL_PREFIX is set here
set(CMAKE_INSTALL_PREFIX
    ${CMAKE_SOURCE_DIR}
    CACHE STRING "Installation prefix for GRAPPA"
)
message(STATUS "GRAPPA will be installed in ${CMAKE_INSTALL_PREFIX}")

# Setting build type if not specified
set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(
        STATUS
            "GRAPPA: Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified."
    )
    set(CMAKE_BUILD_TYPE
        "${DEFAULT_BUILD_TYPE}"
        CACHE STRING "Choose the type of build."
    )
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
                 "Debug"
                 "Release"
                 "RelWithDebInfo"
                 "MinSizeRel"
    )
endif()

message("   ==== Using ${CMAKE_CXX_COMPILER_ID} compiler ====")

# Find Geant4 (remember to source geant4.sh first)
# ----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable (As taken from G4 examples)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
mark_as_advanced(WITH_GEANT4_UIVIS)
if(WITH_GEANT4_UIVIS)
    list(
        APPEND
        G4_REQUIRED_COMPONENTS
        ui_all
        vis_all
    )
endif()

# Here we set that GRAPPA will prefer static over shared libraries, if both are
# found
option(GRAPPA_PREFER_STATIC_GEANT4 "Prefer Geant4 static over shared" ON)
mark_as_advanced(GRAPPA_PREFER_STATIC_GEANT4)
if(GRAPPA_PREFER_STATIC_GEANT4)
    list(APPEND G4_OPTIONAL_COMPONENTS static)
endif()

find_package(
    Geant4 REQUIRED ${G4_REQUIRED_COMPONENTS}
    OPTIONAL_COMPONENTS ${G4_OPTIONAL_COMPONENTS}
)

if(NOT ${Geant4_FOUND})
    message(FATAL_ERROR "Could not locate Geant4")
endif()

message(STATUS "Geant4 was found with version ${Geant4_VERSION}")

# Check if Geant4 is found with static or shared version

if(${Geant4_shared_FOUND})
    set(GRAPPA_FOUND_SHARED_GEANT4 TRUE)
    set(GRAPPA_USE_SHARED_GEANT4 TRUE)
endif()

if(${Geant4_static_FOUND})
    set(GRAPPA_FOUND_STATIC_GEANT4 TRUE)
    set(GRAPPA_USE_STATIC_GEANT4 TRUE)
endif()

if(GRAPPA_FOUND_SHARED_GEANT4 AND GRAPPA_FOUND_STATIC_GEANT4)
    if(GRAPPA_PREFER_STATIC_GEANT4)
        message(
            STATUS
                "Both static and shared version of Geant4 found. Static version is preferred."
        )
        set(GRAPPA_USE_SHARED_GEANT4 FALSE)
    else()
        message(
            STATUS
                "Both static and shared version of Geant4 found. Shared version is preferred."
        )
        set(GRAPPA_USE_STATIC_GEANT4 FALSE)
    endif()
endif()

# Setting compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(GRAPPA PRIVATE /Od /Zi)
    elseif(CMAKE_BUILD_TYPE MATCHES "Release")
        target_compile_options(
            GRAPPA
            PRIVATE /O2
                    /Ob2
                    /Oi
                    /Ot
                    /GT
                    /GL
                    /Qpar
        )
    endif()
endif()

# Add GRAPPA source files to target
add_subdirectory(src)

# Add Geant4 to target
include(${Geant4_USE_FILE})
target_include_directories(GRAPPA PRIVATE ${Geant4_INCLUDE_DIR})

target_link_libraries(GRAPPA PRIVATE ${Geant4_LIBRARIES})

get_filename_component(Geant4_LIBRARY_PATH "${Geant4_DIR}/../.." ABSOLUTE)

set_target_properties(GRAPPA PROPERTIES LINKER_LANGUAGE CXX)

# Add scripts
add_subdirectory(scripts)

if(APPLE AND GRAPPA_USE_SHARED_GEANT4)
    message(
        STATUS
            "Compiling on MacOS using Geant4 shared libraries requires adding the library path to the INSTALL_RPATH of the executable. Now adding the path ${Geant4_LIBRARY_PATH} to INSTALL_RPATH."
    )
    set_target_properties(
        GRAPPA PROPERTIES INSTALL_RPATH "${Geant4_LIBRARY_PATH}"
    )
endif()

# Install target
install(TARGETS GRAPPA DESTINATION bin)

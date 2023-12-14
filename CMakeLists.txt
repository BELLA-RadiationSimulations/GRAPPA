#Setting CMAKE options
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(GRAPPA VERSION 0.2.6)

# Defining the target
add_executable(GRAPPA)

# Instructions needed to link with Geant 4 if not installed in a standard location
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(APPLE)
    set(MACOSX_RPATH TRUE)
endif(APPLE)

#=============#

# This standard is required for compatibility with GEANT4
set_property(TARGET GRAPPA PROPERTY CXX_STANDARD_REQUIRED 17)

# CMAKE_INSTALL_PREFIX is set here
set(CMAKE_INSTALL_PREFIX
    ${CMAKE_SOURCE_DIR}
    CACHE STRING "Installation prefix for GRAPPA")
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
        CACHE STRING "Choose the type of build.")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                                 "RelWithDebInfo" "MinSizeRel")
endif()

message("   ==== Using ${CMAKE_CXX_COMPILER_ID} compiler ====")

# Find Geant4 (remember to source geant4.sh first)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
# (As taken from G4 examples)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()
if(NOT ${Geant4_FOUND})
    message(FATAL_ERROR "Could not locate Geant 4")
endif()

message(STATUS "Geant 4 was found with version ${Geant4_VERSION}")

# Checking if the Geant4 version was less than 11
# to enable backward compatibility with the analysis manager
# of version 10
if(Geant4_VERSION VERSION_LESS 11)
    message(
        STATUS
            "The major version of Geant 4 is less than 11. Backward compatibility with the analysis manager is enabled."
    )
    target_compile_definitions(GRAPPA PRIVATE GRAPPA_USE_BACKWARDS_ANALYSIS)
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
                    /Qpar)
    endif()
endif()

# Add GRAPPA source files to target
target_sources(
    GRAPPA
    PRIVATE src/GRPActionInitialization.cpp
            src/GRPDetectorConstruction.cpp
            src/GRPHistandNTupleManager.cpp
            src/GRPParticleSource.cpp
            src/GRPPrimaryGeneratorAction.cpp
            src/GRPRunAction.cpp
            src/GRPSDAbsorber.cpp
            src/GRPTrackingAction.cpp
            src/ReadASCII.cpp
            src/grappa.cpp)

# Add Geant4 to target
include(${Geant4_USE_FILE})
target_include_directories(GRAPPA PRIVATE ${Geant4_INCLUDE_DIR})
target_include_directories(GRAPPA PRIVATE ${PROJECT_SOURCE_DIR}/include)

set(UTILITIES_PATH ${CMAKE_INSTALL_PREFIX}/bin/utilities.h)
# Configure ROOT scripts and mark them to install in the install folder
set(ROOT_SCRIPTS_DIR ${PROJECT_SOURCE_DIR}/scripts/ROOT)
file(GLOB ROOT_SCRIPTS_BUILD ${PROJECT_SOURCE_DIR}/scripts/ROOT/*)
foreach(filename ${ROOT_SCRIPTS_BUILD})
    get_filename_component(filename_we ${filename} NAME_WLE)
    configure_file(${ROOT_SCRIPTS_DIR}/${filename_we}.in
                   ${CMAKE_BINARY_DIR}/${filename_we} @ONLY)
    list(APPEND ROOT_SCRIPTS ${CMAKE_BINARY_DIR}/${filename_we})
endforeach()

target_link_libraries(GRAPPA PRIVATE ${Geant4_LIBRARIES})
set_target_properties(GRAPPA PROPERTIES LINKER_LANGUAGE CXX)

# Install target
install(TARGETS GRAPPA DESTINATION bin)

# Install ROOT scripts
install(FILES ${ROOT_SCRIPTS} DESTINATION bin)

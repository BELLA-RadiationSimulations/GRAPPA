#Setting CMAKE options
CMAKE_MINIMUM_REQUIRED(VERSION 3.16 FATAL_ERROR)

PROJECT(GRAPPA VERSION 0.2.3)

# Defining the target
ADD_EXECUTABLE(GRAPPA)

# Instructions needed to link with Geant 4 if not installed in a standard location
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#=============#

# This standard is required for compatibility with GEANT4
set_property(TARGET GRAPPA PROPERTY CXX_STANDARD_REQUIRED 17)

# CMAKE_INSTALL_PREFIX is set here
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE STRING "Installation prefix for GRAPPA")
MESSAGE(STATUS "GRAPPA will be installed in ${CMAKE_INSTALL_PREFIX}")

# Setting build type if not specified
SET(DEFAULT_BUILD_TYPE "Release")
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    MESSAGE(STATUS "GRAPPA: Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    SET(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
        STRING "Choose the type of build.")
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
ENDIF()

MESSAGE("   ==== Using ${CMAKE_CXX_COMPILER_ID} compiler ====")

# Find Geant4 (remember to source geant4.sh first)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
# (As taken from G4 examples)

OPTION(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
IF(WITH_GEANT4_UIVIS)
    FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)
ELSE()
    FIND_PACKAGE(Geant4 REQUIRED)
ENDIF()
IF (NOT ${Geant4_FOUND})
    MESSAGE(FATAL_ERROR "Could not locate Geant 4")
ENDIF()

MESSAGE(STATUS "Geant 4 was found with version ${Geant4_VERSION}")

# Checking if the Geant4 version was less than 11
# to enable backward compatibility with the analysis manager
# of version 10
IF (Geant4_VERSION VERSION_LESS 11)
    MESSAGE(STATUS "The major version of Geant 4 is less than 11. Backward compatibility with the analysis manager is enabled.")
    target_compile_definitions(GRAPPA PRIVATE GRAPPA_USE_BACKWARDS_ANALYSIS)
ENDIF()

# Setting compiler options
IF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    IF (CMAKE_BUILD_TYPE MATCHES "Debug")
        TARGET_COMPILE_OPTIONS(GRAPPA PRIVATE /Od /Zi)
    ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
        TARGET_COMPILE_OPTIONS(GRAPPA PRIVATE /O2 /Ob2 /Oi /Ot /GT /GL /Qpar)
    ENDIF()
ENDIF()

# Add GRAPPA source files to target
TARGET_SOURCES(GRAPPA PRIVATE
    src/GRPActionInitialization.cpp
    src/GRPDetectorConstruction.cpp
    src/GRPHistandNTupleManager.cpp
    src/GRPParticleSource.cpp
    src/GRPPrimaryGeneratorAction.cpp
    src/GRPRunAction.cpp
    src/GRPSDAbsorber.cpp
    src/GRPTrackingAction.cpp
    src/ReadASCII.cpp
    src/grappa.cpp
)

# Add Geant4 to target
INCLUDE(${Geant4_USE_FILE})
TARGET_INCLUDE_DIRECTORIES(GRAPPA PRIVATE ${Geant4_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(GRAPPA PRIVATE ${PROJECT_SOURCE_DIR}/include)

SET(UTILITIES_PATH ${CMAKE_INSTALL_PREFIX}/bin/utilities.h)
# Configure ROOT scripts and mark them to install in the install folder
SET(ROOT_SCRIPTS_DIR ${PROJECT_SOURCE_DIR}/scripts/ROOT)
FILE(GLOB ROOT_SCRIPTS_BUILD ${PROJECT_SOURCE_DIR}/scripts/ROOT/*)
FOREACH(filename ${ROOT_SCRIPTS_BUILD})
    GET_FILENAME_COMPONENT(filename_we ${filename} NAME_WLE)
    CONFIGURE_FILE(${ROOT_SCRIPTS_DIR}/${filename_we}.in
                   ${CMAKE_BINARY_DIR}/${filename_we}
                   @ONLY)
    LIST(APPEND ROOT_SCRIPTS ${CMAKE_BINARY_DIR}/${filename_we})
ENDFOREACH()

TARGET_LINK_LIBRARIES(GRAPPA PRIVATE ${Geant4_LIBRARIES})
SET_TARGET_PROPERTIES(GRAPPA PROPERTIES LINKER_LANGUAGE CXX)

# Install target
INSTALL(TARGETS GRAPPA DESTINATION bin)

# Install ROOT scripts
INSTALL(FILES ${ROOT_SCRIPTS} DESTINATION bin)
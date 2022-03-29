#Setting CMAKE options
CMAKE_MINIMUM_REQUIRED(VERSION 3.16 FATAL_ERROR)

PROJECT(GRAPPA VERSION 0.1)

# Instructions needed to link with Geant 4 if not installed in a standard location
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#=============#

# This standard is required for compatibility with GEANT4
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting build type if not specified
SET(DEFAULT_BUILD_TYPE "Release")
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
MESSAGE(STATUS "GRAPPA: Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
SET(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
STRING "Choose the type of build." FORCE)
SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
"Debug" "Release")
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

# Defining the target
ADD_EXECUTABLE(GRAPPA src/grappa.cc)

IF(Geant4_hdf5_FOUND)
MESSAGE(AUTHOR_WARNING "Geant4 was compiled with HDF5 support but the 'NTUPLE' doens't work in GRAPPA if used with HDF5. Disabling HDF5.")
#MESSAGE(STATUS "Geant4 was compiled with HDF5 support: enabling HDF5 output.")
#target_compile_definitions(GRAPPA PUBLIC GEANT4_HAS_HDF5)
ENDIF()

# Setting compiler options
IF (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    ADD_COMPILE_OPTIONS(-Wall)
    IF (CMAKE_BUILD_TYPE MATCHES "Debug")
        ADD_COMPILE_OPTIONS(-O0 -g3 -lbacktrace -ggdb3)
    ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
        ADD_COMPILE_OPTIONS(-O3 -march=native)
    ENDIF()
ELSEIF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    IF (CMAKE_BUILD_TYPE MATCHES "Debug")
        ADD_COMPILE_OPTIONS(/Od /Zi)
    ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
        ADD_COMPILE_OPTIONS(/O2 /Ob2 /Oi /Ot /GT /GL /Qpar)
    ENDIF()
ENDIF()

# Adding files to sources and headers
FILE(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)
FILE(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add GRAPPA source files to target
TARGET_SOURCES(GRAPPA PRIVATE ${SOURCES})
TARGET_SOURCES(GRAPPA PRIVATE ${HEADERS})

# Add Geant4 to target
INCLUDE(${Geant4_USE_FILE})
TARGET_INCLUDE_DIRECTORIES(GRAPPA PRIVATE ${Geant4_INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES(GRAPPA PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Add ROOT scripts to install in the install folder
FILE(GLOB ROOT_SCRIPTS ${PROJECT_SOURCE_DIR}/scripts/ROOT/*)

TARGET_LINK_LIBRARIES(GRAPPA PRIVATE ${Geant4_LIBRARIES})

SET_TARGET_PROPERTIES(GRAPPA PROPERTIES LINKER_LANGUAGE CXX)

# CMAKE_INSTALL_PREFIX is forced here, maybe we can allow user to specify
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "GRAPPA will be installed in ${CMAKE_INSTALL_PREFIX}")

# Install target
INSTALL(TARGETS GRAPPA DESTINATION bin)
# Install ROOT scripts
INSTALL(FILES ${ROOT_SCRIPTS} DESTINATION bin)
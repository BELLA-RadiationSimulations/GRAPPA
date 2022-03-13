#Setting CMAKE options
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(GRAPPA VERSION 0.1)

# Instructions needed to link with Geant 4 and OpenPMD if not installed in a standard location
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#=============#

# This standard is required for compatibility with GEANT4
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting build type if not specified
set(DEFAULT_BUILD_TYPE "Release")
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "GRAPPA: Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release")
ENDIF()

MESSAGE("   ==== Using ${CMAKE_CXX_COMPILER_ID} compiler ====")

# Find Geant4 (remember to source geant4.sh first)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
# (As taken from G4 examples)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
IF(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
ELSE()
  find_package(Geant4 REQUIRED)
ENDIF()
IF (NOT ${Geant4_FOUND})
   MESSAGE(FATAL_ERROR "Could not locate Geant 4")
ENDIF()

IF(Geant4_hdf5_FOUND)
    MESSAGE(STATUS "Geant4 was compiled with HDF5 support: enabling HDF5 output.")
    target_compile_definitions(GRAPPA PUBLIC GEANT4_HAS_HDF5)
ENDIF()

# Setting compiler options
IF (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall)
    IF (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_compile_options(-O0 -g3 -lbacktrace -ggdb3)
    ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
        add_compile_options(-O3 -march=native)
    ENDIF()
ELSEIF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    IF (CMAKE_BUILD_TYPE MATCHES "Debug")
        add_compile_options(/Od /Zi)
    ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
        add_compile_options(/O2 /Ob2 /Oi /Ot /GT /GL /Qpar)
    ENDIF()
ENDIF()

# Adding files to sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(GRAPPA src/grappa.cc)

# Add GRAPPA source files to target
target_sources(GRAPPA PRIVATE ${sources})
target_sources(GRAPPA PRIVATE ${headers})

# Add Geant4 to target
include(${Geant4_USE_FILE})
target_include_directories(GRAPPA PRIVATE ${Geant4_INCLUDE_DIR})
target_include_directories(GRAPPA PRIVATE ${PROJECT_SOURCE_DIR}/include)


target_link_libraries(GRAPPA PRIVATE ${Geant4_LIBRARIES})

set_target_properties(GRAPPA PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS GRAPPA DESTINATION bin)
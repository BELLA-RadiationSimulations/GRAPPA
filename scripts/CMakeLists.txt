# Copyright 2021-2024
#
# Authors:
# Stanimir Kisyov, Davide Terzani
#
# Creator: Davide Terzani
#
# This file is part of GRAPPA.
#
# License: BSD-3-Clause

set(ROOT_ANALYSIS_INSTALL_DIR bin/analysis)
set(UTILITIES_PATH ${CMAKE_INSTALL_PREFIX}/${ROOT_ANALYSIS_INSTALL_DIR}/utilities.h)

# Configure ROOT scripts and mark them to install in the install folder
set(ROOT_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ROOT)
list(
    APPEND
    ROOT_SCRIPTS_BUILD
    ${ROOT_SCRIPTS_DIR}/DumpPhaseSpace.C.in
    ${ROOT_SCRIPTS_DIR}/MuonAnalysis.C.in
    ${ROOT_SCRIPTS_DIR}/PlotParticles.C.in
    ${ROOT_SCRIPTS_DIR}/utilities.h.in
    ${ROOT_SCRIPTS_DIR}/WriteSametimeParticles.C.in)

foreach(filename ${ROOT_SCRIPTS_BUILD})
    get_filename_component(filename_we ${filename} NAME_WLE)
    configure_file(${ROOT_SCRIPTS_DIR}/${filename_we}.in
                   ${CMAKE_BINARY_DIR}/${filename_we} @ONLY)
    list(APPEND ROOT_SCRIPTS ${CMAKE_BINARY_DIR}/${filename_we})
endforeach()

list(APPEND ROOT_SCRIPTS ${ROOT_SCRIPTS_DIR}/spectrum.py)
# Save run script for installation in bin folder
set(RUN_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/run)
list(
    APPEND
    RUN_SCRIPTS
    ${RUN_SCRIPTS_DIR}/custom_distribution.mac
    ${RUN_SCRIPTS_DIR}/distribution_from_file.mac
    ${RUN_SCRIPTS_DIR}/example_vis.mac
    ${RUN_SCRIPTS_DIR}/GPS_distribution.mac
    ${RUN_SCRIPTS_DIR}/initialize_vis.mac
    ${RUN_SCRIPTS_DIR}/InitialPhaseSpace.dat
    ${RUN_SCRIPTS_DIR}/photon_energy.dat)

# Install run scripts
install(FILES ${RUN_SCRIPTS} DESTINATION bin)
# Install ROOT scripts
install(FILES ${ROOT_SCRIPTS} DESTINATION ${ROOT_ANALYSIS_INSTALL_DIR})

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

set(ROOT_ANALYSIS_INSTALL_DIR bin/analysis)
set(UTILITIES_PATH
    ${CMAKE_INSTALL_PREFIX}/${ROOT_ANALYSIS_INSTALL_DIR}/utilities.h
)

# Configure ROOT scripts and mark them to install in the install folder
set(ROOT_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ROOT)
list(
    APPEND
    ROOT_SCRIPTS_BUILD
    # cmake-format: sortable
    ${ROOT_SCRIPTS_DIR}/DumpPhaseSpace.C.in
    ${ROOT_SCRIPTS_DIR}/MuonAnalysis.C.in
    ${ROOT_SCRIPTS_DIR}/PlotParticles.C.in
    ${ROOT_SCRIPTS_DIR}/utilities.h.in
    ${ROOT_SCRIPTS_DIR}/WriteSametimeParticles.C.in
)

foreach(filename ${ROOT_SCRIPTS_BUILD})
    get_filename_component(filename_we ${filename} NAME_WLE)
    configure_file(
        ${ROOT_SCRIPTS_DIR}/${filename_we}.in
        ${CMAKE_BINARY_DIR}/${filename_we} @ONLY
    )
    list(APPEND ROOT_SCRIPTS ${CMAKE_BINARY_DIR}/${filename_we})
endforeach()

list(
    APPEND
    ROOT_SCRIPTS
    # cmake-format: sortable
    ${ROOT_SCRIPTS_DIR}/spectrum.py
    ${ROOT_SCRIPTS_DIR}/mu_pair_decay_mpi.py
)
# Save run script for installation in bin folder
set(RUN_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/run)
list(
    APPEND
    RUN_SCRIPTS
    # cmake-format: sortable
    ${RUN_SCRIPTS_DIR}/control_biasing.mac
    ${RUN_SCRIPTS_DIR}/custom_distribution.mac
    ${RUN_SCRIPTS_DIR}/distribution_from_file.mac
    ${RUN_SCRIPTS_DIR}/draw_trajectories.mac
    ${RUN_SCRIPTS_DIR}/example_novis.mac
    ${RUN_SCRIPTS_DIR}/example_vis.mac
    ${RUN_SCRIPTS_DIR}/gaussian_10gev.mac
    ${RUN_SCRIPTS_DIR}/InitialPhaseSpace.dat
    ${RUN_SCRIPTS_DIR}/photon_energy.dat
    ${RUN_SCRIPTS_DIR}/set_geometry.mac
)

# Install GDML files if GDML is supported
if(${Geant4_gdml_FOUND})
    add_subdirectory(geometry)
endif()

# Install run scripts
install(FILES ${RUN_SCRIPTS} DESTINATION bin)
# Install ROOT scripts
install(FILES ${ROOT_SCRIPTS} DESTINATION ${ROOT_ANALYSIS_INSTALL_DIR})

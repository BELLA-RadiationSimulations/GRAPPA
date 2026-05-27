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

set(GDML_FILES_INSTALL_DIR bin/geometry)

# Configure ROOT scripts and mark them to install in the install folder
list(
    APPEND
    GDML_SCRIPTS
    # cmake-format: sortable
    ${CMAKE_CURRENT_SOURCE_DIR}/multiple_foils.gdml
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_target.gdml
)

# Install GDML scripts
install(FILES ${GDML_SCRIPTS} DESTINATION ${GDML_FILES_INSTALL_DIR})

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

target_sources(
    GRAPPA
    PRIVATE GRPActionInitialization.cpp
            GRPASCIIReader.cpp
            GRPDetectorConstruction.cpp
            GRPHistandNTupleManager.cpp
            GRPMessages.cpp
            GRPParticleContainer.cpp
            GRPParticleSource.cpp
            GRPPrimaryGeneratorAction.cpp
            GRPRun.cpp
            GRPRunAction.cpp
            GRPSDAbsorber.cpp
            GRPSignalHandler.cpp
            GRPTrackingAction.cpp
            grappa.cpp)

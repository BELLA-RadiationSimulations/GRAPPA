# Copyright 2021-2024
#
# Authors:
# Stanimir Kisyov, Sarah Schröder, Davide Terzani
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
            GRPSensitiveTargetDetectorConstruction.cpp
            GRPDetectorConstruction.cpp
            GRPHistandNTupleManager.cpp
            GRPMessages.cpp
            GRPParticleContainer.cpp
            GRPParticleSource.cpp
            GRPPrimaryGeneratorAction.cpp
            GRPRun.cpp
            GRPRunAction.cpp
            GRPWorldAbsorberSD.cpp
            GRPSignalHandler.cpp
            GRPTrackingAction.cpp
            grappa.cpp
)
